/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-24 10:44:38 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-24 13:52:03 +0800
************************************************************************************************/ 
#include "CanMasterBoard3_6RailChange.h"
#include "DriverConfigSystem.h"
#include SYS_LOG_INCLUDE_FILE

//通道配置数组
//参数分别是 指令  是否需要结果  超时时间  最小参数长度 附带参数长度
//附带参数长度为不定长的时候,指示参数长度为0,由上级调用者手动申请内存
static const CAN_PROT_CMD_CONFIG canMasterBoard3_6RailChangeCmdConfigArray[] = {
    {CAN_RAIL_MUTIL_CMD_BOARD3_6_RAIL_CHANGE_READ_INPUT                 ,        ENABLE,     60000,      2,      1       },//轨道变轨 输入读取
    {CAN_RAIL_MUTIL_CMD_BOARD3_6_RAIL_CHANGE_WRITE_OUTPUT               ,        ENABLE,     60000,      1,      2       },//轨道变轨 输出写出
    {CAN_RAIL_MUTIL_CMD_BOARD3_6_RAIL_CHANGE_MOTOR_RESET                ,        ENABLE,     60000,      5,      1       },//轨道变轨 电机复位
    {CAN_RAIL_MUTIL_CMD_BOARD3_6_RAIL_CHANGE_MOTOR_RUN_STEP             ,        ENABLE,     60000,      5,      5       },//轨道变轨 电机走步数
    {CAN_RAIL_MUTIL_CMD_BOARD3_6_RAIL_CHANGE_RESET_ALL                  ,        ENABLE,     60000,      0,      0       },//轨道变轨 复位
    {CAN_RAIL_MUTIL_CMD_BOARD3_6_RAIL_CHANGE_MOVE_TO_TARGET_RAIL        ,        ENABLE,     60000,      0,      1       },//轨道变轨 移动到指定轨道
    {CAN_RAIL_MUTIL_CMD_BOARD3_6_RAIL_CHANGE_RELEASE_CONTROL            ,        ENABLE,     60000,      0,      1       },//轨道变轨 放行控制
    {CAN_RAIL_MUTIL_CMD_BOARD3_6_RAIL_CHANGE_PUSH_RACK_TO_TARGET_RAIL   ,        ENABLE,     60000,      0,      1       },//轨道变轨 推送样本架

};

/*************************************发送指令等待ACK************************************************/
//轨道变轨 输入读取
void CanMasterBoard3_6RailChangeReadInputWhileAck(RAIL_MODULE_NO railNo,uint8_t index,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD3_6_RAIL_CHANGE;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD3_6_RAIL_CHANGE_READ_INPUT;
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
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterBoard3_6RailChangeCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterBoard3_6RailChangeCmdConfigArray));
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

//轨道变轨 输出写出
void CanMasterBoard3_6RailChangeWriteOutputWhileAck(RAIL_MODULE_NO railNo,uint8_t index,PIN_STATE outState,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD3_6_RAIL_CHANGE;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD3_6_RAIL_CHANGE_WRITE_OUTPUT;
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
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterBoard3_6RailChangeCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterBoard3_6RailChangeCmdConfigArray));
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

//轨道变轨 指定步进电机复位
void CanMasterBoard3_6RailChangeStepMotorResetWhileAck(RAIL_MODULE_NO railNo,uint8_t motorIndex,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD3_6_RAIL_CHANGE;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD3_6_RAIL_CHANGE_MOTOR_RESET;
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
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterBoard3_6RailChangeCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterBoard3_6RailChangeCmdConfigArray));
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

//轨道变轨 电机走步数
void CanMasterBoard3_6RailChangeStepMotorRunStepsWhileAck(RAIL_MODULE_NO railNo,uint8_t motorIndex,int32_t steps,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD3_6_RAIL_CHANGE;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD3_6_RAIL_CHANGE_MOTOR_RUN_STEP;
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
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterBoard3_6RailChangeCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterBoard3_6RailChangeCmdConfigArray));
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

//轨道变轨 整体复位
void CanMasterBoard3_6RailChangeResetAllWhileAck(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD3_6_RAIL_CHANGE;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD3_6_RAIL_CHANGE_RESET_ALL;
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
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterBoard3_6RailChangeCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterBoard3_6RailChangeCmdConfigArray));
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

//轨道变轨 移动到指定轨道
void CanMasterBoard3_6RailChangeMoveToTargetRailWhileAck(RAIL_MODULE_NO railNo,RAIL_CHANGE_POS changePos,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD3_6_RAIL_CHANGE;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD3_6_RAIL_CHANGE_MOVE_TO_TARGET_RAIL;
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
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterBoard3_6RailChangeCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterBoard3_6RailChangeCmdConfigArray));
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
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)changePos;
    // ProtUtilWriteInt32ToBuffer(steps,canCmdSendPtr->cmdBufferPtr+1);
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//轨道变轨  放行控制
void CanMasterBoard3_6RailChangeReleaseControlWhileAck(RAIL_MODULE_NO railNo,RAIL_CHANGE_GATE railGate,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD3_6_RAIL_CHANGE;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD3_6_RAIL_CHANGE_RELEASE_CONTROL;
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
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterBoard3_6RailChangeCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterBoard3_6RailChangeCmdConfigArray));
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
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)railGate;
    // ProtUtilWriteInt32ToBuffer(steps,canCmdSendPtr->cmdBufferPtr+1);
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//轨道变轨 推送样本架
void CanMasterBoard3_6RailChangePushRackToTargetRailWhileAck(RAIL_MODULE_NO railNo,RAIL_CHANGE_POS changePos,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD3_6_RAIL_CHANGE;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD3_6_RAIL_CHANGE_PUSH_RACK_TO_TARGET_RAIL;
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
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterBoard3_6RailChangeCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterBoard3_6RailChangeCmdConfigArray));
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
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)changePos;
    // ProtUtilWriteInt32ToBuffer(steps,canCmdSendPtr->cmdBufferPtr+1);
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}
/*************************************等待指定指令完成返回************************************************/
//轨道变轨 输入读取
void CanMasterBoard3_6RailChangeWaitReadInputResult(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr,PIN_STATE* sensorStatePtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD3_6_RAIL_CHANGE;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD3_6_RAIL_CHANGE_READ_INPUT;
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

//轨道变轨 输出写出
void CanMasterBoard3_6RailChangeWaitWriteOutputResult(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr,uint8_t* writeIndex)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD3_6_RAIL_CHANGE;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD3_6_RAIL_CHANGE_WRITE_OUTPUT;
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

//轨道变轨 指定步进电机复位
void CanMasterBoard3_6RailChangeWaitStepMotorResetResult(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr,MOD_SM_CAN_RESULT *modSmCanResultPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD3_6_RAIL_CHANGE;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD3_6_RAIL_CHANGE_MOTOR_RESET;
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

//轨道变轨 电机走步数
void CanMasterBoard3_6RailChangeWaitStepMotorRunStepsResult(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr,MOD_SM_CAN_RESULT *modSmCanResultPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD3_6_RAIL_CHANGE;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD3_6_RAIL_CHANGE_MOTOR_RUN_STEP;
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

//轨道变轨 整体复位
void CanMasterBoard3_6RailChangeWaitResetAllResult(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD3_6_RAIL_CHANGE;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD3_6_RAIL_CHANGE_RESET_ALL;
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

//轨道变轨 移动到指定轨道
void CanMasterBoard3_6RailChangeWaitMoveToTargetRailResult(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD3_6_RAIL_CHANGE;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD3_6_RAIL_CHANGE_MOVE_TO_TARGET_RAIL;
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

//轨道变轨  放行控制
void CanMasterBoard3_6RailChangeWaitReleaseControlResult(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD3_6_RAIL_CHANGE;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD3_6_RAIL_CHANGE_RELEASE_CONTROL;
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

//轨道变轨 推送样本架
void CanMasterBoard3_6RailChangeWaitPushRackToTargetRailResult(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD3_6_RAIL_CHANGE;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD3_6_RAIL_CHANGE_PUSH_RACK_TO_TARGET_RAIL;
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
//轨道变轨 输入读取
void CanMasterBoard3_6RailChangeReadInputWhileReturn(RAIL_MODULE_NO railNo,uint8_t index,SYS_RESULT_PACK* resultPackPtr,PIN_STATE* sensorStatePtr)
{
    //发送指令等待ACK
    CanMasterBoard3_6RailChangeReadInputWhileAck(railNo,index,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard3_6RailChangeReadInputWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterBoard3_6RailChangeWaitReadInputResult(railNo,resultPackPtr,sensorStatePtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard3_6RailChangeWaitReadInputResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//轨道变轨 输出写出
void CanMasterBoard3_6RailChangeWriteOutputWhileReturn(RAIL_MODULE_NO railNo,uint8_t index,PIN_STATE outState,SYS_RESULT_PACK* resultPackPtr,uint8_t* writeIndex)
{
    //发送指令等待ACK
    CanMasterBoard3_6RailChangeWriteOutputWhileAck(railNo,index,outState,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard3_6RailChangeWriteOutputWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterBoard3_6RailChangeWaitWriteOutputResult(railNo,resultPackPtr,writeIndex);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard3_6RailChangeWaitWriteOutputResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//轨道变轨 指定步进电机复位
void CanMasterBoard3_6RailChangeStepMotorResetWhileReturn(RAIL_MODULE_NO railNo,uint8_t motorIndex,SYS_RESULT_PACK* resultPackPtr,MOD_SM_CAN_RESULT *modSmCanResultPtr)
{
    //发送指令等待ACK
    CanMasterBoard3_6RailChangeStepMotorResetWhileAck(railNo,motorIndex,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard3_6RailChangeStepMotorResetWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterBoard3_6RailChangeWaitStepMotorResetResult(railNo,resultPackPtr,modSmCanResultPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard3_6RailChangeWaitStepMotorResetResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//轨道变轨 电机走步数
void CanMasterBoard3_6RailChangeStepMotorRunStepsWhileReturn(RAIL_MODULE_NO railNo,uint8_t motorIndex,int32_t steps,SYS_RESULT_PACK* resultPackPtr,MOD_SM_CAN_RESULT *modSmCanResultPtr)
{
    //发送指令等待ACK
    CanMasterBoard3_6RailChangeStepMotorRunStepsWhileAck(railNo,motorIndex,steps,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard3_6RailChangeStepMotorRunStepsWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterBoard3_6RailChangeWaitStepMotorRunStepsResult(railNo,resultPackPtr,modSmCanResultPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard3_6RailChangeWaitStepMotorResetResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//轨道变轨 整体复位
void CanMasterBoard3_6RailChangeResetAllWhileReturn(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr)
{
    //发送指令等待ACK
    CanMasterBoard3_6RailChangeResetAllWhileAck(railNo,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard3_6RailChangeResetAllWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterBoard3_6RailChangeWaitResetAllResult(railNo,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard3_6RailChangeWaitResetAllResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//轨道变轨 移动到指定轨道
void CanMasterBoard3_6RailChangeMoveToTargetRailWhileReturn(RAIL_MODULE_NO railNo,RAIL_CHANGE_POS changePos,SYS_RESULT_PACK* resultPackPtr)
{
    //发送指令等待ACK
    CanMasterBoard3_6RailChangeMoveToTargetRailWhileAck(railNo,changePos,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard3_6RailChangeMoveToTargetRailWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterBoard3_6RailChangeWaitMoveToTargetRailResult(railNo,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard3_6RailChangeWaitMoveToTargetRailResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//轨道变轨 放行控制
void CanMasterBoard3_6RailChangeReleaseControlWhileReturn(RAIL_MODULE_NO railNo,RAIL_CHANGE_GATE railGate,SYS_RESULT_PACK* resultPackPtr)
{
    //发送指令等待ACK
    CanMasterBoard3_6RailChangeReleaseControlWhileAck(railNo,railGate,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard3_6RailChangeReleaseControlWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterBoard3_6RailChangeWaitReleaseControlResult(railNo,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard3_6RailChangeWaitReleaseControlResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//轨道变轨 推送样本架
void CanMasterBoard3_6RailChangePushRackToTargetRailWhileReturn(RAIL_MODULE_NO railNo,RAIL_CHANGE_POS changePos,SYS_RESULT_PACK* resultPackPtr)
{
    //发送指令等待ACK
    CanMasterBoard3_6RailChangePushRackToTargetRailWhileAck(railNo,changePos,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard3_6RailChangePushRackToTargetRailWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterBoard3_6RailChangeWaitPushRackToTargetRailResult(railNo,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard3_6RailChangeWaitPushRackToTargetRailResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}
