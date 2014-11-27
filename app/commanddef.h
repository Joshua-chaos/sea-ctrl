/*	
						ZheJiang University
			Control Science & Engineering(CSE)
							file information
	file name:commanddef.h
	version:v1.0
	date:2013/10/25  y/m/d
	modified:2013/11/23
	author：jiaxuechao
*/

#ifndef _COMMANDDEF_H_
#define _COMMANDDEF_H_

//for c++ compatible
#ifdef  __cplusplus
extern "C" {
#endif
//head files
#define  COMMAND_MASK  0x7C0
#define	 ID_MASK			 0x3F
#define  INDEX_MASK    0xFFFF0000	
//range:0x00-0x1f	
typedef struct _PARAMTABLE{
	float Kp;
	float Ki;
	float Kd;
	float FFRatio;
	int32 SpringFree;
	uint32 PosLMT1;
	uint32 PosLMT2;
}PARAMTABLE;

//5bits	
enum ARM_COMMAND
{	
	ERROR_LABEL								= 0x00,    //报错的优先级最高
	COMMAND_ENABLE	 					= 0x01,
	COMMAND_STOP							=	0x02,
	COMMAND_PARAM_SET					= 0x03,
	COMMAND_PARAM_READ				= 0x07,
	COMMAND_MODE_SET					= 0x04,
	COMMAND_SPRING_POS_SET		= 0x05,
	COMMAND_VELOCITY_SET	 		= 0x06,
	COMMAND_HOMING						= 0x08,
	TEST_LABEL								= 0x1D,
	TGND_LABEL 							  = 0x1E,     //it's a fake command 
	COMMAND_NONE							= 0x1F  
};

//16bits
enum PARAM_INDEX
{
	PARAM_KP									= 0x0001,
	PARAM_KI									= 0x0002,
	PARAM_KD									= 0x0003,
	PARAM_FFRATIO							= 0x0004,
	PARAM_SpringFree					= 0x0005,
	PARAM_PosLMT1							= 0x0006,
	PARAM_PosLMT2							= 0x0007,
	PARAM_W_RFLASH						= 0xFFFF
};

//8bits
enum ARM_STATE
{
	STATE_ENABLE						= 0x00,
	STATE_DISABLE						= 0xFF,
	STATE_TORQUE						= 0x01,
	STATE_VELOCITY					=	0x02
};


#ifdef __cplusplus
    }
#endif
#endif

