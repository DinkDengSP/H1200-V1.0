/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-09-04 11:57:26 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-09-21 13:12:26 +0800
************************************************************************************************/ 
#include "C21NetOumengInterBase.h"


//C21向欧蒙接口申请试管架
ERROR_SUB C21NetSendCmdC21ApplyRankIn(uint32_t rankCode)
{
    uint8_t sendBufferLocal[4];
    UserMemSet(sendBufferLocal,0,4);
    //写入rankcode
    ProtocolC21NetUtilWriteUint32ToBuffer(rankCode,sendBufferLocal);
    //发送指令
    return ProtocolC21NetSocketSendCmdDetail(MACRO_C21_NET_CMD_C21_APPLY_RANK_IN,sendBufferLocal,4);
}

//欧蒙接口响应C21的申请试管
ERROR_SUB C21NetRespCmdC21ApplyRankIn(uint32_t errorCode,uint32_t rankCode)
{
    uint8_t sendBufferLocal[8];
    UserMemSet(sendBufferLocal,0,8);
    //写入错误代码
    ProtocolC21NetUtilWriteUint32ToBuffer(errorCode,sendBufferLocal);
    //写入架子编码
    ProtocolC21NetUtilWriteUint32ToBuffer(rankCode,sendBufferLocal+4);
    //发送指令
    return ProtocolC21NetSocketSendCmdDetail(MACRO_C21_NET_CMD_C21_APPLY_RANK_IN,sendBufferLocal,8);
}

//欧蒙接口通知C21进架结果
ERROR_SUB C21NetZetaNotifyC21RankInResult(uint32_t errorCode,uint32_t rankCode,C21_NET_RANK_MSG* rankMsgPtr)
{
    uint8_t* sendBufferLocalPtr = ProtocolC21NetSocketMemMallocWhileSuccess(328);
    UserMemSet(sendBufferLocalPtr,0,328);
    //错误码
    ProtocolC21NetUtilWriteUint32ToBuffer(errorCode,sendBufferLocalPtr);
    //写入rankcode
    ProtocolC21NetUtilWriteUint32ToBuffer(rankCode,sendBufferLocalPtr+4);
    //拷贝条码
    if((rankMsgPtr->tube1BarCodeBufPtr != NULL)&&(rankMsgPtr->tube1BarCodeLength != 0))
    {
        if(rankMsgPtr->tube1BarCodeLength > 31)rankMsgPtr->tube1BarCodeLength = 31;
        UserMemCopy(sendBufferLocalPtr+8,rankMsgPtr->tube1BarCodeBufPtr,rankMsgPtr->tube1BarCodeLength);
    }
    if((rankMsgPtr->tube2BarCodeBufPtr != NULL)&&(rankMsgPtr->tube2BarCodeLength != 0))
    {
        if(rankMsgPtr->tube2BarCodeLength > 31)rankMsgPtr->tube2BarCodeLength = 31;
        UserMemCopy(sendBufferLocalPtr+8+32,rankMsgPtr->tube2BarCodeBufPtr,rankMsgPtr->tube2BarCodeLength);
    }
    if((rankMsgPtr->tube3BarCodeBufPtr != NULL)&&(rankMsgPtr->tube3BarCodeLength != 0))
    {
        if(rankMsgPtr->tube3BarCodeLength > 31)rankMsgPtr->tube3BarCodeLength = 31;
        UserMemCopy(sendBufferLocalPtr+8+(32*2),rankMsgPtr->tube3BarCodeBufPtr,rankMsgPtr->tube3BarCodeLength);
    }
    if((rankMsgPtr->tube4BarCodeBufPtr != NULL)&&(rankMsgPtr->tube4BarCodeLength != 0))
    {
        if(rankMsgPtr->tube4BarCodeLength > 31)rankMsgPtr->tube4BarCodeLength = 31;
        UserMemCopy(sendBufferLocalPtr+8+(32*3),rankMsgPtr->tube4BarCodeBufPtr,rankMsgPtr->tube4BarCodeLength);
    }
    if((rankMsgPtr->tube5BarCodeBufPtr != NULL)&&(rankMsgPtr->tube5BarCodeLength != 0))
    {
        if(rankMsgPtr->tube5BarCodeLength > 31)rankMsgPtr->tube5BarCodeLength = 31;
        UserMemCopy(sendBufferLocalPtr+8+(32*4),rankMsgPtr->tube5BarCodeBufPtr,rankMsgPtr->tube5BarCodeLength);
    }
    if((rankMsgPtr->tube6BarCodeBufPtr != NULL)&&(rankMsgPtr->tube6BarCodeLength != 0))
    {
        if(rankMsgPtr->tube6BarCodeLength > 31)rankMsgPtr->tube6BarCodeLength = 31;
        UserMemCopy(sendBufferLocalPtr+8+(32*5),rankMsgPtr->tube6BarCodeBufPtr,rankMsgPtr->tube6BarCodeLength);
    }
    if((rankMsgPtr->tube7BarCodeBufPtr != NULL)&&(rankMsgPtr->tube7BarCodeLength != 0))
    {
        if(rankMsgPtr->tube7BarCodeLength > 31)rankMsgPtr->tube7BarCodeLength = 31;
        UserMemCopy(sendBufferLocalPtr+8+(32*6),rankMsgPtr->tube7BarCodeBufPtr,rankMsgPtr->tube7BarCodeLength);
    }
    if((rankMsgPtr->tube8BarCodeBufPtr != NULL)&&(rankMsgPtr->tube8BarCodeLength != 0))
    {
        if(rankMsgPtr->tube8BarCodeLength > 31)rankMsgPtr->tube8BarCodeLength = 31;
        UserMemCopy(sendBufferLocalPtr+8+(32*7),rankMsgPtr->tube8BarCodeBufPtr,rankMsgPtr->tube8BarCodeLength);
    }
    if((rankMsgPtr->tube9BarCodeBufPtr != NULL)&&(rankMsgPtr->tube9BarCodeLength != 0))
    {
        if(rankMsgPtr->tube9BarCodeLength > 31)rankMsgPtr->tube9BarCodeLength = 31;
        UserMemCopy(sendBufferLocalPtr+8+(32*8),rankMsgPtr->tube9BarCodeBufPtr,rankMsgPtr->tube9BarCodeLength);
    }
    if((rankMsgPtr->tube10BarCodeBufPtr != NULL)&&(rankMsgPtr->tube10BarCodeLength != 0))
    {
        if(rankMsgPtr->tube10BarCodeLength > 31)rankMsgPtr->tube10BarCodeLength = 31;
        UserMemCopy(sendBufferLocalPtr+8+(32*9),rankMsgPtr->tube10BarCodeBufPtr,rankMsgPtr->tube10BarCodeLength);
    }
    //发送指令
    return ProtocolC21NetSocketSendCmdDetail(MACRO_C21_NET_CMD_ZETA_REPORT_RANK_IN_RESULT,sendBufferLocalPtr,328);
}

//C21回复欧蒙接口进入的架子的取架结果
ERROR_SUB C21NetRespC21RankTakeAwayResultToZeta(uint32_t errorCode)
{
    uint8_t sendBufferLocal[4];
    UserMemSet(sendBufferLocal,0,4);
    //写入rankcode
    ProtocolC21NetUtilWriteUint32ToBuffer(errorCode,sendBufferLocal);
    //发送指令
    return ProtocolC21NetSocketSendCmdDetail(MACRO_C21_NET_CMD_ZETA_REPORT_RANK_IN_RESULT,sendBufferLocal,4);
}

//C21告知出架子
ERROR_SUB C21NetNotifyZetaRackOut(uint32_t rankCode,C21_NET_RANK_OUT_TYPE rankType,C21_NET_TUBE_STATE_SET* tubeStateSetPtr)
{
    uint8_t* sendBufferLocalPtr = ProtocolC21NetSocketMemMallocWhileSuccess(15);
    UserMemSet(sendBufferLocalPtr,0,15);
    //写入rankcode
    ProtocolC21NetUtilWriteUint32ToBuffer(rankCode,sendBufferLocalPtr);
    //状态
    sendBufferLocalPtr[4] = (uint8_t)rankType;
    //错误码
    UserMemCopy(sendBufferLocalPtr+5,(uint8_t*)tubeStateSetPtr,10);
    //发送指令
    return ProtocolC21NetSocketSendCmdDetail(MACRO_C21_NET_CMD_C21_NOTOFY_RANK_OUT,sendBufferLocalPtr,15);
}

//欧蒙接口告知是否可以出架子
ERROR_SUB C21NetRespZetaRackOut(uint32_t errorCode,uint32_t rankCode)
{
    uint8_t sendBufferLocal[9];
    UserMemSet(sendBufferLocal,0,9);
    //错误码
    ProtocolC21NetUtilWriteUint32ToBuffer(errorCode,sendBufferLocal);
    //写入rankcode
    ProtocolC21NetUtilWriteUint32ToBuffer(rankCode,sendBufferLocal+4);
    //发送指令
    return ProtocolC21NetSocketSendCmdDetail(MACRO_C21_NET_CMD_C21_NOTOFY_RANK_OUT,sendBufferLocal,8);
}

//欧蒙接口告知出架结果
ERROR_SUB C21NetReportZetaRackOutResult(uint32_t errorCode,uint32_t rankCode)
{
    uint8_t sendBufferLocal[9];
    UserMemSet(sendBufferLocal,0,9);
    //错误码
    ProtocolC21NetUtilWriteUint32ToBuffer(errorCode,sendBufferLocal);
    //写入rankcode
    ProtocolC21NetUtilWriteUint32ToBuffer(rankCode,sendBufferLocal+4);
    //发送指令
    return ProtocolC21NetSocketSendCmdDetail(MACRO_C21_NET_CMD_ZETA_REPORT_RANK_OUT_PROC_RESULT,sendBufferLocal,8);
}

//C21设备上报设备状态
ERROR_SUB C21NetReportC21DeviceState(C21_NET_DEVICE_STATE deviceState)
{
    //指令发送
    return ProtocolC21NetSocketSendCmdDetail(MACRO_C21_NET_CMD_C21_REPORT_DEVICE_STATE_CHANGE,(uint8_t*)(&deviceState),1);
}

//欧蒙接口上报设备状态
ERROR_SUB C21NetReportZetaDeviceState(C21_NET_DEVICE_STATE deviceState)
{
    //指令发送
    return ProtocolC21NetSocketSendCmdDetail(MACRO_C21_NET_CMD_ZETA_REPORT_DEVICE_STATE_CHANGE,(uint8_t*)(&deviceState),1);
}

//欧蒙接口上报心跳
ERROR_SUB C21NetReportZetaHeart(void)
{
    //指令发送
    return ProtocolC21NetSocketSendCmdDetail(MACRO_C21_NET_CMD_ZETA_REPORT_HEART,NULL,0);
}

//C21控制欧蒙接口启动校准
ERROR_SUB C21NetSendCmdZetaStartPosAdjust(void)
{
    //指令发送
    return ProtocolC21NetSocketSendCmdDetail(MACRO_C21_NET_CMD_ZETA_POS_ADJUST_START,NULL,0);
}

//欧蒙接口反馈启动校准执行结果
ERROR_SUB C21NetRespCmdZetaStartPosAdjustResult(uint32_t errorCode)
{
    uint8_t sendBufferLocal[4];
    UserMemSet(sendBufferLocal,0,4);
    //写入rankcode
    ProtocolC21NetUtilWriteUint32ToBuffer(errorCode,sendBufferLocal);
    //发送指令
    return ProtocolC21NetSocketSendCmdDetail(MACRO_C21_NET_CMD_ZETA_POS_ADJUST_START,sendBufferLocal,4);
}

//C21控制欧蒙接口执行到指定校准位置
ERROR_SUB C21NetSendCmdZetaPosAdjustNext(int32_t targetCoordinate)
{
    uint8_t sendBufferLocal[4];
    UserMemSet(sendBufferLocal,0,4);
    //写入targetCoordinate
    ProtocolC21NetUtilWriteInt32ToBuffer(targetCoordinate,sendBufferLocal);
    //发送指令
    return ProtocolC21NetSocketSendCmdDetail(MACRO_C21_NET_CMD_ZETA_POS_ADJUST_NEXT,sendBufferLocal,4);
}

//欧蒙接口反馈校准位置执行结果
ERROR_SUB C21NetRespCmdZetaPosAdjustNext(uint32_t errorCode)
{
    uint8_t sendBufferLocal[4];
    UserMemSet(sendBufferLocal,0,4);
    //写入rankcode
    ProtocolC21NetUtilWriteUint32ToBuffer(errorCode,sendBufferLocal);
    //发送指令
    return ProtocolC21NetSocketSendCmdDetail(MACRO_C21_NET_CMD_ZETA_POS_ADJUST_NEXT,sendBufferLocal,4);
}

//C21控制欧蒙接口完成校准并写入参数
ERROR_SUB C21NetSendCmdZetaPosAdjustOverWithParam(uint8_t pos,int32_t adjustPosOver)
{
    uint8_t sendBufferLocal[5];
    UserMemSet(sendBufferLocal,0,5);
    sendBufferLocal[0] = pos;
    //写入targetCoordinate
    ProtocolC21NetUtilWriteInt32ToBuffer(adjustPosOver,sendBufferLocal+1);
    //发送指令
    return ProtocolC21NetSocketSendCmdDetail(MACRO_C21_NET_CMD_ZETA_POS_ADJUST_OVER,sendBufferLocal,5);
}

//欧蒙接口反馈校准完成指令的执行结果
ERROR_SUB C21NetRespCmdZetaPosAdjustOverWithParam(uint32_t errorCode)
{
    uint8_t sendBufferLocal[4];
    UserMemSet(sendBufferLocal,0,4);
    //写入rankcode
    ProtocolC21NetUtilWriteUint32ToBuffer(errorCode,sendBufferLocal);
    //发送指令
    return ProtocolC21NetSocketSendCmdDetail(MACRO_C21_NET_CMD_ZETA_POS_ADJUST_OVER,sendBufferLocal,4);
}

//C21控制欧蒙接口解除故障
ERROR_SUB C21NetSendCmdZetaTooubleShooting(uint8_t troubleLevel,uint32_t errorCode)
{
    uint8_t sendBufferLocal[5];
    UserMemSet(sendBufferLocal,0,5);
    sendBufferLocal[0] = troubleLevel;
    //写入targetCoordinate
    ProtocolC21NetUtilWriteInt32ToBuffer(errorCode,sendBufferLocal+1);
    //发送指令
    return ProtocolC21NetSocketSendCmdDetail(MACRO_C21_NET_CMD_ZETA_TROUBLE_SHOOTING,sendBufferLocal,5);
}

//欧蒙接口反馈解除故障执行结果
ERROR_SUB C21NetRespCmdZetaTooubleShooting(uint32_t errorCode)
{
    uint8_t sendBufferLocal[4];
    UserMemSet(sendBufferLocal,0,4);
    //写入rankcode
    ProtocolC21NetUtilWriteUint32ToBuffer(errorCode,sendBufferLocal);
    //发送指令
    return ProtocolC21NetSocketSendCmdDetail(MACRO_C21_NET_CMD_ZETA_TROUBLE_SHOOTING,sendBufferLocal,4);
}

//欧蒙接口主动故障上报
ERROR_SUB C21NetOumengInterReportErrorToC21(uint8_t troubleLevel,uint32_t errorCode)
{
    uint8_t sendBufferLocal[5];
    UserMemSet(sendBufferLocal,0,5);
    sendBufferLocal[0] = troubleLevel;
    //写入targetCoordinate
    ProtocolC21NetUtilWriteInt32ToBuffer(errorCode,sendBufferLocal+1);
    //发送指令
    return ProtocolC21NetSocketSendCmdDetail(MACRO_C21_NET_CMD_ZETA_REPORT_ERROR,sendBufferLocal,5);
}

//通用上报
ERROR_SUB C21NetOumengInterReport(uint16_t cmdCode,uint8_t* dataBody,uint16_t dataBodyLength)
{
    return ProtocolC21NetSocketSendCmdDetail(cmdCode, dataBody, dataBodyLength);
}
