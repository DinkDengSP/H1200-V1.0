/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-27 17:08:07 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-12-06 14:20:25 +0800
************************************************************************************************/ 
#include "ModImplBaseMixReagent.h"
#include "ModTaskHeader.h"


//输入单元列表 输入序号 有效电平
const MODULE_INPUT_MAP_UNIT mixReagentPortInConfigArray[] = {
    {INPUT_MIX_REAGENT_UPDOWN_ORGIN,VALID_LEVEL_INPUT_MIX_REAGENT_UPDOWN_ORGIN},//升降电机原点
    {INPUT_MIX_REAGENT_ROTATE_ORGIN,VALID_LEVEL_INPUT_MIX_REAGENT_ROTATE_ORGIN},//混匀电机原点
};
#define LENGTH_MODULE_MIX_REAGENT_PORT_IN_CONFIG_ARRAY       DIM_ARRAY_ELEMENT_COUNT(mixReagentPortInConfigArray)

//输出单元列表 输入序号 有效电平
const MODULE_OUTPUT_MAP_UNIT mixReagentPortOutConfigArray[] = {
    {MODULE_BASE_OUTPUT_PIN_RESERVE,VALID_LEVEL_MODULE_BASE_OUTPUT_PIN_RESERVE},
};
#define LENGTH_MODULE_MIX_REAGENT_PORT_OUT_CONFIG_ARRAY       DIM_ARRAY_ELEMENT_COUNT(mixReagentPortOutConfigArray)


//电机控制列表
const MODULE_STEP_MOTOR_CONFIG mixReagentStepMotorConfigArray[] = {
    {MIX_REAGENT_STEP_MOTOR_UPDOWN_SYS_ID,60000,INDEX_MAIN_SYS_PARAM_MIX_REAGENT,INDEX_SUB_PARAM_MIX_REAGENT_RESET_CORRECT_UPDOWN},//升降
    {MIX_REAGENT_STEP_MOTOR_ROTATE_SYS_ID,60000,INDEX_MAIN_SYS_PARAM_MIX_REAGENT,INDEX_SUB_PARAM_MIX_REAGENT_RESET_CORRECT_ROTATE},//旋转
};
#define LENGTH_MODULE_MIX_REAGENT_STEP_MOTOR_CONFIG_ARRAY     DIM_ARRAY_ELEMENT_COUNT(mixReagentStepMotorConfigArray)

//读取模块输入
static ERROR_SUB ModImplBaseMixReagentReadPortInSub(uint8_t portIdx,PIN_STATE* pinStatePtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //调用基础实现
    errorSub = H360SysActModInputRead(portIdx,LENGTH_MODULE_MIX_REAGENT_PORT_IN_CONFIG_ARRAY,mixReagentPortInConfigArray,
                                        pinStatePtr);
    return errorSub;
}

//写入模块输出
static ERROR_SUB ModImplBaseMixReagentWritePortOutSub(uint8_t portIdx,PIN_STATE setState)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //调用基础实现
    errorSub = H360SysActModOutputWrite(portIdx,setState,LENGTH_MODULE_MIX_REAGENT_PORT_OUT_CONFIG_ARRAY,mixReagentPortOutConfigArray);
    return errorSub;
}

//步进电机复位
static ERROR_SUB ModImplBaseMixReagentStepMotorResetSub(uint8_t stepMotorIdx,int32_t* motorCoordinatePos)
{
    return CommonSrvImplBaseModuleStepMotorReset(stepMotorIdx,mixReagentStepMotorConfigArray,
                                                    LENGTH_MODULE_MIX_REAGENT_STEP_MOTOR_CONFIG_ARRAY,
                                                    motorCoordinatePos);
}

//步进电机走位
static ERROR_SUB ModImplBaseMixReagentStepMotorRunStepsSub(uint8_t stepMotorIdx,int32_t runSteps,int32_t* motorCoordinatePos)
{
    return CommonSrvImplBaseModuleStepMotorRunSteps(stepMotorIdx,runSteps,mixReagentStepMotorConfigArray,
                                                        LENGTH_MODULE_MIX_REAGENT_STEP_MOTOR_CONFIG_ARRAY,
                                                        motorCoordinatePos);
}

//读取模块输入
void ModImplBaseMixReagentReadPortIn(uint8_t portIdx,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    *dataBufLen = 2;
    *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_MOD_TASK,(*dataBufLen));
    ERROR_SUB errorSub = ERROR_SUB_OK;
    PIN_STATE readInputState = PIN_STATE_INVALID;
    //返回数据赋值
    (*dataBufPtrPtr)[0] = portIdx;
    (*dataBufPtrPtr)[1] = readInputState;
    //读取输入
    errorSub = ModImplBaseMixReagentReadPortInSub(portIdx,&readInputState);
    if(errorSub != ERROR_SUB_OK)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_MIX_REAGENT_READ_MOD_INPUT,
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
void ModImplBaseMixReagentWritePortOut(uint8_t portIdx,PIN_STATE setState,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    *dataBufLen = 1;
    *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_MOD_TASK,(*dataBufLen));
    ERROR_SUB errorSub = ERROR_SUB_OK;
    PIN_STATE readInputState = PIN_STATE_INVALID;
    //返回数据赋值
    (*dataBufPtrPtr)[0] = portIdx;
    //写入输出
    errorSub = ModImplBaseMixReagentWritePortOutSub(portIdx,setState);
    if(errorSub != ERROR_SUB_OK)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_MIX_REAGENT_WRITE_MOD_OUTPUT,
                                        ERROR_LEVEL_ERROR,errorSub);
        return;
    }
    //构建完成无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

//步进电机复位
void ModImplBaseMixReagentStepMotorReset(uint8_t stepMotorIdx,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    *dataBufLen = 5;
    *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_MOD_TASK,(*dataBufLen));
    ERROR_SUB errorSub = ERROR_SUB_OK;
    int32_t motorPosAfterReset = 0;
    //返回数据赋值
    (*dataBufPtrPtr)[0] = stepMotorIdx;
    ProtUtilWriteInt32ToBuffer(motorPosAfterReset,(*dataBufPtrPtr)+1);
    //电机复位
    errorSub = ModImplBaseMixReagentStepMotorResetSub(stepMotorIdx,&motorPosAfterReset);
    if(errorSub != ERROR_SUB_OK)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_MIX_REAGENT_STEP_MOTOR_RESET,
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
void ModImplBaseMixReagentStepMotorRunSteps(uint8_t stepMotorIdx,int32_t runSteps,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    *dataBufLen = 5;
    *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_MOD_TASK,(*dataBufLen));
    ERROR_SUB errorSub = ERROR_SUB_OK;
    int32_t motorPosAfterRun = 0;
    //返回数据赋值
    (*dataBufPtrPtr)[0] = stepMotorIdx;
    ProtUtilWriteInt32ToBuffer(motorPosAfterRun,(*dataBufPtrPtr)+1);
    //电机走位
    errorSub = ModImplBaseMixReagentStepMotorRunStepsSub(stepMotorIdx,runSteps,&motorPosAfterRun);
    if(errorSub != ERROR_SUB_OK)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_MIX_REAGENT_STEP_MOTOR_RUN_STEP,
                                        ERROR_LEVEL_ERROR,errorSub);
        return;
    }
    //写入参数
    ProtUtilWriteInt32ToBuffer(motorPosAfterRun,(*dataBufPtrPtr)+1);
    //构建完成无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

/*-------------------------------------清洗混匀上升---------------------------------------*/
void ModImplMixReagentUpMode1(SYS_RESULT_PACK* resultPackPtr)
{
    //参数配置
    SYS_RESULT_PACK sysResultPack;
    //参数辅助,用于系统参数读取
    int32_t paramUtil;
    //走坐标
    IPC_STEP_MOTOR_CMD_RUN_COORDINATE stepMotorRunCoordinateCmd;
    //读取升起来的高度
    SysParamReadSingle(INDEX_MAIN_SYS_PARAM_MIX_REAGENT,INDEX_SUB_PARAM_MIX_REAGENT_UP_POS_MODE1,&paramUtil);
    //走坐标指令初始化
    IPC_StepMotorBaseSetRunCoordinateCmdDefault(&stepMotorRunCoordinateCmd);
    stepMotorRunCoordinateCmd.runStepCurve = S_CURVE_MIX_REAGENT_UPDOWN_UP_MODE1;
    stepMotorRunCoordinateCmd.targetCoordinate = paramUtil;
    stepMotorRunCoordinateCmd.timeOutMs = 5000,
    //电机抬起来
    IPC_SrvStepMotorRunCoordinateWhileReturn(MIX_REAGENT_STEP_MOTOR_UPDOWN_LOCAL_ID,&stepMotorRunCoordinateCmd,&sysResultPack,NULL);
    if(sysResultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_MIX_REAGENT_UP_MODE1_RUN_COORDINATE,
                                        ERROR_LEVEL_ERROR,sysResultPack.errorSub);
        return;
    }
    //构建完成无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

void ModImplMixReagentUpMode2(SYS_RESULT_PACK* resultPackPtr)
{
    //参数配置
    SYS_RESULT_PACK sysResultPack;
    //参数辅助,用于系统参数读取
    int32_t paramUtil;
    //走坐标
    IPC_STEP_MOTOR_CMD_RUN_COORDINATE stepMotorRunCoordinateCmd;
    //读取升起来的高度
    SysParamReadSingle(INDEX_MAIN_SYS_PARAM_MIX_REAGENT,INDEX_SUB_PARAM_MIX_REAGENT_UP_POS_MODE2,&paramUtil);
    //走坐标指令初始化
    IPC_StepMotorBaseSetRunCoordinateCmdDefault(&stepMotorRunCoordinateCmd);
    stepMotorRunCoordinateCmd.runStepCurve = S_CURVE_MIX_REAGENT_UPDOWN_UP_MODE2;
    stepMotorRunCoordinateCmd.targetCoordinate = paramUtil;
    stepMotorRunCoordinateCmd.timeOutMs = 5000,
    //电机抬起来
    IPC_SrvStepMotorRunCoordinateWhileReturn(MIX_REAGENT_STEP_MOTOR_UPDOWN_LOCAL_ID,&stepMotorRunCoordinateCmd,&sysResultPack,NULL);
    if(sysResultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_MIX_REAGENT_UP_MODE2_RUN_COORDINATE,
                                        ERROR_LEVEL_ERROR,sysResultPack.errorSub);
        return;
    }
    //构建完成无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

void ModImplMixReagentUpMode3(SYS_RESULT_PACK* resultPackPtr)
{
    //参数配置
    SYS_RESULT_PACK sysResultPack;
    //参数辅助,用于系统参数读取
    int32_t paramUtil;
    //走坐标
    IPC_STEP_MOTOR_CMD_RUN_COORDINATE stepMotorRunCoordinateCmd;
    //读取升起来的高度
    SysParamReadSingle(INDEX_MAIN_SYS_PARAM_MIX_REAGENT,INDEX_SUB_PARAM_MIX_REAGENT_UP_POS_MODE3,&paramUtil);
    //走坐标指令初始化
    IPC_StepMotorBaseSetRunCoordinateCmdDefault(&stepMotorRunCoordinateCmd);
    stepMotorRunCoordinateCmd.runStepCurve = S_CURVE_MIX_REAGENT_UPDOWN_UP_MODE3;
    stepMotorRunCoordinateCmd.targetCoordinate = paramUtil;
    stepMotorRunCoordinateCmd.timeOutMs = 5000,
    //电机抬起来
    IPC_SrvStepMotorRunCoordinateWhileReturn(MIX_REAGENT_STEP_MOTOR_UPDOWN_LOCAL_ID,&stepMotorRunCoordinateCmd,&sysResultPack,NULL);
    if(sysResultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_MIX_REAGENT_UP_MODE3_RUN_COORDINATE,
                                        ERROR_LEVEL_ERROR,sysResultPack.errorSub);
        return;
    }
    //构建完成无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

void ModImplMixReagentUpMode4(SYS_RESULT_PACK* resultPackPtr)
{
    //参数配置
    SYS_RESULT_PACK sysResultPack;
    //参数辅助,用于系统参数读取
    int32_t paramUtil;
    //走坐标
    IPC_STEP_MOTOR_CMD_RUN_COORDINATE stepMotorRunCoordinateCmd;
    //读取升起来的高度
    SysParamReadSingle(INDEX_MAIN_SYS_PARAM_MIX_REAGENT,INDEX_SUB_PARAM_MIX_REAGENT_UP_POS_MODE4,&paramUtil);
    //走坐标指令初始化
    IPC_StepMotorBaseSetRunCoordinateCmdDefault(&stepMotorRunCoordinateCmd);
    stepMotorRunCoordinateCmd.runStepCurve = S_CURVE_MIX_REAGENT_UPDOWN_UP_MODE4;
    stepMotorRunCoordinateCmd.targetCoordinate = paramUtil;
    stepMotorRunCoordinateCmd.timeOutMs = 5000,
    //电机抬起来
    IPC_SrvStepMotorRunCoordinateWhileReturn(MIX_REAGENT_STEP_MOTOR_UPDOWN_LOCAL_ID,&stepMotorRunCoordinateCmd,&sysResultPack,NULL);
    if(sysResultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_MIX_REAGENT_UP_MODE4_RUN_COORDINATE,
                                        ERROR_LEVEL_ERROR,sysResultPack.errorSub);
        return;
    }
    //构建完成无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

void ModImplMixReagentUpMode5(SYS_RESULT_PACK* resultPackPtr)
{
    //参数配置
    SYS_RESULT_PACK sysResultPack;
    //参数辅助,用于系统参数读取
    int32_t paramUtil;
    //走坐标
    IPC_STEP_MOTOR_CMD_RUN_COORDINATE stepMotorRunCoordinateCmd;
    //读取升起来的高度
    SysParamReadSingle(INDEX_MAIN_SYS_PARAM_MIX_REAGENT,INDEX_SUB_PARAM_MIX_REAGENT_UP_POS_MODE5,&paramUtil);
    //走坐标指令初始化
    IPC_StepMotorBaseSetRunCoordinateCmdDefault(&stepMotorRunCoordinateCmd);
    stepMotorRunCoordinateCmd.runStepCurve = S_CURVE_MIX_REAGENT_UPDOWN_UP_MODE5;
    stepMotorRunCoordinateCmd.targetCoordinate = paramUtil;
    stepMotorRunCoordinateCmd.timeOutMs = 5000,
    //电机抬起来
    IPC_SrvStepMotorRunCoordinateWhileReturn(MIX_REAGENT_STEP_MOTOR_UPDOWN_LOCAL_ID,&stepMotorRunCoordinateCmd,&sysResultPack,NULL);
    if(sysResultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_MIX_REAGENT_UP_MODE5_RUN_COORDINATE,
                                        ERROR_LEVEL_ERROR,sysResultPack.errorSub);
        return;
    }
    //构建完成无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

void ModImplMixReagentUpMode6(SYS_RESULT_PACK* resultPackPtr)
{
    //参数配置
    SYS_RESULT_PACK sysResultPack;
    //参数辅助,用于系统参数读取
    int32_t paramUtil;
    //走坐标
    IPC_STEP_MOTOR_CMD_RUN_COORDINATE stepMotorRunCoordinateCmd;
    //读取升起来的高度
    SysParamReadSingle(INDEX_MAIN_SYS_PARAM_MIX_REAGENT,INDEX_SUB_PARAM_MIX_REAGENT_UP_POS_MODE6,&paramUtil);
    //走坐标指令初始化
    IPC_StepMotorBaseSetRunCoordinateCmdDefault(&stepMotorRunCoordinateCmd);
    stepMotorRunCoordinateCmd.runStepCurve = S_CURVE_MIX_REAGENT_UPDOWN_UP_MODE6;
    stepMotorRunCoordinateCmd.targetCoordinate = paramUtil;
    stepMotorRunCoordinateCmd.timeOutMs = 5000,
    //电机抬起来
    IPC_SrvStepMotorRunCoordinateWhileReturn(MIX_REAGENT_STEP_MOTOR_UPDOWN_LOCAL_ID,&stepMotorRunCoordinateCmd,&sysResultPack,NULL);
    if(sysResultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_MIX_REAGENT_UP_MODE6_RUN_COORDINATE,
                                        ERROR_LEVEL_ERROR,sysResultPack.errorSub);
        return;
    }
    //构建完成无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

void ModImplMixReagentUpMode7(SYS_RESULT_PACK* resultPackPtr)
{
    //参数配置
    SYS_RESULT_PACK sysResultPack;
    //参数辅助,用于系统参数读取
    int32_t paramUtil;
    //走坐标
    IPC_STEP_MOTOR_CMD_RUN_COORDINATE stepMotorRunCoordinateCmd;
    //读取升起来的高度
    SysParamReadSingle(INDEX_MAIN_SYS_PARAM_MIX_REAGENT,INDEX_SUB_PARAM_MIX_REAGENT_UP_POS_MODE7,&paramUtil);
    //走坐标指令初始化
    IPC_StepMotorBaseSetRunCoordinateCmdDefault(&stepMotorRunCoordinateCmd);
    stepMotorRunCoordinateCmd.runStepCurve = S_CURVE_MIX_REAGENT_UPDOWN_UP_MODE7;
    stepMotorRunCoordinateCmd.targetCoordinate = paramUtil;
    stepMotorRunCoordinateCmd.timeOutMs = 5000,
    //电机抬起来
    IPC_SrvStepMotorRunCoordinateWhileReturn(MIX_REAGENT_STEP_MOTOR_UPDOWN_LOCAL_ID,&stepMotorRunCoordinateCmd,&sysResultPack,NULL);
    if(sysResultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_MIX_REAGENT_UP_MODE7_RUN_COORDINATE,
                                        ERROR_LEVEL_ERROR,sysResultPack.errorSub);
        return;
    }
    //构建完成无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

void ModImplMixReagentUpMode8(SYS_RESULT_PACK* resultPackPtr)
{
    //参数配置
    SYS_RESULT_PACK sysResultPack;
    //参数辅助,用于系统参数读取
    int32_t paramUtil;
    //走坐标
    IPC_STEP_MOTOR_CMD_RUN_COORDINATE stepMotorRunCoordinateCmd;
    //读取升起来的高度
    SysParamReadSingle(INDEX_MAIN_SYS_PARAM_MIX_REAGENT,INDEX_SUB_PARAM_MIX_REAGENT_UP_POS_MODE8,&paramUtil);
    //走坐标指令初始化
    IPC_StepMotorBaseSetRunCoordinateCmdDefault(&stepMotorRunCoordinateCmd);
    stepMotorRunCoordinateCmd.runStepCurve = S_CURVE_MIX_REAGENT_UPDOWN_UP_MODE8;
    stepMotorRunCoordinateCmd.targetCoordinate = paramUtil;
    stepMotorRunCoordinateCmd.timeOutMs = 5000,
    //电机抬起来
    IPC_SrvStepMotorRunCoordinateWhileReturn(MIX_REAGENT_STEP_MOTOR_UPDOWN_LOCAL_ID,&stepMotorRunCoordinateCmd,&sysResultPack,NULL);
    if(sysResultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_MIX_REAGENT_UP_MODE8_RUN_COORDINATE,
                                        ERROR_LEVEL_ERROR,sysResultPack.errorSub);
        return;
    }
    //构建完成无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

void ModImplMixReagentUpMode9(SYS_RESULT_PACK* resultPackPtr)
{
    //参数配置
    SYS_RESULT_PACK sysResultPack;
    //参数辅助,用于系统参数读取
    int32_t paramUtil;
    //走坐标
    IPC_STEP_MOTOR_CMD_RUN_COORDINATE stepMotorRunCoordinateCmd;
    //读取升起来的高度
    SysParamReadSingle(INDEX_MAIN_SYS_PARAM_MIX_REAGENT,INDEX_SUB_PARAM_MIX_REAGENT_UP_POS_MODE9,&paramUtil);
    //走坐标指令初始化
    IPC_StepMotorBaseSetRunCoordinateCmdDefault(&stepMotorRunCoordinateCmd);
    stepMotorRunCoordinateCmd.runStepCurve = S_CURVE_MIX_REAGENT_UPDOWN_UP_MODE9;
    stepMotorRunCoordinateCmd.targetCoordinate = paramUtil;
    stepMotorRunCoordinateCmd.timeOutMs = 5000,
    //电机抬起来
    IPC_SrvStepMotorRunCoordinateWhileReturn(MIX_REAGENT_STEP_MOTOR_UPDOWN_LOCAL_ID,&stepMotorRunCoordinateCmd,&sysResultPack,NULL);
    if(sysResultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_MIX_REAGENT_UP_MODE9_RUN_COORDINATE,
                                        ERROR_LEVEL_ERROR,sysResultPack.errorSub);
        return;
    }
    //构建完成无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

void ModImplMixReagentUpMode10(SYS_RESULT_PACK* resultPackPtr)
{
    //参数配置
    SYS_RESULT_PACK sysResultPack;
    //参数辅助,用于系统参数读取
    int32_t paramUtil;
    //走坐标
    IPC_STEP_MOTOR_CMD_RUN_COORDINATE stepMotorRunCoordinateCmd;
    //读取升起来的高度
    SysParamReadSingle(INDEX_MAIN_SYS_PARAM_MIX_REAGENT,INDEX_SUB_PARAM_MIX_REAGENT_UP_POS_MODE10,&paramUtil);
    //走坐标指令初始化
    IPC_StepMotorBaseSetRunCoordinateCmdDefault(&stepMotorRunCoordinateCmd);
    stepMotorRunCoordinateCmd.runStepCurve = S_CURVE_MIX_REAGENT_UPDOWN_UP_MODE10;
    stepMotorRunCoordinateCmd.targetCoordinate = paramUtil;
    stepMotorRunCoordinateCmd.timeOutMs = 5000,
    //电机抬起来
    IPC_SrvStepMotorRunCoordinateWhileReturn(MIX_REAGENT_STEP_MOTOR_UPDOWN_LOCAL_ID,&stepMotorRunCoordinateCmd,&sysResultPack,NULL);
    if(sysResultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_MIX_REAGENT_UP_MODE10_RUN_COORDINATE,
                                        ERROR_LEVEL_ERROR,sysResultPack.errorSub);
        return;
    }
    //构建完成无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}
/*-------------------------------------清洗混匀旋转,不带升降---------------------------------------*/
void ModImplMixReagentStartRotateMode1(SYS_RESULT_PACK* resultPackPtr)
{
    //参数配置
    SYS_RESULT_PACK sysResultPack;
    //参数辅助,用于系统参数读取
    int32_t paramUtil;
    //持续运转
    IPC_STEP_MOTOR_CMD_RUN_ALWAY stepMotorRunAlwaysCmd;
    //减速停止
    IPC_STEP_MOTOR_CMD_STOP_SLOW stepMotorSlowStopCmd;
    //读取运转时间
    SysParamReadSingle(INDEX_MAIN_SYS_PARAM_MIX_REAGENT,INDEX_SUB_PARAM_MIX_REAGENT_ROTATE_TIME_MS_MODE1,&paramUtil);
    //填充持续运转指令
    IPC_StepMotorBaseSetRunAlwaysCmdDefault(&stepMotorRunAlwaysCmd);
    stepMotorRunAlwaysCmd.runAlwaysDir = STEP_MOTOR_DIR_CW;
    stepMotorRunAlwaysCmd.runStepCurve = S_CURVE_MIX_REAGENT_ROTATE_MODE1;
    //开始持续运转
    IPC_SrvStepMotorRunAlwaysWhileReturn(MIX_REAGENT_STEP_MOTOR_ROTATE_LOCAL_ID,&stepMotorRunAlwaysCmd,&sysResultPack);
    if(sysResultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_MIX_REAGENT_START_ROTATE_MODE1_RUN_ALWAYS,
                                        ERROR_LEVEL_ERROR,sysResultPack.errorSub);
        return;
    }
    //等待运转指定时间
    CoreDelayMsSensitivity(paramUtil);
    //填充减速停止
    IPC_StepMotorBaseSetStopSlowCmdDefault(&stepMotorSlowStopCmd);
    stepMotorSlowStopCmd.slowDownMode = 1;//快速减速
    stepMotorSlowStopCmd.slowDownSteps = 0;//减速步数,目前没使用
    stepMotorSlowStopCmd.timeOutMs = 5000;//减速停止时间
    //开始减速停止
    IPC_SrvStepMotorStopSlowDownWhileReturn(MIX_REAGENT_STEP_MOTOR_ROTATE_LOCAL_ID,&stepMotorSlowStopCmd,&sysResultPack,NULL);
    if(sysResultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_MIX_REAGENT_START_ROTATE_MODE1_DECC_STOP,
                                        ERROR_LEVEL_ERROR,sysResultPack.errorSub);
        return;
    }
    //构建完成无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

void ModImplMixReagentStartRotateMode2(SYS_RESULT_PACK* resultPackPtr)
{
    //参数配置
    SYS_RESULT_PACK sysResultPack;
    //参数辅助,用于系统参数读取
    int32_t paramUtil;
    //持续运转
    IPC_STEP_MOTOR_CMD_RUN_ALWAY stepMotorRunAlwaysCmd;
    //减速停止
    IPC_STEP_MOTOR_CMD_STOP_SLOW stepMotorSlowStopCmd;
    //读取运转时间
    SysParamReadSingle(INDEX_MAIN_SYS_PARAM_MIX_REAGENT,INDEX_SUB_PARAM_MIX_REAGENT_ROTATE_TIME_MS_MODE2,&paramUtil);
    //填充持续运转指令
    IPC_StepMotorBaseSetRunAlwaysCmdDefault(&stepMotorRunAlwaysCmd);
    stepMotorRunAlwaysCmd.runAlwaysDir = STEP_MOTOR_DIR_CW;
    stepMotorRunAlwaysCmd.runStepCurve = S_CURVE_MIX_REAGENT_ROTATE_MODE2;
    //开始持续运转
    IPC_SrvStepMotorRunAlwaysWhileReturn(MIX_REAGENT_STEP_MOTOR_ROTATE_LOCAL_ID,&stepMotorRunAlwaysCmd,&sysResultPack);
    if(sysResultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_MIX_REAGENT_START_ROTATE_MODE2_RUN_ALWAYS,
                                        ERROR_LEVEL_ERROR,sysResultPack.errorSub);
        return;
    }
    //等待运转指定时间
    CoreDelayMsSensitivity(paramUtil);
    //填充减速停止
    IPC_StepMotorBaseSetStopSlowCmdDefault(&stepMotorSlowStopCmd);
    stepMotorSlowStopCmd.slowDownMode = 1;//快速减速
    stepMotorSlowStopCmd.slowDownSteps = 0;//减速步数,目前没使用
    stepMotorSlowStopCmd.timeOutMs = 5000;//减速停止时间
    //开始减速停止
    IPC_SrvStepMotorStopSlowDownWhileReturn(MIX_REAGENT_STEP_MOTOR_ROTATE_LOCAL_ID,&stepMotorSlowStopCmd,&sysResultPack,NULL);
    if(sysResultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_MIX_REAGENT_START_ROTATE_MODE2_DECC_STOP,
                                        ERROR_LEVEL_ERROR,sysResultPack.errorSub);
        return;
    }
    //构建完成无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

void ModImplMixReagentStartRotateMode3(SYS_RESULT_PACK* resultPackPtr)
{
    //参数配置
    SYS_RESULT_PACK sysResultPack;
    //参数辅助,用于系统参数读取
    int32_t paramUtil;
    //持续运转
    IPC_STEP_MOTOR_CMD_RUN_ALWAY stepMotorRunAlwaysCmd;
    //减速停止
    IPC_STEP_MOTOR_CMD_STOP_SLOW stepMotorSlowStopCmd;
    //读取运转时间
    SysParamReadSingle(INDEX_MAIN_SYS_PARAM_MIX_REAGENT,INDEX_SUB_PARAM_MIX_REAGENT_ROTATE_TIME_MS_MODE3,&paramUtil);
    //填充持续运转指令
    IPC_StepMotorBaseSetRunAlwaysCmdDefault(&stepMotorRunAlwaysCmd);
    stepMotorRunAlwaysCmd.runAlwaysDir = STEP_MOTOR_DIR_CW;
    stepMotorRunAlwaysCmd.runStepCurve = S_CURVE_MIX_REAGENT_ROTATE_MODE3;
    //开始持续运转
    IPC_SrvStepMotorRunAlwaysWhileReturn(MIX_REAGENT_STEP_MOTOR_ROTATE_LOCAL_ID,&stepMotorRunAlwaysCmd,&sysResultPack);
    if(sysResultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_MIX_REAGENT_START_ROTATE_MODE3_RUN_ALWAYS,
                                        ERROR_LEVEL_ERROR,sysResultPack.errorSub);
        return;
    }
    //等待运转指定时间
    CoreDelayMsSensitivity(paramUtil);
    //填充减速停止
    IPC_StepMotorBaseSetStopSlowCmdDefault(&stepMotorSlowStopCmd);
    stepMotorSlowStopCmd.slowDownMode = 1;//快速减速
    stepMotorSlowStopCmd.slowDownSteps = 0;//减速步数,目前没使用
    stepMotorSlowStopCmd.timeOutMs = 5000;//减速停止时间
    //开始减速停止
    IPC_SrvStepMotorStopSlowDownWhileReturn(MIX_REAGENT_STEP_MOTOR_ROTATE_LOCAL_ID,&stepMotorSlowStopCmd,&sysResultPack,NULL);
    if(sysResultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_MIX_REAGENT_START_ROTATE_MODE3_DECC_STOP,
                                        ERROR_LEVEL_ERROR,sysResultPack.errorSub);
        return;
    }
    //构建完成无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

void ModImplMixReagentStartRotateMode4(SYS_RESULT_PACK* resultPackPtr)
{
    //参数配置
    SYS_RESULT_PACK sysResultPack;
    //参数辅助,用于系统参数读取
    int32_t paramUtil;
    //持续运转
    IPC_STEP_MOTOR_CMD_RUN_ALWAY stepMotorRunAlwaysCmd;
    //减速停止
    IPC_STEP_MOTOR_CMD_STOP_SLOW stepMotorSlowStopCmd;
    //读取运转时间
    SysParamReadSingle(INDEX_MAIN_SYS_PARAM_MIX_REAGENT,INDEX_SUB_PARAM_MIX_REAGENT_ROTATE_TIME_MS_MODE4,&paramUtil);
    //填充持续运转指令
    IPC_StepMotorBaseSetRunAlwaysCmdDefault(&stepMotorRunAlwaysCmd);
    stepMotorRunAlwaysCmd.runAlwaysDir = STEP_MOTOR_DIR_CW;
    stepMotorRunAlwaysCmd.runStepCurve = S_CURVE_MIX_REAGENT_ROTATE_MODE4;
    //开始持续运转
    IPC_SrvStepMotorRunAlwaysWhileReturn(MIX_REAGENT_STEP_MOTOR_ROTATE_LOCAL_ID,&stepMotorRunAlwaysCmd,&sysResultPack);
    if(sysResultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_MIX_REAGENT_START_ROTATE_MODE4_RUN_ALWAYS,
                                        ERROR_LEVEL_ERROR,sysResultPack.errorSub);
        return;
    }
    //等待运转指定时间
    CoreDelayMsSensitivity(paramUtil);
    //填充减速停止
    IPC_StepMotorBaseSetStopSlowCmdDefault(&stepMotorSlowStopCmd);
    stepMotorSlowStopCmd.slowDownMode = 1;//快速减速
    stepMotorSlowStopCmd.slowDownSteps = 0;//减速步数,目前没使用
    stepMotorSlowStopCmd.timeOutMs = 5000;//减速停止时间
    //开始减速停止
    IPC_SrvStepMotorStopSlowDownWhileReturn(MIX_REAGENT_STEP_MOTOR_ROTATE_LOCAL_ID,&stepMotorSlowStopCmd,&sysResultPack,NULL);
    if(sysResultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_MIX_REAGENT_START_ROTATE_MODE4_DECC_STOP,
                                        ERROR_LEVEL_ERROR,sysResultPack.errorSub);
        return;
    }
    //构建完成无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

void ModImplMixReagentStartRotateMode5(SYS_RESULT_PACK* resultPackPtr)
{
    //参数配置
    SYS_RESULT_PACK sysResultPack;
    //参数辅助,用于系统参数读取
    int32_t paramUtil;
    //持续运转
    IPC_STEP_MOTOR_CMD_RUN_ALWAY stepMotorRunAlwaysCmd;
    //减速停止
    IPC_STEP_MOTOR_CMD_STOP_SLOW stepMotorSlowStopCmd;
    //读取运转时间
    SysParamReadSingle(INDEX_MAIN_SYS_PARAM_MIX_REAGENT,INDEX_SUB_PARAM_MIX_REAGENT_ROTATE_TIME_MS_MODE5,&paramUtil);
    //填充持续运转指令
    IPC_StepMotorBaseSetRunAlwaysCmdDefault(&stepMotorRunAlwaysCmd);
    stepMotorRunAlwaysCmd.runAlwaysDir = STEP_MOTOR_DIR_CW;
    stepMotorRunAlwaysCmd.runStepCurve = S_CURVE_MIX_REAGENT_ROTATE_MODE5;
    //开始持续运转
    IPC_SrvStepMotorRunAlwaysWhileReturn(MIX_REAGENT_STEP_MOTOR_ROTATE_LOCAL_ID,&stepMotorRunAlwaysCmd,&sysResultPack);
    if(sysResultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_MIX_REAGENT_START_ROTATE_MODE5_RUN_ALWAYS,
                                        ERROR_LEVEL_ERROR,sysResultPack.errorSub);
        return;
    }
    //等待运转指定时间
    CoreDelayMsSensitivity(paramUtil);
    //填充减速停止
    IPC_StepMotorBaseSetStopSlowCmdDefault(&stepMotorSlowStopCmd);
    stepMotorSlowStopCmd.slowDownMode = 1;//快速减速
    stepMotorSlowStopCmd.slowDownSteps = 0;//减速步数,目前没使用
    stepMotorSlowStopCmd.timeOutMs = 5000;//减速停止时间
    //开始减速停止
    IPC_SrvStepMotorStopSlowDownWhileReturn(MIX_REAGENT_STEP_MOTOR_ROTATE_LOCAL_ID,&stepMotorSlowStopCmd,&sysResultPack,NULL);
    if(sysResultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_MIX_REAGENT_START_ROTATE_MODE5_DECC_STOP,
                                        ERROR_LEVEL_ERROR,sysResultPack.errorSub);
        return;
    }
    //构建完成无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

void ModImplMixReagentStartRotateMode6(SYS_RESULT_PACK* resultPackPtr)
{
    //参数配置
    SYS_RESULT_PACK sysResultPack;
    //参数辅助,用于系统参数读取
    int32_t paramUtil;
    //持续运转
    IPC_STEP_MOTOR_CMD_RUN_ALWAY stepMotorRunAlwaysCmd;
    //减速停止
    IPC_STEP_MOTOR_CMD_STOP_SLOW stepMotorSlowStopCmd;
    //读取运转时间
    SysParamReadSingle(INDEX_MAIN_SYS_PARAM_MIX_REAGENT,INDEX_SUB_PARAM_MIX_REAGENT_ROTATE_TIME_MS_MODE6,&paramUtil);
    //填充持续运转指令
    IPC_StepMotorBaseSetRunAlwaysCmdDefault(&stepMotorRunAlwaysCmd);
    stepMotorRunAlwaysCmd.runAlwaysDir = STEP_MOTOR_DIR_CW;
    stepMotorRunAlwaysCmd.runStepCurve = S_CURVE_MIX_REAGENT_ROTATE_MODE6;
    //开始持续运转
    IPC_SrvStepMotorRunAlwaysWhileReturn(MIX_REAGENT_STEP_MOTOR_ROTATE_LOCAL_ID,&stepMotorRunAlwaysCmd,&sysResultPack);
    if(sysResultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_MIX_REAGENT_START_ROTATE_MODE6_RUN_ALWAYS,
                                        ERROR_LEVEL_ERROR,sysResultPack.errorSub);
        return;
    }
    //等待运转指定时间
    CoreDelayMsSensitivity(paramUtil);
    //填充减速停止
    IPC_StepMotorBaseSetStopSlowCmdDefault(&stepMotorSlowStopCmd);
    stepMotorSlowStopCmd.slowDownMode = 1;//快速减速
    stepMotorSlowStopCmd.slowDownSteps = 0;//减速步数,目前没使用
    stepMotorSlowStopCmd.timeOutMs = 5000;//减速停止时间
    //开始减速停止
    IPC_SrvStepMotorStopSlowDownWhileReturn(MIX_REAGENT_STEP_MOTOR_ROTATE_LOCAL_ID,&stepMotorSlowStopCmd,&sysResultPack,NULL);
    if(sysResultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_MIX_REAGENT_START_ROTATE_MODE6_DECC_STOP,
                                        ERROR_LEVEL_ERROR,sysResultPack.errorSub);
        return;
    }
    //构建完成无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

void ModImplMixReagentStartRotateMode7(SYS_RESULT_PACK* resultPackPtr)
{
    //参数配置
    SYS_RESULT_PACK sysResultPack;
    //参数辅助,用于系统参数读取
    int32_t paramUtil;
    //持续运转
    IPC_STEP_MOTOR_CMD_RUN_ALWAY stepMotorRunAlwaysCmd;
    //减速停止
    IPC_STEP_MOTOR_CMD_STOP_SLOW stepMotorSlowStopCmd;
    //读取运转时间
    SysParamReadSingle(INDEX_MAIN_SYS_PARAM_MIX_REAGENT,INDEX_SUB_PARAM_MIX_REAGENT_ROTATE_TIME_MS_MODE7,&paramUtil);
    //填充持续运转指令
    IPC_StepMotorBaseSetRunAlwaysCmdDefault(&stepMotorRunAlwaysCmd);
    stepMotorRunAlwaysCmd.runAlwaysDir = STEP_MOTOR_DIR_CW;
    stepMotorRunAlwaysCmd.runStepCurve = S_CURVE_MIX_REAGENT_ROTATE_MODE7;
    //开始持续运转
    IPC_SrvStepMotorRunAlwaysWhileReturn(MIX_REAGENT_STEP_MOTOR_ROTATE_LOCAL_ID,&stepMotorRunAlwaysCmd,&sysResultPack);
    if(sysResultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_MIX_REAGENT_START_ROTATE_MODE7_RUN_ALWAYS,
                                        ERROR_LEVEL_ERROR,sysResultPack.errorSub);
        return;
    }
    //等待运转指定时间
    CoreDelayMsSensitivity(paramUtil);
    //填充减速停止
    IPC_StepMotorBaseSetStopSlowCmdDefault(&stepMotorSlowStopCmd);
    stepMotorSlowStopCmd.slowDownMode = 1;//快速减速
    stepMotorSlowStopCmd.slowDownSteps = 0;//减速步数,目前没使用
    stepMotorSlowStopCmd.timeOutMs = 5000;//减速停止时间
    //开始减速停止
    IPC_SrvStepMotorStopSlowDownWhileReturn(MIX_REAGENT_STEP_MOTOR_ROTATE_LOCAL_ID,&stepMotorSlowStopCmd,&sysResultPack,NULL);
    if(sysResultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_MIX_REAGENT_START_ROTATE_MODE7_DECC_STOP,
                                        ERROR_LEVEL_ERROR,sysResultPack.errorSub);
        return;
    }
    //构建完成无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

void ModImplMixReagentStartRotateMode8(SYS_RESULT_PACK* resultPackPtr)
{
    //参数配置
    SYS_RESULT_PACK sysResultPack;
    //参数辅助,用于系统参数读取
    int32_t paramUtil;
    //持续运转
    IPC_STEP_MOTOR_CMD_RUN_ALWAY stepMotorRunAlwaysCmd;
    //减速停止
    IPC_STEP_MOTOR_CMD_STOP_SLOW stepMotorSlowStopCmd;
    //读取运转时间
    SysParamReadSingle(INDEX_MAIN_SYS_PARAM_MIX_REAGENT,INDEX_SUB_PARAM_MIX_REAGENT_ROTATE_TIME_MS_MODE8,&paramUtil);
    //填充持续运转指令
    IPC_StepMotorBaseSetRunAlwaysCmdDefault(&stepMotorRunAlwaysCmd);
    stepMotorRunAlwaysCmd.runAlwaysDir = STEP_MOTOR_DIR_CW;
    stepMotorRunAlwaysCmd.runStepCurve = S_CURVE_MIX_REAGENT_ROTATE_MODE8;
    //开始持续运转
    IPC_SrvStepMotorRunAlwaysWhileReturn(MIX_REAGENT_STEP_MOTOR_ROTATE_LOCAL_ID,&stepMotorRunAlwaysCmd,&sysResultPack);
    if(sysResultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_MIX_REAGENT_START_ROTATE_MODE8_RUN_ALWAYS,
                                        ERROR_LEVEL_ERROR,sysResultPack.errorSub);
        return;
    }
    //等待运转指定时间
    CoreDelayMsSensitivity(paramUtil);
    //填充减速停止
    IPC_StepMotorBaseSetStopSlowCmdDefault(&stepMotorSlowStopCmd);
    stepMotorSlowStopCmd.slowDownMode = 1;//快速减速
    stepMotorSlowStopCmd.slowDownSteps = 0;//减速步数,目前没使用
    stepMotorSlowStopCmd.timeOutMs = 5000;//减速停止时间
    //开始减速停止
    IPC_SrvStepMotorStopSlowDownWhileReturn(MIX_REAGENT_STEP_MOTOR_ROTATE_LOCAL_ID,&stepMotorSlowStopCmd,&sysResultPack,NULL);
    if(sysResultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_MIX_REAGENT_START_ROTATE_MODE8_DECC_STOP,
                                        ERROR_LEVEL_ERROR,sysResultPack.errorSub);
        return;
    }
    //构建完成无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

void ModImplMixReagentStartRotateMode9(SYS_RESULT_PACK* resultPackPtr)
{
    //参数配置
    SYS_RESULT_PACK sysResultPack;
    //参数辅助,用于系统参数读取
    int32_t paramUtil;
    //持续运转
    IPC_STEP_MOTOR_CMD_RUN_ALWAY stepMotorRunAlwaysCmd;
    //减速停止
    IPC_STEP_MOTOR_CMD_STOP_SLOW stepMotorSlowStopCmd;
    //读取运转时间
    SysParamReadSingle(INDEX_MAIN_SYS_PARAM_MIX_REAGENT,INDEX_SUB_PARAM_MIX_REAGENT_ROTATE_TIME_MS_MODE9,&paramUtil);
    //填充持续运转指令
    IPC_StepMotorBaseSetRunAlwaysCmdDefault(&stepMotorRunAlwaysCmd);
    stepMotorRunAlwaysCmd.runAlwaysDir = STEP_MOTOR_DIR_CW;
    stepMotorRunAlwaysCmd.runStepCurve = S_CURVE_MIX_REAGENT_ROTATE_MODE9;
    //开始持续运转
    IPC_SrvStepMotorRunAlwaysWhileReturn(MIX_REAGENT_STEP_MOTOR_ROTATE_LOCAL_ID,&stepMotorRunAlwaysCmd,&sysResultPack);
    if(sysResultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_MIX_REAGENT_START_ROTATE_MODE9_RUN_ALWAYS,
                                        ERROR_LEVEL_ERROR,sysResultPack.errorSub);
        return;
    }
    //等待运转指定时间
    CoreDelayMsSensitivity(paramUtil);
    //填充减速停止
    IPC_StepMotorBaseSetStopSlowCmdDefault(&stepMotorSlowStopCmd);
    stepMotorSlowStopCmd.slowDownMode = 1;//快速减速
    stepMotorSlowStopCmd.slowDownSteps = 0;//减速步数,目前没使用
    stepMotorSlowStopCmd.timeOutMs = 5000;//减速停止时间
    //开始减速停止
    IPC_SrvStepMotorStopSlowDownWhileReturn(MIX_REAGENT_STEP_MOTOR_ROTATE_LOCAL_ID,&stepMotorSlowStopCmd,&sysResultPack,NULL);
    if(sysResultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_MIX_REAGENT_START_ROTATE_MODE9_DECC_STOP,
                                        ERROR_LEVEL_ERROR,sysResultPack.errorSub);
        return;
    }
    //构建完成无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

void ModImplMixReagentStartRotateMode10(SYS_RESULT_PACK* resultPackPtr)
{
    //参数配置
    SYS_RESULT_PACK sysResultPack;
    //参数辅助,用于系统参数读取
    int32_t paramUtil;
    //持续运转
    IPC_STEP_MOTOR_CMD_RUN_ALWAY stepMotorRunAlwaysCmd;
    //减速停止
    IPC_STEP_MOTOR_CMD_STOP_SLOW stepMotorSlowStopCmd;
    //读取运转时间
    SysParamReadSingle(INDEX_MAIN_SYS_PARAM_MIX_REAGENT,INDEX_SUB_PARAM_MIX_REAGENT_ROTATE_TIME_MS_MODE10,&paramUtil);
    //填充持续运转指令
    IPC_StepMotorBaseSetRunAlwaysCmdDefault(&stepMotorRunAlwaysCmd);
    stepMotorRunAlwaysCmd.runAlwaysDir = STEP_MOTOR_DIR_CW;
    stepMotorRunAlwaysCmd.runStepCurve = S_CURVE_MIX_REAGENT_ROTATE_MODE10;
    //开始持续运转
    IPC_SrvStepMotorRunAlwaysWhileReturn(MIX_REAGENT_STEP_MOTOR_ROTATE_LOCAL_ID,&stepMotorRunAlwaysCmd,&sysResultPack);
    if(sysResultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_MIX_REAGENT_START_ROTATE_MODE10_RUN_ALWAYS,
                                        ERROR_LEVEL_ERROR,sysResultPack.errorSub);
        return;
    }
    //等待运转指定时间
    CoreDelayMsSensitivity(paramUtil);
    //填充减速停止
    IPC_StepMotorBaseSetStopSlowCmdDefault(&stepMotorSlowStopCmd);
    stepMotorSlowStopCmd.slowDownMode = 1;//快速减速
    stepMotorSlowStopCmd.slowDownSteps = 0;//减速步数,目前没使用
    stepMotorSlowStopCmd.timeOutMs = 5000;//减速停止时间
    //开始减速停止
    IPC_SrvStepMotorStopSlowDownWhileReturn(MIX_REAGENT_STEP_MOTOR_ROTATE_LOCAL_ID,&stepMotorSlowStopCmd,&sysResultPack,NULL);
    if(sysResultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_MIX_REAGENT_START_ROTATE_MODE10_DECC_STOP,
                                        ERROR_LEVEL_ERROR,sysResultPack.errorSub);
        return;
    }
    //构建完成无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

/*--------------------------------------清洗混匀集成混匀--------------------------------------*/
void ModImplMixReagentRotateIntegratedMode1(SYS_RESULT_PACK* resultPackPtr)
{
    //参数配置
    SYS_RESULT_PACK sysResultPack;
    //参数辅助,用于系统参数读取
    int32_t paramUtil;
    //走坐标
    IPC_STEP_MOTOR_CMD_RUN_COORDINATE stepMotorRunCoordinateCmd;
    //读取升起来的高度
    SysParamReadSingle(INDEX_MAIN_SYS_PARAM_MIX_REAGENT,INDEX_SUB_PARAM_MIX_REAGENT_UP_POS_MODE1,&paramUtil);
    //走坐标指令初始化
    IPC_StepMotorBaseSetRunCoordinateCmdDefault(&stepMotorRunCoordinateCmd);
    stepMotorRunCoordinateCmd.runStepCurve = S_CURVE_MIX_REAGENT_UPDOWN_UP_MODE1;
    stepMotorRunCoordinateCmd.targetCoordinate = paramUtil;
    stepMotorRunCoordinateCmd.timeOutMs = 5000,
    //电机抬起来
    IPC_SrvStepMotorRunCoordinateWhileReturn(MIX_REAGENT_STEP_MOTOR_UPDOWN_LOCAL_ID,&stepMotorRunCoordinateCmd,&sysResultPack,NULL);
    if(sysResultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_MIX_REAGENT_INTEGRATED_MODE1_UP_RUN_COORDINATE,
                                        ERROR_LEVEL_ERROR,sysResultPack.errorSub);
        return;
    }
    //持续运转
    IPC_STEP_MOTOR_CMD_RUN_ALWAY stepMotorRunAlwaysCmd;
    //读取运转时间
    SysParamReadSingle(INDEX_MAIN_SYS_PARAM_MIX_REAGENT,INDEX_SUB_PARAM_MIX_REAGENT_ROTATE_TIME_MS_MODE1,&paramUtil);
    //填充持续运转指令
    IPC_StepMotorBaseSetRunAlwaysCmdDefault(&stepMotorRunAlwaysCmd);
    stepMotorRunAlwaysCmd.runAlwaysDir = STEP_MOTOR_DIR_CW;
    stepMotorRunAlwaysCmd.runStepCurve = S_CURVE_MIX_REAGENT_ROTATE_MODE1;
    //开始持续运转
    IPC_SrvStepMotorRunAlwaysWhileReturn(MIX_REAGENT_STEP_MOTOR_ROTATE_LOCAL_ID,&stepMotorRunAlwaysCmd,&sysResultPack);
    if(sysResultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_MIX_REAGENT_INTEGRATED_MODE1_ROTATE_RUN_ALWAYS,
                                        ERROR_LEVEL_ERROR,sysResultPack.errorSub);
        return;
    }
    //等待运转指定时间
    CoreDelayMsSensitivity(paramUtil);
    //减速停止
    IPC_STEP_MOTOR_CMD_STOP_SLOW stepMotorSlowStopCmd;
    //填充减速停止
    IPC_StepMotorBaseSetStopSlowCmdDefault(&stepMotorSlowStopCmd);
    stepMotorSlowStopCmd.slowDownMode = 1;//快速减速
    stepMotorSlowStopCmd.slowDownSteps = 0;//减速步数,目前没使用
    stepMotorSlowStopCmd.timeOutMs = 5000;//减速停止时间
    //开始减速停止
    IPC_SrvStepMotorStopSlowDownWhileReturn(MIX_REAGENT_STEP_MOTOR_ROTATE_LOCAL_ID,&stepMotorSlowStopCmd,&sysResultPack,NULL);
    if(sysResultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_MIX_REAGENT_INTEGRATED_MODE1_DECC_STOP,
                                        ERROR_LEVEL_ERROR,sysResultPack.errorSub);
        return;
    }
    //回零指令结构体
    IPC_STEP_MOTOR_CMD_RETURN_ZERO stepMotorReturnZeroCmd;
    //读取复位修正
    SysParamReadSingle(INDEX_MAIN_SYS_PARAM_MIX_REAGENT,INDEX_SUB_PARAM_MIX_REAGENT_RESET_CORRECT_UPDOWN,&paramUtil);
    //配置回零指令结构体,升降回零
    IPC_StepMotorBaseSetReturnZeroCmdDefault(&stepMotorReturnZeroCmd);
    stepMotorReturnZeroCmd.needReset = ENABLE;
    stepMotorReturnZeroCmd.timeOutMs = 5000;
    stepMotorReturnZeroCmd.returnZeroCorrectPos = paramUtil;
    stepMotorReturnZeroCmd.runStepCurve = S_CURVE_MIX_REAGENT_UPDOWN_DOWN_MODE1;
    IPC_SrvStepMotorReturnZeroWhileAck(MIX_REAGENT_STEP_MOTOR_UPDOWN_LOCAL_ID,&stepMotorReturnZeroCmd,&sysResultPack);
    if(sysResultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_MIX_REAGENT_INTEGRATED_MODE1_DOWN_RETURN_ZERO,
                                        ERROR_LEVEL_ERROR,sysResultPack.errorSub);
        return;
    }
    //复位指令结构体
    IPC_STEP_MOTOR_CMD_RESET stepMotorResetCmd;
    //读取复位修正
    SysParamReadSingle(INDEX_MAIN_SYS_PARAM_MIX_REAGENT,INDEX_SUB_PARAM_MIX_REAGENT_RESET_CORRECT_ROTATE,&paramUtil);
    //配置复位指令结构体,旋转复位
    IPC_StepMotorBaseSetResetCmdDefault(&stepMotorResetCmd);
    stepMotorResetCmd.resetCorrectCurve = S_CURVE_MIX_REAGENT_ROTATE_DEBUG;//修正曲线
    stepMotorResetCmd.resetCorrectPos = paramUtil;//复位修正
    stepMotorResetCmd.timeOutMs = 5000;//超时时间
    //开始复位
    IPC_SrvStepMotorResetWhileReturn(MIX_REAGENT_STEP_MOTOR_ROTATE_LOCAL_ID,&stepMotorResetCmd,&sysResultPack,NULL);
    if(sysResultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_MIX_REAGENT_INTEGRATED_MODE1_ROTATE_RESET,
                                        ERROR_LEVEL_ERROR,sysResultPack.errorSub);
        return;
    }
    //等待升降电机回零完成
    IPC_SrvStepMotorReturnZeroWaitCmdReturn(MIX_REAGENT_STEP_MOTOR_UPDOWN_LOCAL_ID,&sysResultPack,NULL);
    if(sysResultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_MIX_REAGENT_INTEGRATED_MODE1_DOWN_RETURN_ZERO,
                                        ERROR_LEVEL_ERROR,sysResultPack.errorSub);
        return;
    }
    //构建完成无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

void ModImplMixReagentRotateIntegratedMode2(SYS_RESULT_PACK* resultPackPtr)
{
    //参数配置
    SYS_RESULT_PACK sysResultPack;
    //参数辅助,用于系统参数读取
    int32_t paramUtil;
    //走坐标
    IPC_STEP_MOTOR_CMD_RUN_COORDINATE stepMotorRunCoordinateCmd;
    //读取升起来的高度
    SysParamReadSingle(INDEX_MAIN_SYS_PARAM_MIX_REAGENT,INDEX_SUB_PARAM_MIX_REAGENT_UP_POS_MODE2,&paramUtil);
    //走坐标指令初始化
    IPC_StepMotorBaseSetRunCoordinateCmdDefault(&stepMotorRunCoordinateCmd);
    stepMotorRunCoordinateCmd.runStepCurve = S_CURVE_MIX_REAGENT_UPDOWN_UP_MODE2;
    stepMotorRunCoordinateCmd.targetCoordinate = paramUtil;
    stepMotorRunCoordinateCmd.timeOutMs = 5000,
    //电机抬起来
    IPC_SrvStepMotorRunCoordinateWhileReturn(MIX_REAGENT_STEP_MOTOR_UPDOWN_LOCAL_ID,&stepMotorRunCoordinateCmd,&sysResultPack,NULL);
    if(sysResultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_MIX_REAGENT_INTEGRATED_MODE2_UP_RUN_COORDINATE,
                                        ERROR_LEVEL_ERROR,sysResultPack.errorSub);
        return;
    }
    //持续运转
    IPC_STEP_MOTOR_CMD_RUN_ALWAY stepMotorRunAlwaysCmd;
    //读取运转时间
    SysParamReadSingle(INDEX_MAIN_SYS_PARAM_MIX_REAGENT,INDEX_SUB_PARAM_MIX_REAGENT_ROTATE_TIME_MS_MODE2,&paramUtil);
    //填充持续运转指令
    IPC_StepMotorBaseSetRunAlwaysCmdDefault(&stepMotorRunAlwaysCmd);
    stepMotorRunAlwaysCmd.runAlwaysDir = STEP_MOTOR_DIR_CW;
    stepMotorRunAlwaysCmd.runStepCurve = S_CURVE_MIX_REAGENT_ROTATE_MODE2;
    //开始持续运转
    IPC_SrvStepMotorRunAlwaysWhileReturn(MIX_REAGENT_STEP_MOTOR_ROTATE_LOCAL_ID,&stepMotorRunAlwaysCmd,&sysResultPack);
    if(sysResultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_MIX_REAGENT_INTEGRATED_MODE2_ROTATE_RUN_ALWAYS,
                                        ERROR_LEVEL_ERROR,sysResultPack.errorSub);
        return;
    }
    //等待运转指定时间
    CoreDelayMsSensitivity(paramUtil);
    //减速停止
    IPC_STEP_MOTOR_CMD_STOP_SLOW stepMotorSlowStopCmd;
    //填充减速停止
    IPC_StepMotorBaseSetStopSlowCmdDefault(&stepMotorSlowStopCmd);
    stepMotorSlowStopCmd.slowDownMode = 1;//快速减速
    stepMotorSlowStopCmd.slowDownSteps = 0;//减速步数,目前没使用
    stepMotorSlowStopCmd.timeOutMs = 5000;//减速停止时间
    //开始减速停止
    IPC_SrvStepMotorStopSlowDownWhileReturn(MIX_REAGENT_STEP_MOTOR_ROTATE_LOCAL_ID,&stepMotorSlowStopCmd,&sysResultPack,NULL);
    if(sysResultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_MIX_REAGENT_INTEGRATED_MODE2_DECC_STOP,
                                        ERROR_LEVEL_ERROR,sysResultPack.errorSub);
        return;
    }
    //回零指令结构体
    IPC_STEP_MOTOR_CMD_RETURN_ZERO stepMotorReturnZeroCmd;
    //读取复位修正
    SysParamReadSingle(INDEX_MAIN_SYS_PARAM_MIX_REAGENT,INDEX_SUB_PARAM_MIX_REAGENT_RESET_CORRECT_UPDOWN,&paramUtil);
    //配置回零指令结构体,升降回零
    IPC_StepMotorBaseSetReturnZeroCmdDefault(&stepMotorReturnZeroCmd);
    stepMotorReturnZeroCmd.needReset = ENABLE;
    stepMotorReturnZeroCmd.timeOutMs = 5000;
    stepMotorReturnZeroCmd.returnZeroCorrectPos = paramUtil;
    stepMotorReturnZeroCmd.runStepCurve = S_CURVE_MIX_REAGENT_UPDOWN_DOWN_MODE2;
    IPC_SrvStepMotorReturnZeroWhileAck(MIX_REAGENT_STEP_MOTOR_UPDOWN_LOCAL_ID,&stepMotorReturnZeroCmd,&sysResultPack);
    if(sysResultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_MIX_REAGENT_INTEGRATED_MODE2_DOWN_RETURN_ZERO,
                                        ERROR_LEVEL_ERROR,sysResultPack.errorSub);
        return;
    }
    //复位指令结构体
    IPC_STEP_MOTOR_CMD_RESET stepMotorResetCmd;
    //读取复位修正
    SysParamReadSingle(INDEX_MAIN_SYS_PARAM_MIX_REAGENT,INDEX_SUB_PARAM_MIX_REAGENT_RESET_CORRECT_ROTATE,&paramUtil);
    //配置复位指令结构体,旋转复位
    IPC_StepMotorBaseSetResetCmdDefault(&stepMotorResetCmd);
    stepMotorResetCmd.resetCorrectCurve = S_CURVE_MIX_REAGENT_ROTATE_DEBUG;//修正曲线
    stepMotorResetCmd.resetCorrectPos = paramUtil;//复位修正
    stepMotorResetCmd.timeOutMs = 5000;//超时时间
    //开始复位
    IPC_SrvStepMotorResetWhileReturn(MIX_REAGENT_STEP_MOTOR_ROTATE_LOCAL_ID,&stepMotorResetCmd,&sysResultPack,NULL);
    if(sysResultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_MIX_REAGENT_INTEGRATED_MODE2_ROTATE_RESET,
                                        ERROR_LEVEL_ERROR,sysResultPack.errorSub);
        return;
    }
    //等待升降电机回零完成
    IPC_SrvStepMotorReturnZeroWaitCmdReturn(MIX_REAGENT_STEP_MOTOR_UPDOWN_LOCAL_ID,&sysResultPack,NULL);
    if(sysResultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_MIX_REAGENT_INTEGRATED_MODE2_DOWN_RETURN_ZERO,
                                        ERROR_LEVEL_ERROR,sysResultPack.errorSub);
        return;
    }
    //构建完成无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

void ModImplMixReagentRotateIntegratedMode3(SYS_RESULT_PACK* resultPackPtr)
{
    //参数配置
    SYS_RESULT_PACK sysResultPack;
    //参数辅助,用于系统参数读取
    int32_t paramUtil;
    //走坐标
    IPC_STEP_MOTOR_CMD_RUN_COORDINATE stepMotorRunCoordinateCmd;
    //读取升起来的高度
    SysParamReadSingle(INDEX_MAIN_SYS_PARAM_MIX_REAGENT,INDEX_SUB_PARAM_MIX_REAGENT_UP_POS_MODE3,&paramUtil);
    //走坐标指令初始化
    IPC_StepMotorBaseSetRunCoordinateCmdDefault(&stepMotorRunCoordinateCmd);
    stepMotorRunCoordinateCmd.runStepCurve = S_CURVE_MIX_REAGENT_UPDOWN_UP_MODE3;
    stepMotorRunCoordinateCmd.targetCoordinate = paramUtil;
    stepMotorRunCoordinateCmd.timeOutMs = 5000,
    //电机抬起来
    IPC_SrvStepMotorRunCoordinateWhileReturn(MIX_REAGENT_STEP_MOTOR_UPDOWN_LOCAL_ID,&stepMotorRunCoordinateCmd,&sysResultPack,NULL);
    if(sysResultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_MIX_REAGENT_INTEGRATED_MODE3_UP_RUN_COORDINATE,
                                        ERROR_LEVEL_ERROR,sysResultPack.errorSub);
        return;
    }
    //持续运转
    IPC_STEP_MOTOR_CMD_RUN_ALWAY stepMotorRunAlwaysCmd;
    //读取运转时间
    SysParamReadSingle(INDEX_MAIN_SYS_PARAM_MIX_REAGENT,INDEX_SUB_PARAM_MIX_REAGENT_ROTATE_TIME_MS_MODE3,&paramUtil);
    //填充持续运转指令
    IPC_StepMotorBaseSetRunAlwaysCmdDefault(&stepMotorRunAlwaysCmd);
    stepMotorRunAlwaysCmd.runAlwaysDir = STEP_MOTOR_DIR_CW;
    stepMotorRunAlwaysCmd.runStepCurve = S_CURVE_MIX_REAGENT_ROTATE_MODE3;
    //开始持续运转
    IPC_SrvStepMotorRunAlwaysWhileReturn(MIX_REAGENT_STEP_MOTOR_ROTATE_LOCAL_ID,&stepMotorRunAlwaysCmd,&sysResultPack);
    if(sysResultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_MIX_REAGENT_INTEGRATED_MODE3_ROTATE_RUN_ALWAYS,
                                        ERROR_LEVEL_ERROR,sysResultPack.errorSub);
        return;
    }
    //等待运转指定时间
    CoreDelayMsSensitivity(paramUtil);
    //减速停止
    IPC_STEP_MOTOR_CMD_STOP_SLOW stepMotorSlowStopCmd;
    //填充减速停止
    IPC_StepMotorBaseSetStopSlowCmdDefault(&stepMotorSlowStopCmd);
    stepMotorSlowStopCmd.slowDownMode = 1;//快速减速
    stepMotorSlowStopCmd.slowDownSteps = 0;//减速步数,目前没使用
    stepMotorSlowStopCmd.timeOutMs = 5000;//减速停止时间
    //开始减速停止
    IPC_SrvStepMotorStopSlowDownWhileReturn(MIX_REAGENT_STEP_MOTOR_ROTATE_LOCAL_ID,&stepMotorSlowStopCmd,&sysResultPack,NULL);
    if(sysResultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_MIX_REAGENT_INTEGRATED_MODE3_DECC_STOP,
                                        ERROR_LEVEL_ERROR,sysResultPack.errorSub);
        return;
    }
    //回零指令结构体
    IPC_STEP_MOTOR_CMD_RETURN_ZERO stepMotorReturnZeroCmd;
    //读取复位修正
    SysParamReadSingle(INDEX_MAIN_SYS_PARAM_MIX_REAGENT,INDEX_SUB_PARAM_MIX_REAGENT_RESET_CORRECT_UPDOWN,&paramUtil);
    //配置回零指令结构体,升降回零
    IPC_StepMotorBaseSetReturnZeroCmdDefault(&stepMotorReturnZeroCmd);
    stepMotorReturnZeroCmd.needReset = ENABLE;
    stepMotorReturnZeroCmd.timeOutMs = 5000;
    stepMotorReturnZeroCmd.returnZeroCorrectPos = paramUtil;
    stepMotorReturnZeroCmd.runStepCurve = S_CURVE_MIX_REAGENT_UPDOWN_DOWN_MODE3;
    IPC_SrvStepMotorReturnZeroWhileAck(MIX_REAGENT_STEP_MOTOR_UPDOWN_LOCAL_ID,&stepMotorReturnZeroCmd,&sysResultPack);
    if(sysResultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_MIX_REAGENT_INTEGRATED_MODE3_DOWN_RETURN_ZERO,
                                        ERROR_LEVEL_ERROR,sysResultPack.errorSub);
        return;
    }
    //复位指令结构体
    IPC_STEP_MOTOR_CMD_RESET stepMotorResetCmd;
    //读取复位修正
    SysParamReadSingle(INDEX_MAIN_SYS_PARAM_MIX_REAGENT,INDEX_SUB_PARAM_MIX_REAGENT_RESET_CORRECT_ROTATE,&paramUtil);
    //配置复位指令结构体,旋转复位
    IPC_StepMotorBaseSetResetCmdDefault(&stepMotorResetCmd);
    stepMotorResetCmd.resetCorrectCurve = S_CURVE_MIX_REAGENT_ROTATE_DEBUG;//修正曲线
    stepMotorResetCmd.resetCorrectPos = paramUtil;//复位修正
    stepMotorResetCmd.timeOutMs = 5000;//超时时间
    //开始复位
    IPC_SrvStepMotorResetWhileReturn(MIX_REAGENT_STEP_MOTOR_ROTATE_LOCAL_ID,&stepMotorResetCmd,&sysResultPack,NULL);
    if(sysResultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_MIX_REAGENT_INTEGRATED_MODE3_ROTATE_RESET,
                                        ERROR_LEVEL_ERROR,sysResultPack.errorSub);
        return;
    }
    //等待升降电机回零完成
    IPC_SrvStepMotorReturnZeroWaitCmdReturn(MIX_REAGENT_STEP_MOTOR_UPDOWN_LOCAL_ID,&sysResultPack,NULL);
    if(sysResultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_MIX_REAGENT_INTEGRATED_MODE3_DOWN_RETURN_ZERO,
                                        ERROR_LEVEL_ERROR,sysResultPack.errorSub);
        return;
    }
    //构建完成无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

void ModImplMixReagentRotateIntegratedMode4(SYS_RESULT_PACK* resultPackPtr)
{
    //参数配置
    SYS_RESULT_PACK sysResultPack;
    //参数辅助,用于系统参数读取
    int32_t paramUtil;
    //走坐标
    IPC_STEP_MOTOR_CMD_RUN_COORDINATE stepMotorRunCoordinateCmd;
    //读取升起来的高度
    SysParamReadSingle(INDEX_MAIN_SYS_PARAM_MIX_REAGENT,INDEX_SUB_PARAM_MIX_REAGENT_UP_POS_MODE4,&paramUtil);
    //走坐标指令初始化
    IPC_StepMotorBaseSetRunCoordinateCmdDefault(&stepMotorRunCoordinateCmd);
    stepMotorRunCoordinateCmd.runStepCurve = S_CURVE_MIX_REAGENT_UPDOWN_UP_MODE4;
    stepMotorRunCoordinateCmd.targetCoordinate = paramUtil;
    stepMotorRunCoordinateCmd.timeOutMs = 5000,
    //电机抬起来
    IPC_SrvStepMotorRunCoordinateWhileReturn(MIX_REAGENT_STEP_MOTOR_UPDOWN_LOCAL_ID,&stepMotorRunCoordinateCmd,&sysResultPack,NULL);
    if(sysResultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_MIX_REAGENT_INTEGRATED_MODE4_UP_RUN_COORDINATE,
                                        ERROR_LEVEL_ERROR,sysResultPack.errorSub);
        return;
    }
    //持续运转
    IPC_STEP_MOTOR_CMD_RUN_ALWAY stepMotorRunAlwaysCmd;
    //读取运转时间
    SysParamReadSingle(INDEX_MAIN_SYS_PARAM_MIX_REAGENT,INDEX_SUB_PARAM_MIX_REAGENT_ROTATE_TIME_MS_MODE4,&paramUtil);
    //填充持续运转指令
    IPC_StepMotorBaseSetRunAlwaysCmdDefault(&stepMotorRunAlwaysCmd);
    stepMotorRunAlwaysCmd.runAlwaysDir = STEP_MOTOR_DIR_CW;
    stepMotorRunAlwaysCmd.runStepCurve = S_CURVE_MIX_REAGENT_ROTATE_MODE4;
    //开始持续运转
    IPC_SrvStepMotorRunAlwaysWhileReturn(MIX_REAGENT_STEP_MOTOR_ROTATE_LOCAL_ID,&stepMotorRunAlwaysCmd,&sysResultPack);
    if(sysResultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_MIX_REAGENT_INTEGRATED_MODE4_ROTATE_RUN_ALWAYS,
                                        ERROR_LEVEL_ERROR,sysResultPack.errorSub);
        return;
    }
    //等待运转指定时间
    CoreDelayMsSensitivity(paramUtil);
    //减速停止
    IPC_STEP_MOTOR_CMD_STOP_SLOW stepMotorSlowStopCmd;
    //填充减速停止
    IPC_StepMotorBaseSetStopSlowCmdDefault(&stepMotorSlowStopCmd);
    stepMotorSlowStopCmd.slowDownMode = 1;//快速减速
    stepMotorSlowStopCmd.slowDownSteps = 0;//减速步数,目前没使用
    stepMotorSlowStopCmd.timeOutMs = 5000;//减速停止时间
    //开始减速停止
    IPC_SrvStepMotorStopSlowDownWhileReturn(MIX_REAGENT_STEP_MOTOR_ROTATE_LOCAL_ID,&stepMotorSlowStopCmd,&sysResultPack,NULL);
    if(sysResultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_MIX_REAGENT_INTEGRATED_MODE4_DECC_STOP,
                                        ERROR_LEVEL_ERROR,sysResultPack.errorSub);
        return;
    }
    //回零指令结构体
    IPC_STEP_MOTOR_CMD_RETURN_ZERO stepMotorReturnZeroCmd;
    //读取复位修正
    SysParamReadSingle(INDEX_MAIN_SYS_PARAM_MIX_REAGENT,INDEX_SUB_PARAM_MIX_REAGENT_RESET_CORRECT_UPDOWN,&paramUtil);
    //配置回零指令结构体,升降回零
    IPC_StepMotorBaseSetReturnZeroCmdDefault(&stepMotorReturnZeroCmd);
    stepMotorReturnZeroCmd.needReset = ENABLE;
    stepMotorReturnZeroCmd.timeOutMs = 5000;
    stepMotorReturnZeroCmd.returnZeroCorrectPos = paramUtil;
    stepMotorReturnZeroCmd.runStepCurve = S_CURVE_MIX_REAGENT_UPDOWN_DOWN_MODE4;
    IPC_SrvStepMotorReturnZeroWhileAck(MIX_REAGENT_STEP_MOTOR_UPDOWN_LOCAL_ID,&stepMotorReturnZeroCmd,&sysResultPack);
    if(sysResultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_MIX_REAGENT_INTEGRATED_MODE4_DOWN_RETURN_ZERO,
                                        ERROR_LEVEL_ERROR,sysResultPack.errorSub);
        return;
    }
    //复位指令结构体
    IPC_STEP_MOTOR_CMD_RESET stepMotorResetCmd;
    //读取复位修正
    SysParamReadSingle(INDEX_MAIN_SYS_PARAM_MIX_REAGENT,INDEX_SUB_PARAM_MIX_REAGENT_RESET_CORRECT_ROTATE,&paramUtil);
    //配置复位指令结构体,旋转复位
    IPC_StepMotorBaseSetResetCmdDefault(&stepMotorResetCmd);
    stepMotorResetCmd.resetCorrectCurve = S_CURVE_MIX_REAGENT_ROTATE_DEBUG;//修正曲线
    stepMotorResetCmd.resetCorrectPos = paramUtil;//复位修正
    stepMotorResetCmd.timeOutMs = 5000;//超时时间
    //开始复位
    IPC_SrvStepMotorResetWhileReturn(MIX_REAGENT_STEP_MOTOR_ROTATE_LOCAL_ID,&stepMotorResetCmd,&sysResultPack,NULL);
    if(sysResultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_MIX_REAGENT_INTEGRATED_MODE4_ROTATE_RESET,
                                        ERROR_LEVEL_ERROR,sysResultPack.errorSub);
        return;
    }
    //等待升降电机回零完成
    IPC_SrvStepMotorReturnZeroWaitCmdReturn(MIX_REAGENT_STEP_MOTOR_UPDOWN_LOCAL_ID,&sysResultPack,NULL);
    if(sysResultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_MIX_REAGENT_INTEGRATED_MODE4_DOWN_RETURN_ZERO,
                                        ERROR_LEVEL_ERROR,sysResultPack.errorSub);
        return;
    }
    //构建完成无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

void ModImplMixReagentRotateIntegratedMode5(SYS_RESULT_PACK* resultPackPtr)
{
    //参数配置
    SYS_RESULT_PACK sysResultPack;
    //参数辅助,用于系统参数读取
    int32_t paramUtil;
    //走坐标
    IPC_STEP_MOTOR_CMD_RUN_COORDINATE stepMotorRunCoordinateCmd;
    //读取升起来的高度
    SysParamReadSingle(INDEX_MAIN_SYS_PARAM_MIX_REAGENT,INDEX_SUB_PARAM_MIX_REAGENT_UP_POS_MODE5,&paramUtil);
    //走坐标指令初始化
    IPC_StepMotorBaseSetRunCoordinateCmdDefault(&stepMotorRunCoordinateCmd);
    stepMotorRunCoordinateCmd.runStepCurve = S_CURVE_MIX_REAGENT_UPDOWN_UP_MODE5;
    stepMotorRunCoordinateCmd.targetCoordinate = paramUtil;
    stepMotorRunCoordinateCmd.timeOutMs = 5000,
    //电机抬起来
    IPC_SrvStepMotorRunCoordinateWhileReturn(MIX_REAGENT_STEP_MOTOR_UPDOWN_LOCAL_ID,&stepMotorRunCoordinateCmd,&sysResultPack,NULL);
    if(sysResultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_MIX_REAGENT_INTEGRATED_MODE5_UP_RUN_COORDINATE,
                                        ERROR_LEVEL_ERROR,sysResultPack.errorSub);
        return;
    }
    //持续运转
    IPC_STEP_MOTOR_CMD_RUN_ALWAY stepMotorRunAlwaysCmd;
    //读取运转时间
    SysParamReadSingle(INDEX_MAIN_SYS_PARAM_MIX_REAGENT,INDEX_SUB_PARAM_MIX_REAGENT_ROTATE_TIME_MS_MODE5,&paramUtil);
    //填充持续运转指令
    IPC_StepMotorBaseSetRunAlwaysCmdDefault(&stepMotorRunAlwaysCmd);
    stepMotorRunAlwaysCmd.runAlwaysDir = STEP_MOTOR_DIR_CW;
    stepMotorRunAlwaysCmd.runStepCurve = S_CURVE_MIX_REAGENT_ROTATE_MODE5;
    //开始持续运转
    IPC_SrvStepMotorRunAlwaysWhileReturn(MIX_REAGENT_STEP_MOTOR_ROTATE_LOCAL_ID,&stepMotorRunAlwaysCmd,&sysResultPack);
    if(sysResultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_MIX_REAGENT_INTEGRATED_MODE5_ROTATE_RUN_ALWAYS,
                                        ERROR_LEVEL_ERROR,sysResultPack.errorSub);
        return;
    }
    //等待运转指定时间
    CoreDelayMsSensitivity(paramUtil);
    //减速停止
    IPC_STEP_MOTOR_CMD_STOP_SLOW stepMotorSlowStopCmd;
    //填充减速停止
    IPC_StepMotorBaseSetStopSlowCmdDefault(&stepMotorSlowStopCmd);
    stepMotorSlowStopCmd.slowDownMode = 1;//快速减速
    stepMotorSlowStopCmd.slowDownSteps = 0;//减速步数,目前没使用
    stepMotorSlowStopCmd.timeOutMs = 5000;//减速停止时间
    //开始减速停止
    IPC_SrvStepMotorStopSlowDownWhileReturn(MIX_REAGENT_STEP_MOTOR_ROTATE_LOCAL_ID,&stepMotorSlowStopCmd,&sysResultPack,NULL);
    if(sysResultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_MIX_REAGENT_INTEGRATED_MODE5_DECC_STOP,
                                        ERROR_LEVEL_ERROR,sysResultPack.errorSub);
        return;
    }
    //回零指令结构体
    IPC_STEP_MOTOR_CMD_RETURN_ZERO stepMotorReturnZeroCmd;
    //读取复位修正
    SysParamReadSingle(INDEX_MAIN_SYS_PARAM_MIX_REAGENT,INDEX_SUB_PARAM_MIX_REAGENT_RESET_CORRECT_UPDOWN,&paramUtil);
    //配置回零指令结构体,升降回零
    IPC_StepMotorBaseSetReturnZeroCmdDefault(&stepMotorReturnZeroCmd);
    stepMotorReturnZeroCmd.needReset = ENABLE;
    stepMotorReturnZeroCmd.timeOutMs = 5000;
    stepMotorReturnZeroCmd.returnZeroCorrectPos = paramUtil;
    stepMotorReturnZeroCmd.runStepCurve = S_CURVE_MIX_REAGENT_UPDOWN_DOWN_MODE5;
    IPC_SrvStepMotorReturnZeroWhileAck(MIX_REAGENT_STEP_MOTOR_UPDOWN_LOCAL_ID,&stepMotorReturnZeroCmd,&sysResultPack);
    if(sysResultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_MIX_REAGENT_INTEGRATED_MODE5_DOWN_RETURN_ZERO,
                                        ERROR_LEVEL_ERROR,sysResultPack.errorSub);
        return;
    }
    //复位指令结构体
    IPC_STEP_MOTOR_CMD_RESET stepMotorResetCmd;
    //读取复位修正
    SysParamReadSingle(INDEX_MAIN_SYS_PARAM_MIX_REAGENT,INDEX_SUB_PARAM_MIX_REAGENT_RESET_CORRECT_ROTATE,&paramUtil);
    //配置复位指令结构体,旋转复位
    IPC_StepMotorBaseSetResetCmdDefault(&stepMotorResetCmd);
    stepMotorResetCmd.resetCorrectCurve = S_CURVE_MIX_REAGENT_ROTATE_DEBUG;//修正曲线
    stepMotorResetCmd.resetCorrectPos = paramUtil;//复位修正
    stepMotorResetCmd.timeOutMs = 5000;//超时时间
    //开始复位
    IPC_SrvStepMotorResetWhileReturn(MIX_REAGENT_STEP_MOTOR_ROTATE_LOCAL_ID,&stepMotorResetCmd,&sysResultPack,NULL);
    if(sysResultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_MIX_REAGENT_INTEGRATED_MODE5_ROTATE_RESET,
                                        ERROR_LEVEL_ERROR,sysResultPack.errorSub);
        return;
    }
    //等待升降电机回零完成
    IPC_SrvStepMotorReturnZeroWaitCmdReturn(MIX_REAGENT_STEP_MOTOR_UPDOWN_LOCAL_ID,&sysResultPack,NULL);
    if(sysResultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_MIX_REAGENT_INTEGRATED_MODE5_DOWN_RETURN_ZERO,
                                        ERROR_LEVEL_ERROR,sysResultPack.errorSub);
        return;
    }
    //构建完成无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

void ModImplMixReagentRotateIntegratedMode6(SYS_RESULT_PACK* resultPackPtr)
{
    //参数配置
    SYS_RESULT_PACK sysResultPack;
    //参数辅助,用于系统参数读取
    int32_t paramUtil;
    //走坐标
    IPC_STEP_MOTOR_CMD_RUN_COORDINATE stepMotorRunCoordinateCmd;
    //读取升起来的高度
    SysParamReadSingle(INDEX_MAIN_SYS_PARAM_MIX_REAGENT,INDEX_SUB_PARAM_MIX_REAGENT_UP_POS_MODE6,&paramUtil);
    //走坐标指令初始化
    IPC_StepMotorBaseSetRunCoordinateCmdDefault(&stepMotorRunCoordinateCmd);
    stepMotorRunCoordinateCmd.runStepCurve = S_CURVE_MIX_REAGENT_UPDOWN_UP_MODE6;
    stepMotorRunCoordinateCmd.targetCoordinate = paramUtil;
    stepMotorRunCoordinateCmd.timeOutMs = 5000,
    //电机抬起来
    IPC_SrvStepMotorRunCoordinateWhileReturn(MIX_REAGENT_STEP_MOTOR_UPDOWN_LOCAL_ID,&stepMotorRunCoordinateCmd,&sysResultPack,NULL);
    if(sysResultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_MIX_REAGENT_INTEGRATED_MODE6_UP_RUN_COORDINATE,
                                        ERROR_LEVEL_ERROR,sysResultPack.errorSub);
        return;
    }
    //持续运转
    IPC_STEP_MOTOR_CMD_RUN_ALWAY stepMotorRunAlwaysCmd;
    //读取运转时间
    SysParamReadSingle(INDEX_MAIN_SYS_PARAM_MIX_REAGENT,INDEX_SUB_PARAM_MIX_REAGENT_ROTATE_TIME_MS_MODE6,&paramUtil);
    //填充持续运转指令
    IPC_StepMotorBaseSetRunAlwaysCmdDefault(&stepMotorRunAlwaysCmd);
    stepMotorRunAlwaysCmd.runAlwaysDir = STEP_MOTOR_DIR_CW;
    stepMotorRunAlwaysCmd.runStepCurve = S_CURVE_MIX_REAGENT_ROTATE_MODE6;
    //开始持续运转
    IPC_SrvStepMotorRunAlwaysWhileReturn(MIX_REAGENT_STEP_MOTOR_ROTATE_LOCAL_ID,&stepMotorRunAlwaysCmd,&sysResultPack);
    if(sysResultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_MIX_REAGENT_INTEGRATED_MODE6_ROTATE_RUN_ALWAYS,
                                        ERROR_LEVEL_ERROR,sysResultPack.errorSub);
        return;
    }
    //等待运转指定时间
    CoreDelayMsSensitivity(paramUtil);
    //减速停止
    IPC_STEP_MOTOR_CMD_STOP_SLOW stepMotorSlowStopCmd;
    //填充减速停止
    IPC_StepMotorBaseSetStopSlowCmdDefault(&stepMotorSlowStopCmd);
    stepMotorSlowStopCmd.slowDownMode = 1;//快速减速
    stepMotorSlowStopCmd.slowDownSteps = 0;//减速步数,目前没使用
    stepMotorSlowStopCmd.timeOutMs = 5000;//减速停止时间
    //开始减速停止
    IPC_SrvStepMotorStopSlowDownWhileReturn(MIX_REAGENT_STEP_MOTOR_ROTATE_LOCAL_ID,&stepMotorSlowStopCmd,&sysResultPack,NULL);
    if(sysResultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_MIX_REAGENT_INTEGRATED_MODE6_DECC_STOP,
                                        ERROR_LEVEL_ERROR,sysResultPack.errorSub);
        return;
    }
    //回零指令结构体
    IPC_STEP_MOTOR_CMD_RETURN_ZERO stepMotorReturnZeroCmd;
    //读取复位修正
    SysParamReadSingle(INDEX_MAIN_SYS_PARAM_MIX_REAGENT,INDEX_SUB_PARAM_MIX_REAGENT_RESET_CORRECT_UPDOWN,&paramUtil);
    //配置回零指令结构体,升降回零
    IPC_StepMotorBaseSetReturnZeroCmdDefault(&stepMotorReturnZeroCmd);
    stepMotorReturnZeroCmd.needReset = ENABLE;
    stepMotorReturnZeroCmd.timeOutMs = 5000;
    stepMotorReturnZeroCmd.returnZeroCorrectPos = paramUtil;
    stepMotorReturnZeroCmd.runStepCurve = S_CURVE_MIX_REAGENT_UPDOWN_DOWN_MODE6;
    IPC_SrvStepMotorReturnZeroWhileAck(MIX_REAGENT_STEP_MOTOR_UPDOWN_LOCAL_ID,&stepMotorReturnZeroCmd,&sysResultPack);
    if(sysResultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_MIX_REAGENT_INTEGRATED_MODE6_DOWN_RETURN_ZERO,
                                        ERROR_LEVEL_ERROR,sysResultPack.errorSub);
        return;
    }
    //复位指令结构体
    IPC_STEP_MOTOR_CMD_RESET stepMotorResetCmd;
    //读取复位修正
    SysParamReadSingle(INDEX_MAIN_SYS_PARAM_MIX_REAGENT,INDEX_SUB_PARAM_MIX_REAGENT_RESET_CORRECT_ROTATE,&paramUtil);
    //配置复位指令结构体,旋转复位
    IPC_StepMotorBaseSetResetCmdDefault(&stepMotorResetCmd);
    stepMotorResetCmd.resetCorrectCurve = S_CURVE_MIX_REAGENT_ROTATE_DEBUG;//修正曲线
    stepMotorResetCmd.resetCorrectPos = paramUtil;//复位修正
    stepMotorResetCmd.timeOutMs = 5000;//超时时间
    //开始复位
    IPC_SrvStepMotorResetWhileReturn(MIX_REAGENT_STEP_MOTOR_ROTATE_LOCAL_ID,&stepMotorResetCmd,&sysResultPack,NULL);
    if(sysResultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_MIX_REAGENT_INTEGRATED_MODE6_ROTATE_RESET,
                                        ERROR_LEVEL_ERROR,sysResultPack.errorSub);
        return;
    }
    //等待升降电机回零完成
    IPC_SrvStepMotorReturnZeroWaitCmdReturn(MIX_REAGENT_STEP_MOTOR_UPDOWN_LOCAL_ID,&sysResultPack,NULL);
    if(sysResultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_MIX_REAGENT_INTEGRATED_MODE6_DOWN_RETURN_ZERO,
                                        ERROR_LEVEL_ERROR,sysResultPack.errorSub);
        return;
    }
    //构建完成无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

void ModImplMixReagentRotateIntegratedMode7(SYS_RESULT_PACK* resultPackPtr)
{
    //参数配置
    SYS_RESULT_PACK sysResultPack;
    //参数辅助,用于系统参数读取
    int32_t paramUtil;
    //走坐标
    IPC_STEP_MOTOR_CMD_RUN_COORDINATE stepMotorRunCoordinateCmd;
    //读取升起来的高度
    SysParamReadSingle(INDEX_MAIN_SYS_PARAM_MIX_REAGENT,INDEX_SUB_PARAM_MIX_REAGENT_UP_POS_MODE7,&paramUtil);
    //走坐标指令初始化
    IPC_StepMotorBaseSetRunCoordinateCmdDefault(&stepMotorRunCoordinateCmd);
    stepMotorRunCoordinateCmd.runStepCurve = S_CURVE_MIX_REAGENT_UPDOWN_UP_MODE7;
    stepMotorRunCoordinateCmd.targetCoordinate = paramUtil;
    stepMotorRunCoordinateCmd.timeOutMs = 5000,
    //电机抬起来
    IPC_SrvStepMotorRunCoordinateWhileReturn(MIX_REAGENT_STEP_MOTOR_UPDOWN_LOCAL_ID,&stepMotorRunCoordinateCmd,&sysResultPack,NULL);
    if(sysResultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_MIX_REAGENT_INTEGRATED_MODE7_UP_RUN_COORDINATE,
                                        ERROR_LEVEL_ERROR,sysResultPack.errorSub);
        return;
    }
    //持续运转
    IPC_STEP_MOTOR_CMD_RUN_ALWAY stepMotorRunAlwaysCmd;
    //读取运转时间
    SysParamReadSingle(INDEX_MAIN_SYS_PARAM_MIX_REAGENT,INDEX_SUB_PARAM_MIX_REAGENT_ROTATE_TIME_MS_MODE7,&paramUtil);
    //填充持续运转指令
    IPC_StepMotorBaseSetRunAlwaysCmdDefault(&stepMotorRunAlwaysCmd);
    stepMotorRunAlwaysCmd.runAlwaysDir = STEP_MOTOR_DIR_CW;
    stepMotorRunAlwaysCmd.runStepCurve = S_CURVE_MIX_REAGENT_ROTATE_MODE7;
    //开始持续运转
    IPC_SrvStepMotorRunAlwaysWhileReturn(MIX_REAGENT_STEP_MOTOR_ROTATE_LOCAL_ID,&stepMotorRunAlwaysCmd,&sysResultPack);
    if(sysResultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_MIX_REAGENT_INTEGRATED_MODE7_ROTATE_RUN_ALWAYS,
                                        ERROR_LEVEL_ERROR,sysResultPack.errorSub);
        return;
    }
    //等待运转指定时间
    CoreDelayMsSensitivity(paramUtil);
    //减速停止
    IPC_STEP_MOTOR_CMD_STOP_SLOW stepMotorSlowStopCmd;
    //填充减速停止
    IPC_StepMotorBaseSetStopSlowCmdDefault(&stepMotorSlowStopCmd);
    stepMotorSlowStopCmd.slowDownMode = 1;//快速减速
    stepMotorSlowStopCmd.slowDownSteps = 0;//减速步数,目前没使用
    stepMotorSlowStopCmd.timeOutMs = 5000;//减速停止时间
    //开始减速停止
    IPC_SrvStepMotorStopSlowDownWhileReturn(MIX_REAGENT_STEP_MOTOR_ROTATE_LOCAL_ID,&stepMotorSlowStopCmd,&sysResultPack,NULL);
    if(sysResultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_MIX_REAGENT_INTEGRATED_MODE7_DECC_STOP,
                                        ERROR_LEVEL_ERROR,sysResultPack.errorSub);
        return;
    }
    //回零指令结构体
    IPC_STEP_MOTOR_CMD_RETURN_ZERO stepMotorReturnZeroCmd;
    //读取复位修正
    SysParamReadSingle(INDEX_MAIN_SYS_PARAM_MIX_REAGENT,INDEX_SUB_PARAM_MIX_REAGENT_RESET_CORRECT_UPDOWN,&paramUtil);
    //配置回零指令结构体,升降回零
    IPC_StepMotorBaseSetReturnZeroCmdDefault(&stepMotorReturnZeroCmd);
    stepMotorReturnZeroCmd.needReset = ENABLE;
    stepMotorReturnZeroCmd.timeOutMs = 5000;
    stepMotorReturnZeroCmd.returnZeroCorrectPos = paramUtil;
    stepMotorReturnZeroCmd.runStepCurve = S_CURVE_MIX_REAGENT_UPDOWN_DOWN_MODE7;
    IPC_SrvStepMotorReturnZeroWhileAck(MIX_REAGENT_STEP_MOTOR_UPDOWN_LOCAL_ID,&stepMotorReturnZeroCmd,&sysResultPack);
    if(sysResultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_MIX_REAGENT_INTEGRATED_MODE7_DOWN_RETURN_ZERO,
                                        ERROR_LEVEL_ERROR,sysResultPack.errorSub);
        return;
    }
    //复位指令结构体
    IPC_STEP_MOTOR_CMD_RESET stepMotorResetCmd;
    //读取复位修正
    SysParamReadSingle(INDEX_MAIN_SYS_PARAM_MIX_REAGENT,INDEX_SUB_PARAM_MIX_REAGENT_RESET_CORRECT_ROTATE,&paramUtil);
    //配置复位指令结构体,旋转复位
    IPC_StepMotorBaseSetResetCmdDefault(&stepMotorResetCmd);
    stepMotorResetCmd.resetCorrectCurve = S_CURVE_MIX_REAGENT_ROTATE_DEBUG;//修正曲线
    stepMotorResetCmd.resetCorrectPos = paramUtil;//复位修正
    stepMotorResetCmd.timeOutMs = 5000;//超时时间
    //开始复位
    IPC_SrvStepMotorResetWhileReturn(MIX_REAGENT_STEP_MOTOR_ROTATE_LOCAL_ID,&stepMotorResetCmd,&sysResultPack,NULL);
    if(sysResultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_MIX_REAGENT_INTEGRATED_MODE7_ROTATE_RESET,
                                        ERROR_LEVEL_ERROR,sysResultPack.errorSub);
        return;
    }
    //等待升降电机回零完成
    IPC_SrvStepMotorReturnZeroWaitCmdReturn(MIX_REAGENT_STEP_MOTOR_UPDOWN_LOCAL_ID,&sysResultPack,NULL);
    if(sysResultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_MIX_REAGENT_INTEGRATED_MODE7_DOWN_RETURN_ZERO,
                                        ERROR_LEVEL_ERROR,sysResultPack.errorSub);
        return;
    }
    //构建完成无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

void ModImplMixReagentRotateIntegratedMode8(SYS_RESULT_PACK* resultPackPtr)
{
    //参数配置
    SYS_RESULT_PACK sysResultPack;
    //参数辅助,用于系统参数读取
    int32_t paramUtil;
    //走坐标
    IPC_STEP_MOTOR_CMD_RUN_COORDINATE stepMotorRunCoordinateCmd;
    //读取升起来的高度
    SysParamReadSingle(INDEX_MAIN_SYS_PARAM_MIX_REAGENT,INDEX_SUB_PARAM_MIX_REAGENT_UP_POS_MODE8,&paramUtil);
    //走坐标指令初始化
    IPC_StepMotorBaseSetRunCoordinateCmdDefault(&stepMotorRunCoordinateCmd);
    stepMotorRunCoordinateCmd.runStepCurve = S_CURVE_MIX_REAGENT_UPDOWN_UP_MODE8;
    stepMotorRunCoordinateCmd.targetCoordinate = paramUtil;
    stepMotorRunCoordinateCmd.timeOutMs = 5000,
    //电机抬起来
    IPC_SrvStepMotorRunCoordinateWhileReturn(MIX_REAGENT_STEP_MOTOR_UPDOWN_LOCAL_ID,&stepMotorRunCoordinateCmd,&sysResultPack,NULL);
    if(sysResultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_MIX_REAGENT_INTEGRATED_MODE8_UP_RUN_COORDINATE,
                                        ERROR_LEVEL_ERROR,sysResultPack.errorSub);
        return;
    }
    //持续运转
    IPC_STEP_MOTOR_CMD_RUN_ALWAY stepMotorRunAlwaysCmd;
    //读取运转时间
    SysParamReadSingle(INDEX_MAIN_SYS_PARAM_MIX_REAGENT,INDEX_SUB_PARAM_MIX_REAGENT_ROTATE_TIME_MS_MODE8,&paramUtil);
    //填充持续运转指令
    IPC_StepMotorBaseSetRunAlwaysCmdDefault(&stepMotorRunAlwaysCmd);
    stepMotorRunAlwaysCmd.runAlwaysDir = STEP_MOTOR_DIR_CW;
    stepMotorRunAlwaysCmd.runStepCurve = S_CURVE_MIX_REAGENT_ROTATE_MODE8;
    //开始持续运转
    IPC_SrvStepMotorRunAlwaysWhileReturn(MIX_REAGENT_STEP_MOTOR_ROTATE_LOCAL_ID,&stepMotorRunAlwaysCmd,&sysResultPack);
    if(sysResultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_MIX_REAGENT_INTEGRATED_MODE8_ROTATE_RUN_ALWAYS,
                                        ERROR_LEVEL_ERROR,sysResultPack.errorSub);
        return;
    }
    //等待运转指定时间
    CoreDelayMsSensitivity(paramUtil);
    //减速停止
    IPC_STEP_MOTOR_CMD_STOP_SLOW stepMotorSlowStopCmd;
    //填充减速停止
    IPC_StepMotorBaseSetStopSlowCmdDefault(&stepMotorSlowStopCmd);
    stepMotorSlowStopCmd.slowDownMode = 1;//快速减速
    stepMotorSlowStopCmd.slowDownSteps = 0;//减速步数,目前没使用
    stepMotorSlowStopCmd.timeOutMs = 5000;//减速停止时间
    //开始减速停止
    IPC_SrvStepMotorStopSlowDownWhileReturn(MIX_REAGENT_STEP_MOTOR_ROTATE_LOCAL_ID,&stepMotorSlowStopCmd,&sysResultPack,NULL);
    if(sysResultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_MIX_REAGENT_INTEGRATED_MODE8_DECC_STOP,
                                        ERROR_LEVEL_ERROR,sysResultPack.errorSub);
        return;
    }
    //回零指令结构体
    IPC_STEP_MOTOR_CMD_RETURN_ZERO stepMotorReturnZeroCmd;
    //读取复位修正
    SysParamReadSingle(INDEX_MAIN_SYS_PARAM_MIX_REAGENT,INDEX_SUB_PARAM_MIX_REAGENT_RESET_CORRECT_UPDOWN,&paramUtil);
    //配置回零指令结构体,升降回零
    IPC_StepMotorBaseSetReturnZeroCmdDefault(&stepMotorReturnZeroCmd);
    stepMotorReturnZeroCmd.needReset = ENABLE;
    stepMotorReturnZeroCmd.timeOutMs = 5000;
    stepMotorReturnZeroCmd.returnZeroCorrectPos = paramUtil;
    stepMotorReturnZeroCmd.runStepCurve = S_CURVE_MIX_REAGENT_UPDOWN_DOWN_MODE8;
    IPC_SrvStepMotorReturnZeroWhileAck(MIX_REAGENT_STEP_MOTOR_UPDOWN_LOCAL_ID,&stepMotorReturnZeroCmd,&sysResultPack);
    if(sysResultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_MIX_REAGENT_INTEGRATED_MODE8_DOWN_RETURN_ZERO,
                                        ERROR_LEVEL_ERROR,sysResultPack.errorSub);
        return;
    }
    //复位指令结构体
    IPC_STEP_MOTOR_CMD_RESET stepMotorResetCmd;
    //读取复位修正
    SysParamReadSingle(INDEX_MAIN_SYS_PARAM_MIX_REAGENT,INDEX_SUB_PARAM_MIX_REAGENT_RESET_CORRECT_ROTATE,&paramUtil);
    //配置复位指令结构体,旋转复位
    IPC_StepMotorBaseSetResetCmdDefault(&stepMotorResetCmd);
    stepMotorResetCmd.resetCorrectCurve = S_CURVE_MIX_REAGENT_ROTATE_DEBUG;//修正曲线
    stepMotorResetCmd.resetCorrectPos = paramUtil;//复位修正
    stepMotorResetCmd.timeOutMs = 5000;//超时时间
    //开始复位
    IPC_SrvStepMotorResetWhileReturn(MIX_REAGENT_STEP_MOTOR_ROTATE_LOCAL_ID,&stepMotorResetCmd,&sysResultPack,NULL);
    if(sysResultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_MIX_REAGENT_INTEGRATED_MODE8_ROTATE_RESET,
                                        ERROR_LEVEL_ERROR,sysResultPack.errorSub);
        return;
    }
    //等待升降电机回零完成
    IPC_SrvStepMotorReturnZeroWaitCmdReturn(MIX_REAGENT_STEP_MOTOR_UPDOWN_LOCAL_ID,&sysResultPack,NULL);
    if(sysResultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_MIX_REAGENT_INTEGRATED_MODE8_DOWN_RETURN_ZERO,
                                        ERROR_LEVEL_ERROR,sysResultPack.errorSub);
        return;
    }
    //构建完成无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

void ModImplMixReagentRotateIntegratedMode9(SYS_RESULT_PACK* resultPackPtr)
{
    //参数配置
    SYS_RESULT_PACK sysResultPack;
    //参数辅助,用于系统参数读取
    int32_t paramUtil;
    //走坐标
    IPC_STEP_MOTOR_CMD_RUN_COORDINATE stepMotorRunCoordinateCmd;
    //读取升起来的高度
    SysParamReadSingle(INDEX_MAIN_SYS_PARAM_MIX_REAGENT,INDEX_SUB_PARAM_MIX_REAGENT_UP_POS_MODE9,&paramUtil);
    //走坐标指令初始化
    IPC_StepMotorBaseSetRunCoordinateCmdDefault(&stepMotorRunCoordinateCmd);
    stepMotorRunCoordinateCmd.runStepCurve = S_CURVE_MIX_REAGENT_UPDOWN_UP_MODE9;
    stepMotorRunCoordinateCmd.targetCoordinate = paramUtil;
    stepMotorRunCoordinateCmd.timeOutMs = 5000,
    //电机抬起来
    IPC_SrvStepMotorRunCoordinateWhileReturn(MIX_REAGENT_STEP_MOTOR_UPDOWN_LOCAL_ID,&stepMotorRunCoordinateCmd,&sysResultPack,NULL);
    if(sysResultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_MIX_REAGENT_INTEGRATED_MODE9_UP_RUN_COORDINATE,
                                        ERROR_LEVEL_ERROR,sysResultPack.errorSub);
        return;
    }
    //持续运转
    IPC_STEP_MOTOR_CMD_RUN_ALWAY stepMotorRunAlwaysCmd;
    //读取运转时间
    SysParamReadSingle(INDEX_MAIN_SYS_PARAM_MIX_REAGENT,INDEX_SUB_PARAM_MIX_REAGENT_ROTATE_TIME_MS_MODE9,&paramUtil);
    //填充持续运转指令
    IPC_StepMotorBaseSetRunAlwaysCmdDefault(&stepMotorRunAlwaysCmd);
    stepMotorRunAlwaysCmd.runAlwaysDir = STEP_MOTOR_DIR_CW;
    stepMotorRunAlwaysCmd.runStepCurve = S_CURVE_MIX_REAGENT_ROTATE_MODE9;
    //开始持续运转
    IPC_SrvStepMotorRunAlwaysWhileReturn(MIX_REAGENT_STEP_MOTOR_ROTATE_LOCAL_ID,&stepMotorRunAlwaysCmd,&sysResultPack);
    if(sysResultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_MIX_REAGENT_INTEGRATED_MODE9_ROTATE_RUN_ALWAYS,
                                        ERROR_LEVEL_ERROR,sysResultPack.errorSub);
        return;
    }
    //等待运转指定时间
    CoreDelayMsSensitivity(paramUtil);
    //减速停止
    IPC_STEP_MOTOR_CMD_STOP_SLOW stepMotorSlowStopCmd;
    //填充减速停止
    IPC_StepMotorBaseSetStopSlowCmdDefault(&stepMotorSlowStopCmd);
    stepMotorSlowStopCmd.slowDownMode = 1;//快速减速
    stepMotorSlowStopCmd.slowDownSteps = 0;//减速步数,目前没使用
    stepMotorSlowStopCmd.timeOutMs = 5000;//减速停止时间
    //开始减速停止
    IPC_SrvStepMotorStopSlowDownWhileReturn(MIX_REAGENT_STEP_MOTOR_ROTATE_LOCAL_ID,&stepMotorSlowStopCmd,&sysResultPack,NULL);
    if(sysResultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_MIX_REAGENT_INTEGRATED_MODE9_DECC_STOP,
                                        ERROR_LEVEL_ERROR,sysResultPack.errorSub);
        return;
    }
    //回零指令结构体
    IPC_STEP_MOTOR_CMD_RETURN_ZERO stepMotorReturnZeroCmd;
    //读取复位修正
    SysParamReadSingle(INDEX_MAIN_SYS_PARAM_MIX_REAGENT,INDEX_SUB_PARAM_MIX_REAGENT_RESET_CORRECT_UPDOWN,&paramUtil);
    //配置回零指令结构体,升降回零
    IPC_StepMotorBaseSetReturnZeroCmdDefault(&stepMotorReturnZeroCmd);
    stepMotorReturnZeroCmd.needReset = ENABLE;
    stepMotorReturnZeroCmd.timeOutMs = 5000;
    stepMotorReturnZeroCmd.returnZeroCorrectPos = paramUtil;
    stepMotorReturnZeroCmd.runStepCurve = S_CURVE_MIX_REAGENT_UPDOWN_DOWN_MODE9;
    IPC_SrvStepMotorReturnZeroWhileAck(MIX_REAGENT_STEP_MOTOR_UPDOWN_LOCAL_ID,&stepMotorReturnZeroCmd,&sysResultPack);
    if(sysResultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_MIX_REAGENT_INTEGRATED_MODE9_DOWN_RETURN_ZERO,
                                        ERROR_LEVEL_ERROR,sysResultPack.errorSub);
        return;
    }
    //复位指令结构体
    IPC_STEP_MOTOR_CMD_RESET stepMotorResetCmd;
    //读取复位修正
    SysParamReadSingle(INDEX_MAIN_SYS_PARAM_MIX_REAGENT,INDEX_SUB_PARAM_MIX_REAGENT_RESET_CORRECT_ROTATE,&paramUtil);
    //配置复位指令结构体,旋转复位
    IPC_StepMotorBaseSetResetCmdDefault(&stepMotorResetCmd);
    stepMotorResetCmd.resetCorrectCurve = S_CURVE_MIX_REAGENT_ROTATE_DEBUG;//修正曲线
    stepMotorResetCmd.resetCorrectPos = paramUtil;//复位修正
    stepMotorResetCmd.timeOutMs = 5000;//超时时间
    //开始复位
    IPC_SrvStepMotorResetWhileReturn(MIX_REAGENT_STEP_MOTOR_ROTATE_LOCAL_ID,&stepMotorResetCmd,&sysResultPack,NULL);
    if(sysResultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_MIX_REAGENT_INTEGRATED_MODE9_ROTATE_RESET,
                                        ERROR_LEVEL_ERROR,sysResultPack.errorSub);
        return;
    }
    //等待升降电机回零完成
    IPC_SrvStepMotorReturnZeroWaitCmdReturn(MIX_REAGENT_STEP_MOTOR_UPDOWN_LOCAL_ID,&sysResultPack,NULL);
    if(sysResultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_MIX_REAGENT_INTEGRATED_MODE9_DOWN_RETURN_ZERO,
                                        ERROR_LEVEL_ERROR,sysResultPack.errorSub);
        return;
    }
    //构建完成无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

void ModImplMixReagentRotateIntegratedMode10(SYS_RESULT_PACK* resultPackPtr)
{
    //参数配置
    SYS_RESULT_PACK sysResultPack;
    //参数辅助,用于系统参数读取
    int32_t paramUtil;
    //走坐标
    IPC_STEP_MOTOR_CMD_RUN_COORDINATE stepMotorRunCoordinateCmd;
    //读取升起来的高度
    SysParamReadSingle(INDEX_MAIN_SYS_PARAM_MIX_REAGENT,INDEX_SUB_PARAM_MIX_REAGENT_UP_POS_MODE10,&paramUtil);
    //走坐标指令初始化
    IPC_StepMotorBaseSetRunCoordinateCmdDefault(&stepMotorRunCoordinateCmd);
    stepMotorRunCoordinateCmd.runStepCurve = S_CURVE_MIX_REAGENT_UPDOWN_UP_MODE10;
    stepMotorRunCoordinateCmd.targetCoordinate = paramUtil;
    stepMotorRunCoordinateCmd.timeOutMs = 5000,
    //电机抬起来
    IPC_SrvStepMotorRunCoordinateWhileReturn(MIX_REAGENT_STEP_MOTOR_UPDOWN_LOCAL_ID,&stepMotorRunCoordinateCmd,&sysResultPack,NULL);
    if(sysResultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_MIX_REAGENT_INTEGRATED_MODE10_UP_RUN_COORDINATE,
                                        ERROR_LEVEL_ERROR,sysResultPack.errorSub);
        return;
    }
    //持续运转
    IPC_STEP_MOTOR_CMD_RUN_ALWAY stepMotorRunAlwaysCmd;
    //读取运转时间
    SysParamReadSingle(INDEX_MAIN_SYS_PARAM_MIX_REAGENT,INDEX_SUB_PARAM_MIX_REAGENT_ROTATE_TIME_MS_MODE10,&paramUtil);
    //填充持续运转指令
    IPC_StepMotorBaseSetRunAlwaysCmdDefault(&stepMotorRunAlwaysCmd);
    stepMotorRunAlwaysCmd.runAlwaysDir = STEP_MOTOR_DIR_CW;
    stepMotorRunAlwaysCmd.runStepCurve = S_CURVE_MIX_REAGENT_ROTATE_MODE10;
    //开始持续运转
    IPC_SrvStepMotorRunAlwaysWhileReturn(MIX_REAGENT_STEP_MOTOR_ROTATE_LOCAL_ID,&stepMotorRunAlwaysCmd,&sysResultPack);
    if(sysResultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_MIX_REAGENT_INTEGRATED_MODE10_ROTATE_RUN_ALWAYS,
                                        ERROR_LEVEL_ERROR,sysResultPack.errorSub);
        return;
    }
    //等待运转指定时间
    CoreDelayMsSensitivity(paramUtil);
    //减速停止
    IPC_STEP_MOTOR_CMD_STOP_SLOW stepMotorSlowStopCmd;
    //填充减速停止
    IPC_StepMotorBaseSetStopSlowCmdDefault(&stepMotorSlowStopCmd);
    stepMotorSlowStopCmd.slowDownMode = 1;//快速减速
    stepMotorSlowStopCmd.slowDownSteps = 0;//减速步数,目前没使用
    stepMotorSlowStopCmd.timeOutMs = 5000;//减速停止时间
    //开始减速停止
    IPC_SrvStepMotorStopSlowDownWhileReturn(MIX_REAGENT_STEP_MOTOR_ROTATE_LOCAL_ID,&stepMotorSlowStopCmd,&sysResultPack,NULL);
    if(sysResultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_MIX_REAGENT_INTEGRATED_MODE10_DECC_STOP,
                                        ERROR_LEVEL_ERROR,sysResultPack.errorSub);
        return;
    }
    //回零指令结构体
    IPC_STEP_MOTOR_CMD_RETURN_ZERO stepMotorReturnZeroCmd;
    //读取复位修正
    SysParamReadSingle(INDEX_MAIN_SYS_PARAM_MIX_REAGENT,INDEX_SUB_PARAM_MIX_REAGENT_RESET_CORRECT_UPDOWN,&paramUtil);
    //配置回零指令结构体,升降回零
    IPC_StepMotorBaseSetReturnZeroCmdDefault(&stepMotorReturnZeroCmd);
    stepMotorReturnZeroCmd.needReset = ENABLE;
    stepMotorReturnZeroCmd.timeOutMs = 5000;
    stepMotorReturnZeroCmd.returnZeroCorrectPos = paramUtil;
    stepMotorReturnZeroCmd.runStepCurve = S_CURVE_MIX_REAGENT_UPDOWN_DOWN_MODE10;
    IPC_SrvStepMotorReturnZeroWhileAck(MIX_REAGENT_STEP_MOTOR_UPDOWN_LOCAL_ID,&stepMotorReturnZeroCmd,&sysResultPack);
    if(sysResultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_MIX_REAGENT_INTEGRATED_MODE10_DOWN_RETURN_ZERO,
                                        ERROR_LEVEL_ERROR,sysResultPack.errorSub);
        return;
    }
    //复位指令结构体
    IPC_STEP_MOTOR_CMD_RESET stepMotorResetCmd;
    //读取复位修正
    SysParamReadSingle(INDEX_MAIN_SYS_PARAM_MIX_REAGENT,INDEX_SUB_PARAM_MIX_REAGENT_RESET_CORRECT_ROTATE,&paramUtil);
    //配置复位指令结构体,旋转复位
    IPC_StepMotorBaseSetResetCmdDefault(&stepMotorResetCmd);
    stepMotorResetCmd.resetCorrectCurve = S_CURVE_MIX_REAGENT_ROTATE_DEBUG;//修正曲线
    stepMotorResetCmd.resetCorrectPos = paramUtil;//复位修正
    stepMotorResetCmd.timeOutMs = 5000;//超时时间
    //开始复位
    IPC_SrvStepMotorResetWhileReturn(MIX_REAGENT_STEP_MOTOR_ROTATE_LOCAL_ID,&stepMotorResetCmd,&sysResultPack,NULL);
    if(sysResultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_MIX_REAGENT_INTEGRATED_MODE10_ROTATE_RESET,
                                        ERROR_LEVEL_ERROR,sysResultPack.errorSub);
        return;
    }
    //等待升降电机回零完成
    IPC_SrvStepMotorReturnZeroWaitCmdReturn(MIX_REAGENT_STEP_MOTOR_UPDOWN_LOCAL_ID,&sysResultPack,NULL);
    if(sysResultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_MIX_REAGENT_INTEGRATED_MODE10_DOWN_RETURN_ZERO,
                                        ERROR_LEVEL_ERROR,sysResultPack.errorSub);
        return;
    }
    //构建完成无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

/*--------------------------------------函数指针数组集合--------------------------------------*/
//清洗混匀上升的函数指针数组
static const ModImplMixUpFuncPtr ModImplMixReagentUpFuncPtrArray[] = 
{
    ModImplMixReagentUpMode1,
    ModImplMixReagentUpMode2,
    ModImplMixReagentUpMode3,
    ModImplMixReagentUpMode4,
    ModImplMixReagentUpMode5,
    ModImplMixReagentUpMode6,
    ModImplMixReagentUpMode7,
    ModImplMixReagentUpMode8,
    ModImplMixReagentUpMode9,
    ModImplMixReagentUpMode10,
};

//清洗混匀旋转不带升降的函数指针数组
static const ModImplMixStartRotateFuncPtr ModImplMixReagentStartRotateFuncPtrArray[] = {
    ModImplMixReagentStartRotateMode1,
    ModImplMixReagentStartRotateMode2,
    ModImplMixReagentStartRotateMode3,
    ModImplMixReagentStartRotateMode4,
    ModImplMixReagentStartRotateMode5,
    ModImplMixReagentStartRotateMode6,
    ModImplMixReagentStartRotateMode7,
    ModImplMixReagentStartRotateMode8,
    ModImplMixReagentStartRotateMode9,
    ModImplMixReagentStartRotateMode10,
};

//清洗混匀集成混匀的函数指针数组
static const ModImplMixRotateIntegratedFuncPtr ModImplMixReagentRotateIntegratedFuncPtrArray[] = {
    ModImplMixReagentRotateIntegratedMode1,
    ModImplMixReagentRotateIntegratedMode2,
    ModImplMixReagentRotateIntegratedMode3,
    ModImplMixReagentRotateIntegratedMode4,
    ModImplMixReagentRotateIntegratedMode5,
    ModImplMixReagentRotateIntegratedMode6,
    ModImplMixReagentRotateIntegratedMode7,
    ModImplMixReagentRotateIntegratedMode8,
    ModImplMixReagentRotateIntegratedMode9,
    ModImplMixReagentRotateIntegratedMode10,
};

//复位
void ModImplBaseMixReagentReset(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //设置返回值
    *dataBufLen = 0;
    *dataBufPtrPtr = NULL;
    //执行操作
    //参数配置
    SYS_RESULT_PACK sysResultPack;
    //参数辅助,用于系统参数读取
    int32_t paramUtil;
    //复位指令结构体
    IPC_STEP_MOTOR_CMD_RESET stepMotorResetCmd;
    //读取复位修正
    SysParamReadSingle(INDEX_MAIN_SYS_PARAM_MIX_REAGENT,INDEX_SUB_PARAM_MIX_REAGENT_RESET_CORRECT_UPDOWN,&paramUtil);
    //配置复位指令结构体,升降复位
    IPC_StepMotorBaseSetResetCmdDefault(&stepMotorResetCmd);
    stepMotorResetCmd.resetCorrectCurve = S_CURVE_MIX_REAGENT_UPDOWN_DEBUG;//修正曲线
    stepMotorResetCmd.resetCorrectPos = paramUtil;//复位修正
    stepMotorResetCmd.timeOutMs = 60000;//超时时间
    //开始复位
    IPC_SrvStepMotorResetWhileReturn(MIX_REAGENT_STEP_MOTOR_UPDOWN_LOCAL_ID,&stepMotorResetCmd,&sysResultPack,NULL);
    if(sysResultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_MIX_REAGENT_RESET_UPDOWN,
                                        ERROR_LEVEL_ERROR,sysResultPack.errorSub);
        return;
    }
    //读取复位修正
    SysParamReadSingle(INDEX_MAIN_SYS_PARAM_MIX_REAGENT,INDEX_SUB_PARAM_MIX_REAGENT_RESET_CORRECT_ROTATE,&paramUtil);
    //配置复位指令结构体,旋转复位
    IPC_StepMotorBaseSetResetCmdDefault(&stepMotorResetCmd);
    stepMotorResetCmd.resetCorrectCurve = S_CURVE_MIX_REAGENT_ROTATE_DEBUG;//修正曲线
    stepMotorResetCmd.resetCorrectPos = paramUtil;//复位修正
    stepMotorResetCmd.timeOutMs = 5000;//超时时间
    //开始复位
    IPC_SrvStepMotorResetWhileReturn(MIX_REAGENT_STEP_MOTOR_ROTATE_LOCAL_ID,&stepMotorResetCmd,&sysResultPack,NULL);
    if(sysResultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_MIX_REAGENT_RESET_ROTATE,
                                        ERROR_LEVEL_ERROR,sysResultPack.errorSub);
        return;
    }
    //构建完成无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

//清洗混匀升起来
void ModImplBaseMixReagentUp(MIX_MODE mixMode,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //设置返回值
    *dataBufLen = 0;
    *dataBufPtrPtr = NULL;
    //模式修正
    if(mixMode > MIX_MODE_MAX)
    {
        mixMode = MIX_MODE_MAX;
    }
    //调用相应的函数
    ModImplMixReagentUpFuncPtrArray[mixMode](resultPackPtr);
    if(resultPackPtr->resultCode != PROT_RESULT_SUCCESS)
    {
        //发生错误
        return;
    }
    //构建完成无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}


//清洗混匀降下去
void ModImplBaseMixReagentDown(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //设置返回值
    *dataBufLen = 0;
    *dataBufPtrPtr = NULL;
    //执行操作
    //参数配置
    SYS_RESULT_PACK sysResultPack;
    //参数辅助,用于系统参数读取
    int32_t paramUtil;
    //回零指令结构体
    IPC_STEP_MOTOR_CMD_RETURN_ZERO stepMotorReturnZeroCmd;
    //读取复位修正
    SysParamReadSingle(INDEX_MAIN_SYS_PARAM_MIX_REAGENT,INDEX_SUB_PARAM_MIX_REAGENT_RESET_CORRECT_UPDOWN,&paramUtil);
    //配置回零指令结构体,升降回零
    IPC_StepMotorBaseSetReturnZeroCmdDefault(&stepMotorReturnZeroCmd);
    stepMotorReturnZeroCmd.needReset = ENABLE;
    stepMotorReturnZeroCmd.timeOutMs = 5000;
    stepMotorReturnZeroCmd.returnZeroCorrectPos = paramUtil;
    stepMotorReturnZeroCmd.runStepCurve = S_CURVE_MIX_REAGENT_UPDOWN_DOWN_MODE1;
    IPC_SrvStepMotorReturnZeroWhileReturn(MIX_REAGENT_STEP_MOTOR_UPDOWN_LOCAL_ID,&stepMotorReturnZeroCmd,&sysResultPack,NULL);
    if(sysResultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_MIX_REAGENT_DOWN_UPDOWN_RETURN_ZERO,
                                        ERROR_LEVEL_ERROR,sysResultPack.errorSub);
        return;
    }
    //构建完成无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}


//清洗混匀转一次
void ModImplBaseMixReagentStartRotate(MIX_MODE mixMode,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //设置返回值
    *dataBufLen = 0;
    *dataBufPtrPtr = NULL;
    //模式修正
    if(mixMode > MIX_MODE_MAX)
    {
        mixMode = MIX_MODE_MAX;
    }
    //等待反应杯稳定
    CoreDelayMsSensitivity(MIX_REAHENT_NEED_WAIT_CUP_STABLE_MS);
    //调用相应的函数
    ModImplMixReagentStartRotateFuncPtrArray[mixMode](resultPackPtr);
    if(resultPackPtr->resultCode != PROT_RESULT_SUCCESS)
    {
        //发生错误
        return;
    }
    //构建完成无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}


//集成清洗混匀
void ModImplBaseMixReagentRotateIntegrated(MIX_MODE mixMode,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //设置返回值
    *dataBufLen = 0;
    *dataBufPtrPtr = NULL;
    //模式修正
    if(mixMode > MIX_MODE_MAX)
    {
        mixMode = MIX_MODE_MAX;
    }
    //调用相应的函数
    ModImplMixReagentRotateIntegratedFuncPtrArray[mixMode](resultPackPtr);
    if(resultPackPtr->resultCode != PROT_RESULT_SUCCESS)
    {
        //发生错误
        return;
    }
    //构建完成无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}





















