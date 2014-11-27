/*	
						ZheJiang University
			Control Science & Engineering(CSE)
							file information
	file name:motor.h
	version:v1.0
	date:2013/4/9  y/m/d
	modified:2013/4/11
	author£∫jiaxuechao
*/
#ifndef _MOTOR_H_
#define _MOTOR_H_

//for c++ compatible
#ifdef  __cplusplus
extern "C" {
#endif

//head files
#include "elmodef.h"
#include "SDO.h"	
// #include "error.h"	
// #include "utility.h"

error_code Shutdown(uint8 prio);	
error_code Switchon(uint8 prio);
error_code Enableoperation(uint8 prio);
error_code Faultreset(uint8 prio);//Œ¥≤‚ ‘µƒ√¸¡Ó
error_code SetPositionMode(uint8 prio);
error_code SetPosition(uint32 data);
error_code SetVelMode(uint8 prio);
error_code SetVelocity(uint32 data);
error_code SetTorqueMode(uint8 prio);
error_code SetTorque(uint32 data);
error_code SetHomeMode(uint8 prio);
error_code GetPosition(uint32 *data);

void motor_init(void);	



#ifdef __cplusplus
    }
#endif
#endif

