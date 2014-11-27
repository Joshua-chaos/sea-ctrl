/*	
						ZheJiang University
			Control Science & Engineering(CSE)
							file information
	file name:elmo_velocity.cpp
	version:v1.0
	lastdate:2013/4/16  y/m/d
	author：jiaxuechao
*/
#include "main.h"
// uint8 Tflag;

int main (void)
{  
	 //indicate the system is started 
   Howl();
   //initial the target board,here just open uart0,can1
   TargetInit();
   //initial the target,here work in torque mode	
	 motor_init();
// 	 //open timer0
// 	 Tflag=0;
// 	 Timer0Init();
	 //start the task
	 Task();
	while (1);
                               
}

void Task (void)
{	
//  char str[20];

	while(1){
    for(int i=0; i<40; i++){
		int target = (i%2 ? -10000 : 10000) * (rand()%1000 / 1000.0f);

		//	write target position 
		SetPosition(target);
    //  break the form command and start the new one
		Dnld16(OBJID_CONTROL, 0, (uint16)0x3F,0);	
		//	this command sent twice,enable everything
	  Dnld16(OBJID_CONTROL, 0, (uint16)0x0f,0);
		Dnld16(OBJID_CONTROL, 0, (uint16)0x0f,0);
		  do{
			 uint32 state=0;
			 Upld32(OBJID_STATUS, 0, &state);
			 if(state&(1<<10)){break;}//wait for #6041 bits10 set 1
       }while(1);
		sysDelayNS(1000);
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
	sprintf(message, "velocity started:\r\n");
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
	err=Dnld8(OBJID_HOME_METHOD, 0, (uint8)35,2);
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
    
  //	set position mode step:9
	err=SetPositionMode(1);
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
// 	Tflag=1;
	T0IR        = 0x01;					                                /* 清除中断标志	                */
	VICVectAddr = 0x00;					                                /* 通知VIC中断处理结束          */
}

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
	

static void showerr(uint8 err,uint8 step)
{ static char Err_message[32];
	sprintf(Err_message, "step:%2d err:%2d\r\n",step,err);
  UART0_SendStr (Err_message);
}
