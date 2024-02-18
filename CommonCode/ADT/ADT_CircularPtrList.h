/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-03-03 11:52:01 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-03-03 16:32:24 +0800
************************************************************************************************/ 
#ifndef __ADT_CIRCULAR_PTR_LIST_H_
#define __ADT_CIRCULAR_PTR_LIST_H_
#include "ADT_Base.h"

#define ADT_T                   ADT_CIRCULAR_PTR_LIST_T
#define ADT_LIST_NODE_T         ADT_CIRCULAR_PTR_LIST_NODE_T
//指定一个数据类型,****对外部文件ADT_CIRCULAR_PTR_LIST_T数据类型是一个指向
//****ADT_CIRCULAR_PTR_LIST_T结构体的指针(注意这句话)

//的指针,不透明指针的实现,需要基础知识如下.
//C语言存在两个命名空间,所有的变量,函数,类型定义,和枚举定义共享一个名称空间
//所有的全局结构,联合和枚举标记则共享另一个名称空间
//typedef struct ADT_T *ADT_T里面的*ADT_T名称空间是类型定义
//struct ADT_T 的名称空间是全局结构,两个名称空间的名字可以一样.
//而预处理并不需要知道ADT_CIRCULAR_PTR_LIST_T的具体类型,这个具体类型是在链接阶段的时候绑定的
//这样的处理可以实现程序对外部的细节隐藏
typedef struct ADT_T *ADT_T;
typedef struct ADT_LIST_NODE_T *ADT_LIST_NODE_T;

//循环指针链表初始化
ADT_T ADT_CircularPtrListCreate(ADT_MemMallocFuncPtr memMallocFunc,ADT_MemFreeFuncPtr memFreeFunc,
                                    ADT_LockSystem lockSysFuncPtr,ADT_UnLockSystem unlockSysFuncPtr,ADT_SendString sendStrFunc);

//循环指针链表检查链表是否为空
ADT_BOOL ADT_CircularPtrListIsEmpty(ADT_T circularPtrListPtr);

//循环指针链表获取链表元素个数
uint32_t ADT_CircularPtrListGetNodeCount(ADT_T circularPtrListPtr);

//循环指针链表清空
void ADT_CircularPtrListClear(ADT_T circularPtrListPtr);

//循环指针链表销毁
void ADT_CircularPtrListDestroy(ADT_T* circularPtrListPtrPtr);

//循环指针链表查找指定元素节点
ADT_LIST_NODE_T ADT_CircularPtrListFindNodeWithData(ADT_T circularPtrListPtr,void* element,ADT_BaseElementCmp compareFuncPtr);

//循环指针链表插入元素到头节点
ADT_RESULT ADT_CircularPtrListInsertNodeFront(ADT_T circularPtrListPtr,void* element);

//循环指针链表插入元素到尾节点
ADT_RESULT ADT_CircularPtrListInsertNodeEnd(ADT_T circularPtrListPtr,void* element);

//循环指针链表插入元素到指定节点后方
ADT_RESULT ADT_CircularPtrListInsertNodeCurrent(ADT_T circularPtrListPtr,ADT_LIST_NODE_T currentNodePtr,void* element);

//顶部弹出一个元素
void* ADT_CircularPtrListPopTop(ADT_T circularPtrListPtr);

//查看顶部元素
ADT_LIST_NODE_T ADT_CircularPtrListPeekTop(ADT_T circularPtrListPtr);

//底部弹出一个元素
void* ADT_CircularPtrListPopButtom(ADT_T circularPtrListPtr);

//获取节点数据中保存的数据值
void* ADT_CircularPtrListGetNodeVal(ADT_LIST_NODE_T listNodePtr);

//循环指针链表删除头节点
ADT_RESULT ADT_CircularPtrListDeleteNodeFront(ADT_T circularPtrListPtr);

//循环指针链表删除尾节点
ADT_RESULT ADT_CircularPtrListDeleteNodeEnd(ADT_T circularPtrListPtr);

//循环指针链表删除指定节点
ADT_RESULT ADT_CircularPtrListDeleteNodeCurrent(ADT_T circularPtrListPtr,ADT_LIST_NODE_T currentNodePtr);

//节点遍历
void ADT_CircularPtrListTraverse(ADT_T circularPtrListPtr,ADT_BaseElementOperation opFuncPtr);

//节点条件遍历,满足条件的节点才会执行
void ADT_CircularPtrListTraverseCondition(ADT_T circularPtrListPtr,void* element,ADT_BaseElementCmp compareFuncPtr,
                                                ADT_BaseElementOperation opFuncPtr);

//节点条件遍历并删除节点,满足条件的节点执行指定操作并最终删除掉这个节点
void ADT_CircularPtrListTraverseConditionDelete(ADT_T circularPtrListPtr,void* element,ADT_BaseElementCmp compareFuncPtr,
                                                ADT_BaseElementOperation opFuncPtr);

//节点遍历并删除满足条件的节点
void ADT_CircularPtrListTraverseDelete(ADT_T circularPtrListPtr,ADT_BaseElementMeetCondition meetConditionFuncPtr,uint32_t utilData);

//释放宏定义,外部可以使用
#undef ADT_T
#undef ADT_LIST_NODE_T

#endif
