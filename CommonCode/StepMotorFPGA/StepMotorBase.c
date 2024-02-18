/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-05-10 10:13:33 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-05-11 08:52:11 +0800
************************************************************************************************/ 
#include "StepMotorBase.h"
#include "tx_mutex.h"
#include "tx_event_flags.h"

//外部导入电机配置
#if(STEP_MOTOR_NUM_COUNT > 0)
extern const STEP_MOTOR_ATTRI stepMotor1Attri;
#endif
#if(STEP_MOTOR_NUM_COUNT > 1)
extern const STEP_MOTOR_ATTRI stepMotor2Attri;
#endif
#if(STEP_MOTOR_NUM_COUNT > 2)
extern const STEP_MOTOR_ATTRI stepMotor3Attri;
#endif
#if(STEP_MOTOR_NUM_COUNT > 3)
extern const STEP_MOTOR_ATTRI stepMotor4Attri;
#endif
#if(STEP_MOTOR_NUM_COUNT > 4)
extern const STEP_MOTOR_ATTRI stepMotor5Attri;
#endif
#if(STEP_MOTOR_NUM_COUNT > 5)
extern const STEP_MOTOR_ATTRI stepMotor6Attri;
#endif
#if(STEP_MOTOR_NUM_COUNT > 6)
extern const STEP_MOTOR_ATTRI stepMotor7Attri;
#endif
#if(STEP_MOTOR_NUM_COUNT > 7)
extern const STEP_MOTOR_ATTRI stepMotor8Attri;
#endif
#if(STEP_MOTOR_NUM_COUNT > 8)
extern const STEP_MOTOR_ATTRI stepMotor9Attri;
#endif
#if(STEP_MOTOR_NUM_COUNT > 9)
extern const STEP_MOTOR_ATTRI stepMotor10Attri;
#endif
#if(STEP_MOTOR_NUM_COUNT > 10)
extern const STEP_MOTOR_ATTRI stepMotor11Attri;
#endif
#if(STEP_MOTOR_NUM_COUNT > 11)
extern const STEP_MOTOR_ATTRI stepMotor12Attri;
#endif
#if(STEP_MOTOR_NUM_COUNT > 12)
extern const STEP_MOTOR_ATTRI stepMotor13Attri;
#endif
#if(STEP_MOTOR_NUM_COUNT > 13)
extern const STEP_MOTOR_ATTRI stepMotor14Attri;
#endif
#if(STEP_MOTOR_NUM_COUNT > 14)
extern const STEP_MOTOR_ATTRI stepMotor15Attri;
#endif
#if(STEP_MOTOR_NUM_COUNT > 15)
extern const STEP_MOTOR_ATTRI stepMotor16Attri;
#endif

//构建配置信息数组,方便获取和初始化
static STEP_MOTOR_ATTRI* stepMotorAttriPtrArray[STEP_MOTOR_NUM_COUNT] = {
#if(STEP_MOTOR_NUM_COUNT > 0)
    (STEP_MOTOR_ATTRI*)&stepMotor1Attri,
#endif
#if(STEP_MOTOR_NUM_COUNT > 1)
    (STEP_MOTOR_ATTRI*)&stepMotor2Attri,
#endif
#if(STEP_MOTOR_NUM_COUNT > 2)
    (STEP_MOTOR_ATTRI*)&stepMotor3Attri,
#endif
#if(STEP_MOTOR_NUM_COUNT > 3)
    (STEP_MOTOR_ATTRI*)&stepMotor4Attri,
#endif
#if(STEP_MOTOR_NUM_COUNT > 4)
    (STEP_MOTOR_ATTRI*)&stepMotor5Attri,
#endif
#if(STEP_MOTOR_NUM_COUNT > 5)
    (STEP_MOTOR_ATTRI*)&stepMotor6Attri,
#endif
#if(STEP_MOTOR_NUM_COUNT > 6)
    (STEP_MOTOR_ATTRI*)&stepMotor7Attri,
#endif
#if(STEP_MOTOR_NUM_COUNT > 7)
    (STEP_MOTOR_ATTRI*)&stepMotor8Attri,
#endif
#if(STEP_MOTOR_NUM_COUNT > 8)
    (STEP_MOTOR_ATTRI*)&stepMotor9Attri,
#endif
#if(STEP_MOTOR_NUM_COUNT > 9)
    (STEP_MOTOR_ATTRI*)&stepMotor10Attri,
#endif
#if(STEP_MOTOR_NUM_COUNT > 10)
    (STEP_MOTOR_ATTRI*)&stepMotor11Attri,
#endif
#if(STEP_MOTOR_NUM_COUNT > 11)
    (STEP_MOTOR_ATTRI*)&stepMotor12Attri,
#endif
#if(STEP_MOTOR_NUM_COUNT > 12)
    (STEP_MOTOR_ATTRI*)&stepMotor13Attri,
#endif
#if(STEP_MOTOR_NUM_COUNT > 13)
    (STEP_MOTOR_ATTRI*)&stepMotor14Attri,
#endif
#if(STEP_MOTOR_NUM_COUNT > 14)
    (STEP_MOTOR_ATTRI*)&stepMotor15Attri,
#endif
#if(STEP_MOTOR_NUM_COUNT > 15)
    (STEP_MOTOR_ATTRI*)&stepMotor16Attri,
#endif
};

//创建电机数据集合
static STEP_MOTOR_DATA_SET stepMotorDataSetArray[STEP_MOTOR_NUM_COUNT];

//获取属性指针
static STEP_MOTOR_ATTRI* StepMotorBaseGetAttriPtr(STEP_MOTOR_NO motorNo)
{
    uint8_t stepMotorIndex = (uint8_t)(motorNo);
    return stepMotorAttriPtrArray[stepMotorIndex];
}

//获取步进电机数据集合
STEP_MOTOR_DATA_SET* StepMotorBaseGetDataSetPtr(STEP_MOTOR_NO motorNo)
{
    uint8_t stepMotorIndex = (uint8_t)(motorNo);
    return &(stepMotorDataSetArray[stepMotorIndex]);
}

//获取步进电机访问信号量
ERROR_SUB StepMotorBaseGetLock(STEP_MOTOR_NO motorNo)
{
    STEP_MOTOR_DATA_SET* motorDataSetPtr = StepMotorBaseGetDataSetPtr(motorNo);
    uint32_t txResult = TX_SUCCESS;
    txResult = tx_mutex_get(&(motorDataSetPtr->stateSet.mutexAccess),MS_TO_OS_TICK(STEP_MOTOR_GET_MUTEX_MAX_TIME_MS));
    if(txResult != TX_SUCCESS)
    {
        MCU_LogPrintf("StepMotorBaseGetLock Failed. txResult: 0X%08X\r\n",txResult);
        return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_MUTEX_GET_FAIL + (ERROR_SUB)(((uint32_t)motorNo)<<8));
    }
    return ERROR_SUB_OK;
}

//释放步进电机访问信号量
void StepMotorBaseReleaseLock(STEP_MOTOR_NO motorNo)
{
    STEP_MOTOR_DATA_SET* motorDataSetPtr = StepMotorBaseGetDataSetPtr(motorNo);
    //检查当前是不是自己的互斥信号量
    if(motorDataSetPtr->stateSet.mutexAccess.tx_mutex_owner == tx_thread_identify())
    {
        tx_mutex_put(&(motorDataSetPtr->stateSet.mutexAccess));
    }
    return;
}   

//初始化指定电机数据集合
void StepMotorBaseInitDataSetSingle(STEP_MOTOR_NO motorNo)
{
    STEP_MOTOR_DATA_SET* motorDataSetPtr = StepMotorBaseGetDataSetPtr(motorNo);
    //首先设置电机属性
    motorDataSetPtr->attriPtr = StepMotorBaseGetAttriPtr(motorNo);
    //检查互斥信号量是否初始化,已经初始化就不要反复初始化了
    if(motorDataSetPtr->stateSet.mutexAccess.tx_mutex_id == TX_MUTEX_ID)
    {
        return;
    }
    //创建互斥信号量
    tx_mutex_create(&(motorDataSetPtr->stateSet.mutexAccess),"StepMotorAccessMutex",TX_INHERIT);
    //设置电机状态,无关是否使能
    motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_IDLE;
    motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_NONE;
    motorDataSetPtr->stateSet.currentDir = STEP_MOTOR_DIR_CW;
    motorDataSetPtr->stateSet.currentMotorCoordinate = 0;
    motorDataSetPtr->stateSet.baseMotorCoordinate = 0;
    motorDataSetPtr->stateSet.startTimeTick = 0;
    motorDataSetPtr->stateSet.runningTimeMsCount = 0;
    //清空中断保存数据
    motorDataSetPtr->stateSet.errorSubCodeInt = ERROR_SUB_OK;
    motorDataSetPtr->stateSet.motorStateFpgaInt = STEP_MOTOR_STATE_IDLE;
    motorDataSetPtr->stateSet.motorStopReasonFpgaInt = STEP_MOTOR_STOP_REASON_NONE;
    //电机不处于学习模式
    motorDataSetPtr->stateSet.teachMode = STEP_MOTOR_MODE_TEACH_DISABLE;
    //检查电机是否使能,使能的情况下创建数据结构
    if(motorDataSetPtr->attriPtr->motorRunAttri.motorExist == ENABLE)
    {
        //主曲线
        motorDataSetPtr->stateSet.mainCurveTabDataPtr = STEP_MOTOR_MEM_MALLOC(DIM_STRUCT_LENGTH_BYTE(STEP_MOTOR_MAIN_CURVE_TAB_DATA));
        //辅助曲线
        motorDataSetPtr->stateSet.utilCurveTabDataPtr = STEP_MOTOR_MEM_MALLOC(DIM_STRUCT_LENGTH_BYTE(STEP_MOTOR_UTIL_CURVE_TAB_DATA));
        //FPGA电机寄存器
        motorDataSetPtr->stateSet.fpgaRegSetPtr = STEP_MOTOR_MEM_MALLOC(DIM_STRUCT_LENGTH_BYTE(FPGA_STEP_MOTOR_REGS));
    }
    else
    {
        //电机不使能
        motorDataSetPtr->stateSet.mainCurveTabDataPtr = NULL;
        //辅助曲线
        motorDataSetPtr->stateSet.utilCurveTabDataPtr = NULL;
        //FPGA电机寄存器
        motorDataSetPtr->stateSet.fpgaRegSetPtr = NULL;
    }
}


//步进电机配置FPGA属性,同时禁用电机
ERROR_SUB StepMotorBaseSetConfigSingleDisableMotor(STEP_MOTOR_NO motorNo)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    FPGA_MOTOR_ATTRI motorAttri;
    STEP_MOTOR_DATA_SET* motorDataSetPtr = StepMotorBaseGetDataSetPtr(motorNo);
    //步进电机全部切入空闲
    errorSub = BoardFPGA_StepMotorConvertToIdle(motorNo);
    if(errorSub != ERROR_SUB_OK)
    {
        MCU_LogShowErrorSubCode(errorSub);
        //电机配置写入错误
        return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_CONVERT_TO_IDLE_FAIL + (ERROR_SUB)(((uint32_t)motorNo)<<8));
    }
    //首先读取全部的步进电机寄存器
    BoardFPGA_StepMotorGetAllReg(motorNo,motorDataSetPtr->stateSet.fpgaRegSetPtr);
    //配置有效电平和编码器系数
    if(motorDataSetPtr->attriPtr->motorEncoderAttri.encoderSet == ENABLE)
    {
        motorAttri.encoderFactor = motorDataSetPtr->attriPtr->motorEncoderAttri.ratioWithMotorPos;
    }
    else
    {
        motorAttri.encoderFactor = 1.0f;
    }
    //设置有效信号
    //原点传感器有效信号
    motorAttri.configReg.valBits.resetSensorValidLevel = motorDataSetPtr->attriPtr->motorSensorAttri.resetSensorValidLevel;
    //急停传感器有效信号
    motorAttri.configReg.valBits.emStopSensorValidLevel = motorDataSetPtr->attriPtr->motorSensorAttri.emStopSensorValidLevel;
    //辅助急停传感器有效信号
    motorAttri.configReg.valBits.utilStopSensorValidLevel = motorDataSetPtr->attriPtr->motorSensorAttri.utilStopSensorValidLevel;
    //电机使能有效信号
    motorAttri.configReg.valBits.motorEnableValidLevel = motorDataSetPtr->attriPtr->motorSensorAttri.motorEnablePinValidLevel;
    //电机正方向信号高低电平
    motorAttri.configReg.valBits.motorDirCwLevel = motorDataSetPtr->attriPtr->motorSensorAttri.motorDirPinCwValidLevel;
    //电机脉冲信号空闲状态
    motorAttri.configReg.valBits.motorPluseIdleLevel = motorDataSetPtr->attriPtr->motorSensorAttri.motorPlusePinIdleLevel;
    //电机编码器A相有效信号
    motorAttri.configReg.valBits.encoderValidLevelA = motorDataSetPtr->attriPtr->motorEncoderAttri.validLevelA;
    //电机编码器B相有效信号
    motorAttri.configReg.valBits.encoderValidLevelB = motorDataSetPtr->attriPtr->motorEncoderAttri.validLevelB;
    //写入配置
    errorSub = BoardFPGA_StepMotorWriteMotorAttri(motorNo,&motorAttri);
    if(errorSub != ERROR_SUB_OK)
    {
        MCU_LogShowErrorSubCode(errorSub);
        //电机配置写入错误
        return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_WRITE_CONFIG_FAIL + (ERROR_SUB)(((uint32_t)motorNo)<<8));
    }
    //清空速度步数表
    errorSub = BoardFPGA_StepMotorClearSpdStpTab(motorNo);
    if(errorSub != ERROR_SUB_OK)
    {
        MCU_LogShowErrorSubCode(errorSub);
        //电机配置写入错误
        return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_CLEAR_SPD_STP_TAB_FAIL + (ERROR_SUB)(((uint32_t)motorNo)<<8));
    }
    //配置阶段失能电机和编码器
    errorSub = BoardFPGA_StepMotorAndEncoderState(motorNo,DISABLE,DISABLE);
    if(errorSub != ERROR_SUB_OK)
    {
        MCU_LogShowErrorSubCode(errorSub);
        //电机配置写入错误
        return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_CONTROL_MOTOR_ENCODER_SET_FAIL + (ERROR_SUB)(((uint32_t)motorNo)<<8));
    }
    //设置电机状态,初始化空闲状态
    motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_IDLE;
    //加载FPGA寄存器状态
    BoardFPGA_StepMotorGetAllReg(motorNo,(motorDataSetPtr->stateSet.fpgaRegSetPtr));
    return ERROR_SUB_OK;
}

//电机按照属性使能
ERROR_SUB StepMotorBaseEnableByAttriSingle(STEP_MOTOR_NO motorNo)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    STEP_MOTOR_DATA_SET* motorDataSetPtr = StepMotorBaseGetDataSetPtr(motorNo);
    //获取电机是否存在和编码器是否使能的属性
    FunctionalState motorEnableSet = motorDataSetPtr->attriPtr->motorRunAttri.motorExist;
    FunctionalState encoderEnableSet = motorDataSetPtr->attriPtr->motorEncoderAttri.encoderSet;
    //设置电机使能或者编码器使能
    errorSub = BoardFPGA_StepMotorAndEncoderState(motorNo,motorEnableSet,encoderEnableSet);
    if(errorSub != ERROR_SUB_OK)
    {
        MCU_LogShowErrorSubCode(errorSub);
        //电机配置写入错误
        return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_CONTROL_MOTOR_ENCODER_SET_FAIL + (ERROR_SUB)(((uint32_t)motorNo)<<8));
    }
    //不管编码器是否使能,清空编码器序号
    errorSub = BoardFPGA_StepMotorEncoderPosClear(motorNo);
    if(errorSub != ERROR_SUB_OK)
    {
        MCU_LogShowErrorSubCode(errorSub);
        //编码器计数清零错误
        return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_ENCODER_POS_CLEAR_FAIL + (ERROR_SUB)(((uint32_t)motorNo)<<8));
    }
    //电机切换到空闲状态
    errorSub = BoardFPGA_StepMotorConvertToIdle(motorNo);
    if(errorSub != ERROR_SUB_OK)
    {
        MCU_LogShowErrorSubCode(errorSub);
        //电机配置写入错误
        return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_CONVERT_TO_IDLE_FAIL + (ERROR_SUB)(((uint32_t)motorNo)<<8));
    }
    //设置状态,状态空闲
    motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_IDLE;
    motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_NONE;
    //坐标为0
    motorDataSetPtr->stateSet.currentMotorCoordinate = 0;
    //目标坐标为0
    motorDataSetPtr->stateSet.baseMotorCoordinate = 0;
    //清空中断保存数据
    motorDataSetPtr->stateSet.errorSubCodeInt = ERROR_SUB_OK;
    motorDataSetPtr->stateSet.motorStateFpgaInt = STEP_MOTOR_STATE_IDLE;
    motorDataSetPtr->stateSet.motorStopReasonFpgaInt = STEP_MOTOR_STOP_REASON_NONE;
    //加载FPGA寄存器状态
    BoardFPGA_StepMotorGetAllReg(motorNo,(motorDataSetPtr->stateSet.fpgaRegSetPtr));
    //如果当前电机需要使能,那么延时等待一下,不能全部电机瞬间同时上电
    if(motorEnableSet == ENABLE)
    {
        CoreDelayMs(STEP_MOTOR_ENABLE_TIME_INTERVAL_MS);
    }
    return ERROR_SUB_OK;
}

//RTT中断显示电机停止讯息
void StepMotorRTT_ShowIntMsgMotorState(STEP_MOTOR_NO motorNo,STEP_MOTOR_STATE motorState,STEP_MOTOR_STOP_REASON stopReason)
{
#if(STEP_MOTOR_ENABLE_INT_MSG_SHOW != CONFIG_FUNC_ENABLE)
    //电机序号
    switch(motorNo)
    {
        case STEP_MOTOR_NO1:
            MCU_RTT_SendStringInt("IntMotorState STEP_MOTOR_NO1 ");
            break;
        case STEP_MOTOR_NO2:
            MCU_RTT_SendStringInt("IntMotorState STEP_MOTOR_NO2 ");
            break;
        case STEP_MOTOR_NO3:
            MCU_RTT_SendStringInt("IntMotorState STEP_MOTOR_NO3 ");
            break;
        case STEP_MOTOR_NO4:
            MCU_RTT_SendStringInt("IntMotorState STEP_MOTOR_NO4 ");
            break;
        case STEP_MOTOR_NO5:
            MCU_RTT_SendStringInt("IntMotorState STEP_MOTOR_NO5 ");
            break;
        case STEP_MOTOR_NO6:
            MCU_RTT_SendStringInt("IntMotorState STEP_MOTOR_NO6 ");
            break;
        case STEP_MOTOR_NO7:
            MCU_RTT_SendStringInt("IntMotorState STEP_MOTOR_NO7 ");
            break;
        case STEP_MOTOR_NO8:
            MCU_RTT_SendStringInt("IntMotorState STEP_MOTOR_NO8 ");
            break;
        case STEP_MOTOR_NO9:
            MCU_RTT_SendStringInt("IntMotorState STEP_MOTOR_NO9 ");
            break;
        case STEP_MOTOR_NO10:
            MCU_RTT_SendStringInt("IntMotorState STEP_MOTOR_NO10 ");
            break;
        case STEP_MOTOR_NO11:
            MCU_RTT_SendStringInt("IntMotorState STEP_MOTOR_NO11 ");
            break;
        case STEP_MOTOR_NO12:
            MCU_RTT_SendStringInt("IntMotorState STEP_MOTOR_NO12 ");
            break;
        case STEP_MOTOR_NO13:
            MCU_RTT_SendStringInt("IntMotorState STEP_MOTOR_NO13 ");
            break;
        case STEP_MOTOR_NO14:
            MCU_RTT_SendStringInt("IntMotorState STEP_MOTOR_NO14 ");
            break;
        case STEP_MOTOR_NO15:
            MCU_RTT_SendStringInt("IntMotorState STEP_MOTOR_NO15 ");
            break;
        case STEP_MOTOR_NO16:
            MCU_RTT_SendStringInt("IntMotorState STEP_MOTOR_NO16 ");
            break;
        default: 
            MCU_RTT_SendStringInt("Unknow Motor No ");
            break;
    }
    //电机状态
    switch(motorState)
    {
        case STEP_MOTOR_STATE_IDLE:
            MCU_RTT_SendStringInt("STEP_MOTOR_STATE_IDLE ");
            break;
        case STEP_MOTOR_STATE_CW_FIX_SPD:
            MCU_RTT_SendStringInt("STEP_MOTOR_STATE_CW_FIX_SPD ");
            break;
        case STEP_MOTOR_STATE_CCW_FIX_SPD:
            MCU_RTT_SendStringInt("STEP_MOTOR_STATE_CCW_FIX_SPD ");
            break;
        case STEP_MOTOR_STATE_CW_TAB_SPD:
            MCU_RTT_SendStringInt("STEP_MOTOR_STATE_CW_TAB_SPD ");
            break;
        case STEP_MOTOR_STATE_CCW_TAB_SPD:
            MCU_RTT_SendStringInt("STEP_MOTOR_STATE_CCW_TAB_SPD ");
            break;
        case STEP_MOTOR_STATE_STOP_CONDITION:
            MCU_RTT_SendStringInt("STEP_MOTOR_STATE_STOP_CONDITION ");
            break;
        case STEP_MOTOR_STATE_STOP_CMD:
            MCU_RTT_SendStringInt("STEP_MOTOR_STATE_STOP_CMD ");
            break;
        default: 
            MCU_RTT_SendStringInt("Unknow MotorState ");
            break;
    }
    //电机停止源
    switch(stopReason)
    {
        case STEP_MOTOR_STOP_REASON_NONE:
            MCU_RTT_SendStringInt("STEP_MOTOR_STOP_REASON_NONE \r\n");
            break;
        case STEP_MOTOR_STOP_REASON_CW_SPD_FIX_PARAM_SPD_ERR:
            MCU_RTT_SendStringInt("STEP_MOTOR_STOP_REASON_CW_SPD_FIX_PARAM_SPD_ERR \r\n");
            break;
        case STEP_MOTOR_STOP_REASON_CW_SPD_FIX_PARAM_STP_ERR:
            MCU_RTT_SendStringInt("STEP_MOTOR_STOP_REASON_CW_SPD_FIX_PARAM_STP_ERR \r\n");
            break;
        case STEP_MOTOR_STOP_REASON_CW_SPD_FIX_CMD:
            MCU_RTT_SendStringInt("STEP_MOTOR_STOP_REASON_CW_SPD_FIX_CMD \r\n");
            break;
        case STEP_MOTOR_STOP_REASON_CW_SPD_FIX_RESET_SENSOR_VALID:
            MCU_RTT_SendStringInt("STEP_MOTOR_STOP_REASON_CW_SPD_FIX_RESET_SENSOR_VALID \r\n");
            break;
        case STEP_MOTOR_STOP_REASON_CW_SPD_FIX_RESET_SENSOR_INVALID:
            MCU_RTT_SendStringInt("STEP_MOTOR_STOP_REASON_CW_SPD_FIX_RESET_SENSOR_INVALID \r\n");
            break;
        case STEP_MOTOR_STOP_REASON_CW_SPD_FIX_EM_STOP_SENSOR_VALID:
            MCU_RTT_SendStringInt("STEP_MOTOR_STOP_REASON_CW_SPD_FIX_EM_STOP_SENSOR_VALID \r\n");
            break;
        case STEP_MOTOR_STOP_REASON_CW_SPD_FIX_UTIL_STOP_SENSOR_VALID:
            MCU_RTT_SendStringInt("STEP_MOTOR_STOP_REASON_CW_SPD_FIX_UTIL_STOP_SENSOR_VALID \r\n");
            break;
        case STEP_MOTOR_STOP_REASON_CW_SPD_FIX_PLUSE_OVER:
            MCU_RTT_SendStringInt("STEP_MOTOR_STOP_REASON_CW_SPD_FIX_PLUSE_OVER \r\n");
            break;
        case STEP_MOTOR_STOP_REASON_CCW_SPD_FIX_PARAM_SPD_ERR:
            MCU_RTT_SendStringInt("STEP_MOTOR_STOP_REASON_CCW_SPD_FIX_PARAM_SPD_ERR ");
            break;
        case STEP_MOTOR_STOP_REASON_CCW_SPD_FIX_PARAM_STP_ERR:
            MCU_RTT_SendStringInt("STEP_MOTOR_STOP_REASON_CCW_SPD_FIX_PARAM_STP_ERR \r\n");
            break;
        case STEP_MOTOR_STOP_REASON_CCW_SPD_FIX_CMD:
            MCU_RTT_SendStringInt("STEP_MOTOR_STOP_REASON_CCW_SPD_FIX_CMD \r\n");
            break;
        case STEP_MOTOR_STOP_REASON_CCW_SPD_FIX_RESET_SENSOR_VALID:
            MCU_RTT_SendStringInt("STEP_MOTOR_STOP_REASON_CCW_SPD_FIX_RESET_SENSOR_VALID \r\n");
            break;
        case STEP_MOTOR_STOP_REASON_CCW_SPD_FIX_RESET_SENSOR_INVALID:
            MCU_RTT_SendStringInt("STEP_MOTOR_STOP_REASON_CCW_SPD_FIX_RESET_SENSOR_INVALID \r\n");
            break;
        case STEP_MOTOR_STOP_REASON_CCW_SPD_FIX_EM_STOP_SENSOR_VALID:
            MCU_RTT_SendStringInt("STEP_MOTOR_STOP_REASON_CCW_SPD_FIX_EM_STOP_SENSOR_VALID \r\n");
            break;
        case STEP_MOTOR_STOP_REASON_CCW_SPD_FIX_UTIL_STOP_SENSOR_VALID:
            MCU_RTT_SendStringInt("STEP_MOTOR_STOP_REASON_CCW_SPD_FIX_UTIL_STOP_SENSOR_VALID \r\n");
            break;
        case STEP_MOTOR_STOP_REASON_CCW_SPD_FIX_PLUSE_OVER:
            MCU_RTT_SendStringInt("STEP_MOTOR_STOP_REASON_CCW_SPD_FIX_PLUSE_OVER \r\n");
            break;
        case STEP_MOTOR_STOP_REASON_CW_SPD_TAB_PARAM_SPD_ERR:
            MCU_RTT_SendStringInt("STEP_MOTOR_STOP_REASON_CW_SPD_TAB_PARAM_SPD_ERR \r\n");
            break;
        case STEP_MOTOR_STOP_REASON_CW_SPD_TAB_PARAM_STP_ERR:
            MCU_RTT_SendStringInt("STEP_MOTOR_STOP_REASON_CW_SPD_TAB_PARAM_STP_ERR \r\n");
            break;
        case STEP_MOTOR_STOP_REASON_CW_SPD_TAB_CMD:
            MCU_RTT_SendStringInt("STEP_MOTOR_STOP_REASON_CW_SPD_TAB_CMD \r\n");
            break;
        case STEP_MOTOR_STOP_REASON_CW_SPD_TAB_RESET_SENSOR_VALID:
            MCU_RTT_SendStringInt("STEP_MOTOR_STOP_REASON_CW_SPD_TAB_RESET_SENSOR_VALID \r\n");
            break;
        case STEP_MOTOR_STOP_REASON_CW_SPD_TAB_RESET_SENSOR_INVALID:
            MCU_RTT_SendStringInt("STEP_MOTOR_STOP_REASON_CW_SPD_TAB_RESET_SENSOR_INVALID \r\n");
            break;
        case STEP_MOTOR_STOP_REASON_CW_SPD_TAB_EM_STOP_SENSOR_VALID:
            MCU_RTT_SendStringInt("STEP_MOTOR_STOP_REASON_CW_SPD_TAB_EM_STOP_SENSOR_VALID \r\n");
            break;
        case STEP_MOTOR_STOP_REASON_CW_SPD_TAB_UTIL_STOP_SENSOR_VALID:
            MCU_RTT_SendStringInt("STEP_MOTOR_STOP_REASON_CW_SPD_TAB_UTIL_STOP_SENSOR_VALID \r\n");
            break;
        case STEP_MOTOR_STOP_REASON_CW_SPD_TAB_PLUSE_OVER:
            MCU_RTT_SendStringInt("STEP_MOTOR_STOP_REASON_CW_SPD_TAB_PLUSE_OVER \r\n");
            break;
        case STEP_MOTOR_STOP_REASON_CW_SPD_TAB_START_INDEX:
            MCU_RTT_SendStringInt("STEP_MOTOR_STOP_REASON_CW_SPD_TAB_START_INDEX \r\n");
            break;
        case STEP_MOTOR_STOP_REASON_CW_SPD_TAB_STOP_INDEX:
            MCU_RTT_SendStringInt("STEP_MOTOR_STOP_REASON_CW_SPD_TAB_STOP_INDEX \r\n");
            break;
        case STEP_MOTOR_STOP_REASON_CW_SPD_TAB_INDEX_RANGE:
            MCU_RTT_SendStringInt("STEP_MOTOR_STOP_REASON_CW_SPD_TAB_INDEX_RANGE \r\n");
            break;
        case STEP_MOTOR_STOP_REASON_CCW_SPD_TAB_PARAM_SPD_ERR:
            MCU_RTT_SendStringInt("STEP_MOTOR_STOP_REASON_CCW_SPD_TAB_PARAM_SPD_ERR \r\n");
            break;
        case STEP_MOTOR_STOP_REASON_CCW_SPD_TAB_PARAM_STP_ERR:
            MCU_RTT_SendStringInt("STEP_MOTOR_STOP_REASON_CCW_SPD_TAB_PARAM_STP_ERR \r\n");
            break;
        case STEP_MOTOR_STOP_REASON_CCW_SPD_TAB_CMD:
            MCU_RTT_SendStringInt("STEP_MOTOR_STOP_REASON_CCW_SPD_TAB_CMD \r\n");
            break;
        case STEP_MOTOR_STOP_REASON_CCW_SPD_TAB_RESET_SENSOR_VALID:
            MCU_RTT_SendStringInt("STEP_MOTOR_STOP_REASON_CCW_SPD_TAB_RESET_SENSOR_VALID \r\n");
            break;
        case STEP_MOTOR_STOP_REASON_CCW_SPD_TAB_RESET_SENSOR_INVALID:
            MCU_RTT_SendStringInt("STEP_MOTOR_STOP_REASON_CCW_SPD_TAB_RESET_SENSOR_INVALID \r\n");
            break;
        case STEP_MOTOR_STOP_REASON_CCW_SPD_TAB_EM_STOP_SENSOR_VALID:
            MCU_RTT_SendStringInt("STEP_MOTOR_STOP_REASON_CCW_SPD_TAB_EM_STOP_SENSOR_VALID \r\n");
            break;
        case STEP_MOTOR_STOP_REASON_CCW_SPD_TAB_UTIL_STOP_SENSOR_VALID:
            MCU_RTT_SendStringInt("STEP_MOTOR_STOP_REASON_CCW_SPD_TAB_UTIL_STOP_SENSOR_VALID \r\n");
            break;
        case STEP_MOTOR_STOP_REASON_CCW_SPD_TAB_PLUSE_OVER:
            MCU_RTT_SendStringInt("STEP_MOTOR_STOP_REASON_CCW_SPD_TAB_PLUSE_OVER \r\n");
            break;
        case STEP_MOTOR_STOP_REASON_CCW_SPD_TAB_START_INDEX:
            MCU_RTT_SendStringInt("STEP_MOTOR_STOP_REASON_CCW_SPD_TAB_START_INDEX \r\n");
            break;
        case STEP_MOTOR_STOP_REASON_CCW_SPD_TAB_STOP_INDEX:
            MCU_RTT_SendStringInt("STEP_MOTOR_STOP_REASON_CCW_SPD_TAB_STOP_INDEX \r\n");
            break;
        case STEP_MOTOR_STOP_REASON_CCW_SPD_TAB_INDEX_RANGE:
            MCU_RTT_SendStringInt("STEP_MOTOR_STOP_REASON_CCW_SPD_TAB_INDEX_RANGE \r\n");
            break;
        default:
            MCU_RTT_SendStringInt("Unknow StopReason \r\n");
            break;
    }
#else
    asm("nop");
#endif
}



























