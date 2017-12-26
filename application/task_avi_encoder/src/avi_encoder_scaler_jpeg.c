#include "avi_encoder_app.h"
#include "font.h"
#include "mini_dvr_api.h"
#include "program_01.h"
#include "program_05.h"

#if MPEG4_ENCODE_ENABLE == 1
#include "drv_l1_mpeg4.h"
#endif

/* os task stack size */
#define C_SCALER_STACK_SIZE			128
#define	C_JPEG_STACK_SIZE			128
#define C_SCALER_QUEUE_MAX			5
#define C_JPEG_QUEUE_MAX			5
#if VIDEO_ENCODE_MODE == C_VIDEO_ENCODE_FRAME_MODE
	#define C_CMOS_FRAME_QUEUE_MAX	AVI_ENCODE_CSI_BUFFER_NO
#elif VIDEO_ENCODE_MODE == C_VIDEO_ENCODE_FIFO_MODE
	#define C_CMOS_FRAME_QUEUE_MAX	1024/SENSOR_FIFO_LINE	//for sensor height = 1024
#endif

const INT8U *number[] = {
	acFontHZArial01700030, 
	acFontHZArial01700031, 
	acFontHZArial01700032,
	acFontHZArial01700033, 
	acFontHZArial01700034,
	acFontHZArial01700035,
	acFontHZArial01700036,
	acFontHZArial01700037, 
	acFontHZArial01700038,
	acFontHZArial01700039 
};

/* os tsak stack */
INT32U	ScalerTaskStack[C_SCALER_STACK_SIZE];
INT32U	JpegTaskStack[C_JPEG_STACK_SIZE];

/* os task queue */
OS_EVENT *scaler_task_q; 
OS_EVENT *scaler_task_ack_m;
OS_EVENT *cmos_frame_q;
OS_EVENT *vid_enc_task_q;
OS_EVENT *vid_enc_task_ack_m;
OS_EVENT *scaler_hw_sem;
void *scaler_task_q_stack[C_SCALER_QUEUE_MAX];
void *cmos_frame_q_stack[C_CMOS_FRAME_QUEUE_MAX];
void *video_encode_task_q_stack[C_JPEG_QUEUE_MAX];

#if AVI_ENCODE_PREVIEW_DISPLAY_EN == 1
	INT32U video_preview_flag;
#endif

#if AVI_WIDTH==720
//720x480
#define OSD_LINE_NUM		432
#define OSD_COLUMN_NUM		720
#else
//640x480
#define OSD_LINE_NUM		480
#define OSD_COLUMN_NUM		640
#endif

#define OSD_SPACE			16*2
//=======================================================================
//  Draw OSD function
//=======================================================================
void cpu_draw_osd(const INT8U *source_addr, INT32U target_addr, INT16U offset, INT16U res){
	const INT8U* chptr;
	INT8U i;
	register INT8U tmp;
	register INT32U *ptr;
	register INT32U color_value;
	//if (g_sensor_id == SENSOR_CHIP_OV7680) color_value = 0x59955940;		//深绿色
	//else if (g_sensor_id == SENSOR_CHIP_BF3603) color_value = 0xff80ff80;	//白色
	//else color_value = 0xe301e393;	//黄色
	color_value = 0xe301e393;	//黄色
	ptr = (INT32U*) target_addr;
	ptr+= offset/4;
	chptr = source_addr;
	for(i=0;i<(19-3-4);i++)
	{
		tmp = *chptr++;
		if (tmp&0x40) *ptr = color_value; ptr++;
		if (tmp&0x20) *ptr = color_value; ptr++;
		if (tmp&0x10) *ptr = color_value; ptr++;
		if (tmp&0x08) *ptr = color_value; ptr++;
		if (tmp&0x04) *ptr = color_value; ptr++;
		if (tmp&0x02) *ptr = color_value; ptr++;
		if (tmp&0x01) *ptr = color_value; ptr += 2;
 		ptr += ((OSD_COLUMN_NUM-16)*2)/4;
 	}
}
//=======================================================================
//  Draw OSD function
//=======================================================================
void cpu_draw_time_osd(TIME_T current_time, INT32U target_buffer, INT16U resolution)
{
	INT8U  data;
	INT16U offset, wtemp;
	INT32U line;
	INT32U *ptr;
	
	if (g_pc_camera)
	{//以下是PC Camera 模式的OSD:
  		//++++++++++++++++++++++++++++ For Bonding Test
  		//显示邦定测试结果代码:
		if (g_bonding_test_mode)
		{
			if (g_bonding_test_result != C_B_TEST_OK)
			{
				ptr = (INT32U*) target_buffer;
				for (line=0; line<(OSD_COLUMN_NUM*OSD_LINE_NUM/2); line++)
				{
					*ptr++ = 0x519a51e6;	//红色
				}
				line = target_buffer + (OSD_LINE_NUM/2)*OSD_COLUMN_NUM*2;
				offset = (OSD_COLUMN_NUM/2)*2;
				cpu_draw_osd(number[(g_bonding_test_result%100)/10], line,offset,resolution);
				cpu_draw_osd(number[(g_bonding_test_result%10)/1], line,offset+OSD_SPACE,resolution);
				return;
			}
		}
   		//---------------------------- For Bonding Test
		if (g_pc_camera == 1) return;	//进入 PC Camera 默认不显示 OSD 信息;
		line = target_buffer + (OSD_LINE_NUM-40-30)*OSD_COLUMN_NUM*2;
		offset = (OSD_COLUMN_NUM-9*8*2)*2;
		//+++++++++++++++++++++++++++++++++++++++ 显示产品编号
		/*
		#if CUSTOMIZE_FOR_DIWEI==1
		cpu_draw_osd(acFont_d,								line,offset+OSD_SPACE*0,resolution);
		cpu_draw_osd(acFont_v,								line,offset+OSD_SPACE*1,resolution);
		#endif			
		*/
		cpu_draw_osd(number[(PRODUCT_NUM%10000)/1000],line,offset+OSD_SPACE*0-OSD_SPACE*2,resolution);
		cpu_draw_osd(number[(PRODUCT_NUM%1000)/100],  line,offset+OSD_SPACE*0-OSD_SPACE*1,resolution);
		cpu_draw_osd(number[(PRODUCT_NUM%100)/10],	  line,offset+OSD_SPACE*0,            resolution);
		cpu_draw_osd(number[(PRODUCT_NUM%10)/1],      line,offset+OSD_SPACE*1,            resolution);
		//--------------------------------------- 显示产品编号
		cpu_draw_osd(acFont_v,								line,offset+OSD_SPACE*3,resolution);
		cpu_draw_osd(number[(PROGRAM_VERSION_NUM%1000)/100],line,offset+OSD_SPACE*4,resolution);
		cpu_draw_osd(acFont_dot,							line,offset+OSD_SPACE*5,resolution);
		cpu_draw_osd(number[(PROGRAM_VERSION_NUM%100)/10],	line,offset+OSD_SPACE*6,resolution);
		cpu_draw_osd(number[(PROGRAM_VERSION_NUM%10)/1],	line,offset+OSD_SPACE*7,resolution);
		if (g_pccamera_showtime==0) return;
	}
	//以下是正常拍照/录像的OSD:
	line = target_buffer + (OSD_LINE_NUM-40)*OSD_COLUMN_NUM*2;
	offset = (OSD_COLUMN_NUM-160*2)*2;

#if CUSTOMIZE_FOR_DIWEI==1
//显示: 月 / 日 / 年
	// year
	wtemp = current_time.tm_year; 
	data = wtemp/1000;
	wtemp -= data*1000;
	cpu_draw_osd(number[data], line, offset+OSD_SPACE*6, resolution);
	data = wtemp/100;
	wtemp -= data*100;
	cpu_draw_osd(number[data],line,offset+OSD_SPACE*7,resolution);
	data = wtemp/10;
	wtemp -= data*10;
	cpu_draw_osd(number[data],line,offset+OSD_SPACE*8,resolution);
	data = wtemp;
	cpu_draw_osd(number[data],line,offset+OSD_SPACE*9,resolution);
	
	// '/'
	cpu_draw_osd(acFontHZArial017Slash,line,offset+OSD_SPACE*5,resolution);
	
	//month
	wtemp = current_time.tm_mon; 
	cpu_draw_osd(number[wtemp/10],line,offset+OSD_SPACE*0,resolution);
	cpu_draw_osd(number[wtemp%10],line,offset+OSD_SPACE*1,resolution);
	
	// '/'
	cpu_draw_osd(acFontHZArial017Slash,line,offset+OSD_SPACE*2,resolution);
	
	//day
	wtemp = current_time.tm_mday;
	cpu_draw_osd(number[wtemp/10],line,offset+OSD_SPACE*3,resolution);
	cpu_draw_osd(number[wtemp%10],line,offset+OSD_SPACE*4,resolution);
#else
//显示: 年 / 月 / 日
	// year
	wtemp = current_time.tm_year; 
	data = wtemp/1000;
	wtemp -= data*1000;
	cpu_draw_osd(number[data], line, offset, resolution);
	data = wtemp/100;
	wtemp -= data*100;
	cpu_draw_osd(number[data],line,offset+OSD_SPACE*1,resolution);
	data = wtemp/10;
	wtemp -= data*10;
	cpu_draw_osd(number[data],line,offset+OSD_SPACE*2,resolution);
	data = wtemp;
	cpu_draw_osd(number[data],line,offset+OSD_SPACE*3,resolution);
	
	// '/'
	cpu_draw_osd(acFontHZArial017Slash,line,offset+OSD_SPACE*4,resolution);
	
	//month
	wtemp = current_time.tm_mon; 
	cpu_draw_osd(number[wtemp/10],line,offset+OSD_SPACE*5,resolution);
	cpu_draw_osd(number[wtemp%10],line,offset+OSD_SPACE*6,resolution);
	
	// '/'
	cpu_draw_osd(acFontHZArial017Slash,line,offset+OSD_SPACE*7,resolution);
	
	//day
	wtemp = current_time.tm_mday;
	cpu_draw_osd(number[wtemp/10],line,offset+OSD_SPACE*8,resolution);
	cpu_draw_osd(number[wtemp%10],line,offset+OSD_SPACE*9,resolution);
#endif
	//hour
	wtemp = current_time.tm_hour;
	cpu_draw_osd(number[wtemp/10],line,offset+OSD_SPACE*11,resolution);
	cpu_draw_osd(number[wtemp%10],line,offset+OSD_SPACE*12,resolution);
	
	// ':'
	cpu_draw_osd(acFontHZArial017Comma, line,offset+OSD_SPACE*13,resolution);
	
	//minute
	wtemp = current_time.tm_min;
	cpu_draw_osd(number[wtemp/10],line,offset+OSD_SPACE*14,resolution);
	cpu_draw_osd(number[wtemp%10],line,offset+OSD_SPACE*15,resolution);
	
	// ':'
	cpu_draw_osd(acFontHZArial017Comma,line,offset+OSD_SPACE*16,resolution);
	
	//second
	wtemp = current_time.tm_sec;
	cpu_draw_osd(number[wtemp/10], line,offset+OSD_SPACE*17,resolution);
	cpu_draw_osd(number[wtemp%10],line,offset+OSD_SPACE*18,resolution);	
}

#if 0
extern INT16U ad_value;
extern INT16U ad_18_value;
void cpu_draw_advalue_osd(INT16U value, INT32U target_buffer, INT16U resolution, INT16U aa)
{
	INT8U  data;
	INT16U offset, space, wtemp;
	INT32U line;
	if (resolution == 320){
		line = target_buffer + 220*resolution*2;//QVGA
		offset = 250*2;
		}
	else{
		line = target_buffer + aa*resolution*2;//VGA	
		offset = 480*2;
	}
	space = 16;
//Arial 17
	// year
	wtemp = value; 
	data = wtemp/10000;
	wtemp -= data*10000;
	cpu_draw_osd(number[data], line, offset, resolution);
	data = wtemp/1000;
	wtemp -= data*1000;
	cpu_draw_osd(number[data],line,offset+space*1,resolution);
	data = wtemp/100;
	wtemp -= data*100;
	cpu_draw_osd(number[data],line,offset+space*2,resolution);
	data = wtemp/10;
	wtemp -= data*10;
	cpu_draw_osd(number[data],line,offset+space*3,resolution);
	data = wtemp;
	cpu_draw_osd(number[data],line,offset+space*4,resolution);
	
}
#endif
///////////////////////////////////////////////////////////////////////////////////////////////////////
// scaler task
///////////////////////////////////////////////////////////////////////////////////////////////////////
INT32S scaler_task_create(INT8U pori)
{
	INT8U  err;
	INT32S nRet;
	
	scaler_task_q = OSQCreate(scaler_task_q_stack, C_SCALER_QUEUE_MAX);
	if(!scaler_task_q) RETURN(STATUS_FAIL);
	
	scaler_task_ack_m = OSMboxCreate(NULL);
	if(!scaler_task_ack_m) RETURN(STATUS_FAIL);
	
	cmos_frame_q = OSQCreate(cmos_frame_q_stack, C_CMOS_FRAME_QUEUE_MAX);
	if(!cmos_frame_q) RETURN(STATUS_FAIL);	
	
	scaler_hw_sem = OSSemCreate(1);
	if(!scaler_hw_sem) RETURN(STATUS_FAIL);
	
	err = OSTaskCreate(scaler_task_entry, (void *)NULL, &ScalerTaskStack[C_SCALER_STACK_SIZE - 1], pori);	
	if(err != OS_NO_ERR) RETURN(STATUS_FAIL);
	
	nRet = STATUS_OK;
Return:
	return nRet;
}

INT32S scaler_task_del(void)
{
	INT8U  err;
	INT32S nRet, msg;
	
	nRet = STATUS_OK;
	POST_MESSAGE(scaler_task_q, MSG_SCALER_TASK_EXIT, scaler_task_ack_m, 5000, msg, err);
Return:	
	OSQFlush(scaler_task_q);
   	OSQDel(scaler_task_q, OS_DEL_ALWAYS, &err);
   	
   	OSQFlush(cmos_frame_q);
   	OSQDel(cmos_frame_q, OS_DEL_ALWAYS, &err);
   	
	OSMboxDel(scaler_task_ack_m, OS_DEL_ALWAYS, &err);
	OSSemDel(scaler_hw_sem, OS_DEL_ALWAYS, &err);
	return nRet;
}

INT32S scaler_task_start(void)
{
	INT8U  err;
	INT32S nRet, msg;
//	mm_dump();
	if(avi_encode_memory_alloc() < 0)
	{
		avi_encode_memory_free();
		DEBUG_MSG(DBG_PRINT("avi memory alloc fail!!!\r\n"));
		RETURN(STATUS_FAIL);				
	}
	
	nRet = STATUS_OK;
	POST_MESSAGE(scaler_task_q, MSG_SCALER_TASK_INIT, scaler_task_ack_m, 5000, msg, err);
Return:		
	return nRet;	
}

INT32S scaler_task_stop(void)
{
	INT8U  err;
	INT32S nRet, msg;
	
	nRet = STATUS_OK;
	POST_MESSAGE(scaler_task_q, MSG_SCALER_TASK_STOP, scaler_task_ack_m, 5000, msg, err);
Return:		
	avi_encode_memory_free();
	return nRet;
}

void scaler_task_entry(void *parm)
{
	INT8U err, scaler_mode, skip_cnt;
	INT16U csi_width, csi_height;
	INT16U encode_width, encode_height; 
	INT16U dip_width, dip_height;
	INT16U dip_buff_width, dip_buff_height;
	INT32U msg_id, sensor_frame, scaler_frame, display_frame;
	INT32U input_format, output_format, display_input_format, display_output_format;

	while(1)
	{
		msg_id = (INT32U) OSQPend(scaler_task_q, 0, &err);
		if((err != OS_NO_ERR)||	!msg_id)
			continue;
			
		switch(msg_id & 0xFF000000)
		{
		case MSG_SCALER_TASK_INIT:
			skip_cnt = 0;
			csi_width = pAviEncVidPara->sensor_capture_width;
			csi_height = pAviEncVidPara->sensor_capture_height;
			encode_width = pAviEncVidPara->encode_width;
			encode_height = pAviEncVidPara->encode_height;
			dip_width = pAviEncVidPara->display_width;
			dip_height = pAviEncVidPara->display_height;
			dip_buff_width = pAviEncVidPara->display_buffer_width;
			dip_buff_height = pAviEncVidPara->display_buffer_height;
			input_format = pAviEncVidPara->sensor_output_format;
			output_format = C_SCALER_CTRL_OUT_YUYV;
			display_frame = 0;
			pAviEncPara->fifo_enc_err_flag = 0;
			pAviEncPara->fifo_irq_cnt = 0; 
			pAviEncPara->vid_pend_cnt = 0;
			pAviEncPara->vid_post_cnt = 0;
			display_input_format = C_SCALER_CTRL_IN_YUYV;
			display_output_format = pAviEncVidPara->display_output_format;
			if(pAviEncVidPara->scaler_flag)
				scaler_mode = C_SCALER_FIT_BUFFER;
				//scaler_mode = C_NO_SCALER_FIT_BUFFER;
			else
				scaler_mode = C_SCALER_FULL_SCREEN;
				
			OSMboxPost(scaler_task_ack_m, (void*)C_ACK_SUCCESS);
			break;
		
		case MSG_SCALER_TASK_STOP:
			OSQFlush(scaler_task_q);
			OSMboxPost(scaler_task_ack_m, (void*)C_ACK_SUCCESS);
			break;
				
		case MSG_SCALER_TASK_EXIT:
			OSMboxPost(scaler_task_ack_m, (void*)C_ACK_SUCCESS);
			OSTaskDel(OS_PRIO_SELF);
			break;
#if AVI_ENCODE_PREVIEW_DISPLAY_EN == 1		
		case MSG_SCALER_TASK_PREVIEW_ON:
			video_preview_flag = 1;
			break;
		
		case MSG_SCALER_TASK_PREVIEW_OFF:
			video_preview_flag = 0;
			break;
		
		case MSG_SCALER_TASK_PREVIEW_UNLOCK:
			{
				INT32U ack_ptr, tmp_ptr, i;
				ack_ptr = (msg_id & 0xFFFFFF);
				for (i=0;i<AVI_ENCODE_DISPALY_BUFFER_NO;i++) {
					tmp_ptr = (pAviEncVidPara->display_output_addr[i] & 0xFFFFFF);
					if (ack_ptr == tmp_ptr) {
						pAviEncVidPara->display_output_addr[i] = ack_ptr;
						break;
					}
				}
			} 
			break;
#endif		
		default:
			sensor_frame = msg_id;
		#if AVI_ENCODE_DIGITAL_ZOOM_EN == 1
			scaler_frame = avi_encode_get_scaler_frame();
			scaler_zoom_once(C_SCALER_ZOOM_FIT_BUFFER,
							pAviEncVidPara->scaler_zoom_ratio,
							input_format, output_format, 
							csi_width, csi_height, 
							encode_width, encode_height,
							encode_width, encode_height, 
							sensor_frame, 0, 0, 
							scaler_frame, 0, 0);
    	#else
    		if(pAviEncVidPara->scaler_flag)
    		{   
    			scaler_frame = avi_encode_get_scaler_frame();
    			scaler_zoom_once(C_SCALER_FULL_SCREEN,
								pAviEncVidPara->scaler_zoom_ratio,
								input_format, output_format, 
								csi_width, csi_height, 
								encode_width, encode_height,
								encode_width, encode_height, 
								sensor_frame, 0, 0, 
								scaler_frame, 0, 0);
    		}
    		else
    		{
    			scaler_frame = sensor_frame;	
			}
		#endif
/*		#if	AVI_ENCODE_SHOW_TIME
			{
				if (!ap_state_config_date_stamp_get()) {
					TIME_T	g_osd_time;
					cal_time_get(&g_osd_time);
					cpu_draw_time_osd(g_osd_time, scaler_frame);
				}
			}
		#endif*/
		 	if(AVI_ENCODE_DIGITAL_ZOOM_EN || pAviEncVidPara->scaler_flag)
		 	{
//				OSQPost(cmos_frame_q, (void *)sensor_frame);
			}
			
			
		#if AVI_ENCODE_PREVIEW_DISPLAY_EN == 1	
			if(pAviEncVidPara->dispaly_scaler_flag)
			{	
				INT32U *display_ptr;
				
				display_ptr = (INT32U *) avi_encode_get_display_frame();
				if (display_ptr && video_preview_flag) {
					if (skip_cnt == 0) {
						display_frame = *display_ptr;
						scaler_zoom_once(scaler_mode, 
										pAviEncVidPara->scaler_zoom_ratio,
										display_input_format, display_output_format, 
										encode_width, encode_height, 
										dip_width, dip_height,
										dip_buff_width, dip_buff_height, 
										scaler_frame, 0, 0, 
										display_frame, 0, 0);					
						*display_ptr |= MSG_SCALER_TASK_PREVIEW_LOCK;
						OSQPost(DisplayTaskQ, (void *) display_frame);
					}
				}
				skip_cnt++;
				if (skip_cnt > 1) {
					skip_cnt = 0;
				}
        	}
        	else
        	{
				display_frame =	scaler_frame;
			}
		#endif
#if 0 //VIDEO_ENCODE_USE_MODE != VIDEO_ENCODE_WITH_FIFO_IRQ			
			if ((avi_encode_get_status() & C_AVI_VID_ENC_START) || (avi_encode_get_status() & C_AVI_ENCODE_USB_WEBCAM))
			{
				if(pAviEncPara->vid_post_cnt == pAviEncPara->vid_pend_cnt)
				{
					pAviEncPara->vid_post_cnt++;
					OSQPost(vid_enc_task_q, (void *)scaler_frame);
				}
			}
	
			if((AVI_ENCODE_DIGITAL_ZOOM_EN == 0) && (pAviEncVidPara->scaler_flag ==0))
			{
				OSQPost(cmos_frame_q, (void *)sensor_frame);
			}
#endif			
		}
	}		
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
//	video encode task
///////////////////////////////////////////////////////////////////////////////////////////////////////
INT32S video_encode_task_create(INT8U pori)
{
	INT8U  err;
	INT32S nRet;
	
	vid_enc_task_q = OSQCreate(video_encode_task_q_stack, C_JPEG_QUEUE_MAX);
	if(!scaler_task_q) RETURN(STATUS_FAIL);
	
	vid_enc_task_ack_m = OSMboxCreate(NULL);
	if(!scaler_task_ack_m) RETURN(STATUS_FAIL);
	
	err = OSTaskCreate(video_encode_task_entry, (void *)NULL, &JpegTaskStack[C_JPEG_STACK_SIZE-1], pori); 	
	if(err != OS_NO_ERR) RETURN(STATUS_FAIL);
		
	nRet = STATUS_OK;
Return:
	return nRet;
}

INT32S video_encode_task_del(void)
{
	INT8U  err;
	INT32S nRet, msg;
	
	nRet = STATUS_OK;
	POST_MESSAGE(vid_enc_task_q, MSG_VIDEO_ENCODE_TASK_EXIT, vid_enc_task_ack_m, 5000, msg, err);
Return:	
	OSQFlush(vid_enc_task_q);
   	OSQDel(vid_enc_task_q, OS_DEL_ALWAYS, &err);
	OSMboxDel(vid_enc_task_ack_m, OS_DEL_ALWAYS, &err);
	return nRet;
}

INT32S video_encode_task_start(void)
{
	INT8U  err;
	INT32S nRet, msg;
	
	nRet = STATUS_OK;
	POST_MESSAGE(vid_enc_task_q, MSG_VIDEO_ENCODE_TASK_MJPEG_INIT, vid_enc_task_ack_m, 5000, msg, err);
Return:
	return nRet;	
}

INT32S video_encode_task_stop(void)
{
	INT8U  err;
	INT32S nRet, msg;
	
	nRet = STATUS_OK;
	POST_MESSAGE(vid_enc_task_q, MSG_VIDEO_ENCODE_TASK_STOP, vid_enc_task_ack_m, 5000, msg, err);
Return:
    return nRet;	
}

//INT32U time_tick, sum2, sum3, cnt;
void video_encode_task_entry(void *parm)
{
	INT8U   err;
	INT16U  encode_width, encode_height;
	INT16U	csi_width, csi_height;
	INT32S  header_size, encode_size;
	INT32U	output_frame, video_frame;
	INT32U  msg_id, yuv_sampling_mode;
#if MPEG4_ENCODE_ENABLE == 1
	#define MAX_P_FRAME		10
	INT8U	time_inc_bit, p_cnt;
	INT32U  temp, write_refer_addr, reference_addr; 
#endif
#if VIDEO_ENCODE_MODE == C_VIDEO_ENCODE_FIFO_MODE
	INT8U  jpeg_start_flag;
	INT16U scaler_height; 
	INT32U input_y_len, input_uv_len, blk_size;
	INT32U y_frame, u_frame, v_frame; 
	INT32S status, nRet;
#else
	INT32U scaler_frame;
#endif
#if C_UVC == CUSTOM_ON
	ISOTaskMsg isosend;
#endif
	//R_IOC_DIR |= 0x0C; R_IOC_ATT |= 0x0C; R_IOC_O_DATA = 0x0;
	while(1)
	{
		msg_id = (INT32U) OSQPend(vid_enc_task_q, 0, &err);
		if(err != OS_NO_ERR)
		    continue;

		switch(msg_id)
		{
		case MSG_VIDEO_ENCODE_TASK_MJPEG_INIT:
		 	encode_width = pAviEncVidPara->encode_width;
		 	encode_height = pAviEncVidPara->encode_height;
		 	csi_width = pAviEncVidPara->sensor_capture_width;
		 	csi_height = pAviEncVidPara->sensor_capture_height;
		 	yuv_sampling_mode = C_JPEG_FORMAT_YUYV;
			output_frame = 0;
		#if VIDEO_ENCODE_MODE == C_VIDEO_ENCODE_FIFO_MODE
			pAviEncPara->vid_post_cnt = pAviEncVidPara->sensor_capture_height / SENSOR_FIFO_LINE;
			if(pAviEncVidPara->sensor_capture_height % SENSOR_FIFO_LINE)
				while(1);//this must be no remainder

			jpeg_start_flag = 0;
			scaler_height = pAviEncVidPara->sensor_capture_height / pAviEncPara->vid_post_cnt;
			input_y_len = pAviEncVidPara->sensor_capture_width * scaler_height;
			input_uv_len = input_y_len >> 1; //YUV422
			//input_uv_len = input_y_len >> 2; //YUV420
			blk_size = input_y_len << 1;
			if(pAviEncVidPara->scaler_flag)
				yuv_sampling_mode = C_JPEG_FORMAT_YUV_SEPARATE;
			else
				yuv_sampling_mode = C_JPEG_FORMAT_YUYV;
		#endif
		#if AVI_ENCODE_VIDEO_ENCODE_EN == 1
			header_size = avi_encode_set_jpeg_quality(pAviEncVidPara->quality_value);
		#endif
			OSMboxPost(vid_enc_task_ack_m, (void*)C_ACK_SUCCESS);
			break;				 		
		case MSG_VIDEO_ENCODE_TASK_STOP:
		#if MPEG4_ENCODE_ENABLE == 1	
			if(pAviVidPara->video_format == C_XVID_FORMAT)
				avi_encode_mpeg4_free();
		#endif
			OSQFlush(vid_enc_task_q);
			OSMboxPost(vid_enc_task_ack_m, (void*)C_ACK_SUCCESS);
			break;
			
		case MSG_VIDEO_ENCODE_TASK_EXIT:
			OSMboxPost(vid_enc_task_ack_m, (void*)C_ACK_SUCCESS);	
			OSTaskDel(OS_PRIO_SELF);
			break;
		
		default:
#if VIDEO_ENCODE_MODE == C_VIDEO_ENCODE_FRAME_MODE
//			pAviEncPara->vid_pend_cnt++;
			if ((avi_encode_get_status() & C_AVI_VID_ENC_START) || (avi_encode_get_status() & C_AVI_ENCODE_USB_WEBCAM)) {
				if (msg_id != 0x40000000) {
					scaler_frame = msg_id;
				} else {
					break;
				}
			} else {
				break;
			}
#if C_UVC == CUSTOM_ON			
			if((avi_encode_get_status()&C_AVI_ENCODE_USB_WEBCAM))
			{
				video_frame = (INT32U) OSQAccept(usbwebcam_frame_q, &err);
				if(!((err == OS_NO_ERR) && video_frame))
    			{
    				goto VIDEO_ENCODE_FRAME_MODE_END;
    			}
				 	
			}
			else
			{
				video_frame = avi_encode_get_video_frame();
			}
#else
				video_frame = avi_encode_get_video_frame();
#endif		
			output_frame = video_frame + header_size;
		
			if (1) {	//if (s_usbd_pin == 0) {
				TIME_T	g_osd_time;
				cal_time_get(&g_osd_time);
				//cpu_draw_time_osd(g_osd_time, scaler_frame, 720);
				cpu_draw_time_osd(g_osd_time, scaler_frame, AVI_WIDTH);
#if 0
				cpu_draw_advalue_osd(ad_value, scaler_frame, 720, 220);
				cpu_draw_advalue_osd(ad_18_value, scaler_frame, 720, 320);
#endif				
			}
			if(pAviEncVidPara->video_format == C_MJPG_FORMAT)
			{
				encode_size = jpeg_encode_once(pAviEncVidPara->quality_value, yuv_sampling_mode, 
												encode_width, encode_height, scaler_frame, 0, 0, output_frame);
				pAviEncPara->ready_frame = video_frame;
				pAviEncPara->ready_size = encode_size + header_size;
				pAviEncPara->key_flag = AVIIF_KEYFRAME;
				cache_invalid_range(pAviEncPara->ready_frame, pAviEncPara->ready_size);
			}
#if C_UVC == CUSTOM_ON
			if((avi_encode_get_status()&C_AVI_ENCODE_USB_WEBCAM))
			{
				isosend.FrameSize = pAviEncPara->ready_size;
				isosend.AddrFrame = video_frame;
				err = OSQPost(USBCamApQ, (void*)(&isosend));//usb start send data
			}
#endif
VIDEO_ENCODE_FRAME_MODE_END:		
			break;			
#elif VIDEO_ENCODE_MODE == C_VIDEO_ENCODE_FIFO_MODE				
			pAviEncPara->vid_pend_cnt++;
			if (msg_id & 0x80000000) {
				if (pAviEncPara->vid_pend_cnt == pAviEncPara->vid_post_cnt) {
					pAviEncPara->vid_pend_cnt = 0;
				} else {
					pAviEncPara->vid_pend_cnt = 0xFF;
				}
			}
			msg_id &= 0x7FFFFFFF;
			if (msg_id != 0x40000000) {
				y_frame = msg_id;
				//if (pAviEncPara->vid_pend_cnt == pAviEncPara->vid_post_cnt) {
				if (pAviEncPara->vid_pend_cnt == 0) {
					if (!(avi_encode_get_status() & C_AVI_ENCODE_USB_WEBCAM)) {
						TIME_T	g_osd_time;
						cal_time_get(&g_osd_time);
						cpu_draw_time_osd(g_osd_time, y_frame, 720);
					}
					nRet = 3;
					//pAviEncPara->vid_pend_cnt = 0; 
				} else if (pAviEncPara->vid_pend_cnt == 1) {
					jpeg_start_flag = 1;
					nRet = 1;
				} else if (pAviEncPara->vid_pend_cnt < pAviEncPara->vid_post_cnt) {
					nRet = 2;
				} else if (pAviEncPara->vid_pend_cnt == 0xFF) {
					pAviEncPara->vid_pend_cnt = 0;
					jpeg_start_flag = 0;
					nRet = 4;
				}		
				if ((pAviEncPara->avi_encode_status & C_AVI_VID_ENC_START) || (avi_encode_get_status() & C_AVI_ENCODE_USB_WEBCAM) || capture_flag) {
					if (!jpeg_start_flag) {
						nRet = 4;
					}
				} else {
					jpeg_start_flag = 0;
					nRet = 4;
				}
			} else {
//				DBG_PRINT("*");
				if (pAviEncPara->vid_pend_cnt == pAviEncPara->vid_post_cnt) {
					pAviEncPara->vid_pend_cnt = 0;
				}
				pAviEncPara->ready_frame = pAviEncPara->ready_size = pAviEncPara->key_flag = 0;
				y_frame = jpeg_start_flag = 0;
				nRet = 4;
			}			
			u_frame = v_frame = 0;			
			switch(nRet)
			{
				case 1:
					if (!capture_flag) {
//						video_frame = avi_encode_get_video_frame();
#if C_UVC == CUSTOM_ON			
						if ((avi_encode_get_status() & C_AVI_ENCODE_USB_WEBCAM)) {
							video_frame = (INT32U) OSQAccept(usbwebcam_frame_q, &err);
							if(!((err == OS_NO_ERR) && video_frame)) {
			    				if (pAviEncPara->vid_pend_cnt == pAviEncPara->vid_post_cnt) {
									pAviEncPara->vid_pend_cnt = 0;
								}
								pAviEncPara->ready_frame = pAviEncPara->ready_size = pAviEncPara->key_flag = 0;
								y_frame = jpeg_start_flag = 0;
			    				break;
			    			}
			    			pAviEncPara->ready_size = 0;
						} else {
							video_frame = avi_encode_get_video_frame();
						}
#else
						video_frame = avi_encode_get_video_frame();
#endif						
						status = jpeg_encode_fifo_start(0, 
														pAviEncVidPara->quality_value, 
														yuv_sampling_mode, 
														encode_width, encode_height, 
														y_frame, u_frame, v_frame, 
														video_frame + header_size, input_y_len, input_uv_len);
						if(status < 0) {
							DEBUG_MSG(DBG_PRINT("E5"));
							goto VIDEO_ENCODE_FIFO_MODE_FAIL;
						}
					} else {
						gp_memcpy_align((void *)write_buff_addr, (void *) y_frame, blk_size);
					}
					break;
					
				case 2:
					if (!capture_flag) {
						status = jpeg_encode_fifo_once(	0, 
														status, 
														y_frame, u_frame, v_frame, 
														input_y_len, input_uv_len);
						if(status < 0) {
							DEBUG_MSG(DBG_PRINT("E6"));
							goto VIDEO_ENCODE_FIFO_MODE_FAIL;
						}
					} else {
						gp_memcpy_align((void *)(write_buff_addr + ((pAviEncPara->vid_pend_cnt -1)*blk_size)), (void *) y_frame, blk_size);
					}
					break;
					
				case 3:
					if (!capture_flag) {
						encode_size = jpeg_encode_fifo_stop(0, 
															status, 
															y_frame, u_frame, v_frame, 
															input_y_len, input_uv_len);
						if (encode_size > 0) {
							if (pAviEncVidPara->video_format == C_MJPG_FORMAT && pAviEncPara->ready_size == 0) {
								pAviEncPara->ready_frame = video_frame;
								pAviEncPara->ready_size = encode_size + header_size;
								pAviEncPara->key_flag = AVIIF_KEYFRAME;
								cache_invalid_range(pAviEncPara->ready_frame, pAviEncPara->ready_size);
								vid_enc_buffer_lock(pAviEncPara->ready_frame);
							}
#if C_UVC == CUSTOM_ON
							if ((avi_encode_get_status() & C_AVI_ENCODE_USB_WEBCAM)) {
								isosend.FrameSize = pAviEncPara->ready_size;
								isosend.AddrFrame = video_frame;
								err = OSQPost(USBCamApQ, (void*)(&isosend));
							}
#endif							
						} else {
							DEBUG_MSG(DBG_PRINT("E7"));
							goto VIDEO_ENCODE_FIFO_MODE_FAIL;
						}
					} else {
						gp_memcpy_align((void *)(write_buff_addr + ((pAviEncPara->vid_post_cnt - 1)*blk_size)), (void *) y_frame, blk_size);
						capture_flag = 0;
						OSQPost(AVIEncodeApQ, (void *) MSG_AVI_CAPTURE_PICTURE);
					}
					break;
				case 4:
					break;
			}
			if (y_frame) {
				OSQPost(cmos_frame_q, (void *)y_frame);
			}
			break;

VIDEO_ENCODE_FIFO_MODE_FAIL:
			DEBUG_MSG(DBG_PRINT("E"));
			pAviEncPara->vid_pend_cnt = 0;
			pAviEncPara->ready_frame = 0;
			pAviEncPara->ready_size = 0;
			pAviEncPara->key_flag = 0;
			if(jpeg_start_flag == 1)
			{
				jpeg_start_flag = 0;
				jpeg_encode_stop();
			}
#endif
		break;
		}
	}	
}
///////////////////////////////////////////////////////////////////////////////////////////////////////
