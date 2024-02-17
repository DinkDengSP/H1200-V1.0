/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-03 16:12:17 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-08 09:34:52 +0800
************************************************************************************************/ 
#ifndef __BOARD_EC20_UTIL_H_
#define __BOARD_EC20_UTIL_H_
#include "BoardEC20_Config.h"

//获取锁定
void BoardEC20UtilGetLock(void);
//释放锁定
void BoardEC20UtilReleaseLock(void);
//清理接收缓存
void BoardEC20UtilClearRecvCache(void);
//EC20等待执行时间的返回并检查返回是否包含指定数据
ERROR_SUB BoardEC20UtilWaitResponse(uint32_t timeOutMs);
//EC20检查串口返回字符串中是否包含指定的字串
ERROR_SUB BoardEC20UtilSubStrContain(uint8_t* subStr,uint32_t* matchIndexPtr);
//EC20发送字符串并检测指定返回
ERROR_SUB BoardEC20UtilSendCmdWaitResult(uint8_t* commandStr,uint8_t* checkSubStr,
                                            uint16_t waitTimeMsMax,uint8_t retryCountMax);
//数据拷贝,不包含尾指针的字符
void BoardEC20UtilMemCopyWithPtr(uint8_t* dstMem,char* startPtr,char* endPtr,uint16_t maxSize);
//查找子串序号
ERROR_SUB BoardEC20UtilFindSubStrIndex(uint8_t* srcString,uint16_t srcStringLength,
                                        uint8_t* subString,uint32_t* matchIndexPtr);

#endif



