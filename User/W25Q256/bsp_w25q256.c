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
    W25Q256_Reset();
    res = W25Q256_Enter4ByteAddressMode();
    return res;
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

    uint8_t cmd[] = {EXIT_4BYTE_ADDRESS_MODE};


    W25Q256_Enable();
    HAL_SPI_Transmit(&spi_port, cmd, 1, W25Q256_TIMEOUT_VALUE);
    W25Q256_Disable();



    while(W25Q256_GetStatus() == W25Q256_BUSY)
    {
        ;
    }

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

    while(W25Q256_GetStatus() == W25Q256_BUSY)
    {
        ;
    }
    return W25Q256_OK;
}



/**
  * @brief  Read Manufacture/Device ID.
	* @param  return value address
  * @retval None
  */
HAL_StatusTypeDef W25Q256_Read_ID(uint8_t *ID)
{
    uint8_t cmd[] = {READ_ID_CMD, 0x00, 0x00, 0x00};
    HAL_StatusTypeDef e_rc = HAL_OK;

    W25Q256_Enable();
    do{
        e_rc = HAL_SPI_Transmit(&spi_port, cmd, 4, W25Q256_TIMEOUT_VALUE);
        if(e_rc != HAL_OK)
            break;

        e_rc = HAL_SPI_Receive(&spi_port, ID, 2, W25Q256_TIMEOUT_VALUE);
        if(e_rc != HAL_OK)
            break;

    }while(0);
    W25Q256_Disable();

    return e_rc;
}



uint8_t W25Q256_Device_ID(void)
{
    uint8_t cmd[] = {DEVICE_ID_CMD, 0x00, 0x00, 0x00};
    uint8_t device_id = 0;
    uint32_t tickstart = HAL_GetTick();


    W25Q256_Enable();
    HAL_SPI_Transmit(&spi_port, cmd, 4, W25Q256_TIMEOUT_VALUE);

    HAL_SPI_Receive(&spi_port, &device_id, 1, W25Q256_TIMEOUT_VALUE);
    W25Q256_Disable();

    while(W25Q256_GetStatus() == W25Q256_BUSY)
    {
        if((HAL_GetTick()-tickstart) > W25Q256_TIMEOUT_VALUE)
        {
            return W25Q256_TIMEOUT;
        }
    }
    return device_id;

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
  * @brief  Writes an amount of data to the QSPI memory.
  * @param  pData: Pointer to data to be written
  * @param  WriteAddr: Write start address
  * @param  Size: Size of data to write,No more than 256byte.
  * @retval QSPI memory status
  */
uint8_t W25Q256_Write(uint8_t* pData, uint32_t WriteAddr, uint32_t Size)
{
    uint8_t cmd[5];

    if(WriteAddr >= W25Q256FV_FLASH_SIZE)
        return W25Q256_ERROR;






    /* Configure the command */
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





/********************************End of File************************************/