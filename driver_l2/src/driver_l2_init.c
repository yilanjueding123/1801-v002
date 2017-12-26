#include "driver_l2.h"

extern INT32S g_nand_flash;

void drv_l2_init(void)
{

  #if (defined _DRV_L2_SYS_TIMER_EN) &&  (_DRV_L2_SYS_TIMER_EN == 1)
	sys_init_timer();
  #endif
  
  #if (defined _DRV_L2_EXT_RTC) && (_DRV_L2_EXT_RTC==DRV_L2_ON)
  	drv_l2_external_rtc_init();
  #endif
  
  #if (defined _DRV_L2_IR) && (_DRV_L2_IR==DRV_L2_ON)
	drv_l2_ir_init();
  #endif

  #if (defined _DRV_L2_ITOUCH) && (_DRV_L2_ITOUCH == DRV_L2_ON)  
	 // drv_l2_itouch_init();
  #endif

  #if (defined _DRV_L2_NAND) && (_DRV_L2_NAND == DRV_L2_ON)
#if MINI_DVR_STORAGE_TYPE == NAND_FLASH
  if(g_nand_flash == NAND_OK)
	NandInfoAutoGet();
//	DrvNand_lowlevelformat();	
	DrvNand_initial();
//	nand_part0_para_set(Nand_part0_Offset_Get(), DrvNand_get_Size() - Nand_part1_size_Get() - Nand_part0_Offset_Get(), (NAND_READ_ALLOW|NAND_WRITE_ALLOW));
//	nand_part1_para_set(Nand_part0_Offset_Get() + DrvNand_get_Size() - Nand_part1_size_Get() - Nand_part0_Offset_Get(), Nand_part1_size_Get(), (NAND_READ_ALLOW|NAND_WRITE_ALLOW));
#endif
  #endif
}
