#ifndef __FIRMWARE_UPGRADE_H__
#define __FIRMWARE_UPGRADE_H__

#include "project.h"
#include "jh_define.h"
//=======================================================
extern void firmware_upgrade_server(void);

#define USED_WATCHDOG_FUNCTION	USED  //�Ƿ��� watchdog ����; USED -> ��, UNUSED -> ��,

#define RC_PLANE_MODULE			0 //ң�طɻ�ģ��
#define MINI_DV_MODULE			1 //Mini DV
#define PRODUCT_TYPE			MINI_DV_MODULE  //��Ʒ����

#define EXIT_KEY_IO       		KEY_1_PIN    //������ɺ󰴼��˳�, ����IO
#define KEY_TRIGGER_LEVEL       KEY_1_TRIGGER_LEVEL              //����ǰζ�����,һ��ѡ��ػ��ĵ�ƽ״̬

#define STATUS_TRUE		  0
#define STATUS_ERROR	  1
#define STATUS_RETURN	  2

//#define C_UPGRADE_BUFFER_SIZE		0x40000			// Must be multiple of 64K
//#define C_UPGRADE_NAND_PAGE_SIZE	0x800			// Must be multiple of 2K
#define C_UPGRADE_SPI_BLOCK_SIZE	0x10000			// 64K
#define C_UPGRADE_SPI_WRITE_SIZE	0x100			// 256 bytes
#define C_UPGRADE_FAIL_RETRY		20				// Must larger than 0
//=======================================================
#endif