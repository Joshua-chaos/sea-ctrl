/*	
						ZheJiang University
			Control Science & Engineering(CSE)
							file information
	file name:elmo_velocity.cpp
	version:v1.0
	lastdate:2013/10/31  y/m/d
	author：jiaxuechao
*/
#include "main.h"
//*****global values*****
uint8 Tflag;
uint8 ARMSTATE=STATE_DISABLE;
uint32 CANTimeCounter=0;
int 	spring_pos[FilterOrder+1]={0,0,0,0,0,0,0,0,0,0,0};
float	Num[FilterOrder+1]={0.0627,0.0599,0.0799,0.0969,0.1083,0.1123,0.1083,0.0969,0.0799,0.0599,0.0627};
float vel_loadbuf[FilterOrder+1]={0,0,0,0,0,0,0,0,0,0,0};
strRxBUF Command;
static REG_CANDF CanMessage[2];
//values used in control loop
int spring_pos_set1=0,spring_pos_set2=0,vel_target=0;
static PARAMTABLE param;
static ENCODER encoder[2];
int main (void)
{  
#if development_board
	 //indicate the system is started 
   Howl();
#endif
   //initial the target board,here just open uart0,can1
   TargetInit();
	 CAN2IER |=0x01;         /*接收中断使能*/
	 vicIrqFuncSet(23,2,(INT32U)CAN2_Handler);

   //initial the target,here work in torque mode	
	 motor_init();

	 //initial the encoder
	 SSI_Init();
	 //open timer0
	 Tflag=0;
	 Timer0Init();
	 //start the task
	 Task();
	while (1);                             
}

void Task (void)
{	
  //char str[20];
  SPRING spring;
	uint32 Tcounter=0;
	//inner used values
	float spring_velocity_set=0;
	static int vel_set=0;
	int delta0;
	float delta1;
	float vel_load;
  ReadParam(&param);
  uint32 i,temp;
	
	for(i=0;i<10;i++)
	{
		Update_encoder(encoder);
		temp=encoder[0].cur_pos;
		temp=encoder[1].cur_pos;
		temp=temp;
		sysDelayNS(10);
	}
	//joint homing
  JointHoming();

	Update_encoder(encoder);
	//initial the encoder 需要测量转速前必须执行这一步
	encoder[0].ready=0; 
	encoder[1].ready=0;
	//initial the spring	
	spring.pos=0;


#if test_mode
	while(1){

		while(Tflag==0);   //wait for the timer flag
		//========================================================
		Update_encoder(encoder);
		//encoder0 is load,encoder1 is motor
		//spring.pos=spring.pos+encoder[0].posinterval-encoder[1].posinterval;
		spring.pos=encoder[0].cur_pos-encoder[1].cur_pos;		
		Correction(&spring.pos);//correct the sping position
	  spring.pos=spring.pos-param.SpringFree;	
		spring.velocity=encoder[0].velocity-encoder[1].velocity;
		push_int(spring_pos,spring.pos);
		vel_load=encoder[0].velocity;
		push_float(vel_loadbuf,vel_load);
		Tflag=0;
//        CanMessage[0].DWord=(*(unsigned long *)(&(encoder[0].cur_pos)));
// 		    CanMessage[1].DWord=(*(unsigned long *)(&(encoder[1].cur_pos)));
// 		    CAN2_Sent(COMMAND_NONE,CanMessage);
	    CanMessage[0].DWord=(*(unsigned long *)(&(encoder[0].cur_pos)));
			CanMessage[0].Bits.Data4|=0x00;
			CanMessage[1].DWord=(*(unsigned long *)(&(encoder[0].velocity)));
			CAN2_Sent(COMMAND_NONE,CanMessage);
			CanMessage[0].DWord=0x00;
			CanMessage[0].Bits.Data4|=0x80;
			CanMessage[1].DWord=(*(unsigned long *)(&(spring.pos)));
			CAN2_Sent(COMMAND_NONE,CanMessage);
#if Knee
			CanMessage[0].DWord=0x00;
			CanMessage[1].DWord=0x00;
			CanMessage[0].Bits.Data1=(IO0PIN&(1<<23))>>23;
			CAN2_Sent(TGND_LABEL,CanMessage);	
#endif			
	}
#else
	
	while(1){		
		while(Tflag==0);   //wait for the timer flag
		//========================================================
		Update_encoder(encoder);
		//encoder0 is load,encoder1 is motor
		//spring.pos=spring.pos+encoder[0].posinterval-encoder[1].posinterval;
		spring.pos=encoder[0].cur_pos-encoder[1].cur_pos;		
		Correction(&spring.pos);//correct the sping position
	  spring.pos=spring.pos-param.SpringFree;	
		spring.velocity=encoder[0].velocity-encoder[1].velocity;
		push_int(spring_pos,spring.pos);
		vel_load=encoder[0].velocity;
		push_float(vel_loadbuf,vel_load);
		Tflag=0;
		//=======================================================
	  if(Tcounter>=25)
		{ 
			//acquire spring_set every 5ms
				CommandParse(Command);
				Command.CANID=0x00;  //clear command area
			  Command.DATAA.DWord=0x00;
			  Command.DATAB.DWord=0x00;
			Tcounter=1;
		}else
		{
			Tcounter++;
		}
		
		if((Tcounter%5)==0){			
			switch(ARMSTATE){
				case STATE_DISABLE:
					vel_set = 0;
				break;
				case STATE_VELOCITY:
					vel_set=vel_target;
				break;
				case STATE_TORQUE:
					vel_set = 0;
					spring.pos=filter(Num,spring_pos);
					if(spring_pos_set1>spring.pos)
						{delta0=spring_pos_set1-spring.pos;}
					else if(spring_pos_set2<spring.pos)
						{delta0=spring_pos_set2-spring.pos;}
					else{delta0=0;}
					delta1=spring_velocity_set-spring.velocity;
					#if Knee
					#else
					vel_load=filter2(Num,vel_loadbuf);
			    #endif
					vel_set=param.Kp*delta0+param.Kd*delta1+param.FFRatio*vel_load;
					vel_set=vel_set*REALRATIO;//必须计算完成再下达								
				break;	
				default:vel_set=0;
				break;
			}
			
			//限幅，最高优先级
			if(encoder[0].cur_pos>param.PosLMT1 && encoder[0].cur_pos<HALF_CIRCLE && vel_set>0)
				{vel_set=0;}
			else if(encoder[0].cur_pos<param.PosLMT2 && encoder[0].cur_pos>HALF_CIRCLE && vel_set<0)
				{vel_set=0;}
				
			if(CANTimeCounter>=400){vel_set=0;}
			//	set profiled velocity
      CAN1_Sent(0x401,4,(char *)(&vel_set));
			//SetVelocity(vel_set);		
			//Enableoperation(0);//	enable start
			//	Dnld16(OBJID_CONTROL, 0, (uint16)0x010f,0);//	quick stop 
		}

		if(Tcounter%25==20)
		{ 
	    CanMessage[0].DWord=(*(unsigned long *)(&(encoder[0].cur_pos)));
			CanMessage[0].Bits.Data4|=0x00;
			CanMessage[1].DWord=(*(unsigned long *)(&(encoder[0].velocity)));
			CAN2_Sent(COMMAND_NONE,CanMessage);
			CanMessage[0].DWord=ARMSTATE;
			CanMessage[0].Bits.Data4|=0x80;
			CanMessage[1].DWord=(*(unsigned long *)(&(spring.pos)));
			CAN2_Sent(COMMAND_NONE,CanMessage);		
			
#if Knee	
//       CanMessage[0].DWord=(*(unsigned long *)(&(vel_set)));
// 			CanMessage[1].DWord=(*(unsigned long *)(&(encoder[1].velocity)));
// 			CAN2_Sent(TEST_LABEL,CanMessage);
			
			CanMessage[0].DWord=0x00;
			CanMessage[1].DWord=0x00;
			CanMessage[0].Bits.Data1=(IO0PIN&(1<<23))>>23;
			CAN2_Sent(TGND_LABEL,CanMessage);	
#endif			
		}

 }
#endif
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
	uint8 err;
	char data[8];
	
// 	err=SetHomeMode(1);if(err){showerr(err,1);}//	set homing mode step:1	
// 	//	here the mean of homing is that to use the current position as homing position
// 	err=Dnld8(OBJID_HOME_METHOD, 0, (uint8)35,2);if(err){showerr(err,2);}//	set homing method step:2  
// 	err=Shutdown(3);if(err){showerr(err,3);}//	shutdown step:3
// 	//wait at least 3ms to avoid the Priority inversion
//    sysDelayNS(3);
//   err=Switchon(4);if(err){showerr(err,4);}//	switch on step:4
// 	err=Enableoperation(5);if(err){showerr(err,5);}//	enable operation step:5
// 	err=Dnld16(OBJID_CONTROL, 0, (uint16)0x1f,6);if(err){showerr(err,6);}//	start home mode step:6
// 	err=Dnld16(OBJID_CONTROL, 0, (uint16)0x1f,7);if(err){showerr(err,7);}//	home mode active step:7
//   sysDelayNS(100);
	//==========================速度模式配置===============================  
	err=SetVelMode(1);if(err){showerr(err,9);} //	set velocity mode step:9	
	err=Shutdown(2);if(err){showerr(err,10);}//	shutdown step:10
  sysDelayNS(3);//wait at least 3ms to avoid the Priority inversion
  err=Switchon(3);if(err){showerr(err,11);}//	switch on step:11
	err=Enableoperation(4);if(err){showerr(err,12);}//	enable operation step:12
	sysDelayNS(3);
	//==========================配置RPDO====================================
	//Rpdo 设置
	//****映射控制字：0x6040 到第一个RPDO 0x1600
	//disable before set step:14
	err=Dnld16(0x1600, 0x00,(uint16)0x0000,1);if(err){showerr(err, 14);}
	//map 0x1600 0x01 to 映射控制字：0x6040 step:15
	err=Dnld32(0x1600, 0x01,0x60400010,2);if(err){showerr(err, 15);}
	//enable after set step:16
	err=Dnld16(0x1600, 0x00,(uint16)0x0001,3);if(err){showerr(err, 16);}
	sysDelayNS(3);
    
	//****映射目标速度：0x60ff到第三个RPDO 0x1602
	//disable before set step:17
	err=Dnld16(0x1602, 0x00,(uint16)0x0000,4);if(err){showerr(err, 17);}
	//map 0x1602 0x01 to 映射控制字：0x60ff step:18
	err=Dnld32(0x1602, 0x01,0x60ff0020,5);if(err){showerr(err, 18);}
	//enable after set step:19
	err=Dnld16(0x1602, 0x00,(uint16)0x0001,6);if(err){showerr(err, 19);}
	sysDelayNS(3);

	//RPDO触发方式设置为为： 异步
	err=Dnld8(0x1400, 0x02,(uint8)0xff,7);if(err){showerr(err, 20);}
	err=Dnld8(0x1402, 0x02,(uint8)0xff,8);if(err){showerr(err, 21);}
	sysDelayNS(3);
	//==========================配置TPDO====================================
	//TPDO映射
	//disable before set step:22
	err=Dnld16(0x1A00, 0x00,(uint16)0x0000,9);if(err){showerr(err, 22);}
	//map 0x1A00 0x01 to 映射状态字：0x6041 step:23
	err=Dnld32(0x1A00, 0x01,0x60410010,10);if(err){showerr(err, 23);}
	//map 0x1A01 0x01 to 映射控制字：0x606c step:24
	//err=Dnld32(0x1A00, 0x02,0x606C0020,11);if(err){showerr(err, 24);}
	//disable before set step:25
	err=Dnld16(0x1A00, 0x00,(uint16)0x0001,12);if(err){showerr(err, 25);}

	//TPDO触发方式设置为为： 异步
	err=Dnld8(0x1800, 0x02,(uint8)0xff,13);if(err){showerr(err, 26);}
	sysDelayNS(3);
	//==========================配置window====================================
	//set velocity window step:27
	err=Dnld16(OBJID_VEL_WINDOW, 0x00,(uint16)10,1);if(err){showerr(err, 27);}
	
	//set velocity velocity window time step:28
	err=Dnld16(OBJID_VEL_WINDOW_TIME, 0x00,(uint16)1,2);if(err){showerr(err, 28);}
	
	sysDelayNS(3);
	//上述映射成功后，激活总线在可操作状态
	//00 01 00
	data[0]=0x01;data[1]=0x00;err=CAN1_Sent(0x000,2,data);if(err){showerr(err, 22);}
	
}

/* description:	 
//		function: 
//			@param: 
//return value:	
*/
void CommandParse(strRxBUF Cmd)
{	
	switch((Cmd.CANID&COMMAND_MASK)>>6){
		    case COMMAND_STOP:
						ARMSTATE=STATE_DISABLE;break;
		    case COMMAND_ENABLE:
						ARMSTATE=STATE_TORQUE;break;
				case COMMAND_MODE_SET:
						switch(Cmd.DATAA.Bits.Data1){
							case STATE_TORQUE:
									ARMSTATE=STATE_TORQUE;break;
							case STATE_VELOCITY:
									ARMSTATE=STATE_VELOCITY;break;
							default:ARMSTATE=STATE_DISABLE;break;
						}
				break;
				case COMMAND_SPRING_POS_SET:
						spring_pos_set1=Cmd.DATAA.DWord;
						spring_pos_set2=Cmd.DATAB.DWord;
			  break;
				case COMMAND_HOMING:
						JointHoming();
			  break;
				case COMMAND_VELOCITY_SET:
					  vel_target=Cmd.DATAA.DWord;
				break;
		    case COMMAND_PARAM_SET:
						switch((Cmd.DATAA.DWord&INDEX_MASK)>>16){
							case PARAM_KP:
								param.Kp=(*(float *)(&(Cmd.DATAB.DWord)));
							break;
							case PARAM_KI:
								param.Ki=(*(float *)(&(Cmd.DATAB.DWord)));
							break;
							case PARAM_KD:
								param.Kd=(*(float *)(&(Cmd.DATAB.DWord)));
							break;
							case PARAM_FFRATIO:
								param.FFRatio=(*(float *)(&(Cmd.DATAB.DWord)));
							break;
							case PARAM_SpringFree:
								param.SpringFree=(*(int32 *)(&(Cmd.DATAB.DWord)));
							break;
							case PARAM_PosLMT1:
								param.PosLMT1=(*(uint32 *)(&(Cmd.DATAB.DWord)));
							break;
							case PARAM_PosLMT2:
								param.PosLMT2=(*(uint32 *)(&(Cmd.DATAB.DWord)));
							break;
							case PARAM_W_RFLASH:
								WriteParam(&param);
							break;
							default:break;
						}
				break;
				case COMMAND_PARAM_READ:
					  switch((Cmd.DATAA.DWord&INDEX_MASK)>>16){
							case PARAM_KP:
								CanMessage[1].DWord=(*(unsigned long *)(&param.Kp));
							break;
							case PARAM_KI:
								CanMessage[1].DWord=(*(unsigned long *)(&param.Ki));
							break;
							case PARAM_KD:
								CanMessage[1].DWord=(*(unsigned long *)(&param.Kd));
							break;
							case PARAM_FFRATIO:
								CanMessage[1].DWord=(*(unsigned long *)(&param.FFRatio));
							break;
							case PARAM_SpringFree:
								CanMessage[1].DWord=(*(unsigned long *)(&param.SpringFree));
							break;
							case PARAM_PosLMT1:
								CanMessage[1].DWord=(*(unsigned long *)(&param.PosLMT1));
							break;
							case PARAM_PosLMT2:
								CanMessage[1].DWord=(*(unsigned long *)(&param.PosLMT2));
							break;
							case PARAM_W_RFLASH:
								ReadParam(&param);
							break;
							default:break;
						}
						CanMessage[0].DWord=Cmd.DATAA.DWord;
						CAN2_Sent(COMMAND_PARAM_READ,CanMessage);
				break;
				default:break;} 
}

void JointHoming(void)
{
		int vel_set;
	  int times=0;
		do{
		Update_encoder(encoder);
	  if(encoder[1].cur_pos>50 && encoder[1].cur_pos<HALF_CIRCLE)
	  { 
		  vel_set=-encoder[1].cur_pos;
	  }
	  else if(encoder[1].cur_pos>HALF_CIRCLE && encoder[1].cur_pos<(FULL_CIRCLE-50))
	  { 
      vel_set=FULL_CIRCLE-encoder[1].cur_pos;
	  }
	  else
	  {
		  vel_set=0;
			times++;
	  }
		vel_set=vel_set*REALRATIO;
	  //SetVelocity(vel_set);
		CAN1_Sent(0x401,4,(char *)(&vel_set));
	  sysDelayNS(20);
}while(vel_set!=0||times<10);
}
/* description:	the task when CAN2 exception hanppen 
//		function: void CAN_Exception (void)
//			@param: 
//return value:	
*/
void CAN2_Exception(void)
{
	  CAN2ICR &=~0x01;
	  //软件验收滤波
	  if((LPC2400REG(CAN2,RID)&ID_MASK)==Device_ID){
			CANTimeCounter=0;
			ReadCanRxBuf(CAN2,&Command);
			}
		CanCmd(CAN2,REL_REC_BUF,0);	
	  VICVectAddr = 0x00;   
}

/* description:	the task when timer0 exception hanppen 
//		function: void Timer0_Exception (void)
//			@param: 
//return value:	
*/
 void Timer0_Exception (void)
{
	Tflag=1;
	//CANTimeCounter++;
	T0IR        = 0x01;					                                /* 清除中断标志	                */
	VICVectAddr = 0x00;					                                /* 通知VIC中断处理结束          */
}

static void showerr(uint8 err,uint8 step)
{ 
#if UARTOPEN	
	static char Err_message[32];
	sprintf(Err_message, "step:%2d err:%2d\r\n",step,err);
  UART0_SendStr (Err_message);
#endif	
	CanMessage[0].DWord=step;
  CanMessage[1].DWord=err;
	CAN2_Sent(ERROR_LABEL,CanMessage);
}
