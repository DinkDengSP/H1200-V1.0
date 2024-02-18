/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-24 10:44:38 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-24 13:52:03 +0800
************************************************************************************************/ 
#include "CanMasterBoard1SampleHand.h"
#include "DriverConfigSystem.h"
#include SYS_LOG_INCLUDE_FILE

//通道配置数组
//参数分别是 指令  是否需要结果  超时时间  最小参数长度 附带参数长度
//附带参数长度为不定长的时候,指示参数长度为0,由上级调用者手动申请内存
static const CAN_PROT_CMD_CONFIG canMasterBoard1SampleHandCmdConfigArray[] = {
    {CAN_RAIL_MUTIL_CMD_BOARD1_SAMPLE_HAND_READ_INPUT                            ,        ENABLE,     60000,      2,      1       },//转运机械手 输入读取
    {CAN_RAIL_MUTIL_CMD_BOARD1_SAMPLE_HAND_WRITE_OUTPUT                          ,        ENABLE,     60000,      1,      2       },//转运机械手 输出写入
    {CAN_RAIL_MUTIL_CMD_BOARD1_SAMPLE_HAND_MOTOR_RESET                           ,        ENABLE,     60000,      5,      1       },//转运机械手 电机复位
    {CAN_RAIL_MUTIL_CMD_BOARD1_SAMPLE_HAND_MOTOR_RUN_STEP                        ,        ENABLE,     60000,      5,      5       },//转运机械手 电机走步数
    {CAN_RAIL_MUTIL_CMD_BOARD1_SAMPLE_HAND_RESET_ALL                             ,        ENABLE,     60000,      1,      0       },//转运机械手 复位
    {CAN_RAIL_MUTIL_CMD_BOARD1_SAMPLE_HAND_MOVE_TO_TARGET_POS                    ,        ENABLE,     60000,      0,      3       },//转运机械手 移动到指定位置
    {CAN_RAIL_MUTIL_CMD_BOARD1_SAMPLE_HAND_RACK_EXIST_SCAN                       ,        ENABLE,     60000,      1,      0       },//转运机械手 样本仓扫描
    {CAN_RAIL_MUTIL_CMD_BOARD1_SAMPLE_HAND_CATCH_TARGET_RACK                     ,        ENABLE,     60000,      1,      3       },//转运机械手 抓取指定位置样本架
    {CAN_RAIL_MUTIL_CMD_BOARD1_SAMPLE_HAND_PUT_TARGET_RACK                       ,        ENABLE,     60000,      1,      3       },//转运机械手 样本架放回指定位置样本架
    {CAN_RAIL_MUTIL_CMD_BOARD1_SAMPLE_HAND_CATCH_TRAY_PUT_TARGET_RACK            ,        ENABLE,     60000,      1,      3       },//转运机械手 抓取样本仓样本架放到指定位置
    {CAN_RAIL_MUTIL_CMD_BOARD1_SAMPLE_HAND_CATCH_TARGET_RACK_PUT_TRAY            ,        ENABLE,     60000,      1,      3       },//转运机械手 抓取指定位置样本架放到样本仓
    {CAN_RAIL_MUTIL_CMD_BOARD1_SAMPLE_HAND_CATCH_TARGET_PUT_TARGET_RACK          ,        ENABLE,     60000,      1,      2       },//转运机械手 抓取指定位置样本架放到制动位置，非样本仓
    {CAN_RAIL_MUTIL_CMD_BOARD1_SAMPLE_HAND_CATCH_ROTATE_PUT_ENTRY_CHANGE_SCAN    ,        ENABLE,     60000,    162,      0       },//转运机械手 抓取旋转扫码试管架放入输入变轨并扫码
    {CAN_RAIL_MUTIL_CMD_BOARD1_SAMPLE_HAND_RACK_PUT_ENTRY_CHANGE_SCAN            ,        ENABLE,     60000,    162,      0       },//转运机械手 手上试管架放入输入变轨并扫码
    {CAN_RAIL_MUTIL_CMD_BOARD1_SAMPLE_HAND_OPEN_OR_CLOSE_SCAN                    ,        ENABLE,     60000,      0,      1       },//转运机械手 扫码枪控制
};

/*************************************发送指令等待ACK************************************************/
//转运机械手 输入读取
void CanMasterBoard1SampleHandReadInputWhileAck(uint8_t index,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD1_SAMPLE_HAND;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD1_SAMPLE_HAND_READ_INPUT;
    uint8_t targetModule = CAN_MODULE_NO_BOARD1;
    uint8_t targetBoardID = CAN_BOARD_ID_BOARD1;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        CAN_PROT_LOG("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterBoard1SampleHandCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterBoard1SampleHandCmdConfigArray));
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

//转运机械手 输出写出
void CanMasterBoard1SampleHandWriteOutputWhileAck(uint8_t index,PIN_STATE outState,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD1_SAMPLE_HAND;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD1_SAMPLE_HAND_WRITE_OUTPUT;
    uint8_t targetModule = CAN_MODULE_NO_BOARD1;
    uint8_t targetBoardID = CAN_BOARD_ID_BOARD1;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        CAN_PROT_LOG("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterBoard1SampleHandCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterBoard1SampleHandCmdConfigArray));
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

//转运机械手 指定步进电机复位
void CanMasterBoard1SampleHandStepMotorResetWhileAck(uint8_t motorIndex,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD1_SAMPLE_HAND;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD1_SAMPLE_HAND_MOTOR_RESET;
    uint8_t targetModule = CAN_MODULE_NO_BOARD1;
    uint8_t targetBoardID = CAN_BOARD_ID_BOARD1;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        CAN_PROT_LOG("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterBoard1SampleHandCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterBoard1SampleHandCmdConfigArray));
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

//转运机械手 电机走步数
void CanMasterBoard1SampleHandStepMotorRunStepsWhileAck(uint8_t motorIndex,int32_t steps,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD1_SAMPLE_HAND;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD1_SAMPLE_HAND_MOTOR_RUN_STEP;
    uint8_t targetModule = CAN_MODULE_NO_BOARD1;
    uint8_t targetBoardID = CAN_BOARD_ID_BOARD1;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        CAN_PROT_LOG("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterBoard1SampleHandCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterBoard1SampleHandCmdConfigArray));
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

//转运机械手 整体复位
void CanMasterBoard1SampleHandResetAllWhileAck(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD1_SAMPLE_HAND;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD1_SAMPLE_HAND_RESET_ALL;
    uint8_t targetModule = CAN_MODULE_NO_BOARD1;
    uint8_t targetBoardID = CAN_BOARD_ID_BOARD1;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        CAN_PROT_LOG("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterBoard1SampleHandCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterBoard1SampleHandCmdConfigArray));
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

//转运机械手 样本转运机械手运行到指定位置
void CanMasterBoard1SampleHandMoveToTargetPosWhileAck(SAMPLE_HAND_POS mainPos,uint8_t trayPos,uint8_t rackPos,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD1_SAMPLE_HAND;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD1_SAMPLE_HAND_MOVE_TO_TARGET_POS;
    uint8_t targetModule = CAN_MODULE_NO_BOARD1;
    uint8_t targetBoardID = CAN_BOARD_ID_BOARD1;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        CAN_PROT_LOG("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterBoard1SampleHandCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterBoard1SampleHandCmdConfigArray));
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
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)mainPos;
    canCmdSendPtr->cmdBufferPtr[1] = (uint8_t)trayPos;
    canCmdSendPtr->cmdBufferPtr[2] = (uint8_t)rackPos;
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//转运机械手 架子扫描
void CanMasterBoard1SampleHandRackExistScanWhileAck(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD1_SAMPLE_HAND;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD1_SAMPLE_HAND_RACK_EXIST_SCAN;
    uint8_t targetModule = CAN_MODULE_NO_BOARD1;
    uint8_t targetBoardID = CAN_BOARD_ID_BOARD1;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        CAN_PROT_LOG("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterBoard1SampleHandCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterBoard1SampleHandCmdConfigArray));
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
    // canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)mainPos;
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//转运机械手 抓取指定位置样本架
void CanMasterBoard1SampleHandCatchTargetRackWhileAck(SAMPLE_HAND_POS mainPos,uint8_t trayPos,uint8_t rackPos,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD1_SAMPLE_HAND;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD1_SAMPLE_HAND_CATCH_TARGET_RACK;
    uint8_t targetModule = CAN_MODULE_NO_BOARD1;
    uint8_t targetBoardID = CAN_BOARD_ID_BOARD1;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        CAN_PROT_LOG("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterBoard1SampleHandCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterBoard1SampleHandCmdConfigArray));
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
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)mainPos;
    canCmdSendPtr->cmdBufferPtr[1] = (uint8_t)trayPos;
    canCmdSendPtr->cmdBufferPtr[2] = (uint8_t)rackPos;
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//转运机械手 架子放回指定位置
void CanMasterBoard1SampleHandPutTargetRackWhileAck(SAMPLE_HAND_POS mainPos,uint8_t trayPos,uint8_t rackPos,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD1_SAMPLE_HAND;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD1_SAMPLE_HAND_PUT_TARGET_RACK;
    uint8_t targetModule = CAN_MODULE_NO_BOARD1;
    uint8_t targetBoardID = CAN_BOARD_ID_BOARD1;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        CAN_PROT_LOG("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterBoard1SampleHandCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterBoard1SampleHandCmdConfigArray));
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
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)mainPos;
    canCmdSendPtr->cmdBufferPtr[1] = (uint8_t)trayPos;
    canCmdSendPtr->cmdBufferPtr[2] = (uint8_t)rackPos;
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}


//转运机械手 抓取样本仓样本架放到指定位置
void CanMasterBoard1SampleHandCatchTrayPutTargetRackWhileAck(uint8_t trayPos,uint8_t rackPos,SAMPLE_HAND_POS mainPutPos,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD1_SAMPLE_HAND;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD1_SAMPLE_HAND_CATCH_TRAY_PUT_TARGET_RACK;
    uint8_t targetModule = CAN_MODULE_NO_BOARD1;
    uint8_t targetBoardID = CAN_BOARD_ID_BOARD1;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        CAN_PROT_LOG("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterBoard1SampleHandCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterBoard1SampleHandCmdConfigArray));
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
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)trayPos;
    canCmdSendPtr->cmdBufferPtr[1] = (uint8_t)rackPos;
    canCmdSendPtr->cmdBufferPtr[2] = (uint8_t)mainPutPos;
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//转运机械手 抓取指定位置样本架放到样本仓
void CanMasterBoard1SampleHandCatchTargetRackPutTrayWhileAck(SAMPLE_HAND_POS mainCatchPos,uint8_t trayPos,uint8_t rackPos,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD1_SAMPLE_HAND;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD1_SAMPLE_HAND_CATCH_TARGET_RACK_PUT_TRAY;
    uint8_t targetModule = CAN_MODULE_NO_BOARD1;
    uint8_t targetBoardID = CAN_BOARD_ID_BOARD1;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        CAN_PROT_LOG("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterBoard1SampleHandCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterBoard1SampleHandCmdConfigArray));
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
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)mainCatchPos;
    canCmdSendPtr->cmdBufferPtr[1] = (uint8_t)trayPos;
    canCmdSendPtr->cmdBufferPtr[2] = (uint8_t)rackPos;
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//转运机械手 抓取指定位置放到指定位置
void CanMasterBoard1SampleHandCatchTargetPutTargetRackWhileAck(SAMPLE_HAND_POS mainCatchPos,SAMPLE_HAND_POS mainPutPos,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD1_SAMPLE_HAND;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD1_SAMPLE_HAND_CATCH_TARGET_PUT_TARGET_RACK;
    uint8_t targetModule = CAN_MODULE_NO_BOARD1;
    uint8_t targetBoardID = CAN_BOARD_ID_BOARD1;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        CAN_PROT_LOG("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterBoard1SampleHandCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterBoard1SampleHandCmdConfigArray));
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
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)mainCatchPos;
    canCmdSendPtr->cmdBufferPtr[1] = (uint8_t)mainPutPos;
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//转运机械手 抓取旋转扫码试管架放入输入变轨变轨并扫码
void CanMasterBoard1SampleHandCatchRotatePutEntryChangeAndScanWhileAck(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD1_SAMPLE_HAND;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD1_SAMPLE_HAND_CATCH_ROTATE_PUT_ENTRY_CHANGE_SCAN;
    uint8_t targetModule = CAN_MODULE_NO_BOARD1;
    uint8_t targetBoardID = CAN_BOARD_ID_BOARD1;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        CAN_PROT_LOG("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterBoard1SampleHandCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterBoard1SampleHandCmdConfigArray));
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
    // canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)mainCatchPos;
    // canCmdSendPtr->cmdBufferPtr[1] = (uint8_t)mainPutPos;
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//转运机械手 手上试管架放入输入变轨变轨并扫码
void CanMasterBoard1SampleHandRackPutEntryChangeAndScanWhileAck(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD1_SAMPLE_HAND;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD1_SAMPLE_HAND_RACK_PUT_ENTRY_CHANGE_SCAN;
    uint8_t targetModule = CAN_MODULE_NO_BOARD1;
    uint8_t targetBoardID = CAN_BOARD_ID_BOARD1;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        CAN_PROT_LOG("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterBoard1SampleHandCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterBoard1SampleHandCmdConfigArray));
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
    // canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)mainCatchPos;
    // canCmdSendPtr->cmdBufferPtr[1] = (uint8_t)mainPutPos;
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//转运机械手 扫码枪控制
void CanMasterBoard1SampleHandOpenOrCloseScanWhileAck(uint8_t action,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD1_SAMPLE_HAND;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD1_SAMPLE_HAND_OPEN_OR_CLOSE_SCAN;
    uint8_t targetModule = CAN_MODULE_NO_BOARD1;
    uint8_t targetBoardID = CAN_BOARD_ID_BOARD1;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        CAN_PROT_LOG("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterBoard1SampleHandCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterBoard1SampleHandCmdConfigArray));
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
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)action;
    // canCmdSendPtr->cmdBufferPtr[1] = (uint8_t)mainPutPos;
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}
/*************************************等待指定指令完成返回************************************************/
//转运机械手 输入读取
void CanMasterBoard1SampleHandWaitReadInputResult(SYS_RESULT_PACK* resultPackPtr,PIN_STATE* sensorStatePtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD1_SAMPLE_HAND;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD1_SAMPLE_HAND_READ_INPUT;
    uint8_t targetModule = CAN_MODULE_NO_BOARD1;
    uint8_t targetBoardID = CAN_BOARD_ID_BOARD1;
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

//转运机械手 输出写出
void CanMasterBoard1SampleHandWaitWriteOutputResult(SYS_RESULT_PACK* resultPackPtr,uint8_t* writeIndex)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD1_SAMPLE_HAND;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD1_SAMPLE_HAND_WRITE_OUTPUT;
    uint8_t targetModule = CAN_MODULE_NO_BOARD1;
    uint8_t targetBoardID = CAN_BOARD_ID_BOARD1;
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


//转运机械手 指定步进电机复位
void CanMasterBoard1SampleHandWaitStepMotorResetResult(SYS_RESULT_PACK* resultPackPtr,MOD_SM_CAN_RESULT *modSmCanResultPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD1_SAMPLE_HAND;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD1_SAMPLE_HAND_MOTOR_RESET;
    uint8_t targetModule = CAN_MODULE_NO_BOARD1;
    uint8_t targetBoardID = CAN_BOARD_ID_BOARD1;
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

//转运机械手 电机走步数
void CanMasterBoard1SampleHandWaitStepMotorRunStepsResult(SYS_RESULT_PACK* resultPackPtr,MOD_SM_CAN_RESULT *modSmCanResultPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD1_SAMPLE_HAND;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD1_SAMPLE_HAND_MOTOR_RUN_STEP;
    uint8_t targetModule = CAN_MODULE_NO_BOARD1;
    uint8_t targetBoardID = CAN_BOARD_ID_BOARD1;
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

//转运机械手 整体复位
void CanMasterBoard1SampleHandWaitResetAllResult(SYS_RESULT_PACK* resultPackPtr,SAMPLE_RACK_STATE* rackExistPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD1_SAMPLE_HAND;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD1_SAMPLE_HAND_RESET_ALL;
    uint8_t targetModule = CAN_MODULE_NO_BOARD1;
    uint8_t targetBoardID = CAN_BOARD_ID_BOARD1;
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

//转运机械手 样本转运机械手运行到指定位置
void CanMasterBoard1SampleHandWaitMoveToTargetPosResult(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD1_SAMPLE_HAND;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD1_SAMPLE_HAND_MOVE_TO_TARGET_POS;
    uint8_t targetModule = CAN_MODULE_NO_BOARD1;
    uint8_t targetBoardID = CAN_BOARD_ID_BOARD1;
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
    //     *rackExistPtr = SAMPLE_RACK_STATE_NONE;
    //     if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
    //     {
    //         *rackExistPtr = (SAMPLE_RACK_STATE)(returnDataBufferPtr[0]);
    //     }
    // }
    //内存释放
    CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//转运机械手 架子扫描
void CanMasterBoard1SampleHandWaitRackExistScanResult(SYS_RESULT_PACK* resultPackPtr,uint8_t *scanLengthPtr,uint8_t **trayRackExistPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD1_SAMPLE_HAND;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD1_SAMPLE_HAND_RACK_EXIST_SCAN;
    uint8_t targetModule = CAN_MODULE_NO_BOARD1;
    uint8_t targetBoardID = CAN_BOARD_ID_BOARD1;
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
    if(scanLengthPtr != NULL)
    {
        //结果解析
        *scanLengthPtr = returnDataBufferPtr[0];
        if((*scanLengthPtr + 1) > returnDataBufferLength)
        {
            //数据结果长度异常
            *trayRackExistPtr = NULL;
        }
        else
        {
            *trayRackExistPtr = UserMemMallocWhileSuccess(MEM_REGION_SYS_MSG_CAN,*scanLengthPtr);
            UserMemCopy(*trayRackExistPtr,returnDataBufferPtr+1,*scanLengthPtr);
        }
        // *rackExistPtr = SAMPLE_RACK_STATE_NONE;
        // if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        // {
        //     *rackExistPtr = (SAMPLE_RACK_STATE)(returnDataBufferPtr[0]);
        // }
    }
    //内存释放
    CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//转运机械手 抓取指定位置样本架
void CanMasterBoard1SampleHandWaitCatchTargetRackResult(SYS_RESULT_PACK* resultPackPtr,SAMPLE_RACK_STATE* rackExistPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD1_SAMPLE_HAND;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD1_SAMPLE_HAND_CATCH_TARGET_RACK;
    uint8_t targetModule = CAN_MODULE_NO_BOARD1;
    uint8_t targetBoardID = CAN_BOARD_ID_BOARD1;
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

//转运机械手 架子放回指定位置
void CanMasterBoard1SampleHandWaitPutTargetRackResult(SYS_RESULT_PACK* resultPackPtr,SAMPLE_RACK_STATE* rackExistPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD1_SAMPLE_HAND;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD1_SAMPLE_HAND_PUT_TARGET_RACK;
    uint8_t targetModule = CAN_MODULE_NO_BOARD1;
    uint8_t targetBoardID = CAN_BOARD_ID_BOARD1;
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

//转运机械手 抓取样本仓样本架放到指定位置
void CanMasterBoard1SampleHandWaitCatchTrayPutTargetRackResult(SYS_RESULT_PACK* resultPackPtr,SAMPLE_RACK_STATE* rackExistPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD1_SAMPLE_HAND;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD1_SAMPLE_HAND_CATCH_TRAY_PUT_TARGET_RACK;
    uint8_t targetModule = CAN_MODULE_NO_BOARD1;
    uint8_t targetBoardID = CAN_BOARD_ID_BOARD1;
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

//转运机械手 抓取指定位置样本架放到样本仓
void CanMasterBoard1SampleHandWaitCatchTargetRackPutTrayResult(SYS_RESULT_PACK* resultPackPtr,SAMPLE_RACK_STATE* rackExistPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD1_SAMPLE_HAND;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD1_SAMPLE_HAND_CATCH_TARGET_RACK_PUT_TRAY;
    uint8_t targetModule = CAN_MODULE_NO_BOARD1;
    uint8_t targetBoardID = CAN_BOARD_ID_BOARD1;
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

//转运机械手 抓取指定位置放到指定位置 非样本仓
void CanMasterBoard1SampleHandWaitCatchTargetPutTargetRackResult(SYS_RESULT_PACK* resultPackPtr,SAMPLE_RACK_STATE* rackExistPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD1_SAMPLE_HAND;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD1_SAMPLE_HAND_CATCH_TARGET_PUT_TARGET_RACK;
    uint8_t targetModule = CAN_MODULE_NO_BOARD1;
    uint8_t targetBoardID = CAN_BOARD_ID_BOARD1;
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

//转运机械手 抓取旋转扫码试管架放入输入变轨变轨并扫码
void CanMasterBoard1SampleHandWaitCatchRotatePutEntryChangeAndScanResult(SYS_RESULT_PACK* resultPackPtr,SAMPLE_RACK_STATE* rackExistPtr,uint8_t *codeExistPtr,uint8_t **codeBuffPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD1_SAMPLE_HAND;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD1_SAMPLE_HAND_CATCH_ROTATE_PUT_ENTRY_CHANGE_SCAN;
    uint8_t targetModule = CAN_MODULE_NO_BOARD1;
    uint8_t targetBoardID = CAN_BOARD_ID_BOARD1;
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
    if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
    {
        if(rackExistPtr != NULL)
        {
            *rackExistPtr = (SAMPLE_RACK_STATE)(returnDataBufferPtr[0]);
        }
        if(codeExistPtr != NULL)
        {
            *codeExistPtr = returnDataBufferPtr[1];
        }
        if(codeBuffPtr != NULL)
        {
            *codeBuffPtr = UserMemMallocWhileSuccess(MEM_REGION_SYS_MSG_CAN,PER_BAR_CODE_MAX_LENGTH*BARSCAN_INDEX_MAX);
            UserMemCopy(*codeBuffPtr,returnDataBufferPtr+2,PER_BAR_CODE_MAX_LENGTH*BARSCAN_INDEX_MAX);
        }
    }
    //内存释放
    CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//转运机械手 手上试管架放入输入变轨变轨并扫码
void CanMasterBoard1SampleHandWaitRackPutEntryChangeAndScanResult(SYS_RESULT_PACK* resultPackPtr,SAMPLE_RACK_STATE* rackExistPtr,uint8_t *codeExistPtr,uint8_t **codeBuffPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD1_SAMPLE_HAND;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD1_SAMPLE_HAND_RACK_PUT_ENTRY_CHANGE_SCAN;
    uint8_t targetModule = CAN_MODULE_NO_BOARD1;
    uint8_t targetBoardID = CAN_BOARD_ID_BOARD1;
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
    if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
    {
        if(rackExistPtr != NULL)
        {
            *rackExistPtr = (SAMPLE_RACK_STATE)(returnDataBufferPtr[0]);
        }
        if(codeExistPtr != NULL)
        {
            *codeExistPtr = returnDataBufferPtr[1];
        }
        if(codeBuffPtr != NULL)
        {
            *codeBuffPtr = UserMemMallocWhileSuccess(MEM_REGION_MOD_TASK,PER_BAR_CODE_MAX_LENGTH*BARSCAN_INDEX_MAX);
            UserMemCopy(*codeBuffPtr,returnDataBufferPtr+2,PER_BAR_CODE_MAX_LENGTH*BARSCAN_INDEX_MAX);
        }
    }
    //内存释放
    CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//转运机械手 扫码枪控制
void CanMasterBoard1SampleHandWaitOpenOrCloseScanResult(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD1_SAMPLE_HAND;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD1_SAMPLE_HAND_OPEN_OR_CLOSE_SCAN;
    uint8_t targetModule = CAN_MODULE_NO_BOARD1;
    uint8_t targetBoardID = CAN_BOARD_ID_BOARD1;
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
    // if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
    // {
    // }
    //内存释放
    CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}
/*************************************发送指令等待完成返回************************************************/
//转运机械手 输入读取
void CanMasterBoard1SampleHandReadInputWhileReturn(uint8_t index,SYS_RESULT_PACK* resultPackPtr,PIN_STATE* sensorStatePtr)
{
    //发送指令等待ACK
    CanMasterBoard1SampleHandReadInputWhileAck(index,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard1SampleHandReadInputWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterBoard1SampleHandWaitReadInputResult(resultPackPtr,sensorStatePtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard1SampleHandWaitReadInputResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//转运机械手 输出写出
void CanMasterBoard1SampleHandWriteOutputWhileReturn(uint8_t index,PIN_STATE outState,SYS_RESULT_PACK* resultPackPtr,uint8_t* writeIndex)
{
    //发送指令等待ACK
    CanMasterBoard1SampleHandWriteOutputWhileAck(index,outState,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard1SampleHandWriteOutputWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterBoard1SampleHandWaitWriteOutputResult(resultPackPtr,writeIndex);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard1SampleHandWaitWriteOutputResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//转运机械手 指定步进电机复位
void CanMasterBoard1SampleHandStepMotorResetWhileReturn(uint8_t motorIndex,SYS_RESULT_PACK* resultPackPtr,MOD_SM_CAN_RESULT *modSmCanResultPtr)
{
    //发送指令等待ACK
    CanMasterBoard1SampleHandStepMotorResetWhileAck(motorIndex,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard1SampleHandStepMotorResetWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterBoard1SampleHandWaitStepMotorResetResult(resultPackPtr,modSmCanResultPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard1SampleHandWaitStepMotorResetResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//转运机械手 电机走步数
void CanMasterBoard1SampleHandStepMotorRunStepsWhileReturn(uint8_t motorIndex,int32_t steps,SYS_RESULT_PACK* resultPackPtr,MOD_SM_CAN_RESULT *modSmCanResultPtr)
{
    //发送指令等待ACK
    CanMasterBoard1SampleHandStepMotorRunStepsWhileAck(motorIndex,steps,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard1SampleHandStepMotorRunStepsWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterBoard1SampleHandWaitStepMotorRunStepsResult(resultPackPtr,modSmCanResultPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard1SampleHandWaitStepMotorResetResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//转运机械手 整体复位
void CanMasterBoard1SampleHandResetAllWhileReturn(SYS_RESULT_PACK* resultPackPtr,SAMPLE_RACK_STATE* rackExistPtr)
{
    //发送指令等待ACK
    CanMasterBoard1SampleHandResetAllWhileAck(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard1SampleHandResetAllWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterBoard1SampleHandWaitResetAllResult(resultPackPtr,rackExistPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard1SampleHandWaitResetAllResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//转运机械手 样本转运机械手运行到指定位置
void CanMasterBoard1SampleHandMoveToTargetPosWhileReturn(SAMPLE_HAND_POS mainPos,uint8_t trayPos,uint8_t rackPos,SYS_RESULT_PACK* resultPackPtr)
{
    //发送指令等待ACK
    CanMasterBoard1SampleHandMoveToTargetPosWhileAck(mainPos,trayPos,rackPos,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard1SampleHandMoveToTargetPosWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterBoard1SampleHandWaitMoveToTargetPosResult(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard1SampleHandWaitMoveToTargetPosResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//转运机械手 架子扫描
void CanMasterBoard1SampleHandRackExistScanWhileReturn(SYS_RESULT_PACK* resultPackPtr,uint8_t *scanLengthPtr,uint8_t **trayRackExistPtr)
{
    //发送指令等待ACK
    CanMasterBoard1SampleHandRackExistScanWhileAck(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard1SampleHandRackExistScanWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterBoard1SampleHandWaitRackExistScanResult(resultPackPtr,scanLengthPtr,trayRackExistPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard1SampleHandWaitRackExistScanResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}


//转运机械手 抓取指定位置样本架
void CanMasterBoard1SampleHandCatchTargetRackWhileReturn(SAMPLE_HAND_POS mainPos,uint8_t trayPos,uint8_t rackPos,SYS_RESULT_PACK* resultPackPtr,SAMPLE_RACK_STATE* rackExistPtr)
{
    //发送指令等待ACK
    CanMasterBoard1SampleHandCatchTargetRackWhileAck(mainPos,trayPos,rackPos,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard1SampleHandCatchTargetRackWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterBoard1SampleHandWaitCatchTargetRackResult(resultPackPtr,rackExistPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard1SampleHandWaitRackExistScanResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//转运机械手 架子放回指定位置
void CanMasterBoard1SampleHandPutTargetRackWhileReturn(SAMPLE_HAND_POS mainPos,uint8_t trayPos,uint8_t rackPos,SYS_RESULT_PACK* resultPackPtr,SAMPLE_RACK_STATE* rackExistPtr)
{
    //发送指令等待ACK
    CanMasterBoard1SampleHandPutTargetRackWhileAck(mainPos,trayPos,rackPos,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard1SampleHandPutTargetRackWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterBoard1SampleHandWaitPutTargetRackResult(resultPackPtr,rackExistPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard1SampleHandWaitPutTargetRackResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}


//转运机械手 抓取样本仓样本架放到指定位置
void CanMasterBoard1SampleHandCatchTrayPutTargetRackWhileReturn(uint8_t trayPos,uint8_t rackPos,SAMPLE_HAND_POS mainPutPos,SYS_RESULT_PACK* resultPackPtr,SAMPLE_RACK_STATE* rackExistPtr)
{
    //发送指令等待ACK
    CanMasterBoard1SampleHandCatchTrayPutTargetRackWhileAck(trayPos,rackPos,mainPutPos,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard1SampleHandCatchTrayPutTargetRackWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterBoard1SampleHandWaitCatchTrayPutTargetRackResult(resultPackPtr,rackExistPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard1SampleHandWaitCatchTrayPutTargetRackResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//转运机械手 抓取指定位置样本架放到样本仓
void CanMasterBoard1SampleHandCatchTargetRackPutTrayWhileReturn(SAMPLE_HAND_POS mainCatchPos,uint8_t trayPos,uint8_t rackPos,SYS_RESULT_PACK* resultPackPtr,SAMPLE_RACK_STATE* rackExistPtr)
{
    //发送指令等待ACK
    CanMasterBoard1SampleHandCatchTargetRackPutTrayWhileAck(mainCatchPos,trayPos,rackPos,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard1SampleHandCatchTargetRackPutTrayWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterBoard1SampleHandWaitCatchTargetRackPutTrayResult(resultPackPtr,rackExistPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard1SampleHandWaitCatchTargetRackPutTrayResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}


//转运机械手 抓取指定位置放到指定位置 非样本仓
void CanMasterBoard1SampleHandCatchTargetPutTargetRackWhileReturn(SAMPLE_HAND_POS mainCatchPos,SAMPLE_HAND_POS mainPutPos,SYS_RESULT_PACK* resultPackPtr,SAMPLE_RACK_STATE* rackExistPtr)
{
    //发送指令等待ACK
    CanMasterBoard1SampleHandCatchTargetPutTargetRackWhileAck(mainCatchPos,mainPutPos,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard1SampleHandCatchTargetPutTargetRackWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterBoard1SampleHandWaitCatchTargetPutTargetRackResult(resultPackPtr,rackExistPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard1SampleHandWaitCatchTargetPutTargetRackResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//转运机械手  抓取旋转扫码试管架放入输入变轨变轨并扫码
void CanMasterBoard1SampleHandCatchRotatePutEntryChangeAndScanWhileReturn(SYS_RESULT_PACK* resultPackPtr,SAMPLE_RACK_STATE* rackExistPtr,uint8_t *codeExistPtr,uint8_t **codeBuffPtr)
{
    //发送指令等待ACK
    CanMasterBoard1SampleHandCatchRotatePutEntryChangeAndScanWhileAck(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard1SampleHandCatchRotatePutEntryChangeAndScanWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterBoard1SampleHandWaitCatchRotatePutEntryChangeAndScanResult(resultPackPtr,rackExistPtr,codeExistPtr,codeBuffPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard1SampleHandWaitCatchRotatePutEntryChangeAndScanResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//转运机械手 手上试管架放入输入变轨变轨并扫码
void CanMasterBoard1SampleHandRackPutEntryChangeAndScanWhileReturn(SYS_RESULT_PACK* resultPackPtr,SAMPLE_RACK_STATE* rackExistPtr,uint8_t *codeExistPtr,uint8_t **codeBuffPtr)
{
    //发送指令等待ACK
    CanMasterBoard1SampleHandRackPutEntryChangeAndScanWhileAck(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard1SampleHandRackPutEntryChangeAndScanWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterBoard1SampleHandWaitRackPutEntryChangeAndScanResult(resultPackPtr,rackExistPtr,codeExistPtr,codeBuffPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard1SampleHandWaitRackPutEntryChangeAndScanResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//转运机械手 扫码枪控制
void CanMasterBoard1SampleHandOpenOrCloseScanWhileReturn(uint8_t action,SYS_RESULT_PACK* resultPackPtr)
{
    //发送指令等待ACK
    CanMasterBoard1SampleHandOpenOrCloseScanWhileAck(action,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard1SampleHandOpenOrCloseScanWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterBoard1SampleHandWaitOpenOrCloseScanResult(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard1SampleHandWaitOpenOrCloseScanResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}








