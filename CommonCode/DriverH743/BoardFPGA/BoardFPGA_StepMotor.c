/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-04-26 10:18:21 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-05-12 16:46:56 +0800
************************************************************************************************/ 
#include "BoardFPGA_StepMotor.h"

/**********************************STEP_MOTOR1基础操作函数*******************************************/
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 0)
//读取配置寄存器 STEP_MOTOR1
static FPGA_STEP_MOTOR_REG_CONFIG BoardFPGA_StepMotor1ReadRegConfig(void)
{
    FPGA_STEP_MOTOR_REG_CONFIG regConfig;
    regConfig.valAll = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR1_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_CONFIG));
    return regConfig;
}

//读取编码器系数寄存器 STEP_MOTOR1
static float BoardFPGA_StepMotor1ReadRegEncoderFactor(void)
{
    FPGA_STEP_MOTOR_REG_PARAM1 regParam1;
    regParam1.valBits.factorUint32 = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR1_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM1));
    return regParam1.factorFloat;
}

//读取定频运转速度寄存器 STEP_MOTOR1
static uint32_t BoardFPGA_StepMotor1ReadRegFixSpdCnt(void)
{
    uint32_t fixSpdCnt;
    fixSpdCnt = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR1_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM2));
    return fixSpdCnt;
}

//读取本次运行总步数寄存器 STEP_MOTOR1
static uint32_t BoardFPGA_StepMotor1ReadRegTotalStepSet(void)
{
    uint32_t totalStepSet;
    totalStepSet = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR1_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM3));
    return totalStepSet;
}

//读取设置步数曲线的起始结束位置 STEP_MOTOR1
static FPGA_STEP_MOTOR_REG_PARAM4 BoardFPGA_StepMotor1ReadRegTabSpdIndexRange(void)
{
    FPGA_STEP_MOTOR_REG_PARAM4 regParam4;
    regParam4.valAll = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR1_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM4));
    return regParam4;
}

//读取配置的停止条件寄存器 STEP_MOTOR1
static FPGA_STEP_MOTOR_REG_PARAM5 BoardFPGA_StepMotor1ReadRegStopCondition(void)
{
    FPGA_STEP_MOTOR_REG_PARAM5 regParam5;
    regParam5.valAll = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR1_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM5));
    return regParam5;
}

//读取当前控制寄存器 STEP_MOTOR1
static FPGA_STEP_MOTOR_REG_CONTROL BoardFPGA_StepMotor1ReadRegControl(void)
{
    FPGA_STEP_MOTOR_REG_CONTROL regControl;
    regControl.valAll = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR1_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_CONTROL));
    return regControl;
}

//读取已经发出的脉冲寄存器 STEP_MOTOR1
static uint32_t BoardFPGA_StepMotor1ReadRegTotalStepSendOut(void)
{
    uint32_t totalStepSendOut;
    totalStepSendOut = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR1_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_STATE1));
    return totalStepSendOut;
}

//读取当前编码器位置寄存器 STEP_MOTOR1
static int32_t BoardFPGA_StepMotor1ReadRegEncoderPos(void)
{
    FPGA_STEP_MOTOR_REG_STATE2 encoderPos;
    encoderPos.encoderPosUint32 = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR1_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_STATE2));
    return encoderPos.valBits.encoderPosInt32;
}

//读取当前信号状态和电机步数表索引序号寄存器 STEP_MOTOR1
static FPGA_STEP_MOTOR_REG_STATE3 BoardFPGA_StepMotor1ReadRegSensorStateTabIndex(void)
{
    FPGA_STEP_MOTOR_REG_STATE3 regState3;
    regState3.valAll = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR1_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_STATE3));
    return regState3;
}

//读取电机状态寄存器 STEP_MOTOR1
static FPGA_STEP_MOTOR_REG_STATE4 BoardFPGA_StepMotor1ReadRegMotorState(void)
{
    FPGA_STEP_MOTOR_REG_STATE4 regState4;
    regState4.valAll = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR1_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_STATE4));
    return regState4;
}

//读取当前速度已经发送的脉冲寄存器 STEP_MOTOR1
static uint32_t BoardFPGA_StepMotor1ReadRegCurTabIdxPluseSendOut(void)
{
    uint32_t curTabIdxPluseSendOut;
    curTabIdxPluseSendOut = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR1_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_STATE5));
    return curTabIdxPluseSendOut;
}

//读取速度表元素 STEP_MOTOR1
static ERROR_SUB BoardFPGA_StepMotor1ReadSpdTabElemByIdx(uint16_t index,uint32_t* elemValPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    *elemValPtr = 0;
    //关闭全局中断
    BOARD_FPGA_LOCK();
    //写入序号
    errorSub = BoardFPGA_WriteDataByAddr((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR1_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_SPD_TAB_ELEM_READ),(uint32_t)index);
    if(errorSub != ERROR_SUB_OK)
    {
        //打开全局中断
        BOARD_FPGA_UNLOCK(lastIntState);
        return errorSub;
    }
    //读取元素
    *elemValPtr = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR1_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_SPD_TAB_ELEM));
    //打开全局中断
    BOARD_FPGA_UNLOCK(lastIntState);
    return errorSub;
}

//读取步数表元素 STEP_MOTOR1
static ERROR_SUB BoardFPGA_StepMotor1ReadStpTabElemByIdx(uint16_t index,uint32_t* elemValPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    *elemValPtr = 0;
    //关闭全局中断
    BOARD_FPGA_LOCK();
    //写入序号
    errorSub = BoardFPGA_WriteDataByAddr((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR1_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_STP_TAB_ELEM_READ),(uint32_t)index);
    if(errorSub != ERROR_SUB_OK)
    {
        //打开全局中断
        BOARD_FPGA_UNLOCK(lastIntState);
        return errorSub;
    }
    //读取元素
    *elemValPtr = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR1_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_STP_TAB_ELEM));
    //打开全局中断
    BOARD_FPGA_UNLOCK(lastIntState);
    return errorSub;
}

//写入配置寄存器 STEP_MOTOR1
static ERROR_SUB BoardFPGA_StepMotor1WriteRegConfig(FPGA_STEP_MOTOR_REG_CONFIG regConfig)
{
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR1_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_CONFIG),regConfig.valAll,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR1_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_CONFIG));
}

//写入编码器系数寄存器 STEP_MOTOR1
static ERROR_SUB BoardFPGA_StepMotor1WriteRegEncoderFactor(float factor)
{
    FPGA_STEP_MOTOR_REG_PARAM1 encoderFactor;
    encoderFactor.factorFloat = factor;
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR1_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_PARAM1),encoderFactor.valBits.factorUint32,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR1_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM1));
}

//写入定频速度计数值寄存器 STEP_MOTOR1
static ERROR_SUB BoardFPGA_StepMotor1WriteRegFixSpdCnt(uint32_t fixSpdCnt)
{
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR1_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_PARAM2),fixSpdCnt,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR1_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM2));
}

//写入本次运行总步数寄存器 STEP_MOTOR1
static ERROR_SUB BoardFPGA_StepMotor1WriteRegTotalStepSet(uint32_t totalStep)
{
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR1_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_PARAM3),totalStep,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR1_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM3));
}

//设置步数曲线的起始结束位置 STEP_MOTOR1
static ERROR_SUB BoardFPGA_StepMotor1WriteRegTabSpdIndexRange(FPGA_STEP_MOTOR_REG_PARAM4 tabRange)
{
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR1_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_PARAM4),tabRange.valAll,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR1_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM4));
}

//写入停止条件 STEP_MOTOR1
static ERROR_SUB BoardFPGA_StepMotor1WriteRegStopCondition(FPGA_STEP_MOTOR_REG_PARAM5 stopCondition)
{
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR1_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_PARAM5),stopCondition.valAll,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR1_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM5));
}

//写入控制字 STEP_MOTOR1
static ERROR_SUB BoardFPGA_StepMotor1WriteRegControl(FPGA_STEP_MOTOR_REG_CONTROL regControl)
{
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR1_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_CONTROL),regControl.valAll,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR1_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_CONTROL));
}

//写入速度表 STEP_MOTOR1
static ERROR_SUB BoardFPGA_StepMotor1WriteRegSpdTabElem(uint16_t index,uint32_t val)
{
    return BoardFPGA_WriteDataByAddr((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR1_BASE_ADDR + FPGA_REG_WRITE_ADDR_STEP_MOTOR_SPD_TAB_ELEM_WRITE + index),val);
}

//写入步数表 STEP_MOTOR1
static ERROR_SUB BoardFPGA_StepMotor1WriteRegStpTabElem(uint16_t index,uint32_t val)
{
    return BoardFPGA_WriteDataByAddr((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR1_BASE_ADDR + FPGA_REG_WRITE_ADDR_STEP_MOTOR_STP_TAB_ELEM_WRITE + index),val);
}
#endif
/**********************************STEP_MOTOR2基础操作函数*******************************************/
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 1)
//读取配置寄存器 STEP_MOTOR2
static FPGA_STEP_MOTOR_REG_CONFIG BoardFPGA_StepMotor2ReadRegConfig(void)
{
    FPGA_STEP_MOTOR_REG_CONFIG regConfig;
    regConfig.valAll = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR2_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_CONFIG));
    return regConfig;
}

//读取编码器系数寄存器 STEP_MOTOR2
static float BoardFPGA_StepMotor2ReadRegEncoderFactor(void)
{
    FPGA_STEP_MOTOR_REG_PARAM1 regParam1;
    regParam1.valBits.factorUint32 = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR2_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM1));
    return regParam1.factorFloat;
}

//读取定频运转速度寄存器 STEP_MOTOR2
static uint32_t BoardFPGA_StepMotor2ReadRegFixSpdCnt(void)
{
    uint32_t fixSpdCnt;
    fixSpdCnt = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR2_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM2));
    return fixSpdCnt;
}

//读取本次运行总步数寄存器 STEP_MOTOR2
static uint32_t BoardFPGA_StepMotor2ReadRegTotalStepSet(void)
{
    uint32_t totalStepSet;
    totalStepSet = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR2_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM3));
    return totalStepSet;
}

//读取设置步数曲线的起始结束位置 STEP_MOTOR2
static FPGA_STEP_MOTOR_REG_PARAM4 BoardFPGA_StepMotor2ReadRegTabSpdIndexRange(void)
{
    FPGA_STEP_MOTOR_REG_PARAM4 regParam4;
    regParam4.valAll = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR2_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM4));
    return regParam4;
}

//读取配置的停止条件寄存器 STEP_MOTOR2
static FPGA_STEP_MOTOR_REG_PARAM5 BoardFPGA_StepMotor2ReadRegStopCondition(void)
{
    FPGA_STEP_MOTOR_REG_PARAM5 regParam5;
    regParam5.valAll = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR2_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM5));
    return regParam5;
}

//读取当前控制寄存器 STEP_MOTOR2
static FPGA_STEP_MOTOR_REG_CONTROL BoardFPGA_StepMotor2ReadRegControl(void)
{
    FPGA_STEP_MOTOR_REG_CONTROL regControl;
    regControl.valAll = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR2_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_CONTROL));
    return regControl;
}

//读取已经发出的脉冲寄存器 STEP_MOTOR2
static uint32_t BoardFPGA_StepMotor2ReadRegTotalStepSendOut(void)
{
    uint32_t totalStepSendOut;
    totalStepSendOut = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR2_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_STATE1));
    return totalStepSendOut;
}

//读取当前编码器位置寄存器 STEP_MOTOR2
static int32_t BoardFPGA_StepMotor2ReadRegEncoderPos(void)
{
    FPGA_STEP_MOTOR_REG_STATE2 encoderPos;
    encoderPos.encoderPosUint32 = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR2_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_STATE2));
    return encoderPos.valBits.encoderPosInt32;
}

//读取当前信号状态和电机步数表索引序号寄存器 STEP_MOTOR2
static FPGA_STEP_MOTOR_REG_STATE3 BoardFPGA_StepMotor2ReadRegSensorStateTabIndex(void)
{
    FPGA_STEP_MOTOR_REG_STATE3 regState3;
    regState3.valAll = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR2_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_STATE3));
    return regState3;
}

//读取电机状态寄存器 STEP_MOTOR2
static FPGA_STEP_MOTOR_REG_STATE4 BoardFPGA_StepMotor2ReadRegMotorState(void)
{
    FPGA_STEP_MOTOR_REG_STATE4 regState4;
    regState4.valAll = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR2_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_STATE4));
    return regState4;
}

//读取当前速度已经发送的脉冲寄存器 STEP_MOTOR2
static uint32_t BoardFPGA_StepMotor2ReadRegCurTabIdxPluseSendOut(void)
{
    uint32_t curTabIdxPluseSendOut;
    curTabIdxPluseSendOut = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR2_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_STATE5));
    return curTabIdxPluseSendOut;
}

//读取速度表元素 STEP_MOTOR2
static ERROR_SUB BoardFPGA_StepMotor2ReadSpdTabElemByIdx(uint16_t index,uint32_t* elemValPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    *elemValPtr = 0;
    //关闭全局中断
    BOARD_FPGA_LOCK();
    //写入序号
    errorSub = BoardFPGA_WriteDataByAddr((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR2_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_SPD_TAB_ELEM_READ),(uint32_t)index);
    if(errorSub != ERROR_SUB_OK)
    {
        //打开全局中断
        BOARD_FPGA_UNLOCK(lastIntState);
        return errorSub;
    }
    //读取元素
    *elemValPtr = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR2_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_SPD_TAB_ELEM));
    //打开全局中断
    BOARD_FPGA_UNLOCK(lastIntState);
    return errorSub;
}

//读取步数表元素 STEP_MOTOR2
static ERROR_SUB BoardFPGA_StepMotor2ReadStpTabElemByIdx(uint16_t index,uint32_t* elemValPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    *elemValPtr = 0;
    //关闭全局中断
    BOARD_FPGA_LOCK();
    //写入序号
    errorSub = BoardFPGA_WriteDataByAddr((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR2_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_STP_TAB_ELEM_READ),(uint32_t)index);
    if(errorSub != ERROR_SUB_OK)
    {
        //打开全局中断
        BOARD_FPGA_UNLOCK(lastIntState);
        return errorSub;
    }
    //读取元素
    *elemValPtr = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR2_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_STP_TAB_ELEM));
    //打开全局中断
    BOARD_FPGA_UNLOCK(lastIntState);
    return errorSub;
}

//写入配置寄存器 STEP_MOTOR2
static ERROR_SUB BoardFPGA_StepMotor2WriteRegConfig(FPGA_STEP_MOTOR_REG_CONFIG regConfig)
{
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR2_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_CONFIG),regConfig.valAll,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR2_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_CONFIG));
}

//写入编码器系数寄存器 STEP_MOTOR2
static ERROR_SUB BoardFPGA_StepMotor2WriteRegEncoderFactor(float factor)
{
    FPGA_STEP_MOTOR_REG_PARAM1 encoderFactor;
    encoderFactor.factorFloat = factor;
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR2_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_PARAM1),encoderFactor.valBits.factorUint32,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR2_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM1));
}

//写入定频速度计数值寄存器 STEP_MOTOR2
static ERROR_SUB BoardFPGA_StepMotor2WriteRegFixSpdCnt(uint32_t fixSpdCnt)
{
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR2_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_PARAM2),fixSpdCnt,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR2_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM2));
}

//写入本次运行总步数寄存器 STEP_MOTOR2
static ERROR_SUB BoardFPGA_StepMotor2WriteRegTotalStepSet(uint32_t totalStep)
{
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR2_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_PARAM3),totalStep,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR2_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM3));
}

//设置步数曲线的起始结束位置 STEP_MOTOR2
static ERROR_SUB BoardFPGA_StepMotor2WriteRegTabSpdIndexRange(FPGA_STEP_MOTOR_REG_PARAM4 tabRange)
{
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR2_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_PARAM4),tabRange.valAll,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR2_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM4));
}

//写入停止条件 STEP_MOTOR2
static ERROR_SUB BoardFPGA_StepMotor2WriteRegStopCondition(FPGA_STEP_MOTOR_REG_PARAM5 stopCondition)
{
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR2_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_PARAM5),stopCondition.valAll,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR2_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM5));
}

//写入控制字 STEP_MOTOR2
static ERROR_SUB BoardFPGA_StepMotor2WriteRegControl(FPGA_STEP_MOTOR_REG_CONTROL regControl)
{
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR2_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_CONTROL),regControl.valAll,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR2_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_CONTROL));
}

//写入速度表 STEP_MOTOR2
static ERROR_SUB BoardFPGA_StepMotor2WriteRegSpdTabElem(uint16_t index,uint32_t val)
{
    return BoardFPGA_WriteDataByAddr((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR2_BASE_ADDR + FPGA_REG_WRITE_ADDR_STEP_MOTOR_SPD_TAB_ELEM_WRITE + index),val);
}

//写入步数表 STEP_MOTOR2
static ERROR_SUB BoardFPGA_StepMotor2WriteRegStpTabElem(uint16_t index,uint32_t val)
{
    return BoardFPGA_WriteDataByAddr((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR2_BASE_ADDR + FPGA_REG_WRITE_ADDR_STEP_MOTOR_STP_TAB_ELEM_WRITE + index),val);
}
#endif
/**********************************STEP_MOTOR3基础操作函数*******************************************/
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 2)
//读取配置寄存器 STEP_MOTOR3
static FPGA_STEP_MOTOR_REG_CONFIG BoardFPGA_StepMotor3ReadRegConfig(void)
{
    FPGA_STEP_MOTOR_REG_CONFIG regConfig;
    regConfig.valAll = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR3_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_CONFIG));
    return regConfig;
}

//读取编码器系数寄存器 STEP_MOTOR3
static float BoardFPGA_StepMotor3ReadRegEncoderFactor(void)
{
    FPGA_STEP_MOTOR_REG_PARAM1 regParam1;
    regParam1.valBits.factorUint32 = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR3_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM1));
    return regParam1.factorFloat;
}

//读取定频运转速度寄存器 STEP_MOTOR3
static uint32_t BoardFPGA_StepMotor3ReadRegFixSpdCnt(void)
{
    uint32_t fixSpdCnt;
    fixSpdCnt = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR3_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM2));
    return fixSpdCnt;
}

//读取本次运行总步数寄存器 STEP_MOTOR3
static uint32_t BoardFPGA_StepMotor3ReadRegTotalStepSet(void)
{
    uint32_t totalStepSet;
    totalStepSet = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR3_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM3));
    return totalStepSet;
}

//读取设置步数曲线的起始结束位置 STEP_MOTOR3
static FPGA_STEP_MOTOR_REG_PARAM4 BoardFPGA_StepMotor3ReadRegTabSpdIndexRange(void)
{
    FPGA_STEP_MOTOR_REG_PARAM4 regParam4;
    regParam4.valAll = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR3_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM4));
    return regParam4;
}

//读取配置的停止条件寄存器 STEP_MOTOR3
static FPGA_STEP_MOTOR_REG_PARAM5 BoardFPGA_StepMotor3ReadRegStopCondition(void)
{
    FPGA_STEP_MOTOR_REG_PARAM5 regParam5;
    regParam5.valAll = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR3_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM5));
    return regParam5;
}

//读取当前控制寄存器 STEP_MOTOR3
static FPGA_STEP_MOTOR_REG_CONTROL BoardFPGA_StepMotor3ReadRegControl(void)
{
    FPGA_STEP_MOTOR_REG_CONTROL regControl;
    regControl.valAll = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR3_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_CONTROL));
    return regControl;
}

//读取已经发出的脉冲寄存器 STEP_MOTOR3
static uint32_t BoardFPGA_StepMotor3ReadRegTotalStepSendOut(void)
{
    uint32_t totalStepSendOut;
    totalStepSendOut = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR3_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_STATE1));
    return totalStepSendOut;
}

//读取当前编码器位置寄存器 STEP_MOTOR3
static int32_t BoardFPGA_StepMotor3ReadRegEncoderPos(void)
{
    FPGA_STEP_MOTOR_REG_STATE2 encoderPos;
    encoderPos.encoderPosUint32 = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR3_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_STATE2));
    return encoderPos.valBits.encoderPosInt32;
}

//读取当前信号状态和电机步数表索引序号寄存器 STEP_MOTOR3
static FPGA_STEP_MOTOR_REG_STATE3 BoardFPGA_StepMotor3ReadRegSensorStateTabIndex(void)
{
    FPGA_STEP_MOTOR_REG_STATE3 regState3;
    regState3.valAll = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR3_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_STATE3));
    return regState3;
}

//读取电机状态寄存器 STEP_MOTOR3
static FPGA_STEP_MOTOR_REG_STATE4 BoardFPGA_StepMotor3ReadRegMotorState(void)
{
    FPGA_STEP_MOTOR_REG_STATE4 regState4;
    regState4.valAll = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR3_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_STATE4));
    return regState4;
}

//读取当前速度已经发送的脉冲寄存器 STEP_MOTOR3
static uint32_t BoardFPGA_StepMotor3ReadRegCurTabIdxPluseSendOut(void)
{
    uint32_t curTabIdxPluseSendOut;
    curTabIdxPluseSendOut = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR3_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_STATE5));
    return curTabIdxPluseSendOut;
}

//读取速度表元素 STEP_MOTOR3
static ERROR_SUB BoardFPGA_StepMotor3ReadSpdTabElemByIdx(uint16_t index,uint32_t* elemValPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    *elemValPtr = 0;
    //关闭全局中断
    BOARD_FPGA_LOCK();
    //写入序号
    errorSub = BoardFPGA_WriteDataByAddr((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR3_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_SPD_TAB_ELEM_READ),(uint32_t)index);
    if(errorSub != ERROR_SUB_OK)
    {
        //打开全局中断
        BOARD_FPGA_UNLOCK(lastIntState);
        return errorSub;
    }
    //读取元素
    *elemValPtr = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR3_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_SPD_TAB_ELEM));
    //打开全局中断
    BOARD_FPGA_UNLOCK(lastIntState);
    return errorSub;
}

//读取步数表元素 STEP_MOTOR3
static ERROR_SUB BoardFPGA_StepMotor3ReadStpTabElemByIdx(uint16_t index,uint32_t* elemValPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    *elemValPtr = 0;
    //关闭全局中断
    BOARD_FPGA_LOCK();
    //写入序号
    errorSub = BoardFPGA_WriteDataByAddr((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR3_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_STP_TAB_ELEM_READ),(uint32_t)index);
    if(errorSub != ERROR_SUB_OK)
    {
        //打开全局中断
        BOARD_FPGA_UNLOCK(lastIntState);
        return errorSub;
    }
    //读取元素
    *elemValPtr = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR3_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_STP_TAB_ELEM));
    //打开全局中断
    BOARD_FPGA_UNLOCK(lastIntState);
    return errorSub;
}

//写入配置寄存器 STEP_MOTOR3
static ERROR_SUB BoardFPGA_StepMotor3WriteRegConfig(FPGA_STEP_MOTOR_REG_CONFIG regConfig)
{
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR3_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_CONFIG),regConfig.valAll,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR3_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_CONFIG));
}

//写入编码器系数寄存器 STEP_MOTOR3
static ERROR_SUB BoardFPGA_StepMotor3WriteRegEncoderFactor(float factor)
{
    FPGA_STEP_MOTOR_REG_PARAM1 encoderFactor;
    encoderFactor.factorFloat = factor;
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR3_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_PARAM1),encoderFactor.valBits.factorUint32,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR3_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM1));
}

//写入定频速度计数值寄存器 STEP_MOTOR3
static ERROR_SUB BoardFPGA_StepMotor3WriteRegFixSpdCnt(uint32_t fixSpdCnt)
{
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR3_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_PARAM2),fixSpdCnt,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR3_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM2));
}

//写入本次运行总步数寄存器 STEP_MOTOR3
static ERROR_SUB BoardFPGA_StepMotor3WriteRegTotalStepSet(uint32_t totalStep)
{
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR3_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_PARAM3),totalStep,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR3_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM3));
}

//设置步数曲线的起始结束位置 STEP_MOTOR3
static ERROR_SUB BoardFPGA_StepMotor3WriteRegTabSpdIndexRange(FPGA_STEP_MOTOR_REG_PARAM4 tabRange)
{
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR3_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_PARAM4),tabRange.valAll,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR3_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM4));
}

//写入停止条件 STEP_MOTOR3
static ERROR_SUB BoardFPGA_StepMotor3WriteRegStopCondition(FPGA_STEP_MOTOR_REG_PARAM5 stopCondition)
{
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR3_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_PARAM5),stopCondition.valAll,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR3_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM5));
}

//写入控制字 STEP_MOTOR3
static ERROR_SUB BoardFPGA_StepMotor3WriteRegControl(FPGA_STEP_MOTOR_REG_CONTROL regControl)
{
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR3_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_CONTROL),regControl.valAll,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR3_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_CONTROL));
}

//写入速度表 STEP_MOTOR3
static ERROR_SUB BoardFPGA_StepMotor3WriteRegSpdTabElem(uint16_t index,uint32_t val)
{
    return BoardFPGA_WriteDataByAddr((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR3_BASE_ADDR + FPGA_REG_WRITE_ADDR_STEP_MOTOR_SPD_TAB_ELEM_WRITE + index),val);
}

//写入步数表 STEP_MOTOR3
static ERROR_SUB BoardFPGA_StepMotor3WriteRegStpTabElem(uint16_t index,uint32_t val)
{
    return BoardFPGA_WriteDataByAddr((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR3_BASE_ADDR + FPGA_REG_WRITE_ADDR_STEP_MOTOR_STP_TAB_ELEM_WRITE + index),val);
}
#endif
/**********************************STEP_MOTOR4基础操作函数*******************************************/
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 3)
//读取配置寄存器 STEP_MOTOR4
static FPGA_STEP_MOTOR_REG_CONFIG BoardFPGA_StepMotor4ReadRegConfig(void)
{
    FPGA_STEP_MOTOR_REG_CONFIG regConfig;
    regConfig.valAll = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR4_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_CONFIG));
    return regConfig;
}

//读取编码器系数寄存器 STEP_MOTOR4
static float BoardFPGA_StepMotor4ReadRegEncoderFactor(void)
{
    FPGA_STEP_MOTOR_REG_PARAM1 regParam1;
    regParam1.valBits.factorUint32 = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR4_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM1));
    return regParam1.factorFloat;
}

//读取定频运转速度寄存器 STEP_MOTOR4
static uint32_t BoardFPGA_StepMotor4ReadRegFixSpdCnt(void)
{
    uint32_t fixSpdCnt;
    fixSpdCnt = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR4_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM2));
    return fixSpdCnt;
}

//读取本次运行总步数寄存器 STEP_MOTOR4
static uint32_t BoardFPGA_StepMotor4ReadRegTotalStepSet(void)
{
    uint32_t totalStepSet;
    totalStepSet = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR4_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM3));
    return totalStepSet;
}

//读取设置步数曲线的起始结束位置 STEP_MOTOR4
static FPGA_STEP_MOTOR_REG_PARAM4 BoardFPGA_StepMotor4ReadRegTabSpdIndexRange(void)
{
    FPGA_STEP_MOTOR_REG_PARAM4 regParam4;
    regParam4.valAll = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR4_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM4));
    return regParam4;
}

//读取配置的停止条件寄存器 STEP_MOTOR4
static FPGA_STEP_MOTOR_REG_PARAM5 BoardFPGA_StepMotor4ReadRegStopCondition(void)
{
    FPGA_STEP_MOTOR_REG_PARAM5 regParam5;
    regParam5.valAll = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR4_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM5));
    return regParam5;
}

//读取当前控制寄存器 STEP_MOTOR4
static FPGA_STEP_MOTOR_REG_CONTROL BoardFPGA_StepMotor4ReadRegControl(void)
{
    FPGA_STEP_MOTOR_REG_CONTROL regControl;
    regControl.valAll = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR4_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_CONTROL));
    return regControl;
}

//读取已经发出的脉冲寄存器 STEP_MOTOR4
static uint32_t BoardFPGA_StepMotor4ReadRegTotalStepSendOut(void)
{
    uint32_t totalStepSendOut;
    totalStepSendOut = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR4_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_STATE1));
    return totalStepSendOut;
}

//读取当前编码器位置寄存器 STEP_MOTOR4
static int32_t BoardFPGA_StepMotor4ReadRegEncoderPos(void)
{
    FPGA_STEP_MOTOR_REG_STATE2 encoderPos;
    encoderPos.encoderPosUint32 = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR4_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_STATE2));
    return encoderPos.valBits.encoderPosInt32;
}

//读取当前信号状态和电机步数表索引序号寄存器 STEP_MOTOR4
static FPGA_STEP_MOTOR_REG_STATE3 BoardFPGA_StepMotor4ReadRegSensorStateTabIndex(void)
{
    FPGA_STEP_MOTOR_REG_STATE3 regState3;
    regState3.valAll = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR4_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_STATE3));
    return regState3;
}

//读取电机状态寄存器 STEP_MOTOR4
static FPGA_STEP_MOTOR_REG_STATE4 BoardFPGA_StepMotor4ReadRegMotorState(void)
{
    FPGA_STEP_MOTOR_REG_STATE4 regState4;
    regState4.valAll = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR4_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_STATE4));
    return regState4;
}

//读取当前速度已经发送的脉冲寄存器 STEP_MOTOR4
static uint32_t BoardFPGA_StepMotor4ReadRegCurTabIdxPluseSendOut(void)
{
    uint32_t curTabIdxPluseSendOut;
    curTabIdxPluseSendOut = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR4_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_STATE5));
    return curTabIdxPluseSendOut;
}

//读取速度表元素 STEP_MOTOR4
static ERROR_SUB BoardFPGA_StepMotor4ReadSpdTabElemByIdx(uint16_t index,uint32_t* elemValPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    *elemValPtr = 0;
    //关闭全局中断
    BOARD_FPGA_LOCK();
    //写入序号
    errorSub = BoardFPGA_WriteDataByAddr((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR4_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_SPD_TAB_ELEM_READ),(uint32_t)index);
    if(errorSub != ERROR_SUB_OK)
    {
        //打开全局中断
        BOARD_FPGA_UNLOCK(lastIntState);
        return errorSub;
    }
    //读取元素
    *elemValPtr = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR4_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_SPD_TAB_ELEM));
    //打开全局中断
    BOARD_FPGA_UNLOCK(lastIntState);
    return errorSub;
}

//读取步数表元素 STEP_MOTOR4
static ERROR_SUB BoardFPGA_StepMotor4ReadStpTabElemByIdx(uint16_t index,uint32_t* elemValPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    *elemValPtr = 0;
    //关闭全局中断
    BOARD_FPGA_LOCK();
    //写入序号
    errorSub = BoardFPGA_WriteDataByAddr((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR4_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_STP_TAB_ELEM_READ),(uint32_t)index);
    if(errorSub != ERROR_SUB_OK)
    {
        //打开全局中断
        BOARD_FPGA_UNLOCK(lastIntState);
        return errorSub;
    }
    //读取元素
    *elemValPtr = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR4_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_STP_TAB_ELEM));
    //打开全局中断
    BOARD_FPGA_UNLOCK(lastIntState);
    return errorSub;
}

//写入配置寄存器 STEP_MOTOR4
static ERROR_SUB BoardFPGA_StepMotor4WriteRegConfig(FPGA_STEP_MOTOR_REG_CONFIG regConfig)
{
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR4_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_CONFIG),regConfig.valAll,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR4_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_CONFIG));
}

//写入编码器系数寄存器 STEP_MOTOR4
static ERROR_SUB BoardFPGA_StepMotor4WriteRegEncoderFactor(float factor)
{
    FPGA_STEP_MOTOR_REG_PARAM1 encoderFactor;
    encoderFactor.factorFloat = factor;
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR4_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_PARAM1),encoderFactor.valBits.factorUint32,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR4_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM1));
}

//写入定频速度计数值寄存器 STEP_MOTOR4
static ERROR_SUB BoardFPGA_StepMotor4WriteRegFixSpdCnt(uint32_t fixSpdCnt)
{
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR4_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_PARAM2),fixSpdCnt,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR4_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM2));
}

//写入本次运行总步数寄存器 STEP_MOTOR4
static ERROR_SUB BoardFPGA_StepMotor4WriteRegTotalStepSet(uint32_t totalStep)
{
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR4_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_PARAM3),totalStep,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR4_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM3));
}

//设置步数曲线的起始结束位置 STEP_MOTOR4
static ERROR_SUB BoardFPGA_StepMotor4WriteRegTabSpdIndexRange(FPGA_STEP_MOTOR_REG_PARAM4 tabRange)
{
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR4_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_PARAM4),tabRange.valAll,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR4_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM4));
}

//写入停止条件 STEP_MOTOR4
static ERROR_SUB BoardFPGA_StepMotor4WriteRegStopCondition(FPGA_STEP_MOTOR_REG_PARAM5 stopCondition)
{
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR4_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_PARAM5),stopCondition.valAll,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR4_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM5));
}

//写入控制字 STEP_MOTOR4
static ERROR_SUB BoardFPGA_StepMotor4WriteRegControl(FPGA_STEP_MOTOR_REG_CONTROL regControl)
{
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR4_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_CONTROL),regControl.valAll,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR4_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_CONTROL));
}

//写入速度表 STEP_MOTOR4
static ERROR_SUB BoardFPGA_StepMotor4WriteRegSpdTabElem(uint16_t index,uint32_t val)
{
    return BoardFPGA_WriteDataByAddr((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR4_BASE_ADDR + FPGA_REG_WRITE_ADDR_STEP_MOTOR_SPD_TAB_ELEM_WRITE + index),val);
}

//写入步数表 STEP_MOTOR4
static ERROR_SUB BoardFPGA_StepMotor4WriteRegStpTabElem(uint16_t index,uint32_t val)
{
    return BoardFPGA_WriteDataByAddr((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR4_BASE_ADDR + FPGA_REG_WRITE_ADDR_STEP_MOTOR_STP_TAB_ELEM_WRITE + index),val);
}
#endif
/**********************************STEP_MOTOR5基础操作函数*******************************************/
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 4)
//读取配置寄存器 STEP_MOTOR5
static FPGA_STEP_MOTOR_REG_CONFIG BoardFPGA_StepMotor5ReadRegConfig(void)
{
    FPGA_STEP_MOTOR_REG_CONFIG regConfig;
    regConfig.valAll = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR5_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_CONFIG));
    return regConfig;
}

//读取编码器系数寄存器 STEP_MOTOR5
static float BoardFPGA_StepMotor5ReadRegEncoderFactor(void)
{
    FPGA_STEP_MOTOR_REG_PARAM1 regParam1;
    regParam1.valBits.factorUint32 = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR5_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM1));
    return regParam1.factorFloat;
}

//读取定频运转速度寄存器 STEP_MOTOR5
static uint32_t BoardFPGA_StepMotor5ReadRegFixSpdCnt(void)
{
    uint32_t fixSpdCnt;
    fixSpdCnt = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR5_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM2));
    return fixSpdCnt;
}

//读取本次运行总步数寄存器 STEP_MOTOR5
static uint32_t BoardFPGA_StepMotor5ReadRegTotalStepSet(void)
{
    uint32_t totalStepSet;
    totalStepSet = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR5_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM3));
    return totalStepSet;
}

//读取设置步数曲线的起始结束位置 STEP_MOTOR5
static FPGA_STEP_MOTOR_REG_PARAM4 BoardFPGA_StepMotor5ReadRegTabSpdIndexRange(void)
{
    FPGA_STEP_MOTOR_REG_PARAM4 regParam4;
    regParam4.valAll = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR5_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM4));
    return regParam4;
}

//读取配置的停止条件寄存器 STEP_MOTOR5
static FPGA_STEP_MOTOR_REG_PARAM5 BoardFPGA_StepMotor5ReadRegStopCondition(void)
{
    FPGA_STEP_MOTOR_REG_PARAM5 regParam5;
    regParam5.valAll = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR5_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM5));
    return regParam5;
}

//读取当前控制寄存器 STEP_MOTOR5
static FPGA_STEP_MOTOR_REG_CONTROL BoardFPGA_StepMotor5ReadRegControl(void)
{
    FPGA_STEP_MOTOR_REG_CONTROL regControl;
    regControl.valAll = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR5_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_CONTROL));
    return regControl;
}

//读取已经发出的脉冲寄存器 STEP_MOTOR5
static uint32_t BoardFPGA_StepMotor5ReadRegTotalStepSendOut(void)
{
    uint32_t totalStepSendOut;
    totalStepSendOut = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR5_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_STATE1));
    return totalStepSendOut;
}

//读取当前编码器位置寄存器 STEP_MOTOR5
static int32_t BoardFPGA_StepMotor5ReadRegEncoderPos(void)
{
    FPGA_STEP_MOTOR_REG_STATE2 encoderPos;
    encoderPos.encoderPosUint32 = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR5_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_STATE2));
    return encoderPos.valBits.encoderPosInt32;
}

//读取当前信号状态和电机步数表索引序号寄存器 STEP_MOTOR5
static FPGA_STEP_MOTOR_REG_STATE3 BoardFPGA_StepMotor5ReadRegSensorStateTabIndex(void)
{
    FPGA_STEP_MOTOR_REG_STATE3 regState3;
    regState3.valAll = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR5_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_STATE3));
    return regState3;
}

//读取电机状态寄存器 STEP_MOTOR5
static FPGA_STEP_MOTOR_REG_STATE4 BoardFPGA_StepMotor5ReadRegMotorState(void)
{
    FPGA_STEP_MOTOR_REG_STATE4 regState4;
    regState4.valAll = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR5_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_STATE4));
    return regState4;
}

//读取当前速度已经发送的脉冲寄存器 STEP_MOTOR5
static uint32_t BoardFPGA_StepMotor5ReadRegCurTabIdxPluseSendOut(void)
{
    uint32_t curTabIdxPluseSendOut;
    curTabIdxPluseSendOut = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR5_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_STATE5));
    return curTabIdxPluseSendOut;
}

//读取速度表元素 STEP_MOTOR5
static ERROR_SUB BoardFPGA_StepMotor5ReadSpdTabElemByIdx(uint16_t index,uint32_t* elemValPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    *elemValPtr = 0;
    //关闭全局中断
    BOARD_FPGA_LOCK();
    //写入序号
    errorSub = BoardFPGA_WriteDataByAddr((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR5_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_SPD_TAB_ELEM_READ),(uint32_t)index);
    if(errorSub != ERROR_SUB_OK)
    {
        //打开全局中断
        BOARD_FPGA_UNLOCK(lastIntState);
        return errorSub;
    }
    //读取元素
    *elemValPtr = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR5_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_SPD_TAB_ELEM));
    //打开全局中断
    BOARD_FPGA_UNLOCK(lastIntState);
    return errorSub;
}

//读取步数表元素 STEP_MOTOR5
static ERROR_SUB BoardFPGA_StepMotor5ReadStpTabElemByIdx(uint16_t index,uint32_t* elemValPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    *elemValPtr = 0;
    //关闭全局中断
    BOARD_FPGA_LOCK();
    //写入序号
    errorSub = BoardFPGA_WriteDataByAddr((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR5_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_STP_TAB_ELEM_READ),(uint32_t)index);
    if(errorSub != ERROR_SUB_OK)
    {
        //打开全局中断
        BOARD_FPGA_UNLOCK(lastIntState);
        return errorSub;
    }
    //读取元素
    *elemValPtr = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR5_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_STP_TAB_ELEM));
    //打开全局中断
    BOARD_FPGA_UNLOCK(lastIntState);
    return errorSub;
}

//写入配置寄存器 STEP_MOTOR5
static ERROR_SUB BoardFPGA_StepMotor5WriteRegConfig(FPGA_STEP_MOTOR_REG_CONFIG regConfig)
{
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR5_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_CONFIG),regConfig.valAll,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR5_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_CONFIG));
}

//写入编码器系数寄存器 STEP_MOTOR5
static ERROR_SUB BoardFPGA_StepMotor5WriteRegEncoderFactor(float factor)
{
    FPGA_STEP_MOTOR_REG_PARAM1 encoderFactor;
    encoderFactor.factorFloat = factor;
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR5_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_PARAM1),encoderFactor.valBits.factorUint32,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR5_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM1));
}

//写入定频速度计数值寄存器 STEP_MOTOR5
static ERROR_SUB BoardFPGA_StepMotor5WriteRegFixSpdCnt(uint32_t fixSpdCnt)
{
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR5_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_PARAM2),fixSpdCnt,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR5_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM2));
}

//写入本次运行总步数寄存器 STEP_MOTOR5
static ERROR_SUB BoardFPGA_StepMotor5WriteRegTotalStepSet(uint32_t totalStep)
{
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR5_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_PARAM3),totalStep,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR5_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM3));
}

//设置步数曲线的起始结束位置 STEP_MOTOR5
static ERROR_SUB BoardFPGA_StepMotor5WriteRegTabSpdIndexRange(FPGA_STEP_MOTOR_REG_PARAM4 tabRange)
{
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR5_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_PARAM4),tabRange.valAll,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR5_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM4));
}

//写入停止条件 STEP_MOTOR5
static ERROR_SUB BoardFPGA_StepMotor5WriteRegStopCondition(FPGA_STEP_MOTOR_REG_PARAM5 stopCondition)
{
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR5_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_PARAM5),stopCondition.valAll,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR5_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM5));
}

//写入控制字 STEP_MOTOR5
static ERROR_SUB BoardFPGA_StepMotor5WriteRegControl(FPGA_STEP_MOTOR_REG_CONTROL regControl)
{
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR5_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_CONTROL),regControl.valAll,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR5_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_CONTROL));
}

//写入速度表 STEP_MOTOR5
static ERROR_SUB BoardFPGA_StepMotor5WriteRegSpdTabElem(uint16_t index,uint32_t val)
{
    return BoardFPGA_WriteDataByAddr((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR5_BASE_ADDR + FPGA_REG_WRITE_ADDR_STEP_MOTOR_SPD_TAB_ELEM_WRITE + index),val);
}

//写入步数表 STEP_MOTOR5
static ERROR_SUB BoardFPGA_StepMotor5WriteRegStpTabElem(uint16_t index,uint32_t val)
{
    return BoardFPGA_WriteDataByAddr((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR5_BASE_ADDR + FPGA_REG_WRITE_ADDR_STEP_MOTOR_STP_TAB_ELEM_WRITE + index),val);
}
#endif
/**********************************STEP_MOTOR6基础操作函数*******************************************/
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 5)
//读取配置寄存器 STEP_MOTOR6
static FPGA_STEP_MOTOR_REG_CONFIG BoardFPGA_StepMotor6ReadRegConfig(void)
{
    FPGA_STEP_MOTOR_REG_CONFIG regConfig;
    regConfig.valAll = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR6_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_CONFIG));
    return regConfig;
}

//读取编码器系数寄存器 STEP_MOTOR6
static float BoardFPGA_StepMotor6ReadRegEncoderFactor(void)
{
    FPGA_STEP_MOTOR_REG_PARAM1 regParam1;
    regParam1.valBits.factorUint32 = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR6_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM1));
    return regParam1.factorFloat;
}

//读取定频运转速度寄存器 STEP_MOTOR6
static uint32_t BoardFPGA_StepMotor6ReadRegFixSpdCnt(void)
{
    uint32_t fixSpdCnt;
    fixSpdCnt = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR6_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM2));
    return fixSpdCnt;
}

//读取本次运行总步数寄存器 STEP_MOTOR6
static uint32_t BoardFPGA_StepMotor6ReadRegTotalStepSet(void)
{
    uint32_t totalStepSet;
    totalStepSet = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR6_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM3));
    return totalStepSet;
}

//读取设置步数曲线的起始结束位置 STEP_MOTOR6
static FPGA_STEP_MOTOR_REG_PARAM4 BoardFPGA_StepMotor6ReadRegTabSpdIndexRange(void)
{
    FPGA_STEP_MOTOR_REG_PARAM4 regParam4;
    regParam4.valAll = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR6_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM4));
    return regParam4;
}

//读取配置的停止条件寄存器 STEP_MOTOR6
static FPGA_STEP_MOTOR_REG_PARAM5 BoardFPGA_StepMotor6ReadRegStopCondition(void)
{
    FPGA_STEP_MOTOR_REG_PARAM5 regParam5;
    regParam5.valAll = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR6_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM5));
    return regParam5;
}

//读取当前控制寄存器 STEP_MOTOR6
static FPGA_STEP_MOTOR_REG_CONTROL BoardFPGA_StepMotor6ReadRegControl(void)
{
    FPGA_STEP_MOTOR_REG_CONTROL regControl;
    regControl.valAll = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR6_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_CONTROL));
    return regControl;
}

//读取已经发出的脉冲寄存器 STEP_MOTOR6
static uint32_t BoardFPGA_StepMotor6ReadRegTotalStepSendOut(void)
{
    uint32_t totalStepSendOut;
    totalStepSendOut = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR6_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_STATE1));
    return totalStepSendOut;
}

//读取当前编码器位置寄存器 STEP_MOTOR6
static int32_t BoardFPGA_StepMotor6ReadRegEncoderPos(void)
{
    FPGA_STEP_MOTOR_REG_STATE2 encoderPos;
    encoderPos.encoderPosUint32 = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR6_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_STATE2));
    return encoderPos.valBits.encoderPosInt32;
}

//读取当前信号状态和电机步数表索引序号寄存器 STEP_MOTOR6
static FPGA_STEP_MOTOR_REG_STATE3 BoardFPGA_StepMotor6ReadRegSensorStateTabIndex(void)
{
    FPGA_STEP_MOTOR_REG_STATE3 regState3;
    regState3.valAll = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR6_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_STATE3));
    return regState3;
}

//读取电机状态寄存器 STEP_MOTOR6
static FPGA_STEP_MOTOR_REG_STATE4 BoardFPGA_StepMotor6ReadRegMotorState(void)
{
    FPGA_STEP_MOTOR_REG_STATE4 regState4;
    regState4.valAll = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR6_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_STATE4));
    return regState4;
}

//读取当前速度已经发送的脉冲寄存器 STEP_MOTOR6
static uint32_t BoardFPGA_StepMotor6ReadRegCurTabIdxPluseSendOut(void)
{
    uint32_t curTabIdxPluseSendOut;
    curTabIdxPluseSendOut = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR6_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_STATE5));
    return curTabIdxPluseSendOut;
}

//读取速度表元素 STEP_MOTOR6
static ERROR_SUB BoardFPGA_StepMotor6ReadSpdTabElemByIdx(uint16_t index,uint32_t* elemValPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    *elemValPtr = 0;
    //关闭全局中断
    BOARD_FPGA_LOCK();
    //写入序号
    errorSub = BoardFPGA_WriteDataByAddr((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR6_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_SPD_TAB_ELEM_READ),(uint32_t)index);
    if(errorSub != ERROR_SUB_OK)
    {
        //打开全局中断
        BOARD_FPGA_UNLOCK(lastIntState);
        return errorSub;
    }
    //读取元素
    *elemValPtr = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR6_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_SPD_TAB_ELEM));
    //打开全局中断
    BOARD_FPGA_UNLOCK(lastIntState);
    return errorSub;
}

//读取步数表元素 STEP_MOTOR6
static ERROR_SUB BoardFPGA_StepMotor6ReadStpTabElemByIdx(uint16_t index,uint32_t* elemValPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    *elemValPtr = 0;
    //关闭全局中断
    BOARD_FPGA_LOCK();
    //写入序号
    errorSub = BoardFPGA_WriteDataByAddr((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR6_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_STP_TAB_ELEM_READ),(uint32_t)index);
    if(errorSub != ERROR_SUB_OK)
    {
        //打开全局中断
        BOARD_FPGA_UNLOCK(lastIntState);
        return errorSub;
    }
    //读取元素
    *elemValPtr = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR6_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_STP_TAB_ELEM));
    //打开全局中断
    BOARD_FPGA_UNLOCK(lastIntState);
    return errorSub;
}

//写入配置寄存器 STEP_MOTOR6
static ERROR_SUB BoardFPGA_StepMotor6WriteRegConfig(FPGA_STEP_MOTOR_REG_CONFIG regConfig)
{
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR6_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_CONFIG),regConfig.valAll,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR6_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_CONFIG));
}

//写入编码器系数寄存器 STEP_MOTOR6
static ERROR_SUB BoardFPGA_StepMotor6WriteRegEncoderFactor(float factor)
{
    FPGA_STEP_MOTOR_REG_PARAM1 encoderFactor;
    encoderFactor.factorFloat = factor;
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR6_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_PARAM1),encoderFactor.valBits.factorUint32,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR6_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM1));
}

//写入定频速度计数值寄存器 STEP_MOTOR6
static ERROR_SUB BoardFPGA_StepMotor6WriteRegFixSpdCnt(uint32_t fixSpdCnt)
{
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR6_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_PARAM2),fixSpdCnt,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR6_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM2));
}

//写入本次运行总步数寄存器 STEP_MOTOR6
static ERROR_SUB BoardFPGA_StepMotor6WriteRegTotalStepSet(uint32_t totalStep)
{
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR6_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_PARAM3),totalStep,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR6_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM3));
}

//设置步数曲线的起始结束位置 STEP_MOTOR6
static ERROR_SUB BoardFPGA_StepMotor6WriteRegTabSpdIndexRange(FPGA_STEP_MOTOR_REG_PARAM4 tabRange)
{
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR6_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_PARAM4),tabRange.valAll,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR6_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM4));
}

//写入停止条件 STEP_MOTOR6
static ERROR_SUB BoardFPGA_StepMotor6WriteRegStopCondition(FPGA_STEP_MOTOR_REG_PARAM5 stopCondition)
{
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR6_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_PARAM5),stopCondition.valAll,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR6_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM5));
}

//写入控制字 STEP_MOTOR6
static ERROR_SUB BoardFPGA_StepMotor6WriteRegControl(FPGA_STEP_MOTOR_REG_CONTROL regControl)
{
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR6_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_CONTROL),regControl.valAll,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR6_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_CONTROL));
}

//写入速度表 STEP_MOTOR6
static ERROR_SUB BoardFPGA_StepMotor6WriteRegSpdTabElem(uint16_t index,uint32_t val)
{
    return BoardFPGA_WriteDataByAddr((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR6_BASE_ADDR + FPGA_REG_WRITE_ADDR_STEP_MOTOR_SPD_TAB_ELEM_WRITE + index),val);
}

//写入步数表 STEP_MOTOR6
static ERROR_SUB BoardFPGA_StepMotor6WriteRegStpTabElem(uint16_t index,uint32_t val)
{
    return BoardFPGA_WriteDataByAddr((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR6_BASE_ADDR + FPGA_REG_WRITE_ADDR_STEP_MOTOR_STP_TAB_ELEM_WRITE + index),val);
}
#endif
/**********************************STEP_MOTOR7基础操作函数*******************************************/
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 6)
//读取配置寄存器 STEP_MOTOR7
static FPGA_STEP_MOTOR_REG_CONFIG BoardFPGA_StepMotor7ReadRegConfig(void)
{
    FPGA_STEP_MOTOR_REG_CONFIG regConfig;
    regConfig.valAll = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR7_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_CONFIG));
    return regConfig;
}

//读取编码器系数寄存器 STEP_MOTOR7
static float BoardFPGA_StepMotor7ReadRegEncoderFactor(void)
{
    FPGA_STEP_MOTOR_REG_PARAM1 regParam1;
    regParam1.valBits.factorUint32 = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR7_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM1));
    return regParam1.factorFloat;
}

//读取定频运转速度寄存器 STEP_MOTOR7
static uint32_t BoardFPGA_StepMotor7ReadRegFixSpdCnt(void)
{
    uint32_t fixSpdCnt;
    fixSpdCnt = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR7_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM2));
    return fixSpdCnt;
}

//读取本次运行总步数寄存器 STEP_MOTOR7
static uint32_t BoardFPGA_StepMotor7ReadRegTotalStepSet(void)
{
    uint32_t totalStepSet;
    totalStepSet = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR7_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM3));
    return totalStepSet;
}

//读取设置步数曲线的起始结束位置 STEP_MOTOR7
static FPGA_STEP_MOTOR_REG_PARAM4 BoardFPGA_StepMotor7ReadRegTabSpdIndexRange(void)
{
    FPGA_STEP_MOTOR_REG_PARAM4 regParam4;
    regParam4.valAll = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR7_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM4));
    return regParam4;
}

//读取配置的停止条件寄存器 STEP_MOTOR7
static FPGA_STEP_MOTOR_REG_PARAM5 BoardFPGA_StepMotor7ReadRegStopCondition(void)
{
    FPGA_STEP_MOTOR_REG_PARAM5 regParam5;
    regParam5.valAll = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR7_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM5));
    return regParam5;
}

//读取当前控制寄存器 STEP_MOTOR7
static FPGA_STEP_MOTOR_REG_CONTROL BoardFPGA_StepMotor7ReadRegControl(void)
{
    FPGA_STEP_MOTOR_REG_CONTROL regControl;
    regControl.valAll = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR7_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_CONTROL));
    return regControl;
}

//读取已经发出的脉冲寄存器 STEP_MOTOR7
static uint32_t BoardFPGA_StepMotor7ReadRegTotalStepSendOut(void)
{
    uint32_t totalStepSendOut;
    totalStepSendOut = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR7_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_STATE1));
    return totalStepSendOut;
}

//读取当前编码器位置寄存器 STEP_MOTOR7
static int32_t BoardFPGA_StepMotor7ReadRegEncoderPos(void)
{
    FPGA_STEP_MOTOR_REG_STATE2 encoderPos;
    encoderPos.encoderPosUint32 = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR7_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_STATE2));
    return encoderPos.valBits.encoderPosInt32;
}

//读取当前信号状态和电机步数表索引序号寄存器 STEP_MOTOR7
static FPGA_STEP_MOTOR_REG_STATE3 BoardFPGA_StepMotor7ReadRegSensorStateTabIndex(void)
{
    FPGA_STEP_MOTOR_REG_STATE3 regState3;
    regState3.valAll = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR7_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_STATE3));
    return regState3;
}

//读取电机状态寄存器 STEP_MOTOR7
static FPGA_STEP_MOTOR_REG_STATE4 BoardFPGA_StepMotor7ReadRegMotorState(void)
{
    FPGA_STEP_MOTOR_REG_STATE4 regState4;
    regState4.valAll = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR7_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_STATE4));
    return regState4;
}

//读取当前速度已经发送的脉冲寄存器 STEP_MOTOR7
static uint32_t BoardFPGA_StepMotor7ReadRegCurTabIdxPluseSendOut(void)
{
    uint32_t curTabIdxPluseSendOut;
    curTabIdxPluseSendOut = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR7_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_STATE5));
    return curTabIdxPluseSendOut;
}

//读取速度表元素 STEP_MOTOR7
static ERROR_SUB BoardFPGA_StepMotor7ReadSpdTabElemByIdx(uint16_t index,uint32_t* elemValPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    *elemValPtr = 0;
    //关闭全局中断
    BOARD_FPGA_LOCK();
    //写入序号
    errorSub = BoardFPGA_WriteDataByAddr((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR7_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_SPD_TAB_ELEM_READ),(uint32_t)index);
    if(errorSub != ERROR_SUB_OK)
    {
        //打开全局中断
        BOARD_FPGA_UNLOCK(lastIntState);
        return errorSub;
    }
    //读取元素
    *elemValPtr = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR7_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_SPD_TAB_ELEM));
    //打开全局中断
    BOARD_FPGA_UNLOCK(lastIntState);
    return errorSub;
}

//读取步数表元素 STEP_MOTOR7
static ERROR_SUB BoardFPGA_StepMotor7ReadStpTabElemByIdx(uint16_t index,uint32_t* elemValPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    *elemValPtr = 0;
    //关闭全局中断
    BOARD_FPGA_LOCK();
    //写入序号
    errorSub = BoardFPGA_WriteDataByAddr((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR7_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_STP_TAB_ELEM_READ),(uint32_t)index);
    if(errorSub != ERROR_SUB_OK)
    {
        //打开全局中断
        BOARD_FPGA_UNLOCK(lastIntState);
        return errorSub;
    }
    //读取元素
    *elemValPtr = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR7_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_STP_TAB_ELEM));
    //打开全局中断
    BOARD_FPGA_UNLOCK(lastIntState);
    return errorSub;
}

//写入配置寄存器 STEP_MOTOR7
static ERROR_SUB BoardFPGA_StepMotor7WriteRegConfig(FPGA_STEP_MOTOR_REG_CONFIG regConfig)
{
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR7_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_CONFIG),regConfig.valAll,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR7_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_CONFIG));
}

//写入编码器系数寄存器 STEP_MOTOR7
static ERROR_SUB BoardFPGA_StepMotor7WriteRegEncoderFactor(float factor)
{
    FPGA_STEP_MOTOR_REG_PARAM1 encoderFactor;
    encoderFactor.factorFloat = factor;
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR7_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_PARAM1),encoderFactor.valBits.factorUint32,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR7_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM1));
}

//写入定频速度计数值寄存器 STEP_MOTOR7
static ERROR_SUB BoardFPGA_StepMotor7WriteRegFixSpdCnt(uint32_t fixSpdCnt)
{
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR7_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_PARAM2),fixSpdCnt,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR7_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM2));
}

//写入本次运行总步数寄存器 STEP_MOTOR7
static ERROR_SUB BoardFPGA_StepMotor7WriteRegTotalStepSet(uint32_t totalStep)
{
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR7_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_PARAM3),totalStep,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR7_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM3));
}

//设置步数曲线的起始结束位置 STEP_MOTOR7
static ERROR_SUB BoardFPGA_StepMotor7WriteRegTabSpdIndexRange(FPGA_STEP_MOTOR_REG_PARAM4 tabRange)
{
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR7_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_PARAM4),tabRange.valAll,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR7_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM4));
}

//写入停止条件 STEP_MOTOR7
static ERROR_SUB BoardFPGA_StepMotor7WriteRegStopCondition(FPGA_STEP_MOTOR_REG_PARAM5 stopCondition)
{
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR7_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_PARAM5),stopCondition.valAll,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR7_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM5));
}

//写入控制字 STEP_MOTOR7
static ERROR_SUB BoardFPGA_StepMotor7WriteRegControl(FPGA_STEP_MOTOR_REG_CONTROL regControl)
{
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR7_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_CONTROL),regControl.valAll,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR7_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_CONTROL));
}

//写入速度表 STEP_MOTOR7
static ERROR_SUB BoardFPGA_StepMotor7WriteRegSpdTabElem(uint16_t index,uint32_t val)
{
    return BoardFPGA_WriteDataByAddr((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR7_BASE_ADDR + FPGA_REG_WRITE_ADDR_STEP_MOTOR_SPD_TAB_ELEM_WRITE + index),val);
}

//写入步数表 STEP_MOTOR7
static ERROR_SUB BoardFPGA_StepMotor7WriteRegStpTabElem(uint16_t index,uint32_t val)
{
    return BoardFPGA_WriteDataByAddr((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR7_BASE_ADDR + FPGA_REG_WRITE_ADDR_STEP_MOTOR_STP_TAB_ELEM_WRITE + index),val);
}
#endif
/**********************************STEP_MOTOR8基础操作函数*******************************************/
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 7)
//读取配置寄存器 STEP_MOTOR8
static FPGA_STEP_MOTOR_REG_CONFIG BoardFPGA_StepMotor8ReadRegConfig(void)
{
    FPGA_STEP_MOTOR_REG_CONFIG regConfig;
    regConfig.valAll = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR8_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_CONFIG));
    return regConfig;
}

//读取编码器系数寄存器 STEP_MOTOR8
static float BoardFPGA_StepMotor8ReadRegEncoderFactor(void)
{
    FPGA_STEP_MOTOR_REG_PARAM1 regParam1;
    regParam1.valBits.factorUint32 = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR8_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM1));
    return regParam1.factorFloat;
}

//读取定频运转速度寄存器 STEP_MOTOR8
static uint32_t BoardFPGA_StepMotor8ReadRegFixSpdCnt(void)
{
    uint32_t fixSpdCnt;
    fixSpdCnt = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR8_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM2));
    return fixSpdCnt;
}

//读取本次运行总步数寄存器 STEP_MOTOR8
static uint32_t BoardFPGA_StepMotor8ReadRegTotalStepSet(void)
{
    uint32_t totalStepSet;
    totalStepSet = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR8_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM3));
    return totalStepSet;
}

//读取设置步数曲线的起始结束位置 STEP_MOTOR8
static FPGA_STEP_MOTOR_REG_PARAM4 BoardFPGA_StepMotor8ReadRegTabSpdIndexRange(void)
{
    FPGA_STEP_MOTOR_REG_PARAM4 regParam4;
    regParam4.valAll = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR8_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM4));
    return regParam4;
}

//读取配置的停止条件寄存器 STEP_MOTOR8
static FPGA_STEP_MOTOR_REG_PARAM5 BoardFPGA_StepMotor8ReadRegStopCondition(void)
{
    FPGA_STEP_MOTOR_REG_PARAM5 regParam5;
    regParam5.valAll = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR8_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM5));
    return regParam5;
}

//读取当前控制寄存器 STEP_MOTOR8
static FPGA_STEP_MOTOR_REG_CONTROL BoardFPGA_StepMotor8ReadRegControl(void)
{
    FPGA_STEP_MOTOR_REG_CONTROL regControl;
    regControl.valAll = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR8_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_CONTROL));
    return regControl;
}

//读取已经发出的脉冲寄存器 STEP_MOTOR8
static uint32_t BoardFPGA_StepMotor8ReadRegTotalStepSendOut(void)
{
    uint32_t totalStepSendOut;
    totalStepSendOut = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR8_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_STATE1));
    return totalStepSendOut;
}

//读取当前编码器位置寄存器 STEP_MOTOR8
static int32_t BoardFPGA_StepMotor8ReadRegEncoderPos(void)
{
    FPGA_STEP_MOTOR_REG_STATE2 encoderPos;
    encoderPos.encoderPosUint32 = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR8_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_STATE2));
    return encoderPos.valBits.encoderPosInt32;
}

//读取当前信号状态和电机步数表索引序号寄存器 STEP_MOTOR8
static FPGA_STEP_MOTOR_REG_STATE3 BoardFPGA_StepMotor8ReadRegSensorStateTabIndex(void)
{
    FPGA_STEP_MOTOR_REG_STATE3 regState3;
    regState3.valAll = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR8_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_STATE3));
    return regState3;
}

//读取电机状态寄存器 STEP_MOTOR8
static FPGA_STEP_MOTOR_REG_STATE4 BoardFPGA_StepMotor8ReadRegMotorState(void)
{
    FPGA_STEP_MOTOR_REG_STATE4 regState4;
    regState4.valAll = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR8_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_STATE4));
    return regState4;
}

//读取当前速度已经发送的脉冲寄存器 STEP_MOTOR8
static uint32_t BoardFPGA_StepMotor8ReadRegCurTabIdxPluseSendOut(void)
{
    uint32_t curTabIdxPluseSendOut;
    curTabIdxPluseSendOut = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR8_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_STATE5));
    return curTabIdxPluseSendOut;
}

//读取速度表元素 STEP_MOTOR8
static ERROR_SUB BoardFPGA_StepMotor8ReadSpdTabElemByIdx(uint16_t index,uint32_t* elemValPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    *elemValPtr = 0;
    //关闭全局中断
    BOARD_FPGA_LOCK();
    //写入序号
    errorSub = BoardFPGA_WriteDataByAddr((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR8_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_SPD_TAB_ELEM_READ),(uint32_t)index);
    if(errorSub != ERROR_SUB_OK)
    {
        //打开全局中断
        BOARD_FPGA_UNLOCK(lastIntState);
        return errorSub;
    }
    //读取元素
    *elemValPtr = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR8_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_SPD_TAB_ELEM));
    //打开全局中断
    BOARD_FPGA_UNLOCK(lastIntState);
    return errorSub;
}

//读取步数表元素 STEP_MOTOR8
static ERROR_SUB BoardFPGA_StepMotor8ReadStpTabElemByIdx(uint16_t index,uint32_t* elemValPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    *elemValPtr = 0;
    //关闭全局中断
    BOARD_FPGA_LOCK();
    //写入序号
    errorSub = BoardFPGA_WriteDataByAddr((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR8_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_STP_TAB_ELEM_READ),(uint32_t)index);
    if(errorSub != ERROR_SUB_OK)
    {
        //打开全局中断
        BOARD_FPGA_UNLOCK(lastIntState);
        return errorSub;
    }
    //读取元素
    *elemValPtr = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR8_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_STP_TAB_ELEM));
    //打开全局中断
    BOARD_FPGA_UNLOCK(lastIntState);
    return errorSub;
}

//写入配置寄存器 STEP_MOTOR8
static ERROR_SUB BoardFPGA_StepMotor8WriteRegConfig(FPGA_STEP_MOTOR_REG_CONFIG regConfig)
{
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR8_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_CONFIG),regConfig.valAll,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR8_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_CONFIG));
}

//写入编码器系数寄存器 STEP_MOTOR8
static ERROR_SUB BoardFPGA_StepMotor8WriteRegEncoderFactor(float factor)
{
    FPGA_STEP_MOTOR_REG_PARAM1 encoderFactor;
    encoderFactor.factorFloat = factor;
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR8_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_PARAM1),encoderFactor.valBits.factorUint32,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR8_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM1));
}

//写入定频速度计数值寄存器 STEP_MOTOR8
static ERROR_SUB BoardFPGA_StepMotor8WriteRegFixSpdCnt(uint32_t fixSpdCnt)
{
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR8_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_PARAM2),fixSpdCnt,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR8_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM2));
}

//写入本次运行总步数寄存器 STEP_MOTOR8
static ERROR_SUB BoardFPGA_StepMotor8WriteRegTotalStepSet(uint32_t totalStep)
{
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR8_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_PARAM3),totalStep,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR8_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM3));
}

//设置步数曲线的起始结束位置 STEP_MOTOR8
static ERROR_SUB BoardFPGA_StepMotor8WriteRegTabSpdIndexRange(FPGA_STEP_MOTOR_REG_PARAM4 tabRange)
{
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR8_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_PARAM4),tabRange.valAll,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR8_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM4));
}

//写入停止条件 STEP_MOTOR8
static ERROR_SUB BoardFPGA_StepMotor8WriteRegStopCondition(FPGA_STEP_MOTOR_REG_PARAM5 stopCondition)
{
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR8_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_PARAM5),stopCondition.valAll,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR8_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM5));
}

//写入控制字 STEP_MOTOR8
static ERROR_SUB BoardFPGA_StepMotor8WriteRegControl(FPGA_STEP_MOTOR_REG_CONTROL regControl)
{
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR8_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_CONTROL),regControl.valAll,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR8_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_CONTROL));
}

//写入速度表 STEP_MOTOR8
static ERROR_SUB BoardFPGA_StepMotor8WriteRegSpdTabElem(uint16_t index,uint32_t val)
{
    return BoardFPGA_WriteDataByAddr((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR8_BASE_ADDR + FPGA_REG_WRITE_ADDR_STEP_MOTOR_SPD_TAB_ELEM_WRITE + index),val);
}

//写入步数表 STEP_MOTOR8
static ERROR_SUB BoardFPGA_StepMotor8WriteRegStpTabElem(uint16_t index,uint32_t val)
{
    return BoardFPGA_WriteDataByAddr((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR8_BASE_ADDR + FPGA_REG_WRITE_ADDR_STEP_MOTOR_STP_TAB_ELEM_WRITE + index),val);
}
#endif
/**********************************STEP_MOTOR9基础操作函数*******************************************/
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 8)
//读取配置寄存器 STEP_MOTOR9
static FPGA_STEP_MOTOR_REG_CONFIG BoardFPGA_StepMotor9ReadRegConfig(void)
{
    FPGA_STEP_MOTOR_REG_CONFIG regConfig;
    regConfig.valAll = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR9_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_CONFIG));
    return regConfig;
}

//读取编码器系数寄存器 STEP_MOTOR9
static float BoardFPGA_StepMotor9ReadRegEncoderFactor(void)
{
    FPGA_STEP_MOTOR_REG_PARAM1 regParam1;
    regParam1.valBits.factorUint32 = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR9_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM1));
    return regParam1.factorFloat;
}

//读取定频运转速度寄存器 STEP_MOTOR9
static uint32_t BoardFPGA_StepMotor9ReadRegFixSpdCnt(void)
{
    uint32_t fixSpdCnt;
    fixSpdCnt = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR9_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM2));
    return fixSpdCnt;
}

//读取本次运行总步数寄存器 STEP_MOTOR9
static uint32_t BoardFPGA_StepMotor9ReadRegTotalStepSet(void)
{
    uint32_t totalStepSet;
    totalStepSet = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR9_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM3));
    return totalStepSet;
}

//读取设置步数曲线的起始结束位置 STEP_MOTOR9
static FPGA_STEP_MOTOR_REG_PARAM4 BoardFPGA_StepMotor9ReadRegTabSpdIndexRange(void)
{
    FPGA_STEP_MOTOR_REG_PARAM4 regParam4;
    regParam4.valAll = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR9_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM4));
    return regParam4;
}

//读取配置的停止条件寄存器 STEP_MOTOR9
static FPGA_STEP_MOTOR_REG_PARAM5 BoardFPGA_StepMotor9ReadRegStopCondition(void)
{
    FPGA_STEP_MOTOR_REG_PARAM5 regParam5;
    regParam5.valAll = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR9_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM5));
    return regParam5;
}

//读取当前控制寄存器 STEP_MOTOR9
static FPGA_STEP_MOTOR_REG_CONTROL BoardFPGA_StepMotor9ReadRegControl(void)
{
    FPGA_STEP_MOTOR_REG_CONTROL regControl;
    regControl.valAll = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR9_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_CONTROL));
    return regControl;
}

//读取已经发出的脉冲寄存器 STEP_MOTOR9
static uint32_t BoardFPGA_StepMotor9ReadRegTotalStepSendOut(void)
{
    uint32_t totalStepSendOut;
    totalStepSendOut = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR9_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_STATE1));
    return totalStepSendOut;
}

//读取当前编码器位置寄存器 STEP_MOTOR9
static int32_t BoardFPGA_StepMotor9ReadRegEncoderPos(void)
{
    FPGA_STEP_MOTOR_REG_STATE2 encoderPos;
    encoderPos.encoderPosUint32 = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR9_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_STATE2));
    return encoderPos.valBits.encoderPosInt32;
}

//读取当前信号状态和电机步数表索引序号寄存器 STEP_MOTOR9
static FPGA_STEP_MOTOR_REG_STATE3 BoardFPGA_StepMotor9ReadRegSensorStateTabIndex(void)
{
    FPGA_STEP_MOTOR_REG_STATE3 regState3;
    regState3.valAll = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR9_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_STATE3));
    return regState3;
}

//读取电机状态寄存器 STEP_MOTOR9
static FPGA_STEP_MOTOR_REG_STATE4 BoardFPGA_StepMotor9ReadRegMotorState(void)
{
    FPGA_STEP_MOTOR_REG_STATE4 regState4;
    regState4.valAll = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR9_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_STATE4));
    return regState4;
}

//读取当前速度已经发送的脉冲寄存器 STEP_MOTOR9
static uint32_t BoardFPGA_StepMotor9ReadRegCurTabIdxPluseSendOut(void)
{
    uint32_t curTabIdxPluseSendOut;
    curTabIdxPluseSendOut = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR9_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_STATE5));
    return curTabIdxPluseSendOut;
}

//读取速度表元素 STEP_MOTOR9
static ERROR_SUB BoardFPGA_StepMotor9ReadSpdTabElemByIdx(uint16_t index,uint32_t* elemValPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    *elemValPtr = 0;
    //关闭全局中断
    BOARD_FPGA_LOCK();
    //写入序号
    errorSub = BoardFPGA_WriteDataByAddr((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR9_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_SPD_TAB_ELEM_READ),(uint32_t)index);
    if(errorSub != ERROR_SUB_OK)
    {
        //打开全局中断
        BOARD_FPGA_UNLOCK(lastIntState);
        return errorSub;
    }
    //读取元素
    *elemValPtr = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR9_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_SPD_TAB_ELEM));
    //打开全局中断
    BOARD_FPGA_UNLOCK(lastIntState);
    return errorSub;
}

//读取步数表元素 STEP_MOTOR9
static ERROR_SUB BoardFPGA_StepMotor9ReadStpTabElemByIdx(uint16_t index,uint32_t* elemValPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    *elemValPtr = 0;
    //关闭全局中断
    BOARD_FPGA_LOCK();
    //写入序号
    errorSub = BoardFPGA_WriteDataByAddr((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR9_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_STP_TAB_ELEM_READ),(uint32_t)index);
    if(errorSub != ERROR_SUB_OK)
    {
        //打开全局中断
        BOARD_FPGA_UNLOCK(lastIntState);
        return errorSub;
    }
    //读取元素
    *elemValPtr = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR9_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_STP_TAB_ELEM));
    //打开全局中断
    BOARD_FPGA_UNLOCK(lastIntState);
    return errorSub;
}

//写入配置寄存器 STEP_MOTOR9
static ERROR_SUB BoardFPGA_StepMotor9WriteRegConfig(FPGA_STEP_MOTOR_REG_CONFIG regConfig)
{
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR9_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_CONFIG),regConfig.valAll,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR9_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_CONFIG));
}

//写入编码器系数寄存器 STEP_MOTOR9
static ERROR_SUB BoardFPGA_StepMotor9WriteRegEncoderFactor(float factor)
{
    FPGA_STEP_MOTOR_REG_PARAM1 encoderFactor;
    encoderFactor.factorFloat = factor;
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR9_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_PARAM1),encoderFactor.valBits.factorUint32,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR9_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM1));
}

//写入定频速度计数值寄存器 STEP_MOTOR9
static ERROR_SUB BoardFPGA_StepMotor9WriteRegFixSpdCnt(uint32_t fixSpdCnt)
{
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR9_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_PARAM2),fixSpdCnt,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR9_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM2));
}

//写入本次运行总步数寄存器 STEP_MOTOR9
static ERROR_SUB BoardFPGA_StepMotor9WriteRegTotalStepSet(uint32_t totalStep)
{
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR9_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_PARAM3),totalStep,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR9_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM3));
}

//设置步数曲线的起始结束位置 STEP_MOTOR9
static ERROR_SUB BoardFPGA_StepMotor9WriteRegTabSpdIndexRange(FPGA_STEP_MOTOR_REG_PARAM4 tabRange)
{
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR9_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_PARAM4),tabRange.valAll,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR9_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM4));
}

//写入停止条件 STEP_MOTOR9
static ERROR_SUB BoardFPGA_StepMotor9WriteRegStopCondition(FPGA_STEP_MOTOR_REG_PARAM5 stopCondition)
{
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR9_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_PARAM5),stopCondition.valAll,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR9_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM5));
}

//写入控制字 STEP_MOTOR9
static ERROR_SUB BoardFPGA_StepMotor9WriteRegControl(FPGA_STEP_MOTOR_REG_CONTROL regControl)
{
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR9_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_CONTROL),regControl.valAll,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR9_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_CONTROL));
}

//写入速度表 STEP_MOTOR9
static ERROR_SUB BoardFPGA_StepMotor9WriteRegSpdTabElem(uint16_t index,uint32_t val)
{
    return BoardFPGA_WriteDataByAddr((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR9_BASE_ADDR + FPGA_REG_WRITE_ADDR_STEP_MOTOR_SPD_TAB_ELEM_WRITE + index),val);
}

//写入步数表 STEP_MOTOR9
static ERROR_SUB BoardFPGA_StepMotor9WriteRegStpTabElem(uint16_t index,uint32_t val)
{
    return BoardFPGA_WriteDataByAddr((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR9_BASE_ADDR + FPGA_REG_WRITE_ADDR_STEP_MOTOR_STP_TAB_ELEM_WRITE + index),val);
}
#endif
/**********************************STEP_MOTOR10基础操作函数*******************************************/
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 9)
//读取配置寄存器 STEP_MOTOR10
static FPGA_STEP_MOTOR_REG_CONFIG BoardFPGA_StepMotor10ReadRegConfig(void)
{
    FPGA_STEP_MOTOR_REG_CONFIG regConfig;
    regConfig.valAll = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR10_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_CONFIG));
    return regConfig;
}

//读取编码器系数寄存器 STEP_MOTOR10
static float BoardFPGA_StepMotor10ReadRegEncoderFactor(void)
{
    FPGA_STEP_MOTOR_REG_PARAM1 regParam1;
    regParam1.valBits.factorUint32 = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR10_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM1));
    return regParam1.factorFloat;
}

//读取定频运转速度寄存器 STEP_MOTOR10
static uint32_t BoardFPGA_StepMotor10ReadRegFixSpdCnt(void)
{
    uint32_t fixSpdCnt;
    fixSpdCnt = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR10_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM2));
    return fixSpdCnt;
}

//读取本次运行总步数寄存器 STEP_MOTOR10
static uint32_t BoardFPGA_StepMotor10ReadRegTotalStepSet(void)
{
    uint32_t totalStepSet;
    totalStepSet = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR10_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM3));
    return totalStepSet;
}

//读取设置步数曲线的起始结束位置 STEP_MOTOR10
static FPGA_STEP_MOTOR_REG_PARAM4 BoardFPGA_StepMotor10ReadRegTabSpdIndexRange(void)
{
    FPGA_STEP_MOTOR_REG_PARAM4 regParam4;
    regParam4.valAll = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR10_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM4));
    return regParam4;
}

//读取配置的停止条件寄存器 STEP_MOTOR10
static FPGA_STEP_MOTOR_REG_PARAM5 BoardFPGA_StepMotor10ReadRegStopCondition(void)
{
    FPGA_STEP_MOTOR_REG_PARAM5 regParam5;
    regParam5.valAll = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR10_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM5));
    return regParam5;
}

//读取当前控制寄存器 STEP_MOTOR10
static FPGA_STEP_MOTOR_REG_CONTROL BoardFPGA_StepMotor10ReadRegControl(void)
{
    FPGA_STEP_MOTOR_REG_CONTROL regControl;
    regControl.valAll = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR10_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_CONTROL));
    return regControl;
}

//读取已经发出的脉冲寄存器 STEP_MOTOR10
static uint32_t BoardFPGA_StepMotor10ReadRegTotalStepSendOut(void)
{
    uint32_t totalStepSendOut;
    totalStepSendOut = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR10_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_STATE1));
    return totalStepSendOut;
}

//读取当前编码器位置寄存器 STEP_MOTOR10
static int32_t BoardFPGA_StepMotor10ReadRegEncoderPos(void)
{
    FPGA_STEP_MOTOR_REG_STATE2 encoderPos;
    encoderPos.encoderPosUint32 = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR10_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_STATE2));
    return encoderPos.valBits.encoderPosInt32;
}

//读取当前信号状态和电机步数表索引序号寄存器 STEP_MOTOR10
static FPGA_STEP_MOTOR_REG_STATE3 BoardFPGA_StepMotor10ReadRegSensorStateTabIndex(void)
{
    FPGA_STEP_MOTOR_REG_STATE3 regState3;
    regState3.valAll = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR10_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_STATE3));
    return regState3;
}

//读取电机状态寄存器 STEP_MOTOR10
static FPGA_STEP_MOTOR_REG_STATE4 BoardFPGA_StepMotor10ReadRegMotorState(void)
{
    FPGA_STEP_MOTOR_REG_STATE4 regState4;
    regState4.valAll = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR10_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_STATE4));
    return regState4;
}

//读取当前速度已经发送的脉冲寄存器 STEP_MOTOR10
static uint32_t BoardFPGA_StepMotor10ReadRegCurTabIdxPluseSendOut(void)
{
    uint32_t curTabIdxPluseSendOut;
    curTabIdxPluseSendOut = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR10_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_STATE5));
    return curTabIdxPluseSendOut;
}

//读取速度表元素 STEP_MOTOR10
static ERROR_SUB BoardFPGA_StepMotor10ReadSpdTabElemByIdx(uint16_t index,uint32_t* elemValPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    *elemValPtr = 0;
    //关闭全局中断
    BOARD_FPGA_LOCK();
    //写入序号
    errorSub = BoardFPGA_WriteDataByAddr((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR10_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_SPD_TAB_ELEM_READ),(uint32_t)index);
    if(errorSub != ERROR_SUB_OK)
    {
        //打开全局中断
        BOARD_FPGA_UNLOCK(lastIntState);
        return errorSub;
    }
    //读取元素
    *elemValPtr = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR10_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_SPD_TAB_ELEM));
    //打开全局中断
    BOARD_FPGA_UNLOCK(lastIntState);
    return errorSub;
}

//读取步数表元素 STEP_MOTOR10
static ERROR_SUB BoardFPGA_StepMotor10ReadStpTabElemByIdx(uint16_t index,uint32_t* elemValPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    *elemValPtr = 0;
    //关闭全局中断
    BOARD_FPGA_LOCK();
    //写入序号
    errorSub = BoardFPGA_WriteDataByAddr((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR10_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_STP_TAB_ELEM_READ),(uint32_t)index);
    if(errorSub != ERROR_SUB_OK)
    {
        //打开全局中断
        BOARD_FPGA_UNLOCK(lastIntState);
        return errorSub;
    }
    //读取元素
    *elemValPtr = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR10_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_STP_TAB_ELEM));
    //打开全局中断
    BOARD_FPGA_UNLOCK(lastIntState);
    return errorSub;
}

//写入配置寄存器 STEP_MOTOR10
static ERROR_SUB BoardFPGA_StepMotor10WriteRegConfig(FPGA_STEP_MOTOR_REG_CONFIG regConfig)
{
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR10_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_CONFIG),regConfig.valAll,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR10_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_CONFIG));
}

//写入编码器系数寄存器 STEP_MOTOR10
static ERROR_SUB BoardFPGA_StepMotor10WriteRegEncoderFactor(float factor)
{
    FPGA_STEP_MOTOR_REG_PARAM1 encoderFactor;
    encoderFactor.factorFloat = factor;
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR10_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_PARAM1),encoderFactor.valBits.factorUint32,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR10_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM1));
}

//写入定频速度计数值寄存器 STEP_MOTOR10
static ERROR_SUB BoardFPGA_StepMotor10WriteRegFixSpdCnt(uint32_t fixSpdCnt)
{
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR10_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_PARAM2),fixSpdCnt,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR10_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM2));
}

//写入本次运行总步数寄存器 STEP_MOTOR10
static ERROR_SUB BoardFPGA_StepMotor10WriteRegTotalStepSet(uint32_t totalStep)
{
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR10_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_PARAM3),totalStep,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR10_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM3));
}

//设置步数曲线的起始结束位置 STEP_MOTOR10
static ERROR_SUB BoardFPGA_StepMotor10WriteRegTabSpdIndexRange(FPGA_STEP_MOTOR_REG_PARAM4 tabRange)
{
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR10_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_PARAM4),tabRange.valAll,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR10_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM4));
}

//写入停止条件 STEP_MOTOR10
static ERROR_SUB BoardFPGA_StepMotor10WriteRegStopCondition(FPGA_STEP_MOTOR_REG_PARAM5 stopCondition)
{
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR10_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_PARAM5),stopCondition.valAll,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR10_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM5));
}

//写入控制字 STEP_MOTOR10
static ERROR_SUB BoardFPGA_StepMotor10WriteRegControl(FPGA_STEP_MOTOR_REG_CONTROL regControl)
{
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR10_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_CONTROL),regControl.valAll,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR10_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_CONTROL));
}

//写入速度表 STEP_MOTOR10
static ERROR_SUB BoardFPGA_StepMotor10WriteRegSpdTabElem(uint16_t index,uint32_t val)
{
    return BoardFPGA_WriteDataByAddr((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR10_BASE_ADDR + FPGA_REG_WRITE_ADDR_STEP_MOTOR_SPD_TAB_ELEM_WRITE + index),val);
}

//写入步数表 STEP_MOTOR10
static ERROR_SUB BoardFPGA_StepMotor10WriteRegStpTabElem(uint16_t index,uint32_t val)
{
    return BoardFPGA_WriteDataByAddr((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR10_BASE_ADDR + FPGA_REG_WRITE_ADDR_STEP_MOTOR_STP_TAB_ELEM_WRITE + index),val);
}
#endif
/**********************************STEP_MOTOR101基础操作函数*******************************************/
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 10)
//读取配置寄存器 STEP_MOTOR11
static FPGA_STEP_MOTOR_REG_CONFIG BoardFPGA_StepMotor11ReadRegConfig(void)
{
    FPGA_STEP_MOTOR_REG_CONFIG regConfig;
    regConfig.valAll = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR11_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_CONFIG));
    return regConfig;
}

//读取编码器系数寄存器 STEP_MOTOR11
static float BoardFPGA_StepMotor11ReadRegEncoderFactor(void)
{
    FPGA_STEP_MOTOR_REG_PARAM1 regParam1;
    regParam1.valBits.factorUint32 = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR11_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM1));
    return regParam1.factorFloat;
}

//读取定频运转速度寄存器 STEP_MOTOR11
static uint32_t BoardFPGA_StepMotor11ReadRegFixSpdCnt(void)
{
    uint32_t fixSpdCnt;
    fixSpdCnt = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR11_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM2));
    return fixSpdCnt;
}

//读取本次运行总步数寄存器 STEP_MOTOR11
static uint32_t BoardFPGA_StepMotor11ReadRegTotalStepSet(void)
{
    uint32_t totalStepSet;
    totalStepSet = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR11_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM3));
    return totalStepSet;
}

//读取设置步数曲线的起始结束位置 STEP_MOTOR11
static FPGA_STEP_MOTOR_REG_PARAM4 BoardFPGA_StepMotor11ReadRegTabSpdIndexRange(void)
{
    FPGA_STEP_MOTOR_REG_PARAM4 regParam4;
    regParam4.valAll = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR11_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM4));
    return regParam4;
}

//读取配置的停止条件寄存器 STEP_MOTOR11
static FPGA_STEP_MOTOR_REG_PARAM5 BoardFPGA_StepMotor11ReadRegStopCondition(void)
{
    FPGA_STEP_MOTOR_REG_PARAM5 regParam5;
    regParam5.valAll = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR11_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM5));
    return regParam5;
}

//读取当前控制寄存器 STEP_MOTOR11
static FPGA_STEP_MOTOR_REG_CONTROL BoardFPGA_StepMotor11ReadRegControl(void)
{
    FPGA_STEP_MOTOR_REG_CONTROL regControl;
    regControl.valAll = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR11_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_CONTROL));
    return regControl;
}

//读取已经发出的脉冲寄存器 STEP_MOTOR11
static uint32_t BoardFPGA_StepMotor11ReadRegTotalStepSendOut(void)
{
    uint32_t totalStepSendOut;
    totalStepSendOut = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR11_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_STATE1));
    return totalStepSendOut;
}

//读取当前编码器位置寄存器 STEP_MOTOR11
static int32_t BoardFPGA_StepMotor11ReadRegEncoderPos(void)
{
    FPGA_STEP_MOTOR_REG_STATE2 encoderPos;
    encoderPos.encoderPosUint32 = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR11_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_STATE2));
    return encoderPos.valBits.encoderPosInt32;
}

//读取当前信号状态和电机步数表索引序号寄存器 STEP_MOTOR11
static FPGA_STEP_MOTOR_REG_STATE3 BoardFPGA_StepMotor11ReadRegSensorStateTabIndex(void)
{
    FPGA_STEP_MOTOR_REG_STATE3 regState3;
    regState3.valAll = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR11_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_STATE3));
    return regState3;
}

//读取电机状态寄存器 STEP_MOTOR11
static FPGA_STEP_MOTOR_REG_STATE4 BoardFPGA_StepMotor11ReadRegMotorState(void)
{
    FPGA_STEP_MOTOR_REG_STATE4 regState4;
    regState4.valAll = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR11_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_STATE4));
    return regState4;
}

//读取当前速度已经发送的脉冲寄存器 STEP_MOTOR11
static uint32_t BoardFPGA_StepMotor11ReadRegCurTabIdxPluseSendOut(void)
{
    uint32_t curTabIdxPluseSendOut;
    curTabIdxPluseSendOut = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR11_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_STATE5));
    return curTabIdxPluseSendOut;
}

//读取速度表元素 STEP_MOTOR11
static ERROR_SUB BoardFPGA_StepMotor11ReadSpdTabElemByIdx(uint16_t index,uint32_t* elemValPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    *elemValPtr = 0;
    //关闭全局中断
    BOARD_FPGA_LOCK();
    //写入序号
    errorSub = BoardFPGA_WriteDataByAddr((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR11_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_SPD_TAB_ELEM_READ),(uint32_t)index);
    if(errorSub != ERROR_SUB_OK)
    {
        //打开全局中断
        BOARD_FPGA_UNLOCK(lastIntState);
        return errorSub;
    }
    //读取元素
    *elemValPtr = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR11_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_SPD_TAB_ELEM));
    //打开全局中断
    BOARD_FPGA_UNLOCK(lastIntState);
    return errorSub;
}

//读取步数表元素 STEP_MOTOR11
static ERROR_SUB BoardFPGA_StepMotor11ReadStpTabElemByIdx(uint16_t index,uint32_t* elemValPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    *elemValPtr = 0;
    //关闭全局中断
    BOARD_FPGA_LOCK();
    //写入序号
    errorSub = BoardFPGA_WriteDataByAddr((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR11_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_STP_TAB_ELEM_READ),(uint32_t)index);
    if(errorSub != ERROR_SUB_OK)
    {
        //打开全局中断
        BOARD_FPGA_UNLOCK(lastIntState);
        return errorSub;
    }
    //读取元素
    *elemValPtr = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR11_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_STP_TAB_ELEM));
    //打开全局中断
    BOARD_FPGA_UNLOCK(lastIntState);
    return errorSub;
}

//写入配置寄存器 STEP_MOTOR11
static ERROR_SUB BoardFPGA_StepMotor11WriteRegConfig(FPGA_STEP_MOTOR_REG_CONFIG regConfig)
{
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR11_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_CONFIG),regConfig.valAll,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR11_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_CONFIG));
}

//写入编码器系数寄存器 STEP_MOTOR11
static ERROR_SUB BoardFPGA_StepMotor11WriteRegEncoderFactor(float factor)
{
    FPGA_STEP_MOTOR_REG_PARAM1 encoderFactor;
    encoderFactor.factorFloat = factor;
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR11_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_PARAM1),encoderFactor.valBits.factorUint32,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR11_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM1));
}

//写入定频速度计数值寄存器 STEP_MOTOR11
static ERROR_SUB BoardFPGA_StepMotor11WriteRegFixSpdCnt(uint32_t fixSpdCnt)
{
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR11_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_PARAM2),fixSpdCnt,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR11_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM2));
}

//写入本次运行总步数寄存器 STEP_MOTOR11
static ERROR_SUB BoardFPGA_StepMotor11WriteRegTotalStepSet(uint32_t totalStep)
{
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR11_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_PARAM3),totalStep,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR11_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM3));
}

//设置步数曲线的起始结束位置 STEP_MOTOR11
static ERROR_SUB BoardFPGA_StepMotor11WriteRegTabSpdIndexRange(FPGA_STEP_MOTOR_REG_PARAM4 tabRange)
{
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR11_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_PARAM4),tabRange.valAll,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR11_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM4));
}

//写入停止条件 STEP_MOTOR11
static ERROR_SUB BoardFPGA_StepMotor11WriteRegStopCondition(FPGA_STEP_MOTOR_REG_PARAM5 stopCondition)
{
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR11_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_PARAM5),stopCondition.valAll,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR11_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM5));
}

//写入控制字 STEP_MOTOR11
static ERROR_SUB BoardFPGA_StepMotor11WriteRegControl(FPGA_STEP_MOTOR_REG_CONTROL regControl)
{
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR11_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_CONTROL),regControl.valAll,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR11_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_CONTROL));
}

//写入速度表 STEP_MOTOR11
static ERROR_SUB BoardFPGA_StepMotor11WriteRegSpdTabElem(uint16_t index,uint32_t val)
{
    return BoardFPGA_WriteDataByAddr((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR11_BASE_ADDR + FPGA_REG_WRITE_ADDR_STEP_MOTOR_SPD_TAB_ELEM_WRITE + index),val);
}

//写入步数表 STEP_MOTOR11
static ERROR_SUB BoardFPGA_StepMotor11WriteRegStpTabElem(uint16_t index,uint32_t val)
{
    return BoardFPGA_WriteDataByAddr((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR11_BASE_ADDR + FPGA_REG_WRITE_ADDR_STEP_MOTOR_STP_TAB_ELEM_WRITE + index),val);
}
#endif
/**********************************STEP_MOTOR112基础操作函数*******************************************/
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 11)
//读取配置寄存器 STEP_MOTOR12
static FPGA_STEP_MOTOR_REG_CONFIG BoardFPGA_StepMotor12ReadRegConfig(void)
{
    FPGA_STEP_MOTOR_REG_CONFIG regConfig;
    regConfig.valAll = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR12_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_CONFIG));
    return regConfig;
}

//读取编码器系数寄存器 STEP_MOTOR12
static float BoardFPGA_StepMotor12ReadRegEncoderFactor(void)
{
    FPGA_STEP_MOTOR_REG_PARAM1 regParam1;
    regParam1.valBits.factorUint32 = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR12_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM1));
    return regParam1.factorFloat;
}

//读取定频运转速度寄存器 STEP_MOTOR12
static uint32_t BoardFPGA_StepMotor12ReadRegFixSpdCnt(void)
{
    uint32_t fixSpdCnt;
    fixSpdCnt = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR12_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM2));
    return fixSpdCnt;
}

//读取本次运行总步数寄存器 STEP_MOTOR12
static uint32_t BoardFPGA_StepMotor12ReadRegTotalStepSet(void)
{
    uint32_t totalStepSet;
    totalStepSet = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR12_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM3));
    return totalStepSet;
}

//读取设置步数曲线的起始结束位置 STEP_MOTOR12
static FPGA_STEP_MOTOR_REG_PARAM4 BoardFPGA_StepMotor12ReadRegTabSpdIndexRange(void)
{
    FPGA_STEP_MOTOR_REG_PARAM4 regParam4;
    regParam4.valAll = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR12_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM4));
    return regParam4;
}

//读取配置的停止条件寄存器 STEP_MOTOR12
static FPGA_STEP_MOTOR_REG_PARAM5 BoardFPGA_StepMotor12ReadRegStopCondition(void)
{
    FPGA_STEP_MOTOR_REG_PARAM5 regParam5;
    regParam5.valAll = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR12_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM5));
    return regParam5;
}

//读取当前控制寄存器 STEP_MOTOR12
static FPGA_STEP_MOTOR_REG_CONTROL BoardFPGA_StepMotor12ReadRegControl(void)
{
    FPGA_STEP_MOTOR_REG_CONTROL regControl;
    regControl.valAll = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR12_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_CONTROL));
    return regControl;
}

//读取已经发出的脉冲寄存器 STEP_MOTOR12
static uint32_t BoardFPGA_StepMotor12ReadRegTotalStepSendOut(void)
{
    uint32_t totalStepSendOut;
    totalStepSendOut = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR12_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_STATE1));
    return totalStepSendOut;
}

//读取当前编码器位置寄存器 STEP_MOTOR12
static int32_t BoardFPGA_StepMotor12ReadRegEncoderPos(void)
{
    FPGA_STEP_MOTOR_REG_STATE2 encoderPos;
    encoderPos.encoderPosUint32 = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR12_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_STATE2));
    return encoderPos.valBits.encoderPosInt32;
}

//读取当前信号状态和电机步数表索引序号寄存器 STEP_MOTOR12
static FPGA_STEP_MOTOR_REG_STATE3 BoardFPGA_StepMotor12ReadRegSensorStateTabIndex(void)
{
    FPGA_STEP_MOTOR_REG_STATE3 regState3;
    regState3.valAll = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR12_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_STATE3));
    return regState3;
}

//读取电机状态寄存器 STEP_MOTOR12
static FPGA_STEP_MOTOR_REG_STATE4 BoardFPGA_StepMotor12ReadRegMotorState(void)
{
    FPGA_STEP_MOTOR_REG_STATE4 regState4;
    regState4.valAll = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR12_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_STATE4));
    return regState4;
}

//读取当前速度已经发送的脉冲寄存器 STEP_MOTOR12
static uint32_t BoardFPGA_StepMotor12ReadRegCurTabIdxPluseSendOut(void)
{
    uint32_t curTabIdxPluseSendOut;
    curTabIdxPluseSendOut = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR12_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_STATE5));
    return curTabIdxPluseSendOut;
}

//读取速度表元素 STEP_MOTOR12
static ERROR_SUB BoardFPGA_StepMotor12ReadSpdTabElemByIdx(uint16_t index,uint32_t* elemValPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    *elemValPtr = 0;
    //关闭全局中断
    BOARD_FPGA_LOCK();
    //写入序号
    errorSub = BoardFPGA_WriteDataByAddr((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR12_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_SPD_TAB_ELEM_READ),(uint32_t)index);
    if(errorSub != ERROR_SUB_OK)
    {
        //打开全局中断
        BOARD_FPGA_UNLOCK(lastIntState);
        return errorSub;
    }
    //读取元素
    *elemValPtr = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR12_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_SPD_TAB_ELEM));
    //打开全局中断
    BOARD_FPGA_UNLOCK(lastIntState);
    return errorSub;
}

//读取步数表元素 STEP_MOTOR12
static ERROR_SUB BoardFPGA_StepMotor12ReadStpTabElemByIdx(uint16_t index,uint32_t* elemValPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    *elemValPtr = 0;
    //关闭全局中断
    BOARD_FPGA_LOCK();
    //写入序号
    errorSub = BoardFPGA_WriteDataByAddr((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR12_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_STP_TAB_ELEM_READ),(uint32_t)index);
    if(errorSub != ERROR_SUB_OK)
    {
        //打开全局中断
        BOARD_FPGA_UNLOCK(lastIntState);
        return errorSub;
    }
    //读取元素
    *elemValPtr = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR12_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_STP_TAB_ELEM));
    //打开全局中断
    BOARD_FPGA_UNLOCK(lastIntState);
    return errorSub;
}

//写入配置寄存器 STEP_MOTOR12
static ERROR_SUB BoardFPGA_StepMotor12WriteRegConfig(FPGA_STEP_MOTOR_REG_CONFIG regConfig)
{
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR12_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_CONFIG),regConfig.valAll,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR12_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_CONFIG));
}

//写入编码器系数寄存器 STEP_MOTOR12
static ERROR_SUB BoardFPGA_StepMotor12WriteRegEncoderFactor(float factor)
{
    FPGA_STEP_MOTOR_REG_PARAM1 encoderFactor;
    encoderFactor.factorFloat = factor;
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR12_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_PARAM1),encoderFactor.valBits.factorUint32,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR12_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM1));
}

//写入定频速度计数值寄存器 STEP_MOTOR12
static ERROR_SUB BoardFPGA_StepMotor12WriteRegFixSpdCnt(uint32_t fixSpdCnt)
{
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR12_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_PARAM2),fixSpdCnt,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR12_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM2));
}

//写入本次运行总步数寄存器 STEP_MOTOR12
static ERROR_SUB BoardFPGA_StepMotor12WriteRegTotalStepSet(uint32_t totalStep)
{
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR12_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_PARAM3),totalStep,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR12_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM3));
}

//设置步数曲线的起始结束位置 STEP_MOTOR12
static ERROR_SUB BoardFPGA_StepMotor12WriteRegTabSpdIndexRange(FPGA_STEP_MOTOR_REG_PARAM4 tabRange)
{
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR12_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_PARAM4),tabRange.valAll,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR12_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM4));
}

//写入停止条件 STEP_MOTOR12
static ERROR_SUB BoardFPGA_StepMotor12WriteRegStopCondition(FPGA_STEP_MOTOR_REG_PARAM5 stopCondition)
{
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR12_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_PARAM5),stopCondition.valAll,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR12_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM5));
}

//写入控制字 STEP_MOTOR12
static ERROR_SUB BoardFPGA_StepMotor12WriteRegControl(FPGA_STEP_MOTOR_REG_CONTROL regControl)
{
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR12_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_CONTROL),regControl.valAll,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR12_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_CONTROL));
}

//写入速度表 STEP_MOTOR12
static ERROR_SUB BoardFPGA_StepMotor12WriteRegSpdTabElem(uint16_t index,uint32_t val)
{
    return BoardFPGA_WriteDataByAddr((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR12_BASE_ADDR + FPGA_REG_WRITE_ADDR_STEP_MOTOR_SPD_TAB_ELEM_WRITE + index),val);
}

//写入步数表 STEP_MOTOR12
static ERROR_SUB BoardFPGA_StepMotor12WriteRegStpTabElem(uint16_t index,uint32_t val)
{
    return BoardFPGA_WriteDataByAddr((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR12_BASE_ADDR + FPGA_REG_WRITE_ADDR_STEP_MOTOR_STP_TAB_ELEM_WRITE + index),val);
}
#endif
/**********************************STEP_MOTOR13基础操作函数*******************************************/
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 12)
//读取配置寄存器 STEP_MOTOR13
static FPGA_STEP_MOTOR_REG_CONFIG BoardFPGA_StepMotor13ReadRegConfig(void)
{
    FPGA_STEP_MOTOR_REG_CONFIG regConfig;
    regConfig.valAll = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR13_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_CONFIG));
    return regConfig;
}

//读取编码器系数寄存器 STEP_MOTOR13
static float BoardFPGA_StepMotor13ReadRegEncoderFactor(void)
{
    FPGA_STEP_MOTOR_REG_PARAM1 regParam1;
    regParam1.valBits.factorUint32 = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR13_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM1));
    return regParam1.factorFloat;
}

//读取定频运转速度寄存器 STEP_MOTOR13
static uint32_t BoardFPGA_StepMotor13ReadRegFixSpdCnt(void)
{
    uint32_t fixSpdCnt;
    fixSpdCnt = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR13_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM2));
    return fixSpdCnt;
}

//读取本次运行总步数寄存器 STEP_MOTOR13
static uint32_t BoardFPGA_StepMotor13ReadRegTotalStepSet(void)
{
    uint32_t totalStepSet;
    totalStepSet = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR13_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM3));
    return totalStepSet;
}

//读取设置步数曲线的起始结束位置 STEP_MOTOR13
static FPGA_STEP_MOTOR_REG_PARAM4 BoardFPGA_StepMotor13ReadRegTabSpdIndexRange(void)
{
    FPGA_STEP_MOTOR_REG_PARAM4 regParam4;
    regParam4.valAll = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR13_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM4));
    return regParam4;
}

//读取配置的停止条件寄存器 STEP_MOTOR13
static FPGA_STEP_MOTOR_REG_PARAM5 BoardFPGA_StepMotor13ReadRegStopCondition(void)
{
    FPGA_STEP_MOTOR_REG_PARAM5 regParam5;
    regParam5.valAll = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR13_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM5));
    return regParam5;
}

//读取当前控制寄存器 STEP_MOTOR13
static FPGA_STEP_MOTOR_REG_CONTROL BoardFPGA_StepMotor13ReadRegControl(void)
{
    FPGA_STEP_MOTOR_REG_CONTROL regControl;
    regControl.valAll = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR13_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_CONTROL));
    return regControl;
}

//读取已经发出的脉冲寄存器 STEP_MOTOR13
static uint32_t BoardFPGA_StepMotor13ReadRegTotalStepSendOut(void)
{
    uint32_t totalStepSendOut;
    totalStepSendOut = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR13_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_STATE1));
    return totalStepSendOut;
}

//读取当前编码器位置寄存器 STEP_MOTOR13
static int32_t BoardFPGA_StepMotor13ReadRegEncoderPos(void)
{
    FPGA_STEP_MOTOR_REG_STATE2 encoderPos;
    encoderPos.encoderPosUint32 = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR13_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_STATE2));
    return encoderPos.valBits.encoderPosInt32;
}

//读取当前信号状态和电机步数表索引序号寄存器 STEP_MOTOR13
static FPGA_STEP_MOTOR_REG_STATE3 BoardFPGA_StepMotor13ReadRegSensorStateTabIndex(void)
{
    FPGA_STEP_MOTOR_REG_STATE3 regState3;
    regState3.valAll = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR13_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_STATE3));
    return regState3;
}

//读取电机状态寄存器 STEP_MOTOR13
static FPGA_STEP_MOTOR_REG_STATE4 BoardFPGA_StepMotor13ReadRegMotorState(void)
{
    FPGA_STEP_MOTOR_REG_STATE4 regState4;
    regState4.valAll = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR13_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_STATE4));
    return regState4;
}

//读取当前速度已经发送的脉冲寄存器 STEP_MOTOR13
static uint32_t BoardFPGA_StepMotor13ReadRegCurTabIdxPluseSendOut(void)
{
    uint32_t curTabIdxPluseSendOut;
    curTabIdxPluseSendOut = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR13_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_STATE5));
    return curTabIdxPluseSendOut;
}

//读取速度表元素 STEP_MOTOR13
static ERROR_SUB BoardFPGA_StepMotor13ReadSpdTabElemByIdx(uint16_t index,uint32_t* elemValPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    *elemValPtr = 0;
    //关闭全局中断
    BOARD_FPGA_LOCK();
    //写入序号
    errorSub = BoardFPGA_WriteDataByAddr((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR13_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_SPD_TAB_ELEM_READ),(uint32_t)index);
    if(errorSub != ERROR_SUB_OK)
    {
        //打开全局中断
        BOARD_FPGA_UNLOCK(lastIntState);
        return errorSub;
    }
    //读取元素
    *elemValPtr = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR13_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_SPD_TAB_ELEM));
    //打开全局中断
    BOARD_FPGA_UNLOCK(lastIntState);
    return errorSub;
}

//读取步数表元素 STEP_MOTOR13
static ERROR_SUB BoardFPGA_StepMotor13ReadStpTabElemByIdx(uint16_t index,uint32_t* elemValPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    *elemValPtr = 0;
    //关闭全局中断
    BOARD_FPGA_LOCK();
    //写入序号
    errorSub = BoardFPGA_WriteDataByAddr((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR13_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_STP_TAB_ELEM_READ),(uint32_t)index);
    if(errorSub != ERROR_SUB_OK)
    {
        //打开全局中断
        BOARD_FPGA_UNLOCK(lastIntState);
        return errorSub;
    }
    //读取元素
    *elemValPtr = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR13_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_STP_TAB_ELEM));
    //打开全局中断
    BOARD_FPGA_UNLOCK(lastIntState);
    return errorSub;
}

//写入配置寄存器 STEP_MOTOR13
static ERROR_SUB BoardFPGA_StepMotor13WriteRegConfig(FPGA_STEP_MOTOR_REG_CONFIG regConfig)
{
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR13_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_CONFIG),regConfig.valAll,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR13_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_CONFIG));
}

//写入编码器系数寄存器 STEP_MOTOR13
static ERROR_SUB BoardFPGA_StepMotor13WriteRegEncoderFactor(float factor)
{
    FPGA_STEP_MOTOR_REG_PARAM1 encoderFactor;
    encoderFactor.factorFloat = factor;
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR13_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_PARAM1),encoderFactor.valBits.factorUint32,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR13_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM1));
}

//写入定频速度计数值寄存器 STEP_MOTOR13
static ERROR_SUB BoardFPGA_StepMotor13WriteRegFixSpdCnt(uint32_t fixSpdCnt)
{
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR13_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_PARAM2),fixSpdCnt,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR13_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM2));
}

//写入本次运行总步数寄存器 STEP_MOTOR13
static ERROR_SUB BoardFPGA_StepMotor13WriteRegTotalStepSet(uint32_t totalStep)
{
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR13_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_PARAM3),totalStep,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR13_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM3));
}

//设置步数曲线的起始结束位置 STEP_MOTOR13
static ERROR_SUB BoardFPGA_StepMotor13WriteRegTabSpdIndexRange(FPGA_STEP_MOTOR_REG_PARAM4 tabRange)
{
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR13_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_PARAM4),tabRange.valAll,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR13_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM4));
}

//写入停止条件 STEP_MOTOR13
static ERROR_SUB BoardFPGA_StepMotor13WriteRegStopCondition(FPGA_STEP_MOTOR_REG_PARAM5 stopCondition)
{
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR13_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_PARAM5),stopCondition.valAll,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR13_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM5));
}

//写入控制字 STEP_MOTOR13
static ERROR_SUB BoardFPGA_StepMotor13WriteRegControl(FPGA_STEP_MOTOR_REG_CONTROL regControl)
{
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR13_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_CONTROL),regControl.valAll,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR13_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_CONTROL));
}

//写入速度表 STEP_MOTOR13
static ERROR_SUB BoardFPGA_StepMotor13WriteRegSpdTabElem(uint16_t index,uint32_t val)
{
    return BoardFPGA_WriteDataByAddr((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR13_BASE_ADDR + FPGA_REG_WRITE_ADDR_STEP_MOTOR_SPD_TAB_ELEM_WRITE + index),val);
}

//写入步数表 STEP_MOTOR13
static ERROR_SUB BoardFPGA_StepMotor13WriteRegStpTabElem(uint16_t index,uint32_t val)
{
    return BoardFPGA_WriteDataByAddr((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR13_BASE_ADDR + FPGA_REG_WRITE_ADDR_STEP_MOTOR_STP_TAB_ELEM_WRITE + index),val);
}
#endif
/**********************************STEP_MOTOR14基础操作函数*******************************************/
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 13)
//读取配置寄存器 STEP_MOTOR14
static FPGA_STEP_MOTOR_REG_CONFIG BoardFPGA_StepMotor14ReadRegConfig(void)
{
    FPGA_STEP_MOTOR_REG_CONFIG regConfig;
    regConfig.valAll = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR14_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_CONFIG));
    return regConfig;
}

//读取编码器系数寄存器 STEP_MOTOR14
static float BoardFPGA_StepMotor14ReadRegEncoderFactor(void)
{
    FPGA_STEP_MOTOR_REG_PARAM1 regParam1;
    regParam1.valBits.factorUint32 = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR14_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM1));
    return regParam1.factorFloat;
}

//读取定频运转速度寄存器 STEP_MOTOR14
static uint32_t BoardFPGA_StepMotor14ReadRegFixSpdCnt(void)
{
    uint32_t fixSpdCnt;
    fixSpdCnt = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR14_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM2));
    return fixSpdCnt;
}

//读取本次运行总步数寄存器 STEP_MOTOR14
static uint32_t BoardFPGA_StepMotor14ReadRegTotalStepSet(void)
{
    uint32_t totalStepSet;
    totalStepSet = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR14_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM3));
    return totalStepSet;
}

//读取设置步数曲线的起始结束位置 STEP_MOTOR14
static FPGA_STEP_MOTOR_REG_PARAM4 BoardFPGA_StepMotor14ReadRegTabSpdIndexRange(void)
{
    FPGA_STEP_MOTOR_REG_PARAM4 regParam4;
    regParam4.valAll = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR14_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM4));
    return regParam4;
}

//读取配置的停止条件寄存器 STEP_MOTOR14
static FPGA_STEP_MOTOR_REG_PARAM5 BoardFPGA_StepMotor14ReadRegStopCondition(void)
{
    FPGA_STEP_MOTOR_REG_PARAM5 regParam5;
    regParam5.valAll = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR14_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM5));
    return regParam5;
}

//读取当前控制寄存器 STEP_MOTOR14
static FPGA_STEP_MOTOR_REG_CONTROL BoardFPGA_StepMotor14ReadRegControl(void)
{
    FPGA_STEP_MOTOR_REG_CONTROL regControl;
    regControl.valAll = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR14_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_CONTROL));
    return regControl;
}

//读取已经发出的脉冲寄存器 STEP_MOTOR14
static uint32_t BoardFPGA_StepMotor14ReadRegTotalStepSendOut(void)
{
    uint32_t totalStepSendOut;
    totalStepSendOut = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR14_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_STATE1));
    return totalStepSendOut;
}

//读取当前编码器位置寄存器 STEP_MOTOR14
static int32_t BoardFPGA_StepMotor14ReadRegEncoderPos(void)
{
    FPGA_STEP_MOTOR_REG_STATE2 encoderPos;
    encoderPos.encoderPosUint32 = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR14_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_STATE2));
    return encoderPos.valBits.encoderPosInt32;
}

//读取当前信号状态和电机步数表索引序号寄存器 STEP_MOTOR14
static FPGA_STEP_MOTOR_REG_STATE3 BoardFPGA_StepMotor14ReadRegSensorStateTabIndex(void)
{
    FPGA_STEP_MOTOR_REG_STATE3 regState3;
    regState3.valAll = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR14_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_STATE3));
    return regState3;
}

//读取电机状态寄存器 STEP_MOTOR14
static FPGA_STEP_MOTOR_REG_STATE4 BoardFPGA_StepMotor14ReadRegMotorState(void)
{
    FPGA_STEP_MOTOR_REG_STATE4 regState4;
    regState4.valAll = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR14_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_STATE4));
    return regState4;
}

//读取当前速度已经发送的脉冲寄存器 STEP_MOTOR14
static uint32_t BoardFPGA_StepMotor14ReadRegCurTabIdxPluseSendOut(void)
{
    uint32_t curTabIdxPluseSendOut;
    curTabIdxPluseSendOut = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR14_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_STATE5));
    return curTabIdxPluseSendOut;
}

//读取速度表元素 STEP_MOTOR14
static ERROR_SUB BoardFPGA_StepMotor14ReadSpdTabElemByIdx(uint16_t index,uint32_t* elemValPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    *elemValPtr = 0;
    //关闭全局中断
    BOARD_FPGA_LOCK();
    //写入序号
    errorSub = BoardFPGA_WriteDataByAddr((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR14_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_SPD_TAB_ELEM_READ),(uint32_t)index);
    if(errorSub != ERROR_SUB_OK)
    {
        //打开全局中断
        BOARD_FPGA_UNLOCK(lastIntState);
        return errorSub;
    }
    //读取元素
    *elemValPtr = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR14_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_SPD_TAB_ELEM));
    //打开全局中断
    BOARD_FPGA_UNLOCK(lastIntState);
    return errorSub;
}

//读取步数表元素 STEP_MOTOR14
static ERROR_SUB BoardFPGA_StepMotor14ReadStpTabElemByIdx(uint16_t index,uint32_t* elemValPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    *elemValPtr = 0;
    //关闭全局中断
    BOARD_FPGA_LOCK();
    //写入序号
    errorSub = BoardFPGA_WriteDataByAddr((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR14_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_STP_TAB_ELEM_READ),(uint32_t)index);
    if(errorSub != ERROR_SUB_OK)
    {
        //打开全局中断
        BOARD_FPGA_UNLOCK(lastIntState);
        return errorSub;
    }
    //读取元素
    *elemValPtr = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR14_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_STP_TAB_ELEM));
    //打开全局中断
    BOARD_FPGA_UNLOCK(lastIntState);
    return errorSub;
}

//写入配置寄存器 STEP_MOTOR14
static ERROR_SUB BoardFPGA_StepMotor14WriteRegConfig(FPGA_STEP_MOTOR_REG_CONFIG regConfig)
{
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR14_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_CONFIG),regConfig.valAll,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR14_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_CONFIG));
}

//写入编码器系数寄存器 STEP_MOTOR14
static ERROR_SUB BoardFPGA_StepMotor14WriteRegEncoderFactor(float factor)
{
    FPGA_STEP_MOTOR_REG_PARAM1 encoderFactor;
    encoderFactor.factorFloat = factor;
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR14_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_PARAM1),encoderFactor.valBits.factorUint32,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR14_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM1));
}

//写入定频速度计数值寄存器 STEP_MOTOR14
static ERROR_SUB BoardFPGA_StepMotor14WriteRegFixSpdCnt(uint32_t fixSpdCnt)
{
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR14_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_PARAM2),fixSpdCnt,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR14_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM2));
}

//写入本次运行总步数寄存器 STEP_MOTOR14
static ERROR_SUB BoardFPGA_StepMotor14WriteRegTotalStepSet(uint32_t totalStep)
{
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR14_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_PARAM3),totalStep,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR14_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM3));
}

//设置步数曲线的起始结束位置 STEP_MOTOR14
static ERROR_SUB BoardFPGA_StepMotor14WriteRegTabSpdIndexRange(FPGA_STEP_MOTOR_REG_PARAM4 tabRange)
{
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR14_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_PARAM4),tabRange.valAll,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR14_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM4));
}

//写入停止条件 STEP_MOTOR14
static ERROR_SUB BoardFPGA_StepMotor14WriteRegStopCondition(FPGA_STEP_MOTOR_REG_PARAM5 stopCondition)
{
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR14_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_PARAM5),stopCondition.valAll,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR14_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM5));
}

//写入控制字 STEP_MOTOR14
static ERROR_SUB BoardFPGA_StepMotor14WriteRegControl(FPGA_STEP_MOTOR_REG_CONTROL regControl)
{
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR14_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_CONTROL),regControl.valAll,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR14_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_CONTROL));
}

//写入速度表 STEP_MOTOR14
static ERROR_SUB BoardFPGA_StepMotor14WriteRegSpdTabElem(uint16_t index,uint32_t val)
{
    return BoardFPGA_WriteDataByAddr((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR14_BASE_ADDR + FPGA_REG_WRITE_ADDR_STEP_MOTOR_SPD_TAB_ELEM_WRITE + index),val);
}

//写入步数表 STEP_MOTOR14
static ERROR_SUB BoardFPGA_StepMotor14WriteRegStpTabElem(uint16_t index,uint32_t val)
{
    return BoardFPGA_WriteDataByAddr((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR14_BASE_ADDR + FPGA_REG_WRITE_ADDR_STEP_MOTOR_STP_TAB_ELEM_WRITE + index),val);
}
#endif
/**********************************STEP_MOTOR15基础操作函数*******************************************/
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 14)
//读取配置寄存器 STEP_MOTOR15
static FPGA_STEP_MOTOR_REG_CONFIG BoardFPGA_StepMotor15ReadRegConfig(void)
{
    FPGA_STEP_MOTOR_REG_CONFIG regConfig;
    regConfig.valAll = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR15_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_CONFIG));
    return regConfig;
}

//读取编码器系数寄存器 STEP_MOTOR15
static float BoardFPGA_StepMotor15ReadRegEncoderFactor(void)
{
    FPGA_STEP_MOTOR_REG_PARAM1 regParam1;
    regParam1.valBits.factorUint32 = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR15_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM1));
    return regParam1.factorFloat;
}

//读取定频运转速度寄存器 STEP_MOTOR15
static uint32_t BoardFPGA_StepMotor15ReadRegFixSpdCnt(void)
{
    uint32_t fixSpdCnt;
    fixSpdCnt = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR15_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM2));
    return fixSpdCnt;
}

//读取本次运行总步数寄存器 STEP_MOTOR15
static uint32_t BoardFPGA_StepMotor15ReadRegTotalStepSet(void)
{
    uint32_t totalStepSet;
    totalStepSet = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR15_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM3));
    return totalStepSet;
}

//读取设置步数曲线的起始结束位置 STEP_MOTOR15
static FPGA_STEP_MOTOR_REG_PARAM4 BoardFPGA_StepMotor15ReadRegTabSpdIndexRange(void)
{
    FPGA_STEP_MOTOR_REG_PARAM4 regParam4;
    regParam4.valAll = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR15_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM4));
    return regParam4;
}

//读取配置的停止条件寄存器 STEP_MOTOR15
static FPGA_STEP_MOTOR_REG_PARAM5 BoardFPGA_StepMotor15ReadRegStopCondition(void)
{
    FPGA_STEP_MOTOR_REG_PARAM5 regParam5;
    regParam5.valAll = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR15_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM5));
    return regParam5;
}

//读取当前控制寄存器 STEP_MOTOR15
static FPGA_STEP_MOTOR_REG_CONTROL BoardFPGA_StepMotor15ReadRegControl(void)
{
    FPGA_STEP_MOTOR_REG_CONTROL regControl;
    regControl.valAll = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR15_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_CONTROL));
    return regControl;
}

//读取已经发出的脉冲寄存器 STEP_MOTOR15
static uint32_t BoardFPGA_StepMotor15ReadRegTotalStepSendOut(void)
{
    uint32_t totalStepSendOut;
    totalStepSendOut = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR15_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_STATE1));
    return totalStepSendOut;
}

//读取当前编码器位置寄存器 STEP_MOTOR15
static int32_t BoardFPGA_StepMotor15ReadRegEncoderPos(void)
{
    FPGA_STEP_MOTOR_REG_STATE2 encoderPos;
    encoderPos.encoderPosUint32 = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR15_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_STATE2));
    return encoderPos.valBits.encoderPosInt32;
}

//读取当前信号状态和电机步数表索引序号寄存器 STEP_MOTOR15
static FPGA_STEP_MOTOR_REG_STATE3 BoardFPGA_StepMotor15ReadRegSensorStateTabIndex(void)
{
    FPGA_STEP_MOTOR_REG_STATE3 regState3;
    regState3.valAll = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR15_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_STATE3));
    return regState3;
}

//读取电机状态寄存器 STEP_MOTOR15
static FPGA_STEP_MOTOR_REG_STATE4 BoardFPGA_StepMotor15ReadRegMotorState(void)
{
    FPGA_STEP_MOTOR_REG_STATE4 regState4;
    regState4.valAll = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR15_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_STATE4));
    return regState4;
}

//读取当前速度已经发送的脉冲寄存器 STEP_MOTOR15
static uint32_t BoardFPGA_StepMotor15ReadRegCurTabIdxPluseSendOut(void)
{
    uint32_t curTabIdxPluseSendOut;
    curTabIdxPluseSendOut = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR15_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_STATE5));
    return curTabIdxPluseSendOut;
}

//读取速度表元素 STEP_MOTOR15
static ERROR_SUB BoardFPGA_StepMotor15ReadSpdTabElemByIdx(uint16_t index,uint32_t* elemValPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    *elemValPtr = 0;
    //关闭全局中断
    BOARD_FPGA_LOCK();
    //写入序号
    errorSub = BoardFPGA_WriteDataByAddr((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR15_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_SPD_TAB_ELEM_READ),(uint32_t)index);
    if(errorSub != ERROR_SUB_OK)
    {
        //打开全局中断
        BOARD_FPGA_UNLOCK(lastIntState);
        return errorSub;
    }
    //读取元素
    *elemValPtr = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR15_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_SPD_TAB_ELEM));
    //打开全局中断
    BOARD_FPGA_UNLOCK(lastIntState);
    return errorSub;
}

//读取步数表元素 STEP_MOTOR15
static ERROR_SUB BoardFPGA_StepMotor15ReadStpTabElemByIdx(uint16_t index,uint32_t* elemValPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    *elemValPtr = 0;
    //关闭全局中断
    BOARD_FPGA_LOCK();
    //写入序号
    errorSub = BoardFPGA_WriteDataByAddr((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR15_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_STP_TAB_ELEM_READ),(uint32_t)index);
    if(errorSub != ERROR_SUB_OK)
    {
        //打开全局中断
        BOARD_FPGA_UNLOCK(lastIntState);
        return errorSub;
    }
    //读取元素
    *elemValPtr = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR15_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_STP_TAB_ELEM));
    //打开全局中断
    BOARD_FPGA_UNLOCK(lastIntState);
    return errorSub;
}

//写入配置寄存器 STEP_MOTOR15
static ERROR_SUB BoardFPGA_StepMotor15WriteRegConfig(FPGA_STEP_MOTOR_REG_CONFIG regConfig)
{
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR15_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_CONFIG),regConfig.valAll,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR15_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_CONFIG));
}

//写入编码器系数寄存器 STEP_MOTOR15
static ERROR_SUB BoardFPGA_StepMotor15WriteRegEncoderFactor(float factor)
{
    FPGA_STEP_MOTOR_REG_PARAM1 encoderFactor;
    encoderFactor.factorFloat = factor;
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR15_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_PARAM1),encoderFactor.valBits.factorUint32,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR15_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM1));
}

//写入定频速度计数值寄存器 STEP_MOTOR15
static ERROR_SUB BoardFPGA_StepMotor15WriteRegFixSpdCnt(uint32_t fixSpdCnt)
{
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR15_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_PARAM2),fixSpdCnt,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR15_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM2));
}

//写入本次运行总步数寄存器 STEP_MOTOR15
static ERROR_SUB BoardFPGA_StepMotor15WriteRegTotalStepSet(uint32_t totalStep)
{
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR15_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_PARAM3),totalStep,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR15_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM3));
}

//设置步数曲线的起始结束位置 STEP_MOTOR15
static ERROR_SUB BoardFPGA_StepMotor15WriteRegTabSpdIndexRange(FPGA_STEP_MOTOR_REG_PARAM4 tabRange)
{
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR15_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_PARAM4),tabRange.valAll,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR15_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM4));
}

//写入停止条件 STEP_MOTOR15
static ERROR_SUB BoardFPGA_StepMotor15WriteRegStopCondition(FPGA_STEP_MOTOR_REG_PARAM5 stopCondition)
{
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR15_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_PARAM5),stopCondition.valAll,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR15_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM5));
}

//写入控制字 STEP_MOTOR15
static ERROR_SUB BoardFPGA_StepMotor15WriteRegControl(FPGA_STEP_MOTOR_REG_CONTROL regControl)
{
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR15_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_CONTROL),regControl.valAll,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR15_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_CONTROL));
}

//写入速度表 STEP_MOTOR15
static ERROR_SUB BoardFPGA_StepMotor15WriteRegSpdTabElem(uint16_t index,uint32_t val)
{
    return BoardFPGA_WriteDataByAddr((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR15_BASE_ADDR + FPGA_REG_WRITE_ADDR_STEP_MOTOR_SPD_TAB_ELEM_WRITE + index),val);
}

//写入步数表 STEP_MOTOR15
static ERROR_SUB BoardFPGA_StepMotor15WriteRegStpTabElem(uint16_t index,uint32_t val)
{
    return BoardFPGA_WriteDataByAddr((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR15_BASE_ADDR + FPGA_REG_WRITE_ADDR_STEP_MOTOR_STP_TAB_ELEM_WRITE + index),val);
}
#endif
/**********************************STEP_MOTOR16基础操作函数*******************************************/
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 15)
//读取配置寄存器 STEP_MOTOR16
static FPGA_STEP_MOTOR_REG_CONFIG BoardFPGA_StepMotor16ReadRegConfig(void)
{
    FPGA_STEP_MOTOR_REG_CONFIG regConfig;
    regConfig.valAll = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR16_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_CONFIG));
    return regConfig;
}

//读取编码器系数寄存器 STEP_MOTOR16
static float BoardFPGA_StepMotor16ReadRegEncoderFactor(void)
{
    FPGA_STEP_MOTOR_REG_PARAM1 regParam1;
    regParam1.valBits.factorUint32 = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR16_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM1));
    return regParam1.factorFloat;
}

//读取定频运转速度寄存器 STEP_MOTOR16
static uint32_t BoardFPGA_StepMotor16ReadRegFixSpdCnt(void)
{
    uint32_t fixSpdCnt;
    fixSpdCnt = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR16_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM2));
    return fixSpdCnt;
}

//读取本次运行总步数寄存器 STEP_MOTOR16
static uint32_t BoardFPGA_StepMotor16ReadRegTotalStepSet(void)
{
    uint32_t totalStepSet;
    totalStepSet = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR16_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM3));
    return totalStepSet;
}

//读取设置步数曲线的起始结束位置 STEP_MOTOR16
static FPGA_STEP_MOTOR_REG_PARAM4 BoardFPGA_StepMotor16ReadRegTabSpdIndexRange(void)
{
    FPGA_STEP_MOTOR_REG_PARAM4 regParam4;
    regParam4.valAll = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR16_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM4));
    return regParam4;
}

//读取配置的停止条件寄存器 STEP_MOTOR16
static FPGA_STEP_MOTOR_REG_PARAM5 BoardFPGA_StepMotor16ReadRegStopCondition(void)
{
    FPGA_STEP_MOTOR_REG_PARAM5 regParam5;
    regParam5.valAll = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR16_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM5));
    return regParam5;
}

//读取当前控制寄存器 STEP_MOTOR16
static FPGA_STEP_MOTOR_REG_CONTROL BoardFPGA_StepMotor16ReadRegControl(void)
{
    FPGA_STEP_MOTOR_REG_CONTROL regControl;
    regControl.valAll = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR16_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_CONTROL));
    return regControl;
}

//读取已经发出的脉冲寄存器 STEP_MOTOR16
static uint32_t BoardFPGA_StepMotor16ReadRegTotalStepSendOut(void)
{
    uint32_t totalStepSendOut;
    totalStepSendOut = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR16_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_STATE1));
    return totalStepSendOut;
}

//读取当前编码器位置寄存器 STEP_MOTOR16
static int32_t BoardFPGA_StepMotor16ReadRegEncoderPos(void)
{
    FPGA_STEP_MOTOR_REG_STATE2 encoderPos;
    encoderPos.encoderPosUint32 = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR16_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_STATE2));
    return encoderPos.valBits.encoderPosInt32;
}

//读取当前信号状态和电机步数表索引序号寄存器 STEP_MOTOR16
static FPGA_STEP_MOTOR_REG_STATE3 BoardFPGA_StepMotor16ReadRegSensorStateTabIndex(void)
{
    FPGA_STEP_MOTOR_REG_STATE3 regState3;
    regState3.valAll = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR16_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_STATE3));
    return regState3;
}

//读取电机状态寄存器 STEP_MOTOR16
static FPGA_STEP_MOTOR_REG_STATE4 BoardFPGA_StepMotor16ReadRegMotorState(void)
{
    FPGA_STEP_MOTOR_REG_STATE4 regState4;
    regState4.valAll = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR16_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_STATE4));
    return regState4;
}

//读取当前速度已经发送的脉冲寄存器 STEP_MOTOR16
static uint32_t BoardFPGA_StepMotor16ReadRegCurTabIdxPluseSendOut(void)
{
    uint32_t curTabIdxPluseSendOut;
    curTabIdxPluseSendOut = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR16_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_STATE5));
    return curTabIdxPluseSendOut;
}

//读取速度表元素 STEP_MOTOR16
static ERROR_SUB BoardFPGA_StepMotor16ReadSpdTabElemByIdx(uint16_t index,uint32_t* elemValPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    *elemValPtr = 0;
    //关闭全局中断
    BOARD_FPGA_LOCK();
    //写入序号
    errorSub = BoardFPGA_WriteDataByAddr((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR16_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_SPD_TAB_ELEM_READ),(uint32_t)index);
    if(errorSub != ERROR_SUB_OK)
    {
        //打开全局中断
        BOARD_FPGA_UNLOCK(lastIntState);
        return errorSub;
    }
    //读取元素
    *elemValPtr = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR16_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_SPD_TAB_ELEM));
    //打开全局中断
    BOARD_FPGA_UNLOCK(lastIntState);
    return errorSub;
}

//读取步数表元素 STEP_MOTOR16
static ERROR_SUB BoardFPGA_StepMotor16ReadStpTabElemByIdx(uint16_t index,uint32_t* elemValPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    *elemValPtr = 0;
    //关闭全局中断
    BOARD_FPGA_LOCK();
    //写入序号
    errorSub = BoardFPGA_WriteDataByAddr((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR16_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_STP_TAB_ELEM_READ),(uint32_t)index);
    if(errorSub != ERROR_SUB_OK)
    {
        //打开全局中断
        BOARD_FPGA_UNLOCK(lastIntState);
        return errorSub;
    }
    //读取元素
    *elemValPtr = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR16_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_STP_TAB_ELEM));
    //打开全局中断
    BOARD_FPGA_UNLOCK(lastIntState);
    return errorSub;
}

//写入配置寄存器 STEP_MOTOR16
static ERROR_SUB BoardFPGA_StepMotor16WriteRegConfig(FPGA_STEP_MOTOR_REG_CONFIG regConfig)
{
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR16_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_CONFIG),regConfig.valAll,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR16_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_CONFIG));
}

//写入编码器系数寄存器 STEP_MOTOR16
static ERROR_SUB BoardFPGA_StepMotor16WriteRegEncoderFactor(float factor)
{
    FPGA_STEP_MOTOR_REG_PARAM1 encoderFactor;
    encoderFactor.factorFloat = factor;
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR16_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_PARAM1),encoderFactor.valBits.factorUint32,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR16_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM1));
}

//写入定频速度计数值寄存器 STEP_MOTOR16
static ERROR_SUB BoardFPGA_StepMotor16WriteRegFixSpdCnt(uint32_t fixSpdCnt)
{
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR16_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_PARAM2),fixSpdCnt,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR16_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM2));
}

//写入本次运行总步数寄存器 STEP_MOTOR16
static ERROR_SUB BoardFPGA_StepMotor16WriteRegTotalStepSet(uint32_t totalStep)
{
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR16_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_PARAM3),totalStep,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR16_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM3));
}

//设置步数曲线的起始结束位置 STEP_MOTOR16
static ERROR_SUB BoardFPGA_StepMotor16WriteRegTabSpdIndexRange(FPGA_STEP_MOTOR_REG_PARAM4 tabRange)
{
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR16_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_PARAM4),tabRange.valAll,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR16_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM4));
}

//写入停止条件 STEP_MOTOR16
static ERROR_SUB BoardFPGA_StepMotor16WriteRegStopCondition(FPGA_STEP_MOTOR_REG_PARAM5 stopCondition)
{
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR16_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_PARAM5),stopCondition.valAll,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR16_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_PARAM5));
}

//写入控制字 STEP_MOTOR16
static ERROR_SUB BoardFPGA_StepMotor16WriteRegControl(FPGA_STEP_MOTOR_REG_CONTROL regControl)
{
    return BoardFPGA_WriteDataByAddrWithCheck((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR16_BASE_ADDR+FPGA_REG_WRITE_ADDR_STEP_MOTOR_CONTROL),regControl.valAll,
                                                        (uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR16_BASE+FPGA_REG_READ_ADDR_STEP_MOTOR_CONTROL));
}

//写入速度表 STEP_MOTOR16
static ERROR_SUB BoardFPGA_StepMotor16WriteRegSpdTabElem(uint16_t index,uint32_t val)
{
    return BoardFPGA_WriteDataByAddr((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR16_BASE_ADDR + FPGA_REG_WRITE_ADDR_STEP_MOTOR_SPD_TAB_ELEM_WRITE + index),val);
}

//写入步数表 STEP_MOTOR16
static ERROR_SUB BoardFPGA_StepMotor16WriteRegStpTabElem(uint16_t index,uint32_t val)
{
    return BoardFPGA_WriteDataByAddr((uint32_t)(FPGA_REG_WRITE_ADDR_STEP_MOTOR16_BASE_ADDR + FPGA_REG_WRITE_ADDR_STEP_MOTOR_STP_TAB_ELEM_WRITE + index),val);
}
#endif
/**********************************STEP_MOTOR函数数组***********************************************/
//读取配置寄存器
static const BoardFPGA_StepMotorReadRegConfigFunc stepMotorReadRegConfigFuncArray[BOARD_FPGA_STEP_MOTOR_NUM_MAX] = {
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 0)
    BoardFPGA_StepMotor1ReadRegConfig,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 1)
    BoardFPGA_StepMotor2ReadRegConfig,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 2)
    BoardFPGA_StepMotor3ReadRegConfig,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 3)
    BoardFPGA_StepMotor4ReadRegConfig,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 4)
    BoardFPGA_StepMotor5ReadRegConfig,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 5)
    BoardFPGA_StepMotor6ReadRegConfig,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 6)
    BoardFPGA_StepMotor7ReadRegConfig,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 7)
    BoardFPGA_StepMotor8ReadRegConfig,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 8)
    BoardFPGA_StepMotor9ReadRegConfig,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 9)
    BoardFPGA_StepMotor10ReadRegConfig,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 10)
    BoardFPGA_StepMotor11ReadRegConfig,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 11)
    BoardFPGA_StepMotor12ReadRegConfig,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 12)
    BoardFPGA_StepMotor13ReadRegConfig,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 13)
    BoardFPGA_StepMotor14ReadRegConfig,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 14)
    BoardFPGA_StepMotor15ReadRegConfig,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 15)
    BoardFPGA_StepMotor16ReadRegConfig,
#endif
};

//读取编码器系数寄存器
static const BoardFPGA_StepMotorReadRegEncoderFactorFunc stepMotorReadRegEncoderFactorFuncArray[BOARD_FPGA_STEP_MOTOR_NUM_MAX] = {
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 0)
    BoardFPGA_StepMotor1ReadRegEncoderFactor,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 1)
    BoardFPGA_StepMotor2ReadRegEncoderFactor,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 2)
    BoardFPGA_StepMotor3ReadRegEncoderFactor,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 3)
    BoardFPGA_StepMotor4ReadRegEncoderFactor,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 4)
    BoardFPGA_StepMotor5ReadRegEncoderFactor,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 5)
    BoardFPGA_StepMotor6ReadRegEncoderFactor,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 6)
    BoardFPGA_StepMotor7ReadRegEncoderFactor,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 7)
    BoardFPGA_StepMotor8ReadRegEncoderFactor,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 8)
    BoardFPGA_StepMotor9ReadRegEncoderFactor,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 9)
    BoardFPGA_StepMotor10ReadRegEncoderFactor,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 10)
    BoardFPGA_StepMotor11ReadRegEncoderFactor,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 11)
    BoardFPGA_StepMotor12ReadRegEncoderFactor,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 12)
    BoardFPGA_StepMotor13ReadRegEncoderFactor,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 13)
    BoardFPGA_StepMotor14ReadRegEncoderFactor,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 14)
    BoardFPGA_StepMotor15ReadRegEncoderFactor,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 15)
    BoardFPGA_StepMotor16ReadRegEncoderFactor,
#endif
};

//读取定频运转速度寄存器
static const BoardFPGA_StepMotorReadRegFixSpdCntFunc stepMotorReadRegFixSpdCntFuncArray[BOARD_FPGA_STEP_MOTOR_NUM_MAX] = {
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 1)
    BoardFPGA_StepMotor1ReadRegFixSpdCnt,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 1)
    BoardFPGA_StepMotor2ReadRegFixSpdCnt,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 2)
    BoardFPGA_StepMotor3ReadRegFixSpdCnt,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 3)
    BoardFPGA_StepMotor4ReadRegFixSpdCnt,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 4)
    BoardFPGA_StepMotor5ReadRegFixSpdCnt,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 5)
    BoardFPGA_StepMotor6ReadRegFixSpdCnt,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 6)
    BoardFPGA_StepMotor7ReadRegFixSpdCnt,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 7)
    BoardFPGA_StepMotor8ReadRegFixSpdCnt,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 8)
    BoardFPGA_StepMotor9ReadRegFixSpdCnt,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 9)
    BoardFPGA_StepMotor10ReadRegFixSpdCnt,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 10)
    BoardFPGA_StepMotor11ReadRegFixSpdCnt,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 11)
    BoardFPGA_StepMotor12ReadRegFixSpdCnt,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 12)
    BoardFPGA_StepMotor13ReadRegFixSpdCnt,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 13)
    BoardFPGA_StepMotor14ReadRegFixSpdCnt,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 14)
    BoardFPGA_StepMotor15ReadRegFixSpdCnt,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 15)
    BoardFPGA_StepMotor16ReadRegFixSpdCnt,
#endif
};

//读取本次运行总步数寄存器
static const BoardFPGA_StepMotorReadRegTotalStepSetFunc stepMotorReadRegTotalStepSetFuncArray[BOARD_FPGA_STEP_MOTOR_NUM_MAX] = {
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 1)
    BoardFPGA_StepMotor1ReadRegTotalStepSet,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 1)
    BoardFPGA_StepMotor2ReadRegTotalStepSet,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 2)
    BoardFPGA_StepMotor3ReadRegTotalStepSet,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 3)
    BoardFPGA_StepMotor4ReadRegTotalStepSet,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 4)
    BoardFPGA_StepMotor5ReadRegTotalStepSet,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 5)
    BoardFPGA_StepMotor6ReadRegTotalStepSet,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 6)
    BoardFPGA_StepMotor7ReadRegTotalStepSet,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 7)
    BoardFPGA_StepMotor8ReadRegTotalStepSet,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 8)
    BoardFPGA_StepMotor9ReadRegTotalStepSet,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 9)
    BoardFPGA_StepMotor10ReadRegTotalStepSet,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 10)
    BoardFPGA_StepMotor11ReadRegTotalStepSet,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 11)
    BoardFPGA_StepMotor12ReadRegTotalStepSet,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 12)
    BoardFPGA_StepMotor13ReadRegTotalStepSet,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 13)
    BoardFPGA_StepMotor14ReadRegTotalStepSet,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 14)
    BoardFPGA_StepMotor15ReadRegTotalStepSet,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 15)
    BoardFPGA_StepMotor16ReadRegTotalStepSet,
#endif
};

//读取设置步数曲线的起始结束位置
static const BoardFPGA_StepMotorReadRegTabSpdIndexRangeFunc stepMotorReadRegTabSpdIndexRangeFuncArray[BOARD_FPGA_STEP_MOTOR_NUM_MAX] = {
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 1)
    BoardFPGA_StepMotor1ReadRegTabSpdIndexRange,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 1)
    BoardFPGA_StepMotor2ReadRegTabSpdIndexRange,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 2)
    BoardFPGA_StepMotor3ReadRegTabSpdIndexRange,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 3)
    BoardFPGA_StepMotor4ReadRegTabSpdIndexRange,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 4)
    BoardFPGA_StepMotor5ReadRegTabSpdIndexRange,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 5)
    BoardFPGA_StepMotor6ReadRegTabSpdIndexRange,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 6)
    BoardFPGA_StepMotor7ReadRegTabSpdIndexRange,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 7)
    BoardFPGA_StepMotor8ReadRegTabSpdIndexRange,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 8)
    BoardFPGA_StepMotor9ReadRegTabSpdIndexRange,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 9)
    BoardFPGA_StepMotor10ReadRegTabSpdIndexRange,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 10)
    BoardFPGA_StepMotor11ReadRegTabSpdIndexRange,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 11)
    BoardFPGA_StepMotor12ReadRegTabSpdIndexRange,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 12)
    BoardFPGA_StepMotor13ReadRegTabSpdIndexRange,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 13)
    BoardFPGA_StepMotor14ReadRegTabSpdIndexRange,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 14)
    BoardFPGA_StepMotor15ReadRegTabSpdIndexRange,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 15)
    BoardFPGA_StepMotor16ReadRegTabSpdIndexRange,
#endif
};

//读取配置的停止条件寄存器
static const BoardFPGA_StepMotorReadRegStopConditionFunc stepMotorReadRegStopConditionFuncArray[BOARD_FPGA_STEP_MOTOR_NUM_MAX] = {
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 1)
    BoardFPGA_StepMotor1ReadRegStopCondition,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 1)
    BoardFPGA_StepMotor2ReadRegStopCondition,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 2)
    BoardFPGA_StepMotor3ReadRegStopCondition,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 3)
    BoardFPGA_StepMotor4ReadRegStopCondition,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 4)
    BoardFPGA_StepMotor5ReadRegStopCondition,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 5)
    BoardFPGA_StepMotor6ReadRegStopCondition,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 6)
    BoardFPGA_StepMotor7ReadRegStopCondition,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 7)
    BoardFPGA_StepMotor8ReadRegStopCondition,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 8)
    BoardFPGA_StepMotor9ReadRegStopCondition,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 9)
    BoardFPGA_StepMotor10ReadRegStopCondition,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 10)
    BoardFPGA_StepMotor11ReadRegStopCondition,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 11)
    BoardFPGA_StepMotor12ReadRegStopCondition,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 12)
    BoardFPGA_StepMotor13ReadRegStopCondition,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 13)
    BoardFPGA_StepMotor14ReadRegStopCondition,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 14)
    BoardFPGA_StepMotor15ReadRegStopCondition,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 15)
    BoardFPGA_StepMotor16ReadRegStopCondition,
#endif
};

//读取当前控制寄存器
static const BoardFPGA_StepMotorReadRegControlFunc stepMotorReadRegControlFuncArray[BOARD_FPGA_STEP_MOTOR_NUM_MAX] = {
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 1)
    BoardFPGA_StepMotor1ReadRegControl,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 1)
    BoardFPGA_StepMotor2ReadRegControl,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 2)
    BoardFPGA_StepMotor3ReadRegControl,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 3)
    BoardFPGA_StepMotor4ReadRegControl,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 4)
    BoardFPGA_StepMotor5ReadRegControl,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 5)
    BoardFPGA_StepMotor6ReadRegControl,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 6)
    BoardFPGA_StepMotor7ReadRegControl,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 7)
    BoardFPGA_StepMotor8ReadRegControl,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 8)
    BoardFPGA_StepMotor9ReadRegControl,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 9)
    BoardFPGA_StepMotor10ReadRegControl,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 10)
    BoardFPGA_StepMotor11ReadRegControl,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 11)
    BoardFPGA_StepMotor12ReadRegControl,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 12)
    BoardFPGA_StepMotor13ReadRegControl,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 13)
    BoardFPGA_StepMotor14ReadRegControl,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 14)
    BoardFPGA_StepMotor15ReadRegControl,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 15)
    BoardFPGA_StepMotor16ReadRegControl,
#endif
};

//读取已经发出的脉冲寄存器
static const BoardFPGA_StepMotorReadRegTotalStepSendOutFunc stepMotorReadRegTotalStepSendOutFuncArray[BOARD_FPGA_STEP_MOTOR_NUM_MAX] = {
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 1)
    BoardFPGA_StepMotor1ReadRegTotalStepSendOut,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 1)
    BoardFPGA_StepMotor2ReadRegTotalStepSendOut,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 2)
    BoardFPGA_StepMotor3ReadRegTotalStepSendOut,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 3)
    BoardFPGA_StepMotor4ReadRegTotalStepSendOut,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 4)
    BoardFPGA_StepMotor5ReadRegTotalStepSendOut,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 5)
    BoardFPGA_StepMotor6ReadRegTotalStepSendOut,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 6)
    BoardFPGA_StepMotor7ReadRegTotalStepSendOut,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 7)
    BoardFPGA_StepMotor8ReadRegTotalStepSendOut,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 8)
    BoardFPGA_StepMotor9ReadRegTotalStepSendOut,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 9)
    BoardFPGA_StepMotor10ReadRegTotalStepSendOut,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 10)
    BoardFPGA_StepMotor11ReadRegTotalStepSendOut,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 11)
    BoardFPGA_StepMotor12ReadRegTotalStepSendOut,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 12)
    BoardFPGA_StepMotor13ReadRegTotalStepSendOut,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 13)
    BoardFPGA_StepMotor14ReadRegTotalStepSendOut,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 14)
    BoardFPGA_StepMotor15ReadRegTotalStepSendOut,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 15)
    BoardFPGA_StepMotor16ReadRegTotalStepSendOut,
#endif
};

//读取当前编码器位置寄存器
static const BoardFPGA_StepMotorReadRegEncoderPosFunc stepMotorReadRegEncoderPosFuncArray[BOARD_FPGA_STEP_MOTOR_NUM_MAX] = {
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 1)
    BoardFPGA_StepMotor1ReadRegEncoderPos,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 1)
    BoardFPGA_StepMotor2ReadRegEncoderPos,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 2)
    BoardFPGA_StepMotor3ReadRegEncoderPos,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 3)
    BoardFPGA_StepMotor4ReadRegEncoderPos,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 4)
    BoardFPGA_StepMotor5ReadRegEncoderPos,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 5)
    BoardFPGA_StepMotor6ReadRegEncoderPos,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 6)
    BoardFPGA_StepMotor7ReadRegEncoderPos,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 7)
    BoardFPGA_StepMotor8ReadRegEncoderPos,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 8)
    BoardFPGA_StepMotor9ReadRegEncoderPos,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 9)
    BoardFPGA_StepMotor10ReadRegEncoderPos,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 10)
    BoardFPGA_StepMotor11ReadRegEncoderPos,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 11)
    BoardFPGA_StepMotor12ReadRegEncoderPos,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 12)
    BoardFPGA_StepMotor13ReadRegEncoderPos,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 13)
    BoardFPGA_StepMotor14ReadRegEncoderPos,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 14)
    BoardFPGA_StepMotor15ReadRegEncoderPos,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 15)
    BoardFPGA_StepMotor16ReadRegEncoderPos,
#endif
};

//读取当前信号状态和电机步数表索引序号寄存器
static const BoardFPGA_StepMotorReadRegSensorStateTabIndexFunc stepMotorReadRegSensorStateTabIndexFuncArray[BOARD_FPGA_STEP_MOTOR_NUM_MAX] = {
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 1)
    BoardFPGA_StepMotor1ReadRegSensorStateTabIndex,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 1)
    BoardFPGA_StepMotor2ReadRegSensorStateTabIndex,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 2)
    BoardFPGA_StepMotor3ReadRegSensorStateTabIndex,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 3)
    BoardFPGA_StepMotor4ReadRegSensorStateTabIndex,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 4)
    BoardFPGA_StepMotor5ReadRegSensorStateTabIndex,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 5)
    BoardFPGA_StepMotor6ReadRegSensorStateTabIndex,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 6)
    BoardFPGA_StepMotor7ReadRegSensorStateTabIndex,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 7)
    BoardFPGA_StepMotor8ReadRegSensorStateTabIndex,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 8)
    BoardFPGA_StepMotor9ReadRegSensorStateTabIndex,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 9)
    BoardFPGA_StepMotor10ReadRegSensorStateTabIndex,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 10)
    BoardFPGA_StepMotor11ReadRegSensorStateTabIndex,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 11)
    BoardFPGA_StepMotor12ReadRegSensorStateTabIndex,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 12)
    BoardFPGA_StepMotor13ReadRegSensorStateTabIndex,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 13)
    BoardFPGA_StepMotor14ReadRegSensorStateTabIndex,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 14)
    BoardFPGA_StepMotor15ReadRegSensorStateTabIndex,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 15)
    BoardFPGA_StepMotor16ReadRegSensorStateTabIndex,
#endif
};

//读取电机状态寄存器
static const BoardFPGA_StepMotorReadRegMotorStateFunc stepMotorReadRegMotorStateFuncArray[BOARD_FPGA_STEP_MOTOR_NUM_MAX] = {
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 1)
    BoardFPGA_StepMotor1ReadRegMotorState,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 1)
    BoardFPGA_StepMotor2ReadRegMotorState,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 2)
    BoardFPGA_StepMotor3ReadRegMotorState,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 3)
    BoardFPGA_StepMotor4ReadRegMotorState,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 4)
    BoardFPGA_StepMotor5ReadRegMotorState,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 5)
    BoardFPGA_StepMotor6ReadRegMotorState,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 6)
    BoardFPGA_StepMotor7ReadRegMotorState,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 7)
    BoardFPGA_StepMotor8ReadRegMotorState,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 8)
    BoardFPGA_StepMotor9ReadRegMotorState,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 9)
    BoardFPGA_StepMotor10ReadRegMotorState,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 10)
    BoardFPGA_StepMotor11ReadRegMotorState,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 11)
    BoardFPGA_StepMotor12ReadRegMotorState,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 12)
    BoardFPGA_StepMotor13ReadRegMotorState,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 13)
    BoardFPGA_StepMotor14ReadRegMotorState,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 14)
    BoardFPGA_StepMotor15ReadRegMotorState,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 15)
    BoardFPGA_StepMotor16ReadRegMotorState,
#endif
};

//读取当前速度已经发送的脉冲寄存器
static const BoardFPGA_StepMotorReadRegCurTabIdxPluseSendOutFunc stepMotorReadRegCurTabIdxPluseSendOutFuncArray[BOARD_FPGA_STEP_MOTOR_NUM_MAX] = {
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 1)
    BoardFPGA_StepMotor1ReadRegCurTabIdxPluseSendOut,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 1)
    BoardFPGA_StepMotor2ReadRegCurTabIdxPluseSendOut,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 2)
    BoardFPGA_StepMotor3ReadRegCurTabIdxPluseSendOut,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 3)
    BoardFPGA_StepMotor4ReadRegCurTabIdxPluseSendOut,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 4)
    BoardFPGA_StepMotor5ReadRegCurTabIdxPluseSendOut,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 5)
    BoardFPGA_StepMotor6ReadRegCurTabIdxPluseSendOut,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 6)
    BoardFPGA_StepMotor7ReadRegCurTabIdxPluseSendOut,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 7)
    BoardFPGA_StepMotor8ReadRegCurTabIdxPluseSendOut,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 8)
    BoardFPGA_StepMotor9ReadRegCurTabIdxPluseSendOut,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 9)
    BoardFPGA_StepMotor10ReadRegCurTabIdxPluseSendOut,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 10)
    BoardFPGA_StepMotor11ReadRegCurTabIdxPluseSendOut,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 11)
    BoardFPGA_StepMotor12ReadRegCurTabIdxPluseSendOut,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 12)
    BoardFPGA_StepMotor13ReadRegCurTabIdxPluseSendOut,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 13)
    BoardFPGA_StepMotor14ReadRegCurTabIdxPluseSendOut,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 14)
    BoardFPGA_StepMotor15ReadRegCurTabIdxPluseSendOut,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 15)
    BoardFPGA_StepMotor16ReadRegCurTabIdxPluseSendOut,
#endif
};

//读取速度表元素
static const BoardFPGA_StepMotorReadSpdTabElemByIdxFunc stepMotorReadSpdTabElemByIdxFuncArray[BOARD_FPGA_STEP_MOTOR_NUM_MAX] = {
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 1)
    BoardFPGA_StepMotor1ReadSpdTabElemByIdx,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 1)
    BoardFPGA_StepMotor2ReadSpdTabElemByIdx,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 2)
    BoardFPGA_StepMotor3ReadSpdTabElemByIdx,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 3)
    BoardFPGA_StepMotor4ReadSpdTabElemByIdx,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 4)
    BoardFPGA_StepMotor5ReadSpdTabElemByIdx,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 5)
    BoardFPGA_StepMotor6ReadSpdTabElemByIdx,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 6)
    BoardFPGA_StepMotor7ReadSpdTabElemByIdx,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 7)
    BoardFPGA_StepMotor8ReadSpdTabElemByIdx,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 8)
    BoardFPGA_StepMotor9ReadSpdTabElemByIdx,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 9)
    BoardFPGA_StepMotor10ReadSpdTabElemByIdx,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 10)
    BoardFPGA_StepMotor11ReadSpdTabElemByIdx,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 11)
    BoardFPGA_StepMotor12ReadSpdTabElemByIdx,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 12)
    BoardFPGA_StepMotor13ReadSpdTabElemByIdx,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 13)
    BoardFPGA_StepMotor14ReadSpdTabElemByIdx,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 14)
    BoardFPGA_StepMotor15ReadSpdTabElemByIdx,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 15)
    BoardFPGA_StepMotor16ReadSpdTabElemByIdx,
#endif
};

//读取步数表元素
static const BoardFPGA_StepMotorReadStpTabElemByIdxFunc stepMotorReadStpTabElemByIdxFuncArray[BOARD_FPGA_STEP_MOTOR_NUM_MAX] = {
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 1)
    BoardFPGA_StepMotor1ReadStpTabElemByIdx,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 1)
    BoardFPGA_StepMotor2ReadStpTabElemByIdx,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 2)
    BoardFPGA_StepMotor3ReadStpTabElemByIdx,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 3)
    BoardFPGA_StepMotor4ReadStpTabElemByIdx,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 4)
    BoardFPGA_StepMotor5ReadStpTabElemByIdx,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 5)
    BoardFPGA_StepMotor6ReadStpTabElemByIdx,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 6)
    BoardFPGA_StepMotor7ReadStpTabElemByIdx,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 7)
    BoardFPGA_StepMotor8ReadStpTabElemByIdx,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 8)
    BoardFPGA_StepMotor9ReadStpTabElemByIdx,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 9)
    BoardFPGA_StepMotor10ReadStpTabElemByIdx,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 10)
    BoardFPGA_StepMotor11ReadStpTabElemByIdx,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 11)
    BoardFPGA_StepMotor12ReadStpTabElemByIdx,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 12)
    BoardFPGA_StepMotor13ReadStpTabElemByIdx,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 13)
    BoardFPGA_StepMotor14ReadStpTabElemByIdx,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 14)
    BoardFPGA_StepMotor15ReadStpTabElemByIdx,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 15)
    BoardFPGA_StepMotor16ReadStpTabElemByIdx,
#endif
};

//写入配置寄存器
static const BoardFPGA_StepMotorWriteRegConfigFunc stepMotorWriteRegConfigFuncArray[BOARD_FPGA_STEP_MOTOR_NUM_MAX] = {
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 1)
    BoardFPGA_StepMotor1WriteRegConfig,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 1)
    BoardFPGA_StepMotor2WriteRegConfig,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 2)
    BoardFPGA_StepMotor3WriteRegConfig,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 3)
    BoardFPGA_StepMotor4WriteRegConfig,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 4)
    BoardFPGA_StepMotor5WriteRegConfig,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 5)
    BoardFPGA_StepMotor6WriteRegConfig,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 6)
    BoardFPGA_StepMotor7WriteRegConfig,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 7)
    BoardFPGA_StepMotor8WriteRegConfig,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 8)
    BoardFPGA_StepMotor9WriteRegConfig,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 9)
    BoardFPGA_StepMotor10WriteRegConfig,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 10)
    BoardFPGA_StepMotor11WriteRegConfig,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 11)
    BoardFPGA_StepMotor12WriteRegConfig,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 12)
    BoardFPGA_StepMotor13WriteRegConfig,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 13)
    BoardFPGA_StepMotor14WriteRegConfig,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 14)
    BoardFPGA_StepMotor15WriteRegConfig,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 15)
    BoardFPGA_StepMotor16WriteRegConfig,
#endif
};

//写入编码器系数寄存器
static const BoardFPGA_StepMotorWriteRegEncoderFactorFunc stepMotorWriteRegEncoderFactorFuncArray[BOARD_FPGA_STEP_MOTOR_NUM_MAX] = {
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 1)
    BoardFPGA_StepMotor1WriteRegEncoderFactor,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 1)
    BoardFPGA_StepMotor2WriteRegEncoderFactor,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 2)
    BoardFPGA_StepMotor3WriteRegEncoderFactor,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 3)
    BoardFPGA_StepMotor4WriteRegEncoderFactor,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 4)
    BoardFPGA_StepMotor5WriteRegEncoderFactor,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 5)
    BoardFPGA_StepMotor6WriteRegEncoderFactor,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 6)
    BoardFPGA_StepMotor7WriteRegEncoderFactor,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 7)
    BoardFPGA_StepMotor8WriteRegEncoderFactor,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 8)
    BoardFPGA_StepMotor9WriteRegEncoderFactor,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 9)
    BoardFPGA_StepMotor10WriteRegEncoderFactor,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 10)
    BoardFPGA_StepMotor11WriteRegEncoderFactor,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 11)
    BoardFPGA_StepMotor12WriteRegEncoderFactor,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 12)
    BoardFPGA_StepMotor13WriteRegEncoderFactor,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 13)
    BoardFPGA_StepMotor14WriteRegEncoderFactor,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 14)
    BoardFPGA_StepMotor15WriteRegEncoderFactor,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 15)
    BoardFPGA_StepMotor16WriteRegEncoderFactor,
#endif
};

//写入定频速度计数值寄存器
static const BoardFPGA_StepMotorWriteRegFixSpdCntFunc stepMotorWriteRegFixSpdCntFuncArray[BOARD_FPGA_STEP_MOTOR_NUM_MAX] = {
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 1)
    BoardFPGA_StepMotor1WriteRegFixSpdCnt,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 1)
    BoardFPGA_StepMotor2WriteRegFixSpdCnt,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 2)
    BoardFPGA_StepMotor3WriteRegFixSpdCnt,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 3)
    BoardFPGA_StepMotor4WriteRegFixSpdCnt,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 4)
    BoardFPGA_StepMotor5WriteRegFixSpdCnt,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 5)
    BoardFPGA_StepMotor6WriteRegFixSpdCnt,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 6)
    BoardFPGA_StepMotor7WriteRegFixSpdCnt,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 7)
    BoardFPGA_StepMotor8WriteRegFixSpdCnt,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 8)
    BoardFPGA_StepMotor9WriteRegFixSpdCnt,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 9)
    BoardFPGA_StepMotor10WriteRegFixSpdCnt,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 10)
    BoardFPGA_StepMotor11WriteRegFixSpdCnt,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 11)
    BoardFPGA_StepMotor12WriteRegFixSpdCnt,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 12)
    BoardFPGA_StepMotor13WriteRegFixSpdCnt,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 13)
    BoardFPGA_StepMotor14WriteRegFixSpdCnt,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 14)
    BoardFPGA_StepMotor15WriteRegFixSpdCnt,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 15)
    BoardFPGA_StepMotor16WriteRegFixSpdCnt,
#endif
};

//写入本次运行总步数寄存器
static const BoardFPGA_StepMotorWriteRegTotalStepSetFunc stepMotorWriteRegTotalStepSetFuncArray[BOARD_FPGA_STEP_MOTOR_NUM_MAX] = {
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 1)
    BoardFPGA_StepMotor1WriteRegTotalStepSet,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 1)
    BoardFPGA_StepMotor2WriteRegTotalStepSet,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 2)
    BoardFPGA_StepMotor3WriteRegTotalStepSet,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 3)
    BoardFPGA_StepMotor4WriteRegTotalStepSet,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 4)
    BoardFPGA_StepMotor5WriteRegTotalStepSet,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 5)
    BoardFPGA_StepMotor6WriteRegTotalStepSet,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 6)
    BoardFPGA_StepMotor7WriteRegTotalStepSet,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 7)
    BoardFPGA_StepMotor8WriteRegTotalStepSet,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 8)
    BoardFPGA_StepMotor9WriteRegTotalStepSet,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 9)
    BoardFPGA_StepMotor10WriteRegTotalStepSet,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 10)
    BoardFPGA_StepMotor11WriteRegTotalStepSet,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 11)
    BoardFPGA_StepMotor12WriteRegTotalStepSet,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 12)
    BoardFPGA_StepMotor13WriteRegTotalStepSet,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 13)
    BoardFPGA_StepMotor14WriteRegTotalStepSet,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 14)
    BoardFPGA_StepMotor15WriteRegTotalStepSet,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 15)
    BoardFPGA_StepMotor16WriteRegTotalStepSet,
#endif
};

//设置步数曲线的起始结束位置
static const BoardFPGA_StepMotorWriteRegTabSpdIndexRangeFunc stepMotorWriteRegTabSpdIndexRangeFuncArray[BOARD_FPGA_STEP_MOTOR_NUM_MAX] = {
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 1)
    BoardFPGA_StepMotor1WriteRegTabSpdIndexRange,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 1)
    BoardFPGA_StepMotor2WriteRegTabSpdIndexRange,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 2)
    BoardFPGA_StepMotor3WriteRegTabSpdIndexRange,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 3)
    BoardFPGA_StepMotor4WriteRegTabSpdIndexRange,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 4)
    BoardFPGA_StepMotor5WriteRegTabSpdIndexRange,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 5)
    BoardFPGA_StepMotor6WriteRegTabSpdIndexRange,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 6)
    BoardFPGA_StepMotor7WriteRegTabSpdIndexRange,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 7)
    BoardFPGA_StepMotor8WriteRegTabSpdIndexRange,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 8)
    BoardFPGA_StepMotor9WriteRegTabSpdIndexRange,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 9)
    BoardFPGA_StepMotor10WriteRegTabSpdIndexRange,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 10)
    BoardFPGA_StepMotor11WriteRegTabSpdIndexRange,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 11)
    BoardFPGA_StepMotor12WriteRegTabSpdIndexRange,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 12)
    BoardFPGA_StepMotor13WriteRegTabSpdIndexRange,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 13)
    BoardFPGA_StepMotor14WriteRegTabSpdIndexRange,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 14)
    BoardFPGA_StepMotor15WriteRegTabSpdIndexRange,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 15)
    BoardFPGA_StepMotor16WriteRegTabSpdIndexRange,
#endif
};

//写入停止条件
static const BoardFPGA_StepMotorWriteRegStopConditionFunc stepMotorWriteRegStopConditionFuncArray[BOARD_FPGA_STEP_MOTOR_NUM_MAX] = {
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 1)
    BoardFPGA_StepMotor1WriteRegStopCondition,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 1)
    BoardFPGA_StepMotor2WriteRegStopCondition,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 2)
    BoardFPGA_StepMotor3WriteRegStopCondition,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 3)
    BoardFPGA_StepMotor4WriteRegStopCondition,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 4)
    BoardFPGA_StepMotor5WriteRegStopCondition,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 5)
    BoardFPGA_StepMotor6WriteRegStopCondition,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 6)
    BoardFPGA_StepMotor7WriteRegStopCondition,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 7)
    BoardFPGA_StepMotor8WriteRegStopCondition,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 8)
    BoardFPGA_StepMotor9WriteRegStopCondition,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 9)
    BoardFPGA_StepMotor10WriteRegStopCondition,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 10)
    BoardFPGA_StepMotor11WriteRegStopCondition,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 11)
    BoardFPGA_StepMotor12WriteRegStopCondition,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 12)
    BoardFPGA_StepMotor13WriteRegStopCondition,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 13)
    BoardFPGA_StepMotor14WriteRegStopCondition,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 14)
    BoardFPGA_StepMotor15WriteRegStopCondition,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 15)
    BoardFPGA_StepMotor16WriteRegStopCondition,
#endif
};

//写入控制字
static const BoardFPGA_StepMotorWriteRegControlFunc stepMotorWriteRegControlFuncArray[BOARD_FPGA_STEP_MOTOR_NUM_MAX] = {
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 1)
    BoardFPGA_StepMotor1WriteRegControl,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 1)
    BoardFPGA_StepMotor2WriteRegControl,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 2)
    BoardFPGA_StepMotor3WriteRegControl,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 3)
    BoardFPGA_StepMotor4WriteRegControl,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 4)
    BoardFPGA_StepMotor5WriteRegControl,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 5)
    BoardFPGA_StepMotor6WriteRegControl,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 6)
    BoardFPGA_StepMotor7WriteRegControl,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 7)
    BoardFPGA_StepMotor8WriteRegControl,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 8)
    BoardFPGA_StepMotor9WriteRegControl,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 9)
    BoardFPGA_StepMotor10WriteRegControl,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 10)
    BoardFPGA_StepMotor11WriteRegControl,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 11)
    BoardFPGA_StepMotor12WriteRegControl,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 12)
    BoardFPGA_StepMotor13WriteRegControl,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 13)
    BoardFPGA_StepMotor14WriteRegControl,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 14)
    BoardFPGA_StepMotor15WriteRegControl,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 15)
    BoardFPGA_StepMotor16WriteRegControl,
#endif
};

//写入速度表
static const BoardFPGA_StepMotorWriteRegSpdTabElemFunc stepMotorWriteRegSpdTabElemFuncArray[BOARD_FPGA_STEP_MOTOR_NUM_MAX] = {
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 1)
    BoardFPGA_StepMotor1WriteRegSpdTabElem,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 1)
    BoardFPGA_StepMotor2WriteRegSpdTabElem,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 2)
    BoardFPGA_StepMotor3WriteRegSpdTabElem,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 3)
    BoardFPGA_StepMotor4WriteRegSpdTabElem,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 4)
    BoardFPGA_StepMotor5WriteRegSpdTabElem,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 5)
    BoardFPGA_StepMotor6WriteRegSpdTabElem,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 6)
    BoardFPGA_StepMotor7WriteRegSpdTabElem,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 7)
    BoardFPGA_StepMotor8WriteRegSpdTabElem,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 8)
    BoardFPGA_StepMotor9WriteRegSpdTabElem,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 9)
    BoardFPGA_StepMotor10WriteRegSpdTabElem,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 10)
    BoardFPGA_StepMotor11WriteRegSpdTabElem,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 11)
    BoardFPGA_StepMotor12WriteRegSpdTabElem,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 12)
    BoardFPGA_StepMotor13WriteRegSpdTabElem,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 13)
    BoardFPGA_StepMotor14WriteRegSpdTabElem,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 14)
    BoardFPGA_StepMotor15WriteRegSpdTabElem,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 15)
    BoardFPGA_StepMotor16WriteRegSpdTabElem,
#endif
};

//写入步数表
static const BoardFPGA_StepMotorWriteRegStpTabElemFunc stepMotorWriteRegStpTabElemFuncArray[BOARD_FPGA_STEP_MOTOR_NUM_MAX] = {
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 1)
    BoardFPGA_StepMotor1WriteRegStpTabElem,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 1)
    BoardFPGA_StepMotor2WriteRegStpTabElem,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 2)
    BoardFPGA_StepMotor3WriteRegStpTabElem,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 3)
    BoardFPGA_StepMotor4WriteRegStpTabElem,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 4)
    BoardFPGA_StepMotor5WriteRegStpTabElem,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 5)
    BoardFPGA_StepMotor6WriteRegStpTabElem,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 6)
    BoardFPGA_StepMotor7WriteRegStpTabElem,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 7)
    BoardFPGA_StepMotor8WriteRegStpTabElem,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 8)
    BoardFPGA_StepMotor9WriteRegStpTabElem,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 9)
    BoardFPGA_StepMotor10WriteRegStpTabElem,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 10)
    BoardFPGA_StepMotor11WriteRegStpTabElem,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 11)
    BoardFPGA_StepMotor12WriteRegStpTabElem,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 12)
    BoardFPGA_StepMotor13WriteRegStpTabElem,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 13)
    BoardFPGA_StepMotor14WriteRegStpTabElem,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 14)
    BoardFPGA_StepMotor15WriteRegStpTabElem,
#endif
#if(BOARD_FPGA_STEP_MOTOR_NUM_MAX > 15)
    BoardFPGA_StepMotor16WriteRegStpTabElem,
#endif
};


/**********************************STEP_MOTOR对外开放API--基础*******************************************/
//读取寄存器
//读取配置寄存器
FPGA_STEP_MOTOR_REG_CONFIG BoardFPGA_StepMotorReadRegConfig(STEP_MOTOR_NO motorNo)
{
    return stepMotorReadRegConfigFuncArray[motorNo]();
}

//读取编码器系数寄存器
float BoardFPGA_StepMotorReadRegEncoderFactor(STEP_MOTOR_NO motorNo)
{
    return stepMotorReadRegEncoderFactorFuncArray[motorNo]();
}

//读取定频运转速度寄存器
uint32_t BoardFPGA_StepMotorReadRegFixSpdCnt(STEP_MOTOR_NO motorNo)
{
    return stepMotorReadRegFixSpdCntFuncArray[motorNo]();
}

//读取本次运行总步数寄存器
uint32_t BoardFPGA_StepMotorReadRegTotalStepSet(STEP_MOTOR_NO motorNo)
{
    return stepMotorReadRegTotalStepSetFuncArray[motorNo]();
}
 
//读取设置步数曲线的起始结束位置
FPGA_STEP_MOTOR_REG_PARAM4 BoardFPGA_StepMotorReadRegTabSpdIndexRange(STEP_MOTOR_NO motorNo)
{
    return stepMotorReadRegTabSpdIndexRangeFuncArray[motorNo]();
}

//读取配置的停止条件寄存器
FPGA_STEP_MOTOR_REG_PARAM5 BoardFPGA_StepMotorReadRegStopCondition(STEP_MOTOR_NO motorNo)
{
    return stepMotorReadRegStopConditionFuncArray[motorNo]();
}
  
//读取当前控制寄存器
FPGA_STEP_MOTOR_REG_CONTROL BoardFPGA_StepMotorReadRegControl(STEP_MOTOR_NO motorNo)
{
    return stepMotorReadRegControlFuncArray[motorNo]();
}

//读取已经发出的脉冲寄存器
uint32_t BoardFPGA_StepMotorReadRegTotalStepSendOut(STEP_MOTOR_NO motorNo)
{
    return stepMotorReadRegTotalStepSendOutFuncArray[motorNo]();
}

//读取当前编码器位置寄存器
int32_t BoardFPGA_StepMotorReadRegEncoderPos(STEP_MOTOR_NO motorNo)
{
    return stepMotorReadRegEncoderPosFuncArray[motorNo]();
}

//读取当前信号状态和电机步数表索引序号寄存器
FPGA_STEP_MOTOR_REG_STATE3 BoardFPGA_StepMotorReadRegSensorStateTabIndex(STEP_MOTOR_NO motorNo)
{
    return stepMotorReadRegSensorStateTabIndexFuncArray[motorNo]();
}
 
//读取电机状态寄存器
FPGA_STEP_MOTOR_REG_STATE4 BoardFPGA_StepMotorReadRegMotorState(STEP_MOTOR_NO motorNo)
{
    return stepMotorReadRegMotorStateFuncArray[motorNo]();
}

//读取当前速度已经发送的脉冲寄存器
uint32_t BoardFPGA_StepMotorReadRegCurTabIdxPluseSendOut(STEP_MOTOR_NO motorNo)
{
    return stepMotorReadRegCurTabIdxPluseSendOutFuncArray[motorNo]();
}

//读取速度表元素
ERROR_SUB BoardFPGA_StepMotorReadSpdTabElemByIdx(STEP_MOTOR_NO motorNo,uint16_t index,uint32_t* elemValPtr)
{
    return stepMotorReadSpdTabElemByIdxFuncArray[motorNo](index,elemValPtr);
}

//读取步数表元素
ERROR_SUB BoardFPGA_StepMotorReadStpTabElemByIdx(STEP_MOTOR_NO motorNo,uint16_t index,uint32_t* elemValPtr)
{
    return stepMotorReadStpTabElemByIdxFuncArray[motorNo](index,elemValPtr);
}


//写入寄存器
//写入配置寄存器
ERROR_SUB BoardFPGA_StepMotorWriteRegConfig(STEP_MOTOR_NO motorNo,FPGA_STEP_MOTOR_REG_CONFIG regConfig)
{
    return stepMotorWriteRegConfigFuncArray[motorNo](regConfig);
}

//写入编码器系数寄存器
ERROR_SUB BoardFPGA_StepMotorWriteRegEncoderFactor(STEP_MOTOR_NO motorNo,float factor)
{
    return stepMotorWriteRegEncoderFactorFuncArray[motorNo](factor);
}

//写入定频速度计数值寄存器
ERROR_SUB BoardFPGA_StepMotorWriteRegFixSpdCnt(STEP_MOTOR_NO motorNo,uint32_t fixSpdCnt)
{
    return stepMotorWriteRegFixSpdCntFuncArray[motorNo](fixSpdCnt);
}

//写入本次运行总步数寄存器
ERROR_SUB BoardFPGA_StepMotorWriteRegTotalStepSet(STEP_MOTOR_NO motorNo,uint32_t totalStep)
{
    return stepMotorWriteRegTotalStepSetFuncArray[motorNo](totalStep);
}

//设置步数曲线的起始结束位置
ERROR_SUB BoardFPGA_StepMotorWriteRegTabSpdIndexRange(STEP_MOTOR_NO motorNo,FPGA_STEP_MOTOR_REG_PARAM4 tabRange)
{
    return stepMotorWriteRegTabSpdIndexRangeFuncArray[motorNo](tabRange);
}

//写入停止条件
ERROR_SUB BoardFPGA_StepMotorWriteRegStopCondition(STEP_MOTOR_NO motorNo,FPGA_STEP_MOTOR_REG_PARAM5 stopCondition)
{
    return stepMotorWriteRegStopConditionFuncArray[motorNo](stopCondition);
}

//写入控制字
ERROR_SUB BoardFPGA_StepMotorWriteRegControl(STEP_MOTOR_NO motorNo,FPGA_STEP_MOTOR_REG_CONTROL regControl)
{
    return stepMotorWriteRegControlFuncArray[motorNo](regControl);
}

//写入速度表
ERROR_SUB BoardFPGA_StepMotorWriteRegSpdTabElem(STEP_MOTOR_NO motorNo,uint16_t index,uint32_t val)
{
    return stepMotorWriteRegSpdTabElemFuncArray[motorNo](index,val);
}

//写入步数表
ERROR_SUB BoardFPGA_StepMotorWriteRegStpTabElem(STEP_MOTOR_NO motorNo,uint16_t index,uint32_t val)
{
    return stepMotorWriteRegStpTabElemFuncArray[motorNo](index,val);
}

/**********************************STEP_MOTOR对外开放API--集成*******************************************/
//获取电机全部寄存器状态
void BoardFPGA_StepMotorGetAllReg(STEP_MOTOR_NO motorNo,FPGA_STEP_MOTOR_REGS* allRegsPtr)
{
    //关闭中断
    BOARD_FPGA_LOCK();
    //可读可写寄存器
    allRegsPtr->writeReadRegs.motorConfig = BoardFPGA_StepMotorReadRegConfig(motorNo);
    allRegsPtr->writeReadRegs.encoderFactor = BoardFPGA_StepMotorReadRegEncoderFactor(motorNo);
    allRegsPtr->writeReadRegs.fixSpdClkCnt = BoardFPGA_StepMotorReadRegFixSpdCnt(motorNo);
    allRegsPtr->writeReadRegs.totalPluseSet = BoardFPGA_StepMotorReadRegTotalStepSet(motorNo);
    allRegsPtr->writeReadRegs.spdTableIndex = BoardFPGA_StepMotorReadRegTabSpdIndexRange(motorNo);
    allRegsPtr->writeReadRegs.stopConditionSet = BoardFPGA_StepMotorReadRegStopCondition(motorNo);
    allRegsPtr->writeReadRegs.controlSet = BoardFPGA_StepMotorReadRegControl(motorNo);
    //只读寄存器
    allRegsPtr->readOnlyRegs.motorPluseSendOutAll = BoardFPGA_StepMotorReadRegTotalStepSendOut(motorNo);
    allRegsPtr->readOnlyRegs.encoderCurrentPos = BoardFPGA_StepMotorReadRegEncoderPos(motorNo);
    allRegsPtr->readOnlyRegs.sensorStateAndTabIndexCurrent = BoardFPGA_StepMotorReadRegSensorStateTabIndex(motorNo);
    allRegsPtr->readOnlyRegs.motorStateAndStopReason = BoardFPGA_StepMotorReadRegMotorState(motorNo);
    allRegsPtr->readOnlyRegs.motorPluseSendOutCurrentIndex = BoardFPGA_StepMotorReadRegCurTabIdxPluseSendOut(motorNo);
    //打开全局中断
    BOARD_FPGA_UNLOCK(lastIntState);
}

//获取电机状态寄存器
void BoardFPGA_StepMotorGetReadOnlyReg(STEP_MOTOR_NO motorNo,FPGA_STEP_MOTOR_REG_READONLY* readonlyRegsPtr)
{
    //关闭中断
    BOARD_FPGA_LOCK();
    //只读寄存器
    readonlyRegsPtr->motorPluseSendOutAll = BoardFPGA_StepMotorReadRegTotalStepSendOut(motorNo);
    readonlyRegsPtr->encoderCurrentPos = BoardFPGA_StepMotorReadRegEncoderPos(motorNo);
    readonlyRegsPtr->sensorStateAndTabIndexCurrent = BoardFPGA_StepMotorReadRegSensorStateTabIndex(motorNo);
    readonlyRegsPtr->motorStateAndStopReason = BoardFPGA_StepMotorReadRegMotorState(motorNo);
    readonlyRegsPtr->motorPluseSendOutCurrentIndex = BoardFPGA_StepMotorReadRegCurTabIdxPluseSendOut(motorNo);
    //打开全局中断
    BOARD_FPGA_UNLOCK(lastIntState);
}

//获取电机可读可写寄存器状态
void BoardFPGA_StepMotorGetWriteReadReg(STEP_MOTOR_NO motorNo,FPGA_STEP_MOTOR_REG_WRITE_READ* writeReadRegsPtr)
{
    //关闭中断
    BOARD_FPGA_LOCK();
    //可读可写寄存器
    writeReadRegsPtr->motorConfig = BoardFPGA_StepMotorReadRegConfig(motorNo);
    writeReadRegsPtr->encoderFactor = BoardFPGA_StepMotorReadRegEncoderFactor(motorNo);
    writeReadRegsPtr->fixSpdClkCnt = BoardFPGA_StepMotorReadRegFixSpdCnt(motorNo);
    writeReadRegsPtr->totalPluseSet = BoardFPGA_StepMotorReadRegTotalStepSet(motorNo);
    writeReadRegsPtr->spdTableIndex = BoardFPGA_StepMotorReadRegTabSpdIndexRange(motorNo);
    writeReadRegsPtr->stopConditionSet = BoardFPGA_StepMotorReadRegStopCondition(motorNo);
    writeReadRegsPtr->controlSet = BoardFPGA_StepMotorReadRegControl(motorNo);
    //打开全局中断
    BOARD_FPGA_UNLOCK(lastIntState);
}

//读取电机中断状态
STEP_MOTOR_INT BoardFPGA_StepMotorGetIntSrc(void)
{
    //关闭中断
    STEP_MOTOR_INT motorInt;
    motorInt.valAll = BoardFPGA_ReadReg32((uint16_t)(FPGA_REG_READ_ADDR_STEP_MOTOR_INT));
    return motorInt;
}

//步进电机停止
ERROR_SUB BoardFPGA_StepMotorCmdStop(STEP_MOTOR_NO motorNo,FPGA_STEP_MOTOR_REG_STATE4* motorStatePtrAfterStop)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //关闭中断
    BOARD_FPGA_LOCK();
    //读取电机状态
    *motorStatePtrAfterStop = BoardFPGA_StepMotorReadRegMotorState(motorNo);
    //检查电机状态
    if(motorStatePtrAfterStop->valBits.motorState == STEP_MOTOR_STATE_IDLE)
    {
        //打开全局中断
        BOARD_FPGA_UNLOCK(lastIntState);
        //空闲状态,本身就停止了
        return ERROR_SUB_OK;
    }
    if(motorStatePtrAfterStop->valBits.motorState == STEP_MOTOR_STATE_STOP_CMD)
    {
        //打开全局中断
        BOARD_FPGA_UNLOCK(lastIntState);
        //指令停止状态,本身就停止了
        return ERROR_SUB_OK;
    }
    //其余状态,电机正在运转,发送指令,电机停止,首先读取控制寄存器
    FPGA_STEP_MOTOR_REG_CONTROL controlReg = BoardFPGA_StepMotorReadRegControl(motorNo);
    //设置控制寄存器的值,清空编码器和清空步数表不能触发
    controlReg.valBits.encoderPosClearSet = DISABLE;
    //发送急停指令
    controlReg.valBits.motorCmd = STEP_MOTOR_CMD_STOP;
    //发送控制寄存器
    errorSub = BoardFPGA_StepMotorWriteRegControl(motorNo,controlReg);
    if(errorSub != ERROR_SUB_OK)
    {
        //打开全局中断
        BOARD_FPGA_UNLOCK(lastIntState);
        MCU_LogShowErrorSubCode(errorSub);
        return errorSub;
    }
    //状态必须切换
    *motorStatePtrAfterStop = BoardFPGA_StepMotorReadRegMotorState(motorNo);
    if(motorStatePtrAfterStop->valBits.motorState != STEP_MOTOR_STATE_STOP_CMD)
    {
        //打开全局中断
        BOARD_FPGA_UNLOCK(lastIntState);
        MCU_LogShowErrorSubCode(ERROR_SUB_BOARD_FPGA_STEP_MOTOR_CMD_STOP_FAIL);
        //步进电机指令急停失败
        return ERROR_SUB_BOARD_FPGA_STEP_MOTOR_CMD_STOP_FAIL;
    }
    //打开全局中断
    BOARD_FPGA_UNLOCK(lastIntState);
    //急停完成
    return ERROR_SUB_OK;
}

//步进电机进入空闲状态
ERROR_SUB BoardFPGA_StepMotorConvertToIdle(STEP_MOTOR_NO motorNo)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    FPGA_STEP_MOTOR_REG_STATE4 motorState;
    //关闭中断
    BOARD_FPGA_LOCK();
    //首先电机急停
    errorSub = BoardFPGA_StepMotorCmdStop(motorNo,&motorState);
    if(errorSub != ERROR_SUB_OK)
    {
        //打开全局中断
        BOARD_FPGA_UNLOCK(lastIntState);
        MCU_LogShowErrorSubCode(errorSub);
        return errorSub;
    }
    //然后查看电机状态看看是否需要进入空闲
    if(motorState.valBits.motorState == STEP_MOTOR_STATE_IDLE)
    {
        //打开全局中断
        BOARD_FPGA_UNLOCK(lastIntState);
        return ERROR_SUB_OK;
    }
    //需要进入空闲
    FPGA_STEP_MOTOR_REG_CONTROL controlReg = BoardFPGA_StepMotorReadRegControl(motorNo);
    //设置控制寄存器的值,清空编码器和清空步数表不能触发
    controlReg.valBits.encoderPosClearSet = DISABLE;
    //发送进入空闲指令
    controlReg.valBits.motorCmd = STEP_MOTOR_CMD_IDLE;
    //发送控制寄存器
    errorSub = BoardFPGA_StepMotorWriteRegControl(motorNo,controlReg);
    if(errorSub != ERROR_SUB_OK)
    {
        //打开全局中断
        BOARD_FPGA_UNLOCK(lastIntState);
        MCU_LogShowErrorSubCode(errorSub);
        return errorSub;
    }
    motorState = BoardFPGA_StepMotorReadRegMotorState(motorNo);
    if(motorState.valBits.motorState != STEP_MOTOR_STATE_IDLE)
    {
        //打开全局中断
        BOARD_FPGA_UNLOCK(lastIntState);
        MCU_LogShowErrorSubCode(ERROR_SUB_BOARD_FPGA_STEP_MOTOR_CMD_IDLE);
        //步进电机指令急停失败
        return ERROR_SUB_BOARD_FPGA_STEP_MOTOR_CMD_IDLE;
    }
    //打开全局中断
    BOARD_FPGA_UNLOCK(lastIntState);
    //空闲
    return ERROR_SUB_OK;
}

//写入电机配置
ERROR_SUB BoardFPGA_StepMotorWriteMotorAttri(STEP_MOTOR_NO motorNo,FPGA_MOTOR_ATTRI* attriPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //关闭中断
    BOARD_FPGA_LOCK();
    //写配置之前,电机必须空闲
    errorSub = BoardFPGA_StepMotorConvertToIdle(motorNo);
    if(errorSub != ERROR_SUB_OK)
    {
        //打开全局中断
        BOARD_FPGA_UNLOCK(lastIntState);
        MCU_LogShowErrorSubCode(errorSub);
        return errorSub;
    }
    //先写配置寄存器
    errorSub = BoardFPGA_StepMotorWriteRegConfig(motorNo,attriPtr->configReg);
    if(errorSub != ERROR_SUB_OK)
    {
        //打开全局中断
        BOARD_FPGA_UNLOCK(lastIntState);
        MCU_LogShowErrorSubCode(errorSub);
        return errorSub;
    }
    //再写编码器系数
    errorSub = BoardFPGA_StepMotorWriteRegEncoderFactor(motorNo,attriPtr->encoderFactor);
    if(errorSub != ERROR_SUB_OK)
    {
        //打开全局中断
        BOARD_FPGA_UNLOCK(lastIntState);
        MCU_LogShowErrorSubCode(errorSub);
        return errorSub;
    }
    //打开全局中断
    BOARD_FPGA_UNLOCK(lastIntState);
    //空闲
    return ERROR_SUB_OK;
}

//清空电机速度步数表数据
ERROR_SUB BoardFPGA_StepMotorClearSpdStpTab(STEP_MOTOR_NO motorNo)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    uint16_t indexUtil = 0;
    //关闭中断
    BOARD_FPGA_LOCK();
    for(indexUtil = 0; indexUtil < BOARD_FPGA_STEP_MOTOR_TAB_LENGTH; indexUtil++)
    {
        //速度表元素清空
        errorSub = BoardFPGA_StepMotorWriteRegSpdTabElem(motorNo,indexUtil,0);
        if(errorSub != ERROR_SUB_OK)
        {
            //打开全局中断
            BOARD_FPGA_UNLOCK(lastIntState);
            MCU_LogShowErrorSubCode(errorSub);
            return errorSub;
        }
        //步数表元素清空
        errorSub = BoardFPGA_StepMotorWriteRegStpTabElem(motorNo,indexUtil,0);
        if(errorSub != ERROR_SUB_OK)
        {
            //打开全局中断
            BOARD_FPGA_UNLOCK(lastIntState);
            MCU_LogShowErrorSubCode(errorSub);
            return errorSub;
        }
    }
    //写入完成,写入表序号
    FPGA_STEP_MOTOR_REG_PARAM4 motorRegParam4;
    motorRegParam4.valBits.tabSpdStartIdx = 0;
    motorRegParam4.valBits.tabSpdStopIdx = 0;
    errorSub = BoardFPGA_StepMotorWriteRegTabSpdIndexRange(motorNo,motorRegParam4);
    if(errorSub != ERROR_SUB_OK)
    {
        //打开全局中断
        BOARD_FPGA_UNLOCK(lastIntState);
        MCU_LogShowErrorSubCode(errorSub);
        return errorSub;
    }
    //写入总步数为0
    errorSub = BoardFPGA_StepMotorWriteRegTotalStepSet(motorNo,0);
    if(errorSub != ERROR_SUB_OK)
    {
        //打开全局中断
        BOARD_FPGA_UNLOCK(lastIntState);
        MCU_LogShowErrorSubCode(errorSub);
        return errorSub;
    }
    //打开全局中断
    BOARD_FPGA_UNLOCK(lastIntState);
    return ERROR_SUB_OK;
}

//电机使能
ERROR_SUB BoardFPGA_StepMotorEnable(STEP_MOTOR_NO motorNo)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //关闭中断
    BOARD_FPGA_LOCK();
    //读取控制寄存器
    FPGA_STEP_MOTOR_REG_CONTROL controlReg = BoardFPGA_StepMotorReadRegControl(motorNo);
    //检查电机是否已经使能
    if(controlReg.valBits.motorWorkStateSet == ENABLE)
    {
        //已经使能无需操作
        //打开全局中断
        BOARD_FPGA_UNLOCK(lastIntState);
        return ERROR_SUB_OK;
    }
    //写配置之前,电机必须空闲
    errorSub = BoardFPGA_StepMotorConvertToIdle(motorNo);
    if(errorSub != ERROR_SUB_OK)
    {
        //打开全局中断
        BOARD_FPGA_UNLOCK(lastIntState);
        MCU_LogShowErrorSubCode(errorSub);
        return errorSub;
    }
    //设置控制寄存器的值,清空编码器和清空步数表不能触发
    controlReg.valBits.encoderPosClearSet = DISABLE;
    //指令为空
    controlReg.valBits.motorCmd = STEP_MOTOR_CMD_NONE;
    //写入电机使能
    controlReg.valBits.motorWorkStateSet = ENABLE;
    //写入控制信号
    errorSub = BoardFPGA_StepMotorWriteRegControl(motorNo,controlReg);
    if(errorSub != ERROR_SUB_OK)
    {
        //打开全局中断
        BOARD_FPGA_UNLOCK(lastIntState);
        MCU_LogShowErrorSubCode(errorSub);
        return errorSub;
    }
    //打开全局中断
    BOARD_FPGA_UNLOCK(lastIntState);
    return ERROR_SUB_OK;
}

//电机禁止
ERROR_SUB BoardFPGA_StepMotorDisable(STEP_MOTOR_NO motorNo)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //关闭中断
    BOARD_FPGA_LOCK();
    //读取控制寄存器
    FPGA_STEP_MOTOR_REG_CONTROL controlReg = BoardFPGA_StepMotorReadRegControl(motorNo);
    //检查电机是否已经禁止
    if(controlReg.valBits.motorWorkStateSet == DISABLE)
    {
        //已经禁止无需操作
        //打开全局中断
        BOARD_FPGA_UNLOCK(lastIntState);
        return ERROR_SUB_OK;
    }
    //写配置之前,电机必须空闲
    errorSub = BoardFPGA_StepMotorConvertToIdle(motorNo);
    if(errorSub != ERROR_SUB_OK)
    {
        //打开全局中断
        BOARD_FPGA_UNLOCK(lastIntState);
        MCU_LogShowErrorSubCode(errorSub);
        return errorSub;
    }
    //设置控制寄存器的值,清空编码器和清空步数表不能触发
    controlReg.valBits.encoderPosClearSet = DISABLE;
    //指令为空
    controlReg.valBits.motorCmd = STEP_MOTOR_CMD_NONE;
    //写入电机禁止
    controlReg.valBits.motorWorkStateSet = DISABLE;
    //写入控制信号
    errorSub = BoardFPGA_StepMotorWriteRegControl(motorNo,controlReg);
    if(errorSub != ERROR_SUB_OK)
    {
        //打开全局中断
        BOARD_FPGA_UNLOCK(lastIntState);
        MCU_LogShowErrorSubCode(errorSub);
        return errorSub;
    }
    //打开全局中断
    BOARD_FPGA_UNLOCK(lastIntState);
    return ERROR_SUB_OK;
}

//编码器使能
ERROR_SUB BoardFPGA_StepMotorEncoderEnable(STEP_MOTOR_NO motorNo)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //关闭中断
    BOARD_FPGA_LOCK();
    //读取控制寄存器
    FPGA_STEP_MOTOR_REG_CONTROL controlReg = BoardFPGA_StepMotorReadRegControl(motorNo);
    //检查编码器是否已经使能
    if(controlReg.valBits.encoderWorkStateSet == ENABLE)
    {
        //已经使能无需操作
        //打开全局中断
        BOARD_FPGA_UNLOCK(lastIntState);
        return ERROR_SUB_OK;
    }
    //写配置之前,电机必须空闲
    errorSub = BoardFPGA_StepMotorConvertToIdle(motorNo);
    if(errorSub != ERROR_SUB_OK)
    {
        //打开全局中断
        BOARD_FPGA_UNLOCK(lastIntState);
        MCU_LogShowErrorSubCode(errorSub);
        return errorSub;
    }
    //设置控制寄存器的值,清空编码器和清空步数表不能触发
    controlReg.valBits.encoderPosClearSet = DISABLE;
    //指令为空
    controlReg.valBits.motorCmd = STEP_MOTOR_CMD_NONE;
    //写入编码器使能
    controlReg.valBits.encoderWorkStateSet = ENABLE;
    //写入控制信号
    errorSub = BoardFPGA_StepMotorWriteRegControl(motorNo,controlReg);
    if(errorSub != ERROR_SUB_OK)
    {
        //打开全局中断
        BOARD_FPGA_UNLOCK(lastIntState);
        MCU_LogShowErrorSubCode(errorSub);
        return errorSub;
    }
    //打开全局中断
    BOARD_FPGA_UNLOCK(lastIntState);
    return ERROR_SUB_OK;
}

//编码器禁止
ERROR_SUB BoardFPGA_StepMotorEncoderDisable(STEP_MOTOR_NO motorNo)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //关闭中断
    BOARD_FPGA_LOCK();
    //读取控制寄存器
    FPGA_STEP_MOTOR_REG_CONTROL controlReg = BoardFPGA_StepMotorReadRegControl(motorNo);
    //检查编码器是否已经禁止
    if(controlReg.valBits.encoderWorkStateSet == DISABLE)
    {
        //已经禁止无需操作
        //打开全局中断
        BOARD_FPGA_UNLOCK(lastIntState);
        return ERROR_SUB_OK;
    }
    //写配置之前,电机必须空闲
    errorSub = BoardFPGA_StepMotorConvertToIdle(motorNo);
    if(errorSub != ERROR_SUB_OK)
    {
        //打开全局中断
        BOARD_FPGA_UNLOCK(lastIntState);
        MCU_LogShowErrorSubCode(errorSub);
        return errorSub;
    }
    //设置控制寄存器的值,清空编码器和清空步数表不能触发
    controlReg.valBits.encoderPosClearSet = DISABLE;
    //指令为空
    controlReg.valBits.motorCmd = STEP_MOTOR_CMD_NONE;
    //写入编码器禁止
    controlReg.valBits.encoderWorkStateSet = DISABLE;
    //写入控制信号
    errorSub = BoardFPGA_StepMotorWriteRegControl(motorNo,controlReg);
    if(errorSub != ERROR_SUB_OK)
    {
        //打开全局中断
        BOARD_FPGA_UNLOCK(lastIntState);
        MCU_LogShowErrorSubCode(errorSub);
        return errorSub;
    }
    //打开全局中断
    BOARD_FPGA_UNLOCK(lastIntState);
    return ERROR_SUB_OK;
}

//写入编码器和电机控制状态
ERROR_SUB BoardFPGA_StepMotorAndEncoderState(STEP_MOTOR_NO motorNo,FunctionalState motorStateSet,FunctionalState encoderStateSet)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //关闭中断
    BOARD_FPGA_LOCK();
    //读取控制寄存器
    FPGA_STEP_MOTOR_REG_CONTROL controlReg = BoardFPGA_StepMotorReadRegControl(motorNo);
    //检查当前状态和设置状态是否相同
    if((controlReg.valBits.encoderWorkStateSet == encoderStateSet)&&(controlReg.valBits.motorWorkStateSet == motorStateSet))
    {
        //已经一致无需操作
        //打开全局中断
        BOARD_FPGA_UNLOCK(lastIntState);
        return ERROR_SUB_OK;
    }
    //写配置之前,电机必须空闲
    errorSub = BoardFPGA_StepMotorConvertToIdle(motorNo);
    if(errorSub != ERROR_SUB_OK)
    {
        //打开全局中断
        BOARD_FPGA_UNLOCK(lastIntState);
        MCU_LogShowErrorSubCode(errorSub);
        return errorSub;
    }
    //设置控制寄存器的值,清空编码器不能触发
    controlReg.valBits.encoderPosClearSet = DISABLE;
    //指令为空
    controlReg.valBits.motorCmd = STEP_MOTOR_CMD_NONE;
    //写入设置
    controlReg.valBits.motorWorkStateSet = motorStateSet;
    controlReg.valBits.encoderWorkStateSet = encoderStateSet;
    //写入控制信号
    errorSub = BoardFPGA_StepMotorWriteRegControl(motorNo,controlReg);
    if(errorSub != ERROR_SUB_OK)
    {
        //打开全局中断
        BOARD_FPGA_UNLOCK(lastIntState);
        MCU_LogShowErrorSubCode(errorSub);
        return errorSub;
    }
    //打开全局中断
    BOARD_FPGA_UNLOCK(lastIntState);
    return ERROR_SUB_OK;
}

//编码器计数器清空
ERROR_SUB BoardFPGA_StepMotorEncoderPosClear(STEP_MOTOR_NO motorNo)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //关闭中断
    BOARD_FPGA_LOCK();
    //读取控制寄存器
    FPGA_STEP_MOTOR_REG_CONTROL controlReg = BoardFPGA_StepMotorReadRegControl(motorNo);
    //写入之前电机必须空闲
    errorSub = BoardFPGA_StepMotorConvertToIdle(motorNo);
    if(errorSub != ERROR_SUB_OK)
    {
        //打开全局中断
        BOARD_FPGA_UNLOCK(lastIntState);
        MCU_LogShowErrorSubCode(errorSub);
        return errorSub;
    }
    //先设置清空编码器为0
    controlReg.valBits.encoderPosClearSet = DISABLE;
    //指令为空
    controlReg.valBits.motorCmd = STEP_MOTOR_CMD_NONE;
    //写入控制信号
    errorSub = BoardFPGA_StepMotorWriteRegControl(motorNo,controlReg);
    if(errorSub != ERROR_SUB_OK)
    {
        //打开全局中断
        BOARD_FPGA_UNLOCK(lastIntState);
        MCU_LogShowErrorSubCode(errorSub);
        return errorSub;
    }
    //再设置清空编码器
    controlReg.valBits.encoderPosClearSet = ENABLE;
    //指令为空
    controlReg.valBits.motorCmd = STEP_MOTOR_CMD_NONE;
    //写入控制信号
    errorSub = BoardFPGA_StepMotorWriteRegControl(motorNo,controlReg);
    if(errorSub != ERROR_SUB_OK)
    {
        //打开全局中断
        BOARD_FPGA_UNLOCK(lastIntState);
        MCU_LogShowErrorSubCode(errorSub);
        return errorSub;
    }
    //打开全局中断
    BOARD_FPGA_UNLOCK(lastIntState);
    return ERROR_SUB_OK;
}


//步进电机定频运转
ERROR_SUB BoardFPGA_StepMotorFixSpdRunStart(STEP_MOTOR_NO motorNo,FPGA_MOTOR_FIX_SPD_CMD* cmd)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //关闭中断
    BOARD_FPGA_LOCK();
    //写入之前电机必须空闲
    errorSub = BoardFPGA_StepMotorConvertToIdle(motorNo);
    if(errorSub != ERROR_SUB_OK)
    {
        //打开全局中断
        BOARD_FPGA_UNLOCK(lastIntState);
        MCU_LogShowErrorSubCode(errorSub);
        return errorSub;
    }
    //设置定频运转参数
    uint32_t fixSpdCnt = BOARD_FPGA_RPM_TO_CNT(cmd->rpmSet,cmd->motorDiv);
    //写入定频运转速度
    errorSub = BoardFPGA_StepMotorWriteRegFixSpdCnt(motorNo,fixSpdCnt);
    if(errorSub != ERROR_SUB_OK)
    {
        //打开全局中断
        BOARD_FPGA_UNLOCK(lastIntState);
        MCU_LogShowErrorSubCode(errorSub);
        return errorSub;
    }
    //写入总步数
    errorSub = BoardFPGA_StepMotorWriteRegTotalStepSet(motorNo,cmd->stepCount);
    if(errorSub != ERROR_SUB_OK)
    {
        //打开全局中断
        BOARD_FPGA_UNLOCK(lastIntState);
        MCU_LogShowErrorSubCode(errorSub);
        return errorSub;
    }
    //是否清除编码器坐标
    if(cmd->clearEncoderPosSet == ENABLE)
    {
        errorSub = BoardFPGA_StepMotorEncoderPosClear(motorNo);
        if(errorSub != ERROR_SUB_OK)
        {
            //打开全局中断
            BOARD_FPGA_UNLOCK(lastIntState);
            MCU_LogShowErrorSubCode(errorSub);
            return errorSub;
        }
    }
    //写入停止条件
    errorSub = BoardFPGA_StepMotorWriteRegStopCondition(motorNo,cmd->stopCondition);
    if(errorSub != ERROR_SUB_OK)
    {
        //打开全局中断
        BOARD_FPGA_UNLOCK(lastIntState);
        MCU_LogShowErrorSubCode(errorSub);
        return errorSub;
    }
    //指令启动
    //读取控制寄存器
    FPGA_STEP_MOTOR_REG_CONTROL controlReg = BoardFPGA_StepMotorReadRegControl(motorNo);
    //先设置清空编码器为0
    controlReg.valBits.encoderPosClearSet = DISABLE;
    controlReg.valBits.spdStpTablePreWriteSet = DISABLE;
    //指令运转,决定方向
    if(cmd->motorDir == STEP_MOTOR_DIR_CW)
    {
        controlReg.valBits.motorCmd = STEP_MOTOR_CMD_FIX_CW;
    }
    else
    {
        controlReg.valBits.motorCmd = STEP_MOTOR_CMD_FIX_CCW;
    }
    //写入控制信号
    errorSub = BoardFPGA_StepMotorWriteRegControl(motorNo,controlReg);
    if(errorSub != ERROR_SUB_OK)
    {
        //打开全局中断
        BOARD_FPGA_UNLOCK(lastIntState);
        MCU_LogShowErrorSubCode(errorSub);
        return errorSub;
    }
    //读取状态,不能是空闲
    FPGA_STEP_MOTOR_REG_STATE4 motorState = BoardFPGA_StepMotorReadRegMotorState(motorNo);
    if(motorState.valBits.motorState == STEP_MOTOR_STATE_IDLE)
    {
        //打开全局中断
        BOARD_FPGA_UNLOCK(lastIntState);
        MCU_LogShowErrorSubCode(ERROR_SUB_BOARD_FPGA_STEP_MOTOR_CMD_FIX_SPD_START_FAIL);
        return ERROR_SUB_BOARD_FPGA_STEP_MOTOR_CMD_FIX_SPD_START_FAIL;
    }
    //打开全局中断
    BOARD_FPGA_UNLOCK(lastIntState);
    return ERROR_SUB_OK;
}

//步进电机步数表运转
ERROR_SUB BoardFPGA_StepMotorTabSpdRunStart(STEP_MOTOR_NO motorNo,FPGA_MOTOR_TAB_SPD_CMD* cmd,FunctionalState writePreSet)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    uint32_t readCheckVal = 0;
    //关闭中断
    BOARD_FPGA_LOCK();
    //写入之前电机必须空闲
    errorSub = BoardFPGA_StepMotorConvertToIdle(motorNo);
    if(errorSub != ERROR_SUB_OK)
    {
        //打开全局中断
        BOARD_FPGA_UNLOCK(lastIntState);
        MCU_LogShowErrorSubCode(errorSub);
        return errorSub;
    }
    //写入起始结束序号
    FPGA_STEP_MOTOR_REG_PARAM4 tabSpdIdxRange;
    tabSpdIdxRange.valBits.tabSpdStartIdx = 0;
    tabSpdIdxRange.valBits.tabSpdStopIdx = cmd->tableLength -1;
    errorSub = BoardFPGA_StepMotorWriteRegTabSpdIndexRange(motorNo,tabSpdIdxRange);
    if(errorSub != ERROR_SUB_OK)
    {
        //打开全局中断
        BOARD_FPGA_UNLOCK(lastIntState);
        MCU_LogShowErrorSubCode(errorSub);
        return errorSub;
    }
    //写入总运行步数
    errorSub = BoardFPGA_StepMotorWriteRegTotalStepSet(motorNo,cmd->stepCountTotal);
    if(errorSub != ERROR_SUB_OK)
    {
        //打开全局中断
        BOARD_FPGA_UNLOCK(lastIntState);
        MCU_LogShowErrorSubCode(errorSub);
        return errorSub;
    }
    //是否清空编码器
    //是否清除编码器坐标
    if(cmd->clearEncoderPosSet == ENABLE)
    {
        errorSub = BoardFPGA_StepMotorEncoderPosClear(motorNo);
        if(errorSub != ERROR_SUB_OK)
        {
            //打开全局中断
            BOARD_FPGA_UNLOCK(lastIntState);
            MCU_LogShowErrorSubCode(errorSub);
            return errorSub;
        }
    }
    //写入停止条件
    errorSub = BoardFPGA_StepMotorWriteRegStopCondition(motorNo,cmd->stopCondition);
    if(errorSub != ERROR_SUB_OK)
    {
        //打开全局中断
        BOARD_FPGA_UNLOCK(lastIntState);
        MCU_LogShowErrorSubCode(errorSub);
        return errorSub;
    }
    //写入数据表
    uint16_t indexUtil = 0;
    uint32_t spdCnt;
    uint16_t writeTabLength = 0;
    if(writePreSet == DISABLE)
    {
        writeTabLength = cmd->tableLength;
    }
    else
    {
        //写入的表项大于预写入长度,使用预写入功能
        if(cmd->tableLength >= BOARD_FPGA_WRITE_TAB_PRE_LEN)
        {
            //先预写入一部分
            writeTabLength = BOARD_FPGA_WRITE_TAB_PRE_LEN;
        }
        else
        {
            //不足预写入长度,全部写入
            writeTabLength = cmd->tableLength;
        }
    }
    //写入表
    for(indexUtil = 0; indexUtil < writeTabLength;indexUtil++)
    {
        spdCnt = BOARD_FPGA_RPM_TO_CNT(cmd->rpmSetArrayPtr[indexUtil],cmd->motorDiv);
        //不能为0
        if(spdCnt == 0)spdCnt = 1;
        errorSub = BoardFPGA_StepMotorWriteRegSpdTabElem(motorNo,indexUtil,spdCnt);
        if(errorSub != ERROR_SUB_OK)
        {
            //打开全局中断
            BOARD_FPGA_UNLOCK(lastIntState);
            MCU_LogShowErrorSubCode(errorSub);
            return errorSub;
        }
        //不能为0
        if(cmd->stepSetArrayPtr[indexUtil] == 0)cmd->stepSetArrayPtr[indexUtil] = 1;
        errorSub = BoardFPGA_StepMotorWriteRegStpTabElem(motorNo,indexUtil,cmd->stepSetArrayPtr[indexUtil]);
        if(errorSub != ERROR_SUB_OK)
        {
            //打开全局中断
            BOARD_FPGA_UNLOCK(lastIntState);
            MCU_LogShowErrorSubCode(errorSub);
            return errorSub;
        }
        if(indexUtil == 0)
        {
            //读取第一个元素,检查是否匹配
            errorSub = BoardFPGA_StepMotorReadSpdTabElemByIdx(motorNo,0,&readCheckVal);
            if(errorSub != ERROR_SUB_OK)
            {
                //打开全局中断
                BOARD_FPGA_UNLOCK(lastIntState);
                MCU_LogShowErrorSubCode(errorSub);
                return errorSub;
            }
            if(readCheckVal != spdCnt)
            {
                //打开全局中断
                BOARD_FPGA_UNLOCK(lastIntState);
                MCU_LogShowErrorSubCode(ERROR_SUB_BOARD_FPGA_STEP_MOTOR_TAB_WRITE_START_SPD_NOT_MATCH);
                return ERROR_SUB_BOARD_FPGA_STEP_MOTOR_TAB_WRITE_START_SPD_NOT_MATCH;
            }
            //读取第一个元素,检查是否匹配
            errorSub = BoardFPGA_StepMotorReadStpTabElemByIdx(motorNo,0,&readCheckVal);
            if(errorSub != ERROR_SUB_OK)
            {
                //打开全局中断
                BOARD_FPGA_UNLOCK(lastIntState);
                MCU_LogShowErrorSubCode(errorSub);
                return errorSub;
            }
            if(readCheckVal != cmd->stepSetArrayPtr[0])
            {
                //打开全局中断
                BOARD_FPGA_UNLOCK(lastIntState);
                MCU_LogShowErrorSubCode(ERROR_SUB_BOARD_FPGA_STEP_MOTOR_TAB_WRITE_START_STP_NOT_MATCH);
                return ERROR_SUB_BOARD_FPGA_STEP_MOTOR_TAB_WRITE_START_STP_NOT_MATCH;
            }
        }
    }
    //写入一个0分割下一个表系数
    if(indexUtil < BOARD_FPGA_STEP_MOTOR_TAB_LENGTH)
    {
        errorSub = BoardFPGA_StepMotorWriteRegSpdTabElem(motorNo,indexUtil,0);
        if(errorSub != ERROR_SUB_OK)
        {
            //打开全局中断
            BOARD_FPGA_UNLOCK(lastIntState);
            MCU_LogShowErrorSubCode(errorSub);
            return errorSub;
        }
        errorSub = BoardFPGA_StepMotorWriteRegStpTabElem(motorNo,indexUtil,0);
        if(errorSub != ERROR_SUB_OK)
        {
            //打开全局中断
            BOARD_FPGA_UNLOCK(lastIntState);
            MCU_LogShowErrorSubCode(errorSub);
            return errorSub;
        }
    }
    //读取控制寄存器
    FPGA_STEP_MOTOR_REG_CONTROL controlReg = BoardFPGA_StepMotorReadRegControl(motorNo);
    //先设置清空编码器为0
    controlReg.valBits.encoderPosClearSet = DISABLE;
    controlReg.valBits.spdStpTablePreWriteSet = writePreSet;
    //指令运转,决定方向
    if(cmd->motorDir == STEP_MOTOR_DIR_CW)
    {
        controlReg.valBits.motorCmd = STEP_MOTOR_CMD_TAB_CW;
    }
    else
    {
        controlReg.valBits.motorCmd = STEP_MOTOR_CMD_TAB_CCW;
    }
    //指令启动,写入控制信号
    errorSub = BoardFPGA_StepMotorWriteRegControl(motorNo,controlReg);
    if(errorSub != ERROR_SUB_OK)
    {
        //打开全局中断
        BOARD_FPGA_UNLOCK(lastIntState);
        MCU_LogShowErrorSubCode(errorSub);
        return errorSub;
    }
    //如果使用了预写入,那么继续写入
    if((writePreSet == ENABLE)&&(cmd->tableLength > BOARD_FPGA_WRITE_TAB_PRE_LEN))
    {
        //写入的表项大于预写入长度,使用预写入功能
        for(indexUtil = BOARD_FPGA_WRITE_TAB_PRE_LEN; indexUtil < cmd->tableLength;indexUtil++)
        {
            spdCnt = BOARD_FPGA_RPM_TO_CNT(cmd->rpmSetArrayPtr[indexUtil],cmd->motorDiv);
            //不能为0
            if(spdCnt == 0)spdCnt = 1;
            errorSub = BoardFPGA_StepMotorWriteRegSpdTabElem(motorNo,indexUtil,spdCnt);
            if(errorSub != ERROR_SUB_OK)
            {
                //打开全局中断
                BOARD_FPGA_UNLOCK(lastIntState);
                MCU_LogShowErrorSubCode(errorSub);
                return errorSub;
            }
            if(cmd->stepSetArrayPtr[indexUtil] == 0)cmd->stepSetArrayPtr[indexUtil] = 1;
            errorSub = BoardFPGA_StepMotorWriteRegStpTabElem(motorNo,indexUtil,cmd->stepSetArrayPtr[indexUtil]);
            if(errorSub != ERROR_SUB_OK)
            {
                //打开全局中断
                BOARD_FPGA_UNLOCK(lastIntState);
                MCU_LogShowErrorSubCode(errorSub);
                return errorSub;
            }
        }
    }
    //读取状态,不能是空闲
    FPGA_STEP_MOTOR_REG_STATE4 motorState = BoardFPGA_StepMotorReadRegMotorState(motorNo);
    if(motorState.valBits.motorState == STEP_MOTOR_STATE_IDLE)
    {
        //打开全局中断
        BOARD_FPGA_UNLOCK(lastIntState);
        MCU_LogShowErrorSubCode(ERROR_SUB_BOARD_FPGA_STEP_MOTOR_CMD_TAB_SPD_START_FAIL);
        return ERROR_SUB_BOARD_FPGA_STEP_MOTOR_CMD_TAB_SPD_START_FAIL;
    }
    //打开全局中断
    BOARD_FPGA_UNLOCK(lastIntState);
    return ERROR_SUB_OK;
}

// //等待步进电机运行完成,同步的,状态查询
// ERROR_SUB BoardFPGA_StepMotorWaitStopSync(STEP_MOTOR_NO motorNo,uint32_t timeOutMs,FPGA_STEP_MOTOR_REG_STATE4* motorStateReg)
// {
//     uint32_t startTimeTick = tx_time_get();
//     do
//     {
//         //读取状态寄存器
//         *motorStateReg = BoardFPGA_StepMotorReadRegMotorState(motorNo);
//         if(motorStateReg->valBits.motorState == STEP_MOTOR_STATE_IDLE)
//         {
//             return ERROR_SUB_OK;
//         }
//         else if(motorStateReg->valBits.motorState == STEP_MOTOR_STATE_STOP_CONDITION)
//         {
//             return ERROR_SUB_OK;
//         }
//         else if(motorStateReg->valBits.motorState == STEP_MOTOR_STATE_STOP_CMD)
//         {
//             return ERROR_SUB_OK;
//         }
//         else
//         {
//             //延时
//             CoreDelayMinTick();
//         }
//     } while (tx_time_get() - startTimeTick <= MS_TO_OS_TICK(timeOutMs));
//     MCU_LogShowErrorSubCode(ERROR_SUB_BOARD_FPGA_STEP_MOTOR_WAIT_STOP_TIME_OUT);
//     return ERROR_SUB_BOARD_FPGA_STEP_MOTOR_WAIT_STOP_TIME_OUT;
// }



// #include "StepMotor.h"
// FPGA_STEP_MOTOR_REG_STATE4 motorStateTest;
// //全曲线
// static STEP_MOTOR_MAIN_CURVE_TAB_DATA stepMotorCurveMainData;
// //半曲线
// static STEP_MOTOR_UTIL_CURVE_TAB_DATA stepMotorCurveUtilData;

// //电机测试配置
// ERROR_SUB BoardFPGA_StepMotorConfigTest(void)
// {
//     ERROR_SUB errorSub = ERROR_SUB_OK;
//     STEP_MOTOR_NO motorNo = STEP_MOTOR_NO1;
//     //首先配置寄存器
//     FPGA_MOTOR_ATTRI motorAttri;
//     for(motorNo = STEP_MOTOR_NO1;motorNo < FPGA_STEP_MOTOR_7;motorNo++)
//     {
//         motorAttri.encoderFactor = 1.0f;
//         motorAttri.configReg.valBits.resetSensorValidLevel = GPIO_PIN_SET;
//         motorAttri.configReg.valBits.emStopSensorValidLevel = GPIO_PIN_SET;
//         motorAttri.configReg.valBits.utilStopSensorValidLevel = GPIO_PIN_SET;
//         motorAttri.configReg.valBits.motorEnableValidLevel = GPIO_PIN_RESET;
//         motorAttri.configReg.valBits.motorDirCwLevel = GPIO_PIN_SET;
//         motorAttri.configReg.valBits.motorPluseIdleLevel = GPIO_PIN_RESET;
//         motorAttri.configReg.valBits.encoderValidLevelA = GPIO_PIN_SET;
//         motorAttri.configReg.valBits.encoderValidLevelB = GPIO_PIN_SET;
//         motorAttri.configReg.valBits.encoderValidLevelZ = GPIO_PIN_SET;
//         //写入配置寄存器
//         errorSub = BoardFPGA_StepMotorWriteMotorAttri(motorNo,&motorAttri);
//         if(errorSub != ERROR_SUB_OK)
//         {
//             MCU_LogShowErrorSubCode(errorSub);
//             return errorSub;
//         }
//     }
//     return ERROR_SUB_OK;
// }

// ERROR_SUB BoardFPGA_StepMotorFixSpdTest(void)
// {
//     ERROR_SUB errorSub = ERROR_SUB_OK;
//     STEP_MOTOR_NO motorNo = STEP_MOTOR_NO1;
//     FPGA_MOTOR_FIX_SPD_CMD fixSpdCmd;
//     uint32_t startTimeTick = tx_time_get();
//     for(motorNo = STEP_MOTOR_NO1;motorNo < FPGA_STEP_MOTOR_7;motorNo++)
//     {
//         //电机和编码器一起使能
//         errorSub = BoardFPGA_StepMotorAndEncoderState(motorNo,ENABLE,ENABLE);
//         if(errorSub != ERROR_SUB_OK)
//         {
//             MCU_LogShowErrorSubCode(errorSub);
//             return errorSub;
//         }
//         //清空编码器计数器
//         errorSub = BoardFPGA_StepMotorEncoderPosClear(motorNo);
//         if(errorSub != ERROR_SUB_OK)
//         {
//             MCU_LogShowErrorSubCode(errorSub);
//             return errorSub;
//         }
//         fixSpdCmd.motorDir = STEP_MOTOR_DIR_CW;
//         fixSpdCmd.motorDiv = 1;
//         fixSpdCmd.rpmSet = 200.0;
//         fixSpdCmd.stepCount = 10000;
//         fixSpdCmd.clearEncoderPosSet = DISABLE;
//         //停止条件设定
//         fixSpdCmd.stopCondition.valBits.resetSensorValidStop = DISABLE;
//         fixSpdCmd.stopCondition.valBits.resetSensorInvalidStop = DISABLE;
//         fixSpdCmd.stopCondition.valBits.emStopSensorValidStop = DISABLE;
//         fixSpdCmd.stopCondition.valBits.utilStopSensorValidStop = DISABLE;
//         fixSpdCmd.stopCondition.valBits.pluseSendOverStop = ENABLE;
//         //开始运转
//         errorSub = BoardFPGA_StepMotorFixSpdRunStart(motorNo,&fixSpdCmd);
//         if(errorSub != ERROR_SUB_OK)
//         {
//             MCU_LogShowErrorSubCode(errorSub);
//             return errorSub;
//         }
//     }
//     startTimeTick = tx_time_get();
//     for(motorNo = STEP_MOTOR_NO1;motorNo < FPGA_STEP_MOTOR_7;motorNo++)
//     {
//         //等待运转完成
//         errorSub = BoardFPGA_StepMotorWaitStopSync(motorNo,60000,&motorStateTest);
//         if(errorSub != ERROR_SUB_OK)
//         {
//             MCU_LogShowErrorSubCode(errorSub);
//             return errorSub;
//         }
//         //检测停止源
//         if((motorStateTest.valBits.stopReason != STEP_MOTOR_STOP_REASON_CW_SPD_FIX_PLUSE_OVER)||
//                 (motorStateTest.valBits.motorState != STEP_MOTOR_STATE_STOP_CONDITION))
//         {
//             MCU_LogShowErrorSubCode(ERROR_SUB_BOARD_FPGA_FUNC_TEST_FAIL);
//             MCU_LogPrintf("MotorState: 0X%04X,StopReason: 0X%04X\r\n",motorStateTest.valBits.motorState,motorStateTest.valBits.stopReason);
//             return ERROR_SUB_BOARD_FPGA_FUNC_TEST_FAIL;
//         }
//         //进入空闲
//         errorSub = BoardFPGA_StepMotorConvertToIdle(motorNo);
//         if(errorSub != ERROR_SUB_OK)
//         {
//             MCU_LogShowErrorSubCode(errorSub);
//             return errorSub;
//         }
//     }
//     //输出日志
//     MCU_LogPrintf("Motor FixSpeedCW, Test Success Over,TimeUse: %d ms\r\n",OS_TICK_TO_MS(tx_time_get() - startTimeTick));
//     for(motorNo = STEP_MOTOR_NO1;motorNo < FPGA_STEP_MOTOR_7;motorNo++)
//     {
//         //反向运转
//         fixSpdCmd.motorDir = STEP_MOTOR_DIR_CCW;
//         //开始运转
//         errorSub = BoardFPGA_StepMotorFixSpdRunStart(motorNo,&fixSpdCmd);
//         if(errorSub != ERROR_SUB_OK)
//         {
//             MCU_LogShowErrorSubCode(errorSub);
//             return errorSub;
//         }
//     }
//     startTimeTick = tx_time_get();
//     for(motorNo = STEP_MOTOR_NO1;motorNo < FPGA_STEP_MOTOR_7;motorNo++)
//     {
//         //等待运转完成
//         errorSub = BoardFPGA_StepMotorWaitStopSync(motorNo,60000,&motorStateTest);
//         if(errorSub != ERROR_SUB_OK)
//         {
//             MCU_LogShowErrorSubCode(errorSub);
//             return errorSub;
//         }
//         //检测停止源
//         if((motorStateTest.valBits.stopReason != STEP_MOTOR_STOP_REASON_CCW_SPD_FIX_PLUSE_OVER)||
//                 (motorStateTest.valBits.motorState != STEP_MOTOR_STATE_STOP_CONDITION))
//         {
//             MCU_LogShowErrorSubCode(ERROR_SUB_BOARD_FPGA_FUNC_TEST_FAIL);
//             MCU_LogPrintf("MotorState: 0X%04X,StopReason: 0X%04X\r\n",motorStateTest.valBits.motorState,motorStateTest.valBits.stopReason);
//             return ERROR_SUB_BOARD_FPGA_FUNC_TEST_FAIL;
//         }
//         //进入空闲
//         errorSub = BoardFPGA_StepMotorConvertToIdle(motorNo);
//         if(errorSub != ERROR_SUB_OK)
//         {
//             MCU_LogShowErrorSubCode(errorSub);
//             return errorSub;
//         }
//         //电机解锁
//         errorSub = BoardFPGA_StepMotorAndEncoderState(motorNo,DISABLE,DISABLE);
//         if(errorSub != ERROR_SUB_OK)
//         {
//             MCU_LogShowErrorSubCode(errorSub);
//             return errorSub;
//         }
//     }
//     //输出日志
//     MCU_LogPrintf("Motor FixSpeedCCW, Test Success Over,TimeUse: %d ms\r\n",OS_TICK_TO_MS(tx_time_get() - startTimeTick));
//     return ERROR_SUB_OK;
// }

// //电机测试步数表运转
// ERROR_SUB BoardFPGA_StepMotorTabSpdTest(void)
// {
//     ERROR_SUB errorSub = ERROR_SUB_OK;
//     STEP_MOTOR_NO motorNo = STEP_MOTOR_NO1;
//     FPGA_MOTOR_TAB_SPD_CMD tabSpdCmd;
//     STEP_MOTOR_TAB_CURVE_ATTRI tabCalcCmd;
//     uint32_t startTimeTick = tx_time_get();
//     //计算步数表
//     stepMotorCurveMainData.tabLengthAfterCalc = 0;
//     //速度参数
//     tabCalcCmd.startRpm = 100.0;
//     tabCalcCmd.maxRpm = 2000.0;
//     tabCalcCmd.accelerationStart = 0.0;
//     tabCalcCmd.accelerationMax = 8.0;
//     tabCalcCmd.accelerationGradient = 0.05;
//     tabCalcCmd.spdRatio = 1.0;
//     tabCalcCmd.ratioMode = SPEED_RATIO_MODE_GLOBAL;
//     tabCalcCmd.startSteps = 1;
//     tabCalcCmd.stepAllocMode = STEP_ALLOC_MODE_SAME_TIME_FRAME;
//     //设置参数
//     tabSpdCmd.motorDir = STEP_MOTOR_DIR_CW;
//     tabSpdCmd.motorDiv = 1;
//     tabSpdCmd.rpmSetArrayPtr = stepMotorCurveMainData.spdTableArray;
//     tabSpdCmd.stepSetArrayPtr = stepMotorCurveMainData.stpTableArray;
//     tabSpdCmd.tableLength = stepMotorCurveMainData.tabLengthAfterCalc;
//     tabSpdCmd.stepCountTotal = 10000;
//     tabSpdCmd.clearEncoderPosSet = ENABLE;
//     //停止条件设定
//     tabSpdCmd.stopCondition.valBits.resetSensorValidStop = DISABLE;
//     tabSpdCmd.stopCondition.valBits.resetSensorInvalidStop = DISABLE;
//     tabSpdCmd.stopCondition.valBits.emStopSensorValidStop = DISABLE;
//     tabSpdCmd.stopCondition.valBits.utilStopSensorValidStop = DISABLE;
//     tabSpdCmd.stopCondition.valBits.pluseSendOverStop = ENABLE;
//     //计算电机加速表
//     StepMotorUtilCalcTableWithStep(&stepMotorCurveMainData,&tabCalcCmd,tabSpdCmd.stepCountTotal);
//     //计算完成写入数据
//     tabSpdCmd.tableLength = stepMotorCurveMainData.tabLengthAfterCalc;
//     for(motorNo = STEP_MOTOR_NO1;motorNo < FPGA_STEP_MOTOR_7;motorNo++)
//     {
//         //电机和编码器一起使能
//         errorSub = BoardFPGA_StepMotorAndEncoderState(motorNo,ENABLE,ENABLE);
//         if(errorSub != ERROR_SUB_OK)
//         {
//             MCU_LogShowErrorSubCode(errorSub);
//             return errorSub;
//         }
//         //清空编码器计数器
//         errorSub = BoardFPGA_StepMotorEncoderPosClear(motorNo);
//         if(errorSub != ERROR_SUB_OK)
//         {
//             MCU_LogShowErrorSubCode(errorSub);
//             return errorSub;
//         }
//         //开始运转
//         errorSub = BoardFPGA_StepMotorTabSpdRunStart(motorNo,&tabSpdCmd,DISABLE);
//         if(errorSub != ERROR_SUB_OK)
//         {
//             MCU_LogShowErrorSubCode(errorSub);
//             return errorSub;
//         }
//     }
//     startTimeTick = tx_time_get();
//     for(motorNo = STEP_MOTOR_NO1;motorNo < FPGA_STEP_MOTOR_7;motorNo++)
//     {
//         //等待运转完成
//         errorSub = BoardFPGA_StepMotorWaitStopSync(motorNo,60000,&motorStateTest);
//         if(errorSub != ERROR_SUB_OK)
//         {
//             MCU_LogShowErrorSubCode(errorSub);
//             return errorSub;
//         }
//         //检测停止源
//         if((motorStateTest.valBits.stopReason != STEP_MOTOR_STOP_REASON_CW_SPD_TAB_PLUSE_OVER)||
//                 (motorStateTest.valBits.motorState != STEP_MOTOR_STATE_STOP_CONDITION))
//         {
//             MCU_LogPrintf("MotorState: 0X%04X,StopReason: 0X%04X\r\n",motorStateTest.valBits.motorState,motorStateTest.valBits.stopReason);
//             MCU_LogShowErrorSubCode(ERROR_SUB_BOARD_FPGA_FUNC_TEST_FAIL);
//             return ERROR_SUB_BOARD_FPGA_FUNC_TEST_FAIL;
//         }
//         //进入空闲
//         errorSub = BoardFPGA_StepMotorConvertToIdle(motorNo);
//         if(errorSub != ERROR_SUB_OK)
//         {
//             MCU_LogShowErrorSubCode(errorSub);
//             return errorSub;
//         }
//     }
//     //输出日志
//     MCU_LogPrintf("Motor TabSpeedCW, Test Success Over,TimeUse: %d ms\r\n",OS_TICK_TO_MS(tx_time_get() - startTimeTick));
//     for(motorNo = STEP_MOTOR_NO1;motorNo < FPGA_STEP_MOTOR_7;motorNo++)
//     {
//         //反向运转
//         tabSpdCmd.motorDir = STEP_MOTOR_DIR_CCW;
//         //开始运转
//         errorSub = BoardFPGA_StepMotorTabSpdRunStart(motorNo,&tabSpdCmd,DISABLE);
//         if(errorSub != ERROR_SUB_OK)
//         {
//             MCU_LogShowErrorSubCode(errorSub);
//             return errorSub;
//         }
//     }
//     startTimeTick = tx_time_get();
//     for(motorNo = STEP_MOTOR_NO1;motorNo < FPGA_STEP_MOTOR_7;motorNo++)
//     {
//         //等待运转完成
//         errorSub = BoardFPGA_StepMotorWaitStopSync(motorNo,60000,&motorStateTest);
//         if(errorSub != ERROR_SUB_OK)
//         {
//             MCU_LogShowErrorSubCode(errorSub);
//             return errorSub;
//         }
//         //检测停止源
//         if((motorStateTest.valBits.stopReason != STEP_MOTOR_STOP_REASON_CCW_SPD_TAB_PLUSE_OVER)||
//                 (motorStateTest.valBits.motorState != STEP_MOTOR_STATE_STOP_CONDITION))
//         {
//             MCU_LogShowErrorSubCode(ERROR_SUB_BOARD_FPGA_FUNC_TEST_FAIL);
//             MCU_LogPrintf("MotorState: 0X%04X,StopReason: 0X%04X\r\n",motorStateTest.valBits.motorState,motorStateTest.valBits.stopReason);
//             return ERROR_SUB_BOARD_FPGA_FUNC_TEST_FAIL;
//         }
//         //进入空闲
//         errorSub = BoardFPGA_StepMotorConvertToIdle(motorNo);
//         if(errorSub != ERROR_SUB_OK)
//         {
//             MCU_LogShowErrorSubCode(errorSub);
//             return errorSub;
//         }
//         //电机解锁
//         errorSub = BoardFPGA_StepMotorAndEncoderState(motorNo,DISABLE,DISABLE);
//         if(errorSub != ERROR_SUB_OK)
//         {
//             MCU_LogShowErrorSubCode(errorSub);
//             return errorSub;
//         }
//     }
//     //输出日志
//     MCU_LogPrintf("Motor TabSpeedCCW, Test Success Over,TimeUse: %d ms\r\n",OS_TICK_TO_MS(tx_time_get() - startTimeTick));
//     return ERROR_SUB_OK;
// }

// //电机持续运转启动
// ERROR_SUB BoardFPGA_StepMotorTabSpdRunAlways(void)
// {
//     ERROR_SUB errorSub = ERROR_SUB_OK;
//     STEP_MOTOR_NO motorNo = STEP_MOTOR_NO1;
//     FPGA_MOTOR_TAB_SPD_CMD tabSpdCmd;
//     STEP_MOTOR_TAB_CURVE_ATTRI tabCalcCmd;
//     SM_CURVE_TAB_SET tabCurveSet;
//     tabCurveSet.mainCurveDataPtr = &stepMotorCurveMainData;
//     tabCurveSet.utilCurveDataPtr = &stepMotorCurveUtilData;
//     uint32_t startTimeTick = tx_time_get();
//     //速度参数
//     tabCalcCmd.startRpm = 100.0;
//     tabCalcCmd.maxRpm = 2000.0;
//     tabCalcCmd.accelerationStart = 0.0;
//     tabCalcCmd.accelerationMax = 8.0;
//     tabCalcCmd.accelerationGradient = 0.05;
//     tabCalcCmd.spdRatio = 1.0;
//     tabCalcCmd.ratioMode = SPEED_RATIO_MODE_GLOBAL;
//     tabCalcCmd.startSteps = 1;
//     tabCalcCmd.stepAllocMode = STEP_ALLOC_MODE_SAME_TIME_FRAME;
//     //设置参数
//     tabSpdCmd.motorDir = STEP_MOTOR_DIR_CW;
//     tabSpdCmd.motorDiv = 1;
//     tabSpdCmd.rpmSetArrayPtr = tabCurveSet.utilCurveDataPtr->spdTableArray;
//     tabSpdCmd.stepSetArrayPtr = tabCurveSet.utilCurveDataPtr->stpTableArray;
//     tabSpdCmd.tableLength = tabCurveSet.utilCurveDataPtr->tabLengthAfterCalc;
//     tabSpdCmd.stepCountTotal = tabCurveSet.utilCurveDataPtr->totalRunningSteps;
//     tabSpdCmd.clearEncoderPosSet = ENABLE;
//     //停止条件设定
//     tabSpdCmd.stopCondition.valBits.resetSensorValidStop = DISABLE;
//     tabSpdCmd.stopCondition.valBits.resetSensorInvalidStop = DISABLE;
//     tabSpdCmd.stopCondition.valBits.emStopSensorValidStop = DISABLE;
//     tabSpdCmd.stopCondition.valBits.utilStopSensorValidStop = DISABLE;
//     tabSpdCmd.stopCondition.valBits.pluseSendOverStop = DISABLE;
//     //计算电机加速表
//     StepMotorUtilCalcTableWithRunAlwaysAcc(&tabCurveSet,&tabCalcCmd);
//     //计算完成写入数据
//     tabSpdCmd.tableLength = tabCurveSet.utilCurveDataPtr->tabLengthAfterCalc;
//     for(motorNo = STEP_MOTOR_NO1;motorNo < FPGA_STEP_MOTOR_7;motorNo++)
//     {
//         //电机和编码器一起使能
//         errorSub = BoardFPGA_StepMotorAndEncoderState(motorNo,ENABLE,ENABLE);
//         if(errorSub != ERROR_SUB_OK)
//         {
//             MCU_LogShowErrorSubCode(errorSub);
//             return errorSub;
//         }
//         //清空编码器计数器
//         errorSub = BoardFPGA_StepMotorEncoderPosClear(motorNo);
//         if(errorSub != ERROR_SUB_OK)
//         {
//             MCU_LogShowErrorSubCode(errorSub);
//             return errorSub;
//         }
//         //开始运转
//         errorSub = BoardFPGA_StepMotorTabSpdRunStart(motorNo,&tabSpdCmd,DISABLE);
//         if(errorSub != ERROR_SUB_OK)
//         {
//             MCU_LogShowErrorSubCode(errorSub);
//             return errorSub;
//         }
//     }
//     //运行10s
//     CoreDelayMs(10000);
//     FPGA_STEP_MOTOR_REG_STATE3 motorState3;
//     //电机减速停止
//     for(motorNo = STEP_MOTOR_NO1;motorNo < FPGA_STEP_MOTOR_7;motorNo++)
//     {
//         // errorSub = BoardFPGA_StepMotorCmdStop(motorNo,&motorStateTest);
//         // if(errorSub != ERROR_SUB_OK)
//         // {
//         //     MCU_LogShowErrorSubCode(errorSub);
//         //     return errorSub;
//         // }
//         // if(motorStateTest.valBits.motorState != STEP_MOTOR_STATE_STOP_CMD)
//         // {
//         //     MCU_LogShowErrorSubCode(ERROR_SUB_BOARD_FPGA_STEP_MOTOR_RUN_ALWAYS_STOP_NOT_MATCH);
//         //     return ERROR_SUB_BOARD_FPGA_STEP_MOTOR_RUN_ALWAYS_STOP_NOT_MATCH;
//         // }
//         //读取当前序号
//         motorState3 = BoardFPGA_StepMotorReadRegSensorStateTabIndex(motorNo);
//         //计算减速表
//         StepMotorUtilCalcTableWithRunAlwaysDec(&tabCurveSet,motorState3.valBits.tabIndexCurrent);
//         //启动减速
//         //设置参数
//         tabSpdCmd.motorDir = STEP_MOTOR_DIR_CW;
//         tabSpdCmd.motorDiv = 1;
//         tabSpdCmd.rpmSetArrayPtr = tabCurveSet.utilCurveDataPtr->spdTableArray;
//         tabSpdCmd.stepSetArrayPtr = tabCurveSet.utilCurveDataPtr->stpTableArray;
//         tabSpdCmd.tableLength = tabCurveSet.utilCurveDataPtr->tabLengthAfterCalc;
//         tabSpdCmd.stepCountTotal = tabCurveSet.utilCurveDataPtr->totalRunningSteps;
//         tabSpdCmd.clearEncoderPosSet = ENABLE;
//         //停止条件设定
//         tabSpdCmd.stopCondition.valBits.resetSensorValidStop = DISABLE;
//         tabSpdCmd.stopCondition.valBits.resetSensorInvalidStop = DISABLE;
//         tabSpdCmd.stopCondition.valBits.emStopSensorValidStop = DISABLE;
//         tabSpdCmd.stopCondition.valBits.utilStopSensorValidStop = DISABLE;
//         tabSpdCmd.stopCondition.valBits.pluseSendOverStop = ENABLE;
//         //计算完成写入数据
//         tabSpdCmd.tableLength = tabCurveSet.utilCurveDataPtr->tabLengthAfterCalc;
//         //开始运转
//         errorSub = BoardFPGA_StepMotorTabSpdRunStart(motorNo,&tabSpdCmd,ENABLE);
//         if(errorSub != ERROR_SUB_OK)
//         {
//             MCU_LogShowErrorSubCode(errorSub);
//             return errorSub;
//         }
//     }
//     startTimeTick = tx_time_get();
//     for(motorNo = STEP_MOTOR_NO1;motorNo < FPGA_STEP_MOTOR_7;motorNo++)
//     {
//         //等待运转完成
//         errorSub = BoardFPGA_StepMotorWaitStopSync(motorNo,60000,&motorStateTest);
//         if(errorSub != ERROR_SUB_OK)
//         {
//             MCU_LogShowErrorSubCode(errorSub);
//             return errorSub;
//         }
//         //检测停止源
//         if((motorStateTest.valBits.stopReason != STEP_MOTOR_STOP_REASON_CW_SPD_TAB_PLUSE_OVER)||
//                 (motorStateTest.valBits.motorState != STEP_MOTOR_STATE_STOP_CONDITION))
//         {
//             MCU_LogShowErrorSubCode(ERROR_SUB_BOARD_FPGA_FUNC_TEST_FAIL);
//             MCU_LogPrintf("MotorState: 0X%04X,StopReason: 0X%04X\r\n",motorStateTest.valBits.motorState,motorStateTest.valBits.stopReason);
//             return ERROR_SUB_BOARD_FPGA_FUNC_TEST_FAIL;
//         }
//         //进入空闲
//         errorSub = BoardFPGA_StepMotorConvertToIdle(motorNo);
//         if(errorSub != ERROR_SUB_OK)
//         {
//             MCU_LogShowErrorSubCode(errorSub);
//             return errorSub;
//         }
//         //电机解锁
//         errorSub = BoardFPGA_StepMotorAndEncoderState(motorNo,DISABLE,DISABLE);
//         if(errorSub != ERROR_SUB_OK)
//         {
//             MCU_LogShowErrorSubCode(errorSub);
//             return errorSub;
//         }
//     }
    
//     //输出日志
//     MCU_LogPrintf("Motor RunAlways, Test Success Over,TimeUse: %d ms\r\n",OS_TICK_TO_MS(tx_time_get() - startTimeTick));
//     return ERROR_SUB_OK;
// }

// //电机急停模拟
// ERROR_SUB BoardFPGA_StepMotorTabSpdRunDacc(void)
// {
//     ERROR_SUB errorSub = ERROR_SUB_OK;
//     STEP_MOTOR_NO motorNo = STEP_MOTOR_NO1;
//     FPGA_MOTOR_TAB_SPD_CMD tabSpdCmd;
//     STEP_MOTOR_TAB_CURVE_ATTRI tabCalcCmd;
//     SM_CURVE_TAB_SET tabCurveSet;
//     tabCurveSet.mainCurveDataPtr = &stepMotorCurveMainData;
//     tabCurveSet.utilCurveDataPtr = &stepMotorCurveUtilData;
//     uint32_t startTimeTick = tx_time_get();
//     //计算步数表
//     stepMotorCurveMainData.tabLengthAfterCalc = 0;
//     //速度参数
//     tabCalcCmd.startRpm = 100.0;
//     tabCalcCmd.maxRpm = 2000.0;
//     tabCalcCmd.accelerationStart = 0.0;
//     tabCalcCmd.accelerationMax = 8.0;
//     tabCalcCmd.accelerationGradient = 0.05;
//     tabCalcCmd.spdRatio = 1.0;
//     tabCalcCmd.ratioMode = SPEED_RATIO_MODE_GLOBAL;
//     tabCalcCmd.startSteps = 1;
//     tabCalcCmd.stepAllocMode = STEP_ALLOC_MODE_SAME_TIME_FRAME;
//     //设置参数
//     tabSpdCmd.motorDir = STEP_MOTOR_DIR_CW;
//     tabSpdCmd.motorDiv = 1;
//     tabSpdCmd.rpmSetArrayPtr = tabCurveSet.mainCurveDataPtr->spdTableArray;
//     tabSpdCmd.stepSetArrayPtr = tabCurveSet.mainCurveDataPtr->stpTableArray;
//     tabSpdCmd.tableLength = tabCurveSet.mainCurveDataPtr->tabLengthAfterCalc;
//     tabSpdCmd.stepCountTotal = 10000;
//     tabSpdCmd.clearEncoderPosSet = ENABLE;
//     //停止条件设定
//     tabSpdCmd.stopCondition.valBits.resetSensorValidStop = DISABLE;
//     tabSpdCmd.stopCondition.valBits.resetSensorInvalidStop = DISABLE;
//     tabSpdCmd.stopCondition.valBits.emStopSensorValidStop = DISABLE;
//     tabSpdCmd.stopCondition.valBits.utilStopSensorValidStop = DISABLE;
//     tabSpdCmd.stopCondition.valBits.pluseSendOverStop = ENABLE;
//     //计算电机加速表
//     StepMotorUtilCalcTableWithStep(&stepMotorCurveMainData,&tabCalcCmd,10000);
//     //计算完成写入数据
//     tabSpdCmd.tableLength = stepMotorCurveMainData.tabLengthAfterCalc;
//     for(motorNo = STEP_MOTOR_NO1;motorNo < FPGA_STEP_MOTOR_7;motorNo++)
//     {
//         //电机和编码器一起使能
//         errorSub = BoardFPGA_StepMotorAndEncoderState(motorNo,ENABLE,ENABLE);
//         if(errorSub != ERROR_SUB_OK)
//         {
//             MCU_LogShowErrorSubCode(errorSub);
//             return errorSub;
//         }
//         //清空编码器计数器
//         errorSub = BoardFPGA_StepMotorEncoderPosClear(motorNo);
//         if(errorSub != ERROR_SUB_OK)
//         {
//             MCU_LogShowErrorSubCode(errorSub);
//             return errorSub;
//         }
//         //开始运转
//         errorSub = BoardFPGA_StepMotorTabSpdRunStart(motorNo,&tabSpdCmd,DISABLE);
//         if(errorSub != ERROR_SUB_OK)
//         {
//             MCU_LogShowErrorSubCode(errorSub);
//             return errorSub;
//         }
//     }
//     //延时等待运转
//     CoreDelayMs(MCU_RandomGetNextUnsigned(500,2000));
//     FPGA_STEP_MOTOR_REG_STATE3 motorState3;
//     STEP_MOTOR_DACC_PARAM daccParams;
//     daccParams.daccSteps = 100;
//     //电机减速停止
//     for(motorNo = STEP_MOTOR_NO1;motorNo < FPGA_STEP_MOTOR_7;motorNo++)
//     {
//         errorSub = BoardFPGA_StepMotorCmdStop(motorNo,&motorStateTest);
//         if(errorSub != ERROR_SUB_OK)
//         {
//             MCU_LogShowErrorSubCode(errorSub);
//             return errorSub;
//         }
//         if(motorStateTest.valBits.motorState != STEP_MOTOR_STATE_STOP_CMD)
//         {
//             MCU_LogShowErrorSubCode(ERROR_SUB_BOARD_FPGA_STEP_MOTOR_RUN_ALWAYS_STOP_NOT_MATCH);
//             return ERROR_SUB_BOARD_FPGA_STEP_MOTOR_RUN_ALWAYS_STOP_NOT_MATCH;
//         }
//         //读取当前序号
//         motorState3 = BoardFPGA_StepMotorReadRegSensorStateTabIndex(motorNo);
//         daccParams.currentSpdIndex = motorState3.valBits.tabIndexCurrent;
//         //计算减速表
//         StepMotorUtilCalcTableDacc(&tabCurveSet,&daccParams);
//         //启动减速
//         //设置参数
//         tabSpdCmd.motorDir = STEP_MOTOR_DIR_CW;
//         tabSpdCmd.motorDiv = 1;
//         tabSpdCmd.rpmSetArrayPtr = tabCurveSet.utilCurveDataPtr->spdTableArray;
//         tabSpdCmd.stepSetArrayPtr = tabCurveSet.utilCurveDataPtr->stpTableArray;
//         tabSpdCmd.tableLength = tabCurveSet.utilCurveDataPtr->tabLengthAfterCalc;
//         tabSpdCmd.stepCountTotal = tabCurveSet.utilCurveDataPtr->totalRunningSteps;
//         tabSpdCmd.clearEncoderPosSet = ENABLE;
//         //停止条件设定
//         tabSpdCmd.stopCondition.valBits.resetSensorValidStop = DISABLE;
//         tabSpdCmd.stopCondition.valBits.resetSensorInvalidStop = DISABLE;
//         tabSpdCmd.stopCondition.valBits.emStopSensorValidStop = DISABLE;
//         tabSpdCmd.stopCondition.valBits.utilStopSensorValidStop = DISABLE;
//         tabSpdCmd.stopCondition.valBits.pluseSendOverStop = ENABLE;
//         //计算完成写入数据
//         tabSpdCmd.tableLength = tabCurveSet.utilCurveDataPtr->tabLengthAfterCalc;
//         //开始运转
//         errorSub = BoardFPGA_StepMotorTabSpdRunStart(motorNo,&tabSpdCmd,ENABLE);
//         if(errorSub != ERROR_SUB_OK)
//         {
//             MCU_LogShowErrorSubCode(errorSub);
//             return errorSub;
//         }
//     }
//     startTimeTick = tx_time_get();
//     for(motorNo = STEP_MOTOR_NO1;motorNo < FPGA_STEP_MOTOR_7;motorNo++)
//     {
//         //等待运转完成
//         errorSub = BoardFPGA_StepMotorWaitStopSync(motorNo,60000,&motorStateTest);
//         if(errorSub != ERROR_SUB_OK)
//         {
//             MCU_LogShowErrorSubCode(errorSub);
//             return errorSub;
//         }
//         //检测停止源
//         if((motorStateTest.valBits.stopReason != STEP_MOTOR_STOP_REASON_CW_SPD_TAB_PLUSE_OVER)||
//                 (motorStateTest.valBits.motorState != STEP_MOTOR_STATE_STOP_CONDITION))
//         {
//             MCU_LogShowErrorSubCode(ERROR_SUB_BOARD_FPGA_FUNC_TEST_FAIL);
//             MCU_LogPrintf("MotorState: 0X%04X,StopReason: 0X%04X\r\n",motorStateTest.valBits.motorState,motorStateTest.valBits.stopReason);
//             return ERROR_SUB_BOARD_FPGA_FUNC_TEST_FAIL;
//         }
//         //进入空闲
//         errorSub = BoardFPGA_StepMotorConvertToIdle(motorNo);
//         if(errorSub != ERROR_SUB_OK)
//         {
//             MCU_LogShowErrorSubCode(errorSub);
//             return errorSub;
//         }
//         //电机解锁
//         errorSub = BoardFPGA_StepMotorAndEncoderState(motorNo,DISABLE,DISABLE);
//         if(errorSub != ERROR_SUB_OK)
//         {
//             MCU_LogShowErrorSubCode(errorSub);
//             return errorSub;
//         }
//     }
//     //输出日志
//     MCU_LogPrintf("Motor RunDacc, Test Success Over,TimeUse: %d ms\r\n",OS_TICK_TO_MS(tx_time_get() - startTimeTick));
//     return ERROR_SUB_OK;
// }