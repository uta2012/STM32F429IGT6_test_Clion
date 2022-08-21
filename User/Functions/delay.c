/**
  ******************************************************************************
  * @file    delay.c
  * @author  ROG Zephyrus M16
  * @date    11:48 AM 8/19/2022
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
#include "delay.h"



/**
 * @brief  delay us.
 * @param  delay us
 * @retval None
 */
#define CPU_FREQUENCY_MHZ    180		// STM32时钟主频
void Delay_us(__IO uint32_t delay)
{
    int last, curr, val;
    int temp;

    while (delay != 0)
    {
        temp = delay > 900 ? 900 : delay;
        last = SysTick->VAL;
        curr = last - CPU_FREQUENCY_MHZ * temp;
        if (curr >= 0)
        {
            do
            {
                val = SysTick->VAL;
            }
            while ((val < last) && (val >= curr));
        }
        else
        {
            curr += CPU_FREQUENCY_MHZ * 1000;
            do
            {
                val = SysTick->VAL;
            }
            while ((val <= last) || (val > curr));
        }
        delay -= temp;
    }
}




/********************************End of File************************************/