/*	
						ZheJiang University
			Control Science & Engineering(CSE)
							file information
	file name:I2CINT.h
	version:v1.0
	date:2013/10/30  y/m/d
	author£ºjiaxuechao
*/

#ifndef  I2CINT_H
#define  I2CINT_H

//for c++ compatible
#ifdef  __cplusplus
extern "C" {
#endif

//head file
#include	"config.h"
#include	"lpc2400.h"
#include  "utility.h"

#define	ONE_BYTE_SUBA	1
#define TWO_BYTE_SUBA	2
#define X_ADD_8_SUBA	3

extern uint8 IRcvByte(uint8 sla, uint8 *dat);
extern uint8 ISendByte(uint8 sla, uint8 dat);
extern uint8 I2C_ReadNByte (uint8 sla, uint32 suba_type, uint32 suba, uint8 *s, uint32 num);
extern uint8 I2C_WriteNByte(uint8 sla, uint8 suba_type, uint32 suba, uint8 *s, uint32 num);
extern void __irq IRQ_I2C(void);

#ifdef __cplusplus
    }
#endif	
#endif


