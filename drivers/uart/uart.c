/*	
						ZheJiang University
			Control Science & Engineering(CSE)
							file information
	file name:uart.c
	version:v1.0
	date:2013/4/16  y/m/d
	author��jiaxuechao
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
    
    PCONP |= 1<<3;                                     /*  ��PCONP������UART*/
    PINSEL0 = (PINSEL0 & 0xffffff0f)|(1<<4)|(1<<6);    /* ѡ��ܽ�P0.2��P0.3ΪUART0 */

    U0LCR = 0x80;                               /* ������ʷ�Ƶ���ӼĴ��� */
    Fdiv = (Fpclk / 16) / bps;                  /* ���ò����� */
    U0DLM = Fdiv / 256;							
	  U0DLL = Fdiv % 256;						
    U0LCR = 0x03;                               /* ��ֹ���ʷ�Ƶ���ӼĴ��� */
                                                /* ͬʱ����UART����ģʽΪ8λ�ַ�����,1��ֹͣλ,����żУ��λ */
	  U0IER = 0x00;                               /* ��ֹ�ж� */
    U0FCR = 0x81;                               /* ʹ��FIFO�����ջ������Ĵ�����Ϊ8���ֽ� */
} 

/* description:	Send the data 
//		function: void UART0_SendByte (uint8 data)
//			@param: data
//return value:	
*/
void UART0_SendByte (uint8 data)
{
    U0THR = data;                                /*  ��������*/
    while ((U0LSR & 0X40) == 0);                 /*  �ȴ����ݷ������*/
}

/* description:	Send the data 
//		function: extern void UART0_SendStr (char *str)
//			@param: data
//return value:	
*/
extern void UART0_SendStr (char *str)
{
    while (1) {
        if (*str == '\0') {/*  ����������,�˳�*/
            break;
        }
        UART0_SendByte(*str++);
    }
}
#endif
