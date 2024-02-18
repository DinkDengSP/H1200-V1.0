/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-24 13:00:56 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-30 10:15:15 +0800
************************************************************************************************/ 
#include "CanMasterPlateReaction.h"
#include "DriverConfigSystem.h"
#include SYS_LOG_INCLUDE_FILE

//通道的模块号和板号定义
#define CAN_MODULE_NO_PLATE_REACTION        CAN_MODULE_NO_BOARD4
#define CAN_BOARD_ID_PLATE_REACTION         CAN_BOARD_ID_BOARD4

//通道配置数组
//参数分别是 指令  是否需要结果  超时时间  返回最小参数长度 附带参数长度
//附带参数长度为不定长的时候,指示参数长度为0,由上级调用者手动申请内存
static const CAN_PROT_CMD_CONFIG canMasterPlateReactionCmdConfigArray[] = {
    {CAN_MASTER_CMD_H360_PLATE_REACTION_PORT_IN               , ENABLE, 3000 ,  2,  1 },//模块内输入读取
    {CAN_MASTER_CMD_H360_PLATE_REACTION_PORT_OUT              , ENABLE, 3000 ,  1,  2 },//模块内输出写入
    {CAN_MASTER_CMD_H360_PLATE_REACTION_MOTOR_RESET           , ENABLE, 60000,  5,  1 },//模块内电机复位
    {CAN_MASTER_CMD_H360_PLATE_REACTION_MOTOR_RUN_STEPS       , ENABLE, 60000,  5,  5 },//模块内电机走步数
    {CAN_MASTER_CMD_H360_PLATE_REACTION_RESET                 , ENABLE, 60000,  3,  0 },//反应盘复位
    {CAN_MASTER_CMD_H360_PLATE_REACTION_RUN_IDX_CUP_POSITIVE  , ENABLE, 5000 ,  3,  1 },//反应盘正向运转到指定杯位
    {CAN_MASTER_CMD_H360_PLATE_REACTION_RUN_ANY_CUPS_POSITIVE , ENABLE, 5000 ,  3,  1 },//反应盘正向运转指定数量杯位
    {CAN_MASTER_CMD_H360_PLATE_REACTION_RUN_IDX_CUP_NEGATIVE  , ENABLE, 5000 ,  3,  1 },//反应盘反向运转到指定杯位
    {CAN_MASTER_CMD_H360_PLATE_REACTION_RUN_ANY_CUPS_NEGATIVE , ENABLE, 5000 ,  3,  1 },//反应盘反向运转指定数量杯位
};

//将获取到的数据转换为结构体
static void CanMasterPlateReactionGetMsgSet(uint8_t* buf,PLATE_REACTION_MSG* msgSetPtr)
{
    msgSetPtr->currentNewCupIndex = buf[0];
    msgSetPtr->cupExistNewPos = (PIN_STATE)buf[1];
    msgSetPtr->cupExistOldPos = (PIN_STATE)buf[2];
}

/*************************************发送指令等待ACK************************************************/
//模块内输入读取
void CanMasterPlateReactionReadSingleWhileAck(uint8_t modInIdx,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_PLATE_REACTION;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_PLATE_REACTION_PORT_IN;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_PLATE_REACTION,CAN_BOARD_ID_PLATE_REACTION,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterPlateReactionCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterPlateReactionCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_PLATE_REACTION,CAN_BOARD_ID_PLATE_REACTION,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)modInIdx;
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//模块内输出写入
void CanMasterPlateReactionWriteSingleWhileAck(uint8_t modOutIdx,PIN_STATE pinState,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_PLATE_REACTION;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_PLATE_REACTION_PORT_OUT;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_PLATE_REACTION,CAN_BOARD_ID_PLATE_REACTION,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterPlateReactionCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterPlateReactionCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_PLATE_REACTION,CAN_BOARD_ID_PLATE_REACTION,singalChannel,channelCmdCode,canCmdConfigPtr);
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
void CanMasterPlateReactionResetMotorWhileAck(uint8_t modMotorIdx,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_PLATE_REACTION;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_PLATE_REACTION_MOTOR_RESET;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_PLATE_REACTION,CAN_BOARD_ID_PLATE_REACTION,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterPlateReactionCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterPlateReactionCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_PLATE_REACTION,CAN_BOARD_ID_PLATE_REACTION,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)modMotorIdx;
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//模块内电机走位
void CanMasterPlateReactionRunStepsMotorWhileAck(uint8_t modMotorIdx,int32_t steps,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_PLATE_REACTION;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_PLATE_REACTION_MOTOR_RUN_STEPS;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_PLATE_REACTION,CAN_BOARD_ID_PLATE_REACTION,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterPlateReactionCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterPlateReactionCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_PLATE_REACTION,CAN_BOARD_ID_PLATE_REACTION,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)modMotorIdx;
    ProtUtilWriteInt32ToBuffer(steps,canCmdSendPtr->cmdBufferPtr+1);
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//反应盘复位
void CanMasterPlateReactionResetWhileAck(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_PLATE_REACTION;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_PLATE_REACTION_RESET;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_PLATE_REACTION,CAN_BOARD_ID_PLATE_REACTION,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterPlateReactionCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterPlateReactionCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_PLATE_REACTION,CAN_BOARD_ID_PLATE_REACTION,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    asm("nop");
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//反应盘正向运转到指定杯位
void CanMasterPlateReactionRunIndexCupPositiveWhileAck(uint8_t targetCup,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_PLATE_REACTION;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_PLATE_REACTION_RUN_IDX_CUP_POSITIVE;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_PLATE_REACTION,CAN_BOARD_ID_PLATE_REACTION,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterPlateReactionCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterPlateReactionCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_PLATE_REACTION,CAN_BOARD_ID_PLATE_REACTION,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)targetCup;
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//反应盘正向运转指定数量杯位
void CanMasterPlateReactionRunAnyCupsPositiveWhileAck(uint8_t cups,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_PLATE_REACTION;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_PLATE_REACTION_RUN_ANY_CUPS_POSITIVE;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_PLATE_REACTION,CAN_BOARD_ID_PLATE_REACTION,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterPlateReactionCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterPlateReactionCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_PLATE_REACTION,CAN_BOARD_ID_PLATE_REACTION,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)cups;
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//反应盘反向运转到指定杯位
void CanMasterPlateReactionRunIndexCupNegativeWhileAck(uint8_t targetCup,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_PLATE_REACTION;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_PLATE_REACTION_RUN_IDX_CUP_NEGATIVE;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_PLATE_REACTION,CAN_BOARD_ID_PLATE_REACTION,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterPlateReactionCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterPlateReactionCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_PLATE_REACTION,CAN_BOARD_ID_PLATE_REACTION,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)targetCup;
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//反应盘反向运转指定数量杯位
void CanMasterPlateReactionRunAnyCupsNegativeWhileAck(uint8_t cups,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_PLATE_REACTION;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_PLATE_REACTION_RUN_ANY_CUPS_NEGATIVE;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_PLATE_REACTION,CAN_BOARD_ID_PLATE_REACTION,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterPlateReactionCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterPlateReactionCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_PLATE_REACTION,CAN_BOARD_ID_PLATE_REACTION,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)cups;
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

/*************************************等待指定指令完成返回************************************************/
//模块内输入读取
void CanMasterPlateReactionWaitReadSingleResult(SYS_RESULT_PACK* resultPackPtr,PIN_STATE* pinStatePtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_PLATE_REACTION;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_PLATE_REACTION_PORT_IN;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_PLATE_REACTION,CAN_BOARD_ID_PLATE_REACTION,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_PLATE_REACTION,CAN_BOARD_ID_PLATE_REACTION,returnDataBufferPtr,returnDataBufferLength);
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
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_PLATE_REACTION,CAN_BOARD_ID_PLATE_REACTION,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//模块内输出写入
void CanMasterPlateReactionWaitWriteSingleResult(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_PLATE_REACTION;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_PLATE_REACTION_PORT_OUT;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_PLATE_REACTION,CAN_BOARD_ID_PLATE_REACTION,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_PLATE_REACTION,CAN_BOARD_ID_PLATE_REACTION,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //参数解析
    asm("nop");
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_PLATE_REACTION,CAN_BOARD_ID_PLATE_REACTION,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//模块内电机复位
void CanMasterPlateReactionWaitResetMotorResult(SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_PLATE_REACTION;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_PLATE_REACTION_MOTOR_RESET;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_PLATE_REACTION,CAN_BOARD_ID_PLATE_REACTION,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_PLATE_REACTION,CAN_BOARD_ID_PLATE_REACTION,returnDataBufferPtr,returnDataBufferLength);
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
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_PLATE_REACTION,CAN_BOARD_ID_PLATE_REACTION,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//模块内电机走位
void CanMasterPlateReactionWaitRunStepsMotorResult(SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_PLATE_REACTION;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_PLATE_REACTION_MOTOR_RUN_STEPS;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_PLATE_REACTION,CAN_BOARD_ID_PLATE_REACTION,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_PLATE_REACTION,CAN_BOARD_ID_PLATE_REACTION,returnDataBufferPtr,returnDataBufferLength);
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
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_PLATE_REACTION,CAN_BOARD_ID_PLATE_REACTION,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//反应盘复位
void CanMasterPlateReactionWaitResetWhileResult(SYS_RESULT_PACK* resultPackPtr,PLATE_REACTION_MSG* msgSetPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_PLATE_REACTION;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_PLATE_REACTION_RESET;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_PLATE_REACTION,CAN_BOARD_ID_PLATE_REACTION,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_PLATE_REACTION,CAN_BOARD_ID_PLATE_REACTION,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //参数解析
    if(msgSetPtr != NULL)
    {
        //结果解析
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            CanMasterPlateReactionGetMsgSet(returnDataBufferPtr,msgSetPtr);
        }
    }
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_PLATE_REACTION,CAN_BOARD_ID_PLATE_REACTION,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//反应盘正向运转到指定杯位
void CanMasterPlateReactionWaitRunIndexCupPositiveWhileResult(SYS_RESULT_PACK* resultPackPtr,PLATE_REACTION_MSG* msgSetPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_PLATE_REACTION;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_PLATE_REACTION_RUN_IDX_CUP_POSITIVE;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_PLATE_REACTION,CAN_BOARD_ID_PLATE_REACTION,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_PLATE_REACTION,CAN_BOARD_ID_PLATE_REACTION,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //参数解析
    if(msgSetPtr != NULL)
    {
        //结果解析
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            CanMasterPlateReactionGetMsgSet(returnDataBufferPtr,msgSetPtr);
        }
    }
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_PLATE_REACTION,CAN_BOARD_ID_PLATE_REACTION,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//反应盘正向运转指定数量杯位
void CanMasterPlateReactionWaitRunAnyCupsPositiveWhileResult(SYS_RESULT_PACK* resultPackPtr,PLATE_REACTION_MSG* msgSetPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_PLATE_REACTION;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_PLATE_REACTION_RUN_ANY_CUPS_POSITIVE;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_PLATE_REACTION,CAN_BOARD_ID_PLATE_REACTION,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_PLATE_REACTION,CAN_BOARD_ID_PLATE_REACTION,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //参数解析
    if(msgSetPtr != NULL)
    {
        //结果解析
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            CanMasterPlateReactionGetMsgSet(returnDataBufferPtr,msgSetPtr);
        }
    }
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_PLATE_REACTION,CAN_BOARD_ID_PLATE_REACTION,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//反应盘反向运转到指定杯位
void CanMasterPlateReactionWaitRunIndexCupNegativeWhileResult(SYS_RESULT_PACK* resultPackPtr,PLATE_REACTION_MSG* msgSetPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_PLATE_REACTION;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_PLATE_REACTION_RUN_IDX_CUP_NEGATIVE;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_PLATE_REACTION,CAN_BOARD_ID_PLATE_REACTION,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_PLATE_REACTION,CAN_BOARD_ID_PLATE_REACTION,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //参数解析
    if(msgSetPtr != NULL)
    {
        //结果解析
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            CanMasterPlateReactionGetMsgSet(returnDataBufferPtr,msgSetPtr);
        }
    }
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_PLATE_REACTION,CAN_BOARD_ID_PLATE_REACTION,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//反应盘反向运转指定数量杯位
void CanMasterPlateReactionWaitRunAnyCupsNegativeWhileResult(SYS_RESULT_PACK* resultPackPtr,PLATE_REACTION_MSG* msgSetPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_PLATE_REACTION;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_PLATE_REACTION_RUN_ANY_CUPS_NEGATIVE;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_PLATE_REACTION,CAN_BOARD_ID_PLATE_REACTION,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_PLATE_REACTION,CAN_BOARD_ID_PLATE_REACTION,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //参数解析
    if(msgSetPtr != NULL)
    {
        //结果解析
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            CanMasterPlateReactionGetMsgSet(returnDataBufferPtr,msgSetPtr);
        }
    }
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_PLATE_REACTION,CAN_BOARD_ID_PLATE_REACTION,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}


/*************************************发送指令等待完成返回************************************************/
//模块内输入读取
void CanMasterPlateReactionReadSingleWhileReturn(uint8_t modInIdx,SYS_RESULT_PACK* resultPackPtr,PIN_STATE* pinStatePtr)
{
    //发送指令等待ACK
    CanMasterPlateReactionReadSingleWhileAck(modInIdx,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterPlateReactionReadSingleWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterPlateReactionWaitReadSingleResult(resultPackPtr,pinStatePtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterPlateReactionWaitReadSingleResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//模块内输出写入
void CanMasterPlateReactionWriteSingleWhileReturn(uint8_t modOutIdx,PIN_STATE pinState,SYS_RESULT_PACK* resultPackPtr)
{
    //发送指令等待ACK
    CanMasterPlateReactionWriteSingleWhileAck(modOutIdx,pinState,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterPlateReactionWriteSingleWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterPlateReactionWaitWriteSingleResult(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterPlateReactionWaitWriteSingleResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//模块内电机复位
void CanMasterPlateReactionResetMotorWhileReturn(uint8_t modMotorIdx,SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr)
{
    //发送指令等待ACK
    CanMasterPlateReactionResetMotorWhileAck(modMotorIdx,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterPlateReactionResetMotorWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterPlateReactionWaitResetMotorResult(resultPackPtr,motorCurPosPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterPlateReactionWaitResetMotorResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//模块内电机走位
void CanMasterPlateReactionRunStepsMotorWhileReturn(uint8_t modMotorIdx,int32_t steps,SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr)
{
    //发送指令等待ACK
    CanMasterPlateReactionRunStepsMotorWhileAck(modMotorIdx,steps,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterPlateReactionRunStepsMotorWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterPlateReactionWaitRunStepsMotorResult(resultPackPtr,motorCurPosPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterPlateReactionWaitRunStepsMotorResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//反应盘复位
void CanMasterPlateReactionResetWhileReturn(SYS_RESULT_PACK* resultPackPtr,PLATE_REACTION_MSG* msgSetPtr)
{
    //发送指令等待ACK
    CanMasterPlateReactionResetWhileAck(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterPlateReactionResetWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterPlateReactionWaitResetWhileResult(resultPackPtr,msgSetPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterPlateReactionWaitResetWhileResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//反应盘正向运转到指定杯位
void CanMasterPlateReactionRunIndexCupPositiveWhileReturn(uint8_t targetCup,SYS_RESULT_PACK* resultPackPtr,PLATE_REACTION_MSG* msgSetPtr)
{
    //发送指令等待ACK
    CanMasterPlateReactionRunIndexCupPositiveWhileAck(targetCup,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterPlateReactionRunIndexCupPositiveWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterPlateReactionWaitRunIndexCupPositiveWhileResult(resultPackPtr,msgSetPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterPlateReactionWaitRunIndexCupPositiveWhileResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//反应盘正向运转指定数量杯位
void CanMasterPlateReactionRunAnyCupsPositiveWhileReturn(uint8_t cups,SYS_RESULT_PACK* resultPackPtr,PLATE_REACTION_MSG* msgSetPtr)
{
    //发送指令等待ACK
    CanMasterPlateReactionRunAnyCupsPositiveWhileAck(cups,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterPlateReactionRunAnyCupsPositiveWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterPlateReactionWaitRunAnyCupsPositiveWhileResult(resultPackPtr,msgSetPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterPlateReactionWaitRunAnyCupsPositiveWhileResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//反应盘反向运转到指定杯位
void CanMasterPlateReactionRunIndexCupNegativeWhileReturn(uint8_t targetCup,SYS_RESULT_PACK* resultPackPtr,PLATE_REACTION_MSG* msgSetPtr)
{
    //发送指令等待ACK
    CanMasterPlateReactionRunIndexCupNegativeWhileAck(targetCup,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterPlateReactionRunIndexCupNegativeWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterPlateReactionWaitRunIndexCupNegativeWhileResult(resultPackPtr,msgSetPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterPlateReactionWaitRunIndexCupNegativeWhileResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//反应盘反向运转指定数量杯位
void CanMasterPlateReactionRunAnyCupsNegativeWhileReturn(uint8_t cups,SYS_RESULT_PACK* resultPackPtr,PLATE_REACTION_MSG* msgSetPtr)
{
    //发送指令等待ACK
    CanMasterPlateReactionRunAnyCupsNegativeWhileAck(cups,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterPlateReactionRunAnyCupsNegativeWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterPlateReactionWaitRunAnyCupsNegativeWhileResult(resultPackPtr,msgSetPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterPlateReactionWaitRunAnyCupsNegativeWhileResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}




















