/*				Recorder1.2 for SDAC 3 Channel
				Header 0xBF 0x13 	0x97 	0x74
				Footer 0x40	0x40	0x40	0x40
				Packet size = 468 bytes
				Buadrate = 57600 bps
				SD Interface = RS232
				SDAC Interface = RS232
				filesize = 1 day = 6.06 KB
				Storage = 512 MB SD Card
				Status Led's
				Programmer: Sajjad Hussain S.E.
            Dated: 	04-10-2011
*/
#include "at89c51rc2.h"
#include "usbwiz_oem.h"
#include <intrins.h>
// Main Program Starts Here
void main()
{
 	led1 = 1;//0;
 	led2 = 1;//0;
 	led3 = 0;
 	//led4 = 1;//0;
   rxState = RX_Check_GHI;
   state = IDLE;
   CTS  = 0;
   SEL0 = 1;SEL1 = 0;
   SEL2 = 0;   
	SCON = 0x50;//0x50;
	TMOD = 0x21;
	TL1 = -12; 	//9600 BaudRate @ 6 Clock Cycle
	TH1 = -12;
	TR1 = 1;
	ES = 1;
	AUXR 	= AUXR | 0xFD;
	CKCON0 = CKCON0 | 0x01;
	EA = 1;
	//InitDisp();
	//LCDClear();
	isInit = 1;
	//Running Forever
	while(1)
	{
		if(rxFlag)
		{
			rxFlag = 0;
			switch(rxState)
			{
				case RX_Check_GHI:
					if(rxChr=='!')
					{
						rxState = RX_INIT_DONE;
					}
					break;
				case RX_INIT_DONE:
					if(rxChr==13)
					{
					 	state = FILE_MOUNT;
					}
					break;
				case RX_CHECK_FM:
				/*	if(rxChr == 13)
					{
						rxState = RX_CHECK_FM2;
					}
					break;
				case RX_CHECK_FM2:
					if(rxChr == 13)
					{
						rxState = RX_CHECK_FM3;
					}
					break;	
				case RX_CHECK_FM3:
				*/	if(rxChr == 13)
					{
					 	//Putc('F');
					 	state = CHANGE_BAUD;
					}
					break;
				case RX_CHECK_BR:
					if(rxChr == 13)
					{
						//Putc('=');
						state = CALC_SPACE;
					}
					break;
				case RX_CHECK_MS:
					if(rxChr == '$')
					{
					 	rxCnt = 1;
					 	rxState = RX_SAVE_VAL;
					}
					break;
				case RX_SAVE_VAL:
					if(rxChr == 13)
						rxState = RX_MS_DONE;
					else
						rxBuf[rxCnt++] = rxChr;
					break;
				case RX_MS_DONE:
					if(rxChr = 13)
						state = CHK_FREE_SPACE;
					break;
				///////////////////////////////////////////////////////////////////////////
				case RX_BYTE_0:
					if (rxChr == 0xBF)
					{
						rxBuf[rxCnt++] = rxChr; rxState = RX_BYTE_1; 
					}
					else
					{
						rxCnt = 0; rxState = RX_BYTE_0;
					}
					break;
				case RX_BYTE_1:
					if (rxChr == 0x13)
					{
						rxBuf[rxCnt++] = rxChr; rxState = RX_BYTE_2; 
					}
					else
					{
						rxCnt = 0; rxState = RX_BYTE_0;
					}
					break;
				case RX_BYTE_2:
					if (rxChr == 0x97)
					{
						rxBuf[rxCnt++] = rxChr; rxState = RX_BYTE_3; 
					}
					else
					{
						rxCnt = 0; rxState = RX_BYTE_0;
					}
					break;
				case RX_BYTE_3:
					if (rxChr == 0x74)
					{
						rxBuf[rxCnt++] = rxChr; rxState = RX_DATA; 
					}
					else
					{
						rxCnt = 0; rxState = RX_BYTE_0;
					}
					break;
				case RX_DATA:
					rxBuf[rxCnt++] = rxChr; 
					if (rxCnt == 474)
						rxState = RX_BYTE_474;
					break;
				case RX_BYTE_474:
					if (rxChr == 0x40)
					{
						rxBuf[rxCnt++] = rxChr; rxState = RX_BYTE_475; 
					}
					else
					{
						rxCnt = 0; rxState = RX_BYTE_0;
					}
					break;
				case RX_BYTE_475:
					if (rxChr == 0x40)
					{
						rxBuf[rxCnt++] = rxChr; rxState = RX_BYTE_476;
					}
					else
					{
						rxCnt = 0; rxState = RX_BYTE_0;
					}
					break;
				case RX_BYTE_476:
					if (rxChr == 0x40)
					{
						rxBuf[rxCnt++] = rxChr; rxState = RX_IDLE;
						state = ns_after_pack;
						//Putc('A');
					}
					else
					{
						rxCnt = 0; rxState = RX_BYTE_0;
					}
					break;
				//////////////////////////////////////////////////////////////////////////
				case RX_WRITE_RES:
					if(rxChr == 13)
					{
						state = WRT_PACKET;
					}
					break;
				case RX_WRITE_DONE:
					if(rxChr == 13)
					{
						state = FLUSH_FILE;
					}
					break;
				default:
					break;
			}//sw
		}//rxFlag
		switch(state)
		{
		 	case IDLE:
		 		break;
		 	case FILE_MOUNT:
		 		SD_WriteCmd("FM S\r");
		 		rxState = RX_CHECK_FM;
		 		state = IDLE;
		 		break;
		 	case CHANGE_BAUD:
		 		SD_WriteCmd("BR 0040\r");
				TL1 = -2;
				TH1 = -2;
			 	rxState = RX_CHECK_BR;
			 	state = IDLE;
				break;
			case CALC_SPACE:
				SD_WriteCmd("MS\r");
				rxState = RX_CHECK_MS;
				state = IDLE;
				break;
			case CHK_FREE_SPACE:
				rxCnt = 0;
				tot_size = 0;
 				free_size = 0;
	 			for(h=1;h<9;h++)
	 			{  
	 				nibble = rxBuf[h] - 48;
 					if(nibble>9)
 						nibble = nibble - 7;
 					tot_size = tot_size<<4;
 					tot_size = tot_size + nibble;// No of Totoal Sectors
	 			}
	 			for(h=11;h<19;h++)
	 			{  
 					nibble = rxBuf[h] - 48;
 					if(nibble>9)
 						nibble = nibble - 7;
	 				free_size = free_size<<4;
	 				free_size = free_size + nibble;// No of Free Sectors
	 			}
 				tot_size = tot_size*512;// No of Total Bytes
 				tot_size = tot_size/474;// No of Packets that can be written in Total Space
 				free_size = free_size*512;// No of Free Bytes
	 			free_size = free_size/474;// No of Packets that can be written in Free Space
	 			Percent = (free_size*100)/tot_size; // Percentage of Unused Space
				
 				if(free_size != 0)
 				{
 					//Putc((Percent/10)+48);
	 				//Putc((Percent%10)+48);
		 			//Putc('%');
		 			rxCnt = 0;
		 			led3 = 1;
		 			SEL0 = 0; SEL1 = 1;
		 			rxState = RX_BYTE_0;
			   	ns_after_pack = Drop_1st_Packet;
			   }
		   	else
 			   {
 			   	//Putc('F');
			   	//Putc('L');
			   	rxState = RX_IDLE;
			   }
			   state = IDLE;
				break;
	 		case Drop_1st_Packet:
	 			ns_after_pack = Drop_2nd_Packet;
	 			rxCnt = 0; rxState = RX_BYTE_0;
	 			state = IDLE;
	 			break;
	 		case Drop_2nd_Packet:
	 			ns_after_pack = Drop_3rd_Packet;
	 			rxCnt = 0; rxState = RX_BYTE_0;
	 			state = IDLE;
	 			break;
	 		case Drop_3rd_Packet:
	 			ns_after_pack = Drop_4th_Packet;
	 			rxCnt = 0; rxState = RX_BYTE_0;
	 			state = IDLE;
	 			break;
	 		case Drop_4th_Packet:
	 			ns_after_pack = Drop_5th_Packet;
	 			rxCnt = 0; rxState = RX_BYTE_0;
	 			state = IDLE;
	 			break;
	 		case Drop_5th_Packet:
	 			ns_after_pack = First_Packet;
	 			rxCnt = 0; rxState = RX_BYTE_0;
	 			state = IDLE;
	 			break;
	 		case First_Packet:
	   	 	FileName[0] = rxBuf[473];//Second
	   	 	FileName[1] = rxBuf[472];//Minute
	   	 	FileName[2] = rxBuf[471];//Hour
  		 		FileName[3] = rxBuf[470];//Day
  			 	FileName[4] = rxBuf[469];//Month
  		 		FileName[5] = rxBuf[468];//Year
				PreHour = rxBuf[471];
				PreDay = rxBuf[470];
				SEL0 = 1; SEL1 = 0;
				ns_after_pack = CHK_SPACE_AGAIN;
				state = MAKE_FOLDER;
				break;
			case MAKE_FOLDER:
				// Make New Folder
				SendChar('M');
		 		SendChar('D');
		 		SendChar(' ');
		 		SendChar('2');
		 		SendChar('0');
		 		SendChar((FileName[5]/10)+48);
	 			SendChar((FileName[5]%10)+48);
	 			SendChar((FileName[4]/10)+48);
		 		SendChar((FileName[4]%10)+48);
		 		SendChar((FileName[3]/10)+48);
		 		SendChar((FileName[3]%10)+48);
		 		SendChar('\r');
		 		state = CHANGE_DIR;
		 		//Putc('B');
		 		break;
		 	case CHANGE_DIR:
		 		// Change Directory to New Folder
		 		SendChar('C');
	 			SendChar('D');
	 			SendChar(' ');
	 			SendChar('2');
	 			SendChar('0');
	 			SendChar((FileName[5]/10)+48);
 				SendChar((FileName[5]%10)+48);
 				SendChar((FileName[4]/10)+48);
	 			SendChar((FileName[4]%10)+48);
	 			SendChar((FileName[3]/10)+48);
	 			SendChar((FileName[3]%10)+48);
	 			SendChar('\r');
	 			state = OPEN_FILE;
	 			//Putc('C');
	 			break;
	 		case OPEN_FILE:
	 			// Creat New File
		 		SendChar('O');
	 			SendChar('F');
	 			SendChar(' ');
	 			SendChar('0');
	 			SendChar('A');
	 			SendChar('>');
	 			SendChar((FileName[2]/10)+48);
	 			SendChar((FileName[2]%10)+48);
				SendChar((FileName[1]/10)+48);
				SendChar((FileName[1]%10)+48);
				SendChar((FileName[0]/10)+48);
				SendChar((FileName[0]%10)+48);
	 			SendChar('.');
	 			SendChar('e');
	 			SendChar('v');
	 			SendChar('t');
	 			SendChar('\r');
	 			state = WRITE_FILE;
	 			//Putc('D');
	 			break;
	 		case  WRITE_FILE:
	 			// Send Write Command
		 		SD_WriteCmd("WF 0>01D4\r");
	 			rxState = RX_WRITE_RES;
	 			state = IDLE;
	 			//Putc('E');
	 			break;
	 		case WRT_PACKET:
	 			for(h=0;h<468;h++)
		 			SendChar(rxBuf[h]);
		 		rxState = RX_WRITE_DONE;
		 		state = IDLE;
		 		//Putc('F');
		 		break;
		 	case FLUSH_FILE:
		 		free_size--;
		 		SD_WriteCmd("FF 0\r");
		 		SEL0 = 0; SEL1 = 1;
		 		rxCnt = 0; rxState = RX_BYTE_0;
		 		state = IDLE;
		 		//Putc('G');
		 		break;
			///////////////////////////////////////////////////////////////
			case CHK_SPACE_AGAIN:
				if(free_size != 0)
				{
					Percent = (free_size*100)/tot_size;
	 				Percent = Percent/25;	// 0 1 2 3
	 				state = Percent + 50;	//50 51 52 53
	 				//Putc('L');
		 		}
		 		else
		 		{
		 		 	led1 = 1;//0;
		 		 	led2 = 1;//0;
		 		 	led3 = 1;//0;
		 		 	//led4 = 1;//0;
		 		 	//Putc('F');
		 		 	//Putc('L');
		 		 	state = IDLE;
		 		}
				break;
			case below_100:
				led1 = ~led1;
				state = WRITE_AGAIN;
				break;
			case below_75:
				led1 = 1;//0;
				led2 = ~led2;
				state = WRITE_AGAIN;
				break;
			case below_50:
				led1 = 1;//0;
				led2 = 1;//0;
				led3 = ~led3;
				state = WRITE_AGAIN;
				break;
			case below_25:
				led1 = ~led1;
				led2 = ~led2;
				led3 = ~led3;
				//led1 = 1;//0;
				//led2 = 1;//0;
				//led3 = 1;//0;
				//led4 = ~led4;
				state = WRITE_AGAIN;
				break;
		 	case WRITE_AGAIN:
		 		SEL0 = 1; SEL1 = 0;
	 			if(PreDay!=rxBuf[470])		//Day Changed
	 			{
	 				FileName[0] = rxBuf[473];//Second
	   	 		FileName[1] = rxBuf[472];//Minute
		   	 	FileName[2] = rxBuf[471];//Hour
	  		 		FileName[3] = rxBuf[470];//Day
  				 	FileName[4] = rxBuf[469];//Month
  			 		FileName[5] = rxBuf[468];//Year
	 				// Close Last File
		 			SD_WriteCmd("CF 0\r");
		 			// Change Path to Root Directory
		 			SD_WriteCmd("CD \\\r");
		 			state = MAKE_FOLDER;
		 			//Putc('H');
		 		}
		 		else if((PreDay==rxBuf[470]) && (PreHour!=rxBuf[471]))		//Day Same; Hour Changed
	 			{
	 				FileName[0] = rxBuf[473];//Second
	   	 		FileName[1] = rxBuf[472];//Minute
		   	 	FileName[2] = rxBuf[471];//Hour
	  		 		FileName[3] = rxBuf[470];//Day
  				 	FileName[4] = rxBuf[469];//Month
  			 		FileName[5] = rxBuf[468];//Year
	 				// Close Last File
		 			SD_WriteCmd("CF 0\r");
		 			state = OPEN_FILE;
		 			//Putc('I');
			 	}
			 	else
			 	{
			 		// Write Command
		 			SD_WriteCmd("WF 0>01D4\r");
		 			rxState = RX_WRITE_RES;
		 			state = IDLE;
		 			//Putc('J');
		 		}
			 	PreHour = rxBuf[471];
			 	PreDay = rxBuf[470];
		 		break;
		}//sw
	}//wh
}//main
void Serial() interrupt 4
{
	if (RI)
	{
		RI = 0;
		rxChr = SBUF;
		rxFlag = 1;
   }// End if RI
}
























































