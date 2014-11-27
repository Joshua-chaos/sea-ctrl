/*	
						ZheJiang University
			Control Science & Engineering(CSE)
							file information
	file name:motor.h
	version:v1.0
	date:2013/4/9  y/m/d
	modified:2014/11/27
	author：jiaxuechao
*/
#ifndef __CONFIG_H
#define __CONFIG_H

#ifdef  __cplusplus
extern "C" {
#endif


#ifndef TRUE
#define TRUE  1
#endif                                                                  /*  TRUE                        */

#ifndef FALSE
#define FALSE 0
#endif                                                                  /*  FALSE                       */


/*********************************************************************************************************
  本例子的配置
  Configuration of the example
*********************************************************************************************************/
#define	USE_USB				1

#define Fosc                12000000								    /*	OSC = 12MHz                 */

#if USE_USB	
	
	#define Fusbclk	 48000000
	
	#define Fcclk	(Fosc * 4) 										    /* 主频 Fcclk = 48MHz 		    */
	
	/* 
	 * 如果用了USB,则Fcco是Fcclk
	 * 与Fusbclk在275~550M之间的
	 * 最小公倍数				
	 * Fcco是Fcclk在275~550M之间
	 * 的最小倍数
	 */
	#define Fcco	(Fusbclk * (USBCLKDivValue + 1))							
	#define Fpclk	(Fcclk / 2)
	
	#define PLL_NValue			1
	#define PLL_MValue			(((Fcco / Fosc) * (PLL_NValue + 1) / 2) - 1)
	#define CCLKDivValue		(Fcco / Fcclk - 1)
	#define USBCLKDivValue		5	
#else 
	#define Fcclk	(Fosc  * 4)											/* 主频 Fcclk = 48MHz		    */
	#define Fcco    (Fcclk * 6)
	
	#define Fpclk	(Fcclk / 4)
	
	#define PLL_NValue			1	
	#define PLL_MValue			(((Fcco / Fosc) * (PLL_NValue + 1) / 2) - 1)
	#define CCLKDivValue		(Fcco / Fcclk - 1)
	#define USBCLKDivValue		254		
#endif	                                                                /*  USE_USB                     */

#ifdef __cplusplus
    }
#endif                                                                  /*  __cplusplus                 */
#endif                                                                  /*  __CONFIG_H                  */ 
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
