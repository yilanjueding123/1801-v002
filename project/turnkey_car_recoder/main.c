#include "main.h"
#include "program_02.h"

extern void F_power_Pin_Disable(void);

extern volatile void *NorWakeupVector;
extern volatile void *Wakeup;

#define MainTaskStackSize 				4096 //1024
#define IDLE_TASK_STACK_SIZE			32

INT32U MainTaskStack[MainTaskStackSize];
INT32U IdleTaskStack[IDLE_TASK_STACK_SIZE];


extern void mini_dvr_main(void);
extern volatile INT16U R_idle_task_disable;

void idle_task_entry(void *para)
{
	OS_CPU_SR cpu_sr;
	INT16U i;
	while (1) {
	if (R_idle_task_disable) continue; 
OS_ENTER_CRITICAL();
		i=0x5005;
		R_SYSTEM_WAIT = i;
		i = R_CACHE_CTRL;

		ASM(NOP);
		ASM(NOP);
		ASM(NOP);
		ASM(NOP);
		ASM(NOP);
		ASM(NOP);
		ASM(NOP);
		ASM(NOP);
		ASM(NOP);
		ASM(NOP);
		ASM(NOP);
		ASM(NOP);
		ASM(NOP);
		ASM(NOP);
		ASM(NOP);
		ASM(NOP);
OS_EXIT_CRITICAL();	    
	}
}

void Main_task_entry(void *para)
{
	mini_dvr_main();
	F_power_Pin_Disable();
}

void Main(void *free_memory)
{
	INT32U free_memory_start, free_memory_end;
	//INT32U delay;

	// Touch those sections so that they won't be removed by linker
	if (!NorWakeupVector && !Wakeup) {
		*((INT32U *) free_memory+1) = 0x0;
	}

	watchdog_enable();
	free_memory_start = ((INT32U) free_memory + 3) & (~0x3);	// Align to 4-bytes boundry
	free_memory_end = (INT32U) SDRAM_END_ADDR & ~(0x3);

	//read_efuse_num();
	//for (delay=0; delay<0xD0000; delay++);

	// Initiate Operating System
	OSInit();
	
	(*((volatile INT32U *) 0xD000000C))|=0x30;	//27MHz driving to weak

	// Initiate drvier layer 1 modules
    drv_l1_init();

	timer_freq_setup(0, OS_TICKS_PER_SEC, 0, OSTimeTick);

	// Initiate driver layer 2 modules
	drv_l2_init();

	// Initiate gplib layer modules
	gplib_init(free_memory_start, free_memory_end);
	
	// Initiate applications here
	OSTaskCreate(Main_task_entry, (void *) 0, &MainTaskStack[MainTaskStackSize - 1], 27);
	OSTaskCreate(idle_task_entry, (void *) 0, &IdleTaskStack[IDLE_TASK_STACK_SIZE - 1], (OS_LOWEST_PRIO - 2));
	//ÓÅÏÈ¼¶: 61
	
	OSStart();
}
