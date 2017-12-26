#include "program_04.h"
#include "application.h"
#include "mini_dvr_api.h"
#include "program_03.h"

//===============================================================================================
void red_on(void)
{
	led_1_on();
}

void red_off(void)
{
	led_1_off();
}

void green_on(void)
{
	led_2_on();
}

void green_off(void)
{
	led_2_off();
}

void led_reversal(void)
{
	static INT8U t=0;
	t ^= 1;
	if (t) red_on();
	else red_off();
}

void bonding_led_on(void)
{
	green_on();
}

void bonding_led_off(void)
{
	green_off();
}
//===============================================================================================
void F_power_Pin_Enable(void)
{
  	gpio_init_io(POWER_CTRL_PIN, GPIO_OUTPUT);
  	gpio_set_port_attribute(POWER_CTRL_PIN, ATTRIBUTE_HIGH);
  	gpio_write_io(POWER_CTRL_PIN, DATA_HIGH);
}

void F_power_Pin_Disable(void)
{
	red_off();
	green_off();
	while(gpio_read_io(POWER_KEY));
	while (1)
	{
		power_ctrl_low(1);
		OSTimeDly(100);
	}
}

#if USED_MCU_NAME == MCU_GPDV6655D
void sys_ldo_off(void)
{
	sys_ldo28_ctrl(0,LDO_28_31V);
	sys_ldo33_ctrl(0,LDO_33_33V);
}
#endif

void power_ctrl_low(INT8U flag)
{
  	gpio_init_io(POWER_CTRL_PIN, GPIO_OUTPUT);
  	gpio_set_port_attribute(POWER_CTRL_PIN, ATTRIBUTE_HIGH);
  	gpio_write_io(POWER_CTRL_PIN, DATA_LOW);

	#if USED_MCU_NAME == MCU_GPDV6655D
	if (flag) sys_ldo_off();
	#endif
}
//===============================================================================================
void no_disk_handle(void)
{
	green_off();
	OSTimeDly(30);
	F_Show_Error_And_WaitKey(6);	//闪烁 6 次	
	F_power_Pin_Disable();
}

void disk_no_size_handle(void)
{
	green_off();
	OSTimeDly(30);
	F_Show_Error_And_WaitKey(9);	//闪烁 9 次	
	F_power_Pin_Disable();
}
//===============================================================================================
void F_Show_Error_And_WaitKey(INT32U led_cnt)
{
	INT16U timeover = 0;
	INT16U cnt, flag;
	cnt = 1000;
	flag = 0;
	R_KeyCode = 0;
	while (1)
	{
		OSTimeDly(1);
		function_key_scan();
		if ((R_KeyCode==C_POWER_KEY_SHORT) || (R_KeyCode==C_POWER_KEY_LONG))
		{
			break;
		}
		if (++cnt >= 30)	//300ms
		{
			cnt = 0;
			flag ^= 1;
			if (flag)
			{
				red_on();
			}
			else
			{
				red_off();
				if (++timeover >= led_cnt) break;
			}
		}
	}
}
//下面这个函数和F_Show_Error_And_WaitKey是完全一样的, 除了闪烁不同颜色的 led 之外;
void F_Show_OK_And_WaitKey(INT32U led_cnt)
{
	INT16U timeover = 0;
	INT16U cnt, flag;
	cnt = 1000;
	flag = 0;
	R_KeyCode = 0;
	while (1)
	{
		OSTimeDly(1);
		function_key_scan();
		if ((R_KeyCode==C_POWER_KEY_SHORT) || (R_KeyCode==C_POWER_KEY_LONG))
		{
			break;
		}
		if (++cnt >= 30)	//300ms
		{
			cnt = 0;
			flag ^= 1;
			if (flag)
			{
				green_on();
			}
			else
			{
				green_off();
				if (++timeover >= led_cnt) break;
			}
		}
	}
}
//===============================================================================================
