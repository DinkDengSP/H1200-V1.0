/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-30 10:08:12 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-31 15:02:22 +0800
************************************************************************************************/ 
#include "BoardSRAM.h"
#include "DriverConfigBoard.h"

//端口初始化
void BoardSRAM_Init(void)
{
    HAL_StatusTypeDef halState = HAL_OK;
    //需要使用的数据结构r
    SRAM_HandleTypeDef SRAM_Handler = {0};
    FSMC_NORSRAM_TimingTypeDef SRAM_Timing = {0};
	/* FSMC SRAM所涉及到GPIO配置 */
	MCU_FSMC_PortInit();
    //SRAM实体关联
    SRAM_Handler.Instance       = FSMC_NORSRAM_DEVICE;
    SRAM_Handler.Extended       = FSMC_NORSRAM_EXTENDED_DEVICE;
    SRAM_Handler.Init.NSBank    = FSMC_NORSRAM_BANK4;//使用NE4
    HAL_SRAM_DeInit(&SRAM_Handler);
	//SRAM时间配置,静态存储器当成设备用
    //地址建立时间（ADDSET）为,一个单位6ns
	SRAM_Timing.AddressSetupTime      = BOARD_SRAM_ADDRESS_SETUP_CYCLE;
    //地址保持时间,（ADDHLD）模式A未用到
    SRAM_Timing.AddressHoldTime       = 0x00;
    //数据保存时间为 ,,一个单位6ns
    SRAM_Timing.DataSetupTime         = BOARD_SRAM_DATA_SETUP_CYCLE;
    //设置总线转换周期，仅用于复用模式的NOR操作
    SRAM_Timing.BusTurnAroundDuration = 0x00;
    //设置时钟分频，仅用于同步类型的存储器
    SRAM_Timing.CLKDivision           = 0x00;
    //数据保持时间，仅用于同步型的NOR
    SRAM_Timing.DataLatency           = 0x00;
    //选择匹配SRAM的模式
    SRAM_Timing.AccessMode            = FSMC_ACCESS_MODE_A;
    //SRAM时序配置
    SRAM_Handler.Init.DataAddressMux    =FSMC_DATA_ADDRESS_MUX_DISABLE;//地址/数据线不复用
    SRAM_Handler.Init.MemoryType        =FSMC_MEMORY_TYPE_SRAM;//SRAM
    SRAM_Handler.Init.MemoryDataWidth   =FSMC_NORSRAM_MEM_BUS_WIDTH_16;//16位数据宽度
    SRAM_Handler.Init.BurstAccessMode   =FSMC_BURST_ACCESS_MODE_DISABLE;//是否使能突发访问,仅对同步突发存储器有效,此处未用到
    SRAM_Handler.Init.WaitSignalPolarity=FSMC_WAIT_SIGNAL_POLARITY_LOW;//等待信号的极性,仅在突发模式访问下有用
    SRAM_Handler.Init.WaitSignalActive  =FSMC_WAIT_TIMING_BEFORE_WS;//存储器是在等待周期之前的一个时钟周期还是等待周期期间使能NWAIT
    SRAM_Handler.Init.WriteOperation    =FSMC_WRITE_OPERATION_ENABLE;//存储器写使能
    SRAM_Handler.Init.WaitSignal        =FSMC_WAIT_SIGNAL_DISABLE;//等待使能位,此处未用到
    SRAM_Handler.Init.ExtendedMode      =FSMC_EXTENDED_MODE_DISABLE;//读写使用相同的时序
    SRAM_Handler.Init.AsynchronousWait  =FSMC_ASYNCHRONOUS_WAIT_DISABLE;//是否使能同步传输模式下的等待信号,此处未用到
    SRAM_Handler.Init.WriteBurst        =FSMC_WRITE_BURST_DISABLE;//禁止突发写
	/* 配置SRAM控制器基本参数 */
	halState = HAL_SRAM_Init(&SRAM_Handler, &SRAM_Timing, &SRAM_Timing);
    if(halState != HAL_OK)
    {
        MCU_LogShowHalState(halState,0);
        while(1);
    }
    if(HAL_SRAM_STATE_PROTECTED != HAL_SRAM_GetState(&SRAM_Handler))
    {
        return;
    }
    //使能SRAM写入
    halState = HAL_SRAM_WriteOperation_Enable(&SRAM_Handler);
    if(halState != HAL_OK)
    {
        MCU_LogShowHalState(halState,0);
        while(1);
    }
}


//自检
ERROR_SUB BoardSRAM_Check(void)
{
    uint32_t opAddr = 0X00000000;
    uint32_t indexUtil = 0;
    uint8_t readDat = 0;
    uint8_t writeValue = 0X00;
    //全部写入0XAA
    writeValue = 0XAA;
    opAddr = BOARD_SRAM_BASE_ADDR;
    for(indexUtil = 0; indexUtil < BOARD_SRAM_CAPACITY;indexUtil++)
    {
        *(__IO uint8_t*)(opAddr) = writeValue;
        opAddr++;
    }
    //检查是不是写入的值
    opAddr = BOARD_SRAM_BASE_ADDR;
    for(indexUtil = 0; indexUtil < BOARD_SRAM_CAPACITY;indexUtil++)
    {
        readDat = *(__IO uint8_t*)(opAddr);
        opAddr++;
        if(readDat != writeValue)
        {
            MCU_LogPrintf("%s,Line: %d,IndexUtil: %d,ErrorSubCode: 0X%08X\r\n",__func__,__LINE__,
                                indexUtil,ERROR_SUB_BOARD_SRAM_CHECK_FIRST);
            return ERROR_SUB_BOARD_SRAM_CHECK_FIRST;
        }
    }
    //全部写入0X55
    writeValue = 0X55;
    opAddr = BOARD_SRAM_BASE_ADDR;
    for(indexUtil = 0; indexUtil < BOARD_SRAM_CAPACITY;indexUtil++)
    {
        *(__IO uint8_t*)(opAddr) = writeValue;
        opAddr++;
    }
    //检查是不是写入的值
    opAddr = BOARD_SRAM_BASE_ADDR;
    for(indexUtil = 0; indexUtil < BOARD_SRAM_CAPACITY;indexUtil++)
    {
        readDat = *(__IO uint8_t*)(opAddr);
        opAddr++;
        if(readDat != writeValue)
        {
            MCU_LogPrintf("%s,Line: %d,IndexUtil: %d,ErrorSubCode: 0X%08X\r\n",__func__,__LINE__,
                                indexUtil,ERROR_SUB_BOARD_SRAM_CHECK_SECOND);
            return ERROR_SUB_BOARD_SRAM_CHECK_SECOND;
        }
    }
    //全部写入0X00
    writeValue = 0X00;
    opAddr = BOARD_SRAM_BASE_ADDR;
    for(indexUtil = 0; indexUtil < BOARD_SRAM_CAPACITY;indexUtil++)
    {
        *(__IO uint8_t*)(opAddr) = writeValue;
        opAddr++;
    }
    //检查是不是写入的值
    opAddr = BOARD_SRAM_BASE_ADDR;
    for(indexUtil = 0; indexUtil < BOARD_SRAM_CAPACITY;indexUtil++)
    {
        readDat = *(__IO uint8_t*)(opAddr);
        opAddr++;
        if(readDat != writeValue)
        {
            MCU_LogPrintf("%s,Line: %d,IndexUtil: %d,ErrorSubCode: 0X%08X\r\n",__func__,__LINE__,
                                indexUtil,ERROR_SUB_BOARD_SRAM_CHECK_THIRD);
            return ERROR_SUB_BOARD_SRAM_CHECK_THIRD;
        }
    }
    return ERROR_SUB_OK;
}

//数据写入
void BoardSRAM_WriteBuffer(uint32_t addr,uint8_t* buf,uint32_t length)
{
    uint32_t indexUtil = 0;
    for(indexUtil = 0; indexUtil < length; indexUtil++)
    {
        *(__IO uint8_t*)(addr) = buf[indexUtil];
        addr++;
    }
}

//数据读取
void BoardSRAM_ReadBuffer(uint32_t addr,uint8_t* buf,uint32_t length)
{
    uint32_t indexUtil = 0;
    for(indexUtil = 0; indexUtil < length; indexUtil++)
    {
        buf[indexUtil] = *(__IO uint8_t*)(addr);
        addr++;
    }
}












