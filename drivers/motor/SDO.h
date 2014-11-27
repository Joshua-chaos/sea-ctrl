/*	
						ZheJiang University
			Control Science & Engineering(CSE)
							file information
	file name:SDO.h
	version:v1.0
	date:2013/4/17  y/m/d
	author£ºjiaxuechao
*/
#ifndef _SDO_H_
#define _SDO_H_

//for c++ compatible
#ifdef  __cplusplus
extern "C" {
#endif
//head files

#include "error.h"	
#include "lpc2400can.h"
#include "utility.h"

/* description:	 
//		function: void
//			@param: 
//return value:	
*/
error_code Dnld32( int16 index, int16 sub, uint32 data,uint8 prio);

	/* description:	 
//		function: void
//			@param: 
//return value:	
*/
error_code Upld32(int16 index, int16 sub, uint32 *data );

/* description:	 
//		function: void
//			@param: 
//return value:	
*/
error_code Dnld16( int16 index, int16 sub, uint16 data,uint8 prio );

/* description:	 
//		function: void
//			@param: 
//return value:	
*/
error_code Upld16( int16 index, int16 sub, uint16 *data);

/* description:	 
//		function: void
//			@param: 
//return value:	
*/
error_code Dnld8( int16 index, int16 sub, uint8 data,uint8 prio );

/* description:	 
//		function: void
//			@param: 
//return value:	
*/
error_code Download(int16 index, int16 sub, uint8 size, uint8 *data,uint8 prio );

/* description:	 
//		function: void
//			@param: 
//return value:	
*/
error_code Upload(int16 index, int16 sub, uint8 size, uint8 *data);


#ifdef __cplusplus
    }
#endif
#endif

