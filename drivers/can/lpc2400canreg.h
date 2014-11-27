/*	
						ZheJiang University
			Control Science & Engineering(CSE)
							file information
	file name:lpc2400canreg.h
	version:v1.0
	date:2012/10/11  y/m/d
	author��jiaxuechao
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
//   ���� CANMOD �Ĵ���:
1.This register controls the basic operating mode of the CAN Controller. 
2.Bits not listed read as 0 and should be written as 0.
*/
typedef	union		_canmod_
{
	unsigned long DWord;
	struct	{
		unsigned int	RM 		:1;		//����RMλ(��λλ)
		unsigned int	LOM 	:1;		//����LOMλ(ֻ��ģʽλ)

		unsigned int	STM		:1;		//����STMλ "0"���͵���Ϣ���뱻Ӧ��ű��Ͽ�
		//"1"�Բ���ģʽ,CANCMR��SRRλһ��ʹ�ã����Դﵽ�Է����յ�Ч��

		unsigned int	TPM 	:1;		//����TPMλ "0"3�����ͻ����������ȼ��ɸ��Ե�CAN ID����
		//"1"3�����ͻ����������ȼ��ɸ��Ե�Tx���ȼ������

		unsigned int	SM 		:1;		//����SMλ(˯��ģʽλ)

		unsigned int	RPM 	:1;		//����RPMλ "0"��������������λ��Rx��Tx��Ϊ�͵�ƽ
		//"1"��ת����ģʽ--��������������λ��Rx��Ϊ�ߵ�ƽ

		unsigned int	RSV1 	:1;		//����λ
		unsigned int	TM 		:1;		//����TMλ(����ģʽ)
		unsigned int	RSV24	:24;	//����λ
	}Bits;
}REG_CANMOD;
/*
//   ���� CANCMR �Ĵ���:
1.Writing to this write-only register initiates an action. 
2.Bits not listed should be written as 0. Reading this register yields zeroes.
*/
typedef	union		_cancmr_
{
	unsigned long	DWord;
	struct	
	{
		unsigned int	TR 		:1;		//����TRλ(��������)
		unsigned int	AT 		:1;		//����ATλ(������ֹ)
		unsigned int	RRB 	:1;		//����RRBλ(���ջ������ͷ�)
		unsigned int	CDO 	:1;		//����������λ
		unsigned int	SRR 	:1;		//����SRRλ(�Խ�������)
		unsigned int	STB1 	:1;		//����SEND TxBUF1λ(ѡ��BUF1����)
		unsigned int	STB2 	:1;		//����SEND TxBUF2λ(ѡ��BUF2����)
		unsigned int	STB3 	:1;		//����SEND TxBUF3λ(ѡ��BUF3����)
		unsigned int	RSV24	:24;	//����λ
	}Bits;
}REG_CANCMR;
/*
//   ���� CANGSR �Ĵ���:
//   ȫ��״̬
*/
typedef	union		_cangsr_
{
	unsigned long	DWord;				//�ֲ�������
	struct	
	{
		unsigned int	RBS 	 :1;	//���ջ�����״̬λ
		unsigned int	DOS 	 :1; 	//�������״̬λ
		unsigned int	TBS 	 :1;	//���ͻ�����״̬����λ
		unsigned int	TCS 	 :1;	//�������״̬λ
		unsigned int	RS 		 :1;	//���ڽ���״̬
		unsigned int	TS 		 :1;	//���ڷ���״̬
		unsigned int	ES	 	 :1;	//����״̬
		unsigned int	BS 		 :1;	//���߹ر�
		unsigned int	RSV8	 :8;	//����λ
		unsigned int	RXERR 	 :8;	//���մ������
		unsigned int	TXERR 	 :8;	//���ʹ������
	}Bits;
}REG_CANGSR;
/*
//   ���� CANICR �Ĵ���:
//   �жϲ���
*/
typedef	union		_canicr_
{
	unsigned long	DWord;				//�ֲ�������
	struct	
	{
		unsigned int	RI 		:1;	    //�����ж�λ
		unsigned int	TI1 	:1;	    //TxBUF1���ͳɹ��ж�λ
		unsigned int	EI 		:1;	    //���󱨾��ж�λ
		unsigned int	DOI 	:1;		//��������ж�λ
		unsigned int	WUI 	:1;		//˯�߻����ж�λ
		unsigned int	EPI 	:1;		//�����Ͽ��ж�λ
		unsigned int	ALI 	:1;		//�ٲô����ж�λ
		unsigned int	BEI 	:1;		//���ߴ����ж�
		unsigned int	IDI 	:1;		//���յ�CAN��ʶ���ж�λ
		unsigned int	TI2 	:1;		// TxBUF2���ͳɹ��ж�λ
		unsigned int	TI3 	:1;		// TxBUF3���ͳɹ��ж�λ
		unsigned int	RSV5	:5;		//����λ
		unsigned int	ERRBIT 	:5;		//������벶��
		unsigned int	ERRDIR 	:1;		//������
		unsigned int	ERRC 	:2;		//��������
		unsigned int	ALCBIT 	:8;		//�ٲô�����벶��
	}Bits;
}REG_CANICR;
/*
//   ���� CANIER �Ĵ���:
//   �ж�ʹ��
*/
typedef	union		_canier_
{
	unsigned long    DWord;				//�ֲ�������
	struct	
	{
		unsigned int	RIE 	:1;		//�����ж�ʹ��λ
		unsigned int	TIE1 	:1;		//TxBUF1��������ж�ʹ��λ
		unsigned int	EIE 	:1;		//���󱨾��ж�ʹ��λ
		unsigned int	DOIE 	:1;		//��������ж�ʹ��λ
		unsigned int	WUIE 	:1;		//˯��ģʽ�����ж�ʹ��λ
		unsigned int	EPIE 	:1;		//�����Ͽ��ж�ʹ��λ
		unsigned int	ALIE 	:1;		//�ٲö�ʧ�ж�ʹ��λ
		unsigned int	BEIE 	:1;		//���ߴ����ж�ʹ��λ
		unsigned int	IDIE	:1;		//���յ�CAN��ʶ���ж�ʹ��λ
		unsigned int	TIE2 	:1;		//TxBUF2��������ж�ʹ��λ
		unsigned int	TIE3	:1;		//TxBUF3��������ж�ʹ��λ
		unsigned int	RSV21	:21;	//����λ
	}Bits;
}REG_CANIER;
/*
//   ���� CANBTR �Ĵ���:
1.This register controls how various CAN timings are derived from the VPB clock.
2.It can be read at any time, but can only be written if the RM bit in CANmod is 1.
*/
typedef	union		_canbtr_
{
	unsigned long	DWord;				//�ֲ�������
	struct	
	{
		unsigned int	BRP 	:10;	//Ԥ��Ƶλ��϶���
		unsigned int	RSV4	:4;		//����λ
		unsigned int	SJW 	:2;		//ͬ����ת���
		unsigned int	TSEG1 	:4;		//ʱ���1
		unsigned int	TSEG2 	:3;		//ʱ���2
		unsigned int	SAM 	:1;		//����ģʽλ
		unsigned int	RSV8	:8;		//����
	}Bits;
}REG_CANBTR;
/*
//   ���� CANEWL �Ĵ���:
//   ���������
*/
typedef	union	_canewl_
{
	unsigned long	DWord;				//�ֲ�������
	struct	
	{
		unsigned int	EWL 	:8;		///���������ֵ
		unsigned int	RSV24	:24;	//����λ
	}Bits;
}REG_CANEWL;
/*
//   ���� CANSR �Ĵ���:
//   ״̬�Ĵ���
*/
typedef	union	_cansr_
{
	unsigned long	DWord;				//�ֲ�������
	struct	
	{
		unsigned int	RBS1 	:1;		//���ջ�������Ч,��CANxGRS��RBS��ͬ
		unsigned int	DOS1 	:1;		//�������,��CANxGRS��DOS��ͬ
		unsigned int	TBS1 	:1;		//  TxBUF1����
		unsigned int	TCS1 	:1;		//  TxBUF1�����ݷ������
		unsigned int	RS1 	:1;		//���ڽ���,��CANxGRS��RS��ͬ
		unsigned int	TS1 	:1;		//  TxBUF1���������ڷ���
		unsigned int	ES1 	:1;		//�����Ͽ�,��CANxGRS��ES��ͬ
		unsigned int	BS1 	:1;		//���ߴ���,��CANxGRS��BS��ͬ
		unsigned int	RBS2 	:1;		//���ջ�������Ч,��CANxGRS��RBS��ͬ
		unsigned int	DOS2 	:1;		//�������,��CANxGRS��DOS��ͬ
		unsigned int	TBS2 	:1;		//  TxBUF2����
		unsigned int	TCS2 	:1;		//  TxBUF2�����ݷ������
		unsigned int	RS2 	:1;		//���ڽ���,��CANxGRS��RS��ͬ
		unsigned int	TS2 	:1;		//  TxBUF2���������ڷ���
		unsigned int	ES2 	:1;		//�����Ͽ�,��CANxGRS��ES��ͬ
		unsigned int	BS2 	:1;		//���ߴ���,��CANxGRS��BS��ͬ
		unsigned int	RBS3 	:1;		//���ջ�������Ч,��CANxGRS��RBS��ͬ
		unsigned int	DOS3 	:1;		//�������,��CANxGRS��DOS��ͬ
		unsigned int	TBS3 	:1;		//  TxBUF3����
		unsigned int	TCS3 	:1;		//  TxBUF3�����ݷ������
		unsigned int	RS3 	:1;		//���ڽ���,��CANxGRS��RS��ͬ
		unsigned int	TS3 	:1;		//  TxBUF3���������ڷ���
		unsigned int	ES3 	:1;		//�����Ͽ�,��CANxGRS��ES��ͬ
		unsigned int	BS3 	:1;		//���ߴ���,��CANxGRS��BS��ͬ
		unsigned int	RSV8	:8;		//����
	}Bits;
}REG_CANSR;
/*
//  ���� CANRFS �Ĵ���:
1.This register defines the characteristics of the current received message. 
2.It is read-only in normal operation, but can be written for testing purposes if the RM bit in CANMOD is 1.
*/
typedef	union		_canrfs_
{
	unsigned long		DWord;			//�ֲ�������
	struct	
	{
		unsigned int	IDIN 	:10;	//ID����ֵ
		unsigned int	BP 		:1;		//BP
		unsigned int	RSV5 	:5;		//����
		unsigned int	DLC 	:4;		//���ݳ���
		unsigned int	RSV10	:10;	//����
		unsigned int	RTR 	:1;		//Զ��֡������֡ʶ��λ
		unsigned int	FF 		:1;		//��չ֡����׼֡ʶ��λ
	}Bits;
}REG_CANRFS;
/*
//   ���� CANID �Ĵ���:
//   ��׼֡ID�ṹ
*/
typedef	union		_canstdid_
{
	unsigned long	   DWord;					//�ֲ�������
		struct		        					//��׼֡�ṹ
		{
			unsigned int	ID 		:11;
			unsigned int	RSV21	:21;
		}Bits;
}REG_CANSTDID;
/*
//   ���� CANID �Ĵ���:
//   ��չ֡�ṹ
*/
typedef	union		_canextid_
{
	unsigned long	   DWord;					//�ֲ�������
		struct		 				            //��չ֡�ṹ
		{
			unsigned int	ID 		:29;
			unsigned int	RSV3 	:3;
		}Bits;
}REG_CANEXTID;
/*
//   ���� CANDF �Ĵ���:
*/
typedef	union		_candf_
{
	unsigned long		DWord;			//�ֲ�������
	struct	
	{
		unsigned int	Data1 	:8;		//�����ֽ�
		unsigned int	Data2 	:8;	
		unsigned int	Data3 	:8;	
		unsigned int	Data4 	:8;	
	}Bits;
}REG_CANDF;
/*
//   ���� CANTFI �Ĵ���:
*/
typedef	union		_cantfi_
{
	unsigned long	   DWord;			//�ֽڲ�������
	struct	
	{
		unsigned int	PRIO 	:8;		//���������ݷ������ȼ���ֵ
		unsigned int	RSV8	:8;						
		unsigned int	DLC 	:4;		//�������ݳ���
		unsigned int	RSV10   :10;
		unsigned int	RTR 	:1;		//Զ��֡������֡���λ
		unsigned int	FF   	:1;		//��չ֡����׼֡���λ
	}Bits;
}REG_CANTFI;

/*
//   ���� CANTxSR �Ĵ���:
//   CAN���з���״̬�Ĵ����ṹ
*/
typedef	union		_CANTxSR_
{
	unsigned long		DWord;				//�ֲ�������
	struct	
	{
		unsigned int	TS1 		:1;		//CAN1���ڷ���״̬
		unsigned int	TS2 		:1;		//CAN2���ڷ���״̬
		unsigned int	RSV6a		:6;		//����
		unsigned int	TBS1 		:1;		//CAN1���ͻ���������״̬
		unsigned int	TBS2 		:1;		//CAN2���ͻ���������״̬
		unsigned int	RSV6b    	:6;		// 
		unsigned int	TCS1 		:1;		//CAN1�������״̬
		unsigned int	TCS2 	 	:1;		//CAN2�������״̬
		unsigned int	RSV14		:14;	//����
	}Bits;
}REG_CANTxSR;
/*
//   ���� CANRxSR �Ĵ���:
//   CAN���н���״̬�Ĵ����ṹ
*/
typedef	union		_CANRxSR_
{
	unsigned long		DWord;				//�ֲ�������
	struct	
	{
		unsigned int	RS1 		:1;		//CAN1���ڽ���״̬
		unsigned int	RS2		   	:1;		//CAN2���ڽ���״̬
		unsigned int	RSV6a		:6;		//����
		unsigned int	RBS1	 	:1;		//CAN1���ջ�������Ч
		unsigned int	RBS2	 	:1;		//CAN2���ջ�������Ч
		unsigned int	RSV6b	 	:6;		// 
		unsigned int	DOS1	 	:1;		//CAN1���ջ��������
		unsigned int	DOS2	 	:1;		//CAN2���ջ��������
		unsigned int	RSV14		:14;	//����
	}Bits;
}REG_CANRxSR;
/*
//   ���� CANMSR �Ĵ���:
//   CAN��������״̬�Ĵ����ṹ
*/
typedef	union		_CANMSR_
{
	unsigned long		DWord;				//�ֲ�������
	struct	
	{
		unsigned int	ES1 		:1;		//CAN1���󱨾�
		unsigned int	ES2 		:1;		//CAN2���󱨾�
		unsigned int	RSV6		:6;		//����
		unsigned int	BS1 		:1;		//CAN1��������
		unsigned int	BS2	 		:1;		//CAN2��������
		unsigned int	RSV22		:22;	//����
	}Bits;
}REG_CANMSR;
/*
//  ���� CANAFMR �Ĵ���: 
//  CAN�����˲�ģʽ�Ĵ����ṹ
*/
typedef	union		_canafmr_
{
	unsigned long		DWord;				//�ֲ�������
	struct	
	{
		unsigned int	AccOff	 	:1;	
		unsigned int	AccBP 		:1;									
		unsigned int	eFCAN	 	:1;	
		unsigned int	RSV29	 	:29;
	}Bits;
}REG_CANAFMR;
/*
//   �����˲�����񵥸���־�������ʼ��ַ���ݽṹ����
//
*/
typedef	union	_canF_sa_
{
	unsigned long		DWord;				//�ֲ�������
	struct	
	{
		unsigned int	RSV2 	:2;
		unsigned int	SADR 	:9;	
		unsigned int	RSV21 	:21;
	}Bits;
}REG_CANF_sa;
/*
//   �����˲�������־��������ʼ��ַ���ݽṹ����
*/
typedef	union	_canGRP_sa_
{
	unsigned long		DWord;				//�ֲ�������
	struct	
	{
		unsigned int	RSV2 	:2;
		unsigned int	SADR 	:10;
		unsigned int	RSV20 	:20;
	}Bits;
}REG_CANGRP_sa;
/*
//   LUT����Ĵ������ݽṹ����
*/
typedef	union	_canLUTerr
{
	unsigned long		DWord;				//�ֲ�������
	struct	
	{
		unsigned int	LERR 	:1;
		unsigned int	RSV31 	:31;
	}Bits;
}REG_CANLUTerr;
/*
//   ���� CAN ���ķ����� �ṹ��:
*/
typedef struct _strTxBUF_
{   
  REG_CANTFI  FrameInfo;          // �Ƿ��׼֡
	unsigned int CANID;				// ������FF�й�
	REG_CANDF   DATAA;
	REG_CANDF   DATAB;
}strTxBUF,*P_strTxBUF;
/*
//   ���� CAN ���Ľ����� �ṹ��:
*/
typedef struct _strRxBUF_
{   
  REG_CANRFS  CANRcvFS;          // �Ƿ��׼֡
	unsigned int CANID;				// ������FF�й�
	REG_CANDF   DATAA;
	REG_CANDF   DATAB;
}strRxBUF,*P_strRxBUF;
/*
// 	CAN�������ṹ�嶨��
*/
typedef struct _CANCONTROLLOR
{
	unsigned char	m_Channel;		//CANͨ��
	//unsigned char	m_bIsLOM;		//ֻ��ģʽ��־
	//unsigned char	m_bSLPM;		//˯��ģʽ��־
	unsigned long	m_SysClk;		//ϵͳʱ��
	unsigned char	m_Status;		//״̬
	
	/*INT8U	m_EWL;			//������ֵ
	INT32U	m_CAN_Baud;		//CAN������ֵ
	INT32U	m_RegBaseAddr;	//CAN����������ʼ��ַ
	REG_CANRxBUF *m_RxBufAddr;	//���ջ������׵�ַָ��
	INT16U	m_iCAN_RxBufLen;//���ջ�������������	
	INT16U	m_iRdPoint;		//���ջ����������
	INT16U	m_iWrPiont;		//���ջ�����д���
	INT16U	m_iDataCout;	//���ջ�������Ч������Ŀ
	BOOL	m_bIsFull;		//���ջ���������־
	BOOL	m_IsSendOK;		//���ͳɹ���־
	void	*pSendSem;		//�����ź���
	void	*pRecvSem;		//�����ź���
	void	*pRcvMutexSem;	//���ն�ռ�ź���
	void	*pSndMutexSem;	//���Ͷ�ռ�ź���*/
}CANCONTROLLOR;


#ifdef __cplusplus
      }
#endif
#endif
