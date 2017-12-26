#ifndef __PROGRAM_01_H__
#define __PROGRAM_01_H__
//===============================================================================================
#include "project.h"
#include "jh_define.h"

#define SENSOR_CHIP_NO_READ				0
#define SENSOR_CHIP_DONT_RECOGNISE		1
#define SENSOR_CHIP_OV7725				2
#define SENSOR_CHIP_OV7670				3
#define SENSOR_CHIP_PAS6329				4
#define SENSOR_CHIP_GC0308				5
#define SENSOR_CHIP_BF3703				6
#define SENSOR_CHIP_BF3603				7
#define SENSOR_CHIP_OV7680				8
#define SENSOR_CHIP_BF3A03				9
#define SENSOR_CHIP_OV2640				10

#define SENSOR_INITVALUE_END			0xffff

extern const INT8U sensor_slave_address[];
extern const INT32U sensor_type_name[];
extern const INT16U *sensor_initvalue[];
//===============================================================================================
#endif 		// __PROGRAM_01_H__
