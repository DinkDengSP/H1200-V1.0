/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-02-04 23:38:59 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-02-06 16:32:45 +0800
************************************************************************************************/ 
#ifndef __ADT_PTR_STACK_H_
#define __ADT_PTR_STACK_H_
#include "ADT_Base.h"

//类型名称宏定义
#define ADT_T ADT_PTR_STACK_T

//指定一个数据类型,****对外部文件ADT_CIRCULAR_PTR_QUEUE_T数据类型是一个指向
//****ADT_CIRCULAR_PTR_QUEUE_T结构体的指针(注意这句话)
typedef struct ADT_T *ADT_T;

//创建字节栈
ADT_T ADT_PtrStackCreate(uint32_t maxCapacity,ADT_MemMallocFuncPtr memMallocFunc,ADT_MemFreeFuncPtr memFreeFunc,
                                ADT_LockSystem lockSysFuncPtr,ADT_UnLockSystem unlockSysFuncPtr,ADT_SendString sendStrFunc);
//销毁字节栈
void ADT_PtrStackFree(ADT_T *ptrStackPtrPtr);

//检查字节栈是否为空
ADT_BOOL ADT_PtrStackCheckIsEmpty(ADT_T ptrStackPtr);

//检查字节栈是否为满
ADT_BOOL ADT_PtrStackCheckIsFull(ADT_T ptrStackPtr);

//获取字节栈最大容量
uint32_t ADT_PtrStackGetMaxCapacity(ADT_T ptrStackPtr);

//获取字节栈当前有效数据容量
uint32_t ADT_PtrStackGetCurrentSize(ADT_T ptrStackPtr);

//数据压栈
ADT_RESULT ADT_PtrStackPush(ADT_T ptrStackPtr, void* pushDataPtr);

//数据出栈
ADT_RESULT ADT_PtrStackPop(ADT_T ptrStackPtr,void** popDataPtrPtr);



//释放宏定义,外部可以使用
#undef ADT_T

#endif

