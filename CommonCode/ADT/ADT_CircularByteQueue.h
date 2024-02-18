/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-01-29 14:42:36 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-02-04 20:04:56 +0800
************************************************************************************************/ 
#ifndef __ADT_CIRCULAR_BYTE_QUEUE_H_
#define __ADT_CIRCULAR_BYTE_QUEUE_H_
#include "ADT_Base.h"

#define ADT_T ADT_CIRCULAR_BYTE_QUEUE_T
//指定一个数据类型,****对外部文件ADT_CIRCULAR_BYTE_QUEUE_T数据类型是一个指向
//****ADT_CIRCULAR_BYTE_QUEUE_T结构体的指针(注意这句话)

//的指针,不透明指针的实现,需要基础知识如下.
//C语言存在两个命名空间,所有的变量,函数,类型定义,和枚举定义共享一个名称空间
//所有的全局结构,联合和枚举标记则共享另一个名称空间
//typedef struct ADT_T *ADT_T里面的*ADT_T名称空间是类型定义
//struct ADT_T 的名称空间是全局结构,两个名称空间的名字可以一样.
//而预处理并不需要知道ADT_CIRCULAR_BYTE_QUEUE_T的具体类型,这个具体类型是在链接阶段的时候绑定的
//这样的处理可以实现程序对外部的细节隐藏
typedef struct ADT_T *ADT_T;


//循环队列初始化
ADT_T ADT_CircularByteQueueCreate(uint16_t bufferSize,ADT_MemMallocFuncPtr memMallocFunc,ADT_MemFreeFuncPtr memFreeFunc,
                                ADT_LockSystem lockSysFuncPtr,ADT_UnLockSystem unlockSysFuncPtr,ADT_SendString sendStrFunc);

//释放循环队列
void ADT_CircularByteQueueFree(ADT_T* circularByteQueuePtrPtr);

//获取循环队列最大容量
uint16_t ADT_CircularByteQueueGetMaxCapacity(ADT_T circularByteQueuePtr);

//获取当前循环队列已经存储的数据
uint16_t ADT_CircularByteQueueGetCurrentSize(ADT_T circularByteQueuePtr);

//获取当前循环队列中空数据的量
uint16_t ADT_CircularByteQueueGetCurrentEmptySize(ADT_T circularByteQueuePtr);

//往循环队列中写入指定数量数据,当空间不足的时候,会将缓冲区数据填满,并返回真实的写入量
void ADT_CircularByteQueueWriteBuffer(ADT_T circularByteQueuePtr,uint8_t* bufferWritePtr,uint16_t length,uint16_t* realWriteByteCount);

//从循环队列中读取指定数量数据,当数据不足时,已有的数据写到缓冲区,并返回真实读取到的数据
void ADT_CircularByteQueueReadBuffer(ADT_T circularByteQueuePtr,uint8_t* bufferReadPtr,uint16_t length,uint16_t* realReadByteCount);

//清空队列
void ADT_CircularByteQueueClear(ADT_T circularByteQueuePtr);


//释放宏定义,外部可以使用
#undef ADT_T

#endif


