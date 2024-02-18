/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-09-15 09:57:20 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-20 15:57:33 +0800
************************************************************************************************/ 
#include "BoardIS25LP128F.h"
#include "tx_mutex.h"

//芯片状态寄存器
static IS25LP128F_STATUS_REG_SET w25q256StatusRegSet;
// 互斥信号量
static TX_MUTEX mutexBoardIS25LP128F;

// 设备选中
static void BoardIS25LP128F_ChipSelect(void)
{
    //先拉高
    MCU_PinWriteSingle(PIN_BOARD_IS25LP128F_CS,PIN_INVALID_LEVEL(VALID_LEVEL_BOARD_IS25LP128F_CS));
    //再选中
    MCU_PinWriteSingle(PIN_BOARD_IS25LP128F_CS, VALID_LEVEL_BOARD_IS25LP128F_CS);
}

// 设备释放
static void BoardIS25LP128F_ChipDisSelect(void)
{
    MCU_PinWriteSingle(PIN_BOARD_IS25LP128F_CS, PIN_INVALID_LEVEL(VALID_LEVEL_BOARD_IS25LP128F_CS));
}

// 获取设备互斥信号量
static void BoardIS25LP128F_GetLock(void)
{
    tx_mutex_get(&mutexBoardIS25LP128F, TX_WAIT_FOREVER);
}

// 释放设备互斥信号量
static void BoardIS25LP128F_ReleaseLock(void)
{
    tx_mutex_put(&mutexBoardIS25LP128F);
}

/*-------------------------------基础操作函数,没有设备互斥量的申请-------------------------------------*/
//读取状态状态寄存器
static ERROR_SUB BoardIS25LP128F_ReadStatusReg(IS25LP128F_STATUS_REG* stateRegPtr) 
{  
    ERROR_SUB errorSub = ERROR_SUB_OK;
    stateRegPtr->regValueAll = 0XFF;
    uint8_t sendBuf[2] = {0};
    uint8_t readBuf[2] = {0};
    //发送指令
    sendBuf[0] = CMD_IS25LP128F_READ_STATUS_REG;
    sendBuf[1] = 0X00;
    //申请SPI总线使用权
    BOARD_IS25LP128F_SPI_GET_LOCK();
    //设备CS选中
    BoardIS25LP128F_ChipSelect();    
    //发送读取状态寄存器的指令                     
    errorSub = BOARD_IS25LP128F_SPI_READ_WRITE(sendBuf,readBuf,2);
    if(errorSub != ERROR_SUB_OK)
    {
        //设备CS释放
        BoardIS25LP128F_ChipDisSelect();
        //释放SPI总线使用权
        BOARD_IS25LP128F_SPI_RELEASE_LOCK();
        //错误日志打印
        MCU_LogShowErrorSubCode(errorSub);
        return ERROR_SUB_BOARD_IS25LP128F_READ_STATUS_REG;
    }
    //设备CS释放
    BoardIS25LP128F_ChipDisSelect();
    //释放SPI总线使用权
    BOARD_IS25LP128F_SPI_RELEASE_LOCK();
    //读取结果
    stateRegPtr->regValueAll = readBuf[1];	    
    return ERROR_SUB_OK;
} 

//读取状态功能寄存器
static ERROR_SUB BoardIS25LP128F_ReadFuncReg(IS25LP128F_FUNC_REG* funcRegPtr)   
{  
    ERROR_SUB errorSub = ERROR_SUB_OK;
    funcRegPtr->regValueAll = 0XFF;
    uint8_t sendBuf[2] = {0};
    uint8_t readBuf[2] = {0};
    //发送指令
    sendBuf[0] = CMD_IS25LP128F_READ_FUNC_REG;
    sendBuf[1] = 0X00;
    //申请SPI总线使用权
    BOARD_IS25LP128F_SPI_GET_LOCK();
    //设备CS选中
    BoardIS25LP128F_ChipSelect();    
    //发送读取状态寄存器的指令                     
    errorSub = BOARD_IS25LP128F_SPI_READ_WRITE(sendBuf,readBuf,2);
    if(errorSub != ERROR_SUB_OK)
    {
        //设备CS释放
        BoardIS25LP128F_ChipDisSelect();
        //释放SPI总线使用权
        BOARD_IS25LP128F_SPI_RELEASE_LOCK();
        //错误日志打印
        MCU_LogShowErrorSubCode(errorSub);
        return ERROR_SUB_BOARD_IS25LP128F_READ_FUNC_REG;
    }
    //设备CS释放
    BoardIS25LP128F_ChipDisSelect();
    //释放SPI总线使用权
    BOARD_IS25LP128F_SPI_RELEASE_LOCK();
    //读取结果
    funcRegPtr->regValueAll = readBuf[1];	    
    return ERROR_SUB_OK;
} 

//读取全部的状态寄存器
static ERROR_SUB BoardIS25LP128F_ReadAllStatusReg(IS25LP128F_STATUS_REG_SET* regSetPtr)
{
    ERROR_SUB errorCodeSub = ERROR_SUB_OK;
    //读取状态寄存器
    errorCodeSub = BoardIS25LP128F_ReadStatusReg(&(regSetPtr->statusReg));
    if(errorCodeSub != ERROR_SUB_OK)
    {
        //底层错误打印
        MCU_LogShowErrorSubCode(errorCodeSub);
        return errorCodeSub;
    }
    //读取功能寄存器
    errorCodeSub = BoardIS25LP128F_ReadFuncReg(&(regSetPtr->funcReg));
    if(errorCodeSub != ERROR_SUB_OK)
    {
        //底层错误打印
        MCU_LogShowErrorSubCode(errorCodeSub);
        return errorCodeSub;
    }
    //返回数据
	return ERROR_SUB_OK;
}

//写入状态状态寄存器
static ERROR_SUB BoardIS25LP128F_WriteStatusReg(uint8_t regValue)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    uint8_t sendBuf[2] = {0};
    //发送指令
    sendBuf[0] = CMD_IS25LP128F_WRITE_STATUS_REG;
    sendBuf[1] = regValue;
    //申请SPI总线使用权
    BOARD_IS25LP128F_SPI_GET_LOCK();
    //设备CS选中
    BoardIS25LP128F_ChipSelect();    
    //发送读取状态寄存器的指令                     
    errorSub = BOARD_IS25LP128F_SPI_READ_WRITE(sendBuf,SPI_PTR_NULL,2);
    if(errorSub != ERROR_SUB_OK)
    {
        //设备CS释放
        BoardIS25LP128F_ChipDisSelect();
        //释放SPI总线使用权
        BOARD_IS25LP128F_SPI_RELEASE_LOCK();
        //错误日志打印
        MCU_LogShowErrorSubCode(errorSub);
        return ERROR_SUB_BOARD_IS25LP128F_WRITE_STATUS_REG;
    }
    //设备CS释放
    BoardIS25LP128F_ChipDisSelect();
    //释放SPI总线使用权
    BOARD_IS25LP128F_SPI_RELEASE_LOCK();
    return ERROR_SUB_OK;
}

//写入状态功能寄存器
static ERROR_SUB BoardIS25LP128F_WriteFuncReg(uint8_t regValue)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    uint8_t sendBuf[2] = {0};
    //发送指令
    sendBuf[0] = CMD_IS25LP128F_WRITE_FUNC_REG;
    sendBuf[1] = regValue;
    //申请SPI总线使用权
    BOARD_IS25LP128F_SPI_GET_LOCK();
    //设备CS选中
    BoardIS25LP128F_ChipSelect();    
    //发送读取状态寄存器的指令                     
    errorSub = BOARD_IS25LP128F_SPI_READ_WRITE(sendBuf,SPI_PTR_NULL,2);
    if(errorSub != ERROR_SUB_OK)
    {
        //设备CS释放
        BoardIS25LP128F_ChipDisSelect();
        //释放SPI总线使用权
        BOARD_IS25LP128F_SPI_RELEASE_LOCK();
        //错误日志打印
        MCU_LogShowErrorSubCode(errorSub);
        return ERROR_SUB_BOARD_IS25LP128F_WRITE_FUNC_REG;
    }
    //设备CS释放
    BoardIS25LP128F_ChipDisSelect();
    //释放SPI总线使用权
    BOARD_IS25LP128F_SPI_RELEASE_LOCK();
    return ERROR_SUB_OK;
}

//等待设备写使能生效
static ERROR_SUB BoardIS25LP128F_WaitDeviceWriteEnable(uint32_t timeOutMs)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    IS25LP128F_STATUS_REG reg1State;
    //获取当前时间戳
    uint32_t startTimeStamp = tx_time_get();
    do
    {
        //读取状态状态寄存器
        errorSub = BoardIS25LP128F_ReadStatusReg(&reg1State);
        if(errorSub != ERROR_SUB_OK)
        {
            //错误打印
            MCU_LogShowErrorSubCode(errorSub);
            return errorSub;
        }  
        //检查状态
        if(reg1State.regValueSingle.wel == 1)
        {
            return ERROR_SUB_OK;
        }
        //超时检查
        if(tx_time_get() - startTimeStamp > MS_TO_OS_TICK(timeOutMs))
        {
            //超时
            MCU_LogShowErrorSubCode(errorSub);
            return ERROR_SUB_BOARD_IS25LP128F_WAIT_WR_EN;
        }
        //没超时,延时等待
        CoreDelayMinTick();
    }while(1);
}

//等待芯片写设备禁止
static ERROR_SUB BoardIS25LP128F_WaitDeviceWriteProtect(uint32_t timeOutMs)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    IS25LP128F_STATUS_REG reg1State;
    //获取当前时间戳
    uint32_t startTimeStamp = tx_time_get();
    do
    {
        //读取状态状态寄存器
        errorSub = BoardIS25LP128F_ReadStatusReg(&reg1State);
        if(errorSub != ERROR_SUB_OK)
        {
            //错误打印
            MCU_LogShowErrorSubCode(errorSub);
            return errorSub;
        }  
        //检查状态
        if(reg1State.regValueSingle.wel == 0)
        {
            return ERROR_SUB_OK;
        }
        //超时检查
        if(tx_time_get() - startTimeStamp > MS_TO_OS_TICK(timeOutMs))
        {
            //超时
            MCU_LogShowErrorSubCode(errorSub);
            return ERROR_SUB_BOARD_IS25LP128F_WAIT_WP_EN;
        }
        //没超时,延时等待
        CoreDelayMinTick();
    }while(1);
}

//等待芯片block保护设置生效
static ERROR_SUB BoardIS25LP128F_WaitDeviceBlockProtectValid(uint8_t tbSet,uint8_t bpSet,uint32_t timeOutMs)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    IS25LP128F_STATUS_REG regState;
    IS25LP128F_FUNC_REG regFunc;
    //获取当前时间戳
    uint32_t startTimeStamp = tx_time_get();
    do
    {
        //读取状态状态寄存器
        errorSub = BoardIS25LP128F_ReadStatusReg(&regState);
        if(errorSub != ERROR_SUB_OK)
        {
            //错误打印
            MCU_LogShowErrorSubCode(errorSub);
            return errorSub;
        }  
        //读取方法寄存器
         errorSub = BoardIS25LP128F_ReadFuncReg(&regFunc);
        if(errorSub != ERROR_SUB_OK)
        {
            //错误打印
            MCU_LogShowErrorSubCode(errorSub);
            return errorSub;
        }  
        //检查状态
        if((regState.regValueSingle.bp == bpSet)&&(regFunc.regValueSingle.tbs == tbSet))
        {
            return ERROR_SUB_OK;
        }
        //超时检查
        if(tx_time_get() - startTimeStamp > MS_TO_OS_TICK(timeOutMs))
        {
            //超时
            MCU_LogShowErrorSubCode(errorSub);
            return ERROR_SUB_BOARD_IS25LP128F_WAIT_BP;
        }
        //没超时,延时等待
        CoreDelayMinTick();
    }while(1);
}

//等待芯片进入QUAD模式生效
static ERROR_SUB BoardIS25LP128F_WaitDevice_EnterQuadMode(uint32_t timeOutMs)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    IS25LP128F_STATUS_REG regStatus;
    //获取当前时间戳
    uint32_t startTimeStamp = tx_time_get();
    do
    {
        //读取状态功能寄存器
        errorSub = BoardIS25LP128F_ReadStatusReg(&regStatus);
        if(errorSub != ERROR_SUB_OK)
        {
            //错误打印
            MCU_LogShowErrorSubCode(errorSub);
            return errorSub;
        }  
        //检查状态
        if(regStatus.regValueSingle.qe == 1)
        {
            return ERROR_SUB_OK;
        }
        //超时检查
        if(tx_time_get() - startTimeStamp > MS_TO_OS_TICK(timeOutMs))
        {
            //超时
            MCU_LogShowErrorSubCode(errorSub);
            return ERROR_SUB_BOARD_IS25LP128F_WAIT_QUAD_EN;
        }
        //没超时,延时等待
        CoreDelayMinTick();
    }while(1);
}

//等待设备空闲
ERROR_SUB BoardIS25LP128F_WaitDeviceIdleLocal(uint32_t timeOutMs)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    IS25LP128F_STATUS_REG reg1State;
    //获取当前时间戳
    uint32_t startTimeStamp = tx_time_get();
    do
    {
        //读取状态状态寄存器
        errorSub = BoardIS25LP128F_ReadStatusReg(&reg1State);
        if(errorSub != ERROR_SUB_OK)
        {
            //错误打印
            MCU_LogShowErrorSubCode(errorSub);
            return errorSub;
        }  
        //检查状态
        if(reg1State.regValueSingle.busy == 0)
        {
            return ERROR_SUB_OK;
        }
        //超时检查
        if(tx_time_get() - startTimeStamp > MS_TO_OS_TICK(timeOutMs))
        {
            //超时
            MCU_LogShowErrorSubCode(errorSub);
            return ERROR_SUB_BOARD_IS25LP128F_WAIT_IDLE;
        }
        //没超时,延时等待
        CoreDelayMinTick();
    }while(1);
}

//更新系统状态寄存器
static ERROR_SUB BoardIS25LP128F_ReflushStatusReg(void)
{
    ERROR_SUB errorCodeSub = BoardIS25LP128F_ReadAllStatusReg(&w25q256StatusRegSet);
    return errorCodeSub;
}


//发送写入使能指令
static ERROR_SUB BoardIS25LP128F_SendCmdWriteEnable(void)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //发送指令
    uint8_t sendCommand = CMD_IS25LP128F_WRITE_ENABLE;
    //申请SPI总线使用权
    BOARD_IS25LP128F_SPI_GET_LOCK();
    //设备CS选中
    BoardIS25LP128F_ChipSelect();  
    //发送写使能
    errorSub = BOARD_IS25LP128F_SPI_READ_WRITE(&sendCommand,SPI_PTR_NULL,1); 
    if(errorSub != ERROR_SUB_OK)
    {
        //设备CS释放
        BoardIS25LP128F_ChipDisSelect();
        //释放SPI总线使用权
        BOARD_IS25LP128F_SPI_RELEASE_LOCK();
        //错误打印
        MCU_LogShowErrorSubCode(errorSub);
        return ERROR_SUB_BOARD_IS25LP128F_SEND_CMD_WRITE_ENABLE;
    }                   
	//设备CS释放
    BoardIS25LP128F_ChipDisSelect();
    //释放SPI总线使用权
    BOARD_IS25LP128F_SPI_RELEASE_LOCK();
    return ERROR_SUB_OK;		 
}

//发送写入禁止指令
static ERROR_SUB BoardIS25LP128F_SendCmdWriteDisable(void)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //发送指令
    uint8_t sendCommand = CMD_IS25LP128F_WRITE_DISABLE;
    //申请SPI总线使用权
    BOARD_IS25LP128F_SPI_GET_LOCK();
    //设备CS选中
    BoardIS25LP128F_ChipSelect();  
    //发送写使能
    errorSub = BOARD_IS25LP128F_SPI_READ_WRITE(&sendCommand,SPI_PTR_NULL,1); 
    if(errorSub != ERROR_SUB_OK)
    {
        //设备CS释放
        BoardIS25LP128F_ChipDisSelect();
        //释放SPI总线使用权
        BOARD_IS25LP128F_SPI_RELEASE_LOCK();
        //错误打印
        MCU_LogShowErrorSubCode(errorSub);
        return ERROR_SUB_BOARD_IS25LP128F_SEND_CMD_WRITE_DISABLE;
    }                   
	//设备CS释放
    BoardIS25LP128F_ChipDisSelect();
    //释放SPI总线使用权
    BOARD_IS25LP128F_SPI_RELEASE_LOCK();
    return ERROR_SUB_OK;		 
}

//写入使能
static ERROR_SUB BoardIS25LP128F_WriteEnable(void)
{
    ERROR_SUB errorSubCode = ERROR_SUB_OK;
    //首先读取状态寄存器
    errorSubCode = BoardIS25LP128F_ReflushStatusReg();
    if(errorSubCode != ERROR_SUB_OK)
    {
        //日志打印
        MCU_LogShowErrorSubCode(errorSubCode);
        return errorSubCode;
    }
    //查看WEL状态
    if(w25q256StatusRegSet.statusReg.regValueSingle.wel != 0)
    {
        return ERROR_SUB_OK;
    }
    //发送写入使能指令
    errorSubCode = BoardIS25LP128F_SendCmdWriteEnable();
    if(errorSubCode != ERROR_SUB_OK)
    {
        //日志打印
        MCU_LogShowErrorSubCode(errorSubCode);
        return errorSubCode;
    }
    //等待写入使能生效
    errorSubCode = BoardIS25LP128F_WaitDeviceWriteEnable(BOARD_IS25LP128F_CMD_TIME_OUT);
    if(errorSubCode != ERROR_SUB_OK)
    {
        //日志打印
        MCU_LogShowErrorSubCode(errorSubCode);
        return errorSubCode;
    }
    return ERROR_SUB_OK;
}

//写入禁止
static ERROR_SUB BoardIS25LP128F_WriteDisable(void)
{
    ERROR_SUB errorSubCode = ERROR_SUB_OK;
    //首先读取状态寄存器
    errorSubCode = BoardIS25LP128F_ReflushStatusReg();
    if(errorSubCode != ERROR_SUB_OK)
    {
        //日志打印
        MCU_LogShowErrorSubCode(errorSubCode);
        return errorSubCode;
    }
    //查看WEL状态
    if(w25q256StatusRegSet.statusReg.regValueSingle.wel == 0)
    {
        return ERROR_SUB_OK;
    }
    //发送写入禁止指令
    errorSubCode = BoardIS25LP128F_SendCmdWriteDisable();
    if(errorSubCode != ERROR_SUB_OK)
    {
        //日志打印
        MCU_LogShowErrorSubCode(errorSubCode);
        return errorSubCode;
    }
    //等待写入禁止生效
    errorSubCode = BoardIS25LP128F_WaitDeviceWriteProtect(BOARD_IS25LP128F_CMD_TIME_OUT);
    if(errorSubCode != ERROR_SUB_OK)
    {
        //日志打印
        MCU_LogShowErrorSubCode(errorSubCode);
        return errorSubCode;
    }
    return ERROR_SUB_OK;
}

//进入quad模式
ERROR_SUB BoardIS25LP128F_EnterQuadMode(void)
{
    ERROR_SUB errorSubCode = ERROR_SUB_OK;
    //首先读取状态寄存器
    errorSubCode = BoardIS25LP128F_ReflushStatusReg();
    if(errorSubCode != ERROR_SUB_OK)
    {
        //日志打印
        MCU_LogShowErrorSubCode(errorSubCode);
        return errorSubCode;
    }
    if(w25q256StatusRegSet.statusReg.regValueSingle.qe != 0)
    {
        return ERROR_SUB_OK;
    }
    //进入quad模式,首先写使能
    errorSubCode = BoardIS25LP128F_WriteEnable();
    if(errorSubCode != ERROR_SUB_OK)
    {
        //日志打印
        MCU_LogShowErrorSubCode(errorSubCode);
        return errorSubCode;
    }
    //然后修改quad值
    w25q256StatusRegSet.statusReg.regValueSingle.qe = 1;
    //写入寄存器
    errorSubCode = BoardIS25LP128F_WriteStatusReg(w25q256StatusRegSet.statusReg.regValueAll);
    if(errorSubCode != ERROR_SUB_OK)
    {
        //日志打印
        MCU_LogShowErrorSubCode(errorSubCode);
        return errorSubCode;
    }
    //等待进入QUAD模式
    errorSubCode = BoardIS25LP128F_WaitDevice_EnterQuadMode(BOARD_IS25LP128F_CMD_TIME_OUT);
    if(errorSubCode != ERROR_SUB_OK)
    {
        //日志打印
        MCU_LogShowErrorSubCode(errorSubCode);
        return errorSubCode;
    }
    //关闭写入
    errorSubCode = BoardIS25LP128F_WriteDisable();
    if(errorSubCode != ERROR_SUB_OK)
    {
        //日志打印
        MCU_LogShowErrorSubCode(errorSubCode);
        return errorSubCode;
    }
    return ERROR_SUB_OK;
}

//设置块保护
static ERROR_SUB BoardIS25LP128F_SetBlockProtect(uint8_t tb,uint8_t bp)
{
    ERROR_SUB errorSubCode = ERROR_SUB_OK;
    //首先读取状态寄存器
    errorSubCode = BoardIS25LP128F_ReflushStatusReg();
    if(errorSubCode != ERROR_SUB_OK)
    {
        //日志打印
        MCU_LogShowErrorSubCode(errorSubCode);
        return errorSubCode;
    }
    //查看BP和TB
    if((w25q256StatusRegSet.statusReg.regValueSingle.bp == bp)&&(w25q256StatusRegSet.funcReg.regValueSingle.tbs == tb))
    {
        return ERROR_SUB_OK;
    }
    //需要设置,首先写使能
    errorSubCode = BoardIS25LP128F_WriteEnable();
    if(errorSubCode != ERROR_SUB_OK)
    {
        //日志打印
        MCU_LogShowErrorSubCode(errorSubCode);
        return errorSubCode;
    }
    //写入保护设定
    w25q256StatusRegSet.statusReg.regValueSingle.bp = bp;
    w25q256StatusRegSet.funcReg.regValueSingle.tbs = tb;
    //写入状态状态寄存器
    errorSubCode = BoardIS25LP128F_WriteStatusReg(w25q256StatusRegSet.statusReg.regValueAll);
    if(errorSubCode != ERROR_SUB_OK)
    {
        //日志打印
        MCU_LogShowErrorSubCode(errorSubCode);
        return errorSubCode;
    }
    //写入方法寄存器
    errorSubCode = BoardIS25LP128F_WriteFuncReg(w25q256StatusRegSet.funcReg.regValueAll);
    if(errorSubCode != ERROR_SUB_OK)
    {
        //日志打印
        MCU_LogShowErrorSubCode(errorSubCode);
        return errorSubCode;
    }
    //等待写保护设定生效
    errorSubCode = BoardIS25LP128F_WaitDeviceBlockProtectValid(tb,bp,BOARD_IS25LP128F_CMD_TIME_OUT);
    if(errorSubCode != ERROR_SUB_OK)
    {
        //日志打印
        MCU_LogShowErrorSubCode(errorSubCode);
        return errorSubCode;
    }
    //关闭写入
    errorSubCode = BoardIS25LP128F_WriteDisable();
    if(errorSubCode != ERROR_SUB_OK)
    {
        //日志打印
        MCU_LogShowErrorSubCode(errorSubCode);
        return errorSubCode;
    }
    return ERROR_SUB_OK;
}

//发送POWER Down指令
static ERROR_SUB BoardIS25LP128F_SendCmdPowerDown(void)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //发送指令
    uint8_t sendCommand = CMD_IS25LP128F_POWER_DOWN;
    //申请SPI总线使用权
    BOARD_IS25LP128F_SPI_GET_LOCK();
    //设备CS选中
    BoardIS25LP128F_ChipSelect();  
    //发送写使能
    errorSub = BOARD_IS25LP128F_SPI_READ_WRITE(&sendCommand,SPI_PTR_NULL,1); 
    if(errorSub != ERROR_SUB_OK)
    {
        //设备CS释放
        BoardIS25LP128F_ChipDisSelect();
        //释放SPI总线使用权
        BOARD_IS25LP128F_SPI_RELEASE_LOCK();
        //错误打印
        MCU_LogShowErrorSubCode(errorSub);
        return ERROR_SUB_BOARD_IS25LP128F_SEND_CMD_POWER_DOWN;
    }                   
	//设备CS释放
    BoardIS25LP128F_ChipDisSelect();
    //释放SPI总线使用权
    BOARD_IS25LP128F_SPI_RELEASE_LOCK();
    return ERROR_SUB_OK;		 
}


//发送Release POWER Down指令
static ERROR_SUB BoardIS25LP128F_SendCmdReleasePowerDown(void)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //发送指令
    uint8_t sendCommand = CMD_IS25LP128F_RELEASE_POWER_DOWN;
    //申请SPI总线使用权
    BOARD_IS25LP128F_SPI_GET_LOCK();
    //设备CS选中
    BoardIS25LP128F_ChipSelect();  
    //发送写使能
    errorSub = BOARD_IS25LP128F_SPI_READ_WRITE(&sendCommand,SPI_PTR_NULL,1); 
    if(errorSub != ERROR_SUB_OK)
    {
        //设备CS释放
        BoardIS25LP128F_ChipDisSelect();
        //释放SPI总线使用权
        BOARD_IS25LP128F_SPI_RELEASE_LOCK();
        //错误打印
        MCU_LogShowErrorSubCode(errorSub);
        return ERROR_SUB_BOARD_IS25LP128F_SEND_CMD_RELEASE_POWER_DOWN;
    }                   
	//设备CS释放
    BoardIS25LP128F_ChipDisSelect();
    //释放SPI总线使用权
    BOARD_IS25LP128F_SPI_RELEASE_LOCK();
    return ERROR_SUB_OK;		 
}

//发送指令
static ERROR_SUB BoardIS25LP128F_SendCmdEnableReset(void)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //发送指令
    uint8_t sendCommand = CMD_IS25LP128F_ENABLE_RESET;
    //申请SPI总线使用权
    BOARD_IS25LP128F_SPI_GET_LOCK();
    //设备CS选中
    BoardIS25LP128F_ChipSelect();  
    //发送指令
    errorSub = BOARD_IS25LP128F_SPI_READ_WRITE(&sendCommand,SPI_PTR_NULL,1); 
    if(errorSub != ERROR_SUB_OK)
    {
        //设备CS释放
        BoardIS25LP128F_ChipDisSelect();
        //释放SPI总线使用权
        BOARD_IS25LP128F_SPI_RELEASE_LOCK();
        //错误打印
        MCU_LogShowErrorSubCode(errorSub);
        return ERROR_SUB_BOARD_IS25LP128F_SEND_CMD_ENABLE_RESET;
    }                   
	//设备CS释放
    BoardIS25LP128F_ChipDisSelect();
    //释放SPI总线使用权
    BOARD_IS25LP128F_SPI_RELEASE_LOCK();
    return ERROR_SUB_OK;		 
}

//发送指令
static ERROR_SUB BoardIS25LP128F_SendCmdResetDevice(void)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //发送指令
    uint8_t sendCommand = CMD_IS25LP128F_RESET_DEVICE;
    //申请SPI总线使用权
    BOARD_IS25LP128F_SPI_GET_LOCK();
    //设备CS选中
    BoardIS25LP128F_ChipSelect();  
    //发送指令
    errorSub = BOARD_IS25LP128F_SPI_READ_WRITE(&sendCommand,SPI_PTR_NULL,1); 
    if(errorSub != ERROR_SUB_OK)
    {
        //设备CS释放
        BoardIS25LP128F_ChipDisSelect();
        //释放SPI总线使用权
        BOARD_IS25LP128F_SPI_RELEASE_LOCK();
        //错误打印
        MCU_LogShowErrorSubCode(errorSub);
        return ERROR_SUB_BOARD_IS25LP128F_SEND_CMD_RESET_DEVICE;
    }                   
	//设备CS释放
    BoardIS25LP128F_ChipDisSelect();
    //释放SPI总线使用权
    BOARD_IS25LP128F_SPI_RELEASE_LOCK();
    return ERROR_SUB_OK;		 
}

//发送指令,擦除指定扇区,扇区地址，以4KB为单位的地址
static ERROR_SUB BoardIS25LP128F_SendCmdEraseSector(uint32_t sectorNo)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    uint32_t sectorAddr = sectorNo*BOARD_IS25LP128F_SECTOR_SIZE;
    //发送指令
    uint8_t sendDataBuffer[4];
    sendDataBuffer[0] = CMD_IS25LP128F_SECTOR_ERASE;
    sendDataBuffer[1] = (uint8_t)((sectorAddr)>>16);
    sendDataBuffer[2] = (uint8_t)((sectorAddr)>>8);
    sendDataBuffer[3] = (uint8_t)((sectorAddr));
    //申请SPI总线使用权
    BOARD_IS25LP128F_SPI_GET_LOCK();
    //设备CS选中
    BoardIS25LP128F_ChipSelect();    
    //发送扇区擦除
    errorSub = BOARD_IS25LP128F_SPI_READ_WRITE(sendDataBuffer,SPI_PTR_NULL,4);
    if(errorSub != ERROR_SUB_OK)
    {
        //设备CS释放
        BoardIS25LP128F_ChipDisSelect();
        //释放SPI总线使用权
        BOARD_IS25LP128F_SPI_RELEASE_LOCK();
        //日志显示
        MCU_LogShowErrorSubCode(errorSub);
        return ERROR_SUB_BOARD_IS25LP128F_SEND_CMD_ERASE_SECTOR;
    }               
    //设备CS释放
    BoardIS25LP128F_ChipDisSelect();
    //释放SPI总线使用权
    BOARD_IS25LP128F_SPI_RELEASE_LOCK();
    return errorSub;
}

//擦除块
static ERROR_SUB BoardIS25LP128F_SendCmdEraseBlock(uint32_t blockNo)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    uint32_t blockAddr = blockNo*BOARD_IS25LP128F_SECTOR_SIZE*BOARD_IS25LP128F_FLASH_SECTOR_PER_BLOCK;
    //发送指令
    uint8_t sendDataBuffer[4];
    sendDataBuffer[0] = CMD_IS25LP128F_BLOCK_ERASE_64KB;
    sendDataBuffer[1] = (uint8_t)((blockAddr)>>16);
    sendDataBuffer[2] = (uint8_t)((blockAddr)>>8);
    sendDataBuffer[3] = (uint8_t)((blockAddr));
    //申请SPI总线使用权
    BOARD_IS25LP128F_SPI_GET_LOCK();
    //设备CS选中
    BoardIS25LP128F_ChipSelect();    
    //发送扇区擦除
    errorSub = BOARD_IS25LP128F_SPI_READ_WRITE(sendDataBuffer,SPI_PTR_NULL,4);
    if(errorSub != ERROR_SUB_OK)
    {
        //设备CS释放
        BoardIS25LP128F_ChipDisSelect();
        //释放SPI总线使用权
        BOARD_IS25LP128F_SPI_RELEASE_LOCK();
        //日志显示
        MCU_LogShowErrorSubCode(errorSub);
        return ERROR_SUB_BOARD_IS25LP128F_SEND_CMD_ERASE_BLOCK;
    }               
    //设备CS释放
    BoardIS25LP128F_ChipDisSelect();
    //释放SPI总线使用权
    BOARD_IS25LP128F_SPI_RELEASE_LOCK();
    return errorSub;
}

//擦除全芯片,等待时间500S
static ERROR_SUB BoardIS25LP128F_SendCmdEraseChip(void)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //发送指令
    uint8_t sendCommand = CMD_IS25LP128F_CHIP_ERASE1;
    //申请SPI总线使用权
    BOARD_IS25LP128F_SPI_GET_LOCK();
    //设备CS选中
    BoardIS25LP128F_ChipSelect();    
    //发送扇区擦除
    errorSub = BOARD_IS25LP128F_SPI_READ_WRITE(&sendCommand,SPI_PTR_NULL,1);
    if(errorSub != ERROR_SUB_OK)
    {
        //设备CS释放
        BoardIS25LP128F_ChipDisSelect();
        //释放SPI总线使用权
        BOARD_IS25LP128F_SPI_RELEASE_LOCK();
        //日志显示
        MCU_LogShowErrorSubCode(errorSub);
        return ERROR_SUB_BOARD_IS25LP128F_SEND_CMD_ERASE_CHIP;
    }               
    //设备CS释放
    BoardIS25LP128F_ChipDisSelect();
    //释放SPI总线使用权
    BOARD_IS25LP128F_SPI_RELEASE_LOCK();
    return errorSub;
}

//读取指定地址数据
static ERROR_SUB BoardIS25LP128F_SendCmdReadBuffer(uint8_t* pBuffer, uint32_t addr, uint32_t dataLength)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //发送指令
    uint8_t sendDataBuf[4];
    sendDataBuf[0] = CMD_IS25LP128F_READ_DATA;
    sendDataBuf[1] = (uint8_t)((addr)>>16);
    sendDataBuf[2] = (uint8_t)((addr)>>8);
    sendDataBuf[3] = (uint8_t)((addr));
    //申请SPI总线使用权
    BOARD_IS25LP128F_SPI_GET_LOCK();
    //设备CS选中
    BoardIS25LP128F_ChipSelect();   
    //发送读取命令
    errorSub = BOARD_IS25LP128F_SPI_READ_WRITE(sendDataBuf,SPI_PTR_NULL,4);  
    if(errorSub != ERROR_SUB_OK)
    {
        //设备CS释放
        BoardIS25LP128F_ChipDisSelect();
        //释放SPI总线使用权
        BOARD_IS25LP128F_SPI_RELEASE_LOCK();
        //日志显示
        MCU_LogShowErrorSubCode(errorSub);
        return ERROR_SUB_BOARD_IS25LP128F_SEND_CMD_READ_ADDR;
    }                                         
    //开始读取
    errorSub = BOARD_IS25LP128F_SPI_READ_WRITE(SPI_PTR_NULL,pBuffer,dataLength);  
    if(errorSub != ERROR_SUB_OK)
    {
        //设备CS释放
        BoardIS25LP128F_ChipDisSelect();
        //释放SPI总线使用权
        BOARD_IS25LP128F_SPI_RELEASE_LOCK();
        //日志显示
        MCU_LogShowErrorSubCode(errorSub);
        return ERROR_SUB_BOARD_IS25LP128F_SEND_CMD_READ_DATA;
    }
    //设备CS释放
    BoardIS25LP128F_ChipDisSelect();
    //释放SPI总线使用权
    BOARD_IS25LP128F_SPI_RELEASE_LOCK();
    return errorSub;
}

//写入指定地址数据,页编程，页大小256字节，任意页都可以写入
//数据个数，不能超过页面大小，范围1 - 256
static ERROR_SUB BoardIS25LP128F_SendCmdWritePage(uint8_t* pBuffer, uint32_t addr, uint16_t dataLength)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //发送指令
    uint8_t writeDataBuf[4];
    writeDataBuf[0] = CMD_IS25LP128F_PAGE_PROGRAM;
    writeDataBuf[1] = (uint8_t)((addr)>>16);
    writeDataBuf[2] = (uint8_t)((addr)>>8);
    writeDataBuf[3] = (uint8_t)(addr);
    //申请SPI总线使用权
    BOARD_IS25LP128F_SPI_GET_LOCK();
    //设备CS选中
    BoardIS25LP128F_ChipSelect();   
    //发送页写入指令
    errorSub = BOARD_IS25LP128F_SPI_READ_WRITE(writeDataBuf,SPI_PTR_NULL,4);       
    if(errorSub != ERROR_SUB_OK)
    {
        //设备CS释放
        BoardIS25LP128F_ChipDisSelect();
        //释放SPI总线使用权
        BOARD_IS25LP128F_SPI_RELEASE_LOCK();
        //日志显示
        MCU_LogShowErrorSubCode(errorSub);
        return ERROR_SUB_BOARD_IS25LP128F_SEND_CMD_WRITE_ADDR;
    }                               
    //开始写入
    errorSub = BOARD_IS25LP128F_SPI_READ_WRITE(pBuffer,SPI_PTR_NULL,dataLength);   
    if(errorSub != ERROR_SUB_OK)
    {
        //设备CS释放
        BoardIS25LP128F_ChipDisSelect();
        //释放SPI总线使用权
        BOARD_IS25LP128F_SPI_RELEASE_LOCK();
        //日志显示
        MCU_LogShowErrorSubCode(errorSub);
        return ERROR_SUB_BOARD_IS25LP128F_SEND_CMD_WRITE_DATA;
    }   
    //设备CS释放
    BoardIS25LP128F_ChipDisSelect();
    //释放SPI总线使用权
    BOARD_IS25LP128F_SPI_RELEASE_LOCK();
    return errorSub;
}

//写入指定地址数据,页编程，页大小256字节，任意页都可以写入
//数据个数，不能超过页面大小，范围1 - 256
static ERROR_SUB BoardIS25LP128F_WritePage(uint8_t* pBuffer, uint32_t addr, uint16_t dataLength)
{
    ERROR_SUB errorSubCode = ERROR_SUB_OK;
    //等待设备空闲
    errorSubCode = BoardIS25LP128F_WaitDeviceIdleLocal(BOARD_IS25LP128F_CMD_TIME_OUT);
	if(errorSubCode != ERROR_SUB_OK)
    {
        //日志打印
        MCU_LogShowErrorSubCode(errorSubCode);
        return errorSubCode;
    }
    //写使能
    errorSubCode = BoardIS25LP128F_WriteEnable();	
    if(errorSubCode != ERROR_SUB_OK)
    {
        //日志打印
        MCU_LogShowErrorSubCode(errorSubCode);
        return errorSubCode;
    }
    //发送指令写入
    errorSubCode = BoardIS25LP128F_SendCmdWritePage(pBuffer,addr,dataLength);
    if(errorSubCode != ERROR_SUB_OK)
    {
        //日志打印
        MCU_LogShowErrorSubCode(errorSubCode);
        return errorSubCode;
    }
    //读取完成等待设备空闲
    errorSubCode = BoardIS25LP128F_WaitDeviceIdleLocal(BOARD_IS25LP128F_TIMEOUT_MS_MAX_PAGE_PROGRAM);
	if(errorSubCode != ERROR_SUB_OK)
    {
        //日志打印
        MCU_LogShowErrorSubCode(errorSubCode);
        return ERROR_SUB_BOARD_IS25LP128F_WRITE_PAGE_WAIT_IDLE;
    }
    //关闭写入
    errorSubCode = BoardIS25LP128F_WriteDisable();
    if(errorSubCode != ERROR_SUB_OK)
    {
        //日志打印
        MCU_LogShowErrorSubCode(errorSubCode);
        return errorSubCode;
    }
    return ERROR_SUB_OK;
}

/*----------------------------------------外部调用接口,需要使用设备互斥量--------------------------------------------------*/
//端口初始化
void BoardIS25LP128F_PortInit(void)
{
    if (TX_MUTEX_ID == mutexBoardIS25LP128F.tx_mutex_id)
    {
        // 端口已经初始化了,不需要重新初始化
        return;
    }
    // 互斥信号量初始化
    tx_mutex_create(&mutexBoardIS25LP128F, "mutexBoardIS25LP128F", TX_INHERIT);
    // 首先进行IO初始化
    BOARD_IS25LP128F_SPI_PORT_INIT(BOARD_IS25LP128F_SPI_SPEED, BOARD_IS25LP128F_SPI_CPOL, BOARD_IS25LP128F_SPI_CPHA);
    // CS片选初始化
    MCU_PinInit(PIN_BOARD_IS25LP128F_CS, MCU_PIN_MODE_OUTPUT_PP, MCU_PIN_PULL_PULLUP, MCU_PIN_SPEED_FREQ_VERY_HIGH,0);
    // 设备不选中
    BoardIS25LP128F_ChipDisSelect();
}

//读取板上设备ID
ERROR_SUB BoardIS25LP128F_ReadID(uint32_t* deviceIdPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    uint8_t sendDataBuf[1] = {0};
    uint8_t readTempBuf[3] = {0};
    //设置ID默认为0XFF
    *deviceIdPtr = 0XFFFF;
    //发送指令
    sendDataBuf[0] = CMD_IS25LP128F_JEDEC_ID;
    //获取设备使用权
    BoardIS25LP128F_GetLock();
    //申请SPI总线使用权
    BOARD_IS25LP128F_SPI_GET_LOCK();
    //设备CS选中
    BoardIS25LP128F_ChipSelect();
    //发送读取ID命令	
    errorSub = BOARD_IS25LP128F_SPI_READ_WRITE(sendDataBuf,SPI_PTR_NULL,1);
    if(errorSub != ERROR_SUB_OK)
    {
        //设备CS释放
        BoardIS25LP128F_ChipDisSelect();
        //释放SPI总线使用权
        BOARD_IS25LP128F_SPI_RELEASE_LOCK();
        //释放设备使用权
        BoardIS25LP128F_ReleaseLock();
        //错误打印
        MCU_LogShowErrorSubCode(errorSub);
        return errorSub;
    }   
    //开始读取	 
    errorSub = BOARD_IS25LP128F_SPI_READ_WRITE(SPI_PTR_NULL,readTempBuf,3);
    if(errorSub != ERROR_SUB_OK)
    {
        //设备CS释放
        BoardIS25LP128F_ChipDisSelect();
        //释放SPI总线使用权
        BOARD_IS25LP128F_SPI_RELEASE_LOCK();
        //释放设备使用权
        BoardIS25LP128F_ReleaseLock();
        //错误打印
        MCU_LogShowErrorSubCode(errorSub);
        return errorSub;
    }  
    //设备CS释放
    BoardIS25LP128F_ChipDisSelect();
    //释放SPI总线使用权
    BOARD_IS25LP128F_SPI_RELEASE_LOCK();
    //释放设备使用权
    BoardIS25LP128F_ReleaseLock();
    //ID组合
    (*deviceIdPtr) = readTempBuf[0];
    (*deviceIdPtr) <<= 8;
    (*deviceIdPtr) += readTempBuf[1];
    (*deviceIdPtr) <<= 8;
    (*deviceIdPtr) += readTempBuf[2];
    return ERROR_SUB_OK;
}

//等待设备空闲
ERROR_SUB BoardIS25LP128F_WaitDeviceIdle(uint32_t timeOutMs)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    IS25LP128F_STATUS_REG reg1State;
    //获取设备使用权
    BoardIS25LP128F_GetLock();
    //获取当前时间戳
    uint32_t startTimeStamp = tx_time_get();
    do
    {
        //读取状态状态寄存器
        errorSub = BoardIS25LP128F_ReadStatusReg(&reg1State);
        if(errorSub != ERROR_SUB_OK)
        {
            //释放设备使用权
            BoardIS25LP128F_ReleaseLock();
            //错误打印
            MCU_LogShowErrorSubCode(errorSub);
            return errorSub;
        }  
        //检查状态
        if(reg1State.regValueSingle.busy == 0)
        {
            //释放设备使用权
            BoardIS25LP128F_ReleaseLock();
            return ERROR_SUB_OK;
        }
        //超时检查
        if(tx_time_get() - startTimeStamp > MS_TO_OS_TICK(timeOutMs))
        {
            //释放设备使用权
            BoardIS25LP128F_ReleaseLock();
            //超时
            MCU_LogShowErrorSubCode(errorSub);
            return ERROR_SUB_BOARD_IS25LP128F_WAIT_IDLE;
        }
        //没超时,延时等待
        CoreDelayMinTick();
    }while(1);
}

//芯片进入低功耗模式
ERROR_SUB BoardIS25LP128F_PowerDown(void)
{
    ERROR_SUB errorSubCode = ERROR_SUB_OK;
    //获取设备使用权
    BoardIS25LP128F_GetLock();
    //发送写使能
    errorSubCode = BoardIS25LP128F_WriteEnable();
    if(errorSubCode != ERROR_SUB_OK)
    {
        //释放设备使用权
        BoardIS25LP128F_ReleaseLock();
        //日志打印
        MCU_LogShowErrorSubCode(errorSubCode);
        return errorSubCode;
    }
    //发送芯片进入低功耗模式
    errorSubCode = BoardIS25LP128F_SendCmdPowerDown();
    if(errorSubCode != ERROR_SUB_OK)
    {
        //释放设备使用权
        BoardIS25LP128F_ReleaseLock();
        //日志打印
        MCU_LogShowErrorSubCode(errorSubCode);
        return errorSubCode;
    }
    //延时等待进入低功耗
    CoreDelayMs(5);
    //释放设备使用权
    BoardIS25LP128F_ReleaseLock();
    return ERROR_SUB_OK;
}

//芯片退出低功耗模式
ERROR_SUB BoardIS25LP128F_ReleasePowerDown(void)
{
    ERROR_SUB errorSubCode = ERROR_SUB_OK;
    //获取设备使用权
    BoardIS25LP128F_GetLock();
    //发送芯片退出低功耗
    errorSubCode = BoardIS25LP128F_SendCmdReleasePowerDown();
    if(errorSubCode != ERROR_SUB_OK)
    {
        //释放设备使用权
        BoardIS25LP128F_ReleaseLock();
        //日志打印
        MCU_LogShowErrorSubCode(errorSubCode);
        return errorSubCode;
    }
    //延时等待设备恢复
    CoreDelayMs(5);
    //读取设备ID
    uint32_t deviceID = 0XFFFFFFFF;
    errorSubCode = BoardIS25LP128F_ReadID(&deviceID);
    if(errorSubCode != ERROR_SUB_OK)
    {
        //释放设备使用权
        BoardIS25LP128F_ReleaseLock();
        //日志打印
        MCU_LogShowErrorSubCode(errorSubCode);
        return errorSubCode;
    }
    if(deviceID != BOARD_IS25LP128F_ID)
    {
        MCU_LogPrintf("BoardIS25LP128F_ReadID : 0X%08X\r\n",deviceID);
        //释放设备使用权
        BoardIS25LP128F_ReleaseLock();
        return ERROR_SUB_BOARD_IS25LP128F_ID_MATCH;
    }
    //释放设备使用权
    BoardIS25LP128F_ReleaseLock();
    return ERROR_SUB_OK;
}

//设备复位
ERROR_SUB BoardIS25LP128F_DeviceReset(void)
{
    ERROR_SUB errorCodeSub = ERROR_SUB_OK;
    //获取设备使用权
    BoardIS25LP128F_GetLock();
    //首先等待设备空闲
    errorCodeSub = BoardIS25LP128F_WaitDeviceIdleLocal(BOARD_IS25LP128F_CMD_TIME_OUT);
    if(errorCodeSub != ERROR_SUB_OK)
    {
        //释放设备使用权
        BoardIS25LP128F_ReleaseLock();
        MCU_LogShowErrorSubCode(errorCodeSub);
        return errorCodeSub;
    }
    //复位使能
    errorCodeSub = BoardIS25LP128F_SendCmdEnableReset();
    if(errorCodeSub != ERROR_SUB_OK)
    {
        //释放设备使用权
        BoardIS25LP128F_ReleaseLock();
        MCU_LogShowErrorSubCode(errorCodeSub);
        return ERROR_SUB_BOARD_IS25LP128F_RESET_CMD1;
    }
    //设备复位
    errorCodeSub = BoardIS25LP128F_SendCmdResetDevice();
    if(errorCodeSub != ERROR_SUB_OK)
    {
        //释放设备使用权
        BoardIS25LP128F_ReleaseLock();
        MCU_LogShowErrorSubCode(errorCodeSub);
        return ERROR_SUB_BOARD_IS25LP128F_RESET_CMD2;
    }
    //延时等待复位
    CoreDelayMs(10);
    //等待设备空闲
    errorCodeSub = BoardIS25LP128F_WaitDeviceIdleLocal(BOARD_IS25LP128F_TIMEOUT_MS_RESET);
    if(errorCodeSub != ERROR_SUB_OK)
    {
        //释放设备使用权
        BoardIS25LP128F_ReleaseLock();
        MCU_LogShowErrorSubCode(errorCodeSub);
        return errorCodeSub;
    }
    //释放设备使用权
    BoardIS25LP128F_ReleaseLock();
    return ERROR_SUB_OK;
}

//设置设备默认配置
ERROR_SUB BoardIS25LP128F_SetDefaultConfig(void)
{
    ERROR_SUB errorCodeSub = ERROR_SUB_OK;
    //获取设备使用权
    BoardIS25LP128F_GetLock();
    //等待设备空闲
    errorCodeSub = BoardIS25LP128F_WaitDeviceIdleLocal(BOARD_IS25LP128F_CMD_TIME_OUT);
    if(errorCodeSub != ERROR_SUB_OK)
    {
        //释放设备使用权
        BoardIS25LP128F_ReleaseLock();
        MCU_LogShowErrorSubCode(errorCodeSub);
        return errorCodeSub;
    }
    //读取设备ID
    uint32_t deviceID = 0XFFFFFFFF;
    errorCodeSub = BoardIS25LP128F_ReadID(&deviceID);
    MCU_LogPrintf("BoardIS25LP128F_SetDefaultConfig Read DeviceID: 0X%08X\r\n",deviceID);
    //显示读取到的设备ID
    if(deviceID != BOARD_IS25LP128F_ID)
    {
        //释放设备使用权
        BoardIS25LP128F_ReleaseLock();
        MCU_LogShowErrorSubCode(errorCodeSub);
        return ERROR_SUB_BOARD_IS25LP128F_ID_MATCH;
    }
    //首先设备复位
    errorCodeSub = BoardIS25LP128F_DeviceReset();
    if(errorCodeSub != ERROR_SUB_OK)
    {
        //释放设备使用权
        BoardIS25LP128F_ReleaseLock();
        MCU_LogShowErrorSubCode(errorCodeSub);
        return errorCodeSub;
    }
    //设置默认块保护
    errorCodeSub = BoardIS25LP128F_SetBlockProtect(BOARD_IS25LP128F_DEFAULT_TB,BOARD_IS25LP128F_DEFAULT_BP);
    if(errorCodeSub != ERROR_SUB_OK)
    {
        //释放设备使用权
        BoardIS25LP128F_ReleaseLock();
        MCU_LogShowErrorSubCode(errorCodeSub);
        return errorCodeSub;
    }
    //释放设备使用权
    BoardIS25LP128F_ReleaseLock();
    return ERROR_SUB_OK;
}

//擦除指定扇区,扇区地址，以4KB为单位的地址
ERROR_SUB BoardIS25LP128F_EraseSector(uint32_t sectorNo)
{
    ERROR_SUB errorSubCode = ERROR_SUB_OK;
    //获取设备使用权
    BoardIS25LP128F_GetLock();
    //写使能
    errorSubCode = BoardIS25LP128F_WriteEnable();
    if(errorSubCode != ERROR_SUB_OK)
    {
        //释放设备使用权
        BoardIS25LP128F_ReleaseLock();
        //日志打印
        MCU_LogShowErrorSubCode(errorSubCode);
        return errorSubCode;
    }
    #if(BOARD_IS25LP128F_OP_TIME_MEASURE_ENABLE == 1)
    uint32_t startTimeStamp = tx_time_get();
    #endif
    //擦除指令发送
    errorSubCode = BoardIS25LP128F_SendCmdEraseSector(sectorNo);
    if(errorSubCode != ERROR_SUB_OK)
    {
        //释放设备使用权
        BoardIS25LP128F_ReleaseLock();
        //日志打印
        MCU_LogShowErrorSubCode(errorSubCode);
        return errorSubCode;
    }
    //等待设备执行完成,状态空闲
    errorSubCode = BoardIS25LP128F_WaitDeviceIdleLocal(BOARD_IS25LP128F_TIMEOUT_MS_MAX_SECTOR_ERASE);
	if(errorSubCode != ERROR_SUB_OK)
    {
        //释放设备使用权
        BoardIS25LP128F_ReleaseLock();
        //日志打印
        MCU_LogShowErrorSubCode(errorSubCode);
        return errorSubCode;
    }
    //关闭写入
    errorSubCode = BoardIS25LP128F_WriteDisable();
    if(errorSubCode != ERROR_SUB_OK)
    {
        //释放设备使用权
        BoardIS25LP128F_ReleaseLock();
        //日志打印
        MCU_LogShowErrorSubCode(errorSubCode);
        return errorSubCode;
    }
    //释放设备使用权
    BoardIS25LP128F_ReleaseLock();
    #if(BOARD_IS25LP128F_OP_TIME_MEASURE_ENABLE == 1)
    MCU_LogPrintf("BoardIS25LP128F_EraseSector sectorNo: %d,Time: %d ms\r\n",sectorNo,tx_time_get() - startTimeStamp);
    #endif
    return ERROR_SUB_OK;
}

//擦除块
ERROR_SUB BoardIS25LP128F_EraseBlock(uint32_t blockNo)
{
    ERROR_SUB errorSubCode = ERROR_SUB_OK;
    //获取设备使用权
    BoardIS25LP128F_GetLock();
    //写使能
    errorSubCode = BoardIS25LP128F_WriteEnable();
    if(errorSubCode != ERROR_SUB_OK)
    {
        //释放设备使用权
        BoardIS25LP128F_ReleaseLock();
        //日志打印
        MCU_LogShowErrorSubCode(errorSubCode);
        return errorSubCode;
    }
    #if(BOARD_IS25LP128F_OP_TIME_MEASURE_ENABLE == 1)
    uint32_t startTimeStamp = tx_time_get();
    #endif
    //擦除指令发送
    errorSubCode = BoardIS25LP128F_SendCmdEraseBlock(blockNo);
    if(errorSubCode != ERROR_SUB_OK)
    {
        //释放设备使用权
        BoardIS25LP128F_ReleaseLock();
        //日志打印
        MCU_LogShowErrorSubCode(errorSubCode);
        return errorSubCode;
    }
    //等待设备执行完成,状态空闲
    errorSubCode = BoardIS25LP128F_WaitDeviceIdleLocal(BOARD_IS25LP128F_TIMEOUT_MS_MAX_BLOCK_64KB_ERASE);
	if(errorSubCode != ERROR_SUB_OK)
    {
        //释放设备使用权
        BoardIS25LP128F_ReleaseLock();
        //日志打印
        MCU_LogShowErrorSubCode(errorSubCode);
        return errorSubCode;
    }
    //关闭写入
    errorSubCode = BoardIS25LP128F_WriteDisable();
    if(errorSubCode != ERROR_SUB_OK)
    {
        //释放设备使用权
        BoardIS25LP128F_ReleaseLock();
        //日志打印
        MCU_LogShowErrorSubCode(errorSubCode);
        return errorSubCode;
    }
    //释放设备使用权
    BoardIS25LP128F_ReleaseLock();
    #if(BOARD_IS25LP128F_OP_TIME_MEASURE_ENABLE == 1)
    MCU_LogPrintf("BoardIS25LP128F_EraseBlock blockNo: %d,Time: %d ms\r\n",blockNo,tx_time_get() - startTimeStamp);
    #endif
    return ERROR_SUB_OK;
}

//擦除全芯片,等待时间500S
ERROR_SUB BoardIS25LP128F_EraseChip(void)
{
    ERROR_SUB errorSubCode = ERROR_SUB_OK;
    //获取设备使用权
    BoardIS25LP128F_GetLock();
    //写使能
    errorSubCode = BoardIS25LP128F_WriteEnable();
    if(errorSubCode != ERROR_SUB_OK)
    {
        //释放设备使用权
        BoardIS25LP128F_ReleaseLock();
        //日志打印
        MCU_LogShowErrorSubCode(errorSubCode);
        return errorSubCode;
    }
    #if(BOARD_IS25LP128F_OP_TIME_MEASURE_ENABLE == 1)
    uint32_t startTimeStamp = tx_time_get();
    #endif
    //擦除指令发送
    errorSubCode = BoardIS25LP128F_SendCmdEraseChip();
    if(errorSubCode != ERROR_SUB_OK)
    {
        //释放设备使用权
        BoardIS25LP128F_ReleaseLock();
        //日志打印
        MCU_LogShowErrorSubCode(errorSubCode);
        return errorSubCode;
    }
    //等待设备执行完成,状态空闲
    errorSubCode = BoardIS25LP128F_WaitDeviceIdleLocal(BOARD_IS25LP128F_TIMEOUT_MS_MAX_CHIP_ERASE);
	if(errorSubCode != ERROR_SUB_OK)
    {
        //释放设备使用权
        BoardIS25LP128F_ReleaseLock();
        //日志打印
        MCU_LogShowErrorSubCode(errorSubCode);
        return errorSubCode;
    }
    //关闭写入
    errorSubCode = BoardIS25LP128F_WriteDisable();
    if(errorSubCode != ERROR_SUB_OK)
    {
        //释放设备使用权
        BoardIS25LP128F_ReleaseLock();
        //日志打印
        MCU_LogShowErrorSubCode(errorSubCode);
        return errorSubCode;
    }
    //释放设备使用权
    BoardIS25LP128F_ReleaseLock();
    #if(BOARD_IS25LP128F_OP_TIME_MEASURE_ENABLE == 1)
    MCU_LogPrintf("BoardIS25LP128F_EraseChip Time: %d ms\r\n",tx_time_get() - startTimeStamp);
    #endif
    return ERROR_SUB_OK;
}


//读取指定地址数据
ERROR_SUB BoardIS25LP128F_ReadBuffer(uint8_t* pBuffer, uint32_t addr, uint16_t dataLength)
{
    ERROR_SUB errorSubCode = ERROR_SUB_OK;
    //获取设备使用权
    BoardIS25LP128F_GetLock();
    //等待设备空闲
    errorSubCode = BoardIS25LP128F_WaitDeviceIdleLocal(BOARD_IS25LP128F_CMD_TIME_OUT);
	if(errorSubCode != ERROR_SUB_OK)
    {
        //释放设备使用权
        BoardIS25LP128F_ReleaseLock();
        //日志打印
        MCU_LogShowErrorSubCode(errorSubCode);
        return errorSubCode;
    }
    #if(BOARD_IS25LP128F_OP_TIME_MEASURE_ENABLE == 1)
    uint32_t startTimeStamp = tx_time_get();
    #endif
    //发送指令读取
    errorSubCode = BoardIS25LP128F_SendCmdReadBuffer(pBuffer,addr,dataLength);
    if(errorSubCode != ERROR_SUB_OK)
    {
        //释放设备使用权
        BoardIS25LP128F_ReleaseLock();
        //日志打印
        MCU_LogShowErrorSubCode(errorSubCode);
        return errorSubCode;
    }
    //读取完成等待设备空闲
    errorSubCode = BoardIS25LP128F_WaitDeviceIdleLocal(BOARD_IS25LP128F_CMD_TIME_OUT);
	if(errorSubCode != ERROR_SUB_OK)
    {
        //释放设备使用权
        BoardIS25LP128F_ReleaseLock();
        //日志打印
        MCU_LogShowErrorSubCode(errorSubCode);
        return errorSubCode;
    }
    //释放设备使用权
    BoardIS25LP128F_ReleaseLock();
    #if(BOARD_IS25LP128F_OP_TIME_MEASURE_ENABLE == 1)
    MCU_LogPrintf("BoardIS25LP128F_ReadBuffer Addr: %d,Length: %d,Time: %d ms\r\n",addr,dataLength,
                        tx_time_get() - startTimeStamp);
    #endif
    return ERROR_SUB_OK;
}

//无检验写SPI FLASH 
//必须确保所写的地址范围内的数据全部为0XFF,否则在非0XFF处写入的数据将失败!
//具有自动换页功能 
//在指定地址开始写入指定长度的数据,但是要确保地址不越界!
ERROR_SUB BoardIS25LP128F_WriteNoCheck(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)
{
    ERROR_SUB errorCodeSub = ERROR_SUB_OK;
    uint16_t pageremain;
    //获取设备使用权
    BoardIS25LP128F_GetLock();
    #if(BOARD_IS25LP128F_OP_TIME_MEASURE_ENABLE == 1)
    uint32_t startTimeStamp = tx_time_get();
    #endif   
    //单页剩余的字节数
	pageremain=BOARD_IS25LP128F_PAGE_SIZE-WriteAddr%BOARD_IS25LP128F_PAGE_SIZE; 
    //不大于256个字节		 	    
	if(NumByteToWrite <= pageremain)
    {
        pageremain = NumByteToWrite;
    }
	while(1)
	{	   
        //页写入
		errorCodeSub = BoardIS25LP128F_WritePage(pBuffer,WriteAddr,pageremain);
        if(errorCodeSub != ERROR_SUB_OK)
        {
            //释放设备使用权
            BoardIS25LP128F_ReleaseLock();
            return errorCodeSub;
        }
		if(NumByteToWrite==pageremain)
        {
            break;//写入结束了
        }
	 	else
		{
            //写入地址增加
			pBuffer += pageremain;
			WriteAddr+=pageremain;	
            //减去已经写入了的字节数
			NumByteToWrite-=pageremain;			  
			if(NumByteToWrite>BOARD_IS25LP128F_PAGE_SIZE)
            {
                //一次可以写入BOARD_IS25LP128F_PAGE_SIZE个字节
                pageremain=BOARD_IS25LP128F_PAGE_SIZE; 
            } 
			else 
            {
                //不够BOARD_IS25LP128F_PAGE_SIZE个字节了
                pageremain=NumByteToWrite; 
            } 	  
		}
	}   
    //释放设备使用权
    BoardIS25LP128F_ReleaseLock();
    #if(BOARD_IS25LP128F_OP_TIME_MEASURE_ENABLE == 1)
    MCU_LogPrintf("BoardIS25LP128F_WriteNoCheck Addr: %d,Length: %d,Time: %d ms\r\n",WriteAddr,NumByteToWrite,
                        tx_time_get() - startTimeStamp);
    #endif
    return ERROR_SUB_OK;
}

//在指定地址开始写入指定长度的数据
//该函数带擦除操作! 
uint8_t is25lp128UtilBuffer[BOARD_IS25LP128F_SECTOR_SIZE] = {0};	
static ERROR_SUB BoardIS25LP128F_WriteBufferSafeLocal(uint8_t* pBuffer, uint32_t addr, uint16_t dataLength)
{
    uint32_t sectorNo = 0;
	uint16_t sectorOffset = 0;
	uint16_t sectorRemainBytes = 0;	   
 	uint16_t indexUtil = 0;    
    ERROR_SUB errorCodeSub = ERROR_SUB_OK;   
    //扇区地址   
    sectorNo = addr/BOARD_IS25LP128F_SECTOR_SIZE; 
    //在扇区内的偏移
    sectorOffset = addr%BOARD_IS25LP128F_SECTOR_SIZE;
    //扇区剩余空间大小 
    sectorRemainBytes = BOARD_IS25LP128F_SECTOR_SIZE-sectorOffset; 
 	if(dataLength<=sectorRemainBytes)
    {
        //不大于4096个字节
        sectorRemainBytes=dataLength;
    }
    //获取设备使用权
    BoardIS25LP128F_GetLock();
	while(1) 
	{	
        //读出整个扇区的内容
		errorCodeSub = BoardIS25LP128F_ReadBuffer(is25lp128UtilBuffer,sectorNo*BOARD_IS25LP128F_SECTOR_SIZE,BOARD_IS25LP128F_SECTOR_SIZE);
        if(errorCodeSub != ERROR_SUB_OK)
        {
            //释放设备使用权
            BoardIS25LP128F_ReleaseLock();
            return errorCodeSub;
        }
		for(indexUtil=0;indexUtil<sectorRemainBytes;indexUtil++)//校验数据
		{
			if(is25lp128UtilBuffer[sectorOffset+indexUtil]!=0XFF)
            {
                break;//需要擦除  	 
            }
		}
		if(indexUtil<sectorRemainBytes)//需要擦除
		{
            //擦除这个扇区
			errorCodeSub = BoardIS25LP128F_EraseSector(sectorNo);
            if(errorCodeSub != ERROR_SUB_OK)
            {
                //释放设备使用权
                BoardIS25LP128F_ReleaseLock();
                return errorCodeSub;
            }
            //复制
			for(indexUtil=0;indexUtil<sectorRemainBytes;indexUtil++)	   
			{
				is25lp128UtilBuffer[indexUtil+sectorOffset]=pBuffer[indexUtil];	  
			}
            //写入整个扇区
			errorCodeSub = BoardIS25LP128F_WriteNoCheck(is25lp128UtilBuffer,sectorNo*BOARD_IS25LP128F_SECTOR_SIZE,BOARD_IS25LP128F_SECTOR_SIZE);  
            if(errorCodeSub != ERROR_SUB_OK)
            {
                //释放设备使用权
                BoardIS25LP128F_ReleaseLock();
                return errorCodeSub;
            }
		}
        else 
        {
            //写已经擦除了的,直接写入扇区剩余区间. 
            errorCodeSub = BoardIS25LP128F_WriteNoCheck(pBuffer,addr,sectorRemainBytes);	
            if(errorCodeSub != ERROR_SUB_OK)
            {
                //释放设备使用权
                BoardIS25LP128F_ReleaseLock();
                return errorCodeSub;
            }
        }			   
		if(dataLength == sectorRemainBytes)
        {
            //写入结束了
            break;
        }
		else
		{
            //写入未结束
			sectorNo++;//扇区地址增1
			sectorOffset=0;//偏移位置为0 	 
		   	pBuffer+=sectorRemainBytes;  //指针偏移
			addr+=sectorRemainBytes;//写地址偏移	   
		   	dataLength-=sectorRemainBytes;//字节数递减
			if(dataLength > BOARD_IS25LP128F_SECTOR_SIZE)
            {
                //下一个扇区还是写不完
                sectorRemainBytes = BOARD_IS25LP128F_SECTOR_SIZE;
            }
			else 
            {
                //下一个扇区可以写完了
                sectorRemainBytes = dataLength;
            }			
		}	 
	};
    //释放设备使用权
    BoardIS25LP128F_ReleaseLock();
    return ERROR_SUB_OK;
}

ERROR_SUB BoardIS25LP128F_WriteBufferSafe(uint8_t* pBuffer, uint32_t addr, uint16_t dataLength)
{
    ERROR_SUB errorCodeSub = ERROR_SUB_OK;
    //获取设备使用权
    BoardIS25LP128F_GetLock();
    //首先进行写入数据
    errorCodeSub = BoardIS25LP128F_WriteBufferSafeLocal(pBuffer,addr,dataLength);
    if(errorCodeSub != ERROR_SUB_OK)
    {
        //释放设备使用权
        BoardIS25LP128F_ReleaseLock();
        return errorCodeSub;
    }
    //申请内存
    uint8_t* readBufferPtr = MCU_MemMallocWhileSuccessAxiSram(dataLength);
    //读取数据
    errorCodeSub = BoardIS25LP128F_ReadBuffer(readBufferPtr,addr,dataLength);
    //数据匹配
    int32_t memCmpResult = 0;
    memCmpResult = UserMemCmp(pBuffer,readBufferPtr,dataLength);
    //释放内存
    MCU_MemFreeAxiSram(readBufferPtr);
    //释放设备使用权
    BoardIS25LP128F_ReleaseLock();
    if(memCmpResult != 0)
    {
        return ERROR_SUB_BOARD_IS25LP128F_WRITE_READ_MATCH;
    }
    return ERROR_SUB_OK;
}


//程序自检
ERROR_SUB BoardIS25LP128F_Check(void)
{
    ERROR_SUB errorCodeSub = ERROR_SUB_OK;
    uint32_t testAddr = MCU_RandomGetNextUnsigned((BOARD_IS25LP128F_FLASH_SIZE/2+1),BOARD_IS25LP128F_FLASH_SIZE-4);
    uint32_t testValue = MCU_RandomGetNextUnsigned(0,UINT32_MAX);
    uint32_t srcDat = 0;
    uint32_t readDat = 0;
    //开始自检
    uint32_t deviceID = 0X00000000;
    errorCodeSub = BoardIS25LP128F_ReadID(&deviceID);
    if(errorCodeSub != ERROR_SUB_OK)
    {
        return errorCodeSub;
    }
    if(deviceID != BOARD_IS25LP128F_ID)
    {
        MCU_LogPrintf("BoardIS25LP128F_ReadID : 0X%08X\r\n",deviceID);
        return ERROR_SUB_BOARD_IS25LP128F_ID_MATCH;
    }
    //读写测试
    //自检开始
    MCU_LogPrintf("%s Start,Addr: 0X%08X\r\n",__func__,testAddr);
    //获取设备使用权
    BoardIS25LP128F_GetLock();
    //读取源
    errorCodeSub = BoardIS25LP128F_ReadBuffer((uint8_t*)(&srcDat),testAddr,4);
    if(errorCodeSub != ERROR_SUB_OK)
    {
        //释放设备使用权
        BoardIS25LP128F_ReleaseLock();
        //错误日志打印
        MCU_LogShowErrorSubCode(errorCodeSub);
        return errorCodeSub;
    }
    //写入新的数据
    errorCodeSub = BoardIS25LP128F_WriteBufferSafe((uint8_t*)(&testValue),testAddr,4);
    if(errorCodeSub != ERROR_SUB_OK)
    {
        //释放设备使用权
        BoardIS25LP128F_ReleaseLock();
        //错误日志打印
        MCU_LogShowErrorSubCode(errorCodeSub);
        return errorCodeSub;
    }
    //读取出来
    errorCodeSub = BoardIS25LP128F_ReadBuffer((uint8_t*)(&readDat),testAddr,4);
    if(errorCodeSub != ERROR_SUB_OK)
    {
        //释放设备使用权
        BoardIS25LP128F_ReleaseLock();
        //错误日志打印
        MCU_LogShowErrorSubCode(errorCodeSub);
        return errorCodeSub;
    }
    //比较数据
    if(readDat != testValue)
    {
        //释放设备使用权
        BoardIS25LP128F_ReleaseLock();
        //错误日志打印
        MCU_LogShowErrorSubCode(errorCodeSub);
        return ERROR_SUB_BOARD_IS25LP128F_CHECK_FIRST;
    }
    //原始数据写回去
    errorCodeSub = BoardIS25LP128F_WriteBufferSafe((uint8_t*)(&srcDat),testAddr,4);
    if(errorCodeSub != ERROR_SUB_OK)
    {
        //释放设备使用权
        BoardIS25LP128F_ReleaseLock();
        //错误日志打印
        MCU_LogShowErrorSubCode(errorCodeSub);
        return errorCodeSub;
    }
    //设置异常值
    readDat = 0XAA5555AA;
    //读取出来
    errorCodeSub = BoardIS25LP128F_ReadBuffer((uint8_t*)(&readDat),testAddr,4);
    if(errorCodeSub != ERROR_SUB_OK)
    {
        //释放设备使用权
        BoardIS25LP128F_ReleaseLock();
        //错误日志打印
        MCU_LogShowErrorSubCode(errorCodeSub);
        return errorCodeSub;
    }
    //比较数据
    if(readDat != srcDat)
    {
        //释放设备使用权
        BoardIS25LP128F_ReleaseLock();
        //错误日志打印
        MCU_LogShowErrorSubCode(errorCodeSub);
        return ERROR_SUB_BOARD_IS25LP128F_CHECK_SECOND;
    }
    //释放设备使用权
    BoardIS25LP128F_ReleaseLock();
    //成功
    MCU_LogPrintf("%s Success Over\r\n",__func__);
    return ERROR_SUB_OK;
}


//测试用例
#if(BOARD_IS25LP128F_TEST_CASE_ENABLE == CONFIG_FUNC_ENABLE)
//查询周期
#define MS_BOARD_IS25LP128F_TEST_CASE_PERIOD                5000
#define IS25LP128F_TEST_BUF_LEN                             BOARD_IS25LP128F_SECTOR_SIZE
//上一次执行时间
static uint32_t lastTickW25q256TestCase;
//测试用例初始化
void BoardIS25LP128F_TestCaseInit(void)
{
    //上一次执行时间
    lastTickW25q256TestCase = tx_time_get();
}

#include "SrvTaskHeader.h"
#include "UserMemManage.h"
//测试用例周期运转
void BoardIS25LP128F_TestCaseRunPeriod(void)
{
    ERROR_SUB errorCodeSub = ERROR_SUB_OK;
    if(tx_time_get() - lastTickW25q256TestCase < MS_BOARD_IS25LP128F_TEST_CASE_PERIOD)
    {
        //还没到周期
        return;
    }
    lastTickW25q256TestCase = tx_time_get();
    uint16_t indexUtil = 0;
    uint8_t* srcDataBufPtr = UserMemMallocWhileSuccess(MEM_AXI_SRAM,IS25LP128F_TEST_BUF_LEN);
    uint8_t* writeDataBufPtr = UserMemMallocWhileSuccess(MEM_AXI_SRAM,IS25LP128F_TEST_BUF_LEN);
    uint8_t* readDataBufPtr = UserMemMallocWhileSuccess(MEM_AXI_SRAM,IS25LP128F_TEST_BUF_LEN);
    uint32_t testAddr = MCU_RandomGetNextUnsigned((BOARD_IS25LP128F_FLASH_SIZE/2+1),BOARD_IS25LP128F_FLASH_SIZE-IS25LP128F_TEST_BUF_LEN);
    SystemPrintf("\r\n%s ,Start testAddr: 0X%08X\r\n",__func__,testAddr);
    //读取源数据
    errorCodeSub = BoardIS25LP128F_ReadBuffer(srcDataBufPtr,testAddr,IS25LP128F_TEST_BUF_LEN);
    if(errorCodeSub != ERROR_SUB_OK)
    {
        SystemPrintf("BoardIS25LP128F_ReadBuffer Src First, 0X%08X\r\n",errorCodeSub);
        //内存释放
        UserMemFree(MEM_AXI_SRAM,srcDataBufPtr);
        UserMemFree(MEM_AXI_SRAM,writeDataBufPtr);
        UserMemFree(MEM_AXI_SRAM,readDataBufPtr);
        while(1);
    }
    //写入新数据
    for(indexUtil = 0; indexUtil < IS25LP128F_TEST_BUF_LEN;indexUtil++)
    {
        writeDataBufPtr[indexUtil] = (uint8_t)MCU_RandomGetNextUnsigned(0,255);
    }
    //写入指定地址
    errorCodeSub = BoardIS25LP128F_WriteBufferSafe(writeDataBufPtr,testAddr,IS25LP128F_TEST_BUF_LEN);
    if(errorCodeSub != ERROR_SUB_OK)
    {
        SystemPrintf("BoardIS25LP128F_WriteBufferSafe Create, 0X%08X\r\n",errorCodeSub);
        //内存释放
        UserMemFree(MEM_AXI_SRAM,srcDataBufPtr);
        UserMemFree(MEM_AXI_SRAM,writeDataBufPtr);
        UserMemFree(MEM_AXI_SRAM,readDataBufPtr);
        while(1);
    }
    //读取新数据
    errorCodeSub = BoardIS25LP128F_ReadBuffer(readDataBufPtr,testAddr,IS25LP128F_TEST_BUF_LEN);
    if(errorCodeSub != ERROR_SUB_OK)
    {
        SystemPrintf("BoardIS25LP128F_ReadBuffer Create, 0X%08X\r\n",errorCodeSub);
        //内存释放
        UserMemFree(MEM_AXI_SRAM,srcDataBufPtr);
        UserMemFree(MEM_AXI_SRAM,writeDataBufPtr);
        UserMemFree(MEM_AXI_SRAM,readDataBufPtr);
        while(1);
    }
    //匹配新数据
    for(indexUtil = 0; indexUtil < IS25LP128F_TEST_BUF_LEN;indexUtil++)
    {
        if(writeDataBufPtr[indexUtil] != readDataBufPtr[indexUtil])
        {
            SystemPrintf("First Index: %d WriteDat: %d ReadDat : %d\r\n",indexUtil,writeDataBufPtr[indexUtil],
                                readDataBufPtr[indexUtil]);
            //内存释放
            UserMemFree(MEM_AXI_SRAM,srcDataBufPtr);
            UserMemFree(MEM_AXI_SRAM,writeDataBufPtr);
            UserMemFree(MEM_AXI_SRAM,readDataBufPtr);
            while(1);
        }
    }
    //写入原始数据
    errorCodeSub = BoardIS25LP128F_WriteBufferSafe(srcDataBufPtr,testAddr,IS25LP128F_TEST_BUF_LEN);
    if(errorCodeSub != ERROR_SUB_OK)
    {
        SystemPrintf("BoardIS25LP128F_WriteBufferSafe Src, 0X%08X\r\n",errorCodeSub);
        //内存释放
        UserMemFree(MEM_AXI_SRAM,srcDataBufPtr);
        UserMemFree(MEM_AXI_SRAM,writeDataBufPtr);
        UserMemFree(MEM_AXI_SRAM,readDataBufPtr);
        while(1);
    }
    //读取原始数据
    errorCodeSub = BoardIS25LP128F_ReadBuffer(readDataBufPtr,testAddr,IS25LP128F_TEST_BUF_LEN);
    if(errorCodeSub != ERROR_SUB_OK)
    {
        SystemPrintf("BoardIS25LP128F_ReadBuffer SrcSecond, 0X%08X\r\n",errorCodeSub);
        //内存释放
        UserMemFree(MEM_AXI_SRAM,srcDataBufPtr);
        UserMemFree(MEM_AXI_SRAM,writeDataBufPtr);
        UserMemFree(MEM_AXI_SRAM,readDataBufPtr);
        while(1);
    }
    //匹配原始数据
    for(indexUtil = 0; indexUtil < IS25LP128F_TEST_BUF_LEN;indexUtil++)
    {
        if(srcDataBufPtr[indexUtil] != readDataBufPtr[indexUtil])
        {
            SystemPrintf("Second Index: %d WriteDat: %d ReadDat : %d\r\n",indexUtil,srcDataBufPtr[indexUtil],
                                readDataBufPtr[indexUtil]);
            //内存释放
            UserMemFree(MEM_AXI_SRAM,srcDataBufPtr);
            UserMemFree(MEM_AXI_SRAM,writeDataBufPtr);
            UserMemFree(MEM_AXI_SRAM,readDataBufPtr);
            while(1);
        }
    }
    //内存释放
    UserMemFree(MEM_AXI_SRAM,srcDataBufPtr);
    UserMemFree(MEM_AXI_SRAM,writeDataBufPtr);
    UserMemFree(MEM_AXI_SRAM,readDataBufPtr);
    SystemPrintf("\r\n%s Success Over\r\n",__func__);
    return;
}

#endif
























