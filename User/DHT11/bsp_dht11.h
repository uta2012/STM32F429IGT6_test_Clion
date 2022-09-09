/**
  ******************************************************************************
  * @file    bsp_dht11.h
  * @author  ROG Zephyrus M16
  * @date    10:49 AM 8/19/2022
  * @brief   None.
  *          
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 UTA.
  * All rights reserved.
  *
  * By(UTA personally owned)
  * https://blog.csdn.net/qq_51913390
  *
  ******************************************************************************
  */
/********************************End of Head************************************/


#ifndef __BSP_DHT11_H__
#define __BSP_DHT11_H__

#ifdef __cplusplus
extern "C" {
#endif


/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include <drivers/pin.h>
#include "drv_common.h"



#define Input   0
#define Output  1





typedef struct
{
    uint8_t humidity_int;
    uint8_t humidity_float;
    uint8_t temperature_int;
    uint8_t temperature_float;
    uint8_t checksum;

    float humidity;
    float temperature;

}DHT11_DATA_TypeDef;



#define DHT11_Read_DATA()   rt_pin_read(rt_pin_get("PE.2"))
#define DHT11_DATA_SET()    rt_pin_write(rt_pin_get("PE.2"), PIN_HIGH)
#define DHT11_DATA_RESET()  rt_pin_write(rt_pin_get("PE.2"), PIN_LOW)
void DHT11_Pin_Mode(uint8_t mode);
uint8_t DHT11_Read_Byte(void);
uint8_t DHT11_Read_Temperature_and_Humidity(DHT11_DATA_TypeDef *DHT11_data);

#endif //__BSP_DHT11_H__

/********************************End of File************************************/
