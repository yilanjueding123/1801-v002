#ifndef __JH_DEFINE_H__
#define __JH_DEFINE_H__
//===============================================================================================
#define PRODUCT_NUM						1801
#define PRODUCT_DATA					20171226
#define PROGRAM_VERSION_NUM				2//1	

//====================================================
//20160426 liuxi
#define MCU_GPDV6655B		0 //����ʹ��24M����
#define MCU_GPDV6655C		1 //����Ҫ����
#define MCU_GPDV6655D		2 //����Ҫ����, 3.3V��ѡ���ڲ�LDO�ṩ

#define USED_MCU_NAME       MCU_GPDV6655D

#define LDO_ENABLE			1//ѡ�������ڲ�LDO, ��ʹ���ⲿLDOʱ, ʵ����ֻ�ǽ��ڲ�LDO������ó���С��
//====================================================
#define CUSTOMIZE_FOR_DIWEI				0

#define VOLTATE_DETECT_FROM_PJ4054		1	//=1: USBģʽ�е�ص�ѹ�����ͨ����ȡ���IC PJ4054 �ĳ��״ָ̬ʾ��������;
                                            //=0: USBģʽ�е�ص�ѹ�����ͨ�� LINEIN 0 ������;

#define MP3_RECORD						0
//===============================================================================================
#endif 		// __JH_DEFINE_H__
