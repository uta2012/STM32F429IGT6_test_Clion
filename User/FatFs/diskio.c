/*-----------------------------------------------------------------------*/
/* Low level disk I/O module SKELETON for FatFs     (C)ChaN, 2019        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "ff.h"			/* Obtains integer types */
#include "diskio.h"		/* Declarations of disk functions */
#include "bsp_w25q256.h"


/* Definitions of physical drive number for each drive */
//#define DEV_RAM		0	/* Example: Map Ramdisk to physical drive 0 */
//#define DEV_MMC		1	/* Example: Map MMC/SD card to physical drive 1 */
//#define DEV_USB		2	/* Example: Map USB MSD to physical drive 2 */
#define DEV_FLASH		3	/* Example: Map FLASH to physical drive 3 */

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat;
	int result;

	switch (pdrv) {

#ifdef DEV_RAM
	case DEV_RAM :
		result = RAM_disk_status();

		// translate the reslut code here

		return stat;
#endif
#ifdef DEV_MMC
	case DEV_MMC :
		result = MMC_disk_status();

		// translate the reslut code here

		return stat;
#endif
#ifdef DEV_USB
	case DEV_USB :
		result = USB_disk_status();

		// translate the reslut code here

		return stat;
#endif
#ifdef DEV_FLASH
    case DEV_FLASH :
        //result = USB_disk_status();
        // translate the reslut code here
        if(sFLASH_ID != W25Q256_JEDEC_ID())
            stat = RES_ERROR;
        else
            stat = RES_OK;

        return stat;
#endif
    }

	return STA_NOINIT;

}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat;
	int result;

	switch (pdrv) {
#ifdef DEV_RAM
	case DEV_RAM :
		result = RAM_disk_initialize();

		// translate the reslut code here

		return stat;
#endif
#ifdef DEV_MMC
	case DEV_MMC :
		result = MMC_disk_initialize();

		// translate the reslut code here

		return stat;
#endif
#ifdef DEV_USB
	case DEV_USB :
		result = USB_disk_initialize();

		// translate the reslut code here

		return stat;
#endif
#ifdef DEV_FLASH
        case DEV_FLASH :
            // translate the reslut code here

            W25Q256_Erase_Chip();
            result = W25Q256_Init();
            HAL_Delay(500);
            W25Q256_WAKEUP();
            if(result == W25Q256_OK)
                stat = RES_OK;
            else
                stat = RES_ERROR;

            return stat;
#endif
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	LBA_t sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
)
{
	DRESULT res;
	int result;

	switch (pdrv) {
#ifdef DEV_RAM
	case DEV_RAM :
		// translate the arguments here

		result = RAM_disk_read(buff, sector, count);

		// translate the reslut code here

		return res;
#endif
#ifdef DEV_MMC
	case DEV_MMC :
		// translate the arguments here

		result = MMC_disk_read(buff, sector, count);

		// translate the reslut code here

		return res;
#endif
#ifdef DEV_USB
	case DEV_USB :
		// translate the arguments here

		result = USB_disk_read(buff, sector, count);

		// translate the reslut code here

		return res;
#endif
    case DEV_FLASH :
        // translate the arguments here
        result = W25Q256_Read(buff, sector*4096, count*4096);;
        // translate the reslut code here
            if(result == W25Q256_OK)
                res = RES_OK;
            else
                res = RES_ERROR;

        return res;
	}

	return RES_PARERR;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wincompatible-pointer-types-discards-qualifiers"
DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	LBA_t sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
)
{
	DRESULT res;
	int result;

	switch (pdrv) {
#ifdef DEV_RAM
	case DEV_RAM :
		// translate the arguments here

		result = RAM_disk_write(buff, sector, count);

		// translate the reslut code here

		return res;
#endif
#ifdef DEV_MMC
	case DEV_MMC :
		// translate the arguments here

		result = MMC_disk_write(buff, sector, count);

		// translate the reslut code here

		return res;
#endif
#ifdef DEV_USB
	case DEV_USB :
		// translate the arguments here

		result = USB_disk_write(buff, sector, count);

		// translate the reslut code here

		return res;
#endif
    case DEV_FLASH :
        // translate the arguments here
        //W25Q256_Erase_Sector(sector*4069);
        result = W25Q256_Write((BYTE*)buff, sector*4069, count*4096);
        // translate the reslut code here
         if(result == W25Q256_OK)
             res = RES_OK;
         else
             res = RES_ERROR;

        return res;
	}

	return RES_PARERR;
}


#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res;
	int result;

	switch (pdrv) {
#ifdef DEV_RAM
	case DEV_RAM :

		// Process of the command for the RAM drive

		return res;
#endif
#ifdef DEV_MMC
	case DEV_MMC :

		// Process of the command for the MMC/SD card

		return res;
#endif
#ifdef DEV_USB
	case DEV_USB :

		// Process of the command the USB drive

		return res;
#endif
    case DEV_FLASH :
        // Process of the command the USB drive
        switch(cmd)
        {
            case GET_SECTOR_COUNT:
                *(DWORD*)buff = 4096;
                break;
            case GET_SECTOR_SIZE:
                *(DWORD*)buff = 4096;
                break;
            case GET_BLOCK_SIZE:
                *(DWORD*)buff = 1;
                break;
        }
        
        res = RES_OK;

        return res;
	}

	return RES_PARERR;
}

DWORD get_fattime (void)
{

    return  (DWORD)((2022-1980) << 25) |    //???
            (DWORD)(1 << 21) |              //???
            (DWORD)(1 << 16) |              //???
            (DWORD)(1 << 11) |              //???
            (DWORD)(1 << 5) |               //???
            (DWORD)(1);                     //???
}



