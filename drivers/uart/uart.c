/*	
						ZheJiang University
			Control Science & Engineering(CSE)
							file information
	file name:uart.c
	version:v1.0
	date:2013/4/16  y/m/d
	author：jiaxuechao
*/

//head files
#include "uart.h"
#if UARTOPEN
/* description:	Initialize the UART0 
//		function: void UART0Init (uint32 bps)
//			@param: bps
//return value:	
*/
void UART0Init (uint32 bps)
{
    uint16 Fdiv;
    
    PCONP |= 1<<3;                                     /*  在PCONP中启动UART*/
    PINSEL0 = (PINSEL0 & 0xffffff0f)|(1<<4)|(1<<6);    /* 选择管脚P0.2、P0.3为UART0 */

    U0LCR = 0x80;                               /* 允许访问分频因子寄存器 */
    Fdiv = (Fpclk / 16) / bps;                  /* 设置波特率 */
    U0DLM = Fdiv / 256;							
	  U0DLL = Fdiv % 256;						
    U0LCR = 0x03;                               /* 禁止访问分频因子寄存器 */
                                                /* 同时设置UART工作模式为8位字符长度,1个停止位,无奇偶校验位 */
	  U0IER = 0x00;                               /* 禁止中断 */
    U0FCR = 0x81;                               /* 使能FIFO，接收缓冲区的触发点为8个字节 */
} 

/* description:	Send the data 
//		function: void UART0_SendByte (uint8 data)
//			@param: data
//return value:	
*/
void UART0_SendByte (uint8 data)
{
    U0THR = data;                                /*  发送数据*/
    while ((U0LSR & 0X40) == 0);                 /*  等待数据发送完毕*/
}

/* description:	Send the data 
//		function: extern void UART0_SendStr (char *str)
//			@param: data
//return value:	
*/
extern void UART0_SendStr (char *str)
{
    while (1) {
        if (*str == '\0') {/*  遇到结束符,退出*/
            break;
        }
        UART0_SendByte(*str++);
    }
}
#endif
