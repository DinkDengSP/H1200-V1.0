/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-02-04 23:38:44 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-02-05 00:05:12 +0800
************************************************************************************************/ 
#ifndef __ADT_BYTE_STACK_H_
#define __ADT_BYTE_STACK_H_
#include "ADT_Base.h"

//类型名称宏定义
#define ADT_T ADT_BYTE_STACK_T

//指定一个数据类型,****对外部文件ADT_CIRCULAR_PTR_QUEUE_T数据类型是一个指向
//****ADT_CIRCULAR_PTR_QUEUE_T结构体的指针(注意这句话)
typedef struct ADT_T *ADT_T;

//创建字节栈
ADT_T ADT_ByteStackCreate(uint32_t maxCapacity,ADT_MemMallocFuncPtr memMallocFunc,ADT_MemFreeFuncPtr memFreeFunc,
                                ADT_LockSystem lockSysFuncPtr,ADT_UnLockSystem unlockSysFuncPtr,ADT_SendString sendStrFunc);
//销毁字节栈
void ADT_ByteStackFree(ADT_T *byteStackPtrPtr);

//检查字节栈是否为空
ADT_BOOL ADT_ByteStackCheckIsEmpty(ADT_T byteStackPtr);

//检查字节栈是否为满
ADT_BOOL ADT_ByteStackCheckIsFull(ADT_T byteStackPtr);

//获取字节栈最大容量
uint32_t ADT_ByteStackGetMaxCapacity(ADT_T byteStackPtr);

//获取字节栈当前有效数据容量
uint32_t ADT_ByteStackGetCurrentSize(ADT_T byteStackPtr);

//数据压栈
ADT_RESULT ADT_ByteStackPush(ADT_T byteStackPtr, uint8_t pushDataByte);

//数据出栈
ADT_RESULT ADT_ByteStackPop(ADT_T byteStackPtr,uint8_t* popDataBytePtr);



//释放宏定义,外部可以使用
#undef ADT_T

#endif




