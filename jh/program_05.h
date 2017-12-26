#ifndef __PROGRAM_05_H__
#define __PROGRAM_05_H__
//===============================================================================================
#include "project.h"
#include "jh_define.h"

extern volatile INT8U  g_bonding_test_mode;
extern volatile INT32U g_bonding_test_option;
extern volatile INT16U g_bonding_test_result;
extern volatile INT32U g_bonding_test_adc_value;
extern volatile INT8U  g_bonding_test_adc_flag;

extern void bonding_test_service(void);
//===============================================================================================
#define C_BOND_OPTION_MODE_SELECT					0x00000001<<0
#define C_BOND_OPTION_SENSOR_MIRROR					0x00000001<<1
#define C_BOND_OPTION_PRODUCT_NUM					0x00000001<<2
#define C_BOND_OPTION_PROGRAM_VERSION				0x00000001<<3
#define C_BOND_OPTION_RTC							0x00000001<<4
#define C_BOND_OPTION_LINK_USB_MUST_READ_PIN     	0x00000001<<5
#define C_BOND_OPTION_WRITER_DETECT 		   	  	0x00000001<<6
#define C_BOND_OPTION_LINE0_WITH_FLOAT         	  	0x00000001<<7
#define C_BOND_OPTION_LINE0_WITH_PL		  	    	0x00000001<<8
#define C_BOND_OPTION_LINE0_WRITE_FILE	  	    	0x00000001<<9
#define C_BOND_OPTION_ERROR_TO_POWER_OFF	     	0x00000001<<10
#define C_BOND_OPTION_AUTO_STOP_RECORD		     	0x00000001<<11
#define C_BOND_OPTION_MANUAL_STOP_TIME_LIMIT	   	0x00000001<<12
#define C_BOND_OPTION_AUTO_LED_1			     	0x00000001<<13
#define C_BOND_OPTION_AUTO_LED_2			     	0x00000001<<14
#define C_BOND_OPTION_AUTO_LED_3			     	0x00000001<<15
#define C_BOND_OPTION_AUTO_LED_4			     	0x00000001<<16
#define C_BOND_OPTION_AUTO_KEY_1			     	0x00000001<<17
#define C_BOND_OPTION_AUTO_KEY_2			     	0x00000001<<18
#define C_BOND_OPTION_AUTO_KEY_3			     	0x00000001<<19
#define C_BOND_OPTION_AUTO_KEY_4			     	0x00000001<<20
#define C_BOND_OPTION_AUTO_LED_1_LEVEL		     	0x00000001<<21
#define C_BOND_OPTION_AUTO_LED_2_LEVEL		     	0x00000001<<22
#define C_BOND_OPTION_AUTO_LED_3_LEVEL		     	0x00000001<<23
#define C_BOND_OPTION_AUTO_LED_4_LEVEL		     	0x00000001<<24
#define C_BOND_OPTION_AUTO_KEY_1_LEVEL		     	0x00000001<<25
#define C_BOND_OPTION_AUTO_KEY_2_LEVEL		     	0x00000001<<26
#define C_BOND_OPTION_AUTO_KEY_3_LEVEL		     	0x00000001<<27
#define C_BOND_OPTION_AUTO_KEY_4_LEVEL		     	0x00000001<<28
#define C_BOND_OPTION_POWER_CTRL_PIN		     	0x00000001<<29	//2012-3-20 11:45


typedef enum
{
	C_B_TEST_OK = 0x00,									//00: °î¶¨²âÊÔOK
	C_B_TEST_ERROR_PRODUCT_NUM,							//01: ²úÆ·±àºÅ´íÎó
	C_B_TEST_ERROR_PROGRAM_VERSION,						//02: Èí¼þ°æ±¾´íÎó
	C_B_TEST_ERROR_TEST_FILE_END_FLAG,					//03: ²âÊÔÎÄ¼þ½áÊø±êÖ¾´íÎó
	C_B_TEST_ERROR_RTC,									//04: RTC´íÎó
	C_B_TEST_ERROR_USB_DETECT_PIN,						//05: USB¼ì²â½Å´íÎó
	C_B_TEST_ERROR_WRITER_DETECT_PIN,					//06: IO_C5´íÎó
	C_B_TEST_ERROR_LINE_0_PIN_WITH_FLOAT,				//07: IO_F7´íÎó
	C_B_TEST_ERROR_LINE_0_PIN_WITH_PL,					//08: IO_F7´íÎó
	C_B_TEST_ERROR_LED_1,								//09: LED_1´íÎó
	C_B_TEST_ERROR_LED_2,								//10: LED_2´íÎó
	C_B_TEST_ERROR_LED_3,								//11: LED_3´íÎó
	C_B_TEST_ERROR_LED_4,								//12: LED_4´íÎó
	C_B_TEST_ERROR_KEY_1,								//13: KEY_1´íÎó
	C_B_TEST_ERROR_KEY_2,								//14: KEY_2´íÎó
	C_B_TEST_ERROR_KEY_3,								//15: KEY_3´íÎó
	C_B_TEST_ERROR_KEY_4,								//16: KEY_4´íÎó
	C_B_TEST_ERROR_POWER_CTRL_PIN						//17: ´íÎó:µçÔ´¿ØÖÆ½ÅÓë3.3V¶ÌÂ·	//2012-3-20 11:45
	
}BONDING_TEST_ERROR_LIST;
//===============================================================================================
#endif 		// __PROGRAM_05_H__
