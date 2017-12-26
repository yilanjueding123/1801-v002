/*
* Purpose: System option setting
*
* Author: wschung
*
* Date: 2008/5/9
*
* Copyright Generalplus Corp. ALL RIGHTS RESERVED.
*
* Version : 1.00
* History :

     1 .Add system_bus_arbiter_init  2008/4/21 wschung
        setting all masters as 0
       .Add system_sdram_driving      2008/4/21 wschung
        setting SDRAM driving as 8 ma

*/
#include "drv_l1_system.h"

//=== This is for code configuration DON'T REMOVE or MODIFY it ===//
#if (defined _DRV_L1_SYSTEM) && (_DRV_L1_SYSTEM == 1)             //
//================================================================//


#define INT_EXTAB             0x04000000
#define INT_RTC               0x80000000

/* use IRAM for storing memory setting, a */
#define R_ISRAM_CS0			        (*((volatile INT32U *) 0xF8000000))
#define R_ISRAM_CS1			        (*((volatile INT32U *) 0xF8000004))
#define R_ISRAM_CS2			        (*((volatile INT32U *) 0xF8000008))
#define R_ISRAM_CS3			        (*((volatile INT32U *) 0xF800000C))

#if (defined DYNAMIC_MCLK) && (DYNAMIC_MCLK==1)
INT32U  MCLK = INIT_MCLK;
INT32U  MHZ  = INIT_MHZ;
#endif

/* Prototypes */
void system_init(void);
void system_32768(INT32U Enable);
void system_half_no_reset(void);
void system_half_wakeup(void);
void system_set_pll(INT32U PLL_CLK);
void system_clks_enable0(INT32U CLK_MASK);
void system_clks_enable1(INT32U CLK_MASK);
void system_clks_disable0(INT32U CLK_MASK);
void system_clks_disable1(INT32U CLK_MASK);
void system_bus_arbiter_init(void);
void system_sdram_driving(void);
void system_enter_halt_mode(void);
void system_power_on_ctrl(void);
INT32U system_rtc_counter_get(void);
void system_rtc_counter_set(INT32U rtc_cnt);
INT32S sys_sdram_self_refresh_thread_cycle(INT8U sref_cycle);
INT32S sys_sdram_CAS_latency_cycle(SDRAM_CL cl_cycle);
INT32S sys_sdram_wakeup_cycle(INT8U RC_cycle);
INT32S sys_sdram_refresh_period_ns(INT32U refresh_period_ns);
INT32S sys_sdram_MHZ_set(SDRAM_CLK_MHZ sd_clk);
INT32S sys_sdram_read_latch_at_CLK_Neg(INT8U ebable_disable);
INT8U mcu_version_get(void);
void sys_sdram_input_clock_dly_enable(INT8U ebable_disable);
static tREFI_ENUM refresh_period=tREFI_15o6u;
INT32S sys_sdram_refresh_period_set(tREFI_ENUM tREFI);

#if MCU_VERSION == GPL32_GP10 
INT32S sdram_calibrationed_flag(void);
#endif

// Provide your System driver and ISR here
void system_init(void)
{
    SD_CLK_OUT_DELAY_CELL clk_out_dly_cell;
    SD_CLK_IN_DELAY_CELL clk_in_dly_cell;
    SD_CLK_PHASE clk_phase; 
    SD_BUS_WIDTH bus_mode; 
    SD_BANK_BOUND bank_bound; 
    SD_SIZE sd_size=SDRAM_SIZE; 
    tREFI_ENUM refresh_period; 
    //SDRAM_CLK_MHZ clock; 
    SD_SWITCH disable_enable;

	if (mcu_version_get() == GPL32_B) {
        clk_out_dly_cell=SDRAM_OUT_DLY_LV1; /*key parameter1*/
        clk_in_dly_cell=SDRAM_IN_DLY_LV0;
        clk_phase=SD_CLK_PHASE_0;
        bus_mode=SD_BUS_16bit;
        bank_bound=SD_BANK_1M;
        sd_size=SD_64Mb;
        refresh_period=tREFI_15o6u;   /*key parameter2*/
       // clock=MHZ;
        disable_enable=SD_ENABLE;
    } else if (mcu_version_get() == GPL32_C) {
        clk_out_dly_cell=SDRAM_OUT_DLY_LV5; /*key parameter1*/
        clk_in_dly_cell=SDRAM_IN_DLY_LV0;
        clk_phase=SD_CLK_PHASE_0;
        bus_mode=SD_BUS_16bit;
        bank_bound=SD_BANK_1M;
        sd_size=SD_64Mb;
        refresh_period=tREFI_15o6u; /*key parameter2*/
      //  clock=MHZ;
        disable_enable =SD_ENABLE;
    } else if (mcu_version_get() == GPL32_GP10) {
      #if 1//BOARD_TYPE == BOARD_TK_32600_V1_0
        if (MHZ <= 96) {
            clk_out_dly_cell=SDRAM_OUT_DLY_LV4; /*key parameter1*/
            clk_in_dly_cell=SDRAM_IN_DLY_LV0;
        } else if (MHZ ==102) {
            clk_out_dly_cell=SDRAM_OUT_DLY_LV3; /*key parameter1*/
            clk_in_dly_cell=SDRAM_IN_DLY_LV2;
        } else if (MHZ ==108) {
            clk_out_dly_cell=SDRAM_OUT_DLY_LV3; /*key parameter1*/
            clk_in_dly_cell=SDRAM_IN_DLY_LV3;
        }        
      #else
      	clk_out_dly_cell=SDRAM_OUT_DLY_LV2;
        clk_in_dly_cell=SDRAM_IN_DLY_LV0;
      #endif

        clk_phase=SD_CLK_PHASE_0;
        bus_mode=SD_BUS_16bit;
        bank_bound=SD_BANK_1M;
        sd_size=SD_256Mb;
        refresh_period=tREFI_15o6u; /*key parameter2*/
      //  clock=MHZ;
        disable_enable =SD_ENABLE;
    } else {
        clk_out_dly_cell=SDRAM_OUT_DLY_LV5; /*key parameter1*/
        clk_in_dly_cell=SDRAM_IN_DLY_LV0;
        clk_phase=SD_CLK_PHASE_0;
        bus_mode=SD_BUS_16bit;
        bank_bound=SD_BANK_1M;
        sd_size=SD_64Mb;
        refresh_period=tREFI_15o6u; /*key parameter2*/
      //  clock=MHZ;
        disable_enable =SD_ENABLE;
    }

    if (sd_size<SD_256Mb)  /* according to general SDRAM Spec. */
    {
        refresh_period=tREFI_15o6u;
    } else { 
        refresh_period=tREFI_7o8u;
    }
    
    sys_sdram_refresh_period_set(refresh_period);
    //sys_sdram_MHZ_set(clock);
#if MCU_VERSION == GPL32_GP10 
    if (MHZ <= 100) {
        sys_sdram_CAS_latency_cycle(CL_2); /*default is CAS slow CL3 for compatible*/
    } else {
        sys_sdram_CAS_latency_cycle(CL_3);
    }
    sys_sdram_read_latch_at_CLK_Neg(1);  /*default is negtive latch*/
#else
    sys_sdram_CAS_latency_cycle(CL_2); /*default is CAS slow CL3 for compatible*/
    sys_sdram_read_latch_at_CLK_Neg(1);  /*default is positive latch*/
#endif
    sys_sdram_init(clk_out_dly_cell, clk_in_dly_cell, clk_phase, bus_mode, bank_bound, sd_size, disable_enable);

    system_sdram_driving(); // Set SDRAM driving
}

#if MCU_VERSION == GPL32_GP10 
INT32S sdram_calibrationed_flag(void) 
{
    return (R_MEM_SDRAM_CTRL0&0x700);
}
#endif 

INT8U mcu_version_get(void)
{
	INT32U value;

	value = *((INT32U *) 0xF0001E00);
	if (value == 0x00000012) {			// Version A
		return 1;
	} else if (value == 0xE2811002) {	// Version B
		return 2;
	} else if (value == 0xE2811003) {	// Version C
		return 3;
	}
	else if (value == 0x826959) { // GPL32600
		return 0x10;
	}

	return 255;
}

void system_32768(INT32U Enable)
{
 if (Enable) R_SYSTEM_CLK_CTRL =0x4400;
  while (R_SYSTEM_POWEP_STATE == 0); //waiting stable
}
void system_half_no_reset(void)
{
    INT32U data;
    //TFT Close

    //DAC/AD close

    /*SDRAM Latch*/
    R_MEM_SDRAM_MISC =0x4;
     /* Halt mode config */
    R_SYSTEM_CTRL |= 0x02;   //without reset
    R_SYSTEM_HALT      = 0x500A; //enter HALF mode
    data =(*((volatile INT32U *) 0xFF000000));//DUMMY OP
}

void system_half_wakeup(void)
{
   //waiting PLL stable

   //TFT initial and enable ?

   //DAC/AD initial and enable ?
}

void system_set_pll(INT32U PLL_CLK)
{
 INT32U data;
 INT32U sys_ctrl_reg_val=R_SYSTEM_CLK_CTRL;
 
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#if USED_MCU_NAME != MCU_GPDV6655B
 if (PLL_CLK >= 115) { 
		//115MHz SDRAM 
		if ((*((volatile INT32U *) 0xD0000088)) & 0x4) { 
		        // GPDV6624C 
		        R_MEM_SDRAM_CTRL0 = 0x8010; 
		} else { 
		        // GPDV6624B 
		        R_MEM_SDRAM_CTRL0 = 0x7010; 
		} 
		R_MEM_SDRAM_CTRL1 = 0x3000;                // 0xD0200044 
		R_MEM_SDRAM_TIMING = 0x0F8A;        // 0xD0200048 
		R_MEM_SDRAM_CBRCYC = 0x0730;        // 0xD020004C                //0x0730(118MHz), 0x6E2(113MHz) 
		R_MEM_SDRAM_MISC = 0x0006;                // 0xD0200050 
		R_MEM_DRV = 0x5555;                                // 0xC0000124                // 0x0000=4mA, 0x5555=8mA, 0xAAAA=12mA 

	} else if (PLL_CLK >= 105) { 
		//105MHz SDRAM 
		if ((*((volatile INT32U *) 0xD0000088)) & 0x4) { 
		        // GPDV6624C 
		        R_MEM_SDRAM_CTRL0 = 0x8010; 
		} else { 
		        // GPDV6624B 
		        R_MEM_SDRAM_CTRL0 = 0x7010; 
		} 
		R_MEM_SDRAM_CTRL1 = 0x3000;                // 0xD0200044 
		R_MEM_SDRAM_TIMING = 0x0F8A;        // 0xD0200048 
		R_MEM_SDRAM_CBRCYC = 0x0666;        // 0xD020004C 
		R_MEM_SDRAM_MISC = 0x0006;                // 0xD0200050 
		R_MEM_DRV = 0x5555;                                // 0xC0000124                // 0x0000=4mA, 0x5555=8mA, 0xAAAA=12mA 

	} else { 
		//96MHz SDRAM 
		R_MEM_SDRAM_CTRL0 = 0x8010; 
		R_MEM_SDRAM_CTRL1 = 0x2000;                // 0xD0200044 
		R_MEM_SDRAM_TIMING = 0x0F8A;        // 0xD0200048 
		R_MEM_SDRAM_CBRCYC = 0x05D9;        // 0xD020004C 
		R_MEM_SDRAM_MISC = 0x0006;                // 0xD0200050 
		R_MEM_DRV = 0x5555;                                // 0xC0000124                // 0x0000=4mA, 0x5555=8mA, 0xAAAA=12mA 
	}
#endif	
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 #if USED_MCU_NAME == MCU_GPDV6655D
 R_SYSTEM_CTRL =0x000003A2;// 0x000003A3;
 #endif

 //PLL setting
 if (PLL_CLK == 96) {
    system_32768(0);         //1 IF Set 32768
    sys_ctrl_reg_val |=0x0400; // dominant add, 11/18/2008
    sys_ctrl_reg_val &= ~(0x8008); // dominant add, 11/18/2008

// ??F|wt iRAM -> Set Slow mode to 6MHz -> Set pll   
//	R_SYSTEM_CLK_CTRL = sys_ctrl_reg_val;	//2  Slow Mode = 6MHz
// 	while (R_SYSTEM_POWEP_STATE == 0); //wait
  	//+++++++++++++++ lx 修改24MHz +++++++++++++++++++++++++ 24MHz Crystal    
    //R_SYSTEM_PLLEN = 0x20;   /* sys clk = 96MHz */
    R_SYSTEM_PLLEN = 0x14;
    R_SYSTEM_PLLEN |= 0x40;
    //------------------------------------------------------ 24MHz Crystal
    sys_ctrl_reg_val |= 0x8008; // dominant add, 11/18/2008
 #if (_DRV_L1_DAC ==1) || (_DRV_L1_ADC == 1)
 	sys_ctrl_reg_val |= 0x10; /* DA/AD PLL Enable */
 #endif
    R_SYSTEM_CLK_CTRL = sys_ctrl_reg_val;
//    sys_sdram_MHZ_set(SD_96MHZ);  // dominant add, 11/18/2008

 }
 else if (PLL_CLK == 48) {
    sys_sdram_MHZ_set(SD_48MHZ);  // dominant add, 11/18/2008
  	R_SYSTEM_PLLEN = 0x10;   /* sys clk = 48MHz */
    sys_ctrl_reg_val &= ~(0x8008); // dominant add, 11/18/2008
    sys_ctrl_reg_val |= (0x8000); // dominant add, 11/18/2008
 #if (_DRV_L1_DAC ==1) || (_DRV_L1_ADC == 1)
 	sys_ctrl_reg_val |= 0x10; /* DA/AD PLL Enable */
 #endif
    R_SYSTEM_CLK_CTRL = sys_ctrl_reg_val;
 }
 else if (PLL_CLK == 6) {
 	R_SYSTEM_CLK_CTRL = 0x400;
 }
 else
 {
 }

	//R_SYSTEM_PLLEN |= 0x100;	//2011-11-14 14:56: 台湾卢工说删除本行, 硬件上即可不用焊 32768 晶振;

 data =(*((volatile INT32U *) 0xFF000000));//DUMMY OP
 data =(*((volatile INT32U *) 0xFF000001));//DUMMY OP
 while (R_SYSTEM_POWEP_STATE == 0); //waiting stable
 //MHZ = PLL_CLK;
}

void system_clks_enable0(INT32U CLK_MASK)
{
 R_SYSTEM_CLK_EN0 |=CLK_MASK ;
}
void system_clks_enable1(INT32U CLK_MASK)
{
 R_SYSTEM_CLK_EN1 |=CLK_MASK ;
}

void system_clks_disable0(INT32U CLK_MASK)
{
 R_SYSTEM_CLK_EN0 &=~CLK_MASK ;
}

void system_clks_disable1(INT32U CLK_MASK)
{
 R_SYSTEM_CLK_EN1 &=~CLK_MASK ;
}

void system_bus_arbiter_init(void)
{
	/*
	0 ppu display
	1 sensor
	2 usb20
	3 ppu engine
	4 dma
	5 jpg
	6 scal
	7 spu
	8 nfc
	9 cpu
	a mp3
	b mp4
	*/


  R_MEM_M2_BUS_PRIORITY =0;
  R_MEM_M3_BUS_PRIORITY =0;
  R_MEM_M4_BUS_PRIORITY =2;
  R_MEM_M5_BUS_PRIORITY =0;
  R_MEM_M6_BUS_PRIORITY =0;
  R_MEM_M7_BUS_PRIORITY =0;
  R_MEM_M8_BUS_PRIORITY =0;
  R_MEM_M9_BUS_PRIORITY =1;
  R_MEM_M10_BUS_PRIORITY =0;
  R_MEM_M11_BUS_PRIORITY =0;

}

void system_sdram_driving(void)
{
 //A version default drivering are 12 ma
 //Set all pin as 8ma
#if 0//BOARD_TYPE < BOARD_TURNKEY_BASE
	#if MCU_VERSION != GPL32_GP10 
	R_MEM_DRV =0x5555;
	#endif
#else
	R_MEM_DRV = 0; /* 4mA */
#endif
/*
	if (mcu_version_get() == GPL32_B) {
    	R_IOC_DRV     = 0xfff0;  // domi mark, 2008/11/14
     	R_IOD_DRV     = 0x55555555;//cfc // domi mark, 2008/11/14
   		R_MEM_SDRAM_CTRL0 =0x1013;  // domi mark, move to system_init()
    	R_IOB_DRV     =0x0008; // domi mark, 2008/11/14
	}
*/
}

#if 0
void system_usbh_cheat(void)
{
    //R_SYSTEM_CLK_CTRL &= ~0x8;
}

void system_usbh_uncheat(void)
{
    R_SYSTEM_CLK_CTRL |= 0x8;
}
#endif

#if 0
void system_rtc_reset_time_set(void)
{
  #if _DRV_L1_RTC == 1
	t_rtc   r_time;
  #endif

  	if ((R_INT_KECON & 0x1) == 0) { /* power on reset */
	  #if _DRV_L1_RTC == 1
		r_time.rtc_sec = 0;
		r_time.rtc_min = 10;
		r_time.rtc_hour = 2;
		rtc_time_set(&r_time);
	  #endif
	}
}
#endif
extern INT32U    day_count;
INT32U system_rtc_counter_get(void)
{
    return (INT32U) day_count;
}

void system_rtc_counter_set(INT32U rtc_cnt)
{
    day_count = rtc_cnt;
}
/* place to internal ram (432byte)*/

#ifndef __CS_COMPILER__
#pragma arm section rwdata="pwr_ctrl", code="pwr_ctrl"
#else
void system_power_on_ctrl(void) __attribute__ ((section(".pwr_ctrl")));
void system_enter_halt_mode(void) __attribute__ ((section(".pwr_ctrl")));
void sys_ir_delay(INT32U t) __attribute__ ((section(".pwr_ctrl")));
#endif
INT32U    day_count = 0xFFFFFFFF;//2454829; /* julian date count */
INT32U    halt_data = 1;
INT32U    ir_opcode = 1;
INT32U	  Alarm_Trriger_By_Gp6 = 1;	
INT32U	  Day_Trriger_By_Gp6 = 1;

void sys_ir_delay(INT32U t)
{
	R_TIMERB_PRELOAD = (0x10000-t);

	R_TIMERB_CTRL |= 0x2000;
	while((R_TIMERB_CTRL & 0x8000) == 0);
  	R_TIMERB_CTRL &= ~0x2000;
}

void system_power_on_ctrl(void)
{
	INT32U  i;

	/* wakeup by RTC_DAY int */
	#if _DRV_L1_RTC == 1
	if (R_RTC_INT_STATUS & RTC_DAY_IEN) {
		R_RTC_INT_STATUS = 0x1f;
		day_count++; /* add 1 to julian date conut */
		if (ir_opcode != 0xFFFF0000) {
			system_enter_halt_mode();
		}
	}
	#endif
	
	if (ir_opcode != 0xFFFF0000) {	
		/* wakeup by extab */
		if (R_INT_IRQFLAG & INT_EXTAB) {
		  	//R_TIMERB_PRELOAD = 0xFFF8; /* 1 msec */
			//R_TIMERB_PRELOAD = 0xF999; /* 200 msec */
			//R_TIMERB_CTRL = 0x8063;/*8192 (122us per-count)*/
			R_TIMERB_CTRL = 0x8061;
			R_INT_KECON |= 0x40; /* clear exta int */
		 
			/* default IOF5 is input with pull low */
			//R_IOF_DIR &= ~0x20; /* IOF5 */
		  	sys_ir_delay(7519); /* 10 ms in 48MHz*/
		  	//sys_ir_delay(22557);
		  	for (i=0;i<22;i++) {
				if ((R_IOF_I_DATA & 0x20) == 0) {
					system_enter_halt_mode();
				}
				sys_ir_delay(752); /* 1 ms in 48MHz*/
			}
				
		    #if 0
  			cnt =0;
    		for (i=0; i<=200; i++) { /* press at least 200ms */
				if (cnt >= 10) {
					system_enter_halt_mode();
				}
    	
  				R_TIMERB_CTRL |= 0x2000;
  				while((R_TIMERB_CTRL & 0x8000) == 0);
  				R_TIMERB_CTRL &= ~0x2000;
    	
				if ((R_IOF_I_DATA & 0x20) == 0) {
					cnt++;
					i = 0;
				}
			}
		    #endif
		}
    }
	//R_MEM_IO_CTRL |= 0x0400;
	//R_MEM_SDRAM_CTRL0 |= 0x10; /* enable SDRAM */
	R_SYSTEM_CTRL &= ~0x20; /* strong 6M mode */
}

void system_enter_halt_mode(void)
{
	//R_MEM_IO_CTRL &= ~0x0400;	/*set SDRAM CS, IOF4,as gpio and output hi before halt*/
	//R_IOF_ATT |= 0x0010;
	//R_IOF_DIR |= 0x0010;
	//R_IOF_O_DATA &= ~0x10;
	//R_MEM_M11_BUS_PRIORITY |= 1;
	R_SYSTEM_PLL_WAIT_CLK = 0x100; /* set pll wait clock to 8 ms when wakeup*/
	//R_MEM_SDRAM_CTRL0 &= ~0x10; /* disable SDRAM */
	R_SYSTEM_CTRL &= ~0x2; /* CPU reset when wakeup */

    R_SYSTEM_HALT = 0x500A;
    halt_data = R_CACHE_CTRL;

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
}

//================================================================================
// 2016-04-26 liuxi
#if USED_MCU_NAME == MCU_GPDV6655D 
INT32S sys_pwr_key0_read(void)
{
	return (R_SYSTEM_POWER_CTRL1 & 0x10);
}

INT32S sys_pwr_key1_read(void)
{
	return (R_SYSTEM_POWER_CTRL1 & 0x20);
}

void sys_ldo12_ctrl(int voltage)
{
	// turn on/off LDO codec
	int reg = R_SYSTEM_LDO_CTRL;

	reg &= (~0x0070);
	reg |= voltage;
	R_SYSTEM_LDO_CTRL = reg;
}

void sys_ldo33_ctrl(int enable, int voltage)
{
	// turn on/off LDO codec
	int reg = R_SYSTEM_LDO_CTRL;

	reg &= (~0x0C00);
	reg |= voltage;
	R_SYSTEM_LDO_CTRL = reg;
	
	if (enable) {
		R_SYSTEM_POWER_CTRL1 |= 0x01;		// Enable 3.3V LDO
	}
	else {
		R_SYSTEM_POWER_CTRL1 &= ~0x01;		// Disable 3.3V LDO
	}
}

void sys_ldo28_ctrl(int enable, int voltage)
{
	// turn on/off sensor LDO
	int reg;
	
	reg = (R_SYSTEM_LDO_CTRL & (~0x1300)) | voltage;
	if (enable) {
		R_SYSTEM_LDO_CTRL = 0x1000 | reg;
	}
	else {
		R_SYSTEM_LDO_CTRL = reg;
	}
}
#endif
//================================================================================

#ifndef __CS_COMPILER__
#pragma arm section rwdata, code
#endif

void sys_ir_opcode_clear(void)
{
	ir_opcode = 0;	
}

INT32U sys_ir_opcode_get(void)
{
	return ir_opcode;	
}

void system_da_ad_pll_en_set(BOOLEAN status)
{
	if (status == TRUE) {
		R_SYSTEM_CLK_CTRL |= 0x10; /* DA/AD PLL Enable */
	}

	else {
		R_SYSTEM_CLK_CTRL &= ~0x10; /* DA/AD PLL FALSE */
	}
}

/* if the sref_cycle less, more easy(more slow) to entry self refresh latch mode */
INT32S sys_sdram_self_refresh_thread_cycle(INT8U sref_cycle)
{
    INT32U sdram_ctrl1_reg_val;
    sdram_ctrl1_reg_val = R_MEM_SDRAM_CTRL1;
    sdram_ctrl1_reg_val &= ~(0x00FF); /*clear [7:0]*/
    sdram_ctrl1_reg_val |= sref_cycle;
    if (sref_cycle!=0 && sref_cycle<64) {return STATUS_FAIL;}
    R_MEM_SDRAM_CTRL1 = sdram_ctrl1_reg_val;

    return STATUS_OK;
}

/* under 96MHz, SDRAM can support CL2 always. */
INT32S sys_sdram_CAS_latency_cycle(SDRAM_CL cl_cycle)
{
    INT32U sdram_ctrl1_reg_val;
    INT8U  cl_val;

    cl_val=(INT8U) cl_cycle;
    sdram_ctrl1_reg_val = R_MEM_SDRAM_CTRL1;
    sdram_ctrl1_reg_val &= ~(0x3000); /*clear [13:12]*/
    sdram_ctrl1_reg_val |= ((cl_val&0x3)<<12);
    if (cl_val<2) {return STATUS_FAIL;}

    R_MEM_SDRAM_CTRL1 = sdram_ctrl1_reg_val;
    return STATUS_OK;
}

/* when entry the latch mode, need RC cycle times to wake up */
INT32S sys_sdram_wakeup_cycle(INT8U RC_cycle)
{
    INT32U sdram_timing_reg_val;

    sdram_timing_reg_val = R_MEM_SDRAM_TIMING;
    RC_cycle &= 0xF;  /* fore bits effect */
    sdram_timing_reg_val &= ~(0xF);
    sdram_timing_reg_val |= RC_cycle;
    R_MEM_SDRAM_TIMING = sdram_timing_reg_val;

    return STATUS_OK;
}

/* large period time get more effect,default is 15600 ns (15.6us), don't less then it */
INT32S sys_sdram_refresh_period_ns(INT32U refresh_period_ns)
{
    INT32U cbrcyc_reg_val;

    //cbrcyc_reg_val = R_MEM_SDRAM_CBRCYC;

    if (refresh_period_ns < 15600) {return STATUS_FAIL;}

    cbrcyc_reg_val=(refresh_period_ns*MHZ/1000)&0xFFFF;
    R_MEM_SDRAM_CBRCYC = cbrcyc_reg_val;

    return STATUS_OK;
}

INT32S sys_sdram_init(SD_CLK_OUT_DELAY_CELL clk_out_dly_cell, SD_CLK_IN_DELAY_CELL clk_in_dly_cell,
                    SD_CLK_PHASE clk_phase, SD_BUS_WIDTH bus_mode, SD_BANK_BOUND bank_bound, 
                    SD_SIZE sd_size, SD_SWITCH disable_enable)
{
    INT32U sdram_ctrl0_val;

    bank_bound = SD_BANK_1M;

    if(clk_in_dly_cell!=SDRAM_IN_DLY_DISABLE)
    {
        sys_sdram_input_clock_dly_enable(1);
    } else {
        sys_sdram_input_clock_dly_enable(0);
        clk_in_dly_cell=SDRAM_IN_DLY_LV0;
    }

    sdram_ctrl0_val = R_MEM_SDRAM_CTRL0;
    sdram_ctrl0_val &= ~(0xFFEF);  /* keep sdram enable and disable status */

    sdram_ctrl0_val |= (((clk_out_dly_cell & 0xF))<<12 | ((clk_phase & 1)<<11) | ((clk_in_dly_cell&0x7)<<8)| ((bank_bound&0x1)<<5) |((disable_enable&0x1)<<4) | ((sd_size & 0x7)<<0));

    R_MEM_SDRAM_CTRL0 = sdram_ctrl0_val;

    return STATUS_OK;
}

INT32S sys_sdram_enable(SD_SWITCH sd_disable_enable)
{
    INT32U sdram_ctrl0_val;
    sdram_ctrl0_val = R_MEM_SDRAM_CTRL0;
    sdram_ctrl0_val &= ~(0x0010); /* clear bit4 */
    sdram_ctrl0_val |= (sd_disable_enable&0x1)<<4;
    R_MEM_SDRAM_CTRL0 = sdram_ctrl0_val;

    return STATUS_OK;
}


INT32S sys_sdram_refresh_period_set(tREFI_ENUM tREFI) {
    refresh_period=tREFI;
    return STATUS_OK;
}

INT32S sys_sdram_MHZ_set(SDRAM_CLK_MHZ sdram_clk)
{

    INT32U cbrcyc_reg_val;
    INT32U oc=1; /* Internal test for overing clock */

    //cbrcyc_reg_val = R_MEM_SDRAM_CBRCYC;

    cbrcyc_reg_val=(refresh_period*sdram_clk/10)&0xFFFF * oc;

    R_MEM_SDRAM_CBRCYC = cbrcyc_reg_val;

    return STATUS_OK;
}

void sys_weak_6M_set(BOOLEAN status)
{
	if (status == TRUE) {
		R_SYSTEM_CTRL |= 0x20; /* weak 6M mode */
	}
	else {
		R_SYSTEM_CTRL &= ~0x20; /* strong 6M mode */
	}
}

void sys_reg_sleep_mode_set(BOOLEAN status)
{
	if (status == TRUE) {
		R_SYSTEM_CTRL |= 0x80; /* enable reg sleep mode */
	}
	else {
		R_SYSTEM_CTRL &= ~0x80; /* disable reg sleep mode */
	}
}

void sys_sdram_auto_refresh_set(INT8U cycle)
{
	R_MEM_SDRAM_CTRL1 &= ~0xFF;
	R_MEM_SDRAM_CTRL1 |= cycle;
}

INT32S sys_sdram_read_latch_at_CLK_Neg(INT8U ebable_disable)
{
    INT32U sdram_misc_val;

    sdram_misc_val = R_MEM_SDRAM_MISC;

    if (ebable_disable==0) {
        sdram_misc_val &= ~(0x2);
    } else {
        sdram_misc_val |= 0x2;
    }
    sdram_misc_val =0x6;
    R_MEM_SDRAM_MISC = sdram_misc_val;
    return STATUS_OK;
}

void sys_sdram_input_clock_dly_enable(INT8U ebable_disable)
{
    INT32U sdram_misc_reg_val;

    sdram_misc_reg_val = R_MEM_SDRAM_MISC;

    if (ebable_disable==0) {
        sdram_misc_reg_val &= ~(0x4);
    } else {
        sdram_misc_reg_val |= (0x4);
    }
    R_MEM_SDRAM_MISC = sdram_misc_reg_val;
}

//=== This is for code configuration DON'T REMOVE or MODIFY it ===//
#endif //(defined _DRV_L1_SYSTEM) && (_DRV_L1_SYSTEM == 1)        //
//================================================================//
