/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-17 12:47:08 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-17 12:47:08 +0800
************************************************************************************************/ 
#ifndef __BOARD_HP_TIME_STAMP_H_
#define __BOARD_HP_TIME_STAMP_H_
#include "DriverHeaderMCU.h"

//主板高精度时间基准数据结构,单位微秒
typedef struct BOARD_HP_TIME_STAMP
{
    uint32_t mainCount;
    uint32_t subCount;
}BOARD_HP_TIME_STAMP;

//高精度时间戳初始化
void BoardHpTimeStampInit(void);

//获取当前时间戳
void BoardHpTimeStampGetStamp(BOARD_HP_TIME_STAMP* timeStampPtr);

//复位时间戳结构体
void BoardHpTimeStampResetStampData(BOARD_HP_TIME_STAMP* timeStampPtr);

//计算两个时间戳之间的差值
int64_t BoardHpTimeStampCalcDiff(BOARD_HP_TIME_STAMP* newStampPtr,BOARD_HP_TIME_STAMP* oldStampPtr);

//拷贝时间戳让两个时间戳一致
void BoardHpTimeStampCopy(BOARD_HP_TIME_STAMP* dstPtr,BOARD_HP_TIME_STAMP* srcPtr);

#endif

