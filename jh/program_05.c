#include "program_05.h"
#include "mini_dvr_api.h"
#include "program_01.h"
#include "program_02.h"
#include "program_04.h"
#include "application.h"
#include "stdio.h"
#include "state_usb.h"

volatile INT8U  g_bonding_test_mode;
volatile INT32U g_bonding_test_option;
volatile INT16U g_bonding_test_result;
volatile INT32U g_bonding_test_adc_value;
volatile INT8U  g_bonding_test_adc_flag;
volatile static INT8U test_rtc_flag;
volatile static INT8U test_adc_index;

void bonding_test_main(void);
void bonding_test_main_pccamera(void);
void bonding_test_main_dvr(void);
void b_test_adc_0(void);
void b_test_adc_1(void);
void b_test_adc_2(void);
void b_test_rtc_0(void);
void b_test_rtc_1(void);
void b_test_rtc_2(void);
void write_error_code(INT16U value);
void bonding_test_error(void);

extern TIME_T	g_current_time;
extern INT8U gpx_rtc_read(INT8U addr);	//GP15 vs GP10: same
extern void sensor_mirror_BF3703(void);
extern void sensor_mirror_PAS6329(void);
extern void bonding_test_adc_start(void);
extern void io_led_key_init(void);

/*
测试条目:                     PCCamera test                 DVR test
<1> 产品编号和软件版本;            y                          y
<2> 所有按键;                      y                          y
<3> 所有led;                       y                          y
<4> rtc:                           y                          y
<5> ADC_LINE_0: 检测ADC            -                          y
<6> USB检测脚:                     y                          y
<7> IOC5烧录脚:                    y                          -
<8> ADC_LINE_0: 检测IO             y                          -
<9> POWER_CTRL_PIN:                y                          y
*/

void bonding_test_service(void)
{
	INT16S fd;
	INT32U addr;
	INT32S nRet;
	INT8U *p;
	INT32U t;
	INT32U product_num;
	INT32U product_ver;

	fd = open("C:\\jh_test.txt", O_RDONLY);
	if(fd < 0)
	{
		return;
	}
	addr = (INT32U)gp_malloc(60);
	if(!addr)
	{
		close(fd);
		return;
	}
	p = (INT8U*)addr;
	for (t=0; t<60; t++) p[t] = 0;
	read(fd, addr, 56);
	close(fd);
	nRet = gp_strncmp((INT8S*)(addr+15), (INT8S *)"select", 6);	//返回0表示参数1和参数2的内容完全相同;
	if (nRet!=0)
	{
		gp_free((void*) addr);
		return;
	}
	g_bonding_test_mode = 1;
	//R_SYSTEM_PLLEN |= 0x100;	//system 32KHz clock from external 32768Hz crystal.
	p += 23;
	t = 0x01;
	while (t)
	{
		if (*p == 'y') g_bonding_test_option |= t;
		t <<= 1;
		p++;
	}
	//--------------------------------------------------
	p = (INT8U*)addr;
	if (p[55] != '$') write_error_code(C_B_TEST_ERROR_TEST_FILE_END_FLAG);
	//--------------------------------------------------
	if ((g_bonding_test_option&C_BOND_OPTION_PRODUCT_NUM) == 0)
	{
		//测试产品编号:
		product_num  = (p[2]-'0')*1000;
		product_num += (p[3]-'0')*100;
		product_num += (p[4]-'0')*10;
		product_num += (p[5]-'0')*1;
		if (product_num != PRODUCT_NUM) write_error_code(C_B_TEST_ERROR_PRODUCT_NUM);
	}
	if ((g_bonding_test_option&C_BOND_OPTION_PROGRAM_VERSION) == 0)
	{
		//测试软件版本:
		product_ver  = (p[8]-'0')*100;
		product_ver += (p[10]-'0')*10;
		product_ver += (p[11]-'0')*1;
		if (product_ver != PROGRAM_VERSION_NUM) write_error_code(C_B_TEST_ERROR_PROGRAM_VERSION);
	}
	gp_free((void*) addr);
	//--------------------------------------------------
	//sensor画面翻转180度:
	if ((g_bonding_test_option&C_BOND_OPTION_SENSOR_MIRROR) != 0)
	{
		//if (g_sensor_id==SENSOR_CHIP_BF3703) sensor_mirror_BF3703();
		//if (g_sensor_id==SENSOR_CHIP_BF3603) sensor_mirror_BF3703();
		//if (g_sensor_id==SENSOR_CHIP_PAS6329) sensor_mirror_PAS6329();
	}
	//--------------------------------------------------
	bonding_test_main();
	while(1);
}
//===============================================================================================
void bonding_test_main(void)
{
	bonding_led_off();
	b_test_rtc_0();
	b_test_adc_0();
	//OSTimeDly(8);
	//--------------------------------
	//unlink( "C:\\00001111_TEST.avi" );
	#if CUSTOMIZE_FOR_DIWEI==1
	unlink("C:\\TAG.TXT");
	_deleteall("C:\\DCIM");	//xiangyong 2012-4-9 16:50
	rmdir("C:\\DCIM");
	#else
	unlink("C:\\time.txt");
	_deleteall("C:\\PHOTO");
	rmdir("C:\\PHOTO");
	_deleteall("C:\\VIDEO");
	rmdir("C:\\VIDEO");
	#endif
	//--------------------------------
	//===============================================================================================
	//===============================================================================================
	//===============================================================================================
	b_test_rtc_1();
	b_test_adc_1();
	//--------------------------------------------------
	b_test_rtc_2();
	b_test_adc_2();
	bonding_led_off();
	R_KeyCode = 0;
	//--------------------------------------------------
	if (g_bonding_test_result != C_B_TEST_OK)
	{
		bonding_test_error();	//这一个函数不会用到 128Hz 中断;
		if ((g_bonding_test_option&C_BOND_OPTION_ERROR_TO_POWER_OFF) == 0)
		{
			while (1) bonding_test_error();	//即使按键, 也反复快速闪灯
		}
		g_bonding_test_option |= C_BOND_OPTION_MODE_SELECT;
	}
	//--------------------------------------------------
	if ((g_bonding_test_option&C_BOND_OPTION_MODE_SELECT) == 0)
	{
		bonding_test_main_dvr();
	}
	else
	{
		bonding_led_off();
		bonding_test_main_pccamera();
	}
}
//===============================================================================================
void bonding_test_main_pccamera(void)
{
	if ((g_bonding_test_option&C_BOND_OPTION_LINK_USB_MUST_READ_PIN) == 0)
	{//等待 USB 检测脚变为高:
		while (1)
		{
			if (io_usb_detect()) break;
		}
	}
	if (io_usb_detect()) power_ctrl_low(0);
	voltage_detect_stop();
	voltage_detect_init(ADC_LINE_0, voltage_detect_charge_mode_isr);
	//goto_usb_device_mode();
	g_pc_camera = 1;
	goto_usb_video_device();
	F_power_Pin_Disable();
}
//===============================================================================================
void bonding_test_main_dvr(void)
{
	INT16U cnt;
	red_on();
	if (capture_jpeg()!=STATUS_OK)
	{
		bonding_test_error();
		F_power_Pin_Disable();
	}
	red_off();
	bonding_led_on();
	R_KeyCode = 0;
	while(1)
	{
		OSTimeDly(1);
		watchdog_clr();
		function_key_scan();
		if (R_KeyCode)
		{
			R_KeyCode = 0;
			bonding_led_off();
			OSTimeDly(30);
			cnt = 0;
		}
		if (++cnt >= 50) cnt = 0;
		if (cnt <= 45)
		{
			bonding_led_on();
		}
		else
		{
			bonding_led_off();
		}
	}
}
//===============================================================================================
void b_test_adc_0(void)
{
	bonding_test_adc_start();
	g_bonding_test_adc_flag = 0;
	test_adc_index = 0;
}
void b_test_adc_1(void)
{
	switch (test_adc_index)
	{
		case 0:
			if (g_bonding_test_adc_flag)
			{
				if ((g_bonding_test_option&C_BOND_OPTION_LINE0_WRITE_FILE) != 0)
				{//当这个 option 被打开时, 其它所有功能就被disable, 仅仅执行本option功能:
					if (1)
					{
						INT16S fd;
						fd = open("C:\\adc.bin", O_RDWR|O_CREAT);
						lseek(fd, 0, SEEK_END);
						write(fd, (INT32U)&g_bonding_test_adc_value, 4);
						close(fd);
					}
					bonding_test_adc_start();
					g_bonding_test_adc_flag = 0;
					break;
				}
				if ((g_bonding_test_adc_value<0x9000) || (g_bonding_test_adc_value>0xD000))
				{
					if ((g_bonding_test_option&C_BOND_OPTION_LINE0_WITH_FLOAT) == 0)
					{
						write_error_code(C_B_TEST_ERROR_LINE_0_PIN_WITH_FLOAT);
					}
				}
			  	gpio_init_io(IO_F7, GPIO_INPUT);
		  		gpio_set_port_attribute(IO_F7, ATTRIBUTE_LOW);
	  			gpio_write_io(IO_F7, DATA_LOW);
			  	R_ADC_USELINEIN = 0;
			  	OSTimeDly(1);
	  			bonding_test_adc_start();
			  	g_bonding_test_adc_flag = 0;
			  	test_adc_index = 1;
			}
			break;
		case 1:
			if (g_bonding_test_adc_flag)
			{
				/*
				if (1)
				{
					if (1)
					{
						INT16S fd;
						INT8U write_buf[20];
						fd = open("C:\\jh_adc.txt", O_RDWR|O_CREAT);
						lseek(fd, 0, SEEK_END);
						sprintf((char *)write_buf, (const char *)"0x%04X\r\n", g_bonding_test_adc_value);
						write(fd, (INT32U)write_buf, 8);
						close(fd);
					}
					bonding_test_adc_start();
					g_bonding_test_adc_flag = 0;
					break;
				}
				*/
				if (g_bonding_test_adc_value < 0x2800)
				{//IO_F7 没有邦好:
					if ((g_bonding_test_option&C_BOND_OPTION_LINE0_WITH_PL) == 0)
					{
						write_error_code(C_B_TEST_ERROR_LINE_0_PIN_WITH_PL);
					}
				}
				test_adc_index = 2;
			}
			break;
		default:
			break;
	}
}
void b_test_adc_2(void)
{
	while (1)
	{
		if (test_adc_index==2) break;
		b_test_adc_1();
	}
}
//===============================================================================================
void b_test_rtc_0(void)
{//rtc 测试第1步: 设置时间
	g_current_time.tm_year = 2012;
	g_current_time.tm_mon = 5;
	g_current_time.tm_mday = 1;
	g_current_time.tm_hour = 10;
	g_current_time.tm_min = 10;
	g_current_time.tm_sec = 10;
	cal_time_set(g_current_time);
	g_public_128hz_cnt = 0;
	test_rtc_flag = 0;
	if ((g_bonding_test_option&C_BOND_OPTION_RTC) != 0) test_rtc_flag = 1;
}
void b_test_rtc_1(void)
{//rtc 测试第2步: 测试时间
	if (g_public_128hz_cnt >= 128+32)
	{
		b_test_rtc_2();
	}
}
void b_test_rtc_2(void)
{//rtc 测试第3步: 等待测试完成
	INT32U t_hour;
	INT32U t_min;
	INT32U t_sec;
	while (1)
	{
		if (test_rtc_flag) break;
		if (g_public_128hz_cnt >= 128+32)
		{
			test_rtc_flag = 1;
			t_sec = gpx_rtc_read(0x0);		//外部RTC-秒钟
			t_min = gpx_rtc_read(0x1);		//外部RTC-分钟
			t_hour = gpx_rtc_read(0x2);		//外部RTC-小时
			if ((t_hour != 10) || (t_min != 10) || (t_sec >= 13) || (t_sec <= 10))
			{
				write_error_code(C_B_TEST_ERROR_RTC);
			}
		}
	}
}
//===============================================================================================
void write_error_code(INT16U value)
{
	if ((g_bonding_test_result==C_B_TEST_OK) && (value!=C_B_TEST_OK))
	{
		g_bonding_test_result = value;
	}
}

void bonding_test_error(void)
{
	INT16U cnt;
	R_KeyCode = 0;
	while (1)
	{
		OSTimeDly(1);
		watchdog_clr();
		function_key_scan();
		if (R_KeyCode==C_POWER_KEY_SHORT)
		{
			R_KeyCode = 0;
			bonding_led_on();
			OSTimeDly(50);
			break;
		}
		if (++cnt >= 12) cnt = 0;
		if (cnt >= 6)
		{
			bonding_led_on();
		}
		else
		{
			bonding_led_off();
		}
	}
}
//===============================================================================================
