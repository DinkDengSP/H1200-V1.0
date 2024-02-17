/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-24 15:09:16 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-12-05 08:41:20 +0800
************************************************************************************************/ 
#include "ModImplBaseLiquidNeedleReagent.h"
#include "ModTaskHeader.h"


//输入单元列表 输入序号 有效电平
const MODULE_INPUT_MAP_UNIT liquidNeedleReagentPortInConfigArray[] = {
    {MODULE_BASE_INPUT_PIN_RESERVE,VALID_LEVEL_MODULE_BASE_INPUT_PIN_RESERVE},
};
#define LENGTH_MODULE_LIQUID_NEEDLE_REAGENT_PORT_IN_CONFIG_ARRAY       DIM_ARRAY_ELEMENT_COUNT(liquidNeedleReagentPortInConfigArray)

//输出单元列表 输入序号 有效电平
const MODULE_OUTPUT_MAP_UNIT liquidNeedleReagentPortOutConfigArray[] = {
    {MODULE_BASE_OUTPUT_PIN_RESERVE,VALID_LEVEL_MODULE_BASE_OUTPUT_PIN_RESERVE},
};
#define LENGTH_MODULE_LIQUID_NEEDLE_REAGENT_PORT_OUT_CONFIG_ARRAY       DIM_ARRAY_ELEMENT_COUNT(liquidNeedleReagentPortOutConfigArray)


//电机控制列表
const MODULE_STEP_MOTOR_CONFIG liquidNeedleReagentStepMotorConfigArray[] = {
    {MODULE_BASE_STEP_MOTOR_NO_RESERVE,60000,MODULE_BASE_STEP_MOTOR_RESET_CORRECT_PAR_MAIN,MODULE_BASE_STEP_MOTOR_RESET_CORRECT_PAR_SUB},
};
#define LENGTH_MODULE_LIQUID_NEEDLE_REAGENT_STEP_MOTOR_CONFIG_ARRAY     DIM_ARRAY_ELEMENT_COUNT(liquidNeedleReagentStepMotorConfigArray)

//读取模块输入
static ERROR_SUB ModImplBaseLiquidNeedleReagentReadPortInSub(uint8_t portIdx,PIN_STATE* pinStatePtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //调用基础实现
    errorSub = H360SysActModInputRead(portIdx,LENGTH_MODULE_LIQUID_NEEDLE_REAGENT_PORT_IN_CONFIG_ARRAY,liquidNeedleReagentPortInConfigArray,
                                        pinStatePtr);
    return errorSub;
}

//写入模块输出
static ERROR_SUB ModImplBaseLiquidNeedleReagentWritePortOutSub(uint8_t portIdx,PIN_STATE setState)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //调用基础实现
    errorSub = H360SysActModOutputWrite(portIdx,setState,LENGTH_MODULE_LIQUID_NEEDLE_REAGENT_PORT_OUT_CONFIG_ARRAY,liquidNeedleReagentPortOutConfigArray);
    return errorSub;
}

//步进电机复位
static ERROR_SUB ModImplBaseLiquidNeedleReagentStepMotorResetSub(uint8_t stepMotorIdx,int32_t* motorCoordinatePos)
{
    return CommonSrvImplBaseModuleStepMotorReset(stepMotorIdx,liquidNeedleReagentStepMotorConfigArray,
                                                    LENGTH_MODULE_LIQUID_NEEDLE_REAGENT_STEP_MOTOR_CONFIG_ARRAY,
                                                    motorCoordinatePos);
}

//步进电机走位
static ERROR_SUB ModImplBaseLiquidNeedleReagentStepMotorRunStepsSub(uint8_t stepMotorIdx,int32_t runSteps,int32_t* motorCoordinatePos)
{
    return CommonSrvImplBaseModuleStepMotorRunSteps(stepMotorIdx,runSteps,liquidNeedleReagentStepMotorConfigArray,
                                                        LENGTH_MODULE_LIQUID_NEEDLE_REAGENT_STEP_MOTOR_CONFIG_ARRAY,
                                                        motorCoordinatePos);
}

//读取模块输入
void ModImplBaseLiquidNeedleReagentReadPortIn(uint8_t portIdx,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    *dataBufLen = 2;
    *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_MOD_TASK,(*dataBufLen));
    ERROR_SUB errorSub = ERROR_SUB_OK;
    PIN_STATE readInputState = PIN_STATE_INVALID;
    //返回数据赋值
    (*dataBufPtrPtr)[0] = portIdx;
    (*dataBufPtrPtr)[1] = readInputState;
    //读取输入
    errorSub = ModImplBaseLiquidNeedleReagentReadPortInSub(portIdx,&readInputState);
    if(errorSub != ERROR_SUB_OK)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_LIQUID_NEEDLE_REAGENT_READ_MOD_INPUT,
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
void ModImplBaseLiquidNeedleReagentWritePortOut(uint8_t portIdx,PIN_STATE setState,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    *dataBufLen = 1;
    *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_MOD_TASK,(*dataBufLen));
    ERROR_SUB errorSub = ERROR_SUB_OK;
    PIN_STATE readInputState = PIN_STATE_INVALID;
    //返回数据赋值
    (*dataBufPtrPtr)[0] = portIdx;
    //写入输出
    errorSub = ModImplBaseLiquidNeedleReagentWritePortOutSub(portIdx,setState);
    if(errorSub != ERROR_SUB_OK)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_LIQUID_NEEDLE_REAGENT_WRITE_MOD_OUTPUT,
                                        ERROR_LEVEL_ERROR,errorSub);
        return;
    }
    //构建完成无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

//步进电机复位
void ModImplBaseLiquidNeedleReagentStepMotorReset(uint8_t stepMotorIdx,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    *dataBufLen = 5;
    *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_MOD_TASK,(*dataBufLen));
    ERROR_SUB errorSub = ERROR_SUB_OK;
    int32_t motorPosAfterReset = 0;
    //返回数据赋值
    (*dataBufPtrPtr)[0] = stepMotorIdx;
    ProtUtilWriteInt32ToBuffer(motorPosAfterReset,(*dataBufPtrPtr)+1);
    //电机复位
    errorSub = ModImplBaseLiquidNeedleReagentStepMotorResetSub(stepMotorIdx,&motorPosAfterReset);
    if(errorSub != ERROR_SUB_OK)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_LIQUID_NEEDLE_REAGENT_STEP_MOTOR_RESET,
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
void ModImplBaseLiquidNeedleReagentStepMotorRunSteps(uint8_t stepMotorIdx,int32_t runSteps,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    *dataBufLen = 5;
    *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_MOD_TASK,(*dataBufLen));
    ERROR_SUB errorSub = ERROR_SUB_OK;
    int32_t motorPosAfterRun = 0;
    //返回数据赋值
    (*dataBufPtrPtr)[0] = stepMotorIdx;
    ProtUtilWriteInt32ToBuffer(motorPosAfterRun,(*dataBufPtrPtr)+1);
    //电机走位
    errorSub = ModImplBaseLiquidNeedleReagentStepMotorRunStepsSub(stepMotorIdx,runSteps,&motorPosAfterRun);
    if(errorSub != ERROR_SUB_OK)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_LIQUID_NEEDLE_REAGENT_STEP_MOTOR_RUN_STEP,
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
void ModImplBaseLiquidNeedleReagentReset(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
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
    //测试模式
    //关闭内针清洗泵
    ModLiquidUtilClosePumpDP306();
    //关闭外针清洗泵
    ModLiquidUtilClosePumpDP305();
    //等待操作
    ModLiquidUtilWaitPumpStable();
    //关闭内针三通阀
    ModLiquidUtilCloseValveV309();
    //关闭外针三通阀
    ModLiquidUtilCloseValveV232();
    //关闭内针清洗阀
    ModLiquidUtilCloseValveV219();
    //关闭外针清洗阀
    ModLiquidUtilCloseValveV216();
    //等待操作
    ModLiquidUtilWaitValveStable();
    //构建完成无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

//打开清洗液路
void ModImplBaseLiquidNeedleReagentOpenClean(NEEDLE_CLEAN_OPT opt,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
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
    //测试模式
    //先开阀后开泵
    if((opt&NEEDLE_CLEAN_OPT_IN) != 0)
    {
        //打开内针清洗阀
        ModLiquidUtilOpenValveV219();
        //V309切换清洗液
        ModLiquidUtilValveV309SwitchLiquidClean();
    }
    if((opt&NEEDLE_CLEAN_OPT_OUT) != 0)
    {
        //打开外针清洗阀
        ModLiquidUtilOpenValveV216();
        //V232切换纯水
        ModLiquidUtilValveV232SwitchPureWater();
    }
    //等待响应
    ModLiquidUtilWaitValveStable();
    if((opt&NEEDLE_CLEAN_OPT_IN) != 0)
    {
        //打开内针清洗泵
        ModLiquidUtilOpenPumpDP306();
    }
    if((opt&NEEDLE_CLEAN_OPT_OUT) != 0)
    {
        //打开外针清洗泵
        ModLiquidUtilOpenPumpDP305();
    }
    //等待响应
    ModLiquidUtilWaitPumpStable();
    //构建完成无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

//关闭清洗液路
void ModImplBaseLiquidNeedleReagentCloseClean(NEEDLE_CLEAN_OPT opt,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
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
    //测试模式
    //先关泵后关阀
    if((opt&NEEDLE_CLEAN_OPT_IN) != 0)
    {
        //关闭内针清洗泵
        ModLiquidUtilClosePumpDP306();
    }
    if((opt&NEEDLE_CLEAN_OPT_OUT) != 0)
    {
        //关闭外针清洗泵
        ModLiquidUtilClosePumpDP305();
    }
    //等待响应
    ModLiquidUtilWaitPumpStable();
    if((opt&NEEDLE_CLEAN_OPT_IN) != 0)
    {
        //关闭内针清洗阀
        ModLiquidUtilCloseValveV219();
        //关闭内针三通
        ModLiquidUtilCloseValveV309();
    }
    if((opt&NEEDLE_CLEAN_OPT_OUT) != 0)
    {
        //关闭外针清洗阀
        ModLiquidUtilCloseValveV216();
        //关闭外针三通
        ModLiquidUtilCloseValveV232();
    }
    //等待响应
    ModLiquidUtilWaitValveStable();
    //构建完成无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

//打开灌注液路
void ModImplBaseLiquidNeedleReagentOpenPrime(NEEDLE_PRIME_OPT opt,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
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
    //测试模式
    //先开阀后开泵
    if((opt&NEEDLE_PRIME_OPT_IN) != 0)
    {
        //打开内针清洗阀
        ModLiquidUtilOpenValveV219();
        //V309切换清洗液
        ModLiquidUtilValveV309SwitchLiquidClean();
    }
    if((opt&NEEDLE_PRIME_OPT_OUT) != 0)
    {
        //打开外针清洗阀
        ModLiquidUtilOpenValveV216();
        //V232切换纯水
        ModLiquidUtilValveV232SwitchPureWater();
    }
    //等待响应
    ModLiquidUtilWaitValveStable();
    if((opt&NEEDLE_PRIME_OPT_IN) != 0)
    {
        //打开内针清洗泵
        ModLiquidUtilOpenPumpDP306();
    }
    if((opt&NEEDLE_PRIME_OPT_OUT) != 0)
    {
        //打开外针清洗泵
        ModLiquidUtilOpenPumpDP305();
    }
    //等待响应
    ModLiquidUtilWaitPumpStable();
    //构建完成无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

//关闭灌注液路
void ModImplBaseLiquidNeedleReagentClosePrime(NEEDLE_PRIME_OPT opt,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
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
    //测试模式
    //先关泵后关阀
    if((opt&NEEDLE_PRIME_OPT_IN) != 0)
    {
        //关闭内针清洗泵
        ModLiquidUtilClosePumpDP306();
    }
    if((opt&NEEDLE_PRIME_OPT_OUT) != 0)
    {
        //关闭外针清洗泵
        ModLiquidUtilClosePumpDP305();
    }
    //等待响应
    ModLiquidUtilWaitPumpStable();
    if((opt&NEEDLE_PRIME_OPT_IN) != 0)
    {
        //关闭内针清洗阀
        ModLiquidUtilCloseValveV219();
        //关闭内针三通
        ModLiquidUtilCloseValveV309();
    }
    if((opt&NEEDLE_PRIME_OPT_OUT) != 0)
    {
        //关闭外针清洗阀
        ModLiquidUtilCloseValveV216();
        //关闭外针三通
        ModLiquidUtilCloseValveV232();
    }
    //等待响应
    ModLiquidUtilWaitValveStable();
    //构建完成无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

//打开清洗维护液路
void ModImplBaseLiquidNeedleReagentOpenMaintain(NEEDLE_MAINTAIN_OPT opt,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
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
    //测试模式
    //先开阀后开泵
    if((opt&NEEDLE_MAINTAIN_OPT_IN) != 0)
    {
        //打开内针清洗阀
        ModLiquidUtilOpenValveV219();
        //V309切换纯水
        ModLiquidUtilValveV309SwitchLiquidPureWater();
    }
    if((opt&NEEDLE_MAINTAIN_OPT_OUT) != 0)
    {
        //打开外针清洗阀
        ModLiquidUtilOpenValveV216();
        //V232切换纯水
        ModLiquidUtilValveV232SwitchPureWater();
    }
    //等待响应
    ModLiquidUtilWaitValveStable();
    if((opt&NEEDLE_MAINTAIN_OPT_IN) != 0)
    {
        //打开内针清洗泵
        ModLiquidUtilOpenPumpDP305();
    }
    if((opt&NEEDLE_MAINTAIN_OPT_OUT) != 0)
    {
        //打开外针清洗泵
        ModLiquidUtilOpenPumpDP305();
    }
    //等待响应
    ModLiquidUtilWaitPumpStable();
    //构建完成无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

//关闭清洗维护液路
void ModImplBaseLiquidNeedleReagentCloseMaintain(NEEDLE_MAINTAIN_OPT opt,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
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
    //测试模式
    //先关泵后关阀
    if((opt&NEEDLE_MAINTAIN_OPT_IN) != 0)
    {
        //关闭内针清洗泵
        ModLiquidUtilClosePumpDP305();
    }
    if((opt&NEEDLE_MAINTAIN_OPT_OUT) != 0)
    {
        //关闭外针清洗泵
        ModLiquidUtilClosePumpDP305();
    }
    //等待响应
    ModLiquidUtilWaitPumpStable();
    if((opt&NEEDLE_MAINTAIN_OPT_IN) != 0)
    {
        //关闭内针清洗阀
        ModLiquidUtilCloseValveV219();
        //关闭内针三通
        ModLiquidUtilCloseValveV309();
    }
    if((opt&NEEDLE_MAINTAIN_OPT_OUT) != 0)
    {
        //关闭外针清洗阀
        ModLiquidUtilCloseValveV216();
        //关闭外针三通
        ModLiquidUtilCloseValveV232();
    }
    //等待响应
    ModLiquidUtilWaitValveStable();
    //构建完成无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}




