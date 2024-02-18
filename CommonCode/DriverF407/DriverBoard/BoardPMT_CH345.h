/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-02-24 17:32:12 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-02-27 14:47:19 +0800
************************************************************************************************/ 
#ifndef __BOARD_PMT_CH345_H_
#define __BOARD_PMT_CH345_H_
#include "DriverHeaderMCU.h"

//光子测量模块端口初始化
void BoardPMT_CH345_PortInit(void);

//将光子测量模块申请的数据释放掉,主要用在多次数据采集中
void BoardPMT_CH345_ReleaseMem(void* memPtr);

//光子测量模块配置初始化状态
ERROR_SUB BoardPMT_CH345_ConfigDefault(void);

//测量模块单次采样开始,参数为门控时间
ERROR_SUB BoardPMT_CH345_SingleSampleStart(uint16_t gateTimeMultipleMs);

//测量模块单次采样,查询是否收到结果
ERROR_SUB BoardPMT_CH345_SingleSampleCheck(uint16_t* lightPointCount);

//测量模块单次采样获取采样结果
ERROR_SUB BoardPMT_CH345_SingleSampleGetResult(uint32_t* dataResultPtr);

//测量模块循环采样开始,参数为门控时间
ERROR_SUB BoardPMT_CH345_CyclicSampleStart(uint16_t gateTimeMultipleMs);

//测量模块循环采样,数据监测,存在几个数据
ERROR_SUB BoardPMT_CH345_CyclicSampleCheckCurrentPoint(uint16_t* lightPointCount);

//测量模块循环采样结束,参数为采样点数
ERROR_SUB BoardPMT_CH345_CyclicSampleGetResultStop(uint32_t** dataResultArrayPtrPtr,uint8_t* realResultCount);

//测量模块停止
ERROR_SUB BoardPMT_CH345_Stop(void);

#endif



