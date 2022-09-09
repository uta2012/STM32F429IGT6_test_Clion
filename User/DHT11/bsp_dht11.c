/**
  ******************************************************************************
  * @file    bsp_dht11.c
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
  @verbatim
   
   None
   
  ******************************************************************************
  */
/********************************End of Head************************************/

/* Includes ------------------------------------------------------------------*/
#include "bsp_dht11.h"







/**
 * @brief  change DHT11 data pin mode.
 * @param  mode -> "input" or "output"
 * @retval None
 */
void DHT11_Pin_Mode(uint8_t mode)
{


    if(mode == Output)
    {
        rt_pin_mode(rt_pin_get("PE.2"), PIN_MODE_INPUT);

    }
    else
    {
        rt_pin_mode(rt_pin_get("PE.2"), PIN_MODE_OUTPUT);
    }
}


uint8_t DHT11_Read_Byte(void)
{
    DHT11_Pin_Mode(Output);
    uint8_t temp;
    for(int i=0; i<8; i++)
    {
       /*每bit以50us低电平标置开始，轮询直到从机发出的50us低电平结束*/
       while(DHT11_Read_DATA() == PIN_LOW);
       /*DHT11 以26~28us的高电平表示“0”，以70us高电平表示“1”，
       *通过检测 x us后的电平即可区别这两个状 ，x 即下面的延时 */
       rt_thread_mdelay(40);//延时x us 这个延时需要大于数据0持续的时间即可
       if(DHT11_Read_DATA() == PIN_LOW)
       {
            /*结果为0*/
            temp &= (uint8_t)~(0x01 << (7-i));
       }
       else
       {
           /*结果为1*/
           while(DHT11_Read_DATA() == GPIO_PIN_SET);
           temp |= (uint8_t)(0x01 << (7-i));
       }
   }
   return temp;
}

uint8_t DHT11_Read_Temperature_and_Humidity(DHT11_DATA_TypeDef *DHT11_data)
{

    DHT11_Pin_Mode(Input);
    DHT11_DATA_RESET();
    rt_thread_mdelay(18);
    DHT11_DATA_SET();
    rt_hw_us_delay(30);
    DHT11_Pin_Mode(Output);
    if(DHT11_Read_DATA() == PIN_LOW)
    {
        while(DHT11_Read_DATA() == PIN_LOW);
        while(DHT11_Read_DATA() == PIN_HIGH);
        DHT11_data->humidity_int         =   DHT11_Read_Byte();
        DHT11_data->humidity_float       =   DHT11_Read_Byte();
        DHT11_data->temperature_int      =   DHT11_Read_Byte();
        DHT11_data->temperature_float    =   DHT11_Read_Byte();
        DHT11_data->checksum             =   DHT11_Read_Byte();

        DHT11_data->humidity =   (float)DHT11_data->humidity_int + (float)DHT11_data->humidity_float/10;
        DHT11_data->temperature  = (float)DHT11_data->temperature_int + (float)DHT11_data->temperature_float/10;

        if(DHT11_data->checksum == DHT11_data->humidity_int + DHT11_data->humidity_float + DHT11_data->temperature_int + DHT11_data->temperature_float)
        {
            return SUCCESS;
        }
        else
        {
            return ERROR;
        }

    }
    else
    {
        return ERROR;
    }

}












/********************************End of File************************************/
