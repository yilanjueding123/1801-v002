#include "mini_dvr_api.h"
#include "application.h"
#include "stdio.h"
#include "jh_define.h"
#include "program_01.h"
#include "program_02.h"
#include "program_03.h"
#include "program_04.h"
#include "program_05.h"
#include "firmware_upgrade.h"

void io_led_key_init(void);

extern INT8U s_usbd_pin;
extern void F_TimeData_ext2int(void);

volatile static INT8U  g_voltage_status;
volatile static INT16U g_adc_count;
volatile static INT32U g_adc_total_value;
#define USER_FILE_MAX_NUM		10000	//avi/jpg/wav, 每类文件最多有10000个: 0000-9999
static CHAR   g_file_path[24];
volatile static INT32S g_jpeg_index;
volatile static INT32S g_avi_index;
volatile static INT32S g_wav_index;
volatile static INT16S g_file_handle;
volatile static INT16U g_updatadir_cnt;

volatile INT8U  g_capture_complete_flag;
volatile INT8U  g_file_size_over;
volatile INT8U  g_avi_packer_error_flag;
volatile INT32U g_public_128hz_cnt;
volatile INT16U g_savefile_indication;
volatile INT8U  g_lvd_flag_savefile;
volatile INT32U g_lvd_cnt_savefile;
volatile INT32U g_lvd_bak_savefile;

volatile INT16U g_6655CD_test_mount_time = 0;
volatile INT16U g_6655CD_test_scan_testfile = 0;	//=1表示扫描到测试文件; =0表示没有扫描到测试文件

volatile INT8U  g_usb_disable;
volatile INT16U g_sensor_id;
volatile INT8U  g_pc_camera;
volatile INT8U  g_pccamera_showtime;
volatile INT16U R_KeyCode;

volatile INT16U R_idle_task_disable = 0; //=0, 默认要执行Idle_task任务以省电 

volatile static INT8U  g_key_1_hold;
volatile static INT8U  g_key_2_hold;
volatile static INT8U  g_key_3_hold;
volatile static INT8U  g_key_4_hold;
volatile static INT16U g_key_1_debounce;
volatile static INT16U g_key_2_debounce;
volatile static INT16U g_key_3_debounce;
volatile static INT16U g_key_4_debounce;

#define SPI_BATT_FULL_THRESHOLD			0xA000	//充电充满门槛电压
#define SPI_SAVE_FILE_THRESHOLD 		0x9100	//0x8D00	//低电压自动保存文件
#define SPI_LVD_THRESHOLD 				0x8D00	//0x8800	//自动关机门槛电压

void io_key_init(void)
{
	//power_ctrl_low();
	F_power_Pin_Enable();
	//-----------------------------------
	*P_USBD_CONFIG1 |= 0x100;	//turn_off_usb_phy();
	s_usbd_pin = 0;
	//-----------------------------------
  	gpio_init_io(C_USBDEVICE_PIN, GPIO_INPUT);
  	gpio_set_port_attribute(C_USBDEVICE_PIN, ATTRIBUTE_LOW);
  	gpio_write_io(C_USBDEVICE_PIN, DATA_LOW);
	//-----------------------------------
	//LED
	io_led_key_init();
	//-----------------------------------
	//变量初始化:
	R_KeyCode = 0;
	g_key_1_hold = 2;
	g_key_2_hold = 2;
	g_key_3_hold = 2;
	g_key_4_hold = 2;
	g_key_1_debounce = 0;
	g_key_2_debounce = 0;
	g_key_3_debounce = 0;
	g_key_4_debounce = 0;
	
	g_bonding_test_mode = 0;
	g_bonding_test_option = 0;
	g_bonding_test_result = 0;
	g_lvd_flag_savefile = 1;
	g_public_128hz_cnt = 0;
	g_sensor_id = SENSOR_CHIP_NO_READ;
	g_pc_camera = 0;
	g_usb_disable = 0;
	g_savefile_indication = 0;
}

void io_led_key_init(void)
{
	#if LED_1_CONFIG == USED
  	gpio_init_io(LED_1_PIN, GPIO_OUTPUT);
  	gpio_set_port_attribute(LED_1_PIN, ATTRIBUTE_HIGH);
  	#if LED_1_TRIGGER_LEVEL == 1
  	gpio_write_io(LED_1_PIN, DATA_LOW);
  	#else
  	gpio_write_io(LED_1_PIN, DATA_HIGH);
	#endif
	#endif

	#if LED_2_CONFIG == USED
  	gpio_init_io(LED_2_PIN, GPIO_OUTPUT);
  	gpio_set_port_attribute(LED_2_PIN, ATTRIBUTE_HIGH);
  	#if LED_2_TRIGGER_LEVEL == 1
  	gpio_write_io(LED_2_PIN, DATA_LOW);
  	#else
  	gpio_write_io(LED_2_PIN, DATA_HIGH);
	#endif
	#endif

	#if LED_3_CONFIG == USED
  	gpio_init_io(LED_3_PIN, GPIO_OUTPUT);
  	gpio_set_port_attribute(LED_3_PIN, ATTRIBUTE_HIGH);
  	#if LED_3_TRIGGER_LEVEL == 1
  	gpio_write_io(LED_3_PIN, DATA_LOW);
  	#else
  	gpio_write_io(LED_3_PIN, DATA_HIGH);
	#endif
	#endif

	#if LED_4_CONFIG == USED
  	gpio_init_io(LED_4_PIN, GPIO_OUTPUT);
  	gpio_set_port_attribute(LED_4_PIN, ATTRIBUTE_HIGH);
  	#if LED_4_TRIGGER_LEVEL == 1
  	gpio_write_io(LED_4_PIN, DATA_LOW);
  	#else
  	gpio_write_io(LED_4_PIN, DATA_HIGH);
	#endif
	#endif
	//-----------------------------------
	//KEY
	#if KEY_1_CONFIG == USED
  	gpio_init_io(KEY_1_PIN, GPIO_INPUT);
  	gpio_set_port_attribute(KEY_1_PIN, ATTRIBUTE_LOW);
  	#if KEY_1_TRIGGER_LEVEL == 1
  	gpio_write_io(KEY_1_PIN, DATA_LOW);
  	#else
  	gpio_write_io(KEY_1_PIN, DATA_HIGH);
	#endif
	#endif

	#if KEY_2_CONFIG == USED
  	gpio_init_io(KEY_2_PIN, GPIO_INPUT);
  	gpio_set_port_attribute(KEY_2_PIN, ATTRIBUTE_LOW);
  	#if KEY_2_TRIGGER_LEVEL == 1
  	gpio_write_io(KEY_2_PIN, DATA_LOW);
  	#else
  	gpio_write_io(KEY_2_PIN, DATA_HIGH);
	#endif
	#endif

	#if KEY_3_CONFIG == USED
  	gpio_init_io(KEY_3_PIN, GPIO_INPUT);
  	gpio_set_port_attribute(KEY_3_PIN, ATTRIBUTE_LOW);
  	#if KEY_3_TRIGGER_LEVEL == 1
  	gpio_write_io(KEY_3_PIN, DATA_LOW);
  	#else
  	gpio_write_io(KEY_3_PIN, DATA_HIGH);
	#endif
	#endif

	#if KEY_4_CONFIG == USED
  	gpio_init_io(KEY_4_PIN, GPIO_INPUT);
  	gpio_set_port_attribute(KEY_4_PIN, ATTRIBUTE_LOW);
  	#if KEY_4_TRIGGER_LEVEL == 1
  	gpio_write_io(KEY_4_PIN, DATA_LOW);
  	#else
  	gpio_write_io(KEY_4_PIN, DATA_HIGH);
	#endif
	#endif
}

INT8U io_usb_detect(void)
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
			if (++count > 50*2) break;
		}
	}
	if (save) return 1;
	else return 0;
}

INT8U read_usb_disable_key(void)
{
	INT8U  ret = 0;
	#if USB_DISABLE_KEY_CONFIG == USED
	INT8U  t, x;
	x = 0;
	while (1)
	{
		t = gpio_read_io(USB_DISABLE_KEY_PIN);
		#if USB_DISABLE_KEY_TRIGGER_LEVEL == 0
		t ^= 1;
		#endif
		if (! t) break;
		if (++x >= 5)
		{
			ret = 1;
			break;
		}
		OSTimeDly(1);	//delay 10ms
	}
	#endif
	return ret;
}

void mount_and_scan_testfile(void)	//for 6655C&6655D test
{
	INT32S nRet, count;
	INT16S fd;

	for(count= 0; count<3; count++)
	{
		nRet = _devicemount(MINI_DVR_STORAGE_TYPE);
		if(nRet >= 0) break;
	}
	
	g_6655CD_test_mount_time = count;
	g_6655CD_test_scan_testfile = 0; //=0表示没有扫描到测试文件
	if(count < 3)
	{//TF卡mount成功:
		fd = open("C:\\jh_test.txt", O_RDONLY);
		if(fd < 0)
		{//搜索不到测试文件:
			;
		}
		else
		{//搜索到测试文件:
			g_6655CD_test_scan_testfile = 1; //=1表示扫描到测试文件
			close(fd);
		}
	}
}

void mount_and_mk_ch_dir(void)
{
	INT32S nRet, count;
	INT64U free_size;
		
//	for(count= 0; count<3; count++)
//	{
//		nRet = _devicemount(MINI_DVR_STORAGE_TYPE);
//		if(nRet >= 0) break;
//	}
	count = g_6655CD_test_mount_time; //for 6655C&6655D test

	if(count >= 3)
	{
		no_disk_handle();
	}
	free_size = vfsFreeSpace(MINI_DVR_STORAGE_TYPE);
	free_size >>= 20;
	
	firmware_upgrade_server();
	rtc_time_get_and_start();

	//xiangyong 2012-4-9 16:50
	//unlink("C:\\index0.tmp");
	//unlink("C:\\index1.tmp");
	#if CUSTOMIZE_FOR_DIWEI == 1
	unlink("C:\\DCIM\\100MEDIA\\index0.tmp");
	unlink("C:\\DCIM\\100MEDIA\\index1.tmp");
	#else
	unlink("C:\\VIDEO\\index0.tmp");
	unlink("C:\\VIDEO\\index1.tmp");
	#endif

	#if CUSTOMIZE_FOR_DIWEI == 1
	nRet = chdir("C:\\DCIM");
	if(nRet != 0)
	{
		nRet = mkdir("C:\\DCIM");
		nRet = chdir("C:\\DCIM");
	}
	#endif
		
	#if MP3_RECORD == 1
	#if CUSTOMIZE_FOR_DIWEI == 1
	nRet = chdir("C:\\DCIM\\RECORD");
	if(nRet != 0)
	{
		nRet = mkdir("C:\\DCIM\\RECORD");
	}
	#else
	nRet = chdir("C:\\RECORD");
	if(nRet != 0)
	{
		nRet = mkdir("C:\\RECORD");
	}
	#endif
	#endif

	#if CUSTOMIZE_FOR_DIWEI == 1
	nRet = chdir("C:\\DCIM\\100MEDIA");
	if(nRet != 0)
	{
		nRet = mkdir("C:\\DCIM\\100MEDIA");
	}
	nRet = chdir("C:\\DCIM\\PHOTO");
	if(nRet != 0)
	{
		nRet = mkdir("C:\\DCIM\\PHOTO");
		nRet = chdir("C:\\DCIM\\PHOTO");
	}
	#else
	nRet = chdir("C:\\VIDEO");
	if(nRet != 0)
	{
		nRet = mkdir("C:\\VIDEO");
	}
	nRet = chdir("C:\\PHOTO");
	if(nRet != 0)
	{
		nRet = mkdir("C:\\PHOTO");
		nRet = chdir("C:\\PHOTO");
	}
	#endif

	//DBG_PRINT("FS_SD, DiskFreeSize = %d MB\r\n", free_size);
	if(free_size == 0)
	{
		disk_no_size_handle();
	}	

	#if MP3_RECORD == 1
	get_file_final_wav_index();
	#endif
	get_file_final_avi_index();
	get_file_final_jpeg_index();
}

INT32S get_file_final_avi_index(void)
{
	CHAR  *pdata;
	struct f_info   file_info;
	INT32S nRet, temp;
	
	g_avi_index = -1;
	#if CUSTOMIZE_FOR_DIWEI == 1
	nRet = chdir("C:\\DCIM\\100MEDIA");
	#else
	nRet = chdir("C:\\VIDEO");
	#endif
	nRet = _findfirst("*.avi", &file_info, D_ALL);
	if(nRet < 0)
		goto Return;

	while(1)
	{	
		pdata = (CHAR*)file_info.f_name; //MOVI0005.avi
		#if CUSTOMIZE_FOR_DIWEI == 1
		if(gp_strncmp((INT8S*)pdata, (INT8S*)"SUNP", 4) == 0)
		#else
		if(gp_strncmp((INT8S*)pdata, (INT8S*)"MOVI", 4) == 0)
		#endif
		{
			temp = (*(pdata + 4) - 0x30)*1000;
			temp += (*(pdata + 5) - 0x30)*100;
			temp += (*(pdata + 6) - 0x30)*10;
			temp += (*(pdata + 7) - 0x30)*1;
			if(temp > g_avi_index)
				g_avi_index = temp;
		}
		
		nRet = _findnext(&file_info);
		if(nRet < 0)
			break;	
	}
	
Return:
	g_avi_index++;
	if(g_avi_index > 9999 || g_avi_index < 0) 
		g_avi_index = 0;

	return g_avi_index;
}

INT32S get_file_final_jpeg_index(void)
{
	CHAR  *pdata;
	struct f_info   file_info;
	INT32S nRet, temp;
	
	g_jpeg_index = -1;
	
	if (g_bonding_test_mode)
	{//邦定测试模式:
		nRet = chdir("C:\\");
	}
	else
	{//正常模式:
#if CUSTOMIZE_FOR_DIWEI == 1
	nRet = chdir("C:\\DCIM\\PHOTO");
#else
	nRet = chdir("C:\\PHOTO");
#endif
	}

	nRet = _findfirst("*.jpg", &file_info, D_ALL);
	if(nRet < 0)
		goto Return;
		
	while(1)
	{	
		pdata = (CHAR*)file_info.f_name; //PICT0005.jpg	
		if(gp_strncmp((INT8S*)pdata, (INT8S*)"PICT", 4) == 0)
		{
			temp = (*(pdata + 4) - 0x30)*1000;
			temp += (*(pdata + 5) - 0x30)*100;
			temp += (*(pdata + 6) - 0x30)*10;
			temp += (*(pdata + 7) - 0x30)*1;
			if(temp > g_jpeg_index)
				g_jpeg_index = temp;
		}
		
		nRet = _findnext(&file_info);
		if(nRet < 0)
			break;	
	}
	
Return:	
	g_jpeg_index++;
	if(g_jpeg_index > 9999 || g_jpeg_index < 0) 
		g_jpeg_index = 0;
	
	return g_jpeg_index;
}

INT32S get_file_final_wav_index(void)
{
	CHAR  *pdata;
	struct f_info   file_info;
	INT32S nRet, temp;
	
	g_wav_index = -1;
#if CUSTOMIZE_FOR_DIWEI == 1
	nRet = chdir("C:\\DCIM\\RECORD");
#else
	nRet = chdir("C:\\RECORD");
#endif
	nRet = _findfirst("*.wav", &file_info, D_ALL);
	if(nRet < 0)
		goto Return;
	
	while(1)
	{	
		pdata = (CHAR*)file_info.f_name; //RECR0005.wav
		if(gp_strncmp((INT8S*)pdata, (INT8S*)"RECR", 4) == 0)
		{
			temp = (*(pdata + 4) - 0x30)*1000;
			temp += (*(pdata + 5) - 0x30)*100;
			temp += (*(pdata + 6) - 0x30)*10;
			temp += (*(pdata + 7) - 0x30)*1;
			if(temp > g_wav_index)
				g_wav_index = temp;
		}
			
		nRet = _findnext(&file_info);
		if(nRet < 0)
			break;	
	}
	
Return:	
	g_wav_index++;
	if(g_wav_index > 9999 || g_wav_index < 0) 
		g_wav_index = 0;
	
	return g_wav_index;
}

INT16S open_avi_file_handle(void)
{
	struct f_info   file_info;
	INT16S nRet;
	INT16S file_handle;
	INT32S index, cnt;
	
	if (!g_bonding_test_mode)
	{//正常模式:
		#if CUSTOMIZE_FOR_DIWEI == 1
		nRet = chdir("C:\\DCIM\\100MEDIA");
		#else
		nRet = chdir("C:\\VIDEO");
		#endif
		index = g_avi_index;
		cnt = 0;
		for (cnt=0; cnt<USER_FILE_MAX_NUM; cnt++)
		{
			#if CUSTOMIZE_FOR_DIWEI == 1
			sprintf((char *)g_file_path, (const char *)"SUNP%04d.avi", index);
			#else
			sprintf((char *)g_file_path, (const char *)"MOVI%04d.avi", index);
			#endif
			nRet = _findfirst(g_file_path, &file_info, D_ALL);
			if (nRet<0) break;
			if (++index >= USER_FILE_MAX_NUM) index = 0;
		}
		if (cnt<USER_FILE_MAX_NUM)
		{
			g_avi_index = index;
			if (++g_avi_index >= USER_FILE_MAX_NUM) g_avi_index = 0;
			file_handle = open (g_file_path, O_WRONLY|O_CREAT|O_TRUNC);
			return file_handle;
		}
		else return -1;
	}
	else
	{//邦定测试模式:
		nRet = chdir("C:\\");
		sprintf((char *)g_file_path, (const char *)"00001111_TEST.avi");
		file_handle = open (g_file_path, O_WRONLY|O_CREAT|O_TRUNC);
		return file_handle;
	}
}

INT16S open_jpg_file_handle(void)
{
	struct f_info   file_info;
	INT16S nRet;
	INT16S file_handle;
	INT32S index, cnt;
	
	if (g_bonding_test_mode)
	{//邦定测试模式:
		nRet = chdir("C:\\");
	}
	else
	{//正常模式:
	#if CUSTOMIZE_FOR_DIWEI == 1
	nRet = chdir("C:\\DCIM\\PHOTO");
	#else
	nRet = chdir("C:\\PHOTO");
	#endif
	}
	
	index = g_jpeg_index;
	cnt = 0;
	for (cnt=0; cnt<USER_FILE_MAX_NUM; cnt++)
	{
		sprintf((char *)g_file_path, (const char *)"PICT%04d.jpg", index);
		nRet = _findfirst(g_file_path, &file_info, D_ALL);
		if (nRet<0) break;
		if (++index >= USER_FILE_MAX_NUM) index = 0;
	}
	if (cnt<USER_FILE_MAX_NUM)
	{
		g_jpeg_index = index;
		if (++g_jpeg_index >= USER_FILE_MAX_NUM) g_jpeg_index = 0;
		file_handle = open (g_file_path, O_WRONLY|O_CREAT|O_TRUNC);
		return file_handle;
	}
	else return -1;
}

INT16S open_wav_file_handle(void)
{
	struct f_info   file_info;
	INT16S nRet;
	INT16S file_handle;
	INT32S index, cnt;
	#if CUSTOMIZE_FOR_DIWEI == 1
	nRet = chdir("C:\\DCIM\\RECORD");
	#else
	nRet = chdir("C:\\RECORD");
	#endif
	index = g_wav_index;
	cnt = 0;
	for (cnt=0; cnt<USER_FILE_MAX_NUM; cnt++)
	{
		sprintf((char *)g_file_path, (const char *)"RECR%04d.wav", index);
		nRet = _findfirst(g_file_path, &file_info, D_ALL);
		if (nRet<0) break;
		if (++index >= USER_FILE_MAX_NUM) index = 0;
	}
	if (cnt<USER_FILE_MAX_NUM)
	{
		g_wav_index = index;
		if (++g_wav_index >= USER_FILE_MAX_NUM) g_wav_index = 0;
		file_handle = open (g_file_path, O_WRONLY|O_CREAT|O_TRUNC);
		return file_handle;
	}
	else return -1;
}

INT32S capture_jpeg(void)
{
	MEDIA_SOURCE src;
	INT64U free_size;
	OSTimeDly(10);	//拍照之前等待 100ms
	if (video_encode_status() == VIDEO_CODEC_PROCESSING) return STATUS_FAIL;
	free_size = vfsFreeSpace(MINI_DVR_STORAGE_TYPE);
	free_size >>= 20;
	if (free_size == 0) return STATUS_FAIL;
	src.type_ID.FileHandle = open_jpg_file_handle();
	src.type = SOURCE_TYPE_FS;
	src.Format.VideoFormat = MJPEG;
	if (src.type_ID.FileHandle >=0)
	{
		if (video_encode_capture_picture(src) == START_OK)
		{
			return STATUS_OK;
		}
		else
		{
			if (src.type_ID.FileHandle >=0) close(src.type_ID.FileHandle);
			return STATUS_FAIL;
		}
	}
	return STATUS_FAIL;
}

void avi_update_dir(void)
{
	if (++g_updatadir_cnt > 500)
	{//5 秒钟更新一次:
		g_updatadir_cnt = 0;
		if(video_encode_status() == VIDEO_CODEC_PROCESSING)
		{
			UpdataDir(g_file_handle);
		}
	}
}

#define REMAIN_SPACE	3 //MB	//此值小于 3 无意义, 因为录像时剩余空间小于 3MB 时会停止录像;
INT32S vid_start(void)
{
	MEDIA_SOURCE src;
	INT64U free_size;
	g_file_size_over = 0;
	g_avi_packer_error_flag = 0;
	g_updatadir_cnt = 0;
	g_lvd_cnt_savefile = 0;
	g_lvd_bak_savefile = g_public_128hz_cnt;
	
	if (video_encode_status() == VIDEO_CODEC_PROCESSING) return STATUS_FAIL;
	free_size = vfsFreeSpace(MINI_DVR_STORAGE_TYPE);
	free_size >>= 20;
	if (free_size < REMAIN_SPACE) return STATUS_FAIL;
	src.type_ID.FileHandle = open_avi_file_handle();
	src.type = SOURCE_TYPE_FS;
	src.Format.VideoFormat = MJPEG;
	if (src.type_ID.FileHandle >= 0)
	{
		g_file_handle = src.type_ID.FileHandle;
		if (video_encode_start(src) == START_OK)
		{
			return STATUS_OK;
		}
		else
		{
			close(src.type_ID.FileHandle);
			unlink(g_file_path);
			return STATUS_FAIL;
		}
	}
	return STATUS_FAIL;
}

INT32S vid_stop(void)
{
	if (video_encode_status() == VIDEO_CODEC_PROCESSING)
	{
		if (video_encode_stop() != START_OK)
		{
			if (g_file_handle >=0) close(g_file_handle);
		}
		//+++++++++++++++++++++++++++++++++++++
		//2012-3-14 15:24
		/* xiangyong 2012-4-9 16:50
		if (video_encode_status() == VIDEO_CODEC_PROCESS_END)
		{
			unlink("C:\\index0.tmp");	
			unlink("C:\\index1.tmp");
		}
		*/
		//-------------------------------------
	}
	return STATUS_OK;
}

//++++++++++++++++++++++++++++ For Bonding Test
void bonding_test_adc_start(void)
{
	g_adc_total_value = 0;
	g_adc_count = 0;
}
//---------------------------- For Bonding Test

//low voltage detect
BOOLEAN voltage_is_low(void)
{
#if 1
	return g_voltage_status;
#else
	return 0;
#endif
}

void voltage_detect_isr(INT16U value)
{//中断程序中要谨慎操作 IO 和 全局变量:



	if (g_savefile_indication)
	{//存贮录像文件时LED快闪:
		g_savefile_indication++;
		if (g_savefile_indication < 10)
		{
			green_on();
		}
		else
		{
			green_off();
			if (g_savefile_indication > 17) g_savefile_indication = 1;
		}
	}
	//-------------------------------------------
	g_public_128hz_cnt++;
	g_adc_count++;
	g_adc_total_value += value;
	if (!g_bonding_test_mode)
	{//正常模式:
		if (g_adc_count >= 127)
		{
			g_adc_count = 0;
			g_adc_total_value >>= 7;
			if (g_adc_total_value >= 0x10000)
			{
				g_adc_total_value = 0xFF00;
			}
			if(g_adc_total_value <= SPI_SAVE_FILE_THRESHOLD)
			{
				if (g_lvd_flag_savefile == 0) g_lvd_flag_savefile = 0x5a;
			}
			if(g_adc_total_value <= SPI_LVD_THRESHOLD)
			{
				//DBG_PRINT("LVD SLEEP !!!\r\n");
				g_voltage_status = TRUE;
			}
		}
	}
	else
	{//邦定测试模式:
		if (g_adc_count >= 63)
		{
			g_adc_count = 0;
			g_adc_total_value >>= 6;
			if (g_adc_total_value >= 0x10000)
			{
				g_adc_total_value = 0xFF00;
			}
			if (!g_bonding_test_adc_flag) {g_bonding_test_adc_value=g_adc_total_value; g_bonding_test_adc_flag=1;}	//For Bonding Test
		}
	}
}

void voltage_detect_charge_mode_isr(INT16U value)
{//中断程序中要谨慎操作 IO 和 全局变量:
	INT8U  charge_status;
	g_public_128hz_cnt++;
	g_adc_count++;
	if (g_bonding_test_mode) return;
	//--------------------------------------------------
	#if VOLTATE_DETECT_FROM_PJ4054==1
	if (g_adc_count == 253)
	{
		gpio_init_io(PJ4054_READ_PIN, GPIO_INPUT);					//按键: 设为上拉输入
	  	gpio_set_port_attribute(PJ4054_READ_PIN, ATTRIBUTE_LOW);
	  	gpio_write_io(PJ4054_READ_PIN, DATA_HIGH);
	}
	else if (g_adc_count >= 255)
	{
		g_adc_count = 0;
		if (gpio_read_io(PJ4054_READ_PIN) == 0) charge_status = 0;	//正在充电:
		else charge_status = 1;										//已经充满:
		power_ctrl_low(0);
	}
	//--------------------------------------------------
	#else
	g_adc_total_value += value;
	if(g_adc_count >= 255)
	{
		g_adc_count = 0;
		g_adc_total_value >>= 8;
		if (g_adc_total_value >= 0x10000)
		{
			g_adc_total_value = 0xFF00;
		}
		if(g_adc_total_value <= SPI_BATT_FULL_THRESHOLD) charge_status = 0;
		else charge_status = 1;
	}
	#endif
	//--------------------------------------------------
	if (g_adc_count==0)
	{
		if (charge_status) red_on();	//充满电后: LED长亮;	//应加信号保护量
		else led_reversal();			//正在充电中: LED慢闪;	//应加信号保护量
	}
}

void voltage_detect_start(void)
{
	adc_manual_sample_start();
}

void voltage_detect_init(INT8U adc_channel, void (*ad_detect_isr)(INT16U data))
{
	g_adc_count = 0;   
	g_adc_total_value = SPI_BATT_FULL_THRESHOLD;
	g_voltage_status = FALSE;

	switch (adc_channel) {
		case ADC_LINE_0:
			gpio_init_io(IO_F7, GPIO_INPUT);
			gpio_set_port_attribute(IO_F7, ATTRIBUTE_HIGH);
			break;
			/*
		case ADC_LINE_1:
			gpio_init_io(IO_F8, GPIO_INPUT);
			gpio_set_port_attribute(IO_F6, ATTRIBUTE_HIGH);
			break;
			
		case ADC_LINE_2:
			gpio_init_io(IO_F6, GPIO_INPUT);
			gpio_set_port_attribute(IO_F8, ATTRIBUTE_HIGH);
			break;
			
		case ADC_LINE_3:
			gpio_init_io(IO_F9, GPIO_INPUT);
			gpio_set_port_attribute(IO_F9, ATTRIBUTE_HIGH);
			break;
			*/
	}
	adc_init();
	adc_manual_ch_set(adc_channel);
	adc_manual_callback_set(ad_detect_isr);
	sys_registe_timer_isr(voltage_detect_start);	//timer base c to start adc convert
}

void voltage_detect_stop(void)
{
	sys_release_timer_isr(voltage_detect_start);
	OSTimeDly(10);	//delay 100ms 
	//以避免voltage_detect_charge_mode_isr 和 voltage_detect_init同时访问 g_adc_count;
}

INT32S save_example_time_to_disk(void)
{
	INT16S fd;
	INT32U addr;
	#if CUSTOMIZE_FOR_DIWEI==1
	fd = open("C:\\TAG.TXT", O_RDWR|O_TRUNC|O_CREAT);
	#else
	fd = open("C:\\time.txt", O_RDWR|O_TRUNC|O_CREAT);
	#endif
	if(fd < 0)
		return STATUS_FAIL;

	addr = (INT32U)gp_malloc(20+5);
	if(!addr)
	{
		close(fd);
		return STATUS_FAIL;
	}
		
	//gp_strcpy((INT8S*)video_info->AudSubFormat, (INT8S *)"adpcm");
	gp_strcpy((INT8S*)addr, (INT8S *)"2016-05-01 23:59:59 ");
	write(fd, addr, 20);
	close(fd);
	gp_free((void*) addr);
	
	return STATUS_OK;
}

//rtc time 
TIME_T	g_current_time;
INT32S rtc_time_get_and_start(void)
{
	INT8U  read_flag=0;
	INT8U  data;
	INT8U  *pdata;
	INT16S fd;
	INT16U wtemp;
	INT32U addr;
	INT32S nRet;
	
	#if CUSTOMIZE_FOR_DIWEI==1
	fd = open("C:\\TAG.TXT", O_RDONLY);
	#else
	fd = open("C:\\time.txt", O_RDONLY);
	#endif
	
	if(fd < 0)
	{
		//DBG_PRINT("OPEN time.txt FAIL!!!!!\r\n");
		goto Fail_Return_3;
	}
	 	
	addr = (INT32U)gp_malloc(20);
	if(!addr)
		goto Fail_Return_2;
		
	nRet = read(fd, addr, 20);
	if(nRet <= 0)
		goto Fail_Return;
		
	read_flag=1;
	nRet = gp_strncmp((INT8S*)addr, (INT8S *)"COPYFIGHT MESSAGE? ", 19);
	if (nRet==0) { save_COPYRIGHT_MESSAGE_to_disk(); return STATUS_FAIL; }
	nRet = gp_strncmp((INT8S*)addr, (INT8S *)"VERSION_NUMBER??", 16);
	if (nRet==0) { save_VERSION_NUMBER_to_disk(); return STATUS_FAIL; }
	nRet = gp_strncmp((INT8S*)addr, (INT8S *)"SENSOR?", 7);
	if (nRet==0) { save_SENSOR_TYPE_to_disk(); return STATUS_FAIL; }
	nRet = gp_strncmp((INT8S*)addr, (INT8S *)"2016-05-01 23:59:59 ", 19);	//返回0表示参数1和参数2的内容完全相同;
	if (nRet==0) goto Fail_Return;
	read_flag=0;

	pdata = (INT8U*)addr;
	//year
	wtemp = 0;
	data = *pdata++;
	data -= 0x30;
	wtemp += data*1000;
	
	data = *pdata++;
	data -= 0x30;
	wtemp += data*100;
	
	data = *pdata++;
	data -= 0x30;
	wtemp += data*10;
	
	data = *pdata++;
	data -= 0x30;
	wtemp += data*1;
	if((wtemp > 2026) || (wtemp < 2016)) goto Fail_Return;
	g_current_time.tm_year = wtemp;
	
	//skip -		
	pdata++;	
	
	//month
	wtemp = 0;
	data = *pdata++;
	data -= 0x30;
	wtemp += data*10;
	
	data = *pdata++;
	data -= 0x30;
	wtemp += data*1;
	if(wtemp>12) goto Fail_Return;
	g_current_time.tm_mon = wtemp;
			
	//skip -		
	pdata++;
	
	//day
	wtemp = 0;
	data = *pdata++;
	data -= 0x30;
	wtemp += data*10;
	
	data = *pdata++;
	data -= 0x30;
	wtemp += data*1;
	if(wtemp>31) goto Fail_Return;
	g_current_time.tm_mday = wtemp;
	
	//skip space		
	pdata++;
	
	//hour
	wtemp = 0;
	data = *pdata++;
	data -= 0x30;
	wtemp += data*10;
	
	data = *pdata++;
	data -= 0x30;
	wtemp += data*1;
	if(wtemp>23) goto Fail_Return;
	g_current_time.tm_hour = wtemp;
			
	//skip :	
	pdata++;
			
	//minute
	wtemp = 0;
	data = *pdata++;
	data -= 0x30;
	wtemp += data*10;
	
	data = *pdata++;
	data -= 0x30;
	wtemp += data*1;
	if(wtemp>59) goto Fail_Return;
	g_current_time.tm_min = wtemp;
	
	//skip :	
	pdata++;
			
	//second
	wtemp = 0;
	data = *pdata++;
	data -= 0x30;
	wtemp += data*10;
	
	data = *pdata++;
	data -= 0x30;
	wtemp += data*1;
	if(wtemp>59) goto Fail_Return;
	g_current_time.tm_sec = wtemp;

	close(fd);
	gp_free((void*) addr);
	calendar_init();
	cal_time_set(g_current_time);
	F_TimeData_ext2int();			//本行和下一行是需要的, 因为上一行有可能没将时间写入"独立供电RTC";
	cal_time_get(&g_current_time);	//

	save_example_time_to_disk();
	return STATUS_OK;

Fail_Return:
	gp_free((void*) addr);
Fail_Return_2:
	close(fd);
Fail_Return_3:
	calendar_init();
	F_TimeData_ext2int();
	cal_time_get(&g_current_time);
//masked by xiangyong	cal_time_set(g_current_time);	//xiangyong: 这一行是不需要的;
	if( read_flag == 0) save_example_time_to_disk();
	return STATUS_FAIL;
}

void video_encode_init(void)
{
	VIDEO_ARGUMENT arg;

	R_SYSTEM_CTRL |= 0x4000;		//2011-3-3 18:59 for CSI_CLK = 24 MHz

	arg.bScaler = 1;
	arg.TargetWidth = AVI_WIDTH;
	arg.TargetHeight = AVI_HEIGHT;
	arg.SensorWidth	= AVI_WIDTH;
	#if AVI_WIDTH == 720
	arg.SensorHeight = 432;
	#else
	arg.SensorHeight = AVI_HEIGHT;
	#endif
	arg.DisplayWidth = TFT_WIDTH;
	arg.DisplayHeight = TFT_HEIGHT;
	arg.DisplayBufferWidth = TFT_WIDTH;
	arg.DisplayBufferHeight = TFT_HEIGHT;	
	arg.VidFrameRate = AVI_FRAME_RATE;
	arg.AudSampleRate = 22050;	//8000;		//录像时如果要修改录像文件的音频采样频率, 可以只修改这个值.
	arg.OutputFormat = IMAGE_OUTPUT_FORMAT_RGB565; 
	user_defined_video_codec_entrance();
	video_encode_entrance();	//建立 4+1 个录像 task;
	video_encode_preview_start(arg);
	video_encode_set_jpeg_quality(QUALITY_FACTOR);
	
	R_SYSTEM_CTRL |= 0x30;			//27M => weak mode : |= 0x30
	R_SYSTEM_CLK_CTRL |= 0x1000;	//32768	=> weak mode : |= 0x1000

}

void function_key_scan(void)
{
//R_KeyCode
//=0: 无动作
//=1: KEY_1 短按 1 次
//=2: KEY_1 长按键
//=3: KEY_2 短按 1 次
//=4: KEY_2 长按键
//=5: KEY_3 短按 1 次
//=6: KEY_3 长按键
//=7: KEY_4 短按 1 次
//=8: KEY_4 长按键

	INT8U t, t1, t2;


	#if KEY_1_CONFIG == USED
	while(1)
	{
		t = gpio_read_io(KEY_1_PIN);
		t1 = gpio_read_io(KEY_1_PIN);
		t2 = gpio_read_io(KEY_1_PIN);
		if ((t==t1)&&(t==t2)) break;
	}
	#if KEY_1_TRIGGER_LEVEL == 0
	t ^= 1;
	#endif
	switch (g_key_1_hold)
	{
		case 0:
			g_key_1_debounce = 0;
			if (t) g_key_1_hold = 1;
			break;
		case 1:
			if (! t)
			{
				g_key_1_hold = 0;
				R_KeyCode = 1;
			}
			else
			{
				//if (++g_key_1_debounce > 90) //长按键时间: 900ms
				if (++g_key_1_debounce > (90+30)) //关机按键时间加长
				{
					g_key_1_hold = 2;
					R_KeyCode = 2;
				}
			}
			break;
		default:
			if (! t) g_key_1_hold = 0;
			break;
	
	}
	#endif

	#if KEY_2_CONFIG == USED
	while(1)
	{
		t = gpio_read_io(KEY_2_PIN);
		t1 = gpio_read_io(KEY_2_PIN);
		t2 = gpio_read_io(KEY_2_PIN);
		if ((t==t1)&&(t==t2)) break;
	}
	#if KEY_2_TRIGGER_LEVEL == 0
	t ^= 1;
	#endif
	switch (g_key_2_hold)
	{
		case 0:
			g_key_2_debounce = 0;
			if (t) g_key_2_hold = 1;
			break;
		case 1:
			if (! t)
			{
				g_key_2_hold = 0;
				R_KeyCode = 3;
			}
			else
			{
				if (++g_key_2_debounce > 90) //长按键时间: 900ms
				{
					g_key_2_hold = 2;
					R_KeyCode = 4;
				}
			}
			break;
		default:
			if (! t) g_key_2_hold = 0;
			break;
	
	}
	#endif

	#if KEY_3_CONFIG == USED
	while(1)
	{
		t = gpio_read_io(KEY_3_PIN);
		t1 = gpio_read_io(KEY_3_PIN);
		t2 = gpio_read_io(KEY_3_PIN);
		if ((t==t1)&&(t==t2)) break;
	}
	#if KEY_3_TRIGGER_LEVEL == 0
	t ^= 1;
	#endif
	switch (g_key_3_hold)
	{
		case 0:
			g_key_3_debounce = 0;
			if (t) g_key_3_hold = 1;
			break;
		case 1:
			if (! t)
			{
				g_key_3_hold = 0;
				R_KeyCode = 5;
			}
			else
			{
				if (++g_key_3_debounce > 90) //长按键时间: 900ms
				{
					g_key_3_hold = 2;
					R_KeyCode = 6;
				}
			}
			break;
		default:
			if (! t) g_key_3_hold = 0;
			break;
	
	}
	#endif
	
	#if KEY_4_CONFIG == USED
	while(1)
	{
		t = gpio_read_io(KEY_4_PIN);
		t1 = gpio_read_io(KEY_4_PIN);
		t2 = gpio_read_io(KEY_4_PIN);
		if ((t==t1)&&(t==t2)) break;
	}
	#if KEY_4_TRIGGER_LEVEL == 0
	t ^= 1;
	#endif
	switch (g_key_4_hold)
	{
		case 0:
			g_key_4_debounce = 0;
			if (t) g_key_4_hold = 1;
			break;
		case 1:
			if (! t)
			{
				g_key_4_hold = 0;
				R_KeyCode = 7;
			}
			else
			{
				if (++g_key_4_debounce > 90) //长按键时间: 900ms
				{
					g_key_4_hold = 2;
					R_KeyCode = 8;
				}
			}
			break;
		default:
			if (! t) g_key_4_hold = 0;
			break;
	
	}
	#endif
}
