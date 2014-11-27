/*	
						ZheJiang University
			Control Science & Engineering(CSE)
							file information
	file name:utility.h
	version:v1.0
	lastdate:2013/4/12  y/m/d
	author��jiaxuechao
	description:define some type
*/

#ifndef _UTILITY_H_
#define _UTILITY_H_

//for c++ compatible
#ifdef  __cplusplus
extern "C" {
#endif
typedef unsigned char  uint8;                                           /*  �޷���8λ���ͱ���           */
typedef signed   char  int8;                                            /*  �з���8λ���ͱ���           */
typedef unsigned short uint16;                                          /*  �޷���16λ���ͱ���          */
typedef signed   short int16;                                           /*  �з���16λ���ͱ���          */
typedef unsigned int   uint32;                                          /*  �޷���32λ���ͱ���          */
typedef signed   int   int32;                                           /*  �з���32λ���ͱ���          */
typedef float          fp32;                                            /*  �����ȸ�������32λ���ȣ�    */
typedef double         fp64;                                            /*  ˫���ȸ�������64λ���ȣ�    */

typedef unsigned char  INT8U;                                           /*  �޷���8λ���ͱ���           */
typedef signed   char  INT8S;                                           /*  �з���8λ���ͱ���           */
typedef unsigned short INT16U;                                          /*  �޷���16λ���ͱ���          */
typedef signed   short INT16S;                                          /*  �з���16λ���ͱ���          */
typedef unsigned int   INT32U;                                          /*  �޷���32λ���ͱ���          */
typedef signed   int   INT32S;                                          /*  �з���32λ���ͱ���          */
typedef float          FP32;                                            /*  �����ȸ�������32λ���ȣ�    */
typedef double         FP64;                                            /*  ˫���ȸ�������64λ���ȣ�    */

#define ByteCast(x)    ((uint8)(x))

uint32 bytes_to_uint32( uint8 *b );
uint16 bytes_to_uint16( uint8 *b );
int32 bytes_to_int32( uint8 *b );
int16 bytes_to_int16( uint8 *b );

#ifdef __cplusplus
    }
#endif
#endif
