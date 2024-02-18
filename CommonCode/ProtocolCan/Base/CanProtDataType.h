/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-20 18:20:58 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-20 20:54:14 +0800
************************************************************************************************/ 
#ifndef __CAN_PROT_DATA_TYPE_H_
#define __CAN_PROT_DATA_TYPE_H_
#include "CanProtMacro.h"
#include "ADT_CircularPtrList.h"

/*************************************基础接收数据包定义*************************************/
//CAN ID解析器
typedef struct CAN_PROT_ID
{
    uint8_t retrySendCount            ;//重发标记0-3
    PROT_PACK_TYPE packType           ;//包类型
    CAN_PROT_FRAME_TYPE frameType     ;//帧类型
    uint8_t singalChannel             ;//信道编号
    uint8_t targetBoardID             ;//目标板号
    uint8_t srcBoardID                ;//源板号
    uint8_t targetModuleNo            ;//目标模块号
    uint8_t srcModuleNo               ;//源模块号
}CAN_PROT_ID;

//接收起始帧的信息
typedef struct CAN_PROT_MSG_FRAME_START
{
    //预期接收字节总数
    uint16_t exceptPackAllByteLength;
    //预期每一包数据包的有效字节长度,非最后一包
    uint8_t exceptSinglePackValidByteCount;
    //预期每一包数据包的有效字节长度,最后一包
    uint8_t exceptLastPackValidByteCount;
    //预期接收的总数据包数量
    uint16_t exceptPackCountAll;
}CAN_PROT_MSG_FRAME_START;

//CAN接收缓存包
typedef struct CAN_PROT_RECV_PACK_CACHE
{
    uint8_t targetBoardID;//目标板号
    uint8_t targetModuleNo;//目标模块号
    uint8_t srcBoardID;//源板号
    uint8_t srcModuleNo;//源模块号
    uint8_t singalChannel;//信道号
    uint8_t retryCount;//重发计数
    CAN_PROT_LONG_PACK_STA longPackStage;//接收状态,预期下一包的类型
    uint32_t lastFrameRecvTimeStamp;//上一包数据的接收时间
    uint16_t exceptPackAllByteLength;//预期接收的缓存区总字节长度
    uint8_t exceptSinglePackValidByteCount;//预期接收的单包有效数据长度
    uint8_t exceptLastPackValidByteCount;//预期接收的最后一包的有效数据长度
    uint16_t exceptPackCountAll;//预期接收的数据包总数
    uint16_t currentPackAllByteLength;//当前已经接收的字节长度
    uint16_t currentPackCountAll;//当前已经接收的数据包数量
    uint8_t* recvDataBufferPtr;//接收数据的缓存
}CAN_PROT_RECV_PACK_CACHE;

//CAN接收缓存包头
typedef struct CAN_PROT_RECV_PACK_CACHE_HEADER
{
    uint8_t targetBoardID;//目标板号
    uint8_t targetModuleNo;//目标模块号
    uint8_t srcBoardID;//源板号
    uint8_t srcModuleNo;//源模块号
    uint8_t singalChannel;//信道号
}CAN_PROT_RECV_PACK_CACHE_HEADER;

/*************************************接收包类型定义***************************************************/
//日志数据包
typedef struct CAN_PROT_LOG_PACK
{
    uint8_t targetBoardID;//目标板号
    uint8_t targetModuleNo;//目标模块号
    uint8_t srcBoardID;//源板号
    uint8_t srcModuleNo;//源模块号
    uint8_t singalChannel;//信道号
    uint16_t logDataBufferLength;//日志数据长度
    uint8_t* logDataBufferPtr;//日志数据指针
}CAN_PROT_LOG_PACK;

//指令包
typedef struct CAN_PROT_CMD_PACK
{
    uint8_t targetBoardID;//目标板号
    uint8_t targetModuleNo;//目标模块号
    uint8_t srcBoardID;//源板号
    uint8_t srcModuleNo;//源模块号
    uint8_t singalChannel;//信道号
    uint8_t retryCount;//重发次数
    uint32_t cmdValue;
    uint16_t paramBufferLength;
    uint8_t* paramBufferPtr;
}CAN_PROT_CMD_PACK;

//指令ACK数据包
typedef struct CAN_PROT_ACK_CMD_PACK
{
    uint8_t targetBoardID;//目标板号
    uint8_t targetModuleNo;//目标模块号
    uint8_t srcBoardID;//源板号
    uint8_t srcModuleNo;//源模块号
    uint8_t singalChannel;//信道号
    uint8_t retryCount;//重发次数
    uint32_t ackCmdVal;
    PROT_ACK ackCode;
}CAN_PROT_ACK_CMD_PACK;

//结果数据包
typedef struct CAN_PROT_RESULT_PACK
{
    uint8_t targetBoardID;//目标板号
    uint8_t targetModuleNo;//目标模块号
    uint8_t srcBoardID;//源板号
    uint8_t srcModuleNo;//源模块号
    uint8_t singalChannel;//信道号
    uint8_t retryCount;//重发次数
    uint32_t cmdValue;//对应的指令
    PROT_RESULT resultCode;//结果码
    ERROR_MAIN errorMainCode;//错误主码
    ERROR_LEVEL errorLevelCode;//错误级别
    ERROR_SUB errorSubCode;//错误子码
    uint8_t* resultParamBufferPtr;//结果参数缓存区
    uint16_t resultParamBufferLength;//结果参数长度
}CAN_PROT_RESULT_PACK;

//结果上传的ACK
typedef struct CAN_PROT_ACK_RESULT_PACK
{
    uint8_t targetBoardID;//目标板号
    uint8_t targetModuleNo;//目标模块号
    uint8_t srcBoardID;//源板号
    uint8_t srcModuleNo;//源模块号
    uint8_t singalChannel;//信道号
    uint8_t retryCount;//重发次数
    uint32_t ackCmdVal;//ACK指令
}CAN_PROT_ACK_RESULT_PACK;

//主动上传数据包
typedef struct CAN_PROT_UPLOAD_PACK
{
    uint8_t targetBoardID;//目标板号
    uint8_t targetModuleNo;//目标模块号
    uint8_t srcBoardID;//源板号
    uint8_t srcModuleNo;//源模块号
    uint8_t singalChannel;//信道号
    uint8_t retryCount;//重发次数
    uint32_t uploadID;
    uint8_t* uploadDataBufferPtr;//结果参数缓存区
    uint16_t uploadDataBufferLength;//结果参数长度
}CAN_PROT_UPLOAD_PACK;

//上传数据ACK数据包
typedef struct CAN_PROT_ACK_UPLOAD_PACK
{
    uint8_t targetBoardID;//目标板号
    uint8_t targetModuleNo;//目标模块号
    uint8_t srcBoardID;//源板号
    uint8_t srcModuleNo;//源模块号
    uint8_t singalChannel;//信道号
    uint8_t retryCount;//重发次数
    uint32_t uploadID;//上传ID
}CAN_PROT_ACK_UPLOAD_PACK;

/*************************************系统内部等待队列数据包定义定义*************************************/
//指令等待数据缓冲
typedef struct CAN_PROT_CMD_WAIT_PACK
{
    uint8_t targetBoardID;//目标板号
    uint8_t targetModuleNo;//目标模块号
    uint8_t singalChannel;//信道号
    uint8_t retryCount;//重发次数
    uint32_t cmdValue;//指令码
    uint16_t cmdParamBufferLength;//指令参数长度
    uint8_t* cmdParamBufferPtr;//指令参数缓存
    uint32_t lastSendTimeTickMs;//上一次发送的时间
    PROT_CMD_STA cmdStage;//指令状态
    FunctionalState cmdNeedResult;//指令是否需要结果返回
    uint32_t resultTimeOutMs;//指令结果返回的超时时间
    uint16_t exceptResultLengthMin;//预期返回结果参数的最小长度 
    PROT_RESULT resultCode;//结果码
    ERROR_MAIN errorMainCode;//错误主码
    ERROR_LEVEL errorLevelCode;//错误级别
    ERROR_SUB errorSubCode;//错误子码
    uint8_t* resultParamBufferPtr;//结果参数缓存区
    uint16_t resultParamBufferLength;//结果参数长度
}CAN_PROT_CMD_WAIT_PACK;

//指令等待数据头
typedef struct CAN_PROT_CMD_WAIT_PACK_HEADER
{
    uint8_t targetBoardID;//目标板号
    uint8_t targetModuleNo;//目标模块号
    uint8_t singalChannel;//信道号
    uint32_t cmdValue;//指令码
}CAN_PROT_CMD_WAIT_PACK_HEADER;

//发送结果包并等待返回ACK的缓存数据
typedef struct CAN_PROT_RESULT_WAIT_ACK_PACK
{
    uint8_t targetBoardID;//目标板号
    uint8_t targetModuleNo;//目标模块号
    uint8_t singalChannel;//信道号
    uint8_t retryCount;//重发次数
    uint32_t cmdValue;//对应的指令
    PROT_RESULT resultCode;//结果码
    ERROR_MAIN errorMainCode;//错误主码
    ERROR_LEVEL errorLevelCode;//错误级别
    ERROR_SUB errorSubCode;//错误子码
    uint8_t* resultParamBufferPtr;//结果参数缓存区
    uint16_t resultParamBufferLength;//结果参数长度
    uint32_t lastSendTimeTickMs;//上一次发送的时间
}CAN_PROT_RESULT_WAIT_ACK_PACK;

//上送上传包并等待ACK的缓存数据
typedef struct CAN_PROT_UPLOAD_WAIT_ACK_PACK
{
    uint8_t targetBoardID;//目标板号
    uint8_t targetModuleNo;//目标模块号
    uint8_t retryCount;//重发次数
    uint32_t uploadID;
    uint8_t* uploadDataBufferPtr;//结果参数缓存区
    uint16_t uploadDataBufferLength;//结果参数长度
    uint32_t lastSendTimeTickMs;//上一次发送的时间
}CAN_PROT_UPLOAD_WAIT_ACK_PACK;

/****************************************发送数据包定义**************************************************/
//数据包发送结构体
typedef struct CAN_PROT_PACK
{
    uint8_t srcModuleNo;    //源模块号
    uint8_t srcBoardID;     //源板号
    uint8_t targetModule;   //目标模块号
    uint8_t targetBoard;    //目标板号
    PROT_PACK_TYPE packType;//包类型
    uint8_t singalChannel;  //信道编号
    uint8_t retryCount;     //重发次数
    uint16_t packDataLen;   //数据长度
    uint8_t* packDataBufPtr;//数据缓存区
}CAN_PROT_PACK;

//CAN指令发送包
typedef struct CAN_PROT_CMD_SEND
{
    uint8_t targetModule;
    uint8_t targetBoardID;
    uint8_t singalChannel;
    uint32_t cmdVal;
    uint8_t* cmdBufferPtr;
    uint16_t cmdBufferLength;
    FunctionalState cmdNeedResult;
    uint32_t resultTimeOutMs;
    uint16_t exceptResultLengthMin;
}CAN_PROT_CMD_SEND;

//CAN指令的基础配置
typedef struct CAN_PROT_CMD_CONFIG
{
    uint32_t cmdVal;                //指令
    FunctionalState cmdNeedResult;  //指令是否需要返回
    uint32_t resultTimeOutMs;       //指令超时时间
    uint16_t exceptResultLengthMin; //指令返回数据的最小长度
    uint16_t paramBufferLen;        //参数长度,如果参数是不定长的话,写0,由高层取申请
}CAN_PROT_CMD_CONFIG;

/****************************************内部缓存与其他定义定义**************************************************/
//CAN初始化信息集合
typedef struct CAN_PROT_INIT_SET
{
    //数据访问互斥信号量
    TX_MUTEX mutexSocketCanDataAccess;
    //当前模块号
    uint8_t selfModuleNo;
    //当前自身板号
    uint8_t selfBoardID;
    //接收缓存
    ADT_CIRCULAR_PTR_LIST_T listRecvLogMsgCachePtr;//日志接收缓存链表指针
    ADT_CIRCULAR_PTR_LIST_T listRecvCmdMsgCachePtr;//指令接收缓存链表指针
    ADT_CIRCULAR_PTR_LIST_T listRecvResultMsgCachePtr;//结果接收缓存链表指针
    ADT_CIRCULAR_PTR_LIST_T listRecvUploadMsgCachePtr;//上传数据接收缓存链表指针
    //发送缓存
    ADT_CIRCULAR_PTR_LIST_T listSendResultWaitAckCachePtr;//发送结果包等待ACK的缓存指针
    ADT_CIRCULAR_PTR_LIST_T listSendUploadWaitAckCachePtr;//发送上传包等待ACK的缓存指针
    ADT_CIRCULAR_PTR_LIST_T listSendCmdWaitCachePtr;//发送指令包等待ACK或者return的缓存指针
}CAN_PROT_INIT_SET;

//转发数据类型
typedef struct CAN_PROT_FWD_DAT
{
    CAN_PROT_FWD_TYPE forwardType;
    PORT_CAN portNo;
    void* forwardPackPtr;
}CAN_PROT_FWD_DAT;

//ACK发送数据类型
typedef struct CAN_PROT_ACK_PACK
{
    CAN_PROT_ACK_TYPE ackType;//ACK类型
    uint8_t targetModule;
    uint8_t targetBoardID;
    uint8_t singalChannel;
    uint8_t retryCount;
    uint32_t cmdVal;
    PROT_ACK ackCmd;
}CAN_PROT_ACK_PACK;


#endif




































