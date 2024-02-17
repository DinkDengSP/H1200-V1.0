/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-23 09:44:56 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-01 11:30:20 +0800
************************************************************************************************/ 
#include "BoardMB85RS2MT.h"
#include "DriverConfigBoard.h"
#include "tx_mutex.h"

static TX_MUTEX mutexBoardMB85RS2MT;

//选中
static void BoardMB85RS2MT_ChipSelect(void)
{
    MCU_PinWriteSingle(PIN_BOARD_MB85RS2MT_CS,VALID_LEVEL_BOARD_MB85RS2MT_CS);
}

//释放
static void BoardMB85RS2MT_ChipDisSelect(void)
{
    MCU_PinWriteSingle(PIN_BOARD_MB85RS2MT_CS,INVALID_LEVEL_BOARD_MB85RS2MT_CS);
}

//上锁
static void BoardMB85RS2MT_Lock(void)
{
    tx_mutex_get(&mutexBoardMB85RS2MT,TX_WAIT_FOREVER);
}

//解锁
static void BoardMB85RS2MT_UnLock(void)
{
    tx_mutex_put(&mutexBoardMB85RS2MT);
}

//初始化铁电存储器
void BoardMB85RS2MT_PortInit(void)
{
    if(mutexBoardMB85RS2MT.tx_mutex_id == TX_MUTEX_ID)
    {
        return;
    }
    //创建互斥信号量
    tx_mutex_create(&mutexBoardMB85RS2MT,"mutexBoardMB85RS2MT",TX_INHERIT);
    //CS端口初始化
    MCU_PinInit(PIN_BOARD_MB85RS2MT_CS,MCU_PIN_MODE_OUTPUT_PP,MCU_PIN_PULL_PULLUP,MCU_PIN_SPEED_FREQ_HIGH,0);
    BoardMB85RS2MT_ChipDisSelect();
    //SPI初始化
    BOARD_MB85RS2MT_SPI_PORT_INIT(BOARD_MB85RS2MT_SPI_SPEED,BOARD_MB85RS2MT_SPI_CPOL,BOARD_MB85RS2MT_SPI_CPHA);
}


//读取芯片ID
static ERROR_SUB BoardMB85RS2MT_ReadID(uint32_t *chipID)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    uint8_t sendBuf[5] = {0};
    uint8_t readBuf[5] = {0};
    //发送指令
    sendBuf[0] = MB85RS2MT_CMD_RDID;
    UserMemSet(sendBuf+1,0XFF,4);
    UserMemSet(readBuf,0XFF,5);
    //片选使能
	BoardMB85RS2MT_ChipSelect();  
    //写入读寄存器命令
    errorSub = BOARD_MB85RS2MT_SPI_READ_WRITE(sendBuf,readBuf,5);
    if(errorSub != ERROR_SUB_OK)
    {
        //释放SPI
        BoardMB85RS2MT_ChipDisSelect();
        //错误打印
        MCU_LogShowErrorSubCode(errorSub);
        return errorSub;
    }
    //释放SPI
    BoardMB85RS2MT_ChipDisSelect();
    uint8_t* buf= (uint8_t*)chipID;
    buf[0] = readBuf[1];
	buf[1] = readBuf[2];
	buf[2] = readBuf[3];
    buf[3] = readBuf[4];
    //返回
    return errorSub;
}

//读状态寄存器
static ERROR_SUB BoardMB85RS2MT_ReadStatusReg(MB85RS2MT_STATUS_REG* statusRegPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    uint8_t sendBuf[2] = {0};
    uint8_t readBuf[2] = {0};
    //发送指令
    sendBuf[0] = MB85RS2MT_CMD_RDSR;
    sendBuf[1] = 0X00;
    //片选使能
	BoardMB85RS2MT_ChipSelect();  
    //写入读寄存器命令
    errorSub = BOARD_MB85RS2MT_SPI_READ_WRITE(sendBuf,readBuf,2);
    if(errorSub != ERROR_SUB_OK)
    {
        //释放SPI
        BoardMB85RS2MT_ChipDisSelect();
        //错误打印
        MCU_LogShowErrorSubCode(errorSub);
        return errorSub;
    }
    //获取寄存器状态
    statusRegPtr->regValueAll = readBuf[1];
    //释放SPI
    BoardMB85RS2MT_ChipDisSelect();
    //返回
    return errorSub;
}

//写状态寄存器
static ERROR_SUB BoardMB85RS2MT_SendCmdWriteStatusReg(uint8_t regVal)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    uint8_t sendBuf[2] = {0};
    uint8_t readBuf[2] = {0};
    //发送指令
    sendBuf[0] = MB85RS2MT_CMD_WRSR;
    sendBuf[1] = regVal;
    //片选使能
	BoardMB85RS2MT_ChipSelect();  
    //写入写状态寄存器命令
    errorSub = BOARD_MB85RS2MT_SPI_READ_WRITE(sendBuf,readBuf,2);
    if(errorSub != ERROR_SUB_OK)
    {
        //释放SPI
        BoardMB85RS2MT_ChipDisSelect();
        //错误打印
        MCU_LogShowErrorSubCode(errorSub);
        return errorSub;
    }
    //释放SPI
    BoardMB85RS2MT_ChipDisSelect();
    //返回
    return errorSub;
}

//发送写入使能指令
static ERROR_SUB BoardMB85RS2MT_SendCmdWritePermissions(FunctionalState permissions)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    uint8_t cmd = MB85RS2MT_CMD_WREN;
    if(permissions == DISABLE)
    {
        cmd = MB85RS2MT_CMD_WRDI;
    }
    //片选使能
	BoardMB85RS2MT_ChipSelect();  
    //写入命令
    errorSub = BOARD_MB85RS2MT_SPI_READ_WRITE(&cmd,SPI_PTR_NULL,1);
    if(errorSub != ERROR_SUB_OK)
    {
        //释放SPI
        BoardMB85RS2MT_ChipDisSelect();
        //错误打印
        MCU_LogShowErrorSubCode(errorSub);
        return errorSub;
    }
    //释放SPI
    BoardMB85RS2MT_ChipDisSelect();
    //返回
    return errorSub;
}

//写入使能
static ERROR_SUB BoardMB85RS2MT_WriteEnable(void)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    MB85RS2MT_STATUS_REG statusReg;
    uint16_t retryCount = 0;
    do
    {
        //发送写入使能
        errorSub = BoardMB85RS2MT_SendCmdWritePermissions(ENABLE);
        if(errorSub != ERROR_SUB_OK)
        {
            //错误打印
            MCU_LogShowErrorSubCode(errorSub);
            return errorSub;
        }
        //读取状态寄存器
        errorSub = BoardMB85RS2MT_ReadStatusReg(&statusReg);
        if(errorSub != ERROR_SUB_OK)
        {
            //错误打印
            MCU_LogShowErrorSubCode(errorSub);
            return errorSub;
        }
        //检查是否成功
        if(statusReg.regValueSingle.wel == 0)
        {
            retryCount++;
            continue;
        }
        break;
    } while (retryCount < BOARD_MB85RS2MT_WRITE_SR_RETRY_MAX);
    //检查是否成功
    if(statusReg.regValueSingle.wel == 0)
    {
        MCU_LogPrintf("MB85RS2MT StatusReg,wel: %d,bp: %d,wpen: %d\r\n",statusReg.regValueSingle.wel,
                        statusReg.regValueSingle.bp,statusReg.regValueSingle.wpen);
        errorSub = ERROR_SUB_BOARD_MB85RS2MT_WRITE_ENABLE;
        //错误打印
        MCU_LogShowErrorSubCode(errorSub);
        return errorSub;
    }
    return errorSub;
}

//写入禁止
static ERROR_SUB BoardMB85RS2MT_WriteDisable(void)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    MB85RS2MT_STATUS_REG statusReg;
    uint16_t retryCount = 0;
    do
    {
        //发送写入禁止
        errorSub = BoardMB85RS2MT_SendCmdWritePermissions(DISABLE);
        if(errorSub != ERROR_SUB_OK)
        {
            //错误打印
            MCU_LogShowErrorSubCode(errorSub);
            return errorSub;
        }
        //读取状态寄存器
        errorSub = BoardMB85RS2MT_ReadStatusReg(&statusReg);
        if(errorSub != ERROR_SUB_OK)
        {
            //错误打印
            MCU_LogShowErrorSubCode(errorSub);
            return errorSub;
        }
        //检查是否成功
        if(statusReg.regValueSingle.wel == 1)
        {
            retryCount++;
            continue;
        }
        break;
    } while (retryCount < BOARD_MB85RS2MT_WRITE_SR_RETRY_MAX);
    //检查是否成功
    if(statusReg.regValueSingle.wel == 1)
    {
        MCU_LogPrintf("MB85RS2MT StatusReg,wel: %d,bp: %d,wpen: %d\r\n",statusReg.regValueSingle.wel,
                        statusReg.regValueSingle.bp,statusReg.regValueSingle.wpen);
        errorSub = ERROR_SUB_BOARD_MB85RS2MT_WRITE_DISABLE;
        //错误打印
        MCU_LogShowErrorSubCode(errorSub);
        return errorSub;
    }
    return errorSub;
}

//写状态寄存器
static ERROR_SUB BoardMB85RS2MT_WriteStatusReg(uint8_t setRegState)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //首先写入使能
    errorSub = BoardMB85RS2MT_WriteEnable();
    if(errorSub != ERROR_SUB_OK)
    {
        //错误打印
        MCU_LogShowErrorSubCode(errorSub);
        return errorSub;
    }
    //然后写入状态寄存器的值
    errorSub = BoardMB85RS2MT_SendCmdWriteStatusReg(setRegState);
    if(errorSub != ERROR_SUB_OK)
    {
        //错误打印
        MCU_LogShowErrorSubCode(errorSub);
        return errorSub;
    }
    return errorSub;
}

//配置铁电存储器默认状态
ERROR_SUB BoardMB85RS2MT_ConfigDefault(void)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //读取ID
    uint32_t chipID = 0;
    //上锁
    BoardMB85RS2MT_Lock();
    errorSub = BoardMB85RS2MT_ReadID(&chipID);
    if(errorSub != ERROR_SUB_OK)
    {
        //解锁
        BoardMB85RS2MT_UnLock();
        //错误打印
        MCU_LogShowErrorSubCode(errorSub);
        return errorSub;
    }
    //ID需要处理一下适配不同批次芯片
    MCU_LogPrintf("BoardMB85RS2MT ChipID: 0X%08X\r\n",chipID);
    chipID &= BOARD_MB85RS2MT_ID_MASK;
    //检查ID
    if(chipID != BOARD_MB85RS2MT_ID)
    {
        //解锁
        BoardMB85RS2MT_UnLock();
        errorSub = ERROR_SUB_BOARD_MB85RS2MT_CHECK_ID;
        //错误打印
        MCU_LogShowErrorSubCode(errorSub);
        return errorSub;
    }
    //读取状态寄存器
    MB85RS2MT_STATUS_REG statusReg;
    errorSub = BoardMB85RS2MT_ReadStatusReg(&statusReg);
    if(errorSub != ERROR_SUB_OK)
    {
        //解锁
        BoardMB85RS2MT_UnLock();
        //错误打印
        MCU_LogShowErrorSubCode(errorSub);
        return errorSub;
    }
    //检查是否需要写入
    if(statusReg.regValueSingle.bp == MB85RS2MT_STATUS_REG_BP_NONE)
    {
        //解锁
        BoardMB85RS2MT_UnLock();
        return errorSub;
    }
    //需要写入
    uint16_t retryCount = 0;
    do
    {
        statusReg.regValueSingle.bp = MB85RS2MT_STATUS_REG_BP_NONE;
        statusReg.regValueSingle.wpen = 0;
        //写入状态寄存器
        errorSub = BoardMB85RS2MT_WriteStatusReg(statusReg.regValueAll);
        if(errorSub != ERROR_SUB_OK)
        {
            //解锁
            BoardMB85RS2MT_UnLock();
            //错误打印
            MCU_LogShowErrorSubCode(errorSub);
            return errorSub;
        }
        //读取状态寄存器
        errorSub = BoardMB85RS2MT_ReadStatusReg(&statusReg);
        if(errorSub != ERROR_SUB_OK)
        {
            //解锁
            BoardMB85RS2MT_UnLock();
            //错误打印
            MCU_LogShowErrorSubCode(errorSub);
            return errorSub;
        }
        //检查写入是否成功
        if(statusReg.regValueSingle.bp != MB85RS2MT_STATUS_REG_BP_NONE)
        {
            retryCount++;
            continue;
        }
        break;
    } while (retryCount < BOARD_MB85RS2MT_WRITE_SR_RETRY_MAX);
    //检查写入是否成功
    if(statusReg.regValueSingle.bp != MB85RS2MT_STATUS_REG_BP_NONE)
    {
        //解锁
        BoardMB85RS2MT_UnLock();
        errorSub = ERROR_SUB_BOARD_MB85RS2MT_CONFIG;
        //错误打印
        MCU_LogShowErrorSubCode(errorSub);
        return errorSub;
    }
    return errorSub;
}

//写缓存
ERROR_SUB BoardMB85RS2MT_WriteBuffer(uint32_t address, uint8_t *pdata, uint16_t length)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //上锁
    BoardMB85RS2MT_Lock();
    //写入使能
    errorSub = BoardMB85RS2MT_WriteEnable();
    if(errorSub != ERROR_SUB_OK)
    {
        //解锁
        BoardMB85RS2MT_UnLock();
        //错误打印
        MCU_LogShowErrorSubCode(errorSub);
        return errorSub;
    }
    //发送指令
    uint8_t writeDataBuf[4];
    writeDataBuf[0] = MB85RS2MT_CMD_WRITE;
    writeDataBuf[1] = (uint8_t)((address)>>16);
    writeDataBuf[2] = (uint8_t)((address)>>8);
    writeDataBuf[3] = (uint8_t)(address);
    //片选使能
	BoardMB85RS2MT_ChipSelect(); 
    //发送写入指令
    errorSub = BOARD_MB85RS2MT_SPI_READ_WRITE(writeDataBuf,SPI_PTR_NULL,4);       
    if(errorSub != ERROR_SUB_OK)
    {
        //释放SPI
        BoardMB85RS2MT_ChipDisSelect();
        //解锁
        BoardMB85RS2MT_UnLock();
        //错误打印
        MCU_LogShowErrorSubCode(errorSub);
        return errorSub;
    }  
    //开始写入
    errorSub = BOARD_MB85RS2MT_SPI_READ_WRITE(pdata,SPI_PTR_NULL,length);   
    if(errorSub != ERROR_SUB_OK)
    {
        //释放SPI
        BoardMB85RS2MT_ChipDisSelect();
        //解锁
        BoardMB85RS2MT_UnLock();
        //错误打印
        MCU_LogShowErrorSubCode(errorSub);
        return errorSub;
    }   
    //释放SPI
    BoardMB85RS2MT_ChipDisSelect();
    //写入完成,锁住
    errorSub = BoardMB85RS2MT_WriteDisable();
    if(errorSub != ERROR_SUB_OK)
    {
        //解锁
        BoardMB85RS2MT_UnLock();
        //错误打印
        MCU_LogShowErrorSubCode(errorSub);
        return errorSub;
    }
    //解锁
    BoardMB85RS2MT_UnLock();
    return ERROR_SUB_OK;
}

//读缓存
ERROR_SUB BoardMB85RS2MT_ReadBuffer(uint32_t address, uint8_t *pdata, uint16_t length)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //上锁
    BoardMB85RS2MT_Lock();
    //发送指令
#if(BOARD_MB85RS2MT_FAST_READ == CONFIG_FUNC_DISABLE)
    uint8_t writeDataBuf[4];
    writeDataBuf[0] = MB85RS2MT_CMD_READ;
#else
    uint8_t writeDataBuf[5];
    writeDataBuf[0] = MB85RS2MT_CMD_FAST_READ;
#endif
    writeDataBuf[1] = (uint8_t)((address)>>16);
    writeDataBuf[2] = (uint8_t)((address)>>8);
    writeDataBuf[3] = (uint8_t)(address);
#if(BOARD_MB85RS2MT_FAST_READ == CONFIG_FUNC_ENABLE)
    writeDataBuf[4] = 0XFF;
#endif
    //片选使能
	BoardMB85RS2MT_ChipSelect();
#if(BOARD_MB85RS2MT_FAST_READ == CONFIG_FUNC_DISABLE)
    //发送写入指令
    errorSub = BOARD_MB85RS2MT_SPI_READ_WRITE(writeDataBuf,SPI_PTR_NULL,4);   
#else
    //发送写入指令
    errorSub = BOARD_MB85RS2MT_SPI_READ_WRITE(writeDataBuf,SPI_PTR_NULL,5);   
#endif    
    if(errorSub != ERROR_SUB_OK)
    {
        //释放SPI
        BoardMB85RS2MT_ChipDisSelect();
        //解锁
        BoardMB85RS2MT_UnLock();
        //错误打印
        MCU_LogShowErrorSubCode(errorSub);
        return errorSub;
    }  
    //开始读取
    errorSub = BOARD_MB85RS2MT_SPI_READ_WRITE(SPI_PTR_NULL,pdata,length);   
    if(errorSub != ERROR_SUB_OK)
    {
        //释放SPI
        BoardMB85RS2MT_ChipDisSelect();
        //解锁
        BoardMB85RS2MT_UnLock();
        //错误打印
        MCU_LogShowErrorSubCode(errorSub);
        return errorSub;
    }   
    //释放SPI
    BoardMB85RS2MT_ChipDisSelect();
    //解锁
    BoardMB85RS2MT_UnLock();
    return ERROR_SUB_OK;
}

//铁电存储器自检
ERROR_SUB BoardMB85RS2MT_SelfCheck(void)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    uint32_t chipID = 0XFFFFFFFF;
    //上锁
    BoardMB85RS2MT_Lock();
    //读取ID
    errorSub = BoardMB85RS2MT_ReadID(&chipID);
    if(errorSub != ERROR_SUB_OK)
    {
        //解锁
        BoardMB85RS2MT_UnLock();
        //错误打印
        MCU_LogShowErrorSubCode(errorSub);
        return errorSub;
    }
    //ID需要处理一下适配不同批次芯片
    MCU_LogPrintf("BoardMB85RS2MT ChipID: 0X%08X\r\n",chipID);
    chipID &= BOARD_MB85RS2MT_ID_MASK;
    //检查ID
    if(chipID != BOARD_MB85RS2MT_ID)
    {
        //解锁
        BoardMB85RS2MT_UnLock();
        errorSub = ERROR_SUB_BOARD_MB85RS2MT_CHECK_ID;
        //错误打印
        MCU_LogShowErrorSubCode(errorSub);
        return errorSub;
    }
    uint32_t srcDat = 0;
    uint32_t writeDat = MCU_RandomGetNextUnsigned(0,0XFFFFFFFF);
    uint32_t readDat = 0;
    uint32_t testAddr = MCU_RandomGetNextUnsigned(0,BOARD_MB85RS2MT_SIZE-1);
    //读取源
    errorSub = BoardMB85RS2MT_ReadBuffer(testAddr,(uint8_t*)(&srcDat),4);
    if(errorSub != ERROR_SUB_OK)
    {
        //解锁
        BoardMB85RS2MT_UnLock();
        //错误打印
        MCU_LogShowErrorSubCode(errorSub);
        return errorSub;
    }
    //写入新的数据
    errorSub = BoardMB85RS2MT_WriteBuffer(testAddr,(uint8_t*)(&writeDat),4);
    if(errorSub != ERROR_SUB_OK)
    {
        //解锁
        BoardMB85RS2MT_UnLock();
        //错误打印
        MCU_LogShowErrorSubCode(errorSub);
        return errorSub;
    }
    //读取出来
    errorSub = BoardMB85RS2MT_ReadBuffer(testAddr,(uint8_t*)(&readDat),4);
    if(errorSub != ERROR_SUB_OK)
    {
        //解锁
        BoardMB85RS2MT_UnLock();
        //错误打印
        MCU_LogShowErrorSubCode(errorSub);
        return errorSub;
    }
    //比较数据
    if(readDat != writeDat)
    {
        //解锁
        BoardMB85RS2MT_UnLock();
        errorSub = ERROR_SUB_BOARD_MB85RS2MT_CHECK;
        //错误打印
        MCU_LogShowErrorSubCode(errorSub);
        return errorSub;
    }
    //原始数据写回去
    errorSub = BoardMB85RS2MT_WriteBuffer(testAddr,(uint8_t*)(&srcDat),4);
    if(errorSub != ERROR_SUB_OK)
    {
        //解锁
        BoardMB85RS2MT_UnLock();
        //错误打印
        MCU_LogShowErrorSubCode(errorSub);
        return errorSub;
    }
    //读取出来
    errorSub = BoardMB85RS2MT_ReadBuffer(testAddr,(uint8_t*)(&readDat),4);
    if(errorSub != ERROR_SUB_OK)
    {
        //解锁
        BoardMB85RS2MT_UnLock();
        //错误打印
        MCU_LogShowErrorSubCode(errorSub);
        return errorSub;
    }
    //比较数据
    if(readDat != srcDat)
    {
        //解锁
        BoardMB85RS2MT_UnLock();
        errorSub = ERROR_SUB_BOARD_MB85RS2MT_CHECK;
        //错误打印
        MCU_LogShowErrorSubCode(errorSub);
        return errorSub;
    }
    //解锁
    BoardMB85RS2MT_UnLock();
    return ERROR_SUB_OK;
}

//芯片低级格式化
ERROR_SUB BoardMB85RS2MT_EraseChip(void)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    uint32_t addrIndexUtil = 0;
    uint32_t byteIndexUtil = 0;
    //申请读写内存
    uint8_t* writeBufferPtr = MCU_MemMallocWhileSuccessSramIn(BOARD_MB85RS2MT_BUFFER_LEN);
    uint8_t* readBufferPtr =  MCU_MemMallocWhileSuccessSramIn(BOARD_MB85RS2MT_BUFFER_LEN);
    uint16_t byteLength = 0;
    uint8_t eraseFlag = 0;
    //写入全部设置为1
    UserMemSet(writeBufferPtr,0XFF,BOARD_MB85RS2MT_BUFFER_LEN);
    //上锁
    BoardMB85RS2MT_Lock();
    //循环写入
    for(addrIndexUtil = 0;addrIndexUtil < BOARD_MB85RS2MT_SIZE;)
    {
        if(BOARD_MB85RS2MT_SIZE - addrIndexUtil >= BOARD_MB85RS2MT_BUFFER_LEN)
        {
            byteLength = BOARD_MB85RS2MT_BUFFER_LEN;
        }
        else
        {
            byteLength = BOARD_MB85RS2MT_SIZE - addrIndexUtil;
        }
        //首先读取
        errorSub = BoardMB85RS2MT_ReadBuffer(addrIndexUtil,readBufferPtr,byteLength);
        if(errorSub != ERROR_SUB_OK)
        {
            //解锁
            BoardMB85RS2MT_UnLock();
            //释放内存
            MCU_MemFreeSramIn(writeBufferPtr);
            MCU_MemFreeSramIn(readBufferPtr);
            //错误打印
            MCU_LogShowErrorSubCode(errorSub);
            return errorSub;
        }
        eraseFlag = 0;
        //检查是否需要擦除标记
        for(byteIndexUtil = 0; byteIndexUtil < byteLength;byteIndexUtil++)
        {
            if(readBufferPtr[byteIndexUtil] != 0XFF)
            {
                eraseFlag = 1;
                break;
            }
        }
        //检查是否需要写入
        if(eraseFlag == 0)
        {
            //不需要写入
            addrIndexUtil += byteLength;
            //结束本次循环
            continue;
        }
        //开始写入
        errorSub = BoardMB85RS2MT_WriteBuffer(addrIndexUtil,writeBufferPtr,byteLength);
        if(errorSub != ERROR_SUB_OK)
        {
            //解锁
            BoardMB85RS2MT_UnLock();
            //释放内存
            MCU_MemFreeSramIn(writeBufferPtr);
            MCU_MemFreeSramIn(readBufferPtr);
            //错误打印
            MCU_LogShowErrorSubCode(errorSub);
            return errorSub;
        }
        //写入完成,进行下一项
        addrIndexUtil += byteLength;
    }
    //整个一边写入扫描完成,检查
    for(addrIndexUtil = 0;addrIndexUtil < BOARD_MB85RS2MT_SIZE;)
    {
        if(BOARD_MB85RS2MT_SIZE - addrIndexUtil >= BOARD_MB85RS2MT_BUFFER_LEN)
        {
            byteLength = BOARD_MB85RS2MT_BUFFER_LEN;
        }
        else
        {
            byteLength = BOARD_MB85RS2MT_SIZE - addrIndexUtil;
        }
        //首先读取
        errorSub = BoardMB85RS2MT_ReadBuffer(addrIndexUtil,readBufferPtr,byteLength);
        if(errorSub != ERROR_SUB_OK)
        {
            //解锁
            BoardMB85RS2MT_UnLock();
            //释放内存
            MCU_MemFreeSramIn(writeBufferPtr);
            MCU_MemFreeSramIn(readBufferPtr);
            //错误打印
            MCU_LogShowErrorSubCode(errorSub);
            return errorSub;
        }
        for(byteIndexUtil = 0; byteIndexUtil < byteLength;byteIndexUtil++)
        {
            if(readBufferPtr[byteIndexUtil] != 0XFF)
            {
                //擦除失败
                //解锁
                BoardMB85RS2MT_UnLock();
                errorSub = ERROR_SUB_BOARD_MB85RS2MT_ERASE;
                //释放内存
                MCU_MemFreeSramIn(writeBufferPtr);
                MCU_MemFreeSramIn(readBufferPtr);
                //错误打印
                MCU_LogShowErrorSubCode(errorSub);
                return errorSub;
            }
        }
        //进行下一项
        addrIndexUtil += byteLength;
    }
    //解锁
    BoardMB85RS2MT_UnLock();
    //释放内存
    MCU_MemFreeSramIn(writeBufferPtr);
    MCU_MemFreeSramIn(readBufferPtr);
    return ERROR_SUB_OK;
}












