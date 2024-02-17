/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-17 16:13:03 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-09 17:54:17 +0800
************************************************************************************************/ 
#ifndef __UART_PROT_PORT_H_
#define __UART_PROT_PORT_H_
#include "DriverHeaderBoard.h"
#include "UartProtMacro.h"
#include "UartProtDataType.h"

//错误信息打印函数
#define UART_PROT_LOG(format,...)                    MCU_LogPrintf(format, ##__VA_ARGS__)  
//等待链表的容量上限
#define CAPACITY_MAX_UART_PROT_WAIT_LIST             1000
//查询指令是否完成的周期毫秒
#define TIME_MS_INTERVAL_CMD_QUERY_UART_PROT         20
//两帧数据之间的延迟时间毫秒
#define TIME_INVERVAL_MS_UART_PROT_FRAME             100
//ACK超时时间毫秒
#define TIME_OUT_MS_ACK_UART_PROT                    200
//是否支持指令,0不支持接收指令,1支持
#define UART_PROT_CMD_SUPPORT                        CONFIG_FUNC_ENABLE
//是否支持上报ID,0不支持,1支持
#define UART_PROT_UPLOAD_SUPPORT                     CONFIG_FUNC_ENABLE
//清除缓存指令的时候是否强制清除未完成的指令
#define UART_PROT_FORCE_CLEAR_NOT_COMPLETE           CONFIG_FUNC_DISABLE

/****************************************************SOCKET ALL***********************************************************/
//数据包CRC16计算
uint16_t UartProtUtilCalcCRC16(uint8_t* bufferPtr,uint16_t bufferLength);

//自身模块号
uint8_t UartProtGetSelfModuleNo(void);
//自身板号
uint8_t UartProtGetSelfBoardID(void);
//获取数据发送互斥锁
void UartProtGetMutexLock(void);
//释放数据发送互斥锁
void UartProtReleaseMutexLock(void);
//发送数据一帧
ERROR_SUB UartProtSendFrame(uint16_t msgBodyLen,uint8_t* bodyBuf);

//内存申请
void* UartProtMemMalloc(uint32_t sizeBytes);
//内存申请,必须成功
void* UartProtMemMallocWhileSuccess(uint32_t sizeBytes);
//内存释放
void UartProtMemFree(void* memPtr);

//SOCKET 日志信息处理,发送过来的数据的内存不能直接用指针,必须深拷贝,因为调用者会清除内存
void UartProtLogMsgSelfProcess(UART_PROT_LOG_PACK_RECV* logPackPtr);
//SOCKET 指令信息处理,发送过来的数据的内存不能直接用指针,必须深拷贝,因为调用者会清除内存
void UartProtCmdMsgSelfProcess(UART_PROT_CMD_PACK_RECV* cmdPackPtr);
//SOCKET 主动上传信息处理,发送过来的数据的内存不能直接用指针,必须深拷贝,因为调用者会清除内存
void UartProtUploadMsgSelfProcess(UART_PROT_UPLOAD_PACK_RECV* uploadPackPtr);


/****************************************************转发函数***********************************************************/
//SOCKET AckCmd数据转发,发送过来的数据的内存不能直接用指针,必须深拷贝,因为调用者会清除内存
ERROR_SUB UartProtAckCmdForward(UART_PROT_ACK_CMD_PACK_RECV* ackCmdPackPtr);
//SOCKET AckResult数据转发,发送过来的数据的内存不能直接用指针,必须深拷贝,因为调用者会清除内存
ERROR_SUB UartProtAckResultForward(UART_PROT_ACK_RESULT_PACK_RECV* ackResultPackPtr);
//SOCKET AckUpload数据转发,发送过来的数据的内存不能直接用指针,必须深拷贝,因为调用者会清除内存
ERROR_SUB UartProtAckUploadForward(UART_PROT_ACK_UPLOAD_PACK_RECV* ackUploadPackPtr);
//SOCKET 日志信息转发,发送过来的数据的内存不能直接用指针,必须深拷贝,因为调用者会清除内存
void UartProtLogMsgForward(UART_PROT_LOG_PACK_RECV* logPackPtr);
//SOCKET 指令信息转发,发送过来的数据的内存不能直接用指针,必须深拷贝,因为调用者会清除内存
void UartProtCmdMsgForward(UART_PROT_CMD_PACK_RECV* cmdPackPtr);
//SOCKET 结果信息转发,发送过来的数据的内存不能直接用指针,必须深拷贝,因为调用者会清除内存
void UartProtResultMsgForward(UART_PROT_RESULT_PACK_RECV* resultPackPtr);
//SOCKET 主动上传信息抓发,发送过来的数据的内存不能直接用指针,必须深拷贝,因为调用者会清除内存
void UartProtUploadMsgForward(UART_PROT_UPLOAD_PACK_RECV* uploadPackPtr);
//ACK信息转发
ERROR_SUB UartProtAckMsgDispatch(UART_PROT_ACK_PACK* ackPackPtr);

#endif











