/*	
						ZheJiang University
			Control Science & Engineering(CSE)
							file information
	file name:main.cpp
	version:v1.0
	lastdate:2013/10/25  y/m/d
	author：jiaxuechao
*/

#ifndef __MAIN_H 
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif                                        /*  __cplusplus       */

	
/*********************************************************************
  Application Program Configurations
**********************************************************************/
#include  <stdio.h>
#include	<math.h>
/*********************************************************************
  ARM specital code
**********************************************************************/	
#include "target.h"
#include "filter.h"	
#include "commanddef.h"	
#define development_board 0
#define test_mode 0
#define Knee     1
	
#if Knee
//膝关节相关设置	
#define Joint_ID  (0x1&0xF)
#define Axis_ID   (0x1&0x3)
//#define RATIO  120
//#define ENCODERNUM 2000
#define REALRATIO 0.915527 //REALRATIO=RATIO*ENCODERNUM/2^18
#else
//髋关节相关设置	
#define Joint_ID  (0x0&0xF)
#define Axis_ID   (0x1&0x3)
//#define RATIO  160
//#define ENCODERNUM 2000
#define REALRATIO 1.220703 //REALRATIO=RATIO*ENCODERNUM/2^18
#endif
#define Device_ID ((Joint_ID<<2)|Axis_ID)



#if development_board
/*********************************************************************
  在这里添加用户宏定义                                 
*********************************************************************/
#define     BUZZER                  1 << 30  
#define     BUZZER_Set()            FIO3DIR |= BUZZER,FIO3CLR = BUZZER
#define     BUZZER_Clr()            FIO3SET = BUZZER
#endif

typedef struct _SPRING{
	int32	 pos;
	float  velocity;
}SPRING;

/* description:	it is a function used in debug
//		function: static void showerr(uint8 err,uint8 step);
//			@param: 
//return value:	
*/
static void showerr(uint8 err,uint8 step);

void Task (void);

void JointHoming (void);

void Correction(int *pos);

void CAN2_Exception(void);

error_code CAN1_Sent(uint32 COB_ID,uint8 len,char *data);

error_code CAN2_Sent(uint32 label,REG_CANDF *CanMessage);

void CommandParse(strRxBUF Cmd);

uint8 WriteParam(PARAMTABLE *paramtable);

void ReadParam(PARAMTABLE *paramtable);
#if development_board
void Howl(void);
#endif
/*********************************************************************
** Function name:       FIQ_Exception
** Descriptions:        FIQ中断服务程序
** input parameters:    none
** output parameters:   none
** Returned value:      none
*********************************************************************/
void FIQ_Exception (void);

#ifdef __cplusplus
    }
#endif                                      /*  __cplusplus         */
#endif                                      /*  __MAIN_H            */
