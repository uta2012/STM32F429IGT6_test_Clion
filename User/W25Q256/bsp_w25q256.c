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
    res = W25Q256_Enter_4ByteAddressMode();
    W25Q256_Wait_Busy();
    if(res == W25Q256_OK)
        return W25Q256_OK;
    else
        return W25Q256_ERROR;
}



/**
 * @brief  Reset the W25Q256FV.
 * @retval None
 */
uint8_t W25Q256_Reset(void)
{
    uint8_t cmd[] = {RESET_ENABLE_CMD, RESET_MEMORY_CMD};
    W25Q256_Enable();
    HAL_SPI_Transmit(&spi_port, cmd, 2, W25Q256_TIMEOUT_VALUE);
    W25Q256_Disable();
    W25Q256_Wait_Busy();
    return W25Q256_OK;
}





//读取W25QXX的状态寄存器
//BIT7  6   5   4   3   2   1   0
//SPR   RV  TB BP2 BP1 BP0 WEL BUSY
//SPR:默认0,状态寄存器保护位,配合WP使用
//TB,BP2,BP1,BP0:FLASH区域写保护设置
//WEL:写使能锁定
//BUSY:忙标记位(1,忙;0,空闲)
//默认:0x00

/**
 * @brief  Read register1's status and return that value.
 *         Register1 Structure
 *         BIT 7  6   5   4   3   2   1   0
 *         SPR   RV  TB  BP2 BP1 BP0 WEL BUSY
 *
 * @retval register1's value.
 */
uint8_t W25Q256_Read_SR1(void)
{
    uint8_t res=0;

    uint8_t cmd[1] = {READ_STATUS_REG1_CMD};

    W25Q256_Enable();                            //使能器件

    HAL_SPI_Transmit(&spi_port, cmd, 1, W25Q256_TIMEOUT_VALUE);
    HAL_SPI_Receive(&spi_port, &res, 1, W25Q256_TIMEOUT_VALUE);
    W25Q256_Disable();                           //取消片选
    return res;
}


/**
 * @brief  Read register2's status and return that value.
 *
 * @retval register2's value.
 */
uint8_t W25Q256_Read_SR2(void)
{
    uint8_t res=0;

    uint8_t cmd[1] = {READ_STATUS_REG2_CMD };

    W25Q256_Enable();                            //使能器件

    HAL_SPI_Transmit(&spi_port, cmd, 1, W25Q256_TIMEOUT_VALUE);
    HAL_SPI_Receive(&spi_port, &res, 1, W25Q256_TIMEOUT_VALUE);
    W25Q256_Disable();                           //取消片选
    return res;
}


/**
 * @brief  Read register3's status and return that value.
 *
 * @retval register3's value.
 */
uint8_t W25Q256_Read_SR3(void)
{
    uint8_t res=0;

    uint8_t cmd[1] = {READ_STATUS_REG3_CMD};

    W25Q256_Enable();                            //使能器件

    HAL_SPI_Transmit(&spi_port, cmd, 1, W25Q256_TIMEOUT_VALUE);
    HAL_SPI_Receive(&spi_port, &res, 1, W25Q256_TIMEOUT_VALUE);
    W25Q256_Disable();                           //取消片选
    return res;
}


//等到busy寄存器变成0

/**
 * @brief  Wait the W25Q256 busy status over,
 *         and then Register1's BUSY bit(No.0 bit) transform to 0.
 *
 * @retval None
 */
void W25Q256_Wait_Busy(void)
{

    while((W25Q256_Read_SR1()&0x01)==0x01);  		// 等待BUSY位清空

}



/**
  * @brief  W25QXX enter 4 bit AddressMode,default is 3 bit AddressMode.
  * @retval None
  */
uint8_t W25Q256_Enter_4ByteAddressMode(void)
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
  *
  * @retval None
  */
uint8_t W25Q256_Exit_4ByteAddressMode(void)
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


    W25Q256_Wait_Busy();

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

    W25Q256_Wait_Busy();

    return W25Q256_OK;
}

/**
  * @brief  This function send a Write Disable and wait it is effective.
  * @retval None
  */
uint8_t W25Q256_WriteDisable(void)
{
    uint8_t cmd[] = {WRITE_DISABLE_CMD};

    W25Q256_Enable();
    HAL_SPI_Transmit(&spi_port, cmd, 1, W25Q256_TIMEOUT_VALUE);
    W25Q256_Disable();

    W25Q256_Wait_Busy();

    return W25Q256_OK;
}



/**
 * @brief  Get the jedec ID of the W25Q256.
 * @retval The jedec ID of the W25Q256.
 */
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

    W25Q256_Wait_Busy();

    jedc_id = (jedc_id_temp[0] << 16) | (jedc_id_temp[1] << 8) | jedc_id_temp[2];

    return jedc_id;
}





/**
  * @brief  Reads an amount of data from the QSPI memory.
  * @param  pData: Pointer to data to be read
  * @param  Read_Addr: Read start address 0-FFFFFFFF
  * @param  Read_Size: Size of data to read
  *
  * @retval QSPI memory status(W25Q256_OK)
  */
uint8_t W25Q256_Read(uint8_t* pData, uint32_t Read_Addr, uint32_t Read_Size)
{
    uint8_t cmd[5];

    /* Configure the command */
    cmd[0] = READ_CMD;
    cmd[1] = (uint8_t)((Read_Addr & 0xFF000000) >> 24);
    cmd[2] = (uint8_t)((Read_Addr & 0xFF0000) >> 16);
    cmd[3] = (uint8_t)((Read_Addr & 0xFF00) >> 8);
    cmd[4] = (uint8_t)((Read_Addr & 0xFF) >> 0);

    W25Q256_Enable();
    HAL_SPI_Transmit(&spi_port, cmd, 5, W25Q256_TIMEOUT_VALUE);

    HAL_SPI_Receive(&spi_port, pData, Read_Size, W25Q256_TIMEOUT_VALUE);

    W25Q256_Disable();

    W25Q256_Wait_Busy();

    return W25Q256_OK;
}





//SPI在一页(0~65535)内写入少于256个字节的数据
//在指定地址开始写入最大256字节的数据
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)
//NumByteToWrite:要写入的字节数(最大256),该数不应该超过该页的剩余字节数!!!
/**
  * @brief  Page write
  * @param  pData: Pointer to data to be read
  * @param  Page_Addr:  Page Address, a page size is 4096 bytes,
  *                     then the form of page address be  x*4096.
  *
  * @param  Write_Size: How much bytes to write, in this function,
  *                     witch is not more than a page size(4096).
  *
  * @retval QSPI memory status
  */
uint8_t W25Q256_Write_Page(uint8_t* pData, uint32_t Page_Addr, uint32_t Write_Size)
{
    //地址大于内存
    if(Page_Addr >= W25Q256FV_FLASH_SIZE)
        return W25Q256_ERROR;

    if(Write_Size >= W25Q256FV_PAGE_SIZE)
        return W25Q256_ERROR;


    uint8_t cmd[5];
    cmd[0] = PAGE_PROG_CMD;
    cmd[1] = (uint8_t)((Page_Addr & 0xFF000000) >> 24);
    cmd[2] = (uint8_t)((Page_Addr & 0xFF0000) >> 16);
    cmd[3] = (uint8_t)((Page_Addr & 0xFF00) >> 8);
    cmd[4] = (uint8_t)((Page_Addr & 0xFF));

    W25Q256_WriteEnable();
    /* Enable write operations */
    W25Q256_Enable();
    /* Send the command */
    HAL_SPI_Transmit(&spi_port, cmd, 5, W25Q256_TIMEOUT_VALUE);
    /* Transmission of the data */
    HAL_SPI_Transmit(&spi_port, pData, Write_Size, W25Q256_TIMEOUT_VALUE);
    W25Q256_Disable();
    W25Q256_Wait_Busy();
    return W25Q256_OK;
}



//写SPI FLASH
//在指定地址开始写入指定长度的数据
//该函数带擦除操作!
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)
//NumByteToWrite:要写入的字节数(最大65535)

/**
  * @brief  Write the data to Flash
  * @param  pData: Pointer to data to be written
  * @param  Write_Addr: Write addrress.(0-0xFFFFFFFF)
  * @param  Write_Size: Not much than 32MByte.
  * @retval QSPI memory status
  */
uint8_t W25Q256_Write(uint8_t* pData, uint32_t Write_Addr, uint32_t Write_Size)
{
    uint32_t sector_position = 0;
    uint16_t sector_offset = 0;
    uint16_t sector_remain = 0;
    uint16_t i;

    uint8_t buf[W25Q256FV_SECTOR_SIZE];

    sector_position = Write_Addr / W25Q256FV_SECTOR_SIZE;//扇区地址
    sector_offset = Write_Addr % W25Q256FV_SECTOR_SIZE;//在扇区内的偏移
    sector_remain = W25Q256FV_SECTOR_SIZE - sector_offset;//扇区剩余空间大小
    //printf("ad:%X,nb:%X\r\n",Write_Addr,Write_Size);//测试用
    if(Write_Size <= sector_remain)
        sector_remain = Write_Size;//不大于4096个字节
    while(1)
    {
//        W25Q256_Read(buf,sector_position*W25Q256FV_SECTOR_SIZE,W25Q256FV_SECTOR_SIZE);//读出整个扇区的内容
//        for(i=0; i<sector_remain; i++)//校验数据
//        {
//            if(buf[sector_offset+i] != 0XFF)
//                break;//需要擦除
//        }
//        if(i < sector_remain)//需要擦除
//        {
            W25Q256_Erase_Sector(sector_position);		//擦除这个扇区
//            for(i=0; i<sector_remain; i++)	   		//复制
//            {
//                buf[i+sector_offset]=pData[i];
//            }
//            W25Q256_Write_NoCheck(buf,sector_position*W25Q256FV_SECTOR_SIZE,W25Q256FV_SECTOR_SIZE);//写入整个扇区
//
//        }
//        else
            W25Q256_Write_NoCheck(pData,Write_Addr,sector_remain);//写已经擦除了的,直接写入扇区剩余区间.

        if(Write_Size == sector_remain)
            break;//写入结束了
        else//写入未结束
        {
            sector_position++;//扇区地址增1
            sector_offset=0;//偏移位置为0

            pData += sector_remain;  				//指针偏移
            Write_Addr += sector_remain;				//写地址偏移
            Write_Size -= sector_remain;			//字节数递减
            if(Write_Size > W25Q256FV_SECTOR_SIZE)
                sector_remain = W25Q256FV_SECTOR_SIZE;//下一个扇区还是写不完
            else
                sector_remain = Write_Size;		//下一个扇区可以写完了
        }
        W25Q256_Wait_Busy();
    }

    return W25Q256_OK;
}




//无检验写SPI FLASH
//必须确保所写的地址范围内的数据全部为0XFF,否则在非0XFF处写入的数据将失败!
//具有自动换页功能
//在指定地址开始写入指定长度的数据,但是要确保地址不越界!
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)
//NumByteToWrite:要写入的字节数(最大65535)
//CHECK OK

/**
 * @brief  Write the data to Flash without erase.
 * @param  pData:   Pointer to data to be written
 * @param  Write_Addr:  Write addrress.(0-0xFFFFFFFF)
 * @param  Write_Size:  Not much than 32MByte.
 *
 * @retval None
 */
void W25Q256_Write_NoCheck(uint8_t* pData,uint32_t Write_Addr,uint16_t Write_Size)
{
    uint16_t page_remain;
    page_remain = W25Q256FV_PAGE_SIZE - (Write_Addr % W25Q256FV_PAGE_SIZE); //单页剩余的字节数

    //如果剩下的空间大于要写入数据的大小
    if(Write_Size <= page_remain)
        page_remain = Write_Size;//不大于256个字节

    while(1)
    {
        W25Q256_Write_Page(pData,Write_Addr,page_remain);
        if(Write_Size == page_remain)
            break;//写入结束了
        else //NumByteToWrite>page_remain
        {
            pData += page_remain;
            Write_Addr += page_remain;

            Write_Size -= page_remain;			  //减去已经写入了的字节数
            if(Write_Size > W25Q256FV_PAGE_SIZE)
                page_remain = W25Q256FV_PAGE_SIZE; //一次可以写入256个字节
            else
                page_remain = Write_Size; 	  //不够256个字节了
        }
        W25Q256_Wait_Busy();
    }
}













//擦除一个扇区
//Dst_Addr:扇区地址 根据实际容量设置
//擦除一个山区的最少时间:150ms

/**
 * @brief  Erase a sector.
 * @param  Sector_Addr: x * SECTOR_SIZE (x= 1,2,3,4....)
 *
 * @retval None
 */
uint8_t W25Q256_Erase_Sector(uint32_t Sector_Addr)
{
    uint8_t cmd[5];

    /* Check parameter */
    if(Sector_Addr >= W25Q256FV_FLASH_SIZE)
        return W25Q256_ERROR;



    cmd[0] = SECTOR_ERASE_CMD;
    cmd[1] = (uint8_t)((Sector_Addr & 0xFF000000) >> 24);
    cmd[2] = (uint8_t)((Sector_Addr & 0xFF0000) >> 16);
    cmd[3] = (uint8_t)((Sector_Addr & 0xFF00) >> 8);
    cmd[4] = (uint8_t)((Sector_Addr & 0xFF));

    /* Enable write operations */
    W25Q256_WriteEnable();

    /*Select the FLASH: Chip Select low */
    W25Q256_Enable();

    /* Send the read ID command */
    HAL_SPI_Transmit(&spi_port, cmd, 5, W25Q256_TIMEOUT_VALUE);
    /*Deselect the FLASH: Chip Select high */
    W25Q256_Disable();

    W25Q256_Wait_Busy();

    return W25Q256_OK;

}


/**
  * @brief  Erases the specified block of the QSPI memory.
  * @param  Block_Address: Block address to erase
  * @retval QSPI memory status
  */
uint8_t W25Q256_Erase_Block(uint32_t Block_Address)
{
    uint8_t cmd[5];

    if(Block_Address >= W25Q256FV_FLASH_SIZE)
        return W25Q256_ERROR;



    cmd[0] = BLOCK_ERASE_CMD;
    cmd[1] = (uint8_t)((Block_Address & 0xFF000000) >> 24);
    cmd[2] = (uint8_t)((Block_Address & 0xFF0000) >> 16);
    cmd[3] = (uint8_t)((Block_Address & 0xFF00) >> 8);
    cmd[4] = (uint8_t)((Block_Address & 0xFF));

    /* Enable write operations */
    W25Q256_WriteEnable();

    /*Select the FLASH: Chip Select low */
    W25Q256_Enable();
    /* Send the read ID command */
    HAL_SPI_Transmit(&spi_port, cmd, 5, W25Q256_TIMEOUT_VALUE);
    /*Deselect the FLASH: Chip Select high */
    W25Q256_Disable();

    W25Q256_Wait_Busy();
    return W25Q256_OK;
}



/**
  * @brief  Erases the entire QSPI memory.This function will take a very long time.
  * @retval QSPI memory status
  */
uint8_t W25Q256_Erase_Chip(void)
{
    uint8_t cmd[1] = {CHIP_ERASE_CMD};

/* Enable write operations */
    W25Q256_WriteEnable();

    /*Select the FLASH: Chip Select low */
    W25Q256_Enable();
    /* Send the read ID command */
    HAL_SPI_Transmit(&spi_port, cmd, 1, W25Q256_TIMEOUT_VALUE);
    /*Deselect the FLASH: Chip Select high */
    W25Q256_Disable();
    W25Q256_Wait_Busy();

    return W25Q256_OK;

}


/**
  * @brief  Enter PowerDown mode.
  * @retval None
  */
void W25Q256_PowerDown(void)
{
    int8_t cmd[] = {POWER_DOWN_CMD};
    /* 选择 FLASH: CS 低 */
    W25Q256_Enable();
    /* 发送 掉电 命令 */
    HAL_SPI_Transmit(&spi_port, cmd, 1, W25Q256_TIMEOUT_VALUE);
    /* 停止信号  FLASH: CS 高 */
    W25Q256_Disable();
    W25Q256_Wait_Busy();
}

/**
  * @brief  from PowerDown mode WAKEUP.
  * @retval None
  */
void W25Q256_WAKEUP(void)
{
    int8_t cmd[] = {RELEASE_POWER_DOWN_CMD};
    /* 选择 FLASH: CS 低 */
    W25Q256_Enable();
    /* 发送 掉电 命令 */
    HAL_SPI_Transmit(&spi_port, cmd, 1, W25Q256_TIMEOUT_VALUE);
    /* 停止信号  FLASH: CS 高 */
    W25Q256_Disable();
    W25Q256_Wait_Busy();
}







/**
 * @brief  Test.
 * @retval None
 */
void W25Q256_Test(void)
{
    char buf[50] = {0};




    char data[50] = {0};
    printf("\r\n1\r\n");
    strcpy(data, (char *)"11111 11111!\r\n");
    printf("\r\ndata: %S\r\n", data);
    W25Q256_Write((uint8_t*)data, 0x4, 50);
    W25Q256_Read((uint8_t*)buf, 0x4, 50);
    printf("\r\nbuf: %S\r\n", buf);

    W25Q256_Read((uint8_t*)buf, 0x6, 50);
    printf("\r\nbuf: %S\r\n", buf);


//    printf("\r\n2\r\n");
//    strcpy(data, (char *)"22222 22222!\r\n");
//    W25Q256_Write((uint8_t*)data, 0, 50);
//    W25Q256_Read((uint8_t*)buf, 0, 50);
//    printf("\r\n%s\r\n", buf);
//
//    printf("\r\n3\r\n");
//    strcpy(data, (char *)"33333 33333!\r\n");
//    W25Q256_Write((uint8_t*)data, 0x00, 50);
//    W25Q256_Read((uint8_t*)buf, 0x00, 50);
//    printf("\r\n%s\r\n", buf);
//
//    printf("\r\n4\r\n");
//    strcpy(data, (char *)"44444 44444!\r\n");
//    W25Q256_Write((uint8_t*)data, 0x00, 50);
//    W25Q256_Read((uint8_t*)buf, 0x00, 50);
//    printf("\r\n%s\r\n", buf);
//
//    printf("\r\n5\r\n");
//    strcpy(data, (char *)"55555 55555!\r\n");
//    W25Q256_Write((uint8_t*)data, 0x00, 50);
//    W25Q256_Read((uint8_t*)buf, 0x00, 50);
//    printf("\r\n%s\r\n", buf);
//
//    printf("\r\n6\r\n");
//    strcpy(data, (char *)"66666 66666!\r\n");
//    W25Q256_Write((uint8_t*)data, 0x00, 50);
//    W25Q256_Read((uint8_t*)buf, 0x00, 50);
//    printf("\r\n%s\r\n", buf);
}






/********************************End of File************************************/