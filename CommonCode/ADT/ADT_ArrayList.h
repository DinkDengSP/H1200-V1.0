/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-03-09 09:52:07 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-03-09 14:42:13 +0800
************************************************************************************************/ 
#ifndef __ADT_ARRAY_LIST_H_
#define __ADT_ARRAY_LIST_H_
#include "ADT_Base.h"

//类型名称宏定义
#define ADT_T               ADT_ARRAY_LIST_T

//指定一个数据类型,****对外部文件ADT_ARRAY_LIST_T数据类型是一个指向
//****ADT_ARRAY_LIST_T结构体的指针(注意这句话)
typedef struct ADT_T                *ADT_T;

//创建对象
ADT_T ADT_ArrayListCreate(uint32_t initLength,ADT_SortBaseCompare sortCompareFunc,
                            ADT_MemMallocFuncPtr memMallocFunc,ADT_MemFreeFuncPtr memFreeFunc,
                            ADT_LockSystem lockSysFuncPtr,ADT_UnLockSystem unlockSysFuncPtr,
                            ADT_SendString sendStrFunc);
//销毁
void ADT_ArrayListDestroy(ADT_T* ptrListPtrPtr);

//释放全部的数据
void ADT_ArrayListClear(ADT_T ptrListPtr);

//获取指定位置元素
void* ADT_ArrayListGetIndex(ADT_T arrayListPtr,uint32_t index,ADT_RESULT* resultPtr);

//尾部增加元素
ADT_RESULT ADT_ArrayListAppend(ADT_T arrayListPtr, void* data);

//头部增加元素
ADT_RESULT ADT_ArrayListPrepend(ADT_T arrayListPtr, void* data);

//移除指定位置元素
ADT_RESULT ADT_ArrayListRemove(ADT_T arrayListPtr, uint32_t index);

//移除指定区域元素
ADT_RESULT ADT_ArrayListRemoveRange(ADT_T arrayListPtr, uint32_t index,uint32_t length);

//指定位置插入元素
ADT_RESULT ADT_ArrayListInsert(ADT_T arrayListPtr, uint32_t index,void* data);

//查找指定元素位置
ADT_RESULT ADT_ArrayListIndexOf(ADT_T arrayListPtr,void* data,uint32_t* indexPtr);

//元素排序
ADT_RESULT ADT_ArrayListSort(ADT_T arrayListPtr);


//释放宏定义,外部可以使用
#undef ADT_T
#undef ADT_NODE_T

#endif
