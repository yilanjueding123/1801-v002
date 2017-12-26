#ifndef __CUSTOMER_H__
#define __CUSTOMER_H__

#include "jh_define.h"

#define CUSTOM_ON       1
#define CUSTOM_OFF      0

#define USED            1
#define UNUSED          0

//for debug use
#define DBG_MESSAGE		CUSTOM_OFF

//configure
#define GPL32680_MINI_DVR_EMU_64PIN			0
#define GPL32680_MINI_DVR_EMU_128PIN		1
#define GPL32680_MINI_DVR_EMU_V2_0			2
#define GPL32680_MINI_DVR_DEMO_BOARD		3
#define GPL32680_MINI_DVR_PEN_BONDING		4
#define GPL32680_MINI_DVR_PEN_QFN			5
#define GPL32680_MINI_DVR_KEYCHAIN_BONDING	6
#define GPL32680_MINI_DVR_KEYCHAIN_QFN		7
#define MINI_DVR_BOARD_VERSION				GPL32680_MINI_DVR_EMU_128PIN

#define AVI_FRAME_RATE		30
#define QUALITY_FACTOR		50		//录像Jpeg压缩Q值

#define AVI_WIDTH			720	//720x480
//#define AVI_WIDTH			640	//640x480

#define AVI_HEIGHT			480
#define PIC_WIDTH			1280	//1600
#define PIC_HEIGH			960//1024	//1200
#define PIC_Q_VALUE			90		//拍照Jpeg压缩Q值
#define TFT_WIDTH			320
#define TFT_HEIGHT			240

//mini DVR storage type
#define NAND_FLASH						0
#define T_FLASH							2
#define MINI_DVR_STORAGE_TYPE			T_FLASH

//motion detect sensitive
#define LOW								300		//total is 1198
#define MEDIUM							150
#define HIGH							70
#define MD_SENSITIVE					MEDIUM

//audio record sampling rate
#define AUD_SAMPLING_RATE_8K			8000
#define AUD_SAMPLING_RATE_16K			16000
#define AUD_SAMPLING_RATE_22K			22050
#define AUD_SAMPLING_RATE_44K			44100
#define AUD_REC_SAMPLING_RATE			AUD_SAMPLING_RATE_44K

//mic phone input source
#define C_ADC_LINE_IN					0
#define C_GPY0050_IN					1
#define C_BUILDIN_MIC_IN				2  //only GPL32600 support
#define MIC_INPUT_SRC					C_BUILDIN_MIC_IN

#define UNUSED_IO_PIN					IO_D15
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ IO Config
#define C_POWER_KEY_SHORT				1
#define C_POWER_KEY_LONG				2
//#define C_FUNCTION_KEY_SHORT			3
//#define C_FUNCTION_KEY_LONG			4
//======================================================
#define	KEY_1_CONFIG					USED
#define	KEY_2_CONFIG					UNUSED
#define	KEY_3_CONFIG					UNUSED
#define	KEY_4_CONFIG					UNUSED

#define	LED_1_CONFIG					USED
#define	LED_2_CONFIG					USED
#define	LED_3_CONFIG					UNUSED
#define	LED_4_CONFIG					UNUSED
//======================================================
#define KEY_1_PIN						IO_G0 //Power Key
#define KEY_2_PIN						UNUSED_IO_PIN
#define KEY_3_PIN						UNUSED_IO_PIN
#define KEY_4_PIN						UNUSED_IO_PIN

#define LED_1_PIN						IO_C10//IO_G8	//red
#define LED_2_PIN						IO_F5//IO_F2	//green
#define LED_3_PIN						UNUSED_IO_PIN
#define LED_4_PIN						UNUSED_IO_PIN
//======================================================
#define	KEY_1_TRIGGER_LEVEL				1
#define	KEY_2_TRIGGER_LEVEL				0
#define	KEY_3_TRIGGER_LEVEL				0
#define	KEY_4_TRIGGER_LEVEL				0

#define	LED_1_TRIGGER_LEVEL				1
#define	LED_2_TRIGGER_LEVEL				1
#define	LED_3_TRIGGER_LEVEL				0
#define	LED_4_TRIGGER_LEVEL				0
//======================================================
#define	POWER_KEY						KEY_1_PIN

#define C_USBDEVICE_PIN   				IO_G1
#define POWER_CTRL_PIN					UNUSED_IO_PIN
#define PJ4054_READ_PIN					IO_H5//POWER_CTRL_PIN
#define SCCB_SCL    					IO_G8//IO_G9
#define SCCB_SDA      					IO_G9//IO_G8
//======================================================
#define	USB_DISABLE_KEY_CONFIG			USED
#define	USB_DISABLE_KEY_PIN				KEY_1_PIN
#define	USB_DISABLE_KEY_TRIGGER_LEVEL	KEY_1_TRIGGER_LEVEL

#define PC_CAMERA_KEY_CONFIG			USED
#define PC_CAMERA_KEY_PIN				KEY_1_PIN
#define PC_CAMERA_KEY_TRIGGER_LEVEL		KEY_1_TRIGGER_LEVEL
//======================================================
#define SENSOR_MIRROR_ENABLE			0
#define C_IDLE_SLEEP					100*60	//60秒钟
//---------------------------------------------------------------- IO Config

//function
#define C_UVC										CUSTOM_ON	//PC Camera 功能是否开启
#define C_USB_AUDIO						            CUSTOM_ON	//USB Mic 功能是否开启
#define C_MOTION_DETECTION							CUSTOM_OFF	//移动侦测 功能是否开启
#define C_AUTO_DEL_FILE								CUSTOM_OFF	//自动删档 功能是否开启
#define C_CYCLIC_VIDEO_RECORD						CUSTOM_OFF	//循环录影 功能是否开启
#define C_VIDEO_PREVIEW								CUSTOM_OFF	//only for 行车记录仪: TFT显示摄像头数据 功能是否开启
#define C_LOGO										CUSTOM_OFF	//only for 行车记录仪: 开机时TFT显示logo画面

#define C_USB_DET_ENABLE 				CUSTOM_ON

//define video encode mode
#define VIDEO_ENCODE_WITH_PPU_IRQ		0
#define VIDEO_ENCODE_WITH_TG_IRQ		1	//only gpl32600 support
#define VIDEO_ENCODE_WITH_FIFO_IRQ		2	//only gpl32600 support
#define VIDEO_ENCODE_USE_MODE			VIDEO_ENCODE_WITH_TG_IRQ

//audio record digital gain
#define DIGITAL_GAIN_1X                 0
#define DIGITAL_GAIN_2X                 1
#define DIGITAL_GAIN_4X                 2
#define DIGITAL_GAIN_6X                 3
#define DIGITAL_GAIN_8X                 4
#define AUDIO_RECORD_DIGITAL_GAIN       DIGITAL_GAIN_1X
 
#define SENSOR_OV7725_ENABLE			0
#define SENSOR_OV7670_ENABLE			1	//已经更新 OV7670 的初始化程序到 v2.00版本;
#define SENSOR_PAS6329_ENABLE			1
#define SENSOR_GC0308_ENABLE			1
#define SENSOR_BF3703_ENABLE			1
#define SENSOR_BF3603_ENABLE			1
#define SENSOR_BF3A03_ENABLE			1
#define SENSOR_OV7680_ENABLE			0
#define SENSOR_OV2640_ENABLE			1

#endif //__CUSTOMER_H__
