/*	
						ZheJiang University
			Control Science & Engineering(CSE)
							file information
	file name:utility.h
	version:v1.0
	lastdate:2013/4/12  y/m/d
	author：jiaxuechao
	description:define some type
*/

#ifndef _UTILITY_H_
#define _UTILITY_H_

//for c++ compatible
#ifdef  __cplusplus
extern "C" {
#endif
typedef unsigned char  uint8;                                           /*  无符号8位整型变量           */
typedef signed   char  int8;                                            /*  有符号8位整型变量           */
typedef unsigned short uint16;                                          /*  无符号16位整型变量          */
typedef signed   short int16;                                           /*  有符号16位整型变量          */
typedef unsigned int   uint32;                                          /*  无符号32位整型变量          */
typedef signed   int   int32;                                           /*  有符号32位整型变量          */
typedef float          fp32;                                            /*  单精度浮点数（32位长度）    */
typedef double         fp64;                                            /*  双精度浮点数（64位长度）    */

typedef unsigned char  INT8U;                                           /*  无符号8位整型变量           */
typedef signed   char  INT8S;                                           /*  有符号8位整型变量           */
typedef unsigned short INT16U;                                          /*  无符号16位整型变量          */
typedef signed   short INT16S;                                          /*  有符号16位整型变量          */
typedef unsigned int   INT32U;                                          /*  无符号32位整型变量          */
typedef signed   int   INT32S;                                          /*  有符号32位整型变量          */
typedef float          FP32;                                            /*  单精度浮点数（32位长度）    */
typedef double         FP64;                                            /*  双精度浮点数（64位长度）    */

#define ByteCast(x)    ((uint8)(x))

uint32 bytes_to_uint32( uint8 *b );
uint16 bytes_to_uint16( uint8 *b );
int32 bytes_to_int32( uint8 *b );
int16 bytes_to_int16( uint8 *b );

#ifdef __cplusplus
    }
#endif
#endif
