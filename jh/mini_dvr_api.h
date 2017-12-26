#ifndef __MINI_DVR_API_H__
#define __MINI_DVR_API_H__
//===============================================================================================
#include "project.h"
#include "jh_define.h"

typedef enum
{
	MODE_AVI_ENCODE = 0x00,
	MODE_JPEG_CAPTURE = 0x01,
	MODE_WAV_RECORD = 0x02
}MINI_DVR_MODE_ENUM;

extern volatile INT8U  g_capture_complete_flag;
extern volatile INT8U  g_file_size_over;
extern volatile INT8U  g_avi_packer_error_flag;
extern volatile INT32U g_public_128hz_cnt;
extern volatile INT16U g_savefile_indication;
extern volatile INT8U  g_lvd_flag_savefile;
extern volatile INT32U g_lvd_cnt_savefile;
extern volatile INT32U g_lvd_bak_savefile;

extern volatile INT16U g_6655CD_test_mount_time;
extern volatile INT16U g_6655CD_test_scan_testfile;

extern volatile INT8U  g_usb_disable;
extern volatile INT16U g_sensor_id;
extern volatile INT8U  g_pc_camera;
extern volatile INT8U  g_pccamera_showtime;
extern volatile INT16U R_KeyCode;
extern volatile INT16U R_idle_task_disable;

extern void io_key_init(void);
extern INT8U io_usb_detect(void);
extern INT8U read_usb_disable_key(void);
extern void mount_and_scan_testfile(void);
extern void mount_and_mk_ch_dir(void);
extern INT32S get_file_final_avi_index(void);
extern INT32S get_file_final_jpeg_index(void);
extern INT32S get_file_final_wav_index(void);
extern INT16S open_avi_file_handle(void);
extern INT16S open_jpg_file_handle(void);
extern INT16S open_wav_file_handle(void);
extern INT32S capture_jpeg(void);
extern void avi_update_dir(void);
extern INT32S vid_start(void);
extern INT32S vid_stop(void);
extern BOOLEAN voltage_is_low(void);
extern void voltage_detect_isr(INT16U value);
extern void voltage_detect_charge_mode_isr(INT16U value);
extern void voltage_detect_init(INT8U adc_channel, void (*ad_detect_isr)(INT16U data));
extern void voltage_detect_stop(void);
extern INT32S rtc_time_get_and_start(void);
extern void video_encode_init(void);
extern void function_key_scan(void);
//===============================================================================================
#endif 		// __MINI_DVR_API_H__
