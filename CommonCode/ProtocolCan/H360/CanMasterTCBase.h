/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-24 10:44:38 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-20 13:58:09 +0800
************************************************************************************************/ 
#ifndef __CAN_MASTER_TC_BASE_H_
#define __CAN_MASTER_TC_BASE_H_
#include "CanMasterUtil.h"

typedef enum TC_BASE_CHANNEL
{
    TC_BASE_CHANNEL_BOARD               = 0X00,//板上温度通道
    TC_BASE_CHANNEL_ENVIR               = 0X01,//环境温度通道
    TC_BASE_CHANNEL_PLATE_REACTION      = 0X02,//反应盘温度通道
    TC_BASE_CHANNEL_PLATE_WASH          = 0X03,//清洗盘温度通道
    TC_BASE_CHANNEL_PLATE_REAGENT       = 0X04,//试剂盘温度通道
}TC_BASE_CHANNEL;

//通道温控状态
typedef enum TEMP_CONTROL_STATE
{
    TEMP_CONTROL_STATE_IDLE                   = 0X00,//控温空闲,也就是初始化状态
    TEMP_CONTROL_STATE_DISABLE                = 0X01,//控温禁止
    TEMP_CONTROL_STATE_OFF                    = 0X02,//控温关闭
    TEMP_CONTROL_STATE_RUNNING_WAIT_WATER     = 0X03,//温控运行,等待上水
    TEMP_CONTROL_STATE_RUNNING_MAX_POWER      = 0X04,//控温最高功率运行
    TEMP_CONTROL_STATE_RUNNING_MIN_POWER      = 0X05,//控温最低功率运行
    TEMP_CONTROL_STATE_RUNNING_PID            = 0X06,//控温PID运行 
    TEMP_CONTROL_STATE_RUNNING_NORMAL         = 0X07,//系统常规运行
    TEMP_CONTROL_STATE_STOP_FLOAT             = 0X08,//控温停止,循环水不足
    TEMP_CONTROL_STATE_STOP_BOARD_SENSOR      = 0X09,//控温停止,板上温度异常
    TEMP_CONTROL_STATE_STOP_BOARD_HIGH        = 0X0A,//控温停止,板上温度上限
    TEMP_CONTROL_STATE_STOP_BOARD_LOW         = 0X0B,//控温停止,板上温度下限
    TEMP_CONTROL_STATE_STOP_SENSOR_ERR        = 0X0C,//控温停止,传感器错误      
    TEMP_CONTROL_STATE_STOP_SENSOR_HIGH       = 0X0D,//控温停止,传感器温度上限
    TEMP_CONTROL_STATE_STOP_SENSOR_LOW        = 0X0E,//控温停止,传感器温度下限
    TEMP_CONTROL_STATE_STOP_CURRENT           = 0X0F,//控温停止,电流检测异常
    TEMP_CONTROL_STATE_STOP_ENVIR             = 0X10,//控温停止,环境温度控制异常
    TEMP_CONTROL_STATE_ADD_REFRIGERANT_MODE   = 0X11,//控温停止,添加制冷液模式
}TEMP_CONTROL_STATE;


/*************************************发送指令等待ACK************************************************/
//开启指定通道温度上报
void CanMasterTCBaseReportFlagSetWhileAck(TC_BASE_CHANNEL channel,FunctionalState flagSet,uint32_t intervalTimeMs,
                                            SYS_RESULT_PACK* resultPackPtr);
//获取板上控温状态
void CanMasterTCBaseGetBoardTempControlWhileAck(SYS_RESULT_PACK* resultPackPtr);
//获取环境控温状态
void CanMasterTCBaseGetEnvirTempControlWhileAck(SYS_RESULT_PACK* resultPackPtr);
//获取反应盘控温状态
void CanMasterTCBaseGetPlateReactionTempControlWhileAck(SYS_RESULT_PACK* resultPackPtr);
//获取清洗盘控温状态
void CanMasterTCBaseGetPlateWashTempControlWhileAck(SYS_RESULT_PACK* resultPackPtr);
//获取试剂盘控温状态
void CanMasterTCBaseGetPlateReagentTempControlWhileAck(SYS_RESULT_PACK* resultPackPtr);
//获取试剂盘浮球状态
void CanMasterTCBaseGetPlateReagentFloatSensorWhileAck(SYS_RESULT_PACK* resultPackPtr);
//获取反应盘电流检测值
void CanMasterTCBaseGetPlateReactionCurrentWhileAck(SYS_RESULT_PACK* resultPackPtr);
//获取清洗盘电流检测值
void CanMasterTCBaseGetPlateWashCurrentWhileAck(SYS_RESULT_PACK* resultPackPtr);
//获取试剂盘电流检测值
void CanMasterTCBaseGetPlateReagentCurrentWhileAck(SYS_RESULT_PACK* resultPackPtr);
//设置试剂盘增加制冷液模式
void CanMasterTCBaseSetAddRefrigerantModeWhileAck(FunctionalState setMode,SYS_RESULT_PACK* resultPackPtr);
/*************************************等待指定指令完成返回************************************************/
//开启指定通道温度上报
void CanMasterTCBaseWaitReportFlagSetResult(SYS_RESULT_PACK* resultPackPtr);
//获取板上控温状态
void CanMasterTCBaseWaitGetBoardTempControlResult(float* tempValPtr,ERROR_SUB* errorSensorPtr,SYS_RESULT_PACK* resultPackPtr);
//获取环境控温状态
void CanMasterTCBaseWaitGetEnvirTempControlResult(float* tempValPtr,ERROR_SUB* errorSensorPtr,TEMP_CONTROL_STATE* controlStatePtr,
                                                    SYS_RESULT_PACK* resultPackPtr);
//获取反应盘控温状态
void CanMasterTCBaseWaitGetPlateReactionTempControlResult(float* tempValPtr,ERROR_SUB* errorSensorPtr,TEMP_CONTROL_STATE* controlStatePtr,
                                                    SYS_RESULT_PACK* resultPackPtr);
//获取清洗盘控温状态
void CanMasterTCBaseWaitPlateWashTempControlResult(float* tempValPtr,ERROR_SUB* errorSensorPtr,TEMP_CONTROL_STATE* controlStatePtr,
                                                    SYS_RESULT_PACK* resultPackPtr);
//获取试剂盘控温状态
void CanMasterTCBaseWaitPlateReagentTempControlResult(float* tempValPtr,ERROR_SUB* errorSensorPtr,TEMP_CONTROL_STATE* controlStatePtr,
                                                    PIN_STATE* pinStatePtr,SYS_RESULT_PACK* resultPackPtr);
//获取试剂盘浮球状态
void CanMasterTCBaseWaitGetPlateReagentFloatSensorResult(PIN_STATE* pinStatePtr,SYS_RESULT_PACK* resultPackPtr);
//获取反应盘电流检测值
void CanMasterTCBaseWaitGetPlateReactionCurrentResult(int32_t* currentMaPtr,SYS_RESULT_PACK* resultPackPtr);
//获取清洗盘电流检测值
void CanMasterTCBaseWaitGetPlateWashCurrentResult(int32_t* currentMaPtr,SYS_RESULT_PACK* resultPackPtr);
//获取试剂盘电流检测值
void CanMasterTCBaseWaitGetPlateReagentCurrentResult(int32_t* current1MaPtr,int32_t* current2MaPtr,int32_t* current3MaPtr,int32_t* current4MaPtr,
                                                        SYS_RESULT_PACK* resultPackPtr);
//设置试剂盘增加制冷液模式
void CanMasterTCBaseWaitSetAddRefrigerantModeResult(SYS_RESULT_PACK* resultPackPtr);
/*************************************发送指令等待完成返回************************************************/
//开启指定通道温度上报
void CanMasterTCBaseReportFlagSetWhileReturn(TC_BASE_CHANNEL channel,FunctionalState flagSet,uint32_t intervalTimeMs,
                                                SYS_RESULT_PACK* resultPackPtr);
//获取板上控温状态
void CanMasterTCBaseGetBoardTempControlWhileReturn(float* tempValPtr,ERROR_SUB* errorSensorPtr,SYS_RESULT_PACK* resultPackPtr);
//获取环境控温状态
void CanMasterTCBaseGetEnvirTempControlWhileReturn(float* tempValPtr,ERROR_SUB* errorSensorPtr,TEMP_CONTROL_STATE* controlStatePtr,
                                                    SYS_RESULT_PACK* resultPackPtr);
//获取反应盘控温状态
void CanMasterTCBaseGetPlateReactionTempControlWhileReturn(float* tempValPtr,ERROR_SUB* errorSensorPtr,TEMP_CONTROL_STATE* controlStatePtr,
                                                    SYS_RESULT_PACK* resultPackPtr);
//获取清洗盘控温状态
void CanMasterTCBaseGetPlateWashTempControlWhileReturn(float* tempValPtr,ERROR_SUB* errorSensorPtr,TEMP_CONTROL_STATE* controlStatePtr,
                                                    SYS_RESULT_PACK* resultPackPtr);
//获取试剂盘控温状态
void CanMasterTCBaseGetPlateReagentTempControlWhileReturn(float* tempValPtr,ERROR_SUB* errorSensorPtr,TEMP_CONTROL_STATE* controlStatePtr,
                                                    PIN_STATE* pinStatePtr,SYS_RESULT_PACK* resultPackPtr);
//获取试剂盘浮球状态
void CanMasterTCBaseGetPlateReagentFloatSensorWhileReturn(PIN_STATE* pinStatePtr,SYS_RESULT_PACK* resultPackPtr);
//获取反应盘电流检测值
void CanMasterTCBaseGetPlateReactionCurrentWhileReturn(int32_t* currentMaPtr,SYS_RESULT_PACK* resultPackPtr);
//获取清洗盘电流检测值
void CanMasterTCBaseGetPlateWashCurrentWhileReturn(int32_t* currentMaPtr,SYS_RESULT_PACK* resultPackPtr);
//获取试剂盘电流检测值
void CanMasterTCBaseGetPlateReagentCurrentWhileReturn(int32_t* current1MaPtr,int32_t* current2MaPtr,int32_t* current3MaPtr,int32_t* current4MaPtr,
                                                        SYS_RESULT_PACK* resultPackPtr);
//设置试剂盘增加制冷液模式
void CanMasterTCBaseSetAddRefrigerantModeWhileReturn(FunctionalState setMode,SYS_RESULT_PACK* resultPackPtr);

/*************************************主动上报************************************************/
//上报板卡传感器信息
void CanMasterTCBaseReportBoardTemp(uint8_t targetModuleNo,uint8_t targetBoardID,float tempVal,ERROR_SUB errorSubSensor,
                                        SYS_RESULT_PACK* resultPackPtr);
//上报环境控温信息
void CanMasterTCBaseReportEnvirTempControl(uint8_t targetModuleNo,uint8_t targetBoardID,float tempVal,ERROR_SUB errorSubSensor,
                                            TEMP_CONTROL_STATE controlState,SYS_RESULT_PACK* resultPackPtr);
//上报反应盘控温信息
void CanMasterTCBaseReportPlateReactionTempControl(uint8_t targetModuleNo,uint8_t targetBoardID,float tempVal,ERROR_SUB errorSubSensor,
                                                    TEMP_CONTROL_STATE controlState,SYS_RESULT_PACK* resultPackPtr);
//上报清洗盘控温信息
void CanMasterTCBaseReportPlateWashTempControl(uint8_t targetModuleNo,uint8_t targetBoardID,float tempVal,ERROR_SUB errorSubSensor,
                                                TEMP_CONTROL_STATE controlState,SYS_RESULT_PACK* resultPackPtr);
//上报试剂盘控温信息
void CanMasterTCBaseReportPlateReagentTempControl(uint8_t targetModuleNo,uint8_t targetBoardID,float tempVal,ERROR_SUB errorSubSensor,
                                                    TEMP_CONTROL_STATE controlState,PIN_STATE floatSensorState,SYS_RESULT_PACK* resultPackPtr);


#endif



