/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-27 17:08:07 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-12-07 08:45:55 +0800
************************************************************************************************/ 
#include "ModImplBasePlateReaction.h"
#include "ModTaskHeader.h"


//输入单元列表 输入序号 有效电平
const MODULE_INPUT_MAP_UNIT plateReactionPortInConfigArray[] = {
    {INPUT_PLATE_REACTION_ROTATE_ORGIN      ,VALID_LEVEL_INPUT_PLATE_REACTION_ROTATE_ORGIN  },//旋转电机原点
    {INPUT_PLATE_REACTION_ENCODER           ,VALID_LEVEL_INPUT_PLATE_REACTION_ENCODER       },//反应盘码盘
    {INPUT_PLATE_REACTION_NEW_CUP_LIGHT     ,VALID_LEVEL_INPUT_PLATE_REACTION_NEW_CUP_LIGHT },//新杯检测有无光纤传感器信号
    {INPUT_PLATE_REACTION_OLD_CUP_LIGHT     ,VALID_LEVEL_INPUT_PLATE_REACTION_OLD_CUP_LIGHT },//旧杯检测有无光纤传感器信号
};
#define LENGTH_MODULE_PLATE_REACTION_PORT_IN_CONFIG_ARRAY       DIM_ARRAY_ELEMENT_COUNT(plateReactionPortInConfigArray)

//输出单元列表 输入序号 有效电平
const MODULE_OUTPUT_MAP_UNIT plateReactionPortOutConfigArray[] = {
    {MODULE_BASE_OUTPUT_PIN_RESERVE,VALID_LEVEL_MODULE_BASE_OUTPUT_PIN_RESERVE},
};
#define LENGTH_MODULE_PLATE_REACTION_PORT_OUT_CONFIG_ARRAY       DIM_ARRAY_ELEMENT_COUNT(plateReactionPortOutConfigArray)


//电机控制列表
const MODULE_STEP_MOTOR_CONFIG plateReactionStepMotorConfigArray[] = {
    {PLATE_REACTION_STEP_MOTOR_ROTATE_SYS_ID,60000,INDEX_MAIN_SYS_PARAM_PLATE_REACTION,INDEX_SUB_PARAM_PLATE_REACTION_RESET_CORRECT},//反应盘旋转电机
};
#define LENGTH_MODULE_PLATE_REACTION_STEP_MOTOR_CONFIG_ARRAY     DIM_ARRAY_ELEMENT_COUNT(plateReactionStepMotorConfigArray)

//读取模块输入
static ERROR_SUB ModImplBasePlateReactionReadPortInSub(uint8_t portIdx,PIN_STATE* pinStatePtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //调用基础实现
    errorSub = H360SysActModInputRead(portIdx,LENGTH_MODULE_PLATE_REACTION_PORT_IN_CONFIG_ARRAY,plateReactionPortInConfigArray,
                                        pinStatePtr);
    return errorSub;
}

//写入模块输出
static ERROR_SUB ModImplBasePlateReactionWritePortOutSub(uint8_t portIdx,PIN_STATE setState)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //调用基础实现
    errorSub = H360SysActModOutputWrite(portIdx,setState,LENGTH_MODULE_PLATE_REACTION_PORT_OUT_CONFIG_ARRAY,plateReactionPortOutConfigArray);
    return errorSub;
}

//步进电机复位
static ERROR_SUB ModImplBasePlateReactionStepMotorResetSub(uint8_t stepMotorIdx,int32_t* motorCoordinatePos)
{
    return CommonSrvImplBaseModuleStepMotorReset(stepMotorIdx,plateReactionStepMotorConfigArray,
                                                    LENGTH_MODULE_PLATE_REACTION_STEP_MOTOR_CONFIG_ARRAY,
                                                    motorCoordinatePos);
}

//步进电机走位
static ERROR_SUB ModImplBasePlateReactionStepMotorRunStepsSub(uint8_t stepMotorIdx,int32_t runSteps,int32_t* motorCoordinatePos)
{
    return CommonSrvImplBaseModuleStepMotorRunSteps(stepMotorIdx,runSteps,plateReactionStepMotorConfigArray,
                                                        LENGTH_MODULE_PLATE_REACTION_STEP_MOTOR_CONFIG_ARRAY,
                                                        motorCoordinatePos);
}

//读取模块输入
void ModImplBasePlateReactionReadPortIn(uint8_t portIdx,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    *dataBufLen = 2;
    *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_MOD_TASK,(*dataBufLen));
    ERROR_SUB errorSub = ERROR_SUB_OK;
    PIN_STATE readInputState = PIN_STATE_INVALID;
    //返回数据赋值
    (*dataBufPtrPtr)[0] = portIdx;
    (*dataBufPtrPtr)[1] = readInputState;
    //读取输入
    errorSub = ModImplBasePlateReactionReadPortInSub(portIdx,&readInputState);
    if(errorSub != ERROR_SUB_OK)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_PLATE_REACTION_READ_MOD_INPUT,
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
void ModImplBasePlateReactionWritePortOut(uint8_t portIdx,PIN_STATE setState,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    *dataBufLen = 1;
    *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_MOD_TASK,(*dataBufLen));
    ERROR_SUB errorSub = ERROR_SUB_OK;
    PIN_STATE readInputState = PIN_STATE_INVALID;
    //返回数据赋值
    (*dataBufPtrPtr)[0] = portIdx;
    //写入输出
    errorSub = ModImplBasePlateReactionWritePortOutSub(portIdx,setState);
    if(errorSub != ERROR_SUB_OK)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_PLATE_REACTION_WRITE_MOD_OUTPUT,
                                        ERROR_LEVEL_ERROR,errorSub);
        return;
    }
    //构建完成无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

//步进电机复位
void ModImplBasePlateReactionStepMotorReset(uint8_t stepMotorIdx,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    *dataBufLen = 5;
    *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_MOD_TASK,(*dataBufLen));
    ERROR_SUB errorSub = ERROR_SUB_OK;
    int32_t motorPosAfterReset = 0;
    //返回数据赋值
    (*dataBufPtrPtr)[0] = stepMotorIdx;
    ProtUtilWriteInt32ToBuffer(motorPosAfterReset,(*dataBufPtrPtr)+1);
    //电机复位
    errorSub = ModImplBasePlateReactionStepMotorResetSub(stepMotorIdx,&motorPosAfterReset);
    if(errorSub != ERROR_SUB_OK)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_PLATE_REACTION_STEP_MOTOR_RESET,
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
void ModImplBasePlateReactionStepMotorRunSteps(uint8_t stepMotorIdx,int32_t runSteps,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    *dataBufLen = 5;
    *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_MOD_TASK,(*dataBufLen));
    ERROR_SUB errorSub = ERROR_SUB_OK;
    int32_t motorPosAfterRun = 0;
    //返回数据赋值
    (*dataBufPtrPtr)[0] = stepMotorIdx;
    ProtUtilWriteInt32ToBuffer(motorPosAfterRun,(*dataBufPtrPtr)+1);
    //电机走位
    errorSub = ModImplBasePlateReactionStepMotorRunStepsSub(stepMotorIdx,runSteps,&motorPosAfterRun);
    if(errorSub != ERROR_SUB_OK)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_PLATE_REACTION_STEP_MOTOR_RUN_STEP,
                                        ERROR_LEVEL_ERROR,errorSub);
        return;
    }
    //写入参数
    ProtUtilWriteInt32ToBuffer(motorPosAfterRun,(*dataBufPtrPtr)+1);
    //构建完成无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}


//计算反应盘运行曲线
static SM_CURVE_SELECT ModImplBasePlateReactionCalcStepMotorCurve(int32_t steps)
{
    //先把负数步数修正为正数步数
    if(steps < 0)
    {
        steps = 0 - steps;
    }
    //首先获取一格的脉冲
    int32_t oneCupOffset = PLATE_REACTION_ONE_HOLE_OFFSET_STEPS;
    //参数为0,报错
    if(oneCupOffset == 0)
    {
        return S_CURVE_PLATE_REACTION_ROTATE_DEBUG;
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
    if(cupCount <= 1)
    {
        return S_CURVE_PLATE_REACTION_ROTATE_MOVE_1_CUP;
    }
    else if(cupCount <= 9)
    {
        return S_CURVE_PLATE_REACTION_ROTATE_MOVE_9_CUP;
    }
    else if(cupCount <= 13)
    {
        return S_CURVE_PLATE_REACTION_ROTATE_MOVE_13_CUP;
    }
    else if(cupCount <= 40)
    {
        return S_CURVE_PLATE_REACTION_ROTATE_MOVE_40_CUP;
    }
    else if(cupCount <= 59)
    {
        return S_CURVE_PLATE_REACTION_ROTATE_MOVE_59_CUP;
    }
    else if(cupCount <= 120)
    {
        return S_CURVE_PLATE_REACTION_ROTATE_MOVE_120_CUP;
    }
    else
    {
        return S_CURVE_PLATE_REACTION_ROTATE_DEBUG;
    }
}

//读取新杯光纤
static ERROR_SUB ModImplBasePlateReactionReadNewCupSensor(PIN_STATE* newCupSensorStatePtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    GPIO_PinState inputState;
    *newCupSensorStatePtr = PIN_STATE_INVALID;
    //读取新杯光纤
    errorSub = H360SysActPortInRead(INPUT_PLATE_REACTION_NEW_CUP_LIGHT,&inputState);
    if(errorSub != ERROR_SUB_OK)
    {
        return errorSub;
    }
    //转换光纤状态
    if(inputState == VALID_LEVEL_INPUT_PLATE_REACTION_NEW_CUP_LIGHT)
    {
        *newCupSensorStatePtr = PIN_STATE_VALID;
    }
    else
    {
        *newCupSensorStatePtr = PIN_STATE_INVALID;
    }
    return ERROR_SUB_OK;
}

//读取旧杯光纤
static ERROR_SUB ModImplBasePlateReactionReadOldCupSensor(PIN_STATE* oldCupSensorStatePtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    GPIO_PinState inputState;
    *oldCupSensorStatePtr = PIN_STATE_INVALID;
    //读取新杯光纤
    errorSub = H360SysActPortInRead(INPUT_PLATE_REACTION_OLD_CUP_LIGHT,&inputState);
    if(errorSub != ERROR_SUB_OK)
    {
        return errorSub;
    }
    //转换光纤状态
    if(inputState == VALID_LEVEL_INPUT_PLATE_REACTION_OLD_CUP_LIGHT)
    {
        *oldCupSensorStatePtr = PIN_STATE_VALID;
    }
    else
    {
        *oldCupSensorStatePtr = PIN_STATE_INVALID;
    }
    return ERROR_SUB_OK;
}

//当前反应盘新杯位的杯位序号
static uint8_t currentPlateReactionNewCupHoleIndex = 0;

//复位
void ModImplBasePlateReactionReset(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //返回数据缓存
    *dataBufLen = 3;
    *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_MOD_TASK,(*dataBufLen));
    //构建完成无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    //参数配置
    ERROR_SUB errorSub = ERROR_SUB_OK;
    SYS_RESULT_PACK sysResultPack;
    //参数辅助,用于系统参数读取
    int32_t paramUtil;
    //复位指令结构体
    IPC_STEP_MOTOR_CMD_RESET stepMotorResetCmd;
    //读取复位修正
    SysParamReadSingle(INDEX_MAIN_SYS_PARAM_PLATE_REACTION,INDEX_SUB_PARAM_PLATE_REACTION_RESET_CORRECT,&paramUtil);
    //配置复位指令结构体,旋转电机复位
    IPC_StepMotorBaseSetResetCmdDefault(&stepMotorResetCmd);
    stepMotorResetCmd.resetCorrectCurve = S_CURVE_PLATE_REACTION_ROTATE_DEBUG;//修正曲线
    stepMotorResetCmd.resetCorrectPos = paramUtil;//复位修正
    stepMotorResetCmd.timeOutMs = 60000;//超时时间
    //开始复位
    IPC_SrvStepMotorResetWhileReturn(PLATE_REACTION_STEP_MOTOR_ROTATE_LOCAL_ID,&stepMotorResetCmd,&sysResultPack,NULL);
    if(sysResultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_PLATE_REACTION_RESET_ROTATE_MOTOR_RST,
                                        ERROR_LEVEL_ERROR,sysResultPack.errorSub);
        return;
    }
    //设置当前序号为复位位置
    currentPlateReactionNewCupHoleIndex = PLATE_REACTION_INIT_CUP_INDEX;
    //复位完成,等待指定光纤稳定时间
    CoreDelayMsSensitivity(TIME_MS_PLATE_REACTION_WAIT_LIGHT_STABLE);
    //数据填充
    (*dataBufPtrPtr)[0] = (uint8_t)(currentPlateReactionNewCupHoleIndex);
    //检杯光纤
    PIN_STATE cupSensorState = PIN_STATE_INVALID;
    //读取新杯光纤
    errorSub = ModImplBasePlateReactionReadNewCupSensor(&cupSensorState);
    if(errorSub != ERROR_SUB_OK)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_PLATE_REACTION_RESET_SENSOR_NEW_CUP,
                                        ERROR_LEVEL_ERROR,errorSub);
        return;
    }
    //数据填充
    (*dataBufPtrPtr)[1] = (uint8_t)(cupSensorState);
    //读取旧杯光纤
    errorSub = ModImplBasePlateReactionReadOldCupSensor(&cupSensorState);
    if(errorSub != ERROR_SUB_OK)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_PLATE_REACTION_RESET_SENSOR_OLD_CUP,
                                        ERROR_LEVEL_ERROR,errorSub);
        return;
    }
    //数据填充
    (*dataBufPtrPtr)[2] = (uint8_t)(cupSensorState);
    //返回完成
    return;
}

//正向运转到指定杯位
void ModImplBasePlateReactionRunIndexCupPositive(uint8_t targetCup,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //返回数据缓存
    *dataBufLen = 3;
    *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_MOD_TASK,(*dataBufLen));
    //构建完成无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    //参数配置
    ERROR_SUB errorSub = ERROR_SUB_OK;
    SYS_RESULT_PACK sysResultPack;
    //修正参数
    if(targetCup > PLATE_REACTION_MAX_CUP_INDEX)
    {
        targetCup = PLATE_REACTION_MAX_CUP_INDEX;
    }
    //获取单个杯位偏移
    int32_t oneCupOffset = PLATE_REACTION_ONE_HOLE_OFFSET_STEPS;
    /*计算移动到指定杯位需要的步数 */
    int32_t stepNeedRun = 0;
    //计算坐标
    if(targetCup > currentPlateReactionNewCupHoleIndex)
    {
        stepNeedRun = (targetCup - currentPlateReactionNewCupHoleIndex)*oneCupOffset;
    }
    else if(targetCup < currentPlateReactionNewCupHoleIndex)
    {
        stepNeedRun = (PLATE_REACTION_MAX_CUP_INDEX - currentPlateReactionNewCupHoleIndex + targetCup)*oneCupOffset;
    }
    else
    {
        stepNeedRun = 0;
    }
    //根据步数选择相应的速度曲线
    SM_CURVE_SELECT selectCurve = ModImplBasePlateReactionCalcStepMotorCurve(stepNeedRun);
    //走步数结构体
    IPC_STEP_MOTOR_CMD_RUN_STEP stepMotorRunStepCmd;
    IPC_StepMotorBaseSetRunStepCmdDefault(&stepMotorRunStepCmd);
    stepMotorRunStepCmd.totalSteps = stepNeedRun;//步数
    stepMotorRunStepCmd.runStepCurve = selectCurve;//曲线
    IPC_SrvStepMotorRunStepWhileReturn(PLATE_REACTION_STEP_MOTOR_ROTATE_LOCAL_ID,&stepMotorRunStepCmd,&sysResultPack,NULL);
    //走步数
    if(sysResultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_PLATE_REACTION_IDX_CUP_POSI_ROTATE_RUN_STEPS,
                                        ERROR_LEVEL_ERROR,sysResultPack.errorSub);
        return;
    }
    //更新杯位
    currentPlateReactionNewCupHoleIndex = targetCup;
    //数据填充
    (*dataBufPtrPtr)[0] = (uint8_t)(currentPlateReactionNewCupHoleIndex);
    //等待指定光纤稳定时间
    CoreDelayMsSensitivity(TIME_MS_PLATE_REACTION_WAIT_LIGHT_STABLE);
    //检杯光纤
    PIN_STATE cupSensorState = PIN_STATE_INVALID;
    //读取新杯光纤
    errorSub = ModImplBasePlateReactionReadNewCupSensor(&cupSensorState);
    if(errorSub != ERROR_SUB_OK)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_PLATE_REACTION_IDX_CUP_POSI_SENSOR_NEW_CUP,
                                        ERROR_LEVEL_ERROR,errorSub);
        return;
    }
    //数据填充
    (*dataBufPtrPtr)[1] = (uint8_t)(cupSensorState);
    //读取旧杯光纤
    errorSub = ModImplBasePlateReactionReadOldCupSensor(&cupSensorState);
    if(errorSub != ERROR_SUB_OK)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_PLATE_REACTION_IDX_CUP_POSI_SENSOR_OLD_CUP,
                                        ERROR_LEVEL_ERROR,errorSub);
        return;
    }
    //数据填充
    (*dataBufPtrPtr)[2] = (uint8_t)(cupSensorState);
    //返回完成
    return;
}

//正向运转指定数量杯位
void ModImplBasePlateReactionRunAnyCupsPositive(uint8_t cups,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //返回数据缓存
    *dataBufLen = 3;
    *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_MOD_TASK,(*dataBufLen));
    //构建完成无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    //参数配置
    ERROR_SUB errorSub = ERROR_SUB_OK;
    SYS_RESULT_PACK sysResultPack;
    //修正参数
    if(cups > PLATE_REACTION_MAX_CUP_INDEX)
    {
        cups = PLATE_REACTION_MAX_CUP_INDEX;
    }
    //获取单个杯位偏移
    int32_t oneCupOffset = PLATE_REACTION_ONE_HOLE_OFFSET_STEPS;
    /*计算移动到指定杯位需要的步数 */
    int32_t stepNeedRun = 0;
    stepNeedRun = oneCupOffset*cups;
    //根据步数选择相应的速度曲线
    SM_CURVE_SELECT selectCurve = ModImplBasePlateReactionCalcStepMotorCurve(stepNeedRun);
    //走步数结构体
    IPC_STEP_MOTOR_CMD_RUN_STEP stepMotorRunStepCmd;
    IPC_StepMotorBaseSetRunStepCmdDefault(&stepMotorRunStepCmd);
    stepMotorRunStepCmd.totalSteps = stepNeedRun;//步数
    stepMotorRunStepCmd.runStepCurve = selectCurve;//曲线
    IPC_SrvStepMotorRunStepWhileReturn(PLATE_REACTION_STEP_MOTOR_ROTATE_LOCAL_ID,&stepMotorRunStepCmd,&sysResultPack,NULL);
    //走步数
    if(sysResultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_PLATE_REACTION_ANY_CUP_POSI_ROTATE_RUN_STEPS,
                                        ERROR_LEVEL_ERROR,sysResultPack.errorSub);
        return;
    }
    //更新杯位
    currentPlateReactionNewCupHoleIndex += cups;
    if(currentPlateReactionNewCupHoleIndex > PLATE_REACTION_MAX_CUP_INDEX)
    {
        currentPlateReactionNewCupHoleIndex -= PLATE_REACTION_MAX_CUP_INDEX;
    }
    //数据填充
    (*dataBufPtrPtr)[0] = (uint8_t)(currentPlateReactionNewCupHoleIndex);
    //等待指定光纤稳定时间
    CoreDelayMsSensitivity(TIME_MS_PLATE_REACTION_WAIT_LIGHT_STABLE);
    //检杯光纤
    PIN_STATE cupSensorState = PIN_STATE_INVALID;
    //读取新杯光纤
    errorSub = ModImplBasePlateReactionReadNewCupSensor(&cupSensorState);
    if(errorSub != ERROR_SUB_OK)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_PLATE_REACTION_ANY_CUP_POSI_SENSOR_NEW_CUP,
                                        ERROR_LEVEL_ERROR,errorSub);
        return;
    }
    //数据填充
    (*dataBufPtrPtr)[1] = (uint8_t)(cupSensorState);
    //读取旧杯光纤
    errorSub = ModImplBasePlateReactionReadOldCupSensor(&cupSensorState);
    if(errorSub != ERROR_SUB_OK)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_PLATE_REACTION_ANY_CUP_POSI_SENSOR_OLD_CUP,
                                        ERROR_LEVEL_ERROR,errorSub);
        return;
    }
    //数据填充
    (*dataBufPtrPtr)[2] = (uint8_t)(cupSensorState);
    //返回完成
    return;
}

//反向运转到指定杯位
void ModImplBasePlateReactionRunIndexCupNegative(uint8_t targetCup,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //返回数据缓存
    *dataBufLen = 3;
    *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_MOD_TASK,(*dataBufLen));
    //构建完成无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    //参数配置
    ERROR_SUB errorSub = ERROR_SUB_OK;
    SYS_RESULT_PACK sysResultPack;
    //修正参数
    if(targetCup > PLATE_REACTION_MAX_CUP_INDEX)
    {
        targetCup = PLATE_REACTION_MAX_CUP_INDEX;
    }
    //获取单个杯位偏移
    int32_t oneCupOffset = PLATE_REACTION_ONE_HOLE_OFFSET_STEPS;
    //一整圈的步数,这里有个BUG,就是一格为负数的时候需要做正负号的修正处理,这里我们不管
    //但是严格来说代码是不安全的
    int32_t holeCircleSteps = oneCupOffset*PLATE_REACTION_MAX_CUP_INDEX;
    /*计算移动到指定杯位需要的步数 */
    int32_t stepNeedRun = 0;
    //计算坐标
    if(targetCup > currentPlateReactionNewCupHoleIndex)
    {
        stepNeedRun = (targetCup - currentPlateReactionNewCupHoleIndex)*oneCupOffset;
        //计算出反转需要的步数
        stepNeedRun = holeCircleSteps - stepNeedRun;
    }
    else if(targetCup < currentPlateReactionNewCupHoleIndex)
    {
        stepNeedRun = (PLATE_REACTION_MAX_CUP_INDEX - currentPlateReactionNewCupHoleIndex + targetCup)*oneCupOffset;
        //计算出反转需要的步数
        stepNeedRun = holeCircleSteps - stepNeedRun;
    }
    else
    {
        stepNeedRun = 0;
    }
    //根据步数选择相应的速度曲线
    SM_CURVE_SELECT selectCurve = ModImplBasePlateReactionCalcStepMotorCurve(stepNeedRun);
    //走步数结构体
    IPC_STEP_MOTOR_CMD_RUN_STEP stepMotorRunStepCmd;
    IPC_StepMotorBaseSetRunStepCmdDefault(&stepMotorRunStepCmd);
    stepMotorRunStepCmd.totalSteps = 0 - stepNeedRun;//步数
    stepMotorRunStepCmd.runStepCurve = selectCurve;//曲线
    IPC_SrvStepMotorRunStepWhileReturn(PLATE_REACTION_STEP_MOTOR_ROTATE_LOCAL_ID,&stepMotorRunStepCmd,&sysResultPack,NULL);
    //走步数
    if(sysResultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_PLATE_REACTION_IDX_CUP_NEGA_ROTATE_RUN_STEPS,
                                        ERROR_LEVEL_ERROR,sysResultPack.errorSub);
        return;
    }
    //更新杯位
    currentPlateReactionNewCupHoleIndex = targetCup;
    //数据填充
    (*dataBufPtrPtr)[0] = (uint8_t)(currentPlateReactionNewCupHoleIndex);
    //等待指定光纤稳定时间
    CoreDelayMsSensitivity(TIME_MS_PLATE_REACTION_WAIT_LIGHT_STABLE);
    //检杯光纤
    PIN_STATE cupSensorState = PIN_STATE_INVALID;
    //读取新杯光纤
    errorSub = ModImplBasePlateReactionReadNewCupSensor(&cupSensorState);
    if(errorSub != ERROR_SUB_OK)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_PLATE_REACTION_IDX_CUP_NEGA_SENSOR_NEW_CUP,
                                        ERROR_LEVEL_ERROR,errorSub);
        return;
    }
    //数据填充
    (*dataBufPtrPtr)[1] = (uint8_t)(cupSensorState);
    //读取旧杯光纤
    errorSub = ModImplBasePlateReactionReadOldCupSensor(&cupSensorState);
    if(errorSub != ERROR_SUB_OK)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_PLATE_REACTION_IDX_CUP_NEGA_SENSOR_OLD_CUP,
                                        ERROR_LEVEL_ERROR,errorSub);
        return;
    }
    //数据填充
    (*dataBufPtrPtr)[2] = (uint8_t)(cupSensorState);
    //返回完成
    return;
}

//反向运转指定数量杯位
void ModImplBasePlateReactionRunAnyCupsNegative(uint8_t cups,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //返回数据缓存
    *dataBufLen = 3;
    *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_MOD_TASK,(*dataBufLen));
    //构建完成无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    //参数配置
    ERROR_SUB errorSub = ERROR_SUB_OK;
    SYS_RESULT_PACK sysResultPack;
    //修正参数
    if(cups > PLATE_REACTION_MAX_CUP_INDEX)
    {
        cups = PLATE_REACTION_MAX_CUP_INDEX;
    }
    //获取单个杯位偏移
    int32_t oneCupOffset = PLATE_REACTION_ONE_HOLE_OFFSET_STEPS;
    /*计算移动到指定杯位需要的步数 */
    int32_t stepNeedRun = 0;
    stepNeedRun = oneCupOffset*cups;
    //根据步数选择相应的速度曲线
    SM_CURVE_SELECT selectCurve = ModImplBasePlateReactionCalcStepMotorCurve(stepNeedRun);
    //走步数结构体
    IPC_STEP_MOTOR_CMD_RUN_STEP stepMotorRunStepCmd;
    IPC_StepMotorBaseSetRunStepCmdDefault(&stepMotorRunStepCmd);
    stepMotorRunStepCmd.totalSteps = 0 - stepNeedRun;//步数
    stepMotorRunStepCmd.runStepCurve = selectCurve;//曲线
    IPC_SrvStepMotorRunStepWhileReturn(PLATE_REACTION_STEP_MOTOR_ROTATE_LOCAL_ID,&stepMotorRunStepCmd,&sysResultPack,NULL);
    //走步数
    if(sysResultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_PLATE_REACTION_ANY_CUP_NEGA_ROTATE_RUN_STEPS,
                                        ERROR_LEVEL_ERROR,sysResultPack.errorSub);
        return;
    }
    //更新杯位
    if(currentPlateReactionNewCupHoleIndex > cups)
    {
        currentPlateReactionNewCupHoleIndex -= cups;
    }
    else
    {
        currentPlateReactionNewCupHoleIndex = PLATE_REACTION_MAX_CUP_INDEX + currentPlateReactionNewCupHoleIndex - cups;
    }
    //数据填充
    (*dataBufPtrPtr)[0] = (uint8_t)(currentPlateReactionNewCupHoleIndex);
    //等待指定光纤稳定时间
    CoreDelayMsSensitivity(TIME_MS_PLATE_REACTION_WAIT_LIGHT_STABLE);
    //检杯光纤
    PIN_STATE cupSensorState = PIN_STATE_INVALID;
    //读取新杯光纤
    errorSub = ModImplBasePlateReactionReadNewCupSensor(&cupSensorState);
    if(errorSub != ERROR_SUB_OK)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_PLATE_REACTION_ANY_CUP_NEGA_SENSOR_NEW_CUP,
                                        ERROR_LEVEL_ERROR,errorSub);
        return;
    }
    //数据填充
    (*dataBufPtrPtr)[1] = (uint8_t)(cupSensorState);
    //读取旧杯光纤
    errorSub = ModImplBasePlateReactionReadOldCupSensor(&cupSensorState);
    if(errorSub != ERROR_SUB_OK)
    {
        //构建错误代码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_H360_PLATE_REACTION_ANY_CUP_NEGA_SENSOR_OLD_CUP,
                                        ERROR_LEVEL_ERROR,errorSub);
        return;
    }
    //数据填充
    (*dataBufPtrPtr)[2] = (uint8_t)(cupSensorState);
    //返回完成
    return;
}



















