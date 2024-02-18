/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-01-29 14:46:53 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-02-04 20:39:09 +0800
************************************************************************************************/ 
#ifndef __ADT_CIRCULAR_PTR_QUEUE_H_
#define __ADT_CIRCULAR_PTR_QUEUE_H_
#include "ADT_Base.h"

//类型名称宏定义
#define ADT_T ADT_CIRCULAR_PTR_QUEUE_T

//指定一个数据类型,****对外部文件ADT_CIRCULAR_PTR_QUEUE_T数据类型是一个指向
//****ADT_CIRCULAR_PTR_QUEUE_T结构体的指针(注意这句话)
typedef struct ADT_T *ADT_T;

//循环队列初始化
ADT_T ADT_CircularPtrQueueCreate(uint16_t bufferSize,ADT_MemMallocFuncPtr memMallocFunc,ADT_MemFreeFuncPtr memFreeFunc,
                                ADT_LockSystem lockSysFuncPtr,ADT_UnLockSystem unlockSysFuncPtr,ADT_SendString sendStrFunc);

//释放循环队列
void ADT_CircularPtrQueueFree(ADT_T* circularPtrQueuePtrPtr);

//获取队列最大容量
uint16_t ADT_CircularPtrQueueGetCapacity(ADT_T circularPtrQueuePtr);

//获取队列当前总有效数据量
uint16_t ADT_CircularPtrQueueGetCurrentSize(ADT_T circularPtrQueuePtr);

//往循环队列中写入数据
ADT_RESULT ADT_CircularPtrQueueWrite(ADT_T circularPtrQueuePtr,void* ptr);

//从循环队列中读取数据
void* ADT_CircularPtrQueueGet(ADT_T circularPtrQueuePtr);

//释放宏定义,外部可以使用
#undef ADT_T


#endif

