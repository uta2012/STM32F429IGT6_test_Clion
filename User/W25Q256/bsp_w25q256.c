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

#include "bsp_w25q256.h"


/**
 * @brief  Initializes the W25Q256FV interface.
 * @retval None
 */
uint8_t W25Q256_Init(void)
{
    uint8_t res = 0;
    res = W25Q256_Enter4ByteAddressMode();
    if(res == W25Q256_OK)
        return W25Q256_OK;
    else
        return W25Q256_ERROR;
}



/**
 * @brief  Initializes the W25Q256FV interface.
 * @retval None
 */
uint8_t W25Q256_Reset(void)
{
    uint8_t cmd[] = {RESET_ENABLE_CMD, RESET_MEMORY_CMD};
    W25Q256_Enable();
    HAL_SPI_Transmit(&spi_port, cmd, 2, W25Q256_TIMEOUT_VALUE);
    W25Q256_Disable();

    return W25Q256_OK;
}


/**
  * @brief  Reads current status of the W25Q256FV.
  * @retval W25Q256FV memory status
  */
uint8_t W25Q256_GetStatus(void)
{
    uint8_t cmd[] = {READ_STATUS_REG1_CMD};
    uint8_t status;

    W25Q256_Enable();
    HAL_SPI_Transmit(&spi_port, cmd, 1, W25Q256_TIMEOUT_VALUE);
    HAL_SPI_Receive(&spi_port, &status, 1, W25Q256_TIMEOUT_VALUE);
    W25Q256_Disable();
    if((status & W25Q256FV_FSR_BUSY) != 0)
    {
        return W25Q256_BUSY;
    }
    else
    {
        return W25Q256_OK;
    }
}


/**
  * @brief  This function send a Enter 4-Byte Address Mode (B7h)
  * @retval None
  */
uint8_t W25Q256_Enter4ByteAddressMode(void)
{
    uint8_t cmd[] = {READ_STATUS_REG3_CMD };
    uint8_t res = 0;

    W25Q256_Enable();
    HAL_SPI_Transmit(&spi_port, cmd, 1, W25Q256_TIMEOUT_VALUE);
    HAL_SPI_Receive(&spi_port, &res, 1, W25Q256_TIMEOUT_VALUE);
    W25Q256_Disable();


    if((res & 0x01) == 0)
    {
        cmd[0] = ENTER_4BYTE_ADDRESS_MODE;
        W25Q256_Enable();
        HAL_SPI_Transmit(&spi_port, cmd, 1, W25Q256_TIMEOUT_VALUE);
        W25Q256_Disable();
    }

    return W25Q256_OK;
}

/**
  * @brief  This function send a Exit 4-Byte Address Mode (E9h)
  * @retval None
  */
uint8_t W25Q256_Exit4ByteAddressMode(void)
{

    uint8_t cmd[] = {READ_STATUS_REG3_CMD };
    uint8_t res = 0;

    W25Q256_Enable();
    HAL_SPI_Transmit(&spi_port, cmd, 1, W25Q256_TIMEOUT_VALUE);
    HAL_SPI_Receive(&spi_port, &res, 1, W25Q256_TIMEOUT_VALUE);
    W25Q256_Disable();

    if((res & 0x01) == 1)
    {
        cmd[0] = ENTER_4BYTE_ADDRESS_MODE;
        W25Q256_Enable();
        HAL_SPI_Transmit(&spi_port, cmd, 1, W25Q256_TIMEOUT_VALUE);
        W25Q256_Disable();
    }


    while(W25Q256_GetStatus() == W25Q256_BUSY);

    return W25Q256_OK;
}


/**
  * @brief  This function send a Write Enable and wait it is effective.
  * @retval None
  */
uint8_t W25Q256_WriteEnable(void)
{
    uint8_t cmd[] = {WRITE_ENABLE_CMD};

    W25Q256_Enable();
    HAL_SPI_Transmit(&spi_port, cmd, 1, W25Q256_TIMEOUT_VALUE);
    W25Q256_Disable();

    while(W25Q256_GetStatus() == W25Q256_BUSY);

    return W25Q256_OK;
}






uint32_t W25Q256_JEDEC_ID(void)
{
    uint8_t cmd[] = {READ_JEDEC_ID_CMD};

    //jedc_id[0]:ManufactureID  jedc_id[1]:Memory Type ID  jedc_id[2]:Capacity ID
    uint8_t jedc_id_temp[3];
    uint32_t jedc_id = 0;

    W25Q256_Enable();
    HAL_SPI_Transmit(&spi_port, cmd, 1, W25Q256_TIMEOUT_VALUE);

    HAL_SPI_Receive(&spi_port, jedc_id_temp, 3, W25Q256_TIMEOUT_VALUE);
    W25Q256_Disable();

    while(W25Q256_GetStatus() == W25Q256_BUSY);

    jedc_id = (jedc_id_temp[0] << 16) | (jedc_id_temp[1] << 8) | jedc_id_temp[2];

    return jedc_id;
}





/**
  * @brief  Reads an amount of data from the QSPI memory.
  * @param  pData: Pointer to data to be read
  * @param  ReadAddr: Read start address
  * @param  Size: Size of data to read
  * @retval QSPI memory status
  */
uint8_t W25Q256_Read(uint8_t* pData, uint32_t ReadAddr, uint32_t Size)
{
    uint8_t cmd[5];

    /* Configure the command */
    cmd[0] = READ_CMD;
    cmd[1] = (uint8_t)((ReadAddr & 0xFF000000) >> 24);
    cmd[2] = (uint8_t)((ReadAddr & 0xFF0000) >> 16);
    cmd[3] = (uint8_t)((ReadAddr & 0xFF00) >> 8);
    cmd[4] = (uint8_t)((ReadAddr & 0xFF) >> 0);

    W25Q256_Enable();
    HAL_SPI_Transmit(&spi_port, cmd, 5, W25Q256_TIMEOUT_VALUE);

    HAL_SPI_Receive(&spi_port, pData, Size, W25Q256_TIMEOUT_VALUE);

    W25Q256_Disable();

    while(W25Q256_GetStatus() == W25Q256_BUSY);

    return W25Q256_OK;

}




/**
  * @brief  页写入(配合Sector Write使用)
  * @param  pData: Pointer to data to be read
  * @param  WriteAddr:
  * @param  Size:
  * @retval QSPI memory status
  */
uint8_t W25Q256_Page_Write(uint8_t* pData, uint32_t WriteAddr, uint32_t Size)
{
    //地址大于内存
    if(WriteAddr >= W25Q256FV_FLASH_SIZE)
        return W25Q256_ERROR;

    if(Size >= W25Q256FV_PAGE_SIZE)
        return W25Q256_ERROR;


    uint8_t cmd[5];
    cmd[0] = PAGE_PROG_CMD;
    cmd[1] = (uint8_t)((WriteAddr & 0xFF000000) >> 24);
    cmd[2] = (uint8_t)((WriteAddr & 0xFF0000) >> 16);
    cmd[3] = (uint8_t)((WriteAddr & 0xFF00) >> 8);
    cmd[4] = (uint8_t)((WriteAddr & 0xFF));

    W25Q256_WriteEnable();
    /* Enable write operations */
    W25Q256_Enable();
    /* Send the command */
    HAL_SPI_Transmit(&spi_port, cmd, 5, W25Q256_TIMEOUT_VALUE);
    /* Transmission of the data */
    HAL_SPI_Transmit(&spi_port, pData, Size, W25Q256_TIMEOUT_VALUE);
    W25Q256_Disable();
    while(W25Q256_GetStatus() == W25Q256_BUSY);
    return W25Q256_OK;
}





/**
  * @brief  按照扇区写入数据
  * @param  pData: Pointer to data to be written
  * @param  WriteAddr:
  * @param  Size:
  * @retval QSPI memory status
  */
uint8_t W25Q256_Write(uint8_t* pData, uint32_t WriteAddr, uint32_t Size)
{
    uint16_t page = Size / W25Q256FV_PAGE_SIZE;
    uint8_t remain = Size % W25Q256FV_PAGE_SIZE;

    //如果地址对齐
    if((WriteAddr % W25Q256FV_PAGE_SIZE) == 0)
    {
        //如果数据小于一页
        if(page == 0)
        {
            W25Q256_Page_Write(pData, WriteAddr, Size);
        }
        else
        {
            while(page--)
            {
                W25Q256_Page_Write(pData, WriteAddr, W25Q256FV_PAGE_SIZE);
                pData += W25Q256FV_PAGE_SIZE;
                WriteAddr += W25Q256FV_PAGE_SIZE;
            }
            W25Q256_Page_Write(pData, WriteAddr, remain);
        }

    }
    else
    {
        uint32_t new_addr = 0;
        while(new_addr < WriteAddr)
            new_addr += W25Q256FV_PAGE_SIZE;


        if((new_addr-WriteAddr) > Size)
            W25Q256_Page_Write(pData, WriteAddr, Size);
        else
        {
            //先把开头内存的写了
            W25Q256_Page_Write(pData, WriteAddr, (new_addr-WriteAddr));
            pData += (new_addr-WriteAddr);

            page = (Size - (new_addr-WriteAddr)) / W25Q256FV_PAGE_SIZE;
            remain = (Size - (new_addr-WriteAddr)) % W25Q256FV_PAGE_SIZE;

            while(page--)
            {
                W25Q256_Page_Write(pData, new_addr, W25Q256FV_PAGE_SIZE);
                pData += W25Q256FV_PAGE_SIZE;
                WriteAddr += W25Q256FV_PAGE_SIZE;
            }
            W25Q256_Page_Write(pData, WriteAddr, remain);
        }
    }

    while(W25Q256_GetStatus() == W25Q256_BUSY);

    return W25Q256_OK;
}


uint8_t W25Q256_Erase_Sector(uint32_t Address)
{
    uint8_t cmd[5];

    /* Check parameter */
    if(Address >= W25Q256FV_FLASH_SIZE)
        return W25Q256_ERROR;

    cmd[0] = SECTOR_ERASE_CMD;
    cmd[1] = (uint8_t)((Address & 0xFF000000) >> 24);
    cmd[2] = (uint8_t)((Address & 0xFF0000) >> 16);
    cmd[3] = (uint8_t)((Address & 0xFF00) >> 8);
    cmd[4] = (uint8_t)((Address & 0xFF));

    /* Enable write operations */
    W25Q256_WriteEnable();

    /*Select the FLASH: Chip Select low */
    W25Q256_Enable();

    /* Send the read ID command */
    HAL_SPI_Transmit(&spi_port, cmd, 5, W25Q256_TIMEOUT_VALUE);
    /*Deselect the FLASH: Chip Select high */
    W25Q256_Disable();

    while(W25Q256_GetStatus() == W25Q256_BUSY);

    return W25Q256_OK;

}


/**
  * @brief  Erases the specified block of the QSPI memory.
  * @param  BlockAddress: Block address to erase
  * @retval QSPI memory status
  */
uint8_t W25Q256_Erase_Block(uint32_t Address)
{
    uint8_t cmd[5];
    uint32_t tickstart = HAL_GetTick();


    if(Address >= W25Q256FV_FLASH_SIZE)
        return W25Q256_ERROR;

    cmd[0] = BLOCK_ERASE_CMD;
    cmd[1] = (uint8_t)((Address & 0xFF000000) >> 24);
    cmd[2] = (uint8_t)((Address & 0xFF0000) >> 16);
    cmd[3] = (uint8_t)((Address & 0xFF00) >> 8);
    cmd[4] = (uint8_t)((Address & 0xFF));

    /* Enable write operations */
    W25Q256_WriteEnable();

    /*Select the FLASH: Chip Select low */
    W25Q256_Enable();
    /* Send the read ID command */
    HAL_SPI_Transmit(&spi_port, cmd, 5, W25Q256_TIMEOUT_VALUE);
    /*Deselect the FLASH: Chip Select high */
    W25Q256_Disable();

    /* Wait the end of Flash writing */
    while(W25Q256_GetStatus() == W25Q256_BUSY)
    {
        /* Check for the Timeout */
        if((HAL_GetTick() - tickstart) > W25Q256FV_BLOCK_ERASE_MAX_TIME)
        {
            return W25Q256_TIMEOUT;
        }
    }
    return W25Q256_OK;
}



/**
  * @brief  Erases the entire QSPI memory.This function will take a very long time.
  * @retval QSPI memory status
  */
uint8_t W25Q256_Erase_Chip(void)
{
    uint8_t cmd[1];
    uint32_t tickstart = HAL_GetTick();
    cmd[0] = CHIP_ERASE_CMD;
/* Enable write operations */
    W25Q256_WriteEnable();

    /*Select the FLASH: Chip Select low */
    W25Q256_Enable();
    /* Send the read ID command */
    HAL_SPI_Transmit(&spi_port, cmd, 1, W25Q256_TIMEOUT_VALUE);
    /*Deselect the FLASH: Chip Select high */
    W25Q256_Disable();



    while(W25Q256_GetStatus() != W25Q256_BUSY)
    {
        /* Check for the Timeout */
        if((HAL_GetTick() - tickstart) > W25Q256FV_BULK_ERASE_MAX_TIME)
        {
            return W25Q256_TIMEOUT;
        }
    }
    return W25Q256_OK;

}


//进入掉电模式
void W25Q256_PowerDown(void)
{
    int8_t cmd[] = {POWER_DOWN_CMD};
    /* 选择 FLASH: CS 低 */
    W25Q256_Enable();
    /* 发送 掉电 命令 */
    HAL_SPI_Transmit(&spi_port, cmd, 1, W25Q256_TIMEOUT_VALUE);
    /* 停止信号  FLASH: CS 高 */
    W25Q256_Disable();
}

//唤醒
void W25Q256_WAKEUP(void)
{
    int8_t cmd[] = {RELEASE_POWER_DOWN_CMD};
    /* 选择 FLASH: CS 低 */
    W25Q256_Enable();
    /* 发送 掉电 命令 */
    HAL_SPI_Transmit(&spi_port, cmd, 1, W25Q256_TIMEOUT_VALUE);
    /* 停止信号  FLASH: CS 高 */
    W25Q256_Disable();
    while(W25Q256_GetStatus() == W25Q256_BUSY);
}


/********************************End of File************************************/