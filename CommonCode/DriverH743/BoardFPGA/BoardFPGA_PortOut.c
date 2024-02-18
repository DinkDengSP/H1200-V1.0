/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-04-26 10:17:30 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-04-26 17:24:11 +0800
************************************************************************************************/ 
#include "BoardFPGA_PortOut.h"

/**************************************PORTA函数******************************************/
//读取输出有效电平
FPGA_PORT_VALID_LEVEL BoardFPGA_PortOutReadValidLevelA(void)
{
    FPGA_PORT_VALID_LEVEL validLevelSet;
    validLevelSet.valueAll = BoardFPGA_ReadReg32(FPGA_REG_READ_ADDR_PORT_OUT_GROUP_A_MODE);
    return validLevelSet;
}

//设置输出有效电平
ERROR_SUB BoardFPGA_PortOutWriteValidLevelA(FPGA_PORT_VALID_LEVEL validLevelSet)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    errorSub = BoardFPGA_WriteDataByAddrWithCheck(FPGA_REG_WRITE_ADDR_PORT_OUT_GROUP_A_MODE,validLevelSet.valueAll,
                                                            FPGA_REG_READ_ADDR_PORT_OUT_GROUP_A_MODE);
    return errorSub;
}

//设置输出状态
ERROR_SUB BoardFPGA_PortOutWriteSetValA(FPGA_PORT_OUT_SET portValSet)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    errorSub = BoardFPGA_WriteDataByAddrWithCheck(FPGA_REG_WRITE_ADDR_PORT_OUT_GROUP_A_STATE,portValSet.valAll,
                                                            FPGA_REG_READ_ADDR_PORT_OUT_GROUP_A_SET);
    return errorSub;
}

//读取设置的输出状态
FPGA_PORT_OUT_SET BoardFPGA_PortOutReadSetValA(void)
{
    FPGA_PORT_OUT_SET portVal;
    portVal.valAll = BoardFPGA_ReadReg32(FPGA_REG_READ_ADDR_PORT_OUT_GROUP_A_SET);
    return portVal;
}

//读取实际输出状态
FPGA_PORT_OUT_REALLY BoardFPGA_PortOutReadReallyStateA(void)
{
    FPGA_PORT_OUT_REALLY portReally;
    portReally.valAll = BoardFPGA_ReadReg32(FPGA_REG_READ_ADDR_PORT_OUT_GROUP_A_REAL);
    return portReally;
}

/**************************************PORTB函数******************************************/
//读取输出有效电平
FPGA_PORT_VALID_LEVEL BoardFPGA_PortOutReadValidLevelB(void)
{
    FPGA_PORT_VALID_LEVEL validLevelSet;
    validLevelSet.valueAll = BoardFPGA_ReadReg32(FPGA_REG_READ_ADDR_PORT_OUT_GROUP_B_MODE);
    return validLevelSet;
}

//设置输出有效电平
ERROR_SUB BoardFPGA_PortOutWriteValidLevelB(FPGA_PORT_VALID_LEVEL validLevelSet)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    errorSub = BoardFPGA_WriteDataByAddrWithCheck(FPGA_REG_WRITE_ADDR_PORT_OUT_GROUP_B_MODE,validLevelSet.valueAll,
                                                            FPGA_REG_READ_ADDR_PORT_OUT_GROUP_B_MODE);
    return errorSub;
}

//设置输出状态
ERROR_SUB BoardFPGA_PortOutWriteSetValB(FPGA_PORT_OUT_SET portValSet)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    errorSub = BoardFPGA_WriteDataByAddrWithCheck(FPGA_REG_WRITE_ADDR_PORT_OUT_GROUP_B_STATE,portValSet.valAll,
                                                            FPGA_REG_READ_ADDR_PORT_OUT_GROUP_B_SET);
    return errorSub;
}

//读取设置的输出状态
FPGA_PORT_OUT_SET BoardFPGA_PortOutReadSetValB(void)
{
    FPGA_PORT_OUT_SET portVal;
    portVal.valAll = BoardFPGA_ReadReg32(FPGA_REG_READ_ADDR_PORT_OUT_GROUP_B_SET);
    return portVal;
}

//读取实际输出状态
FPGA_PORT_OUT_REALLY BoardFPGA_PortOutReadReallyStateB(void)
{
    FPGA_PORT_OUT_REALLY portReally;
    portReally.valAll = BoardFPGA_ReadReg32(FPGA_REG_READ_ADDR_PORT_OUT_GROUP_B_REAL);
    return portReally;
}


/**************************************PORTC函数******************************************/
//读取输出有效电平
FPGA_PORT_VALID_LEVEL BoardFPGA_PortOutReadValidLevelC(void)
{
    FPGA_PORT_VALID_LEVEL validLevelSet;
    validLevelSet.valueAll = BoardFPGA_ReadReg32(FPGA_REG_READ_ADDR_PORT_OUT_GROUP_C_MODE);
    return validLevelSet;
}

//设置输出有效电平
ERROR_SUB BoardFPGA_PortOutWriteValidLevelC(FPGA_PORT_VALID_LEVEL validLevelSet)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    errorSub = BoardFPGA_WriteDataByAddrWithCheck(FPGA_REG_WRITE_ADDR_PORT_OUT_GROUP_C_MODE,validLevelSet.valueAll,
                                                            FPGA_REG_READ_ADDR_PORT_OUT_GROUP_C_MODE);
    return errorSub;
}

//设置输出状态
ERROR_SUB BoardFPGA_PortOutWriteSetValC(FPGA_PORT_OUT_SET portValSet)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    errorSub = BoardFPGA_WriteDataByAddrWithCheck(FPGA_REG_WRITE_ADDR_PORT_OUT_GROUP_C_STATE,portValSet.valAll,
                                                            FPGA_REG_READ_ADDR_PORT_OUT_GROUP_C_SET);
    return errorSub;
}

//读取设置的输出状态
FPGA_PORT_OUT_SET BoardFPGA_PortOutReadSetValC(void)
{
    FPGA_PORT_OUT_SET portVal;
    portVal.valAll = BoardFPGA_ReadReg32(FPGA_REG_READ_ADDR_PORT_OUT_GROUP_C_SET);
    return portVal;
}

//读取实际输出状态
FPGA_PORT_OUT_REALLY BoardFPGA_PortOutReadReallyStateC(void)
{
    FPGA_PORT_OUT_REALLY portReally;
    portReally.valAll = BoardFPGA_ReadReg32(FPGA_REG_READ_ADDR_PORT_OUT_GROUP_C_REAL);
    return portReally;
}


/**************************************PORTD函数******************************************/
//读取输出有效电平
FPGA_PORT_VALID_LEVEL BoardFPGA_PortOutReadValidLevelD(void)
{
    FPGA_PORT_VALID_LEVEL validLevelSet;
    validLevelSet.valueAll = BoardFPGA_ReadReg32(FPGA_REG_READ_ADDR_PORT_OUT_GROUP_D_MODE);
    return validLevelSet;
}

//设置输出有效电平
ERROR_SUB BoardFPGA_PortOutWriteValidLevelD(FPGA_PORT_VALID_LEVEL validLevelSet)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    errorSub = BoardFPGA_WriteDataByAddrWithCheck(FPGA_REG_WRITE_ADDR_PORT_OUT_GROUP_D_MODE,validLevelSet.valueAll,
                                                            FPGA_REG_READ_ADDR_PORT_OUT_GROUP_D_MODE);
    return errorSub;
}

//设置输出状态
ERROR_SUB BoardFPGA_PortOutWriteSetValD(FPGA_PORT_OUT_SET portValSet)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    errorSub = BoardFPGA_WriteDataByAddrWithCheck(FPGA_REG_WRITE_ADDR_PORT_OUT_GROUP_D_STATE,portValSet.valAll,
                                                            FPGA_REG_READ_ADDR_PORT_OUT_GROUP_D_SET);
    return errorSub;
}

//读取设置的输出状态
FPGA_PORT_OUT_SET BoardFPGA_PortOutReadSetValD(void)
{
    FPGA_PORT_OUT_SET portVal;
    portVal.valAll = BoardFPGA_ReadReg32(FPGA_REG_READ_ADDR_PORT_OUT_GROUP_D_SET);
    return portVal;
}

//读取实际输出状态
FPGA_PORT_OUT_REALLY BoardFPGA_PortOutReadReallyStateD(void)
{
    FPGA_PORT_OUT_REALLY portReally;
    portReally.valAll = BoardFPGA_ReadReg32(FPGA_REG_READ_ADDR_PORT_OUT_GROUP_D_REAL);
    return portReally;
}




/**************************************函数数组******************************************/
//读取输出有效电平
static const BoardFPGA_PortOutReadValidLevelFunc portOutReadValidLevelFuncArray[BOARD_FPGA_PORT_OUT_MAX] = {
    BoardFPGA_PortOutReadValidLevelA,
    BoardFPGA_PortOutReadValidLevelB,
    BoardFPGA_PortOutReadValidLevelC,
    BoardFPGA_PortOutReadValidLevelD,
};

//设置输出有效电平
static const BoardFPGA_PortOutWriteValidLevelFunc portOutWriteValidLevelFuncArray[BOARD_FPGA_PORT_OUT_MAX] = {
    BoardFPGA_PortOutWriteValidLevelA,
    BoardFPGA_PortOutWriteValidLevelB,
    BoardFPGA_PortOutWriteValidLevelC,
    BoardFPGA_PortOutWriteValidLevelD,
};

//设置输出状态
static const BoardFPGA_PortOutWriteSetValFunc portOutWriteSetValFuncArray[BOARD_FPGA_PORT_OUT_MAX] = {
    BoardFPGA_PortOutWriteSetValA,
    BoardFPGA_PortOutWriteSetValB,
    BoardFPGA_PortOutWriteSetValC,
    BoardFPGA_PortOutWriteSetValD,
};

//读取设置的输出状态
static const BoardFPGA_PortOutReadSetValFunc portOutReadSetValFuncArray[BOARD_FPGA_PORT_OUT_MAX] = {
    BoardFPGA_PortOutReadSetValA,
    BoardFPGA_PortOutReadSetValB,
    BoardFPGA_PortOutReadSetValC,
    BoardFPGA_PortOutReadSetValD,
};

//读取实际输出状态
static const BoardFPGA_PortOutReadReallyStateFunc portOutReadReallyStateFuncArray[BOARD_FPGA_PORT_OUT_MAX] = {
    BoardFPGA_PortOutReadReallyStateA,
    BoardFPGA_PortOutReadReallyStateB,
    BoardFPGA_PortOutReadReallyStateC,
    BoardFPGA_PortOutReadReallyStateD,
};

/**************************************对外开放API******************************************/
//单次设置输出端口的全部引脚的有效电平
ERROR_SUB BoardFPGA_PortOutWriteValidLevelAll(FPGA_OUT_PORT port,FPGA_PORT_VALID_LEVEL portValidLevelSet)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    errorSub = portOutWriteValidLevelFuncArray[port](portValidLevelSet);
    return errorSub;
}

//如果设置为低电平有效,那么当端口低电平时,读取到的状态为1
//写入输出端口单个引脚有效电平,如果是高电平有效,那么当端口高电品时读取到的时1
ERROR_SUB BoardFPGA_PortOutWriteValidLevelSingle(FPGA_OUT_PORT port,FPGA_OUT_PIN pin,GPIO_PinState pinValidLevelSet)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //关闭全局中断
    BOARD_FPGA_LOCK();
    //首先读取
    FPGA_PORT_VALID_LEVEL validLevels = portOutReadValidLevelFuncArray[port]();
    //对当前数据进行处理
    if(pinValidLevelSet == GPIO_PIN_RESET)
    {
        //低电平有效,清零
        validLevels.valueAll &= (~pin);
    }
    else
    {
        validLevels.valueAll |= pin;
    }  
    //写入
    errorSub = portOutWriteValidLevelFuncArray[port](validLevels);
    //打开全局中断
    BOARD_FPGA_UNLOCK(lastIntState);
    return errorSub;
}

//读取输出端口的有效电平
FPGA_PORT_VALID_LEVEL BoardFPGA_PortOutReadValidLevelAll(FPGA_OUT_PORT port)
{
    FPGA_PORT_VALID_LEVEL validLevelLocal;
    validLevelLocal = portOutReadValidLevelFuncArray[port]();
    return validLevelLocal;
}

//读取指定端口的指定引脚的有效电平
GPIO_PinState BoardFPGA_PortOutReadValidLevelSingle(FPGA_OUT_PORT port,FPGA_OUT_PIN pin)
{
    //首先读取
    FPGA_PORT_VALID_LEVEL validLevels = portOutReadValidLevelFuncArray[port]();
    //提取有效数据
    if((validLevels.valueAll&pin) != 0)
    {
        return GPIO_PIN_SET;
    }
    return GPIO_PIN_RESET;
}

//单次设置输出端口的输出状态
ERROR_SUB BoardFPGA_PortOutWriteSetValAll(FPGA_OUT_PORT port,FPGA_PORT_OUT_SET portVal)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    errorSub = portOutWriteSetValFuncArray[port](portVal);
    return errorSub;
}

//写入指定单个端口的输出状态
ERROR_SUB BoardFPGA_PortOutWriteSetValSingle(FPGA_OUT_PORT port,FPGA_OUT_PIN pin,PIN_STATE pinState)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //关闭全局中断
    BOARD_FPGA_LOCK();
    //首先读取
    FPGA_PORT_OUT_SET outSets = portOutReadSetValFuncArray[port]();
    //对当前数据进行处理
    if(pinState == PIN_STATE_INVALID)
    {
        //低电平有效,清零
        outSets.valAll &= (~pin);
    }
    else
    {
        outSets.valAll |= pin;
    }  
    //写入
    errorSub = portOutWriteSetValFuncArray[port](outSets);
    //打开全局中断
    BOARD_FPGA_UNLOCK(lastIntState);
    return errorSub;
}

//读取输出端口设置的输出状态
FPGA_PORT_OUT_SET BoardFPGA_PortOutReadSetValAll(FPGA_OUT_PORT port)
{
    FPGA_PORT_OUT_SET outSets;
    outSets = portOutReadSetValFuncArray[port]();
    return outSets;
}

//读取指定端口设置的输出状态
PIN_STATE BoardFPGA_PortOutReadSetValSingle(FPGA_OUT_PORT port,FPGA_OUT_PIN pin)
{
    //首先读取
    FPGA_PORT_OUT_SET outSets = portOutReadSetValFuncArray[port]();
    //提取有效数据
    if((outSets.valAll&pin) != 0)
    {
        return PIN_STATE_VALID;
    }
    return PIN_STATE_INVALID;
}

//读取指定端口实际输出电平
FPGA_PORT_OUT_REALLY BoardFPGA_PortOutReadReallyStateAll(FPGA_OUT_PORT port)
{
    FPGA_PORT_OUT_REALLY reallyOutSets;
    reallyOutSets = portOutReadReallyStateFuncArray[port]();
    return reallyOutSets;
}

//读取指定端口单个状态
GPIO_PinState BoardFPGA_PortOutReadReallyStateSingle(FPGA_OUT_PORT port,FPGA_OUT_PIN pin)
{
    FPGA_PORT_OUT_REALLY reallyOutSets = portOutReadReallyStateFuncArray[port]();
    //提取有效数据
    if((reallyOutSets.valAll&pin) != 0)
    {
        return GPIO_PIN_SET;
    }
    return GPIO_PIN_RESET;
}


//输出端口测试
ERROR_SUB  BoardFPGA_PortOutTestA(void)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    GPIO_PinState pinReallyState1,pinReallyState2,pinReallyState3;
    uint8_t indexUtil = 0;
    uint32_t pinIndex;
    for(indexUtil = 0 ;indexUtil < BOARD_FPGA_OUT_PORT_PIN_MAX;indexUtil++)
    {
        pinIndex = (0X00000001<<indexUtil);
        //读取端口配置有效电平
        GPIO_PinState validLevel = BoardFPGA_PortOutReadValidLevelSingle(FPGA_OUT_PORT_A,(FPGA_OUT_PIN)pinIndex);
        //读取实际输出值
        pinReallyState1 = BoardFPGA_PortOutReadReallyStateSingle(FPGA_OUT_PORT_A,(FPGA_OUT_PIN)pinIndex);
        //修改有效电平
        errorSub = BoardFPGA_PortOutWriteValidLevelSingle(FPGA_OUT_PORT_A,(FPGA_OUT_PIN)pinIndex,(GPIO_PinState)(!validLevel));
        if(errorSub != ERROR_SUB_OK)
        {
            return errorSub;
        }
        //再次端口值
        pinReallyState2 = BoardFPGA_PortOutReadReallyStateSingle(FPGA_OUT_PORT_A,(FPGA_OUT_PIN)pinIndex);
        //两次端口值必须不一样
        if(pinReallyState2 == pinReallyState1)
        {
            return ERROR_SUB_BOARD_FPGA_FUNC_TEST_FAIL;
        }
        //写回去原来的值
        errorSub = BoardFPGA_PortOutWriteValidLevelSingle(FPGA_OUT_PORT_A,(FPGA_OUT_PIN)pinIndex,validLevel);
        if(errorSub != ERROR_SUB_OK)
        {
            return errorSub;
        }
        //再次读取端口值
        pinReallyState3 = BoardFPGA_PortOutReadReallyStateSingle(FPGA_OUT_PORT_A,(FPGA_OUT_PIN)pinIndex);
        //两次端口值得必须一样
        if(pinReallyState3 != pinReallyState1)
        {
            return ERROR_SUB_BOARD_FPGA_FUNC_TEST_FAIL;
        }
        //获取当前输出值
        PIN_STATE pinOutState1 = BoardFPGA_PortOutReadSetValSingle(FPGA_OUT_PORT_A,(FPGA_OUT_PIN)pinIndex);
        //获取当前实际值
        pinReallyState1 = BoardFPGA_PortOutReadReallyStateSingle(FPGA_OUT_PORT_A,(FPGA_OUT_PIN)pinIndex);
        //设置新的输出值
        errorSub = BoardFPGA_PortOutWriteSetValSingle(FPGA_OUT_PORT_A,(FPGA_OUT_PIN)pinIndex,(PIN_STATE)(!pinOutState1));
        if(errorSub != ERROR_SUB_OK)
        {
            return errorSub;
        }
        //获取新的输出值
        PIN_STATE pinOutState2 = BoardFPGA_PortOutReadSetValSingle(FPGA_OUT_PORT_A,(FPGA_OUT_PIN)pinIndex);
        //获取当前实际值
        pinReallyState2 = BoardFPGA_PortOutReadReallyStateSingle(FPGA_OUT_PORT_A,(FPGA_OUT_PIN)pinIndex);
        if(pinOutState1 == pinOutState2)
        {
            return ERROR_SUB_BOARD_FPGA_FUNC_TEST_FAIL;
        }
        if(pinReallyState1 == pinReallyState2)
        {
            return ERROR_SUB_BOARD_FPGA_FUNC_TEST_FAIL;
        }
        //写回去原来的输出值
        errorSub = BoardFPGA_PortOutWriteSetValSingle(FPGA_OUT_PORT_A,(FPGA_OUT_PIN)pinIndex,pinOutState1);
        if(errorSub != ERROR_SUB_OK)
        {
            return errorSub;
        }
        pinOutState2 = BoardFPGA_PortOutReadSetValSingle(FPGA_OUT_PORT_A,(FPGA_OUT_PIN)pinIndex);
        if(pinOutState1 != pinOutState2)
        {
            return ERROR_SUB_BOARD_FPGA_FUNC_TEST_FAIL;
        }
        //获取当前实际值
        pinReallyState2 = BoardFPGA_PortOutReadReallyStateSingle(FPGA_OUT_PORT_A,(FPGA_OUT_PIN)pinIndex);
        if(pinReallyState1 != pinReallyState2)
        {
            return ERROR_SUB_BOARD_FPGA_FUNC_TEST_FAIL;
        }
    }
    return ERROR_SUB_OK;
}








