/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2021-03-31 11:17:14
**LastEditors: DengXiaoJun
**LastEditTime: 2021-06-09 16:01:56
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "StepMotor.h"
#include "StepMotorUtil.h"
#include "StepMotorTable.h"

/**************************************电机走坐标函数申明*****************************************/
static void StepMotorRunCoordinateIntProcessSubNormal(STEP_MOTOR_VAR* stepMotorVarPtr);
static void StepMotorRunCoordinateIntProcessSubCompensate(STEP_MOTOR_VAR* stepMotorVarPtr);
static void StepMotorRunCoordinateIntWaitProcessSubCompensate(STEP_MOTOR_VAR* stepMotorVarPtr);
/***********************************************************************************************/
//走步数编码器信号检测
void StepMotorRunCoordinateIntEncoderProcess(STEP_MOTOR_VAR* stepMotorVarPtr)
{
    //检查是否配置了电机准备OK点
    if(stepMotorVarPtr->attriPtr->baseConfig.readyOkPinIndex < BOARD_PORT_IN_COUNT)
    {
        //读取电机准备OK状态
        GPIO_PinState readyOkSensorValue = BoardPortInReadState(stepMotorVarPtr->attriPtr->baseConfig.readyOkPinIndex);
        //伺服电机没有准备好
        if(readyOkSensorValue != stepMotorVarPtr->attriPtr->baseConfig.readyOkPinValidLevel)
        {
            //发生故障
            stepMotorVarPtr->var.motorMainState = STEP_MOTOR_MAIN_STOP;
            stepMotorVarPtr->var.motorSubState =  STEP_MOTOR_STOP_SUB_SLAVE_READY_FAULT;  
        }
    }
    //最大的电机位置和编码器位置差异
    int32_t encoderLossLimit = (int32_t)(stepMotorVarPtr->attriPtr->baseConfig.encoderLossLimit);
    //编码器信号识别判定
    if(stepMotorVarPtr->attriPtr->callBackFuncConfig.encoderCountGet != NULL)
    {
        int32_t coordinateDifference = stepMotorVarPtr->var.currentCoordinate - stepMotorVarPtr->var.currentEncoderCoordinate;
        //计算绝对值
        if(coordinateDifference < 0)
        {
            coordinateDifference = 0 - coordinateDifference;
        }
        //检测电机是否堵住
        if(coordinateDifference > encoderLossLimit)
        {
            //编码器的信号丢失数量超过了电机配置的最大丢步限制
            stepMotorVarPtr->var.motorMainState = STEP_MOTOR_MAIN_STOP;
            stepMotorVarPtr->var.motorSubState =  STEP_MOTOR_STOP_SUB_RUN_COORDINATE_BLOCK;
        }
    }
}

//走坐标中断处理子状态机
void StepMotorRunCoordinateIntProcess(STEP_MOTOR_VAR* stepMotorVarPtr)
{
    //编码器状态处理
    StepMotorRunCoordinateIntEncoderProcess(stepMotorVarPtr);
    //查看状态,如果状态为停止,切换锁止电流并停止
    if(stepMotorVarPtr->var.motorMainState == STEP_MOTOR_MAIN_STOP)
    {
        //电机停止
        stepMotorVarPtr->attriPtr->callBackFuncConfig.stepMotorStop();
        return;
    }
    switch(stepMotorVarPtr->var.motorSubState)
    {
        case STEP_MOTOR_RUN_COORDINATE_SUB_NORMAL:
            //步进电机正常走步数
            StepMotorRunCoordinateIntProcessSubNormal(stepMotorVarPtr);
            break;
        case STEP_MOTOR_RUN_COORDINATE_SUB_COMPENSATE:
            //补偿
            StepMotorRunCoordinateIntProcessSubCompensate(stepMotorVarPtr);
            break;
        case STEP_MOTOR_RUN_COORDINATE_SUB_WAIT:
            //补偿
            StepMotorRunCoordinateIntWaitProcessSubCompensate(stepMotorVarPtr);
            break;            
        default:
            //不识别的状态,直接停掉
            stepMotorVarPtr->var.motorMainState = STEP_MOTOR_MAIN_STOP;
            stepMotorVarPtr->var.motorSubState =  STEP_MOTOR_STOP_SUB_UNKNOW;
            break;
    }
    //查看状态,如果状态为停止,切换锁止电流并停止
    if(stepMotorVarPtr->var.motorMainState == STEP_MOTOR_MAIN_STOP)
    {
        //电机停止
        stepMotorVarPtr->attriPtr->callBackFuncConfig.stepMotorStop();
    }
}


/********************************************************步进电机正常走坐标的状态机实现*********************************************/
static void StepMotorRunCoordinateIntProcessSubNormal(STEP_MOTOR_VAR* stepMotorVarPtr)
{
    uint32_t currentDataIndex = stepMotorVarPtr->var.currentTableIndex;
    //检查步数表中是否还有未发送的数据
    if(currentDataIndex >= stepMotorVarPtr->var.currentTableMaxLength)
    {
        //当前坐标已经到达最大位置,全部的数据都发送完成了
        stepMotorVarPtr->var.motorMainState = STEP_MOTOR_MAIN_STOP;
        stepMotorVarPtr->var.motorSubState =  STEP_MOTOR_STOP_SUB_NORMAL;
    }
    else
    {
        //如果发送之前该位置已经等于0，则步进电机不发送脉冲，状态停止
        if(stepMotorVarPtr->var.stepMotorStepTableDirect[currentDataIndex] == 0)
        {
            stepMotorVarPtr->var.motorMainState = STEP_MOTOR_MAIN_STOP;
            stepMotorVarPtr->var.motorSubState =  STEP_MOTOR_STOP_SUB_NORMAL;
            return;
        }
        //发出脉冲
        stepMotorVarPtr->attriPtr->callBackFuncConfig.stepMotorStart(stepMotorVarPtr->var.stepMotorSpeedTableDirect[currentDataIndex]);
        //内部的步数表存储的步数减少
        stepMotorVarPtr->var.stepMotorStepTableDirect[currentDataIndex] -= 1;
        //当前步数数据发送完成
        if(stepMotorVarPtr->var.stepMotorStepTableDirect[currentDataIndex] == 0)
        {
            //定位往前加1
            stepMotorVarPtr->var.currentTableIndex += 1;
        }
    }
}
/********************************************************步进电机正常补偿走坐标的状态机实现*********************************************/
static void StepMotorRunCoordinateIntProcessSubCompensate(STEP_MOTOR_VAR* stepMotorVarPtr)
{
    uint32_t currentDataIndex = stepMotorVarPtr->var.currentTableIndex;
    //当前坐标不变，脉冲增加
    if(stepMotorVarPtr->var.currentDir == stepMotorVarPtr->attriPtr->baseConfig.dir)
    {
        stepMotorVarPtr->var.currentCoordinate -= 1;
    }
    else
    {
        stepMotorVarPtr->var.currentCoordinate += 1;
    }
    //发出脉冲
    stepMotorVarPtr->attriPtr->callBackFuncConfig.stepMotorStart(stepMotorVarPtr->var.stepMotorSpeedTableDirect[currentDataIndex]);
}

/********************************************************步进电机过步补偿走坐标的状态机实现*********************************************/
static void StepMotorRunCoordinateIntWaitProcessSubCompensate(STEP_MOTOR_VAR* stepMotorVarPtr)
{
    uint32_t currentDataIndex = stepMotorVarPtr->var.currentTableIndex;
    //电机坐标加2，脉冲加1
    if(stepMotorVarPtr->var.currentDir == stepMotorVarPtr->attriPtr->baseConfig.dir)
    {
        stepMotorVarPtr->var.currentCoordinate += 1;
    }
    else
    {
        stepMotorVarPtr->var.currentCoordinate -= 1;
    }
    //发出脉冲
    stepMotorVarPtr->attriPtr->callBackFuncConfig.stepMotorStart(stepMotorVarPtr->var.stepMotorSpeedTableDirect[currentDataIndex]);
}
/*********************************************************************************************************************************/
//未设置修正补偿次数的情况下的电机运行完成编码器检测
ERROR_SUB StepMotorRun2SpecialCoordinatesCorrectZeroCheck(STEP_MOTOR_NO motorNo,STEP_MOTOR_VAR* stepMotorVarPtr,int32_t exceptTargetCoordinate)
{
    int32_t correctPassLimit = (int32_t)(stepMotorVarPtr->attriPtr->baseConfig.encoderPassLimit);
    //期望坐标和编码器坐标误差
    int32_t coordinateDifference = exceptTargetCoordinate - stepMotorVarPtr->var.currentEncoderCoordinate;
    if(coordinateDifference < 0)
    {
        coordinateDifference = 0 - coordinateDifference;
    }
    //将当前坐标设置为编码器坐标
    stepMotorVarPtr->var.currentCoordinate = stepMotorVarPtr->var.currentEncoderCoordinate;
    if(coordinateDifference <= correctPassLimit)
    {
        //误差容限在允许范围内
        return ERROR_SUB_OK;
    }
    else
    {
        //误差不在允许范围内,报错,修正次数为0且误差异常
        return StepMotorRunEncoderCorrectZeroPassLimitError(motorNo);
    }
}

//执行编码器修正
ERROR_SUB StepMotorRun2SpecialCoordinatesCorrectWithEncoder(StepMotorRun2CoordationCmd* runCoordinatesConfigPtr,STEP_MOTOR_VAR* stepMotorVarPtr,int32_t exceptTargetCoordinate)
{
    ERROR_SUB errorCode = ERROR_SUB_OK;
    //打开了矫正功能,开始进行循环矫正
    int32_t correctPassLimit = (int32_t)(stepMotorVarPtr->attriPtr->baseConfig.encoderPassLimit);
    //修正计数器  
    uint32_t correctCount = 0;
    //期望坐标和编码器坐标误差
    int32_t coordinateDifference = 0;
    while(correctCount < stepMotorVarPtr->attriPtr->baseConfig.encoderCorrectRetryMax)
    {
        //将当前坐标设置为编码器坐标
        stepMotorVarPtr->var.currentCoordinate = stepMotorVarPtr->var.currentEncoderCoordinate;
        //计算误差
        coordinateDifference = exceptTargetCoordinate - stepMotorVarPtr->var.currentEncoderCoordinate;
        if(coordinateDifference < 0)
        {
            coordinateDifference = 0 - coordinateDifference;
        }
        //如果编码器误差和实际目标误差小于可以允许的误差,就不用修正
        if(coordinateDifference <= correctPassLimit)
        {
            //误差容限在允许范围内,跳出循环
            return ERROR_SUB_OK;
        }
        else
        {
            //误差不在允许范围内,需要进行修正,先把当前坐标设置为编码器坐标
            //设置下一步的修正步数
            runCoordinatesConfigPtr->targetCoordinate = exceptTargetCoordinate;
        }
        //编码器误差和实际目标误差小于50，用曲线1修正
        if(coordinateDifference < 50)
        {
            runCoordinatesConfigPtr->selectConfig = 1;    
        }
        //开始计算步进电机运行曲线
        StepMotorTableCalcWithCoordinate(runCoordinatesConfigPtr->targetCoordinate,runCoordinatesConfigPtr->motorNo,runCoordinatesConfigPtr->selectConfig,
                                            runCoordinatesConfigPtr->speedRatioMode,runCoordinatesConfigPtr->speedRatio);
        //设置走位状态
        stepMotorVarPtr->var.motorMainState = STEP_MOTOR_MAIN_RUN_COORDINATE;
        stepMotorVarPtr->var.motorSubState = STEP_MOTOR_RUN_COORDINATE_SUB_NORMAL;
        //计算电机方向
        StepMotorUtilCalcDirWithTargetCoordinate(stepMotorVarPtr,runCoordinatesConfigPtr->targetCoordinate);
        //发出正式的走位脉冲
        StepMotorUtilSendStartPluse(stepMotorVarPtr);
        //等待电机停止运行
        StepMotorUtilWaitMotorStop(stepMotorVarPtr,runCoordinatesConfigPtr->timeOutSet);
        errorCode = StepMotorConvert2ErrorSubCode(runCoordinatesConfigPtr->motorNo,stepMotorVarPtr->var.motorSubState);
        //查看是否出现错误,如果出现错误,无论是否安装编码器均停止
        if(errorCode != ERROR_SUB_OK)
        {
            return errorCode;
        }
        //修正计数器增加
        correctCount++;
    }
    //修正完成,查看修正结果,更新编码器坐标
    stepMotorVarPtr->var.currentCoordinate = stepMotorVarPtr->var.currentEncoderCoordinate;
    //计算误差
    coordinateDifference = exceptTargetCoordinate - stepMotorVarPtr->var.currentEncoderCoordinate;
    if(coordinateDifference < 0)
    {
        coordinateDifference = 0 - coordinateDifference;
    }
    //检查修正结果
    if(coordinateDifference < correctPassLimit)
    {
        //误差容限在允许范围内,修正完成
        return ERROR_SUB_OK;
    }
    else
    {
        //误差不在允许范围内,修正失败
        return StepMotorEncoderCorrectMaxError(runCoordinatesConfigPtr->motorNo);
    }
}

//指定步进电机运行到指定位置
ERROR_SUB StepMotorRun2SpecialCoordinates(StepMotorRun2CoordationCmd* runCoordinatesConfigPtr)
{
    ERROR_SUB errorCode = ERROR_SUB_OK;
    STEP_MOTOR_NO motorNo = runCoordinatesConfigPtr->motorNo;
    STEP_MOTOR_VAR* stepMotorVarPtr = &(stepMotorVarArray[motorNo]);
    //如果存在编码器.通过编码器更新电机坐标
    if(stepMotorVarPtr->attriPtr->callBackFuncConfig.encoderCountGet != NULL)
    {
        StepMotorGetCurrentEncoderPos(motorNo,&(stepMotorVarPtr->var.currentEncoderCoordinate));
        stepMotorVarPtr->var.currentCoordinate = stepMotorVarPtr->var.currentEncoderCoordinate;
    }
    //计算期望运行完成后的目标坐标,正常情况下运行完成编码器会到这个位置
    int32_t exceptTargetCoordinate = runCoordinatesConfigPtr->targetCoordinate;
    stepMotorVarPtr->var.targetCoordinate = exceptTargetCoordinate;
    //检查步进电机状态是否是指令急停,指令急停状态只能复位不能运动
    errorCode = StepMotorUtilCheckEmergencyCmdState(runCoordinatesConfigPtr->motorNo,stepMotorVarPtr);
    if(errorCode != ERROR_SUB_OK)return errorCode;
    //如果目标坐标和实际坐标相等,直接停止
    if(runCoordinatesConfigPtr->targetCoordinate == stepMotorVarPtr->var.currentCoordinate)
    {
        //电机停下来,等待状态切换
        stepMotorVarPtr->var.motorMainState = STEP_MOTOR_MAIN_STOP;
        stepMotorVarPtr->var.motorSubState = STEP_MOTOR_STOP_SUB_NORMAL;
        return ERROR_SUB_OK;
    }
    //检查步进电目标坐标是否在限制坐标范围内
    errorCode = StepMotorUtilCheckTargetCoordinateLimit(runCoordinatesConfigPtr->motorNo,stepMotorVarPtr,runCoordinatesConfigPtr->targetCoordinate);
    if(errorCode != ERROR_SUB_OK)return errorCode;
    //相关状态变量初始化到准备走坐标
    StepMotorStateInitForRunCoordinate(&(stepMotorVarPtr->var));
    //检测急停引脚状态和初始化急停相关状态
    errorCode = StepMotorUtilCheckAndSetEmergencyPinState(motorNo,stepMotorVarPtr);
    if(errorCode != ERROR_SUB_OK)return errorCode;
    //检测辅助急停1引脚状态和初始化相关变量标志
    errorCode = StepMotorUtilCheckAndSetUtilStop1PinState(motorNo,stepMotorVarPtr,runCoordinatesConfigPtr->utilStop1Enable);
    if(errorCode != ERROR_SUB_OK)return errorCode;
    //检测辅助急停2引脚状态和初始化相关变量标志
    errorCode = StepMotorUtilCheckAndSetUtilStop2PinState(motorNo,stepMotorVarPtr,runCoordinatesConfigPtr->utilStop2Enable);
    if(errorCode != ERROR_SUB_OK)return errorCode;
    //开始计算步进电机运行曲线
    StepMotorTableCalcWithCoordinate(runCoordinatesConfigPtr->targetCoordinate,motorNo,runCoordinatesConfigPtr->selectConfig,
                                        runCoordinatesConfigPtr->speedRatioMode,runCoordinatesConfigPtr->speedRatio
                                        );
    //记录下启动运行时间,后续会通过这个时间进行电机运行时间的检测
    stepMotorVarPtr->var.stepMotorStartRunningTimeMs = OS_TICK_TO_MS(tx_time_get());
    //设置走位状态
    stepMotorVarPtr->var.motorMainState = STEP_MOTOR_MAIN_RUN_COORDINATE;
    stepMotorVarPtr->var.motorSubState = STEP_MOTOR_RUN_COORDINATE_SUB_NORMAL;
    //计算电机方向
    StepMotorUtilCalcDirWithTargetCoordinate(stepMotorVarPtr,runCoordinatesConfigPtr->targetCoordinate);
    //发出正式的走位脉冲
    StepMotorUtilSendStartPluse(stepMotorVarPtr);
    //等待电机停止运行
    StepMotorUtilWaitMotorStop(stepMotorVarPtr,runCoordinatesConfigPtr->timeOutSet);
    //如果存在编码器.通过编码器更新电机坐标
    if(stepMotorVarPtr->attriPtr->callBackFuncConfig.encoderCountGet != NULL)
    {
        StepMotorGetCurrentEncoderPos(motorNo,&(stepMotorVarPtr->var.currentEncoderCoordinate));
        stepMotorVarPtr->var.currentCoordinate = stepMotorVarPtr->var.currentEncoderCoordinate;
    }
    //将数据转换为结果代码之后发送出去
    errorCode = StepMotorConvert2ErrorSubCode(runCoordinatesConfigPtr->motorNo,stepMotorVarPtr->var.motorSubState);
    //查看是否出现错误,如果出现错误,无论是否安装编码器均停止
    if(errorCode != ERROR_SUB_OK)
    {
        //要根据是否打开了相关的中断和辅助中断来关闭响应的辅助中断和中断
        StepMotorUtilUnregisterIntPort(stepMotorVarPtr);
        return errorCode;
    }
    //如果编码器不存在,这里就运行完成了
    if(stepMotorVarPtr->attriPtr->callBackFuncConfig.encoderCountGet == NULL)
    {
        //要根据是否打开了相关的中断和辅助中断来关闭响应的辅助中断和中断
        StepMotorUtilUnregisterIntPort(stepMotorVarPtr);
        //这里直接返回正常,因为如果存在异常,前面就已经返回了,运行不到这里来
        return ERROR_SUB_OK;
    }
    //到这里标识存在编码器,首先看有没有开启矫正功能
    if(stepMotorVarPtr->attriPtr->baseConfig.encoderCorrectRetryMax == 0)
    {
        //没有开启校正功能的情况下,直接检测当前坐标和目标坐标是否吻合,如果超过一定限制,会报错,
        //这里需要注意，在编码器坐标到目标位置停止之后，直接进入检测编码器的坐标会过冲一部分，大概1s后检测编码器坐标过冲现象会消失，判断是机械传动的误差导致的，因为运行的通量要求，加延迟会导致运行时间过长，达不到要求，所以不去检测
        errorCode = StepMotorRun2SpecialCoordinatesCorrectZeroCheck(runCoordinatesConfigPtr->motorNo,stepMotorVarPtr,exceptTargetCoordinate);
        //要根据是否打开了相关的中断和辅助中断来关闭响应的辅助中断和中断
        StepMotorUtilUnregisterIntPort(stepMotorVarPtr);
        //将当前坐标设置为编码器坐标
        stepMotorVarPtr->var.currentCoordinate = stepMotorVarPtr->var.currentEncoderCoordinate;
        return errorCode;
    }
    //打开了矫正功能,开始进行循环矫正
    errorCode = StepMotorRun2SpecialCoordinatesCorrectWithEncoder(runCoordinatesConfigPtr,stepMotorVarPtr,exceptTargetCoordinate);
    //要根据是否打开了相关的中断和辅助中断来关闭响应的辅助中断和中断
    StepMotorUtilUnregisterIntPort(stepMotorVarPtr);
    //更新编码器坐标
    StepMotorGetCurrentEncoderPos(motorNo,&(stepMotorVarPtr->var.currentEncoderCoordinate));
    stepMotorVarPtr->var.currentCoordinate = stepMotorVarPtr->var.currentEncoderCoordinate;
    //回传结果
    return errorCode;
}
