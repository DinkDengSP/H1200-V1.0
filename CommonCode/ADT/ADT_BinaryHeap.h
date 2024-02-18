/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-03-09 09:52:40 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-03-09 16:05:03 +0800
************************************************************************************************/ 
#ifndef __ADT_BINARY_HEAP_H_
#define __ADT_BINARY_HEAP_H_
#include "ADT_Base.h"

//二叉堆是一种特殊的堆，二叉堆是完全二元树（二叉树）或者是近似完全二元树（二叉树）。
//二叉堆有两种：最大堆和最小堆。最大堆：父结点的键值总是大于或等于任何一个子节点的键值；
//最小堆：父结点的键值总是小于或等于任何一个子节点的键值。二叉堆一般用数组来表示。
//例如，根节点在数组中的位置是0，第n个位置的子节点分别在2n+1和 2n+2。因此，第0个位置的子节点在1和2，
//1的子节点在3和4。以此类推。这种存储方式便於寻找父节点和子节点。
//在二叉堆上可以进行插入节点、删除节点、取出值最小的节点、减小节点的值等基本操作。 
//形式上看，它从顶点开始，每个节点有两个子节点，每个子节点又各自有自己的两个子节点；
//数值上看，每个节点的两个子节点都比它大或和它相等
//类型名称宏定义
#define ADT_T           ADT_BINARY_HEAP_T

//指定一个数据类型,****对外部文件ADT_BINARY_HEAP_T数据类型是一个指向
//****ADT_BINARY_HEAP_T结构体的指针(注意这句话)
typedef struct ADT_T    *ADT_T;

//创建对象
ADT_T ADT_BinaryHeapCreate(BinHeapType type,ADT_SortBaseCompare sortCompareFunc,
                            ADT_MemMallocFuncPtr memMallocFunc,ADT_MemFreeFuncPtr memFreeFunc,
                            ADT_LockSystem lockSysFuncPtr,ADT_UnLockSystem unlockSysFuncPtr,
                            ADT_SendString sendStrFunc);
//销毁
void ADT_BinaryHeapDestroy(ADT_T* binaryHeapPtrPtr);

//释放全部的数据
void ADT_BinaryHeapClear(ADT_T binaryHeapPtr);

//插入节点
ADT_RESULT ADT_BinaryHeapInsert(ADT_T binaryHeapPtr, void* value);;

//弹出节点
void* ADT_BinaryHeapPop(ADT_T binaryHeapPtr,ADT_RESULT* resultPtr);

//获取节点元素数量
uint32_t ADT_BinaryHeapNumEntries(ADT_T binaryHeapPtr);

//释放宏定义,外部可以使用
#undef ADT_T

#endif
