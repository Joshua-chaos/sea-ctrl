/*	
						ZheJiang University
			Control Science & Engineering(CSE)
							file information
	file name:motor.c
	version:v1.0
	date:2013/4/9  y/m/d
	modified:2013/4/12
	author£ºjiaxuechao
*/

//head files
#include "SDO.h"


/* description:	 
//		function: void Dnld32( int16 index, int16 sub, uint32 data )
//			@param: 
//return value:	
*/
error_code Dnld32( int16 index, int16 sub, uint32 data,uint8 prio)
{  
   uint8 buff[4];
   buff[0] = ByteCast(data);
   buff[1] = ByteCast(data>>8);
   buff[2] = ByteCast(data>>16);
   buff[3] = ByteCast(data>>24);
	 return Download(index,sub,4,buff,prio);
}
/* description:	 
//		function: void
//			@param: 
//return value:	
*/
error_code Upld32( int16 index, int16 sub, uint32 *data )
{  
	 uint8 err;
   uint8 buff[4];
   buff[0] = buff[1] = buff[2] = buff[3] = 0;

   err = Upload( index, sub, 4, buff );

   *data = bytes_to_uint32( buff );

   return err;
}
/* description:	 
//		function: void
//			@param: 
//return value:	
*/
error_code Dnld16( int16 index, int16 sub, uint16 data,uint8 prio )
{ 
   uint8 buff[2];
   buff[0] = ByteCast(data);
   buff[1] = ByteCast(data>>8);
	 return Download(index,sub,2,buff,prio);
}

/* description:	 
//		function: void
//			@param: 
//return value:	
*/
error_code Upld16( int16 index, int16 sub, uint16 *data )
{  
	 uint8 err;
   uint8 buff[2];
   buff[0] = buff[1] = 0;

   err = Upload( index, sub, 2, buff );

   *data = bytes_to_uint16( buff );
   return err;
}

/* description:	 
//		function: void
//			@param: 
//return value:	
*/
error_code Dnld8( int16 index, int16 sub, uint8 data,uint8 prio )
{ 
	 return Download(index,sub,1,&data,prio);
}

/* description:	 
//		function: void
//			@param: 
//return value:	
*/
error_code Download(int16 index, int16 sub, uint8 size, uint8 *data,uint8 prio )
{
  static CANFRAME frame;
	static error_code err;
	int8 i;

		frame.ID=0x00000601;
		frame.LEN=8;
		frame.RTR=CAN_FRAME_DATA;
		frame.FF=CAN_FRAME_STD;
		frame.DATA[1]  = ByteCast(index);
		frame.DATA[2]  = ByteCast(index>>8);
		frame.DATA[3]  = ByteCast(sub);

      // If the data size is <= 4 bytes, then send an expedited download
      if( size <= 4 )
      {
	     frame.DATA[0] = 0x23 | ((4-size)<<2);
	    for( i=0; i<size; i++ ) frame.DATA[i+4] = ByteCast(data[i]);
	    for( ; i<4; i++ )       frame.DATA[i+4] = 0;
      }

      err=Xmit(CAN1,&err,prio,&frame);
       if(err==read_wait_err){err=none_err;}
    return err;
}

/* description:	 
//		function: void
//			@param: 
//return value:	
*/
error_code Upload( int16 index, int16 sub, uint8 size,uint8 *data )
{ 
	static CANFRAME frame;
	static error_code err;
		
		frame.ID=0x00000601;
		frame.LEN=8;
		frame.RTR=CAN_FRAME_DATA;
		frame.FF=CAN_FRAME_STD;
		frame.DATA[0]  = 0x40; 
		frame.DATA[1]  = ByteCast(index);
		frame.DATA[2]  = ByteCast(index>>8);
		frame.DATA[3]  = ByteCast(sub);
	
	 err=Xmit(CAN1,&err,0,&frame);
	
	 if((err==read_wait_err)||(err==sent_finished))
	 {err=WaitForTransfer(CAN1,size,100,&frame,data);}
	return err;
}

