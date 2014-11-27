/*	
						ZheJiang University
			Control Science & Engineering(CSE)
							file information
	file name:norflash.c
	version:v1.0
	date:2013/10/29  y/m/d
	author：jiaxuechao
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
     *  配置外部总线访问SST36VF1601C
     *  的速度，其中，将读取速度
     *  配置为最快，写速度配置为最快 	
     */
  //SDARM 初始化时已经打开   
//   PINSEL6          = 0x55555555;  /* 配置外部总线D0~D15  	        */
//   PINSEL8          = 0x55555554;	/* 配置外部总线A1~A15  	        */
//   PINSEL9          = 0x40050155;  /* 配置外部总线A16~A20,OE,WE,CS1*/

//  EMCControl       = 0x01;				/* 使能外部EMC，禁止镜像        */
  EMCStaticConfig1 = 0x00000081;  /* 配置外部总线，16bit宽度 	    */ 
	EMCStaticWaitRd1 = 4;						/* 配置外部总线，读延时5cclk 	*/ 
	EMCStaticWaitOen1  = 0x01;			/* 输出使能延时1cclk 			*/ 
	
	EMCStaticWaitWen1  = 0x00;			/* 写使能延时1cclk 			    */ 
	EMCStaticWaitWr1   = 0x02;	
}

/* description:	 半字写
//		function:
//			@param: 
//return value:	
*/
uint8  WordProgram(uint32 iAddr, uint16 iData)
{
    volatile uint16  *ipTmp;
    uint16  itemp1,itemp2;

    ipTmp = GetAddr(0x5555);	                                        /* 转换地址0x5555                */
    ipTmp[0] = 0xaaaa;			                                        /* 第一个周期,地址0x5555,数据0xAA*/
    ipTmp = GetAddr(0x2aaa);
    ipTmp[0] = 0x5555;			                                        /* 第二个周期,地址0x2aaa,数据0x55*/
    ipTmp = GetAddr(0x5555);
    ipTmp[0] = 0xa0a0;			                                        /* 第三个周期,地址0x5555,数据0xA0*/
    
    ipTmp = (volatile uint16  *)(FLASH_ADDR|(iAddr&0x1FFFFF));
		*ipTmp = iData;				                                        /* 第四个周期,地址Addr,数据Data  */
   
   /*
    *  等待操作完成 (若编程操作没有完成，每次读操作DQ6会跳变)
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
    ipTmp[0] = 0xaaaa;			                                        /* 第一个周期,地址0x5555,数据0xAA*/
    ipTmp = GetAddr(0x2aaa);
    ipTmp[0] = 0x5555;			                                        /* 第二个周期,地址0x2aaa,数据0x55*/
    ipTmp = GetAddr(0x5555);
    ipTmp[0] = 0x8080;			                                        /* 第三个周期,地址0x5555,数据0x80*/
    ipTmp = GetAddr(0x5555);
    ipTmp[0] = 0xaaaa;			                                        /* 第四个周期,地址0x5555,数据0xAA*/
    ipTmp = GetAddr(0x2aaa);
    ipTmp[0] = 0x5555;			                                        /* 第五个周期,地址0x2aaa,数据0x55*/
    ipTmp = GetAddr(0x5555);
    ipTmp[0] = 0x1010;			                                        /* 第六个周期,地址0x5555,数据0x10*/
    
    /* 
     *  等待操作完成 (若擦除操作没有完成，每次读操作DQ6会跳变) 
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
    ipTmp[0] = 0xaa;			                                        /* 第一个周期,地址0x5555,数据0xAA*/
    ipTmp = GetAddr(0x2aaa);
    ipTmp[0] = 0x55;			                                        /* 第二个周期,地址0x2aaa,数据0x55*/
    ipTmp = GetAddr(0x5555);
    ipTmp[0] = 0x80;			                                        /* 第三个周期,地址0x5555,数据0x80*/
    ipTmp = GetAddr(0x5555);
    ipTmp[0] = 0xaa;			                                        /* 第四个周期,地址0x5555,数据0xAA*/
    ipTmp = GetAddr(0x2aaa);
    ipTmp[0] = 0x55;			                                        /* 第五个周期,地址0x2aaa,数据0x55*/
    ipTmp = GetAddr(SA);
    ipTmp[0] = 0x30;			                                        /* 第六个周期,地址0x5555,数据0x10*/
    
    /* 
     *  等待操作完成 (若擦除操作没有完成，每次读操作DQ6会跳变) 
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
