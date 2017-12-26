#ifndef __JH_DEFINE_H__
#define __JH_DEFINE_H__
//===============================================================================================
#define PRODUCT_NUM						1801
#define PRODUCT_DATA					20171226
#define PROGRAM_VERSION_NUM				2//1	

//====================================================
//20160426 liuxi
#define MCU_GPDV6655B		0 //必须使用24M晶振
#define MCU_GPDV6655C		1 //不需要晶振
#define MCU_GPDV6655D		2 //不需要晶振, 3.3V可选由内部LDO提供

#define USED_MCU_NAME       MCU_GPDV6655D

#define LDO_ENABLE			1//选择主控内部LDO, 当使用外部LDO时, 实际上只是将内部LDO输出设置成最小了
//====================================================
#define CUSTOMIZE_FOR_DIWEI				0

#define VOLTATE_DETECT_FROM_PJ4054		1	//=1: USB模式中电池电压检测是通过读取充电IC PJ4054 的充电状态指示脚来检测的;
                                            //=0: USB模式中电池电压检测是通过 LINEIN 0 来检测的;

#define MP3_RECORD						0
//===============================================================================================
#endif 		// __JH_DEFINE_H__
