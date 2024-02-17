/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-24 16:33:38 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-08-24 17:00:20 +0800
************************************************************************************************/ 
#ifndef __STEP_MOTOR_ERROR_H_
#define __STEP_MOTOR_ERROR_H_
#include "StepMotorDataType.h"
#include "StepMotorMacro.h"

ERROR_SUB StepMotorReturnZeroParamError(STEP_MOTOR_NO motorNo);

ERROR_SUB StepMotorEncoderCorrectMaxError(STEP_MOTOR_NO motorNo);

ERROR_SUB StepMotorEncoderResetFuncNullError(STEP_MOTOR_NO motorNo);

ERROR_SUB StepMotorEncoderReadFuncNullError(STEP_MOTOR_NO motorNo);

ERROR_SUB StepMotorRunningTimeOutError(STEP_MOTOR_NO motorNo);

ERROR_SUB StepMotorRunCoordationParamError(STEP_MOTOR_NO motorNo);

ERROR_SUB StepMotorRunAlwaysParamError(STEP_MOTOR_NO motorNo);

ERROR_SUB StepMotorRunEncoderCorrectZeroPassLimitError(STEP_MOTOR_NO motorNo);

ERROR_SUB StepMotorEmergencyCmdError(STEP_MOTOR_NO motorNo);

ERROR_SUB StepMotorRunStepParamError(STEP_MOTOR_NO motorNo);

ERROR_SUB StepMotorEmergencySignalError(STEP_MOTOR_NO motorNo);

ERROR_SUB StepMotorResetStepLimitError(STEP_MOTOR_NO motorNo);

ERROR_SUB StepMotorRunCordinateMinError(STEP_MOTOR_NO motorNo);

ERROR_SUB StepMotorRunCordinateMaxError(STEP_MOTOR_NO motorNo);

ERROR_SUB StepMotorUtilStop1Error(STEP_MOTOR_NO motorNo);

ERROR_SUB StepMotorUtilStop2Error(STEP_MOTOR_NO motorNo);

ERROR_SUB StepMotorRunTimeOutZeroError(STEP_MOTOR_NO motorNo);

ERROR_SUB StepMotorResetCorrectMaxError(STEP_MOTOR_NO motorNo);

ERROR_SUB StepMotorResetCorrectMinError(STEP_MOTOR_NO motorNo);

ERROR_SUB StepMotorResetEncoderBlockError(STEP_MOTOR_NO motorNo);

ERROR_SUB StepMotorResetEncodeDirError(STEP_MOTOR_NO motorNo);

ERROR_SUB StepMotorResetEncodeMoreError(STEP_MOTOR_NO motorNo);

ERROR_SUB StepMotorResetLeaveOriginSensorError(STEP_MOTOR_NO motorNo);

ERROR_SUB StepMotorRunStepEncoderLossLimitError(STEP_MOTOR_NO motorNo);

ERROR_SUB StepMotorRunCoordinateEncoderLossLimitError(STEP_MOTOR_NO motorNo);

ERROR_SUB StepMotorSlaveReadyPinCheckError(STEP_MOTOR_NO motorNo);

#endif



