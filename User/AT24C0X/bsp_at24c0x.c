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
  @verbatim

   None

  ******************************************************************************
  */
/********************************End of Head************************************/

/* Includes ------------------------------------------------------------------*/

#include "bsp_at24c0x.h"




/**
  * @brief   写一个字节到I2C EEPROM中
  * @param
  * 	@arg hi2c:I2C句柄
  *		@arg pBuffer:缓冲区指针
  *		@arg WriteAddr:写EEPROM地址
  *		@arg NumByteToWrite:写的字节数
  * @retval  无
  */
void I2C_EEPROM_Write_Buffer(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t* pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite)
{
    uint8_t Addr = WriteAddr % EEPROM_PAGESIZE;//判断起始地址是否在页的开头，也就是页的倍数，0就是再开头，不是0就是不在开头
    uint8_t Num_Of_Page = NumByteToWrite / EEPROM_PAGESIZE;//要写多少整数页
    uint8_t Num_Of_Byte = NumByteToWrite % EEPROM_PAGESIZE;//不足一页的字节数


    //起始地址在页的开头
    if(Addr == 0)
    {
        //如果写不满一页
        if(Num_Of_Page == 0)
        {
            //直接写 Num_Of_Byte
            I2C_EEPROM_Write_Page(hi2c, DevAddress, pBuffer, WriteAddr, Num_Of_Byte);
            return;
        }
            //大于一页
        else
        {
            //一页一页写
            while(Num_Of_Page--)
            {
                I2C_EEPROM_Write_Page(hi2c, DevAddress, pBuffer, WriteAddr, EEPROM_PAGESIZE);
                //写完一页之后指针跟着移动EEPROM_PAGESIZE
                pBuffer	+=	EEPROM_PAGESIZE;
                WriteAddr	+=	EEPROM_PAGESIZE;
            }
            //完整的页写完之后再写剩下的
            if(Num_Of_Byte != 0)
            {
                I2C_EEPROM_Write_Page(hi2c, DevAddress, pBuffer, WriteAddr, Num_Of_Byte);
                return;
            }
        }
    }
        //起始地址不在在页的开头
    else
    {
        //先计算开头有多少个空余的，先把空余的填满
        uint8_t count = EEPROM_PAGESIZE - Addr;

        //如果写入内容大小小于count
        if(NumByteToWrite < count)
        {
            I2C_EEPROM_Write_Page(hi2c, DevAddress, pBuffer, WriteAddr, NumByteToWrite);
            return;
        }
        else
        {

            //填满空余的
            I2C_EEPROM_Write_Page(hi2c, DevAddress, pBuffer, WriteAddr, count);
            //指针跟着移动
            pBuffer	+=	count;
            WriteAddr	=+	count;

            //然后要重新算一遍NumByteToWrite，Num_Of_Page，Num_Of_Byte
            NumByteToWrite -= count;
            Num_Of_Page = NumByteToWrite / EEPROM_PAGESIZE;
            Num_Of_Byte =NumByteToWrite % EEPROM_PAGESIZE;

            //重复上面 一页一页写满，然后写Num_Of_Byte的操作
            //一页一页写
            while(Num_Of_Page--)
            {
                I2C_EEPROM_Write_Page(hi2c, DevAddress, pBuffer, WriteAddr, EEPROM_PAGESIZE);

                //写完一页之后指针跟着移动EEPROM_PAGESIZE
                pBuffer	+=	EEPROM_PAGESIZE;
                WriteAddr	+=	EEPROM_PAGESIZE;
            }
            //完整的页写完之后再写剩下的
            if(Num_Of_Byte != 0)
            {
                I2C_EEPROM_Write_Page(hi2c, DevAddress, pBuffer, WriteAddr, Num_Of_Byte);
                return;
            }
        }
    }
}

//I2C_MEMADD_SIZE_8BIT
//HAL_I2C_GetState(I2C_HandleTypeDef *hi2c)
//HAL_I2C_IsDeviceReady(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint32_t Trials, uint32_t Timeout)
//HAL_I2C_Mem_Write(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout)
//HAL_I2C_Mem_Read(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout)


/**
  * @brief   写一个字节到I2C EEPROM中
  * @param
  * 	@arg hi2c:I2C句柄
  * 	@arg DevAddress:设备地址
  *		@arg pBuffer:缓冲区指针
  *		@arg WriteAddr:写地址
  * @retval  无
  */
HAL_StatusTypeDef I2C_EEPROM_Write_Byte(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t* pBuffer, uint8_t WriteAddr)
{
    HAL_StatusTypeDef status = HAL_OK;

    status = HAL_I2C_Mem_Write(hi2c, DevAddress, (uint16_t)WriteAddr, I2C_MEMADD_SIZE_8BIT, pBuffer, 1, 100);

    /* Check the communication status */
    if(status != HAL_OK)
    {
        /* Execute user timeout callback */
        //I2Cx_Error(Addr);
    }
    while (HAL_I2C_GetState(hi2c) != HAL_I2C_STATE_READY)
    {

    }

    /* Check if the EEPROM is ready for a new operation */
    while (HAL_I2C_IsDeviceReady(hi2c, DevAddress, EEPROM_MAX_TRIALS, I2Cx_TIMEOUT_MAX) == HAL_TIMEOUT);

    /* Wait for the end of the transfer */
    while (HAL_I2C_GetState(hi2c) != HAL_I2C_STATE_READY)
    {

    }
    return status;
}


/**
  * @brief   在EEPROM的一个写循环中可以写多个字节，但一次写入的字节数
  *          不能超过EEPROM页的大小，AT24C02每页有8个字节
  * @param
  * 	@arg hi2c:I2C句柄
  *		@arg pBuffer:缓冲区指针
  *		@arg WriteAddr:写地址
  *     @arg NumByteToWrite:写的字节数
  * @retval  无
  */
HAL_StatusTypeDef I2C_EEPROM_Write_Page(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t* pBuffer, uint8_t WriteAddr, uint8_t NumByteToWrite)
{
    HAL_StatusTypeDef status = HAL_OK;
    /* Write EEPROM_PAGESIZE */
    status=HAL_I2C_Mem_Write(hi2c, DevAddress, WriteAddr, I2C_MEMADD_SIZE_8BIT, (uint8_t*)(pBuffer),NumByteToWrite, 100);

    while (HAL_I2C_GetState(hi2c) != HAL_I2C_STATE_READY)
    {

    }

    /* Check if the EEPROM is ready for a new operation */
    while (HAL_I2C_IsDeviceReady(hi2c, DevAddress, EEPROM_MAX_TRIALS, I2Cx_TIMEOUT_MAX) == HAL_TIMEOUT);

    /* Wait for the end of the transfer */
    while (HAL_I2C_GetState(hi2c) != HAL_I2C_STATE_READY)
    {

    }
    return status;
}

/**
  * @brief   从EEPROM里面读取一块数据
  * @param
  * 	@arg hi2c:I2C句柄
  *		@arg pBuffer:存放从EEPROM读取的数据的缓冲区指针
  *		@arg WriteAddr:接收数据的EEPROM的地址
  *     @arg NumByteToWrite:要从EEPROM读取的字节数
  * @retval  无
  */
HAL_StatusTypeDef I2C_EEPROM_Read_Buffer(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t* pBuffer, uint8_t ReadAddr, uint16_t NumByteToRead)
{
    HAL_StatusTypeDef status = HAL_OK;

    status=HAL_I2C_Mem_Read(hi2c, DevAddress, ReadAddr, I2C_MEMADD_SIZE_8BIT, (uint8_t *)pBuffer, NumByteToRead,1000);

    return status;
}








/********************************End of File************************************/