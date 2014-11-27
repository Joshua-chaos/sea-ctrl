/*	
						ZheJiang University
			Control Science & Engineering(CSE)
							file information
	file name:lpc2400canreg.h
	version:v1.0
	date:2012/10/11  y/m/d
	author：jiaxuechao
*/
#ifndef _LPC2400CANREG_H_
#define _LPC2400CANREG_H_
//for c++ compatible
#ifdef  __cplusplus
extern "C" {
#endif
//head file
#include "lpc2400.h"

#define LPC2400REG(CH,OFFSET) (*((volatile unsigned long*)(CAN1_BASE_ADDR+(CH<<14)+(OFFSET<<2))))
#define LPC2400ADD(CH,OFFSET) (CAN1_BASE_ADDR + (CH<<14) + (OFFSET<<2))
enum LPC2400CANCTRLOR_OFFSET {MOD=0,CMR,GSR,ICR,IER,BTR,EWL,SR,RFS,RID,RDA,RDB,
														TFI1,TID1,TDA1,TDB1,TFI2,TID2,TDA2,TDB2,TFI3,TID3,TDA3,TDB3};

/*
//   定义 CANMOD 寄存器:
1.This register controls the basic operating mode of the CAN Controller. 
2.Bits not listed read as 0 and should be written as 0.
*/
typedef	union		_canmod_
{
	unsigned long DWord;
	struct	{
		unsigned int	RM 		:1;		//定义RM位(复位位)
		unsigned int	LOM 	:1;		//定义LOM位(只听模式位)

		unsigned int	STM		:1;		//定义STM位 "0"发送的信息必须被应答才被认可
		//"1"自测试模式,CANCMR的SRR位一起使用，可以达到自发自收的效果

		unsigned int	TPM 	:1;		//定义TPM位 "0"3个发送缓冲区的优先级由各自的CAN ID决定
		//"1"3个发送缓冲区的优先级由各自的Tx优先级域决定

		unsigned int	SM 		:1;		//定义SM位(睡眠模式位)

		unsigned int	RPM 	:1;		//定义RPM位 "0"如果传输的是显性位，Rx和Tx脚为低电平
		//"1"翻转极性模式--如果传输的是显性位，Rx脚为高电平

		unsigned int	RSV1 	:1;		//保留位
		unsigned int	TM 		:1;		//定义TM位(测试模式)
		unsigned int	RSV24	:24;	//保留位
	}Bits;
}REG_CANMOD;
/*
//   定义 CANCMR 寄存器:
1.Writing to this write-only register initiates an action. 
2.Bits not listed should be written as 0. Reading this register yields zeroes.
*/
typedef	union		_cancmr_
{
	unsigned long	DWord;
	struct	
	{
		unsigned int	TR 		:1;		//定义TR位(发送请求)
		unsigned int	AT 		:1;		//定义AT位(发送中止)
		unsigned int	RRB 	:1;		//定义RRB位(接收缓冲区释放)
		unsigned int	CDO 	:1;		//清除数据溢出位
		unsigned int	SRR 	:1;		//定义SRR位(自接收请求)
		unsigned int	STB1 	:1;		//定义SEND TxBUF1位(选择BUF1发送)
		unsigned int	STB2 	:1;		//定义SEND TxBUF2位(选择BUF2发送)
		unsigned int	STB3 	:1;		//定义SEND TxBUF3位(选择BUF3发送)
		unsigned int	RSV24	:24;	//保留位
	}Bits;
}REG_CANCMR;
/*
//   定义 CANGSR 寄存器:
//   全局状态
*/
typedef	union		_cangsr_
{
	unsigned long	DWord;				//字操作定义
	struct	
	{
		unsigned int	RBS 	 :1;	//接收缓冲区状态位
		unsigned int	DOS 	 :1; 	//数据溢出状态位
		unsigned int	TBS 	 :1;	//发送缓冲区状态锁定位
		unsigned int	TCS 	 :1;	//发送完成状态位
		unsigned int	RS 		 :1;	//正在接收状态
		unsigned int	TS 		 :1;	//正在发送状态
		unsigned int	ES	 	 :1;	//错误状态
		unsigned int	BS 		 :1;	//总线关闭
		unsigned int	RSV8	 :8;	//保留位
		unsigned int	RXERR 	 :8;	//接收错误计数
		unsigned int	TXERR 	 :8;	//发送错误计数
	}Bits;
}REG_CANGSR;
/*
//   定义 CANICR 寄存器:
//   中断捕获
*/
typedef	union		_canicr_
{
	unsigned long	DWord;				//字操作定义
	struct	
	{
		unsigned int	RI 		:1;	    //接收中断位
		unsigned int	TI1 	:1;	    //TxBUF1发送成功中断位
		unsigned int	EI 		:1;	    //错误报警中断位
		unsigned int	DOI 	:1;		//数据溢出中断位
		unsigned int	WUI 	:1;		//睡眠唤醒中断位
		unsigned int	EPI 	:1;		//错误认可中断位
		unsigned int	ALI 	:1;		//仲裁错误中断位
		unsigned int	BEI 	:1;		//总线错误中断
		unsigned int	IDI 	:1;		//接收到CAN标识符中断位
		unsigned int	TI2 	:1;		// TxBUF2发送成功中断位
		unsigned int	TI3 	:1;		// TxBUF3发送成功中断位
		unsigned int	RSV5	:5;		//保留位
		unsigned int	ERRBIT 	:5;		//错误代码捕获
		unsigned int	ERRDIR 	:1;		//错误方向
		unsigned int	ERRC 	:2;		//错误类型
		unsigned int	ALCBIT 	:8;		//仲裁错误代码捕获
	}Bits;
}REG_CANICR;
/*
//   定义 CANIER 寄存器:
//   中断使能
*/
typedef	union		_canier_
{
	unsigned long    DWord;				//字操作定义
	struct	
	{
		unsigned int	RIE 	:1;		//接收中断使能位
		unsigned int	TIE1 	:1;		//TxBUF1发送完成中断使能位
		unsigned int	EIE 	:1;		//错误报警中断使能位
		unsigned int	DOIE 	:1;		//数据溢出中断使能位
		unsigned int	WUIE 	:1;		//睡眠模式唤醒中断使能位
		unsigned int	EPIE 	:1;		//错误认可中断使能位
		unsigned int	ALIE 	:1;		//仲裁丢失中断使能位
		unsigned int	BEIE 	:1;		//总线错误中断使能位
		unsigned int	IDIE	:1;		//接收到CAN标识符中断使能位
		unsigned int	TIE2 	:1;		//TxBUF2发送完成中断使能位
		unsigned int	TIE3	:1;		//TxBUF3发送完成中断使能位
		unsigned int	RSV21	:21;	//保留位
	}Bits;
}REG_CANIER;
/*
//   定义 CANBTR 寄存器:
1.This register controls how various CAN timings are derived from the VPB clock.
2.It can be read at any time, but can only be written if the RM bit in CANmod is 1.
*/
typedef	union		_canbtr_
{
	unsigned long	DWord;				//字操作定义
	struct	
	{
		unsigned int	BRP 	:10;	//预分频位组合定义
		unsigned int	RSV4	:4;		//保留位
		unsigned int	SJW 	:2;		//同步跳转宽度
		unsigned int	TSEG1 	:4;		//时间段1
		unsigned int	TSEG2 	:3;		//时间段2
		unsigned int	SAM 	:1;		//采样模式位
		unsigned int	RSV8	:8;		//保留
	}Bits;
}REG_CANBTR;
/*
//   定义 CANEWL 寄存器:
//   出错警告界限
*/
typedef	union	_canewl_
{
	unsigned long	DWord;				//字操作定义
	struct	
	{
		unsigned int	EWL 	:8;		///出错警告界限值
		unsigned int	RSV24	:24;	//保留位
	}Bits;
}REG_CANEWL;
/*
//   定义 CANSR 寄存器:
//   状态寄存器
*/
typedef	union	_cansr_
{
	unsigned long	DWord;				//字操作定义
	struct	
	{
		unsigned int	RBS1 	:1;		//接收缓冲区有效,与CANxGRS的RBS相同
		unsigned int	DOS1 	:1;		//数据溢出,与CANxGRS的DOS相同
		unsigned int	TBS1 	:1;		//  TxBUF1锁定
		unsigned int	TCS1 	:1;		//  TxBUF1的数据发送完成
		unsigned int	RS1 	:1;		//正在接收,与CANxGRS的RS相同
		unsigned int	TS1 	:1;		//  TxBUF1的数据正在发送
		unsigned int	ES1 	:1;		//错误认可,与CANxGRS的ES相同
		unsigned int	BS1 	:1;		//总线错误,与CANxGRS的BS相同
		unsigned int	RBS2 	:1;		//接收缓冲区有效,与CANxGRS的RBS相同
		unsigned int	DOS2 	:1;		//数据溢出,与CANxGRS的DOS相同
		unsigned int	TBS2 	:1;		//  TxBUF2锁定
		unsigned int	TCS2 	:1;		//  TxBUF2的数据发送完成
		unsigned int	RS2 	:1;		//正在接收,与CANxGRS的RS相同
		unsigned int	TS2 	:1;		//  TxBUF2的数据正在发送
		unsigned int	ES2 	:1;		//错误认可,与CANxGRS的ES相同
		unsigned int	BS2 	:1;		//总线错误,与CANxGRS的BS相同
		unsigned int	RBS3 	:1;		//接收缓冲区有效,与CANxGRS的RBS相同
		unsigned int	DOS3 	:1;		//数据溢出,与CANxGRS的DOS相同
		unsigned int	TBS3 	:1;		//  TxBUF3锁定
		unsigned int	TCS3 	:1;		//  TxBUF3的数据发送完成
		unsigned int	RS3 	:1;		//正在接收,与CANxGRS的RS相同
		unsigned int	TS3 	:1;		//  TxBUF3的数据正在发送
		unsigned int	ES3 	:1;		//错误认可,与CANxGRS的ES相同
		unsigned int	BS3 	:1;		//总线错误,与CANxGRS的BS相同
		unsigned int	RSV8	:8;		//保留
	}Bits;
}REG_CANSR;
/*
//  定义 CANRFS 寄存器:
1.This register defines the characteristics of the current received message. 
2.It is read-only in normal operation, but can be written for testing purposes if the RM bit in CANMOD is 1.
*/
typedef	union		_canrfs_
{
	unsigned long		DWord;			//字操作定义
	struct	
	{
		unsigned int	IDIN 	:10;	//ID索引值
		unsigned int	BP 		:1;		//BP
		unsigned int	RSV5 	:5;		//保留
		unsigned int	DLC 	:4;		//数据长度
		unsigned int	RSV10	:10;	//保留
		unsigned int	RTR 	:1;		//远程帧、数据帧识别位
		unsigned int	FF 		:1;		//扩展帧、标准帧识别位
	}Bits;
}REG_CANRFS;
/*
//   定义 CANID 寄存器:
//   标准帧ID结构
*/
typedef	union		_canstdid_
{
	unsigned long	   DWord;					//字操作定义
		struct		        					//标准帧结构
		{
			unsigned int	ID 		:11;
			unsigned int	RSV21	:21;
		}Bits;
}REG_CANSTDID;
/*
//   定义 CANID 寄存器:
//   扩展帧结构
*/
typedef	union		_canextid_
{
	unsigned long	   DWord;					//字操作定义
		struct		 				            //扩展帧结构
		{
			unsigned int	ID 		:29;
			unsigned int	RSV3 	:3;
		}Bits;
}REG_CANEXTID;
/*
//   定义 CANDF 寄存器:
*/
typedef	union		_candf_
{
	unsigned long		DWord;			//字操作定义
	struct	
	{
		unsigned int	Data1 	:8;		//数据字节
		unsigned int	Data2 	:8;	
		unsigned int	Data3 	:8;	
		unsigned int	Data4 	:8;	
	}Bits;
}REG_CANDF;
/*
//   定义 CANTFI 寄存器:
*/
typedef	union		_cantfi_
{
	unsigned long	   DWord;			//字节操作定义
	struct	
	{
		unsigned int	PRIO 	:8;		//缓冲区数据发送优先级的值
		unsigned int	RSV8	:8;						
		unsigned int	DLC 	:4;		//发送数据长度
		unsigned int	RSV10   :10;
		unsigned int	RTR 	:1;		//远程帧、数据帧辨别位
		unsigned int	FF   	:1;		//扩展帧、标准帧辨别位
	}Bits;
}REG_CANTFI;

/*
//   定义 CANTxSR 寄存器:
//   CAN集中发送状态寄存器结构
*/
typedef	union		_CANTxSR_
{
	unsigned long		DWord;				//字操作定义
	struct	
	{
		unsigned int	TS1 		:1;		//CAN1正在发送状态
		unsigned int	TS2 		:1;		//CAN2正在发送状态
		unsigned int	RSV6a		:6;		//保留
		unsigned int	TBS1 		:1;		//CAN1发送缓冲区锁定状态
		unsigned int	TBS2 		:1;		//CAN2发送缓冲区锁定状态
		unsigned int	RSV6b    	:6;		// 
		unsigned int	TCS1 		:1;		//CAN1发送完成状态
		unsigned int	TCS2 	 	:1;		//CAN2发送完成状态
		unsigned int	RSV14		:14;	//保留
	}Bits;
}REG_CANTxSR;
/*
//   定义 CANRxSR 寄存器:
//   CAN集中接收状态寄存器结构
*/
typedef	union		_CANRxSR_
{
	unsigned long		DWord;				//字操作定义
	struct	
	{
		unsigned int	RS1 		:1;		//CAN1正在接收状态
		unsigned int	RS2		   	:1;		//CAN2正在接收状态
		unsigned int	RSV6a		:6;		//保留
		unsigned int	RBS1	 	:1;		//CAN1接收缓冲区有效
		unsigned int	RBS2	 	:1;		//CAN2接收缓冲区有效
		unsigned int	RSV6b	 	:6;		// 
		unsigned int	DOS1	 	:1;		//CAN1接收缓冲区溢出
		unsigned int	DOS2	 	:1;		//CAN2接收缓冲区溢出
		unsigned int	RSV14		:14;	//保留
	}Bits;
}REG_CANRxSR;
/*
//   定义 CANMSR 寄存器:
//   CAN集中其他状态寄存器结构
*/
typedef	union		_CANMSR_
{
	unsigned long		DWord;				//字操作定义
	struct	
	{
		unsigned int	ES1 		:1;		//CAN1错误报警
		unsigned int	ES2 		:1;		//CAN2错误报警
		unsigned int	RSV6		:6;		//保留
		unsigned int	BS1 		:1;		//CAN1总线脱离
		unsigned int	BS2	 		:1;		//CAN2总线脱离
		unsigned int	RSV22		:22;	//保留
	}Bits;
}REG_CANMSR;
/*
//  定义 CANAFMR 寄存器: 
//  CAN验收滤波模式寄存器结构
*/
typedef	union		_canafmr_
{
	unsigned long		DWord;				//字操作定义
	struct	
	{
		unsigned int	AccOff	 	:1;	
		unsigned int	AccBP 		:1;									
		unsigned int	eFCAN	 	:1;	
		unsigned int	RSV29	 	:29;
	}Bits;
}REG_CANAFMR;
/*
//   验收滤波器表格单个标志符表格起始地址数据结构定义
//
*/
typedef	union	_canF_sa_
{
	unsigned long		DWord;				//字操作定义
	struct	
	{
		unsigned int	RSV2 	:2;
		unsigned int	SADR 	:9;	
		unsigned int	RSV21 	:21;
	}Bits;
}REG_CANF_sa;
/*
//   验收滤波器表格标志符组表格起始地址数据结构定义
*/
typedef	union	_canGRP_sa_
{
	unsigned long		DWord;				//字操作定义
	struct	
	{
		unsigned int	RSV2 	:2;
		unsigned int	SADR 	:10;
		unsigned int	RSV20 	:20;
	}Bits;
}REG_CANGRP_sa;
/*
//   LUT错误寄存器数据结构定义
*/
typedef	union	_canLUTerr
{
	unsigned long		DWord;				//字操作定义
	struct	
	{
		unsigned int	LERR 	:1;
		unsigned int	RSV31 	:31;
	}Bits;
}REG_CANLUTerr;
/*
//   定义 CAN 报文发送项 结构体:
*/
typedef struct _strTxBUF_
{   
  REG_CANTFI  FrameInfo;          // 是否标准帧
	unsigned int CANID;				// 长度与FF有关
	REG_CANDF   DATAA;
	REG_CANDF   DATAB;
}strTxBUF,*P_strTxBUF;
/*
//   定义 CAN 报文接收项 结构体:
*/
typedef struct _strRxBUF_
{   
  REG_CANRFS  CANRcvFS;          // 是否标准帧
	unsigned int CANID;				// 长度与FF有关
	REG_CANDF   DATAA;
	REG_CANDF   DATAB;
}strRxBUF,*P_strRxBUF;
/*
// 	CAN控制器结构体定义
*/
typedef struct _CANCONTROLLOR
{
	unsigned char	m_Channel;		//CAN通道
	//unsigned char	m_bIsLOM;		//只听模式标志
	//unsigned char	m_bSLPM;		//睡眠模式标志
	unsigned long	m_SysClk;		//系统时钟
	unsigned char	m_Status;		//状态
	
	/*INT8U	m_EWL;			//出错警告值
	INT32U	m_CAN_Baud;		//CAN波特率值
	INT32U	m_RegBaseAddr;	//CAN控制器的起始基址
	REG_CANRxBUF *m_RxBufAddr;	//接收缓冲区首地址指针
	INT16U	m_iCAN_RxBufLen;//接收缓冲区数量长度	
	INT16U	m_iRdPoint;		//接收缓冲区读入口
	INT16U	m_iWrPiont;		//接收缓冲区写入口
	INT16U	m_iDataCout;	//接收缓冲区有效数据数目
	BOOL	m_bIsFull;		//接收缓冲区满标志
	BOOL	m_IsSendOK;		//发送成功标志
	void	*pSendSem;		//发送信号量
	void	*pRecvSem;		//接收信号量
	void	*pRcvMutexSem;	//接收独占信号量
	void	*pSndMutexSem;	//发送独占信号量*/
}CANCONTROLLOR;


#ifdef __cplusplus
      }
#endif
#endif
