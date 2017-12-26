#ifndef __PROGRAM_04_H__
#define __PROGRAM_04_H__
//===============================================================================================
#include "project.h"
#include "jh_define.h"

extern void red_on(void);
extern void red_off(void);
extern void green_on(void);
extern void green_off(void);
extern void led_reversal(void);
extern void bonding_led_on(void);
extern void bonding_led_off(void);
extern void F_power_Pin_Enable(void);
extern void F_power_Pin_Disable(void);
extern void power_ctrl_low(INT8U flag);
extern void no_disk_handle(void);
extern void disk_no_size_handle(void);
extern void F_Show_Error_And_WaitKey(INT32U led_cnt);
extern void F_Show_OK_And_WaitKey(INT32U led_cnt);
//===============================================================================================
#endif 		// __PROGRAM_04_H__
