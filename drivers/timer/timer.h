/*	
						ZheJiang University
			Control Science & Engineering(CSE)
							file information
	file name:timer.h
	version:v1.0
	date:2013/4/9  y/m/d
	modified:2013/4/16
	author£ºjiaxuechao
*/
#ifndef _TIMER_H_
#define _TIMER_H_

//for c++ compatible
#ifdef  __cplusplus
extern "C" {
#endif
//head files


#include "lpc2400.h"
#include "config.h"
#include "vicControl.h"
#include "utility.h"

/* description:	software delay 
//		function: void  sysDelayNS(INT32U  uiDly)
//			@param: the bigger idly,the longer delay
//return value:	
*/
void  sysDelayNS(INT32U  uiDly);	
	
/* description:	initial the timer0 
//		function: void Timer0Init (void)
//			@param: 
//return value:	
*/
void Timer0Init (void);

/* description:	the task when timer0 exception hanppen 
//		function: void Timer0_Exception (void)
//			@param: 
//return value:	
*/
extern void Timer0_Handler(void);
extern void CAN2_Handler(void);
void Timer0_Exception (void);	


/* description:	initial the timer1 
//		function: void Timer1Init (void)
//			@param: 
//return value:	
*/
void Timer1Init (void);


#ifdef __cplusplus
    }
#endif
#endif
