/*	
						ZheJiang University
			Control Science & Engineering(CSE)
							file information
	file name:norflash.h
	version:v1.0
	date:2013/10/29  y/m/d
	author：jiaxuechao
*/
#ifndef _NORFLASH_H_
#define _NORFLASH_H_

//for c++ compatible
#ifdef  __cplusplus
extern "C" {
#endif

#define  FLASH_ADDR		0x81000000 //FLASH的起始地址(分配为Bank1块)
#define  GetAddr(addr)	(volatile uint16  *)(FLASH_ADDR|(addr<<1))
//head files
#include "lpc2400.h"
#include "utility.h"
#include "config.h"	

#define SA(x) (x<<15)

	
/* description:	 
//		function:
//			@param: 
//return value:	
*/
void NORFlash_Init(void);

/* description:	 半字写
//		function:
//			@param: 
//return value:	
*/
uint8 WordProgram(uint32 iAddr, uint16 iData);

/* description:		erase all
//		function:
//			@param: 
//return value:	
*/
uint8  ChipErase(void); 

/* description:		erase all
//		function:
//			@param: 
//return value:	
*/

uint8  SectorErase(uint32 SA);

#ifdef __cplusplus
    }
#endif
#endif

