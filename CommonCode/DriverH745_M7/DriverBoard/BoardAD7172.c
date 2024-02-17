/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-17 11:49:19 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-17 11:49:19 +0800
************************************************************************************************/ 
#include "BoardAD7172.h"
#include "DriverConfigBoard.h"
#include "tx_mutex.h"

static TX_MUTEX mutexBoardAD7172;

//选中
static void BoardAD7172_ChipSelect(void)
{
    MCU_PinWriteSingle(PIN_BOARD_AD7172_CS,VALID_LEVEL_BOARD_AD7172_CS);
}

//释放
static void BoardAD7172_ChipDisSelect(void)
{
    MCU_PinWriteSingle(PIN_BOARD_AD7172_CS,INVALID_LEVEL_BOARD_AD7172_CS);
}

//上锁
static void BoardAD7172_Lock(void)
{
    tx_mutex_get(&mutexBoardAD7172,TX_WAIT_FOREVER);
}

//解锁
static void BoardAD7172_UnLock(void)
{
    tx_mutex_put(&mutexBoardAD7172);
}

//端口初始化
void BoardAD7172_PortInit(void)
{
    if(mutexBoardAD7172.tx_mutex_id == TX_MUTEX_ID)
    {
        return;
    }
    //创建互斥信号量
    tx_mutex_create(&mutexBoardAD7172,"mutexBoardAD7172",TX_INHERIT);
    //CS端口初始化
    MCU_PinInit(PIN_BOARD_AD7172_CS,MCU_PIN_MODE_OUTPUT_PP,MCU_PIN_PULL_PULLUP,MCU_PIN_SPEED_FREQ_VERY_HIGH,0);
    BoardAD7172_ChipDisSelect();
    //SYNC_ERR端口初始化
    MCU_PinInit(PIN_BOARD_AD7172_SYNC_ERR,MCU_PIN_MODE_INPUT,MCU_PIN_PULL_PULLUP,MCU_PIN_SPEED_FREQ_MEDIUM,0);
    //SPI初始化
    BOARD_AD7172_SPI_PORT_INIT(BOARD_AD7172_SPI_SPEED,BOARD_AD7172_SPI_CPOL,BOARD_AD7172_SPI_CPHA);
}

//AD7172写入数据
static void BoardAD7172_WriteData(uint8_t cmd,uint8_t* writeDataBuffer,uint8_t writeDataLength)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //申请互斥信号量
    BoardAD7172_Lock();
    //选中芯片
    BoardAD7172_ChipSelect();
    //打印写入指令
    MCU_LogPrintf("BoardAD7172_WriteData Cmd: 0X%02X,DataLength: %d\r\n",cmd,writeDataLength);
    //写入命令
    errorSub = BOARD_AD7172_SPI_READ_WRITE(&cmd,SPI_PTR_NULL,1);
    if(errorSub != ERROR_SUB_OK)
    {
        MCU_LogPrintf("BoardAD7172_WriteData ,WriteCmd Error: 0X%08X\r\n",errorSub);
    }
    //写入数据
    errorSub = BOARD_AD7172_SPI_READ_WRITE(writeDataBuffer,SPI_PTR_NULL,writeDataLength);
    if(errorSub != ERROR_SUB_OK)
    {
        MCU_LogPrintf("BoardAD7172_WriteData ,WriteData Error: 0X%08X\r\n",errorSub);
    }
    //释放芯片
    BoardAD7172_ChipDisSelect();
    //释放互斥信号量
    BoardAD7172_UnLock();
}

//AD7172读取数据
static void BoardAD7172_ReadData(uint8_t cmd,uint8_t needReadDataLength,uint8_t* readDataBuffer)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //申请互斥信号量
    BoardAD7172_Lock();
    //选中芯片
    BoardAD7172_ChipSelect();
    //打印写入指令
    MCU_LogPrintf("BoardAD7172_ReadData Cmd: 0X%02X,DataLength: %d\r\n",cmd,needReadDataLength);
    //写入命令
    errorSub = BOARD_AD7172_SPI_READ_WRITE(&cmd,SPI_PTR_NULL,1);
    if(errorSub != ERROR_SUB_OK)
    {
        MCU_LogPrintf("BoardAD7172_ReadData ,WriteCmd Error: 0X%08X\r\n",errorSub);
    }
    //读取数据
    errorSub = BOARD_AD7172_SPI_READ_WRITE(SPI_PTR_NULL,readDataBuffer,needReadDataLength);
    if(errorSub != ERROR_SUB_OK)
    {
        MCU_LogPrintf("BoardAD7172_ReadData ,ReadData Error: 0X%08X\r\n",errorSub);
    }
    //释放芯片
    BoardAD7172_ChipDisSelect();
    //释放互斥信号量
    BoardAD7172_UnLock();
}

//AD7172复位,写入64个1
void BoardAD7172_ChipReset(void)
{
    uint8_t cmd = 0XFF;
    uint8_t writeDataBuffer[] = {0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF};
    BoardAD7172_WriteData(cmd,writeDataBuffer,7);
    //延时等待复位
    CoreDelayMs(10);
}

//创建寄存器读写指令
static uint8_t BoardAD7172_CreateCmd(BOARD_AD7172_FLAG wrFlag,BOARD_AD7172_REG_ADDR regAddr)
{
    uint8_t cmd = 0;
    cmd += ((uint8_t)wrFlag);
    cmd <<= 6;
    cmd += ((uint8_t)regAddr);
    return cmd;
}

//ID检测
ERROR_SUB BoardAD7172_ReadID(uint16_t* idPtr)
{
    *idPtr = 0X0000;
    uint8_t cmd = BoardAD7172_CreateCmd(BOARD_AD7172_FLAG_READ,BOARD_AD7172_REG_ADDR_ID);
    uint8_t regValueArray[2] = {0,0};
    BoardAD7172_ReadData(cmd,2,regValueArray);
    //解析结果
    *idPtr += regValueArray[0];
    *idPtr <<= 8;
    *idPtr += regValueArray[1];
    //结果掩码处理
    *idPtr &= BOARD_AD7172_ID_MASK;
    return ERROR_SUB_OK;
}







