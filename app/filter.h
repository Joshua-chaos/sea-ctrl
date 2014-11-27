/*	
						ZheJiang University
			Control Science & Engineering(CSE)
							file information
	file name:filter.h
	version:v1.0
	date:2013/10/12  y/m/d
	author£ºjiaxuechao
*/
#ifndef _FILTER_H_
#define _FILTER_H_

//for c++ compatible
#ifdef  __cplusplus
extern "C" {
#endif

#define FilterOrder 10	
/* description:	push 
//		function: void filter1(void);
//			@param: 
//return value:	
*/
void push_int(int *data,int input);	

/* description:	push 
//		function: void filter1(void);
//			@param: 
//return value:	
*/
void push_float(float *data,float input);	
	
/* description:	filter 
//		function: int filter (float *Coef,int *data)
//			@param: 
//return value:	
*/
int filter (float *Coef,int *data);

/* description:	filter 
//		function: void filter1(void);
//			@param: 
//return value:	
*/
float filter2 (float *Coef,float *data);

#ifdef __cplusplus
    }
#endif
#endif

