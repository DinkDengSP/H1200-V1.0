/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-03-04 12:33:07 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-03-04 13:36:15 +0800
************************************************************************************************/ 
#ifndef __ADT_PTR_LIST_H_
#define __ADT_PTR_LIST_H_
#include "ADT_Base.h"

//类型名称宏定义
#define ADT_T               ADT_PTR_LIST_T
#define ADT_LIST_NODE_T     ADT_PTR_LIST_NODE_T

//指定一个数据类型,****对外部文件ADT_CIRCULAR_PTR_QUEUE_T数据类型是一个指向
//****ADT_CIRCULAR_PTR_QUEUE_T结构体的指针(注意这句话)
typedef struct ADT_T *ADT_T;
typedef struct ADT_LIST_NODE_T *ADT_LIST_NODE_T;

//创建对象
ADT_T ADT_PtrListCreate(ADT_MemMallocFuncPtr memMallocFunc,ADT_MemFreeFuncPtr memFreeFunc,
                                ADT_LockSystem lockSysFuncPtr,ADT_UnLockSystem unlockSysFuncPtr,ADT_SendString sendStrFunc);
//销毁指针链表
void ADT_PtrListDestroy(ADT_T* ptrListPtrPtr);

//释放全部的指针链表数据
void ADT_PtrListClear(ADT_T ptrListPtr);

//创建一个当前链表的拷贝
ADT_T ADT_PtrListCopy(ADT_T ptrListPtr);

//将两个链表连接起来,并且释放后一个链表
ADT_RESULT ADT_PtrListAppendFreeTail(ADT_T ptrListPtr,ADT_T* ptrListTailPtrPtr);

//压入一个链表元素
ADT_RESULT ADT_PtrListPush(ADT_T ptrListPtr,void* element);

//弹出一个链表元素
void* ADT_PtrListPop(ADT_T ptrListPtr);

//链表反转
ADT_T ADT_PtrListReverse(ADT_T ptrListPtr);

//节点遍历
void ADT_PtrListTraverse(ADT_T ptrListPtr,ADT_AdvanceElementOperation opFuncPtr,void* paramPtr);

//获取指针链表当前有效数据容量
uint32_t ADT_PtrListGetCurrentSize(ADT_T ptrListPtr);

//链表转换为指针数组
void** ADT_PtrListToArray(ADT_T ptrListPtr,uint16_t* elementLength);

//释放指针数组
void ADT_PtrListFreeArray(ADT_T ptrListPtr,void* arrayPtr);



//释放宏定义,外部可以使用
#undef ADT_T
#undef ADT_LIST_NODE_T

#endif


