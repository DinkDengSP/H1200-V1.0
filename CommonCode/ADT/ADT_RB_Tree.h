/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-03-07 16:37:11 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-03-08 13:56:21 +0800
************************************************************************************************/ 
#ifndef __ADT_RB_TREE_H_
#define __ADT_RB_TREE_H_
#include "ADT_Base.h"

//类型名称宏定义
#define ADT_T       ADT_RB_TREE_T
#define ADT_NODE_T  ADT_RB_TREE_NODE_T

//指定一个数据类型,****对外部文件ADT_RB_TREE_T数据类型是一个指向
//****ADT_RB_TREE_T结构体的指针(注意这句话)
typedef struct ADT_T        *ADT_T;
typedef struct ADT_NODE_T   *ADT_NODE_T;

//创建红黑二叉树
ADT_T ADT_RB_TreeCreate(ADT_AVLTreeCompareFunc compFuncPtr,ADT_MemMallocFuncPtr memMallocFunc,ADT_MemFreeFuncPtr memFreeFunc,
                                ADT_LockSystem lockSysFuncPtr,ADT_UnLockSystem unlockSysFuncPtr,ADT_SendString sendStrFunc);

//清空红黑二叉树
void ADT_RB_TreeClear(ADT_T rbTreePtr);

//红黑二叉树销毁
void ADT_RB_TreeDestroy(ADT_T* rbTreePtrPtr);

//红黑二叉树获取元素数量
int ADT_RB_TreeNumEntries(ADT_T rbTreePtr);

//红黑二叉树插入节点
ADT_NODE_T ADT_RB_TreeInsert(ADT_T rbTreePtr, void* key, void* value);

//红黑二叉树移除节点
void ADT_RB_TreeRemoveNode(ADT_T rbTreePtr, ADT_NODE_T rbTreeNodePtr);

//红黑二叉树移除节点
ADT_RESULT ADT_RB_TreeRemove(ADT_T rbTreePtr, void* key);

//红黑二叉树搜索节点
ADT_NODE_T ADT_RB_TreeLookupNode(ADT_T rbTreePtr, void* key);

//红黑二叉树搜索节点并得到节点值
void* ADT_RB_TreeLookup(ADT_T rbTreePtr, void* key,ADT_RESULT* resultPtr);

//红黑二叉树获取根节点
ADT_NODE_T ADT_RB_TreeRootNode(ADT_T rbTreePtr);

//红黑二叉树获取节点的键
void* ADT_RB_TreeNodeKey(ADT_NODE_T rbTreeNodePtr);

//红黑二叉树获取节点的值
void* ADT_RB_TreeNodeValue(ADT_NODE_T rbTreeNodePtr);

//红黑二叉树获取节点的子节点
ADT_NODE_T ADT_RB_TreeNodeChild(ADT_NODE_T rbTreeNodePtr, RBTreeNodeSide side);

//红黑二叉树获取节点的父节点
ADT_NODE_T ADT_RB_TreeNodeParent(ADT_NODE_T rbTreeNodePtr);

//红黑二叉树键转换为数组
void** ADT_RB_TreeToArray(ADT_T rbTreePtr,uint32_t* arrayLength);





//释放宏定义,外部可以使用
#undef ADT_T
#undef ADT_NODE_T

#endif
