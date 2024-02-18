/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-03-08 14:17:55 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-03-08 15:55:57 +0800
************************************************************************************************/ 
#ifndef __ADT_SUNDAY_H_
#define __ADT_SUNDAY_H_
#include "ADT_Base.h"


//SUNDAY匹配算法,参数为查找串,查找串长度,目标串,目标串长度,匹配开始序号,存储查找结果的指针
ADT_RESULT ADT_SundayMatch(uint8_t* textStr,uint8_t* patternStr,uint32_t lengthTextStr,
                                    uint32_t lengthPatternStr,uint32_t matchStartPos,uint32_t* matchIndexPtr);



#endif
