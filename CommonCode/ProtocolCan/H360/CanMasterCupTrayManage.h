/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-22 09:20:50 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-12-25 10:22:06 +0800
************************************************************************************************/ 
#ifndef __CAN_MASTER_CUP_TRAY_MANAGE_H_
#define __CAN_MASTER_CUP_TRAY_MANAGE_H_
#include "CanMasterH360Base.h"

//杯栈管理模块电机
typedef enum INDEX_MOTOR_CUP_TRAY_MANAGE
{
    INDEX_MOTOR_CUP_TRAY_MANAGE_NEW_UPDOWN      = 0X00,//新杯栈升降电机    
    INDEX_MOTOR_CUP_TRAY_MANAGE_EMPTY_UPDOWN    = 0X01,//空杯栈升降电机
    INDEX_MOTOR_CUP_TRAY_MANAGE_PUSH            = 0X02,//推杆电机
    INDEX_MOTOR_CUP_TRAY_MANAGE_CLAMP           = 0X03,//夹手电机
}INDEX_MOTOR_CUP_TRAY_MANAGE;

//杯栈门状态
typedef enum CUP_TRAY_MANAGE_DOOR_STATE
{
    CUP_TRAY_MANAGE_DOOR_STATE_OPEN           = 0,//门打开
    CUP_TRAY_MANAGE_DOOR_STATE_WAIT_OPERAT    = 1,//门等待操作
    CUP_TRAY_MANAGE_DOOR_STATE_CLOSE          = 2,//门关闭
}CUP_TRAY_MANAGE_DOOR_STATE;

//垃圾桶选择
typedef enum GARBAGE_SELECT
{
    GARBAGE_SELECT_1    = 0X01,//垃圾桶1
    GARBAGE_SELECT_2    = 0X02,//垃圾桶2
    GARBAGE_SELECT_ALL  = 0X03,//同时选中两个垃圾桶
}GARBAGE_SELECT;

//杯栈数据包
typedef struct CUP_TRAY_MANAGE_MSG
{
    uint8_t newCupTrayCount;//新杯盘数量
    uint8_t emptyTrayCount;//空杯盘数量
    PIN_STATE newTraySensorState;//新杯盘栈上方传感器状态
    PIN_STATE testTraySensorState;//测试位杯盘传感器状态
    PIN_STATE garbage1SensorState;//垃圾桶1存在状态
    PIN_STATE garbage2SensorState;//垃圾桶2存在状态
}CUP_TRAY_MANAGE_MSG;

//杯栈垃圾桶状态
typedef enum STACK_GARBAGE_STATE
{
    STACK_GARBAGE_IDLE      = 0X00,//垃圾桶空闲
    STACK_GARBAGE_UP        = 0X01,//垃圾桶正在上升
    STACK_GARBAGE_DOWN      = 0X02,//垃圾桶正在下降
    STACK_GARBAGE_OVERLOAD  = 0X03,//垃圾桶过载
}STACK_GARBAGE_STATE;

//垃圾桶状态
typedef struct STACK_GARBAGE
{
    STACK_GARBAGE_STATE garbage1State;//垃圾桶1当前状态
    uint32_t garbage1RunningTimeCount;//垃圾桶1运行时间
    STACK_GARBAGE_STATE garbage2State;//垃圾桶2当前状态
    uint32_t garbage2RunningTimeCount;//垃圾桶2运行时间
}STACK_GARBAGE;


/*************************************发送指令等待ACK************************************************/
//模块内输入读取
void CanMasterCupTrayManageReadSingleWhileAck(uint8_t modInIdx,SYS_RESULT_PACK* resultPackPtr);
//模块内输出写入
void CanMasterCupTrayManageWriteSingleWhileAck(uint8_t modOutIdx,PIN_STATE pinState,SYS_RESULT_PACK* resultPackPtr);
//模块内电机复位
void CanMasterCupTrayManageResetMotorWhileAck(uint8_t modMotorIdx,SYS_RESULT_PACK* resultPackPtr);
//模块内电机走位
void CanMasterCupTrayManageRunStepsMotorWhileAck(uint8_t modMotorIdx,int32_t steps,SYS_RESULT_PACK* resultPackPtr);
//设置杯栈门状态
void CanMasterCupTrayManageSetDoorStateWhileAck(CUP_TRAY_MANAGE_DOOR_STATE doorState,SYS_RESULT_PACK* resultPackPtr);
//新杯盘栈初始化
void CanMasterCupTrayManageNewStackInitWhileAck(SYS_RESULT_PACK* resultPackPtr);
//空杯盘栈初始化
void CanMasterCupTrayManageEmptyStackInitWhileAck(SYS_RESULT_PACK* resultPackPtr);
//推手初始化
void CanMasterCupTrayManagePushInitWhileAck(SYS_RESULT_PACK* resultPackPtr);
//夹手初始化
void CanMasterCupTrayManageClampInitWhileAck(SYS_RESULT_PACK* resultPackPtr);
//垃圾桶下降
void CanMasterCupTrayManageGarbageDownUnlockWhileAck(GARBAGE_SELECT garbageNo,SYS_RESULT_PACK* resultPackPtr);
//垃圾桶上升
void CanMasterCupTrayManageGarbegeUpLockWhileAck(GARBAGE_SELECT garbageNo,SYS_RESULT_PACK* resultPackPtr);
//杯栈模块整体初始化
void CanMasterCupTrayManageInitAllWhileAck(SYS_RESULT_PACK* resultPackPtr);
//夹手释放
void CanMasterCupTrayManageClampOpenWhileAck(SYS_RESULT_PACK* resultPackPtr);
//夹手夹紧
void CanMasterCupTrayManageClampCloseWhileAck(SYS_RESULT_PACK* resultPackPtr);
//新杯盘栈上传一个新盘
void CanMasterCupTrayManageUploadNewTrayWhileAck(SYS_RESULT_PACK* resultPackPtr);
//空杯盘栈从测试区回收一个空盘
void CanMasterCupTrayManageRecycleEmptyTrayWhileAck(SYS_RESULT_PACK* resultPackPtr);
//推手推一个新盘到测试位
void CanMasterCupTrayManagePushNewTrayToTestWhileAck(SYS_RESULT_PACK* resultPackPtr);
//新杯盘栈上升指定层(门开到位状态)
void CanMasterCupTrayManage_NewStackUpSpecialLayerWithDoorOpenStateWhileAck(uint8_t layers,SYS_RESULT_PACK* resultPackPtr);
//新杯盘栈回零(门开到位状态)
void CanMasterCupTrayManageNewStackReturnZeroWhileAck(SYS_RESULT_PACK* resultPackPtr);
//空杯盘上升指定层(门开到位状态)
void CanMasterCupTrayManageEmptyStackUpSpecialLayerWithDoorOpenStateWhileAck(uint8_t layers,SYS_RESULT_PACK* resultPackPtr);
//空杯盘栈回零(门开到位状态)
void CanMasterCupTrayManageEmptyStackReturnZeroWhileAck(SYS_RESULT_PACK* resultPackPtr);
//获取杯栈信息集合
void CanMasterCupTrayManageReadMsgSetWhileAck(SYS_RESULT_PACK* resultPackPtr);

/*************************************等待指定指令完成返回************************************************/
//模块内输入读取
void CanMasterCupTrayManageWaitReadSingleResult(SYS_RESULT_PACK* resultPackPtr,PIN_STATE* pinStatePtr);
//模块内输出写入
void CanMasterCupTrayManageWaitWriteSingleResult(SYS_RESULT_PACK* resultPackPtr);
//模块内电机复位
void CanMasterCupTrayManageWaitResetMotorResult(SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr);
//模块内电机走位
void CanMasterCupTrayManageWaitRunStepsMotorResult(SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr);
//设置杯栈门状态
void CanMasterCupTrayManageWaitSetDoorStateResult(SYS_RESULT_PACK* resultPackPtr);
//新杯盘栈初始化
void CanMasterCupTrayManageWaitNewStackInitResult(SYS_RESULT_PACK* resultPackPtr,CUP_TRAY_MANAGE_MSG* msgSetPtr);
//空杯盘栈初始化
void CanMasterCupTrayManageWaitEmptyStackInitResult(SYS_RESULT_PACK* resultPackPtr,CUP_TRAY_MANAGE_MSG* msgSetPtr);
//推手初始化
void CanMasterCupTrayManageWaitPushInitResult(SYS_RESULT_PACK* resultPackPtr,CUP_TRAY_MANAGE_MSG* msgSetPtr);
//夹手初始化
void CanMasterCupTrayManageWaitClampInitResult(SYS_RESULT_PACK* resultPackPtr,CUP_TRAY_MANAGE_MSG* msgSetPtr);
//垃圾桶下降
void CanMasterCupTrayManageWaitGarbageDownUnlockResult(SYS_RESULT_PACK* resultPackPtr,CUP_TRAY_MANAGE_MSG* msgSetPtr);
//垃圾桶上升
void CanMasterCupTrayManageWaitGarbegeUpLockResult(SYS_RESULT_PACK* resultPackPtr,CUP_TRAY_MANAGE_MSG* msgSetPtr);
//杯栈模块整体初始化
void CanMasterCupTrayManageWaitInitAllResult(SYS_RESULT_PACK* resultPackPtr,CUP_TRAY_MANAGE_MSG* msgSetPtr);
//夹手释放
void CanMasterCupTrayManageWaitClampOpenResult(SYS_RESULT_PACK* resultPackPtr,CUP_TRAY_MANAGE_MSG* msgSetPtr);
//夹手夹紧
void CanMasterCupTrayManageWaitClampCloseResult(SYS_RESULT_PACK* resultPackPtr,CUP_TRAY_MANAGE_MSG* msgSetPtr);
//新杯盘栈上传一个新盘
void CanMasterCupTrayManageWaitUploadNewTrayResult(SYS_RESULT_PACK* resultPackPtr,CUP_TRAY_MANAGE_MSG* msgSetPtr);
//空杯盘栈从测试区回收一个空盘
void CanMasterCupTrayManageWaitRecycleEmptyTrayResult(SYS_RESULT_PACK* resultPackPtr,CUP_TRAY_MANAGE_MSG* msgSetPtr);
//推手推一个新盘到测试位
void CanMasterCupTrayManageWaitPushNewTrayToTestResult(SYS_RESULT_PACK* resultPackPtr,CUP_TRAY_MANAGE_MSG* msgSetPtr);
//新杯盘栈上升指定层(门开到位状态)
void CanMasterCupTrayManageWaitNewStackUpSpecialLayerWithDoorOpenStateResult(SYS_RESULT_PACK* resultPackPtr,CUP_TRAY_MANAGE_MSG* msgSetPtr);
//新杯盘栈回零(门开到位状态)
void CanMasterCupTrayManageWaitNewStackReturnZeroResult(SYS_RESULT_PACK* resultPackPtr,CUP_TRAY_MANAGE_MSG* msgSetPtr);
//空杯盘上升指定层(门开到位状态)
void CanMasterCupTrayManageWaitEmptyStackUpSpecialLayerWithDoorOpenStateResult(SYS_RESULT_PACK* resultPackPtr,CUP_TRAY_MANAGE_MSG* msgSetPtr);
//空杯盘栈回零(门开到位状态)
void CanMasterCupTrayManageWaitEmptyStackReturnZeroResult(SYS_RESULT_PACK* resultPackPtr,CUP_TRAY_MANAGE_MSG* msgSetPtr);
//获取杯栈信息集合
void CanMasterCupTrayManageWaitReadMsgSetResult(SYS_RESULT_PACK* resultPackPtr,CUP_TRAY_MANAGE_MSG* msgSetPtr);

/*************************************发送指令等待完成返回************************************************/
//模块内输入读取
void CanMasterCupTrayManageReadSingleWhileReturn(uint8_t modInIdx,SYS_RESULT_PACK* resultPackPtr,PIN_STATE* pinStatePtr);
//模块内输出写入
void CanMasterCupTrayManageWriteSingleWhileReturn(uint8_t modOutIdx,PIN_STATE pinState,SYS_RESULT_PACK* resultPackPtr);
//模块内电机复位
void CanMasterCupTrayManageResetMotorWhileReturn(uint8_t modMotorIdx,SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr);
//模块内电机走位
void CanMasterCupTrayManageRunStepsMotorWhileReturn(uint8_t modMotorIdx,int32_t steps,SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr);
//设置杯栈门状态
void CanMasterCupTrayManageSetDoorStateWhileReturn(CUP_TRAY_MANAGE_DOOR_STATE doorState,SYS_RESULT_PACK* resultPackPtr);
//新杯盘栈初始化
void CanMasterCupTrayManageNewStackInitWhileReturn(SYS_RESULT_PACK* resultPackPtr,CUP_TRAY_MANAGE_MSG* msgSetPtr);
//空杯盘栈初始化
void CanMasterCupTrayManageEmptyStackInitWhileReturn(SYS_RESULT_PACK* resultPackPtr,CUP_TRAY_MANAGE_MSG* msgSetPtr);
//推手初始化
void CanMasterCupTrayManagePushInitWhileReturn(SYS_RESULT_PACK* resultPackPtr,CUP_TRAY_MANAGE_MSG* msgSetPtr);
//夹手初始化
void CanMasterCupTrayManageClampInitWhileReturn(SYS_RESULT_PACK* resultPackPtr,CUP_TRAY_MANAGE_MSG* msgSetPtr);
//垃圾桶下降
void CanMasterCupTrayManageGarbageDownUnlockWhileReturn(GARBAGE_SELECT garbageNo,SYS_RESULT_PACK* resultPackPtr,CUP_TRAY_MANAGE_MSG* msgSetPtr);
//垃圾桶上升
void CanMasterCupTrayManageGarbegeUpLockWhileReturn(GARBAGE_SELECT garbageNo,SYS_RESULT_PACK* resultPackPtr,CUP_TRAY_MANAGE_MSG* msgSetPtr);
//杯栈模块整体初始化
void CanMasterCupTrayManageInitAllWhileReturn(SYS_RESULT_PACK* resultPackPtr,CUP_TRAY_MANAGE_MSG* msgSetPtr);
//夹手释放
void CanMasterCupTrayManageClampOpenWhileReturn(SYS_RESULT_PACK* resultPackPtr,CUP_TRAY_MANAGE_MSG* msgSetPtr);
//夹手夹紧
void CanMasterCupTrayManageClampCloseWhileReturn(SYS_RESULT_PACK* resultPackPtr,CUP_TRAY_MANAGE_MSG* msgSetPtr);
//新杯盘栈上传一个新盘
void CanMasterCupTrayManageUploadNewTrayWhileReturn(SYS_RESULT_PACK* resultPackPtr,CUP_TRAY_MANAGE_MSG* msgSetPtr);
//空杯盘栈从测试区回收一个空盘
void CanMasterCupTrayManageRecycleEmptyTrayWhileReturn(SYS_RESULT_PACK* resultPackPtr,CUP_TRAY_MANAGE_MSG* msgSetPtr);
//推手推一个新盘到测试位
void CanMasterCupTrayManagePushNewTrayToTestWhileReturn(SYS_RESULT_PACK* resultPackPtr,CUP_TRAY_MANAGE_MSG* msgSetPtr);
//新杯盘栈上升指定层(门开到位状态)
void CanMasterCupTrayManage_NewStackUpSpecialLayerWithDoorOpenStateWhileReturn(uint8_t layers,SYS_RESULT_PACK* resultPackPtr,CUP_TRAY_MANAGE_MSG* msgSetPtr);
//新杯盘栈回零(门开到位状态)
void CanMasterCupTrayManageNewStackReturnZeroWhileReturn(SYS_RESULT_PACK* resultPackPtr,CUP_TRAY_MANAGE_MSG* msgSetPtr);
//空杯盘上升指定层(门开到位状态)
void CanMasterCupTrayManageEmptyStackUpSpecialLayerWithDoorOpenStateWhileReturn(uint8_t layers,SYS_RESULT_PACK* resultPackPtr,CUP_TRAY_MANAGE_MSG* msgSetPtr);
//空杯盘栈回零(门开到位状态)
void CanMasterCupTrayManageEmptyStackReturnZeroWhileReturn(SYS_RESULT_PACK* resultPackPtr,CUP_TRAY_MANAGE_MSG* msgSetPtr);
//获取杯栈信息集合
void CanMasterCupTrayManageReadMsgSetWhileReturn(SYS_RESULT_PACK* resultPackPtr,CUP_TRAY_MANAGE_MSG* msgSetPtr);
























#endif













