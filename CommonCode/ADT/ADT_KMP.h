/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-03-08 14:17:14 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-03-08 14:22:03 +0800
************************************************************************************************/ 
#ifndef __ADT_KMP_H_
#define __ADT_KMP_H_
#include "ADT_Base.h"

//类型名称宏定义
#define ADT_T ADT_KMP_T

//指定一个数据类型,****对外部文件ADT_KMP_T数据类型是一个指向
//****ADT_KMP_T结构体的指针(注意这句话)
typedef struct ADT_T *ADT_T;

//创建处理对象
ADT_T ADT_KMP_Create(ADT_MemMallocFuncPtr memMallocFunc,ADT_MemFreeFuncPtr memFreeFunc,
                    ADT_LockSystem lockSysFuncPtr,ADT_UnLockSystem unlockSysFuncPtr,
                    ADT_SendString sendStrFunc);
//销毁字节栈
void ADT_KMP_Destroy(ADT_T *kmpPtrPtr);

//KMP匹配算法,参数为查找串,查找串长度,目标串,目标串长度,匹配开始序号,存储查找结果的指针
ADT_RESULT ADT_KMP_Match(ADT_T kmpPtr,uint8_t* textStr,uint8_t* patternStr,uint32_t lengthTextStr,
                                    uint32_t lengthPatternStr,uint32_t matchStartPos,uint32_t* matchIndexPtr);


//释放宏定义,外部可以使用
#undef ADT_T

#endif
