/*	
						ZheJiang University
			Control Science & Engineering(CSE)
							file information
	file name:norflash.c
	version:v1.0
	date:2013/10/29  y/m/d
	author��jiaxuechao
*/

//head files
#include "norflash.h"

/* description:	 
//		function:
//			@param: 
//return value:	
*/
void NORFlash_Init(void)
{
//	PCONP|=(1<<11);
	/*  
	 *  Edit for LPC2460/70
     *  �����ⲿ���߷���SST36VF1601C
     *  ���ٶȣ����У�����ȡ�ٶ�
     *  ����Ϊ��죬д�ٶ�����Ϊ��� 	
     */
  //SDARM ��ʼ��ʱ�Ѿ���   
//   PINSEL6          = 0x55555555;  /* �����ⲿ����D0~D15  	        */
//   PINSEL8          = 0x55555554;	/* �����ⲿ����A1~A15  	        */
//   PINSEL9          = 0x40050155;  /* �����ⲿ����A16~A20,OE,WE,CS1*/

//  EMCControl       = 0x01;				/* ʹ���ⲿEMC����ֹ����        */
  EMCStaticConfig1 = 0x00000081;  /* �����ⲿ���ߣ�16bit��� 	    */ 
	EMCStaticWaitRd1 = 4;						/* �����ⲿ���ߣ�����ʱ5cclk 	*/ 
	EMCStaticWaitOen1  = 0x01;			/* ���ʹ����ʱ1cclk 			*/ 
	
	EMCStaticWaitWen1  = 0x00;			/* дʹ����ʱ1cclk 			    */ 
	EMCStaticWaitWr1   = 0x02;	
}

/* description:	 ����д
//		function:
//			@param: 
//return value:	
*/
uint8  WordProgram(uint32 iAddr, uint16 iData)
{
    volatile uint16  *ipTmp;
    uint16  itemp1,itemp2;

    ipTmp = GetAddr(0x5555);	                                        /* ת����ַ0x5555                */
    ipTmp[0] = 0xaaaa;			                                        /* ��һ������,��ַ0x5555,����0xAA*/
    ipTmp = GetAddr(0x2aaa);
    ipTmp[0] = 0x5555;			                                        /* �ڶ�������,��ַ0x2aaa,����0x55*/
    ipTmp = GetAddr(0x5555);
    ipTmp[0] = 0xa0a0;			                                        /* ����������,��ַ0x5555,����0xA0*/
    
    ipTmp = (volatile uint16  *)(FLASH_ADDR|(iAddr&0x1FFFFF));
		*ipTmp = iData;				                                        /* ���ĸ�����,��ַAddr,����Data  */
   
   /*
    *  �ȴ�������� (����̲���û����ɣ�ÿ�ζ�����DQ6������)
    */
    while (1) { 
        itemp1 = *ipTmp;			
        itemp2 = *ipTmp;
        if (itemp1 == itemp2) {  
            if (itemp1 != iData) {  
                return(FALSE);
            }
            else { 
                return(TRUE);
            }
        }
    }	
}
/* description:		erase all
//		function:
//			@param: 
//return value:	
*/
uint8  ChipErase(void) 
{  
    volatile uint16  *ipTmp;
    uint16  itemp1,itemp2;

    ipTmp = GetAddr(0x5555);
    ipTmp[0] = 0xaaaa;			                                        /* ��һ������,��ַ0x5555,����0xAA*/
    ipTmp = GetAddr(0x2aaa);
    ipTmp[0] = 0x5555;			                                        /* �ڶ�������,��ַ0x2aaa,����0x55*/
    ipTmp = GetAddr(0x5555);
    ipTmp[0] = 0x8080;			                                        /* ����������,��ַ0x5555,����0x80*/
    ipTmp = GetAddr(0x5555);
    ipTmp[0] = 0xaaaa;			                                        /* ���ĸ�����,��ַ0x5555,����0xAA*/
    ipTmp = GetAddr(0x2aaa);
    ipTmp[0] = 0x5555;			                                        /* ���������,��ַ0x2aaa,����0x55*/
    ipTmp = GetAddr(0x5555);
    ipTmp[0] = 0x1010;			                                        /* ����������,��ַ0x5555,����0x10*/
    
    /* 
     *  �ȴ�������� (����������û����ɣ�ÿ�ζ�����DQ6������) 
     */
    while (1)				
    {  
        itemp1 = *ipTmp;
        itemp2 = *ipTmp;
        if (itemp1 == itemp2)
        {  
            if (itemp1 != 0xffff)
            {  
                return(FALSE);
            }
            else
            {  
                return(TRUE);
            }
        }
    }
}
/* description:		erase all
//		function:
//			@param: 
//return value:	
*/

uint8  SectorErase(uint32 SA) 
{  
    volatile uint16  *ipTmp;
    uint16  itemp1,itemp2;

    ipTmp = GetAddr(0x5555);
    ipTmp[0] = 0xaa;			                                        /* ��һ������,��ַ0x5555,����0xAA*/
    ipTmp = GetAddr(0x2aaa);
    ipTmp[0] = 0x55;			                                        /* �ڶ�������,��ַ0x2aaa,����0x55*/
    ipTmp = GetAddr(0x5555);
    ipTmp[0] = 0x80;			                                        /* ����������,��ַ0x5555,����0x80*/
    ipTmp = GetAddr(0x5555);
    ipTmp[0] = 0xaa;			                                        /* ���ĸ�����,��ַ0x5555,����0xAA*/
    ipTmp = GetAddr(0x2aaa);
    ipTmp[0] = 0x55;			                                        /* ���������,��ַ0x2aaa,����0x55*/
    ipTmp = GetAddr(SA);
    ipTmp[0] = 0x30;			                                        /* ����������,��ַ0x5555,����0x10*/
    
    /* 
     *  �ȴ�������� (����������û����ɣ�ÿ�ζ�����DQ6������) 
     */
    while (1)				
    {  
        itemp1 = *ipTmp;
        itemp2 = *ipTmp;
        if (itemp1 == itemp2)
        {  
            if (itemp1 != 0xffff)
            {  
                return(FALSE);
            }
            else
            {  
                return(TRUE);
            }
        }
    }
}
