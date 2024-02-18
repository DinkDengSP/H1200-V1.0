/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-03-09 09:53:15 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-03-09 16:57:35 +0800
************************************************************************************************/ 
#ifndef __ADT_BINOMIAL_HEAP_H_
#define __ADT_BINOMIAL_HEAP_H_
#include "ADT_Base.h"

//二项堆（binomial heap）是一种类似于二叉堆的堆结构。
//与二叉堆相比，其优势是可以快速合并两个堆，因此它属于可合并堆（mergeable heap）抽象数据类型的一种
//类型名称宏定义
#define ADT_T           ADT_BINOMIAL_HEAP_T
#define ADT_NODE_T      ADT_BINOMIAL_HEAP_NODE_T

//指定一个数据类型,****对外部文件ADT_BINOMIAL_HEAP_T数据类型是一个指向
//****ADT_BINOMIAL_HEAP_T结构体的指针(注意这句话)
typedef struct ADT_T        *ADT_T;
typedef struct ADT_NODE_T   *ADT_NODE_T;

//创建对象
ADT_T ADT_BinomialHeapCreate(BinHeapType type,ADT_SortBaseCompare sortCompareFunc,
                            ADT_MemMallocFuncPtr memMallocFunc,ADT_MemFreeFuncPtr memFreeFunc,
                            ADT_LockSystem lockSysFuncPtr,ADT_UnLockSystem unlockSysFuncPtr,
                            ADT_SendString sendStrFunc);
//销毁
void ADT_BinomialHeapDestroy(ADT_T* binomialHeapPtrPtr);

//释放全部的数据
void ADT_BinomialHeapClear(ADT_T binomialHeapPtr);

//插入节点
ADT_RESULT ADT_BinomialHeapInsert(ADT_T binomialHeapPtr, void* value);;

//弹出节点
void* ADT_BinomialHeapPop(ADT_T binomialHeapPtr,ADT_RESULT* resultPtr);

//获取节点元素数量
uint32_t ADT_BinomialHeapNumEntries(ADT_T binomialHeapPtr);

//释放宏定义,外部可以使用
#undef ADT_T
#undef ADT_NODE_T

#endif
