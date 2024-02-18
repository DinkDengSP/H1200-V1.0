/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-03-06 14:42:02 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-03-06 15:07:38 +0800
************************************************************************************************/ 
#ifndef __ADT_DYNAMIC_ARRAY_H_
#define __ADT_DYNAMIC_ARRAY_H_
#include "ADT_Base.h"

#define ADT_T ADT_DYNAMIC_ARRAY_T
//指定一个数据类型,****对外部文件ADT_DYNAMIC_ARRAY_T数据类型是一个指向
//****ADT_DYNAMIC_ARRAY_T结构体的指针(注意这句话)

//的指针,不透明指针的实现,需要基础知识如下.
//C语言存在两个命名空间,所有的变量,函数,类型定义,和枚举定义共享一个名称空间
//所有的全局结构,联合和枚举标记则共享另一个名称空间
//typedef struct ADT_T *ADT_T里面的*ADT_T名称空间是类型定义
//struct ADT_T 的名称空间是全局结构,两个名称空间的名字可以一样.
//而预处理并不需要知道ADT_DYNAMIC_ARRAY_T的具体类型,这个具体类型是在链接阶段的时候绑定的
//这样的处理可以实现程序对外部的细节隐藏
typedef struct ADT_T *ADT_T;

//动态数组初始化
ADT_T ADT_DynamicArrayCreate(uint32_t initCapacity,uint32_t elementSize,ADT_MemMallocFuncPtr memMallocFunc,ADT_MemFreeFuncPtr memFreeFunc,
                                ADT_LockSystem lockSysFuncPtr,ADT_UnLockSystem unlockSysFuncPtr,ADT_SendString sendStrFunc);

//清空动态数组内部数据
ADT_T ADT_DynamicArrayClear(ADT_T arrayPtr);

//销毁动态数组结构
void ADT_DynamicArrayDestroy(ADT_T* arrayPtrPtr);

//获取动态数组中元素的数量
uint32_t ADT_DynamicArrayGetElementCapacity(ADT_T arrayPtr);

//获取动态数组中的单个元素的尺寸
uint32_t ADT_DynamicArrayGetSingleElementSize(ADT_T arrayPtr);

//获取数组指定元素
void* ADT_DynamicArrayGet(ADT_T arrayPtr,uint32_t index,ADT_RESULT* resultPtr);

//压入指定数组元素
ADT_RESULT ADT_DynamicArrayPut(ADT_T arrayPtr,uint32_t index,void* element);

//重新调整尺寸
ADT_RESULT ADT_DynamicArrayResize(ADT_T arrayPtr,uint32_t newCapacity);

//创建拷贝
ADT_T ADT_DynamicArrayCopy(ADT_T arrayPtr,uint32_t copyLength);

#undef ADT_T

#endif



