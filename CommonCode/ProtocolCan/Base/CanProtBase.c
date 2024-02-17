/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-20 18:20:58 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-02 17:58:12 +0800
************************************************************************************************/ 
#include "CanProtBase.h"

//CAN1数据缓冲区
extern uint8_t can1ProtFrameBuf[CAN1_PROT_DATA_BUF_LEN];
//CAN2数据缓冲区
extern uint8_t can2ProtFrameBuf[CAN2_PROT_DATA_BUF_LEN];
//CAN1初始化标记
extern CAN_PROT_INIT_SET can1ProtInitSet;
//CAN2初始化标记
extern CAN_PROT_INIT_SET can2ProtInitSet;

//获取数据访问权
void Can1ProtGetDataAccessLock(void)
{
    tx_mutex_get(&(can1ProtInitSet.mutexSocketCanDataAccess),TX_WAIT_FOREVER);
}
//释放数据访问权
void Can1ProtGetDataAccessRelease(void)
{
    tx_mutex_put(&(can1ProtInitSet.mutexSocketCanDataAccess));
}

//获取数据访问权
void Can2ProtGetDataAccessLock(void)
{
    tx_mutex_get(&(can2ProtInitSet.mutexSocketCanDataAccess),TX_WAIT_FOREVER);
}
//释放数据访问权
void Can2ProtGetDataAccessRelease(void)
{
    tx_mutex_put(&(can2ProtInitSet.mutexSocketCanDataAccess));
}

//通过ID组合计算最终CAN ID
uint32_t CanProtBaseCalcID(CAN_PROT_ID* idPtr)
{
    uint32_t idResult = 0;
    //源模块号
    idPtr->srcModuleNo &= 0X07;
    idResult += idPtr->srcModuleNo;
    //目标模块号
    idResult <<= 3;
    idPtr->targetModuleNo &= 0X07;
    idResult += idPtr->targetModuleNo;
    //源板号
    idResult <<= 5;
    idPtr->srcBoardID &= 0x1f;
    idResult += idPtr->srcBoardID;
    //目标板号
    idResult <<= 5;
    idPtr->targetBoardID &= 0x1f;
    idResult += idPtr->targetBoardID;
    //信道号码
    idResult <<= 6;
    idPtr->singalChannel &= 0x3f;
    idResult += idPtr->singalChannel;
    //组包标记
    idResult <<= 2;
    idPtr->frameType &= 0x03;
    idResult += idPtr->frameType;
    //包类型
    idResult <<= 3;
    idPtr->packType &= 0x07;
    idResult += idPtr->packType;
    //重发标记
    idResult <<= 2;
    idPtr->retrySendCount &= 0x03;
    idResult += idPtr->retrySendCount;
    //结果返回
    return idResult;
}

//将接收的ID转换为协议ID
void CanProtBasePackIdAnaly(uint32_t canID,CAN_PROT_ID* idPtr)
{
    //重发计数
    idPtr->retrySendCount = (canID&0X03);
    //包类型
    canID >>= 2;
    idPtr->packType = (PROT_PACK_TYPE)(canID&0X07);
    //组包标记
    canID >>= 3;
    idPtr->frameType = (CAN_PROT_FRAME_TYPE)(canID&0X03);
    //信道编号
    canID >>= 2;
    idPtr->singalChannel = (canID&0X3F);
    //目标板号
    canID >>= 6;
    idPtr->targetBoardID = (canID&0X1F);
    //源板号
    canID >>= 5;
    idPtr->srcBoardID = (canID&0X1F);
    //目标模块号
    canID >>= 5;
    idPtr->targetModuleNo = (canID&0X07);
    //源模块号
    canID >>= 3;
    idPtr->srcModuleNo = (canID&0X07);
}


//CAN1短包发送
static ERROR_SUB Can1ProtSendPackShortFrame(CAN_PROT_PACK* packPtr)
{   
    ERROR_SUB errorSub = ERROR_SUB_OK;
    CAN_PROT_ID canProtID;
    //创建ID
    canProtID.retrySendCount = packPtr->retryCount;
    canProtID.packType = packPtr->packType;
    canProtID.frameType = CAN_PROT_FRAME_TYPE_SHORT;
    canProtID.singalChannel = packPtr->singalChannel;
    canProtID.targetBoardID = packPtr->targetBoard;
    canProtID.targetModuleNo = packPtr->targetModule;
    canProtID.srcBoardID = packPtr->srcBoardID;
    canProtID.srcModuleNo = packPtr->srcModuleNo;
    //上锁
    Can1ProtPortGetMutexLock();
    //数据拷贝
    can1ProtFrameBuf[0] = packPtr->packDataLen;
    UserMemCopy(can1ProtFrameBuf + 1,packPtr->packDataBufPtr,packPtr->packDataLen);
    //计算ID
    uint32_t canID = CanProtBaseCalcID(&canProtID);
    //数据发送
    errorSub = Can1ProtPortSendFrame(canID,packPtr->packDataLen + 1,can1ProtFrameBuf);
    //解锁
    Can1ProtPortReleaseMutexLock();
    if(errorSub != ERROR_SUB_OK)
    {
        //错误打印
        MCU_LogShowErrorSubCode(errorSub);
        errorSub = ERROR_SUB_BOARD_CAN1_PROT_SEND_FRAME_SHORT;
    }
    //返回结果
    return errorSub;
}

//CAN1长包起始帧
static ERROR_SUB Can1ProtSendPackLongFrameStart(CAN_PROT_PACK* packPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    CAN_PROT_ID canProtID;
    //创建ID
    canProtID.retrySendCount = packPtr->retryCount;
    canProtID.packType = packPtr->packType;
    canProtID.frameType = CAN_PROT_FRAME_TYPE_LONG_START;
    canProtID.singalChannel = packPtr->singalChannel;
    canProtID.targetBoardID = packPtr->targetBoard;
    canProtID.targetModuleNo = packPtr->targetModule;
    canProtID.srcBoardID = packPtr->srcBoardID;
    canProtID.srcModuleNo = packPtr->srcModuleNo;
    //上锁
    Can1ProtPortGetMutexLock();
    //总数据长度
    can1ProtFrameBuf[0] = (uint8_t)(packPtr->packDataLen);
    can1ProtFrameBuf[1] = (uint8_t)((packPtr->packDataLen)>>8);
    can1ProtFrameBuf[2] = CAN1_PROT_DATA_BUF_LEN;
    //最后一包数据长度
    uint8_t lastPackDataLen = (packPtr->packDataLen%(CAN1_PROT_DATA_BUF_LEN-2));
    if(lastPackDataLen == 0)
    {
        //最后一包为0,刚好发完
        can1ProtFrameBuf[3] = CAN1_PROT_DATA_BUF_LEN;
    }
    else
    {
        //最后一包不为0,加上两个字节的索引
        can1ProtFrameBuf[3] = 2 + lastPackDataLen;
    }
    //数据包总数
    uint16_t dataPackCount = 0;
    dataPackCount = (packPtr->packDataLen/(CAN1_PROT_DATA_BUF_LEN-2));
    //最后一包不为0,那么还得加一包
    if(lastPackDataLen != 0)
    {
        dataPackCount += 1;
    }
    //写入包数
    can1ProtFrameBuf[4] = (uint8_t)dataPackCount;
    can1ProtFrameBuf[5] = (uint8_t)(dataPackCount>>8);
    //计算ID
    uint32_t canID = CanProtBaseCalcID(&canProtID);
    //数据发送
    errorSub = Can1ProtPortSendFrame(canID,6,can1ProtFrameBuf);
    //解锁
    Can1ProtPortReleaseMutexLock();
    if(errorSub != ERROR_SUB_OK)
    {
        //错误打印
        MCU_LogShowErrorSubCode(errorSub);
    }
    //返回结果
    return errorSub;
}

//CAN1_PORT长包数据帧
static ERROR_SUB Can1ProtSendPackLongFrameData(CAN_PROT_PACK* packPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    CAN_PROT_ID canProtID;
    //创建ID
    canProtID.retrySendCount = packPtr->retryCount;
    canProtID.packType = packPtr->packType;
    canProtID.frameType = CAN_PROT_FRAME_TYPE_LONG_DATA;
    canProtID.singalChannel = packPtr->singalChannel;
    canProtID.targetBoardID = packPtr->targetBoard;
    canProtID.targetModuleNo = packPtr->targetModule;
    canProtID.srcBoardID = packPtr->srcBoardID;
    canProtID.srcModuleNo = packPtr->srcModuleNo;
    //开始发送数据
    uint16_t packIndexUtil = 0;
    uint8_t lastPackDataLen = (packPtr->packDataLen%(CAN1_PROT_DATA_BUF_LEN-2));
    uint16_t dataPackCount = (packPtr->packDataLen/(CAN1_PROT_DATA_BUF_LEN-2));
    //最后一包不为0,那么还得加一包
    if(lastPackDataLen != 0)
    {
        dataPackCount += 1;
    }
    else
    {
        //最后一包数据长度为0的时候,需要修正最后一包长度为满包
        lastPackDataLen = CAN1_PROT_DATA_BUF_LEN-2;
    }
    //计算ID
    uint32_t canID = CanProtBaseCalcID(&canProtID);
    for(packIndexUtil = 0; packIndexUtil < dataPackCount; packIndexUtil++)
    {
        //上锁
        Can1ProtPortGetMutexLock();
        can1ProtFrameBuf[0] = (uint8_t)(packIndexUtil);
        can1ProtFrameBuf[1] = (uint8_t)((packIndexUtil)>>8);
        if(packIndexUtil == (dataPackCount - 1))
        {
            //最后一包,数据拷贝
            UserMemCopy(can1ProtFrameBuf+2,packPtr->packDataBufPtr + (packIndexUtil*(CAN1_PROT_DATA_BUF_LEN - 2)),
                            lastPackDataLen);
            //数据发送
            errorSub = Can1ProtPortSendFrame(canID,lastPackDataLen+2,can1ProtFrameBuf);
            //解锁
            Can1ProtPortReleaseMutexLock();
        }
        else 
        {
            //不是最后一包,数据拷贝
            UserMemCopy(can1ProtFrameBuf+2,packPtr->packDataBufPtr + (packIndexUtil*(CAN1_PROT_DATA_BUF_LEN - 2)),
                            CAN1_PROT_DATA_BUF_LEN - 2);
            //数据发送
            errorSub = Can1ProtPortSendFrame(canID,CAN1_PROT_DATA_BUF_LEN,can1ProtFrameBuf);
            //解锁
            Can1ProtPortReleaseMutexLock();
        }
        if(errorSub != ERROR_SUB_OK)
        {
            //错误打印
            MCU_LogShowErrorSubCode(errorSub);
            //返回结果
            return errorSub;
        }
    }
    return ERROR_SUB_OK;
}

//CAN1长包结束帧
static ERROR_SUB Can1ProtSendPackLongFrameEnd(CAN_PROT_PACK* packPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    CAN_PROT_ID canProtID;
    //创建ID
    canProtID.retrySendCount = packPtr->retryCount;
    canProtID.packType = packPtr->packType;
    canProtID.frameType = CAN_PROT_FRAME_TYPE_LONG_END;
    canProtID.singalChannel = packPtr->singalChannel;
    canProtID.targetBoardID = packPtr->targetBoard;
    canProtID.targetModuleNo = packPtr->targetModule;
    canProtID.srcBoardID = packPtr->srcBoardID;
    canProtID.srcModuleNo = packPtr->srcModuleNo;
    //计算CRC结果
    uint16_t crcResult = CanProtPortCalcCRC16(packPtr->packDataBufPtr,packPtr->packDataLen);
    //上锁
    Can1ProtPortGetMutexLock();
    //总数据长度
    can1ProtFrameBuf[0] = (uint8_t)(crcResult);
    can1ProtFrameBuf[1] = (uint8_t)((crcResult)>>8);
    //计算ID
    uint32_t canID = CanProtBaseCalcID(&canProtID);
    //数据发送
    errorSub = Can1ProtPortSendFrame(canID,2,can1ProtFrameBuf);
    //解锁
    Can1ProtPortReleaseMutexLock();
    if(errorSub != ERROR_SUB_OK)
    {
        //错误打印
        MCU_LogShowErrorSubCode(errorSub);
    }
    //返回结果
    return errorSub;
}

//CAN1长包发送
static ERROR_SUB Can1ProtSendPackLongFrame(CAN_PROT_PACK* packPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //长包起始帧发送
    errorSub = Can1ProtSendPackLongFrameStart(packPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        //错误打印
        MCU_LogShowErrorSubCode(errorSub);
        //转换错误代码
        return ERROR_SUB_BOARD_CAN1_PROT_SEND_FRAME_LONG_START;
    }
    //长包数据帧发送
    errorSub = Can1ProtSendPackLongFrameData(packPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        //错误打印
        MCU_LogShowErrorSubCode(errorSub);
        //转换错误代码
        return ERROR_SUB_BOARD_CAN1_PROT_SEND_FRAME_LONG_DATA;
    }
    //长包结果帧发送
    errorSub = Can1ProtSendPackLongFrameEnd(packPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        //错误打印
        MCU_LogShowErrorSubCode(errorSub);
        //转换错误代码
        return ERROR_SUB_BOARD_CAN1_PROT_SEND_FRAME_LONG_END;
    }
    //正常发送完成
    return ERROR_SUB_OK;
}


//CAN2短包发送
static ERROR_SUB Can2ProtSendPackShortFrame(CAN_PROT_PACK* packPtr)
{   
    ERROR_SUB errorSub = ERROR_SUB_OK;
    CAN_PROT_ID canProtID;
    //创建ID
    canProtID.retrySendCount = packPtr->retryCount;
    canProtID.packType = packPtr->packType;
    canProtID.frameType = CAN_PROT_FRAME_TYPE_SHORT;
    canProtID.singalChannel = packPtr->singalChannel;
    canProtID.targetBoardID = packPtr->targetBoard;
    canProtID.targetModuleNo = packPtr->targetModule;
    canProtID.srcBoardID = packPtr->srcBoardID;
    canProtID.srcModuleNo = packPtr->srcModuleNo;
    //上锁
    Can2ProtPortGetMutexLock();
    //数据拷贝
    can2ProtFrameBuf[0] = packPtr->packDataLen;
    UserMemCopy(can2ProtFrameBuf + 1,packPtr->packDataBufPtr,packPtr->packDataLen);
    //计算ID
    uint32_t canID = CanProtBaseCalcID(&canProtID);
    //数据发送
    errorSub = Can2ProtPortSendFrame(canID,packPtr->packDataLen + 1,can2ProtFrameBuf);
    //解锁
    Can2ProtPortReleaseMutexLock();
    if(errorSub != ERROR_SUB_OK)
    {
        //错误打印
        MCU_LogShowErrorSubCode(errorSub);
        errorSub = ERROR_SUB_BOARD_CAN2_PROT_SEND_FRAME_SHORT;
    }
    //返回结果
    return errorSub;
}

//CAN2长包起始帧
static ERROR_SUB Can2ProtSendPackLongFrameStart(CAN_PROT_PACK* packPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    CAN_PROT_ID canProtID;
    //创建ID
    canProtID.retrySendCount = packPtr->retryCount;
    canProtID.packType = packPtr->packType;
    canProtID.frameType = CAN_PROT_FRAME_TYPE_LONG_START;
    canProtID.singalChannel = packPtr->singalChannel;
    canProtID.targetBoardID = packPtr->targetBoard;
    canProtID.targetModuleNo = packPtr->targetModule;
    canProtID.srcBoardID = packPtr->srcBoardID;
    canProtID.srcModuleNo = packPtr->srcModuleNo;
    //上锁
    Can2ProtPortGetMutexLock();
    //总数据长度
    can2ProtFrameBuf[0] = (uint8_t)(packPtr->packDataLen);
    can2ProtFrameBuf[1] = (uint8_t)((packPtr->packDataLen)>>8);
    can2ProtFrameBuf[2] = CAN2_PROT_DATA_BUF_LEN;
    //最后一包数据长度
    uint8_t lastPackDataLen = (packPtr->packDataLen%(CAN2_PROT_DATA_BUF_LEN-2));
    if(lastPackDataLen == 0)
    {
        //最后一包为0,刚好发完
        can2ProtFrameBuf[3] = CAN2_PROT_DATA_BUF_LEN;
    }
    else
    {
        //最后一包不为0,加上两个字节的索引
        can2ProtFrameBuf[3] = 2 + lastPackDataLen;
    }
    //数据包总数
    uint16_t dataPackCount = 0;
    dataPackCount = (packPtr->packDataLen/(CAN2_PROT_DATA_BUF_LEN-2));
    //最后一包不为0,那么还得加一包
    if(lastPackDataLen != 0)
    {
        dataPackCount += 1;
    }
    //写入包数
    can2ProtFrameBuf[4] = (uint8_t)dataPackCount;
    can2ProtFrameBuf[5] = (uint8_t)(dataPackCount>>8);
    //计算ID
    uint32_t canID = CanProtBaseCalcID(&canProtID);
    //数据发送
    errorSub = Can2ProtPortSendFrame(canID,6,can2ProtFrameBuf);
    //解锁
    Can2ProtPortReleaseMutexLock();
    if(errorSub != ERROR_SUB_OK)
    {
        //错误打印
        MCU_LogShowErrorSubCode(errorSub);
    }
    //返回结果
    return errorSub;
}

//CAN2_PORT长包数据帧
static ERROR_SUB Can2ProtSendPackLongFrameData(CAN_PROT_PACK* packPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    CAN_PROT_ID canProtID;
    //创建ID
    canProtID.retrySendCount = packPtr->retryCount;
    canProtID.packType = packPtr->packType;
    canProtID.frameType = CAN_PROT_FRAME_TYPE_LONG_DATA;
    canProtID.singalChannel = packPtr->singalChannel;
    canProtID.targetBoardID = packPtr->targetBoard;
    canProtID.targetModuleNo = packPtr->targetModule;
    canProtID.srcBoardID = packPtr->srcBoardID;
    canProtID.srcModuleNo = packPtr->srcModuleNo;
    //开始发送数据
    uint16_t packIndexUtil = 0;
    uint8_t lastPackDataLen = (packPtr->packDataLen%(CAN2_PROT_DATA_BUF_LEN-2));
    uint16_t dataPackCount = (packPtr->packDataLen/(CAN2_PROT_DATA_BUF_LEN-2));
    //最后一包不为0,那么还得加一包
    if(lastPackDataLen != 0)
    {
        dataPackCount += 1;
    }
    else
    {
        //最后一包数据长度为0的时候,需要修正最后一包长度为满包
        lastPackDataLen = CAN2_PROT_DATA_BUF_LEN-2;
    }
    //计算ID
    uint32_t canID = CanProtBaseCalcID(&canProtID);
    for(packIndexUtil = 0; packIndexUtil < dataPackCount; packIndexUtil++)
    {
        //上锁
        Can2ProtPortGetMutexLock();
        can2ProtFrameBuf[0] = (uint8_t)(packIndexUtil);
        can2ProtFrameBuf[1] = (uint8_t)((packIndexUtil)>>8);
        if(packIndexUtil == (dataPackCount - 1))
        {
            //最后一包,数据拷贝
            UserMemCopy(can2ProtFrameBuf+2,packPtr->packDataBufPtr + (packIndexUtil*(CAN2_PROT_DATA_BUF_LEN - 2)),
                            lastPackDataLen);
            //数据发送
            errorSub = Can2ProtPortSendFrame(canID,lastPackDataLen+2,can2ProtFrameBuf);
            //解锁
            Can2ProtPortReleaseMutexLock();
        }
        else 
        {
            //不是最后一包,数据拷贝
            UserMemCopy(can2ProtFrameBuf+2,packPtr->packDataBufPtr + (packIndexUtil*(CAN2_PROT_DATA_BUF_LEN - 2)),
                            CAN2_PROT_DATA_BUF_LEN - 2);
            //数据发送
            errorSub = Can2ProtPortSendFrame(canID,CAN2_PROT_DATA_BUF_LEN,can2ProtFrameBuf);
            //解锁
            Can2ProtPortReleaseMutexLock();
        }
        if(errorSub != ERROR_SUB_OK)
        {
            //错误打印
            MCU_LogShowErrorSubCode(errorSub);
            //返回结果
            return errorSub;
        }
    }
    return ERROR_SUB_OK;
}

//CAN2长包结束帧
static ERROR_SUB Can2ProtSendPackLongFrameEnd(CAN_PROT_PACK* packPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    CAN_PROT_ID canProtID;
    //创建ID
    canProtID.retrySendCount = packPtr->retryCount;
    canProtID.packType = packPtr->packType;
    canProtID.frameType = CAN_PROT_FRAME_TYPE_LONG_END;
    canProtID.singalChannel = packPtr->singalChannel;
    canProtID.targetBoardID = packPtr->targetBoard;
    canProtID.targetModuleNo = packPtr->targetModule;
    canProtID.srcBoardID = packPtr->srcBoardID;
    canProtID.srcModuleNo = packPtr->srcModuleNo;
    //计算CRC结果
    uint16_t crcResult = CanProtPortCalcCRC16(packPtr->packDataBufPtr,packPtr->packDataLen);
    //上锁
    Can2ProtPortGetMutexLock();
    //总数据长度
    can2ProtFrameBuf[0] = (uint8_t)(crcResult);
    can2ProtFrameBuf[1] = (uint8_t)((crcResult)>>8);
    //计算ID
    uint32_t canID = CanProtBaseCalcID(&canProtID);
    //数据发送
    errorSub = Can2ProtPortSendFrame(canID,2,can2ProtFrameBuf);
    //解锁
    Can2ProtPortReleaseMutexLock();
    if(errorSub != ERROR_SUB_OK)
    {
        //错误打印
        MCU_LogShowErrorSubCode(errorSub);
    }
    //返回结果
    return errorSub;
}

//CAN2长包发送
static ERROR_SUB Can2ProtSendPackLongFrame(CAN_PROT_PACK* packPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //长包起始帧发送
    errorSub = Can2ProtSendPackLongFrameStart(packPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        //错误打印
        MCU_LogShowErrorSubCode(errorSub);
        //转换错误代码
        return ERROR_SUB_BOARD_CAN2_PROT_SEND_FRAME_LONG_START;
    }
    //长包数据帧发送
    errorSub = Can2ProtSendPackLongFrameData(packPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        //错误打印
        MCU_LogShowErrorSubCode(errorSub);
        //转换错误代码
        return ERROR_SUB_BOARD_CAN2_PROT_SEND_FRAME_LONG_DATA;
    }
    //长包结果帧发送
    errorSub = Can2ProtSendPackLongFrameEnd(packPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        //错误打印
        MCU_LogShowErrorSubCode(errorSub);
        //转换错误代码
        return ERROR_SUB_BOARD_CAN2_PROT_SEND_FRAME_LONG_END;
    }
    //正常发送完成
    return ERROR_SUB_OK;
}

//数据包发送
ERROR_SUB CanProtBaseSendPack(PORT_CAN canPort,CAN_PROT_PACK* packPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    if((packPtr->packDataLen == 0)&&(packPtr->packDataBufPtr != NULL))
    {
        //数据包长度为0但是缓存不是空
        if(canPort == PORT_CAN1)
        {
            return ERROR_SUB_BOARD_CAN1_PROT_PACK_LEN_ZERO_BUF_NOT_NULL;
        }
        else
        {
            return ERROR_SUB_BOARD_CAN2_PROT_PACK_LEN_ZERO_BUF_NOT_NULL;
        }
    }
    if((packPtr->packDataLen != 0)&&(packPtr->packDataBufPtr == NULL))
    {
        //数据包长度不为0但是缓存是空
        if(canPort == PORT_CAN1)
        {
            return ERROR_SUB_BOARD_CAN1_PROT_PACK_LEN_NOT_ZERO_BUF_NULL;
        }
        else
        {
            return ERROR_SUB_BOARD_CAN2_PROT_PACK_LEN_NOT_ZERO_BUF_NULL;
        }
    }
    //数据包长度大于协议支持最大长度
    if((canPort == PORT_CAN1)&&(packPtr->packDataLen > CAN1_PROT_SINGLE_PACK_DATA_MAX))
    {
        return ERROR_SUB_BOARD_CAN1_PROT_PACK_LEN_MAX;
    }
    //数据包长度大于协议支持最大长度
    if((canPort == PORT_CAN2)&&(packPtr->packDataLen > CAN2_PROT_SINGLE_PACK_DATA_MAX))
    {
        return ERROR_SUB_BOARD_CAN2_PROT_PACK_LEN_MAX;
    }
    if(canPort == PORT_CAN1)
    {
        //CAN1处理
        if(packPtr->packDataLen <= CAN1_PROT_DATA_LEN_SINGLE_FRAME)
        {
            //单包数据发送
            errorSub = Can1ProtSendPackShortFrame(packPtr);
        }
        else
        {
            //长包数据发送
            errorSub = Can1ProtSendPackLongFrame(packPtr);
        }
    }
    else
    {
        //CAN2处理
        if(packPtr->packDataLen <= CAN2_PROT_DATA_LEN_SINGLE_FRAME)
        {
            //单包数据发送
            errorSub = Can2ProtSendPackShortFrame(packPtr);
        }
        else
        {
            //长包数据发送
            errorSub = Can2ProtSendPackLongFrame(packPtr);
        }
    }
    return errorSub;
}

//比较日志缓存中的元素是否相等,用于节点查找
ADT_CMP_RESULT ADT_RecvCacheListElementCmp(void* cmpSrc,void* cmpDst)
{
    CAN_PROT_RECV_PACK_CACHE* recvMsgCachePtr = (CAN_PROT_RECV_PACK_CACHE*)cmpSrc;
    CAN_PROT_RECV_PACK_CACHE_HEADER* recvMsgHeaderPtr = (CAN_PROT_RECV_PACK_CACHE_HEADER*)cmpDst;
    if(recvMsgCachePtr == NULL)
    {
        return ADT_CMP_NOT_EQUAL;
    }
    if(recvMsgHeaderPtr == NULL)
    {
        return ADT_CMP_NOT_EQUAL;
    }
    //开始比较
    if(recvMsgCachePtr->targetBoardID != recvMsgHeaderPtr->targetBoardID)
    {
        return ADT_CMP_NOT_EQUAL;
    }
    if(recvMsgCachePtr->targetModuleNo != recvMsgHeaderPtr->targetModuleNo)
    {
        return ADT_CMP_NOT_EQUAL;
    }
    if(recvMsgCachePtr->srcBoardID != recvMsgHeaderPtr->srcBoardID)
    {
        return ADT_CMP_NOT_EQUAL;
    }
    if(recvMsgCachePtr->srcModuleNo != recvMsgHeaderPtr->srcModuleNo)
    {
        return ADT_CMP_NOT_EQUAL;
    }
    if(recvMsgCachePtr->singalChannel != recvMsgHeaderPtr->singalChannel)
    {
        return ADT_CMP_NOT_EQUAL;
    }
    return ADT_CMP_EQUAL;
}


//CAN1_PORT发送指令包,仅发送不做数据存储
ERROR_SUB Can1ProtSendCmdOnly(uint8_t srcModule,uint8_t srcBoardID,uint8_t targetModule,uint8_t targetBoardID,uint8_t singalChannel,
                                            uint8_t retryCount,uint32_t cmdVal,uint8_t* paramBuffer,uint16_t paramLength)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    CAN_PROT_PACK canProtPack;
#if(LOCAL_BUF_LEN_CAN1_PROT_BASE != 0)
    uint8_t sendBuffer[LOCAL_BUF_LEN_CAN1_PROT_BASE] = {0};
#endif
    canProtPack.retryCount = (retryCount&0X03);
    canProtPack.singalChannel = (singalChannel&0X3F);
    canProtPack.targetBoard = (targetBoardID&0X1F);
    canProtPack.targetModule = (targetModule&0X07);
    canProtPack.srcBoardID = (srcBoardID&0X1F);
    canProtPack.srcModuleNo = (srcModule&0X07);
    canProtPack.packDataLen = 3 + paramLength;
#if(LOCAL_BUF_LEN_CAN1_PROT_BASE != 0)
    if(canProtPack.packDataLen > LOCAL_BUF_LEN_CAN1_PROT_BASE)
    {
        //内存申请
        canProtPack.packDataBufPtr = Can1ProtPortMemMallocWhileSuccess(canProtPack.packDataLen);
    }
    else
    {
        //使用栈替代
        canProtPack.packDataBufPtr = sendBuffer;
    }
#else
    canProtPack.packDataBufPtr = Can1ProtPortMemMallocWhileSuccess(canProtPack.packDataLen);
#endif
    //数据写入
    canProtPack.packDataBufPtr[0] = (uint8_t)(cmdVal&0xff);
    cmdVal >>= 8;
    canProtPack.packDataBufPtr[1] = (uint8_t)(cmdVal&0xff);
    cmdVal >>= 8;
    canProtPack.packDataBufPtr[2] = (uint8_t)(cmdVal&0xff);
    //参数写入
    if(paramLength != 0)
    {
        UserMemCopy(canProtPack.packDataBufPtr + 3,paramBuffer,paramLength);
    }
    //数据包类型
    canProtPack.packType = PROT_PACK_TYPE_CMD;
    errorSub = CanProtBaseSendPack(PORT_CAN1,&canProtPack);
#if(LOCAL_BUF_LEN_CAN1_PROT_BASE != 0)
    if(canProtPack.packDataLen > LOCAL_BUF_LEN_CAN1_PROT_BASE)
    {
        //释放内存
        Can1ProtPortMemFree(canProtPack.packDataBufPtr);
    }
#else
    //释放内存
    Can1ProtPortMemFree(canProtPack.packDataBufPtr);
#endif
    return errorSub;
}

//CAN1_PORT发送指令包,仅发送不做数据存储
ERROR_SUB Can1ProtSendCmdOnlySample(uint8_t targetModule,uint8_t targetBoardID,uint8_t singalChannel,uint8_t retryCount,uint32_t cmdVal,
                                                uint8_t* paramBuffer,uint16_t paramLength)
{
    return Can1ProtSendCmdOnly(can1ProtInitSet.selfModuleNo,can1ProtInitSet.selfBoardID,targetModule,targetBoardID,singalChannel,retryCount,
                                            cmdVal,paramBuffer,paramLength);
}

//CAN1_PORT发送指令ACK包
ERROR_SUB Can1ProtSendAckCmd(uint8_t srcModule,uint8_t srcBoardID,uint8_t targetModule,uint8_t targetBoardID,uint8_t singalChannel,
                                        uint8_t retryCount,uint32_t cmdVal,PROT_ACK ackCmd)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    CAN_PROT_PACK canProtPack;
    uint8_t sendBuffer[4] = {0};
    canProtPack.retryCount = (retryCount&0X03);
    canProtPack.singalChannel = (singalChannel&0X3F);
    canProtPack.targetBoard = (targetBoardID&0X1F);
    canProtPack.targetModule = (targetModule&0X07);
    canProtPack.srcBoardID = (srcBoardID&0X1F);
    canProtPack.srcModuleNo = (srcModule&0X07);
    canProtPack.packDataLen = 4;
    //内存申请
    canProtPack.packDataBufPtr = sendBuffer;
    //数据写入
    canProtPack.packDataBufPtr[0] = (uint8_t)(cmdVal&0xff);
    cmdVal >>= 8;
    canProtPack.packDataBufPtr[1] = (uint8_t)(cmdVal&0xff);
    cmdVal >>= 8;
    canProtPack.packDataBufPtr[2] = (uint8_t)(cmdVal&0xff);
    //参数写入
    canProtPack.packDataBufPtr[3] = (uint8_t)(ackCmd);
    //数据包类型
    canProtPack.packType = PROT_PACK_TYPE_ACK_CMD;
    errorSub = CanProtBaseSendPack(PORT_CAN1,&canProtPack);
    return errorSub;
}

//CAN1_PORT发送指令ACK包
ERROR_SUB Can1ProtSendAckCmdSample(uint8_t targetModule,uint8_t targetBoardID,uint8_t singalChannel,uint8_t retryCount,uint32_t cmdVal,
                                                PROT_ACK ackCmd)
{
    return Can1ProtSendAckCmd(can1ProtInitSet.selfModuleNo,can1ProtInitSet.selfBoardID,targetModule,targetBoardID,singalChannel,
                                            retryCount,cmdVal,ackCmd);
}

//CAN1_PORT发送结果包,仅发送不做数据存储
ERROR_SUB Can1ProtSendResultOnly(uint8_t srcModule,uint8_t srcBoardID,uint8_t targetModule,uint8_t targetBoardID,uint8_t singalChannel,
                                    uint8_t retryCount,uint32_t cmdVal,PROT_RESULT resultCode,ERROR_MAIN errorMain,ERROR_LEVEL errorLevel,
                                    ERROR_SUB errorSub,uint8_t* resultParamBuffer,uint16_t resultBufferLen)
{
    ERROR_SUB errorSubFunc = ERROR_SUB_OK;
    uint32_t utilDataTemp = 0;
    CAN_PROT_PACK canProtPack;
#if(LOCAL_BUF_LEN_CAN1_PROT_BASE != 0)
    uint8_t sendBuffer[LOCAL_BUF_LEN_CAN1_PROT_BASE] = {0};
#endif
    canProtPack.retryCount = (retryCount&0X03);
    canProtPack.singalChannel = (singalChannel&0X3F);
    canProtPack.targetBoard = (targetBoardID&0X1F);
    canProtPack.targetModule = (targetModule&0X07);
    canProtPack.srcBoardID = (srcBoardID&0X1F);
    canProtPack.srcModuleNo = (srcModule&0X07);
    canProtPack.packDataLen = 11 + resultBufferLen;
#if(LOCAL_BUF_LEN_CAN1_PROT_BASE != 0)
    if(canProtPack.packDataLen > LOCAL_BUF_LEN_CAN1_PROT_BASE)
    {
        //内存申请
        canProtPack.packDataBufPtr = Can1ProtPortMemMallocWhileSuccess(canProtPack.packDataLen);
    }
    else
    {
        //使用栈替代
        canProtPack.packDataBufPtr = sendBuffer;
    }
#else
    canProtPack.packDataBufPtr = Can1ProtPortMemMallocWhileSuccess(canProtPack.packDataLen);
#endif
    //数据写入
    canProtPack.packDataBufPtr[0] = (uint8_t)(cmdVal&0xff);
    cmdVal >>= 8;
    canProtPack.packDataBufPtr[1] = (uint8_t)(cmdVal&0xff);
    cmdVal >>= 8;
    canProtPack.packDataBufPtr[2] = (uint8_t)(cmdVal&0xff);
    //执行结果
    canProtPack.packDataBufPtr[3] = (uint8_t)resultCode;
    //错误主码
    utilDataTemp = (uint32_t)errorMain;
    canProtPack.packDataBufPtr[4] = (uint8_t)(utilDataTemp&0xff);
    utilDataTemp >>= 8;
    canProtPack.packDataBufPtr[5] = (uint8_t)(utilDataTemp&0xff);
    //错误级别
    canProtPack.packDataBufPtr[6] = (uint8_t)errorLevel;
    //错误子码
    utilDataTemp = (uint32_t)errorSub;
    uint32_t boardIdCode = Can1ProtPortGetSelfBoardID();
    boardIdCode <<= 24;
    //对错误子码进行处理附加板号
    if((utilDataTemp != 0X00000000)&&((utilDataTemp&0XFF000000) == 0X00000000))
    {
        //最大区域等于0
        utilDataTemp |= boardIdCode;
    }
    canProtPack.packDataBufPtr[7] = (uint8_t)(utilDataTemp&0xff);
    utilDataTemp >>= 8;
    canProtPack.packDataBufPtr[8] = (uint8_t)(utilDataTemp&0xff);
    utilDataTemp >>= 8;
    canProtPack.packDataBufPtr[9] = (uint8_t)(utilDataTemp&0xff);
    utilDataTemp >>= 8;
    canProtPack.packDataBufPtr[10] = (uint8_t)(utilDataTemp&0xff);
    //参数拷贝
    if(resultBufferLen > 0)
    {
        UserMemCopy(canProtPack.packDataBufPtr+11,resultParamBuffer,resultBufferLen);
    }
    //数据包类型
    canProtPack.packType = PROT_PACK_TYPE_RESULT;
    //数据包发送
    errorSubFunc = CanProtBaseSendPack(PORT_CAN1,&canProtPack);
#if(LOCAL_BUF_LEN_CAN1_PROT_BASE != 0)
    if(canProtPack.packDataLen > LOCAL_BUF_LEN_CAN1_PROT_BASE)
    {
        //释放内存
        Can1ProtPortMemFree(canProtPack.packDataBufPtr);
    }
#else
    //释放内存
    Can1ProtPortMemFree(canProtPack.packDataBufPtr);
#endif
    return errorSubFunc;
}

//CAN1_PORT发送结果包,仅发送不做数据存储
ERROR_SUB Can1ProtSendResultOnlySample(uint8_t targetModule,uint8_t targetBoardID,uint8_t singalChannel,uint8_t retryCount,uint32_t cmdVal,
                                        PROT_RESULT resultCode,ERROR_MAIN errorMain,ERROR_LEVEL errorLevel,ERROR_SUB errorSub,
                                        uint8_t* resultParamBuffer,uint16_t resultBufferLen)
{
    return Can1ProtSendResultOnly(can1ProtInitSet.selfModuleNo,can1ProtInitSet.selfBoardID,targetModule,targetBoardID,singalChannel,
                                                retryCount,cmdVal,resultCode,errorMain,errorLevel,errorSub,resultParamBuffer,resultBufferLen);
}

//CAN1_PORT发送结果ACK包
ERROR_SUB Can1ProtSendAckResult(uint8_t srcModule,uint8_t srcBoardID,uint8_t targetModule,uint8_t targetBoardID,uint8_t singalChannel,
                                uint8_t retryCount,uint32_t cmdVal)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    CAN_PROT_PACK canProtPack;
    uint8_t sendBuffer[3] = {0};
    canProtPack.retryCount = (retryCount&0X03);
    canProtPack.singalChannel = (singalChannel&0X3F);
    canProtPack.targetBoard = (targetBoardID&0X1F);
    canProtPack.targetModule = (targetModule&0X07);
    canProtPack.srcBoardID = (srcBoardID&0X1F);
    canProtPack.srcModuleNo = (srcModule&0X07);
    canProtPack.packDataLen = 3;
    //内存申请
    canProtPack.packDataBufPtr = sendBuffer;
    //数据写入
    canProtPack.packDataBufPtr[0] = (uint8_t)(cmdVal&0xff);
    cmdVal >>= 8;
    canProtPack.packDataBufPtr[1] = (uint8_t)(cmdVal&0xff);
    cmdVal >>= 8;
    canProtPack.packDataBufPtr[2] = (uint8_t)(cmdVal&0xff);
    //数据包类型
    canProtPack.packType = PROT_PACK_TYPE_ACK_RESULT;
    errorSub = CanProtBaseSendPack(PORT_CAN1,&canProtPack);
    return errorSub;
}

//CAN1_PORT发送结果ACK包
ERROR_SUB Can1ProtSendAckResultSample(uint8_t targetModule,uint8_t targetBoardID,uint8_t singalChannel,uint8_t retryCount,uint32_t cmdVal)
{
    return Can1ProtSendAckResult(can1ProtInitSet.selfModuleNo,can1ProtInitSet.selfBoardID,targetModule,targetBoardID,singalChannel,
                                            retryCount,cmdVal);
}

//CAN1_PORT发送数据上传包,仅发送不做数据存储
ERROR_SUB Can1ProtSendUploadOnly(uint8_t srcModule,uint8_t srcBoardID,uint8_t targetModule,uint8_t targetBoardID,uint8_t retryCount,uint32_t uploadDataID,
                                            uint8_t* uploadDataBuffer,uint16_t uploadDataLen)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    CAN_PROT_PACK canProtPack;
#if(LOCAL_BUF_LEN_CAN1_PROT_BASE != 0)
    uint8_t sendBuffer[LOCAL_BUF_LEN_CAN1_PROT_BASE] = {0};
#endif
    canProtPack.retryCount = (retryCount&0X03);
    canProtPack.singalChannel = 0;
    canProtPack.targetBoard = (targetBoardID&0X1F);
    canProtPack.targetModule = (targetModule&0X07);
    canProtPack.srcBoardID = (srcBoardID&0X1F);
    canProtPack.srcModuleNo = (srcModule&0X07);
    canProtPack.packDataLen = 4 + uploadDataLen;
#if(LOCAL_BUF_LEN_CAN1_PROT_BASE != 0)
    if(canProtPack.packDataLen > LOCAL_BUF_LEN_CAN1_PROT_BASE)
    {
        //内存申请
        canProtPack.packDataBufPtr = Can1ProtPortMemMallocWhileSuccess(canProtPack.packDataLen);
    }
    else
    {
        //使用栈替代
        canProtPack.packDataBufPtr = sendBuffer;
    }
#else
    //内存申请
    canProtPack.packDataBufPtr = Can1ProtPortMemMallocWhileSuccess(canProtPack.packDataLen);
#endif
    //数据写入
    canProtPack.packDataBufPtr[0] = (uint8_t)(uploadDataID&0xff);
    uploadDataID >>= 8;
    canProtPack.packDataBufPtr[1] = (uint8_t)(uploadDataID&0xff);
    uploadDataID >>= 8;
    canProtPack.packDataBufPtr[2] = (uint8_t)(uploadDataID&0xff);
    uploadDataID >>= 8;
    canProtPack.packDataBufPtr[3] = (uint8_t)(uploadDataID&0xff);
    //参数拷贝
    if(uploadDataLen > 0)
    {
        UserMemCopy(canProtPack.packDataBufPtr+4,uploadDataBuffer,uploadDataLen);
    }
    //数据包类型
    canProtPack.packType = PROT_PACK_TYPE_DATA_UPLOAD;
    //数据包发送
    errorSub = CanProtBaseSendPack(PORT_CAN1,&canProtPack);
#if(LOCAL_BUF_LEN_CAN1_PROT_BASE != 0)
    if(canProtPack.packDataLen > LOCAL_BUF_LEN_CAN1_PROT_BASE)
    {
        //释放内存
        Can1ProtPortMemFree(canProtPack.packDataBufPtr);
    }
#else
    //释放内存
    Can1ProtPortMemFree(canProtPack.packDataBufPtr);
#endif
    return errorSub;
}

//CAN1_PORT发送数据上传包,仅发送不做数据存储
ERROR_SUB Can1ProtSendUploadOnlySample(uint8_t targetModule,uint8_t targetBoardID,uint8_t retryCount,uint32_t uploadDataID,
                                                    uint8_t* uploadDataBuffer,uint16_t uploadDataLen)
{
    return Can1ProtSendUploadOnly(can1ProtInitSet.selfModuleNo,can1ProtInitSet.selfBoardID,targetModule,targetBoardID,retryCount,
                                                uploadDataID,uploadDataBuffer,uploadDataLen);
}

//CAN1_PORT发送数据上传ACK包
ERROR_SUB Can1ProtSendAckUpload(uint8_t srcModule,uint8_t srcBoardID,uint8_t targetModule,uint8_t targetBoardID,uint8_t retryCount,
                                            uint32_t uploadDataID)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    CAN_PROT_PACK canProtPack;
    uint8_t sendBuffer[4] = {0};
    canProtPack.retryCount = (retryCount&0X03);
    canProtPack.singalChannel = 0;
    canProtPack.targetBoard = (targetBoardID&0X1F);
    canProtPack.targetModule = (targetModule&0X07);
    canProtPack.srcBoardID = (srcBoardID&0X1F);
    canProtPack.srcModuleNo = (srcModule&0X07);
    canProtPack.packDataLen = 4;
    //内存申请
    canProtPack.packDataBufPtr = sendBuffer;
    //数据写入
    canProtPack.packDataBufPtr[0] = (uint8_t)(uploadDataID&0xff);
    uploadDataID >>= 8;
    canProtPack.packDataBufPtr[1] = (uint8_t)(uploadDataID&0xff);
    uploadDataID >>= 8;
    canProtPack.packDataBufPtr[2] = (uint8_t)(uploadDataID&0xff);
    uploadDataID >>= 8;
    canProtPack.packDataBufPtr[3] = (uint8_t)(uploadDataID&0xff);
    //数据包类型
    canProtPack.packType = PROT_PACK_TYPE_ACK_DATA_UPLOAD;
    errorSub = CanProtBaseSendPack(PORT_CAN1,&canProtPack);
    return errorSub;
}

//CAN1_PORT发送数据上传ACK包
ERROR_SUB Can1ProtSendAckUploadSample(uint8_t targetModule,uint8_t targetBoardID,uint8_t retryCount,uint32_t uploadDataID)
{
    return Can1ProtSendAckUpload(can1ProtInitSet.selfModuleNo,can1ProtInitSet.selfBoardID,targetModule,targetBoardID,
                                            retryCount,uploadDataID);
}

/*******************************************************CAN2_PROT**************************************************************************/

//CAN_PORT2发送指令包,仅发送不做数据存储
ERROR_SUB Can2ProtSendCmdOnly(uint8_t srcModule,uint8_t srcBoardID,uint8_t targetModule,uint8_t targetBoardID,uint8_t singalChannel,
                                            uint8_t retryCount,uint32_t cmdVal,uint8_t* paramBuffer,uint16_t paramLength)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    CAN_PROT_PACK canProtPack;
#if(LOCAL_BUF_LEN_CAN2_PROT_BASE != 0)
    uint8_t sendBuffer[LOCAL_BUF_LEN_CAN2_PROT_BASE] = {0};
#endif
    canProtPack.retryCount = (retryCount&0X03);
    canProtPack.singalChannel = (singalChannel&0X3F);
    canProtPack.targetBoard = (targetBoardID&0X1F);
    canProtPack.targetModule = (targetModule&0X07);
    canProtPack.srcBoardID = (srcBoardID&0X1F);
    canProtPack.srcModuleNo = (srcModule&0X07);
    canProtPack.packDataLen = 3 + paramLength;
#if(LOCAL_BUF_LEN_CAN2_PROT_BASE != 0)
    if(canProtPack.packDataLen > LOCAL_BUF_LEN_CAN2_PROT_BASE)
    {
        //内存申请
        canProtPack.packDataBufPtr = Can2ProtPortMemMallocWhileSuccess(canProtPack.packDataLen);
    }
    else
    {
        //使用栈替代
        canProtPack.packDataBufPtr = sendBuffer;
    }
#else
    canProtPack.packDataBufPtr = Can2ProtPortMemMallocWhileSuccess(canProtPack.packDataLen);
#endif
    //数据写入
    canProtPack.packDataBufPtr[0] = (uint8_t)(cmdVal&0xff);
    cmdVal >>= 8;
    canProtPack.packDataBufPtr[1] = (uint8_t)(cmdVal&0xff);
    cmdVal >>= 8;
    canProtPack.packDataBufPtr[2] = (uint8_t)(cmdVal&0xff);
    //参数写入
    if(paramLength != 0)
    {
        UserMemCopy(canProtPack.packDataBufPtr + 3,paramBuffer,paramLength);
    }
    //数据包类型
    canProtPack.packType = PROT_PACK_TYPE_CMD;
    errorSub = CanProtBaseSendPack(PORT_CAN2,&canProtPack);
#if(LOCAL_BUF_LEN_CAN2_PROT_BASE != 0)
    if(canProtPack.packDataLen > LOCAL_BUF_LEN_CAN2_PROT_BASE)
    {
        //释放内存
        Can2ProtPortMemFree(canProtPack.packDataBufPtr);
    }
#else
    //释放内存
    Can2ProtPortMemFree(canProtPack.packDataBufPtr);
#endif
    return errorSub;
}

//CAN_PORT2发送指令包,仅发送不做数据存储
ERROR_SUB Can2ProtSendCmdOnlySample(uint8_t targetModule,uint8_t targetBoardID,uint8_t singalChannel,uint8_t retryCount,uint32_t cmdVal,
                                            uint8_t* paramBuffer,uint16_t paramLength)
{
    return Can2ProtSendCmdOnly(can2ProtInitSet.selfModuleNo,can2ProtInitSet.selfBoardID,targetModule,targetBoardID,
                                            singalChannel,retryCount,cmdVal,paramBuffer,paramLength);
}

//CAN_PORT2发送指令ACK包
ERROR_SUB Can2ProtSendAckCmd(uint8_t srcModule,uint8_t srcBoardID,uint8_t targetModule,uint8_t targetBoardID,uint8_t singalChannel,
                                        uint8_t retryCount,uint32_t cmdVal,PROT_ACK ackCmd)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    CAN_PROT_PACK canProtPack;
    uint8_t sendBuffer[4] = {0};
    canProtPack.retryCount = (retryCount&0X03);
    canProtPack.singalChannel = (singalChannel&0X3F);
    canProtPack.targetBoard = (targetBoardID&0X1F);
    canProtPack.targetModule = (targetModule&0X07);
    canProtPack.srcBoardID = (srcBoardID&0X1F);
    canProtPack.srcModuleNo = (srcModule&0X07);
    canProtPack.packDataLen = 4;
    //内存申请
    canProtPack.packDataBufPtr = sendBuffer;
    //数据写入
    canProtPack.packDataBufPtr[0] = (uint8_t)(cmdVal&0xff);
    cmdVal >>= 8;
    canProtPack.packDataBufPtr[1] = (uint8_t)(cmdVal&0xff);
    cmdVal >>= 8;
    canProtPack.packDataBufPtr[2] = (uint8_t)(cmdVal&0xff);
    //参数写入
    canProtPack.packDataBufPtr[3] = (uint8_t)(ackCmd);
    //数据包类型
    canProtPack.packType = PROT_PACK_TYPE_ACK_CMD;
    errorSub = CanProtBaseSendPack(PORT_CAN2,&canProtPack);
    return errorSub;
}

//CAN_PORT2发送指令ACK包,使用本机模块号和板号,不需要单独设置
ERROR_SUB Can2ProtSendAckCmdSample(uint8_t targetModule,uint8_t targetBoardID,uint8_t singalChannel,uint8_t retryCount,uint32_t cmdVal,
                                                PROT_ACK ackCmd)
{
    return Can2ProtSendAckCmd(can2ProtInitSet.selfModuleNo,can2ProtInitSet.selfBoardID,targetModule,targetBoardID,singalChannel,
                                            retryCount,cmdVal,ackCmd);
}

//CAN_PORT2发送结果包,仅发送不做数据存储
ERROR_SUB Can2ProtSendResultOnly(uint8_t srcModule,uint8_t srcBoardID,uint8_t targetModule,uint8_t targetBoardID,uint8_t singalChannel,
                                            uint8_t retryCount,uint32_t cmdVal,PROT_RESULT resultCode,ERROR_MAIN errorMain,
                                            ERROR_LEVEL errorLevel,ERROR_SUB errorSub,uint8_t* resultParamBuffer,uint16_t resultBufferLen)
{
    ERROR_SUB errorSubFunc = ERROR_SUB_OK;
    uint32_t utilDataTemp = 0;
    CAN_PROT_PACK canProtPack;
#if(LOCAL_BUF_LEN_CAN2_PROT_BASE != 0)
    uint8_t sendBuffer[LOCAL_BUF_LEN_CAN2_PROT_BASE] = {0};
#endif
    canProtPack.retryCount = (retryCount&0X03);
    canProtPack.singalChannel = (singalChannel&0X3F);
    canProtPack.targetBoard = (targetBoardID&0X1F);
    canProtPack.targetModule = (targetModule&0X07);
    canProtPack.srcBoardID = (srcBoardID&0X1F);
    canProtPack.srcModuleNo = (srcModule&0X07);
    canProtPack.packDataLen = 11 + resultBufferLen;
#if(LOCAL_BUF_LEN_CAN2_PROT_BASE != 0)
    if(canProtPack.packDataLen > LOCAL_BUF_LEN_CAN2_PROT_BASE)
    {
        //内存申请
        canProtPack.packDataBufPtr = Can2ProtPortMemMallocWhileSuccess(canProtPack.packDataLen);
    }
    else
    {
        //使用栈替代
        canProtPack.packDataBufPtr = sendBuffer;
    }
#else
    canProtPack.packDataBufPtr = Can2ProtPortMemMallocWhileSuccess(canProtPack.packDataLen);
#endif
    //数据写入
    canProtPack.packDataBufPtr[0] = (uint8_t)(cmdVal&0xff);
    cmdVal >>= 8;
    canProtPack.packDataBufPtr[1] = (uint8_t)(cmdVal&0xff);
    cmdVal >>= 8;
    canProtPack.packDataBufPtr[2] = (uint8_t)(cmdVal&0xff);
    //执行结果
    canProtPack.packDataBufPtr[3] = (uint8_t)resultCode;
    //错误主码
    utilDataTemp = (uint32_t)errorMain;
    canProtPack.packDataBufPtr[4] = (uint8_t)(utilDataTemp&0xff);
    utilDataTemp >>= 8;
    canProtPack.packDataBufPtr[5] = (uint8_t)(utilDataTemp&0xff);
    //错误级别
    canProtPack.packDataBufPtr[6] = (uint8_t)errorLevel;
    //错误子码
    utilDataTemp = (uint32_t)errorSub;
    uint32_t boardIdCode = Can2ProtPortGetSelfBoardID();
    boardIdCode <<= 24;
    //对错误子码进行处理附加板号
    if((utilDataTemp != 0X00000000)&&((utilDataTemp&0XFF000000) == 0X00000000))
    {
        //最大区域等于0
        utilDataTemp |= boardIdCode;
    }
    canProtPack.packDataBufPtr[7] = (uint8_t)(utilDataTemp&0xff);
    utilDataTemp >>= 8;
    canProtPack.packDataBufPtr[8] = (uint8_t)(utilDataTemp&0xff);
    utilDataTemp >>= 8;
    canProtPack.packDataBufPtr[9] = (uint8_t)(utilDataTemp&0xff);
    utilDataTemp >>= 8;
    canProtPack.packDataBufPtr[10] = (uint8_t)(utilDataTemp&0xff);
    //参数拷贝
    if(resultBufferLen > 0)
    {
        UserMemCopy(canProtPack.packDataBufPtr+11,resultParamBuffer,resultBufferLen);
    }
    //数据包类型
    canProtPack.packType = PROT_PACK_TYPE_RESULT;
    //数据包发送
    errorSubFunc = CanProtBaseSendPack(PORT_CAN2,&canProtPack);
#if(LOCAL_BUF_LEN_CAN2_PROT_BASE != 0)
    if(canProtPack.packDataLen > LOCAL_BUF_LEN_CAN2_PROT_BASE)
    {
        //释放内存
        Can2ProtPortMemFree(canProtPack.packDataBufPtr);
    }
#else
    //释放内存
    Can2ProtPortMemFree(canProtPack.packDataBufPtr);
#endif
    return errorSubFunc;
}

//CAN_PORT2发送结果包,仅发送不做数据存储,使用本机模块号和板号,不需要单独设置
ERROR_SUB Can2ProtSendResultOnlySample(uint8_t targetModule,uint8_t targetBoardID,uint8_t singalChannel,uint8_t retryCount,uint32_t cmdVal,
                                                    PROT_RESULT resultCode,ERROR_MAIN errorMain,ERROR_LEVEL errorLevel,ERROR_SUB errorSub,
                                                    uint8_t* resultParamBuffer,uint16_t resultBufferLen)
{
    return Can2ProtSendResultOnly(can2ProtInitSet.selfModuleNo,can2ProtInitSet.selfBoardID,targetModule,targetBoardID,singalChannel,
                                                retryCount,cmdVal,resultCode,errorMain,errorLevel,errorSub,resultParamBuffer,resultBufferLen);
}

//CAN_PORT2发送结果ACK包
ERROR_SUB Can2ProtSendAckResult(uint8_t srcModule,uint8_t srcBoardID,uint8_t targetModule,uint8_t targetBoardID,uint8_t singalChannel,
                                           uint8_t retryCount,uint32_t cmdVal)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    CAN_PROT_PACK canProtPack;
    uint8_t sendBuffer[3] = {0};
    canProtPack.retryCount = (retryCount&0X03);
    canProtPack.singalChannel = (singalChannel&0X3F);
    canProtPack.targetBoard = (targetBoardID&0X1F);
    canProtPack.targetModule = (targetModule&0X07);
    canProtPack.srcBoardID = (srcBoardID&0X1F);
    canProtPack.srcModuleNo = (srcModule&0X07);
    canProtPack.packDataLen = 3;
    //内存申请
    canProtPack.packDataBufPtr = sendBuffer;
    //数据写入
    canProtPack.packDataBufPtr[0] = (uint8_t)(cmdVal&0xff);
    cmdVal >>= 8;
    canProtPack.packDataBufPtr[1] = (uint8_t)(cmdVal&0xff);
    cmdVal >>= 8;
    canProtPack.packDataBufPtr[2] = (uint8_t)(cmdVal&0xff);
    //数据包类型
    canProtPack.packType = PROT_PACK_TYPE_ACK_RESULT;
    errorSub = CanProtBaseSendPack(PORT_CAN2,&canProtPack);
    return errorSub;
}

//CAN_PORT2发送结果ACK包,使用本机模块号和板号,不需要单独设置
ERROR_SUB Can2ProtSendAckResultSample(uint8_t targetModule,uint8_t targetBoardID,uint8_t singalChannel,uint8_t retryCount,uint32_t cmdVal)
{
    return Can2ProtSendAckResult(can2ProtInitSet.selfModuleNo,can2ProtInitSet.selfBoardID,targetModule,targetBoardID,singalChannel,
                                                retryCount,cmdVal);
}

//CAN_PORT2发送数据上传包,仅发送不做数据存储
ERROR_SUB Can2ProtSendUploadOnly(uint8_t srcModule,uint8_t srcBoardID,uint8_t targetModule,uint8_t targetBoardID,uint8_t retryCount,uint32_t uploadDataID,
                                            uint8_t* uploadDataBuffer,uint16_t uploadDataLen)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    CAN_PROT_PACK canProtPack;
#if(LOCAL_BUF_LEN_CAN2_PROT_BASE != 0)
    uint8_t sendBuffer[LOCAL_BUF_LEN_CAN2_PROT_BASE] = {0};
#endif
    canProtPack.retryCount = (retryCount&0X03);
    canProtPack.singalChannel = 0;
    canProtPack.targetBoard = (targetBoardID&0X1F);
    canProtPack.targetModule = (targetModule&0X07);
    canProtPack.srcBoardID = (srcBoardID&0X1F);
    canProtPack.srcModuleNo = (srcModule&0X07);
    canProtPack.packDataLen = 4 + uploadDataLen;
#if(LOCAL_BUF_LEN_CAN2_PROT_BASE != 0)
    if(canProtPack.packDataLen > LOCAL_BUF_LEN_CAN2_PROT_BASE)
    {
        //内存申请
        canProtPack.packDataBufPtr = Can2ProtPortMemMallocWhileSuccess(canProtPack.packDataLen);
    }
    else
    {
        //使用栈替代
        canProtPack.packDataBufPtr = sendBuffer;
    }
#else
    //内存申请
    canProtPack.packDataBufPtr = Can2ProtPortMemMallocWhileSuccess(canProtPack.packDataLen);
#endif
    //数据写入
    canProtPack.packDataBufPtr[0] = (uint8_t)(uploadDataID&0xff);
    uploadDataID >>= 8;
    canProtPack.packDataBufPtr[1] = (uint8_t)(uploadDataID&0xff);
    uploadDataID >>= 8;
    canProtPack.packDataBufPtr[2] = (uint8_t)(uploadDataID&0xff);
    uploadDataID >>= 8;
    canProtPack.packDataBufPtr[3] = (uint8_t)(uploadDataID&0xff);
    //参数拷贝
    if(uploadDataLen > 0)
    {
        UserMemCopy(canProtPack.packDataBufPtr+4,uploadDataBuffer,uploadDataLen);
    }
    //数据包类型
    canProtPack.packType = PROT_PACK_TYPE_DATA_UPLOAD;
    //数据包发送
    errorSub = CanProtBaseSendPack(PORT_CAN2,&canProtPack);
#if(LOCAL_BUF_LEN_CAN2_PROT_BASE != 0)
    if(canProtPack.packDataLen > LOCAL_BUF_LEN_CAN2_PROT_BASE)
    {
        //释放内存
        Can2ProtPortMemFree(canProtPack.packDataBufPtr);
    }
#else
    //释放内存
    Can2ProtPortMemFree(canProtPack.packDataBufPtr);
#endif
    return errorSub;
}

//CAN_PORT2发送数据上传包,仅发送不做数据存储,使用本机模块号和板号,不需要单独设置
ERROR_SUB Can2ProtSendUploadOnlySample(uint8_t targetModule,uint8_t targetBoardID,uint8_t retryCount,uint32_t uploadDataID,
                                                    uint8_t* uploadDataBuffer,uint16_t uploadDataLen)
{
    return Can2ProtSendUploadOnly(can2ProtInitSet.selfModuleNo,can2ProtInitSet.selfBoardID,targetModule,targetBoardID,
                                                retryCount,uploadDataID,uploadDataBuffer,uploadDataLen);
}

//CAN_PORT2发送数据上传ACK包
ERROR_SUB Can2ProtSendAckUpload(uint8_t srcModule,uint8_t srcBoardID,uint8_t targetModule,uint8_t targetBoardID,uint8_t retryCount,
                                            uint32_t uploadDataID)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    CAN_PROT_PACK canProtPack;
    uint8_t sendBuffer[4] = {0};
    canProtPack.retryCount = (retryCount&0X03);
    canProtPack.singalChannel = 0;
    canProtPack.targetBoard = (targetBoardID&0X1F);
    canProtPack.targetModule = (targetModule&0X07);
    canProtPack.srcBoardID = (srcBoardID&0X1F);
    canProtPack.srcModuleNo = (srcModule&0X07);
    canProtPack.packDataLen = 4;
    //内存申请
    canProtPack.packDataBufPtr = sendBuffer;
    //数据写入
    canProtPack.packDataBufPtr[0] = (uint8_t)(uploadDataID&0xff);
    uploadDataID >>= 8;
    canProtPack.packDataBufPtr[1] = (uint8_t)(uploadDataID&0xff);
    uploadDataID >>= 8;
    canProtPack.packDataBufPtr[2] = (uint8_t)(uploadDataID&0xff);
    uploadDataID >>= 8;
    canProtPack.packDataBufPtr[3] = (uint8_t)(uploadDataID&0xff);
    //数据包类型
    canProtPack.packType = PROT_PACK_TYPE_ACK_DATA_UPLOAD;
    errorSub = CanProtBaseSendPack(PORT_CAN2,&canProtPack);
    return errorSub;
}

//CAN_PORT2发送数据上传ACK包,使用本机模块号和板号,不需要单独设置
ERROR_SUB Can2ProtSendAckUploadSample(uint8_t targetModule,uint8_t targetBoardID,uint8_t retryCount,uint32_t uploadDataID)
{
    return Can2ProtSendAckUpload(can2ProtInitSet.selfModuleNo,can2ProtInitSet.selfBoardID,targetModule,targetBoardID,
                                            retryCount,uploadDataID);
}






















