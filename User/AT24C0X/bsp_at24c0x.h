/**
  ******************************************************************************
  * @file    ${FILE_NAME}
  * @author  ${USER}
  * @date    ${TIME} ${DATE}
  * @brief   None.
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



#ifndef __BSP_AT24C0X_H__
#define __BSP_AT24C0X_H__

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "i2c.h"


/* 设备地址 */
#define EEPROM_ADDRESS	0xA0


/* AT24C01/02每页有8个字节 */
#define EEPROM_PAGESIZE		8
/* AT24C04/08A/16A每页有16个字节 */
//#define EEPROM_PAGESIZE	16

/* Maximum number of trials for HAL_I2C_IsDeviceReady() function */
#define EEPROM_MAX_TRIALS	300

/*等待超时时间*/
#define I2CT_FLAG_TIMEOUT	((uint32_t)0x1000)
#define I2CT_LONG_TIMEOUT	((uint32_t)(10 * I2CT_FLAG_TIMEOUT))
#define I2Cx_TIMEOUT_MAX	300


void I2C_EEPROM_Write_Buffer(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t* pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite);
HAL_StatusTypeDef I2C_EEPROM_Write_Byte(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t* pBuffer, uint8_t WriteAddr);
HAL_StatusTypeDef I2C_EEPROM_Write_Page(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t* pBuffer, uint8_t WriteAddr, uint8_t NumByteToWrite);
HAL_StatusTypeDef I2C_EEPROM_Read_Buffer(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t* pBuffer, uint8_t ReadAddr, uint16_t NumByteToRead);




#endif //__BSP_AT24C0X_H__
/********************************End of File************************************/