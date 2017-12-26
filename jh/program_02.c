#include "program_02.h"
#include "mini_dvr_api.h"
#include "program_01.h"
#include "program_04.h"
#include "application.h"
#include "stdio.h"


static volatile INT32U g_efuse_num;

#define C_EFUSE_ID	0x6F
const INT8U  R_CopyrightMSG_Buf[]= "版权声明: 本产品由深圳市乐信兴业科技有限公司设计,版权所有,仿冒必究. Product ID: 0x"; //82Bytes
//===============================================================================================
void save_COPYRIGHT_MESSAGE_to_disk(void)
{
	INT16S fd;
	INT32U addr;
	INT8U *p;
	fd = open("C:\\CopyrightMSG.txt", O_RDWR|O_TRUNC|O_CREAT);
	if(fd < 0)
		return;
	addr = (INT32U)gp_malloc(87+5+50);
	if(!addr)
	{
		close(fd);
		return;
	}
	p = (INT8U*)addr;
	p += 82;
	gp_strcpy((INT8S*)addr, (INT8S *)R_CopyrightMSG_Buf);
	sprintf((char *)p, (const char *)"%04X", g_efuse_num);
	write(fd, addr, 82+4);
	close(fd);
	gp_free((void*) addr);
}

void save_VERSION_NUMBER_to_disk(void)
{
	INT8U  *p;
	INT16S fd;
	INT32U addr;
	INT32U product_num;
	INT32U data_num;
	product_num = PRODUCT_NUM;
	data_num = PRODUCT_DATA;
	fd = open("C:\\Version.txt", O_RDWR|O_TRUNC|O_CREAT);
	if(fd < 0)
		return;
	addr = (INT32U)gp_malloc(32+5);
	if(!addr)
	{
		close(fd);
		return;
	}
	//gp_strcpy((INT8S*)addr, (INT8S *)"20120501,JH1117,v0.00 ");	//22 Bytes
	p = (INT8U*)addr;
	sprintf((char *)p, (const char *)"%08d,JH%04d,v0.00 ", data_num, product_num);
	p[17] = ((PROGRAM_VERSION_NUM%1000)/100) + '0';
	p[19] = ((PROGRAM_VERSION_NUM%100)/10) + '0';
	p[20] = ((PROGRAM_VERSION_NUM%10)/1) + '0';
	#if CUSTOMIZE_FOR_DIWEI==1
	p[21] = 'B';
	#endif
	write(fd, addr, 22);
	close(fd);
	gp_free((void*) addr);
}

void save_SENSOR_TYPE_to_disk(void)
{
	INT8U  *p;
	INT16S fd;
	INT32U addr;
	fd = open("C:\\sensor.txt", O_RDWR|O_TRUNC|O_CREAT);
	if(fd < 0)
		return;
	addr = (INT32U)gp_malloc(32+5);
	if(!addr)
	{
		close(fd);
		return;
	}
	p = (INT8U*)addr;
	sprintf((char *)p, (const char *)"%04X", sensor_type_name[g_sensor_id]);
	write(fd, addr, 4);
	close(fd);
	gp_free((void*) addr);
}

#define C_ID_REG_SHAM		0x01020304;
void read_efuse_num(void)
{
	INT32U customer_id_reg;
	INT32U t;
	t = C_ID_REG_SHAM;
	//customer_id_reg = 0xC0120010 + C_ID_REG_SHAM;
	customer_id_reg = 0xC012000C + C_ID_REG_SHAM;
	customer_id_reg -= t;
	//customer_id = *((volatile unsigned int *) 0xC012000C);
	g_efuse_num = *((volatile unsigned int *) (customer_id_reg));
}

void check_customer_id(void)
{
	INT32U customer_id;
	INT8U  *p;
	INT32U checksum=4567;
	INT8U  t;
	
	return;  
	
	#if DBG_MESSAGE == CUSTOM_ON
	return;
	#endif
	customer_id = g_efuse_num;
	customer_id >>= 8;
	customer_id &= 0x000000FF;
	if ( customer_id != C_EFUSE_ID ) F_power_Pin_Disable();
	else
	{
		//防止版权信息被修改:
		p = (INT8U*)R_CopyrightMSG_Buf;
		for (t=0; t<60; t++)
		{
			checksum += *p;
			p++;
		}
		if (checksum != (4567+0x00002d0e))	//0x00002d0e 是通过仿真板跑出来的;
		{
			F_power_Pin_Disable();
		}
	}
}
//===============================================================================================
void watchdog_clr(void)
{//WatchDog计时器清零:
	R_SYSTEM_WATCHDOG_CLEAR = 0xA005;
}

void watchdog_enable(void)
{//开启WatchDog:
//	R_SYSTEM_WATCHDOG_CTRL = 0x8000;	//2s
//	R_SYSTEM_WATCHDOG_CTRL = 0x8001;	//1s
//	R_SYSTEM_WATCHDOG_CTRL = 0x8002;	//0.5s
//	R_SYSTEM_WATCHDOG_CTRL = 0x8003;	//0.25s
//	R_SYSTEM_WATCHDOG_CTRL = 0x8006;	//0.125s
	R_SYSTEM_WATCHDOG_CTRL = 0x8007;	//62.5s
	watchdog_clr();
}

void watchdog_disable(void)
{//禁止WatchDog:
	R_SYSTEM_WATCHDOG_CTRL = 0;
}
