/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-09-15 09:57:20 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-09-18 16:33:29 +0800
************************************************************************************************/ 
#include "BoardTCA9555PWR.h"
#include "tx_mutex.h"

//等待设备空闲次数
#define TCA9555_PWR_WAIT_IDLE_COUNT             (TIME_MS_MAX_TCA9555PWR_WAIT_IDLE/BOARD_TCA9555PWR_LOW_LEVEL_SINGLE_TIMEOUT_MS)

//获取设备互斥量
static void BoardTCA9555PWR_GetLock(TX_MUTEX* mutexPtr)
{
    //申请设备互斥信号量
    tx_mutex_get(mutexPtr,TX_WAIT_FOREVER);
}

//释放设备互斥量
static void BoardTCA9555PWR_ReleaseLock(TX_MUTEX* mutexPtr)
{
    //申请设备互斥信号量
    tx_mutex_put(mutexPtr);
}

//初始化
void BoardTCA9555PWR_Init(TX_MUTEX* mutexPtr,uint8_t* mutexName)
{
    if(mutexPtr->tx_mutex_id == TX_MUTEX_ID)
    {
        //已经初始化过了
        return;
    }
    //创建设备互斥信号量
    tx_mutex_create(mutexPtr,(CHAR*)mutexName,TX_INHERIT);
    //端口初始化
    BOARD_TCA9555PWR_LOW_LEVEL_PORT_INIT();
}

//查询设备是否空闲,通过检查器件应答的方式，判断内部写操作是否完成, 一般小于 10ms
//CLK频率为200KHz时,查询次数为30次左右
ERROR_SUB BoardTCA9555PWR_WaitIdle(uint8_t deviceAddr,TX_MUTEX* mutexPtr,uint16_t retryCount)
{
    HAL_StatusTypeDef halState = HAL_OK;
    //申请设备互斥信号量
    BoardTCA9555PWR_GetLock(mutexPtr);
    while(retryCount != 0)
    {
        halState = BOARD_TCA9555PWR_LOW_LEVEL_CHECK_READY(deviceAddr);
        if(halState == HAL_OK)
        {
            //释放设备互斥信号量
            BoardTCA9555PWR_ReleaseLock(mutexPtr);
            return ERROR_SUB_OK;
        }
        //还要等
        retryCount--;
    }
    //释放设备互斥信号量
    BoardTCA9555PWR_ReleaseLock(mutexPtr);
    //显示错误
    MCU_LogPrintf("%s TimeOut\r\n",__func__);
    return ERROR_SUB_BOARD_TCA9555PWR_WAIT_IDLE;
}

//设置全部极性
ERROR_SUB BoardTCA9555PWR_SetAllPolarity(uint8_t deviceAddr,TX_MUTEX* mutexPtr,TCA9555_PORT_POLARITY_SET* polaritySetPtr)
{
    HAL_StatusTypeDef halState = HAL_OK;
    uint8_t writeBufArray[2];
    writeBufArray[0] = (uint8_t)(polaritySetPtr->regVal2Byte);
    writeBufArray[1] = (uint8_t)((polaritySetPtr->regVal2Byte)>>8);
    //申请设备互斥信号量
    BoardTCA9555PWR_GetLock(mutexPtr);
    //检查空闲
    if(ERROR_SUB_OK != BoardTCA9555PWR_WaitIdle(deviceAddr,mutexPtr,TCA9555_PWR_WAIT_IDLE_COUNT))
    {
        //没检查到空闲,释放设备互斥信号量
        BoardTCA9555PWR_ReleaseLock(mutexPtr);
        return ERROR_SUB_BOARD_TCA9555PWR_WAIT_IDLE;
    }
    halState = BOARD_TCA9555PWR_LOW_LEVEL_WRITE(deviceAddr,IIC_SUB_ADDR_SIZE_8BIT,REG_ADDR_TCA9555_POLARITY_VAL_LOW,writeBufArray,2,
                                                TIME_MS_MAX_TCA9555PWR_WAIT_IDLE_BYTE);
    if(halState != HAL_OK)
    {
        MCU_LogShowHalState(halState,0);
        //如果发生错误,不需要等待有效,直接释放设备互斥量
        BoardTCA9555PWR_ReleaseLock(mutexPtr);
        return ERROR_SUB_BOARD_TCA9555PWR_WRITE_POLARITY;
    }
    //检查空闲
    if(ERROR_SUB_OK != BoardTCA9555PWR_WaitIdle(deviceAddr,mutexPtr,TCA9555_PWR_WAIT_IDLE_COUNT))
    {
        //没检查到空闲,释放设备互斥信号量
        BoardTCA9555PWR_ReleaseLock(mutexPtr);
        return ERROR_SUB_BOARD_TCA9555PWR_WAIT_IDLE;
    }
    //完成,空闲
    BoardTCA9555PWR_ReleaseLock(mutexPtr);
    return ERROR_SUB_OK;
}

//读取极性设定
static ERROR_SUB BoardTCA9555PWR_GetAllPolarity(uint8_t deviceAddr,TX_MUTEX* mutexPtr,TCA9555_PORT_POLARITY_SET* polaritySetPtr)
{
    HAL_StatusTypeDef halState = HAL_OK;
    uint8_t readBufArray[2];
    //申请设备互斥信号量
    BoardTCA9555PWR_GetLock(mutexPtr);
    //检查空闲
    if(ERROR_SUB_OK != BoardTCA9555PWR_WaitIdle(deviceAddr,mutexPtr,TCA9555_PWR_WAIT_IDLE_COUNT))
    {
        //没检查到空闲,释放设备互斥信号量
        BoardTCA9555PWR_ReleaseLock(mutexPtr);
        return ERROR_SUB_BOARD_TCA9555PWR_WAIT_IDLE;
    }
    halState = BOARD_TCA9555PWR_LOW_LEVEL_READ(deviceAddr,IIC_SUB_ADDR_SIZE_8BIT,REG_ADDR_TCA9555_POLARITY_VAL_LOW,readBufArray,2,
                                                TIME_MS_MAX_TCA9555PWR_WAIT_IDLE_BYTE);
    //完成,空闲
    BoardTCA9555PWR_ReleaseLock(mutexPtr);
    if(halState != HAL_OK)
    {
        MCU_LogShowHalState(halState,0);
        return ERROR_SUB_BOARD_TCA9555PWR_READ_PORT_POLARITY;
    }
    //数据组合
    polaritySetPtr->regVal2Byte = 0X0000;
    polaritySetPtr->regVal2Byte += readBufArray[1];
    polaritySetPtr->regVal2Byte <<= 8;
    polaritySetPtr->regVal2Byte += readBufArray[0];
    return ERROR_SUB_OK;
}

//设置单个极性
ERROR_SUB BoardTCA9555PWR_SetSinglePolarity(uint8_t deviceAddr,TX_MUTEX* mutexPtr,TCA9555_PIN pin,TCA9555_PORT_POLARITY polaritySet)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    TCA9555_PORT_POLARITY_SET portPolaritySet;
    //申请设备互斥信号量
    BoardTCA9555PWR_GetLock(mutexPtr);
    //读取全部输出
    errorSub = BoardTCA9555PWR_GetAllPolarity(deviceAddr,mutexPtr,&portPolaritySet);
    if(errorSub != ERROR_SUB_OK)
    {
        //释放互斥信号量
        BoardTCA9555PWR_ReleaseLock(mutexPtr);
        return errorSub;
    }
    switch(pin)
    {
        case TCA9555_PIN_1: 
            if(portPolaritySet.regValueSingle.pin1Polarity == polaritySet)
            {
                //释放互斥信号量
                BoardTCA9555PWR_ReleaseLock(mutexPtr);
                return errorSub;
            }
            else
            {
                portPolaritySet.regValueSingle.pin1Polarity = polaritySet;
            }
            break;
        case TCA9555_PIN_2: 
            if(portPolaritySet.regValueSingle.pin2Polarity == polaritySet)
            {
                //释放互斥信号量
                BoardTCA9555PWR_ReleaseLock(mutexPtr);
                return errorSub;
            }
            else
            {
                portPolaritySet.regValueSingle.pin2Polarity = polaritySet;
            }
            break;
        case TCA9555_PIN_3: 
            if(portPolaritySet.regValueSingle.pin3Polarity == polaritySet)
            {
                //释放互斥信号量
                BoardTCA9555PWR_ReleaseLock(mutexPtr);
                return errorSub;
            }
            else
            {
                portPolaritySet.regValueSingle.pin3Polarity = polaritySet;
            }
            break;
        case TCA9555_PIN_4: 
            if(portPolaritySet.regValueSingle.pin4Polarity == polaritySet)
            {
                //释放互斥信号量
                BoardTCA9555PWR_ReleaseLock(mutexPtr);
                return errorSub;
            }
            else
            {
                portPolaritySet.regValueSingle.pin4Polarity = polaritySet;
            }
            break;
        case TCA9555_PIN_5: 
            if(portPolaritySet.regValueSingle.pin5Polarity == polaritySet)
            {
                //释放互斥信号量
                BoardTCA9555PWR_ReleaseLock(mutexPtr);
                return errorSub;
            }
            else
            {
                portPolaritySet.regValueSingle.pin5Polarity = polaritySet;
            }
            break;
        case TCA9555_PIN_6: 
            if(portPolaritySet.regValueSingle.pin6Polarity == polaritySet)
            {
                //释放互斥信号量
                BoardTCA9555PWR_ReleaseLock(mutexPtr);
                return errorSub;
            }
            else
            {
                portPolaritySet.regValueSingle.pin6Polarity = polaritySet;
            }
            break;
        case TCA9555_PIN_7: 
            if(portPolaritySet.regValueSingle.pin7Polarity == polaritySet)
            {
                //释放互斥信号量
                BoardTCA9555PWR_ReleaseLock(mutexPtr);
                return errorSub;
            }
            else
            {
                portPolaritySet.regValueSingle.pin7Polarity = polaritySet;
            }
            break;
        case TCA9555_PIN_8: 
            if(portPolaritySet.regValueSingle.pin8Polarity == polaritySet)
            {
                //释放互斥信号量
                BoardTCA9555PWR_ReleaseLock(mutexPtr);
                return errorSub;
            }
            else
            {
                portPolaritySet.regValueSingle.pin8Polarity = polaritySet;
            }
            break;
        case TCA9555_PIN_9: 
            if(portPolaritySet.regValueSingle.pin9Polarity == polaritySet)
            {
                //释放互斥信号量
                BoardTCA9555PWR_ReleaseLock(mutexPtr);
                return errorSub;
            }
            else
            {
                portPolaritySet.regValueSingle.pin9Polarity = polaritySet;
            }
            break;
        case TCA9555_PIN_10: 
            if(portPolaritySet.regValueSingle.pin10Polarity == polaritySet)
            {
                //释放互斥信号量
                BoardTCA9555PWR_ReleaseLock(mutexPtr);
                return errorSub;
            }
            else
            {
                portPolaritySet.regValueSingle.pin10Polarity = polaritySet;
            }
            break;
        case TCA9555_PIN_11: 
            if(portPolaritySet.regValueSingle.pin11Polarity == polaritySet)
            {
                //释放互斥信号量
                BoardTCA9555PWR_ReleaseLock(mutexPtr);
                return errorSub;
            }
            else
            {
                portPolaritySet.regValueSingle.pin11Polarity = polaritySet;
            }
            break;
        case TCA9555_PIN_12: 
            if(portPolaritySet.regValueSingle.pin12Polarity == polaritySet)
            {
                //释放互斥信号量
                BoardTCA9555PWR_ReleaseLock(mutexPtr);
                return errorSub;
            }
            else
            {
                portPolaritySet.regValueSingle.pin12Polarity = polaritySet;
            }
            break;
        case TCA9555_PIN_13: 
            if(portPolaritySet.regValueSingle.pin13Polarity == polaritySet)
            {
                //释放互斥信号量
                BoardTCA9555PWR_ReleaseLock(mutexPtr);
                return errorSub;
            }
            else
            {
                portPolaritySet.regValueSingle.pin13Polarity = polaritySet;
            }
            break;
        case TCA9555_PIN_14: 
            if(portPolaritySet.regValueSingle.pin14Polarity == polaritySet)
            {
                //释放互斥信号量
                BoardTCA9555PWR_ReleaseLock(mutexPtr);
                return errorSub;
            }
            else
            {
                portPolaritySet.regValueSingle.pin14Polarity = polaritySet;
            }
            break;
        case TCA9555_PIN_15: 
            if(portPolaritySet.regValueSingle.pin15Polarity == polaritySet)
            {
                //释放互斥信号量
                BoardTCA9555PWR_ReleaseLock(mutexPtr);
                return errorSub;
            }
            else
            {
                portPolaritySet.regValueSingle.pin15Polarity = polaritySet;
            }
            break;
        case TCA9555_PIN_16: 
            if(portPolaritySet.regValueSingle.pin16Polarity == polaritySet)
            {
                //释放互斥信号量
                BoardTCA9555PWR_ReleaseLock(mutexPtr);
                return errorSub;
            }
            else
            {
                portPolaritySet.regValueSingle.pin16Polarity = polaritySet;
            }
            break;
    }
    //到这里标志还有一次写入
    errorSub = BoardTCA9555PWR_SetAllPolarity(deviceAddr,mutexPtr,&portPolaritySet);
    //释放互斥信号量
    BoardTCA9555PWR_ReleaseLock(mutexPtr);
    return errorSub;
}

//设置全部方向
ERROR_SUB BoardTCA9555PWR_SetAllDir(uint8_t deviceAddr,TX_MUTEX* mutexPtr,TCA9555_PORT_DIR_SET* dirSetPtr)
{
    HAL_StatusTypeDef halState = HAL_OK;
    uint8_t writeBufArray[2];
    writeBufArray[0] = (uint8_t)(dirSetPtr->regVal2Byte);
    writeBufArray[1] = (uint8_t)((dirSetPtr->regVal2Byte)>>8);
    //申请设备互斥信号量
    BoardTCA9555PWR_GetLock(mutexPtr);
    //检查空闲
    if(ERROR_SUB_OK != BoardTCA9555PWR_WaitIdle(deviceAddr,mutexPtr,TCA9555_PWR_WAIT_IDLE_COUNT))
    {
        //没检查到空闲,释放设备互斥信号量
        BoardTCA9555PWR_ReleaseLock(mutexPtr);
        return ERROR_SUB_BOARD_TCA9555PWR_WAIT_IDLE;
    }
    halState = BOARD_TCA9555PWR_LOW_LEVEL_WRITE(deviceAddr,IIC_SUB_ADDR_SIZE_8BIT,REG_ADDR_TCA9555_DIR_VAL_LOW,writeBufArray,2,
                                                TIME_MS_MAX_TCA9555PWR_WAIT_IDLE_BYTE);
    if(halState != HAL_OK)
    {
        MCU_LogShowHalState(halState,0);
        //如果发生错误,不需要等待有效,直接释放设备互斥量
        BoardTCA9555PWR_ReleaseLock(mutexPtr);
        return ERROR_SUB_BOARD_TCA9555PWR_WRITE_DIR;
    }
    //检查空闲
    if(ERROR_SUB_OK != BoardTCA9555PWR_WaitIdle(deviceAddr,mutexPtr,TCA9555_PWR_WAIT_IDLE_COUNT))
    {
        //没检查到空闲,释放设备互斥信号量
        BoardTCA9555PWR_ReleaseLock(mutexPtr);
        return ERROR_SUB_BOARD_TCA9555PWR_WAIT_IDLE;
    }
    //完成,空闲
    BoardTCA9555PWR_ReleaseLock(mutexPtr);
    return ERROR_SUB_OK;
}

//读取方向设定
static ERROR_SUB BoardTCA9555PWR_GetAllDir(uint8_t deviceAddr,TX_MUTEX* mutexPtr,TCA9555_PORT_DIR_SET* dirSetPtr)
{
    HAL_StatusTypeDef halState = HAL_OK;
    uint8_t readBufArray[2];
    //申请设备互斥信号量
    BoardTCA9555PWR_GetLock(mutexPtr);
    //检查空闲
    if(ERROR_SUB_OK != BoardTCA9555PWR_WaitIdle(deviceAddr,mutexPtr,TCA9555_PWR_WAIT_IDLE_COUNT))
    {
        //没检查到空闲,释放设备互斥信号量
        BoardTCA9555PWR_ReleaseLock(mutexPtr);
        return ERROR_SUB_BOARD_TCA9555PWR_WAIT_IDLE;
    }
    halState = BOARD_TCA9555PWR_LOW_LEVEL_READ(deviceAddr,IIC_SUB_ADDR_SIZE_8BIT,REG_ADDR_TCA9555_DIR_VAL_LOW,readBufArray,2,
                                                TIME_MS_MAX_TCA9555PWR_WAIT_IDLE_BYTE);
    //完成,空闲
    BoardTCA9555PWR_ReleaseLock(mutexPtr);
    if(halState != HAL_OK)
    {
        MCU_LogShowHalState(halState,0);
        return ERROR_SUB_BOARD_TCA9555PWR_READ_PORT_DIR;
    }
    //数据组合
    dirSetPtr->regVal2Byte = 0X0000;
    dirSetPtr->regVal2Byte += readBufArray[1];
    dirSetPtr->regVal2Byte <<= 8;
    dirSetPtr->regVal2Byte += readBufArray[0];
    return ERROR_SUB_OK;
}

//设置单个方向
ERROR_SUB BoardTCA9555PWR_SetSingleDir(uint8_t deviceAddr,TX_MUTEX* mutexPtr,TCA9555_PIN pin,TCA9555_PORT_DIR dirSet)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    TCA9555_PORT_DIR_SET portDirSet;
    //申请设备互斥信号量
    BoardTCA9555PWR_GetLock(mutexPtr);
    //读取全部输出
    errorSub = BoardTCA9555PWR_GetAllDir(deviceAddr,mutexPtr,&portDirSet);
    if(errorSub != ERROR_SUB_OK)
    {
        //释放互斥信号量
        BoardTCA9555PWR_ReleaseLock(mutexPtr);
        return errorSub;
    }
    switch(pin)
    {
        case TCA9555_PIN_1: 
            if(portDirSet.regValueSingle.pin1Dir == dirSet)
            {
                //释放互斥信号量
                BoardTCA9555PWR_ReleaseLock(mutexPtr);
                return errorSub;
            }
            else
            {
                portDirSet.regValueSingle.pin1Dir = dirSet;
            }
            break;
        case TCA9555_PIN_2: 
            if(portDirSet.regValueSingle.pin2Dir == dirSet)
            {
                //释放互斥信号量
                BoardTCA9555PWR_ReleaseLock(mutexPtr);
                return errorSub;
            }
            else
            {
                portDirSet.regValueSingle.pin2Dir = dirSet;
            }
            break;
        case TCA9555_PIN_3: 
            if(portDirSet.regValueSingle.pin3Dir == dirSet)
            {
                //释放互斥信号量
                BoardTCA9555PWR_ReleaseLock(mutexPtr);
                return errorSub;
            }
            else
            {
                portDirSet.regValueSingle.pin3Dir = dirSet;
            }
            break;
        case TCA9555_PIN_4: 
            if(portDirSet.regValueSingle.pin4Dir == dirSet)
            {
                //释放互斥信号量
                BoardTCA9555PWR_ReleaseLock(mutexPtr);
                return errorSub;
            }
            else
            {
                portDirSet.regValueSingle.pin4Dir = dirSet;
            }
            break;
        case TCA9555_PIN_5: 
            if(portDirSet.regValueSingle.pin5Dir == dirSet)
            {
                //释放互斥信号量
                BoardTCA9555PWR_ReleaseLock(mutexPtr);
                return errorSub;
            }
            else
            {
                portDirSet.regValueSingle.pin5Dir = dirSet;
            }
            break;
        case TCA9555_PIN_6: 
            if(portDirSet.regValueSingle.pin6Dir == dirSet)
            {
                //释放互斥信号量
                BoardTCA9555PWR_ReleaseLock(mutexPtr);
                return errorSub;
            }
            else
            {
                portDirSet.regValueSingle.pin6Dir = dirSet;
            }
            break;
        case TCA9555_PIN_7: 
            if(portDirSet.regValueSingle.pin7Dir == dirSet)
            {
                //释放互斥信号量
                BoardTCA9555PWR_ReleaseLock(mutexPtr);
                return errorSub;
            }
            else
            {
                portDirSet.regValueSingle.pin7Dir = dirSet;
            }
            break;
        case TCA9555_PIN_8: 
            if(portDirSet.regValueSingle.pin8Dir == dirSet)
            {
                //释放互斥信号量
                BoardTCA9555PWR_ReleaseLock(mutexPtr);
                return errorSub;
            }
            else
            {
                portDirSet.regValueSingle.pin8Dir = dirSet;
            }
            break;
        case TCA9555_PIN_9: 
            if(portDirSet.regValueSingle.pin9Dir == dirSet)
            {
                //释放互斥信号量
                BoardTCA9555PWR_ReleaseLock(mutexPtr);
                return errorSub;
            }
            else
            {
                portDirSet.regValueSingle.pin9Dir = dirSet;
            }
            break;
        case TCA9555_PIN_10: 
            if(portDirSet.regValueSingle.pin10Dir == dirSet)
            {
                //释放互斥信号量
                BoardTCA9555PWR_ReleaseLock(mutexPtr);
                return errorSub;
            }
            else
            {
                portDirSet.regValueSingle.pin10Dir = dirSet;
            }
            break;
        case TCA9555_PIN_11: 
            if(portDirSet.regValueSingle.pin11Dir == dirSet)
            {
                //释放互斥信号量
                BoardTCA9555PWR_ReleaseLock(mutexPtr);
                return errorSub;
            }
            else
            {
                portDirSet.regValueSingle.pin11Dir = dirSet;
            }
            break;
        case TCA9555_PIN_12: 
            if(portDirSet.regValueSingle.pin12Dir == dirSet)
            {
                //释放互斥信号量
                BoardTCA9555PWR_ReleaseLock(mutexPtr);
                return errorSub;
            }
            else
            {
                portDirSet.regValueSingle.pin12Dir = dirSet;
            }
            break;
        case TCA9555_PIN_13: 
            if(portDirSet.regValueSingle.pin13Dir == dirSet)
            {
                //释放互斥信号量
                BoardTCA9555PWR_ReleaseLock(mutexPtr);
                return errorSub;
            }
            else
            {
                portDirSet.regValueSingle.pin13Dir = dirSet;
            }
            break;
        case TCA9555_PIN_14: 
            if(portDirSet.regValueSingle.pin14Dir == dirSet)
            {
                //释放互斥信号量
                BoardTCA9555PWR_ReleaseLock(mutexPtr);
                return errorSub;
            }
            else
            {
                portDirSet.regValueSingle.pin14Dir = dirSet;
            }
            break;
        case TCA9555_PIN_15: 
            if(portDirSet.regValueSingle.pin15Dir == dirSet)
            {
                //释放互斥信号量
                BoardTCA9555PWR_ReleaseLock(mutexPtr);
                return errorSub;
            }
            else
            {
                portDirSet.regValueSingle.pin15Dir = dirSet;
            }
            break;
        case TCA9555_PIN_16: 
            if(portDirSet.regValueSingle.pin16Dir == dirSet)
            {
                //释放互斥信号量
                BoardTCA9555PWR_ReleaseLock(mutexPtr);
                return errorSub;
            }
            else
            {
                portDirSet.regValueSingle.pin16Dir = dirSet;
            }
            break;
    }
    //到这里标志还有一次写入
    errorSub = BoardTCA9555PWR_SetAllDir(deviceAddr,mutexPtr,&portDirSet);
    //释放互斥信号量
    BoardTCA9555PWR_ReleaseLock(mutexPtr);
    return errorSub;
}

//写入一组输出
ERROR_SUB BoardTCA9555PWR_WriteAllPortOut(uint8_t deviceAddr,TX_MUTEX* mutexPtr,TCA9555_PORT_VAL_SET* valSetPtr)
{
    HAL_StatusTypeDef halState = HAL_OK;
    uint8_t writeBufArray[2];
    writeBufArray[0] = (uint8_t)(valSetPtr->regVal2Byte);
    writeBufArray[1] = (uint8_t)((valSetPtr->regVal2Byte)>>8);
    //申请设备互斥信号量
    BoardTCA9555PWR_GetLock(mutexPtr);
    //检查空闲
    if(ERROR_SUB_OK != BoardTCA9555PWR_WaitIdle(deviceAddr,mutexPtr,TCA9555_PWR_WAIT_IDLE_COUNT))
    {
        //没检查到空闲,释放设备互斥信号量
        BoardTCA9555PWR_ReleaseLock(mutexPtr);
        return ERROR_SUB_BOARD_TCA9555PWR_WAIT_IDLE;
    }
    halState = BOARD_TCA9555PWR_LOW_LEVEL_WRITE(deviceAddr,IIC_SUB_ADDR_SIZE_8BIT,REG_ADDR_TCA9555_OUTPUT_VAL_LOW,writeBufArray,2,
                                                TIME_MS_MAX_TCA9555PWR_WAIT_IDLE_BYTE);
    if(halState != HAL_OK)
    {
        MCU_LogShowHalState(halState,0);
        //如果发生错误,不需要等待有效,直接释放设备互斥量
        BoardTCA9555PWR_ReleaseLock(mutexPtr);
        return ERROR_SUB_BOARD_TCA9555PWR_WRITE_PORT_OUT;
    }
    //检查空闲
    if(ERROR_SUB_OK != BoardTCA9555PWR_WaitIdle(deviceAddr,mutexPtr,TCA9555_PWR_WAIT_IDLE_COUNT))
    {
        //没检查到空闲,释放设备互斥信号量
        BoardTCA9555PWR_ReleaseLock(mutexPtr);
        return ERROR_SUB_BOARD_TCA9555PWR_WAIT_IDLE;
    }
    //完成,空闲
    BoardTCA9555PWR_ReleaseLock(mutexPtr);
    return ERROR_SUB_OK;
}

//读取当前输出状态
ERROR_SUB BoardTCA9555PWR_ReadAllPortOutCurrent(uint8_t deviceAddr,TX_MUTEX* mutexPtr,TCA9555_PORT_VAL_SET* valSetPtr)
{
    HAL_StatusTypeDef halState = HAL_OK;
    uint8_t readBufArray[2];
    //申请设备互斥信号量
    BoardTCA9555PWR_GetLock(mutexPtr);
    //检查空闲
    if(ERROR_SUB_OK != BoardTCA9555PWR_WaitIdle(deviceAddr,mutexPtr,TCA9555_PWR_WAIT_IDLE_COUNT))
    {
        //没检查到空闲,释放设备互斥信号量
        BoardTCA9555PWR_ReleaseLock(mutexPtr);
        return ERROR_SUB_BOARD_TCA9555PWR_WAIT_IDLE;
    }
    halState = BOARD_TCA9555PWR_LOW_LEVEL_READ(deviceAddr,IIC_SUB_ADDR_SIZE_8BIT,REG_ADDR_TCA9555_OUTPUT_VAL_LOW,readBufArray,2,
                                                TIME_MS_MAX_TCA9555PWR_WAIT_IDLE_BYTE);
    //完成,空闲
    BoardTCA9555PWR_ReleaseLock(mutexPtr);
    if(halState != HAL_OK)
    {
        MCU_LogShowHalState(halState,0);
        return ERROR_SUB_BOARD_TCA9555PWR_READ_PORT_OUT;
    }
    //数据组合
    valSetPtr->regVal2Byte = 0X0000;
    valSetPtr->regVal2Byte += readBufArray[1];
    valSetPtr->regVal2Byte <<= 8;
    valSetPtr->regVal2Byte += readBufArray[0];
    return ERROR_SUB_OK;
}

//读取当前单个输出
ERROR_SUB BoardTCA9555PWR_ReadSinglePortOutCurrent(uint8_t deviceAddr,TX_MUTEX* mutexPtr,TCA9555_PIN pin,GPIO_PinState* valSet)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    TCA9555_PORT_VAL_SET portValSet;
    //申请设备互斥信号量
    BoardTCA9555PWR_GetLock(mutexPtr);
    //读取全部输出
    errorSub = BoardTCA9555PWR_ReadAllPortOutCurrent(deviceAddr,mutexPtr,&portValSet);
    if(errorSub != ERROR_SUB_OK)
    {
        //释放互斥信号量
        BoardTCA9555PWR_ReleaseLock(mutexPtr);
        return errorSub;
    }
    //释放互斥信号量
    BoardTCA9555PWR_ReleaseLock(mutexPtr);
    switch(pin)
    {
        case TCA9555_PIN_1: 
            *valSet = portValSet.regValueSingle.pin1Value;
            break;
        case TCA9555_PIN_2: 
            *valSet = portValSet.regValueSingle.pin2Value;
            break;
        case TCA9555_PIN_3: 
            *valSet = portValSet.regValueSingle.pin3Value;
            break;
        case TCA9555_PIN_4: 
            *valSet = portValSet.regValueSingle.pin4Value;
            break;
        case TCA9555_PIN_5: 
            *valSet = portValSet.regValueSingle.pin5Value;
            break;
        case TCA9555_PIN_6: 
            *valSet = portValSet.regValueSingle.pin6Value;
            break;
        case TCA9555_PIN_7: 
            *valSet = portValSet.regValueSingle.pin7Value;
            break;
        case TCA9555_PIN_8: 
            *valSet = portValSet.regValueSingle.pin8Value;
            break;
        case TCA9555_PIN_9: 
            *valSet = portValSet.regValueSingle.pin9Value;
            break;
        case TCA9555_PIN_10: 
            *valSet = portValSet.regValueSingle.pin10Value;
            break;
        case TCA9555_PIN_11: 
            *valSet = portValSet.regValueSingle.pin11Value;
            break;
        case TCA9555_PIN_12: 
            *valSet = portValSet.regValueSingle.pin12Value;
            break;
        case TCA9555_PIN_13: 
            *valSet = portValSet.regValueSingle.pin13Value;
            break;
        case TCA9555_PIN_14: 
            *valSet = portValSet.regValueSingle.pin14Value;
            break;
        case TCA9555_PIN_15: 
            *valSet = portValSet.regValueSingle.pin15Value;
            break;
        case TCA9555_PIN_16: 
            *valSet = portValSet.regValueSingle.pin16Value;
            break;
    }
    return ERROR_SUB_OK;
}

//写入单个输出
ERROR_SUB BoardTCA9555PWR_WriteSinglePortOut(uint8_t deviceAddr,TX_MUTEX* mutexPtr,TCA9555_PIN pin,GPIO_PinState valSet)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    TCA9555_PORT_VAL_SET portValSet;
    //申请设备互斥信号量
    BoardTCA9555PWR_GetLock(mutexPtr);
    //读取全部输出
    errorSub = BoardTCA9555PWR_ReadAllPortOutCurrent(deviceAddr,mutexPtr,&portValSet);
    if(errorSub != ERROR_SUB_OK)
    {
        //释放互斥信号量
        BoardTCA9555PWR_ReleaseLock(mutexPtr);
        return errorSub;
    }
    switch(pin)
    {
        case TCA9555_PIN_1: 
            if(portValSet.regValueSingle.pin1Value == valSet)
            {
                //释放互斥信号量
                BoardTCA9555PWR_ReleaseLock(mutexPtr);
                return errorSub;
            }
            else
            {
                portValSet.regValueSingle.pin1Value = valSet;
            }
            break;
        case TCA9555_PIN_2: 
            if(portValSet.regValueSingle.pin2Value == valSet)
            {
                //释放互斥信号量
                BoardTCA9555PWR_ReleaseLock(mutexPtr);
                return errorSub;
            }
            else
            {
                portValSet.regValueSingle.pin2Value = valSet;
            }
            break;
        case TCA9555_PIN_3: 
            if(portValSet.regValueSingle.pin3Value == valSet)
            {
                //释放互斥信号量
                BoardTCA9555PWR_ReleaseLock(mutexPtr);
                return errorSub;
            }
            else
            {
                portValSet.regValueSingle.pin3Value = valSet;
            }
            break;
        case TCA9555_PIN_4: 
            if(portValSet.regValueSingle.pin4Value == valSet)
            {
                //释放互斥信号量
                BoardTCA9555PWR_ReleaseLock(mutexPtr);
                return errorSub;
            }
            else
            {
                portValSet.regValueSingle.pin4Value = valSet;
            }
            break;
        case TCA9555_PIN_5: 
            if(portValSet.regValueSingle.pin5Value == valSet)
            {
                //释放互斥信号量
                BoardTCA9555PWR_ReleaseLock(mutexPtr);
                return errorSub;
            }
            else
            {
                portValSet.regValueSingle.pin5Value = valSet;
            }
            break;
        case TCA9555_PIN_6: 
            if(portValSet.regValueSingle.pin6Value == valSet)
            {
                //释放互斥信号量
                BoardTCA9555PWR_ReleaseLock(mutexPtr);
                return errorSub;
            }
            else
            {
                portValSet.regValueSingle.pin6Value = valSet;
            }
            break;
        case TCA9555_PIN_7: 
            if(portValSet.regValueSingle.pin7Value == valSet)
            {
                //释放互斥信号量
                BoardTCA9555PWR_ReleaseLock(mutexPtr);
                return errorSub;
            }
            else
            {
                portValSet.regValueSingle.pin7Value = valSet;
            }
            break;
        case TCA9555_PIN_8: 
            if(portValSet.regValueSingle.pin8Value == valSet)
            {
                //释放互斥信号量
                BoardTCA9555PWR_ReleaseLock(mutexPtr);
                return errorSub;
            }
            else
            {
                portValSet.regValueSingle.pin8Value = valSet;
            }
            break;
        case TCA9555_PIN_9: 
            if(portValSet.regValueSingle.pin9Value == valSet)
            {
                //释放互斥信号量
                BoardTCA9555PWR_ReleaseLock(mutexPtr);
                return errorSub;
            }
            else
            {
                portValSet.regValueSingle.pin9Value = valSet;
            }
            break;
        case TCA9555_PIN_10: 
            if(portValSet.regValueSingle.pin10Value == valSet)
            {
                //释放互斥信号量
                BoardTCA9555PWR_ReleaseLock(mutexPtr);
                return errorSub;
            }
            else
            {
                portValSet.regValueSingle.pin10Value = valSet;
            }
            break;
        case TCA9555_PIN_11: 
            if(portValSet.regValueSingle.pin11Value == valSet)
            {
                //释放互斥信号量
                BoardTCA9555PWR_ReleaseLock(mutexPtr);
                return errorSub;
            }
            else
            {
                portValSet.regValueSingle.pin11Value = valSet;
            }
            break;
        case TCA9555_PIN_12: 
            if(portValSet.regValueSingle.pin12Value == valSet)
            {
                //释放互斥信号量
                BoardTCA9555PWR_ReleaseLock(mutexPtr);
                return errorSub;
            }
            else
            {
                portValSet.regValueSingle.pin12Value = valSet;
            }
            break;
        case TCA9555_PIN_13: 
            if(portValSet.regValueSingle.pin13Value == valSet)
            {
                //释放互斥信号量
                BoardTCA9555PWR_ReleaseLock(mutexPtr);
                return errorSub;
            }
            else
            {
                portValSet.regValueSingle.pin13Value = valSet;
            }
            break;
        case TCA9555_PIN_14: 
            if(portValSet.regValueSingle.pin14Value == valSet)
            {
                //释放互斥信号量
                BoardTCA9555PWR_ReleaseLock(mutexPtr);
                return errorSub;
            }
            else
            {
                portValSet.regValueSingle.pin14Value = valSet;
            }
            break;
        case TCA9555_PIN_15: 
            if(portValSet.regValueSingle.pin15Value == valSet)
            {
                //释放互斥信号量
                BoardTCA9555PWR_ReleaseLock(mutexPtr);
                return errorSub;
            }
            else
            {
                portValSet.regValueSingle.pin15Value = valSet;
            }
            break;
        case TCA9555_PIN_16: 
            if(portValSet.regValueSingle.pin16Value == valSet)
            {
                //释放互斥信号量
                BoardTCA9555PWR_ReleaseLock(mutexPtr);
                return errorSub;
            }
            else
            {
                portValSet.regValueSingle.pin16Value = valSet;
            }
            break;
    }
    //到这里标志还有一次写入
    errorSub = BoardTCA9555PWR_WriteAllPortOut(deviceAddr,mutexPtr,&portValSet);
    //释放互斥信号量
    BoardTCA9555PWR_ReleaseLock(mutexPtr);
    return errorSub;
}

//反转指定输出
ERROR_SUB BoardTCA9555PWR_ToogleSinglePortOut(uint8_t deviceAddr,TX_MUTEX* mutexPtr,TCA9555_PIN pin)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //读取指定端口当前输出
    GPIO_PinState currentPinState;
    //申请设备互斥信号量
    BoardTCA9555PWR_GetLock(mutexPtr);
    //读取当前输出
    errorSub = BoardTCA9555PWR_ReadSinglePortOutCurrent(deviceAddr,mutexPtr,pin,&currentPinState);
    if(errorSub != ERROR_SUB_OK)
    {
        //释放互斥信号量
        BoardTCA9555PWR_ReleaseLock(mutexPtr);
        return errorSub;
    }
    //反转
    if(currentPinState == GPIO_PIN_SET)
    {
        currentPinState = GPIO_PIN_RESET;
    }
    else
    {
        currentPinState = GPIO_PIN_SET;
    }
    //写入新的值
    errorSub = BoardTCA9555PWR_WriteSinglePortOut(deviceAddr,mutexPtr,pin,currentPinState);
    if(errorSub != ERROR_SUB_OK)
    {
        //释放互斥信号量
        BoardTCA9555PWR_ReleaseLock(mutexPtr);
        return errorSub;
    }
    //释放互斥信号量
    BoardTCA9555PWR_ReleaseLock(mutexPtr);
    return errorSub;
}

//读取一组输入
ERROR_SUB BoardTCA9555PWR_ReadAllPortIn(uint8_t deviceAddr,TX_MUTEX* mutexPtr,TCA9555_PORT_VAL_SET* valSetPtr)
{
    HAL_StatusTypeDef halState = HAL_OK;
    uint8_t readBufArray[2];
    //申请设备互斥信号量
    BoardTCA9555PWR_GetLock(mutexPtr);
    //检查空闲
    if(ERROR_SUB_OK != BoardTCA9555PWR_WaitIdle(deviceAddr,mutexPtr,TCA9555_PWR_WAIT_IDLE_COUNT))
    {
        //没检查到空闲,释放设备互斥信号量
        BoardTCA9555PWR_ReleaseLock(mutexPtr);
        return ERROR_SUB_BOARD_TCA9555PWR_WAIT_IDLE;
    }
    halState = BOARD_TCA9555PWR_LOW_LEVEL_READ(deviceAddr,IIC_SUB_ADDR_SIZE_8BIT,REG_ADDR_TCA9555_INPUT_VAL_LOW,readBufArray,2,
                                                TIME_MS_MAX_TCA9555PWR_WAIT_IDLE_BYTE);
    //完成,空闲
    BoardTCA9555PWR_ReleaseLock(mutexPtr);
    if(halState != HAL_OK)
    {
        MCU_LogShowHalState(halState,0);
        return ERROR_SUB_BOARD_TCA9555PWR_READ_PORT_IN;
    }
    //数据组合
    valSetPtr->regVal2Byte = 0X0000;
    valSetPtr->regVal2Byte += readBufArray[1];
    valSetPtr->regVal2Byte <<= 8;
    valSetPtr->regVal2Byte += readBufArray[0];
    return ERROR_SUB_OK;
}

//读取单个输入
ERROR_SUB BoardTCA9555PWR_ReadSinglePortIn(uint8_t deviceAddr,TX_MUTEX* mutexPtr,TCA9555_PIN pin,GPIO_PinState* valPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    TCA9555_PORT_VAL_SET portValSet;
    //读取全部输入
    errorSub = BoardTCA9555PWR_ReadAllPortIn(deviceAddr,mutexPtr,&portValSet);
    if(errorSub != ERROR_SUB_OK)
    {
        return errorSub;
    }
    //从结果中检索数据
    *valPtr = (GPIO_PinState)((portValSet.regVal2Byte>>((uint8_t)pin))&(0x0001));
    return errorSub;
}













