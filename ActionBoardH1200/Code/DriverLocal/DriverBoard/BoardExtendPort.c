/************************************************************************ 
 ** Author       : 邓小俊
 ** Date         : 2024-01-16 11:35:28
 ** Description  : When I Has Free Time ,I Will Write Description Below.
 ** description  : 
 ** LastEditors  : 邓小俊
 ** LastEditTime : 2024-01-16 13:15:06
 ** Email: xiaojun.deng@aliyun.com
 ** Copyright (c) 2024 by 公司名称, All Rights Reserved. 
 ************************************************************************/
#include "BoardExtendPort.h"
#include "BoardTCA9555PWR.h"
#include "DriverConfigBoard.h"

//定义扩展端口访问互斥量
static TX_MUTEX mutexExtendPortOut1;
static TX_MUTEX mutexExtendPortIn1;
static TCA9555_PORT_POLARITY_SET extendPortOutPolarity;
static TCA9555_PORT_DIR_SET extendPortOutDir;
static TCA9555_PORT_POLARITY_SET extendPortInPolarity;
static TCA9555_PORT_DIR_SET extendPortInDir;

//按键映射IO口
#define BOARD_EXTEND_PORT_INT_PIN                   MCU_PIN_B_5
//按键上下拉模式
#define BOARD_EXTEND_PORT_INT_PULL_MODE             MCU_PIN_PULL_PULLUP
//按键速度
#define BOARD_EXTEND_PORT_INT_SPEED                 MCU_PIN_SPEED_FREQ_MEDIUM


//扩展端口初始化
void BoardExtendPortInit(void)
{
    BoardTCA9555PWR_Init(&mutexExtendPortOut1,"mutexExtendPortOut1");
    BoardTCA9555PWR_Init(&mutexExtendPortIn1,"mutexExtendPortIn1");
    //输入中断初始化
    MCU_PinInit(BOARD_EXTEND_PORT_INT_PIN, MCU_PIN_MODE_INPUT, BOARD_EXTEND_PORT_INT_PULL_MODE, BOARD_EXTEND_PORT_INT_SPEED,0);
    //初始化输出极性和方向
    extendPortOutPolarity.regValueSingle.pin1Polarity = TCA9555_PORT_POLARITY_RETAIN;
    extendPortOutPolarity.regValueSingle.pin2Polarity = TCA9555_PORT_POLARITY_RETAIN;
    extendPortOutPolarity.regValueSingle.pin3Polarity = TCA9555_PORT_POLARITY_RETAIN;
    extendPortOutPolarity.regValueSingle.pin4Polarity = TCA9555_PORT_POLARITY_RETAIN;
    extendPortOutPolarity.regValueSingle.pin5Polarity = TCA9555_PORT_POLARITY_RETAIN;
    extendPortOutPolarity.regValueSingle.pin6Polarity = TCA9555_PORT_POLARITY_RETAIN;
    extendPortOutPolarity.regValueSingle.pin7Polarity = TCA9555_PORT_POLARITY_RETAIN;
    extendPortOutPolarity.regValueSingle.pin8Polarity = TCA9555_PORT_POLARITY_RETAIN;
    extendPortOutPolarity.regValueSingle.pin9Polarity = TCA9555_PORT_POLARITY_RETAIN;
    extendPortOutPolarity.regValueSingle.pin10Polarity = TCA9555_PORT_POLARITY_RETAIN;
    extendPortOutPolarity.regValueSingle.pin11Polarity = TCA9555_PORT_POLARITY_RETAIN;
    extendPortOutPolarity.regValueSingle.pin12Polarity = TCA9555_PORT_POLARITY_RETAIN;
    extendPortOutPolarity.regValueSingle.pin13Polarity = TCA9555_PORT_POLARITY_RETAIN;
    extendPortOutPolarity.regValueSingle.pin14Polarity = TCA9555_PORT_POLARITY_RETAIN;
    extendPortOutPolarity.regValueSingle.pin15Polarity = TCA9555_PORT_POLARITY_RETAIN;
    extendPortOutPolarity.regValueSingle.pin16Polarity = TCA9555_PORT_POLARITY_RETAIN;
    extendPortOutDir.regValueSingle.pin1Dir = TCA9555_PORT_DIR_OUT;
    extendPortOutDir.regValueSingle.pin2Dir = TCA9555_PORT_DIR_OUT;
    extendPortOutDir.regValueSingle.pin3Dir = TCA9555_PORT_DIR_OUT;
    extendPortOutDir.regValueSingle.pin4Dir = TCA9555_PORT_DIR_OUT;
    extendPortOutDir.regValueSingle.pin5Dir = TCA9555_PORT_DIR_OUT;
    extendPortOutDir.regValueSingle.pin6Dir = TCA9555_PORT_DIR_OUT;
    extendPortOutDir.regValueSingle.pin7Dir = TCA9555_PORT_DIR_OUT;
    extendPortOutDir.regValueSingle.pin8Dir = TCA9555_PORT_DIR_OUT;
    extendPortOutDir.regValueSingle.pin9Dir = TCA9555_PORT_DIR_OUT;
    extendPortOutDir.regValueSingle.pin10Dir = TCA9555_PORT_DIR_OUT;
    extendPortOutDir.regValueSingle.pin11Dir = TCA9555_PORT_DIR_OUT;
    extendPortOutDir.regValueSingle.pin12Dir = TCA9555_PORT_DIR_OUT;
    extendPortOutDir.regValueSingle.pin13Dir = TCA9555_PORT_DIR_OUT;
    extendPortOutDir.regValueSingle.pin14Dir = TCA9555_PORT_DIR_OUT;
    extendPortOutDir.regValueSingle.pin15Dir = TCA9555_PORT_DIR_OUT;
    extendPortOutDir.regValueSingle.pin16Dir = TCA9555_PORT_DIR_OUT;
    //初始化输入极性和方向
    extendPortInPolarity.regValueSingle.pin1Polarity = TCA9555_PORT_POLARITY_RETAIN;
    extendPortInPolarity.regValueSingle.pin2Polarity = TCA9555_PORT_POLARITY_RETAIN;
    extendPortInPolarity.regValueSingle.pin3Polarity = TCA9555_PORT_POLARITY_RETAIN;
    extendPortInPolarity.regValueSingle.pin4Polarity = TCA9555_PORT_POLARITY_RETAIN;
    extendPortInPolarity.regValueSingle.pin5Polarity = TCA9555_PORT_POLARITY_RETAIN;
    extendPortInPolarity.regValueSingle.pin6Polarity = TCA9555_PORT_POLARITY_RETAIN;
    extendPortInPolarity.regValueSingle.pin7Polarity = TCA9555_PORT_POLARITY_RETAIN;
    extendPortInPolarity.regValueSingle.pin8Polarity = TCA9555_PORT_POLARITY_RETAIN;
    extendPortInPolarity.regValueSingle.pin9Polarity = TCA9555_PORT_POLARITY_RETAIN;
    extendPortInPolarity.regValueSingle.pin10Polarity = TCA9555_PORT_POLARITY_RETAIN;
    extendPortInPolarity.regValueSingle.pin11Polarity = TCA9555_PORT_POLARITY_RETAIN;
    extendPortInPolarity.regValueSingle.pin12Polarity = TCA9555_PORT_POLARITY_RETAIN;
    extendPortInPolarity.regValueSingle.pin13Polarity = TCA9555_PORT_POLARITY_RETAIN;
    extendPortInPolarity.regValueSingle.pin14Polarity = TCA9555_PORT_POLARITY_RETAIN;
    extendPortInPolarity.regValueSingle.pin15Polarity = TCA9555_PORT_POLARITY_RETAIN;
    extendPortInPolarity.regValueSingle.pin16Polarity = TCA9555_PORT_POLARITY_RETAIN;
    extendPortInDir.regValueSingle.pin1Dir = TCA9555_PORT_DIR_IN;
    extendPortInDir.regValueSingle.pin2Dir = TCA9555_PORT_DIR_IN;
    extendPortInDir.regValueSingle.pin3Dir = TCA9555_PORT_DIR_IN;
    extendPortInDir.regValueSingle.pin4Dir = TCA9555_PORT_DIR_IN;
    extendPortInDir.regValueSingle.pin5Dir = TCA9555_PORT_DIR_IN;
    extendPortInDir.regValueSingle.pin6Dir = TCA9555_PORT_DIR_IN;
    extendPortInDir.regValueSingle.pin7Dir = TCA9555_PORT_DIR_IN;
    extendPortInDir.regValueSingle.pin8Dir = TCA9555_PORT_DIR_IN;
    extendPortInDir.regValueSingle.pin9Dir = TCA9555_PORT_DIR_IN;
    extendPortInDir.regValueSingle.pin10Dir = TCA9555_PORT_DIR_IN;
    extendPortInDir.regValueSingle.pin11Dir = TCA9555_PORT_DIR_IN;
    extendPortInDir.regValueSingle.pin12Dir = TCA9555_PORT_DIR_IN;
    extendPortInDir.regValueSingle.pin13Dir = TCA9555_PORT_DIR_IN;
    extendPortInDir.regValueSingle.pin14Dir = TCA9555_PORT_DIR_IN;
    extendPortInDir.regValueSingle.pin15Dir = TCA9555_PORT_DIR_IN;
    extendPortInDir.regValueSingle.pin16Dir = TCA9555_PORT_DIR_IN;
}

//配置扩展端口默认值
ERROR_SUB BoardExtendPortConfigDefault(void)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //首先检测扩展芯片均存在
    errorSub = BoardTCA9555PWR_WaitIdle(ADDR_TCA9555_EXTEND_PORT_OUT_1,&mutexExtendPortOut1,TIMES_WAIT_TCA9555_EXTEND_PORT);
    if(errorSub != ERROR_SUB_OK)
    {
        MCU_LogShowErrorSubCode(errorSub);
        return errorSub;
    }
    errorSub = BoardTCA9555PWR_WaitIdle(ADDR_TCA9555_EXTEND_PORT_IN_1,&mutexExtendPortIn1,TIMES_WAIT_TCA9555_EXTEND_PORT);
    if(errorSub != ERROR_SUB_OK)
    {
        MCU_LogShowErrorSubCode(errorSub);
        return errorSub;
    }
    //设置端口输出1极性和方向
    errorSub = BoardTCA9555PWR_SetAllPolarity(ADDR_TCA9555_EXTEND_PORT_OUT_1,&mutexExtendPortOut1,&extendPortOutPolarity);
    if(errorSub != ERROR_SUB_OK)
    {
        MCU_LogShowErrorSubCode(errorSub);
        return errorSub;
    }
    errorSub = BoardTCA9555PWR_SetAllDir(ADDR_TCA9555_EXTEND_PORT_OUT_1,&mutexExtendPortOut1,&extendPortOutDir);
    if(errorSub != ERROR_SUB_OK)
    {
        MCU_LogShowErrorSubCode(errorSub);
        return errorSub;
    }
    //设置端口输入1极性和方向
    errorSub = BoardTCA9555PWR_SetAllPolarity(ADDR_TCA9555_EXTEND_PORT_IN_1,&mutexExtendPortIn1,&extendPortInPolarity);
    if(errorSub != ERROR_SUB_OK)
    {
        MCU_LogShowErrorSubCode(errorSub);
        return errorSub;
    }
    errorSub = BoardTCA9555PWR_SetAllDir(ADDR_TCA9555_EXTEND_PORT_IN_1,&mutexExtendPortIn1,&extendPortInDir);
    if(errorSub != ERROR_SUB_OK)
    {
        MCU_LogShowErrorSubCode(errorSub);
        return errorSub;
    }
    return ERROR_SUB_OK;
}

//输出端口输出指定值
ERROR_SUB BoardExtendPortOutWriteSingle(BOARD_EXTEND_PORT_OUT_PIN outPin,GPIO_PinState outValue)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    if(outPin <= BOARD_EXTEND_PORT_OUT_PIN_16)
    {
        errorSub = BoardTCA9555PWR_WriteSinglePortOut(ADDR_TCA9555_EXTEND_PORT_OUT_1,&mutexExtendPortOut1,(TCA9555_PIN)outPin,outValue);
    }
    else
    {
        errorSub = ERROR_SUB_BOARD_TCA9555PWR_INVALID_PIN;
    }
    if(errorSub != ERROR_SUB_OK)
    {
        MCU_LogShowErrorSubCode(errorSub);
        return errorSub;
    }
    return ERROR_SUB_OK;
}

//输出端口获取当前输出值
ERROR_SUB BoardExtendPortOutReadSingleCurrent(BOARD_EXTEND_PORT_OUT_PIN outPin,GPIO_PinState* outValuePtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    *outValuePtr = GPIO_PIN_RESET;
    if(outPin <= BOARD_EXTEND_PORT_OUT_PIN_16)
    {
        errorSub = BoardTCA9555PWR_ReadSinglePortOutCurrent(ADDR_TCA9555_EXTEND_PORT_OUT_1,&mutexExtendPortOut1,(TCA9555_PIN)outPin,outValuePtr);
    }
    else
    {
        errorSub = ERROR_SUB_BOARD_TCA9555PWR_INVALID_PIN;
    }
    if(errorSub != ERROR_SUB_OK)
    {
        MCU_LogShowErrorSubCode(errorSub);
        return errorSub;
    }
    return ERROR_SUB_OK;
}

//输出端口切换状态
ERROR_SUB BoardExtendPortOutToogleSingle(BOARD_EXTEND_PORT_OUT_PIN outPin)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    if(outPin <= BOARD_EXTEND_PORT_OUT_PIN_16)
    {
        errorSub = BoardTCA9555PWR_ToogleSinglePortOut(ADDR_TCA9555_EXTEND_PORT_OUT_1,&mutexExtendPortOut1,(TCA9555_PIN)outPin);
    }
    else
    {
        errorSub = ERROR_SUB_BOARD_TCA9555PWR_INVALID_PIN;
    }
    if(errorSub != ERROR_SUB_OK)
    {
        MCU_LogShowErrorSubCode(errorSub);
        return errorSub;
    }
    return ERROR_SUB_OK;
}

//获取输入端口状态
ERROR_SUB BoardExtendPortInSingle(BOARD_EXTEND_PORT_IN_PIN inPin,GPIO_PinState* inValuePtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    *inValuePtr = GPIO_PIN_RESET;
    if(inPin <= BOARD_EXTEND_PORT_IN_PIN_16)
    {
        errorSub = BoardTCA9555PWR_ReadSinglePortIn(ADDR_TCA9555_EXTEND_PORT_IN_1,&mutexExtendPortIn1,(TCA9555_PIN)inPin,inValuePtr);
    }
    else
    {
        errorSub = ERROR_SUB_BOARD_TCA9555PWR_INVALID_PIN;
    }
    if(errorSub != ERROR_SUB_OK)
    {
        MCU_LogShowErrorSubCode(errorSub);
        return errorSub;
    }
    return ERROR_SUB_OK;
}

//单次获取全部扩展端口讯息
ERROR_SUB BoardExtendPortInAll(BOARD_EXTEND_PORT_IN_SET* portInValSetPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    portInValSetPtr->port1ValSet.regVal2Byte = 0X0000;
    //开始读取
    errorSub = BoardTCA9555PWR_ReadAllPortIn(ADDR_TCA9555_EXTEND_PORT_IN_1,&mutexExtendPortIn1,&(portInValSetPtr->port1ValSet));
    if(errorSub != ERROR_SUB_OK)
    {
        MCU_LogShowErrorSubCode(errorSub);
        return errorSub;
    }
    return errorSub;
}




