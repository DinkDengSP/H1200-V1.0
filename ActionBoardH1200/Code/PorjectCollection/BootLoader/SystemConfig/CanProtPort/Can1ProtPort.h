/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-20 08:40:07 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-23 14:23:18 +0800
************************************************************************************************/ 
#ifndef __CAN1_PROT_PORT_H_
#define __CAN1_PROT_PORT_H_
#include "DriverHeaderBoard.h"
#include "CanProtMacro.h"
#include "CanProtDataType.h"

//是否支持指令,0不支持接收指令,1支持
#define CAN1_PROT_CMD_SUPPORT                       CONFIG_FUNC_ENABLE
//是否支持上报ID,0不支持,1支持
#define CAN1_PROT_UPLOAD_SUPPORT                    CONFIG_FUNC_ENABLE
//协议支持的数据长度定义
#if(MCU_CAN1_MODE_USER == MCU_CAN_MODE_CLASSICAL)
#define CAN1_PROT_DATA_BUF_LEN                      MCU_CAN_DATA_LEN_MAX_CLASSICAL
#else
#define CAN1_PROT_DATA_BUF_LEN                      MCU_CAN_DATA_LEN_MAX_FD
#endif

//单帧最大存放数据长度
#define CAN1_PROT_DATA_LEN_SINGLE_FRAME             (CAN1_PROT_DATA_BUF_LEN - 1)
//数据单包长度最大值
#define CAN1_PROT_SINGLE_PACK_DATA_MAX              4096
//两帧数据之间的延迟时间,(防止接收方顶不住)
#define TIME_INVERVAL_MS_CAN1_PROT_SEND_FRAME       0
//底层CAN数据发送使用栈提到动态内存申请的阈值(有时候发送的数据比较少,内存申请的开销划不来,直接用栈)
//注意,结果包最少十一个字节,所以想用这个功能,长度最好大于11字节,另外任务的堆栈开销就大了
#define LOCAL_BUF_LEN_CAN1_PROT_BASE                20
//ACK超时时间,正常指令
#define TIME_OUT_MS_ACK_CAN1_PROT_NORMAL            300
//ACK超时时间,透传指令
#define TIME_OUT_MS_ACK_CAN1_PROT_FORWARD           3000

/*******************************************移植接口*****************************************/
//CAN1_PROT自身模块号
uint8_t Can1ProtPortGetSelfModuleNo(void);
//CAN1_PROT自身板号
uint8_t Can1ProtPortGetSelfBoardID(void);
//获取CAN1_PROT互斥锁
void Can1ProtPortGetMutexLock(void);
//释放CAN1_PROT互斥锁
void Can1ProtPortReleaseMutexLock(void);
//发送CAN1_PROT一帧
ERROR_SUB Can1ProtPortSendFrame(uint32_t extendID,uint8_t msgBodyLen,uint8_t* bodyBuf);
//内存申请
void* Can1ProtPortMemMalloc(uint32_t sizeBytes);
//内存申请,必须成功
void* Can1ProtPortMemMallocWhileSuccess(uint32_t sizeBytes);
//内存释放
void Can1ProtPortMemFree(void* memPtr);

/*******************************************转发接口*****************************************/
//发送过来的数据的内存不能直接用指针,必须深拷贝,因为调用者会清除内存
//CAN1_PROT 指令信息转发
void Can1ProtCmdMsgForward(CAN_PROT_CMD_PACK* cmdPackPtr);
//CAN1_PROT AckCmd数据转发
ERROR_SUB Can1ProtAckCmdForward(CAN_PROT_ACK_CMD_PACK* ackCmdPackPtr);
//CAN1_PROT 结果信息转发
void Can1ProtResultMsgForward(CAN_PROT_RESULT_PACK* resultPackPtr);
//CAN1_PROT AckResult数据转发
ERROR_SUB Can1ProtAckResultForward(CAN_PROT_ACK_RESULT_PACK* ackResultPackPtr);
//CAN1_PROT 主动上传信息抓发
void Can1ProtUploadMsgForward(CAN_PROT_UPLOAD_PACK* uploadPackPtr);
//CAN1_PROT AckUpload数据转发
ERROR_SUB Can1ProtAckUploadForward(CAN_PROT_ACK_UPLOAD_PACK* ackUploadPackPtr);
//CAN1_PROT 日志信息转发
void Can1ProtLogMsgForward(CAN_PROT_LOG_PACK* logPackPtr);

/*******************************************处理接口*****************************************/
//发送过来的数据的内存不能直接用指针,必须深拷贝,因为调用者会清除内存
//CAN1_PROT 日志信息处理
void Can1ProtLogMsgSelfProc(CAN_PROT_LOG_PACK* logPackPtr);
//CAN1_PROT 指令信息处理
void Can1ProtCmdMsgSelfProc(CAN_PROT_CMD_PACK* cmdPackPtr);
//CAN1_PROT 主动上传信息处理
void Can1ProtUploadMsgSelfProc(CAN_PROT_UPLOAD_PACK* uploadPackPtr);
//ACK信息转发
ERROR_SUB Can1ProtAckMsgDispatch(CAN_PROT_ACK_PACK* ackPackPtr);

#endif












