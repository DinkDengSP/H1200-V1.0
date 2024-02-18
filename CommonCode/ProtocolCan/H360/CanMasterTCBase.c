/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-24 10:44:38 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2024-01-08 13:26:48 +0800
************************************************************************************************/ 
#include "CanMasterTCBase.h"
#include "DriverConfigSystem.h"
#include SYS_LOG_INCLUDE_FILE

//通道配置数组
//参数分别是 指令  是否需要结果  超时时间  返回的最小参数长度 附带参数长度
//附带参数长度为不定长的时候,指示参数长度为0,由上级调用者手动申请内存
static const CAN_PROT_CMD_CONFIG canMasterTCBaseCmdConfigArray[] = {
    {CAN_MASTER_CMD_H360TC_REPORT_FLAG_SET                  ,   ENABLE,     3000,   0,  6  },//设置温控上报
    {CAN_MASTER_CMD_H360TC_GET_BOARD_TEMP_CONTROL           ,   ENABLE,     3000,   8,  0  },//获取板上控温状态
    {CAN_MASTER_CMD_H360TC_GET_ENVIR_TEMP_CONTROL           ,   ENABLE,     3000,   9,  0  },//获取环境控温状态
    {CAN_MASTER_CMD_H360TC_GET_PLATE_REACTION_TEMP_CONTROL  ,   ENABLE,     3000,   9,  0  },//获取反应盘控温状态 
    {CAN_MASTER_CMD_H360TC_GET_PLATE_WASH_TEMP_CONTROL      ,   ENABLE,     3000,   9,  0  },//获取清洗盘控温状态
    {CAN_MASTER_CMD_H360TC_GET_PLATE_REAGENT_TEMP_CONTROL   ,   ENABLE,     3000,   10,  0  },//获取试剂盘控温状态
    {CAN_MASTER_CMD_H360TC_GET_PLATE_REAGENT_FLOAT_SENSOR   ,   ENABLE,     3000,   1,  0  },//获取试剂盘浮球状态
    {CAN_MASTER_CMD_H360TC_GET_PLATE_REACTION_CURRENT       ,   ENABLE,     3000,   4,  0  },//获取反应盘电流检测
    {CAN_MASTER_CMD_H360TC_GET_PLATE_WASH_CURRENT           ,   ENABLE,     3000,   4,  0  },//获取清洗盘电流检测
    {CAN_MASTER_CMD_H360TC_GET_PLATE_REAGENT_CURRENT        ,   ENABLE,     3000,   16, 0  },//获取试剂盘电流检测
    {CAN_MASTER_CMD_H360TC_SET_ADD_REFRIGERANT_MODE         ,   ENABLE,     3000,   0,  1  },//设置添加制冷液模式
};

/*************************************发送指令等待ACK************************************************/
//开启指定通道温度上报
void CanMasterTCBaseReportFlagSetWhileAck(TC_BASE_CHANNEL channel,FunctionalState flagSet,uint32_t intervalTimeMs,
                                            SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_TC_BASE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360TC_REPORT_FLAG_SET;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_TEMP_CONTROL,CAN_BOARD_ID_TEMP_CONTROL,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterTCBaseCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterTCBaseCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_TEMP_CONTROL,CAN_BOARD_ID_TEMP_CONTROL,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)channel;
    canCmdSendPtr->cmdBufferPtr[1] = (uint8_t)flagSet;
    ProtUtilWriteUint32ToBuffer(intervalTimeMs,canCmdSendPtr->cmdBufferPtr+2);
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//获取板上控温状态
void CanMasterTCBaseGetBoardTempControlWhileAck(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_TC_BASE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360TC_GET_BOARD_TEMP_CONTROL;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_TEMP_CONTROL,CAN_BOARD_ID_TEMP_CONTROL,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterTCBaseCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterTCBaseCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_TEMP_CONTROL,CAN_BOARD_ID_TEMP_CONTROL,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    asm("nop");
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//获取环境控温状态
void CanMasterTCBaseGetEnvirTempControlWhileAck(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_TC_BASE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360TC_GET_ENVIR_TEMP_CONTROL;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_TEMP_CONTROL,CAN_BOARD_ID_TEMP_CONTROL,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterTCBaseCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterTCBaseCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_TEMP_CONTROL,CAN_BOARD_ID_TEMP_CONTROL,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    asm("nop");
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//获取反应盘控温状态
void CanMasterTCBaseGetPlateReactionTempControlWhileAck(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_TC_BASE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360TC_GET_PLATE_REACTION_TEMP_CONTROL;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_TEMP_CONTROL,CAN_BOARD_ID_TEMP_CONTROL,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterTCBaseCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterTCBaseCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_TEMP_CONTROL,CAN_BOARD_ID_TEMP_CONTROL,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    asm("nop");
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//获取清洗盘控温状态
void CanMasterTCBaseGetPlateWashTempControlWhileAck(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_TC_BASE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360TC_GET_PLATE_WASH_TEMP_CONTROL;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_TEMP_CONTROL,CAN_BOARD_ID_TEMP_CONTROL,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterTCBaseCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterTCBaseCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_TEMP_CONTROL,CAN_BOARD_ID_TEMP_CONTROL,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    asm("nop");
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//获取试剂盘控温状态
void CanMasterTCBaseGetPlateReagentTempControlWhileAck(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_TC_BASE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360TC_GET_PLATE_REAGENT_TEMP_CONTROL;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_TEMP_CONTROL,CAN_BOARD_ID_TEMP_CONTROL,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterTCBaseCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterTCBaseCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_TEMP_CONTROL,CAN_BOARD_ID_TEMP_CONTROL,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    asm("nop");
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//获取试剂盘浮球状态
void CanMasterTCBaseGetPlateReagentFloatSensorWhileAck(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_TC_BASE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360TC_GET_PLATE_REAGENT_FLOAT_SENSOR;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_TEMP_CONTROL,CAN_BOARD_ID_TEMP_CONTROL,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterTCBaseCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterTCBaseCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_TEMP_CONTROL,CAN_BOARD_ID_TEMP_CONTROL,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    asm("nop");
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//获取反应盘电流检测值
void CanMasterTCBaseGetPlateReactionCurrentWhileAck(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_TC_BASE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360TC_GET_PLATE_REACTION_CURRENT;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_TEMP_CONTROL,CAN_BOARD_ID_TEMP_CONTROL,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterTCBaseCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterTCBaseCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_TEMP_CONTROL,CAN_BOARD_ID_TEMP_CONTROL,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    asm("nop");
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//获取清洗盘电流检测值
void CanMasterTCBaseGetPlateWashCurrentWhileAck(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_TC_BASE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360TC_GET_PLATE_WASH_CURRENT;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_TEMP_CONTROL,CAN_BOARD_ID_TEMP_CONTROL,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterTCBaseCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterTCBaseCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_TEMP_CONTROL,CAN_BOARD_ID_TEMP_CONTROL,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    asm("nop");
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//获取试剂盘电流检测值
void CanMasterTCBaseGetPlateReagentCurrentWhileAck(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_TC_BASE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360TC_GET_PLATE_REAGENT_CURRENT;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_TEMP_CONTROL,CAN_BOARD_ID_TEMP_CONTROL,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterTCBaseCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterTCBaseCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_TEMP_CONTROL,CAN_BOARD_ID_TEMP_CONTROL,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    asm("nop");
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//设置试剂盘增加制冷液模式
void CanMasterTCBaseSetAddRefrigerantModeWhileAck(FunctionalState setMode,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_TC_BASE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360TC_SET_ADD_REFRIGERANT_MODE;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_TEMP_CONTROL,CAN_BOARD_ID_TEMP_CONTROL,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterTCBaseCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterTCBaseCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_TEMP_CONTROL,CAN_BOARD_ID_TEMP_CONTROL,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)setMode;
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}
/*************************************等待指定指令完成返回************************************************/
//开启指定通道温度上报
void CanMasterTCBaseWaitReportFlagSetResult(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_TC_BASE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360TC_REPORT_FLAG_SET;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_TEMP_CONTROL,CAN_BOARD_ID_TEMP_CONTROL,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_TEMP_CONTROL,CAN_BOARD_ID_TEMP_CONTROL,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //参数解析
    asm("nop");
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_TEMP_CONTROL,CAN_BOARD_ID_TEMP_CONTROL,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//获取板上控温状态
void CanMasterTCBaseWaitGetBoardTempControlResult(float* tempValPtr,ERROR_SUB* errorSensorPtr,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_TC_BASE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360TC_GET_BOARD_TEMP_CONTROL;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_TEMP_CONTROL,CAN_BOARD_ID_TEMP_CONTROL,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_TEMP_CONTROL,CAN_BOARD_ID_TEMP_CONTROL,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //参数解析
    if(tempValPtr != NULL)
    {
        //结果解析
        *tempValPtr = 0.0f;
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            *tempValPtr = (float)(ProtUtilReadInt32Buffer(returnDataBufferPtr));
            *tempValPtr /= 100.0f;
        }
    }
    if(errorSensorPtr != NULL)
    {
        *errorSensorPtr = ERROR_SUB_OK;
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            *errorSensorPtr = (ERROR_SUB)(ProtUtilReadUint32Buffer(returnDataBufferPtr+4));
        }
    }
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_TEMP_CONTROL,CAN_BOARD_ID_TEMP_CONTROL,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//获取环境控温状态
void CanMasterTCBaseWaitGetEnvirTempControlResult(float* tempValPtr,ERROR_SUB* errorSensorPtr,TEMP_CONTROL_STATE* controlStatePtr,
                                                    SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_TC_BASE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360TC_GET_ENVIR_TEMP_CONTROL;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_TEMP_CONTROL,CAN_BOARD_ID_TEMP_CONTROL,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_TEMP_CONTROL,CAN_BOARD_ID_TEMP_CONTROL,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //参数解析
    if(tempValPtr != NULL)
    {
        //结果解析
        *tempValPtr = 0.0f;
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            *tempValPtr = (float)(ProtUtilReadInt32Buffer(returnDataBufferPtr));
            *tempValPtr /= 100.0f;
        }
    }
    if(errorSensorPtr != NULL)
    {
        *errorSensorPtr = ERROR_SUB_OK;
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            *errorSensorPtr = (ERROR_SUB)(ProtUtilReadUint32Buffer(returnDataBufferPtr+4));
        }
    }
    if(controlStatePtr != NULL)
    {
        *controlStatePtr = TEMP_CONTROL_STATE_IDLE;
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            *controlStatePtr = (TEMP_CONTROL_STATE)(returnDataBufferPtr[8]);
        }
    }
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_TEMP_CONTROL,CAN_BOARD_ID_TEMP_CONTROL,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//获取反应盘控温状态
void CanMasterTCBaseWaitGetPlateReactionTempControlResult(float* tempValPtr,ERROR_SUB* errorSensorPtr,TEMP_CONTROL_STATE* controlStatePtr,
                                                    SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_TC_BASE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360TC_GET_PLATE_REACTION_TEMP_CONTROL;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_TEMP_CONTROL,CAN_BOARD_ID_TEMP_CONTROL,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_TEMP_CONTROL,CAN_BOARD_ID_TEMP_CONTROL,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //参数解析
    if(tempValPtr != NULL)
    {
        //结果解析
        *tempValPtr = 0.0f;
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            *tempValPtr = (float)(ProtUtilReadInt32Buffer(returnDataBufferPtr));
            *tempValPtr /= 100.0f;
        }
    }
    if(errorSensorPtr != NULL)
    {
        *errorSensorPtr = ERROR_SUB_OK;
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            *errorSensorPtr = (ERROR_SUB)(ProtUtilReadUint32Buffer(returnDataBufferPtr+4));
        }
    }
    if(controlStatePtr != NULL)
    {
        *controlStatePtr = TEMP_CONTROL_STATE_IDLE;
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            *controlStatePtr = (TEMP_CONTROL_STATE)(returnDataBufferPtr[8]);
        }
    }
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_TEMP_CONTROL,CAN_BOARD_ID_TEMP_CONTROL,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//获取清洗盘控温状态
void CanMasterTCBaseWaitPlateWashTempControlResult(float* tempValPtr,ERROR_SUB* errorSensorPtr,TEMP_CONTROL_STATE* controlStatePtr,
                                                    SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_TC_BASE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360TC_GET_PLATE_WASH_TEMP_CONTROL;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_TEMP_CONTROL,CAN_BOARD_ID_TEMP_CONTROL,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_TEMP_CONTROL,CAN_BOARD_ID_TEMP_CONTROL,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //参数解析
    if(tempValPtr != NULL)
    {
        //结果解析
        *tempValPtr = 0.0f;
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            *tempValPtr = (float)(ProtUtilReadInt32Buffer(returnDataBufferPtr));
            *tempValPtr /= 100.0f;
        }
    }
    if(errorSensorPtr != NULL)
    {
        *errorSensorPtr = ERROR_SUB_OK;
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            *errorSensorPtr = (ERROR_SUB)(ProtUtilReadUint32Buffer(returnDataBufferPtr+4));
        }
    }
    if(controlStatePtr != NULL)
    {
        *controlStatePtr = TEMP_CONTROL_STATE_IDLE;
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            *controlStatePtr = (TEMP_CONTROL_STATE)(returnDataBufferPtr[8]);
        }
    }
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_TEMP_CONTROL,CAN_BOARD_ID_TEMP_CONTROL,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//获取试剂盘控温状态
void CanMasterTCBaseWaitPlateReagentTempControlResult(float* tempValPtr,ERROR_SUB* errorSensorPtr,TEMP_CONTROL_STATE* controlStatePtr,
                                                    PIN_STATE* pinStatePtr,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_TC_BASE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360TC_GET_PLATE_REAGENT_TEMP_CONTROL;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_TEMP_CONTROL,CAN_BOARD_ID_TEMP_CONTROL,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_TEMP_CONTROL,CAN_BOARD_ID_TEMP_CONTROL,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //参数解析
    if(tempValPtr != NULL)
    {
        //结果解析
        *tempValPtr = 0.0f;
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            *tempValPtr = (float)(ProtUtilReadInt32Buffer(returnDataBufferPtr));
            *tempValPtr /= 100.0f;
        }
    }
    if(errorSensorPtr != NULL)
    {
        *errorSensorPtr = ERROR_SUB_OK;
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            *errorSensorPtr = (ERROR_SUB)(ProtUtilReadUint32Buffer(returnDataBufferPtr+4));
        }
    }
    if(controlStatePtr != NULL)
    {
        *controlStatePtr = TEMP_CONTROL_STATE_IDLE;
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            *controlStatePtr = (TEMP_CONTROL_STATE)(returnDataBufferPtr[8]);
        }
    }
    if(pinStatePtr != NULL)
    {
        *pinStatePtr = PIN_STATE_INVALID;
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            *pinStatePtr = (PIN_STATE)(returnDataBufferPtr[9]);
        }
    }
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_TEMP_CONTROL,CAN_BOARD_ID_TEMP_CONTROL,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//获取试剂盘浮球状态
void CanMasterTCBaseWaitGetPlateReagentFloatSensorResult(PIN_STATE* pinStatePtr,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_TC_BASE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360TC_GET_PLATE_REAGENT_FLOAT_SENSOR;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_TEMP_CONTROL,CAN_BOARD_ID_TEMP_CONTROL,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_TEMP_CONTROL,CAN_BOARD_ID_TEMP_CONTROL,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //参数解析
    if(pinStatePtr != NULL)
    {
        //结果解析
        *pinStatePtr = PIN_STATE_INVALID;
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            *pinStatePtr = (PIN_STATE)(returnDataBufferPtr[0]);
        }
    }
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_TEMP_CONTROL,CAN_BOARD_ID_TEMP_CONTROL,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//获取反应盘电流检测值
void CanMasterTCBaseWaitGetPlateReactionCurrentResult(int32_t* currentMaPtr,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_TC_BASE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360TC_GET_PLATE_REACTION_CURRENT;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_TEMP_CONTROL,CAN_BOARD_ID_TEMP_CONTROL,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_TEMP_CONTROL,CAN_BOARD_ID_TEMP_CONTROL,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //参数解析
    if(currentMaPtr != NULL)
    {
        //结果解析
        *currentMaPtr = 0;
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            *currentMaPtr = ProtUtilReadInt32Buffer(returnDataBufferPtr);
        }
    }
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_TEMP_CONTROL,CAN_BOARD_ID_TEMP_CONTROL,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//获取清洗盘电流检测值
void CanMasterTCBaseWaitGetPlateWashCurrentResult(int32_t* currentMaPtr,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_TC_BASE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360TC_GET_PLATE_WASH_CURRENT;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_TEMP_CONTROL,CAN_BOARD_ID_TEMP_CONTROL,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_TEMP_CONTROL,CAN_BOARD_ID_TEMP_CONTROL,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //参数解析
    if(currentMaPtr != NULL)
    {
        //结果解析
        *currentMaPtr = 0;
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            *currentMaPtr = ProtUtilReadInt32Buffer(returnDataBufferPtr);
        }
    }
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_TEMP_CONTROL,CAN_BOARD_ID_TEMP_CONTROL,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//获取试剂盘电流检测值
void CanMasterTCBaseWaitGetPlateReagentCurrentResult(int32_t* current1MaPtr,int32_t* current2MaPtr,int32_t* current3MaPtr,int32_t* current4MaPtr,
                                                        SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_TC_BASE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360TC_GET_PLATE_REAGENT_CURRENT;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_TEMP_CONTROL,CAN_BOARD_ID_TEMP_CONTROL,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_TEMP_CONTROL,CAN_BOARD_ID_TEMP_CONTROL,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //参数解析
    if(current1MaPtr != NULL)
    {
        //结果解析
        *current1MaPtr = 0;
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            *current1MaPtr = ProtUtilReadInt32Buffer(returnDataBufferPtr);
        }
    }
    if(current2MaPtr != NULL)
    {
        //结果解析
        *current2MaPtr = 0;
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            *current2MaPtr = ProtUtilReadInt32Buffer(returnDataBufferPtr+4);
        }
    }
    if(current3MaPtr != NULL)
    {
        //结果解析
        *current3MaPtr = 0;
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            *current3MaPtr = ProtUtilReadInt32Buffer(returnDataBufferPtr+8);
        }
    }
    if(current4MaPtr != NULL)
    {
        //结果解析
        *current4MaPtr = 0;
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            *current4MaPtr = ProtUtilReadInt32Buffer(returnDataBufferPtr+12);
        }
    }
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_TEMP_CONTROL,CAN_BOARD_ID_TEMP_CONTROL,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//设置试剂盘增加制冷液模式
void CanMasterTCBaseWaitSetAddRefrigerantModeResult(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_TC_BASE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360TC_SET_ADD_REFRIGERANT_MODE;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_TEMP_CONTROL,CAN_BOARD_ID_TEMP_CONTROL,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_TEMP_CONTROL,CAN_BOARD_ID_TEMP_CONTROL,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //参数解析
    asm("nop");
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_TEMP_CONTROL,CAN_BOARD_ID_TEMP_CONTROL,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

/*************************************发送指令等待完成返回************************************************/
//开启指定通道温度上报
void CanMasterTCBaseReportFlagSetWhileReturn(TC_BASE_CHANNEL channel,FunctionalState flagSet,uint32_t intervalTimeMs,
                                                SYS_RESULT_PACK* resultPackPtr)
{
    //发送指令等待ACK
    CanMasterTCBaseReportFlagSetWhileAck(channel,flagSet,intervalTimeMs,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterTCBaseReportFlagSetWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterTCBaseWaitReportFlagSetResult(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterTCBaseWaitReportFlagSetResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//获取板上控温状态
void CanMasterTCBaseGetBoardTempControlWhileReturn(float* tempValPtr,ERROR_SUB* errorSensorPtr,SYS_RESULT_PACK* resultPackPtr)
{
    //发送指令等待ACK
    CanMasterTCBaseGetBoardTempControlWhileAck(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterTCBaseGetBoardTempControlWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterTCBaseWaitGetBoardTempControlResult(tempValPtr,errorSensorPtr,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterTCBaseWaitGetBoardTempControlResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//获取环境控温状态
void CanMasterTCBaseGetEnvirTempControlWhileReturn(float* tempValPtr,ERROR_SUB* errorSensorPtr,TEMP_CONTROL_STATE* controlStatePtr,
                                                    SYS_RESULT_PACK* resultPackPtr)
{
    //发送指令等待ACK
    CanMasterTCBaseGetEnvirTempControlWhileAck(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterTCBaseGetEnvirTempControlWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterTCBaseWaitGetEnvirTempControlResult(tempValPtr,errorSensorPtr,controlStatePtr,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterTCBaseWaitGetEnvirTempControlResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//获取反应盘控温状态
void CanMasterTCBaseGetPlateReactionTempControlWhileReturn(float* tempValPtr,ERROR_SUB* errorSensorPtr,TEMP_CONTROL_STATE* controlStatePtr,
                                                    SYS_RESULT_PACK* resultPackPtr)
{
    //发送指令等待ACK
    CanMasterTCBaseGetPlateReactionTempControlWhileAck(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterTCBaseGetPlateReactionTempControlWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterTCBaseWaitGetPlateReactionTempControlResult(tempValPtr,errorSensorPtr,controlStatePtr,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterTCBaseWaitGetPlateReactionTempControlResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//获取清洗盘控温状态
void CanMasterTCBaseGetPlateWashTempControlWhileReturn(float* tempValPtr,ERROR_SUB* errorSensorPtr,TEMP_CONTROL_STATE* controlStatePtr,
                                                    SYS_RESULT_PACK* resultPackPtr)
{
    //发送指令等待ACK
    CanMasterTCBaseGetPlateWashTempControlWhileAck(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterTCBaseGetPlateWashTempControlWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterTCBaseWaitPlateWashTempControlResult(tempValPtr,errorSensorPtr,controlStatePtr,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterTCBaseWaitPlateWashTempControlResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//获取试剂盘控温状态
void CanMasterTCBaseGetPlateReagentTempControlWhileReturn(float* tempValPtr,ERROR_SUB* errorSensorPtr,TEMP_CONTROL_STATE* controlStatePtr,
                                                    PIN_STATE* pinStatePtr,SYS_RESULT_PACK* resultPackPtr)
{
    //发送指令等待ACK
    CanMasterTCBaseGetPlateReagentTempControlWhileAck(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterTCBaseGetPlateReagentTempControlWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterTCBaseWaitPlateReagentTempControlResult(tempValPtr,errorSensorPtr,controlStatePtr,pinStatePtr,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterTCBaseWaitPlateReagentTempControlResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//获取试剂盘浮球状态
void CanMasterTCBaseGetPlateReagentFloatSensorWhileReturn(PIN_STATE* pinStatePtr,SYS_RESULT_PACK* resultPackPtr)
{
    //发送指令等待ACK
    CanMasterTCBaseGetPlateReagentFloatSensorWhileAck(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterTCBaseGetPlateReagentFloatSensorWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterTCBaseWaitGetPlateReagentFloatSensorResult(pinStatePtr,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterTCBaseWaitGetPlateReagentFloatSensorResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//获取反应盘电流检测值
void CanMasterTCBaseGetPlateReactionCurrentWhileReturn(int32_t* currentMaPtr,SYS_RESULT_PACK* resultPackPtr)
{
    //发送指令等待ACK
    CanMasterTCBaseGetPlateReactionCurrentWhileAck(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterTCBaseGetPlateReactionCurrentWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterTCBaseWaitGetPlateReactionCurrentResult(currentMaPtr,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterTCBaseWaitGetPlateReactionCurrentResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//获取清洗盘电流检测值
void CanMasterTCBaseGetPlateWashCurrentWhileReturn(int32_t* currentMaPtr,SYS_RESULT_PACK* resultPackPtr)
{
    //发送指令等待ACK
    CanMasterTCBaseGetPlateWashCurrentWhileAck(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterTCBaseGetPlateWashCurrentWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterTCBaseWaitGetPlateWashCurrentResult(currentMaPtr,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterTCBaseWaitGetPlateWashCurrentResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//获取试剂盘电流检测值
void CanMasterTCBaseGetPlateReagentCurrentWhileReturn(int32_t* current1MaPtr,int32_t* current2MaPtr,int32_t* current3MaPtr,int32_t* current4MaPtr,
                                                        SYS_RESULT_PACK* resultPackPtr)
{
    //发送指令等待ACK
    CanMasterTCBaseGetPlateReagentCurrentWhileAck(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterTCBaseGetPlateReagentCurrentWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterTCBaseWaitGetPlateReagentCurrentResult(current1MaPtr,current2MaPtr,current3MaPtr,current4MaPtr,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterTCBaseWaitGetPlateReagentCurrentResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//设置试剂盘增加制冷液模式
void CanMasterTCBaseSetAddRefrigerantModeWhileReturn(FunctionalState setMode,SYS_RESULT_PACK* resultPackPtr)
{
    //发送指令等待ACK
    CanMasterTCBaseSetAddRefrigerantModeWhileAck(setMode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterTCBaseSetAddRefrigerantModeWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterTCBaseWaitSetAddRefrigerantModeResult(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterTCBaseWaitSetAddRefrigerantModeResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

/*************************************主动上报************************************************/
//上报板卡传感器信息
void CanMasterTCBaseReportBoardTemp(uint8_t targetModuleNo,uint8_t targetBoardID,float tempVal,ERROR_SUB errorSubSensor,
                                        SYS_RESULT_PACK* resultPackPtr)
{
    uint16_t reportDataLength = 8;
    uint8_t* reportBufferPtr = CanProtMemMalloc(targetModuleNo,targetBoardID,reportDataLength);
    uint32_t appendErrorSubBoardID = 0X00000000;
    PORT_CAN protCanReportPC = CanProtPortCalcPort(targetModuleNo,targetBoardID);
    //对ERROR SENSOR做处理
    if((errorSubSensor != 0X00000000)&&((errorSubSensor&0XFF000000) == 0X00000000))
    {
        //加入板号
        if(PORT_CAN1 == protCanReportPC)
        {
            appendErrorSubBoardID += Can1ProtPortGetSelfBoardID();
        }
        else
        {
            appendErrorSubBoardID += Can2ProtPortGetSelfBoardID();
        }
        appendErrorSubBoardID <<= 24;
        errorSubSensor += appendErrorSubBoardID;
    }
    //写入传感器值
    ProtUtilWriteInt32ToBuffer((int32_t)(tempVal*100.0f),reportBufferPtr);
    ProtUtilWriteUint32ToBuffer((uint32_t)(errorSubSensor),reportBufferPtr+4);
    Can1ProtSendUpload(targetModuleNo,targetBoardID,(uint32_t)CAN_MASTER_REPORT_H360_BOARD_TEMP,
                                            reportBufferPtr,reportDataLength,resultPackPtr);
    //释放内存
    CanProtMemFree(targetModuleNo,targetBoardID,reportBufferPtr);
    return;
}

//上报环境控温信息
void CanMasterTCBaseReportEnvirTempControl(uint8_t targetModuleNo,uint8_t targetBoardID,float tempVal,ERROR_SUB errorSubSensor,
                                            TEMP_CONTROL_STATE controlState,SYS_RESULT_PACK* resultPackPtr)
{
    uint16_t reportDataLength = 9;
    uint8_t* reportBufferPtr = CanProtMemMalloc(targetModuleNo,targetBoardID,reportDataLength);
    uint32_t appendErrorSubBoardID = 0X00000000;
    PORT_CAN protCanReportPC = CanProtPortCalcPort(targetModuleNo,targetBoardID);
    //对ERROR SENSOR做处理
    if((errorSubSensor != 0X00000000)&&((errorSubSensor&0XFF000000) == 0X00000000))
    {
        //加入板号
        if(PORT_CAN1 == protCanReportPC)
        {
            appendErrorSubBoardID += Can1ProtPortGetSelfBoardID();
        }
        else
        {
            appendErrorSubBoardID += Can2ProtPortGetSelfBoardID();
        }
        appendErrorSubBoardID <<= 24;
        errorSubSensor += appendErrorSubBoardID;
    }
    //写入传感器值
    ProtUtilWriteInt32ToBuffer((int32_t)(tempVal*100.0f),reportBufferPtr);
    ProtUtilWriteUint32ToBuffer((uint32_t)(errorSubSensor),reportBufferPtr+4);
    reportBufferPtr[8] = (uint8_t)controlState;
    Can1ProtSendUpload(targetModuleNo,targetBoardID,(uint32_t)CAN_MASTER_REPORT_H360_ENVIR_TEMP_CONTROL,
                                            reportBufferPtr,reportDataLength,resultPackPtr);
    //释放内存
    CanProtMemFree(targetModuleNo,targetBoardID,reportBufferPtr);
    return;
}

//上报反应盘控温信息
void CanMasterTCBaseReportPlateReactionTempControl(uint8_t targetModuleNo,uint8_t targetBoardID,float tempVal,ERROR_SUB errorSubSensor,
                                                    TEMP_CONTROL_STATE controlState,SYS_RESULT_PACK* resultPackPtr)
{
    uint16_t reportDataLength = 9;
    uint8_t* reportBufferPtr = CanProtMemMalloc(targetModuleNo,targetBoardID,reportDataLength);
    uint32_t appendErrorSubBoardID = 0X00000000;
    PORT_CAN protCanReportPC = CanProtPortCalcPort(targetModuleNo,targetBoardID);
    //对ERROR SENSOR做处理
    if((errorSubSensor != 0X00000000)&&((errorSubSensor&0XFF000000) == 0X00000000))
    {
        //加入板号
        if(PORT_CAN1 == protCanReportPC)
        {
            appendErrorSubBoardID += Can1ProtPortGetSelfBoardID();
        }
        else
        {
            appendErrorSubBoardID += Can2ProtPortGetSelfBoardID();
        }
        appendErrorSubBoardID <<= 24;
        errorSubSensor += appendErrorSubBoardID;
    }
    //写入传感器值
    ProtUtilWriteInt32ToBuffer((int32_t)(tempVal*100.0f),reportBufferPtr);
    ProtUtilWriteUint32ToBuffer((uint32_t)(errorSubSensor),reportBufferPtr+4);
    reportBufferPtr[8] = (uint8_t)controlState;
    Can1ProtSendUpload(targetModuleNo,targetBoardID,(uint32_t)CAN_MASTER_REPORT_H360_PLATE_REACTION_TEMP_CONTROL,
                                            reportBufferPtr,reportDataLength,resultPackPtr);
    //释放内存
    CanProtMemFree(targetModuleNo,targetBoardID,reportBufferPtr);
    return;
}

//上报清洗盘控温信息
void CanMasterTCBaseReportPlateWashTempControl(uint8_t targetModuleNo,uint8_t targetBoardID,float tempVal,ERROR_SUB errorSubSensor,
                                                TEMP_CONTROL_STATE controlState,SYS_RESULT_PACK* resultPackPtr)
{
    uint16_t reportDataLength = 9;
    uint8_t* reportBufferPtr = CanProtMemMalloc(targetModuleNo,targetBoardID,reportDataLength);
    uint32_t appendErrorSubBoardID = 0X00000000;
    PORT_CAN protCanReportPC = CanProtPortCalcPort(targetModuleNo,targetBoardID);
    //对ERROR SENSOR做处理
    if((errorSubSensor != 0X00000000)&&((errorSubSensor&0XFF000000) == 0X00000000))
    {
        //加入板号
        if(PORT_CAN1 == protCanReportPC)
        {
            appendErrorSubBoardID += Can1ProtPortGetSelfBoardID();
        }
        else
        {
            appendErrorSubBoardID += Can2ProtPortGetSelfBoardID();
        }
        appendErrorSubBoardID <<= 24;
        errorSubSensor += appendErrorSubBoardID;
    }
    //写入传感器值
    ProtUtilWriteInt32ToBuffer((int32_t)(tempVal*100.0f),reportBufferPtr);
    ProtUtilWriteUint32ToBuffer((uint32_t)(errorSubSensor),reportBufferPtr+4);
    reportBufferPtr[8] = (uint8_t)controlState;
    Can1ProtSendUpload(targetModuleNo,targetBoardID,(uint32_t)CAN_MASTER_REPORT_H360_PLATE_WASH_TEMP_CONTROL,
                                            reportBufferPtr,reportDataLength,resultPackPtr);
    //释放内存
    CanProtMemFree(targetModuleNo,targetBoardID,reportBufferPtr);
    return;
}

//上报试剂盘控温信息
void CanMasterTCBaseReportPlateReagentTempControl(uint8_t targetModuleNo,uint8_t targetBoardID,float tempVal,ERROR_SUB errorSubSensor,
                                                    TEMP_CONTROL_STATE controlState,PIN_STATE floatSensorState,
                                                    SYS_RESULT_PACK* resultPackPtr)
{
    uint16_t reportDataLength = 10;
    uint8_t* reportBufferPtr = CanProtMemMalloc(targetModuleNo,targetBoardID,reportDataLength);
    uint32_t appendErrorSubBoardID = 0X00000000;
    PORT_CAN protCanReportPC = CanProtPortCalcPort(targetModuleNo,targetBoardID);
    //对ERROR SENSOR做处理
    if((errorSubSensor != 0X00000000)&&((errorSubSensor&0XFF000000) == 0X00000000))
    {
        //加入板号
        if(PORT_CAN1 == protCanReportPC)
        {
            appendErrorSubBoardID += Can1ProtPortGetSelfBoardID();
        }
        else
        {
            appendErrorSubBoardID += Can2ProtPortGetSelfBoardID();
        }
        appendErrorSubBoardID <<= 24;
        errorSubSensor += appendErrorSubBoardID;
    }
    //写入传感器值
    ProtUtilWriteInt32ToBuffer((int32_t)(tempVal*100.0f),reportBufferPtr);
    ProtUtilWriteUint32ToBuffer((uint32_t)(errorSubSensor),reportBufferPtr+4);
    reportBufferPtr[8] = (uint8_t)controlState;
    reportBufferPtr[9] = (uint8_t)floatSensorState;
    Can1ProtSendUpload(targetModuleNo,targetBoardID,(uint32_t)CAN_MASTER_REPORT_H360_PLATE_REAGENT_TEMP_CONTROL,
                                            reportBufferPtr,reportDataLength,resultPackPtr);
    //释放内存
    CanProtMemFree(targetModuleNo,targetBoardID,reportBufferPtr);
    return;
}

