#include "video_codec_callback.h"
#include "video_codec_osd.h"

#if VIDEO_ENCODE_USE_MODE ==VIDEO_ENCODE_WITH_FIFO_IRQ
	INT8U csi_fifo_flag = 0;
#endif
INT8U  Display_Device = DISPLAY_DEVICE;
INT8U  g_display_mode = TV_TFT_MODE;
INT32U video_codec_show_buffer;
INT32U video_codec_display_flag;
void 	(*tft_vblank_callback)(void);
#if C_MOTION_DETECTION == CUSTOM_ON	
	void 	(*motion_detect_callback)(void);
#endif	

#if VIDEO_DISPALY_WITH_PPU
extern PPU_REGISTER_SETS *video_ppu_register_set;
#endif
extern TIME_T	g_current_time;
	
//=======================================================================================
//		Video decode stop hook function 
//=======================================================================================
void video_decode_end(void)
{
	DBG_PRINT("AVI Decode play-end callback...\r\n");
}

//=======================================================================================
//		Video decode output buffer hook function 
//=======================================================================================
void video_decode_FrameReady(INT8U *FrameBufPtr)
{
#if	VIDEO_DISPALY_WITH_PPU == 0
    video_codec_show_buffer = (INT32U)FrameBufPtr;
 	video_codec_display_flag = 1;
#else	//display with PPU
	gplib_ppu_text_calculate_number_array(video_ppu_register_set, C_PPU_TEXT1, 640, 480, (INT232U)FrameBufPtr);	
	result = gplib_ppu_go_and_wait_done(video_ppu_register_set);
#endif
}


//=======================================================================
//  Video encode hook function 
//=======================================================================
//=======================================================================
//		sensor start 
//=======================================================================
INT32U video_encode_sensor_start(INT32U csi_frame1, INT32U csi_frame2)
{
	OS_CPU_SR cpu_sr;
	// Setup CMOS sensor
#if VIDEO_ENCODE_USE_MODE == VIDEO_ENCODE_WITH_PPU_IRQ
	OS_ENTER_CRITICAL();
	CSI_Init(640, 480, FT_CSI_YUVIN|FT_CSI_YUVOUT|FT_CSI_RGB1555, csi_frame1, NULL);
	OS_EXIT_CRITICAL();
	R_PPU_IRQ_STATUS = 0x40;
	R_PPU_IRQ_EN |= 0x40;	//enable csi frame end irq
#elif VIDEO_ENCODE_USE_MODE == VIDEO_ENCODE_WITH_TG_IRQ
	OS_ENTER_CRITICAL();
	CSI_Init(640, 480, FT_CSI_YUVIN|FT_CSI_YUVOUT|FT_CSI_RGB1555, csi_frame1, NULL);
	OS_EXIT_CRITICAL();
	R_TGR_IRQ_STATUS = 0x1;
	R_TGR_IRQ_EN = 0x1;	//enable csi frame end irq
	vic_irq_enable(VIC_CSI);
#elif VIDEO_ENCODE_USE_MODE == VIDEO_ENCODE_WITH_FIFO_IRQ
	R_TGR_IRQ_STATUS = 0x31;
	OS_ENTER_CRITICAL();
	CSI_Init(640, 480, FT_CSI_YUVIN|FT_CSI_YUVOUT|FT_CSI_RGB1555, csi_frame1, csi_frame2);
	OS_EXIT_CRITICAL();
	R_TGR_IRQ_EN = 0x21;	//enable csi fifo irq and frame end irq
	R_TGR_IRQ_EN |= 0x10;	//enable csi fifo under run
		vic_irq_enable(VIC_CSI);
#endif	
    return 0;
}

//=======================================================================
//		sensor stop 
//=======================================================================
INT32U video_encode_sensor_stop(void)
{
  	//close sensor
#if VIDEO_ENCODE_USE_MODE == VIDEO_ENCODE_WITH_PPU_IRQ
 	R_PPU_IRQ_EN &= ~0x40;
 	R_PPU_IRQ_STATUS = 0x40;	
#elif VIDEO_ENCODE_USE_MODE == VIDEO_ENCODE_WITH_TG_IRQ
	R_TGR_IRQ_EN &= ~0x1;
	R_TGR_IRQ_STATUS = 0x1;	
#elif VIDEO_ENCODE_USE_MODE == VIDEO_ENCODE_WITH_FIFO_IRQ
	R_TGR_IRQ_EN &= ~0x21;
	R_TGR_IRQ_STATUS = 0x31;
#endif
	R_CSI_TG_CTRL0 = 0;
	R_CSI_TG_CTRL1 = 0;	
    return 0;
}

//=======================================================================
//		preview frame buffer ready  
//=======================================================================
INT32S video_encode_display_frame_ready(INT32U frame_buffer)
{

#if	VIDEO_DISPALY_WITH_PPU == 0           
	video_codec_display_flag = 1;
  	video_codec_show_buffer = frame_buffer;
#else	//display with ppu
	INT32S result;
	frame_buffer = R_PPU_FBO_ADDR;//g_ppu_frame;
	gplib_ppu_text_calculate_number_array(video_ppu_register_set, C_PPU_TEXT1, 640, 480, frame_buffer);	// Calculate Number array
	result = gplib_ppu_go(video_ppu_register_set);
	if(result < 0)
		frame_buffer =  -1;	
#endif

	return frame_buffer;		
}

//=======================================================================
//		encode frame buffer ready when use user define mode 
//=======================================================================
INT32S  video_encode_frame_ready(void *workmem, unsigned long fourcc, long cbLen, const void *ptr, int nSamples, int ChunkFlag)
{
	if(fourcc == 0x63643030)
	{	//"00dc", video frame ready 
	
	}
	else if(fourcc == 0x62773130)
	{	//"01wb", audio frame ready
	
	}
	else
		return STATUS_FAIL;
	
	return STATUS_OK;
}

//=======================================================================
//		avi encode end, 
// ISRC: source; IART: Artist; ICOP: copyright; IARL: achival location
// ICMS: commissioned; ICMT: comments;  ICRD: create date; ICRP: cropped
// IDIM: dimensions; IDPI: dots per inch; IENG: engineer; IGNR: genre
// IKEY: keyword; ILGT: lightness; IMED: medium; INAM: name;
// IPLT: palette setting; IPRD: product; ISFT: software; ISHP: sharpness
// ISRC: source; ISRF: source form; ITCH: technician
//=======================================================================
void video_encode_end(void *workmem)
{
	//add description to avi file
	//int AviPackerV3_AddInfoStr(const char *fourcc, const char *info_string);
	AviPackerV3_AddInfoStr(workmem, "ISRC", "Generplus");
	AviPackerV3_AddInfoStr(workmem, "IART", "Generplus");
	AviPackerV3_AddInfoStr(workmem, "ICOP", "Generplus");
	AviPackerV3_AddInfoStr(workmem, "ICRD", "2010-06-29");	
}

//--------------------------------------------------------------------
//	user can add special effect in audio or video when use video encode
//  
//---------------------------------------------------------------------
#if AUDIO_SFX_HANDLE
extern INT16S   *pcm_bg_out[];
extern OS_EVENT    *aud_bg_send_q;

INT32U video_encode_audio_sfx(INT16U *PCM_Buf, INT32U cbLen)
{
	INT32S i, index;
	INT32U Temp;
	OS_Q_DATA OSQData; 
	AUDIO_ARGUMENT aud_arg;
	
	aud_arg.Main_Channel = 0;
	    
	if(audio_decode_status(aud_arg) == AUDIO_CODEC_PROCESSING)
	{
		OSQQuery(aud_bg_send_q, &OSQData);
		index = OSQData.OSNMsgs-1;
		if(index<0)
			index = 0;
		
		for(i = 0; i<(cbLen/2) ; i++)
		{
			Temp = (INT16U) pcm_bg_out[index][i];
			Temp +=  PCM_Buf[i];
			PCM_Buf[i] = Temp >> 1;
		}
	}
	
	return (INT32U)PCM_Buf;
}
#endif

#if VIDEO_SFX_HANDLE
INT32U video_encode_video_sfx(INT32U buf_addr, INT32U cbLen)
{
	INT32U i;
	INT8U *pdata = (INT8U *)buf_addr;
	
	for(i=0; i<cbLen ;i++)
	{
		if(i % 4 == 1)
		 *(pdata + i) <<= 1;	
	}

	return (INT32U)pdata;
}
#endif

void tft_vblank_isr_register(void (*user_isr)(void))
{
	tft_vblank_callback = user_isr;
}

#if C_MOTION_DETECTION == CUSTOM_ON
void motion_detect_isr_register(void (*user_isr)(void))
{
	motion_detect_callback = user_isr;
}
#endif

//=======================================================================
//		Following are the user-defined code for display on TV or TFT LCM
//=======================================================================
//=======================================================================
//	tft and sensor isr handle
//=======================================================================
void user_defined_video_codec_isr_csi(void)
{
#if C_MOTION_DETECTION == CUSTOM_ON
	if(R_PPU_IRQ_STATUS & R_PPU_IRQ_EN & 0x80) //motion detection
	{
		R_PPU_IRQ_STATUS = 0x80;
		if (motion_detect_callback != 0)
    	{
			(*motion_detect_callback)();
		}
	}
#endif	
#if VIDEO_ENCODE_USE_MODE == VIDEO_ENCODE_WITH_TG_IRQ || VIDEO_ENCODE_USE_MODE == VIDEO_ENCODE_WITH_FIFO_IRQ 
	{
		INT32U status;
		status = R_TGR_IRQ_STATUS;
		if(status & R_TGR_IRQ_EN) {
#if VIDEO_ENCODE_USE_MODE ==VIDEO_ENCODE_WITH_FIFO_IRQ
			csi_fifo_flag ^= 0x1;
#endif
			if(status & 0x01) { 			// frame end
				if(status & 0x10 ) { 		// fifo under run
					R_TGR_IRQ_STATUS = 0x31;
					// Error handle
					return;
				}
				R_TGR_IRQ_STATUS = 0x21;
#if VIDEO_ENCODE_USE_MODE == VIDEO_ENCODE_WITH_TG_IRQ
				video_encode_auto_switch_csi_frame();
#elif VIDEO_ENCODE_USE_MODE ==VIDEO_ENCODE_WITH_FIFO_IRQ
//		   		video_encode_auto_switch_csi_fifo_end(csi_fifo_flag);
				video_encode_auto_switch_csi_frame_end(csi_fifo_flag);
			}else if(status & 0x20) { 		// fifo end
				R_TGR_IRQ_STATUS = 0x20;
		   		video_encode_auto_switch_csi_fifo_end(csi_fifo_flag);
#endif
			}
		}
	}
#endif
}

void user_defined_video_codec_entrance(void)
{
#if VIDEO_DISPALY_WITH_PPU == 0
	video_codec_display_flag = 0;	
	R_PPU_IRQ_EN = 0;			// disable all ppu interrupt
	R_PPU_IRQ_STATUS = 0x7FFF;	// Clear all PPU interrupts 	
		vic_irq_register(VIC_CSI, user_defined_video_codec_isr_csi);
	/*
	if(DISPLAY_DEVICE == DISPLAY_TFT)	
		R_PPU_IRQ_EN = 0x2000; // TFT Vertical-Blanking IRQ enable register
	else
		R_PPU_IRQ_EN = 0x0800;
	*/
//	vic_irq_enable(VIC_PPU);
#else
	video_codec_ppu_configure();
#endif
}

void video_codec_show_image(INT8U TV_TFT, INT32U BUF,INT32U DISPLAY_MODE ,INT32U SHOW_TYPE)
{
    switch(SHOW_TYPE)
	{      
	   case IMAGE_OUTPUT_FORMAT_RGB565:
	        if(DISPLAY_MODE==QVGA_MODE)
	           R_PPU_ENABLE=(PPU_QVGA_MODE|PPU_FRAME_BASE_MODE|PPU_RGB565_MODE);
	        else if(DISPLAY_MODE == VGA_MODE)
	           R_PPU_ENABLE=(PPU_VGA_MODE|PPU_FRAME_BASE_MODE|PPU_RGB565_MODE);   
	        else if(DISPLAY_MODE == D1_MODE)
	           	R_PPU_ENABLE=(PPU_QVGA_MODE|TFT_SIZE_720x480|PPU_FRAME_BASE_MODE|PPU_RGB565_MODE);
	        else if(DISPLAY_MODE == TFT_320x240_MODE)
	   			R_PPU_ENABLE=(PPU_QVGA_MODE|TFT_SIZE_320X240|PPU_FRAME_BASE_MODE|PPU_RGB565_MODE);
	   		else if(DISPLAY_MODE == TFT_800x480_MODE)
	   			R_PPU_ENABLE=(PPU_QVGA_MODE|TFT_SIZE_800X480|PPU_FRAME_BASE_MODE|PPU_RGB565_MODE);	    
	   		break;
	   
	   case IMAGE_OUTPUT_FORMAT_RGBG:
	        if(DISPLAY_MODE==QVGA_MODE)
	           R_PPU_ENABLE=(PPU_QVGA_MODE|PPU_FRAME_BASE_MODE|PPU_YUYV_RGBG_FORMAT_MODE|PPU_RGBG_MODE|PPU_RGBG_TYPE2);
	        else if(DISPLAY_MODE == VGA_MODE)
	           R_PPU_ENABLE=(PPU_VGA_MODE|PPU_FRAME_BASE_MODE|PPU_YUYV_RGBG_FORMAT_MODE|PPU_RGBG_MODE|PPU_RGBG_TYPE2);
	        else if(DISPLAY_MODE == D1_MODE)
	        	R_PPU_ENABLE=(PPU_QVGA_MODE|TFT_SIZE_720x480|PPU_FRAME_BASE_MODE|PPU_YUYV_RGBG_FORMAT_MODE|PPU_RGBG_MODE|PPU_RGBG_TYPE2);
	        else if(DISPLAY_MODE == TFT_320x240_MODE) 
	        	R_PPU_ENABLE=(PPU_QVGA_MODE|TFT_SIZE_320X240|PPU_FRAME_BASE_MODE|PPU_YUYV_RGBG_FORMAT_MODE|PPU_RGBG_MODE|PPU_RGBG_TYPE2);
	        else if(DISPLAY_MODE == TFT_800x480_MODE)
	        	R_PPU_ENABLE=(PPU_QVGA_MODE|TFT_SIZE_800X480|PPU_FRAME_BASE_MODE|PPU_YUYV_RGBG_FORMAT_MODE|PPU_RGBG_MODE|PPU_RGBG_TYPE2);
	   		break;
	   
	   case IMAGE_OUTPUT_FORMAT_GRGB:
	   		if(DISPLAY_MODE==QVGA_MODE)
	           	R_PPU_ENABLE=(PPU_QVGA_MODE|PPU_FRAME_BASE_MODE|PPU_YUYV_RGBG_FORMAT_MODE|PPU_RGBG_MODE|PPU_RGBG_TYPE3);
	        else if(DISPLAY_MODE == VGA_MODE)
	           	R_PPU_ENABLE=(PPU_VGA_MODE|PPU_FRAME_BASE_MODE|PPU_YUYV_RGBG_FORMAT_MODE|PPU_RGBG_MODE|PPU_RGBG_TYPE3);
	        else if(DISPLAY_MODE == D1_MODE)
	        	R_PPU_ENABLE=(PPU_QVGA_MODE|TFT_SIZE_720x480|PPU_FRAME_BASE_MODE|PPU_YUYV_RGBG_FORMAT_MODE|PPU_RGBG_MODE|PPU_RGBG_TYPE3);
	        else if(DISPLAY_MODE == TFT_320x240_MODE) 
	        	R_PPU_ENABLE=(PPU_QVGA_MODE|TFT_SIZE_320X240|PPU_FRAME_BASE_MODE|PPU_YUYV_RGBG_FORMAT_MODE|PPU_RGBG_MODE|PPU_RGBG_TYPE3);
	        else if(DISPLAY_MODE == TFT_800x480_MODE)
	        	R_PPU_ENABLE=(PPU_QVGA_MODE|TFT_SIZE_800X480|PPU_FRAME_BASE_MODE|PPU_YUYV_RGBG_FORMAT_MODE|PPU_RGBG_MODE|PPU_RGBG_TYPE3);
	   		break;
	  
	   case IMAGE_OUTPUT_FORMAT_UYVY:
	        if(DISPLAY_MODE == QVGA_MODE)
	           R_PPU_ENABLE=(PPU_QVGA_MODE|PPU_FRAME_BASE_MODE|PPU_YUYV_RGBG_FORMAT_MODE|PPU_YUYV_MODE|PPU_YUYV_TYPE2);
	        else if(DISPLAY_MODE == VGA_MODE)
	           R_PPU_ENABLE=(PPU_VGA_MODE|PPU_FRAME_BASE_MODE|PPU_YUYV_RGBG_FORMAT_MODE|PPU_YUYV_MODE|PPU_YUYV_TYPE2);    
	   		else if(DISPLAY_MODE == D1_MODE)
	   			R_PPU_ENABLE=(PPU_QVGA_MODE|TFT_SIZE_720x480|PPU_FRAME_BASE_MODE|PPU_YUYV_RGBG_FORMAT_MODE|PPU_YUYV_MODE|PPU_YUYV_TYPE2);
	   		else if(DISPLAY_MODE == TFT_320x240_MODE)
	   			R_PPU_ENABLE=(PPU_QVGA_MODE|TFT_SIZE_320X240|PPU_FRAME_BASE_MODE|PPU_YUYV_RGBG_FORMAT_MODE|PPU_YUYV_MODE|PPU_YUYV_TYPE2);
	   		else if(DISPLAY_MODE == TFT_800x480_MODE)
	   			R_PPU_ENABLE=(PPU_QVGA_MODE|TFT_SIZE_800X480|PPU_FRAME_BASE_MODE|PPU_YUYV_RGBG_FORMAT_MODE|PPU_YUYV_MODE|PPU_YUYV_TYPE2);	
	   		break;
	
	   case IMAGE_OUTPUT_FORMAT_YUYV:
	        if(DISPLAY_MODE == QVGA_MODE)
	           R_PPU_ENABLE=(PPU_QVGA_MODE|PPU_FRAME_BASE_MODE|PPU_YUYV_RGBG_FORMAT_MODE|PPU_YUYV_MODE|PPU_YUYV_TYPE3);
	        else if(DISPLAY_MODE == VGA_MODE)
	           R_PPU_ENABLE=(PPU_VGA_MODE|PPU_FRAME_BASE_MODE|PPU_YUYV_RGBG_FORMAT_MODE|PPU_YUYV_MODE|PPU_YUYV_TYPE3);    
	   		else if(DISPLAY_MODE == D1_MODE)
	   			R_PPU_ENABLE=(PPU_QVGA_MODE|TFT_SIZE_720x480|PPU_FRAME_BASE_MODE|PPU_YUYV_RGBG_FORMAT_MODE|PPU_YUYV_MODE|PPU_YUYV_TYPE3);
	   		else if(DISPLAY_MODE == TFT_320x240_MODE)
	   			R_PPU_ENABLE=(PPU_QVGA_MODE|TFT_SIZE_320X240|PPU_FRAME_BASE_MODE|PPU_YUYV_RGBG_FORMAT_MODE|PPU_YUYV_MODE|PPU_YUYV_TYPE3);
	   		else if(DISPLAY_MODE == TFT_800x480_MODE)
	   			R_PPU_ENABLE=(PPU_QVGA_MODE|TFT_SIZE_800X480|PPU_FRAME_BASE_MODE|PPU_YUYV_RGBG_FORMAT_MODE|PPU_YUYV_MODE|PPU_YUYV_TYPE3);
	   		break;
	} 

	if (TV_TFT == 0)//TV
		R_TV_FBI_ADDR = BUF;
	else		//TFT
		R_TFT_FBI_ADDR = BUF;
}
/*
static void TPO_SPI_WriteCmd1 (INT32S nCmdType, INT32U uCmd)
{
	INT32S	nBits;
	INT32S	i;

	//	Initial 3-wire GPIO interface, Initial SPI
	gpio_set_port_attribute(C_TFT_DATA, ATTRIBUTE_HIGH);
	gpio_set_port_attribute(C_TFT_CLK, ATTRIBUTE_HIGH);
	gpio_set_port_attribute(C_TFT_CS, ATTRIBUTE_HIGH);
	gpio_init_io(C_TFT_DATA, GPIO_OUTPUT);					 	
	gpio_init_io(C_TFT_CLK, GPIO_OUTPUT);
	gpio_init_io(C_TFT_CS, GPIO_OUTPUT);	
	gpio_write_io (C_TFT_DATA, DATA_LOW);
	gpio_write_io (C_TFT_CLK, DATA_LOW);
	gpio_write_io (C_TFT_CS, DATA_HIGH);
	drv_msec_wait (10);
	
	nBits = (nCmdType + 2) << 3;
	uCmd &= ~((((INT32U)1) << (nBits - 9 * nCmdType)) - 1);
	
	//	Activate CS low to start transaction
	gpio_write_io (C_TFT_CS, DATA_LOW);
	drv_msec_wait (2);
	for (i=0;i<nBits;i++) {
		//	Activate SPI data
		if ((uCmd & 0x80000000) == 0x80000000) {
			gpio_write_io (C_TFT_DATA, DATA_HIGH);
		}
		if ((uCmd & 0x80000000) == DATA_LOW) {
			gpio_write_io (C_TFT_DATA, DATA_LOW);
		}
		drv_msec_wait (2);
		//	Toggle SPI clock
		gpio_write_io (C_TFT_CLK, DATA_HIGH);
		drv_msec_wait (2);
		gpio_write_io (C_TFT_CLK, DATA_LOW);
		drv_msec_wait (2);
		uCmd <<= 1;
	}
	//	Pull low data
	gpio_write_io (C_TFT_DATA, DATA_LOW);
	//	Activate CS high to stop transaction
	gpio_write_io (C_TFT_CS, DATA_HIGH);
	drv_msec_wait (2);
}

void TFT_TD025THED1_Init(void)
{   	
	//	Reset TFT control register at first
    R_TFT_CTRL=0;
	TPO_SPI_WriteCmd1 (0, 0x08000000);
	R_TFT_HS_WIDTH			= 0;				//	1		=HPW
	R_TFT_H_START			= 1+239;			//	240		=HPW+HBP
	R_TFT_H_END				= 1+239+1280;	    //	1520	=HPW+HBP+HDE
	R_TFT_H_PERIOD			= 1+239+1280+40;	//	1560	=HPW+HBP+HDE+HFP
	R_TFT_VS_WIDTH			= 0;				//	1		=VPW				(DCLK)
	R_TFT_V_START			= 21;			    //	21		=VPW+VBP			(LINE)
	R_TFT_V_END				= 21+240;		    //	261		=VPW+VBP+VDE		(LINE)
	R_TFT_V_PERIOD			= 21+240+1;		    //	262		=VPW+VBP+VDE+VFP	(LINE)
	R_TFT_CTRL           	= 0x0317;
	R_TFT_TS_MISC			&= 0xFFF3FF; 	            
	
	R_IOB_DRV = 0;	
}

static INT32U g_sensor_cnt;
void measure_sensor_isr(void)
{
	if(R_PPU_IRQ_STATUS & R_PPU_IRQ_EN & 0x40) //sensor
	{
		R_PPU_IRQ_STATUS = 0x40;
		g_sensor_cnt++;
	}
}

INT32S video_encode_measure_sensor_frame_rate(void)
{
	INT32U sensor_addr;
	OS_CPU_SR cpu_sr;
	
	//set 
	R_PPU_IRQ_EN = 0;			// disable all ppu interrupt
	R_PPU_IRQ_STATUS = 0x7FFF;	// Clear all PPU interrupts 
	
	vic_irq_register(VIC_PPU, measure_sensor_isr);
	vic_irq_enable(VIC_PPU);
	
	g_sensor_cnt = 0; 
	sensor_addr = (INT32U) gp_malloc_align( 40*480*2, 64);
	
	//sensor start	
	OS_ENTER_CRITICAL();
	if(g_display_mode == QVGA_MODE)
		CSI_Init(320, 240, FT_CSI_YUVIN|FT_CSI_YUVOUT|FT_CSI_RGB1555, sensor_addr, NULL);
	else
		CSI_Init(640, 480, FT_CSI_YUVIN|FT_CSI_YUVOUT|FT_CSI_RGB1555, sensor_addr, NULL);
	OS_EXIT_CRITICAL();
	R_PPU_IRQ_EN |= 0x40;	
	
	//OSTimeDly(500);	//delay 5second
	
	//sensor stop
	R_PPU_IRQ_EN &= ~0x40;	
	R_CSI_TG_CTRL0 = 0;
	R_CSI_TG_CTRL1 = 0;
	
	vic_irq_unregister(VIC_PPU);
	vic_irq_disable(VIC_PPU);
	gp_free((void*)sensor_addr);
	
	return g_sensor_cnt/5;
}
*/