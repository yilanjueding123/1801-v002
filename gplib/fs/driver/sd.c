#define CREAT_DRIVERLAYER_STRUCT

#include "fsystem.h"

//=== This is for code configuration DON'T REMOVE or MODIFY it ===//
#if (defined SD_EN) && (SD_EN == 1)                               //
//================================================================//

#define SD_RW_RETRY_COUNT		3


#define C_DATA3_PIN       38//IOC[6]
#define C_DATA1_PIN       40//IOC[8]
#define C_DATA2_PIN       41//IOC[9]



INT32S SD_Initial(void)
{
//    gpio_write_io(C_DATA3_PIN, 1);
//    gpio_write_io(C_DATA1_PIN, 1);
//    gpio_write_io(C_DATA2_PIN, 1);
	return drvl2_sd_init();
}

INT32S SD_Uninitial(void)
{
     drvl2_sd_card_remove();
	 return 0;
}

void SD_GetDrvInfo(struct DrvInfo* info)
{
	info->nSectors = drvl2_sd_sector_number_get();
	info->nBytesPerSector = 512;
}

//read/write speed test function
INT32S SD_ReadSector(INT32U blkno, INT32U blkcnt, INT32U buf)
{
	INT32S	ret;
	INT32S	i;

    if (fs_sd_ms_plug_out_flag_get()==1) {return 0xFFFFFFFF;}
	for(i = 0; i < SD_RW_RETRY_COUNT; i++)
	{
		ret = drvl2_sd_read(blkno, (INT32U *) buf, blkcnt);
		if(ret == 0)
		{
			break;
		}
	}
	return ret;
}

INT32S SD_WriteSector(INT32U blkno, INT32U blkcnt, INT32U buf)
{
	INT32S	ret;
	INT32S	i;

    if (fs_sd_ms_plug_out_flag_get()==1) {return 0xFFFFFFFF;}
	for(i = 0; i < SD_RW_RETRY_COUNT; i++)
	{
		ret = drvl2_sd_write(blkno, (INT32U *) buf, blkcnt);
		if(ret == 0)
		{
			break;
		}
	}
	return ret;
}

INT32S SD_Flush(void)
{
	return 0;
}

struct Drv_FileSystem FS_SD_driver = {
	"SD",
	DEVICE_READ_ALLOW|DEVICE_WRITE_ALLOW,
	SD_Initial,
	SD_Uninitial,
	SD_GetDrvInfo,
	SD_ReadSector,
	SD_WriteSector,
	SD_Flush,
};

//=== This is for code configuration DON'T REMOVE or MODIFY it ===//
#endif //(defined SD_EN) && (SD_EN == 1)                          //
//================================================================//