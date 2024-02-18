/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-03-06 14:43:49 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-03-07 12:16:50 +0800
************************************************************************************************/ 
#ifndef __ADT_PTR_RING_H_
#define __ADT_PTR_RING_H_
#include "ADT_Base.h"

#define ADT_T                   ADT_PTR_RING_T
#define ADT_NODE_T              ADT_PTR_RING_NODE_T
//指定一个数据类型,****对外部文件ADT_PTR_RING_T数据类型是一个指向
//****ADT_PTR_RING_T结构体的指针(注意这句话)

//的指针,不透明指针的实现,需要基础知识如下.
//C语言存在两个命名空间,所有的变量,函数,类型定义,和枚举定义共享一个名称空间
//所有的全局结构,联合和枚举标记则共享另一个名称空间
//typedef struct ADT_T *ADT_T里面的*ADT_T名称空间是类型定义
//struct ADT_T 的名称空间是全局结构,两个名称空间的名字可以一样.
//而预处理并不需要知道ADT_PTR_RING_T的具体类型,这个具体类型是在链接阶段的时候绑定的
//这样的处理可以实现程序对外部的细节隐藏
typedef struct ADT_T *ADT_T;
typedef struct ADT_NODE_T *ADT_NODE_T;

//注意,环的正向序号从1开始,反向序号从0开始

//循环指针链表初始化
ADT_T ADT_PtrRingCreate(ADT_MemMallocFuncPtr memMallocFunc,ADT_MemFreeFuncPtr memFreeFunc,
                                    ADT_LockSystem lockSysFuncPtr,ADT_UnLockSystem unlockSysFuncPtr,ADT_SendString sendStrFunc);

//循环指针链表检查链表是否为空
ADT_BOOL ADT_PtrRingIsEmpty(ADT_T ptrRingPtr);

//循环指针链表获取链表元素个数
uint32_t ADT_PtrRingGetNodeCount(ADT_T ptrRingPtr);

//循环指针链表清空
void ADT_PtrRingClear(ADT_T ptrRingPtr);

//循环指针链表销毁
void ADT_PtrRingDestroy(ADT_T* ptrRingPtrPtr);

//获取指定节点元素
void* ADT_PtrRingGetIndex(ADT_T ptrRingPtr,uint32_t index,ADT_RESULT* resultPtr);

//循环链表修改指定位置节点数据
void* ADT_PtrRingPutIndex(ADT_T ptrRingPtr,uint32_t index,void* element,ADT_RESULT* resultPtr);

//循环指针链表插入元素到头节点
ADT_RESULT ADT_PtrRingAddHi(ADT_T ptrRingPtr,void* element);

//循环指针链表插入元素到尾节点
ADT_RESULT ADT_PtrRingAddLo(ADT_T ptrRingPtr,void* element);

//循环指针链表插入元素到指定节点后方
ADT_RESULT ADT_PtrRingAddIndex(ADT_T ptrRingPtr,int32_t index,void* element);

//弹出指定位置元素
void* ADT_PtrRingRemoveIndex(ADT_T ptrRingPtr,uint32_t index);

//从HI处移除一个元素
void* ADT_PtrRingRemoveHi(ADT_T ptrRingPtr);

//底部弹出一个元素
void* ADT_PtrRingRemoveLo(ADT_T ptrRingPtr);

//环移动指定元素
ADT_RESULT ADT_PtrRingRotate(ADT_T ptrRingPtr,int32_t rotatePos);


//释放宏定义,外部可以使用
#undef ADT_T
#undef ADT_NODE_T

#endif
