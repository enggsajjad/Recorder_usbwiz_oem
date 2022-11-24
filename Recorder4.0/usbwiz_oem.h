#define Putc WriteData
#define RX_Check_GHI			1
#define RX_INIT_DONE			2
#define RX_CHECK_FM			3
#define RX_CHECK_BR			4
#define RX_CHECK_MS			5
#define RX_SAVE_VAL			6
#define RX_MS_DONE			7
#define RX_WRITE_RES			8
#define RX_WRITE_DONE		9
#define RX_BYTE_0				10
#define RX_BYTE_1				11
#define RX_BYTE_2				12
#define RX_BYTE_3				13
#define RX_BYTE_474			14
#define RX_BYTE_475			15
#define RX_BYTE_476			16
#define RX_DATA				17
#define RX_IDLE				18
#define RX_CHECK_FM2			19
#define RX_CHECK_FM3			20
#define IDLE					25
#define FILE_MOUNT			26
#define CHANGE_BAUD			27
#define CALC_SPACE			28
#define CHK_FREE_SPACE		29
#define Drop_1st_Packet		30
#define Drop_2nd_Packet		31
#define Drop_3rd_Packet		32
#define Drop_4th_Packet		33
#define Drop_5th_Packet		34
#define First_Packet			35
#define MAKE_FOLDER			36
#define CHANGE_DIR			37
#define OPEN_FILE				38
#define WRITE_FILE			39
#define WRT_PACKET			40
#define FLUSH_FILE			41
#define CHK_SPACE_AGAIN		42
#define below_100				53
#define below_75				52
#define below_50				51
#define below_25				50
#define WRITE_AGAIN			47
//Pins Assignments
sbit led1 	= P2^0;// Green
sbit led2 	= P2^1;// Blue
sbit led3 	= P2^2;// Red
//sbit led4	= P2^3;

sbit SEL2 	= P1^0;
sbit RTS 	= P1^1;
sbit SEL0	= P1^2;
sbit SEL1	= P1^3;
sbit CTS		= P1^4;

//sbit RS			=	P2^7;
//sbit RW			=	P2^6;
//sbit ELCD		=	P2^5;
//sbit BKLT		=	P2^4;
//sbit BS  		= P0^7;


xdata unsigned char rxBuf[477];
idata unsigned char FileName[6];
unsigned int rxCnt;
unsigned char state,tmp,nibble;
unsigned char PreHour,PreDay;
unsigned char rxState;
unsigned char ns_after_pack;
unsigned int h;
unsigned long tot_size;
unsigned char Percent;
long free_size;
bit isInit=0;
bit RTS_Flag;
bit rxFlag;
unsigned char rxChr;
void SendChar(unsigned char c)
{
	
	while(RTS);
	TI=1;
	while (!TI);	TI=0;	SBUF = c;
	while (!TI);	TI=0;
}
void SD_WriteCmd(char *cmd)
{
 	TI=1;
 	while(*cmd)
 	{
 		while (!TI);	TI=0;	SBUF = *cmd;
 	 	cmd++;
 	}
 	while (!TI);	TI=0;
}
/*
void LcdBusy()
{
		BS   = 1;		//Make D7th bit of LCD as i/p
   	ELCD   = 1;       //Make port pin as o/p
   	RS   = 0;       //Selected command register
   	RW   = 1;		//We are reading
   	while(BS)
   	{				//read busy flag again and again till it becomes 0 Enable H->L
   		ELCD   = 0;
      	ELCD   = 1;
   	}
}
void WriteControl(unsigned char var)
{
		P0 = var;		//Commands to be Written
   	RS   = 0;       //Selected command register
   	RW   = 0;       //We are writing in instruction register
   	ELCD   = 1;       //Enable H->L
   	ELCD   = 0;
   	LcdBusy();		//Wait for LCD to process the command
}
void WriteData(unsigned char var)
{
		P0 = var;		//Data/Character to be Written
   	RS   = 1;       //Selected data register
   	RW   = 0;       //We are writing
   	ELCD   = 1;       //Enable H->L
   	ELCD   = 0;
   	LcdBusy();      //Wait for LCD to process the command
}
void DelayOnems(void)
{
	TL0 = 0xCD;
	TH0 = 0xF8;
	TR0 = 1;
	while(!TF0);
	TR0 = 0;
	TF0 = 0;
}
void InitDisp()
{
	WriteControl(0x38);
	DelayOnems();
	WriteControl(0x38);
   DelayOnems();
   WriteControl(0x38);
   DelayOnems();
	WriteControl(0x06);
	DelayOnems();
	WriteControl(0x0c);
	DelayOnems();
}
void DispStr(char *s) 
{
	
	while (*s)
		WriteData(*s++);
}
void LCDCursor(unsigned char row, unsigned char col)
{
	switch (row)
	{
		case 1: WriteControl(0x80 + col - 1); break;
		case 2: WriteControl(0xc0 + col - 1); break;
		case 3: WriteControl(0x94 + col - 1); break;
		case 4: WriteControl(0xd4 + col - 1); break;
		default: break;
	}
	DelayOnems();
	DelayOnems();
}
void LCDClear()
{
 	WriteControl(0x01);
 	DelayOnems();
 	DelayOnems();
}
*/
