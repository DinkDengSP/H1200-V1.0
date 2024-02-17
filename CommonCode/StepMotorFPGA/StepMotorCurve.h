/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-05-10 10:13:10 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-05-10 15:01:52 +0800
************************************************************************************************/ 
#ifndef __SM_CURVE_H_
#define __SM_CURVE_H_
#include "StepMotorType.h"

//计算步进电机运转曲线.步数曲线
void StepMotorCurveCalcMainTableWithStep(STEP_MOTOR_MAIN_CURVE_TAB_DATA* dataPtr,STEP_MOTOR_ATTRI_CURVE* curveAttriPtr,
                                            SPEED_RATIO_MODE speedRatioMode,float speedRatio,uint32_t totalSteps);

//计算步进电机运转曲线,坐标曲线
void StepMotorCurveCalcMainTableWithCoordinate(STEP_MOTOR_MAIN_CURVE_TAB_DATA* dataPtr,STEP_MOTOR_ATTRI_CURVE* curveAttriPtr,
                                            SPEED_RATIO_MODE speedRatioMode,float speedRatio,int32_t currentPos,int32_t targetPos);

//计算步进电机运转曲线,回零曲线
void StepMotorCurveCalcMainTableWithReturnZero(STEP_MOTOR_MAIN_CURVE_TAB_DATA* dataPtr,STEP_MOTOR_ATTRI_CURVE* curveAttriPtr,
                                                SPEED_RATIO_MODE speedRatioMode,float speedRatio,int32_t currentCoordinate,
                                                uint32_t returnZeroMaxStep);

//计算步进电机运转曲线.持续运转加速曲线
void StepMotorCurveCalcUtilTableWithRunAlwaysAcc(STEP_MOTOR_MAIN_CURVE_TAB_DATA* mainCurveDataPtr,STEP_MOTOR_UTIL_CURVE_TAB_DATA* utilCurveDataPtr,
                                                    STEP_MOTOR_ATTRI_CURVE* curveAttriPtr,SPEED_RATIO_MODE speedRatioMode,float speedRatio);

//计算步进电机运转曲线.持续运转减速曲线
void StepMotorCurveCalcUtilTableWithRunAlwaysDec(STEP_MOTOR_MAIN_CURVE_TAB_DATA* mainCurveDataPtr,STEP_MOTOR_UTIL_CURVE_TAB_DATA* utilCurveDataPtr,
                                                    uint16_t currentAccSpdTabIndex);

//计算步进电机运转曲线.从曲线运转到DecStop
void StepMotorCurveCalcUtilTableWithFastDecFromCurveSpd(STEP_MOTOR_MAIN_CURVE_TAB_DATA* mainCurveDataPtr,STEP_MOTOR_UTIL_CURVE_TAB_DATA* utilCurveDataPtr,
                                                        uint16_t currentCurveSpdTabIndex,uint32_t daccSteps);

//计算步进电机运转曲线.从定频运转到DecStop
void StepMotorCurveCalcUtilTableWithFastDecFromFixSpd(STEP_MOTOR_UTIL_CURVE_TAB_DATA* utilCurveDataPtr,float highSpd,float lowSpd,uint32_t steps);                                                       

#endif





