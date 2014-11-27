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
#include "motor.h"

error_code Shutdown(uint8 prio)
{
	return Dnld16(OBJID_CONTROL, 0, (uint16)0x06,prio);
}

error_code Switchon(uint8 prio)
{
	return Dnld16(OBJID_CONTROL, 0, (uint16)0x07,prio);
}

error_code Enableoperation(uint8 prio)
{
	return Dnld16(OBJID_CONTROL, 0, (uint16)0x0f,prio);
}

error_code Faultreset(uint8 prio)
{
	return Dnld16(OBJID_CONTROL, 0, (uint16)0x8f,prio);
}

error_code SetPositionMode(uint8 prio)
{
	return Dnld16(OBJID_OP_MODE, 0, ELMOMODE_CAN_POSITION,prio);
}

error_code SetPosition(uint32 data)
{
	return Dnld32(OBJID_POS_TARGET, 0, data,0);
}

error_code SetVelMode(uint8 prio)
{
	return Dnld16(OBJID_OP_MODE, 0, ELMOMODE_CAN_VELOCITY,prio);
}

error_code SetVelocity(uint32 data)
{
	return Dnld32(OBJID_VEL_TARGET, 0, data,0);
}

error_code SetTorqueMode(uint8 prio)
{
	return Dnld16(OBJID_OP_MODE, 0, ELMOMODE_CAN_TORQUE,prio);
}

error_code SetTorque(uint32 data)
{
	return Dnld32(OBJID_TORQUE_TARGET, 0, data,0);
}

error_code SetHomeMode(uint8 prio)
{
  return Dnld16(OBJID_OP_MODE, 0, ELMOMODE_CAN_HOMING,prio);
}

error_code GetPosition(uint32 *data)
{
	return Upld32(OBJID_POS_LOAD, 0, data);
}
