/*	
						ZheJiang University
			Control Science & Engineering(CSE)
							file information
	file name:target.h
	version:v1.0
	date:2013/4/16  y/m/d
	author：jiaxuechao
*/
#ifndef __TARGET_H
#define __TARGET_H

#define	__PRJ_TEMPLET_VER	0x2008070914		/*  工程模板版本为 V1.4		*/

#ifdef __cplusplus
extern "C" {
#endif                                    /*  __cplusplus           */


/*********************************************************************
  应用程序配置
  Application Program Configurations
**********************************************************************/
// #include  <rt_sys.h>
// #include  <ctype.h>
#include  <stdlib.h>
// #include  <setjmp.h>
// #include  <rt_misc.h>		
	
/**********************************************************************
  include files
***********************************************************************/	
#include  "lpc2400.h"
#include  "timer.h"
#include  "uart.h"
#include  "motor.h"
#include  "ssi.h"
#include  "norflash.h"
#ifndef IN_TARGET


/* description:	RESET 
//		function: extern void Reset (void)
//			@param: 
//return value:	
*/
extern void Reset (void);

/* description:	Initialize the target board; 
                it is called in a necessary place, change it as needed 
//		function: void TargetInit (void)
//			@param: 
//return value:	
*/
extern void TargetInit (void);

#endif                            /*  IN_TARGET                   */

#ifdef __cplusplus
    }
#endif                            /*  __cplusplus                 */

#endif                            /*  _TARGET_H                   */
/******************************************************************
  END FILE
*******************************************************************/
