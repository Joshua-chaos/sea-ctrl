/*	
						ZheJiang University
			Control Science & Engineering(CSE)
							file information
	file name:lpc2400can.h
	version:v1.0
	date:2012/10/11  y/m/d
	modified:2013/4/12
	last modified date:2013/10/23
	description:fix a fatal bug in HwEnCan();
							add Ch para to Xmit() and WaitForTransfer()
	author：jiaxuechao
*/
#ifndef _LPC2400CAN_H_
#define _LPC2400CAN_H_
//for c++ compatible
#ifdef  __cplusplus
extern "C" {
#endif
//head files
#include "error.h"
#include "lpc2400canreg.h"
//precompile control define
#define SLEEPEN	  	   0
//can channel and buffer num define
#define	BUFNum1	 		0x20
#define	BUFNum2	 		0x40
#define	BUFNum3	 		0x80
#define	CAN1	 0
#define	CAN2	 1


//Fpclk=24MHz CAN baudrate define                      
#define BPS_1000K	0x140002
#define BPS_800K	0x160002
#define BPS_500K	0x1C0002
#define BPS_250K	0x1C0005
#define BPS_125K	0x1C000B
#define BPS_100K	0x1C000E
#define BPS_50K		0x1C001D
#define BPS_20K		0x1C004A
#define BPS_10K		0x1C0095

//can frame info define
#define CAN_FRAME_STD 0
#define CAN_FRAME_EXT 1
#define CAN_FRAME_DATA 0
#define CAN_FRAME_REM 1

//can command define
#define	NOM_SEND_CMD		0		//normal send
#define	SIG_SEND_CMD		1		//single time send command
#define	SLF_SEND_CMD		2		//selfsend selfreceive
#define	SSR_SEND_CMD		3		//single time selfsend selfrecive
#define	REL_REC_BUF 		4		//release receive buffer
#define	CLR_DATA_OVER 	5		//clear data overflow

//can frame structure define
typedef struct _CANFRMAE
{
	unsigned long ID;		//帧ID
	unsigned char LEN;		//数据长度
	unsigned char RTR;		//远程帧标志
	unsigned char FF;		//扩展帧标志
	unsigned char DATA[8];	//帧数据
}CANFRAME;

/* description: use for can hardware reset
//    function: void	HwRstCAN (unsigned char Ch)
//			@param: Ch:CAN1,CAN2 
//return value:	void
*/
void	HwRstCAN(unsigned char Ch);

/* description: use for can hardware enable
//    function: void	HwEnCAN (unsigned char Ch)
//			@param: Ch:CAN1,CAN2 
//return value:	void
*/
void	HwEnCAN(unsigned char Ch);

/* description: use for can software reset
//    function: unsigned char SwRstCAN(unsigned char Ch);
//			@param: Ch:CAN1,CAN2
//return value:	0(succeed) 1(error) 
*/
unsigned char SwRstCAN(unsigned char Ch);

/* description: use for can quit software reset
//    function: unsigned char SwEnCAN(unsigned char Ch);
//			@param: Ch:CAN1,CAN2
//return value:	0(succeed) 1(error) 
*/
unsigned char SwEnCAN(unsigned char Ch);

/* description: use for can enter sleep mode
//    function: unsigned char CanEntrySM(unsigned char Ch)
//			@param: Ch:CAN1,CAN2
//return value:	0(succeed) 1(error) 
*/
#if SLEEPEN>0
unsigned char	CanEntrySM(unsigned char Ch);
#endif

/* description: use for can quit sleep mode
//    function: unsigned char CanQuitSM(unsigned char Ch)
//			@param: Ch:CAN1,CAN2
//return value:	0(succeed) 1(error) 
*/
#if SLEEPEN>0
unsigned char   CanQuitSM(unsigned char Ch);
#endif

/* description: can initial
//    function: unsigned char CAN_Init(unsigned char Ch, unsigned long Baud);
//			@param: Ch:CAN1,CAN2
//				    :	Baud:CAN baudrate
//return value:	void	
//CAN控制器退出软件复位模式
*/
void CAN_Init(unsigned char Ch, unsigned long Baud);

/* description: can send command
//    function: error_code	CanSendCmd(unsigned char Ch,unsigned char Cmd,unsigned char BUFNum)
//			@param: Ch:CAN1,CAN2
//					    Cmd:NOM_SEND_CMD,SIG_SEND_CMD,SLF_SEND_CMD,SSR_SEND_CMD,REL_REC_BUF,CLR_DATA_OVER
//					    BUFNum:BUFNum1,BUFNum2,BUFNum3,BUFNumAll
//return value: void	
*/
error_code	CanCmd(unsigned char Ch,unsigned char Cmd,unsigned char BUFNum);

/*
***********************************************************************************************************
**函数原型	    :  	宏定义函数 CanBufOffLinePrg(Ch)
**参数说明		:  	Ch		CAN通道号 0表示CAN1，1表示CAN2
**返回值		:	无	
**说	明		:	本函数用于CAN控制器出现总线关闭错误处理
***********************************************************************************************************
*/
/*
#define	CanBufOffLinePrg(Ch)	if(CANMOD(Ch).Bits.RM != 0)		\
									{							\			
										CANGSR(Ch).DWord=0;		\		
										CANMOD(Ch).Bits.RM=0x00;\	
										CANCMR(Ch).DWord=0x02;	\		
									}	
*/

/* description: write can Txbuf 
//		function: error_code WriteCanTxBuf(unsigned char Ch,unsigned char prio,unsigned char TPM,CANFRAME *frame);
//		  @param:	Ch:CAN1,CAN2
//							prio  CANMOD.TPM 置位时，缓冲器根据该字段竞争发送优先权 值小优先级高
//return value:	error_code see error.h  
*/
error_code WriteCanTxBuf(unsigned char Ch,unsigned char * BUFNum,unsigned char prio,CANFRAME *frame);

/* description:	read can Rxbuf 
//		function: void	ReadCanRxBuf(unsigned char Ch,P_strRxBUF Buf)
//			@param: Ch:CAN1,CAN2 
//return value:	void
*/
void ReadCanRxBuf(unsigned char Ch, P_strRxBUF Buf);

/* description:	error handler
//		function: error_code	Xmit(unsigned char *err,CANFRAME *frame);
//			@param: error_code 
//return value:	error_code
*/
error_code	Xmit(unsigned char Ch,unsigned char *err,unsigned char prio,CANFRAME *frame);

/* description:	 
//		function: void
//			@param: 
//return value:	
*/
error_code WaitForTransfer(unsigned char Ch,unsigned char size,unsigned int timeout,CANFRAME *frame,unsigned char *data);

#ifdef __cplusplus
    }
#endif
#endif
