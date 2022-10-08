/**
  ******************************************************************************
  * @file    ${FILE_NAME}
  * @author  ${USER}
  * @date    ${TIME} ${DATE}
  * @brief   W25Q256JV 有 32M (byte)字节 的大小.
  *          有 131,072    个(page)页, 每个  页  256 (byte)字节.
  *          有 512       个(block)块, 每个  块  64K (byte)字节.
  *          有 8,192  个(sector)扇区, 每个 扇区  4K (byte)字节.
  *
  ******************************************************************************
  * @attention
  *
  * Copyright (c) ${YEAR} UTA.
  * All rights reserved.
  *
  * By(UTA personally owned)
  * https://blog.csdn.net/qq_51913390
  *
  ******************************************************************************
  */
/********************************End of Head************************************/


#ifndef BSP_W25Q256_H__
#define BSP_W25Q256_H__

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "gpio.h"
#include "spi.h"
#include "string.h"

#define spi_port    hspi5


//#define  sFLASH_ID                          0xEF3015      //W25X16
//#define  sFLASH_ID                          0xEF4015      //W25Q16
//#define  sFLASH_ID                          0XEF4017      //W25Q64
//#define  sFLASH_ID                          0XEF4018      //W25Q128
#define  sFLASH_ID                            0XEF4019      //W25Q256

/*  W25Q256JV容量  */
/*    256M-bit  */
/*      32M    */


/* 1 chip = 512 block = 4096 sectors = 65536 pages */
/* 1 sectors = 16 pages */
#define W25Q256FV_FLASH_SIZE                  0x2000000 /* 256 MBits => 32MBytes */
#define W25Q256FV_BLOCK_SIZE                  0x10000   /* 512 sectors of 64KBytes */
#define W25Q256FV_SECTOR_SIZE                 0x1000    /* 4096 subsectors of 4kBytes */
#define W25Q256FV_PAGE_SIZE                   0x100     /* 65536 pages of 256 bytes */

#define W25Q256FV_DUMMY_CYCLES_READ           4
#define W25Q256FV_DUMMY_CYCLES_READ_QUAD      10

#define W25Q256FV_BULK_ERASE_MAX_TIME         400000
#define W25Q256FV_BLOCK_ERASE_MAX_TIME        5000
#define W25Q256FV_SECTOR_ERASE_MAX_TIME       2000
#define W25Q256_TIMEOUT_VALUE                 1000


/**
  * @brief  W25Q256FV Commands
  */
/* Reset Operations */
#define RESET_ENABLE_CMD                     0x66
#define RESET_MEMORY_CMD                     0x99

#define ENTER_QPI_MODE_CMD                   0x38
#define EXIT_QPI_MODE_CMD                    0xFF

/* Identification Operations */
#define READ_ID_CMD                          0x90
#define DEVICE_ID_CMD                        0xAB
#define DUAL_READ_ID_CMD                     0x92
#define QUAD_READ_ID_CMD                     0x94
#define READ_JEDEC_ID_CMD                    0x9F

/* Read Operations */
#define READ_CMD                             0x13
#define READ_4BTYEADDR_CMD                   0x13
#define FAST_READ_CMD                        0x0B
#define DUAL_OUT_FAST_READ_CMD               0x3B
#define DUAL_INOUT_FAST_READ_CMD             0xBB
#define QUAD_OUT_FAST_READ_CMD               0x6B
#define QUAD_INOUT_FAST_READ_CMD             0xEB

/* Write Operations */
#define WRITE_ENABLE_CMD                     0x06
#define WRITE_DISABLE_CMD                    0x04

/* Register Operations */
#define READ_STATUS_REG1_CMD                 0x05
#define READ_STATUS_REG2_CMD                 0x35
#define READ_STATUS_REG3_CMD                 0x15

#define WRITE_STATUS_REG1_CMD                0x01
#define WRITE_STATUS_REG2_CMD                0x31
#define WRITE_STATUS_REG3_CMD                0x11


/* Program Operations */
#define PAGE_PROG_CMD                        0x12
#define QUAD_INPUT_PAGE_PROG_CMD             0x32


/* Erase Operations */
#define SECTOR_ERASE_CMD                     0x20
#define BLOCK_ERASE_CMD                      0xD8
#define CHIP_ERASE_CMD                       0x60

#define PROG_ERASE_RESUME_CMD                0x7A
#define PROG_ERASE_SUSPEND_CMD               0x75

/* Address Mode */
#define ENTER_4BYTE_ADDRESS_MODE             0xB7
#define EXIT_4BYTE_ADDRESS_MODE              0xE9

#define POWER_DOWN_CMD                       0xB9
#define RELEASE_POWER_DOWN_CMD               0xAB


/* Flag Status Register */
#define W25Q256FV_FSR_BUSY                    ((uint8_t)0x01)    /*!< busy */
#define W25Q256FV_FSR_WREN                    ((uint8_t)0x02)    /*!< write enable */
#define W25Q256FV_FSR_QE                      ((uint8_t)0x02)    /*!< quad enable */






#define W25Q256_OK                          ((uint8_t)0x00)
#define W25Q256_ERROR                       ((uint8_t)0x01)
#define W25Q256_BUSY                        ((uint8_t)0x02)
#define W25Q256_TIMEOUT				        ((uint8_t)0x03)

#define Dummy_Byte                          0xFF


#define W25Q256_Enable() 			        HAL_GPIO_WritePin(W25Q256_CE_GPIO_Port, W25Q256_CE_Pin, GPIO_PIN_RESET)
#define W25Q256_Disable() 		            HAL_GPIO_WritePin(W25Q256_CE_GPIO_Port, W25Q256_CE_Pin, GPIO_PIN_SET)


uint8_t W25Q256_Init(void);
uint8_t W25Q256_Reset(void);

uint8_t W25Q256_Read_SR1(void);
uint8_t W25Q256_Read_SR2(void);
uint8_t W25Q256_Read_SR3(void);

void W25Q256_Wait_Busy(void);


uint8_t W25Q256_Enter_4ByteAddressMode(void);
uint8_t W25Q256_Exit_4ByteAddressMode(void);



uint8_t W25Q256_WriteEnable(void);
uint8_t W25Q256_WriteDisable(void);

uint32_t W25Q256_JEDEC_ID(void);


uint8_t W25Q256_Read(uint8_t* pData, uint32_t Read_Addr, uint32_t Read_Size);
uint8_t W25Q256_Write_Page(uint8_t* pData, uint32_t Page_Addr, uint32_t Size);
uint8_t W25Q256_Write(uint8_t* pData, uint32_t Write_Addr, uint32_t Write_Size);
void W25Q256_Write_NoCheck(uint8_t* pBuffer,uint32_t Write_Addr,uint16_t Write_Size);
uint8_t W25Q256_Erase_Sector(uint32_t Sector_Addr);
uint8_t W25Q256_Erase_Block(uint32_t Block_Address);
uint8_t W25Q256_Erase_Chip(void);



void W25Q256_PowerDown(void);
void W25Q256_WAKEUP(void);


//测试
void W25Q256_Test(void);


#endif //BSP_W25Q256_H__
/********************************End of File************************************/