/*	
						ZheJiang University
			Control Science & Engineering(CSE)
							file information
	file name:uart.h
	version:v1.0
	date:2013/4/16  y/m/d
	author£ºjiaxuechao
*/
#ifndef _UART_H_
#define _UART_H_

//for c++ compatible
#ifdef  __cplusplus
extern "C" {
#endif
//head files


#include "lpc2400.h"
#include "config.h"
#include "utility.h"
#define UARTOPEN	0	
	
#if UARTOPEN
/* description:	Initialize the UART0 
//		function: void UART0Init (uint32 bps)
//			@param: bps
//return value:	
*/
void UART0Init (uint32 bps);

/* description:	Send the data 
//		function: extern void UART0_SendStr (char *str)
//			@param: data
//return value:	
*/
extern void UART0_SendStr (char *str);
#endif


#ifdef __cplusplus
    }
#endif
#endif
