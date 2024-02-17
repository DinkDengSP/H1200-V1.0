/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-22 09:01:17 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-12-11 15:46:44 +0800
************************************************************************************************/ 
#include "ModImplBasePlateReagent.h"


//输入单元列表 输入序号 有效电平
const MODULE_INPUT_MAP_UNIT plateReagentPortInConfigArray[] = {
    {INPUT_PLATE_REAGENT_ROTATE_ORGIN,VALID_LEVEL_INPUT_PLATE_REAGENT_ROTATE_ORGIN},//旋转电机原点
};
#define LENGTH_MODULE_PLATE_REAGENT_PORT_IN_CONFIG_ARRAY       DIM_ARRAY_ELEMENT_COUNT(plateReagentPortInConfigArray)

//输出单元列表 输入序号 有效电平
const MODULE_OUTPUT_MAP_UNIT plateReagentPortOutConfigArray[] = {
    {MODULE_BASE_OUTPUT_PIN_RESERVE,VALID_LEVEL_MODULE_BASE_OUTPUT_PIN_RESERVE},
};
#define LENGTH_MODULE_PLATE_REAGENT_PORT_OUT_CONFIG_ARRAY       DIM_ARRAY_ELEMENT_COUNT(plateReagentPortOutConfigArray)


//电机控制列表
const MODULE_STEP_MOTOR_CONFIG plateReagentStepMotorConfigArray[] = {
    {PLATE_REAGENT_STEP_MOTOR_ROTATE_SYS_ID,60000,INDEX_MAIN_SYS_PARAM_PLATE_REAGENT,INDEX_SUB_PARAM_PLATE_REAGENT_ROTATE_RESET_CORRECT},//试剂盘旋转电机
};
#define LENGTH_MODULE_PLATE_REAGENT_STEP_MOTOR_CONFIG_ARRAY     DIM_ARRAY_ELEMENT_COUNT(plateReagentStepMotorConfigArray)

//读取模块输入
static ERROR_SUB ModImplBasePlateReagentReadPortInSub(uint8_t portIdx,PIN_STATE* pinStatePtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //调用基础实现
    errorSub = H360SysActModInputRead(portIdx,LENGTH_MODULE_PLATE_REAGENT_PORT_IN_CONFIG_ARRAY,plateReagentPortInConfigArray,
                                        pinStatePtr);
    return errorSub;
}

//写入模块输出
static ERROR_SUB ModImplBasePlateReagentWritePortOutSub(uint8_t portIdx,PIN_STATE setState)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //调用基础实现
    errorSub = H360SysActModOutputWrite(portIdx,setState,LENGTH_MODULE_PLATE_REAGENT_PORT_OUT_CONFIG_ARRAY,plateReagentPortOutConfigArray);
    return errorSub;
}

//步进电机复位
static ERROR_SUB ModImplBasePlateReagentStepMotorResetSub(uint8_t stepMotorIdx,int32_t* motorCoordinatePos)
{
    return CommonSrvImplBaseModuleStepMotorReset(stepMotorIdx,plateReagentStepMotorConfigArray,
                                                    LENGTH_MODULE_PLATE_REAGENT_STEP_MOTOR_CONFIG_ARRAY,
                                                    motorCoordinatePos);
}

//步进电机走位
static ERROR_SUB ModImplBasePlateReagentStepMotorRunStepsSub(uint8_t stepMotorIdx,int32_t runSteps,int32_t* motorCoordinatePos)
{
    return CommonSrvImplBaseModuleStepMotorRunSteps(stepMotorIdx,runSteps,plateReagentStepMotorConfigArray,
                                                        LENGTH_MODULE_PLATE_REAGENT_STEP_MOTOR_CONFIG_ARRAY,
                                                        motorCoordinatePos);
}

//读取模块输入
void ModImplBasePlateReagentReadPortIn(uint8_t portIdx,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    *dataBufLen = 2;
    *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_MOD_TASK,(*dataBufLen));
    ERROR_SUB errorSub = ERROR_SUB_OK;
    PIN_STATE readInputState = PIN_STATE_INVALID;
    //返回数据赋值
    (*dataBufPtrPtr)[0] = portIdx;
    (*dataBufPtrPtr)[1] = readInputState;
    //读取输入
    errorSub = ModImplBasePlateReagentReadPortInSub(portIdx,&readInputState);
    if(errorSub != ERROR_SUB_OK)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_PLATE_REAGENT_READ_MOD_INPUT,
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
void ModImplBasePlateReagentWritePortOut(uint8_t portIdx,PIN_STATE setState,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    *dataBufLen = 1;
    *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_MOD_TASK,(*dataBufLen));
    ERROR_SUB errorSub = ERROR_SUB_OK;
    PIN_STATE readInputState = PIN_STATE_INVALID;
    //返回数据赋值
    (*dataBufPtrPtr)[0] = portIdx;
    //写入输出
    errorSub = ModImplBasePlateReagentWritePortOutSub(portIdx,setState);
    if(errorSub != ERROR_SUB_OK)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_PLATE_REAGENT_WRITE_MOD_OUTPUT,
                                        ERROR_LEVEL_ERROR,errorSub);
        return;
    }
    //构建完成无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

//步进电机复位
void ModImplBasePlateReagentStepMotorReset(uint8_t stepMotorIdx,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    *dataBufLen = 5;
    *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_MOD_TASK,(*dataBufLen));
    ERROR_SUB errorSub = ERROR_SUB_OK;
    int32_t motorPosAfterReset = 0;
    //返回数据赋值
    (*dataBufPtrPtr)[0] = stepMotorIdx;
    ProtUtilWriteInt32ToBuffer(motorPosAfterReset,(*dataBufPtrPtr)+1);
    //电机复位
    errorSub = ModImplBasePlateReagentStepMotorResetSub(stepMotorIdx,&motorPosAfterReset);
    if(errorSub != ERROR_SUB_OK)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_PLATE_REAGENT_STEP_MOTOR_RESET,
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
void ModImplBasePlateReagentStepMotorRunSteps(uint8_t stepMotorIdx,int32_t runSteps,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    *dataBufLen = 5;
    *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_MOD_TASK,(*dataBufLen));
    ERROR_SUB errorSub = ERROR_SUB_OK;
    int32_t motorPosAfterRun = 0;
    //返回数据赋值
    (*dataBufPtrPtr)[0] = stepMotorIdx;
    ProtUtilWriteInt32ToBuffer(motorPosAfterRun,(*dataBufPtrPtr)+1);
    //电机走位
    errorSub = ModImplBasePlateReagentStepMotorRunStepsSub(stepMotorIdx,runSteps,&motorPosAfterRun);
    if(errorSub != ERROR_SUB_OK)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_PLATE_REAGENT_STEP_MOTOR_RUN_STEP,
                                        ERROR_LEVEL_ERROR,errorSub);
        return;
    }
    //写入参数
    ProtUtilWriteInt32ToBuffer(motorPosAfterRun,(*dataBufPtrPtr)+1);
    //构建完成无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}
/*-----------------------------------------------------内部辅助变量----------------------------------------------------------------*/
//当前试剂盘杯位的杯位序号
static uint8_t currentPlateReactionNewCupHoleIndex = PLATE_REAGENT_INIT_CUP_INDEX;
//当前试剂盘偏移
static PLATE_REAGENT_OFFSET currentPlateReagentOffset = PLATE_REAGENT_INIT_OFFSET;
/* 当前扫码偏移 */
static PLATE_REAGENT_SCAN_OFFSET currentScanOffset = PLATE_REAGENT_INIT_SCAN_OFFSET;

/*-----------------------------------------------------内部辅助API----------------------------------------------------------------*/
//计算试剂盘运行曲线
static SM_CURVE_SELECT ModImplBasePlateReagentCalcStepMotorCurve(int32_t steps)
{
    //先把负数步数修正为正数步数
    if(steps < 0)
    {
        steps = 0 - steps;
    }
    //首先获取一格的脉冲
    int32_t oneCupOffset = PLATE_REAGENT_ONE_HOLE_OFFSET_STEPS;
    //参数为0,报错
    if(oneCupOffset == 0)
    {
        return S_CURVE_PLATE_REAGENT_ROTATE_DEBUG;
    }
    //一格对应步数修正
    if(oneCupOffset < 0)
    {
        oneCupOffset = 0 - oneCupOffset;
    }
    //计算目前走的坐标对应几格
    int32_t cupCount =  (steps/oneCupOffset);
    if((steps%oneCupOffset) != 0)
    {
        cupCount += 1;
    }
    //计算曲线
    if(cupCount <= 3)
    {
        return S_CURVE_PLATE_REAGENT_ROTATE_MOVE_1_3_CUP;
    }
    else if(cupCount <= 6)
    {
        return S_CURVE_PLATE_REAGENT_ROTATE_MOVE_4_6_CUP;
    }
    else if(cupCount <= 9)
    {
        return S_CURVE_PLATE_REAGENT_ROTATE_MOVE_7_9_CUP;
    }
    else if(cupCount <= 12)
    {
        return S_CURVE_PLATE_REAGENT_ROTATE_MOVE_10_12_CUP;
    }
    else if(cupCount <= 15)
    {
        return S_CURVE_PLATE_REAGENT_ROTATE_MOVE_13_15_CUP;
    }
    else
    {
        return S_CURVE_PLATE_REAGENT_ROTATE_DEBUG;
    }
}

//计算从当前杯位到目标杯位需要运行的杯位数量
static int8_t ModImplBasePlateReagentCalcHoleCountNeedRun(uint8_t targetCupIndex)
{
    int8_t holeRunCount = 0;
    if(targetCupIndex == currentPlateReactionNewCupHoleIndex)
    {
        holeRunCount = 0;
    }
    else if(targetCupIndex > currentPlateReactionNewCupHoleIndex)
    {
        //目标孔位大于源孔位
        if((targetCupIndex - currentPlateReactionNewCupHoleIndex) <= (PLATE_REAGENT_MAX_CUP_INDEX/2))
        {
            //正向半圈以内
            holeRunCount = targetCupIndex - currentPlateReactionNewCupHoleIndex;
        }
        else
        {
            //超过半圈,反向
            holeRunCount = PLATE_REAGENT_MAX_CUP_INDEX - targetCupIndex + currentPlateReactionNewCupHoleIndex;
            holeRunCount = 0 - holeRunCount;
        }
    }
    else
    {
        //目标孔位小于源孔位
        if((currentPlateReactionNewCupHoleIndex - targetCupIndex) <= (PLATE_REAGENT_MAX_CUP_INDEX/2))
        {
            holeRunCount = currentPlateReactionNewCupHoleIndex - targetCupIndex;
            holeRunCount = 0 - holeRunCount;
        }
        else
        {
            holeRunCount = PLATE_REAGENT_MAX_CUP_INDEX - currentPlateReactionNewCupHoleIndex + targetCupIndex;
        }
    }
    return holeRunCount;
}

//获取偏移步数
static int32_t ModImplBasePlateReagentGetOffsetPos(PLATE_REAGENT_OFFSET offset)
{
    int32_t paramUtil = 0;
    if(offset == PLATE_REAGENT_OFFSET_NONE)
    {
        paramUtil = 0;
    }
    else if(offset == PLATE_REAGENT_OFFSET_NEEDLE_REAGENT_R1)
    {
        SysParamReadSingle(INDEX_MAIN_SYS_PARAM_PLATE_REAGENT,INDEX_SUB_PARAM_PLATE_REAGENT_OFFSET_NEEDLE_REAGENT_WITH_R1,&paramUtil);
    }
    else if(offset == PLATE_REAGENT_OFFSET_NEEDLE_REAGENT_R2)
    {
        SysParamReadSingle(INDEX_MAIN_SYS_PARAM_PLATE_REAGENT,INDEX_SUB_PARAM_PLATE_REAGENT_OFFSET_NEEDLE_REAGENT_WITH_R2,&paramUtil);
    }
    else if(offset == PLATE_REAGENT_OFFSET_NEEDLE_BEAD_R1)
    {
        SysParamReadSingle(INDEX_MAIN_SYS_PARAM_PLATE_REAGENT,INDEX_SUB_PARAM_PLATE_REAGENT_OFFSET_NEEDLE_BEAD_WITH_R1,&paramUtil);
    }
    else if(offset == PLATE_REAGENT_OFFSET_NEEDLE_BEAD_R2)
    {
        SysParamReadSingle(INDEX_MAIN_SYS_PARAM_PLATE_REAGENT,INDEX_SUB_PARAM_PLATE_REAGENT_OFFSET_NEEDLE_BEAD_WITH_R2,&paramUtil);
    }
    else if(offset == PLATE_REAGENT_OFFSET_NEEDLE_BEAD_BEAD)
    {
        SysParamReadSingle(INDEX_MAIN_SYS_PARAM_PLATE_REAGENT,INDEX_SUB_PARAM_PLATE_REAGENT_OFFSET_NEEDLE_BEAD_WITH_BEAD,&paramUtil);
    }
    else if(offset == PLATE_REAGENT_OFFSET_QR_SCAN)
    {
        SysParamReadSingle(INDEX_MAIN_SYS_PARAM_PLATE_REAGENT,INDEX_SUB_PARAM_PLATE_REAGENT_OFFSET_WITH_QR_SCAN,&paramUtil);
    }
    else
    {
        paramUtil = 0;
    }
    return paramUtil;
}

/* 获取扫码偏移步数 */
static int32_t ModImplBasePlateReagentGetScanOffsetPos(PLATE_REAGENT_SCAN_OFFSET offset)
{
    int32_t paramUtil = 0;
    if(offset == PLATE_REAGENT_SCAN_OFFSET_START)
    {
        paramUtil += PLATE_REAGENT_MOVE_SCAN_OFFSET;
    }
    else if(offset == PLATE_REAGENT_SCAN_OFFSET_END)
    {
        paramUtil -= PLATE_REAGENT_MOVE_SCAN_OFFSET;
    }
    else
    {
        paramUtil = 0;
    }
    return paramUtil;
}

//试剂盘复位
static ERROR_SUB ModImplBasePlateReagentMotorRotateReset(void)
{
    //参数配置
    ERROR_SUB errorSub = ERROR_SUB_OK;
    SYS_RESULT_PACK sysResultPack;
    //复位指令结构体
    IPC_STEP_MOTOR_CMD_RESET stepMotorResetCmd;
    //配置复位指令结构体,旋转电机复位
    IPC_StepMotorBaseSetResetCmdDefault(&stepMotorResetCmd);
    stepMotorResetCmd.resetCorrectCurve = S_CURVE_PLATE_REAGENT_ROTATE_DEBUG;//修正曲线
    stepMotorResetCmd.resetCorrectPos = 0;//复位修正
    stepMotorResetCmd.timeOutMs = 60000;//超时时间
    //开始复位
    IPC_SrvStepMotorResetWhileReturn(PLATE_REAGENT_STEP_MOTOR_ROTATE_LOCAL_ID,&stepMotorResetCmd,&sysResultPack,NULL);
    if(sysResultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        return sysResultPack.errorSub;
    }
    return ERROR_SUB_OK;
}

//试剂盘运行到零位偏移
static ERROR_SUB ModImplBasePlateReagentMotorRotateRunToCorrect(void)
{
    //参数配置
    ERROR_SUB errorSub = ERROR_SUB_OK;
    SYS_RESULT_PACK sysResultPack;
    //参数辅助,用于系统参数读取
    int32_t paramUtil = 0;
    //走坐标
    IPC_STEP_MOTOR_CMD_RUN_COORDINATE stepMotorRunCoordinateCmd;
    //读取参数
    SysParamReadSingle(INDEX_MAIN_SYS_PARAM_PLATE_REAGENT,INDEX_SUB_PARAM_PLATE_REAGENT_ROTATE_RESET_CORRECT,&paramUtil);
    //走坐标指令初始化
    IPC_StepMotorBaseSetRunCoordinateCmdDefault(&stepMotorRunCoordinateCmd);
    stepMotorRunCoordinateCmd.runStepCurve = S_CURVE_PLATE_REAGENT_ROTATE_MOVE_13_15_CUP;
    stepMotorRunCoordinateCmd.targetCoordinate = paramUtil;
    stepMotorRunCoordinateCmd.timeOutMs = 60000,
    //电机运转
    IPC_SrvStepMotorRunCoordinateWhileReturn(PLATE_REAGENT_STEP_MOTOR_ROTATE_LOCAL_ID,&stepMotorRunCoordinateCmd,&sysResultPack,NULL);
    if(sysResultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        //构建错误代码
        return sysResultPack.errorSub;
    }
    return ERROR_SUB_OK;
}

//试剂盘运行到指定位置
static ERROR_SUB ModImplBasePlateReagentMotorRotateRunToPos(int32_t targetPos,SM_CURVE_SELECT selectCurve)
{
    //参数配置
    ERROR_SUB errorSub = ERROR_SUB_OK;
    SYS_RESULT_PACK sysResultPack;
    //走坐标
    IPC_STEP_MOTOR_CMD_RUN_COORDINATE stepMotorRunCoordinateCmd;
    //走坐标指令初始化
    IPC_StepMotorBaseSetRunCoordinateCmdDefault(&stepMotorRunCoordinateCmd);
    stepMotorRunCoordinateCmd.runStepCurve = selectCurve;
    stepMotorRunCoordinateCmd.targetCoordinate = targetPos;
    stepMotorRunCoordinateCmd.timeOutMs = 60000,
    //电机抬起来
    IPC_SrvStepMotorRunCoordinateWhileReturn(PLATE_REAGENT_STEP_MOTOR_ROTATE_LOCAL_ID,&stepMotorRunCoordinateCmd,&sysResultPack,NULL);
    if(sysResultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        //构建错误代码
        return sysResultPack.errorSub;
    }
    return ERROR_SUB_OK;
}

//试剂盘运行指定步数
static ERROR_SUB ModImplBasePlateReagentMotorRotateRunSteps(int32_t stepsRun,SM_CURVE_SELECT selectCurve)
{
    //参数配置
    ERROR_SUB errorSub = ERROR_SUB_OK;
    SYS_RESULT_PACK sysResultPack;
    //走步数结构体
    IPC_STEP_MOTOR_CMD_RUN_STEP stepMotorRunStepCmd;
    IPC_StepMotorBaseSetRunStepCmdDefault(&stepMotorRunStepCmd);
    stepMotorRunStepCmd.totalSteps = stepsRun;//步数
    stepMotorRunStepCmd.runStepCurve = selectCurve;//曲线
    IPC_SrvStepMotorRunStepWhileReturn(PLATE_REAGENT_STEP_MOTOR_ROTATE_LOCAL_ID,&stepMotorRunStepCmd,&sysResultPack,NULL);
    //走步数
    if(sysResultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        //构建错误代码
        return sysResultPack.errorSub;
    }
    return ERROR_SUB_OK;
}

/*-----------------------------------------------------外部实现API----------------------------------------------------------------*/
//试剂盘复位
void ModImplBasePlateReagentReset(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //返回数据缓存
    *dataBufLen = 2;
    *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_MOD_TASK,(*dataBufLen));
    //构建完成无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    //调用实现
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //首先试剂盘复位
    errorSub = ModImplBasePlateReagentMotorRotateReset();
    if(errorSub != ERROR_SUB_OK)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_PLATE_REAGENT_RESET_ROTATE_RESET,
                                        ERROR_LEVEL_ERROR,errorSub);
        return;
    }
    //试剂盘运行修正
    errorSub = ModImplBasePlateReagentMotorRotateRunToCorrect();
    if(errorSub != ERROR_SUB_OK)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_PLATE_REAGENT_RESET_ROTATE_CORRECT,
                                        ERROR_LEVEL_ERROR,errorSub);
        return;
    }
    //设置初始值
    currentPlateReactionNewCupHoleIndex = PLATE_REAGENT_INIT_CUP_INDEX;
    currentPlateReagentOffset = PLATE_REAGENT_INIT_OFFSET;
    currentScanOffset = PLATE_REAGENT_INIT_SCAN_OFFSET;
    //数据填充
    (*dataBufPtrPtr)[0] = (uint8_t)currentPlateReactionNewCupHoleIndex;
    (*dataBufPtrPtr)[1] = (uint8_t)currentPlateReagentOffset;
    //返回
    return;
}

//试剂盘运行到指定序号杯位,带偏移
void ModImplBasePlateReagentRun2CupWithOffset(uint8_t targetCupIndex,PLATE_REAGENT_OFFSET targetOffset,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,
                                                uint16_t* dataBufLen)
{
    //返回数据缓存
    *dataBufLen = 2;
    *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_MOD_TASK,(*dataBufLen));
    //构建完成无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    //调用实现
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //检查目标杯位
    if((targetCupIndex == 0)||(targetCupIndex > PLATE_REAGENT_MAX_CUP_INDEX))
    {
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_PLATE_REAGENT_RUN_TO_CUP_OFFSET,
                                        ERROR_LEVEL_ERROR,ERROR_SUB_H360_PLATE_REAGENT_TARGET_CUP_LIMIT);
        return;
    }
    //检查偏移
    if(targetOffset > PLATE_REAGENT_OFFSET_QR_SCAN)
    {
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_PLATE_REAGENT_RUN_TO_CUP_OFFSET,
                                        ERROR_LEVEL_ERROR,ERROR_SUB_H360_PLATE_REAGENT_TARGET_OFFSET_LIMIT);
        return;
    }
    int32_t stepsNextRun = 0;
    int8_t holeRunCount = 0;
    //计算需要运行的孔位数量
    holeRunCount = ModImplBasePlateReagentCalcHoleCountNeedRun(targetCupIndex);
    //计算需要运行的孔位步数总数
    int32_t paramUtil = PLATE_REAGENT_ONE_HOLE_OFFSET_STEPS;
    stepsNextRun = paramUtil * holeRunCount;
    //先确定曲线
    SM_CURVE_SELECT curveSelect = ModImplBasePlateReagentCalcStepMotorCurve(stepsNextRun);
    //计算并去掉偏移步数
    paramUtil = ModImplBasePlateReagentGetOffsetPos(currentPlateReagentOffset);
    stepsNextRun -= paramUtil;
    //计算并加上目标偏移步数
    paramUtil = ModImplBasePlateReagentGetOffsetPos(targetOffset);
    stepsNextRun += paramUtil;
    //计算扫码偏移
    paramUtil = ModImplBasePlateReagentGetScanOffsetPos(currentScanOffset);
    stepsNextRun += paramUtil;
    //运行指定步数
    errorSub = ModImplBasePlateReagentMotorRotateRunSteps(stepsNextRun,curveSelect);
    if(errorSub != ERROR_SUB_OK)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_PLATE_REAGENT_RUN_TO_CUP_OFFSET_ROTATE_RUN_STEP,
                                        ERROR_LEVEL_ERROR,errorSub);
        return;
    }
    //走步数完成,设置位置
    currentPlateReactionNewCupHoleIndex = targetCupIndex;
    currentPlateReagentOffset = targetOffset;
    currentScanOffset = PLATE_REAGENT_INIT_SCAN_OFFSET;
    //数据填充
    (*dataBufPtrPtr)[0] = (uint8_t)currentPlateReactionNewCupHoleIndex;
    (*dataBufPtrPtr)[1] = (uint8_t)currentPlateReagentOffset;
    //返回
    return;
}

//试剂盘运行指定数量个杯位,基于当前位置
void ModImplBasePlateReagentRunAnyCups(uint8_t cups,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //返回数据缓存
    *dataBufLen = 2;
    *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_MOD_TASK,(*dataBufLen));
    //构建完成无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    //调用实现
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //检查运行杯位数量
    if((cups > PLATE_REAGENT_MAX_CUP_INDEX))
    {
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_PLATE_REAGENT_RUN_ANY_CUPS,
                                        ERROR_LEVEL_ERROR,ERROR_SUB_H360_PLATE_REAGENT_TARGET_CUP_LIMIT);
        return;
    }
    int32_t stepsNextRun = 0;
    int8_t holeRunCount = 0;
    //获取目标孔位
    uint8_t targetHoleIndex = currentPlateReactionNewCupHoleIndex;
    targetHoleIndex += cups;
    if(targetHoleIndex > PLATE_REAGENT_MAX_CUP_INDEX)
    {
        targetHoleIndex %=  PLATE_REAGENT_MAX_CUP_INDEX;
    }
    //计算需要运行的孔位数量
    holeRunCount = ModImplBasePlateReagentCalcHoleCountNeedRun(targetHoleIndex);
    //计算需要运行的孔位步数总数
    int32_t paramUtil = PLATE_REAGENT_ONE_HOLE_OFFSET_STEPS;
    stepsNextRun = paramUtil * holeRunCount;
    SM_CURVE_SELECT curveSelect = ModImplBasePlateReagentCalcStepMotorCurve(stepsNextRun);
    //走步数
    errorSub = ModImplBasePlateReagentMotorRotateRunSteps(stepsNextRun,curveSelect);
    if(errorSub != ERROR_SUB_OK)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_PLATE_REAGENT_RUN_ANY_CUPS_ROTATE_RUN_STEP,
                                        ERROR_LEVEL_ERROR,errorSub);
        return;
    }
    //走位完成,设置当前位置
    currentPlateReactionNewCupHoleIndex = targetHoleIndex;
    //数据填充
    (*dataBufPtrPtr)[0] = (uint8_t)currentPlateReactionNewCupHoleIndex;
    (*dataBufPtrPtr)[1] = (uint8_t)currentPlateReagentOffset;
    //返回
    return;
}


//试剂盘运行到指定序号杯位,带偏移
void ModImplBasePlateReagentRun2CupWithOffsetForScan(uint8_t targetCupIndex,PLATE_REAGENT_SCAN_OFFSET scanOffset,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,
                                                        uint16_t* dataBufLen)
{
    //返回数据缓存
    *dataBufLen = 2;
    *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_MOD_TASK,(*dataBufLen));
    //构建完成无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    //调用实现
    ERROR_SUB errorSub = ERROR_SUB_OK;
    int32_t stepsNextRun = 0;
    int8_t holeRunCount = 0;
    SM_CURVE_SELECT curveSelect = S_CURVE_PLATE_REAGENT_ROTATE_DEBUG;
    //目标杯位错误
    if((targetCupIndex == 0)||(targetCupIndex > PLATE_REAGENT_MAX_CUP_INDEX))
    {
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_PLATE_REAGENT_RUN_TO_CUP_FOR_SCAN,
                                        ERROR_LEVEL_ERROR,ERROR_SUB_H360_PLATE_REAGENT_TARGET_CUP_LIMIT);
        return;
    }
    //偏移错误
    if(scanOffset > PLATE_REAGENT_SCAN_OFFSET_END)
    {
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_PLATE_REAGENT_RUN_TO_CUP_FOR_SCAN,
                                        ERROR_LEVEL_ERROR,ERROR_SUB_H360_PLATE_REAGENT_BAR_SCAN_OFFSET_LIMIT);
        return;
    }
    //计算需要运行的孔位数量
    holeRunCount = ModImplBasePlateReagentCalcHoleCountNeedRun(targetCupIndex);
    //计算需要运行的孔位步数总数
    int32_t paramUtil = PLATE_REAGENT_ONE_HOLE_OFFSET_STEPS;
    stepsNextRun = paramUtil * holeRunCount;
    //计算并去掉偏移步数
    paramUtil = ModImplBasePlateReagentGetOffsetPos(currentPlateReagentOffset);
    stepsNextRun -= paramUtil;
    //计算并加上扫码偏移步数
    paramUtil = ModImplBasePlateReagentGetOffsetPos(PLATE_REAGENT_OFFSET_QR_SCAN);
    stepsNextRun += paramUtil;
    //计算扫码偏移
    paramUtil = ModImplBasePlateReagentGetScanOffsetPos(currentScanOffset);
    stepsNextRun += paramUtil;
    //扫码起始点位置
    if(scanOffset == PLATE_REAGENT_SCAN_OFFSET_START)
    {
        stepsNextRun -= PLATE_REAGENT_MOVE_SCAN_OFFSET; 
        //填充走步数结构体
        curveSelect = ModImplBasePlateReagentCalcStepMotorCurve(stepsNextRun);
    }
    else if(scanOffset == PLATE_REAGENT_SCAN_OFFSET_END)
    {
        stepsNextRun +=PLATE_REAGENT_MOVE_SCAN_OFFSET;
        curveSelect = S_CURVE_PLATE_REAGENT_ROTATE_MOVE_FOR_BARSCAN;
    }
    else
    {
        //填充走步数结构体
        curveSelect = ModImplBasePlateReagentCalcStepMotorCurve(stepsNextRun);
    }
    //运行指定步数
    errorSub = ModImplBasePlateReagentMotorRotateRunSteps(stepsNextRun,curveSelect);
    if(errorSub != ERROR_SUB_OK)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_PLATE_REAGENT_RUN_TO_CUP_FOR_SCAN_ROTATE_RUN_STEPS,
                                        ERROR_LEVEL_ERROR,errorSub);
        return;
    }
    //设置当前位置
    currentPlateReactionNewCupHoleIndex = targetCupIndex;
    currentPlateReagentOffset = PLATE_REAGENT_OFFSET_QR_SCAN;
    currentScanOffset = scanOffset;
    //数据填充
    (*dataBufPtrPtr)[0] = (uint8_t)currentPlateReactionNewCupHoleIndex;
    (*dataBufPtrPtr)[1] = (uint8_t)currentPlateReagentOffset;
    //返回
    return;
}










