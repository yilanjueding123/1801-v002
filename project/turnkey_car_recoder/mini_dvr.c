#include "mini_dvr_api.h"
#include "application.h"
#include "state_usb.h"
#include "program_01.h"
#include "program_02.h"
#include "program_03.h"
#include "program_04.h"
#include "program_05.h"
INT8U io_usb_detect_get(void)
{
	INT32U count, save, t;
	count = 0;
	save = 0;
	while(1)
	{
		t = gpio_read_io(C_USBDEVICE_PIN);
		if (save != t)
		{
			save = t;
			count = 0;
		}
		else
		{
			if (++count > 5) break;
		}
	}
	if (save) return 1;
	else return 0;
}

void mini_dvr_main(void)
{
	INT32U idle_count;
	INT16U mode;
	INT32U led_flash_cnt;
	INT8U  led_flash_flag;
	//INT32U t;
	INT16U temp_x, temp_y, temp_z, temp_t;
	//-------------------------------------------
	//开机变量初始化
	idle_count = 0;
	mode = MODE_JPEG_CAPTURE;
	//-------------------------------------------
	io_key_init();
	watchdog_enable();
	green_on();
	red_on();
	//-------------------------------------------
	video_encode_init();
	//-------------------------------------------
	mount_and_scan_testfile();
	//-------------------------------------------
	#if C_USB_DET_ENABLE
	if (io_usb_detect())
	{
		g_usb_disable = read_usb_disable_key();
		g_usb_disable |= g_6655CD_test_scan_testfile;
		if (g_usb_disable==0)
		{
			power_ctrl_low(0);
			red_on();
			green_off();
			voltage_detect_init(ADC_LINE_0, voltage_detect_charge_mode_isr);
			g_pccamera_showtime = 0;
			goto_usb_device_mode();
			goto_usb_video_device();
			F_power_Pin_Disable();
		}
	}
	#endif
	//-------------------------------------------
	green_on();
	check_customer_id();
	mount_and_mk_ch_dir();
	//rtc_time_get_and_start();
	voltage_detect_init(ADC_LINE_0, voltage_detect_isr);
	bonding_test_service();
	//-------------------------------------------
	while(gpio_read_io(POWER_KEY));
	OSTimeDly(30);
	green_off();
	OSTimeDly(10);
	green_on();
	red_off();
//==========================================================================================
	while(1)
	{
		OSTimeDly(1);	//delay 10ms
		watchdog_clr();
		function_key_scan();
		R_idle_task_disable = 0; //必须执行idle_task任务以省电
//==========================================================================================
		if (mode == MODE_AVI_ENCODE)
		{
			if (video_encode_status() == VIDEO_CODEC_PROCESS_END)
			{//storage full and storage write error handle
				green_off();
				red_on();
				OSTimeDly(50);
				F_power_Pin_Disable();
			}
			else
			{
				if (g_file_size_over || g_avi_packer_error_flag)
				{
					vid_stop();
					if (vid_start()!=STATUS_OK)
					{
						green_on();
						red_off();
						mode = MODE_JPEG_CAPTURE;
					}
				}
			}
		}
//==========================================================================================
		if (mode == MODE_AVI_ENCODE)
		{
			if (video_encode_status() == VIDEO_CODEC_PROCESSING)
			{//录像时led闪烁:
				if (++led_flash_cnt >= 200) led_flash_cnt = 0;
				if (1)	//(led_flash_flag)
				{
					if (led_flash_cnt < 15) green_on();
					else green_off();
				}
				else green_off();
			}
		}
//==========================================================================================
		if (R_KeyCode == C_POWER_KEY_SHORT)
		{
			R_KeyCode = 0;
			idle_count = 0;
			switch(mode)
			{
				case MODE_JPEG_CAPTURE:
					green_off();
					if (capture_jpeg()==STATUS_OK)
					{
						//DBG_PRINT("<<<Capture!!!>>>\r\n");
					}
					else
					{//拍照失败, led 闪烁二次:
						OSTimeDly(25);
						red_on();
						OSTimeDly(25);
						red_off();
						OSTimeDly(25);
					}
					red_off();
					green_on();
					break;
				case MODE_AVI_ENCODE:
					if (video_encode_status() == VIDEO_CODEC_PROCESSING)
					{
						g_savefile_indication = 1;
						vid_stop();
						g_savefile_indication = 0;
					}
					red_off();
					green_on();
					mode = MODE_JPEG_CAPTURE;
					break;
			}
		}
//==========================================================================================
		if (R_KeyCode == C_POWER_KEY_LONG)
		{
			R_KeyCode = 0;
			idle_count = 0;
			switch(mode)
			{
				case MODE_JPEG_CAPTURE:
					//绿色led闪三次提示准备开始录像:
					temp_x = 100;
					temp_y = 0;
					temp_z = 0;
					temp_t = 0;
					while(1)
					{
						if (gpio_read_io(POWER_KEY) == 0) temp_t++;
						OSTimeDly(1);	//delay 10ms
						if (++temp_x >= 25)
						{
							temp_x = 0;
							temp_y ^= 1;
							if (temp_y)
							{
								green_off();
								temp_z++;
								if (temp_z >= 4) break;
							}
							else green_on();
						}
					}
					if (temp_t <= 3)
					{//关机:
						red_on();
						OSTimeDly(200);
						F_power_Pin_Disable();
					}
					if (vid_start()==STATUS_OK)
					{
						mode = MODE_AVI_ENCODE;
						g_lvd_flag_savefile = 0;	//这一行仅仅在按键启动录像时执行, 在其它方式启动录像时不要执行;
						led_flash_flag = 0;
						led_flash_cnt = 15;
					}
					else
					{
						//录像不能正常开始:
						OSTimeDly(30);	//300ms
						red_on();
						OSTimeDly(30);	//300ms
						red_off();
						OSTimeDly(30);	//300ms
						green_on();
					}
					break;
				case MODE_AVI_ENCODE:
					if (video_encode_status() == VIDEO_CODEC_PROCESSING)
					{
						green_on();
						//g_savefile_indication = 1;
						vid_stop();
						//g_savefile_indication = 0;
					}
					mode = MODE_JPEG_CAPTURE;
					green_off();
					red_on();
					OSTimeDly(200);
					F_power_Pin_Disable();
					break;
			}
		}
//==========================================================================================
		#if C_USB_DET_ENABLE
		if (1)
		{
			if (gpio_read_io(C_USBDEVICE_PIN) == 1)
			{
				if (g_usb_disable==0)
				{
					if (io_usb_detect())
					{
						green_on();
						if (mode == MODE_AVI_ENCODE)
						{
							if(video_encode_status() == VIDEO_CODEC_PROCESSING)
							{
								vid_stop();	
							}
							mode = MODE_JPEG_CAPTURE;
						}
						power_ctrl_low(0);
						red_on();
						green_off();
						voltage_detect_stop();
						voltage_detect_init(ADC_LINE_0, voltage_detect_charge_mode_isr);
						g_pccamera_showtime = 1;
						goto_usb_device_mode();
						goto_usb_video_device();
						F_power_Pin_Disable();
					}
				}
			}
			else
			{
				if (g_usb_disable == 1)
				{
					if (io_usb_detect_get() == 0) F_power_Pin_Disable();
				}
			}
		}
		#endif
//==========================================================================================
		if (mode == MODE_AVI_ENCODE)
		{//低电压或30分钟保存一次文件:
			if(video_encode_status() == VIDEO_CODEC_PROCESSING)
			{
				if (g_public_128hz_cnt >= (g_lvd_bak_savefile+128*60*5))
				{//低电压保存文件: 要求录像至少已经录了 5 分钟:
					g_lvd_bak_savefile += 128*60*5;
					g_lvd_cnt_savefile += 5;
					if ((g_lvd_cnt_savefile >= 30) || (g_lvd_flag_savefile == 0x5a))
					{
						g_lvd_cnt_savefile = 0;
						g_file_size_over = 1;
						if (g_lvd_flag_savefile == 0x5a) g_lvd_flag_savefile = 1;
					}
				}
			}
		}
//==========================================================================================
		if (mode == MODE_AVI_ENCODE) avi_update_dir();
//==========================================================================================
		if (mode == MODE_AVI_ENCODE)
		{
			if (video_encode_status() != VIDEO_CODEC_PROCESS_END)
			{
				idle_count = 0;
			}
		}
		idle_count++;
		#if DBG_MESSAGE == CUSTOM_OFF
		if(idle_count >= C_IDLE_SLEEP)
		{
			F_power_Pin_Disable();
		}
		#endif
//==========================================================================================
		#if DBG_MESSAGE == CUSTOM_OFF
		if(voltage_is_low())
		{
			if (mode == MODE_AVI_ENCODE)
			{
				if (video_encode_status() == VIDEO_CODEC_PROCESSING)
				{
					vid_stop();
				}
				mode = MODE_JPEG_CAPTURE;
			}
			red_on();
			green_on();
			OSTimeDly(200);	//delay 2s
			F_power_Pin_Disable();
		}
		#endif
//==========================================================================================
	}
}
