/*	
						ZheJiang University
			Control Science & Engineering(CSE)
							file information
	file name:ssi.c
	version:v1.0
	date:2013/8/30  y/m/d
	modified:2013/09/12 y/m/d
	author：jiaxuechao
*/

//head files
#include "ssi.h"

/* description:	 
//		function:
//			@param: 
//return value:	
*/
void SSI_Init(void)
{
	PCONP 	|= (1 << 21);  //SSP0 power on,initaily on
	PCONP 	|= (1 << 10);  //SSP1 power on,initaily on

#if 1
  //set P0.15(SCK0)
	PINSEL0 = ((PINSEL0 & 0x3FFFFFFF) | 0x80000000); 
	//set P0.16(SSEL0),P0.17(MISO0),P0.18(MOSI0)
	PINSEL1 = ((PINSEL1 & 0xFFFFFFC0) | 0xFFFFFF2A); 
#else
  //set P2.22(SCK0),P2.23(SSEL0),P2.26(MISO0),P2.27(MOSI0)
  PINSEL5 |= (0x03 << 12) | (0x03 << 14) | (0x03 << 20) | (0x03 << 22);     
#endif

	//set P0.6(SSEL1),P0.7(SCK1),P0.8(MISO1),P0.9(MOSI1)
	PINSEL0 = ((PINSEL0 & 0xFFF00FFF) | 0x000AA000); 
  
	
  SSP0CPSR = 0x18; //SSPCLK=Fpclk/CPSDVSR=24MHz/24
  SSP1CPSR = 0x18; 	
  //BITCLK=SSPCLK*(SCR+1)
	//set data length 9,frame SSI,SCR=0
  SSP0CR0  = 0x0018;
	SSP1CR0  = 0x0018;

  //set SSP0 to master,LBM=0,and enable it
	//SSP0CR1=0x03;  //回写模式，用于调试
	SSP0CR1 |= (0x01 << 1);  //normal work mode
	SSP1CR1 |= (0x01 << 1);
	
	//开启定时器1，以获得信号的时间戳
	Timer1Init();
}

void Update_encoder(ENCODER *encoder)
{ 
	char i;
	static int temp;
	//update time uint:us
	encoder[0].timeinterval=T1TC-encoder[0].timestamp; //timeinterval=currenttime-lasttime
	encoder[0].timestamp=T1TC;//timestamp used to remember lasttime
	
	//update current position	
  //while ((SSP0SR & 0x02) == 0)；
 	SSP0DR = 0x1a5;	
	SSP0DR = 0x15a;    //两帧连发，产生19个时钟脉冲
	//SSP0CR1 |= (0x01 << 1);
	while((SSP0SR&0x04)==0);
	//encoder[0].cur_pos=0;
	encoder[0].cur_pos=(SSP0DR&0x1ff);
	encoder[0].cur_pos=(encoder[0].cur_pos<<9)|(SSP0DR&0x1ff);
	
	//update timestamp uint:us
	encoder[1].timeinterval=T1TC-encoder[1].timestamp; //timeinterval=currenttime-lasttime
	encoder[1].timestamp=T1TC;//timestamp used to remember lasttime
	
	//update current position
  //while ((SSP1SR & 0x02) == 0);
 	SSP1DR = 0x1a5;	
	SSP1DR = 0x15a;
	//SSP1CR1 |= (0x01 << 1);
	while((SSP1SR&0x04)==0);	
	//encoder[1].cur_pos=0;
	encoder[1].cur_pos=(SSP1DR&0x1ff);
	encoder[1].cur_pos=(encoder[1].cur_pos<<9)|(SSP1DR&0x1ff);
  
 
	for(i=0;i<2;i++){
	//update the position interval
	if(encoder[i].cur_pos!=0)
		{encoder[i].posinterval=encoder[i].cur_pos-encoder[i].last_pos;}
  else    //根据实验假定encoder[i].cur_pos=0 为错误读数较合理，采用一阶保持
		{ temp=encoder[i].last_pos+encoder[i].posinterval;
			if(temp<0)
			{encoder[i].cur_pos=temp+FULL_CIRCLE;}
			else if(temp>FULL_CIRCLE)
			{encoder[i].cur_pos=temp-FULL_CIRCLE;}
			else
			{encoder[i].cur_pos=temp;}				
		}
	 
	//update the last_pos	
	encoder[i].last_pos=encoder[i].cur_pos;
	 
	//位置差纠正
	if(encoder[i].posinterval>HALF_CIRCLE)
	{
		encoder[i].posinterval=encoder[i].posinterval-FULL_CIRCLE;
	}
	else if(encoder[i].posinterval<-HALF_CIRCLE)
	{
		encoder[i].posinterval=encoder[i].posinterval+FULL_CIRCLE;
	}
	
	//caculate the velocity
	if(encoder[i].ready)
	{ 
		//uint: cnt/s
		encoder[i].velocity=1000000.0*encoder[i].posinterval/encoder[i].timeinterval;
	}
	else
	{
		encoder[i].ready=1;
	}
	
	}
}
