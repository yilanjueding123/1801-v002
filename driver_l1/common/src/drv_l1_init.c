/*
* Purpose: system initial functions after reset
*
* Author: dominat
*
* Date: 2007/10/29
*
* Copyright Generalplus Corp. ALL RIGHTS RESERVED.
*
* Version : 1.00
* History :

*/
//Include files
#include "driver_l1.h"
#include "drv_l1_system.h"

void drv_l1_init(void);
void drv_l1_reinit(void);

void drv_l1_init(void)
{
	// First initiate those settings that affect system, like: SDRAM access, system clock, cache, exception handler, interrupt, watchdog
	// Then initiate those hardware modules that are bus masters, like: DMA, JPEG, scaler, PPU, SPU, TFT
	// Finally, initiate other hardware modules
	//================================================================================
	//仅GPDV6655D有内部LDO <20160426  liuxi>
	#if USED_MCU_NAME == MCU_GPDV6655D
	sys_ldo12_ctrl(LDO_12_13V);
	#if LDO_ENABLE == 1
		sys_ldo33_ctrl(1, LDO_33_33V);			// System LDO: On, voltage=3.0V, 3.1V, 3.2V, 3.3V	
  	#else
		sys_ldo33_ctrl(1, LDO_33_30V);			// System LDO: On, voltage=3.0V, 3.1V, 3.2V, 3.3V	
	#endif
	sys_ldo28_ctrl(1, LDO_28_31V);			// Sensor LDO: On, voltage=2.8V, 2.9V, 3.0V, 3.1V
	#endif
	//================================================================================
  #if _DRV_L1_MEMC==1
////	memc_init();						// Initiate Memory controller
  #endif
//Don't set SDRAM again while freerun
  #if  _DRV_L1_SYSTEM == 1
//    system_init();
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#if USED_MCU_NAME != MCU_GPDV6655B
	(*((volatile INT32U *) 0xD0000090)) &= ~0x0F;			// Reset XCKGen
    __asm {
	NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP	
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	};	
    (*((volatile INT32U *) 0xD0000090)) |= 0x08;			// Enable XCKGen
    __asm {
	NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP	
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	};
#endif	
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#if MINI_DVR_BOARD_VERSION == GPL32680_MINI_DVR_EMU_V2_0 || MINI_DVR_BOARD_VERSION == GPL32680_MINI_DVR_DEMO_BOARD
////	R_MEM_SDRAM_CTRL0=0x00005353;
#else	//for EVB
	//R_MEM_SDRAM_CTRL0=0x6411;//0x00004051;
#endif

	/*R_MEM_SDRAM_CTRL1=0x00002000;
	R_MEM_SDRAM_TIMING=0x00000F8A;
	R_MEM_SDRAM_CBRCYC=0x000005D9;
	R_MEM_SDRAM_MISC=0x00000006;*/

   
	system_set_pll(MHZ);				// Set PLL
#if MINI_DVR_BOARD_VERSION != GPL32680_MINI_DVR_EMU_V2_0 && MINI_DVR_BOARD_VERSION != GPL32680_MINI_DVR_DEMO_BOARD
////	system_sdram_driving();			// Set SDRAM driving  // domi mark, put it into system_init()
#endif
	system_bus_arbiter_init();			// Initaile bus arbiter
  #endif

  #if _DRV_L1_CACHE == 1
	cache_init();						// Initiate CPU Cache
  #endif

	exception_table_init();				// Initiate CPU exception handler
  #if _DRV_L1_INTERRUPT == 1
	vic_init();							// Initiate Interrupt controller
  #endif

  #if _DRV_L1_WATCHDOG==1
	watchdog_init();					// Initiate WatchDOg
  #endif

  #if _DRV_L1_DMA == 1
	dma_init();							// Initiate DMA controller
  #endif

  #if _DRV_L1_PPU == 1
	ppu_init();							// Initiate PPU engine
  #endif

  #if _DRV_L1_JPEG == 1
	jpeg_init();						// Initiate JPEG engine
  #endif

  #if _DRV_L1_SCALER == 1
	scaler_init();						// Initiate Scaler engine
  #endif

  #if _DRV_L1_GPIO==1
	gpio_init();						// Initiate GPIO and IO PADs
  #endif

  #if _DRV_L1_TFT==1
//	tft_init();							// Initiate TFT controller
  #endif

  #if _DRV_L1_SPU==1
//	SPU_Initial();							// Initiate SPU
  #endif

  #if _DRV_L1_UART==1
	#if BOARD_TYPE < BOARD_TURNKEY_BASE
						// Initiate UART
      #if (defined CONSOLE_DBG_MSG) && (CONSOLE_DBG_MSG==CUSTOM_ON)  /* jandy add , 2009/01/12 */
      uart0_init();	
    	uart0_buad_rate_set(UART0_BAUD_RATE);
     	uart0_tx_enable();
     	uart0_rx_enable();
      #endif
    #endif
  #endif

  #if _DRV_L1_SPI==1
	spi_disable(0);						// Initiate SPI
  #endif

  #if _DRV_L1_KEYSCAN==1
	//matre_keyscaninit();
  #endif

  #if _DRV_L1_EXT_INT==1
	ext_int_init();
  #endif

  #if _DRV_L1_DAC==1
	dac_init();						// Initiate DAC
  #endif

  #if _DRV_L1_ADC==1
	//adc_init();						// Initiate Analog to Digital Convertor
  #endif

  #if _DRV_L1_TIMER==1
	timer_init();						// Initiate Timer
	timerD_counter_init();              // Tiny Counter Initial (1 tiny count == 2.67 us)
  #endif

 #if _DRV_L1_RTC==1
	rtc_init();							// Initiate Real Time Clock
#if MINI_DVR_BOARD_VERSION > GPL32680_MINI_DVR_EMU_V2_0 && MINI_DVR_BOARD_VERSION != GPL32680_MINI_DVR_DEMO_BOARD
	gpx_rtc_write(8,0x28);
#endif
	/////////////////////////////////////////////////////////////
/*	R_MEM_IO_CTRL |= 0x8861;
	gpio_init_io(IO_F1,GPIO_OUTPUT);
gpio_set_port_attribute(IO_F1, ATTRIBUTE_HIGH);
gpio_write_io(IO_F1,1);
gpio_init_io(IO_F5,GPIO_OUTPUT);
gpio_set_port_attribute(IO_F5, ATTRIBUTE_LOW);
while(1){
gpio_write_io(IO_F5,0);
	}*/
  #endif

  #if _DRV_L1_SW_TIMER== 1
   #if _DRV_L1_RTC==1
	sw_timer_init(TIMER_RTC, 1024);
   #endif
  #endif
  
  #if _DRV_L1_NAND==1	
#if MINI_DVR_STORAGE_TYPE == NAND_FLASH
	DrvL1_Nand_Init();				// Initiate NAND, //for FPGA
	nand_wp_pin_reg(NAND_WP_PIN);
#endif
  #endif
    
  #if (defined SUPPORT_JTAG) && (SUPPORT_JTAG == CUSTOM_OFF)
	gpio_set_ice_en(FALSE);  
  #endif

#if MINI_DVR_BOARD_VERSION == GPL32680_MINI_DVR_EMU_64PIN
 	R_SYSTEM_CLK_EN1 &= 0x7FFF; //close system control for enter idle task
	R_SYSTEM_CLK_EN1 = 0x4C9C;
#else	


#if DBG_MESSAGE == CUSTOM_ON

 	R_SYSTEM_CLK_EN1 &= 0x7FFF; //close system control for enter idle task


#else
	R_SYSTEM_CLK_EN0 = 0x863F;
//	R_SYSTEM_CLK_EN1 = 0x4C98;
	
	R_SYSTEM_CLK_EN1 = 0xDC9C;	//小机
	//R_SYSTEM_CLK_EN1 = 0x4C9C;	//开发板
//	*P_USBD_CONFIG1 |= 0x100;
//	R_SYSTEM_CLK_EN1 = 0xD498;
#endif


#endif


}


void drv_l1_reinit(void)
{
	// First initiate those settings that affect system, like: SDRAM access, system clock, cache, exception handler, interrupt, watchdog
	// Then initiate those hardware modules that are bus masters, like: DMA, JPEG, scaler, PPU, SPU, TFT
	// Finally, initiate other hardware modules

  #if _DRV_L1_SYSTEM == 1
    system_init();
    system_set_pll(MHZ);				// Set PLL
    //system_sdram_driving();			// Set SDRAM driving  // domi mark, put it into system_init()
    system_bus_arbiter_init();			// Initaile bus arbiter
  #endif

  #if _DRV_L1_CACHE == 1
	cache_init();						// Initiate CPU Cache
  #endif

	exception_table_init();				// Initiate CPU exception handler
  #if _DRV_L1_INTERRUPT == 1
	vic_init();							// Initiate Interrupt controller
  #endif

  #if _DRV_L1_WATCHDOG==1
	watchdog_init();					// Initiate WatchDOg
  #endif

  #if _DRV_L1_DMA == 1
	dma_init();							// Initiate DMA controller
  #endif

  #if _DRV_L1_PPU == 1
	ppu_init();							// Initiate PPU engine
  #endif

  #if _DRV_L1_JPEG == 1
	jpeg_init();						// Initiate JPEG engine
  #endif

  #if _DRV_L1_SCALER == 1
	scaler_init();						// Initiate Scaler engine
  #endif

  #if _DRV_L1_GPIO==1
	gpio_init();						// Initiate GPIO and IO PADs
  #endif


  #if _DRV_L1_UART==1
	#if BOARD_TYPE < BOARD_TURNKEY_BASE
    uart0_init();						// Initiate UART
      #if (defined CONSOLE_DBG_MSG) && (CONSOLE_DBG_MSG==CUSTOM_ON)  /* jandy add , 2009/01/12 */
     	uart0_buad_rate_set(UART0_BAUD_RATE);
     	uart0_tx_enable();
     	uart0_rx_enable();
      #endif
    #endif
  #endif

  #if _DRV_L1_SPI==1
	spi_disable(0);						// Initiate SPI
  #endif

  #if _DRV_L1_KEYSCAN==1
	//matre_keyscaninit();
  #endif

  #if _DRV_L1_EXT_INT==1
    ext_int_init();
  #endif

  #if _DRV_L1_TIMER==1
	timer_init();						// Initiate Timer
    timerD_counter_init();              // Tiny Counter Initial (1 tiny count == 2.67 us)
  #endif

 #if _DRV_L1_RTC==1
	rtc_init();							// Initiate Real Time Clock
  #endif

  #if _DRV_L1_SW_TIMER== 1
   #if _DRV_L1_RTC==1
    sw_timer_init(TIMER_RTC, 1024);
   #endif
  #endif
  
  #if _DRV_L1_NAND==1
     DrvL1_Nand_Init();				// Initiate NAND
  #endif
  
  #if BOARD_TYPE == BOARD_TK_V4_0
  	/* iog0 for spy0030 */
  	gpio_set_bkueb(FALSE);
  	gpio_init_io(IO_G0, GPIO_OUTPUT);
  	gpio_set_port_attribute(IO_G0, 1);
  	gpio_write_io(IO_G0, DATA_HIGH);
  	/* enable TFT panel */
  	gpio_set_port_attribute(IO_B4,1);
  	gpio_init_io(IO_B4, GPIO_OUTPUT);
  	gpio_write_io(IO_B4, DATA_HIGH);
  #elif BOARD_TYPE == BOARD_TK_V4_1
  	/* enable TFT panel */
  	gpio_set_port_attribute(IO_F9,1);
  	gpio_init_io(IO_F9, GPIO_OUTPUT);
  	gpio_write_io(IO_F9, DATA_HIGH);
  #endif

}
