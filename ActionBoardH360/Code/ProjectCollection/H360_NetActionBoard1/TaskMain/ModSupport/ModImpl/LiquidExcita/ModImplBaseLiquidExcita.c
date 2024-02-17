/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-24 15:09:16 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-12-06 09:14:45 +0800
************************************************************************************************/ 
#include "ModImplBaseLiquidExcita.h"
#include "ModTaskHeader.h"


//输入单元列表 输入序号 有效电平
const MODULE_INPUT_MAP_UNIT liquidExcitaPortInConfigArray[] = {
    {MODULE_BASE_INPUT_PIN_RESERVE,VALID_LEVEL_MODULE_BASE_INPUT_PIN_RESERVE},
};
#define LENGTH_MODULE_LIQUID_EXCITA_PORT_IN_CONFIG_ARRAY       DIM_ARRAY_ELEMENT_COUNT(liquidExcitaPortInConfigArray)

//输出单元列表 输入序号 有效电平
const MODULE_OUTPUT_MAP_UNIT liquidExcitaPortOutConfigArray[] = {
    {MODULE_BASE_OUTPUT_PIN_RESERVE,VALID_LEVEL_MODULE_BASE_OUTPUT_PIN_RESERVE},
};
#define LENGTH_MODULE_LIQUID_EXCITA_PORT_OUT_CONFIG_ARRAY       DIM_ARRAY_ELEMENT_COUNT(liquidExcitaPortOutConfigArray)


//电机控制列表
const MODULE_STEP_MOTOR_CONFIG liquidExcitaStepMotorConfigArray[] = {
    {MODULE_BASE_STEP_MOTOR_NO_RESERVE,60000,MODULE_BASE_STEP_MOTOR_RESET_CORRECT_PAR_MAIN,MODULE_BASE_STEP_MOTOR_RESET_CORRECT_PAR_SUB},
};
#define LENGTH_MODULE_LIQUID_EXCITA_STEP_MOTOR_CONFIG_ARRAY     DIM_ARRAY_ELEMENT_COUNT(liquidExcitaStepMotorConfigArray)

//读取模块输入
static ERROR_SUB ModImplBaseLiquidExcitaReadPortInSub(uint8_t portIdx,PIN_STATE* pinStatePtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //调用基础实现
    errorSub = H360SysActModInputRead(portIdx,LENGTH_MODULE_LIQUID_EXCITA_PORT_IN_CONFIG_ARRAY,liquidExcitaPortInConfigArray,
                                        pinStatePtr);
    return errorSub;
}

//写入模块输出
static ERROR_SUB ModImplBaseLiquidExcitaWritePortOutSub(uint8_t portIdx,PIN_STATE setState)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //调用基础实现
    errorSub = H360SysActModOutputWrite(portIdx,setState,LENGTH_MODULE_LIQUID_EXCITA_PORT_OUT_CONFIG_ARRAY,liquidExcitaPortOutConfigArray);
    return errorSub;
}

//步进电机复位
static ERROR_SUB ModImplBaseLiquidExcitaStepMotorResetSub(uint8_t stepMotorIdx,int32_t* motorCoordinatePos)
{
    return CommonSrvImplBaseModuleStepMotorReset(stepMotorIdx,liquidExcitaStepMotorConfigArray,
                                                    LENGTH_MODULE_LIQUID_EXCITA_STEP_MOTOR_CONFIG_ARRAY,
                                                    motorCoordinatePos);
}

//步进电机走位
static ERROR_SUB ModImplBaseLiquidExcitaStepMotorRunStepsSub(uint8_t stepMotorIdx,int32_t runSteps,int32_t* motorCoordinatePos)
{
    return CommonSrvImplBaseModuleStepMotorRunSteps(stepMotorIdx,runSteps,liquidExcitaStepMotorConfigArray,
                                                        LENGTH_MODULE_LIQUID_EXCITA_STEP_MOTOR_CONFIG_ARRAY,
                                                        motorCoordinatePos);
}

//读取模块输入
void ModImplBaseLiquidExcitaReadPortIn(uint8_t portIdx,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    *dataBufLen = 2;
    *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_MOD_TASK,(*dataBufLen));
    ERROR_SUB errorSub = ERROR_SUB_OK;
    PIN_STATE readInputState = PIN_STATE_INVALID;
    //返回数据赋值
    (*dataBufPtrPtr)[0] = portIdx;
    (*dataBufPtrPtr)[1] = readInputState;
    //读取输入
    errorSub = ModImplBaseLiquidExcitaReadPortInSub(portIdx,&readInputState);
    if(errorSub != ERROR_SUB_OK)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_LIQUID_EXCITA_READ_MOD_INPUT,
                                        ERROR_LEVEL_ERROR,errorSub);
        return;
    }
    //返回数据赋值
    (*dataBufPtrPtr)[1] = readInputState;
    //构建完成无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

//写入模块输出
void ModImplBaseLiquidExcitaWritePortOut(uint8_t portIdx,PIN_STATE setState,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    *dataBufLen = 1;
    *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_MOD_TASK,(*dataBufLen));
    ERROR_SUB errorSub = ERROR_SUB_OK;
    PIN_STATE readInputState = PIN_STATE_INVALID;
    //返回数据赋值
    (*dataBufPtrPtr)[0] = portIdx;
    //写入输出
    errorSub = ModImplBaseLiquidExcitaWritePortOutSub(portIdx,setState);
    if(errorSub != ERROR_SUB_OK)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_LIQUID_EXCITA_WRITE_MOD_OUTPUT,
                                        ERROR_LEVEL_ERROR,errorSub);
        return;
    }
    //构建完成无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

//步进电机复位
void ModImplBaseLiquidExcitaStepMotorReset(uint8_t stepMotorIdx,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    *dataBufLen = 5;
    *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_MOD_TASK,(*dataBufLen));
    ERROR_SUB errorSub = ERROR_SUB_OK;
    int32_t motorPosAfterReset = 0;
    //返回数据赋值
    (*dataBufPtrPtr)[0] = stepMotorIdx;
    ProtUtilWriteInt32ToBuffer(motorPosAfterReset,(*dataBufPtrPtr)+1);
    //电机复位
    errorSub = ModImplBaseLiquidExcitaStepMotorResetSub(stepMotorIdx,&motorPosAfterReset);
    if(errorSub != ERROR_SUB_OK)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_LIQUID_EXCITA_STEP_MOTOR_RESET,
                                        ERROR_LEVEL_ERROR,errorSub);
        return;
    }
    //写入参数
    ProtUtilWriteInt32ToBuffer(motorPosAfterReset,(*dataBufPtrPtr)+1);
    //构建完成无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

//步进电机走位
void ModImplBaseLiquidExcitaStepMotorRunSteps(uint8_t stepMotorIdx,int32_t runSteps,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    *dataBufLen = 5;
    *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_MOD_TASK,(*dataBufLen));
    ERROR_SUB errorSub = ERROR_SUB_OK;
    int32_t motorPosAfterRun = 0;
    //返回数据赋值
    (*dataBufPtrPtr)[0] = stepMotorIdx;
    ProtUtilWriteInt32ToBuffer(motorPosAfterRun,(*dataBufPtrPtr)+1);
    //电机走位
    errorSub = ModImplBaseLiquidExcitaStepMotorRunStepsSub(stepMotorIdx,runSteps,&motorPosAfterRun);
    if(errorSub != ERROR_SUB_OK)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_LIQUID_EXCITA_STEP_MOTOR_RUN_STEP,
                                        ERROR_LEVEL_ERROR,errorSub);
        return;
    }
    //写入参数
    ProtUtilWriteInt32ToBuffer(motorPosAfterRun,(*dataBufPtrPtr)+1);
    //构建完成无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

//复位
void ModImplBaseLiquidExcitaReset(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //设置返回值
    *dataBufLen = 0;
    *dataBufPtrPtr = NULL;
    //老化模式不干活
    SYS_STATE_MAIN_CONTROL_MSG* mainControlMsgPtr = SysStateGetMainControlMsgPtr();
    if(mainControlMsgPtr->sysRunningMode == SYS_RUNNING_MODE_DEBUG)
    {
        //构建完成无错误
        CoreUtilSetSystemResultPackNoError(resultPackPtr);
        return;
    }
    //测试模式实现
    //关闭V308
    ModLiquidUtilCloseValveV308();
    //关闭V203
    ModLiquidUtilCloseValveV203();
    //关闭V204
    ModLiquidUtilCloseValveV204();
    //关闭DP2
    ModLiquidUtilClosePumpDP2();
    //关闭V215
    ModLiquidUtilCloseValveV215();
    //构建完成无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

//打开正常吸液通道
void ModImplBaseLiquidExcitaOpenNormalAbsorb(BOTTLE_LIQUID_EXCITE bottleSelect,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //设置返回值
    *dataBufLen = 0;
    *dataBufPtrPtr = NULL;
    //老化模式不干活
    SYS_STATE_MAIN_CONTROL_MSG* mainControlMsgPtr = SysStateGetMainControlMsgPtr();
    if(mainControlMsgPtr->sysRunningMode == SYS_RUNNING_MODE_DEBUG)
    {
        //构建完成无错误
        CoreUtilSetSystemResultPackNoError(resultPackPtr);
        return;
    }
    //测试模式实现
    //关闭V215
    ModLiquidUtilCloseValveV215();
    //延时等待响应
    ModLiquidUtilWaitValveStable();
    //根据选瓶决定
    if(bottleSelect == BOTTLE_LIQUID_EXCITE_A)
    {
        //选A瓶
        ModLiquidUtilValveV203SelectBottle1();
    }
    else
    {
        //选B瓶
        ModLiquidUtilValveV203SelectBottle2();
    }
    //延时等待响应
    ModLiquidUtilWaitValveStable();
    //打开注液液路
    ModLiquidUtilOpenValveV204();
    //延时等待响应
    ModLiquidUtilWaitValveStable();
    //构建完成无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

//打开清洗维护吸液通道
void ModImplBaseLiquidExcitaOpenMaintainAbsorb(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //设置返回值
    *dataBufLen = 0;
    *dataBufPtrPtr = NULL;
    //老化模式不干活
    SYS_STATE_MAIN_CONTROL_MSG* mainControlMsgPtr = SysStateGetMainControlMsgPtr();
    if(mainControlMsgPtr->sysRunningMode == SYS_RUNNING_MODE_DEBUG)
    {
        //构建完成无错误
        CoreUtilSetSystemResultPackNoError(resultPackPtr);
        return;
    }
    //测试模式实现
    ModLiquidUtilCloseValveV215();
    //选通纯水瓶
    ModLiquidUtilOpenValveV308();
    //关闭V203
    ModLiquidUtilCloseValveV203();
    //延时等待响应
    ModLiquidUtilWaitValveStable();
    //打开注液液路
    ModLiquidUtilOpenValveV204();
    //延时等待响应
    ModLiquidUtilWaitValveStable();
    //构建完成无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

//关闭液路通道
void ModImplBaseLiquidExcitaClose(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //设置返回值
    *dataBufLen = 0;
    *dataBufPtrPtr = NULL;
    //老化模式不干活
    SYS_STATE_MAIN_CONTROL_MSG* mainControlMsgPtr = SysStateGetMainControlMsgPtr();
    if(mainControlMsgPtr->sysRunningMode == SYS_RUNNING_MODE_DEBUG)
    {
        //构建完成无错误
        CoreUtilSetSystemResultPackNoError(resultPackPtr);
        return;
    }
    //测试模式实现
    //关闭纯水选通阀
    ModLiquidUtilCloseValveV308();
    //关闭V203
    ModLiquidUtilCloseValveV203();
    //延时等待响应
    ModLiquidUtilWaitValveStable();
    //关闭V204
    ModLiquidUtilCloseValveV204();
    //延时等待响应
    ModLiquidUtilWaitValveStable();
    //关闭V215
    ModLiquidUtilCloseValveV215();
    //延时等待响应
    ModLiquidUtilWaitValveStable();
    //构建完成无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

//设置排放废液
void ModImplBaseLiquidExcitaSetClearWaste(LIQUID_EXCITE_WASTE_STATE wasteStateSet,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //设置返回值
    *dataBufLen = 0;
    *dataBufPtrPtr = NULL;
    //老化模式不干活
    SYS_STATE_MAIN_CONTROL_MSG* mainControlMsgPtr = SysStateGetMainControlMsgPtr();
    if(mainControlMsgPtr->sysRunningMode == SYS_RUNNING_MODE_DEBUG)
    {
        //构建完成无错误
        CoreUtilSetSystemResultPackNoError(resultPackPtr);
        return;
    }
    //测试模式实现
    if(wasteStateSet == LIQUID_EXCITE_WASTE_STATE_ON)
    {
        //打开DP2
        ModLiquidUtilOpenPumpDP2();
    }
    else
    {
        //关闭DP2
        ModLiquidUtilClosePumpDP2();
    }
    //构建完成无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

//打开注液通路
void ModImplBaseLiquidExcitaOpenInject(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //设置返回值
    *dataBufLen = 0;
    *dataBufPtrPtr = NULL;
    //老化模式不干活
    SYS_STATE_MAIN_CONTROL_MSG* mainControlMsgPtr = SysStateGetMainControlMsgPtr();
    if(mainControlMsgPtr->sysRunningMode == SYS_RUNNING_MODE_DEBUG)
    {
        //构建完成无错误
        CoreUtilSetSystemResultPackNoError(resultPackPtr);
        return;
    }
    //测试模式实现
    //关闭V203
    ModLiquidUtilCloseValveV203();
    //延时等待响应
    ModLiquidUtilWaitValveStable();
    //关闭V204
    ModLiquidUtilCloseValveV204();
    //延时等待响应
    ModLiquidUtilWaitValveStable();
    //打开V215
    ModLiquidUtilOpenValveV215();
    //延时等待响应
    ModLiquidUtilWaitValveStable();
    //构建完成无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}





