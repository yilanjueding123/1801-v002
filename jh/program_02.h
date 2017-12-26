#ifndef __PROGRAM_02_H__
#define __PROGRAM_02_H__
//===============================================================================================
#include "project.h"
#include "jh_define.h"

extern void save_COPYRIGHT_MESSAGE_to_disk(void);
extern void save_VERSION_NUMBER_to_disk(void);
extern void save_SENSOR_TYPE_to_disk(void);
extern void read_efuse_num(void);
extern void check_customer_id(void);

extern void watchdog_clr(void);
extern void watchdog_enable(void);
extern void watchdog_disable(void);
//===============================================================================================
#endif 		// __PROGRAM_02_H__
