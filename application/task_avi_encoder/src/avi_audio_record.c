#include "avi_audio_record.h"
#include "lpf.h"
#include "mini_dvr_api.h"

#define C_AVI_AUDIO_RECORD_STACK_SIZE	512
#define C_AVI_AUD_ACCEPT_MAX			5

#define USB_AUDIO_PCM_SAMPLES 2400	//2012-3-12 10:31
/*os task stack */
INT32U	AviAudioRecordStack[C_AVI_AUDIO_RECORD_STACK_SIZE];

/* os task queue */
OS_EVENT *avi_aud_q;
OS_EVENT *avi_aud_ack_m;
void *avi_aud_q_stack[C_AVI_AUD_ACCEPT_MAX];

/* static function */ 
static INT32S avi_wave_encode_start(void);
static INT32S avi_wave_encode_stop(void);
static INT32S avi_wave_encode_once(INT16S *pcm_input_addr);

/////////////////////////////////////////////////////////////////////////////////////////////////////////
INT32S avi_adc_record_task_create(INT8U pori)
{	
	INT8U  err;
	INT32S nRet;
	
	avi_aud_q = OSQCreate(avi_aud_q_stack, C_AVI_AUD_ACCEPT_MAX);
	if(!avi_aud_q) RETURN(STATUS_FAIL);
	
	avi_aud_ack_m = OSMboxCreate(NULL);
	if(!avi_aud_ack_m) RETURN(STATUS_FAIL);
	
	err = OSTaskCreate(avi_audio_record_entry, NULL, (void *)&AviAudioRecordStack[C_AVI_AUDIO_RECORD_STACK_SIZE - 1], pori);
	if(err != OS_NO_ERR) RETURN(STATUS_FAIL);
	
	nRet = STATUS_OK;
Return:
    return nRet;
}

INT32S avi_adc_record_task_del(void)
{
	INT8U  err;
	INT32U nRet, msg;
	
	nRet = STATUS_OK;
	POST_MESSAGE(avi_aud_q, AVI_AUDIO_RECORD_EXIT, avi_aud_ack_m, 5000, msg, err);
Return:	
	OSQFlush(avi_aud_q);
   	OSQDel(avi_aud_q, OS_DEL_ALWAYS, &err);
	OSMboxDel(avi_aud_ack_m, OS_DEL_ALWAYS, &err);
	return nRet;
}

INT32S avi_audio_record_start(void)
{
	INT8U  err;
	INT32S nRet, msg;
	
	nRet = STATUS_OK;
	POST_MESSAGE(avi_aud_q, AVI_AUDIO_RECORD_START, avi_aud_ack_m, 5000, msg, err);
Return:		
	return nRet;	
}

INT32S avi_audio_record_restart(void)
{
	INT8U  err;
	INT32S nRet, msg;
	
	nRet = STATUS_OK;
	POST_MESSAGE(avi_aud_q, AVI_AUDIO_RECORD_RESTART, avi_aud_ack_m, 5000, msg, err);
Return:
	return nRet;
}

INT32S avi_audio_record_stop(void)
{
	INT8U  err;
	INT32S nRet, msg;
	
	nRet = STATUS_OK;
	POST_MESSAGE(avi_aud_q, AVI_AUDIO_RECORD_STOPING, avi_aud_ack_m, 5000, msg, err);
Return:	
	if(nRet < 0)
	{
	#if MIC_INPUT_SRC == C_ADC_LINE_IN
		adc_timer_stop(AVI_AUDIO_RECORD_TIMER);
		avi_adc_double_buffer_free();
	#elif MIC_INPUT_SRC == C_GPY0050_IN		
		timer_stop(AVI_AUDIO_RECORD_TIMER);
		gpy0050_disable();
	#endif					
		avi_audio_memory_free();
	}
	return nRet;	
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
void avi_audio_record_entry(void *parm)
{
	INT8U   err, bStop, audio_flag;
	INT32S  nRet, audio_stream, encode_size, ret;
	INT32U  msg_id, pcm_addr, pcm_cwlen;
//#if MIC_INPUT_SRC == C_ADC_LINE_IN || MIC_INPUT_SRC == C_BUILDIN_MIC_IN
//	INT16U  mask;
	INT32U  ready_addr, next_addr;
//#endif
	OS_CPU_SR cpu_sr;
#if C_USB_AUDIO == CUSTOM_ON
	ISOTaskMsg audioios;
#endif

#if ((C_LPF_ENABLE==1) || (AUDIO_RECORD_DIGITAL_GAIN!=DIGITAL_GAIN_1X))
	INT32U i;
	INT16U *ptr;
#endif

#if (C_LPF_ENABLE==1)
	INT16U t;
	INT32U coef_freq;
	INT16U coef_loop;	 
#endif

	while(1) 
	{
		msg_id = (INT32U) OSQPend(avi_aud_q, 0, &err);
		if(err != OS_NO_ERR)
			continue;

		switch(msg_id)
		{
			case AVI_AUDIO_DMA_DONE:
			#if MIC_INPUT_SRC == C_ADC_LINE_IN || MIC_INPUT_SRC == C_BUILDIN_MIC_IN
				pcm_addr = ready_addr;
				ready_addr = next_addr;		
				next_addr = avi_audio_get_next_buffer(); 		
				if(bStop)
				{
					//if(avi_adc_dma_status_get() != 1)
					OSQPost(avi_aud_q, (void *)AVI_AUDIO_RECORD_STOP);  // check dma is done and stop
				}
				else
				{
					avi_adc_double_buffer_set((INT16U*)next_addr, pcm_cwlen);// set dma buffer
				}
			
				// unsigned to signed
			#if AUDIO_RECORD_DIGITAL_GAIN == DIGITAL_GAIN_2X
				cache_invalid_range((INT32U)pcm_addr, pcm_cwlen << 1);
				ptr = (INT16U *)pcm_addr;
				for(i=0; i<pcm_cwlen; i++)
				{
					INT16U t,t2;

					t = t2 = *ptr;
					if (t & 0x8000) {
					    t2 = ~t2+1;
					}
					if (t2 >= 0x3FFF) {
					    t2 = 0x7FF0;
					} else {
						t2 = t2<<1;
					}
					
					if (t & 0x8000) {
					    *ptr++ = ~t2 + 1;
					} else {
					    *ptr++ = t2;
					}
				}
			#elif AUDIO_RECORD_DIGITAL_GAIN == DIGITAL_GAIN_4X
				cache_invalid_range((INT32U)pcm_addr, pcm_cwlen << 1);
				ptr = (INT16U *)pcm_addr;
				for(i=0; i<pcm_cwlen; i++)
				{
					INT16U t,t2;

					t = t2 = *ptr;
					if (t & 0x8000) {
					    t2 = ~t2+1;
					}
					if (t2 >= 0x1FFF) {
					    t2 = 0x7FF0;
					} else {
						t2 = t2<<2;
					}
					
					if (t & 0x8000) {
					    *ptr++ = ~t2 + 1;
					} else {
					    *ptr++ = t2;
					}
				}
			#elif AUDIO_RECORD_DIGITAL_GAIN == DIGITAL_GAIN_6X
				cache_invalid_range((INT32U)pcm_addr, pcm_cwlen << 1);
				ptr = (INT16U *)pcm_addr;
				for(i=0; i<pcm_cwlen; i++)
				{
					INT16U t,t2,t3;

					t = t2 = *ptr;
					if (t & 0x8000) {
					    t2 = ~t2+1;
					}
					if (t2 >= 0x1555) {
					    t2 = 0x7FF0;
					} else {
						t3 = t2<<1;
						t2 = t2<<2;
						t2 += t3;
					}
					
					if (t & 0x8000) {
					    *ptr++ = ~t2 + 1;
					} else {
					    *ptr++ = t2;
					}
				}
			#elif AUDIO_RECORD_DIGITAL_GAIN == DIGITAL_GAIN_8X
				cache_invalid_range((INT32U)pcm_addr, pcm_cwlen << 1);
				ptr = (INT16U *)pcm_addr;
				for(i=0; i<pcm_cwlen; i++)
				{
					INT16U t,t2;

					t = t2 = *ptr;
					if (t & 0x8000) {
					    t2 = ~t2+1;
					}
					if (t2 >= 0xFFF) {
					    t2 = 0x7FF0;
					} else {
						t2 = t2<<3;
					}
					
					if (t & 0x8000) {
					    *ptr++ = ~t2 + 1;
					} else {
					    *ptr++ = t2;
					}
				}
			#endif
			
			#if C_USB_AUDIO == CUSTOM_ON
				if((avi_encode_get_status()&C_AVI_ENCODE_USB_WEBCAM))
				{
					audioios.FrameSize = USB_AUDIO_PCM_SAMPLES*2;
					audioios.AddrFrame = pcm_addr;
					OSQPost(USBAudioApQ, (void*)(&audioios));
					break;
				}
			#endif
				// unsigned to signed
#if (C_LPF_ENABLE ==1)
				cache_invalid_range((INT32U)pcm_addr, pcm_cwlen << 1);
				ptr = (INT16U *)pcm_addr;
				for(i=0; i<pcm_cwlen; i++)
				{
					t = *ptr;
//					t^=mask;		////for MIC_BITTRANS
//#if (C_LPF_ENABLE ==1)							
					//(INT16S)t = LPF_process((INT16S)t);	
					t = LPF_process((INT16S)t);	
//#endif
					*ptr++=t;
				}
#endif
			#elif MIC_INPUT_SRC == C_GPY0050_IN
				if(g_mic_buffer)
					pcm_addr = pAviEncAudPara->pcm_input_addr[0];
				else
					pcm_addr = pAviEncAudPara->pcm_input_addr[1];

#if (C_LPF_ENABLE ==1)				
				cache_invalid_range((INT32U)pcm_addr, pcm_cwlen << 1);	
				ptr = (INT16U *)pcm_addr;
				for(i=0; i<pcm_cwlen; i++)
				{
					t = *ptr;
					(INT16S)t = LPF_process((INT16S)t);
					/*if (t >= 0xAAAA) {
						t = 0xFFF0;
					} else {
						t += (t>>1);
					}*/
					*ptr++=t;
				}
#endif
				if(bStop)
					OSQPost(avi_aud_q, (void *)AVI_AUDIO_RECORD_STOP);
			#endif			
			#if AUDIO_SFX_HANDLE
				pcm_addr = (INT16U *)video_encode_audio_sfx((INT16U *)pcm_addr, pcm_cwlen<<1);
			#endif	
				encode_size = avi_wave_encode_once((INT16S*)pcm_addr);
				audio_stream = encode_size + 8 + 2*16;
				
				// when restart, wait pcm frame end
				if(audio_flag == 1) 
				{
					audio_flag = 0;
					OSMboxPost(avi_aud_ack_m, (void*)C_ACK_SUCCESS);
				}
				
				if((avi_encode_get_status()&C_AVI_ENCODE_START) == 0) break;
				
				ret = avi_encode_disk_size_is_enough(audio_stream);
				if (ret == 0) {
					avi_enc_storage_full();
					continue;
				} else if (ret == 2) {
					g_file_size_over = 1;
					//msgQSend(ApQ, MSG_APQ_RECORD_SWITCH_FILE, NULL, NULL, MSG_PRI_NORMAL);
					//上一行消息发送之后: 录像task会停止当前录像文件, 然后再新建一个录像文件后再录像;
				}
				
				if(encode_size > 0)
				{
					nRet = pfn_avi_encode_put_data(	pAviEncPara->AviPackerCur->avi_workmem, 
													*(long*)"01wb", 
													encode_size, 
													(INT16U*)pAviEncAudPara->pack_buffer_addr, 
													encode_size/pAviEncPara->AviPackerCur->p_avi_wave_info->nBlockAlign, 
													AVIIF_KEYFRAME);
					if(nRet >= 0)
					{	
						OS_ENTER_CRITICAL();
						pAviEncPara->ta += pAviEncPara->delta_ta;
						OS_EXIT_CRITICAL();
						DEBUG_MSG(DBG_PRINT("A"));
					}
					else
					{
						avi_encode_disk_size_is_enough(-audio_stream);
						DEBUG_MSG(DBG_PRINT("AudPutData = %x, size = %d!!!\r\n", nRet-0x80000000, pcm_cwlen<<1));	
					}
				}
				break;
			
			case AVI_AUDIO_RECORD_START:
				bStop = audio_flag = 0;
			  #if C_USB_AUDIO == CUSTOM_ON
				if((avi_encode_get_status()&C_AVI_ENCODE_USB_WEBCAM))
				{
					pcm_cwlen = USB_AUDIO_PCM_SAMPLES;
				}
				else 
			  #endif
				{
					nRet = avi_wave_encode_start();
					if(nRet < 0) goto AUDIO_RECORD_START_FAIL;
					pcm_cwlen = pAviEncAudPara->pcm_input_size * C_WAVE_ENCODE_TIMES;
					encode_size = pAviEncAudPara->pack_size * C_WAVE_ENCODE_TIMES;
				}
				
				nRet = avi_audio_memory_allocate(pcm_cwlen<<1);
				if(nRet < 0) goto AUDIO_RECORD_START_FAIL;
			#if MIC_INPUT_SRC == C_ADC_LINE_IN || MIC_INPUT_SRC == C_BUILDIN_MIC_IN
//				mask = 0x8000;		////for MIC_BITTRANS
				ready_addr = avi_audio_get_next_buffer();
				next_addr = avi_audio_get_next_buffer();
				nRet = avi_adc_double_buffer_put((INT16U*)ready_addr, pcm_cwlen, avi_aud_q);
				if(nRet < 0) goto AUDIO_RECORD_START_FAIL;
				nRet = avi_adc_double_buffer_set((INT16U*)next_addr, pcm_cwlen);
				if(nRet < 0) goto AUDIO_RECORD_START_FAIL;
				avi_adc_hw_start();
/*
#if (C_LPF_ENABLE ==1)
				coef_freq = Cut_Off_Freq;
				coef_loop = Filter_Order;			
				//LPF_init(pAudio_Encode_Para->SampleRate,3);
				LPF_init(coef_freq,coef_loop);
#endif		
*/
			#elif MIC_INPUT_SRC == C_GPY0050_IN
				g_mic_buffer = 0;
				g_mic_cnt = 0;
				gpy0050_enable();
			#endif
			  #if C_USB_AUDIO == CUSTOM_ON
				if((avi_encode_get_status()&C_AVI_ENCODE_USB_WEBCAM))
				{
					OSMboxPost(avi_aud_ack_m, (void*)C_ACK_SUCCESS);
					break;
				}
			  #endif
#if (C_LPF_ENABLE ==1)
				coef_freq = Cut_Off_Freq;
				coef_loop = Filter_Order;			
				//LPF_init(pAudio_Encode_Para->SampleRate,3);
				LPF_init(coef_freq,coef_loop);
#endif		
				pAviEncPara->delta_ta = (INT64S)pAviEncVidPara->dwRate * pcm_cwlen;
				OSMboxPost(avi_aud_ack_m, (void*)C_ACK_SUCCESS);
				break;
AUDIO_RECORD_START_FAIL:
			#if MIC_INPUT_SRC == C_ADC_LINE_IN || MIC_INPUT_SRC == C_BUILDIN_MIC_IN
				avi_adc_hw_stop();
				avi_adc_double_buffer_free();
				avi_audio_memory_free();
				avi_wave_encode_stop();
			#endif
				DBG_PRINT("AudEncStartFail!!!\r\n");
				OSMboxPost(avi_aud_ack_m, (void*)C_ACK_FAIL);
				break;	
				
			case AVI_AUDIO_RECORD_STOP:
			#if MIC_INPUT_SRC == C_ADC_LINE_IN || MIC_INPUT_SRC == C_BUILDIN_MIC_IN
				avi_adc_hw_stop();
				avi_adc_double_buffer_free();
			#elif MIC_INPUT_SRC == C_GPY0050_IN
				avi_encode_audio_timer_stop();
				gpy0050_disable();
			#endif
				avi_audio_memory_free();
				avi_wave_encode_stop();
				OSMboxPost(avi_aud_ack_m, (void*)C_ACK_SUCCESS);
				break;
				
			case AVI_AUDIO_RECORD_STOPING:
				bStop = 1;
				break;
		
			case AVI_AUDIO_RECORD_RESTART:
				audio_flag = 1;
				break;
			
			case AVI_AUDIO_RECORD_EXIT:
				OSQFlush(avi_aud_q);
				OSMboxPost(avi_aud_ack_m, (void*)C_ACK_SUCCESS);
				OSTaskDel(OS_PRIO_SELF);
				break;
		}	
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
static INT32S avi_wave_encode_start(void)
{
	INT32S nRet, size;

	size = wav_enc_get_mem_block_size();
	pAviEncAudPara->work_mem = (INT8U *)gp_malloc(size);
	if(!pAviEncAudPara->work_mem) RETURN(STATUS_FAIL);
	gp_memset((INT8S*)pAviEncAudPara->work_mem, 0, size);
	nRet = wav_enc_Set_Parameter( pAviEncAudPara->work_mem, 
								  pAviEncAudPara->channel_no, 
								  pAviEncAudPara->audio_sample_rate, 
								  pAviEncAudPara->audio_format);
	if(nRet < 0) RETURN(STATUS_FAIL);
	nRet = wav_enc_init(pAviEncAudPara->work_mem);
	if(nRet < 0) RETURN(STATUS_FAIL);
	pAviEncAudPara->pcm_input_size = wav_enc_get_SamplePerFrame(pAviEncAudPara->work_mem);
	
	switch(pAviEncAudPara->audio_format)
	{
	case WAVE_FORMAT_PCM:
		pAviEncAudPara->pack_size = pAviEncAudPara->pcm_input_size;	
		pAviEncAudPara->pack_size *= 2;
		break;
	
	case WAVE_FORMAT_ALAW:
	case WAVE_FORMAT_MULAW:	
	case WAVE_FORMAT_ADPCM:
	case WAVE_FORMAT_IMA_ADPCM:
		pAviEncAudPara->pack_size = wav_enc_get_BytePerPackage(pAviEncAudPara->work_mem);	
		break;
	}
	
	nRet = STATUS_OK;
Return:	
	return nRet;	
}

static INT32S avi_wave_encode_stop(void)
{
	gp_free((void*)pAviEncAudPara->work_mem);
	pAviEncAudPara->work_mem = 0;
	return STATUS_OK;
}

static INT32S avi_wave_encode_once(INT16S *pcm_input_addr)
{
	INT8U *encode_output_addr;
	INT32S nRet, encode_size, N;
	
	encode_size = 0;
	N = C_WAVE_ENCODE_TIMES;
	encode_output_addr = (INT8U*)pAviEncAudPara->pack_buffer_addr;
	while(N--)
	{
		nRet = wav_enc_run(pAviEncAudPara->work_mem, (short *)pcm_input_addr, encode_output_addr);
		if(nRet < 0)		
			return  STATUS_FAIL;
		
		encode_size += nRet;
		pcm_input_addr += pAviEncAudPara->pcm_input_size;
		encode_output_addr += pAviEncAudPara->pack_size;
	}
	
	return encode_size;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////
