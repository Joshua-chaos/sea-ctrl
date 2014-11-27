/*	
						ZheJiang University
			Control Science & Engineering(CSE)
							file information
	file name:ssi.h
	version:v1.0
	date:2013/9/10  y/m/d
	author£ºjiaxuechao
*/
#ifndef _SSI_H_
#define _SSI_H_

//for c++ compatible
#ifdef  __cplusplus
extern "C" {
#endif

//head files
#include "lpc2400.h"
#include "utility.h"
#include "timer.h"

#define HALF_CIRCLE 0x1FFFF    //18bit encoder 0x3FFFF/2
#define FULL_CIRCLE 0x3FFFF
	
typedef struct _ENCODER{
	uint8  ready;   //ready!=0 ready to measure vel
	uint32 cur_pos;
	uint32 last_pos;
	int32  posinterval;
	uint32 timestamp;
	uint32 timeinterval;
	float  velocity;
}ENCODER;
	
/* description:	 
//		function:
//			@param: 
//return value:	
*/
void SSI_Init(void);

	/* description:	 
//		function:
//			@param: 
//return value:	
*/
void Update_encoder(ENCODER *encoder);

#ifdef __cplusplus
    }
#endif
#endif

