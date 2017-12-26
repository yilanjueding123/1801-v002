#include	"drv_l1_sensor.h"
#include "program_01.h"
#include "mini_dvr_api.h"

//2011-10-22 17:05 this file copy from svn3376_0923_B.rar

//=== This is for code configuration DON'T REMOVE or MODIFY it ===//
#if (defined _DRV_L1_SENSOR) && (_DRV_L1_SENSOR == 1)             //
//================================================================//
void sccb_delay (INT16U i);
void sccb_start (void);
void sccb_stop (void);
void sccb_w_phase (INT16U value);
INT16U sccb_r_phase (void);
void sccb_init (INT32U nSCL, INT32U nSDA);
void sccb_write (INT16U id, INT16U addr, INT16U data);
INT16U sccb_read (INT16U id, INT16U addr);
void CSI_Init (INT16S nWidthH, INT16S nWidthV, INT16U uFlag, INT32U uFrmBuf0, INT32U uFrmBuf1);
void Sensor_Bluescreen_Enable(void);
void Sensor_Cut_Enable(void);

INT16U read_cmos_sensor_id(void);
void cmos_sensor_init(INT16S nWidthH, INT16S nWidthV, INT16U uFlag);
//====================================================================================================
// SCCB Control C Code
// using SDA and SDL to control sensor interface
//====================================================================================================

//====================================================================================================
//	Description:	Delay function
//	Syntax:			sccb_delay (
//						INT16U i
//					);
//	Return: 		None
//====================================================================================================
void sccb_delay (
	INT16U i
) {
	INT16U j;

	for (j=0;j<(i<<8);j++)
		i=i;
}

//===================================================================
//	Description:	Start of data transmission
//	Function:		sccb_start
//	Syntax:			void sccb_start (void)
//	Input Paramter:
//					none
//	Return: 		none
//	Note:			Please refer to SCCB spec
//===================================================================
void sccb_start (void)
{
	gpio_write_io(SCCB_SCL, DATA_HIGH);					//SCL1
	sccb_delay (1);
	gpio_write_io(SCCB_SDA, DATA_HIGH);					//SDA1
	sccb_delay (1);
	gpio_write_io(SCCB_SDA, DATA_LOW);					//SDA0
	sccb_delay (1);
}

//===================================================================
//	Description:	Stop of data transmission
//	Function:		sccb_stop
//	Syntax:			void sccb_stop (void)
//	Input Paramter:
//					none
//	Return: 		none
//	Note:			Please refer to SCCB spec
//===================================================================
void sccb_stop (void)
{
	sccb_delay (1);
	gpio_write_io(SCCB_SDA, DATA_LOW);					//SDA0
	sccb_delay (1);
	gpio_write_io(SCCB_SCL, DATA_HIGH);					//SCL1
	sccb_delay (1);
	gpio_write_io(SCCB_SDA, DATA_HIGH);					//SDA1
	sccb_delay (1);
}

//===================================================================
//	Description:	Phase write process
//	Function:		sccb_w_phase
//	Syntax:			void sccb_w_phase (INT16U value)
//	Input Paramter:
//					INT16U value:	output data
//	Return: 		none
//	Note:			Please refer to SCCB spec
//===================================================================
void sccb_w_phase (INT16U value)
{
	INT16U i;

	for (i=0;i<8;i++)
	{
		gpio_write_io(SCCB_SCL, DATA_LOW);					//SCL0
		sccb_delay (1);
		if (value & 0x80)
			gpio_write_io(SCCB_SDA, DATA_HIGH);				//SDA1
		else
			gpio_write_io(SCCB_SDA, DATA_LOW);				//SDA0
//		sccb_delay (1);
		gpio_write_io(SCCB_SCL, DATA_HIGH);					//SCL1
		sccb_delay(1);
		value <<= 1;
	}
	// The 9th bit transmission
	gpio_write_io(SCCB_SCL, DATA_LOW);						//SCL0
	gpio_init_io(SCCB_SDA, GPIO_INPUT);						//SDA is Hi-Z mode
	sccb_delay(1);
	gpio_write_io(SCCB_SCL, DATA_HIGH);						//SCL1
	sccb_delay(1);
	gpio_write_io(SCCB_SCL, DATA_LOW);						//SCL0
	gpio_init_io(SCCB_SDA, GPIO_OUTPUT);						//SDA is Hi-Z mode
}

//===================================================================
//	Description:	Phase read process
//	Function:		sccb_r_phase
//	Syntax:			INT16U sccb_r_phase (void)
//	Input Paramter: none
//	Return: 		INT16U			: input data
//	Note:			Please refer to SCCB spec
//===================================================================
INT16U sccb_r_phase (void)
{
	INT16U i;
	INT16U data;

	gpio_init_io(SCCB_SDA, GPIO_INPUT);						//SDA is Hi-Z mode
	data = 0x00;
	for (i=0;i<8;i++)
	{
		gpio_write_io(SCCB_SCL, DATA_LOW);					//SCL0
		sccb_delay(1);
		gpio_write_io(SCCB_SCL, DATA_HIGH);					//SCL1
		data <<= 1;
		data |=( gpio_read_io(SCCB_SDA));
		sccb_delay(1);
	}
	// The 9th bit transmission
	gpio_write_io(SCCB_SCL, DATA_LOW);						//SCL0
	gpio_init_io(SCCB_SDA, GPIO_OUTPUT);					//SDA is output mode
	gpio_write_io(SCCB_SDA, DATA_HIGH);						//SDA0, the nighth bit is NA must be 1
	sccb_delay(1);
	gpio_write_io(SCCB_SCL, DATA_HIGH);						//SCL1
	sccb_delay(1);
	gpio_write_io(SCCB_SCL, DATA_LOW);						//SCL0
	return data;
}

//====================================================================================================
//	Description:	SCCB Initialization
//	Syntax:			void sccb_init (
//						INT16S nSCL,				// Clock Port No,
//						INT16S nSDA				// Data Port No
//					);
//	Return: 		None
//====================================================================================================
void sccb_init (
	INT32U nSCL,			// Clock Port No
	INT32U nSDA				// Data Port No
){
	//init IO
	gpio_set_port_attribute(nSCL, ATTRIBUTE_HIGH);
	gpio_set_port_attribute(nSDA, ATTRIBUTE_HIGH);
	gpio_init_io(nSCL, GPIO_OUTPUT);				//set dir
	gpio_init_io(nSDA, GPIO_OUTPUT);				//set dir
	gpio_write_io(nSCL, DATA_HIGH);					//SCL1
	gpio_write_io(nSDA, DATA_HIGH);					//SDA0
}

//====================================================================================================
//	Description:	SCCB write register process
//	Syntax:			void sccb_write (
//						INT16U id,					// Slave ID
//						INT16U addr,				// Register Address
//						INT16U data				// Register Data
//					);
//	Return: 		None
//	Note:			Please refer to SCCB spec
//====================================================================================================
void sccb_write (
	INT16U id,					// Slave ID
	INT16U addr,				// Register Address
	INT16U data				  	// Register Data
) {
	// Data re-verification
	id &= 0xFF;
#if	(defined __OV2655_DRV_C__)||(defined __OV3640_DRV_C__)||(defined __OV5642_DRV_C__)
	addr &= 0xFFFF;
#else
	addr &= 0xFF;
#endif
	data &= 0xFF;

	// Serial bus output mode initialization
	gpio_set_port_attribute(SCCB_SCL, ATTRIBUTE_HIGH);
	gpio_set_port_attribute(SCCB_SDA, ATTRIBUTE_HIGH);
	gpio_init_io(SCCB_SCL, GPIO_OUTPUT);				//set dir
	gpio_init_io(SCCB_SDA, GPIO_OUTPUT);				//set dir

	// 3-Phase write transmission cycle is starting now ...
	gpio_write_io(SCCB_SCL, DATA_HIGH);				//SCL1
	gpio_write_io(SCCB_SDA, DATA_LOW);				//SDA0
	sccb_start ();									// Transmission start
	sccb_w_phase (id);								// Phase 1
#if	(defined __OV2655_DRV_C__)||(defined __OV3640_DRV_C__)||(defined __OV5642_DRV_C__)
	sccb_w_phase (addr>>8);							// Phase 2
#endif
	sccb_w_phase (addr);
	sccb_w_phase (data);							// Phase 3
	sccb_stop ();									// Transmission stop
}

//====================================================================================================
//	Description:	SCCB read register process
//	Syntax:			INT16U sccb_read (
//						INT16U id,					// Slave ID
//						INT16U addr				// Register Address
//					);
//	Input Paramter:
//					INT16U id		: slave device id
//					INT16U addr	: register address
//	Return:
//					INT16U 		: register data
//	Note:			Please refer to SCCB spec
//====================================================================================================
INT16U sccb_read (
		INT16U id,					// Slave ID
		INT16U addr					// Register Address
) {
	INT16U data;

	// Data re-verification
	id &= 0xFF;
	addr &= 0xFF;

	// Serial bus output mode initialization
	gpio_set_port_attribute(SCCB_SCL, ATTRIBUTE_HIGH);
	gpio_set_port_attribute(SCCB_SDA, ATTRIBUTE_HIGH);
	gpio_init_io(SCCB_SCL, GPIO_OUTPUT);				//set dir
	gpio_init_io(SCCB_SDA, GPIO_OUTPUT);				//set dir

	// 2-Phase write transmission cycle is starting now ...
	gpio_write_io(SCCB_SCL, DATA_HIGH);				//SCL1
	gpio_write_io(SCCB_SDA, DATA_LOW);				//SDA0
	sccb_start ();									// Transmission start
	sccb_w_phase (id);								// Phase 1
	sccb_w_phase (addr);							// Phase 2
	sccb_stop ();									// Transmission stop

	// 2-Phase read transmission cycle is starting now ...
	sccb_start ();									// Transmission start
	sccb_w_phase (id | 0x01);						// Phase 1 (read)
	data = sccb_r_phase();							// Phase 2
	sccb_stop ();									// Transmission stop
	return data;
}

INT16U read_cmos_sensor_id(void)
{
	INT16U id1, id2;
	
	id1 = sccb_read(0x60, 0x0A);
	id2 = sccb_read(0x60, 0x0B);
	if (((id1&0x00ff)==0x26)&&((id2&0x00ff)==0x42)) return SENSOR_CHIP_OV2640;
	id1 = sccb_read(0xDC, 0xFC);
	id2 = sccb_read(0xDC, 0xFD);
	if (((id1&0x00ff)==0x37)&&((id2&0x00ff)==0x03)) return SENSOR_CHIP_BF3703;
	if (((id1&0x00ff)==0x36)&&((id2&0x00ff)==0x03)) return SENSOR_CHIP_BF3603;
	if (((id1&0x00ff)==0x3A)&&((id2&0x00ff)==0x03)) return SENSOR_CHIP_BF3A03;
	id1 = sccb_read(0x80, 0x00);
	id2 = sccb_read(0x80, 0x01);
	if (((id1&0x00ff)==0x63)&&((id2&0x00ff)==0x27)) return SENSOR_CHIP_PAS6329;
	id1 = sccb_read(0x42, 0x0A);
	id2 = sccb_read(0x42, 0x0B);
	if (((id1&0x00ff)==0x76)&&((id2&0x00ff)==0x73)) return SENSOR_CHIP_OV7670;
	if (((id1&0x00ff)==0x76)&&((id2&0x00ff)==0x80)) return SENSOR_CHIP_OV7680;
	if (((id1&0x00ff)==0x77)&&((id2&0x00ff)==0x21)) return SENSOR_CHIP_OV7725;
	id1 = sccb_read(0x42, 0x00);
	if (((id1&0x00ff)==0x9b)) return SENSOR_CHIP_GC0308;
	return SENSOR_CHIP_DONT_RECOGNISE;
}

void sensor_mirror_BF3703(void)
{
	#if SENSOR_MIRROR_ENABLE == 0
	sccb_write(0xDC, 0x1e, 0x00);
	#else
	sccb_write(0xDC, 0x1e, 0x30);
	#endif
}

void sensor_mirror_PAS6329(void)
{
	sccb_write(0x80, 0xEF, 0x01);
	#if SENSOR_MIRROR_ENABLE == 0
	sccb_write(0x80, 0x21, 0x00);
	#else
	sccb_write(0x80, 0x21, 0x0C);
	#endif
	sccb_write(0x80, 0xEF, 0x00);
}
//====================================================================================================
//	Description:	Sensor Initialization
//	Syntax:			void cmos_sensor_init (
//						INT16S nWidthH,			// Active H Width
//						INT16S nWidthV,			// Active V Width
//						INT16U uFlag				// Flag Type
//					);
//	Return:			None
//====================================================================================================
void cmos_sensor_init (
	INT16S nWidthH,			// Active H Width
	INT16S nWidthV,			// Active V Width
	INT16U uFlag				// Flag Type
) {
	INT16U uCtrlReg1, uCtrlReg2;
	INT16S nReso;

	INT8U  slave_add;
	const INT16U *p;
	INT8U  sensor_reg_add, sensor_reg_val;
	INT8U  t;

	
//INT32U tmp;
#if MINI_DVR_BOARD_VERSION == GPL32680_MINI_DVR_KEYCHAIN_QFN
	gpio_drving_init_io(SCCB_SDA,(IO_DRV_LEVEL) IO_DRIVING_8mA);
    gpio_drving_init_io(SCCB_SCL,(IO_DRV_LEVEL) IO_DRIVING_8mA);
#endif

	// Enable CSI clock to let sensor initialize at first
#if CSI_CLOCK == CSI_CLOCK_27MHZ
	uCtrlReg2 = CLKOEN | CSI_RGB565 |CLK_SEL27M | CSI_HIGHPRI | CSI_NOSTOP;
#else
	uCtrlReg2 = CLKOEN | CSI_RGB565 |CLK_SEL48M | CSI_HIGHPRI | CSI_NOSTOP;
#endif

	uCtrlReg1 = CSIEN | YUV_YUYV | CAP;									// Default CSI Control Register 1
	if (uFlag & FT_CSI_RGB1555)											// RGB1555
	{
		uCtrlReg2 |= CSI_RGB1555;
	}
	if (uFlag & FT_CSI_CCIR656)										// CCIR656?
	{
		uCtrlReg1 |= CCIR656 | VADD_FALL | VRST_FALL | HRST_FALL;	// CCIR656
		uCtrlReg2 |= D_TYPE1;										// CCIR656
	}
	else
	{
		uCtrlReg1 |= VADD_RISE | VRST_FALL | HRST_RISE | HREF;		// NOT CCIR656
		uCtrlReg2 |= D_TYPE0;										// NOT CCIR656
	}
	if (uFlag & FT_CSI_YUVIN)										// YUVIN?
	{
		uCtrlReg1 |= YUVIN;
	}
	if (uFlag & FT_CSI_YUVOUT)										// YUVOUT?
	{
		uCtrlReg1 |= YUVOUT;
	}

	// Whether compression or not?
	nReso = ((nWidthH == 320) && (nWidthV == 240)) ? 1 : 0;
	if (nReso == 1)								// VGA
	{
#ifdef	__TV_QVGA__
		R_CSI_TG_HRATIO = 0x0102;					// Scale to 1/2
		R_CSI_TG_VRATIO = 0x0102;					// Scale to 1/2
		R_CSI_TG_HWIDTH = nWidthH;					// Horizontal frame width
		R_CSI_TG_VHEIGHT = nWidthV*2;				// Vertical frame width
#endif	// __TV_QVGA__
	}
	else
	{
		R_CSI_TG_HRATIO = 0;
		R_CSI_TG_VRATIO = 0;
	}

	R_CSI_TG_VL0START = 0x0000;						// Sensor field 0 vertical latch start register.
	R_CSI_TG_VL1START = 0x0000;						//*P_Sensor_TG_V_L1Start = 0x0000;
	R_CSI_TG_HSTART = 0x0000;						// Sensor horizontal start register.

	R_CSI_TG_CTRL0 = 0;								//reset control0
	
	#if CSI_CLOCK == CSI_CLOCK_27MHZ
	R_CSI_TG_CTRL1 = CSI_NOSTOP|CLKOEN|CLK_SEL27M; //enable CSI CLKO	//modify by xiangyong
	#else
	R_CSI_TG_CTRL1 = CSI_NOSTOP|CLKOEN|CLK_SEL48M; //enable CSI CLKO	//modify by xiangyong
	#endif
	
	drv_msec_wait(100); 							//wait 100ms for CLKO stable
	//tmp = R_SPI0_CTRL;
	R_SPI0_CTRL = 0;
	// CMOS Sensor Initialization Start...
	sccb_init (SCCB_SCL, SCCB_SDA);
	sccb_delay (200);
	
	g_sensor_id = read_cmos_sensor_id();
	slave_add = sensor_slave_address[g_sensor_id];
	p = sensor_initvalue[g_sensor_id];
	t = 1;
	while(1)
	{
		if (*p == SENSOR_INITVALUE_END) break;
		sensor_reg_add = (*p >> 8) & 0x00ff;
		sensor_reg_val = (*p >> 0) & 0x00ff;
		sccb_write(slave_add, sensor_reg_add, sensor_reg_val);
		p++;
		if (t)
		{//sensor的第一个初始值是reset;
			t = 0;
			OSTimeDly(10);
		}
	}	
//R_SPI0_CTRL = tmp;	

//uCtrlReg1 |= (1<<17);
	R_CSI_TG_CTRL1 = uCtrlReg2;					//*P_Sensor_TG_Ctrl2 = uCtrlReg2;
#if CSI_MODE == CSI_PPU_FRAME_MODE
	R_CSI_TG_CTRL0 = uCtrlReg1;					//*P_Sensor_TG_Ctrl1 = uCtrlReg1;
#elif CSI_MODE == CSI_TG_FRAME_MODE
	R_CSI_TG_CTRL0 = uCtrlReg1|0x010000;
#elif CSI_MODE == CSI_FIFO_8_MODE
	R_CSI_TG_CTRL0 = uCtrlReg1|0x110000;
#elif CSI_MODE == CSI_FIFO_16_MODE
	R_CSI_TG_CTRL0 = uCtrlReg1|0x210000;
#elif CSI_MODE == CSI_FIFO_32_MODE
	R_CSI_TG_CTRL0 = uCtrlReg1|0x310000;
#endif

#if AVI_WIDTH==720
//for output D1
	R_CSI_TG_CTRL0 |= (1<<17);
	R_CSI_TG_CUTSTART = 0x0100;
	R_CSI_TG_CUTSIZE = 0x1B28;
	R_CSI_TG_CTRL1 |= 0x0100;
#else

#endif
}

//====================================================================================================
//	Description:	Initial CMOS sensor
//	Function:		CSI_Init (nWidthH, nWidthV, uFlag, uFrmBuf0, uFrmBuf1, uFrmBuf2)
//	Syntax:			void CSI_Init (INT16S nWidthH, INT16S nWidthV, INT16U uFlag, INT32U uFrmBuf0, INT32U uFrmBuf1, INT32U uFrmBuf2)
//	Input Paramter:	INT16S nWidthH: Active horizontal width
//					INT16S nWidthV: Active vertical width
//					INT16U uFlag: Flag type selection
//					INT32U uFrmBuf0: Frame buffer index 0
//	Return: 		none
//====================================================================================================
void CSI_Init (INT16S nWidthH, INT16S nWidthV, INT16U uFlag, INT32U uFrmBuf0, INT32U uFrmBuf1)
{
	// Setup sensor frame buffer start address
	*P_CSI_TG_FBSADDR = uFrmBuf0;
#if CSI_MODE != CSI_PPU_FRAME_MODE
	*P_CSI_TG_FBSADDR_B = uFrmBuf1;
#endif
	// Setup sensor frame size
	R_CSI_TG_HWIDTH = nWidthH;				// Horizontal frame width
#if AVI_WIDTH==720
	R_CSI_TG_VHEIGHT = 448;				// Vertical frame width
#else	
	R_CSI_TG_VHEIGHT = nWidthV;				// Vertical frame width
#endif	
	//
	//	CMOS Sensor Interface (CSI) H/W Initialization
	//
	cmos_sensor_init (nWidthH, nWidthV, uFlag);
}
//====================================================================================================
//	Description:	Wait capture delay complete
//	Function:		CSI_WaitCaptureComplete ()
//	Syntax:			void CSI_WaitCaptureComplete (void)
//	Input Paramter:	none
//	Return: 		none
//====================================================================================================
void CSI_WaitCaptureComplete (void)
{
	while ((R_PPU_IRQ_STATUS & 0x40) == 0);
	R_PPU_IRQ_STATUS = 0x40;
}

void Sensor_Bluescreen_Enable(void) {
	R_CSI_TG_CTRL0 |= 0x0080;
	R_CSI_TG_BSUPPER = 0x001f;
	R_CSI_TG_BSLOWER = 0x0000;
}
void Sensor_Bluescreen_Disable() {
	R_CSI_TG_CTRL0 &= 0xFF7F;
}

void Sensor_Cut_Enable(void) {
	R_CSI_TG_CTRL1 |= 0x0100;	//Enable Cut function
	R_CSI_TG_CUTSTART = 0x0808;
	R_CSI_TG_CUTSIZE  = 0x0808;
}
void Sensor_Cut_Disable(void) {
	R_CSI_TG_CTRL1 &= 0xFEFF;
}

void Sensor_Black_Enable(void) {
	R_CSI_TG_HSTART = 0x50;
	R_CSI_TG_VSTART = 0x28;
	R_CSI_TG_HEND = 0xf0;
	R_CSI_TG_VEND = 0xc8;
}

void Sensor_Black_Disable(void) {
	R_CSI_TG_HSTART = 0x0000;
	R_CSI_TG_VSTART = 0x0000;
	R_CSI_TG_HEND = 0x0140;
	R_CSI_TG_VEND = 0x00f0;
}
#if C_MOTION_DETECTION == CUSTOM_ON
void Sensor_MotionDection_Inital(INT32U buff)
{ 
   	R_CSI_MD_CTRL = MD_VGA|MD_FRAME_2|MD_THRESHOLD|MD_MODE_SINGLEIIR;
   	*P_CSI_MD_FBADDR = buff; 		
}

void Sensor_MotionDection_stop(void)
{
	R_CSI_MD_CTRL &= (~MD_EN);
	R_PPU_IRQ_EN &= ~0x80;
 	R_PPU_IRQ_STATUS |= 0x80;
}
void Sensor_MotionDection_start(void)
{
	R_CSI_MD_CTRL |= (MD_EN);
	R_PPU_IRQ_STATUS |= 0x80;
	R_PPU_IRQ_EN |= 0x80;
}
#endif
//=== This is for code configuration DON'T REMOVE or MODIFY it ===//
#endif //(defined _DRV_L1_SENSOR) && (_DRV_L1_SENSOR == 1)        //
//================================================================//