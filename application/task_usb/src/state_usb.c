
/* pseudo header include */
#include "state_usb.h"
#include "mini_dvr_api.h"
#include "program_02.h"
#include "program_05.h"

INT8U usbd_flag;
extern INT8U s_usbd_pin;
extern INT32U SendISOData;
extern INT32U SendAudioData;

//void state_usb_entry(void* para1);
static void state_usb_init(void);
//static void state_usb_exit(void);
//static void state_usb_suspend(EXIT_FLAG_ENUM exit_flag);
extern void usb_msdc_state_exit(void);
void sys_usbd_state_flag_set(INT8U in_or_out);


void state_usb_init(void)
{
    sys_usbd_state_flag_set(C_USBD_STATE_IN);
	usb_msdc_state_initial();

}

/* mask by xiangyong
void ap_usb_isr(void)
{
	if (*P_USBD_INT == 0x8000) {
		*P_USBD_INT |= 0x8000;
		vic_irq_disable(VIC_USB);
		OSQPost(USBTaskQ, (void *) MSG_USBD_PLUG_IN);
		//msgQSend(ApQ, MSG_APQ_CONNECT_TO_PC, NULL, NULL, MSG_PRI_NORMAL);
	}
}
*/

/* mask by xiangyong
void ap_usb_pcam_isr(void)
{
	if (*P_USBD_INT == 0x8000) {
		*P_USBD_INT |= 0x8000;
		vic_irq_disable(VIC_USB);
		OSQPost(USBTaskQ, (void *) MSG_USBCAM_PLUG_IN);
		//msgQSend(ApQ, MSG_APQ_CONNECT_TO_PC, NULL, NULL, MSG_PRI_NORMAL);
	}
}
*/

void USB_PHY_I2C_CMD(INT8U addr, INT8U data,INT8U type)	//copy from GP10;
{
        //INT8U rdata;
        volatile INT8U rdata;	//xiangyong, 2011-10-27 18:05

        if (type == 0) 
        { 
        //Setting Want R/W Register[15:8] Addr ,[7:0] data 
                *P_USB20_I2CDR = addr<<8 | data; 
        //Enable I2C Enable        [0]:Enable ,[1]: 1:Write 
                *P_USB20_I2CCR = 0x3; 
        //Check I2C Done 
                rdata = *P_USB20_I2CCR; 
                while(rdata&0x1) rdata = *P_USB20_I2CCR; 

        //Do Read Function to Check Value 
                *P_USB20_I2CCR = 0x1; 
                //Check I2C Done 
                rdata = *P_USB20_I2CCR; 
                while(rdata&0x1) rdata = *P_USB20_I2CCR; 
                rdata = *P_USB20_I2CDR; 
                if((rdata&0xFF) !=data) 
                { 
                        //DBG_PRINT("PHYI2C Check Error\n"); 
                } 
        } 
        else if (type == 1) 
        { 
        //Setting Want R/W Register[15:8] Addr ,[7:0] data 
                *P_USB20_I2CDR = addr<<8 | data; 
        //Enable I2C Enable        [0]:Enable ,[1]: 1:Write 0:Read 
                *P_USB20_I2CCR = 0x1; 
        //Check I2C Done 
                rdata = *P_USB20_I2CCR; 
                rdata = data; 
                data = rdata; 

        //Setting Want R/W Register[15:8] Addr ,[7:0] data 
                *P_USB20_I2CDR = addr<<8 | data; 
        //Enable I2C Enable        [0]:Enable ,[1]: 1:Write 
                *P_USB20_I2CCR = 0x3; 
        //Check I2C Done 
                rdata = *P_USB20_I2CCR; 
                while(rdata&0x1) rdata = *P_USB20_I2CCR; 


        //Do Read Function to Check Value 
                *P_USB20_I2CCR = 0x1; 
                //Check I2C Done 
                rdata = *P_USB20_I2CCR; 
                while(rdata&0x1) rdata = *P_USB20_I2CCR; 
                rdata = *P_USB20_I2CDR; 
                if((rdata&0xFF) !=data) 
                { 
                        //DBG_PRINT("PHYI2C Check Error\n"); 
                } 
        } 
}
//==============================================================================================
//==============================================================================================
//==============================================================================================
void goto_usb_device_mode(void)	//add by xiangyong
{
	INT32U usb_debounce;
	INT32U key_debounce;
	INT32U cnt_bak;
	
	usb_debounce = 0;
	key_debounce = 0;
	cnt_bak = g_public_128hz_cnt;
	
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//2016-12-15 liuxi 
	//有Idle 任务时，部份产品USB不能读盘
	//OSTaskDel(OS_LOWEST_PRIO - 2);
	R_idle_task_disable = 1; 
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	
	s_usbd_pin = 1;
	*P_USBD_CONFIG1 &= ~0x100;//|= 0x100;//[8],SW Suspend For PHY
	USB_PHY_I2C_CMD(0xD,0xA7,0);
	OSTimeDly(20);
   	state_usb_init();
	//DBG_PRINT("USBD state Entry\r\n2");
	while (1)	//USB信息处理
	{
		watchdog_clr();
		usb_isr();
	   	usb_std_service_udisk(0);	
	   	usb_msdc_service(0);
	   	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	   	//可在此检测USB线是否拨出:
	   	//可在此检测按键以切换到PC Camera: 如果有按键就 break:
	   	if (cnt_bak != g_public_128hz_cnt)
	   	{
	   		usb_debounce += g_public_128hz_cnt-cnt_bak;
	   		key_debounce += g_public_128hz_cnt-cnt_bak;
	   		cnt_bak = g_public_128hz_cnt;
	   		//---------------------------------
	   		if (gpio_read_io(C_USBDEVICE_PIN)) usb_debounce = 0;
	   		else
	   		{
		   		//++++++++++++++++++++++++++++ For Bonding Test
				if (g_bonding_test_mode) usb_debounce = 0;
	   			//---------------------------- For Bonding Test
	   			if (usb_debounce > 128/2) break;	//debounce time = 500ms
	   		}
	   		//---------------------------------
	   		#if PC_CAMERA_KEY_CONFIG == USED
	   		#if PC_CAMERA_KEY_TRIGGER_LEVEL == 1
	   		if (gpio_read_io(PC_CAMERA_KEY_PIN))
	   		#else
	   		if (! gpio_read_io(PC_CAMERA_KEY_PIN))
	   		#endif
	   		{
		   		//++++++++++++++++++++++++++++ For Bonding Test
				if (g_bonding_test_mode) break;
	   			//---------------------------- For Bonding Test
	   			if (key_debounce > 128/8)	//debounce time = 125ms
	   			{
	   				g_pc_camera = 1;
	   				break;
	   			}
	   		}
	   		else key_debounce = 0;
	   		#endif
	   	}
	   	//-------------------------------------------------------------
	}
	//state_usb_exit();
	usb_uninitial();
	//DBG_PRINT("USBD state Exit\r\n");
	*P_USBD_CONFIG1 |= 0x100;
	if (!g_bonding_test_mode) OSTimeDly(50);
}
//==============================================================================================
void goto_usb_video_device(void)
{
	INT32U usb_debounce;
	INT32U key_debounce;
	INT32U cnt_bak;
	INT8U  key_status;
	
	usb_debounce = 0;
	key_debounce = 0;
	cnt_bak = g_public_128hz_cnt;
	key_status = 0;

	if (!g_pc_camera) return;
	
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//2016-12-15 liuxi 
	//有Idle 任务时，部份产品USB不能读盘
	//OSTaskDel(OS_LOWEST_PRIO - 2);
	R_idle_task_disable = 1; 
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	s_usbd_pin = 1;
	*P_USBD_CONFIG1 &= ~0x100;//|= 0x100;//[8],SW Suspend For PHY
	USB_PHY_I2C_CMD(0xD,0xA7,0);
	OSTimeDly(20);
	usb_cam_entrance(0);
	//OSTimeDly(100);
	usb_webcam_start();
	//DBG_PRINT("P-CAM Entry\r\n");
	while (1)
	{
		watchdog_clr();
		usb_std_service(0);
	   	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	   	//可在此检测USB线是否拨出:
	   	//可在此检测按键以切换到PC Camera + OSD:
	   	if (cnt_bak != g_public_128hz_cnt)
	   	{
	   		usb_debounce += g_public_128hz_cnt-cnt_bak;
	   		key_debounce += g_public_128hz_cnt-cnt_bak;
	   		cnt_bak = g_public_128hz_cnt;
	   		//---------------------------------
	   		if (gpio_read_io(C_USBDEVICE_PIN)) usb_debounce = 0;
	   		else
	   		{
		   		//++++++++++++++++++++++++++++ For Bonding Test
				if (g_bonding_test_mode) usb_debounce = 0;
	   			//---------------------------- For Bonding Test
		   		if (usb_debounce > 128/2) break;	//debounce time = 500ms
	   		}
	   		//---------------------------------
	   		#if PC_CAMERA_KEY_CONFIG == USED
	   		#if PC_CAMERA_KEY_TRIGGER_LEVEL == 1
	   		if (gpio_read_io(PC_CAMERA_KEY_PIN))
	   		#else
	   		if (! gpio_read_io(PC_CAMERA_KEY_PIN))
	   		#endif
	   		{
		   		//++++++++++++++++++++++++++++ For Bonding Test
				if (g_bonding_test_mode) break;
   				//---------------------------- For Bonding Test
	   			if (key_status) key_debounce = 0;
	   			if (key_debounce > 128/8)	//debounce time = 125ms
	   			{
	   				key_status = 1;
	   				if (g_pc_camera == 1) g_pc_camera = 2;
	   				else g_pc_camera = 1;
	   			}
	   		}
	   		else
	   		{
	   			key_debounce = 0;
	   			key_status = 0;
	   		}
	   		#endif
	   	}
	   	//-------------------------------------------------------------
	}
	//DBG_PRINT("P-CAM Exit\r\n");
	usb_webcam_stop();
//	OSTimeDly(30);
	usb_cam_exit();
	vic_irq_disable(VIC_USB);
	usb_uninitial();
	*P_USBD_CONFIG1 |= 0x100;
//	OSTimeDly(20);
//	R_SYSTEM_CLK_EN1 = 0x4C98;
}
//==============================================================================================
//==============================================================================================
//==============================================================================================
#define STG_CFG_CF      0x0001
#define STG_CFG_SD      0x0002
#define STG_CFG_MS      0x0004
#define STG_CFG_USBH    0x0008
#define STG_CFG_USBD    0x0010
#define STG_CFG_NAND1   0x0020
#define STG_CFG_XD      0x0040
#if 0
void state_usb_exit(void)
{
   INT16U stg_init_val=0;    

   //DBG_PRINT("USBD state Exit\r\n");
   sys_usbd_state_flag_set(C_USBD_STATE_OUT);
   /* allow umi to access statck state */

   
   //reinitial cards and NF
#if NAND1_EN == 1
 	if (storage_detection(5)) DrvNand_flush_allblk();
#endif

  if(NAND1_EN == 1) {stg_init_val |= STG_CFG_NAND1;}
  if(SD_EN == 1) {stg_init_val |= STG_CFG_SD;}
  if(MSC_EN == 1) {stg_init_val |= STG_CFG_MS;}
  if(CFC_EN == 1) {stg_init_val |= STG_CFG_CF;}
  if(XD_EN == 1) {stg_init_val |= STG_CFG_XD;}
  // storages_init(0x27);
   storages_init(stg_init_val);


   usb_msdc_state_exit();   
}
#endif
/*
void state_usb_suspend(EXIT_FLAG_ENUM exit_flag)
{
    DBG_PRINT("USBD state Suspend\r\n");
	sys_usbd_state_flag_set(C_USBD_STATE_OUT);
	
}
*/
void sys_usbd_state_flag_set(INT8U in_or_out)
{
	usbd_flag = in_or_out;
}

INT8U sys_usbd_state_flag_get(void)
{
	return usbd_flag;
}
