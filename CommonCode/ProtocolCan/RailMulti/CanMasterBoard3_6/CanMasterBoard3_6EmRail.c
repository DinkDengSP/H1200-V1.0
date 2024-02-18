/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-24 10:44:38 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-24 13:52:03 +0800
************************************************************************************************/ 
#include "CanMasterBoard3_6EmRail.h"
#include "DriverConfigSystem.h"
#include SYS_LOG_INCLUDE_FILE

//通道配置数组
//参数分别是 指令  是否需要结果  超时时间  最小参数长度 附带参数长度
//附带参数长度为不定长的时候,指示参数长度为0,由上级调用者手动申请内存
static const CAN_PROT_CMD_CONFIG canMasterBoard3_6EmRailCmdConfigArray[] = {
    {CAN_RAIL_MUTIL_CMD_BOARD3_6_EMER_RAIL_READ_INPUT                       ,        ENABLE,     60000,      2,      1       },//急诊轨道 输入读取
    {CAN_RAIL_MUTIL_CMD_BOARD3_6_EMER_RAIL_WRITE_OUTPUT                     ,        ENABLE,     60000,      1,      2       },//急诊轨道 输出写出
    {CAN_RAIL_MUTIL_CMD_BOARD3_6_EMER_RAIL_MOTOR_RESET                      ,        ENABLE,     60000,      5,      1       },//急诊轨道 电机复位
    {CAN_RAIL_MUTIL_CMD_BOARD3_6_EMER_RAIL_MOTOR_RUN_STEP                   ,        ENABLE,     60000,      5,      5       },//急诊轨道 电机走步数
    {CAN_RAIL_MUTIL_CMD_BOARD3_6_EMER_RAIL_RESET_ALL                        ,        ENABLE,     60000,      2,      0       },//急诊轨道 复位
    {CAN_RAIL_MUTIL_CMD_BOARD3_6_EMER_RAIL_FLUSH_SENSOR_STATE               ,        ENABLE,     60000,      3,      0       },//急诊轨道 传感器状态刷新
    {CAN_RAIL_MUTIL_CMD_BOARD3_6_EMER_RAIL_MOVE_TO_TARGET_ABSORB_POS        ,        ENABLE,     60000,      0,      1       },//急诊轨道 移动到指定吸样位
    {CAN_RAIL_MUTIL_CMD_BOARD3_6_EMER_RAIL_RACK_MOVE_IN                     ,        ENABLE,     60000,      0,      0       },//急诊轨道 架子预进入
    {CAN_RAIL_MUTIL_CMD_BOARD3_6_EMER_RAIL_RACK_MOVE_TO_TEST                ,        ENABLE,     60000,      0,      0       },//急诊轨道 接收架子
    {CAN_RAIL_MUTIL_CMD_BOARD3_6_EMER_RAIL_RACK_MOVE                        ,        ENABLE,     60000,      0,      1       },//急诊轨道 架子放行
    {CAN_RAIL_MUTIL_CMD_BOARD3_6_EMER_RAIL_PUSH_RACK_TO_RAIL_CHANGE         ,        ENABLE,     60000,      0,      0       },//急诊轨道 放行一个架子到变轨
};

/*************************************发送指令等待ACK************************************************/
//急诊轨道 输入读取
void CanMasterBoard3_6EmerRailReadInputWhileAck(RAIL_MODULE_NO railNo,uint8_t index,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD3_6_EMER_RAIL;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD3_6_EMER_RAIL_READ_INPUT;
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
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterBoard3_6EmRailCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterBoard3_6EmRailCmdConfigArray));
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

//急诊轨道 输出写出
void CanMasterBoard3_6EmerRailWriteOutputWhileAck(RAIL_MODULE_NO railNo,uint8_t index,PIN_STATE outState,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD3_6_EMER_RAIL;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD3_6_EMER_RAIL_WRITE_OUTPUT;
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
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterBoard3_6EmRailCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterBoard3_6EmRailCmdConfigArray));
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

//急诊轨道 指定步进电机复位
void CanMasterBoard3_6EmerRailStepMotorResetWhileAck(RAIL_MODULE_NO railNo,uint8_t motorIndex,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD3_6_EMER_RAIL;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD3_6_EMER_RAIL_MOTOR_RESET;
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
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterBoard3_6EmRailCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterBoard3_6EmRailCmdConfigArray));
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

//急诊轨道 电机走步数
void CanMasterBoard3_6EmerRailStepMotorRunStepsWhileAck(RAIL_MODULE_NO railNo,uint8_t motorIndex,int32_t steps,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD3_6_EMER_RAIL;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD3_6_EMER_RAIL_MOTOR_RUN_STEP;
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
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterBoard3_6EmRailCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterBoard3_6EmRailCmdConfigArray));
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

//急诊轨道 整体复位
void CanMasterBoard3_6EmerRailResetAllWhileAck(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD3_6_EMER_RAIL;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD3_6_EMER_RAIL_RESET_ALL;
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
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterBoard3_6EmRailCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterBoard3_6EmRailCmdConfigArray));
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

//急诊轨道 传感器状态刷新
void CanMasterBoard3_6EmerRailRefreshSensorStateWhileAck(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD3_6_EMER_RAIL;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD3_6_EMER_RAIL_FLUSH_SENSOR_STATE;
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
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterBoard3_6EmRailCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterBoard3_6EmRailCmdConfigArray));
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
    // canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)releaseType;
    // ProtUtilWriteInt32ToBuffer(steps,canCmdSendPtr->cmdBufferPtr+1);
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}


//急诊轨道 移动到指定吸样位
void CanMasterBoard3_6EmerRailMoveToTargetAbsorbPosWhileAck(RAIL_MODULE_NO railNo,ABSORB_POS pos,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD3_6_EMER_RAIL;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD3_6_EMER_RAIL_MOVE_TO_TARGET_ABSORB_POS;
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
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterBoard3_6EmRailCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterBoard3_6EmRailCmdConfigArray));
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
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)pos;
    // ProtUtilWriteInt32ToBuffer(steps,canCmdSendPtr->cmdBufferPtr+1);
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//急诊轨道 架子预进入
void CanMasterBoard3_6EmerRailRackMoveInWhileAck(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD3_6_EMER_RAIL;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD3_6_EMER_RAIL_RACK_MOVE_IN;
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
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterBoard3_6EmRailCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterBoard3_6EmRailCmdConfigArray));
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
    // canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)releaseType;
    // ProtUtilWriteInt32ToBuffer(steps,canCmdSendPtr->cmdBufferPtr+1);
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//急诊轨道 架子进入测试区
void CanMasterBoard3_6EmerRailRackMoveToTestWhileAck(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD3_6_EMER_RAIL;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD3_6_EMER_RAIL_RACK_MOVE_TO_TEST;
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
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterBoard3_6EmRailCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterBoard3_6EmRailCmdConfigArray));
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
    // canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)releaseType;
    // ProtUtilWriteInt32ToBuffer(steps,canCmdSendPtr->cmdBufferPtr+1);
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//急诊轨道 架子放行
void CanMasterBoard3_6EmerRailRackMoveWhileAck(RAIL_MODULE_NO railNo,RACK_MOVE releaseType,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD3_6_EMER_RAIL;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD3_6_EMER_RAIL_RACK_MOVE;
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
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterBoard3_6EmRailCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterBoard3_6EmRailCmdConfigArray));
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
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)releaseType;
    // ProtUtilWriteInt32ToBuffer(steps,canCmdSendPtr->cmdBufferPtr+1);
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//急诊轨道 放行一个架子到变轨
void CanMasterBoard3_6EmerRailPushRackToRailChangeWhileAck(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD3_6_EMER_RAIL;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD3_6_EMER_RAIL_PUSH_RACK_TO_RAIL_CHANGE;
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
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterBoard3_6EmRailCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterBoard3_6EmRailCmdConfigArray));
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
    // canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)releaseType;
    // ProtUtilWriteInt32ToBuffer(steps,canCmdSendPtr->cmdBufferPtr+1);
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}


/*************************************等待指定指令完成返回************************************************/
//急诊轨道 输入读取
void CanMasterBoard3_6EmerRailWaitReadInputResult(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr,PIN_STATE* sensorStatePtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD3_6_EMER_RAIL;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD3_6_EMER_RAIL_READ_INPUT;
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

//急诊轨道 输出写出
void CanMasterBoard3_6EmerRailWaitWriteOutputResult(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr,uint8_t* writeIndex)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD3_6_EMER_RAIL;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD3_6_EMER_RAIL_WRITE_OUTPUT;
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

//急诊轨道 指定步进电机复位
void CanMasterBoard3_6EmerRailWaitStepMotorResetResult(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr,MOD_SM_CAN_RESULT *modSmCanResultPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD3_6_EMER_RAIL;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD3_6_EMER_RAIL_MOTOR_RESET;
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

//急诊轨道 电机走步数
void CanMasterBoard3_6EmerRailWaitStepMotorRunStepsResult(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr,MOD_SM_CAN_RESULT *modSmCanResultPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD3_6_EMER_RAIL;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD3_6_EMER_RAIL_MOTOR_RUN_STEP;
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

//急诊轨道 整体复位
void CanMasterBoard3_6EmerRailWaitResetAllResult(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr,EM_RAIL_SENSOR_STATE *emRailSensorUnitStatePtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD3_6_EMER_RAIL;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD3_6_EMER_RAIL_RESET_ALL;
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
    if(emRailSensorUnitStatePtr != NULL)
    {
        //结果解析
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            emRailSensorUnitStatePtr->absorb =  (SAMPLE_RACK_STATE)(returnDataBufferPtr[0]);
            emRailSensorUnitStatePtr->change = (SAMPLE_RACK_STATE)(returnDataBufferPtr[1]);
        }
    }
    //内存释放
    CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//急诊轨道 传感器状态刷新
void CanMasterBoard3_6EmerRailWaitRefreshSensorStateResult(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr,EM_RAIL_SENSOR_STATE *emRailSensorUnitStatePtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD3_6_EMER_RAIL;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD3_6_EMER_RAIL_FLUSH_SENSOR_STATE;
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
    if(emRailSensorUnitStatePtr != NULL)
    {
        //结果解析
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            emRailSensorUnitStatePtr->entrance = (SAMPLE_RACK_STATE)(returnDataBufferPtr[0]);
            emRailSensorUnitStatePtr->absorb =  (SAMPLE_RACK_STATE)(returnDataBufferPtr[1]);
            emRailSensorUnitStatePtr->exit =    (SAMPLE_RACK_STATE)(returnDataBufferPtr[2]);
        }
    }
    //内存释放
    CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//急诊轨道 移动到指定吸样位
void CanMasterBoard3_6EmerRailWaitMoveToTargetAbsorbPosResult(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD3_6_EMER_RAIL;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD3_6_EMER_RAIL_MOVE_TO_TARGET_ABSORB_POS;
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

//急诊轨道 架子预进入
void CanMasterBoard3_6EmerRailWaitRackMoveInResult(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD3_6_EMER_RAIL;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD3_6_EMER_RAIL_RACK_MOVE_IN;
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

//急诊轨道 架子进入测试区
void CanMasterBoard3_6EmerRailWaitRackMoveToTestResult(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD3_6_EMER_RAIL;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD3_6_EMER_RAIL_RACK_MOVE_TO_TEST;
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

//急诊轨道 架子放行
void CanMasterBoard3_6EmerRailWaitRackMoveResult(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD3_6_EMER_RAIL;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD3_6_EMER_RAIL_RACK_MOVE;
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


//急诊轨道  放行一个架子到变轨
void CanMasterBoard3_6EmerRailWaitPushRackToRailChangeResult(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD3_6_EMER_RAIL;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD3_6_EMER_RAIL_PUSH_RACK_TO_RAIL_CHANGE;
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
//急诊轨道 输入读取
void CanMasterBoard3_6EmerRailReadInputWhileReturn(RAIL_MODULE_NO railNo,uint8_t index,SYS_RESULT_PACK* resultPackPtr,PIN_STATE* sensorStatePtr)
{
    //发送指令等待ACK
    CanMasterBoard3_6EmerRailReadInputWhileAck(railNo,index,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard3_6EmerRailReadInputWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterBoard3_6EmerRailWaitReadInputResult(railNo,resultPackPtr,sensorStatePtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard3_6EmerRailWaitReadInputResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//急诊轨道 输出写出
void CanMasterBoard3_6EmerRailWriteOutputWhileReturn(RAIL_MODULE_NO railNo,uint8_t index,PIN_STATE outState,SYS_RESULT_PACK* resultPackPtr,uint8_t* writeIndex)
{
    //发送指令等待ACK
    CanMasterBoard3_6EmerRailWriteOutputWhileAck(railNo,index,outState,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard3_6EmerRailWriteOutputWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterBoard3_6EmerRailWaitWriteOutputResult(railNo,resultPackPtr,writeIndex);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard3_6EmerRailWaitWriteOutputResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//急诊轨道 指定步进电机复位
void CanMasterBoard3_6EmerRailStepMotorResetWhileReturn(RAIL_MODULE_NO railNo,uint8_t motorIndex,SYS_RESULT_PACK* resultPackPtr,MOD_SM_CAN_RESULT *modSmCanResultPtr)
{
    //发送指令等待ACK
    CanMasterBoard3_6EmerRailStepMotorResetWhileAck(railNo,motorIndex,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard3_6EmerRailStepMotorResetWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterBoard3_6EmerRailWaitStepMotorResetResult(railNo,resultPackPtr,modSmCanResultPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard3_6EmerRailWaitStepMotorResetResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//急诊轨道 电机走步数
void CanMasterBoard3_6EmerRailStepMotorRunStepsWhileReturn(RAIL_MODULE_NO railNo,uint8_t motorIndex,int32_t steps,SYS_RESULT_PACK* resultPackPtr,MOD_SM_CAN_RESULT *modSmCanResultPtr)
{
    //发送指令等待ACK
    CanMasterBoard3_6EmerRailStepMotorRunStepsWhileAck(railNo,motorIndex,steps,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard3_6EmerRailStepMotorRunStepsWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterBoard3_6EmerRailWaitStepMotorRunStepsResult(railNo,resultPackPtr,modSmCanResultPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard3_6EmerRailWaitStepMotorResetResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//急诊轨道 整体复位
void CanMasterBoard3_6EmerRailResetAllWhileReturn(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr,EM_RAIL_SENSOR_STATE *emRailSensorUnitStatePtr)
{
    //发送指令等待ACK
    CanMasterBoard3_6EmerRailResetAllWhileAck(railNo,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard3_6EmerRailResetAllWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterBoard3_6EmerRailWaitResetAllResult(railNo,resultPackPtr,emRailSensorUnitStatePtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard3_6EmerRailWaitResetAllResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//急诊轨道 传感器状态刷新
void CanMasterBoard3_6EmerRailRefreshSensorStateWhileReturn(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr,EM_RAIL_SENSOR_STATE *emRailSensorUnitStatePtr)
{
    //发送指令等待ACK
    CanMasterBoard3_6EmerRailRefreshSensorStateWhileAck(railNo,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard3_6EmerRailRefreshSensorStateWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterBoard3_6EmerRailWaitRefreshSensorStateResult(railNo,resultPackPtr,emRailSensorUnitStatePtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard3_6EmerRailWaitRefreshSensorStateResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//急诊轨道 移动到指定吸样位
void CanMasterBoard3_6EmerRailMoveToTargetAbsorbPosWhileReturn(RAIL_MODULE_NO railNo,ABSORB_POS pos,SYS_RESULT_PACK* resultPackPtr)
{
    //发送指令等待ACK
    CanMasterBoard3_6EmerRailMoveToTargetAbsorbPosWhileAck(railNo,pos,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard3_6EmerRailMoveToTargetAbsorbPosWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterBoard3_6EmerRailWaitMoveToTargetAbsorbPosResult(railNo,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard3_6EmerRailWaitMoveToTargetAbsorbPosResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//急诊轨道 架子预进入
void CanMasterBoard3_6EmerRailRackMoveInWhileReturn(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr)
{
    //发送指令等待ACK
    CanMasterBoard3_6EmerRailRackMoveInWhileAck(railNo,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard3_6EmerRailRackMoveInWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterBoard3_6EmerRailWaitRackMoveInResult(railNo,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard3_6EmerRailWaitRackMoveInResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//急诊轨道 架子进入测试区
void CanMasterBoard3_6EmerRailRackMoveToTestWhileReturn(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr)
{
    //发送指令等待ACK
    CanMasterBoard3_6EmerRailRackMoveToTestWhileAck(railNo,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard3_6EmerRailRackMoveToTestWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterBoard3_6EmerRailWaitRackMoveToTestResult(railNo,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard3_6EmerRailWaitRackMoveToTestResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//急诊轨道 架子放行
void CanMasterBoard3_6EmerRailRackMoveWhileReturn(RAIL_MODULE_NO railNo,RACK_MOVE releaseType,SYS_RESULT_PACK* resultPackPtr)
{
    //发送指令等待ACK
    CanMasterBoard3_6EmerRailRackMoveWhileAck(railNo,releaseType,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard3_6EmerRailRackMoveWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterBoard3_6EmerRailWaitRackMoveResult(railNo,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard3_6EmerRailWaitRackMoveResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//急诊轨道 放行一个架子到变轨
void CanMasterBoard3_6EmerRailPushRackToRailChangeWhileReturn(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr)
{
    //发送指令等待ACK
    CanMasterBoard3_6EmerRailPushRackToRailChangeWhileAck(railNo,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard3_6EmerRailPushRackToRailChangeWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterBoard3_6EmerRailWaitPushRackToRailChangeResult(railNo,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard3_6EmerRailWaitPushRackToRailChangeResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}


