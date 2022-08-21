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
    res = ;
    if(res == )

}

/**
 * @brief  Initializes the W25Q256FV interface.
 * @retval None
 */
uint8_t W25Q256_Reset(void)
{
    uint32_t tickstart = HAL_GetTick();

    uint8_t cmd[] = {RESET_ENABLE_CMD, RESET_MEMORY_CMD};
    W25Q256_Enable();
    HAL_SPI_Transmit(&hspi1, cmd, 2, W25Q256_TIMEOUT_VALUE);
    W25Q256_Disable();

    while(W25Q256_GetStatus() == W25Q256_BUSY)
    {
        if((HAL_GetTick()-tickstart) > W25Q256_TIMEOUT_VALUE)
        {
            return W25Q256_TIMEOUT;
        }
    }
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
    HAL_SPI_Transmit(&hspi1, cmd, 1, W25Q256_TIMEOUT_VALUE);
    HAL_SPI_Receive(&hspi1, &status, 1, W25Q256_TIMEOUT_VALUE);
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






/********************************End of File************************************/