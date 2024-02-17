/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-22 09:20:50 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-29 21:15:05 +0800
************************************************************************************************/ 
#include "CanMasterCupTrayManage.h"
#include "DriverConfigSystem.h"
#include SYS_LOG_INCLUDE_FILE

//通道的模块号和板号定义
#define CAN_MODULE_NO_CUP_TRAY_MANAGE        CAN_MODULE_NO_BOARD2
#define CAN_BOARD_ID_CUP_TRAY_MANAGE         CAN_BOARD_ID_BOARD2

//通道配置数组
//参数分别是 指令  是否需要结果  超时时间  返回最小参数长度 附带参数长度
//附带参数长度为不定长的时候,指示参数长度为0,由上级调用者手动申请内存
static const CAN_PROT_CMD_CONFIG canMasterCupTrayManageCmdConfigArray[] = {
    {CAN_MASTER_CMD_H360_CUP_TRAY_MANAGE_PORT_IN                                            , ENABLE, 3000 ,  2,  1 },//模块内输入读取
    {CAN_MASTER_CMD_H360_CUP_TRAY_MANAGE_PORT_OUT                                           , ENABLE, 3000 ,  1,  2 },//模块内输出写入
    {CAN_MASTER_CMD_H360_CUP_TRAY_MANAGE_MOTOR_RESET                                        , ENABLE, 60000,  5,  1 },//模块内电机复位
    {CAN_MASTER_CMD_H360_CUP_TRAY_MANAGE_MOTOR_RUN_STEPS                                    , ENABLE, 60000,  5,  5 },//模块内电机走步数
    {CAN_MASTER_CMD_H360_CUP_TRAY_MANAGE_SET_DOOR_STATE                                     , ENABLE, 5000 ,  0,  1 },//设置杯栈门状态
    {CAN_MASTER_CMD_H360_CUP_TRAY_MANAGE_NEW_STACK_INIT                                     , ENABLE, 20000,  6,  0 },//新杯盘栈初始化
    {CAN_MASTER_CMD_H360_CUP_TRAY_MANAGE_EMPTY_STACK_INIT                                   , ENABLE, 20000,  6,  0 },//空杯盘栈初始化
    {CAN_MASTER_CMD_H360_CUP_TRAY_MANAGE_PUSH_INIT                                          , ENABLE, 20000,  6,  0 },//推手初始化
    {CAN_MASTER_CMD_H360_CUP_TRAY_MANAGE_CLAMP_INIT                                         , ENABLE, 20000,  6,  0 },//夹手初始化
    {CAN_MASTER_CMD_H360_CUP_TRAY_MANAGE_GARBAGE_DOWN_UNLOCK                                , ENABLE, 20000,  6,  1 },//垃圾桶下降
    {CAN_MASTER_CMD_H360_CUP_TRAY_MANAGE_GARBEGE_UP_LOCK                                    , ENABLE, 20000,  6,  1 },//垃圾桶上升
    {CAN_MASTER_CMD_H360_CUP_TRAY_MANAGE_INIT_ALL                                           , ENABLE, 60000,  6,  0 },//杯栈模块整体初始化
    {CAN_MASTER_CMD_H360_CUP_TRAY_MANAGE_CLAMP_OPEN                                         , ENABLE, 10000,  6,  0 },//夹手释放
    {CAN_MASTER_CMD_H360_CUP_TRAY_MANAGE_CLAMP_CLOSE                                        , ENABLE, 10000,  6,  0 },//夹手夹紧
    {CAN_MASTER_CMD_H360_CUP_TRAY_MANAGE_UPLOAD_NEW_TRAY                                    , ENABLE, 20000,  6,  0 },//新杯盘栈上传一个新盘
    {CAN_MASTER_CMD_H360_CUP_TRAY_MANAGE_RECYCLE_EMPTY_TRAY                                 , ENABLE, 20000,  6,  0 },//空杯盘栈从测试区回收一个空盘
    {CAN_MASTER_CMD_H360_CUP_TRAY_MANAGE_PUSH_NEW_TRAY_TO_TEST                              , ENABLE, 10000,  6,  0 },//推手推一个新盘到测试位
    {CAN_MASTER_CMD_H360_CUP_TRAY_MANAGE_NEW_STACK_UP_SPECIAL_LAYER_WITH_DOOR_OPEN_STATE    , ENABLE, 10000,  6,  1 },//新杯盘栈上升指定层(门开到位状态)
    {CAN_MASTER_CMD_H360_CUP_TRAY_MANAGE_NEW_STACK_RETURN_ZERO                              , ENABLE, 10000,  6,  0 },//新杯盘栈回零(门开到位状态)
    {CAN_MASTER_CMD_H360_CUP_TRAY_MANAGE_EMPTY_STACK_UP_SPECIAL_LAYER_WITH_DOOR_OPEN_STATE  , ENABLE, 10000,  6,  1 },//空杯盘上升指定层(门开到位状态)
    {CAN_MASTER_CMD_H360_CUP_TRAY_MANAGE_EMPTY_STACK_RETURN_ZERO                            , ENABLE, 10000,  6,  0 },//空杯盘栈回零(门开到位状态)
    {CAN_MASTER_CMD_H360_CUP_TRAY_MANAGE_GET_MSG_SET                                        , ENABLE, 5000 ,  6,  0 },//获取杯栈信息集合
};

//将获取到的数据转换为结构体
static void CanMasterCupTrayManageGetMsgSet(uint8_t* buf,CUP_TRAY_MANAGE_MSG* msgSetPtr)
{
    msgSetPtr->newCupTrayCount = buf[0];
    msgSetPtr->emptyTrayCount = buf[1];
    msgSetPtr->newTraySensorState = (PIN_STATE)buf[2];
    msgSetPtr->testTraySensorState = (PIN_STATE)buf[3];
    msgSetPtr->garbage1SensorState = (PIN_STATE)buf[4];
    msgSetPtr->garbage2SensorState = (PIN_STATE)buf[5];
}

/*************************************发送指令等待ACK************************************************/
//模块内输入读取
void CanMasterCupTrayManageReadSingleWhileAck(uint8_t modInIdx,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_CUP_TRAY_MANAGE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_CUP_TRAY_MANAGE_PORT_IN;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterCupTrayManageCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterCupTrayManageCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)modInIdx;
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//模块内输出写入
void CanMasterCupTrayManageWriteSingleWhileAck(uint8_t modOutIdx,PIN_STATE pinState,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_CUP_TRAY_MANAGE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_CUP_TRAY_MANAGE_PORT_OUT;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterCupTrayManageCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterCupTrayManageCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)modOutIdx;
    canCmdSendPtr->cmdBufferPtr[1] = (uint8_t)pinState;
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//模块内电机复位
void CanMasterCupTrayManageResetMotorWhileAck(uint8_t modMotorIdx,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_CUP_TRAY_MANAGE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_CUP_TRAY_MANAGE_MOTOR_RESET;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterCupTrayManageCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterCupTrayManageCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)modMotorIdx;
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//模块内电机走位
void CanMasterCupTrayManageRunStepsMotorWhileAck(uint8_t modMotorIdx,int32_t steps,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_CUP_TRAY_MANAGE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_CUP_TRAY_MANAGE_MOTOR_RUN_STEPS;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterCupTrayManageCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterCupTrayManageCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)modMotorIdx;
    ProtUtilWriteInt32ToBuffer(steps,canCmdSendPtr->cmdBufferPtr+1);
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//设置杯栈门状态
void CanMasterCupTrayManageSetDoorStateWhileAck(CUP_TRAY_MANAGE_DOOR_STATE doorState,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_CUP_TRAY_MANAGE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_CUP_TRAY_MANAGE_SET_DOOR_STATE;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterCupTrayManageCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterCupTrayManageCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)doorState;
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//新杯盘栈初始化
void CanMasterCupTrayManageNewStackInitWhileAck(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_CUP_TRAY_MANAGE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_CUP_TRAY_MANAGE_NEW_STACK_INIT;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterCupTrayManageCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterCupTrayManageCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    asm("nop");
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//空杯盘栈初始化
void CanMasterCupTrayManageEmptyStackInitWhileAck(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_CUP_TRAY_MANAGE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_CUP_TRAY_MANAGE_EMPTY_STACK_INIT;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterCupTrayManageCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterCupTrayManageCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    asm("nop");
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//推手初始化
void CanMasterCupTrayManagePushInitWhileAck(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_CUP_TRAY_MANAGE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_CUP_TRAY_MANAGE_PUSH_INIT;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterCupTrayManageCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterCupTrayManageCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    asm("nop");
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//夹手初始化
void CanMasterCupTrayManageClampInitWhileAck(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_CUP_TRAY_MANAGE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_CUP_TRAY_MANAGE_CLAMP_INIT;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterCupTrayManageCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterCupTrayManageCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    asm("nop");
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//垃圾桶下降
void CanMasterCupTrayManageGarbageDownUnlockWhileAck(GARBAGE_SELECT garbageNo,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_CUP_TRAY_MANAGE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_CUP_TRAY_MANAGE_GARBAGE_DOWN_UNLOCK;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterCupTrayManageCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterCupTrayManageCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)garbageNo;
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//垃圾桶上升
void CanMasterCupTrayManageGarbegeUpLockWhileAck(GARBAGE_SELECT garbageNo,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_CUP_TRAY_MANAGE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_CUP_TRAY_MANAGE_GARBEGE_UP_LOCK;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterCupTrayManageCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterCupTrayManageCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)garbageNo;
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//杯栈模块整体初始化
void CanMasterCupTrayManageInitAllWhileAck(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_CUP_TRAY_MANAGE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_CUP_TRAY_MANAGE_INIT_ALL;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterCupTrayManageCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterCupTrayManageCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    asm("nop");
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//夹手释放
void CanMasterCupTrayManageClampOpenWhileAck(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_CUP_TRAY_MANAGE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_CUP_TRAY_MANAGE_CLAMP_OPEN;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterCupTrayManageCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterCupTrayManageCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    asm("nop");
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//夹手夹紧
void CanMasterCupTrayManageClampCloseWhileAck(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_CUP_TRAY_MANAGE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_CUP_TRAY_MANAGE_CLAMP_CLOSE;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterCupTrayManageCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterCupTrayManageCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    asm("nop");
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//新杯盘栈上传一个新盘
void CanMasterCupTrayManageUploadNewTrayWhileAck(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_CUP_TRAY_MANAGE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_CUP_TRAY_MANAGE_UPLOAD_NEW_TRAY;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterCupTrayManageCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterCupTrayManageCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    asm("nop");
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//空杯盘栈从测试区回收一个空盘
void CanMasterCupTrayManageRecycleEmptyTrayWhileAck(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_CUP_TRAY_MANAGE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_CUP_TRAY_MANAGE_RECYCLE_EMPTY_TRAY;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterCupTrayManageCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterCupTrayManageCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    asm("nop");
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//推手推一个新盘到测试位
void CanMasterCupTrayManagePushNewTrayToTestWhileAck(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_CUP_TRAY_MANAGE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_CUP_TRAY_MANAGE_PUSH_NEW_TRAY_TO_TEST;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterCupTrayManageCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterCupTrayManageCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    asm("nop");
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//新杯盘栈上升指定层(门开到位状态)
void CanMasterCupTrayManage_NewStackUpSpecialLayerWithDoorOpenStateWhileAck(uint8_t layers,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_CUP_TRAY_MANAGE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_CUP_TRAY_MANAGE_NEW_STACK_UP_SPECIAL_LAYER_WITH_DOOR_OPEN_STATE;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterCupTrayManageCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterCupTrayManageCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)layers;
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//新杯盘栈回零(门开到位状态)
void CanMasterCupTrayManageNewStackReturnZeroWhileAck(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_CUP_TRAY_MANAGE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_CUP_TRAY_MANAGE_NEW_STACK_RETURN_ZERO;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterCupTrayManageCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterCupTrayManageCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    asm("nop");
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//空杯盘上升指定层(门开到位状态)
void CanMasterCupTrayManageEmptyStackUpSpecialLayerWithDoorOpenStateWhileAck(uint8_t layers,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_CUP_TRAY_MANAGE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_CUP_TRAY_MANAGE_EMPTY_STACK_UP_SPECIAL_LAYER_WITH_DOOR_OPEN_STATE;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterCupTrayManageCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterCupTrayManageCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)layers;
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//空杯盘栈回零(门开到位状态)
void CanMasterCupTrayManageEmptyStackReturnZeroWhileAck(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_CUP_TRAY_MANAGE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_CUP_TRAY_MANAGE_EMPTY_STACK_RETURN_ZERO;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterCupTrayManageCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterCupTrayManageCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    asm("nop");
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//获取杯栈信息集合
void CanMasterCupTrayManageReadMsgSetWhileAck(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_CUP_TRAY_MANAGE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_CUP_TRAY_MANAGE_GET_MSG_SET;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterCupTrayManageCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterCupTrayManageCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    asm("nop");
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}


/*************************************等待指定指令完成返回************************************************/
//模块内输入读取
void CanMasterCupTrayManageWaitReadSingleResult(SYS_RESULT_PACK* resultPackPtr,PIN_STATE* pinStatePtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_CUP_TRAY_MANAGE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_CUP_TRAY_MANAGE_PORT_IN;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //参数解析
    if(pinStatePtr != NULL)
    {
        //结果解析
        *pinStatePtr = PIN_STATE_INVALID;
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            *pinStatePtr = (PIN_STATE)(returnDataBufferPtr[1]);
        }
    }
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//模块内输出写入
void CanMasterCupTrayManageWaitWriteSingleResult(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_CUP_TRAY_MANAGE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_CUP_TRAY_MANAGE_PORT_OUT;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //参数解析
    asm("nop");
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//模块内电机复位
void CanMasterCupTrayManageWaitResetMotorResult(SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_CUP_TRAY_MANAGE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_CUP_TRAY_MANAGE_MOTOR_RESET;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //参数解析
    if(motorCurPosPtr != NULL)
    {
        //结果解析
        *motorCurPosPtr = 0;
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            *motorCurPosPtr = ProtUtilReadInt32Buffer(returnDataBufferPtr+1);
        }
    }
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//模块内电机走位
void CanMasterCupTrayManageWaitRunStepsMotorResult(SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_CUP_TRAY_MANAGE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_CUP_TRAY_MANAGE_MOTOR_RUN_STEPS;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //参数解析
    if(motorCurPosPtr != NULL)
    {
        //结果解析
        *motorCurPosPtr = 0;
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            *motorCurPosPtr = ProtUtilReadInt32Buffer(returnDataBufferPtr+1);
        }
    }
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//设置杯栈门状态
void CanMasterCupTrayManageWaitSetDoorStateResult(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_CUP_TRAY_MANAGE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_CUP_TRAY_MANAGE_SET_DOOR_STATE;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //参数解析
    asm("nop");
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//新杯盘栈初始化
void CanMasterCupTrayManageWaitNewStackInitResult(SYS_RESULT_PACK* resultPackPtr,CUP_TRAY_MANAGE_MSG* msgSetPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_CUP_TRAY_MANAGE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_CUP_TRAY_MANAGE_NEW_STACK_INIT;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //参数解析
    if(msgSetPtr != NULL)
    {
        //结果解析
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            CanMasterCupTrayManageGetMsgSet(returnDataBufferPtr,msgSetPtr);
        }
    }
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//空杯盘栈初始化
void CanMasterCupTrayManageWaitEmptyStackInitResult(SYS_RESULT_PACK* resultPackPtr,CUP_TRAY_MANAGE_MSG* msgSetPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_CUP_TRAY_MANAGE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_CUP_TRAY_MANAGE_EMPTY_STACK_INIT;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //参数解析
    if(msgSetPtr != NULL)
    {
        //结果解析
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            CanMasterCupTrayManageGetMsgSet(returnDataBufferPtr,msgSetPtr);
        }
    }
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//推手初始化
void CanMasterCupTrayManageWaitPushInitResult(SYS_RESULT_PACK* resultPackPtr,CUP_TRAY_MANAGE_MSG* msgSetPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_CUP_TRAY_MANAGE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_CUP_TRAY_MANAGE_PUSH_INIT;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //参数解析
    if(msgSetPtr != NULL)
    {
        //结果解析
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            CanMasterCupTrayManageGetMsgSet(returnDataBufferPtr,msgSetPtr);
        }
    }
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//夹手初始化
void CanMasterCupTrayManageWaitClampInitResult(SYS_RESULT_PACK* resultPackPtr,CUP_TRAY_MANAGE_MSG* msgSetPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_CUP_TRAY_MANAGE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_CUP_TRAY_MANAGE_CLAMP_INIT;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //参数解析
    if(msgSetPtr != NULL)
    {
        //结果解析
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            CanMasterCupTrayManageGetMsgSet(returnDataBufferPtr,msgSetPtr);
        }
    }
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//垃圾桶下降
void CanMasterCupTrayManageWaitGarbageDownUnlockResult(SYS_RESULT_PACK* resultPackPtr,CUP_TRAY_MANAGE_MSG* msgSetPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_CUP_TRAY_MANAGE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_CUP_TRAY_MANAGE_GARBAGE_DOWN_UNLOCK;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //参数解析
    if(msgSetPtr != NULL)
    {
        //结果解析
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            CanMasterCupTrayManageGetMsgSet(returnDataBufferPtr,msgSetPtr);
        }
    }
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//垃圾桶上升
void CanMasterCupTrayManageWaitGarbegeUpLockResult(SYS_RESULT_PACK* resultPackPtr,CUP_TRAY_MANAGE_MSG* msgSetPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_CUP_TRAY_MANAGE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_CUP_TRAY_MANAGE_GARBEGE_UP_LOCK;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //参数解析
    if(msgSetPtr != NULL)
    {
        //结果解析
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            CanMasterCupTrayManageGetMsgSet(returnDataBufferPtr,msgSetPtr);
        }
    }
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//杯栈模块整体初始化
void CanMasterCupTrayManageWaitInitAllResult(SYS_RESULT_PACK* resultPackPtr,CUP_TRAY_MANAGE_MSG* msgSetPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_CUP_TRAY_MANAGE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_CUP_TRAY_MANAGE_INIT_ALL;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //参数解析
    if(msgSetPtr != NULL)
    {
        //结果解析
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            CanMasterCupTrayManageGetMsgSet(returnDataBufferPtr,msgSetPtr);
        }
    }
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//夹手释放
void CanMasterCupTrayManageWaitClampOpenResult(SYS_RESULT_PACK* resultPackPtr,CUP_TRAY_MANAGE_MSG* msgSetPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_CUP_TRAY_MANAGE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_CUP_TRAY_MANAGE_CLAMP_OPEN;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //参数解析
    if(msgSetPtr != NULL)
    {
        //结果解析
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            CanMasterCupTrayManageGetMsgSet(returnDataBufferPtr,msgSetPtr);
        }
    }
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}   

//夹手夹紧
void CanMasterCupTrayManageWaitClampCloseResult(SYS_RESULT_PACK* resultPackPtr,CUP_TRAY_MANAGE_MSG* msgSetPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_CUP_TRAY_MANAGE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_CUP_TRAY_MANAGE_CLAMP_CLOSE;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //参数解析
    if(msgSetPtr != NULL)
    {
        //结果解析
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            CanMasterCupTrayManageGetMsgSet(returnDataBufferPtr,msgSetPtr);
        }
    }
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//新杯盘栈上传一个新盘
void CanMasterCupTrayManageWaitUploadNewTrayResult(SYS_RESULT_PACK* resultPackPtr,CUP_TRAY_MANAGE_MSG* msgSetPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_CUP_TRAY_MANAGE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_CUP_TRAY_MANAGE_UPLOAD_NEW_TRAY;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //参数解析
    if(msgSetPtr != NULL)
    {
        //结果解析
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            CanMasterCupTrayManageGetMsgSet(returnDataBufferPtr,msgSetPtr);
        }
    }
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//空杯盘栈从测试区回收一个空盘
void CanMasterCupTrayManageWaitRecycleEmptyTrayResult(SYS_RESULT_PACK* resultPackPtr,CUP_TRAY_MANAGE_MSG* msgSetPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_CUP_TRAY_MANAGE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_CUP_TRAY_MANAGE_RECYCLE_EMPTY_TRAY;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //参数解析
    if(msgSetPtr != NULL)
    {
        //结果解析
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            CanMasterCupTrayManageGetMsgSet(returnDataBufferPtr,msgSetPtr);
        }
    }
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//推手推一个新盘到测试位
void CanMasterCupTrayManageWaitPushNewTrayToTestResult(SYS_RESULT_PACK* resultPackPtr,CUP_TRAY_MANAGE_MSG* msgSetPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_CUP_TRAY_MANAGE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_CUP_TRAY_MANAGE_PUSH_NEW_TRAY_TO_TEST;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //参数解析
    if(msgSetPtr != NULL)
    {
        //结果解析
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            CanMasterCupTrayManageGetMsgSet(returnDataBufferPtr,msgSetPtr);
        }
    }
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//新杯盘栈上升指定层(门开到位状态)
void CanMasterCupTrayManageWaitNewStackUpSpecialLayerWithDoorOpenStateResult(SYS_RESULT_PACK* resultPackPtr,CUP_TRAY_MANAGE_MSG* msgSetPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_CUP_TRAY_MANAGE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_CUP_TRAY_MANAGE_NEW_STACK_UP_SPECIAL_LAYER_WITH_DOOR_OPEN_STATE;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //参数解析
    if(msgSetPtr != NULL)
    {
        //结果解析
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            CanMasterCupTrayManageGetMsgSet(returnDataBufferPtr,msgSetPtr);
        }
    }
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//新杯盘栈回零(门开到位状态)
void CanMasterCupTrayManageWaitNewStackReturnZeroResult(SYS_RESULT_PACK* resultPackPtr,CUP_TRAY_MANAGE_MSG* msgSetPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_CUP_TRAY_MANAGE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_CUP_TRAY_MANAGE_NEW_STACK_RETURN_ZERO;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //参数解析
    if(msgSetPtr != NULL)
    {
        //结果解析
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            CanMasterCupTrayManageGetMsgSet(returnDataBufferPtr,msgSetPtr);
        }
    }
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//空杯盘上升指定层(门开到位状态)
void CanMasterCupTrayManageWaitEmptyStackUpSpecialLayerWithDoorOpenStateResult(SYS_RESULT_PACK* resultPackPtr,CUP_TRAY_MANAGE_MSG* msgSetPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_CUP_TRAY_MANAGE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_CUP_TRAY_MANAGE_EMPTY_STACK_UP_SPECIAL_LAYER_WITH_DOOR_OPEN_STATE;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //参数解析
    if(msgSetPtr != NULL)
    {
        //结果解析
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            CanMasterCupTrayManageGetMsgSet(returnDataBufferPtr,msgSetPtr);
        }
    }
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//空杯盘栈回零(门开到位状态)
void CanMasterCupTrayManageWaitEmptyStackReturnZeroResult(SYS_RESULT_PACK* resultPackPtr,CUP_TRAY_MANAGE_MSG* msgSetPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_CUP_TRAY_MANAGE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_CUP_TRAY_MANAGE_EMPTY_STACK_RETURN_ZERO;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //参数解析
    if(msgSetPtr != NULL)
    {
        //结果解析
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            CanMasterCupTrayManageGetMsgSet(returnDataBufferPtr,msgSetPtr);
        }
    }
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//获取杯栈信息集合
void CanMasterCupTrayManageWaitReadMsgSetResult(SYS_RESULT_PACK* resultPackPtr,CUP_TRAY_MANAGE_MSG* msgSetPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_CUP_TRAY_MANAGE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_CUP_TRAY_MANAGE_GET_MSG_SET;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //参数解析
    if(msgSetPtr != NULL)
    {
        //结果解析
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            CanMasterCupTrayManageGetMsgSet(returnDataBufferPtr,msgSetPtr);
        }
    }
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_CUP_TRAY_MANAGE,CAN_BOARD_ID_CUP_TRAY_MANAGE,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

/*************************************发送指令等待完成返回************************************************/
//模块内输入读取
void CanMasterCupTrayManageReadSingleWhileReturn(uint8_t modInIdx,SYS_RESULT_PACK* resultPackPtr,PIN_STATE* pinStatePtr)
{
    //发送指令等待ACK
    CanMasterCupTrayManageReadSingleWhileAck(modInIdx,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterCupTrayManageReadSingleWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterCupTrayManageWaitReadSingleResult(resultPackPtr,pinStatePtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterCupTrayManageWaitReadSingleResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//模块内输出写入
void CanMasterCupTrayManageWriteSingleWhileReturn(uint8_t modOutIdx,PIN_STATE pinState,SYS_RESULT_PACK* resultPackPtr)
{
    //发送指令等待ACK
    CanMasterCupTrayManageWriteSingleWhileAck(modOutIdx,pinState,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterCupTrayManageWriteSingleWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterCupTrayManageWaitWriteSingleResult(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterCupTrayManageWaitWriteSingleResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//模块内电机复位
void CanMasterCupTrayManageResetMotorWhileReturn(uint8_t modMotorIdx,SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr)
{
    //发送指令等待ACK
    CanMasterCupTrayManageResetMotorWhileAck(modMotorIdx,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterCupTrayManageResetMotorWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterCupTrayManageWaitResetMotorResult(resultPackPtr,motorCurPosPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterCupTrayManageWaitResetMotorResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//模块内电机走位
void CanMasterCupTrayManageRunStepsMotorWhileReturn(uint8_t modMotorIdx,int32_t steps,SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr)
{
    //发送指令等待ACK
    CanMasterCupTrayManageRunStepsMotorWhileAck(modMotorIdx,steps,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterCupTrayManageRunStepsMotorWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterCupTrayManageWaitRunStepsMotorResult(resultPackPtr,motorCurPosPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterCupTrayManageWaitRunStepsMotorResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//设置杯栈门状态
void CanMasterCupTrayManageSetDoorStateWhileReturn(CUP_TRAY_MANAGE_DOOR_STATE doorState,SYS_RESULT_PACK* resultPackPtr)
{
    //发送指令等待ACK
    CanMasterCupTrayManageSetDoorStateWhileAck(doorState,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterCupTrayManageSetDoorStateWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterCupTrayManageWaitSetDoorStateResult(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterCupTrayManageWaitSetDoorStateResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//新杯盘栈初始化
void CanMasterCupTrayManageNewStackInitWhileReturn(SYS_RESULT_PACK* resultPackPtr,CUP_TRAY_MANAGE_MSG* msgSetPtr)
{
    //发送指令等待ACK
    CanMasterCupTrayManageNewStackInitWhileAck(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterCupTrayManageNewStackInitWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterCupTrayManageWaitNewStackInitResult(resultPackPtr,msgSetPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterCupTrayManageWaitNewStackInitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//空杯盘栈初始化
void CanMasterCupTrayManageEmptyStackInitWhileReturn(SYS_RESULT_PACK* resultPackPtr,CUP_TRAY_MANAGE_MSG* msgSetPtr)
{
    //发送指令等待ACK
    CanMasterCupTrayManageEmptyStackInitWhileAck(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterCupTrayManageEmptyStackInitWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterCupTrayManageWaitEmptyStackInitResult(resultPackPtr,msgSetPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterCupTrayManageWaitEmptyStackInitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//推手初始化
void CanMasterCupTrayManagePushInitWhileReturn(SYS_RESULT_PACK* resultPackPtr,CUP_TRAY_MANAGE_MSG* msgSetPtr)
{
    //发送指令等待ACK
    CanMasterCupTrayManagePushInitWhileAck(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterCupTrayManagePushInitWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterCupTrayManageWaitPushInitResult(resultPackPtr,msgSetPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterCupTrayManageWaitPushInitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//夹手初始化
void CanMasterCupTrayManageClampInitWhileReturn(SYS_RESULT_PACK* resultPackPtr,CUP_TRAY_MANAGE_MSG* msgSetPtr)
{
    //发送指令等待ACK
    CanMasterCupTrayManageClampInitWhileAck(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterCupTrayManageClampInitWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterCupTrayManageWaitClampInitResult(resultPackPtr,msgSetPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterCupTrayManageWaitClampInitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//垃圾桶下降
void CanMasterCupTrayManageGarbageDownUnlockWhileReturn(GARBAGE_SELECT garbageNo,SYS_RESULT_PACK* resultPackPtr,CUP_TRAY_MANAGE_MSG* msgSetPtr)
{
    //发送指令等待ACK
    CanMasterCupTrayManageGarbageDownUnlockWhileAck(garbageNo,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterCupTrayManageGarbageDownUnlockWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterCupTrayManageWaitGarbageDownUnlockResult(resultPackPtr,msgSetPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterCupTrayManageWaitGarbageDownUnlockResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//垃圾桶上升
void CanMasterCupTrayManageGarbegeUpLockWhileReturn(GARBAGE_SELECT garbageNo,SYS_RESULT_PACK* resultPackPtr,CUP_TRAY_MANAGE_MSG* msgSetPtr)
{
    //发送指令等待ACK
    CanMasterCupTrayManageGarbegeUpLockWhileAck(garbageNo,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterCupTrayManageGarbegeUpLockWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterCupTrayManageWaitGarbegeUpLockResult(resultPackPtr,msgSetPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterCupTrayManageWaitGarbegeUpLockResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//杯栈模块整体初始化
void CanMasterCupTrayManageInitAllWhileReturn(SYS_RESULT_PACK* resultPackPtr,CUP_TRAY_MANAGE_MSG* msgSetPtr)
{
    //发送指令等待ACK
    CanMasterCupTrayManageInitAllWhileAck(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterCupTrayManageInitAllWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterCupTrayManageWaitInitAllResult(resultPackPtr,msgSetPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterCupTrayManageWaitInitAllResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//夹手释放
void CanMasterCupTrayManageClampOpenWhileReturn(SYS_RESULT_PACK* resultPackPtr,CUP_TRAY_MANAGE_MSG* msgSetPtr)
{
    //发送指令等待ACK
    CanMasterCupTrayManageClampOpenWhileAck(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterCupTrayManageClampOpenWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterCupTrayManageWaitClampOpenResult(resultPackPtr,msgSetPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterCupTrayManageWaitClampOpenResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//夹手夹紧
void CanMasterCupTrayManageClampCloseWhileReturn(SYS_RESULT_PACK* resultPackPtr,CUP_TRAY_MANAGE_MSG* msgSetPtr)
{
    //发送指令等待ACK
    CanMasterCupTrayManageClampCloseWhileAck(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterCupTrayManageClampCloseWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterCupTrayManageWaitClampCloseResult(resultPackPtr,msgSetPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterCupTrayManageWaitClampCloseResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//新杯盘栈上传一个新盘
void CanMasterCupTrayManageUploadNewTrayWhileReturn(SYS_RESULT_PACK* resultPackPtr,CUP_TRAY_MANAGE_MSG* msgSetPtr)
{
    //发送指令等待ACK
    CanMasterCupTrayManageUploadNewTrayWhileAck(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterCupTrayManageUploadNewTrayWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterCupTrayManageWaitUploadNewTrayResult(resultPackPtr,msgSetPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterCupTrayManageWaitUploadNewTrayResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//空杯盘栈从测试区回收一个空盘
void CanMasterCupTrayManageRecycleEmptyTrayWhileReturn(SYS_RESULT_PACK* resultPackPtr,CUP_TRAY_MANAGE_MSG* msgSetPtr)
{
    //发送指令等待ACK
    CanMasterCupTrayManageRecycleEmptyTrayWhileAck(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterCupTrayManageRecycleEmptyTrayWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterCupTrayManageWaitRecycleEmptyTrayResult(resultPackPtr,msgSetPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterCupTrayManageWaitRecycleEmptyTrayResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//推手推一个新盘到测试位
void CanMasterCupTrayManagePushNewTrayToTestWhileReturn(SYS_RESULT_PACK* resultPackPtr,CUP_TRAY_MANAGE_MSG* msgSetPtr)
{
    //发送指令等待ACK
    CanMasterCupTrayManagePushNewTrayToTestWhileAck(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterCupTrayManagePushNewTrayToTestWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterCupTrayManageWaitPushNewTrayToTestResult(resultPackPtr,msgSetPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterCupTrayManageWaitPushNewTrayToTestResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//新杯盘栈上升指定层(门开到位状态)
void CanMasterCupTrayManage_NewStackUpSpecialLayerWithDoorOpenStateWhileReturn(uint8_t layers,SYS_RESULT_PACK* resultPackPtr,CUP_TRAY_MANAGE_MSG* msgSetPtr)
{
    //发送指令等待ACK
    CanMasterCupTrayManage_NewStackUpSpecialLayerWithDoorOpenStateWhileAck(layers,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterCupTrayManage_NewStackUpSpecialLayerWithDoorOpenStateWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterCupTrayManageWaitNewStackUpSpecialLayerWithDoorOpenStateResult(resultPackPtr,msgSetPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterCupTrayManageWaitNewStackUpSpecialLayerWithDoorOpenStateResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//新杯盘栈回零(门开到位状态)
void CanMasterCupTrayManageNewStackReturnZeroWhileReturn(SYS_RESULT_PACK* resultPackPtr,CUP_TRAY_MANAGE_MSG* msgSetPtr)
{
    //发送指令等待ACK
    CanMasterCupTrayManageNewStackReturnZeroWhileAck(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterCupTrayManageNewStackReturnZeroWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterCupTrayManageWaitNewStackReturnZeroResult(resultPackPtr,msgSetPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterCupTrayManageWaitNewStackReturnZeroResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//空杯盘上升指定层(门开到位状态)
void CanMasterCupTrayManageEmptyStackUpSpecialLayerWithDoorOpenStateWhileReturn(uint8_t layers,SYS_RESULT_PACK* resultPackPtr,CUP_TRAY_MANAGE_MSG* msgSetPtr)
{
    //发送指令等待ACK
    CanMasterCupTrayManageEmptyStackUpSpecialLayerWithDoorOpenStateWhileAck(layers,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterCupTrayManageEmptyStackUpSpecialLayerWithDoorOpenStateWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterCupTrayManageWaitEmptyStackUpSpecialLayerWithDoorOpenStateResult(resultPackPtr,msgSetPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterCupTrayManageWaitEmptyStackUpSpecialLayerWithDoorOpenStateResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//空杯盘栈回零(门开到位状态)
void CanMasterCupTrayManageEmptyStackReturnZeroWhileReturn(SYS_RESULT_PACK* resultPackPtr,CUP_TRAY_MANAGE_MSG* msgSetPtr)
{
    //发送指令等待ACK
    CanMasterCupTrayManageEmptyStackReturnZeroWhileAck(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterCupTrayManageEmptyStackReturnZeroWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterCupTrayManageWaitEmptyStackReturnZeroResult(resultPackPtr,msgSetPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterCupTrayManageWaitEmptyStackReturnZeroResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//获取杯栈信息集合
void CanMasterCupTrayManageReadMsgSetWhileReturn(SYS_RESULT_PACK* resultPackPtr,CUP_TRAY_MANAGE_MSG* msgSetPtr)
{
    //发送指令等待ACK
    CanMasterCupTrayManageReadMsgSetWhileAck(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterCupTrayManageReadMsgSetWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterCupTrayManageWaitReadMsgSetResult(resultPackPtr,msgSetPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterCupTrayManageWaitReadMsgSetResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}










