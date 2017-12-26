/*Pseudo Header*/
#define MSG_AUDIO_BASE              0x10000100
#define MSG_IMAGE_TASK_BASE         0x10000200
#define MSG_PPU_BASE             	0x10000300
#define MSG_SYSTEM_BASE             0x10000500
#define MSG_FILESRV_BASE            0x10000600
#define MSG_AUDIO_DAC_BASE          0x10000700
#define MSG_USER_TIMER_BASE			0x10000800
#define MSG_RETURN_STATUS_BASE		0x10000900
#define MSG_JPEG_HANDLE_BASE        0x20000300
#define MSG_ICONSHOW_BASE           0x20000100
#define MSG_AVISHOW_BASE			0x20000400
#define MSG_STARTUP_BASE			0x20000500
#define MSG_AVIENC_BASE				0x20000600

typedef enum
{
	MSG_AVI_TICKER = MSG_AVISHOW_BASE,
	MSG_AVI_AUDIO_END,
	MSG_AVI_MJPEG_END,
	MSG_AVI_DECODE_END,
	MSG_AVI_PARSE_HEADER,
	MSG_AVI_STATUS_CHECK,
	MSG_AVI_PLAY,
	MSG_AVI_STOP,
	MSG_AVI_PAUSE,
	MSG_AVI_RESUME,
	MSG_AVI_ERROR,
	MSG_AVI_PLUGOUT,
	MSG_AVI_DISPLAYSIZE_CHANGE,
	MSG_AVI_MAX,
	MSG_START_AVI_STOP,
	MSG_AVI_VOLUME_DOWN,
	MSG_AVI_VOLUME_UP

}MSG_AVI_ENUM;

typedef enum
{
	MSG_AVI_ENCODE_PACKER_START = MSG_AVIENC_BASE,
	MSG_AVI_ENCODE_PACKER_STOP,
	MSG_AVI_ENCODE_START_AUDIO_RECORD,
	MSG_AVI_ENCODE_STOP_AUDIO_RECORD,
	MSG_AVI_ENCODE_START_VIDEO_ENCODE,
	MSG_AVI_ENCODE_STOP_VIDEO_ENCODE,
	MSG_AVI_ENCODE_START_CSI,
	MSG_AVI_ENCODE_STOP_CSI,	
	MSG_AVI_ENCODE_START_SCALER,
	MSG_AVI_ENCODE_STOP_SCALER,
	MSG_AVI_ENCODE_START,
	MSG_AVI_ENCODE_STOP,
	MSG_AVI_ENCODE_PAUSE,
	MSG_AVI_ENCODE_RESUME,
	MSG_AVI_ENCODE_CAPTURE_PICTURE, 
	MSG_AVI_ENCODE_STORAGE_FULL,
	MSG_AVI_ENCODE_STORAGE_ERR,
	MSG_AVI_ENCODE_EXIT 
}MSG_AVIENC_ENUM;

typedef enum
{
    MSG_AUD_PLAY=MSG_AUDIO_BASE,
    MSG_AUD_PLAY_BY_SPI,
    MSG_AUD_MIDI_PLAY,
    MSG_AUD_PAUSE,
    MSG_AUD_STOP,
    MSG_AUD_RESUME,
    MSG_AUD_AVI_PLAY,
    MSG_AUD_SET_MUTE,
    MSG_AUD_VOLUME_SET,
    MSG_AUD_DECODE_NEXT_FRAME,
    MSG_AUD_SPU_RESTART,
    MSG_AUD_PLAY_RES,
    MSG_AUD_PLAY_SPI_RES,
    MSG_AUD_STOP_RES,
    MSG_AUD_PAUSE_RES,
    MSG_AUD_RESUME_RES,
    MSG_AUD_MUTE_SET_RES,
    MSG_AUD_VOLUME_SET_RES,
    MSG_AUD_BG_PLAY_RES,
    MSG_AUD_BG_PLAY_SPI_RES,
    MSG_AUD_BG_STOP_RES,
    MSG_AUD_BG_PAUSE_RES,
    MSG_AUD_BG_RESUME_RES,
    MSG_AUD_MAX
} MSG_AUD_ENUM;

typedef enum
{
    MSG_ICON_DECODE=MSG_ICONSHOW_BASE,
    MSG_ICON_DECODE_ERR,
    MSG_ICON_AVI_DECODE,
    MSG_ICON_INSERT,
    MSG_ICON_CFG,
    MSG_ICON_HANDOVER,
    MSG_ICON_FLY_FILL,
    MSG_ICON_REMOVE,
    MSG_ICON_FS_WAIT,
    MSG_ICON_RING_BUF_UPDATE,
    MSG_ICON_RING_BUF_REG_TO_SPRITE_IMG,
    MSG_ICON_SLIDE_WIN_CFG,
    MSG_ICON_RUNTIME_DISPLAY,
    MSG_ICON_ERROR_SHOW,
    MSG_ICON_KEY_SELECT_MOTION,
    MSG_ICON_IMAGE_FS_SCAN,
    MSG_ICON_SW_TIMER_ISR,
    MSG_ICON_COVER_FLOW_STATE,
    MSG_ICON_MAX
} MSG_ICONSHOW_ENUM;

typedef enum
{
    MSG_MJPEG_DECODE_OK=MSG_JPEG_HANDLE_BASE,
    MSG_MJPEG_DECODE_FAIL,
    MSG_MJPEG_DECODE_STATUS,
    MSG_JPEG_DECODE_MAX
} MSG_MJPEG_DECODE_ENUM;

typedef enum
{
	// Request
    MSG_IMAGE_PARSE_HEADER_REQ = MSG_IMAGE_TASK_BASE,
    MSG_IMAGE_DECODE_REQ,
    MSG_IMAGE_DECODE_EXT_REQ,
    MSG_IMAGE_SCALE_REQ,
    MSG_MJPEG_PARSE_HEADER_REQ,
    MSG_MJPEG_DECODE,
    MSG_MJPEG_DECODE_1,
    MSG_IMAGE_STOP_REQ,
    MSG_IMAGE_REQUEST_MAX,
	// Reply
	MSG_IMAGE_PARSE_HEADER_REPLY,
	MSG_IMAGE_DECODE_REPLY,
	MSG_IMAGE_DECODE_EXT_REPLY,
	MSG_IMAGE_SCALE_REPLY,
	MSG_MJPEG_PARSE_HEADER_REPLY,
    MSG_IMAGE_STOP_REPLY,
	MSG_IMAGE_REPLY_MAX
} MSG_IMAGE_TASK_ENUM;

typedef enum
{
    MSG_SYS_AD_KEY=MSG_SYSTEM_BASE,
    MSG_SYS_UART_KEY,
    MSG_SYS_IR_KEY,
    MSG_SYS_ITOUCH_KEY,
    MSG_SYS_KEY_POWER,
    MSG_SYS_LOW_BATTERY,
    MSG_SYS_RTC,
    MSG_SYS_ALARM,
    MSG_SYS_HALT,
    MSG_SYS_CARD_PLUG_CHANGE,
    MSG_SYS_QUERY_CARD_EXIST,
    MSG_SYS_STORAGE_SCAN_START,
    MSG_SYS_STORAGE_SCAN_DONE,
    MSG_SYS_ERROR,
    MSG_SYS_USBD_PLUG_CHANGE,
    MSG_SYS_NAUGHTY_EVENT,
    MSG_SYS_USBD,
    MSG_SYS_USBH,
    MSG_SYS_ADAPTER_PLUG,
    MSG_SYS_BATTERY_CHECK,
    MSG_SYS_MAX
} MSG_SYS_ENUM;


typedef enum
{
    MSG_FILESRV_FS_READ=MSG_FILESRV_BASE,
    MSG_FILESRV_NVRAM_READ,
    MSG_FILESRV_NVRAM_AUDIO_READ,	// added by Bruce, 2009/02/19
    MSG_FILESRV_SPI_L1_READ,
    MSG_FILESRV_SPI_L3_READ,
    MSG_FILESRV_FLUSH,
    MSG_FILESRV_FLUSH_DONE,
    MSG_FILESRV_SCAN,
    MSG_FILESRV_SCAN_CONTINUE,
    MSG_FILESRV_MOUNT,
    MSG_FILESRV_SORTING,
    MSG_FILESRV_DEL,     /*RFU*/
    MSG_FILESRV_COPY,    /*RFU*/
    MSG_FILESRV_MOVE,    /*RFU*/
    MSG_FILESRV_RENAME,  /*RFU*/
    MSG_FILESRV_OPEN,    /*RFU*/
    MSG_FILESRV_CLOSE,   /*RFU*/
    MSG_FILESRV_AVIAUDIO_FS_READ,  /*avi use only */
    MSG_FILESRV_AVIVIDEO_FS_READ,  /*avi use only */
    MSG_FILESRV_USER_DEFINE_READ,	// added by Bruce, 2008/10/27
    MSG_FILESRV_MAX   /*RFU*/
} MSG_FILESRV_ENUM;

typedef enum
{
    MSG_AUD_DMA_DBF_START=MSG_AUDIO_DAC_BASE,
    MSG_AUD_DMA_DBF_DONE,
    MSG_AUD_DMA_DBF_RESTART,
    MSG_AUD_DMA_WIDX_CLEAR,
    MSG_AUD_STOP_SMOOTH_START,
    MSG_AUD_RAMP_DOWN_START,
    MSG_AUD_SPU_WIDX_CLEAR,
    MSG_AUD_SPU_SOFTCH_START,
    MSG_AUD_SPU_LEFT_DONE,
    MSG_AUD_SPU_RIGHT_DONE,
    MSG_AUD_DMA_PAUSE,
    MSG_AUD_DAC_MAX
} MSG_AUD_DAC_ENUM;

typedef enum
{
    MSG_USER_TIMER0=MSG_USER_TIMER_BASE,
    MSG_USER_TIMER1,
    MSG_USER_TIMER2,
    MSG_USER_TIMER3,
    MSG_USER_TIMER4,
    MSG_USER_TIMER_MAX
} MSG_USER_TIMER_ENUM;

typedef enum
{
	MSG_SLIDE_SHOW_NEXT_TICK = MSG_RETURN_STATUS_BASE,
	MSG_RETURN_STATUS_MAX
} MSG_RETURN_STATUS_ENUM;

//avi encode
typedef enum
{
	MSG_AVI_START_SENSOR = 0x1000,
	MSG_AVI_STOP_SENSOR,
	MSG_AVI_START_ENCODE,
	MSG_AVI_STOP_ENCODE,
	MSG_AVI_RESUME_ENCODE,
	MSG_AVI_PAUSE_ENCODE,
	MSG_AVI_CAPTURE_PICTURE,
	MSG_AVI_STORAGE_FULL,
	MSG_AVI_STORAGE_ERR,
	MSG_AVI_ENCODE_STATE_EXIT,
	MSG_AVI_ONE_FRAME_ENCODE,
	MSG_AVI_MOTION_DETECTION_INIT,
	MSG_AVI_MOTION_DETECTION_EXIT,
	MSG_AVI_START_MOTION_DETECTION,
	MSG_AVI_STOP_MOTION_DETECTION,
	MSG_AVI_CHECK_MOTION_DETECTION,
	MSG_AVI_START_USB_WEBCAM,
	MSG_AVI_STOP_USB_WEBCAM 
}AVI_ENCODE_STATE_ENUM;

//scaler message
typedef enum
{
	MSG_SCALER_TASK_INIT = 0x1000000,
	MSG_SCALER_TASK_PREVIEW_ON = 0x2000000,
	MSG_SCALER_TASK_PREVIEW_OFF = 0x3000000,
	MSG_SCALER_TASK_PREVIEW_LOCK = 0x4000000,
	MSG_SCALER_TASK_PREVIEW_UNLOCK = 0x5000000,
	MSG_SCALER_TASK_STOP = 0x6000000,
	MSG_SCALER_TASK_EXIT = 0x7000000
}AVI_ENCODE_SCALER_ENUM;

//video encode message
typedef enum
{
	MSG_VIDEO_ENCODE_TASK_MJPEG_INIT = 0x3000,
	MSG_VIDEO_ENCODE_TASK_MPEG4_INIT,
	MSG_VIDEO_ENCODE_TASK_STOP,
	MSG_VIDEO_ENCODE_TASK_EXIT
}AVI_ENCODE_VIDEO_ENUM;

typedef enum
{
	AVI_AUDIO_DMA_DONE = 1,//C_DMA_STATUS_DONE
	AVI_AUDIO_RECORD_START = 0x4000,
	AVI_AUDIO_RECORD_STOP,
	AVI_AUDIO_RECORD_STOPING,
	AVI_AUDIO_RECORD_RESTART,
	AVI_AUDIO_RECORD_EXIT
}AVI_ENCODE_AUDIO_ENUM;

typedef enum
{
	//AVI_DEL_FILE = 0x5000
	MSG_STORAGE_SERVICE_DEL = 0x5000,
	MSG_STORAGE_SERVICE_STORAGE_CHECK,
	MSG_STORAGE_SERVICE_FREESIZE_CHECK_SWITCH,
	MSG_STORAGE_SERVICE_FREESIZE_CHECK,
	MSG_STORAGE_SERVICE_VIDEO_FILE_DEL,
	MSG_STORAGE_SERVICE_AUTO_DEL_LOCK,
	MSG_STORAGE_SERVICE_TIMER_START,
	MSG_STORAGE_SERVICE_TIMER_STOP,
	MSG_STORAGE_SERVICE_AUD_REQ,
	MSG_STORAGE_SERVICE_AUD_REPLY,
	MSG_STORAGE_SERVICE_PIC_REQ,
	MSG_STORAGE_SERVICE_PIC_REPLY,
	MSG_STORAGE_SERVICE_PIC_DONE,
	MSG_STORAGE_SERVICE_VID_REQ,
	MSG_STORAGE_SERVICE_VID_REPLY,
	MSG_STORAGE_SERVICE_VID_CYCLIC_REQ,
	MSG_STORAGE_SERVICE_VID_CYCLIC_REPLY,
	MSG_STORAGE_SERVICE_MOUNT,
	MSG_STORAGE_SERVICE_NO_STORAGE,
	MSG_STORAGE_SERVICE_FORMAT_REQ,
	MSG_STORAGE_SERVICE_FORMAT_REPLY,
	MSG_STORAGE_SERVICE_BROWSE_REQ,		
	MSG_STORAGE_SERVICE_BROWSE_REPLY,
	MSG_STORAGE_SERVICE_THUMBNAIL_REQ,		
	MSG_STORAGE_SERVICE_THUMBNAIL_REPLY	
}TASK_STORAGE_SERVICE_ENUM;

typedef enum
{
	MSG_PERIPHERAL_TASK_KEY_DETECT = 0x6000,
	MSG_PERIPHERAL_TASK_KEY_REGISTER,
	MSG_PERIPHERAL_TASK_AD_DETECT_CHECK,
	MSG_PERIPHERAL_TASK_BAT_STS_REQ,
	MSG_PERIPHERAL_TASK_MOTION_DETECT_START,
	MSG_PERIPHERAL_TASK_MOTION_DETECT_STOP,	
	MSG_PERIPHERAL_TASK_MOTION_DETECT_JUDGE,
	MSG_PERIPHERAL_TASK_USBD_DETECT_INIT,
	MSG_PERIPHERAL_TASK_USBD_DETECT_CHECK,
	MSG_PERIPHERAL_TASK_LCD_BACKLIGHT_SET,
	MSG_PERIPHERAL_TASK_LED_SET,
	MSG_PERIPHERAL_TASK_LED_FLASH_SET,
	MSG_PERIPHERAL_TASK_LED_FLASH,
	MSG_PERIPHERAL_TASK_ZOOM_FLAG,
	MSG_PERIPHERAL_TASK_TEST
}TASK_PERIPHERAL_ENUM;

typedef enum
{
	MSG_APQ_MODE = 0x7000,
	MSG_APQ_PERIPHERAL_TASK_READY,
	MSG_APQ_FUNCTION_KEY_ACTIVE,
	MSG_APQ_NEXT_KEY_ACTIVE,
	MSG_APQ_PREV_KEY_ACTIVE,
	MSG_APQ_POWER_KEY_ACTIVE,
	MSG_APQ_MENU_KEY_ACTIVE,
	MSG_APQ_RECORD_SWITCH_FILE,
	MSG_APQ_AVI_PACKER_ERROR,
	MSG_APQ_CYCLIC_VIDEO_RECORD,
	MSG_APQ_VIDEO_FILE_DEL_REPLY,
	MSG_APQ_MOTION_DETECT_ACTIVE,
	MSG_APQ_MOTION_DETECT_TICK,
	MSG_APQ_MOTION_DETECT_TICK_END,
	MSG_APQ_MOTION_DETECT_ENTRY_CHECK,
	MSG_APQ_CONNECT_TO_PC,
	MSG_APQ_DISCONNECT_TO_PC,
	MSG_APQ_BATTERY_LVL_SHOW,
	MSG_APQ_LOW_BATTERY_HANDLE,
	MSG_APQ_KEY_IDLE,
	MSG_APQ_MJPEG_DECODE_END,
	MSG_APQ_SCROLL_BAR_END,
	MSG_APQ_PREVIEW_EFFECT_END,
	MSG_APQ_AUTO_POWER_OFF,
	MSG_APQ_INIT_THUMBNAIL
}TASK_STATE_HANDLING_APQ_ENUM;

typedef enum
{
	MSG_USBD_PLUG_IN = 0x8000,
	MSG_USBD_PLUG_OUT,
	MSG_USBD_INIT,
	MSG_USBD_PCAM_INIT,
	MSG_USBCAM_PLUG_IN,
	MSG_USBD_SWITCH
}TASK_USB_ENUM;