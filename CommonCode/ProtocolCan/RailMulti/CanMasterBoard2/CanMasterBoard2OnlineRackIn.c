/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-24 10:44:38 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-24 13:52:03 +0800
************************************************************************************************/ 
#include "CanMasterBoard2OnlineRackIn.h"
#include "DriverConfigSystem.h"
#include SYS_LOG_INCLUDE_FILE

//通道配置数组
//参数分别是 指令  是否需要结果  超时时间  最小参数长度 附带参数长度
//附带参数长度为不定长的时候,指示参数长度为0,由上级调用者手动申请内存
static const CAN_PROT_CMD_CONFIG canMasterBoard2OnlineRackInCmdConfigArray[] = {
    {CAN_RAIL_MUTIL_CMD_BOARD2_ONLINE_RACK_IN_READ_INPUT                            ,        ENABLE,     60000,      2,      1       },//联机接口进架 输入读取
    {CAN_RAIL_MUTIL_CMD_BOARD2_ONLINE_RACK_IN_WRITE_OUTPUT                          ,        ENABLE,     60000,      1,      2       },//联机接口进架 输出写出
    {CAN_RAIL_MUTIL_CMD_BOARD2_ONLINE_RACK_IN_MOTOR_RESET                            ,        ENABLE,     60000,      5,      1       },//联机接口进架 电机复位
    {CAN_RAIL_MUTIL_CMD_BOARD2_ONLINE_RACK_IN_MOTOR_RUN_STEP                         ,        ENABLE,     60000,      5,      5       },//联机接口进架 电机走步数
    {CAN_RAIL_MUTIL_CMD_BOARD2_ONLINE_RACK_IN_RESET_ALL                              ,        ENABLE,     60000,      0,      0       },//联机接口进架 复位
    {CAN_RAIL_MUTIL_CMD_BOARD2_ONLINE_RACK_IN_RELEASE_CONTROL                        ,        ENABLE,     60000,      0,      1       },//联机接口进架 放行控制
};

/*************************************发送指令等待ACK************************************************/
//联机接口进架 输入读取
void CanMasterBoard2OnlineRackInReadInputWhileAck(uint8_t index,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD2_ONLINE_RACK_IN;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD2_ONLINE_RACK_IN_READ_INPUT;
    uint8_t targetModule = CAN_MODULE_NO_BOARD1;
    uint8_t targetBoardID = CAN_BOARD_ID_BOARD1;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        CAN_PROT_LOG("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterBoard2OnlineRackInCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterBoard2OnlineRackInCmdConfigArray));
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

//联机接口进架 输出写出
void CanMasterBoard2OnlineRackInWriteOutputWhileAck(uint8_t index,PIN_STATE outState,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD2_ONLINE_RACK_IN;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD2_ONLINE_RACK_IN_WRITE_OUTPUT;
    uint8_t targetModule = CAN_MODULE_NO_BOARD1;
    uint8_t targetBoardID = CAN_BOARD_ID_BOARD1;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        CAN_PROT_LOG("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterBoard2OnlineRackInCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterBoard2OnlineRackInCmdConfigArray));
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
//联机接口进架 指定步进电机复位
void CanMasterBoard2OnlineRackInStepMotorResetWhileAck(uint8_t motorIndex,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD2_ONLINE_RACK_IN;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD2_ONLINE_RACK_IN_MOTOR_RESET;
    uint8_t targetModule = CAN_MODULE_NO_BOARD2;
    uint8_t targetBoardID = CAN_BOARD_ID_BOARD2;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        CAN_PROT_LOG("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterBoard2OnlineRackInCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterBoard2OnlineRackInCmdConfigArray));
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

//联机接口进架 电机走步数
void CanMasterBoard2OnlineRackInStepMotorRunStepsWhileAck(uint8_t motorIndex,int32_t steps,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD2_ONLINE_RACK_IN;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD2_ONLINE_RACK_IN_MOTOR_RUN_STEP;
    uint8_t targetModule = CAN_MODULE_NO_BOARD2;
    uint8_t targetBoardID = CAN_BOARD_ID_BOARD2;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        CAN_PROT_LOG("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterBoard2OnlineRackInCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterBoard2OnlineRackInCmdConfigArray));
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

//联机接口进架 整体复位
void CanMasterBoard2OnlineRackInResetAllWhileAck(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD2_ONLINE_RACK_IN;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD2_ONLINE_RACK_IN_RESET_ALL;
    uint8_t targetModule = CAN_MODULE_NO_BOARD2;
    uint8_t targetBoardID = CAN_BOARD_ID_BOARD2;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        CAN_PROT_LOG("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterBoard2OnlineRackInCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterBoard2OnlineRackInCmdConfigArray));
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

//联机接口进架 放行控制
void CanMasterBoard2OnlineRackInReleaseControlWhileAck(ONLINE_IN_GATE gateCtrl,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD2_ONLINE_RACK_IN;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD2_ONLINE_RACK_IN_RELEASE_CONTROL;
    uint8_t targetModule = CAN_MODULE_NO_BOARD2;
    uint8_t targetBoardID = CAN_BOARD_ID_BOARD2;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        CAN_PROT_LOG("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterBoard2OnlineRackInCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterBoard2OnlineRackInCmdConfigArray));
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
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)gateCtrl;
    // ProtUtilWriteInt32ToBuffer(steps,canCmdSendPtr->cmdBufferPtr+1);
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}


/*************************************等待指定指令完成返回************************************************/

//联机接口进架  输入读取
void CanMasterBoard2OnlineRackInWaitReadInputResult(SYS_RESULT_PACK* resultPackPtr,PIN_STATE* sensorStatePtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD2_ONLINE_RACK_IN;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD2_ONLINE_RACK_IN_READ_INPUT;
    uint8_t targetModule = CAN_MODULE_NO_BOARD2;
    uint8_t targetBoardID = CAN_BOARD_ID_BOARD2;
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

//联机接口进架  输出写出
void CanMasterBoard2OnlineRackInWaitWriteOutputResult(SYS_RESULT_PACK* resultPackPtr,uint8_t* writeIndex)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD2_ONLINE_RACK_IN;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD2_ONLINE_RACK_IN_WRITE_OUTPUT;
    uint8_t targetModule = CAN_MODULE_NO_BOARD2;
    uint8_t targetBoardID = CAN_BOARD_ID_BOARD2;
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
//联机接口进架 指定步进电机复位
void CanMasterBoard2OnlineRackInWaitStepMotorResetResult(SYS_RESULT_PACK* resultPackPtr,MOD_SM_CAN_RESULT *modSmCanResultPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD2_ONLINE_RACK_IN;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD2_ONLINE_RACK_IN_MOTOR_RESET;
    uint8_t targetModule = CAN_MODULE_NO_BOARD2;
    uint8_t targetBoardID = CAN_BOARD_ID_BOARD2;
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

//联机接口进架 电机走步数
void CanMasterBoard2OnlineRackInWaitStepMotorRunStepsResult(SYS_RESULT_PACK* resultPackPtr,MOD_SM_CAN_RESULT *modSmCanResultPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD2_ONLINE_RACK_IN;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD2_ONLINE_RACK_IN_MOTOR_RUN_STEP;
    uint8_t targetModule = CAN_MODULE_NO_BOARD2;
    uint8_t targetBoardID = CAN_BOARD_ID_BOARD2;
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

//联机接口进架 整体复位
void CanMasterBoard2OnlineRackInWaitResetAllResult(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD2_ONLINE_RACK_IN;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD2_ONLINE_RACK_IN_RESET_ALL;
    uint8_t targetModule = CAN_MODULE_NO_BOARD2;
    uint8_t targetBoardID = CAN_BOARD_ID_BOARD2;
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

//联机接口进架 放行控制
void CanMasterBoard2OnlineRackInWaitReleaseControlResult(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD2_ONLINE_RACK_IN;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD2_ONLINE_RACK_IN_RELEASE_CONTROL;
    uint8_t targetModule = CAN_MODULE_NO_BOARD2;
    uint8_t targetBoardID = CAN_BOARD_ID_BOARD2;
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
//联机接口进架  输入读取
void CanMasterBoard2OnlineRackInReadInputWhileReturn(uint8_t index,SYS_RESULT_PACK* resultPackPtr,PIN_STATE* sensorStatePtr)
{
    //发送指令等待ACK
    CanMasterBoard2OnlineRackInReadInputWhileAck(index,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard2OnlineRackInReadInputWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterBoard2OnlineRackInWaitReadInputResult(resultPackPtr,sensorStatePtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard2OnlineRackInWaitReadInputResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//联机接口进架  输出写出
void CanMasterBoard2OnlineRackInWriteOutputWhileReturn(uint8_t index,PIN_STATE outState,SYS_RESULT_PACK* resultPackPtr,uint8_t* writeIndex)
{
    //发送指令等待ACK
    CanMasterBoard2OnlineRackInWriteOutputWhileAck(index,outState,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard2OnlineRackInWriteOutputWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterBoard2OnlineRackInWaitWriteOutputResult(resultPackPtr,writeIndex);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard2OnlineRackInWaitWriteOutputResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//联机接口进架 指定步进电机复位
void CanMasterBoard2OnlineRackInStepMotorResetWhileReturn(uint8_t motorIndex,SYS_RESULT_PACK* resultPackPtr,MOD_SM_CAN_RESULT *modSmCanResultPtr)
{
    //发送指令等待ACK
    CanMasterBoard2OnlineRackInStepMotorResetWhileAck(motorIndex,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard2OnlineRackInStepMotorResetWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterBoard2OnlineRackInWaitStepMotorResetResult(resultPackPtr,modSmCanResultPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard2OnlineRackInWaitStepMotorResetResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//联机接口进架 电机走步数
void CanMasterBoard2OnlineRackInStepMotorRunStepsWhileReturn(uint8_t motorIndex,int32_t steps,SYS_RESULT_PACK* resultPackPtr,MOD_SM_CAN_RESULT *modSmCanResultPtr)
{
    //发送指令等待ACK
    CanMasterBoard2OnlineRackInStepMotorRunStepsWhileAck(motorIndex,steps,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard2OnlineRackInStepMotorRunStepsWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterBoard2OnlineRackInWaitStepMotorRunStepsResult(resultPackPtr,modSmCanResultPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard2OnlineRackInWaitStepMotorResetResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//联机接口进架 整体复位
void CanMasterBoard2OnlineRackInResetAllWhileReturn(SYS_RESULT_PACK* resultPackPtr)
{
    //发送指令等待ACK
    CanMasterBoard2OnlineRackInResetAllWhileAck(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard2OnlineRackInResetAllWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterBoard2OnlineRackInWaitResetAllResult(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard2OnlineRackInWaitResetAllResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//联机接口进架 放行控制
void CanMasterBoard2OnlineRackInReleaseControlWhileReturn(ONLINE_IN_GATE gateCtrl,SYS_RESULT_PACK* resultPackPtr)
{
    //发送指令等待ACK
    CanMasterBoard2OnlineRackInReleaseControlWhileAck(gateCtrl,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard2OnlineRackInReleaseControlWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterBoard2OnlineRackInWaitReleaseControlResult(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard2OnlineRackInWaitReleaseControlResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}
