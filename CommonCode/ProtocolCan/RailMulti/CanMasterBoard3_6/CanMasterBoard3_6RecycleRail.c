/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-24 10:44:38 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-24 13:52:03 +0800
************************************************************************************************/ 
#include "CanMasterBoard3_6RecycleRail.h"
#include "DriverConfigSystem.h"
#include SYS_LOG_INCLUDE_FILE

//通道配置数组
//参数分别是 指令  是否需要结果  超时时间  最小参数长度 附带参数长度
//附带参数长度为不定长的时候,指示参数长度为0,由上级调用者手动申请内存
static const CAN_PROT_CMD_CONFIG canMasterBoard3_6RecycleRailCmdConfigArray[] = {
    {CAN_RAIL_MUTIL_CMD_BOARD3_6_RECYCLE_RAIL_READ_INPUT            ,        ENABLE,     60000,      2,      1       },//回收轨道 输入读取
    {CAN_RAIL_MUTIL_CMD_BOARD3_6_RECYCLE_RAIL_WRITE_OUTPUT          ,        ENABLE,     60000,      1,      2       },//回收轨道 输出写出
    {CAN_RAIL_MUTIL_CMD_BOARD3_6_RECYCLE_RAIL_MOTOR_RESET           ,        ENABLE,     60000,      5,      1       },//回收轨道 电机复位
    {CAN_RAIL_MUTIL_CMD_BOARD3_6_RECYCLE_RAIL_MOTOR_RUN_STEP        ,        ENABLE,     60000,      5,      5       },//回收轨道 电机走步数
    {CAN_RAIL_MUTIL_CMD_BOARD3_6_RECYCLE_RAIL_RESET_ALL             ,        ENABLE,     60000,      1,      0       },//回收轨道 复位
    {CAN_RAIL_MUTIL_CMD_BOARD3_6_RECYCLE_RAIL_MOVE_RACK_TO_EXIT     ,        ENABLE,     60000,      1,      0       },//回收轨道 回收架子到出口
    {CAN_RAIL_MUTIL_CMD_BOARD3_6_RECYCLE_RAIL_RECEIVE_RACK          ,        ENABLE,     60000,      1,      0       },//回收轨道 接收架子
    {CAN_RAIL_MUTIL_CMD_BOARD3_6_RECYCLE_RAIL_PUSH_RACK_TO_NEXT     ,        ENABLE,     60000,      0,      0       },//回收轨道 推送架子到下一模块
};

/*************************************发送指令等待ACK************************************************/
//回收轨道 输入读取
void CanMasterBoard3_6RecycleRailReadInputWhileAck(RAIL_MODULE_NO railNo,uint8_t index,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD3_6_RECYCLE_RAIL;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD3_6_RECYCLE_RAIL_READ_INPUT;
    uint8_t targetModule = CAN_MODULE_NO_BOARD3;
    uint8_t targetBoardID = CAN_BOARD_ID_BOARD3;
    CanRailMultiUtilGetRailModuleBoardNo(railNo,&targetModule,&targetBoardID);
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        CAN_PROT_LOG("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterBoard3_6RecycleRailCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterBoard3_6RecycleRailCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        CAN_PROT_LOG("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(targetModule,targetBoardID,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)index;
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//回收轨道 输出写出
void CanMasterBoard3_6RecycleRailWriteOutputWhileAck(RAIL_MODULE_NO railNo,uint8_t index,PIN_STATE outState,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD3_6_RECYCLE_RAIL;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD3_6_RECYCLE_RAIL_WRITE_OUTPUT;
    uint8_t targetModule = CAN_MODULE_NO_BOARD3;
    uint8_t targetBoardID = CAN_BOARD_ID_BOARD3;
    CanRailMultiUtilGetRailModuleBoardNo(railNo,&targetModule,&targetBoardID);
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        CAN_PROT_LOG("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterBoard3_6RecycleRailCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterBoard3_6RecycleRailCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        CAN_PROT_LOG("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(targetModule,targetBoardID,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)index;
    canCmdSendPtr->cmdBufferPtr[1] = (uint8_t)outState;
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//回收轨道 指定步进电机复位
void CanMasterBoard3_6RecycleRailStepMotorResetWhileAck(RAIL_MODULE_NO railNo,uint8_t motorIndex,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD3_6_RECYCLE_RAIL;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD3_6_RECYCLE_RAIL_MOTOR_RESET;
    uint8_t targetModule = CAN_MODULE_NO_BOARD3;
    uint8_t targetBoardID = CAN_BOARD_ID_BOARD3;
    CanRailMultiUtilGetRailModuleBoardNo(railNo,&targetModule,&targetBoardID);
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        CAN_PROT_LOG("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterBoard3_6RecycleRailCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterBoard3_6RecycleRailCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        CAN_PROT_LOG("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(targetModule,targetBoardID,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)motorIndex;
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//回收轨道 电机走步数
void CanMasterBoard3_6RecycleRailStepMotorRunStepsWhileAck(RAIL_MODULE_NO railNo,uint8_t motorIndex,int32_t steps,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD3_6_RECYCLE_RAIL;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD3_6_RECYCLE_RAIL_MOTOR_RUN_STEP;
    uint8_t targetModule = CAN_MODULE_NO_BOARD3;
    uint8_t targetBoardID = CAN_BOARD_ID_BOARD3;
    CanRailMultiUtilGetRailModuleBoardNo(railNo,&targetModule,&targetBoardID);
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        CAN_PROT_LOG("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterBoard3_6RecycleRailCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterBoard3_6RecycleRailCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        CAN_PROT_LOG("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(targetModule,targetBoardID,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)motorIndex;
    ProtUtilWriteInt32ToBuffer(steps,canCmdSendPtr->cmdBufferPtr+1);
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//回收轨道 整体复位
void CanMasterBoard3_6RecycleRailResetAllWhileAck(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD3_6_RECYCLE_RAIL;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD3_6_RECYCLE_RAIL_RESET_ALL;
    uint8_t targetModule = CAN_MODULE_NO_BOARD3;
    uint8_t targetBoardID = CAN_BOARD_ID_BOARD3;
    CanRailMultiUtilGetRailModuleBoardNo(railNo,&targetModule,&targetBoardID);
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        CAN_PROT_LOG("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterBoard3_6RecycleRailCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterBoard3_6RecycleRailCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        CAN_PROT_LOG("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(targetModule,targetBoardID,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    // canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)motorIndex;
    // ProtUtilWriteInt32ToBuffer(steps,canCmdSendPtr->cmdBufferPtr+1);
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//回收轨道 回收架子到出口
void CanMasterBoard3_6RecycleRailMovRackToExitWhileAck(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD3_6_RECYCLE_RAIL;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD3_6_RECYCLE_RAIL_MOVE_RACK_TO_EXIT;
    uint8_t targetModule = CAN_MODULE_NO_BOARD3;
    uint8_t targetBoardID = CAN_BOARD_ID_BOARD3;
    CanRailMultiUtilGetRailModuleBoardNo(railNo,&targetModule,&targetBoardID);
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        CAN_PROT_LOG("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterBoard3_6RecycleRailCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterBoard3_6RecycleRailCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        CAN_PROT_LOG("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(targetModule,targetBoardID,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    // canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)motorIndex;
    // ProtUtilWriteInt32ToBuffer(steps,canCmdSendPtr->cmdBufferPtr+1);
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//回收轨道 接收架子
void CanMasterBoard3_6RecycleRailReceiveRackWhileAck(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD3_6_RECYCLE_RAIL;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD3_6_RECYCLE_RAIL_RECEIVE_RACK;
    uint8_t targetModule = CAN_MODULE_NO_BOARD3;
    uint8_t targetBoardID = CAN_BOARD_ID_BOARD3;
    CanRailMultiUtilGetRailModuleBoardNo(railNo,&targetModule,&targetBoardID);
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        CAN_PROT_LOG("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterBoard3_6RecycleRailCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterBoard3_6RecycleRailCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        CAN_PROT_LOG("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(targetModule,targetBoardID,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    // canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)motorIndex;
    // ProtUtilWriteInt32ToBuffer(steps,canCmdSendPtr->cmdBufferPtr+1);
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//回收轨道 推送架子到下一模块
void CanMasterBoard3_6RecycleRailPushRackToNextWhileAck(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD3_6_RECYCLE_RAIL;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD3_6_RECYCLE_RAIL_PUSH_RACK_TO_NEXT;
    uint8_t targetModule = CAN_MODULE_NO_BOARD3;
    uint8_t targetBoardID = CAN_BOARD_ID_BOARD3;
    CanRailMultiUtilGetRailModuleBoardNo(railNo,&targetModule,&targetBoardID);
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        CAN_PROT_LOG("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterBoard3_6RecycleRailCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterBoard3_6RecycleRailCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        CAN_PROT_LOG("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(targetModule,targetBoardID,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    // canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)motorIndex;
    // ProtUtilWriteInt32ToBuffer(steps,canCmdSendPtr->cmdBufferPtr+1);
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}
/*************************************等待指定指令完成返回************************************************/
//回收轨道 输入读取
void CanMasterBoard3_6RecycleRailWaitReadInputResult(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr,PIN_STATE* sensorStatePtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD3_6_RECYCLE_RAIL;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD3_6_RECYCLE_RAIL_READ_INPUT;
    uint8_t targetModule = CAN_MODULE_NO_BOARD3;
    uint8_t targetBoardID = CAN_BOARD_ID_BOARD3;
    CanRailMultiUtilGetRailModuleBoardNo(railNo,&targetModule,&targetBoardID);
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    if(sensorStatePtr != NULL)
    {
        //结果解析
        *sensorStatePtr = PIN_STATE_INVALID;
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            *sensorStatePtr = (PIN_STATE)(returnDataBufferPtr[0]);
        }
    }
    //内存释放
    CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//回收轨道 输出写出
void CanMasterBoard3_6RecycleRailWaitWriteOutputResult(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr,uint8_t* writeIndex)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD3_6_RECYCLE_RAIL;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD3_6_RECYCLE_RAIL_WRITE_OUTPUT;
    uint8_t targetModule = CAN_MODULE_NO_BOARD3;
    uint8_t targetBoardID = CAN_BOARD_ID_BOARD3;
    CanRailMultiUtilGetRailModuleBoardNo(railNo,&targetModule,&targetBoardID);
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    if(writeIndex != NULL)
    {
        //结果解析
        *writeIndex = 0;
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            *writeIndex = (uint8_t)(returnDataBufferPtr[0]);
        }
    }
    //内存释放
    CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//回收轨道 指定步进电机复位
void CanMasterBoard3_6RecycleRailWaitStepMotorResetResult(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr,MOD_SM_CAN_RESULT *modSmCanResultPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD3_6_RECYCLE_RAIL;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD3_6_RECYCLE_RAIL_MOTOR_RESET;
    uint8_t targetModule = CAN_MODULE_NO_BOARD3;
    uint8_t targetBoardID = CAN_BOARD_ID_BOARD3;
    CanRailMultiUtilGetRailModuleBoardNo(railNo,&targetModule,&targetBoardID);
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    if(modSmCanResultPtr != NULL)
    {
        //结果解析
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            modSmCanResultPtr->motorIndex = returnDataBufferPtr[0];
            modSmCanResultPtr->motorPosCurrent = ProtUtilReadInt32Buffer(returnDataBufferPtr+1);
        }
    }
    //内存释放
    CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//回收轨道 电机走步数
void CanMasterBoard3_6RecycleRailWaitStepMotorRunStepsResult(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr,MOD_SM_CAN_RESULT *modSmCanResultPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD3_6_RECYCLE_RAIL;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD3_6_RECYCLE_RAIL_MOTOR_RUN_STEP;
    uint8_t targetModule = CAN_MODULE_NO_BOARD3;
    uint8_t targetBoardID = CAN_BOARD_ID_BOARD3;
    CanRailMultiUtilGetRailModuleBoardNo(railNo,&targetModule,&targetBoardID);
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    if(modSmCanResultPtr != NULL)
    {
        //结果解析
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            modSmCanResultPtr->motorIndex = returnDataBufferPtr[0];
            modSmCanResultPtr->motorPosCurrent = ProtUtilReadInt32Buffer(returnDataBufferPtr+1);
        }
    }
    //内存释放
    CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//回收轨道 整体复位
void CanMasterBoard3_6RecycleRailWaitResetAllResult(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr,SAMPLE_RACK_STATE *rackExistPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD3_6_RECYCLE_RAIL;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD3_6_RECYCLE_RAIL_RESET_ALL;
    uint8_t targetModule = CAN_MODULE_NO_BOARD3;
    uint8_t targetBoardID = CAN_BOARD_ID_BOARD3;
    CanRailMultiUtilGetRailModuleBoardNo(railNo,&targetModule,&targetBoardID);
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    if(rackExistPtr != NULL)
    {
        //结果解析
        *rackExistPtr = SAMPLE_RACK_STATE_NONE;
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            *rackExistPtr = (SAMPLE_RACK_STATE)(returnDataBufferPtr[0]);
        }
    }
    //内存释放
    CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//回收轨道 回收架子到出口
void CanMasterBoard3_6RecycleRailWaitMovRackToExitResult(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr,SAMPLE_RACK_STATE *rackExistPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD3_6_RECYCLE_RAIL;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD3_6_RECYCLE_RAIL_MOVE_RACK_TO_EXIT;
    uint8_t targetModule = CAN_MODULE_NO_BOARD3;
    uint8_t targetBoardID = CAN_BOARD_ID_BOARD3;
    CanRailMultiUtilGetRailModuleBoardNo(railNo,&targetModule,&targetBoardID);
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    if(rackExistPtr != NULL)
    {
        //结果解析
        *rackExistPtr = SAMPLE_RACK_STATE_NONE;
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            *rackExistPtr = (SAMPLE_RACK_STATE)(returnDataBufferPtr[0]);
        }
    }
    //内存释放
    CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//回收轨道 接收架子
void CanMasterBoard3_6RecycleRailWaitReceiveRackResult(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr,SAMPLE_RACK_STATE *rackEntryPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD3_6_RECYCLE_RAIL;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD3_6_RECYCLE_RAIL_RECEIVE_RACK;
    uint8_t targetModule = CAN_MODULE_NO_BOARD3;
    uint8_t targetBoardID = CAN_BOARD_ID_BOARD3;
    CanRailMultiUtilGetRailModuleBoardNo(railNo,&targetModule,&targetBoardID);
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    if(rackEntryPtr != NULL)
    {
        //结果解析
        *rackEntryPtr = SAMPLE_RACK_STATE_NONE;
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            *rackEntryPtr = (SAMPLE_RACK_STATE)(returnDataBufferPtr[0]);
        }
    }
    //内存释放
    CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//回收轨道 推送架子到下一模块
void CanMasterBoard3_6RecycleRailWaitPushRackToNextResult(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD3_6_RECYCLE_RAIL;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD3_6_RECYCLE_RAIL_PUSH_RACK_TO_NEXT;
    uint8_t targetModule = CAN_MODULE_NO_BOARD3;
    uint8_t targetBoardID = CAN_BOARD_ID_BOARD3;
    CanRailMultiUtilGetRailModuleBoardNo(railNo,&targetModule,&targetBoardID);
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    // if(rackExistPtr != NULL)
    // {
    //     //结果解析
    //     *rackExistPtr = SENSOR_STATE_INVALID;
    //     if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
    //     {
    //         *rackExistPtr = (SENSOR_STATE)(returnDataBufferPtr[0]);
    //     }
    // }
    //内存释放
    CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}
/*************************************发送指令等待完成返回************************************************/
//回收轨道 输入读取
void CanMasterBoard3_6RecycleRailReadInputWhileReturn(RAIL_MODULE_NO railNo,uint8_t index,SYS_RESULT_PACK* resultPackPtr,PIN_STATE* sensorStatePtr)
{
    //发送指令等待ACK
    CanMasterBoard3_6RecycleRailReadInputWhileAck(railNo,index,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard3_6RecycleRailReadInputWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterBoard3_6RecycleRailWaitReadInputResult(railNo,resultPackPtr,sensorStatePtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard3_6RecycleRailWaitReadInputResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//回收轨道 输出写出
void CanMasterBoard3_6RecycleRailWriteOutputWhileReturn(RAIL_MODULE_NO railNo,uint8_t index,PIN_STATE outState,SYS_RESULT_PACK* resultPackPtr,uint8_t* writeIndex)
{
    //发送指令等待ACK
    CanMasterBoard3_6RecycleRailWriteOutputWhileAck(railNo,index,outState,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard3_6RecycleRailWriteOutputWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterBoard3_6RecycleRailWaitWriteOutputResult(railNo,resultPackPtr,writeIndex);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard3_6RecycleRailWaitWriteOutputResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//回收轨道 指定步进电机复位
void CanMasterBoard3_6RecycleRailStepMotorResetWhileReturn(RAIL_MODULE_NO railNo,uint8_t motorIndex,SYS_RESULT_PACK* resultPackPtr,MOD_SM_CAN_RESULT *modSmCanResultPtr)
{
    //发送指令等待ACK
    CanMasterBoard3_6RecycleRailStepMotorResetWhileAck(railNo,motorIndex,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard3_6RecycleRailStepMotorResetWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterBoard3_6RecycleRailWaitStepMotorResetResult(railNo,resultPackPtr,modSmCanResultPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard3_6RecycleRailWaitStepMotorResetResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//回收轨道 电机走步数
void CanMasterBoard3_6RecycleRailStepMotorRunStepsWhileReturn(RAIL_MODULE_NO railNo,uint8_t motorIndex,int32_t steps,SYS_RESULT_PACK* resultPackPtr,MOD_SM_CAN_RESULT *modSmCanResultPtr)
{
    //发送指令等待ACK
    CanMasterBoard3_6RecycleRailStepMotorRunStepsWhileAck(railNo,motorIndex,steps,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard3_6RecycleRailStepMotorRunStepsWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterBoard3_6RecycleRailWaitStepMotorRunStepsResult(railNo,resultPackPtr,modSmCanResultPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard3_6RecycleRailWaitStepMotorResetResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//回收轨道 整体复位
void CanMasterBoard3_6RecycleRailResetAllWhileReturn(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr,SAMPLE_RACK_STATE *rackExistPtr)
{
    //发送指令等待ACK
    CanMasterBoard3_6RecycleRailResetAllWhileAck(railNo,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard3_6RecycleRailResetAllWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterBoard3_6RecycleRailWaitResetAllResult(railNo,resultPackPtr,rackExistPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard3_6RecycleRailWaitResetAllResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//回收轨道 回收架子到出口
void CanMasterBoard3_6RecycleRailMovRackToExitWhileReturn(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr,SAMPLE_RACK_STATE *rackExistPtr)
{
    //发送指令等待ACK
    CanMasterBoard3_6RecycleRailMovRackToExitWhileAck(railNo,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard3_6RecycleRailMovRackToExitWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterBoard3_6RecycleRailWaitMovRackToExitResult(railNo,resultPackPtr,rackExistPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard3_6RecycleRailWaitMovRackToExitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//回收轨道 接收架子
void CanMasterBoard3_6RecycleRailReceiveRackWhileReturn(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr,SAMPLE_RACK_STATE *rackEntryPtr)
{
    //发送指令等待ACK
    CanMasterBoard3_6RecycleRailReceiveRackWhileAck(railNo,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard3_6RecycleRailReceiveRackWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterBoard3_6RecycleRailWaitReceiveRackResult(railNo,resultPackPtr,rackEntryPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard3_6RecycleRailWaitReceiveRackResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//回收轨道 推送架子到下一模块
void CanMasterBoard3_6RecycleRailPushRackToNextWhileReturn(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr,SAMPLE_RACK_STATE *rackExistPtr)
{
    //发送指令等待ACK
    CanMasterBoard3_6RecycleRailPushRackToNextWhileAck(railNo,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard3_6RecycleRailPushRackToNextWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterBoard3_6RecycleRailWaitPushRackToNextResult(railNo,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard3_6RecycleRailWaitPushRackToNextResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}


