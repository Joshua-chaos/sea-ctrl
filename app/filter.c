/*	
						ZheJiang University
			Control Science & Engineering(CSE)
							file information
	file name:filter.c
	version:v1.0
	date:2013/10/12  y/m/d
	author£ºjiaxuechao
*/

//head files
#include "filter.h"

/* description:	filter 
//		function: void filter1(void);
//			@param: 
//return value:	
*/
void push_int(int *data,int input)
{
	int i;
	for(i=0;i<FilterOrder;i++)
	{
		data[i]=data[i+1];
	}
	data[FilterOrder]=input;
}

/* description:	filter 
//		function: void filter1(void);
//			@param: 
//return value:	
*/
void push_float(float *data,float input)
{
	int i;
	for(i=0;i<FilterOrder;i++)
	{
		data[i]=data[i+1];
	}
	data[FilterOrder]=input;
}
/* description:	filter 
//		function: void filter1(void);
//			@param: 
//return value:	
*/
int filter (float *Coef,int *data)
{ 
	int i;
	float output=0;
	for(i=0;i<(FilterOrder+1);i++)
	{
		output=output+data[i]*Coef[i];
	}
  return output;
}

/* description:	filter 
//		function: void filter1(void);
//			@param: 
//return value:	
*/
float filter2 (float *Coef,float *data)
{ 
	int i;
	float output=0;
	for(i=0;i<(FilterOrder+1);i++)
	{
		output=output+data[i]*Coef[i];
	}
  return output;
}
