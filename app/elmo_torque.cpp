/*	
						ZheJiang University
			Control Science & Engineering(CSE)
							file information
	file name:elmo_torque.cpp
	version:v1.0
	lastdate:2013/5/10  y/m/d
	author：jiaxuechao
*/
#include "main.h"

uint8 Tflag;

void Measure()
{ 
	ENCODER encoder[2];
	SPRING spring;
	
	Update_encoder(encoder);
	//initial the spring
	spring.pos_free=0;  //这个值预先设定，值为弹簧的自由时，两个编码器的角度差
	//计算弹簧形变量初值，即预紧量
	spring.pos=encoder[1].cur_pos-encoder[0].cur_pos-spring.pos_free;
	
	//initial the encoder 需要测量转速前必须执行这一步
	encoder[0].ready=0; 
	encoder[1].ready=0;
	
	//float temp[2];
	char message[100];
	while(1){

	Update_encoder(encoder);
	//encoder1 is load,encoder0 is motor
  spring.pos=spring.pos+encoder[1].posinterval-encoder[0].posinterval;
	spring.pos=encoder[1].velocity-encoder[0].velocity;
	//for(int i=0;i<2;i++){
	//temp[i]=360.0*encoder[i].cur_pos/0x3ffff;}
	sprintf(message, "pos:%x vel:%f\r\n",encoder[0].cur_pos,encoder[0].velocity);	
	//sprintf(message, "left:%5f right:%5f\r\n",temp[0],temp[1]);
  UART0_SendStr (message);
	
	}
}

int main (void)
{  
#if development_board
	 //indicate the system is started 
   Howl();
#endif
   //initial the target board,here just open uart0,can1
   TargetInit();
   //initial the target,here work in torque mode	
	 //motor_init();
	 //initial the encoder
	 SSI_Init();
	 //open timer0
	 Tflag=0;
	 Timer0Init();
	 //start the task
	 //Task();
	 Measure();
	while (1);
                               
}

void Task (void)
{	
//  char str[20];

	while(1){
    for(int i=0; i<10000; i++){
		int torque = sin(float(i)/10000*3.1415*2) * 30;
		while(Tflag==0);   //wait for the timer flag
		//	set profiled torque 
		SetTorque(torque);

		//	enable start
	  Dnld16(OBJID_CONTROL, 0, (uint16)0x0f,0);
		Tflag=0;
    }	
 }
}

/* description:	Initialize the motor,the motor can work in three diffrient mode
								posiotion,velocity,torque.this one is initialed to torque,modify
								the code if needed
//		function: void motor_init(void)
//			@param: 
//return value:	
*/
void motor_init(void)
{  
	static char message[32];
	uint8 err;
	sprintf(message, "torque started:\r\n");
  UART0_SendStr (message);
	
	
	//get initial position step:0
	uint32 pos;
	err=GetPosition(&pos);
	if(err){showerr(err,0);}
  else{
	sprintf(message, "initial pos: %4d\r\n",pos);
  UART0_SendStr (message);
		}

	//	set homing mode step:1
	err=SetHomeMode(1);
  if(err){showerr(err,1);}
	
	//	set homing method step:2
	//	here the mean of homing is that
	//  to use the current position as homing position
	err=Dnld8(0x6098, 0, (uint8)35,2);
  if(err){showerr(err,2);} 
	 
	//	shutdown step:3
	err=Dnld16(OBJID_CONTROL, 0, (uint16)0x06,3);
  if(err){showerr(err,3);}
	
	//wait at least 3ms to avoid the Priority inversion
   sysDelayNS(3);
	 
	//	switch on step:4
  err=Dnld16(OBJID_CONTROL, 0, (uint16)0x07,4);
  if(err){showerr(err,4);}
	
	//	enable operation step:5
	err=Dnld16(OBJID_CONTROL, 0, (uint16)0x0f,5);
  if(err){showerr(err,5);}
	
	//	 start home mode step:6
	err=Dnld16(OBJID_CONTROL, 0, (uint16)0x1f,6);
  if(err){showerr(err,6);}
	 
	//	 home mode active step:7
	err=Dnld16(OBJID_CONTROL, 0, (uint16)0x1f,7);
  if(err){showerr(err,7);}
   
   sysDelayNS(10);
	 
	//get current position step:8
	err=GetPosition(&pos);
	if(err){showerr(err,8);}
	else{
	sprintf(message, "current pos: %4d\r\n",pos);
  UART0_SendStr (message);
		}
	//===================================
    
  //	set torque mode step:9
	err=SetTorqueMode(1);
  if(err){showerr(err,9);}	
	
  //	shutdown step:10
	err=Dnld16(OBJID_CONTROL, 0, (uint16)0x06,2);
  if(err){showerr(err,10);}
  
	//	switch on step:11
  err=Dnld16(OBJID_CONTROL, 0, (uint16)0x07,3);
  if(err){showerr(err,11);}
	
	//wait at least 3ms to avoid the Priority inversion
   sysDelayNS(3);

	//	enable operation step:12
	err=Dnld16(OBJID_CONTROL, 0, (uint16)0x0f,4);
  if(err){showerr(err,12);}

// 	while(1){;} //just for debug
}


/* description:	the task when timer0 exception hanppen 
//		function: void Timer0_Exception (void)
//			@param: 
//return value:	
*/
 void Timer0_Exception (void)
{
	Tflag=1;
	T0IR        = 0x01;					                                /* 清除中断标志	                */
	VICVectAddr = 0x00;					                                /* 通知VIC中断处理结束          */
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

static void showerr(uint8 err,uint8 step)
{ static char Err_message[32];
	sprintf(Err_message, "step:%2d err:%2d\r\n",step,err);
  UART0_SendStr (Err_message);
}
