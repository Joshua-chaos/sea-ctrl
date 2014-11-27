/*	
						ZheJiang University
			Control Science & Engineering(CSE)
							file information
	file name:lpc2400can.c
	version:v1.0
	date:2012/10/11  y/m/d
	modified:2013/4/12
	last modified date:2013/10/23
	description:fix a fatal bug in HwEnCan();
							add Ch para to Xmit() and WaitForTransfer()
	author：jiaxuechao
*/

//head files
#include "lpc2400can.h"

/* description: use for can hardware reset
//    function: void	HwRstCAN (unsigned char Ch)
//			@param: Ch:CAN1,CAN2 
//return value:	void
*/
void		HwRstCAN(unsigned char Ch)
{
	PCONP &= ~((unsigned int)0x01 << (13+Ch));
}
/* description: use for can hardware enable
//    function: void	HwEnCAN (unsigned char Ch)
//			@param: Ch:CAN1,CAN2 
//return value:	void
*/
void		HwEnCAN(unsigned char Ch)
{
	switch(Ch) {														  /*	配置CAN1控制器引脚			 */
		case 0:
			PCONP |= 1 << 13;											/*	打开CAN控制器电源			 */
			
			PINSEL0 = PINSEL0 & (~0x0F) | 0x05;   /*  使能CAN1引脚                 */
			
			break;
		case 1:
			PCONP |= 1<<14;								     	  /*	打开CAN2控制器电源			 */
			
			PINSEL0 = PINSEL0 & (~(0x0F<<8)) | (0x0A<<8);                    /*  使能CAN2引脚                 */
			
			break;
		default:
			break;
	}
}
/* description: use for can software reset
//    function: unsigned char SwRstCAN(unsigned char Ch);
//			@param: Ch:CAN1,CAN2
//return value:	0(succeed) 1(error) 
*/
unsigned char SwRstCAN(unsigned char Ch)
{  
	REG_CANMOD regmod;
	regmod.DWord = LPC2400REG(Ch,MOD);									/*	读取CANMOD寄存器			 */
	regmod.Bits.RM = 1;													        /*	RM位置1 					 */
	LPC2400REG(Ch,MOD) = regmod.DWord;									/*	回写CANMOD寄存器			 */
	regmod.DWord = LPC2400REG(Ch,MOD);									/*	验证写入值					 */	
	return (unsigned char)((0 != regmod.Bits.RM)? 0:1); 
}
/* description: use for can quit software reset
//    function: unsigned char SwEnCAN(unsigned char Ch);
//			@param: Ch:CAN1,CAN2
//return value:	0(succeed) 1(error) 
*/
unsigned char SwEnCAN(unsigned char Ch)
{
	REG_CANMOD regmod;	
	regmod.DWord = LPC2400REG(Ch,MOD);									/*	读取CANMOD寄存器			 */
	regmod.Bits.RM = 0;												        	/*	RM位为0						 */
	LPC2400REG(Ch,MOD) = regmod.DWord;									/*	回写CANMOD寄存器			 */
	regmod.DWord = LPC2400REG(Ch,MOD);									/*	验证写入值					 */	
	return (unsigned char)((0 != regmod.Bits.RM)? 1:0); 
}
/* description: use for can enter sleep mode
//    function: unsigned char CanEntrySM(unsigned char Ch)
//			@param: Ch:CAN1,CAN2
//return value:	0(succeed) 1(error) 
*/
#if SLEEPEN>0
unsigned char	CanEntrySM(unsigned char Ch)
{
	REG_CANMOD regmod;	
	regmod.DWord = LPC2400REG(Ch,MOD);									/*	读取CANMOD寄存器			 */
	regmod.Bits.SM = 1;											 /*	SM只有在正常模式，总线空闲时才能置位 */
	LPC2400REG(Ch,MOD) = regmod.DWord;									/*	回写CANMOD寄存器			 */
	regmod.DWord = LPC2400REG(Ch,MOD);									/*	验证写入值					 */	
	return (unsigned char)((0 != regmod.Bits.SM)? 0:1); 
}
#endif
/* description: use for can quit sleep mode
//    function: unsigned char CanQuitSM(unsigned char Ch)
//			@param: Ch:CAN1,CAN2
//return value:	0(succeed) 1(error) 
*/
#if SLEEPEN>0
unsigned char   CanQuitSM(unsigned char Ch)
{
	REG_CANMOD regmod,Temp;	
	Temp.DWord = LPC2400REG(Ch,MOD);									/*	读取CANMOD寄存器  备份		 */
	regmod.DWord = LPC2400REG(Ch,MOD);									/*	读取CANMOD寄存器			 */
	regmod.Bits.RM = 1;											 		/*	RM置位						 */
	LPC2400REG(Ch,MOD) = regmod.DWord;									/*	回写CANMOD寄存器			 */
	regmod.Bits.SM = 0;													/*	SM唤醒						 */
	LPC2400REG(Ch,MOD) = regmod.DWord;									/*	回写CANMOD寄存器			 */
	regmod.Bits.RM = Temp.Bits.RM;										/*	RM还原						 */
	LPC2400REG(Ch,MOD) = regmod.DWord;									/*	回写CANMOD寄存器			 */
	regmod.DWord = LPC2400REG(Ch,MOD);									/*	验证写入值					 */	
	return (unsigned char)((0 != regmod.Bits.SM)? 1:0); 
}
#endif
/* description: can initial
//    function: unsigned char CAN_Init(unsigned char Ch, unsigned long Baud);
//			@param: Ch:CAN1,CAN2
//				    :	Baud:CAN baudrate
//return value:	void	
//CAN控制器退出软件复位模式
*/
void CAN_Init(unsigned char Ch, unsigned long Baud)
{
	HwEnCAN(Ch);
	SwRstCAN(Ch);
	LPC2400REG(Ch,BTR) = Baud;
	CAN_AFMR &= ~0x07;													/*	设置为旁路模式				 */
	CAN_AFMR|=(1<<1); 
	SwEnCAN(Ch);
}
/* description: can send command
//    function: error_code CanSendCmd(unsigned char Ch,unsigned char Cmd,unsigned char BUFNum)
//			@param: Ch:CAN1,CAN2
//					    Cmd:NOM_SEND_CMD,SIG_SEND_CMD,SLF_SEND_CMD,SSR_SEND_CMD,REL_REC_BUF,CLR_DATA_OVER
//					    BUFNum:BUFNum1,BUFNum2,BUFNum3,BUFNumAll
//return value:	void	
*/
error_code	CanCmd(unsigned char Ch,unsigned char Cmd,unsigned char BUFNum)
{
	REG_CANCMR Temp;
	REG_CANMOD *canmod;
	
  //read can general state registor,check whether the bus is sendbusy
	if(LPC2400REG(Ch,GSR)&(0x11<<4)) {return (sent_busy_err);}

	//point to CANMOD registor
	canmod =(REG_CANMOD *)LPC2400ADD(Ch,MOD);
  
  Temp.DWord=0x00;
  //select buffer to sent
  Temp.DWord|=BUFNum;

	switch(Cmd)
	{  
		case SIG_SEND_CMD:											//single time send command
			Temp.Bits.AT =1;											
			Temp.Bits.TR =1;
			break;
		case SLF_SEND_CMD:
		case SSR_SEND_CMD:
			if(canmod->Bits.STM == 0)							//STM=0 normal mode,STM=1 test mode
			{
				canmod->Bits.RM =1;
				canmod->Bits.STM =1;
				canmod->Bits.RM =0;
			}
			Temp.Bits.SRR =1;
			if(Cmd == 3)													//single time selfsend selfrecive
			{
				Temp.Bits.AT =1;
			}
			break;
		case REL_REC_BUF:
		    Temp.Bits.RRB =1;                   //release receive buffer
		    break;
		case CLR_DATA_OVER: 
		    Temp.Bits.CDO =1;										//clear data overflow
		    break;
		case NOM_SEND_CMD:
		default:									     			    //normal send
			Temp.Bits.TR =1;
			break;
	}
	LPC2400REG(Ch,CMR) = Temp.DWord;          //write cancmd registor
	
	if(LPC2400REG(Ch,GSR)&(1<<3)) {						//read can general state registor
		return(sent_finished);
	} else { return (read_wait_err);}
}
/* description:	write can Txbuf 
//		function:	error_code WriteCanTxBuf(unsigned char Ch,unsigned char prio,unsigned char TPM,CANFRAME *frame);
//		  @param:	Ch:CAN1,CAN2
//							prio  CANMOD.TPM 置位时，缓冲器根据该字段竞争发送优先权 值小优先级高
//return value:	error_code see error.h
*/
error_code WriteCanTxBuf(unsigned char Ch,unsigned char *BUFNum,unsigned char prio,CANFRAME *frame)
{ 
	REG_CANTFI FrameInfo;
	REG_CANSR  regcansr;
	unsigned char TFI=TFI1,TID=TID1,TDA=TDA1,TDB=TDB1;
	unsigned char i;

	FrameInfo.DWord=0;
	//check and write frame information
	if(frame->LEN>8) {
		return(frame_len_err);
	} else {
		FrameInfo.Bits.FF=frame->FF;
		FrameInfo.Bits.RTR=frame->RTR;
		FrameInfo.Bits.DLC=frame->LEN;
		FrameInfo.Bits.PRIO=prio;
		//if prio!=0,use prio to judge priority,else use can-id to judge
    if(prio){LPC2400REG(Ch, MOD)|=(1<<3);}
    else{LPC2400REG(Ch, MOD)&=~(1<<3);}			
	}

	//check the tx buffer,find a idle one
	regcansr.DWord=LPC2400REG(Ch, SR);   //get can state information
  if((0== regcansr.Bits.TS1)&&(0 != regcansr.Bits.TBS1)){
			*BUFNum=BUFNum1;
			TFI=TFI1;TID=TID1;TDA=TDA1;TDB=TDB1;
    }
		else if((0== regcansr.Bits.TS2)&&(0 !=regcansr.Bits.TBS2)){
			*BUFNum=BUFNum2;
			TFI=TFI2;TID=TID2;TDA=TDA2;TDB=TDB2;
		}
		else if((0== regcansr.Bits.TS3)&&(0 != regcansr.Bits.TBS3)){
			*BUFNum=BUFNum3;
			TFI=TFI3;TID=TID3;TDA=TDA3;TDB=TDB3;
		}
		else{
		return (buffer_full_err);}


	  //write frame information into buffer
		LPC2400REG(Ch,TFI)=FrameInfo.DWord;
		
		//check id and write it into buffer
		if(frame->FF) {							//if FF==1,id is 29bits
		        LPC2400REG(Ch,TID) &= 0xE0000000;
		        LPC2400REG(Ch,TID) |= (unsigned long)(frame->ID & 0x1fffffff);
	            } 
	            else {            //if FF==0,id is 11bits
		        LPC2400REG(Ch,TID) &= 0xfffff800;								
		        LPC2400REG(Ch,TID)|= (unsigned long)(frame->ID & 0x000007ff);
	           }	

		//write data into buffer				 
		LPC2400REG(Ch,TDA)=0;
		LPC2400REG(Ch,TDB)=0;
		for(i=0;i<4;i++){
					LPC2400REG(Ch,TDA)|=(frame->DATA[i]<<(i<<3));
			    LPC2400REG(Ch,TDB)|=(frame->DATA[i+4]<<(i<<3)); 
		}

	return (CanCmd(Ch,NOM_SEND_CMD,*BUFNum));
}
/* description:	read can Rxbuf 
//		function: void	ReadCanRxBuf(unsigned char Ch,P_strRxBUF Buf)
//			@param: Ch:CAN1,CAN2 
//return value:	void
*/
void ReadCanRxBuf(unsigned char Ch,P_strRxBUF Buf)
{
	Buf->CANRcvFS.DWord 	=LPC2400REG(Ch,RFS);
	Buf->CANID 				    =LPC2400REG(Ch,RID);
	Buf->DATAA.DWord		  =LPC2400REG(Ch,RDA);
	Buf->DATAB.DWord	  	=LPC2400REG(Ch,RDB);
}

/* description:	error handler
//		function: error_code	Xmit(unsigned char *err,CANFRAME *frame);
//			@param: error_code 
//return value:	error_code
*/
error_code	Xmit(unsigned char Ch,unsigned char *err,unsigned char prio,CANFRAME *frame)
{ 
	static unsigned char BUFNum;
	unsigned char to;
	BUFNum=0x00;
	
	*err=WriteCanTxBuf(Ch,&BUFNum,prio,frame);
	while(1){
				//if buffer is full try to write several times,if failed echo buf_timeout_err
				if(*err==buffer_full_err){
					for(to=0;to<20;to++){
					CanCmd(Ch,NOM_SEND_CMD,BUFNum);	//2014/04/22 debug
					*err=WriteCanTxBuf(Ch,&BUFNum,prio,frame);
					if(*err!=buffer_full_err){break;}		
							}
					if(to>=20){*err=buf_timeout_err;}
					continue;
					}
				//if sent is busy,try sevsral times,if failed echo sent_timeout_err
				if(*err==sent_busy_err){
					for(to=0;to<100;to++){
					*err=CanCmd(Ch,NOM_SEND_CMD,BUFNum);
					if(*err!=sent_busy_err){break;}		
							}
					if(to>=100){*err=sent_timeout_err;}
					continue;
					}

					break;//echo any error haven't handled
				}

			return *err;
}

/* description:	 
//		function: void
//			@param: 
//return value:	
*/
error_code WaitForTransfer(unsigned char Ch,unsigned char size,unsigned int timeout,CANFRAME *frame,unsigned char *data)
{ 
		unsigned int to,i,j;
	  unsigned char err;
	  REG_CANDF recdata;
	  
	  for(to=0;to<timeout;to++){
			 //receive a frame
			 if(LPC2400REG(Ch,GSR)&1){
			   err=none_err;
				 
// 				 //check frame info
// 				 if((LPC2400REG(Ch,RFS)&0xC0000000)!=0){err=bad_recformat_err;}
// 				 
// 				 //check id
// 				 if(LPC2400REG(Ch,RID)!=0x00000581){err=bad_recid_err;}
				 
				 //check index and sub
				  recdata.DWord=LPC2400REG(Ch,RDA);
					if(((unsigned char)(recdata.DWord))!=0x42)
						  {
							err=bad_rechead_err;
							}
					else{
								for(j=1;j<4;j++)
								{
								if(frame->DATA[j]!=((unsigned char)(recdata.DWord>>(j*8))))
									{
									 err=bad_recindex_err;
									 break;	//break from this loop
									}
								}
							}	
				//if the frame is wanted,then get the right data							
				if(err==none_err)
					{
						recdata.DWord=LPC2400REG(Ch,RDB);					
						for(j=0;j<size;j++)
						{
					  data[j]=((unsigned char)(recdata.DWord>>(j*8)));
						}
					}
				//no matter whether error happened,released the buffer
			    CanCmd(Ch,REL_REC_BUF,0);

				//break form the for-loop before timeout
					if(err==none_err){break;}
			 }
			else{
				//software delay for some time,big i long delay maybe 1ms
				 for(i=10; i > 0; i--){for(j = 0; j < 5000; j++);}
			    }
		}
    if(to>=timeout){err=rdbk_timeout_err;}

    return err;
}
