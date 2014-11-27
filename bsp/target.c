/*	
						ZheJiang University
			Control Science & Engineering(CSE)
							file information
	file name:target.c
	version:v1.0
	date:2013/4/16  y/m/d
	author：jiaxuechao
*/
#define IN_TARGET
#include "target.h"

/* description:	Initialize the target board; 
                it is called in a necessary place, change it as needed 
//		function: void TargetInit (void)
//			@param: 
//return value:	
*/
void TargetInit (void)
{   
    srand((INT32U) TargetInit); 
#if UARTOPEN	
    UART0Init(115200);            //initial the UART0 set baudrate 9600bps
#endif	
	  CAN_Init(CAN1, BPS_1000K);	//initial the CAN1,set baudrate 1Mbps
	  CAN_Init(CAN2, BPS_1000K);  //channel to PC
		NORFlash_Init();
#if Knee	
	  PINSEL1 &=~(0x03<<14);      //open P0.23
    FIO0DIR &=~(1<<23);	        //input
#endif	
}

/* description:	Initialize the SDRAM 
//		function: void  SDRAMInit( void )
//			@param: 
//return value:	
*/
void  SDRAMInit( void )
{  
    uint32 i, dummy ;

    LCD_CTRL = 0x00;
    #ifdef __OUT_CHIP                                 
      sysDelayNS(2);
    #else   
    	sysDelayNS(10);
    #endif

    /* 
    * 使能SDRAM总线连接 
    */
    PINSEL4    = 0x50000000;
    PINSEL5    = 0x05050555;
    PINSEL6   |= 0x55555555;
    PINSEL8   |= 0x55555555;
    PINSEL9   |= 0x50555555;
    EMCControl = 0x00000001;		                                    /* Disable Address mirror       */
 
    /* 
     * 设置SDRAM访问时序. 
     */
    EMCDynamictRP        = 1;
    EMCDynamictRAS       = 2;
    EMCDynamictSREX      = 1;
    EMCDynamictAPR       = 1;
    EMCDynamictDAL       = 5;
    EMCDynamictWR        = 2;
    EMCDynamictRC        = 3;
    EMCDynamictRFC       = 3;
    EMCDynamictXSR       = 5;
    EMCDynamictRRD       = 1;
    EMCDynamictMRD       = 2;

    EMCDynamicReadConfig = 1;                                           /*  Command delayed strategy    */

    /*
     * Default setting,
     * RAS latency 3 CCLKs,
     * CAS latenty 3 CCLKs.
     */
    EMCDynamicRASCAS0   = 0x00000303;

    /*
     * 64MB, 1Mx16,
     * 4 banks, row=12,
     * column=8
     */
    EMCDynamicConfig0   = 0x00000280;
    for (i = 0; i < 0x80; i++) {
    }

    /*
     * Mem clock enable,
     * CLKOUT runs, send command:
     * NOP
     */
    EMCDynamicControl   = 0x00000183;
    for (i = 0; i < 0x80; i++) {
    }

    /*
     * Send command:
     * PRECHARGE-ALL,
     * shortest possible refresh period
     */
    EMCDynamicControl   = 0x00000103;
    for (i = 0; i < 0x80; i++) {
    }


    /*
     * set 32 CCLKs
     * between SDRAM refresh
     * cycles
     */
    EMCDynamicRefresh   = 0x00000004;
    for (i = 0; i < 0x80; i++) {                                        /*  wait 128 AHB clock cycles   */
    }

    /*
     * set 45 x 16CCLKs=720CCLK=15us
     * between SDRAM refresh
     * cycles
     */
    EMCDynamicRefresh   = 45;

    /*
     * To set mode register in SDRAM,
     * enter mode by issue  MODE command, after finishing,
     * bailout and back to NORMAL mode.
     * Mem clock enable, CLKOUT runs, send command: MODE
     */
    EMCDynamicControl   = 0x00000083;
    dummy               = *((volatile int *)(0xa0019800));
		dummy=dummy;
    EMCDynamicControl   = 0x00000000;                                   /*  Send command: NORMAL        */
    EMCDynamicConfig0   = 0x00080280;                                   /*  Enable buffer               */
    for (i = 0; i < 0x80; i++) {
    }

}

/* description:	Initialize the target after reset
//		function: void TargetResetInit (void)
//			@param: 
//return value:	
*/
extern void Reset(void);
void TargetResetInit (void)
{	
#ifdef __IN_CHIP 
    SDRAMInit();                                                        /* 配置外部SDRAM                */  
    MEMMAP = 0x01;                   									                  /* remap 从片内Flash		      	*/                                                                               
#endif                                                                  /* __IN_CHIP                    */

#ifdef __OUT_CHIP    
   
    /*  
     * Edit for LPC2460/70 
     * 将中断向量表复制到片内
     * 的SRAM的前64个字节
     */
    uint8 i;
    uint32 *cp1,*cp2;
    cp1=(uint32*)Reset;
    cp2=(uint32*)0x40000000; 
    for(i=0;i<16;i++)
    {  
       *cp2++ = *cp1++;
    }   
    MEMMAP           = 0x02;                   							/* remap 到内部RAM	  	        */

	EMCControl       = 0x01;											/* 使能外部EMC，禁止镜像        */
	SDRAMInit();                                                        /* 配置外部SDRAM                */
#endif                                                                  /* __OUT_CHIP                   */ 		

#ifdef __DEBUG_SDRAM 
    /*  
     * Edit for LPC2460/70 
     * 将中断向量表复制到片内
     * 的SRAM的前64个字节
     */   
    uint8 i;
    uint32 *cp1,*cp2;
    cp1=(uint32*)Reset;
    cp2=(uint32*)0x40000000; 
    for(i=0;i<16;i++)
    {  
       *cp2++ = *cp1++;
    }  
    MEMMAP = 0x2;                  	 	                                /* remap 从内部SRAM映射         */
#endif                                                                  /* __DEBUG_SDRAM                */

	if ( PLLSTAT & (1 << 25) )
    {
		PLLCON = 1;														/* Enable PLL, disconnected     */
		PLLFEED = 0xAA;
		PLLFEED = 0x55;
    }

    PLLCON = 0;															/* Disable PLL, disconnected    */
    PLLFEED = 0xAA;
    PLLFEED = 0x55;
    while(PLLSTAT & (3 << 24));	
	
    SCS = (SCS & 0x04)|0x20;             								/* Enable the mainOSC,1MHz~20MHz*/
	while((SCS & 0x40) == 0 );			 								/* Wait until main OSC is usable*/
	
	CLKSRCSEL = 0x01;					 								/* select main OSC as the PLL 	*/
																		/* clock source 				*/
										 
    PLLCFG  = (((PLL_NValue) << 16)|(PLL_MValue));    
      
	PLLCON = 1;
    PLLFEED = 0xAA;														/* Enable but disconnect the PLL*/
    PLLFEED = 0x55;	
    while (((PLLSTAT & (1 << 24)) == 0));								/* Wait until the PLL is usable */
	
	CCLKCFG = CCLKDivValue;	
	   	
	#if USE_USB 			
 		USBCLKCFG = USBCLKDivValue;										/* usbclk = 480 MHz/10 = 48 MHz */
 		PCONP |= 0x80000000;											/* Turn On USB PCLK 			*/
	#else 	  		
		USBCLKCFG = USBCLKDivValue;	
		PCONP &= 0x7FFFFFFF;    	
	#endif                                                              /* USE_USB                      */ 		
	while ( ((PLLSTAT & (1 << 26)) == 0) );								/* Check lock bit status 		*/
	while (((PLLSTAT & 0x00007FFF) != PLL_MValue) && (((PLLSTAT & 0x00FF0000) >> 16) != PLL_NValue));
	
 	PLLCON  = 3;														/* connect the PLL 				*/
    PLLFEED = 0xAA;
    PLLFEED = 0x55;	    						
	while (((PLLSTAT & (1 << 25))!= (1 << 25)));						/* Wait until the PLL is 		*/ 
																		/* connected and locked 		*/
	
	/*  
	 *  Set system timers for each component  
	 */
#if (Fpclk / (Fcclk / 4)) == 1

    PCLKSEL0 = 0x00000000;												/* PCLK is 1/4 CCLK 	        */
    PCLKSEL1 = 0x00000000;

#else

    #if (Fpclk / (Fcclk / 4)) == 2

    PCLKSEL0 = 0xAAAAAAAA;												/* PCLK is 1/2 CCLK 			*/
    PCLKSEL1 = 0xAAAAAAAA;	 

    #else

    PCLKSEL0 = 0x55555555;												/* PCLK is the same as CCLK 	*/
    PCLKSEL1 = 0x55555555;	
    
    #endif 
#endif

	/* 
	 *  Set memory accelerater module
	 */     
   MAMCR = 0;
#if Fcclk < 20000000
    MAMTIM = 1;
#else
    #if Fcclk < 40000000
    	MAMTIM = 2;
    #else
   		MAMTIM = 3;
    #endif
#endif
    MAMCR = 2;      	
   
    /* 
     *  Add your codes here 
     */
   
    return;
}

/*****************************************************************************
  The implementations for some library functions
  For more details, please refer to the ADS compiler handbook and The library
  function manual
  User could change it as needed
******************************************************************************/
#pragma import(__use_no_semihosting_swi)
#pragma import(__use_two_region_memory)

void _ttywrch (int ch)
{
    ch = ch;
}

/*****************************************************************************
  End File
******************************************************************************/
