/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-24 16:33:38 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-08-24 17:38:10 +0800
************************************************************************************************/ 
#include "StepMotorError.h"

// ERROR_SUB StepMotorReturnZeroParamError(STEP_MOTOR_NO motorNo)
// {
//     switch(motorNo)
//     {
//         case ppp:
//             return xxx;
//         case ppp:
//             return xxx;
//         case ppp:
//             return xxx;
//         case ppp:
//             return xxx;
//         case ppp:
//             return xxx;
//         case ppp:
//             return xxx;
//         case ppp:
//             return xxx;
//         case ppp:
//             return xxx;
//         default:
//             return xxx;
//     }
// }


ERROR_SUB StepMotorReturnZeroParamError(STEP_MOTOR_NO motorNo)
{
    switch(motorNo)
    {
        case STEP_MOTOR_NO1:
            return ERROR_SUB_SM_1_PARAM_RETURN_ZERO;
        case STEP_MOTOR_NO2:
            return ERROR_SUB_SM_2_PARAM_RETURN_ZERO;
        case STEP_MOTOR_NO3:
            return ERROR_SUB_SM_3_PARAM_RETURN_ZERO;
        case STEP_MOTOR_NO4:
            return ERROR_SUB_SM_4_PARAM_RETURN_ZERO;
        case STEP_MOTOR_NO5:
            return ERROR_SUB_SM_5_PARAM_RETURN_ZERO;
        case STEP_MOTOR_NO6:
            return ERROR_SUB_SM_6_PARAM_RETURN_ZERO;
        case STEP_MOTOR_NO7:
            return ERROR_SUB_SM_7_PARAM_RETURN_ZERO;
        case STEP_MOTOR_NO8:
            return ERROR_SUB_SM_8_PARAM_RETURN_ZERO;
        default:
            return ERROR_SUB_SM_1_PARAM_RETURN_ZERO;
    }
}

ERROR_SUB StepMotorEncoderCorrectMaxError(STEP_MOTOR_NO motorNo)
{
    switch(motorNo)
    {
        case STEP_MOTOR_NO1:
            return ERROR_SUB_SM_1_RUN_ENCODER_CORRECT_MAX;
        case STEP_MOTOR_NO2:
            return ERROR_SUB_SM_2_RUN_ENCODER_CORRECT_MAX;
        case STEP_MOTOR_NO3:
            return ERROR_SUB_SM_3_RUN_ENCODER_CORRECT_MAX;
        case STEP_MOTOR_NO4:
            return ERROR_SUB_SM_4_RUN_ENCODER_CORRECT_MAX;
        case STEP_MOTOR_NO5:
            return ERROR_SUB_SM_5_RUN_ENCODER_CORRECT_MAX;
        case STEP_MOTOR_NO6:
            return ERROR_SUB_SM_6_RUN_ENCODER_CORRECT_MAX;
        case STEP_MOTOR_NO7:
            return ERROR_SUB_SM_7_RUN_ENCODER_CORRECT_MAX;
        case STEP_MOTOR_NO8:
            return ERROR_SUB_SM_8_RUN_ENCODER_CORRECT_MAX;
        default:
            return ERROR_SUB_SM_1_RUN_ENCODER_CORRECT_MAX;
    }
}

ERROR_SUB StepMotorEncoderResetFuncNullError(STEP_MOTOR_NO motorNo)
{
    switch(motorNo)
    {
        case STEP_MOTOR_NO1:
            return ERROR_SUB_SM_1_RUN_ENCODER_RESET_NULL;
        case STEP_MOTOR_NO2:
            return ERROR_SUB_SM_2_RUN_ENCODER_RESET_NULL;
        case STEP_MOTOR_NO3:
            return ERROR_SUB_SM_3_RUN_ENCODER_RESET_NULL;
        case STEP_MOTOR_NO4:
            return ERROR_SUB_SM_4_RUN_ENCODER_RESET_NULL;
        case STEP_MOTOR_NO5:
            return ERROR_SUB_SM_5_RUN_ENCODER_RESET_NULL;
        case STEP_MOTOR_NO6:
            return ERROR_SUB_SM_6_RUN_ENCODER_RESET_NULL;
        case STEP_MOTOR_NO7:
            return ERROR_SUB_SM_7_RUN_ENCODER_RESET_NULL;
        case STEP_MOTOR_NO8:
            return ERROR_SUB_SM_8_RUN_ENCODER_RESET_NULL;
        default:
            return ERROR_SUB_SM_1_RUN_ENCODER_RESET_NULL;
    }
}

ERROR_SUB StepMotorEncoderReadFuncNullError(STEP_MOTOR_NO motorNo)
{
    switch(motorNo)
    {
        case STEP_MOTOR_NO1:
            return ERROR_SUB_SM_1_RUN_ENCODER_READ_NULL;
        case STEP_MOTOR_NO2:
            return ERROR_SUB_SM_2_RUN_ENCODER_READ_NULL;
        case STEP_MOTOR_NO3:
            return ERROR_SUB_SM_3_RUN_ENCODER_READ_NULL;
        case STEP_MOTOR_NO4:
            return ERROR_SUB_SM_4_RUN_ENCODER_READ_NULL;
        case STEP_MOTOR_NO5:
            return ERROR_SUB_SM_5_RUN_ENCODER_READ_NULL;
        case STEP_MOTOR_NO6:
            return ERROR_SUB_SM_6_RUN_ENCODER_READ_NULL;
        case STEP_MOTOR_NO7:
            return ERROR_SUB_SM_7_RUN_ENCODER_READ_NULL;
        case STEP_MOTOR_NO8:
            return ERROR_SUB_SM_8_RUN_ENCODER_READ_NULL;
        default:
            return ERROR_SUB_SM_1_RUN_ENCODER_READ_NULL;
    }
}

ERROR_SUB StepMotorRunningTimeOutError(STEP_MOTOR_NO motorNo)
{
    switch(motorNo)
    {
        case STEP_MOTOR_NO1:
            return ERROR_SUB_SM_1_TIMEOUT;
        case STEP_MOTOR_NO2:
            return ERROR_SUB_SM_2_TIMEOUT;
        case STEP_MOTOR_NO3:
            return ERROR_SUB_SM_3_TIMEOUT;
        case STEP_MOTOR_NO4:
            return ERROR_SUB_SM_4_TIMEOUT;
        case STEP_MOTOR_NO5:
            return ERROR_SUB_SM_5_TIMEOUT;
        case STEP_MOTOR_NO6:
            return ERROR_SUB_SM_6_TIMEOUT;
        case STEP_MOTOR_NO7:
            return ERROR_SUB_SM_7_TIMEOUT;
        case STEP_MOTOR_NO8:
            return ERROR_SUB_SM_8_TIMEOUT;
        default:
            return ERROR_SUB_SM_1_TIMEOUT;
    }
}

ERROR_SUB StepMotorRunCoordationParamError(STEP_MOTOR_NO motorNo)
{
    switch(motorNo)
    {
        case STEP_MOTOR_NO1:
            return ERROR_SUB_SM_1_PARAM_COORDATION;
        case STEP_MOTOR_NO2:
            return ERROR_SUB_SM_2_PARAM_COORDATION;
        case STEP_MOTOR_NO3:
            return ERROR_SUB_SM_3_PARAM_COORDATION;
        case STEP_MOTOR_NO4:
            return ERROR_SUB_SM_4_PARAM_COORDATION;
        case STEP_MOTOR_NO5:
            return ERROR_SUB_SM_5_PARAM_COORDATION;
        case STEP_MOTOR_NO6:
            return ERROR_SUB_SM_6_PARAM_COORDATION;
        case STEP_MOTOR_NO7:
            return ERROR_SUB_SM_7_PARAM_COORDATION;
        case STEP_MOTOR_NO8:
            return ERROR_SUB_SM_8_PARAM_COORDATION;
        default:
            return ERROR_SUB_SM_1_PARAM_COORDATION;
    }
}

ERROR_SUB StepMotorRunAlwaysParamError(STEP_MOTOR_NO motorNo)
{
    switch(motorNo)
    {
        case STEP_MOTOR_NO1:
            return ERROR_SUB_SM_1_PARAM_RUN_ALWAYS;
        case STEP_MOTOR_NO2:
            return ERROR_SUB_SM_2_PARAM_RUN_ALWAYS;
        case STEP_MOTOR_NO3:
            return ERROR_SUB_SM_3_PARAM_RUN_ALWAYS;
        case STEP_MOTOR_NO4:
            return ERROR_SUB_SM_4_PARAM_RUN_ALWAYS;
        case STEP_MOTOR_NO5:
            return ERROR_SUB_SM_5_PARAM_RUN_ALWAYS;
        case STEP_MOTOR_NO6:
            return ERROR_SUB_SM_6_PARAM_RUN_ALWAYS;
        case STEP_MOTOR_NO7:
            return ERROR_SUB_SM_7_PARAM_RUN_ALWAYS;
        case STEP_MOTOR_NO8:
            return ERROR_SUB_SM_8_PARAM_RUN_ALWAYS;
        default:
            return ERROR_SUB_SM_1_PARAM_RUN_ALWAYS;
    }
}

ERROR_SUB StepMotorRunEncoderCorrectZeroPassLimitError(STEP_MOTOR_NO motorNo)
{
    switch(motorNo)
    {
        case STEP_MOTOR_NO1:
            return ERROR_SUB_SM_1_RUN_ENCODER_CORRECT_ZERO_PASS_LIMIT;
        case STEP_MOTOR_NO2:
            return ERROR_SUB_SM_2_RUN_ENCODER_CORRECT_ZERO_PASS_LIMIT;
        case STEP_MOTOR_NO3:
            return ERROR_SUB_SM_3_RUN_ENCODER_CORRECT_ZERO_PASS_LIMIT;
        case STEP_MOTOR_NO4:
            return ERROR_SUB_SM_4_RUN_ENCODER_CORRECT_ZERO_PASS_LIMIT;
        case STEP_MOTOR_NO5:
            return ERROR_SUB_SM_5_RUN_ENCODER_CORRECT_ZERO_PASS_LIMIT;
        case STEP_MOTOR_NO6:
            return ERROR_SUB_SM_6_RUN_ENCODER_CORRECT_ZERO_PASS_LIMIT;
        case STEP_MOTOR_NO7:
            return ERROR_SUB_SM_7_RUN_ENCODER_CORRECT_ZERO_PASS_LIMIT;
        case STEP_MOTOR_NO8:
            return ERROR_SUB_SM_8_RUN_ENCODER_CORRECT_ZERO_PASS_LIMIT;
        default:
            return ERROR_SUB_SM_1_RUN_ENCODER_CORRECT_ZERO_PASS_LIMIT;
    }
}

ERROR_SUB StepMotorEmergencyCmdError(STEP_MOTOR_NO motorNo)
{
    switch(motorNo)
    {
        case STEP_MOTOR_NO1:
            return ERROR_SUB_SM_1_EMERGENCY_CMD;
        case STEP_MOTOR_NO2:
            return ERROR_SUB_SM_2_EMERGENCY_CMD;
        case STEP_MOTOR_NO3:
            return ERROR_SUB_SM_3_EMERGENCY_CMD;
        case STEP_MOTOR_NO4:
            return ERROR_SUB_SM_4_EMERGENCY_CMD;
        case STEP_MOTOR_NO5:
            return ERROR_SUB_SM_5_EMERGENCY_CMD;
        case STEP_MOTOR_NO6:
            return ERROR_SUB_SM_6_EMERGENCY_CMD;
        case STEP_MOTOR_NO7:
            return ERROR_SUB_SM_7_EMERGENCY_CMD;
        case STEP_MOTOR_NO8:
            return ERROR_SUB_SM_8_EMERGENCY_CMD;
        default:
            return ERROR_SUB_SM_1_EMERGENCY_CMD;
    }
}

ERROR_SUB StepMotorRunStepParamError(STEP_MOTOR_NO motorNo)
{
    switch(motorNo)
    {
        case STEP_MOTOR_NO1:
            return ERROR_SUB_SM_1_PARAM_STEPS;
        case STEP_MOTOR_NO2:
            return ERROR_SUB_SM_2_PARAM_STEPS;
        case STEP_MOTOR_NO3:
            return ERROR_SUB_SM_3_PARAM_STEPS;
        case STEP_MOTOR_NO4:
            return ERROR_SUB_SM_4_PARAM_STEPS;
        case STEP_MOTOR_NO5:
            return ERROR_SUB_SM_5_PARAM_STEPS;
        case STEP_MOTOR_NO6:
            return ERROR_SUB_SM_6_PARAM_STEPS;
        case STEP_MOTOR_NO7:
            return ERROR_SUB_SM_7_PARAM_STEPS;
        case STEP_MOTOR_NO8:
            return ERROR_SUB_SM_8_PARAM_STEPS;
        default:
            return ERROR_SUB_SM_1_PARAM_STEPS;
    }
}

ERROR_SUB StepMotorEmergencySignalError(STEP_MOTOR_NO motorNo)
{
    switch(motorNo)
    {
        case STEP_MOTOR_NO1:
            return ERROR_SUB_SM_1_EMERGENCY_SIGNAL;
        case STEP_MOTOR_NO2:
            return ERROR_SUB_SM_2_EMERGENCY_SIGNAL;
        case STEP_MOTOR_NO3:
            return ERROR_SUB_SM_3_EMERGENCY_SIGNAL;
        case STEP_MOTOR_NO4:
            return ERROR_SUB_SM_4_EMERGENCY_SIGNAL;
        case STEP_MOTOR_NO5:
            return ERROR_SUB_SM_5_EMERGENCY_SIGNAL;
        case STEP_MOTOR_NO6:
            return ERROR_SUB_SM_6_EMERGENCY_SIGNAL;
        case STEP_MOTOR_NO7:
            return ERROR_SUB_SM_7_EMERGENCY_SIGNAL;
        case STEP_MOTOR_NO8:
            return ERROR_SUB_SM_8_EMERGENCY_SIGNAL;
        default:
            return ERROR_SUB_SM_1_EMERGENCY_SIGNAL;
    }
}

ERROR_SUB StepMotorResetStepLimitError(STEP_MOTOR_NO motorNo)
{
    switch(motorNo)
    {
        case STEP_MOTOR_NO1:
            return ERROR_SUB_SM_1_RESET_STEP_LIMIT;
        case STEP_MOTOR_NO2:
            return ERROR_SUB_SM_2_RESET_STEP_LIMIT;
        case STEP_MOTOR_NO3:
            return ERROR_SUB_SM_3_RESET_STEP_LIMIT;
        case STEP_MOTOR_NO4:
            return ERROR_SUB_SM_4_RESET_STEP_LIMIT;
        case STEP_MOTOR_NO5:
            return ERROR_SUB_SM_5_RESET_STEP_LIMIT;
        case STEP_MOTOR_NO6:
            return ERROR_SUB_SM_6_RESET_STEP_LIMIT;
        case STEP_MOTOR_NO7:
            return ERROR_SUB_SM_7_RESET_STEP_LIMIT;
        case STEP_MOTOR_NO8:
            return ERROR_SUB_SM_8_RESET_STEP_LIMIT;
        default:
            return ERROR_SUB_SM_1_RESET_STEP_LIMIT;
    }
}

ERROR_SUB StepMotorRunCordinateMinError(STEP_MOTOR_NO motorNo)
{
    switch(motorNo)
    {
        case STEP_MOTOR_NO1:
            return ERROR_SUB_SM_1_CORDINATE_MIN;
        case STEP_MOTOR_NO2:
            return ERROR_SUB_SM_2_CORDINATE_MIN;
        case STEP_MOTOR_NO3:
            return ERROR_SUB_SM_3_CORDINATE_MIN;
        case STEP_MOTOR_NO4:
            return ERROR_SUB_SM_4_CORDINATE_MIN;
        case STEP_MOTOR_NO5:
            return ERROR_SUB_SM_5_CORDINATE_MIN;
        case STEP_MOTOR_NO6:
            return ERROR_SUB_SM_6_CORDINATE_MIN;
        case STEP_MOTOR_NO7:
            return ERROR_SUB_SM_7_CORDINATE_MIN;
        case STEP_MOTOR_NO8:
            return ERROR_SUB_SM_8_CORDINATE_MIN;
        default:
            return ERROR_SUB_SM_1_CORDINATE_MIN;
    }
}

ERROR_SUB StepMotorRunCordinateMaxError(STEP_MOTOR_NO motorNo)
{
    switch(motorNo)
    {
        case STEP_MOTOR_NO1:
            return ERROR_SUB_SM_1_CORDINATE_MAX;
        case STEP_MOTOR_NO2:
            return ERROR_SUB_SM_2_CORDINATE_MAX;
        case STEP_MOTOR_NO3:
            return ERROR_SUB_SM_3_CORDINATE_MAX;
        case STEP_MOTOR_NO4:
            return ERROR_SUB_SM_4_CORDINATE_MAX;
        case STEP_MOTOR_NO5:
            return ERROR_SUB_SM_5_CORDINATE_MAX;
        case STEP_MOTOR_NO6:
            return ERROR_SUB_SM_6_CORDINATE_MAX;
        case STEP_MOTOR_NO7:
            return ERROR_SUB_SM_7_CORDINATE_MAX;
        case STEP_MOTOR_NO8:
            return ERROR_SUB_SM_8_CORDINATE_MAX;
        default:
            return ERROR_SUB_SM_1_CORDINATE_MAX;
    }
}

ERROR_SUB StepMotorUtilStop1Error(STEP_MOTOR_NO motorNo)
{
    switch(motorNo)
    {
        case STEP_MOTOR_NO1:
            return ERROR_SUB_SM_1_UTIL_STOP1;
        case STEP_MOTOR_NO2:
            return ERROR_SUB_SM_2_UTIL_STOP1;
        case STEP_MOTOR_NO3:
            return ERROR_SUB_SM_3_UTIL_STOP1;
        case STEP_MOTOR_NO4:
            return ERROR_SUB_SM_4_UTIL_STOP1;
        case STEP_MOTOR_NO5:
            return ERROR_SUB_SM_5_UTIL_STOP1;
        case STEP_MOTOR_NO6:
            return ERROR_SUB_SM_6_UTIL_STOP1;
        case STEP_MOTOR_NO7:
            return ERROR_SUB_SM_7_UTIL_STOP1;
        case STEP_MOTOR_NO8:
            return ERROR_SUB_SM_8_UTIL_STOP1;
        default:
            return ERROR_SUB_SM_1_UTIL_STOP1;
    }
}

ERROR_SUB StepMotorUtilStop2Error(STEP_MOTOR_NO motorNo)
{
    switch(motorNo)
    {
        case STEP_MOTOR_NO1:
            return ERROR_SUB_SM_1_UTIL_STOP2;
        case STEP_MOTOR_NO2:
            return ERROR_SUB_SM_2_UTIL_STOP2;
        case STEP_MOTOR_NO3:
            return ERROR_SUB_SM_3_UTIL_STOP2;
        case STEP_MOTOR_NO4:
            return ERROR_SUB_SM_4_UTIL_STOP2;
        case STEP_MOTOR_NO5:
            return ERROR_SUB_SM_5_UTIL_STOP2;
        case STEP_MOTOR_NO6:
            return ERROR_SUB_SM_6_UTIL_STOP2;
        case STEP_MOTOR_NO7:
            return ERROR_SUB_SM_7_UTIL_STOP2;
        case STEP_MOTOR_NO8:
            return ERROR_SUB_SM_8_UTIL_STOP2;
        default:
            return ERROR_SUB_SM_1_UTIL_STOP2;
    }
}

ERROR_SUB StepMotorRunTimeOutZeroError(STEP_MOTOR_NO motorNo)
{
    switch(motorNo)
    {
        case STEP_MOTOR_NO1:
            return ERROR_SUB_SM_1_TIME_OUT_ZERO;
        case STEP_MOTOR_NO2:
            return ERROR_SUB_SM_2_TIME_OUT_ZERO;
        case STEP_MOTOR_NO3:
            return ERROR_SUB_SM_3_TIME_OUT_ZERO;
        case STEP_MOTOR_NO4:
            return ERROR_SUB_SM_4_TIME_OUT_ZERO;
        case STEP_MOTOR_NO5:
            return ERROR_SUB_SM_5_TIME_OUT_ZERO;
        case STEP_MOTOR_NO6:
            return ERROR_SUB_SM_6_TIME_OUT_ZERO;
        case STEP_MOTOR_NO7:
            return ERROR_SUB_SM_7_TIME_OUT_ZERO;
        case STEP_MOTOR_NO8:
            return ERROR_SUB_SM_8_TIME_OUT_ZERO;
        default:
            return ERROR_SUB_SM_1_TIME_OUT_ZERO;
    }
}

ERROR_SUB StepMotorResetCorrectMaxError(STEP_MOTOR_NO motorNo)
{
    switch(motorNo)
    {
        case STEP_MOTOR_NO1:
            return ERROR_SUB_SM_1_RESET_CORRECT_MAX;
        case STEP_MOTOR_NO2:
            return ERROR_SUB_SM_2_RESET_CORRECT_MAX;
        case STEP_MOTOR_NO3:
            return ERROR_SUB_SM_3_RESET_CORRECT_MAX;
        case STEP_MOTOR_NO4:
            return ERROR_SUB_SM_4_RESET_CORRECT_MAX;
        case STEP_MOTOR_NO5:
            return ERROR_SUB_SM_5_RESET_CORRECT_MAX;
        case STEP_MOTOR_NO6:
            return ERROR_SUB_SM_6_RESET_CORRECT_MAX;
        case STEP_MOTOR_NO7:
            return ERROR_SUB_SM_7_RESET_CORRECT_MAX;
        case STEP_MOTOR_NO8:
            return ERROR_SUB_SM_8_RESET_CORRECT_MAX;
        default:
            return ERROR_SUB_SM_1_RESET_CORRECT_MAX;
    }
}

ERROR_SUB StepMotorResetCorrectMinError(STEP_MOTOR_NO motorNo)
{
    switch(motorNo)
    {
        case STEP_MOTOR_NO1:
            return ERROR_SUB_SM_1_RESET_CORRECT_MIN;
        case STEP_MOTOR_NO2:
            return ERROR_SUB_SM_2_RESET_CORRECT_MIN;
        case STEP_MOTOR_NO3:
            return ERROR_SUB_SM_3_RESET_CORRECT_MIN;
        case STEP_MOTOR_NO4:
            return ERROR_SUB_SM_4_RESET_CORRECT_MIN;
        case STEP_MOTOR_NO5:
            return ERROR_SUB_SM_5_RESET_CORRECT_MIN;
        case STEP_MOTOR_NO6:
            return ERROR_SUB_SM_6_RESET_CORRECT_MIN;
        case STEP_MOTOR_NO7:
            return ERROR_SUB_SM_7_RESET_CORRECT_MIN;
        case STEP_MOTOR_NO8:
            return ERROR_SUB_SM_8_RESET_CORRECT_MIN;
        default:
            return ERROR_SUB_SM_1_RESET_CORRECT_MIN;
    }
}

ERROR_SUB StepMotorResetEncoderBlockError(STEP_MOTOR_NO motorNo)
{
    switch(motorNo)
    {
        case STEP_MOTOR_NO1:
            return ERROR_SUB_SM_1_RESET_ENCODER_BLOCK;
        case STEP_MOTOR_NO2:
            return ERROR_SUB_SM_2_RESET_ENCODER_BLOCK;
        case STEP_MOTOR_NO3:
            return ERROR_SUB_SM_3_RESET_ENCODER_BLOCK;
        case STEP_MOTOR_NO4:
            return ERROR_SUB_SM_4_RESET_ENCODER_BLOCK;
        case STEP_MOTOR_NO5:
            return ERROR_SUB_SM_5_RESET_ENCODER_BLOCK;
        case STEP_MOTOR_NO6:
            return ERROR_SUB_SM_6_RESET_ENCODER_BLOCK;
        case STEP_MOTOR_NO7:
            return ERROR_SUB_SM_7_RESET_ENCODER_BLOCK;
        case STEP_MOTOR_NO8:
            return ERROR_SUB_SM_8_RESET_ENCODER_BLOCK;
        default:
            return ERROR_SUB_SM_1_RESET_ENCODER_BLOCK;
    }
}

ERROR_SUB StepMotorResetEncodeDirError(STEP_MOTOR_NO motorNo)
{
    switch(motorNo)
    {
        case STEP_MOTOR_NO1:
            return ERROR_SUB_SM_1_RESET_ENCODER_DIR;
        case STEP_MOTOR_NO2:
            return ERROR_SUB_SM_2_RESET_ENCODER_DIR;
        case STEP_MOTOR_NO3:
            return ERROR_SUB_SM_3_RESET_ENCODER_DIR;
        case STEP_MOTOR_NO4:
            return ERROR_SUB_SM_4_RESET_ENCODER_DIR;
        case STEP_MOTOR_NO5:
            return ERROR_SUB_SM_5_RESET_ENCODER_DIR;
        case STEP_MOTOR_NO6:
            return ERROR_SUB_SM_6_RESET_ENCODER_DIR;
        case STEP_MOTOR_NO7:
            return ERROR_SUB_SM_7_RESET_ENCODER_DIR;
        case STEP_MOTOR_NO8:
            return ERROR_SUB_SM_8_RESET_ENCODER_DIR;
        default:
            return ERROR_SUB_SM_1_RESET_ENCODER_DIR;
    }
}

ERROR_SUB StepMotorResetEncodeMoreError(STEP_MOTOR_NO motorNo)
{
    switch(motorNo)
    {
        case STEP_MOTOR_NO1:
            return ERROR_SUB_SM_1_RESET_ENCODER_MORE;
        case STEP_MOTOR_NO2:
            return ERROR_SUB_SM_2_RESET_ENCODER_MORE;
        case STEP_MOTOR_NO3:
            return ERROR_SUB_SM_3_RESET_ENCODER_MORE;
        case STEP_MOTOR_NO4:
            return ERROR_SUB_SM_4_RESET_ENCODER_MORE;
        case STEP_MOTOR_NO5:
            return ERROR_SUB_SM_5_RESET_ENCODER_MORE;
        case STEP_MOTOR_NO6:
            return ERROR_SUB_SM_6_RESET_ENCODER_MORE;
        case STEP_MOTOR_NO7:
            return ERROR_SUB_SM_7_RESET_ENCODER_MORE;
        case STEP_MOTOR_NO8:
            return ERROR_SUB_SM_8_RESET_ENCODER_MORE;
        default:
            return ERROR_SUB_SM_1_RESET_ENCODER_MORE;
    }
}

ERROR_SUB StepMotorResetLeaveOriginSensorError(STEP_MOTOR_NO motorNo)
{
    switch(motorNo)
    {
        case STEP_MOTOR_NO1:
            return ERROR_SUB_SM_1_RESET_LEAVE_ORIGIN_SENSOR;
        case STEP_MOTOR_NO2:
            return ERROR_SUB_SM_2_RESET_LEAVE_ORIGIN_SENSOR;
        case STEP_MOTOR_NO3:
            return ERROR_SUB_SM_3_RESET_LEAVE_ORIGIN_SENSOR;
        case STEP_MOTOR_NO4:
            return ERROR_SUB_SM_4_RESET_LEAVE_ORIGIN_SENSOR;
        case STEP_MOTOR_NO5:
            return ERROR_SUB_SM_5_RESET_LEAVE_ORIGIN_SENSOR;
        case STEP_MOTOR_NO6:
            return ERROR_SUB_SM_6_RESET_LEAVE_ORIGIN_SENSOR;
        case STEP_MOTOR_NO7:
            return ERROR_SUB_SM_7_RESET_LEAVE_ORIGIN_SENSOR;
        case STEP_MOTOR_NO8:
            return ERROR_SUB_SM_8_RESET_LEAVE_ORIGIN_SENSOR;
        default:
            return ERROR_SUB_SM_8_RESET_LEAVE_ORIGIN_SENSOR;
    }
}

ERROR_SUB StepMotorRunStepEncoderLossLimitError(STEP_MOTOR_NO motorNo)
{
    switch(motorNo)
    {
        case STEP_MOTOR_NO1:
            return ERROR_SUB_SM_1_RUN_STEP_ENCODER_LOSS_LIMIT;
        case STEP_MOTOR_NO2:
            return ERROR_SUB_SM_2_RUN_STEP_ENCODER_LOSS_LIMIT;
        case STEP_MOTOR_NO3:
            return ERROR_SUB_SM_3_RUN_STEP_ENCODER_LOSS_LIMIT;
        case STEP_MOTOR_NO4:
            return ERROR_SUB_SM_4_RUN_STEP_ENCODER_LOSS_LIMIT;
        case STEP_MOTOR_NO5:
            return ERROR_SUB_SM_5_RUN_STEP_ENCODER_LOSS_LIMIT;
        case STEP_MOTOR_NO6:
            return ERROR_SUB_SM_6_RUN_STEP_ENCODER_LOSS_LIMIT;
        case STEP_MOTOR_NO7:
            return ERROR_SUB_SM_7_RUN_STEP_ENCODER_LOSS_LIMIT;
        case STEP_MOTOR_NO8:
            return ERROR_SUB_SM_8_RUN_STEP_ENCODER_LOSS_LIMIT;
        default:
            return ERROR_SUB_SM_1_RUN_STEP_ENCODER_LOSS_LIMIT;
    }
}

ERROR_SUB StepMotorRunCoordinateEncoderLossLimitError(STEP_MOTOR_NO motorNo)
{
    switch(motorNo)
    {
        case STEP_MOTOR_NO1:
            return ERROR_SUB_SM_1_RUN_COORDINATE_ENCODER_LOSS_LIMIT;
        case STEP_MOTOR_NO2:
            return ERROR_SUB_SM_2_RUN_COORDINATE_ENCODER_LOSS_LIMIT;
        case STEP_MOTOR_NO3:
            return ERROR_SUB_SM_3_RUN_COORDINATE_ENCODER_LOSS_LIMIT;
        case STEP_MOTOR_NO4:
            return ERROR_SUB_SM_4_RUN_COORDINATE_ENCODER_LOSS_LIMIT;
        case STEP_MOTOR_NO5:
            return ERROR_SUB_SM_5_RUN_COORDINATE_ENCODER_LOSS_LIMIT;
        case STEP_MOTOR_NO6:
            return ERROR_SUB_SM_6_RUN_COORDINATE_ENCODER_LOSS_LIMIT;
        case STEP_MOTOR_NO7:
            return ERROR_SUB_SM_7_RUN_COORDINATE_ENCODER_LOSS_LIMIT;
        case STEP_MOTOR_NO8:
            return ERROR_SUB_SM_8_RUN_COORDINATE_ENCODER_LOSS_LIMIT;
        default:
            return ERROR_SUB_SM_1_RUN_COORDINATE_ENCODER_LOSS_LIMIT;
    }
}

ERROR_SUB StepMotorSlaveReadyPinCheckError(STEP_MOTOR_NO motorNo)
{
    switch(motorNo)
    {
        case STEP_MOTOR_NO1:
            return ERROR_SUB_SM_1_SLAVE_READY_FAULT;
        case STEP_MOTOR_NO2:
            return ERROR_SUB_SM_2_SLAVE_READY_FAULT;
        case STEP_MOTOR_NO3:
            return ERROR_SUB_SM_3_SLAVE_READY_FAULT;
        case STEP_MOTOR_NO4:
            return ERROR_SUB_SM_4_SLAVE_READY_FAULT;
        case STEP_MOTOR_NO5:
            return ERROR_SUB_SM_5_SLAVE_READY_FAULT;
        case STEP_MOTOR_NO6:
            return ERROR_SUB_SM_6_SLAVE_READY_FAULT;
        case STEP_MOTOR_NO7:
            return ERROR_SUB_SM_7_SLAVE_READY_FAULT;
        case STEP_MOTOR_NO8:
            return ERROR_SUB_SM_8_SLAVE_READY_FAULT;
        default:
            return ERROR_SUB_SM_1_SLAVE_READY_FAULT;
    }
}


