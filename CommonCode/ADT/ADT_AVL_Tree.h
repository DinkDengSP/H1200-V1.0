#ifndef __ADT_AVL_TREE_H_
#define __ADT_AVL_TREE_H_
#include "ADT_Base.h"

//类型名称宏定义
#define ADT_T       ADT_AVL_TREE_T
#define ADT_NODE_T  ADT_AVL_TREE_NODE_T

//指定一个数据类型,****对外部文件ADT_CIRCULAR_PTR_QUEUE_T数据类型是一个指向
//****ADT_CIRCULAR_PTR_QUEUE_T结构体的指针(注意这句话)
typedef struct ADT_T        *ADT_T;
typedef struct ADT_NODE_T   *ADT_NODE_T;

//创建平衡二叉树
ADT_T ADT_AVL_TreeCreate(ADT_AVLTreeCompareFunc compFuncPtr,ADT_MemMallocFuncPtr memMallocFunc,ADT_MemFreeFuncPtr memFreeFunc,
                                ADT_LockSystem lockSysFuncPtr,ADT_UnLockSystem unlockSysFuncPtr,ADT_SendString sendStrFunc);

//清空平衡二叉树
void ADT_AVL_TreeClear(ADT_T avlTreePtr);

//销毁平衡二叉树
void ADT_AVL_TreeDestroy(ADT_T* avlTreePtrPtr);

//平衡二叉树获取元素数量
uint32_t ADT_AVL_TreeNumEntries(ADT_T avlTreePtr);

//平衡二叉树节点处的子数高度
uint32_t ADT_AVL_TreeSubTreeHeight(ADT_NODE_T avlTreeNodePtr);;

//平衡二叉树插入数据
ADT_NODE_T ADT_AVL_TreeInsert(ADT_T avlTreePtr, void* key,void* value);

//平衡二叉树移除节点
void ADT_AVL_TreeRemoveNode(ADT_T avlTreePtr, ADT_NODE_T avlTreeNodePtr);

//平衡二叉树移除指定的键对应的节点
ADT_RESULT ADT_AVL_TreeRemove(ADT_T avlTreePtr, void* key);

//平衡二叉树使用键查找节点,返回节点指针
ADT_NODE_T ADT_AVL_TreeLookupNode(ADT_T avlTreePtr, void* key);

//平衡二叉树使用键查找节点,返回节点值
void* ADT_AVL_TreeLookup(ADT_T avlTreePtr, void* key,ADT_RESULT* resultPtr);

//平衡二叉树获取根节点
ADT_NODE_T ADT_AVL_TreeRootNode(ADT_T avlTreePtr);

//平衡二叉树获取节点中包含的键
void* ADT_AVL_TreeNodeKey(ADT_NODE_T avlTreeNodePtr);

//平衡二叉树获取节点中包含的值
void* ADT_AVL_TreeNodeValue(ADT_NODE_T avlTreeNodePtr);

//平衡二叉树获取节点的子节点
ADT_NODE_T ADT_AVL_TreeNodeChild(ADT_NODE_T avlTreeNodePtr, AVLTreeNodeSide side);

//平衡二叉树查找节点的父节点
ADT_NODE_T ADT_AVL_TreeNodeParent(ADT_NODE_T avlTreeNodePtr);

//平衡二叉树数据转换为数组
void** ADT_AVL_TreeKeyValueToArray(ADT_T avlTreePtr,uint32_t* arrayLength);


//释放宏定义,外部可以使用
#undef ADT_T
#undef ADT_NODE_T

#endif

