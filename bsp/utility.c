#include "utility.h"

uint32 bytes_to_uint32( uint8 *b )
{
	uint32 ret;
	ret  = (uint32)(ByteCast(b[0]));
	ret |= (uint32)(ByteCast(b[1])) << 8;
	ret |= (uint32)(ByteCast(b[2])) << 16;
	ret |= (uint32)(ByteCast(b[3])) << 24;
	return ret;
}

uint16 bytes_to_uint16( uint8 *b )
{
	uint16 ret;
	ret  = (uint16)(ByteCast(b[0]));
	ret |= (uint16)(ByteCast(b[1])) << 8;
	return ret;
}

int32 bytes_to_int32( uint8 *b )
{
	return (int32)bytes_to_uint32(b);
}

int16 bytes_to_int16( uint8 *b )
{
	return (int16)bytes_to_uint16(b);
}

