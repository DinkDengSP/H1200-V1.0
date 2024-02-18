/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-09-11 13:33:37 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-10 11:21:10 +0800
************************************************************************************************/ 
#ifndef __PROTOCOL_CAN_RACK_MOVE_H_
#define __PROTOCOL_CAN_RACK_MOVE_H_
#include "ProtocolCanOumengInterBase.h"

//试管架移动单元电机定义
typedef enum STEP_MOTOR_RACK_MOVE
{
    STEP_MOTOR_RACK_MOVE_LATERALLY  = 0x00,//试管架横移电机
    STEP_MOTOR_RACK_MOVE_UPDOWN     = 0x01,//试管架升降电机
    STEP_MOTOR_RACK_MOVE_BELT       = 0x02,//试管架装载皮带电机
    STEP_MOTOR_RACK_MOVE_ROD        = 0x03,//试管架装载导杆电机
}STEP_MOTOR_RACK_MOVE;

//试管架移动单元传感器ID定义
typedef enum RACK_MOVE_SENSOR
{
    RACK_MOVE_SENSOR_MOTOR_LATERALLY_RESET      = 0X00,//横移电机原点
    RACK_MOVE_SENSOR_MOTOR_UPDOWN_RESET         = 0X01,//升降电机原点
    RACK_MOVE_SENSOR_RACK_LATERALLY_LEFT_LIMIT  = 0X02,//横移电机左限位
    RACK_MOVE_SENSOR_RACK_LATERALLY_RIGHT_LIMIT = 0X03,//横移电机右限位
    RACK_MOVE_SENSOR_RACK_LATERALLY_READY       = 0X04,//横移电机ready信号
    RACK_LOAD_SENSOR_MOTOR_LOAD_BELT_RESET      = 0X05,//试管架装载皮带原点传感器
    RACK_LOAD_SENSOR_MOTOR_LOAD_ROD_RESET       = 0X06,//试管架装载顶杆原点传感器
    RACK_LOAD_SENSOR_RACK_CHECK_SENSOR_FRONT    = 0X07,//输入试管架前检测光电
    RACK_LOAD_SENSOR_RACK_CHECK_SENSOR_END      = 0X08,//输入试管架后检测光电
    RACK_LOAD_SENSOR_TUBE_TYPE_CHECK_SENSOR     = 0X09,//试管类型检测光电
}RACK_MOVE_SENSOR;

//单字节对齐
#pragma pack(push,1)
//试管架移动单元状态集合
typedef struct RACK_MOVE_SENSOR_SET
{
    PIN_STATE motorLaterallyReset;       //横移电机原点
    PIN_STATE motorUpDownReset;          //升降电机原点
    PIN_STATE motorLaterallyLeftLimit;   //横移电机左限位
    PIN_STATE motorLaterallyRightLimit;  //横移电机右限位
    PIN_STATE motorLaterallyReadyLimit;  //横移电机ready信号
    PIN_STATE motorBeltReset;            //试管架装载皮带原点传感器
    PIN_STATE motorRodReset;             //试管架装载顶杆原点传感器
    PIN_STATE rackCheckFront;            //输入试管架前检测光电
    PIN_STATE rackCheckEnd;              //输入试管架后检测光电
    PIN_STATE rackTubeTypeCheck;         //试管类型检测光电
}RACK_MOVE_SENSOR_SET;
#pragma pack(pop) 

//当前试管架运输机构位置
typedef enum RACK_LATERALLY_POS
{
    RACK_LATERALLY_POS_RESET             = 0X00,//复位位置
    RACK_LATERALLY_POS_IDLE              = 0X01,//空闲位置
    RACK_LATERALLY_POS_RACK_STORE_1      = 0X02,//试管架仓1号位
    RACK_LATERALLY_POS_RACK_STORE_2      = 0X03,//
    RACK_LATERALLY_POS_RACK_STORE_3      = 0X04,//
    RACK_LATERALLY_POS_RACK_STORE_4      = 0X05,//
    RACK_LATERALLY_POS_RACK_STORE_5      = 0X06,//
    RACK_LATERALLY_POS_RACK_STORE_6      = 0X07,//
    RACK_LATERALLY_POS_RACK_STORE_7      = 0X08,//
    RACK_LATERALLY_POS_RACK_STORE_8      = 0X09,//
    RACK_LATERALLY_POS_RACK_STORE_9      = 0X0A,//
    RACK_LATERALLY_POS_RACK_STORE_10     = 0X0B,//
    RACK_LATERALLY_POS_RACK_STORE_11     = 0X0C,//
    RACK_LATERALLY_POS_RACK_STORE_12     = 0X0D,//
    RACK_LATERALLY_POS_RACK_STORE_13     = 0X0E,//
    RACK_LATERALLY_POS_RACK_STORE_14     = 0X0F,//
    RACK_LATERALLY_POS_RACK_STORE_15     = 0X10,//
    RACK_LATERALLY_POS_RACK_STORE_16     = 0X11,//
    RACK_LATERALLY_POS_RACK_STORE_17     = 0X12,//
    RACK_LATERALLY_POS_RACK_STORE_18     = 0X13,//
    RACK_LATERALLY_POS_RACK_STORE_19     = 0X14,//
    RACK_LATERALLY_POS_RACK_STORE_20     = 0X15,//
    RACK_LATERALLY_POS_RACK_STORE_21     = 0X16,//
    RACK_LATERALLY_POS_RACK_STORE_22     = 0X17,//
    RACK_LATERALLY_POS_RACK_STORE_23     = 0X18,//
    RACK_LATERALLY_POS_C21               = 0X19,//C21接口位置
    RACK_LATERALLY_POS_UNKNOW            = 0XFF,//未知位置
}RACK_LATERALLY_POS;

//当前试管架装载机构状态
typedef enum RACK_LOAD_STATE
{
    RACK_LOAD_STATE_RESET                   = 0X00,//复位位置
    RACK_LOAD_STATE_IDLE                    = 0X01,//空闲位置
    RACK_LOAD_STATE_LOAD_RACK_STORE         = 0X02,//从试管架仓加载试管的位置
    RACK_LOAD_STATE_PUSH_RACK_STORE         = 0X03,//往试管架仓卸载试管的位置
    RACK_LOAD_STATE_LOAD_RACK_C21           = 0X04,//从C21加载试管的位置
    RACK_LOAD_STATE_PUSH_RACK_C21           = 0X05,//往C21卸载试管的位置
    RACK_LOAD_STATE_BARSCAN                 = 0X06,//扫码位置
    RACK_LOAD_STATE_UNKNOW                  = 0XFF,//未知位置
}RACK_LOAD_STATE;

//单字节对齐
#pragma pack(push,1)
//试管架移动单元当前位置
typedef struct RACK_MOVE_POS
{
    RACK_LATERALLY_POS laterallyPos;
    RACK_LOAD_STATE loadState;
}RACK_MOVE_POS;
#pragma pack(pop) 

//试管类型
typedef enum RACK_TUBE_TYPE
{
    RACK_TUBE_TYPE_LOW      = 0X00,//矮类型试管
    RACK_TUBE_TYPE_HIGH     = 0X01,//高类型试管
}RACK_TUBE_TYPE;

//单字节对齐
#pragma pack(push,1)
//试管信息
typedef struct RACK_TUBE_MSG
{
    RACK_TUBE_TYPE tubeType;//试管类型
    uint8_t tubeLabel[8];//试管标签信息
    uint8_t tubeLableLength;//试管标签信息长度
    uint8_t tubeBarCode[32];//试管条码讯息
    uint8_t tubeBarCodeLength;//试管条码讯息长度
}RACK_TUBE_MSG;
#pragma pack(pop) 

//单字节对齐
#pragma pack(push,1)
//试管架讯息
typedef struct RACK_BAR_MSG
{
    uint32_t rackID;
    RACK_TUBE_MSG tube1Msg;
    RACK_TUBE_MSG tube2Msg;
    RACK_TUBE_MSG tube3Msg;
    RACK_TUBE_MSG tube4Msg;
    RACK_TUBE_MSG tube5Msg;
    RACK_TUBE_MSG tube6Msg;
    RACK_TUBE_MSG tube7Msg;
    RACK_TUBE_MSG tube8Msg;
    RACK_TUBE_MSG tube9Msg;
    RACK_TUBE_MSG tube10Msg;
}RACK_BAR_MSG;
#pragma pack(pop)

/*********************************************************发送指令等待ACK********************************************************************/
//试管架移动单元电机复位
void CanMasterRackMoveSendMotorResetWhileAck(STEP_MOTOR_RACK_MOVE motorNo,SYS_RESULT_PACK* resultPackPtr);
//试管架移动单元电机走步数
void CanMasterRackMoveSendMotorRunStepWhileAck(STEP_MOTOR_RACK_MOVE motorNo,int32_t steps,SYS_RESULT_PACK* resultPackPtr);
//试管架移动单元电机状态读取
void CanMasterRackMoveSendMotorStateReadWhileAck(STEP_MOTOR_RACK_MOVE motorNo,SYS_RESULT_PACK* resultPackPtr);
//试管架移动单元传感器信号单个读取
void CanMasterRackMoveSendSensorReadSingleWhileAck(RACK_MOVE_SENSOR sensorNo,SYS_RESULT_PACK* resultPackPtr);
//试管架移动单元传感器信号批量读取
void CanMasterRackMoveSendSensorReadAllWhileAck(SYS_RESULT_PACK* resultPackPtr);
//获取试管架移动单元当前位置
void CanMasterRackMoveSendGetCurrentPosWhileAck(SYS_RESULT_PACK* resultPackPtr);
//试管架移动单元复位
void CanMasterRackMoveSendModuleResetWhileAck(SYS_RESULT_PACK* resultPackPtr);
//试管架移动单元到空闲位置
void CanMasterRackMoveSendModuleToIdleWhileAck(SYS_RESULT_PACK* resultPackPtr);
//横移电机移动到指定试管架仓位置
void CanMasterRackMoveSendLaterallyToStoreByPosWhileAck(RACK_STORE_HOUSE rackHousePos,SYS_RESULT_PACK* resultPackPtr);
//横移电机移动到C21接口位置
void CanMasterRackMoveSendLaterallyToC21WhileAck(SYS_RESULT_PACK* resultPackPtr);
//装载机构移动到装载试管架仓位置
void CanMasterRackMoveSendRunToLoadRackStorePosWhileAck(RACK_STORE_HOUSE rackHousePos,SYS_RESULT_PACK* resultPackPtr);
//装载机构移动到卸载试管架仓位置
void CanMasterRackMoveSendRunToUnLoadRackStorePosWhileAck(RACK_STORE_HOUSE rackHousePos,SYS_RESULT_PACK* resultPackPtr);
//装载机构移动到装载C21位置
void CanMasterRackMoveSendRunToLoadRackC21PosWhileAck(SYS_RESULT_PACK* resultPackPtr);
//装载机构移动到卸载C21位置
void CanMasterRackMoveSendRunToUnLoadRackC21PosWhileAck(SYS_RESULT_PACK* resultPackPtr);
//在当前位置执行试管架仓样本架装载
void CanMasterRackMoveSendLoadInRackFormStoreCurrentWhileAck(FunctionalState barScanSet,SYS_RESULT_PACK* resultPackPtr);
//在当前位置执行试管架仓样本架放入
void CanMasterRackMoveSendPushOutRackToStoreCurrentWhileAck(SYS_RESULT_PACK* resultPackPtr);
//在当前位置执行C21样本架装载
void CanMasterRackMoveSendLoadInRackFormC21CurrentWhileAck(SYS_RESULT_PACK* resultPackPtr);
//在当前位置执行C21样本架放入
void CanMasterRackMoveSendPushOutRackToC21CurrentWhileAck(SYS_RESULT_PACK* resultPackPtr);
//集成指令,从试管架仓中装载一个试管架,参数为仓号和是否扫码
void CanMasterRackMoveSendLoadInRackFormStoreByPosWhileAck(RACK_STORE_HOUSE rackHousePos,FunctionalState barScanSet,SYS_RESULT_PACK* resultPackPtr);
//集成指令,往试管架仓中放入一个试管架,参数为仓号
void CanMasterRackMoveSendPushOutRackToStoreByPosWhileAck(RACK_STORE_HOUSE rackHousePos,SYS_RESULT_PACK* resultPackPtr);
//集成指令,从C21接口装载一个试管架
void CanMasterRackMoveSendLoadInRackFormC21WhileAck(SYS_RESULT_PACK* resultPackPtr);
//集成指令,往C21接口放入一个试管架
void CanMasterRackMoveSendPushOutRackToC21WhileAck(SYS_RESULT_PACK* resultPackPtr);
//集成指令,从试管仓中取出一个试管架放入另一个试管仓中
void CanMasterRackMoveSendLoadInRackFormStoreByPosToStoreByPosWhileAck(RACK_STORE_HOUSE rackSrcHousePos,RACK_STORE_HOUSE rackTargetHousePos,
                                                                                FunctionalState barScanSet,SYS_RESULT_PACK* resultPackPtr);
//集成指令,从试管仓中取出一个试管架放入C21中
void CanMasterRackMoveSendLoadInRackFormStoreByPosToC21WhileAck(RACK_STORE_HOUSE rackSrcHousePos,FunctionalState barScanSet,SYS_RESULT_PACK* resultPackPtr);
//从C21中取出一个试管架放入试管架仓中
void CanMasterRackMoveSendLoadInRackFormC21ToRackStoreWhileAck(RACK_STORE_HOUSE rackTargetHousePos,SYS_RESULT_PACK* resultPackPtr);
//C21校准启动
void CanMasterRackMoveC21AdjustStartWhileAck(SYS_RESULT_PACK* resultPackPtr);
//C21校准走步数
void CanMasterRackMoveC21AdjustRunWhileAck(int32_t targetCoordinate,SYS_RESULT_PACK* resultPackPtr);
//C21校准完成
void CanMasterRackMoveC21AdjustEndWhileAck(int32_t targetCoordinate,SYS_RESULT_PACK* resultPackPtr);


/*********************************************************等待指定指令完成返回********************************************************************/
//试管架移动单元电机复位
void CanMasterRackMoveWaitMotorResetResult(SYS_RESULT_PACK* resultBasePack,CAN_MASTER_SM_STATE* motorStatePtr);
//试管架移动单元电机走步数
void CanMasterRackMoveWaitMotorRunStepResult(SYS_RESULT_PACK* resultBasePack,CAN_MASTER_SM_STATE* motorStatePtr);
//试管架移动单元电机状态读取
void CanMasterRackMoveWaitMotorStateReadResult(SYS_RESULT_PACK* resultBasePack,CAN_MASTER_SM_STATE* motorStatePtr);
//试管架移动单元传感器信号读取
void CanMasterRackMoveWaitSensorReadSingleResult(SYS_RESULT_PACK* resultBasePack,PIN_STATE* sensorStatePtr);
//试管架移动单元传感器信号批量读取
void CanMasterRackMoveWaitSensorReadAllResult(SYS_RESULT_PACK* resultBasePack,RACK_MOVE_SENSOR_SET* sensorSetPtr);
//获取试管架移动单元当前位置
void CanMasterRackMoveWaitGetCurrentPosResult(SYS_RESULT_PACK* resultBasePack,RACK_MOVE_POS* rackMovePosPtr);
//试管架移动单元复位
void CanMasterRackMoveWaitModuleResetResult(SYS_RESULT_PACK* resultBasePack,RACK_MOVE_SENSOR_SET* sensorSetPtr);
//试管架移动单元到空闲位置
void CanMasterRackMoveWaitModuleToIdleResult(SYS_RESULT_PACK* resultBasePack,RACK_MOVE_SENSOR_SET* sensorSetPtr);
//横移电机移动到指定试管架仓位置
void CanMasterRackMoveWaitLaterallyToStoreByPosResult(SYS_RESULT_PACK* resultBasePack,RACK_MOVE_POS* rackMovePosPtr);
//横移电机移动到C21接口位置
void CanMasterRackMoveWaitLaterallyToC21Result(SYS_RESULT_PACK* resultBasePack,RACK_MOVE_POS* rackMovePosPtr);
//装载机构移动到装载试管架仓位置
void CanMasterRackMoveWaitRunToLoadRackStorePosResult(SYS_RESULT_PACK* resultBasePack,RACK_MOVE_POS* rackMovePosPtr);
//装载机构移动到卸载试管架仓位置
void CanMasterRackMoveWaitRunToUnLoadRackStorePosResult(SYS_RESULT_PACK* resultBasePack,RACK_MOVE_POS* rackMovePosPtr);
//装载机构移动到装载C21位置
void CanMasterRackMoveWaitRunToLoadRackC21PosResult(SYS_RESULT_PACK* resultBasePack,RACK_MOVE_POS* rackMovePosPtr);
//装载机构移动到卸载C21位置
void CanMasterRackMoveWaitRunToUnLoadRackC21PosResult(SYS_RESULT_PACK* resultBasePack,RACK_MOVE_POS* rackMovePosPtr);
//在当前位置执行试管架仓样本架装载
void CanMasterRackMoveWaitLoadInRackFormStoreCurrentResult(SYS_RESULT_PACK* resultBasePack,RACK_BAR_MSG* rackBarMsgPtr,
                                                                    RACK_MOVE_SENSOR_SET* sensorSetPtr);
//在当前位置执行试管架仓样本架放入
void CanMasterRackMoveWaitPushOutRackToStoreCurrentResult(SYS_RESULT_PACK* resultBasePack,RACK_MOVE_SENSOR_SET* sensorSetPtr);
//在当前位置执行C21样本架装载
void CanMasterRackMoveWaitLoadInRackFormC21CurrentResult(SYS_RESULT_PACK* resultBasePack,RACK_MOVE_SENSOR_SET* sensorSetPtr);
//在当前位置执行C21样本架放入
void CanMasterRackMoveWaitPushOutRackToC21CurrentResult(SYS_RESULT_PACK* resultBasePack,RACK_MOVE_SENSOR_SET* sensorSetPtr);

//集成指令,从试管架仓中装载一个试管架,参数为仓号和是否扫码
void CanMasterRackMoveWaitLoadInRackFormStoreByPosResult(SYS_RESULT_PACK* resultBasePack,RACK_BAR_MSG* rackBarMsgPtr,
                                                                    RACK_MOVE_SENSOR_SET* sensorSetPtr);
//集成指令,往试管架仓中放入一个试管架,参数为仓号
void CanMasterRackMoveWaitPushOutRackToStoreByPosResult(SYS_RESULT_PACK* resultBasePack,RACK_MOVE_SENSOR_SET* sensorSetPtr);
//集成指令,从C21接口装载一个试管架
void CanMasterRackMoveWaitLoadInRackFormC21Result(SYS_RESULT_PACK* resultBasePack,RACK_MOVE_SENSOR_SET* sensorSetPtr);
//集成指令,往C21接口放入一个试管架
void CanMasterRackMoveWaitPushOutRackToC21Result(SYS_RESULT_PACK* resultBasePack,RACK_MOVE_SENSOR_SET* sensorSetPtr);
//集成指令,从试管仓中取出一个试管架放入另一个试管仓中
void CanMasterRackMoveWaitLoadInRackFormStoreByPosToStoreByPosResult(SYS_RESULT_PACK* resultBasePack,RACK_BAR_MSG* rackBarMsgPtr,
                                                                    RACK_MOVE_SENSOR_SET* sensorSetPtr);
//集成指令,从试管仓中取出一个试管架放入C21中
void CanMasterRackMoveWaitLoadInRackFormStoreByPosToC21Result(SYS_RESULT_PACK* resultBasePack,RACK_BAR_MSG* rackBarMsgPtr,
                                                                    RACK_MOVE_SENSOR_SET* sensorSetPtr);
//从C21中取出一个试管架放入试管架仓中
void CanMasterRackMoveWaitLoadInRackFormC21ToRackStoreResult(SYS_RESULT_PACK* resultBasePack,RACK_MOVE_SENSOR_SET* sensorSetPtr);
//C21校准启动
void CanMasterRackMoveWaitC21AdjustStartResult(SYS_RESULT_PACK* resultBasePack);
//C21校准走步数
void CanMasterRackMoveWaitC21AdjustRunResult(SYS_RESULT_PACK* resultBasePack);
//C21校准完成
void CanMasterRackMoveWaitC21AdjustEndResult(SYS_RESULT_PACK* resultBasePack);


/*********************************************************发送指令等待完成返回********************************************************************/
//试管架移动单元电机复位
void CanMasterRackMoveSendMotorResetWhileReturn(STEP_MOTOR_RACK_MOVE motorNo,SYS_RESULT_PACK* resultBasePack,CAN_MASTER_SM_STATE* motorStatePtr);
//试管架移动单元电机走步数
void CanMasterRackMoveSendMotorRunStepWhileReturn(STEP_MOTOR_RACK_MOVE motorNo,int32_t steps,SYS_RESULT_PACK* resultBasePack,CAN_MASTER_SM_STATE* motorStatePtr);
//试管架移动单元电机状态读取
void CanMasterRackMoveSendMotorStateReadWhileReturn(STEP_MOTOR_RACK_MOVE motorNo,SYS_RESULT_PACK* resultBasePack,CAN_MASTER_SM_STATE* motorStatePtr);
//试管架移动单元传感器信号单个读取
void CanMasterRackMoveSendSensorReadSingleWhileReturn(RACK_MOVE_SENSOR sensorNo,SYS_RESULT_PACK* resultBasePack,PIN_STATE* sensorStatePtr);
//试管架移动单元传感器信号批量读取
void CanMasterRackMoveSendSensorReadAllWhileReturn(SYS_RESULT_PACK* resultBasePack,RACK_MOVE_SENSOR_SET* sensorSetPtr);
//获取试管架移动单元当前位置
void CanMasterRackMoveSendGetCurrentPosWhileReturn(SYS_RESULT_PACK* resultBasePack,RACK_MOVE_POS* rackMovePosPtr);
//试管架移动单元复位
void CanMasterRackMoveSendModuleResetWhileReturn(SYS_RESULT_PACK* resultBasePack,RACK_MOVE_SENSOR_SET* sensorSetPtr);
//试管架移动单元到空闲位置
void CanMasterRackMoveSendModuleToIdleWhileReturn(SYS_RESULT_PACK* resultBasePack,RACK_MOVE_SENSOR_SET* sensorSetPtr);
//横移电机移动到指定试管架仓位置
void CanMasterRackMoveSendLaterallyToStoreByPosWhileReturn(RACK_STORE_HOUSE rackHousePos,SYS_RESULT_PACK* resultBasePack,RACK_MOVE_POS* rackMovePosPtr);
//横移电机移动到C21接口位置
void CanMasterRackMoveSendLaterallyToC21WhileReturn(SYS_RESULT_PACK* resultBasePack,RACK_MOVE_POS* rackMovePosPtr);
//装载机构移动到装载试管架仓位置
void CanMasterRackMoveSendRunToLoadRackStorePosWhileReturn(RACK_STORE_HOUSE rackHousePos,SYS_RESULT_PACK* resultBasePack,RACK_MOVE_POS* rackMovePosPtr);
//装载机构移动到卸载试管架仓位置
void CanMasterRackMoveSendRunToUnLoadRackStorePosWhileReturn(RACK_STORE_HOUSE rackHousePos,SYS_RESULT_PACK* resultBasePack,RACK_MOVE_POS* rackMovePosPtr);
//装载机构移动到装载C21位置
void CanMasterRackMoveSendRunToLoadRackC21PosWhileReturn(SYS_RESULT_PACK* resultBasePack,RACK_MOVE_POS* rackMovePosPtr);
//装载机构移动到卸载C21位置
void CanMasterRackMoveSendRunToUnLoadRackC21PosWhileReturn(SYS_RESULT_PACK* resultBasePack,RACK_MOVE_POS* rackMovePosPtr);
//在当前位置执行试管架仓样本架装载
void CanMasterRackMoveSendLoadInRackFormStoreCurrentWhileReturn(FunctionalState barScanSet,SYS_RESULT_PACK* resultBasePack,RACK_BAR_MSG* rackBarMsgPtr,
                                                                    RACK_MOVE_SENSOR_SET* sensorSetPtr);
//在当前位置执行试管架仓样本架放入
void CanMasterRackMoveSendPushOutRackToStoreCurrentWhileReturn(SYS_RESULT_PACK* resultBasePack,RACK_MOVE_SENSOR_SET* sensorSetPtr);
//在当前位置执行C21样本架装载
void CanMasterRackMoveSendLoadInRackFormC21CurrentWhileReturn(SYS_RESULT_PACK* resultBasePack,RACK_MOVE_SENSOR_SET* sensorSetPtr);
//在当前位置执行C21样本架放入
void CanMasterRackMoveSendPushOutRackToC21CurrentWhileReturn(SYS_RESULT_PACK* resultBasePack,RACK_MOVE_SENSOR_SET* sensorSetPtr);
//集成指令,从试管架仓中装载一个试管架,参数为仓号和是否扫码
void CanMasterRackMoveSendLoadInRackFormStoreByPosWhileReturn(RACK_STORE_HOUSE rackHousePos,FunctionalState barScanSet,SYS_RESULT_PACK* resultBasePack,
                                                                    RACK_BAR_MSG* rackBarMsgPtr,RACK_MOVE_SENSOR_SET* sensorSetPtr);
//集成指令,往试管架仓中放入一个试管架,参数为仓号
void CanMasterRackMoveSendPushOutRackToStoreByPosWhileReturn(RACK_STORE_HOUSE rackHousePos,SYS_RESULT_PACK* resultBasePack,RACK_MOVE_SENSOR_SET* sensorSetPtr);
//集成指令,从C21接口装载一个试管架
void CanMasterRackMoveSendLoadInRackFormC21WhileReturn(SYS_RESULT_PACK* resultBasePack,RACK_MOVE_SENSOR_SET* sensorSetPtr);
//集成指令,往C21接口放入一个试管架
void CanMasterRackMoveSendPushOutRackToC21WhileReturn(SYS_RESULT_PACK* resultBasePack,RACK_MOVE_SENSOR_SET* sensorSetPtr);
//集成指令,从试管仓中取出一个试管架放入另一个试管仓中
void CanMasterRackMoveSendLoadInRackFormStoreByPosToStoreByPosWhileReturn(RACK_STORE_HOUSE rackSrcHousePos,RACK_STORE_HOUSE rackTargetHousePos,
                                                                                FunctionalState barScanSet,SYS_RESULT_PACK* resultBasePack,RACK_BAR_MSG* rackBarMsgPtr,
                                                                                RACK_MOVE_SENSOR_SET* sensorSetPtr);
//集成指令,从试管仓中取出一个试管架放入C21中
void CanMasterRackMoveSendLoadInRackFormStoreByPosToC21WhileReturn(RACK_STORE_HOUSE rackSrcHousePos,FunctionalState barScanSet,SYS_RESULT_PACK* resultBasePack,RACK_BAR_MSG* rackBarMsgPtr,
                                                                    RACK_MOVE_SENSOR_SET* sensorSetPtr);
//从C21中取出一个试管架放入试管架仓中
void CanMasterRackMoveSendLoadInRackFormC21ToRackStoreWhileReturn(RACK_STORE_HOUSE rackTargetHousePos,SYS_RESULT_PACK* resultBasePack,RACK_MOVE_SENSOR_SET* sensorSetPtr);
//C21校准启动
void CanMasterRackMoveC21AdjustStartWhileReturn(SYS_RESULT_PACK* resultBasePack);
//C21校准走步数
void CanMasterRackMoveC21AdjustRunWhileReturn(int32_t targetCoordinate,SYS_RESULT_PACK* resultBasePack);
//C21校准完成
void CanMasterRackMoveC21AdjustEndWhileReturn(int32_t targetCoordinate,SYS_RESULT_PACK* resultBasePack);





#endif



