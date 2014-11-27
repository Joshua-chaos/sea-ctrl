/*	
						ZheJiang University
			Control Science & Engineering(CSE)
							file information
	file name:miscellaneous.c
	version:v1.0
	lastdate:2013/10/15  y/m/d
	author£ºjiaxuechao
*/
#include "main.h"

void Correction(int *pos)
{
		  if(*pos>HALF_CIRCLE)
	  {
		  *pos=*pos-FULL_CIRCLE;
	  }
	  else if(*pos<-HALF_CIRCLE)
	  {
		  *pos=*pos+FULL_CIRCLE;
	  }
}

error_code CAN1_Sent(uint32 COB_ID,uint8 len,char *data)
{	
	 static CANFRAME frame;
   static error_code err;
	 uint8 i;
	
   frame.ID=COB_ID;
   frame.LEN=len;
	 frame.RTR=CAN_FRAME_DATA;
	 frame.FF=CAN_FRAME_STD;

   for(i=0;i<len;i++){frame.DATA[i]=data[i];}

   err=Xmit(CAN1,&err,1,&frame);
       if(err==read_wait_err){err=none_err;}
    return err;	
}
error_code CAN2_Sent(uint32 label,REG_CANDF *CanMessage)
{
	static CANFRAME frame;
	static error_code err;
	
		frame.ID=Device_ID|((label&0x1F)<<6);
		frame.LEN=8;
		frame.RTR=CAN_FRAME_DATA;
		frame.FF=CAN_FRAME_STD;
	  frame.DATA[0]=CanMessage[0].Bits.Data1;
	  frame.DATA[1]=CanMessage[0].Bits.Data2;
		frame.DATA[2]=CanMessage[0].Bits.Data3;
		frame.DATA[3]=CanMessage[0].Bits.Data4;
		frame.DATA[4]=CanMessage[1].Bits.Data1;
		frame.DATA[5]=CanMessage[1].Bits.Data2;
		frame.DATA[6]=CanMessage[1].Bits.Data3;
		frame.DATA[7]=CanMessage[1].Bits.Data4;
	
	err=Xmit(CAN2,&err,1,&frame);
       if(err==read_wait_err){err=none_err;}
    return err;	
}

uint8 WriteParam(PARAMTABLE *paramtable)
{	
	uint8 err;
	uint32 i;
	uint16 *data;
	err=TRUE;
	data=(uint16 *)paramtable;
	SectorErase(SA(0));
	for(i=0;i<sizeof(*paramtable)/sizeof(*data);i++)
		{
			err&=WordProgram((i<<1),data[i]);
		}
	return err;
}

void ReadParam(PARAMTABLE *paramtable)
{
	*paramtable=(*(PARAMTABLE *)(GetAddr(0)));
}

#if development_board
void Howl(void)
{
		BUZZER_Set();
	  sysDelayNS(100);
	  BUZZER_Clr();
	  sysDelayNS(100);
	  BUZZER_Set();
	  sysDelayNS(100);
	  BUZZER_Clr();
}
#endif
