/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-05-10 10:14:22 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-05-18 16:34:38 +0800
************************************************************************************************/ 
#include "StepMotorMain.h"

//导入初始化单个电机数据集
extern void StepMotorBaseInitDataSetSingle(STEP_MOTOR_NO motorNo);
//导入配置FPGA 电机IP核的必备配置并初始化禁用FPGA
extern ERROR_SUB StepMotorBaseSetConfigSingleDisableMotor(STEP_MOTOR_NO motorNo);
//导入电机按照属性使能函数
ERROR_SUB StepMotorBaseEnableByAttriSingle(STEP_MOTOR_NO motorNo);

//电机库数据初始化
void StepMotorMainLibDataInit(void)
{
    STEP_MOTOR_NO motorIndex = STEP_MOTOR_NO1;
    //电机参数加载初始化
    for(motorIndex = STEP_MOTOR_NO1; motorIndex < (STEP_MOTOR_NO)STEP_MOTOR_NUM_COUNT; motorIndex++)
    {
        StepMotorBaseInitDataSetSingle(motorIndex);
    }
}

//电机库基本配置,并且禁用全部的电机
ERROR_SUB StepMotorMainAllMotorConfigAndDisable(void)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    STEP_MOTOR_NO motorIndex = STEP_MOTOR_NO1;
    //电机参数加载初始化
    for(motorIndex = STEP_MOTOR_NO1; motorIndex < (STEP_MOTOR_NO)STEP_MOTOR_NUM_COUNT; motorIndex++)
    {
        errorSub = StepMotorBaseSetConfigSingleDisableMotor(motorIndex);
        if(errorSub != ERROR_SUB_OK)
        {
            MCU_LogShowErrorSubCode(errorSub);
            //电机配置写入错误
            return errorSub;
        }
    }
    return errorSub;
}

//电机指令急停
ERROR_SUB StepMotorMainEmStopCmd(STEP_MOTOR_NO motorNo)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    STEP_MOTOR_DATA_SET* motorDataSetPtr = StepMotorBaseGetDataSetPtr(motorNo);
    //申请互斥信号量
    errorSub = StepMotorBaseGetLock(motorNo);
    if(errorSub != ERROR_SUB_OK)
    {
        return errorSub;
    }
    //状态修改为急停
    motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
    motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_EM_STOP_CMD;
    //急停自动退出教学模式
    if(motorDataSetPtr->stateSet.teachMode == STEP_MOTOR_MODE_TEACH_ENABLE)
    {
        motorDataSetPtr->stateSet.teachMode = STEP_MOTOR_MODE_TEACH_DISABLE;
    }
    //获取FPGA寄存器状态
    BoardFPGA_StepMotorGetAllReg(motorNo,motorDataSetPtr->stateSet.fpgaRegSetPtr);
    //电机切入空闲
    errorSub = BoardFPGA_StepMotorConvertToIdle(motorNo);
    if(errorSub != ERROR_SUB_OK)
    {
        //释放互斥信号量
        StepMotorBaseReleaseLock(motorNo);
        //错误打印
        MCU_LogShowErrorSubCode(errorSub);
        //电机配置写入错误
        return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_CMD_EM_STOP_FAIL + (ERROR_SUB)(((uint32_t)motorNo)<<8));
    }
    if(motorDataSetPtr->attriPtr->motorEncoderAttri.encoderSet == ENABLE)
    {
        //有编码器的情况下更新坐标,设置电机当前坐标
        motorDataSetPtr->stateSet.currentMotorCoordinate = motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs.encoderCurrentPos;
        //基础坐标等于当前坐标
        motorDataSetPtr->stateSet.baseMotorCoordinate = motorDataSetPtr->stateSet.currentMotorCoordinate;
    }
    else 
    {
        //没有编码器的情况下更新坐标
        if((motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs.motorStateAndStopReason.valBits.motorState == STEP_MOTOR_STATE_CW_FIX_SPD)||
            (motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs.motorStateAndStopReason.valBits.motorState == STEP_MOTOR_STATE_CW_TAB_SPD))
        {
            //正方向
            motorDataSetPtr->stateSet.currentMotorCoordinate = motorDataSetPtr->stateSet.baseMotorCoordinate + motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs.motorPluseSendOutAll;
        }
        else if((motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs.motorStateAndStopReason.valBits.motorState == STEP_MOTOR_STATE_CCW_FIX_SPD)||
            (motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs.motorStateAndStopReason.valBits.motorState == STEP_MOTOR_STATE_CCW_TAB_SPD))
        {
            //负方向
            motorDataSetPtr->stateSet.currentMotorCoordinate = motorDataSetPtr->stateSet.baseMotorCoordinate - motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs.motorPluseSendOutAll;
        }
        motorDataSetPtr->stateSet.baseMotorCoordinate = motorDataSetPtr->stateSet.currentMotorCoordinate;
    }
    //释放互斥信号量
    StepMotorBaseReleaseLock(motorNo);
    return ERROR_SUB_OK;
}

//电机强制进入初始化状态
ERROR_SUB StepMotorMainForceToDefaultState(STEP_MOTOR_NO motorNo)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    STEP_MOTOR_DATA_SET* motorDataSetPtr = StepMotorBaseGetDataSetPtr(motorNo);
    //申请互斥信号量
    errorSub = StepMotorBaseGetLock(motorNo);
    if(errorSub != ERROR_SUB_OK)
    {
        return errorSub;
    }
    motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_IDLE;
    motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_NONE;
    //自动退出教学模式
    if(motorDataSetPtr->stateSet.teachMode == STEP_MOTOR_MODE_TEACH_ENABLE)
    {
        motorDataSetPtr->stateSet.teachMode = STEP_MOTOR_MODE_TEACH_DISABLE;
    }
    //清空坐标
    motorDataSetPtr->stateSet.baseMotorCoordinate = 0;
    motorDataSetPtr->stateSet.currentMotorCoordinate = 0;
    //方向设置为正向
    motorDataSetPtr->stateSet.currentDir = STEP_MOTOR_DIR_CW;
    //清空中断错误代码
    motorDataSetPtr->stateSet.errorSubCodeInt = ERROR_SUB_OK;
    motorDataSetPtr->stateSet.motorStateFpgaInt = STEP_MOTOR_STATE_IDLE;
    motorDataSetPtr->stateSet.motorStopReasonFpgaInt = STEP_MOTOR_STOP_REASON_NONE;
    //不管编码器是否使能,清空编码器序号
    errorSub = BoardFPGA_StepMotorEncoderPosClear(motorNo);
    if(errorSub != ERROR_SUB_OK)
    {
        //释放互斥信号量
        StepMotorBaseReleaseLock(motorNo);
        MCU_LogShowErrorSubCode(errorSub);
        //编码器计数清零错误
        return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_ENCODER_POS_CLEAR_FAIL + (ERROR_SUB)(((uint32_t)motorNo)<<8));
    }
    //加载FPGA寄存器状态
    BoardFPGA_StepMotorGetAllReg(motorNo,(motorDataSetPtr->stateSet.fpgaRegSetPtr));
    //释放互斥信号量
    StepMotorBaseReleaseLock(motorNo);
    return ERROR_SUB_OK;
}

//电机进入编码器学习模式
ERROR_SUB StepMotorMainEnterTeachingMode(STEP_MOTOR_NO motorNo)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    STEP_MOTOR_DATA_SET* motorDataSetPtr = StepMotorBaseGetDataSetPtr(motorNo);
    //申请互斥信号量
    errorSub = StepMotorBaseGetLock(motorNo);
    if(errorSub != ERROR_SUB_OK)
    {
        return errorSub;
    }
    //如果已经在学习模式,直接返回
    if(motorDataSetPtr->stateSet.teachMode == STEP_MOTOR_MODE_TEACH_ENABLE)
    {
        //释放互斥信号量
        StepMotorBaseReleaseLock(motorNo);
        return ERROR_SUB_OK;
    }
    //检查电机属性是否使能,电机不存在,无法进入学习模式
    if(motorDataSetPtr->attriPtr->motorRunAttri.motorExist == DISABLE)
    {
        //释放互斥信号量
        StepMotorBaseReleaseLock(motorNo);
        return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_ENTER_TEACH_MODE_MOTOR_NOT_EXIST + (ERROR_SUB)(((uint32_t)motorNo)<<8));
    }
    //检查编码器是否使能,编码器不使能,无法进入学习模式
    if(motorDataSetPtr->attriPtr->motorEncoderAttri.encoderSet == DISABLE)
    {
        //释放互斥信号量
        StepMotorBaseReleaseLock(motorNo);
        return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_ENTER_TEACH_MODE_ENCODER_NOT_ENABLE + (ERROR_SUB)(((uint32_t)motorNo)<<8));
    }
    //查看电机是否处于空闲状态,只有处于空闲状态才能进入学习模式
    if(motorDataSetPtr->stateSet.mainState != STEP_MOTOR_STATE_MAIN_IDLE)
    {
        //释放互斥信号量
        StepMotorBaseReleaseLock(motorNo);
        return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_ENTER_TEACH_MODE_MOTOR_NOT_IDLE + (ERROR_SUB)(((uint32_t)motorNo)<<8));
    }
    //电机释放
    errorSub = BoardFPGA_StepMotorAndEncoderState(motorNo,DISABLE,ENABLE);
    if(errorSub != ERROR_SUB_OK)
    {
        MCU_LogShowErrorSubCode(errorSub);
        //释放互斥信号量
        StepMotorBaseReleaseLock(motorNo);
        //电机配置写入错误
        return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_CONTROL_MOTOR_ENCODER_SET_FAIL + (ERROR_SUB)(((uint32_t)motorNo)<<8));
    }
    //清空电机中断状态码
    motorDataSetPtr->stateSet.errorSubCodeInt = ERROR_SUB_OK;
    motorDataSetPtr->stateSet.motorStateFpgaInt = STEP_MOTOR_STATE_IDLE;
    motorDataSetPtr->stateSet.motorStopReasonFpgaInt = STEP_MOTOR_STOP_REASON_NONE;
    //条件都满足了,进入学习模式之前加载编码器信号
    BoardFPGA_StepMotorGetAllReg(motorNo,motorDataSetPtr->stateSet.fpgaRegSetPtr);
    //设置电机当前坐标
    motorDataSetPtr->stateSet.currentMotorCoordinate = motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs.encoderCurrentPos;
    //基础坐标等于当前坐标
    motorDataSetPtr->stateSet.baseMotorCoordinate = motorDataSetPtr->stateSet.currentMotorCoordinate;
    //设置进入学习模式
    motorDataSetPtr->stateSet.teachMode = STEP_MOTOR_MODE_TEACH_ENABLE;
    //释放互斥信号量
    StepMotorBaseReleaseLock(motorNo);
    return ERROR_SUB_OK;
}

//电机失能
ERROR_SUB StepMotorMainDisable(STEP_MOTOR_NO motorNo)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    STEP_MOTOR_DATA_SET* motorDataSetPtr = StepMotorBaseGetDataSetPtr(motorNo);
    //申请互斥信号量
    errorSub = StepMotorBaseGetLock(motorNo);
    if(errorSub != ERROR_SUB_OK)
    {
        return errorSub;
    }
    //检查电机属性是否使能,电机不存在,无法控制
    if(motorDataSetPtr->attriPtr->motorRunAttri.motorExist == DISABLE)
    {
        //释放互斥信号量
        StepMotorBaseReleaseLock(motorNo);
        return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_DISABLE_MOTOR_NOT_EXIST + (ERROR_SUB)(((uint32_t)motorNo)<<8));
    }
    //电机处于空闲状态下才能失能
    if(motorDataSetPtr->stateSet.mainState != STEP_MOTOR_STATE_MAIN_IDLE)
    {
        //释放互斥信号量
        StepMotorBaseReleaseLock(motorNo);
        return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_DISABLE_NOT_IDLE + (ERROR_SUB)(((uint32_t)motorNo)<<8));
    }
    //根据编码器状态更新电机坐标
    BoardFPGA_StepMotorGetAllReg(motorNo,motorDataSetPtr->stateSet.fpgaRegSetPtr);
    //检查编码器是否使能
    FunctionalState encoderEnable = motorDataSetPtr->attriPtr->motorEncoderAttri.encoderSet;
    //电机先写入空闲状态
    errorSub = BoardFPGA_StepMotorConvertToIdle(motorNo);
    if(errorSub != ERROR_SUB_OK)
    {
        //释放互斥信号量
        StepMotorBaseReleaseLock(motorNo);
        //错误打印
        MCU_LogShowErrorSubCode(errorSub);
        //电机配置写入错误
        return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_CONTROL_MOTOR_ENCODER_SET_FAIL + (ERROR_SUB)(((uint32_t)motorNo)<<8));
    }
    //电机释放
    errorSub = BoardFPGA_StepMotorAndEncoderState(motorNo,DISABLE,encoderEnable);
    if(errorSub != ERROR_SUB_OK)
    {
        MCU_LogShowErrorSubCode(errorSub);
        //释放互斥信号量
        StepMotorBaseReleaseLock(motorNo);
        //电机配置写入错误
        return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_CONTROL_MOTOR_ENCODER_SET_FAIL + (ERROR_SUB)(((uint32_t)motorNo)<<8));
    }
    //清空电机中断状态码
    motorDataSetPtr->stateSet.errorSubCodeInt = ERROR_SUB_OK;
    motorDataSetPtr->stateSet.motorStateFpgaInt = STEP_MOTOR_STATE_IDLE;
    motorDataSetPtr->stateSet.motorStopReasonFpgaInt = STEP_MOTOR_STOP_REASON_NONE;
    if(encoderEnable == ENABLE)
    {
        //有编码器的情况下更新坐标,设置电机当前坐标
        motorDataSetPtr->stateSet.currentMotorCoordinate = motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs.encoderCurrentPos;
        //基础坐标等于当前坐标
        motorDataSetPtr->stateSet.baseMotorCoordinate = motorDataSetPtr->stateSet.currentMotorCoordinate;
    }
    else
    {
        //没有编码器的情况下更新坐标
        if((motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs.motorStateAndStopReason.valBits.motorState == STEP_MOTOR_STATE_CW_FIX_SPD)||
            (motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs.motorStateAndStopReason.valBits.motorState == STEP_MOTOR_STATE_CW_TAB_SPD))
        {
            //正方向
            motorDataSetPtr->stateSet.currentMotorCoordinate = motorDataSetPtr->stateSet.baseMotorCoordinate + motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs.motorPluseSendOutAll;
        }
        else if((motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs.motorStateAndStopReason.valBits.motorState == STEP_MOTOR_STATE_CCW_FIX_SPD)||
            (motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs.motorStateAndStopReason.valBits.motorState == STEP_MOTOR_STATE_CCW_TAB_SPD))
        {
            //负方向
            motorDataSetPtr->stateSet.currentMotorCoordinate = motorDataSetPtr->stateSet.baseMotorCoordinate - motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs.motorPluseSendOutAll;
        }
        motorDataSetPtr->stateSet.baseMotorCoordinate = motorDataSetPtr->stateSet.currentMotorCoordinate;
    }
    //释放互斥信号量
    StepMotorBaseReleaseLock(motorNo);
    return ERROR_SUB_OK;
}

//读取当前电机编码器值
ERROR_SUB StepMotorReadEncoderPos(STEP_MOTOR_NO motorNo,int32_t* encoderPosPtr)
{
    //检查电机属性是否使能,电机不使能,编码器返回0
    ERROR_SUB errorSub = ERROR_SUB_OK;
    *encoderPosPtr = 0;
    STEP_MOTOR_DATA_SET* motorDataSetPtr = StepMotorBaseGetDataSetPtr(motorNo);
    //申请互斥信号量
    errorSub = StepMotorBaseGetLock(motorNo);
    if(errorSub != ERROR_SUB_OK)
    {
        return errorSub;
    }
    //检查电机属性是否存在,电机不存在,编码器返回0
    if(motorDataSetPtr->attriPtr->motorRunAttri.motorExist == DISABLE)
    {
        *encoderPosPtr = 0;
        //释放互斥信号量
        StepMotorBaseReleaseLock(motorNo);
        return ERROR_SUB_OK;
    }
    //检查编码器属性是否使能,编码器不使能,编码器返回0
    if(motorDataSetPtr->attriPtr->motorEncoderAttri.encoderSet == DISABLE)
    {
        *encoderPosPtr = 0;
        //释放互斥信号量
        StepMotorBaseReleaseLock(motorNo);
        return ERROR_SUB_OK;
    }
    //检查完成,读取电机寄存器
    BoardFPGA_StepMotorGetAllReg(motorNo,motorDataSetPtr->stateSet.fpgaRegSetPtr);
    //返回编码器当前状态
    *encoderPosPtr = motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs.encoderCurrentPos;
    //设置电机当前坐标
    motorDataSetPtr->stateSet.currentMotorCoordinate = *encoderPosPtr;
    //基础坐标等于当前坐标
    motorDataSetPtr->stateSet.baseMotorCoordinate = motorDataSetPtr->stateSet.currentMotorCoordinate;
    //释放互斥信号量
    StepMotorBaseReleaseLock(motorNo);
    return ERROR_SUB_OK;
}

//读取当前电机坐标和编码器坐标
ERROR_SUB StepMotorReadMotorPosAndEncoderPos(STEP_MOTOR_NO motorNo,int32_t* motorPosPtr,int32_t* encoderPosPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    *motorPosPtr = 0;
    *encoderPosPtr = 0;
    STEP_MOTOR_DATA_SET* motorDataSetPtr = StepMotorBaseGetDataSetPtr(motorNo);
    errorSub = StepMotorReadEncoderPos(motorNo,encoderPosPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        return errorSub;
    }
    *motorPosPtr = motorDataSetPtr->stateSet.currentMotorCoordinate;
    return ERROR_SUB_OK;
}

//读取电机传感器状态
ERROR_SUB StepMotorReadSensorState(STEP_MOTOR_NO motorNo,MOTOR_SENSOR_SET* sensorStatePtr)
{
    //检查电机属性是否使能,电机不使能,编码器返回0
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //初始化信号
    sensorStatePtr->resetSensorState = PIN_STATE_INVALID;
    sensorStatePtr->emStopSensorState = PIN_STATE_INVALID;
    sensorStatePtr->utilStopSensorState = PIN_STATE_INVALID;
    //获取电机对象指针
    STEP_MOTOR_DATA_SET* motorDataSetPtr = StepMotorBaseGetDataSetPtr(motorNo);
    //申请互斥信号量
    errorSub = StepMotorBaseGetLock(motorNo);
    if(errorSub != ERROR_SUB_OK)
    {
        return errorSub;
    }
    //检查电机属性是否存在,电机不存在,没有信号返回
    if(motorDataSetPtr->attriPtr->motorRunAttri.motorExist == DISABLE)
    {
        //释放互斥信号量
        StepMotorBaseReleaseLock(motorNo);
        return ERROR_SUB_OK;
    }
    //检查完成,读取电机寄存器
    motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs.sensorStateAndTabIndexCurrent =  BoardFPGA_StepMotorReadRegSensorStateTabIndex(motorNo);
    //记录信号状态,原点
    if(motorDataSetPtr->attriPtr->motorSensorAttri.resetSensorSet == DISABLE)
    {
        sensorStatePtr->resetSensorState = PIN_STATE_INVALID;
    }
    else
    {
        if(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs.sensorStateAndTabIndexCurrent.valBits.resetSensorState == PIN_STATE_VALID)
        {
            sensorStatePtr->resetSensorState = PIN_STATE_VALID;
        }
        else
        {
            sensorStatePtr->resetSensorState = PIN_STATE_INVALID;
        }
    }
    //记录信号状态,急停
    if(motorDataSetPtr->attriPtr->motorSensorAttri.emStopSensorSet == DISABLE)
    {
        sensorStatePtr->emStopSensorState = PIN_STATE_INVALID;
    }
    else
    {
        if(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs.sensorStateAndTabIndexCurrent.valBits.emStopSensorState == PIN_STATE_VALID)
        {
            sensorStatePtr->emStopSensorState = PIN_STATE_VALID;
        }
        else
        {
            sensorStatePtr->emStopSensorState = PIN_STATE_INVALID;
        }
    }
    //记录信号状态,辅助急停
    if(motorDataSetPtr->attriPtr->motorSensorAttri.utilStopSensorSet == DISABLE)
    {
        sensorStatePtr->utilStopSensorState = PIN_STATE_INVALID;
    }
    else
    {
        if(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs.sensorStateAndTabIndexCurrent.valBits.utilStopSensorState == PIN_STATE_VALID)
        {
            sensorStatePtr->utilStopSensorState = PIN_STATE_VALID;
        }
        else
        {
            sensorStatePtr->utilStopSensorState = PIN_STATE_INVALID;
        }
    }
    //释放互斥信号量
    StepMotorBaseReleaseLock(motorNo);
    return ERROR_SUB_OK;
}

ERROR_SUB StepMotorReadMotorRealTimePosAndEncoderPos(STEP_MOTOR_NO motorNo,int32_t* motorPos,int32_t* encoderPosPtr)
{
    return ERROR_SUB_OK;
}

//未知状态下收到电机中断,电机切入指令停止,并进入异常停止状态
static void StepMotorUnknowStateIntProcess(STEP_MOTOR_NO motorNo,STEP_MOTOR_DATA_SET* motorDataSetPtr,STEP_MOTOR_STATE motorState,STEP_MOTOR_STOP_REASON stopReason)
{
    FPGA_STEP_MOTOR_REG_STATE4 motorStateReg;
    //电机切入异常停止状态
    motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
    //未知状态下收到了电机中断
    motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_UNKNOW_STATE_INT;
    //加载电机只读寄存器状态
    BoardFPGA_StepMotorGetReadOnlyReg(motorNo,&(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs));
    //电机切入指令停止,后续由应用程序处理
    motorDataSetPtr->stateSet.errorSubCodeInt = BoardFPGA_StepMotorCmdStop(motorNo,&motorStateReg);
    if(motorDataSetPtr->stateSet.errorSubCodeInt != ERROR_SUB_OK)
    {
        MCU_RTT_SendStringInt("StepMotorUnknowStateIntProcess BoardFPGA_StepMotorCmdStop Error!!\r\n");
    }
    //保存电机中断代码
    motorDataSetPtr->stateSet.motorStateFpgaInt = motorState;
    motorDataSetPtr->stateSet.motorStopReasonFpgaInt = stopReason;
}

//空闲状态下的中断处理,电机切入指令停止,并进入异常停止状态
static void StepMotorIdleStateIntProcess(STEP_MOTOR_NO motorNo,STEP_MOTOR_DATA_SET* motorDataSetPtr,STEP_MOTOR_STATE motorState,STEP_MOTOR_STOP_REASON stopReason)
{
    FPGA_STEP_MOTOR_REG_STATE4 motorStateReg;
    //电机切入异常停止状态
    motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
    //空闲状态下收到了电机中断
    motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_IDLE_STATE_INT;
    //加载电机只读寄存器状态
    BoardFPGA_StepMotorGetReadOnlyReg(motorNo,&(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs));
    //电机切入指令停止,后续由应用程序处理
    motorDataSetPtr->stateSet.errorSubCodeInt = BoardFPGA_StepMotorCmdStop(motorNo,&motorStateReg);
    if(motorDataSetPtr->stateSet.errorSubCodeInt != ERROR_SUB_OK)
    {
        MCU_RTT_SendStringInt("StepMotorIdleStateIntProcess BoardFPGA_StepMotorCmdStop Error!!\r\n");
    }
    //保存电机中断代码
    motorDataSetPtr->stateSet.motorStateFpgaInt = motorState;
    motorDataSetPtr->stateSet.motorStopReasonFpgaInt = stopReason;
}

//停止状态下收到了中断,电机切入指令停止,并进入异常停止状态
static void StepMotorStopStateIntProcess(STEP_MOTOR_NO motorNo,STEP_MOTOR_DATA_SET* motorDataSetPtr,STEP_MOTOR_STATE motorState,STEP_MOTOR_STOP_REASON stopReason)
{
    FPGA_STEP_MOTOR_REG_STATE4 motorStateReg;
    //电机切入异常停止状态
    motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
    //停止状态下收到了电机中断
    motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_STOP_STATE_INT;
    //加载电机只读寄存器状态
    BoardFPGA_StepMotorGetReadOnlyReg(motorNo,&(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs));
    //电机切入指令停止,后续由应用程序处理
    motorDataSetPtr->stateSet.errorSubCodeInt = BoardFPGA_StepMotorCmdStop(motorNo,&motorStateReg);
    if(motorDataSetPtr->stateSet.errorSubCodeInt != ERROR_SUB_OK)
    {
        MCU_RTT_SendStringInt("StepMotorStopStateIntProcess BoardFPGA_StepMotorCmdStop Error!!\r\n");
    }
    //保存电机中断代码
    motorDataSetPtr->stateSet.motorStateFpgaInt = motorState;
    motorDataSetPtr->stateSet.motorStopReasonFpgaInt = stopReason;
}

//外部导入的状态机中断处理函数
//复位状态下的中断处理
extern void StepMotorResetStateIntProcess(STEP_MOTOR_NO motorNo,STEP_MOTOR_DATA_SET* motorDataSetPtr,STEP_MOTOR_STATE motorState,STEP_MOTOR_STOP_REASON stopReason);
//走步数状态下的中断处理
extern void StepMotorRunStepsStateIntProcess(STEP_MOTOR_NO motorNo,STEP_MOTOR_DATA_SET* motorDataSetPtr,STEP_MOTOR_STATE motorState,STEP_MOTOR_STOP_REASON stopReason);
//走坐标状态下的中断处理
extern void StepMotorRunCoordinateStateIntProcess(STEP_MOTOR_NO motorNo,STEP_MOTOR_DATA_SET* motorDataSetPtr,STEP_MOTOR_STATE motorState,STEP_MOTOR_STOP_REASON stopReason);
//持续运转状态下的中断处理
extern void StepMotorRunAlwaysStateIntProcess(STEP_MOTOR_NO motorNo,STEP_MOTOR_DATA_SET* motorDataSetPtr,STEP_MOTOR_STATE motorState,STEP_MOTOR_STOP_REASON stopReason);
//回零状态下的中断处理
extern void StepMotorReturnZeroIntProcess(STEP_MOTOR_NO motorNo,STEP_MOTOR_DATA_SET* motorDataSetPtr,STEP_MOTOR_STATE motorState,STEP_MOTOR_STOP_REASON stopReason);


//电机中断处理函数
void StepMotorMainIntCallBack(STEP_MOTOR_NO motorNo,STEP_MOTOR_STATE motorState,STEP_MOTOR_STOP_REASON stopReason)
{
    //获取电机属性
    STEP_MOTOR_DATA_SET* motorDataSetPtr = StepMotorBaseGetDataSetPtr(motorNo);
    //获取电机当前状态,根据不同的状态执行不同的状态机
    switch(motorDataSetPtr->stateSet.mainState)
    {
        case STEP_MOTOR_STATE_MAIN_IDLE:
            //空闲状态下的中断处理
            StepMotorIdleStateIntProcess(motorNo,motorDataSetPtr,motorState,stopReason);
            break;
        case STEP_MOTOR_STATE_MAIN_STOP:
            //停止状态下的中断处理
            StepMotorStopStateIntProcess(motorNo,motorDataSetPtr,motorState,stopReason);
            break;
        case STEP_MOTOR_STATE_MAIN_RESET:
            //复位状态下的中断处理
            StepMotorResetStateIntProcess(motorNo,motorDataSetPtr,motorState,stopReason);
            break;
        case STEP_MOTOR_STATE_MAIN_RUN_STEPS:
            //走步数状态下的中断处理
            StepMotorRunStepsStateIntProcess(motorNo,motorDataSetPtr,motorState,stopReason);
            break;
        case STEP_MOTOR_STATE_MAIN_RUN_COORDINATE:
            //走坐标状态下的中断处理
            StepMotorRunCoordinateStateIntProcess(motorNo,motorDataSetPtr,motorState,stopReason);
            break;
        case STEP_MOTOR_STATE_MAIN_RUN_ALWAYS:
            //持续运转状态下的中断处理
            StepMotorRunAlwaysStateIntProcess(motorNo,motorDataSetPtr,motorState,stopReason);
            break;
        case STEP_MOTOR_STATE_MAIN_RETURN_ZERO:
            //回零状态下的中断处理
            StepMotorReturnZeroIntProcess(motorNo,motorDataSetPtr,motorState,stopReason);
            break;
        default:
            //未知状态下的中断处理
            StepMotorUnknowStateIntProcess(motorNo,motorDataSetPtr,motorState,stopReason);
            break;
    }
    //RTT输出异常日志
    StepMotorRTT_ShowIntMsgMotorState(motorNo,motorState,stopReason);
}

//根据电机配置使能电机并且开启电机中断
ERROR_SUB StepMotorMainEnableByAttri(void)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    STEP_MOTOR_NO motorIndex = STEP_MOTOR_NO1;
    //电机参数加载初始化
    for(motorIndex = STEP_MOTOR_NO1; motorIndex < (STEP_MOTOR_NO)STEP_MOTOR_NUM_COUNT; motorIndex++)
    {
        errorSub = StepMotorBaseEnableByAttriSingle(motorIndex);
        if(errorSub != ERROR_SUB_OK)
        {
            MCU_LogShowErrorSubCode(errorSub);
            //电机配置写入错误
            return errorSub;
        }
    }
    //开启电机中断
    BoardFPGA_StepMotorIntConfig(StepMotorMainIntCallBack);
    return errorSub;
}

