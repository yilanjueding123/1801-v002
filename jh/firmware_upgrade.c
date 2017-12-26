#include "avi_encoder_app.h"
#include "application.h"
#include "stdio.h"
#include "drv_l2_spi_flash.h"
#include "firmware_upgrade.h"	
#include "program_02.h"
#include "program_04.h"
#include "math.h"
#include "string.h"

extern void print_string(CHAR *fmt, ...);
void preess_key_exit(void);
void firmware_upgrade_init(void);
INT8U Write_UpgradeMsg(void);
INT8U Firmware_Backup(void);
INT8U DetectionAndRead_UpgradeMsg(void);
INT32U STRING_Convert_HEX(CHAR *str);

extern INT16S _findfirst(CHAR *name, struct f_info *f_info, INT16U attr);


volatile INT8U  F_file_exist;
volatile INT8U  F_backup;
volatile INT8U  F_detele;
volatile INT32U C_Total;
volatile INT8U  C_SPI_Size;


INT32U STRING_Convert_HEX(CHAR *str)
{
	INT32U sum = 0;
	INT8U len;
	INT16S i;

	len = strlen(str);
	for(i = 0; i<len; i++)
	{
		if (str[i]>='0'&&str[i]<='9') sum += (str[i]-'0')*(pow(16,len-1-i));
		else if(str[i]>='a'&&str[i]<='f') sum += (str[i]-'a'+10)*(pow(16,len-1-i));
		else if(str[i]>='A'&&str[i]<='F') sum += (str[i]-'A'+10)*(pow(16,len-1-i));
	}
	return sum; 
}

	

INT16U ap_umi_firmware_upgrade(void)
{
	INT32U C_UPGRADE_BUFFER_SIZE;	
	INT32U j, k, t,total_size, complete_size, read_cnt;
	INT32U *firmware_buffer;
	INT32U *firmware_spi;
	struct stat_t statetest;
	INT16S fd,nRet;
	INT8U retry=0;
	CHAR p[4];
	INT8U FileName_Path[7+4+9+4+2];
	INT32U CheckSum = 0;		//bin 计算出来的检验码
	INT32U checksum_addr = 0;
	INT32U *checksum_buffer;  
	INT8U  *q;
	INT16U checksum_cnt;      
	INT8U  checksum_temp[8+1];  
	INT32U CheckSum_BIN;        //文件名里的检验码

	
	struct f_info   file_info;
	AviEncPacker_t *pAviEncPacker;
	INT16U led_count = 0;
	INT8U  led_flag = 0;
	//=========================================================================
	sprintf((CHAR*)p,(const CHAR*)"%04d",PRODUCT_NUM);
	gp_strcpy((INT8S*)FileName_Path,(INT8S*)"C:\\JH_");
	gp_strcat((INT8S*)FileName_Path,(INT8S*)p);
	gp_strcat((INT8S*)FileName_Path,(INT8S*)"*.bin");
	
	nRet = _findfirst((CHAR*)FileName_Path, &file_info ,D_ALL); //查找bin文件
	if (nRet < 0) 
	{
		//DEBUG_MSG(DBG_PRINT("No upgrade firmware inside of the card!!\r\n"));  //没有检测到新的固件
		return STATUS_RETURN;
	}
	//DEBUG_MSG(DBG_PRINT("%s\r\n",file_info.f_name));  //检测到新的固件文件名
	strncpy((CHAR*)checksum_temp,(CHAR*)file_info.f_name+8,8);     //提取检验和
	checksum_temp[8] = NULL;                          //将 checksum_temp 最后一位置为 NULL, 否则会为乱码
    
	gp_strcpy((INT8S*)FileName_Path,(INT8S*)"C:\\");
	gp_strcat((INT8S*)FileName_Path,(INT8S*)file_info.f_name);
	
	//DEBUG_MSG(DBG_PRINT("The firmware directory: %s\r\n",FileName_Path)); //新固件目录及文件名
	fd = open((CHAR*)FileName_Path, O_RDONLY);
	if (fd < 0) {
		//DEBUG_MSG(DBG_PRINT("The firmware name is wrong!!\r\n"));  //没有检测到新的固件
		return	STATUS_RETURN;
	}
	//=========================================================================
	//删除已开启的任务并关闭watchdog.
	OSTaskDel(AVI_ENC_PRIORITY);	
	OSTaskDel(SCALER_PRIORITY);	
	OSTaskDel(JPEG_ENC_PRIORITY);	
	OSTaskDel(AUD_ENC_PRIORITY);	
	//=========================================================================
	//从已打开的文件中获取文件信息, 并将文件长度赋值给 total_size,并设置升级 buffer 大小
	if (fstat(fd, &statetest)) {
		close(fd);
		return STATUS_ERROR;
	}
	total_size = statetest.st_size;
	if (total_size >= 0x80000)  C_UPGRADE_BUFFER_SIZE = 0x100000;     //8Mbit 
	else if (total_size >= 0x40000)  C_UPGRADE_BUFFER_SIZE = 0x80000; //4Mbit
	else C_UPGRADE_BUFFER_SIZE = 0x40000;                             //2Mbit 

	//DEBUG_MSG(DBG_PRINT("total_size = 0x%X \r\n",total_size));
	//=========================================================================
	//释放缓存空间, 保证有足够的空间申请用来存放升级固件
	gp_free((void*) pAviEncPacker->file_write_buffer); //释放写卡缓存
	gp_free((void*) pAviEncPacker->index_write_buffer); //释放
	for(t=0; t<AVI_ENCODE_VIDEO_BUFFER_NO; t++) 
	{
		if(pAviEncVidPara->video_encode_addr[t]) gp_free((void*) pAviEncVidPara->video_encode_addr[t]);
	}

	checksum_buffer = (INT32U *) gp_malloc(256);//申请读TF卡校验用缓存
	if (!checksum_buffer)
	{
		//DEBUG_MSG(DBG_PRINT("checksum_buffer allocate firmware_buffer fail\r\n"));
		gp_free((void*)checksum_buffer);
		return STATUS_ERROR;
	}
	//=========================================================================
	complete_size = 0;
	DEBUG_MSG(DBG_PRINT("Firmware update start....\r\n"));
	SPI_Flash_init();  //初始化SFI FLASH
	OSTimeDly(1);
	//=========================================================================
	//计算新固件的检验码
    for(t=0; t<total_size/256; t++)
    {
		lseek(fd, checksum_addr, SEEK_SET);   //设置分段读取TF卡内固件的地址
		if (read(fd,(INT32U)checksum_buffer,256)<=0)
		{//读TF卡内的固件, 每次读 64K 
			gp_free((void*)checksum_buffer);
			//DEBUG_MSG(DBG_PRINT("Read the firmware failed!!!\r\n"));
			return STATUS_ERROR;
		}
		q = (INT8U*)checksum_buffer;
		for(checksum_cnt=0; checksum_cnt<256; checksum_cnt++)
		{
			CheckSum += *q++;
		}
		checksum_addr += 256;
    }
    gp_free((void*)checksum_buffer);								//释放不用的空间
	CheckSum_BIN = STRING_Convert_HEX((CHAR*)checksum_temp);		//将文件名里的的字符转成16进制的校验码
	if (CheckSum_BIN == CheckSum) 	//对比校验码
	{//文件名里的检验码与计算的相同
		//DEBUG_MSG(DBG_PRINT("CheckSum = 0x%08X \r\n", CheckSum));
	 	DEBUG_MSG(DBG_PRINT("CheckSum is right!!!"));
	}
	else 
	{//文件名里的检验码与计算的不同
		//DEBUG_MSG(DBG_PRINT("In the file name of the checksum = %X \r\n", CheckSum_BIN));
		//DEBUG_MSG(DBG_PRINT("Calculated the checksum= %X \r\n", CheckSum));
		DEBUG_MSG(DBG_PRINT("CheckSum is wrong!!!"));
		return STATUS_ERROR;
	}
	//=========================================================================
	firmware_buffer = (INT32U *) gp_malloc(C_UPGRADE_SPI_BLOCK_SIZE); //申请固件存放的内存
	if (!firmware_buffer) {
		//DEBUG_MSG(DBG_PRINT("firmware upgrade allocate firmware_buffer fail\r\n"));
		gp_free((void*)firmware_buffer);
		close(fd);
		return STATUS_ERROR;
	}
	firmware_spi = (INT32U *) gp_malloc(256);//申请读SPI缓存
	if (!firmware_spi)
	{
		//DEBUG_MSG(DBG_PRINT("firmware_spi allocate firmware_buffer fail\r\n"));
		gp_free((void*)firmware_spi);
		return STATUS_ERROR;
	}
	//=========================================================================
	while (complete_size < total_size) {
		INT32U buffer_left;
		buffer_left = (total_size - complete_size + (C_UPGRADE_SPI_BLOCK_SIZE-1)) & ~(C_UPGRADE_SPI_BLOCK_SIZE-1);  //SPI可写的最大值
		if (buffer_left > C_UPGRADE_BUFFER_SIZE) {
			buffer_left = C_UPGRADE_BUFFER_SIZE;
		}
		while (buffer_left && retry<C_UPGRADE_FAIL_RETRY) {
			complete_size &= ~(C_UPGRADE_SPI_BLOCK_SIZE-1); //已完成的大小, 以 64K 为基数
			//-----------------------------------------------------------------------
			lseek(fd, complete_size, SEEK_SET);   //设置分段读取TF卡内固件的地址
			if (read(fd,(INT32U)firmware_buffer,C_UPGRADE_SPI_BLOCK_SIZE)<=0)
			{//读TF卡内的固件, 每次读 64K 
				//DEBUG_MSG(DBG_PRINT("Read the firmware failed!!!\r\n"));
				return STATUS_ERROR;
			}
			//-----------------------------------------------------------------------
			if (SPI_Flash_erase_block(complete_size)) {					//块擦除SPI,每次 64 K 
				retry++;
				continue;
			}
			read_cnt = 0;
			for (j = C_UPGRADE_SPI_BLOCK_SIZE; j; j-=C_UPGRADE_SPI_WRITE_SIZE) {     //写SPI, 每次一页, 256byte
				if (SPI_Flash_write_page(complete_size, (INT8U *) (firmware_buffer + ((complete_size & (C_UPGRADE_SPI_BLOCK_SIZE-1))>>2)))) {
					//DEBUG_MSG(DBG_PRINT("Firmware writing failure!!!\r\n"));
					//DEBUG_MSG(DBG_PRINT("The address of fault location: 0x%08x - 0x%08x\r\n",complete_size,complete_size+256));
					break;
				}
				//------------------------------------------------------------------
				////固件对比, 从SPI中将更新后的固件读出来,与TF卡内的固件对比.
				if (++led_count > 12)
				{
					led_count = 0;
					led_flag ^= 1;
					if (led_flag) bonding_led_off();
					else bonding_led_on();
				}
				if (SPI_Flash_read_page(complete_size, (INT8U *)firmware_spi)) 
				{
					//DEBUG_MSG(DBG_PRINT("Read SPI failed!!!\r\n"));
					return STATUS_ERROR;
				}
				for(k=0; k<64; k++)
				{//每次比较 256 Byte 
					if (firmware_buffer[(read_cnt*64)+k] != firmware_spi[k])
					{
						//DEBUG_MSG(DBG_PRINT("firmware_buffer = 0x%08x     ",firmware_buffer[(read_cnt*64)+k]));
						//DEBUG_MSG(DBG_PRINT("firmware_spi = 0x%08x \r\n",firmware_spi[k]));
						DEBUG_MSG(DBG_PRINT("Check failed!!!\r\n"));
						return STATUS_ERROR;
					}
				}
				read_cnt++;
				//------------------------------------------------------------------
				complete_size += C_UPGRADE_SPI_WRITE_SIZE;  //写地址递增, 每次加 256
				if (complete_size >= total_size) 
				{
					break;       //如果写完的数据长度大于固件长度则退出
				}
			}
			if (complete_size >= total_size) 
			{//如果写完的数据长度大于固件长度则退出
				//DEBUG_MSG(DBG_PRINT("complete_size = 0x%08x. \r\n",complete_size));
				break; //按固件实际大小决定SPI写入
			}
			//-----------------------------------------------------------------------
			if (j == 0) {
				buffer_left -= C_UPGRADE_SPI_BLOCK_SIZE;    //如果64K(块)写完则从计算剩余容量
			} else {
				retry++;
			}
		}
		if (retry == C_UPGRADE_FAIL_RETRY) {
			//DEBUG_MSG(DBG_PRINT("Upgrade failed!!!\r\n"));
			break;
		}
	}
	//--------------------------------------------------------------------------------------
	gp_free((void*)firmware_buffer);
	gp_free((void*)firmware_spi);
	close(fd);
	DEBUG_MSG(DBG_PRINT("The firmware upgrade is complete!!!\r\n"));
    if (F_detele == 1)
    {//删除更新后的固件
		//DEBUG_MSG(DBG_PRINT("Delete the firmware!!!\r\n"));
    	unlink((CHAR*)FileName_Path);
    }
	//--------------------------------------------------------------------------------------
	C_Total++; //更新计数加1
    if (F_file_exist == 1)
    {//如果卡内有升级信息文件时, 升级完成后更新信息
		Write_UpgradeMsg();              //更新升级信息
    }
    else
    {//如果卡内只有升级固件时, 升级完成后自动删除
    	//DEBUG_MSG(DBG_PRINT("Delete the firmware!!!\r\n"));
    	unlink((CHAR*)FileName_Path);
    }
	return STATUS_TRUE;
}

void firmware_upgrade_server(void)
{
	INT8U  state_id;
	INT8U  temp_x,temp_y;
	INT32U t = 0;
	//--------------------------------------------------------------------------------------
	//初始化标志及变量  
	F_file_exist = 0;   //升级信息文件标志
	F_backup = 0;       //备份旧固件标志
	F_detele = 0;       //删除更新后的固件标志
	C_Total = 0;        //升级数量总数
	C_SPI_Size = 0;
	//--------------------------------------------------------------------------------------
	#if USED_WATCHDOG_FUNCTION
	watchdog_disable();   //如果有watchdog功能时请启用此行
	#endif
	//--------------------------------------------------------------------------------------
	DetectionAndRead_UpgradeMsg();  //检测或读取升级信息
	temp_x = Firmware_Backup();
	if ((temp_x == STATUS_TRUE)||(temp_x == STATUS_RETURN))
	{  //备份旧的固件
		temp_y = ap_umi_firmware_upgrade(); //固件升级
		if ((temp_x == STATUS_RETURN)&&((temp_y == STATUS_RETURN)))  
		{	
			//--------------------------------------------------------------------------------------
			#if USED_WATCHDOG_FUNCTION
			watchdog_enable();    //如果有watchdog功能时请启用此行
			#endif
			//--------------------------------------------------------------------------------------
			return;  //不备份也不升级就返回
		}
		if (temp_y != STATUS_ERROR) state_id = 0;       
		else state_id = 1;
	}
	else state_id = 1;
	//--------------------------------------------------------------------------------------
	#if USED_WATCHDOG_FUNCTION
	watchdog_enable();    //如果有watchdog功能时请启用此行
	#endif
	//--------------------------------------------------------------------------------------
	while(1)
	{
		switch(state_id)
		{//升级后的状态指示
			case 0:   //升级正常
				while(1)
				{
					OSTimeDly(1);
					#if USED_WATCHDOG_FUNCTION
					watchdog_clr();
					#endif
					bonding_led_on();
					if (++t > 3000)
					{
						#if PRODUCT_TYPE == MINI_DV_MODULE
						F_power_Pin_Disable();
						#endif
						while(1)
						{
							bonding_led_off();
						}
					}
					preess_key_exit(); //检测到按键复位
				}
				break;
			case 1: //升级失败
				t=0;
				while (1)
				{
					OSTimeDly(4);
					bonding_led_off();
					OSTimeDly(4);
					bonding_led_on();
					if (++t > 15)
					{
						bonding_led_off();
						OSTimeDly(10);
						#if PRODUCT_TYPE == MINI_DV_MODULE
						F_power_Pin_Disable();
						#endif
					}
				}
				break;
			case 2:
			    break;
		}
	}
}

void preess_key_exit(void)
{//按键退出关机
	INT8U t;
	static INT8U count = 0;
	
	t = gpio_read_io(EXIT_KEY_IO);
	#if KEY_TRIGGER_LEVEL == 0
	t ^= 1;
	#endif
	if (!t) return;
	if (++count >= 10)
	{
		bonding_led_on();
		OSTimeDly(50);
		#if PRODUCT_TYPE == MINI_DV_MODULE
		F_power_Pin_Disable();
		#endif
		while(1)
		{
			bonding_led_off();
		}
	} 
}

INT8U DetectionAndRead_UpgradeMsg(void)
{
	INT16S fd;
	INT32U Read_UpgradeMsg;
	INT8U  *pdata;
	INT32U  Total_temp = 0;
	INT8U   temp;

	fd = open("C:\\FirmwareUpgrade_Msg.ini", O_RDWR);
	if (fd < 0) 
	{
		F_file_exist = 0; 
		return STATUS_ERROR;
	}
	else
	{//检测到升级信息文件
		F_file_exist = 1;    //升级信息文件标志
		Read_UpgradeMsg = (INT32U)gp_malloc(128+6);
		read(fd,Read_UpgradeMsg,128+6);             //读升级信息
		pdata = (INT8U*)Read_UpgradeMsg;
		
		//读升级信息:是否备份?
		pdata += 27;
		temp = *pdata; 
		if (temp == 'Y') F_backup = 1;
		else if (temp == 'N') F_backup = 0;
		else  F_backup = 0;
		//读升级信息:是否删除更新后的固件?
		pdata += (6+1+27);
		temp = *pdata; 
		if (temp == 'Y') F_detele = 1;
		else if (temp == 'N') F_detele = 0;
		else  F_detele = 0;
		//读升级信息:设置SPI FLASH大小
		pdata += (6+1+27);
		temp = *pdata; 
		if (temp == '2') C_SPI_Size = 0;
		else if (temp == '4') C_SPI_Size = 1;
		else if (temp == '8') C_SPI_Size = 2;
		else  C_SPI_Size = 0;
		//读升级信息:升级数量
		pdata += (6+1+27);
		temp = *pdata++;
		temp -= 0x30;
		Total_temp += temp*10000;
		temp = *pdata++;
		temp -= 0x30;
		Total_temp += temp*1000;
		temp = *pdata++;
		temp -= 0x30;
		Total_temp += temp*100;
		temp = *pdata++;
		temp -= 0x30;
		Total_temp += temp*10;
		temp = *pdata++;
		temp -= 0x30;
		Total_temp += temp*1;
		
		if (Total_temp > 99999) C_Total = 0;  //如果数量大于99999, 则从0开始
		else C_Total = Total_temp;

		close(fd);
		gp_free((void*)Read_UpgradeMsg);
		
		return STATUS_TRUE;
	}
}

INT8U Write_UpgradeMsg(void)
{//写升级信息处理
	INT16S fd;
	INT32U upgrade_msg_buffer;
	INT8U  *p;
	
	upgrade_msg_buffer = (INT32U)gp_malloc(128+6);
	if (!upgrade_msg_buffer)
	{
		//DEBUG_MSG(DBG_PRINT("Can't upgrade information!!!\r\n"));
		gp_free((void*)upgrade_msg_buffer);
		return STATUS_ERROR;
	}
	p = (INT8U*)upgrade_msg_buffer;
	
	fd = open("C:\\FirmwareUpgrade_Msg.ini", O_RDWR|O_TRUNC|O_CREAT);
	gp_strcpy((INT8S*)upgrade_msg_buffer, (INT8S*)"Firmware backup    (Y/N) : N    ");
	if (F_backup == 1) p[27] = 'Y';
	else p[27] = 'N';
	p += 32;
	sprintf((CHAR*)p,(const CHAR*)"\r\n");
	gp_strcat((INT8S*)upgrade_msg_buffer, (INT8S*)"Delete the firmware(Y/N) : N    ");
	if (F_detele == 1) p[29] = 'Y';
	else p[29] = 'N';
	p += 32+2;
	sprintf((CHAR*)p,(const CHAR*)"\r\n");
	gp_strcat((INT8S*)upgrade_msg_buffer, (INT8S*)"Setting SPI FLASH size   : 2M   ");
	if (C_SPI_Size == 0) p[29] = '2';
	else if (C_SPI_Size == 1) p[29] = '4';
	else if (C_SPI_Size == 2) p[29] = '8';
	else p[29] = '2';
    p += 32+2;
	sprintf((CHAR*)p,(const CHAR*)"\r\n");
	gp_strcat((INT8S*)upgrade_msg_buffer, (INT8S*)"Upgrade quantity in total: ");
	p += 32+2-5;
	sprintf((char *)p, (const char *)"%05d ", C_Total);
	write(fd, upgrade_msg_buffer, 128+6);
	close(fd);
	gp_free((void*)upgrade_msg_buffer);
	
	//DEBUG_MSG(DBG_PRINT("Upgrade information update successfully!!!\r\n"));
	return STATUS_TRUE;
}

INT8U Firmware_Backup(void)
{//备份
	INT16S fd;
	INT32U backup_buffer;
	INT32U cnt;
	INT8U led_count = 0; 
	INT8U led_flag = 0;
	INT32U SPI_FLASH_SIZE;
	
	if (F_backup == 0) return STATUS_RETURN;
	
	DEBUG_MSG(DBG_PRINT("The firmware backup began....\r\n"));
	//设置备份的 SPI FLASH 大小
	if (C_SPI_Size == 0) SPI_FLASH_SIZE = 0x40000;         //2Mbit  
	else if (C_SPI_Size == 1) SPI_FLASH_SIZE = 0x80000;    //4Mbit
	else if (C_SPI_Size == 2) SPI_FLASH_SIZE = 0x100000;   //8Mbit
	else SPI_FLASH_SIZE = 0x40000;                         //2Mbit
	
	SPI_Flash_init();  //初始化SFI FLASH
	backup_buffer = (INT32U)gp_malloc(256);
	if (!backup_buffer)
	{
		//DEBUG_MSG(DBG_PRINT("The firmware backup failed!!!\r\n")); 
		gp_free((void*)backup_buffer);
		return STATUS_ERROR;
	}
	
	fd = open("C:\\Firmware_Backup.bin", O_RDWR|O_TRUNC|O_CREAT);
	for(cnt = 0; cnt<SPI_FLASH_SIZE/256; cnt++)
	{//备份固件
		if (SPI_Flash_read_page(cnt*256, (INT8U *)backup_buffer)) 
		{//读 SPI FLASH, 每次 256 Byte
			gp_free((void*)backup_buffer);
			DEBUG_MSG(DBG_PRINT("Read SPI failed!!!\r\n"));
			return STATUS_ERROR;
		}
		if (write(fd,(INT32U)backup_buffer,256)<= 0)
		{//写 TF 卡, 每次 256 Byte
			gp_free((void*)backup_buffer);
			//DEBUG_MSG(DBG_PRINT("Write TF Card failed!!!\r\n"));
			return STATUS_ERROR;
		}
		if (++led_count > 6)
		{
			led_count = 0;
			led_flag ^= 1;
			if (led_flag) bonding_led_off();
			else bonding_led_on();
		}
	}	
	close(fd);
	gp_free((void*)backup_buffer);
	DEBUG_MSG(DBG_PRINT("The firmware backup success!!!\r\n"));
	return STATUS_TRUE;
}