/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-04-26 10:16:43 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-05-06 14:12:10 +0800
************************************************************************************************/ 
#include "BoardFPGA_PortIn.h"

/**************************************PORTA函数******************************************/
//读取输入端口的引脚的配置有效电平
static FPGA_PORT_VALID_LEVEL BoardFPGA_PortInReadValidLevelA(void)
{
    FPGA_PORT_VALID_LEVEL portInValidLevelAll;
    portInValidLevelAll.valueAll = BoardFPGA_ReadReg32(FPGA_REG_READ_ADDR_PORT_IN_GROUP_A_MODE);
    return portInValidLevelAll;
}

//单次设置输入端口的全部引脚的配置有效电平
static ERROR_SUB BoardFPGA_PortInWriteValidLevelA(FPGA_PORT_VALID_LEVEL portValidLevelSet)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    errorSub = BoardFPGA_WriteDataByAddrWithCheck(FPGA_REG_WRITE_ADDR_PORT_IN_GROUP_A_MODE,portValidLevelSet.valueAll,
                                                            FPGA_REG_READ_ADDR_PORT_IN_GROUP_A_MODE);
    return errorSub;
}

//读取指定端口当前全部状态
static FPGA_PORT_IN_STATE BoardFPGA_PortInReadStateA(void)
{
    FPGA_PORT_IN_STATE portInState;
    portInState.stateAll = BoardFPGA_ReadReg32(FPGA_REG_READ_ADDR_PORT_IN_GROUP_A_STATE);
    return portInState;
}

/**************************************PORTB函数******************************************/
//读取输入端口的引脚的配置有效电平
static FPGA_PORT_VALID_LEVEL BoardFPGA_PortInReadValidLevelB(void)
{
    FPGA_PORT_VALID_LEVEL portInValidLevelAll;
    portInValidLevelAll.valueAll = BoardFPGA_ReadReg32(FPGA_REG_READ_ADDR_PORT_IN_GROUP_B_MODE);
    return portInValidLevelAll;
}

//单次设置输入端口的全部引脚的配置有效电平
static ERROR_SUB BoardFPGA_PortInWriteValidLevelB(FPGA_PORT_VALID_LEVEL portValidLevelSet)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    errorSub = BoardFPGA_WriteDataByAddrWithCheck(FPGA_REG_WRITE_ADDR_PORT_IN_GROUP_B_MODE,portValidLevelSet.valueAll,
                                                            FPGA_REG_READ_ADDR_PORT_IN_GROUP_B_MODE);
    return errorSub;
}

//读取指定端口当前全部状态
static FPGA_PORT_IN_STATE BoardFPGA_PortInReadStateB(void)
{
    FPGA_PORT_IN_STATE portInState;
    portInState.stateAll = BoardFPGA_ReadReg32(FPGA_REG_READ_ADDR_PORT_IN_GROUP_B_STATE);
    return portInState;
}

/**************************************PORTC函数******************************************/
//读取输入端口的引脚的配置有效电平
static FPGA_PORT_VALID_LEVEL BoardFPGA_PortInReadValidLevelC(void)
{
    FPGA_PORT_VALID_LEVEL portInValidLevelAll;
    portInValidLevelAll.valueAll = BoardFPGA_ReadReg32(FPGA_REG_READ_ADDR_PORT_IN_GROUP_C_MODE);
    return portInValidLevelAll;
}

//单次设置输入端口的全部引脚的配置有效电平
static ERROR_SUB BoardFPGA_PortInWriteValidLevelC(FPGA_PORT_VALID_LEVEL portValidLevelSet)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    errorSub = BoardFPGA_WriteDataByAddrWithCheck(FPGA_REG_WRITE_ADDR_PORT_IN_GROUP_C_MODE,portValidLevelSet.valueAll,
                                                            FPGA_REG_READ_ADDR_PORT_IN_GROUP_C_MODE);
    return errorSub;
}

//读取指定端口当前全部状态
static FPGA_PORT_IN_STATE BoardFPGA_PortInReadStateC(void)
{
    FPGA_PORT_IN_STATE portInState;
    portInState.stateAll = BoardFPGA_ReadReg32(FPGA_REG_READ_ADDR_PORT_IN_GROUP_C_STATE);
    return portInState;
}

/**************************************PORTD函数******************************************/
//读取输入端口的引脚的配置有效电平
static FPGA_PORT_VALID_LEVEL BoardFPGA_PortInReadValidLevelD(void)
{
    FPGA_PORT_VALID_LEVEL portInValidLevelAll;
    portInValidLevelAll.valueAll = BoardFPGA_ReadReg32(FPGA_REG_READ_ADDR_PORT_IN_GROUP_D_MODE);
    return portInValidLevelAll;
}

//单次设置输入端口的全部引脚的配置有效电平
static ERROR_SUB BoardFPGA_PortInWriteValidLevelD(FPGA_PORT_VALID_LEVEL portValidLevelSet)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    errorSub = BoardFPGA_WriteDataByAddrWithCheck(FPGA_REG_WRITE_ADDR_PORT_IN_GROUP_D_MODE,portValidLevelSet.valueAll,
                                                            FPGA_REG_READ_ADDR_PORT_IN_GROUP_D_MODE);
    return errorSub;
}

//读取指定端口当前全部状态
static FPGA_PORT_IN_STATE BoardFPGA_PortInReadStateD(void)
{
    FPGA_PORT_IN_STATE portInState;
    portInState.stateAll = BoardFPGA_ReadReg32(FPGA_REG_READ_ADDR_PORT_IN_GROUP_D_STATE);
    return portInState;
}

/**************************************函数数组******************************************/

//读取输入端口的引脚的配置有效电平
static const BoardFPGA_PortInReadValidLevelFunc portInReadValidLevelFuncArray[BOARD_FPGA_PORT_IN_MAX] = {
    BoardFPGA_PortInReadValidLevelA,
    BoardFPGA_PortInReadValidLevelB,
    BoardFPGA_PortInReadValidLevelC,
    BoardFPGA_PortInReadValidLevelD,
};

//单次设置输入端口的全部引脚的配置有效电平
static const BoardFPGA_PortInWriteValidLevelFunc portInWriteValidLevelFuncArray[BOARD_FPGA_PORT_IN_MAX] = {
    BoardFPGA_PortInWriteValidLevelA,
    BoardFPGA_PortInWriteValidLevelB,
    BoardFPGA_PortInWriteValidLevelC,
    BoardFPGA_PortInWriteValidLevelD,
};

//读取指定端口当前全部状态
static const BoardFPGA_PortInReadStateFunc portInReadStateFuncArray[BOARD_FPGA_PORT_IN_MAX] = {
    BoardFPGA_PortInReadStateA,
    BoardFPGA_PortInReadStateB,
    BoardFPGA_PortInReadStateC,
    BoardFPGA_PortInReadStateD,
};


/**************************************对外开放API******************************************/
//读取全部有效电平配置
FPGA_PORT_VALID_LEVEL BoardFPGA_PortInReadValidLevelAll(FPGA_IN_PORT port)
{
    return portInReadValidLevelFuncArray[port]();
}

//读取单个有效电平配置
GPIO_PinState BoardFPGA_PortInReadValidLevelSingle(FPGA_IN_PORT port,FPGA_IN_PIN pin)
{
    FPGA_PORT_VALID_LEVEL validLevel;
    validLevel = portInReadValidLevelFuncArray[port]();
    //提取有效数据
    if((validLevel.valueAll&pin) != 0)
    {
        return GPIO_PIN_SET;
    }
    return GPIO_PIN_RESET;
}

//写入全部有效电平配置
ERROR_SUB BoardFPGA_PortInWriteValidLevelAll(FPGA_IN_PORT port,FPGA_PORT_VALID_LEVEL validLevelSet)
{
    return portInWriteValidLevelFuncArray[port](validLevelSet);
}

//写入单个有效电平配置
ERROR_SUB BoardFPGA_PortInWriteValidLevelSingle(FPGA_IN_PORT port,FPGA_IN_PIN pin,GPIO_PinState validLevel)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    FPGA_PORT_VALID_LEVEL validLevelSet;
    //关闭全局中断
    BOARD_FPGA_LOCK();
    validLevelSet = portInReadValidLevelFuncArray[port]();
    //对当前数据进行处理
    if(validLevel == GPIO_PIN_RESET)
    {
        //低电平有效,清零
        validLevelSet.valueAll &= (~pin);
    }
    else
    {
        validLevelSet.valueAll |= pin;
    }    
    errorSub = portInWriteValidLevelFuncArray[port](validLevelSet);
    //打开全局中断
    BOARD_FPGA_UNLOCK(lastIntState);
    return errorSub;
}

//读取全部端口状态
FPGA_PORT_IN_STATE BoardFPGA_PortInReadStateAll(FPGA_IN_PORT port)
{
    return portInReadStateFuncArray[port]();
}

//读取单个端口状态
PIN_STATE BoardFPGA_PortInReadStateSingle(FPGA_IN_PORT port,FPGA_IN_PIN pin)
{
    FPGA_PORT_IN_STATE portInState;
    portInState = portInReadStateFuncArray[port]();
    if((portInState.stateAll&pin) != 0)
    {
        return PIN_STATE_VALID; 
    }
    return PIN_STATE_INVALID;
}

//测试端口输入
ERROR_SUB BoardFPGA_PortInTestA(void)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    uint8_t indexUtil = 0;
    uint32_t pinIndex;
    for(indexUtil = 0 ;indexUtil < BOARD_FPGA_IN_PORT_PIN_MAX;indexUtil++)
    {
        pinIndex = (0X00000001<<indexUtil);
        //读取端口配置有效电平
        GPIO_PinState validLevel = BoardFPGA_PortInReadValidLevelSingle(FPGA_IN_PORT_A,(FPGA_IN_PIN)pinIndex);
        //读取端口值
        PIN_STATE pinState1 = BoardFPGA_PortInReadStateSingle(FPGA_IN_PORT_A,(FPGA_IN_PIN)pinIndex);
        //修改有效电平
        errorSub = BoardFPGA_PortInWriteValidLevelSingle(FPGA_IN_PORT_A,(FPGA_IN_PIN)pinIndex,(GPIO_PinState)(!validLevel));
        if(errorSub != ERROR_SUB_OK)
        {
            return errorSub;
        }
        //再次端口值
        PIN_STATE pinState2 = BoardFPGA_PortInReadStateSingle(FPGA_IN_PORT_A,(FPGA_IN_PIN)pinIndex);
        if(pinState2 == pinState1)
        {
            return ERROR_SUB_BOARD_FPGA_FUNC_TEST_FAIL;
        }
        //写回去原来的值
        errorSub = BoardFPGA_PortInWriteValidLevelSingle(FPGA_IN_PORT_A,(FPGA_IN_PIN)pinIndex,validLevel);
        if(errorSub != ERROR_SUB_OK)
        {
            return errorSub;
        }
        //再次读取端口值
        PIN_STATE pinState3 = BoardFPGA_PortInReadStateSingle(FPGA_IN_PORT_A,(FPGA_IN_PIN)pinIndex);
        if(pinState3 != pinState1)
        {
            return ERROR_SUB_BOARD_FPGA_FUNC_TEST_FAIL;
        }
    }
    return ERROR_SUB_OK;
}
