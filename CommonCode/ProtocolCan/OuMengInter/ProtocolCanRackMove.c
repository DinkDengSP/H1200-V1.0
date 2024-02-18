/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-09-11 13:33:37 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-10 12:07:51 +0800
************************************************************************************************/ 
#include "ProtocolCanRackMove.h"
#include "ProtocolCanOumengInterBase.h"
#include "CanMasterStepMotor.h"
#include "DriverConfigSystem.h"
#include SYS_LOG_INCLUDE_FILE

//模块号和板号
#define MODULE_NO_CAN_MASTER_RACK_MOVE             0X05
#define BOARD_ID_CAN_MASTER_RACK_MOVE              0X01

//通道配置数组
//参数分别是 指令  是否需要结果  超时时间  期望返回最小参数长度 附带参数长度
//附带参数长度为不定长的时候,指示参数长度为0,由上级调用者手动申请内存
static const CAN_PROT_CMD_CONFIG canRackMoveMasterCmdConfigArray[] = {
    {CAN_MASTER_CMD_RACK_MOVE_MOTOR_RESET              ,    ENABLE,     60000,  12,  1   },//试管架移动单元电机复位
    {CAN_MASTER_CMD_RACK_MOVE_MOTOR_RUN_STEP           ,    ENABLE,     60000,  12,  5   },//试管架移动单元电机走步数
    {CAN_MASTER_CMD_RACK_MOVE_MOTOR_STATE_READ         ,    ENABLE,     60000,  12,  1   },//试管架移动单元电机状态读取
    {CAN_MASTER_CMD_RACK_MOVE_SENSOR_READ_SINGLE       ,    ENABLE,     60000,  1,   1   },//试管架移动单元传感器信号单个读取
    {CAN_MASTER_CMD_RACK_MOVE_SENSOR_READ_ALL          ,    ENABLE,     60000,  10,  0   },//试管架移动单元传感器信号批量读取
    {CAN_MASTER_CMD_RACK_MOVE_GET_CUR_POS              ,    ENABLE,     60000,  2,   0   },//获取试管架移动单元当前位置
    {CAN_MASTER_CMD_RACK_MOVE_MODULE_RESET             ,    ENABLE,     60000,  10,  0   },//试管架移动单元复位
    {CAN_MASTER_CMD_RACK_MOVE_TO_IDLE                  ,    ENABLE,     60000,  10,  0   },//试管架移动单元到空闲位置
    {CAN_MASTER_CMD_RACK_MOVE_LATERALLY_TO_STORE       ,    ENABLE,     60000,  2,   1   },//横移电机移动到指定试管架仓位置
    {CAN_MASTER_CMD_RACK_MOVE_LATERALLY_TO_C21         ,    ENABLE,     60000,  2,   0   },//横移电机移动到C21接口位置
    {CAN_MASTER_CMD_RACK_MOVE_TO_STORE_LOAD_POS        ,    ENABLE,     60000,  2,   1   },//装载机构移动到装载试管架仓位置
    {CAN_MASTER_CMD_RACK_MOVE_TO_STORE_UNLOAD_POS      ,    ENABLE,     60000,  2,   1   },//装载机构移动到卸载试管架仓位置
    {CAN_MASTER_CMD_RACK_MOVE_TO_C21_LOAD_POS          ,    ENABLE,     60000,  2,   0   },//装载机构移动到装载C21位置
    {CAN_MASTER_CMD_RACK_MOVE_TO_C21_UNLOAD_POS        ,    ENABLE,     60000,  2,   0   },//装载机构移动到卸载C21位置
    {CAN_MASTER_CMD_RACK_MOVE_LOAD_STORE_RACK_CUR      ,    ENABLE,     60000,  445, 2   },//在当前位置执行试管架仓样本架装载
    {CAN_MASTER_CMD_RACK_MOVE_UNLOAD_STORE_RACK_CUR    ,    ENABLE,     60000,  10,  0   },//在当前位置执行试管架仓样本架放入
    {CAN_MASTER_CMD_RACK_MOVE_LOAD_C21_RACK_CUR        ,    ENABLE,     60000,  10,  0   },//在当前位置执行C21样本架装载
    {CAN_MASTER_CMD_RACK_MOVE_UNLOAD_C21_RACK_CUR      ,    ENABLE,     60000,  10,  0   },//在当前位置执行C21样本架放入
    {CAN_MASTER_CMD_RACK_MOVE_LOAD_RACK_STORE_BY_POS   ,    ENABLE,     60000,  445, 2   },//集成指令,从试管架仓中装载一个试管架
    {CAN_MASTER_CMD_RACK_MOVE_UNLOAD_RACK_STORE_BY_POS ,    ENABLE,     60000,  10,  1   },//集成指令,往试管架仓中放入一个试管架
    {CAN_MASTER_CMD_RACK_MOVE_LOAD_RACK_C21            ,    ENABLE,     60000,  10,  0   },//集成指令,从C21接口装载一个试管架
    {CAN_MASTER_CMD_RACK_MOVE_UNLOAD_RACK_C21          ,    ENABLE,     60000,  10,  0   },//集成指令,往C21接口放入一个试管架
    {CAN_MASTER_CMD_RACK_MOVE_LOAD_RACK_STORE_TO_STORE ,    ENABLE,     60000,  445, 3   },//集成指令,从试管仓中取出一个试管架放入另一个试管仓中
    {CAN_MASTER_CMD_RACK_MOVE_LOAD_RACK_STORE_TO_C21   ,    ENABLE,     60000,  445, 2   },//集成指令,从试管仓中取出一个试管架放入C21中
    {CAN_MASTER_CMD_RACK_MOVE_LOAD_RACK_C21_TO_STORE   ,    ENABLE,     60000,  10,  1   },//从C21中取出一个试管架放入试管架仓中
    {CAN_MASTER_CMD_RACK_MOVE_TO_C21_ADJUST_START      ,    ENABLE,     60000,   0,  0   },//移动到C21校准起始点
    {CAN_MASTER_CMD_RACK_MOVE_TO_C21_ADJUST_RUN        ,    ENABLE,     60000,   0,  4   },//C21校准移动
    {CAN_MASTER_CMD_RACK_MOVE_TO_C21_ADJUST_END        ,    ENABLE,     60000,   0,  4   },//C21校准完成
};




/*********************************************************发送指令等待ACK********************************************************************/
//试管架移动单元电机复位
void CanMasterRackMoveSendMotorResetWhileAck(STEP_MOTOR_RACK_MOVE motorNo,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_ACT_RACK_MOVE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_RACK_MOVE_MOTOR_RESET;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(MODULE_NO_CAN_MASTER_RACK_MOVE,BOARD_ID_CAN_MASTER_RACK_MOVE,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode != PROT_RESULT_FAIL)
    {
        CAN_PROT_LOG("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canRackMoveMasterCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canRackMoveMasterCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        CAN_PROT_LOG("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
		return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(MODULE_NO_CAN_MASTER_RACK_MOVE,BOARD_ID_CAN_MASTER_RACK_MOVE,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)motorNo;
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//试管架移动单元电机走步数
void CanMasterRackMoveSendMotorRunStepWhileAck(STEP_MOTOR_RACK_MOVE motorNo,int32_t steps,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_ACT_RACK_MOVE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_RACK_MOVE_MOTOR_RUN_STEP;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(MODULE_NO_CAN_MASTER_RACK_MOVE,BOARD_ID_CAN_MASTER_RACK_MOVE,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode != PROT_RESULT_FAIL)
    {
        CAN_PROT_LOG("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canRackMoveMasterCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canRackMoveMasterCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        CAN_PROT_LOG("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
         //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
		return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(MODULE_NO_CAN_MASTER_RACK_MOVE,BOARD_ID_CAN_MASTER_RACK_MOVE,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)motorNo;
	ProtUtilWriteUint32ToBuffer(steps,canCmdSendPtr->cmdBufferPtr+1);
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//试管架移动单元电机状态读取
void CanMasterRackMoveSendMotorStateReadWhileAck(STEP_MOTOR_RACK_MOVE motorNo,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_ACT_RACK_MOVE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_RACK_MOVE_MOTOR_STATE_READ;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(MODULE_NO_CAN_MASTER_RACK_MOVE,BOARD_ID_CAN_MASTER_RACK_MOVE,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode != PROT_RESULT_FAIL)
    {
        CAN_PROT_LOG("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canRackMoveMasterCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canRackMoveMasterCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        CAN_PROT_LOG("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
         //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
		return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(MODULE_NO_CAN_MASTER_RACK_MOVE,BOARD_ID_CAN_MASTER_RACK_MOVE,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)motorNo;
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//试管架移动单元传感器信号单个读取
void CanMasterRackMoveSendSensorReadSingleWhileAck(RACK_MOVE_SENSOR sensorNo,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_ACT_RACK_MOVE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_RACK_MOVE_SENSOR_READ_SINGLE;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(MODULE_NO_CAN_MASTER_RACK_MOVE,BOARD_ID_CAN_MASTER_RACK_MOVE,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode != PROT_RESULT_FAIL)
    {
        CAN_PROT_LOG("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canRackMoveMasterCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canRackMoveMasterCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        CAN_PROT_LOG("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
		return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(MODULE_NO_CAN_MASTER_RACK_MOVE,BOARD_ID_CAN_MASTER_RACK_MOVE,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)sensorNo;
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//试管架移动单元传感器信号批量读取
void CanMasterRackMoveSendSensorReadAllWhileAck(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_ACT_RACK_MOVE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_RACK_MOVE_SENSOR_READ_ALL;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(MODULE_NO_CAN_MASTER_RACK_MOVE,BOARD_ID_CAN_MASTER_RACK_MOVE,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode != PROT_RESULT_FAIL)
    {
        CAN_PROT_LOG("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canRackMoveMasterCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canRackMoveMasterCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        CAN_PROT_LOG("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
         //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
		return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(MODULE_NO_CAN_MASTER_RACK_MOVE,BOARD_ID_CAN_MASTER_RACK_MOVE,singalChannel,channelCmdCode,canCmdConfigPtr);
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//获取试管架移动单元当前位置
void CanMasterRackMoveSendGetCurrentPosWhileAck(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_ACT_RACK_MOVE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_RACK_MOVE_GET_CUR_POS;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(MODULE_NO_CAN_MASTER_RACK_MOVE,BOARD_ID_CAN_MASTER_RACK_MOVE,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode != PROT_RESULT_FAIL)
    {
        CAN_PROT_LOG("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canRackMoveMasterCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canRackMoveMasterCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        CAN_PROT_LOG("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
		return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(MODULE_NO_CAN_MASTER_RACK_MOVE,BOARD_ID_CAN_MASTER_RACK_MOVE,singalChannel,channelCmdCode,canCmdConfigPtr);
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//试管架移动单元复位
void CanMasterRackMoveSendModuleResetWhileAck(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_ACT_RACK_MOVE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_RACK_MOVE_MODULE_RESET;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(MODULE_NO_CAN_MASTER_RACK_MOVE,BOARD_ID_CAN_MASTER_RACK_MOVE,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode != PROT_RESULT_FAIL)
    {
        CAN_PROT_LOG("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canRackMoveMasterCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canRackMoveMasterCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        CAN_PROT_LOG("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
		return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(MODULE_NO_CAN_MASTER_RACK_MOVE,BOARD_ID_CAN_MASTER_RACK_MOVE,singalChannel,channelCmdCode,canCmdConfigPtr);
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//试管架移动单元到空闲位置
void CanMasterRackMoveSendModuleToIdleWhileAck(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_ACT_RACK_MOVE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_RACK_MOVE_TO_IDLE;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(MODULE_NO_CAN_MASTER_RACK_MOVE,BOARD_ID_CAN_MASTER_RACK_MOVE,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode != PROT_RESULT_FAIL)
    {
        CAN_PROT_LOG("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canRackMoveMasterCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canRackMoveMasterCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        CAN_PROT_LOG("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
		return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(MODULE_NO_CAN_MASTER_RACK_MOVE,BOARD_ID_CAN_MASTER_RACK_MOVE,singalChannel,channelCmdCode,canCmdConfigPtr);
   //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}
;
//横移电机移动到指定试管架仓位置
void CanMasterRackMoveSendLaterallyToStoreByPosWhileAck(RACK_STORE_HOUSE rackHousePos,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_ACT_RACK_MOVE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_RACK_MOVE_LATERALLY_TO_STORE;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(MODULE_NO_CAN_MASTER_RACK_MOVE,BOARD_ID_CAN_MASTER_RACK_MOVE,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode != PROT_RESULT_FAIL)
    {
        CAN_PROT_LOG("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canRackMoveMasterCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canRackMoveMasterCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        CAN_PROT_LOG("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
		return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(MODULE_NO_CAN_MASTER_RACK_MOVE,BOARD_ID_CAN_MASTER_RACK_MOVE,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)rackHousePos;
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//横移电机移动到C21接口位置
void CanMasterRackMoveSendLaterallyToC21WhileAck(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_ACT_RACK_MOVE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_RACK_MOVE_LATERALLY_TO_C21;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(MODULE_NO_CAN_MASTER_RACK_MOVE,BOARD_ID_CAN_MASTER_RACK_MOVE,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode != PROT_RESULT_FAIL)
    {
        CAN_PROT_LOG("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canRackMoveMasterCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canRackMoveMasterCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        CAN_PROT_LOG("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
		return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(MODULE_NO_CAN_MASTER_RACK_MOVE,BOARD_ID_CAN_MASTER_RACK_MOVE,singalChannel,channelCmdCode,canCmdConfigPtr);
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//装载机构移动到装载试管架仓位置
void CanMasterRackMoveSendRunToLoadRackStorePosWhileAck(RACK_STORE_HOUSE rackHousePos,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_ACT_RACK_MOVE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_RACK_MOVE_TO_STORE_LOAD_POS;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(MODULE_NO_CAN_MASTER_RACK_MOVE,BOARD_ID_CAN_MASTER_RACK_MOVE,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode != PROT_RESULT_FAIL)
    {
        CAN_PROT_LOG("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canRackMoveMasterCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canRackMoveMasterCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        CAN_PROT_LOG("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
		return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(MODULE_NO_CAN_MASTER_RACK_MOVE,BOARD_ID_CAN_MASTER_RACK_MOVE,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)rackHousePos;
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//装载机构移动到卸载试管架仓位置
void CanMasterRackMoveSendRunToUnLoadRackStorePosWhileAck(RACK_STORE_HOUSE rackHousePos,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_ACT_RACK_MOVE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_RACK_MOVE_TO_STORE_UNLOAD_POS;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(MODULE_NO_CAN_MASTER_RACK_MOVE,BOARD_ID_CAN_MASTER_RACK_MOVE,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode != PROT_RESULT_FAIL)
    {
        CAN_PROT_LOG("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canRackMoveMasterCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canRackMoveMasterCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        CAN_PROT_LOG("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
		return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(MODULE_NO_CAN_MASTER_RACK_MOVE,BOARD_ID_CAN_MASTER_RACK_MOVE,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)rackHousePos;
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//装载机构移动到装载C21位置
void CanMasterRackMoveSendRunToLoadRackC21PosWhileAck(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_ACT_RACK_MOVE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_RACK_MOVE_TO_C21_LOAD_POS;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(MODULE_NO_CAN_MASTER_RACK_MOVE,BOARD_ID_CAN_MASTER_RACK_MOVE,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode != PROT_RESULT_FAIL)
    {
        CAN_PROT_LOG("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canRackMoveMasterCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canRackMoveMasterCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        CAN_PROT_LOG("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
		return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(MODULE_NO_CAN_MASTER_RACK_MOVE,BOARD_ID_CAN_MASTER_RACK_MOVE,singalChannel,channelCmdCode,canCmdConfigPtr);
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//装载机构移动到卸载C21位置
void CanMasterRackMoveSendRunToUnLoadRackC21PosWhileAck(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_ACT_RACK_MOVE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_RACK_MOVE_TO_C21_UNLOAD_POS;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(MODULE_NO_CAN_MASTER_RACK_MOVE,BOARD_ID_CAN_MASTER_RACK_MOVE,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode != PROT_RESULT_FAIL)
    {
        CAN_PROT_LOG("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canRackMoveMasterCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canRackMoveMasterCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        CAN_PROT_LOG("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
		return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(MODULE_NO_CAN_MASTER_RACK_MOVE,BOARD_ID_CAN_MASTER_RACK_MOVE,singalChannel,channelCmdCode,canCmdConfigPtr);
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//在当前位置执行试管架仓样本架装载
void CanMasterRackMoveSendLoadInRackFormStoreCurrentWhileAck(FunctionalState barScanSet,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_ACT_RACK_MOVE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_RACK_MOVE_LOAD_STORE_RACK_CUR;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(MODULE_NO_CAN_MASTER_RACK_MOVE,BOARD_ID_CAN_MASTER_RACK_MOVE,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode != PROT_RESULT_FAIL)
    {
        CAN_PROT_LOG("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canRackMoveMasterCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canRackMoveMasterCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        CAN_PROT_LOG("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
		return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(MODULE_NO_CAN_MASTER_RACK_MOVE,BOARD_ID_CAN_MASTER_RACK_MOVE,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)barScanSet;
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//在当前位置执行试管架仓样本架放入
void CanMasterRackMoveSendPushOutRackToStoreCurrentWhileAck(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_ACT_RACK_MOVE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_RACK_MOVE_UNLOAD_STORE_RACK_CUR;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(MODULE_NO_CAN_MASTER_RACK_MOVE,BOARD_ID_CAN_MASTER_RACK_MOVE,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode != PROT_RESULT_FAIL)
    {
        CAN_PROT_LOG("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canRackMoveMasterCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canRackMoveMasterCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        CAN_PROT_LOG("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
		return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(MODULE_NO_CAN_MASTER_RACK_MOVE,BOARD_ID_CAN_MASTER_RACK_MOVE,singalChannel,channelCmdCode,canCmdConfigPtr);
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//在当前位置执行C21样本架装载
void CanMasterRackMoveSendLoadInRackFormC21CurrentWhileAck(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_ACT_RACK_MOVE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_RACK_MOVE_LOAD_C21_RACK_CUR;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(MODULE_NO_CAN_MASTER_RACK_MOVE,BOARD_ID_CAN_MASTER_RACK_MOVE,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode != PROT_RESULT_FAIL)
    {
        CAN_PROT_LOG("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canRackMoveMasterCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canRackMoveMasterCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        CAN_PROT_LOG("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
		return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(MODULE_NO_CAN_MASTER_RACK_MOVE,BOARD_ID_CAN_MASTER_RACK_MOVE,singalChannel,channelCmdCode,canCmdConfigPtr);
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//在当前位置执行C21样本架放入
void CanMasterRackMoveSendPushOutRackToC21CurrentWhileAck(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_ACT_RACK_MOVE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_RACK_MOVE_UNLOAD_C21_RACK_CUR;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(MODULE_NO_CAN_MASTER_RACK_MOVE,BOARD_ID_CAN_MASTER_RACK_MOVE,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode != PROT_RESULT_FAIL)
    {
        CAN_PROT_LOG("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canRackMoveMasterCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canRackMoveMasterCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        CAN_PROT_LOG("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
		return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(MODULE_NO_CAN_MASTER_RACK_MOVE,BOARD_ID_CAN_MASTER_RACK_MOVE,singalChannel,channelCmdCode,canCmdConfigPtr);
   //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//集成指令,从试管架仓中装载一个试管架,参数为仓号和是否扫码
void CanMasterRackMoveSendLoadInRackFormStoreByPosWhileAck(RACK_STORE_HOUSE rackHousePos,FunctionalState barScanSet,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_ACT_RACK_MOVE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_RACK_MOVE_LOAD_RACK_STORE_BY_POS;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(MODULE_NO_CAN_MASTER_RACK_MOVE,BOARD_ID_CAN_MASTER_RACK_MOVE,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode != PROT_RESULT_FAIL)
    {
        CAN_PROT_LOG("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canRackMoveMasterCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canRackMoveMasterCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        CAN_PROT_LOG("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
		return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(MODULE_NO_CAN_MASTER_RACK_MOVE,BOARD_ID_CAN_MASTER_RACK_MOVE,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)rackHousePos;
    canCmdSendPtr->cmdBufferPtr[1] = (uint8_t)barScanSet;
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//集成指令,往试管架仓中放入一个试管架,参数为仓号
void CanMasterRackMoveSendPushOutRackToStoreByPosWhileAck(RACK_STORE_HOUSE rackHousePos,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_ACT_RACK_MOVE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_RACK_MOVE_UNLOAD_RACK_STORE_BY_POS;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(MODULE_NO_CAN_MASTER_RACK_MOVE,BOARD_ID_CAN_MASTER_RACK_MOVE,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode != PROT_RESULT_FAIL)
    {
        CAN_PROT_LOG("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canRackMoveMasterCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canRackMoveMasterCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        CAN_PROT_LOG("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
		return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(MODULE_NO_CAN_MASTER_RACK_MOVE,BOARD_ID_CAN_MASTER_RACK_MOVE,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)rackHousePos;
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//集成指令,从C21接口装载一个试管架
void CanMasterRackMoveSendLoadInRackFormC21WhileAck(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_ACT_RACK_MOVE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_RACK_MOVE_LOAD_RACK_C21;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(MODULE_NO_CAN_MASTER_RACK_MOVE,BOARD_ID_CAN_MASTER_RACK_MOVE,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode != PROT_RESULT_FAIL)
    {
        CAN_PROT_LOG("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canRackMoveMasterCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canRackMoveMasterCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        CAN_PROT_LOG("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
		return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(MODULE_NO_CAN_MASTER_RACK_MOVE,BOARD_ID_CAN_MASTER_RACK_MOVE,singalChannel,channelCmdCode,canCmdConfigPtr);
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//集成指令,往C21接口放入一个试管架
void CanMasterRackMoveSendPushOutRackToC21WhileAck(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_ACT_RACK_MOVE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_RACK_MOVE_UNLOAD_RACK_C21;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(MODULE_NO_CAN_MASTER_RACK_MOVE,BOARD_ID_CAN_MASTER_RACK_MOVE,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode != PROT_RESULT_FAIL)
    {
        CAN_PROT_LOG("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canRackMoveMasterCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canRackMoveMasterCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        CAN_PROT_LOG("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
		return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(MODULE_NO_CAN_MASTER_RACK_MOVE,BOARD_ID_CAN_MASTER_RACK_MOVE,singalChannel,channelCmdCode,canCmdConfigPtr);
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//集成指令,从试管仓中取出一个试管架放入另一个试管仓中
void CanMasterRackMoveSendLoadInRackFormStoreByPosToStoreByPosWhileAck(RACK_STORE_HOUSE rackSrcHousePos,RACK_STORE_HOUSE rackTargetHousePos,
                                                                                FunctionalState barScanSet,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_ACT_RACK_MOVE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_RACK_MOVE_LOAD_RACK_STORE_TO_STORE;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(MODULE_NO_CAN_MASTER_RACK_MOVE,BOARD_ID_CAN_MASTER_RACK_MOVE,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode != PROT_RESULT_FAIL)
    {
        CAN_PROT_LOG("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canRackMoveMasterCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canRackMoveMasterCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        CAN_PROT_LOG("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
		return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(MODULE_NO_CAN_MASTER_RACK_MOVE,BOARD_ID_CAN_MASTER_RACK_MOVE,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)rackSrcHousePos;
    canCmdSendPtr->cmdBufferPtr[1] = (uint8_t)rackTargetHousePos;
    canCmdSendPtr->cmdBufferPtr[2] = (uint8_t)barScanSet;
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//集成指令,从试管仓中取出一个试管架放入C21中
void CanMasterRackMoveSendLoadInRackFormStoreByPosToC21WhileAck(RACK_STORE_HOUSE rackSrcHousePos,FunctionalState barScanSet,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_ACT_RACK_MOVE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_RACK_MOVE_LOAD_RACK_STORE_TO_C21;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(MODULE_NO_CAN_MASTER_RACK_MOVE,BOARD_ID_CAN_MASTER_RACK_MOVE,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode != PROT_RESULT_FAIL)
    {
        CAN_PROT_LOG("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canRackMoveMasterCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canRackMoveMasterCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        CAN_PROT_LOG("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
		return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(MODULE_NO_CAN_MASTER_RACK_MOVE,BOARD_ID_CAN_MASTER_RACK_MOVE,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)rackSrcHousePos;
    canCmdSendPtr->cmdBufferPtr[1] = (uint8_t)barScanSet;
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//从C21中取出一个试管架放入试管架仓中
void CanMasterRackMoveSendLoadInRackFormC21ToRackStoreWhileAck(RACK_STORE_HOUSE rackTargetHousePos,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_ACT_RACK_MOVE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_RACK_MOVE_LOAD_RACK_C21_TO_STORE;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(MODULE_NO_CAN_MASTER_RACK_MOVE,BOARD_ID_CAN_MASTER_RACK_MOVE,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode != PROT_RESULT_FAIL)
    {
        CAN_PROT_LOG("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canRackMoveMasterCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canRackMoveMasterCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        CAN_PROT_LOG("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
		return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(MODULE_NO_CAN_MASTER_RACK_MOVE,BOARD_ID_CAN_MASTER_RACK_MOVE,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)rackTargetHousePos;
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//C21校准启动
void CanMasterRackMoveC21AdjustStartWhileAck(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_ACT_RACK_MOVE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_RACK_MOVE_TO_C21_ADJUST_START;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(MODULE_NO_CAN_MASTER_RACK_MOVE,BOARD_ID_CAN_MASTER_RACK_MOVE,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode != PROT_RESULT_FAIL)
    {
        CAN_PROT_LOG("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canRackMoveMasterCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canRackMoveMasterCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        CAN_PROT_LOG("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
		return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(MODULE_NO_CAN_MASTER_RACK_MOVE,BOARD_ID_CAN_MASTER_RACK_MOVE,singalChannel,channelCmdCode,canCmdConfigPtr);
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//C21校准走步数
void CanMasterRackMoveC21AdjustRunWhileAck(int32_t targetCoordinate,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_ACT_RACK_MOVE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_RACK_MOVE_TO_C21_ADJUST_RUN;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(MODULE_NO_CAN_MASTER_RACK_MOVE,BOARD_ID_CAN_MASTER_RACK_MOVE,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode != PROT_RESULT_FAIL)
    {
        CAN_PROT_LOG("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canRackMoveMasterCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canRackMoveMasterCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        CAN_PROT_LOG("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
		return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(MODULE_NO_CAN_MASTER_RACK_MOVE,BOARD_ID_CAN_MASTER_RACK_MOVE,singalChannel,channelCmdCode,canCmdConfigPtr);
    //填充数据
    ProtUtilWriteUint32ToBuffer(targetCoordinate,canCmdSendPtr->cmdBufferPtr);
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//C21校准完成
void CanMasterRackMoveC21AdjustEndWhileAck(int32_t targetCoordinate,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_ACT_RACK_MOVE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_RACK_MOVE_TO_C21_ADJUST_END;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(MODULE_NO_CAN_MASTER_RACK_MOVE,BOARD_ID_CAN_MASTER_RACK_MOVE,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode != PROT_RESULT_FAIL)
    {
        CAN_PROT_LOG("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canRackMoveMasterCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canRackMoveMasterCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        CAN_PROT_LOG("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
		return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(MODULE_NO_CAN_MASTER_RACK_MOVE,BOARD_ID_CAN_MASTER_RACK_MOVE,singalChannel,channelCmdCode,canCmdConfigPtr);
    //填充数据
    ProtUtilWriteUint32ToBuffer(targetCoordinate,canCmdSendPtr->cmdBufferPtr);
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}



/*********************************************************等待指定指令完成返回********************************************************************/
//试管架移动单元电机复位
void CanMasterRackMoveWaitMotorResetResult(SYS_RESULT_PACK* resultPackPtr, CAN_MASTER_SM_STATE* motorStatePtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_ACT_RACK_MOVE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_RACK_MOVE_MOTOR_RESET;
	uint8_t targetModule = MODULE_NO_CAN_MASTER_RACK_MOVE;
    uint8_t targetBoardID = BOARD_ID_CAN_MASTER_RACK_MOVE;
	//保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(MODULE_NO_CAN_MASTER_RACK_MOVE,BOARD_ID_CAN_MASTER_RACK_MOVE,singalChannel,channelCmdCode,resultPackPtr,
						 &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,ProtocolCanWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
		//释放内存
        CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    
    //结果解析
    if(motorStatePtr != NULL)
    {
		//结果解析
        CanMasterStepMotorSetStateDefault(motorStatePtr);
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            CanMasterStepMotorGetResultMotorState(returnDataBufferPtr,motorStatePtr);
        }
    }
    //内存释放
    CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//试管架移动单元电机走步数
void CanMasterRackMoveWaitMotorRunStepResult(SYS_RESULT_PACK* resultPackPtr,CAN_MASTER_SM_STATE* motorStatePtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_ACT_RACK_MOVE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_RACK_MOVE_MOTOR_RUN_STEP;
	uint8_t targetModule = MODULE_NO_CAN_MASTER_RACK_MOVE;
    uint8_t targetBoardID = BOARD_ID_CAN_MASTER_RACK_MOVE;
	//保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(MODULE_NO_CAN_MASTER_RACK_MOVE,BOARD_ID_CAN_MASTER_RACK_MOVE,singalChannel,channelCmdCode,resultPackPtr,
						 &returnDataBufferPtr,&returnDataBufferLength);
	
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,ProtocolCanWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
		//释放内存
        CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //结果解析
    if(motorStatePtr != NULL)
    {
		//结果解析
        CanMasterStepMotorSetStateDefault(motorStatePtr);
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            CanMasterStepMotorGetResultMotorState(returnDataBufferPtr,motorStatePtr);
        }
    }
    //内存释放
    CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//试管架移动单元电机状态读取
void CanMasterRackMoveWaitMotorStateReadResult(SYS_RESULT_PACK* resultPackPtr,CAN_MASTER_SM_STATE* motorStatePtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_ACT_RACK_MOVE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_RACK_MOVE_MOTOR_STATE_READ;
	uint8_t targetModule = MODULE_NO_CAN_MASTER_RACK_MOVE;
    uint8_t targetBoardID = BOARD_ID_CAN_MASTER_RACK_MOVE;
	//保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(MODULE_NO_CAN_MASTER_RACK_MOVE,BOARD_ID_CAN_MASTER_RACK_MOVE,singalChannel,channelCmdCode,resultPackPtr,
						 &returnDataBufferPtr,&returnDataBufferLength);
	
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,ProtocolCanWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
		//释放内存
        CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //结果解析
    if(motorStatePtr != NULL)
    {
		//结果解析
        CanMasterStepMotorSetStateDefault(motorStatePtr);
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            CanMasterStepMotorGetResultMotorState(returnDataBufferPtr,motorStatePtr);
        }
    }
    //内存释放
    CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//试管架移动单元传感器信号读取
void CanMasterRackMoveWaitSensorReadSingleResult(SYS_RESULT_PACK* resultPackPtr,PIN_STATE* sensorStatePtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_ACT_RACK_MOVE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_RACK_MOVE_SENSOR_READ_SINGLE;
	uint8_t targetModule = MODULE_NO_CAN_MASTER_RACK_MOVE;
    uint8_t targetBoardID = BOARD_ID_CAN_MASTER_RACK_MOVE;
	//保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
	//等待指令返回
    CanProtCmdWaitResult(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,ProtocolCanWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
		//释放内存
        CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //结果解析
    if(sensorStatePtr != NULL)
    {
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            if(sensorStatePtr != NULL)
            {
                *sensorStatePtr = (PIN_STATE)(returnDataBufferPtr[0]);
            }
        }
    }
    
    //内存释放
    CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//试管架移动单元传感器信号批量读取
void CanMasterRackMoveWaitSensorReadAllResult(SYS_RESULT_PACK* resultPackPtr,RACK_MOVE_SENSOR_SET* sensorSetPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_ACT_RACK_MOVE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_RACK_MOVE_SENSOR_READ_ALL;
	uint8_t targetModule = MODULE_NO_CAN_MASTER_RACK_MOVE;
    uint8_t targetBoardID = BOARD_ID_CAN_MASTER_RACK_MOVE;
	//保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
	CanProtCmdWaitResult(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,ProtocolCanWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
		//释放内存
        CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    
    //结果解析
    if(sensorSetPtr != NULL)
    {
        UserMemSet((uint8_t*)sensorSetPtr,0,DIM_STRUCT_LENGTH_BYTE(RACK_MOVE_SENSOR_SET));
		if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            if(sensorSetPtr != NULL)
            {
                UserMemCopy((uint8_t*)sensorSetPtr,returnDataBufferPtr,DIM_STRUCT_LENGTH_BYTE(RACK_MOVE_SENSOR_SET));
            }
        }
    }
    
    //内存释放
    CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//获取试管架移动单元当前位置
void CanMasterRackMoveWaitGetCurrentPosResult(SYS_RESULT_PACK* resultPackPtr,RACK_MOVE_POS* rackMovePosPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_ACT_RACK_MOVE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_RACK_MOVE_GET_CUR_POS;
	uint8_t targetModule = MODULE_NO_CAN_MASTER_RACK_MOVE;
    uint8_t targetBoardID = BOARD_ID_CAN_MASTER_RACK_MOVE;
	//保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //创建结果包
    CanProtCmdWaitResult(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
	if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,ProtocolCanWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
		//释放内存
        CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
	
    //结果解析
    if(rackMovePosPtr != NULL)
    {
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            if(rackMovePosPtr != NULL)
            {
                UserMemCopy((uint8_t*)rackMovePosPtr,returnDataBufferPtr,DIM_STRUCT_LENGTH_BYTE(RACK_MOVE_POS));
            }
        }
    }
  
    //内存释放
    CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//试管架移动单元复位
void CanMasterRackMoveWaitModuleResetResult(SYS_RESULT_PACK* resultPackPtr,RACK_MOVE_SENSOR_SET* sensorSetPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_ACT_RACK_MOVE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_RACK_MOVE_MODULE_RESET;
	uint8_t targetModule = MODULE_NO_CAN_MASTER_RACK_MOVE;
    uint8_t targetBoardID = BOARD_ID_CAN_MASTER_RACK_MOVE;
	//保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //创建结果包
    CanProtCmdWaitResult(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
	if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,ProtocolCanWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
		//释放内存
        CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
   
    //结果解析
    if(sensorSetPtr != NULL)
    {
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            if(sensorSetPtr != NULL)
            {
                UserMemCopy((uint8_t*)sensorSetPtr,returnDataBufferPtr,DIM_STRUCT_LENGTH_BYTE(RACK_MOVE_SENSOR_SET));
            }
        }
    }
    //内存释放
    CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//试管架移动单元到空闲位置
void CanMasterRackMoveWaitModuleToIdleResult(SYS_RESULT_PACK* resultPackPtr,RACK_MOVE_SENSOR_SET* sensorSetPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_ACT_RACK_MOVE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_RACK_MOVE_TO_IDLE;
	uint8_t targetModule = MODULE_NO_CAN_MASTER_RACK_MOVE;
    uint8_t targetBoardID = BOARD_ID_CAN_MASTER_RACK_MOVE;
	//保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
	//创建结果包
    CanProtCmdWaitResult(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
	if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,ProtocolCanWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
		//释放内存
        CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
   
    //结果解析
    if(sensorSetPtr != NULL)
    {
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            if(sensorSetPtr != NULL)
            {
                UserMemCopy((uint8_t*)sensorSetPtr,returnDataBufferPtr,DIM_STRUCT_LENGTH_BYTE(RACK_MOVE_SENSOR_SET));
            }
        }
    }
    //内存释放
    CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//横移电机移动到指定试管架仓位置
void CanMasterRackMoveWaitLaterallyToStoreByPosResult(SYS_RESULT_PACK* resultPackPtr,RACK_MOVE_POS* rackMovePosPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_ACT_RACK_MOVE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_RACK_MOVE_LATERALLY_TO_STORE;
	uint8_t targetModule = MODULE_NO_CAN_MASTER_RACK_MOVE;
    uint8_t targetBoardID = BOARD_ID_CAN_MASTER_RACK_MOVE;
	//保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
	//创建结果包
    CanProtCmdWaitResult(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
	if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,ProtocolCanWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
		//释放内存
        CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
   
    //结果解析
    if(rackMovePosPtr != NULL)
    {
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            if(rackMovePosPtr != NULL)
            {
                UserMemCopy((uint8_t*)rackMovePosPtr,returnDataBufferPtr,DIM_STRUCT_LENGTH_BYTE(RACK_MOVE_POS));
            }
        }
    }
    //内存释放
    CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//横移电机移动到C21接口位置
void CanMasterRackMoveWaitLaterallyToC21Result(SYS_RESULT_PACK* resultPackPtr,RACK_MOVE_POS* rackMovePosPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_ACT_RACK_MOVE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_RACK_MOVE_LATERALLY_TO_C21;
	uint8_t targetModule = MODULE_NO_CAN_MASTER_RACK_MOVE;
    uint8_t targetBoardID = BOARD_ID_CAN_MASTER_RACK_MOVE;
	//保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
	//创建结果包
    CanProtCmdWaitResult(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
	if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,ProtocolCanWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
		//释放内存
        CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
   
    //结果解析
    if(rackMovePosPtr != NULL)
    {
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            if(rackMovePosPtr != NULL)
            {
                UserMemCopy((uint8_t*)rackMovePosPtr,returnDataBufferPtr,DIM_STRUCT_LENGTH_BYTE(RACK_MOVE_POS));
            }
        }
    }
    //内存释放
    CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//装载机构移动到装载试管架仓位置
void CanMasterRackMoveWaitRunToLoadRackStorePosResult(SYS_RESULT_PACK* resultPackPtr,RACK_MOVE_POS* rackMovePosPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_ACT_RACK_MOVE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_RACK_MOVE_TO_STORE_LOAD_POS;
	uint8_t targetModule = MODULE_NO_CAN_MASTER_RACK_MOVE;
    uint8_t targetBoardID = BOARD_ID_CAN_MASTER_RACK_MOVE;
	//保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
	//创建结果包
    CanProtCmdWaitResult(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
	if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,ProtocolCanWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
		//释放内存
        CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
   
    //结果解析
    if(rackMovePosPtr != NULL)
    {
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            if(rackMovePosPtr != NULL)
            {
                UserMemCopy((uint8_t*)rackMovePosPtr,returnDataBufferPtr,DIM_STRUCT_LENGTH_BYTE(RACK_MOVE_POS));
            }
        }
    }
    //内存释放
    CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//装载机构移动到卸载试管架仓位置
void CanMasterRackMoveWaitRunToUnLoadRackStorePosResult(SYS_RESULT_PACK* resultPackPtr,RACK_MOVE_POS* rackMovePosPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_ACT_RACK_MOVE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_RACK_MOVE_TO_STORE_UNLOAD_POS;
	uint8_t targetModule = MODULE_NO_CAN_MASTER_RACK_MOVE;
    uint8_t targetBoardID = BOARD_ID_CAN_MASTER_RACK_MOVE;
	//保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
	//创建结果包
    CanProtCmdWaitResult(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
	if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,ProtocolCanWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
		//释放内存
        CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
   
    //结果解析
    if(rackMovePosPtr != NULL)
    {
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            if(rackMovePosPtr != NULL)
            {
                UserMemCopy((uint8_t*)rackMovePosPtr,returnDataBufferPtr,DIM_STRUCT_LENGTH_BYTE(RACK_MOVE_POS));
            }
        }
    }
    //内存释放
    CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//装载机构移动到装载C21位置
void CanMasterRackMoveWaitRunToLoadRackC21PosResult(SYS_RESULT_PACK* resultPackPtr,RACK_MOVE_POS* rackMovePosPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_ACT_RACK_MOVE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_RACK_MOVE_TO_C21_LOAD_POS;
	uint8_t targetModule = MODULE_NO_CAN_MASTER_RACK_MOVE;
    uint8_t targetBoardID = BOARD_ID_CAN_MASTER_RACK_MOVE;
	//保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
	//创建结果包
    CanProtCmdWaitResult(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
	if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,ProtocolCanWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
		//释放内存
        CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
   
    //结果解析
    if(rackMovePosPtr != NULL)
    {
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            if(rackMovePosPtr != NULL)
            {
                UserMemCopy((uint8_t*)rackMovePosPtr,returnDataBufferPtr,DIM_STRUCT_LENGTH_BYTE(RACK_MOVE_POS));
            }
        }
    }
    //内存释放
    CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//装载机构移动到卸载C21位置
void CanMasterRackMoveWaitRunToUnLoadRackC21PosResult(SYS_RESULT_PACK* resultPackPtr,RACK_MOVE_POS* rackMovePosPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_ACT_RACK_MOVE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_RACK_MOVE_TO_C21_UNLOAD_POS;
	uint8_t targetModule = MODULE_NO_CAN_MASTER_RACK_MOVE;
    uint8_t targetBoardID = BOARD_ID_CAN_MASTER_RACK_MOVE;
	//保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
	//创建结果包
    CanProtCmdWaitResult(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
	if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,ProtocolCanWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
		//释放内存
        CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
   
    //结果解析
    if(rackMovePosPtr != NULL)
    {
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            if(rackMovePosPtr != NULL)
            {
                UserMemCopy((uint8_t*)rackMovePosPtr,returnDataBufferPtr,DIM_STRUCT_LENGTH_BYTE(RACK_MOVE_POS));
            }
        }
    }
    //内存释放
    CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//在当前位置执行试管架仓样本架装载
void CanMasterRackMoveWaitLoadInRackFormStoreCurrentResult(SYS_RESULT_PACK* resultPackPtr,RACK_BAR_MSG* rackBarMsgPtr,
                                                                    RACK_MOVE_SENSOR_SET* sensorSetPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_ACT_RACK_MOVE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_RACK_MOVE_LOAD_STORE_RACK_CUR;
	uint8_t targetModule = MODULE_NO_CAN_MASTER_RACK_MOVE;
    uint8_t targetBoardID = BOARD_ID_CAN_MASTER_RACK_MOVE;
	//保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
	//创建结果包
    CanProtCmdWaitResult(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
	if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,ProtocolCanWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
		//释放内存
        CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
   
    //结果解析
    if(rackBarMsgPtr != NULL && sensorSetPtr != NULL)
    {
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            UserMemCopy((uint8_t*)rackBarMsgPtr,returnDataBufferPtr,DIM_STRUCT_LENGTH_BYTE(RACK_BAR_MSG));
			UserMemCopy((uint8_t*)sensorSetPtr,returnDataBufferPtr+DIM_STRUCT_LENGTH_BYTE(RACK_BAR_MSG),
                            DIM_STRUCT_LENGTH_BYTE(RACK_MOVE_SENSOR_SET));
        }
    }
    //内存释放
    CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//在当前位置执行试管架仓样本架放入
void CanMasterRackMoveWaitPushOutRackToStoreCurrentResult(SYS_RESULT_PACK* resultPackPtr,RACK_MOVE_SENSOR_SET* sensorSetPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_ACT_RACK_MOVE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_RACK_MOVE_UNLOAD_STORE_RACK_CUR;
	uint8_t targetModule = MODULE_NO_CAN_MASTER_RACK_MOVE;
    uint8_t targetBoardID = BOARD_ID_CAN_MASTER_RACK_MOVE;
	//保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
	//创建结果包
    CanProtCmdWaitResult(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
	if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,ProtocolCanWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
		//释放内存
        CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
   
    //结果解析
    if(sensorSetPtr != NULL)
    {
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
			UserMemCopy((uint8_t*)sensorSetPtr,returnDataBufferPtr,DIM_STRUCT_LENGTH_BYTE(RACK_MOVE_SENSOR_SET));
        }
    }
	//内存释放
    CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//在当前位置执行C21样本架装载
void CanMasterRackMoveWaitLoadInRackFormC21CurrentResult(SYS_RESULT_PACK* resultPackPtr,RACK_MOVE_SENSOR_SET* sensorSetPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_ACT_RACK_MOVE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_RACK_MOVE_LOAD_C21_RACK_CUR;
	uint8_t targetModule = MODULE_NO_CAN_MASTER_RACK_MOVE;
    uint8_t targetBoardID = BOARD_ID_CAN_MASTER_RACK_MOVE;
	//保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
	//创建结果包
    CanProtCmdWaitResult(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
	if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,ProtocolCanWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
		//释放内存
        CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
   
    //结果解析
    if(sensorSetPtr != NULL)
    {
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
			UserMemCopy((uint8_t*)sensorSetPtr,returnDataBufferPtr,DIM_STRUCT_LENGTH_BYTE(RACK_MOVE_SENSOR_SET));
        }
    }
	//内存释放
    CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//在当前位置执行C21样本架放入
void CanMasterRackMoveWaitPushOutRackToC21CurrentResult(SYS_RESULT_PACK* resultPackPtr,RACK_MOVE_SENSOR_SET* sensorSetPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_ACT_RACK_MOVE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_RACK_MOVE_UNLOAD_C21_RACK_CUR;
	uint8_t targetModule = MODULE_NO_CAN_MASTER_RACK_MOVE;
    uint8_t targetBoardID = BOARD_ID_CAN_MASTER_RACK_MOVE;
	//保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
	//创建结果包
    CanProtCmdWaitResult(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
	if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,ProtocolCanWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
		//释放内存
        CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
   
    //结果解析
    if(sensorSetPtr != NULL)
    {
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
			UserMemCopy((uint8_t*)sensorSetPtr,returnDataBufferPtr,DIM_STRUCT_LENGTH_BYTE(RACK_MOVE_SENSOR_SET));
        }
    }
	//内存释放
    CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//集成指令,从试管架仓中装载一个试管架,参数为仓号和是否扫码
void CanMasterRackMoveWaitLoadInRackFormStoreByPosResult(SYS_RESULT_PACK* resultPackPtr,RACK_BAR_MSG* rackBarMsgPtr,
                                                                    RACK_MOVE_SENSOR_SET* sensorSetPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_ACT_RACK_MOVE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_RACK_MOVE_LOAD_RACK_STORE_BY_POS;
	uint8_t targetModule = MODULE_NO_CAN_MASTER_RACK_MOVE;
    uint8_t targetBoardID = BOARD_ID_CAN_MASTER_RACK_MOVE;
	//保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
	//创建结果包
    CanProtCmdWaitResult(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
	if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,ProtocolCanWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
		//释放内存
        CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
   
    //结果解析
    if(rackBarMsgPtr != NULL && sensorSetPtr != NULL)
    {
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
			
			UserMemCopy((uint8_t*)rackBarMsgPtr,returnDataBufferPtr,DIM_STRUCT_LENGTH_BYTE(RACK_BAR_MSG));
		
			UserMemCopy((uint8_t*)sensorSetPtr,returnDataBufferPtr+DIM_STRUCT_LENGTH_BYTE(RACK_BAR_MSG),
						DIM_STRUCT_LENGTH_BYTE(RACK_MOVE_SENSOR_SET));
        }
    }
	//内存释放
    CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//集成指令,往试管架仓中放入一个试管架,参数为仓号
void CanMasterRackMoveWaitPushOutRackToStoreByPosResult(SYS_RESULT_PACK* resultPackPtr,RACK_MOVE_SENSOR_SET* sensorSetPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_ACT_RACK_MOVE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_RACK_MOVE_UNLOAD_RACK_STORE_BY_POS;
	uint8_t targetModule = MODULE_NO_CAN_MASTER_RACK_MOVE;
    uint8_t targetBoardID = BOARD_ID_CAN_MASTER_RACK_MOVE;
	//保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
	//创建结果包
    CanProtCmdWaitResult(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
	if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,ProtocolCanWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
		//释放内存
        CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
   
    //结果解析
    if(sensorSetPtr != NULL)
    {
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
			UserMemCopy((uint8_t*)sensorSetPtr,returnDataBufferPtr,DIM_STRUCT_LENGTH_BYTE(RACK_MOVE_SENSOR_SET));
        }
    }
	//内存释放
    CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//集成指令,从C21接口装载一个试管架
void CanMasterRackMoveWaitLoadInRackFormC21Result(SYS_RESULT_PACK* resultPackPtr,RACK_MOVE_SENSOR_SET* sensorSetPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_ACT_RACK_MOVE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_RACK_MOVE_LOAD_RACK_C21;
	uint8_t targetModule = MODULE_NO_CAN_MASTER_RACK_MOVE;
    uint8_t targetBoardID = BOARD_ID_CAN_MASTER_RACK_MOVE;
	//保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
	//创建结果包
    CanProtCmdWaitResult(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
	if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,ProtocolCanWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
		//释放内存
        CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
   
    //结果解析
    if(sensorSetPtr != NULL)
    {
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
			UserMemCopy((uint8_t*)sensorSetPtr,returnDataBufferPtr,DIM_STRUCT_LENGTH_BYTE(RACK_MOVE_SENSOR_SET));
        }
    }
	//内存释放
    CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//集成指令,往C21接口放入一个试管架
void CanMasterRackMoveWaitPushOutRackToC21Result(SYS_RESULT_PACK* resultPackPtr,RACK_MOVE_SENSOR_SET* sensorSetPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_ACT_RACK_MOVE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_RACK_MOVE_UNLOAD_RACK_C21;
	uint8_t targetModule = MODULE_NO_CAN_MASTER_RACK_MOVE;
    uint8_t targetBoardID = BOARD_ID_CAN_MASTER_RACK_MOVE;
	//保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
	//创建结果包
    CanProtCmdWaitResult(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
	if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,ProtocolCanWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
		//释放内存
        CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
   
    //结果解析
    if(sensorSetPtr != NULL)
    {
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
			UserMemCopy((uint8_t*)sensorSetPtr,returnDataBufferPtr,DIM_STRUCT_LENGTH_BYTE(RACK_MOVE_SENSOR_SET));
        }
    }
	//内存释放
    CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//集成指令,从试管仓中取出一个试管架放入另一个试管仓中
void CanMasterRackMoveWaitLoadInRackFormStoreByPosToStoreByPosResult(SYS_RESULT_PACK* resultPackPtr,RACK_BAR_MSG* rackBarMsgPtr,
                                                                    RACK_MOVE_SENSOR_SET* sensorSetPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_ACT_RACK_MOVE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_RACK_MOVE_LOAD_RACK_STORE_TO_STORE;
	uint8_t targetModule = MODULE_NO_CAN_MASTER_RACK_MOVE;
    uint8_t targetBoardID = BOARD_ID_CAN_MASTER_RACK_MOVE;
	//保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
	//创建结果包
    CanProtCmdWaitResult(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
	if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,ProtocolCanWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
		//释放内存
        CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
   
    //结果解析
    if(rackBarMsgPtr != NULL && sensorSetPtr != NULL)
    {
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
			
			UserMemCopy((uint8_t*)rackBarMsgPtr,returnDataBufferPtr,DIM_STRUCT_LENGTH_BYTE(RACK_BAR_MSG));
		
			UserMemCopy((uint8_t*)sensorSetPtr,returnDataBufferPtr+DIM_STRUCT_LENGTH_BYTE(RACK_BAR_MSG),
						DIM_STRUCT_LENGTH_BYTE(RACK_MOVE_SENSOR_SET));
        }
    }
	//内存释放
    CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//集成指令,从试管仓中取出一个试管架放入C21中
void CanMasterRackMoveWaitLoadInRackFormStoreByPosToC21Result(SYS_RESULT_PACK* resultPackPtr,RACK_BAR_MSG* rackBarMsgPtr,
                                                                    RACK_MOVE_SENSOR_SET* sensorSetPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_ACT_RACK_MOVE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_RACK_MOVE_LOAD_RACK_STORE_TO_C21;
	uint8_t targetModule = MODULE_NO_CAN_MASTER_RACK_MOVE;
    uint8_t targetBoardID = BOARD_ID_CAN_MASTER_RACK_MOVE;
	//保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
	//创建结果包
    CanProtCmdWaitResult(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
	if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,ProtocolCanWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
		//释放内存
        CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
   
    //结果解析
    if(rackBarMsgPtr != NULL && sensorSetPtr != NULL)
    {
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
			
			UserMemCopy((uint8_t*)rackBarMsgPtr,returnDataBufferPtr,DIM_STRUCT_LENGTH_BYTE(RACK_BAR_MSG));
		
			UserMemCopy((uint8_t*)sensorSetPtr,returnDataBufferPtr+DIM_STRUCT_LENGTH_BYTE(RACK_BAR_MSG),
						DIM_STRUCT_LENGTH_BYTE(RACK_MOVE_SENSOR_SET));
        }
    }
	//内存释放
    CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//从C21中取出一个试管架放入试管架仓中
void CanMasterRackMoveWaitLoadInRackFormC21ToRackStoreResult(SYS_RESULT_PACK* resultPackPtr,RACK_MOVE_SENSOR_SET* sensorSetPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_ACT_RACK_MOVE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_RACK_MOVE_LOAD_RACK_C21_TO_STORE;
	uint8_t targetModule = MODULE_NO_CAN_MASTER_RACK_MOVE;
    uint8_t targetBoardID = BOARD_ID_CAN_MASTER_RACK_MOVE;
	//保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
	//创建结果包
    CanProtCmdWaitResult(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
	if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,ProtocolCanWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
		//释放内存
        CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
   
    //结果解析
    if(sensorSetPtr != NULL)
    {
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
			UserMemCopy((uint8_t*)sensorSetPtr,returnDataBufferPtr,DIM_STRUCT_LENGTH_BYTE(RACK_MOVE_SENSOR_SET));
        }
    }
	//内存释放
    CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//C21校准启动
void CanMasterRackMoveWaitC21AdjustStartResult(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_ACT_RACK_MOVE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_RACK_MOVE_TO_C21_ADJUST_START;
	uint8_t targetModule = MODULE_NO_CAN_MASTER_RACK_MOVE;
    uint8_t targetBoardID = BOARD_ID_CAN_MASTER_RACK_MOVE;
	//保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
	//创建结果包
    CanProtCmdWaitResult(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
	if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,ProtocolCanWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
		//释放内存
        CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //内存释放
    CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//C21校准走步数
void CanMasterRackMoveWaitC21AdjustRunResult(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_ACT_RACK_MOVE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_RACK_MOVE_TO_C21_ADJUST_RUN;
	uint8_t targetModule = MODULE_NO_CAN_MASTER_RACK_MOVE;
    uint8_t targetBoardID = BOARD_ID_CAN_MASTER_RACK_MOVE;
	//保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
	//创建结果包
    CanProtCmdWaitResult(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
	if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,ProtocolCanWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
		//释放内存
        CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //内存释放
    CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//C21校准完成
void CanMasterRackMoveWaitC21AdjustEndResult(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_ACT_RACK_MOVE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_RACK_MOVE_TO_C21_ADJUST_END;
	uint8_t targetModule = MODULE_NO_CAN_MASTER_RACK_MOVE;
    uint8_t targetBoardID = BOARD_ID_CAN_MASTER_RACK_MOVE;
	//保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
	//创建结果包
    CanProtCmdWaitResult(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
	if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,ProtocolCanWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
		//释放内存
        CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //内存释放
    CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}


/*********************************************************发送指令等待完成返回********************************************************************/
//试管架移动单元电机复位
void CanMasterRackMoveSendMotorResetWhileReturn(STEP_MOTOR_RACK_MOVE motorNo,SYS_RESULT_PACK* resultPackPtr,CAN_MASTER_SM_STATE* motorStatePtr)
{
    //发送指令等待ACK
    CanMasterRackMoveSendMotorResetWhileAck(motorNo,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterRackMoveSendMotorResetWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterRackMoveWaitMotorResetResult(resultPackPtr,motorStatePtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterRackMoveWaitMotorResetResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//试管架移动单元电机走步数
void CanMasterRackMoveSendMotorRunStepWhileReturn(STEP_MOTOR_RACK_MOVE motorNo,int32_t steps,SYS_RESULT_PACK* resultPackPtr,CAN_MASTER_SM_STATE* motorStatePtr)
{
    //发送指令等待ACK
    CanMasterRackMoveSendMotorRunStepWhileAck(motorNo,steps,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterRackMoveSendMotorRunStepWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return ;
    }
    //等待结果
    CanMasterRackMoveWaitMotorRunStepResult(resultPackPtr,motorStatePtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterRackMoveWaitMotorRunStepResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//试管架移动单元电机状态读取
void CanMasterRackMoveSendMotorStateReadWhileReturn(STEP_MOTOR_RACK_MOVE motorNo,SYS_RESULT_PACK* resultPackPtr,CAN_MASTER_SM_STATE* motorStatePtr)
{
    //发送指令等待ACK
    CanMasterRackMoveSendMotorStateReadWhileAck(motorNo, resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterRackMoveSendMotorStateReadWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterRackMoveWaitMotorStateReadResult(resultPackPtr,motorStatePtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterRackMoveWaitMotorStateReadResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//试管架移动单元传感器信号单个读取
void CanMasterRackMoveSendSensorReadSingleWhileReturn(RACK_MOVE_SENSOR sensorNo,SYS_RESULT_PACK* resultPackPtr,PIN_STATE* sensorStatePtr)
{
    //发送指令等待ACK
    CanMasterRackMoveSendSensorReadSingleWhileAck(sensorNo,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterRackMoveSendSensorReadSingleWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterRackMoveWaitSensorReadSingleResult(resultPackPtr,sensorStatePtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterRackMoveWaitSensorReadSingleResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//试管架移动单元传感器信号批量读取
void CanMasterRackMoveSendSensorReadAllWhileReturn(SYS_RESULT_PACK* resultPackPtr,RACK_MOVE_SENSOR_SET* sensorSetPtr)
{
    //发送指令等待ACK
    CanMasterRackMoveSendSensorReadAllWhileAck(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterRackMoveSendSensorReadAllWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterRackMoveWaitSensorReadAllResult(resultPackPtr,sensorSetPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterRackMoveWaitSensorReadAllResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//获取试管架移动单元当前位置
void CanMasterRackMoveSendGetCurrentPosWhileReturn(SYS_RESULT_PACK* resultPackPtr,RACK_MOVE_POS* rackMovePosPtr)
{
    //发送指令等待ACK
    CanMasterRackMoveSendGetCurrentPosWhileAck(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterRackMoveSendGetCurrentPosWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return ;
    }
    //等待结果
    CanMasterRackMoveWaitGetCurrentPosResult(resultPackPtr,rackMovePosPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterRackMoveWaitGetCurrentPosResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//试管架移动单元复位
void CanMasterRackMoveSendModuleResetWhileReturn(SYS_RESULT_PACK* resultPackPtr,RACK_MOVE_SENSOR_SET* sensorSetPtr)
{
    //发送指令等待ACK
    CanMasterRackMoveSendModuleResetWhileAck(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterRackMoveSendModuleResetWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterRackMoveWaitModuleResetResult(resultPackPtr,sensorSetPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterRackMoveWaitModuleResetResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//试管架移动单元到空闲位置
void CanMasterRackMoveSendModuleToIdleWhileReturn(SYS_RESULT_PACK* resultPackPtr,RACK_MOVE_SENSOR_SET* sensorSetPtr)
{
    //发送指令等待ACK
    CanMasterRackMoveSendModuleToIdleWhileAck(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterRackMoveSendModuleToIdleWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterRackMoveWaitModuleToIdleResult(resultPackPtr,sensorSetPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterRackMoveWaitModuleToIdleResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//横移电机移动到指定试管架仓位置
void CanMasterRackMoveSendLaterallyToStoreByPosWhileReturn(RACK_STORE_HOUSE rackHousePos,SYS_RESULT_PACK* resultPackPtr,RACK_MOVE_POS* rackMovePosPtr)
{
    //发送指令等待ACK
    CanMasterRackMoveSendLaterallyToStoreByPosWhileAck(rackHousePos,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterRackMoveSendLaterallyToStoreByPosWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterRackMoveWaitLaterallyToStoreByPosResult(resultPackPtr,rackMovePosPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterRackMoveWaitLaterallyToStoreByPosResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//横移电机移动到C21接口位置
void CanMasterRackMoveSendLaterallyToC21WhileReturn(SYS_RESULT_PACK* resultPackPtr,RACK_MOVE_POS* rackMovePosPtr)
{
    //发送指令等待ACK
    CanMasterRackMoveSendLaterallyToC21WhileAck(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterRackMoveSendLaterallyToC21WhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterRackMoveWaitLaterallyToC21Result(resultPackPtr,rackMovePosPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterRackMoveWaitLaterallyToC21Result ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//装载机构移动到装载试管架仓位置
void CanMasterRackMoveSendRunToLoadRackStorePosWhileReturn(RACK_STORE_HOUSE rackHousePos,SYS_RESULT_PACK* resultPackPtr,RACK_MOVE_POS* rackMovePosPtr)
{
    //发送指令等待ACK
    CanMasterRackMoveSendRunToLoadRackStorePosWhileAck(rackHousePos,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterRackMoveSendRunToLoadRackStorePosWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterRackMoveWaitRunToLoadRackStorePosResult(resultPackPtr,rackMovePosPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterRackMoveWaitRunToLoadRackStorePosResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//装载机构移动到卸载试管架仓位置
void CanMasterRackMoveSendRunToUnLoadRackStorePosWhileReturn(RACK_STORE_HOUSE rackHousePos,SYS_RESULT_PACK* resultPackPtr,RACK_MOVE_POS* rackMovePosPtr)
{
    //发送指令等待ACK
    CanMasterRackMoveSendRunToUnLoadRackStorePosWhileAck(rackHousePos,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterRackMoveSendRunToUnLoadRackStorePosWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterRackMoveWaitRunToUnLoadRackStorePosResult(resultPackPtr,rackMovePosPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterRackMoveWaitRunToUnLoadRackStorePosResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//装载机构移动到装载C21位置
void CanMasterRackMoveSendRunToLoadRackC21PosWhileReturn(SYS_RESULT_PACK* resultPackPtr,RACK_MOVE_POS* rackMovePosPtr)
{
    //发送指令等待ACK
    CanMasterRackMoveSendRunToLoadRackC21PosWhileAck(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterRackMoveSendRunToLoadRackC21PosWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterRackMoveWaitRunToLoadRackC21PosResult(resultPackPtr,rackMovePosPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterRackMoveWaitRunToLoadRackC21PosResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//装载机构移动到卸载C21位置
void CanMasterRackMoveSendRunToUnLoadRackC21PosWhileReturn(SYS_RESULT_PACK* resultPackPtr,RACK_MOVE_POS* rackMovePosPtr)
{
    //发送指令等待ACK
    CanMasterRackMoveSendRunToUnLoadRackC21PosWhileAck(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterRackMoveSendRunToUnLoadRackC21PosWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterRackMoveWaitRunToUnLoadRackC21PosResult(resultPackPtr,rackMovePosPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterRackMoveWaitRunToUnLoadRackC21PosResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//在当前位置执行试管架仓样本架装载
void CanMasterRackMoveSendLoadInRackFormStoreCurrentWhileReturn(FunctionalState barScanSet,SYS_RESULT_PACK* resultPackPtr,RACK_BAR_MSG* rackBarMsgPtr,
                                                                    RACK_MOVE_SENSOR_SET* sensorSetPtr)
{
    //发送指令等待ACK
    CanMasterRackMoveSendLoadInRackFormStoreCurrentWhileAck(barScanSet,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterRackMoveSendLoadInRackFormStoreCurrentWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterRackMoveWaitLoadInRackFormStoreCurrentResult(resultPackPtr,rackBarMsgPtr,sensorSetPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterRackMoveWaitLoadInRackFormStoreCurrentResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//在当前位置执行试管架仓样本架放入
void CanMasterRackMoveSendPushOutRackToStoreCurrentWhileReturn(SYS_RESULT_PACK* resultPackPtr,RACK_MOVE_SENSOR_SET* sensorSetPtr)
{
    //发送指令等待ACK
    CanMasterRackMoveSendPushOutRackToStoreCurrentWhileAck(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterRackMoveSendPushOutRackToStoreCurrentWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterRackMoveWaitPushOutRackToStoreCurrentResult(resultPackPtr,sensorSetPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterRackMoveWaitPushOutRackToStoreCurrentResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//在当前位置执行C21样本架装载
void CanMasterRackMoveSendLoadInRackFormC21CurrentWhileReturn(SYS_RESULT_PACK* resultPackPtr,RACK_MOVE_SENSOR_SET* sensorSetPtr)
{
    //发送指令等待ACK
    CanMasterRackMoveSendLoadInRackFormC21CurrentWhileAck(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterRackMoveSendLoadInRackFormC21CurrentWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterRackMoveWaitLoadInRackFormC21CurrentResult(resultPackPtr,sensorSetPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterRackMoveWaitLoadInRackFormC21CurrentResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//在当前位置执行C21样本架放入
void CanMasterRackMoveSendPushOutRackToC21CurrentWhileReturn(SYS_RESULT_PACK* resultPackPtr,RACK_MOVE_SENSOR_SET* sensorSetPtr)
{
    //发送指令等待ACK
    CanMasterRackMoveSendPushOutRackToC21CurrentWhileAck(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterRackMoveSendPushOutRackToC21CurrentWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterRackMoveWaitPushOutRackToC21CurrentResult(resultPackPtr,sensorSetPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterRackMoveWaitPushOutRackToC21CurrentResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return ;
}

//集成指令,从试管架仓中装载一个试管架,参数为仓号和是否扫码
void CanMasterRackMoveSendLoadInRackFormStoreByPosWhileReturn(RACK_STORE_HOUSE rackHousePos,FunctionalState barScanSet,SYS_RESULT_PACK* resultPackPtr,
                                                                    RACK_BAR_MSG* rackBarMsgPtr,RACK_MOVE_SENSOR_SET* sensorSetPtr)
{
    //发送指令等待ACK
    CanMasterRackMoveSendLoadInRackFormStoreByPosWhileAck(rackHousePos,barScanSet,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterRackMoveSendLoadInRackFormStoreByPosWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterRackMoveWaitLoadInRackFormStoreByPosResult(resultPackPtr,rackBarMsgPtr,sensorSetPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterRackMoveWaitLoadInRackFormStoreByPosResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//集成指令,往试管架仓中放入一个试管架,参数为仓号
void CanMasterRackMoveSendPushOutRackToStoreByPosWhileReturn(RACK_STORE_HOUSE rackHousePos,SYS_RESULT_PACK* resultPackPtr,RACK_MOVE_SENSOR_SET* sensorSetPtr)
{
    //发送指令等待ACK
    CanMasterRackMoveSendPushOutRackToStoreByPosWhileAck(rackHousePos,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterRackMoveSendPushOutRackToStoreByPosWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterRackMoveWaitPushOutRackToStoreByPosResult(resultPackPtr,sensorSetPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterRackMoveWaitPushOutRackToStoreByPosResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return ;
}

//集成指令,从C21接口装载一个试管架
void CanMasterRackMoveSendLoadInRackFormC21WhileReturn(SYS_RESULT_PACK* resultPackPtr,RACK_MOVE_SENSOR_SET* sensorSetPtr)
{
    //发送指令等待ACK
    CanMasterRackMoveSendLoadInRackFormC21WhileAck(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterRackMoveSendLoadInRackFormC21WhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return ;
    }
    //等待结果
    CanMasterRackMoveWaitLoadInRackFormC21Result(resultPackPtr,sensorSetPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterRackMoveWaitLoadInRackFormC21Result ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return ;
}

//集成指令,往C21接口放入一个试管架
void CanMasterRackMoveSendPushOutRackToC21WhileReturn(SYS_RESULT_PACK* resultPackPtr,RACK_MOVE_SENSOR_SET* sensorSetPtr)
{
    //发送指令等待ACK
    CanMasterRackMoveSendPushOutRackToC21WhileAck(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterRackMoveSendPushOutRackToC21WhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterRackMoveWaitPushOutRackToC21Result(resultPackPtr,sensorSetPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterRackMoveWaitPushOutRackToC21Result ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//集成指令,从试管仓中取出一个试管架放入另一个试管仓中
void CanMasterRackMoveSendLoadInRackFormStoreByPosToStoreByPosWhileReturn(RACK_STORE_HOUSE rackSrcHousePos,RACK_STORE_HOUSE rackTargetHousePos,
                                                                                FunctionalState barScanSet,SYS_RESULT_PACK* resultPackPtr,RACK_BAR_MSG* rackBarMsgPtr,
                                                                                RACK_MOVE_SENSOR_SET* sensorSetPtr)
{
    //发送指令等待ACK
    CanMasterRackMoveSendLoadInRackFormStoreByPosToStoreByPosWhileAck(rackSrcHousePos,rackTargetHousePos,barScanSet,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterRackMoveSendLoadInRackFormStoreByPosToStoreByPosWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return ;
    }
    //等待结果
    CanMasterRackMoveWaitLoadInRackFormStoreByPosToStoreByPosResult(resultPackPtr,rackBarMsgPtr,sensorSetPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterRackMoveWaitLoadInRackFormStoreByPosToStoreByPosResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return ;
}

//集成指令,从试管仓中取出一个试管架放入C21中
void CanMasterRackMoveSendLoadInRackFormStoreByPosToC21WhileReturn(RACK_STORE_HOUSE rackSrcHousePos,FunctionalState barScanSet,SYS_RESULT_PACK* resultPackPtr,RACK_BAR_MSG* rackBarMsgPtr,
                                                                    RACK_MOVE_SENSOR_SET* sensorSetPtr)
{
    //发送指令等待ACK
    CanMasterRackMoveSendLoadInRackFormStoreByPosToC21WhileAck(rackSrcHousePos,barScanSet,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterRackMoveSendLoadInRackFormStoreByPosToC21WhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterRackMoveWaitLoadInRackFormStoreByPosToC21Result(resultPackPtr,rackBarMsgPtr,sensorSetPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterRackMoveWaitLoadInRackFormStoreByPosToC21Result ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//从C21中取出一个试管架放入试管架仓中
void CanMasterRackMoveSendLoadInRackFormC21ToRackStoreWhileReturn(RACK_STORE_HOUSE rackTargetHousePos,SYS_RESULT_PACK* resultPackPtr,RACK_MOVE_SENSOR_SET* sensorSetPtr)
{
    //发送指令等待ACK
    CanMasterRackMoveSendLoadInRackFormC21ToRackStoreWhileAck(rackTargetHousePos,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterRackMoveSendLoadInRackFormC21ToRackStoreWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterRackMoveWaitLoadInRackFormC21ToRackStoreResult(resultPackPtr,sensorSetPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterRackMoveWaitLoadInRackFormC21ToRackStoreResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//C21校准启动
void CanMasterRackMoveC21AdjustStartWhileReturn(SYS_RESULT_PACK* resultPackPtr)
{
    //发送指令等待ACK
    CanMasterRackMoveC21AdjustStartWhileAck(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterRackMoveC21AdjustStartWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return ;
    }
    //等待结果
    CanMasterRackMoveWaitC21AdjustStartResult(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterRackMoveWaitC21AdjustStartResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//C21校准走步数
void CanMasterRackMoveC21AdjustRunWhileReturn(int32_t targetCoordinate,SYS_RESULT_PACK* resultPackPtr)
{
    //发送指令等待ACK
    CanMasterRackMoveC21AdjustRunWhileAck(targetCoordinate,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterRackMoveC21AdjustRunWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterRackMoveWaitC21AdjustRunResult(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterRackMoveWaitC21AdjustRunResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//C21校准完成
void CanMasterRackMoveC21AdjustEndWhileReturn(int32_t targetCoordinate,SYS_RESULT_PACK* resultPackPtr)
{
    //发送指令等待ACK
    CanMasterRackMoveC21AdjustEndWhileAck(targetCoordinate,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterRackMoveC21AdjustEndWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterRackMoveWaitC21AdjustEndResult(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterRackMoveWaitC21AdjustEndResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}



