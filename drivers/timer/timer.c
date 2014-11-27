/*	
						ZheJiang University
			Control Science & Engineering(CSE)
							file information
	file name:timer.c
	version:v1.0
	date:2013/10/22  y/m/d
	author£ºjiaxuechao
*/

//head files
#include "timer.h"

/* description:	software delay 
//		function: void  sysDelayNS(INT32U  uiDly)
//			@param: the bigger idly,the longer delay
//return value:	
*/
void  sysDelayNS(INT32U  uiDly)
{  
    INT32U  i;
    for(; uiDly > 0; uiDly--){
        for(i = 0; i < 5000; i++);
    }
}

/* description:	initial the timer0 
//		function: void Timer0Init (void)
//			@param: 
//return value:	
*/
void Timer0Init (void)
{
    T0IR = 0xffffffff;  //clear all the interrupt flag
    T0TCR = 0x03;       //open the counter and reset it
    T0TC = 0;
	  T0PR = 0;
    T0MCR = 0x03;       //when MRO=TC,give interrupt and clear TC
    T0MR0 = (Fpclk / 2500);
    T0TCR = 0x01;
	  IRQEnable();
    vicIrqFuncSet(4,0,(INT32U)Timer0_Handler);
 }

/* description:	the task when timer0 exception hanppen 
//		function: void Timer0_Exception (void)
//			@param: 
//return value:	
*/
// void Timer0_Exception (void)
// {
//     T0IR = 0x01;
//     VICVectAddr = 0;    //informed the interrupter controller ,the interrupt is over
// }

/* description:	initial the timer1 
//		function: void Timer1Init (void)
//			@param: 
//return value:	
*/
void Timer1Init (void)
{	
		T1TC=0;        //Ftc=Fpclk/(PR+1)=24MHz/(1+23)=1MHz
		T1PR=0x17;     //Fpclk=24MHz
		T1TCR=0x01;    //open the counter
}
