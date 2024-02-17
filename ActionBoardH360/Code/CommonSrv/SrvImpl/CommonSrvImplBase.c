/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-31 08:43:34 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-12-04 12:57:40 +0800
************************************************************************************************/ 
#include "CommonSrvImplBase.h"

//步进电机复位
ERROR_SUB CommonSrvImplBaseModuleStepMotorReset(uint8_t motorIdx,MODULE_STEP_MOTOR_CONFIG const* configArrayPtr,uint16_t arrayLength,
                                                    int32_t* motorCoordinate)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    *motorCoordinate = 0;
    //序号不支持
    if(motorIdx >= arrayLength)
    {
        return ERROR_SUB_MODULE_STEP_MOTOR_INDEX;
    }
    //电机不支持
    if(configArrayPtr[motorIdx].motorNo == MODULE_BASE_STEP_MOTOR_NO_RESERVE)
    {
        return ERROR_SUB_MODULE_STEP_MOTOR_INDEX;
    }
    int32_t resetCorrectPos = 0;
    //电机复位修正获取
    if((configArrayPtr[motorIdx].resetCorrectMainIndex == MODULE_BASE_STEP_MOTOR_RESET_CORRECT_PAR_MAIN)||
        (configArrayPtr[motorIdx].resetCorrectSubIndex == MODULE_BASE_STEP_MOTOR_RESET_CORRECT_PAR_SUB))
    {
        resetCorrectPos = 0; 
    }
    else
    {
        errorSub = SysParamReadSingle((INDEX_MAIN_SYS_PARAM)configArrayPtr[motorIdx].resetCorrectMainIndex,
                                        configArrayPtr[motorIdx].resetCorrectSubIndex,
                                        &resetCorrectPos);
        if(errorSub != ERROR_SUB_OK)
        {
            return errorSub;
        }
    }
    //电机复位
    CAN_MASTER_SM_CMD_RESET canMasterStepMotorResetCmd;
    CanMasterStepMotorSetResetCmdDefault(&canMasterStepMotorResetCmd);
    canMasterStepMotorResetCmd.timeOutMs = configArrayPtr[motorIdx].resetTimeOutMs;
    canMasterStepMotorResetCmd.resetCorrectPos = resetCorrectPos;
    //复位结果
    CAN_MASTER_SM_STATE stepMotorState;
    SYS_RESULT_PACK stepMotorResetResultPack;
    H360SysActStepMotorResetWhileReturn((H360_SYS_SM)configArrayPtr[motorIdx].motorNo,
                                            &canMasterStepMotorResetCmd,
                                            &stepMotorResetResultPack,
                                            &stepMotorState);
    if(stepMotorResetResultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        //失败,报错
        CanMasterReportErrorToPC(stepMotorResetResultPack.errorMain,stepMotorResetResultPack.errorLevel,
                                    stepMotorResetResultPack.errorSub);
        return stepMotorResetResultPack.errorSub;
    }
    //检查是否存在错误,独立报错
    if((stepMotorResetResultPack.errorMain != ERROR_MAIN_OK)||(stepMotorResetResultPack.errorSub != ERROR_SUB_OK))
    {
        //失败,报错
        CanMasterReportErrorToPC(stepMotorResetResultPack.errorMain,stepMotorResetResultPack.errorLevel,
                                    stepMotorResetResultPack.errorSub);
    }
    *motorCoordinate = stepMotorState.motorPosCurrent;
    //无错误返回
    return ERROR_SUB_OK;
}

//步进电机走位
ERROR_SUB CommonSrvImplBaseModuleStepMotorRunSteps(uint8_t motorIdx,int32_t runSteps,MODULE_STEP_MOTOR_CONFIG const* configArrayPtr,
                                                    uint16_t arrayLength,int32_t* motorCoordinate)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    *motorCoordinate = 0;
    //序号不支持
    if(motorIdx >= arrayLength)
    {
        return ERROR_SUB_MODULE_STEP_MOTOR_INDEX;
    }
    //电机不支持
    if(configArrayPtr[motorIdx].motorNo == MODULE_BASE_STEP_MOTOR_NO_RESERVE)
    {
        return ERROR_SUB_MODULE_STEP_MOTOR_INDEX;
    }
    CAN_MASTER_SM_CMD_RUN_STEP canMasterStepMotorRunStepsCmd;
    CanMasterStepMotorSetRunStepCmdDefault(&canMasterStepMotorRunStepsCmd);
    canMasterStepMotorRunStepsCmd.totalSteps = runSteps;
    canMasterStepMotorRunStepsCmd.timeOutMs = 60000;
    //走步数
    CAN_MASTER_SM_STATE stepMotorState;
    SYS_RESULT_PACK stepMotorRunStepsResultPack;
    //执行指令
    H360SysActStepMotorRunStepWhileReturn((H360_SYS_SM)configArrayPtr[motorIdx].motorNo,
                                            &canMasterStepMotorRunStepsCmd,
                                            &stepMotorRunStepsResultPack,
                                            &stepMotorState);
    if(stepMotorRunStepsResultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        //失败,报错
        CanMasterReportErrorToPC(stepMotorRunStepsResultPack.errorMain,stepMotorRunStepsResultPack.errorLevel,
                                    stepMotorRunStepsResultPack.errorSub);
        return stepMotorRunStepsResultPack.errorSub;
    }
    //检查是否存在错误,独立报错
    if((stepMotorRunStepsResultPack.errorMain != ERROR_MAIN_OK)||(stepMotorRunStepsResultPack.errorSub != ERROR_SUB_OK))
    {
        //失败,报错
        CanMasterReportErrorToPC(stepMotorRunStepsResultPack.errorMain,stepMotorRunStepsResultPack.errorLevel,
                                stepMotorRunStepsResultPack.errorSub);
    }
    *motorCoordinate = stepMotorState.motorPosCurrent;
    //无错误返回
    return ERROR_SUB_OK;
}






