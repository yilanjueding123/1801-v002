#ifndef __DRIVER_L2_CFG_H__
#define __DRIVER_L2_CFG_H__

#define DRV_L2_ON			    	1
#define DRV_L2_OFF			  		0
#define _DRV_L2_SYS_TIMER_EN		DRV_L2_ON
#define _DRV_L2_IR   	            DRV_L2_OFF
#define _DRV_L2_KEYSCAN	            DRV_L2_OFF
#define _DRV_L2_EXT_RTC             DRV_L2_OFF
#define _DRV_L2_ITOUCH	            DRV_L2_OFF
#define _DRV_L2_NAND	            DRV_L2_ON
#define _DRV_L2_USBH                _DRV_L1_USBH
#define _DRV_L2_SPI                 DRV_L2_ON
   
#if _DRV_L2_ITOUCH==DRV_L2_ON
	#define	ITOUCH_SCL       IO_E0
	#define	ITOUCH_SDA       IO_E1
#endif

#endif		// __DRIVER_L2_CFG_H__
