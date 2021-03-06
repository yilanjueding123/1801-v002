#include "program_01.h"

//2011-10-23 17:12, 以下初始化代码 copy from \\prj_dvr\JH1117\Code\第3版;
const INT16U dummy_initvalue[]={SENSOR_INITVALUE_END};

const INT16U OV2640_initvalue[]={
#if SENSOR_OV2640_ENABLE==1
0xff01,
//;>> 400 ;(ms)
0x1280,
0xff00,
0xff00, 
0xff00,
0x2cff,
0x2edf,
0xff01,
0x3c32,
0x1100,
0x0902,
0x03cf,
0x0408, // horizontal turn 88/28
0x13e5,
0x1448,
0x2c0c,
0x3378,
0x3a33,
0x3bfb,
0x3e00,
0x4311,
0x1610,
0x3902,
0x3588,
0x220a,
0x3740,
0x2300,
0x34a0,
0x361a,
0x0602,
0x07c0,
0x0db7,
0x0e01,
0x4c00,
0x4a81,
0x2199,
0x243a,
0x2532,
0x2682,
0x5c00,
0x6300,
0x5d55,
0x5e7d,
0x5f7d,
0x6055,
0x6170,
0x6280,
0x7c05,
0x2080,
0x2830,
0x6c00,
0x6d80,
0x6e00,
0x7002,
0x7196,
0x73e1,
0x3d34,
0x5a57,
0x4fbb,
0x509c,
0x0f43,
0xff00,
0xe57f,
0xf9c0,
0x4124,
0xe014,
0x76ff,
0x33a0,
0x4220,
0x4318,
0x4c00,
0x87d0,
0x883f,
0xd703,
0xd910,
0xd382,
0xc808,
0xc980,
0x7c00,
0x7d02,
0x7c03,
0x7d48,
0x7d48,
0x7c08,
0x7d20,
0x7d10,
0x7d0e,
0x9000,
0x910e,
0x911a,
0x9131,
0x915a,
0x9169,
0x9175,
0x917e,
0x9188,
0x918f,
0x9196,
0x91a3,
0x91af,
0x91c4,
0x91d7,
0x91e8,
0x9120,
0x9200,
0x9306,
0x93e3,
0x9305,
0x9305,
0x9300,
0x9300,
0x9300,
0x9300,
0x9300,
0x9300,
0x9300,
0x9300,
0x9300,
0x9600,
0x9708,
0x9719,
0x9702,
0x970c,
0x9724,
0x9730,
0x9728,
0x9726,
0x9702,
0x9798,
0x9780,
0x9700,
0x9700,
0xc3ed,
0xa400,
0xa800,
0xbf00,
0xbaf0,
0xbc64,
0xbb02,
0xb63d,
0xb857,
0xb738,
0xb94e,
0xb3e8,
0xb4e1,
0xb566,
0xb067,
0xb15e,
0xb204,
0xc700,
0xc651,
0xc511,
0xc49c,
0xcf02,
0xa600,
0xa7e0,
0xa710,
0xa71e,
0xa721,
0xa700,
0xa728,
0xa7d0,
0xa710,
0xa716,
0xa721,
0xa700,
0xa728,
0xa7d0,
0xa710,
0xa717,
0xa721,
0xa700,
0xa728,
0xc0c8,
0xc196,
0x861d,
0x5000,
0x5190,
0x5218,
0x5300,
0x5400,
0x5588,
0x5700,
0x5a90,
0x5b18,
0x5c05,
0xc3ef,
0x7f00,
0xda00,
0xe51f,
0xe167,
0xe000,
0xddff,
0x0500,
0xff01,

0xff01,
0x1200,
0x1711,
0x1875,
0x1901,
0x1a97,
0x3236,
0x4fbb,
0x6d80,
0x3d34,
0x3902,
0x3588, 
0x220a,
0x3740,
0x2300,
0x34a0,
0x361a,
0x0602,
0x07c0,
0x0db7,
0x0e01, 
0x4c00,
0xff00,  
0xe004,
0x8c00, 
0x87d0, 
0xe000,

0xff00, 
0xe014, 
0xe177, 
0xe51f, 
0xd703,
0xda10, 
0xe000,

0xff00,
0xe004,
0xc0c8,
0xc196,
0x861d,
0x5000,
0x5190,
0x522c,
0x5300,
0x5400, 
0x5588,
0x5700,
0x5a90,
0x5b2c, 
0x5c05, 
0xe000,
0xd304,

//;;;LENS
0xff00,
0xc3ef,
//;R
0xa600,
0xa7dd,
0xa778, //;58
0xa77e, //;6e	;3d
0xa724,
0xa700,
0xa725,
//;G
0xa606,
0xa720,
0xa758,
0xa773,
0xa734,
0xa700,
0xa725,
//;B
0xa60c,
0xa728,
0xa758,
0xa76d,
0xa734,
0xa700,
0xa725,

//[Sensor.YUV]	; YUV422
0xff00,
0xe004,
0xe167,
0xe51f,
0xd701,
0xda00,
0xe000,
0xFF01,

#if SENSOR_MIRROR_ENABLE == 1
0x0408,
#else
0x04d8, //0x04c8,
#endif

//[Sensor.YUV.640x480]
0xff01,
0x1240,
0x1711,
0x1843,
0x1900,
0x1a4b,
0x3209,
0x4fca,
0x50a8,
0x5a23,
0x6d00,
0x3912,
0x35da,
0x221a,
0x37c3,
0x2300,
0x34c0,
0x361a,
0x0688,
0x07c0,
0x0d87,
0x0e41,
0x4c00,

0xff00,
0xe004,
0xc064,
0xc14b,
0x8635,
0x5000,
0x51c8,
0x5296,
0x5300,
0x5400,
0x5500,
0x5700,
0x5aa0,
0x5b78,
0x5c00,
0xe000,

//[Sensor.YUV.640x480.22]
0xff01,
0x1100,
0x3d38,
0x2d00,
0x5065,
0x4600,
0x4701,
0xff00,
0xd382,

#endif
SENSOR_INITVALUE_END
};

const INT16U BF3A03_initvalue[]={
#define SENSOR_LOW_FPS_ENABLE	0
#if SENSOR_BF3A03_ENABLE==1
#define BF3A03_MAX_20_FPS		0
#define BF3A03_MAX_25_FPS		1
#define BF3A03_MAX_FPS			BF3A03_MAX_25_FPS
//FPS: 20
//ver: 2.00  
//2.00: 2014-02-12   BYD郑工调试的 20FPS 代码(25FPS)
//3.00: 增加低帧率(17FPS)选择, 也可手动选择20FPS或25FPS,  17FPS来源于BYD邹工 2014年3月24日 邮件  
//
0x1280,
0x095A,	//0x095a,	//0x0955, //lx driv
0x1502,
#if SENSOR_MIRROR_ENABLE == 1
0x1e70,	//0x1e40,//HV mirror
#else
0x1e40,
#endif

//Analog signals
0x0678,//68
0x2100,
0x3e37,
0x292f,
0x2798,
0x162f,	//0x1625, lx0906
0x2000,

//Clock
0x2f4e,	//42  0x2f4C,	//0x2f4e,//有绿色雪花点时，写为0x42; //lx 
0x1110,//分频[1:0]
0x1b09,	//0x1b21,_50 60fpa	//0x1b09,//倍频 PLL使能与0x2f[1]、0x1b[3]  //lx



//数据输出格式
0x4a98,//Sub、Win
0x1200,//YUV、sub时序
0x3a00,//YUV时序

//Manual
0x1308,//手写bit[3]GLB GAIN0
0x0114,
0x0220,
0x8c02,
0x8d4c,
0x8716,//GLB GAIN0

//Auto
0x1307,	//0x1387,	//0x1307,  /liuxi  曝光压缩值

//Denoise
0x700f,//插值模块开关
0x3b00,//去格子噪声
0x710c,//插值模块开关
0x7327,//Denoise
0x7588,//Outdoor denoise
0x76d8,
0x770a,//Low light denoise
0x78ff,
0x7914,
0x7a24,	//0x7a12,//边缘增强  //lx
0x9e04,	//0x9ec4,	//lx
0x7d2a,//去紫边

//Gamma default
0x39c0,//Gamma offset
0x3fc0,
0x9020,
0x5f04,//Dark_sel gamma

/*//默认
0x4022,
0x4123, 
0x4228,
0x4325,
0x441d,
0x4517,
0x4613,
0x4712,
0x4810,
0x490d,
0x4b0b,    
0x4c0b,
0x4e09,
0x4f07,
0x5006,
*/
/*
// 20130909 test  画面有点灰
0x4028,
0x4125, 
0x4223,
0x4320,
0x441d,
0x4517,
0x4613,
0x4712,
0x4810,
0x490d,
0x4b0b,    
0x4c0b,
0x4e09,
0x4f07,
0x5006,
*/
//=====2013年9月10日 17:55===
//==BYD 邹坤 邮件============
//第一组是在2013年9月9日修改的基础上改的，第二组和第三组是将中下段都有拉升，第三组低光部分会比第二组亮一些。
/*//Gamma1：
0x4028,
0x4125,
0x4223,
0x4320,
0x441d,
0x4517,
0x4613,
0x4712,
0x4810,
0x490d,
0x4b0b,
0x4c0b,
0x4e07,
0x4f05,
0x5004,
*/
/*
//Gamma2：
0x4028,
0x412d,
0x4228,
0x4328,
0x4422,
0x4519,
0x4615,
0x470f, 
0x480b, 
0x4909, 
0x4b07, 
0x4c06, 
0x4e04, 
0x4f03, 
0x5003,
*/
//Gamma3：
0x4030,
0x4134,
0x4232,
0x4327,
0x441D,
0x4518,
0x4612,
0x470F,
0x480B,
0x4909,
0x4b07,
0x4c06,
0x4e05,
0x4f03,
0x5002,

/*//Gamma 低噪
0x4024,
0x4130,
0x4224,
0x431d,
0x441a,
0x4514,
0x4611,
0x470e,
0x480d,
0x490c,
0x4b0b,
0x4c09,
0x4e09,
0x4f08,
0x5007,

//Gamma 过曝过度好
0x4018,
0x412c,
0x4228,
0x4320,
0x4416,
0x4510,
0x460f,
0x470f,
0x480e,
0x490a,
0x4b0b,
0x4c09,
0x4e09,
0x4f08,
0x5006, */

/*//Gamma 清晰亮丽
0x4019,
0x411e,
0x421f,
0x4320,
0x441d,
0x4519,
0x4617,
0x4717,
0x4814,
0x4912,
0x4b0f,
0x4c0c,
0x4e08,
0x4f06,
0x5003,
*/
//AE
0x2448,	//0x2450,//非A光Y_target  /lx 
0x9740,//A光Y_target  
0x2588,//AE_LOC
0x8100,//AE speed
0x8218,	//0x821b,//18, lx AE
0x8337,//30,
0x8439,//20,
0x855d,//38,
0x8699,	//0x8688,//55,   //lx 
0x9402,	//0x9482,//对AE亮度权重调节
0x8092,//对过曝点数调节,bit[1]choose 50HZ or 60HZ step;
0x98bf,	//0x988a,//对窗口权重调节
#if SENSOR_LOW_FPS_ENABLE == 0
0x8915,	//0x891d,//帧率调节bit[7:3]				//20FPS
#else
0x892d,	//0x8935,//0x891d,//帧率调节bit[7:3]				//17FPS
#endif
0x8e2c,
0x8f86,

//Banding
0x2b20,
#if SENSOR_LOW_FPS_ENABLE == 0
#if BF3A03_MAX_FPS	==	BF3A03_MAX_20_FPS
0x8adc,	//0x8a93, //50HZ  //lx 45fps 												//20FPS or 25FPS
0x92e1,	//20FPS	 //0x9200, /60fps	//0x9255, //lx 
#else
0x8adc,	//0x8a93, //50HZ  //lx 45fps 												//20FPS or 25FPS
0x924e,	//25fps 
#endif
#else
0x8a93,	//0x8a93, //50HZ  //lx 45fps 												//17FPS
0x9268,	//0x9250,50fps //0x9200, /60fps	//0x9255, //lx 	
0x9301,	
#endif
0x8b7a,//60HZ


//Color
0x5aec,//Outdoor color
0x5190,
0x5210,
0x538d,
0x5488,
0x5782,
0x588d,
0x5a7c,//A light color  
0x5180,
0x5204,
0x538d,
0x5488,
0x5782,
0x588d,

/*
//Color default 
0x5a6c,//Indoor color  
0x5193,
0x5204,
0x538a,
0x5488,
0x5702,
0x588d, 
*/

//Color大镜头-肤色较好
0x5a6C,
0x5190,
0x5204,
0x5385,
0x548A,
0x5702,
0x588d,

/*//Color色彩艳丽
0x5a6c,//Indoor color 
0x51a0,
0x5201,
0x538d,
0x5485,
0x5701,
0x5890,
	
//Color肤色较好           
0x5a6c,//Indoor color   
0x5190,
0x520a,
0x5384,
0x5405,
0x5705,
0x5887,

//Color色彩淡        
0x5a6c,//Indoor color 
0x5185,
0x5206,
0x538a,
0x5481,
0x5702,
0x588a,*/

//Saturation
0xb0a0,//A光饱和度
0xb126,
0xb21c,
0xb4fd,
0xb030,//非A光饱和度 
0xb1d8,
0xb2b0,
0xb471,

//Contrast
0x3c40,//K1
0x5644,//K2 0xb4[4]选择new or old 
0x4d40,//K3
0x5940,//K4

/*//G gain大一些 
0x3556,//shading R
0x6536,//shading G 
0x6644,//shading B
//AWB  
0x6a91,
0x2344,
0xa204,
0xa326,
0xa404,
0xa526,
0xa71a,
0xa810,
0xa91f,
0xaa16,
0xab16,
0xac30,
0xadf0,
0xae57,
0xc5aa,
0xc738,
0xc80d,
0xc916,
0xd309,
0xd415,
0xd000, 
0xd101,
0xd258,*/

//G gain小一些   
0x3546,//shading R
0x653a,//shading G
0x6640,//shading B
//AWB 
0x6ad1,//AWB
0x2322,//G GAIN
0xa208,//非户外 B上下线
0xa326,
0xa404,//非户外 R上下线
0xa526,
0xa713,//B GAIN
0xa88e,//R GAIN
0xa916,//白点
0xaa16,
0xab16,
0xac30,
0xadf0,
0xae57,
0xc566,
0xc738,//户内、外开关bit[4]
0xc80d,//非户外 B上下线
0xc916,
0xd309,//非户外 R上下线
0xd415,
0xd000,//A光下的offset
0xd101,//非A光下的offset
0xd218,	//0xd258, //lx
0xf100,	//lx add AE//01

#endif
SENSOR_INITVALUE_END
};

const INT16U OV7725_initvalue[]={
#if SENSOR_OV7725_ENABLE==1
#endif
SENSOR_INITVALUE_END};

const INT16U OV7670_initvalue[]={
#if SENSOR_OV7670_ENABLE==1
/*current ver: 2.00
history:
ver:1.00 (2011-10-23, copy from \\prj_dvr\JH1117\Code\第3版;) = fps:30-30fps (xiangyong 用示波器实测);
ver:2.00 (copy from 台湾卢工 svn3376_0110 的OV7670初始化代码) = fps:24-24fps (xiangyong 用示波器实测);
note:
*/
//--------------------------------
//0x1280, 
0x1104,
0x6bca,
0x3A04,
0x1200,
0x1713,
0x1801,
0x32B6,
0x1902,
0x1A7A,
0x030F,
0x0C00,
0x3E00,
0x703A,
0x7135,
0x7211,
0x73F0,
0xA23B,
#if SENSOR_MIRROR_ENABLE == 0
0x1E3F,
#else
0x1E0F, 	//0x1E3F,0x1E0F, mirror
#endif
0x7a20,
0x7b03,
0x7c0a,
0x7d1a,
0x7e3f,
0x7f4e,
0x805b,
0x8168,
0x8275,
0x837f,
0x8489,
0x859a,
0x86a6,
0x87bd,
0x88d3,
0x89e8,
0x13E0,
0x0000,
0x1000,
0x0D50,
0x4240,
0x1428,
0xA503,
0xAB03,
0x2450,
0x2543,
0x26a3,
0x9F78,
0xA068,
0xA103,
0xA6d2,
0xA7d2,
0xA8F0,
0xA980,
0xAA14,
0x13E5,
0x0E61,
0x0F4B,
0x1602,
0x2102,
0x2291,
0x2907,
0x330B,
0x350B,
0x371D,
0x3871,
0x392A,
0x3C78,
0x4D40,
0x4E20,
0x6900,
0x7410,
0x8D4F,
0x8E00,
0x8F00,
0x9000,
0x9100,
0x9600,
0x9A80,
0xB084,
0xB10C,
0xB20E,
0xB382,
0xB80A,
0x4302,
0x44f2,
0x4546,
0x4663,
0x4732,
0x483b,
0x5992,
0x5a9b,
0x5ba5,
0x5c7a,
0x5d4a,
0x5e0a,
0x6c0a,
0x6d55,
0x6e11,
0x6f9e,
0x6A40,
0x0140,
0x0240,
0x13f7,
0x4f9c,
0x5099,
0x5102,
0x5229,
0x538b,
0x54b5,
0x581e,
0x6208,
0x6310,
0x6404,
0x6500,
0x6605,
0x9404,
0x9506,
0x4108,
0x3F00,
0x7544,
0x76e1,
0x4C00,
0x7701,
0x3DC0,
0x4B09,
0xC960,
0x4138,
0x5640,
0x3411,
0x3b02,
0xa488,
0x9200,
0x9600,
0x9730,
0x9820,
0x9920,
0x9A84,
0x9B29,
0x9C03,
0x9D99,
0x9E7F,
0x7800,
0x7901,
0xc8f0,
0x790f,
0xc800,
0x7910,
0xc87e,
0x790a,
0xc880,
0x790b,
0xc801,
0x790c,
0xc80f,
0x790d,
0xc820,
0x7909,
0xc880,
0x7902,
0xc8c0,
0x7903,
0xc840,
0x7905,
0xc830,
0x7926,
0x3b02,
0x4302,
0x44f2,
0x304F,
#endif
SENSOR_INITVALUE_END};

const INT16U PAS6329_initvalue[]={
#if SENSOR_PAS6329_ENABLE==1
#define PAS6329_MAX_30_FPS		0
#define PAS6329_MAX_25_FPS		1
#define PAS6329_MAX_FPS			PAS6329_MAX_25_FPS
/*current ver: 2.10
history:
ver:1.00 (2011-10-23, copy from \\prj_dvr\JH1117\Code\第3版;)
ver:2.05 (2011-10-23, 降低锐度;)
ver:2.10 (2012-02-09, 解决画面毛毛)
note:
fps: 20-25fps (xiangyong 用示波器实测)
*/
//--------------------------------
0xEF00,		//page_00
0x0400,
0x0565,
0x060C,
0x0764,	//78,	//00,	//a0,//64,
0x0861,
0x0900,
0x0A64,
0x0C00,
0x0D00,
0x0E00,
0x0F9B,
0x116E,
#if PAS6329_MAX_FPS == PAS6329_MAX_25_FPS
0x1360,	//xiangyong 用示波器实测: 最高fps=25;
0x1422,
#else
0x13FC,	//xiangyong 用示波器实测: 最高fps=30;
0x1421,
#endif
0x1535,
0x1658,
0x1702,
0x1940,
0x1A68,
0x1B44,
0x1C69,
0x1DF5,
0x1E99,
0x1F10,
0x2022,		//18,//20,
0x2110,//16,
0x2260,
0x2322,		//18,//22,
0x2410,//18,
0x256E,
0x2600,
0x2700,
0x2901,
0x2A11,
0x2B20,
0x2C3A,
0x2D64,
0x2E75,
0x2F84,
0x3092,
0x319F,
0x32B5,
0x33C7,
0x34D5,
0x35E0,
0x36EF,
0x37F8,
0x38FD,
0x3901,
0x3A28,
0x3B28,
0x3C24,
0x3D1E,
0x3E02,	//01,//03,
0x3F0f,//0D,
0x400a,//6C,
0x4119,//4A,
0x4241,
0x4301,
0x4400,
0x4502,
0x4602,
0x4731,
0x483C,
0x497f,//7E,
0x4A7f,//7E,
0x4B42,
0x4C10,//05,
0x4D64,
0x4E87,
0x4F79,
0x5075,
0x5196,
0x528c,//86,
0x5314,
0x54F5,
0x5553,//64,
0x560F,	//18,
0x5701,
0x5806,
0x5903,
0x5A08,
0x5B02,
0x5C10,//0B,
0x5DFF,
0x5F05,//08,
0x6001,
0x6104,
0x6202,
0x6330,
0x6405,//0F,
0x6540,//60,
0x6612,	//0x6613, 50Hz/60Hz filter
0x67C7,
0x682D,
0x6B05,//0A,
0x6C0F,	//18,//1A,
0x6D58,//60,
0x6E13,
0x6F50,
0x7000,
0x7100,
0x7201,
0x73C5,
0x749C,
0x750D,
0x765E,
0x7801,
0x7916,
0x7B04,
0x7C03,
0x7D00,
0x7E00,
0x7FFF,
0x8086,
0x8100,
0x8416,
0x860D,
0x87EA,
0x8801,
0x8900,
0x8A00,
0x8B00,
0x8C9E,
0x8E00,
0x8F00,
0x9000,
0x9100,
0x9200,
0x9300,
0x9400,
0x950A,
0x9710,
0x9800,
0x995A,
0x9A41,
0x9B5C,
0x9C46,
0x9D5C,
0x9E46,
0x9F50,
0xA038,
0xA132,
0xA26E,
0xA35A,
0xA456,
0xA505,
0xA601,
0xA701,
0xA960,
0xAA00,
0xC6CD,
0xC700,
0xC880,
0xC900,
0xCA9A,
0xCB00,
0xDF20,
0xE080,
0xE102,
0xE2E0,
0xE301,
0xE600,
0xE987,
0xEA00,
0xEB00,
0xED00,
0xEE3F,
0xED01,
0xEF01,		//page_01
0x0201,
0x0383,
0x047C,
0x0502,
0x0611,
0x0711,
0x0811,
0x0911,
0x0C03,
0x0D18,
0x0E02,
0x0F00,
0x1000,
0x1100,
0x1208,
0x1700,
0x1800,
0x1900,
0x1A00,
0x1B8F,
0x1C02,
0x1DEB,
0x1E01,
0x2014,
#if SENSOR_MIRROR_ENABLE == 0
0x210C,
#else
0x2100,	//0x2100, 0x210C, mirror
#endif
0x2208,
0x2300,
0x2405,
0x2800,
0x2A00,
0x2B4A,
0x2C7D,
0x2D13,
0x2E49,
0x2F4A,
0x3068,
0x3196,
0x32AB,
0x3331,
0x3481,
0x3500,
0x3600,
0x3700,
0x3800,
0x3920,
0x3B00,
0x3C00,
0x3D00,
0x3E00,
0x4000,
0x4106,
0x4200,
0x4382,
0x440E,
0x4523,
0x4821,
0x4911,
0x4A00,
0x4B00,
0x5003,
0x5144,
0x5279,
0x5320,
0x5400,
0x5600,
0x5700,
0x5800,
0x5D00,
0x5E14,
0x6000,
0x6235,
0x637F,
0x6434,
0x6606,
0x6708,
0x6807,
0x6F00,
0x7000,
0x7100,
0x7200,
0x7300,
0x7400,
0x7500,
0x7600,
0x7720,
0x7805,
0x790F,
0x7A0F,
0x7B8A,
0x7C00,
0x7E00,
0x7F01,
0x8000,
0x8200,
0x8700,
0x8800,
0x8900,
0x8A00,
0x8B00,
0x8C00,
0x8D00,
0x8E00,
0x8F00,
0x9000,
0x9101,
0x9E02,
0x9F00,
0xA104,
0xA2C0,
0xA400,
0xA500,
0xA600,
0xA706,
0xA80F,
0xA900,
0xAAAA,
0xAB55,
0xAC66,
0xB005,
0xB13F,
0xB2CA,
0xB402,
0xB60C,
0xBE4A,
0xBF80,
0xC013,
0xC149,
0xC24A,
0xC380,
0xC413,
0xC549,
0xC64A,
0xC780,
0xC813,
0xC949,
0xCA00,
0xCB00,
0xCC00,
0xCD00,
0xCE00,
0xCF00,
0xD000,
0xD100,
0xD200,
0xD300,
0xD400,
0xD500,
0x1101,
0xEF02,		//page_02
0x0000,
0x0B1A,
0x0C4B,
0x0D40,		//44,//40,
0x0E00,
0x0F96,
0x1032,
0x110F,
0x128C,
0x1314,
0x142D,
0x151E,
0x1700,
0x1840,
0x1980,
0x1AB1,
0x1B86,
0x1C6e,//69,
0x1D7D,
0x1E84,
0x1F96,//9B,
0x20F5,
0x2102,
0x2208,
0x2618,
0x2710,//0A,
0x2Ad7,		//80,//D2,
0x2B00,
0x2C10,
0x2D10,
0x2E10,		//00,//10,
0x2F0F,	//18,//1A,
0x300E,	//12,
0x3101,
0x3510,
0x3604,
0x370a,//10,
0x388E,	//92,
0x3900,		//01,
0x3A8B,
0x3C00,
0x3E33,//32,
0x3F1f,//25,
0x4048,//4F,
0x4119,//0C,
0x4210,//0B,
0x4390,//42,
0x44a0,//4D,
0x4580,//46,
0x4680,//48,
0x4704,//02,
0x5020,
0x5102,
0x5210,
0x5302,
0x5401,
0x5500,
0x5608,
0x5706,		//0c,//06,
0x580C,		//0x5816,	//锐利度的强度: 锐度: 值越小, 越模糊;
0x5900,
0x5A05,
0x5B16,		//10,
0x5C0B,					//锐利度的边界判断门槛值: 值越小, 越容易产生锐化现像;
0x5D00,
0x5E08,
0x5F10,//18,
0x6015,	//12,//1F,
0x6100,
0x6205,
0x634F,
0x6403,
0x6544,
0x66c0,//80,
0x6707,
0x6808,
0x6900,
0x6A00,
0x6C32,
0x6D12,
0x6E08,
0x6F0D,
0x7002,
0x7100,
0x720E,
0x7308,
0x7431,
0x7501,
0x7614,
0x770A,
0x7814,
0x7903,
0x7A00,
0x7B02,
0x7C15,
0x7D00,
0x9B02,
0x9C80,
0x9D01,
0x9EE0,
0x9F00,
0xA000,
0xA100,
0xA200,
0xA402,
0xAD00,
0xAE00,
0xB200,
0xBC13,
0xBD01,
0xBE00,
0xBF00,	//0xBF04,
0xC000,
0xC103,
0xC200,
0xE004,
0xE300,
0xE400,
0xE500,
0xE600,
0xEB00,
0x0001,
0xEF01,
0x4B08,
0x1101,
#endif
SENSOR_INITVALUE_END};

const INT16U GC0308_initvalue[]={
#if SENSOR_GC0308_ENABLE==1
/*current ver: 1.00
history:
ver:1.00 (2011-10-23, copy from \\prj_dvr\JH1117\Code\第3版;)
note:
fps: 20.0-25.0fps (xiangyong 用示波器实测)
*/
//--------------------------------
//0xFE80,
0xFE00,
0xD210,
0x2255,
0x5A56,
0x5B40,
0x5C4A,
0x2257,
0x01D7,
0x0228,
0x0F00,
0x0301,
0x048C,
0xE200,
0xE384,
0xE402,
0xE510,
0xE602,
0xE794,
0xE804,
0xE920,
0xEA05,
0xEB28,
0xEC10,
0x0500,
0x0600,
0x0700,
0x0800,
0x0901,
0x0AE8,
0x0B02,
0x0C88,
0x0D02,
0x0E02,
0x1026,
0x110D,
0x122A,
0x1300,
0x1410,
0x150A,
0x1605,
0x1701,
0x1844,
0x1944,
0x1A2A,
0x1B00,
0x1C49,
0x1D9A,
0x1E61,
0x1F16,
0x20FF,
0x21F8,
0x2257,
0x24A2,
0x250F,
0x2603,
0x2F01,
0x30F7,
0x3150,
0x3200,
0x3904,
0x3A18,
0x3B20,
0x3C02,
0x3D02,
0x3E02,
0x3F02,
0x5014,
0x5382,
0x5480,
0x5580,
0x5682,
0x8B40,
0x8C40,
0x8D40,
0x8E2E,
0x8F2E,
0x902E,
0x913C,
0x9250,
0x5D12,
0x5E1A,
0x5F24,
0x6007,
0x6115,
0x6208,
0x6403,
0x66E8,
0x6786,
0x68A2,
0x6918,
0x6A0F,
0x6B00,
0x6C5F,
0x6D8F,
0x6E55,
0x6F38,
0x7015,
0x7133,
0x72DC,
0x7380,
0x7402,
0x753F,
0x7602,
0x7736,
0x7888,
0x7981,
0x7A81,
0x7B22,
0x7CFF,
0x9348,
0x9402,
0x9507,
0x96E0,
0x9740,
0x98F0,
0xB138,
0xB238,
0xBD38,
0xBE36,
0xD0CB,
0xD110,
0xB500,
0xD348,
0xD5F2,
0xD616,
0xDB92,
0xDCA5,
0xDF23,
0xD900,
0xDA00,
0xE009,
0xED04,
0xEEA0,
0xEF40,
0x8003,
0x8003,
0xC000,
0xC110,
0xC21C,
0xC330,
0xC443,
0xC554,
0xC665,
0xC775,
0xC893,
0xC9B0,
0xCACB,
0xCBE6,
0xCCFF,
0xF002,
0xF101,
0xF202,
0xF330,
0xF99F,
0xFA78,
0xFE01,
0x00F5,
0x0220,
0x0410,
0x0510,
0x0620,
0x0815,
0x0AA0,
0x0B64,
0x0C08,
0x0E4C,
0x0F39,
0x1041,
0x1137,
0x1224,
0x1339,
0x1445,
0x1545,
0x16C2,
0x17A8,
0x1818,
0x1955,
0x1AD8,
0x1BF5,
0x7040,
0x7158,
0x7230,
0x7348,
0x7420,
0x7560,
0x7720,
0x7832,
0x34E0,
0x35B0,
0x36C0,
0x37C0,
0x3804,
0x3909,
0x3A12,
0x3B1C,
0x3C28,
0x3D31,
0x3E44,
0x3F57,
0x406C,
0x4181,
0x4294,
0x43A7,
0x44B8,
0x45D6,
0x46EE,
0x470D,
0xFE00,
0xD290,
0x9F14,
0xA028,
0xA144,
0xA25D,
0xA372,
0xA486,
0xA595,
0xA6B1,
0xA7C6,
0xA8D5,
0xA9E1,
0xAAEA,
0xABF1,
0xACF5,
0xADFB,
0xAEFE,
0xAFFF,
0x2300,
0x2D0A,
0x20FF,
0xD290,
0x7300,
0x7754,
0xB340,
0xB480,
0xBA00,
0xBB00,
#if SENSOR_MIRROR_ENABLE == 0
0x1413,
#else
0x1410,		//0x1410,0x1413, mirror
#endif
#endif
SENSOR_INITVALUE_END};

const INT16U BF3703_initvalue[]={
#define SENSOR_LOW_FPS_ENABLE 0
#if SENSOR_BF3703_ENABLE==1
/*current ver: 2.00
history:
ver:1.00 (2011-10-23, copy from \\prj_dvr\JH1117\Code\第3版;)
ver:2.00 (加入低帧率可选项, 低帧率打开时实际帧率是16.6 - 17.1fps) (xiangyong 用示波器实测);
ver:3.00 (2012-11-19  逻辑分析仪测剑龙样机所得,　2012-11-21更新)
ver:4.00 (2014-09-13 BYD 以V2.00为基础, 效果参照 v3.00 修改)
note:
fps: 24.8-25.8fps (xiangyong 用示波器实测)
*/
//--------------------------------
0x1502,
0x0901,
#if SENSOR_MIRROR_ENABLE == 0
0x1e30,
#else
0x1e00,		//0x1e00,0x1e30, mirror
#endif
0x1300,
0x0113,
0x0225,
0x8c02,
0x8d64,
0x871A,
0x1307,
0x051f,
0x0660,
0x141F,
0x2703,
0x06e0,
0x1180,
0x2b20,
#if SENSOR_LOW_FPS_ENABLE == 1
0x9263,
0x9301,
#else
0x9240,
#endif
0x9d93,	//	//50Hz日光灯横波干扰;
0x9e7a,   //60Hz日光灯横波干扰;
0xeb30,
0xbb20,
0xf521,
0xe13c,
0x1603,
0x2ff6, //66, 画面局部偶有红色出现
0x3320,
0x3408,
0x3538,
0x6530,
0x6630,
0x3605,
0x37f6,
0x3846,
0x9bf6,
0x9c46,
0xbc01,
0xbdf6,
0xbe46,
0x8214,
0x8323,
0x9a23,
0x70ff,
0x722f,
0x732f,
0x7427,
0x7a06,	//06,	//4e, 暗态噪点
0x7b28,
0x841a,
0x8520,
0x8904,
0x8a08,
0x8634, //32,	//28, 改亮度 2010-12-11 11:38
0x96a6,
0x970c,
0x9818,
0x8055,
0x248c,	//7A,
0x257c,	//8A,
0x6900,
0x940a,
0x1F20,
0x2220,
0x2620,
0x5646,
0x61d3,
0x7948,
0x3b60,
0x3c20,
0x3980,
0x3fB8,
0x3980,
0x4058,
0x4154,
0x424e,
0x4344,
0x443e,
0x4539,
0x4635,
0x4731,
0x482e,
0x492b,
0x4b29,
0x4c27,
0x4e23,
0x4f20,
0x501e,
0x5119,
0x5221,
0x530c,
0x540f,
0x5776,
0x5885,
0x597b,
0x5aa6,
0x5b2b,
0x5D9d,
0x5C0e,
0x6020,
0xb0d0,
0xb1c8,
0xb2d8,
0xb39a,
0x6a81,
0x2366,
0xa003,
0x06e0,
0xa141,
0xa20e,
0xa325,
0xa40d,
0xa526,	//26, 2010-12-11 11:38
0xa608,
0xa77e,
0xa880,	//80, 2010-12-11 11:38
0xa91d,
0xaa20,
0xab1d,
0xac30,
0xadf0,
0xc816,
0xc922,
0xca15,
0xcb21,
0xaf00,
0xc516,
0xc600,
0xc722,
0xae83,
0xcc30,
0xcd70,
0xee4c,
#if SENSOR_LOW_FPS_ENABLE == 1
0x8e03,
0x8f97,
#else
0x8e02,
0x8fbc,  //最低帧率21 fps 
#endif
#endif
SENSOR_INITVALUE_END};

const INT16U BF3603_initvalue[]={
#if SENSOR_BF3603_ENABLE==1
/*current ver: 1.02
history:
ver:1.00 (2011-10-23, copy from \\prj_dvr\KeyDVR\Code\第八版_J;), fps: 24.9-25.2fps (xiangyong 用示波器实测)
ver:1.01 (2011-11-28, 修改:修正振荡问题, 增加亮度, 修改最低帧率), fps: 16.6-25.2fps (xiangyong 用示波器实测)
ver:1.02 (2011-12-02, 修改:修改最低帧率), fps: 19.9-25.2fps (xiangyong 用示波器实测)
note:
*/
//--------------------------------
0x1502,
0x0901,
#if SENSOR_MIRROR_ENABLE == 0
0x1e30,
#else
0x1e00,	//0x1e00, 0x1e30, mirror
#endif
0x1300,
0x0116,
0x0223,
0x8718,
0x8c02,
0x8d64,
0x1307,
0x2803,
0x2c03,
0x0020,
0x0e20,
0x0f20,
0x0519,
0x1419,
0x0619,
0x0819,
0x1fa0,
0x22a0,
0x26c8,	//08,	//default=0xc8 //修正振荡;
0x27c8,	//08,	//default=0xc8 //修正振荡;
0x2800,
0x2c00,
0x1180,
0x9d99,
0x9e7f,
0x2b00,
0x2F00,
0x1620,
0x8216,
0x8325,
0x8420,
0x8526,
0x9626,
0x970C,
0x6a01,
0x2366,
0x3611,
0x3720,
0x3835,
0x3568,	//38,	//最大增益, 增加亮度
0x6568,	//38,	//最大增益, 增加亮度
0x6668,	//38,	//最大增益, 增加亮度
0x2904,
0x707f,
0x724f,
0x732f,
0x7427,
0x750e,
0x2480,
0x2590,
0x6900,
0x76ff,
0x8634,	//30,	//最大增益, 增加亮度
0x8e02,	//08,	//限制最低帧率高8位
0x8Ffd,	//79,	//限制最低帧率低8位
0x8055,
0x8901,
0x8acb,
0x1f20,
0x2220,
0x9020,
0x911c,
0x9266,	//限制最高帧率低8位
0x9300,	//限制最高帧率高8位
0x3b60,
0x3c30,
0x3fd0,	//b0,	//gammera offset base;
0x3980,
0x4021,
0x412c,
0x4230,
0x431f,
0x441c,
0x4516,
0x4612,
0x470f,
0x480D,
0x490c,
0x4b0b,
0x4c09,
0x4e09,
0x4f08,
0x5007,
0x5134,
0x5294,
0x5380,
0x548b,
0x5732,
0x5887,
0x5988,
0x5A8c,
0x5B34,
0x5c2e,
0x5d17,
0xb0e4,
0xb1c0,
0xb2b0,
0xb386,
0x2100,
0x04bb,
0x5640,
0x6a01,
0x2366,
0xa09f,
0xa151,
0xa210,
0xa326,
0xa40b,
0xa526,
0xa606,
0xa798,
0xa882,
0xa947,
0xaa55,
0xab3f,
0xac42,
0xad45,
0xae4a,
0xaf2a,
0xc530,
0xc630,
0xc739,
0xc82a,
0xc931,
0xca3f,
0xcb41,
0xcc42,
0xcd4A,
0xce46,
0xcf4f,
0xd04b,
0xd15c,
#endif
SENSOR_INITVALUE_END};

const INT16U OV7680_initvalue[]={
#if SENSOR_OV7680_ENABLE==1
/*current ver: 1.00
history:
ver:1.00 (2011-12-17, copy from \\prj_dvr\KeyDVR\Code\第八版_Q;), fps: 14.6-22.0fps (xiangyong 用示波器实测)
note:
*/
//--------------------------------
0x14c2,
0x630b,
0x6401,
0x650c,
0x1201,
0x0c06,
0x829a,
0x2240,
0x2320,
0x3111,
0x4010,
0x4a42,
0x4b64,
0x6750,
0x6b00,
0x6c00,
0x6d00,
0x6e00,
0x424a,
0x4540,
0x3f46,
0x4820,
0x658c,
0x6602,
0x675c,
0x1200,
0x0c16,
0x829c,
0x3e30,
0x8107,
0x8F54,
0x9069,
0x910F,
0x9299,
0x9398,
0x940F,
0x950F,
0x96FF,
0x9700,
0x9810,
0x9920,
0xb790,
0xb88d,
0xb903,
0xba2b,
0xbb74,
0xbc9f,
0xbd5e,
0xbe98,
0xbf98,
0xc000,
0xc128,
0xc270,
0xc398,
0xc41a,
0xaf29,
0xa000,
0xa10a,
0xa21a,
0xa341,
0xa453,
0xa562,
0xa66e,
0xa77f,
0xa88a,
0xa993,
0xaaa5,
0xabb3,
0xacc7,
0xadd7,
0xaee1,
0x310f,
0x3804,
0x3440,
0x3640,
0x373b,
0x895c,
0x8a11,
0x8b12,
0x8d50,
0x96ff,
0x9700,
0xb206,
0xb303,
0xb403,
0xb503,
0xb603,
0xd502,
0xdb40,
0xdc40,
0x2468,
0x2558,
0x2683,
0x2dff,
0x2e05,
0x14a2,		//加大增益: 0x1492,
0x1100,
0x1590,		//0x1540, //2011-12-16 14:18 fps: 22*(2/3)fps - 22fps
0x2d00,
0x2e00,
0x4f99,
0x507f,
0x2123,
0x2ab0,		//0x2ab1,	//25 fps
0x3105,
0x32AF,
0x3330,
0x3420,
0x3500,
0x3620,
0x3720,
0x2F0C,
0x3010,
0xB7B0,
0xB89D,
0xB913,
0xBA16,
0xBB7B,
0xBC91,
0xBD1E,
0xBE98,
0xBF98,
0xC000,
0xC128,
0xC270,
0xC398,
0xC45A,
0xC59D,
0xC69A,
0xC703,
0xC82E,
0xC991,
0xCABF,
0xCB1E,
0xA00E,
0xA11A,
0xA231,
0xA35A,
0xA469,
0xA575,
0xA682,
0xA78D,
0xA898,
0xA9A0,
0xAAAE,
0xABBA,
0xACCD,
0xADDD,
0xAEEC,
0xAF20,
0x899C,
0x8A11,
0x8B12,
0x8C11,
0x8D52,
0x96F0,
0x9700,
0x9CF0,
0x9DF0,
0x9EF0,
0x8F5C,
0x905C,
0x9116,
0x982D,
0x9A4C,
0x92E0,
0x951F,
0x9928,
0x9B48,
0x93C8,
0x941F,
0xB206,
0xB303,
0xB405,
0xB504,
0xB603,
0xD502,
0xDB40,
0xDC40,
0x2490,
0x2580,
0x26B3,
0x2CBB,		//0x2C19,	//22fps
0x5A14,
0x5BE3,
0x5C70,
0x5D30,
0x807F,
0x8107,
0x0900,
#if SENSOR_MIRROR_ENABLE == 0
//0x0cD6,	//0x0c16, 0x0cD6, mirror	//xiangyong: 7680摄像头图像翻转180度时, 画面颜色会变成深黄色;
#else
#endif
#endif
SENSOR_INITVALUE_END};

//---------------------------------------------------------------------------
const INT8U sensor_slave_address[]={
0,		//#define SENSOR_CHIP_NO_READ				0
0,		//#define SENSOR_CHIP_DONT_RECOGNISE		1
0x42,	//#define SENSOR_CHIP_OV7725				2
0x42,	//#define SENSOR_CHIP_OV7670				3
0x80,	//#define SENSOR_CHIP_PAS6329				4
0x42,	//#define SENSOR_CHIP_GC0308				5
0xDC,	//#define SENSOR_CHIP_BF3703				6
0xDC,	//#define SENSOR_CHIP_BF3603				7
0x42,	//#define SENSOR_CHIP_OV7680				8
0xDC,	//#define SENSOR_CHIP_BF3A03				9
0x60,	//#define SENSOR_CHIP_OV2640				10
};

const INT32U sensor_type_name[]={
0x0000,	//#define SENSOR_CHIP_NO_READ				0
0xffff,	//#define SENSOR_CHIP_DONT_RECOGNISE		1
0x7725,	//#define SENSOR_CHIP_OV7725				2
0x7670,	//#define SENSOR_CHIP_OV7670				3
0x6329,	//#define SENSOR_CHIP_PAS6329				4
0x0308,	//#define SENSOR_CHIP_GC0308				5
0x3703,	//#define SENSOR_CHIP_BF3703				6
0x3603,	//#define SENSOR_CHIP_BF3603				7
0x7680,	//#define SENSOR_CHIP_OV7680				8
0x3A03,	//#define SENSOR_CHIP_BF3A03				9
0x2640,	//#define SENSOR_CHIP_OV2640				10
};

const INT16U *sensor_initvalue[]={
dummy_initvalue,	//#define SENSOR_CHIP_NO_READ				0
dummy_initvalue,	//#define SENSOR_CHIP_DONT_RECOGNISE		1
OV7725_initvalue,	//#define SENSOR_CHIP_OV7725				2
OV7670_initvalue,	//#define SENSOR_CHIP_OV7670				3
PAS6329_initvalue,	//#define SENSOR_CHIP_PAS6329				4
GC0308_initvalue,	//#define SENSOR_CHIP_GC0308				5
BF3703_initvalue,	//#define SENSOR_CHIP_BF3703				6
BF3603_initvalue,	//#define SENSOR_CHIP_BF3603				7
OV7680_initvalue,	//#define SENSOR_CHIP_OV7680				8
BF3A03_initvalue,	//#define SENSOR_CHIP_BF3703				9
OV2640_initvalue,	//#define SENSOR_CHIP_OV2640				10
};
