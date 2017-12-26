#ifndef __GP_TIMER_H__
#define __GP_TIMER_H__
/******************************************************************************
 * Paresent Header Include
 ******************************************************************************/
#include "driver_l1.h"

/*
// Each Tiny count time is 2.666... us//
#define TINY_WHILE(BREAK_CONDITION,TIMEOUT_TINY_COUNT)  \
{\
   INT32S ttt1 = tiny_counter_get();\
   INT32S dt=0;\
   while(!((BREAK_CONDITION) || (dt>TIMEOUT_TINY_COUNT)))\
   {\
        dt = ((tiny_counter_get() | 0x10000) - ttt1) & (0xFFFF);\
   }\
}


#if (defined _PROJ_BOOTLOADER) && (_PROJ_TYPE==_PROJ_BOOTLOADER)
	
	#define MINY_WHILE(BREAK_CONDITION,TIMEOUT_MINY_COUNT)  \
	{\
	   INT32S dtm=0;\
	   while(!((BREAK_CONDITION) || (dtm>TIMEOUT_MINY_COUNT)))\
	   {\
	        {dtm++;}\
	   }\
	}

#else   
	#define MINY_WHILE(BREAK_CONDITION,TIMEOUT_MINY_COUNT)  \
	{\
	   INT32S ttt2 = sw_timer_get_counter_L();\
	   INT32S dtm=0;\
	   while(!((BREAK_CONDITION) || (dtm>TIMEOUT_MINY_COUNT)))\
	   {\
	        {dtm = (sw_timer_get_counter_L() - ttt2);}\
	   }\
	}

#endif  //#if (defined _PROJ_BOOTLOADER) && (_PROJ_TYPE==_PROJ_BOOTLOADER)
*/

extern INT32U sw_timer_init(INT32U TimerId, INT32U freq_hz);
extern INT32U sw_timer_get_counter_L(void);
extern INT32U sw_timer_get_counter_H(void);
extern void sw_timer_isr(void);
extern void sw_schedual_isr(void);
extern void sw_timer_ms_delay(INT32U msec);
extern INT32U sw_timer_count_H;
extern INT32U sw_timer_count_L;
extern INT32U sw_schedual_count;


#endif
