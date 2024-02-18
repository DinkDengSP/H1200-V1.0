/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-03-07 16:18:53 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-03-07 19:32:13 +0800
************************************************************************************************/ 
#include "ADT_AVL_Tree.h"

//宏定义结构体类型,这种定义的好处是不用出现大量的各种类型名,代码整洁
//理解上差点意思,但是使用者不需要太了解内部结构
#define ADT_T                               ADT_AVL_TREE_T
#define ADT_NODE_T                          ADT_AVL_TREE_NODE_T
//结构体的类型ID,用于程序运行时的检查
#define ADT_AVL_TREE_T_ID                   0X2020071C

//节点讯息
struct ADT_NODE_T
{
    void* key;//节点的键
    void* value;//节点的值
    struct ADT_NODE_T* children[2];//子节点
    struct ADT_NODE_T* parent;//父节点
    uint32_t height;//树高度
};

//结构体定义
struct ADT_T 
{
    //函数指针
    ADT_MemMallocFuncPtr memMallocFunc;
    ADT_MemFreeFuncPtr memFreeFunc;
    ADT_LockSystem sysLockFunc;
    ADT_UnLockSystem sysUnlockFunc;
    ADT_SendString sendStrFunc;
    //识别ID
    uint32_t adtTypeID;
    //键比较函数
    ADT_AVLTreeCompareFunc compFunc;
    //当前有效数据长度
    uint32_t currentSize;       
    //根节点
    struct ADT_NODE_T* rootNodePtr;
};


//创建平衡二叉树
ADT_T ADT_AVL_TreeCreate(ADT_AVLTreeCompareFunc compFuncPtr,ADT_MemMallocFuncPtr memMallocFunc,ADT_MemFreeFuncPtr memFreeFunc,
                                ADT_LockSystem lockSysFuncPtr,ADT_UnLockSystem unlockSysFuncPtr,ADT_SendString sendStrFunc)
{
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //回调函数检查
    ADT_CALL_BACK_CHECK(ADT_AVL_TreeCreate,memMallocFunc,memFreeFunc,lockSysFuncPtr,unlockSysFuncPtr,sendStrFunc);
    //生成变量
    ADT_T avlTreePtr = NULL;
    if(compFuncPtr == NULL)
    {
        return NULL;
    }
    //内存申请
    ADT_MALLOC_SELF(avlTreePtr,memMallocFunc);
    //函数映射
    ADT_CALL_BACK_REGISTER(avlTreePtr,memMallocFunc,memFreeFunc,lockSysFuncPtr,unlockSysFuncPtr,sendStrFunc);
    //系统锁定
    ADT_SYS_LOCK(avlTreePtr);
    //数据复位
    avlTreePtr->currentSize = 0;
    //创建头节点
    avlTreePtr->rootNodePtr = NULL;
    //绑定比较函数
    avlTreePtr->compFunc = compFuncPtr;
    //功能辅助
    ADT_SET_IDENTIFY_CODE_SET(avlTreePtr,ADT_AVL_TREE_T_ID);
    //系统解锁
    ADT_SYS_UNLOCK(avlTreePtr);
    //返回结构体指针
    return avlTreePtr;
}

//释放下级节点,递归调用
static void ADT_AVL_TreeFreeSubTree(ADT_T avlTreePtr, ADT_NODE_T avlTreeNodePtr)
{
	if (avlTreeNodePtr == NULL) 
    {
		return;
	}
    //释放子节点
	ADT_AVL_TreeFreeSubTree(avlTreePtr, avlTreeNodePtr->children[AVL_TREE_NODE_LEFT]);
	ADT_AVL_TreeFreeSubTree(avlTreePtr, avlTreeNodePtr->children[AVL_TREE_NODE_RIGHT]);
    //释放当前节点
	ADT_MEM_FREE(avlTreePtr,avlTreeNodePtr);
}

//清空平衡二叉树
void ADT_AVL_TreeClear(ADT_T avlTreePtr)
{
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //指针检查
    ADT_BASE_PTR_CHECK(avlTreePtr,ADT_AVL_TreeClear);
    if(avlTreePtr->currentSize == 0)
    {
        avlTreePtr->rootNodePtr = NULL;
        //空链表无操作
        return;
    }
    //ID检查
    ADT_BASE_ID_CHECK(avlTreePtr,ADT_AVL_TreeClear,ADT_AVL_TREE_T_ID);
    //系统锁定
    ADT_SYS_LOCK(avlTreePtr);
    //递归释放
    ADT_AVL_TreeFreeSubTree(avlTreePtr,avlTreePtr->rootNodePtr);
    //元素数量归零
    avlTreePtr->rootNodePtr = NULL;
    avlTreePtr->currentSize = 0;
    //系统解锁
    ADT_SYS_UNLOCK(avlTreePtr);
    return;
}

//销毁平衡二叉树
void ADT_AVL_TreeDestroy(ADT_T* avlTreePtrPtr)
{
    //指针检查
    ADT_BASE_PTR_CHECK(avlTreePtrPtr,ADT_AVL_TreeDestroy);
    //定义实体
    ADT_T avlTreePtr = (*avlTreePtrPtr);
    //指针检查
    ADT_BASE_PTR_CHECK(avlTreePtr,ADT_AVL_TreeDestroy);
    //ID检查
    ADT_BASE_ID_CHECK(avlTreePtr,ADT_AVL_TreeDestroy,ADT_AVL_TREE_T_ID);
    //清空数据
    ADT_AVL_TreeClear(avlTreePtr);
    //释放自身
    ADT_FREE_SELF(avlTreePtr);
    return;
}

//平衡二叉树获取元素数量
uint32_t ADT_AVL_TreeNumEntries(ADT_T avlTreePtr)
{
    //指针检查
    ADT_BASE_PTR_CHECK(avlTreePtr,ADT_AVL_TreeNumEntries);
    //ID检查
    ADT_BASE_ID_CHECK(avlTreePtr,ADT_AVL_TreeNumEntries,ADT_AVL_TREE_T_ID);
    return avlTreePtr->currentSize;
}

//平衡二叉树节点处的子树高度
uint32_t ADT_AVL_TreeSubTreeHeight(ADT_NODE_T avlTreeNodePtr)
{
    if(avlTreeNodePtr == NULL) 
    {
		return 0;
	} 
    else 
    {
		return avlTreeNodePtr->height;
	}
}

//更新平衡二叉树的节点高度
static void ADT_AVL_TreeUpdateHeight(ADT_NODE_T avlTreeNodePtr)
{
	ADT_NODE_T left_subtree;
	ADT_NODE_T right_subtree;
	int left_height, right_height;
    //子节点获取
	left_subtree = avlTreeNodePtr->children[AVL_TREE_NODE_LEFT];
	right_subtree = avlTreeNodePtr->children[AVL_TREE_NODE_RIGHT];
    //获取左右的子树高度
	left_height = ADT_AVL_TreeSubTreeHeight(left_subtree);
	right_height = ADT_AVL_TreeSubTreeHeight(right_subtree);
    //左右不等选高的
	if (left_height > right_height) 
    {
		avlTreeNodePtr->height = left_height + 1;
	} 
    else 
    {
		avlTreeNodePtr->height = right_height + 1;
	}
}

//获取当前节点是父节点的左子节点还是右子节点
static AVLTreeNodeSide ADT_AVL_TreeNodeParentSide(ADT_NODE_T avlTreeNodePtr)
{
	if (avlTreeNodePtr->parent->children[AVL_TREE_NODE_LEFT] == avlTreeNodePtr) 
    {
		return AVL_TREE_NODE_LEFT;
	} 
    else 
    {
		return AVL_TREE_NODE_RIGHT;
	}
}

//交换两个节点到各自的父节点下
static void ADT_AVL_TreeNodeReplace(ADT_T avlTreePtr, ADT_NODE_T node1Ptr,ADT_NODE_T node2Ptr)
{
	AVLTreeNodeSide side;
	if (node2Ptr != NULL) 
    {
        //节点2不为空的情况下,节点2的父节点等于节点1的父节点
		node2Ptr->parent = node1Ptr->parent;
	}
	if (node1Ptr->parent == NULL) 
    {
        //节点1的父节点为空的情况下,节点1就是根节点,那么设置树的根节点为节点2
		avlTreePtr->rootNodePtr = node2Ptr;
	} 
    else 
    {
        //节点1不是根节点,获取节点1是父节点的左节点还是右节点
		side = ADT_AVL_TreeNodeParentSide(node1Ptr);
        //节点1的父节点的对应方向子节点设置为节点2
		node1Ptr->parent->children[side] = node2Ptr;
        //根据新的子节点更新父节点的高度
		ADT_AVL_TreeUpdateHeight(node1Ptr->parent);
	}
}

/* 二叉树旋转,旋转模式如下
 *
 * Left rotation:              Right rotation:
 *
 *      B                             D
 *     / \                           / \
 *    A   D                         B   E
 *       / \                       / \
 *      C   E                     A   C

 * is rotated to:              is rotated to:
 *
 *        D                           B
 *       / \                         / \
 *      B   E                       A   D
 *     / \                             / \
 *    A   C                           C   E
 */

static ADT_NODE_T ADT_AVL_TreeRotate(ADT_T avlTreePtr, ADT_NODE_T nodePtr,AVLTreeNodeSide direction)
{
	ADT_NODE_T newRootPtr;
    //获取节点的子节点,左旋则获取右子,右旋则获取左子
	newRootPtr = nodePtr->children[1-direction];
    //对节点进行交换
	ADT_AVL_TreeNodeReplace(avlTreePtr, nodePtr, newRootPtr);
    //指针重定向
	nodePtr->children[1-direction] = newRootPtr->children[direction];
	newRootPtr->children[direction] = nodePtr;
    //更新父节点
	nodePtr->parent = newRootPtr;
    //如果存在子节点,那么更新子节点的父节点
	if (nodePtr->children[1-direction] != NULL) 
    {
		nodePtr->children[1-direction]->parent = nodePtr;
	}
	//更新受影响的节点深度
	ADT_AVL_TreeUpdateHeight(newRootPtr);
	ADT_AVL_TreeUpdateHeight(nodePtr);
	return newRootPtr;
}

//对二叉树的节点进行平衡
static ADT_NODE_T ADT_AVL_TreeNodeBalance(ADT_T avlTreePtr, ADT_NODE_T nodePtr)
{
	ADT_NODE_T left_subtree;
	ADT_NODE_T right_subtree;
	ADT_NODE_T child;
	int diff;
    //获取节点的左右子树
	left_subtree = nodePtr->children[AVL_TREE_NODE_LEFT];
	right_subtree = nodePtr->children[AVL_TREE_NODE_RIGHT];
    //检查左右子树高度,是否需要平衡话
	diff = ADT_AVL_TreeSubTreeHeight(right_subtree)- ADT_AVL_TreeSubTreeHeight(left_subtree);
	if (diff >= 2) 
    {
		child = right_subtree;
		if (ADT_AVL_TreeSubTreeHeight(child->children[AVL_TREE_NODE_RIGHT]) < ADT_AVL_TreeSubTreeHeight(child->children[AVL_TREE_NODE_LEFT])) 
        {
			//树旋转
			ADT_AVL_TreeRotate(avlTreePtr, right_subtree,AVL_TREE_NODE_RIGHT);
		}
		nodePtr = ADT_AVL_TreeRotate(avlTreePtr, nodePtr, AVL_TREE_NODE_LEFT);

	} 
    else if (diff <= -2) 
    {
		child = nodePtr->children[AVL_TREE_NODE_LEFT];
		if (ADT_AVL_TreeSubTreeHeight(child->children[AVL_TREE_NODE_LEFT])< ADT_AVL_TreeSubTreeHeight(child->children[AVL_TREE_NODE_RIGHT])) 
        {
			ADT_AVL_TreeRotate(avlTreePtr, left_subtree,AVL_TREE_NODE_LEFT);
		}
		nodePtr = ADT_AVL_TreeRotate(avlTreePtr, nodePtr, AVL_TREE_NODE_RIGHT);
	}
    //更新平衡之后受影响的节点高度
	ADT_AVL_TreeUpdateHeight(nodePtr);
	return nodePtr;
}

//对节点平衡化,一直到根节点停止
static void ADT_AVL_TreeBalanceToRoot(ADT_T avlTreePtr, ADT_NODE_T nodePtr)
{
	ADT_NODE_T rover;
	rover = nodePtr;
	while (rover != NULL) 
    {
		rover = ADT_AVL_TreeNodeBalance(avlTreePtr, rover);
        //回到父节点
		rover = rover->parent;
	}
}

//平衡二叉树插入数据
ADT_NODE_T ADT_AVL_TreeInsert(ADT_T avlTreePtr, void* key,void* value)
{
    ADT_NODE_T *rover;
	ADT_NODE_T new_nodePtr;
	ADT_NODE_T previous_node;
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //指针检查
    ADT_BASE_PTR_CHECK(avlTreePtr,ADT_AVL_TreeInsert);
    //ID检查
    ADT_BASE_ID_CHECK(avlTreePtr,ADT_AVL_TreeInsert,ADT_AVL_TREE_T_ID);
    //不能是空键
    if(key == NULL)
    {
        return NULL;
    }
    //系统锁定
    ADT_SYS_LOCK(avlTreePtr);
    rover = &(avlTreePtr->rootNodePtr);
	previous_node = NULL;
	while (*rover != NULL) 
    {
		previous_node = *rover;
		if (avlTreePtr->compFunc(key, (*rover)->key) < 0) 
        {
			rover = &((*rover)->children[AVL_TREE_NODE_LEFT]);
		} 
        else 
        {
			rover = &((*rover)->children[AVL_TREE_NODE_RIGHT]);
		}
	}
    //创建新节点
	new_nodePtr = ADT_MEM_MALLOC(avlTreePtr,sizeof(*new_nodePtr));
    //内存申请失败
	if (new_nodePtr == NULL) 
    {
        //系统解锁
        ADT_SYS_UNLOCK(avlTreePtr);
		return NULL;
	}
    //配置节点讯息
	new_nodePtr->children[AVL_TREE_NODE_LEFT] = NULL;
	new_nodePtr->children[AVL_TREE_NODE_RIGHT] = NULL;
	new_nodePtr->parent = previous_node;
	new_nodePtr->key = key;
	new_nodePtr->value = value;
	new_nodePtr->height = 1;
    //插入节点
	*rover = new_nodePtr;
    //节点平衡话
	ADT_AVL_TreeBalanceToRoot(avlTreePtr, previous_node);
    //节点数量增加
	++(avlTreePtr->currentSize);
    //系统解锁
    ADT_SYS_UNLOCK(avlTreePtr);
    //返回新的节点指针
	return new_nodePtr;
}

//查找与给定节点距离最近的节点
static ADT_NODE_T ADT_AVL_TreeNodeGetReplacement(ADT_T avlTreePtr,ADT_NODE_T nodePtr)
{
	ADT_NODE_T left_subtree;
	ADT_NODE_T right_subtree;
	ADT_NODE_T result;
	ADT_NODE_T child;
	int left_height, right_height;
	int side;
    //子树获取
	left_subtree = nodePtr->children[AVL_TREE_NODE_LEFT];
	right_subtree = nodePtr->children[AVL_TREE_NODE_RIGHT];
    //是否没有子树
	if (left_subtree == NULL && right_subtree == NULL) 
    {
		return NULL;
	}
    //获取子树高度
	left_height = ADT_AVL_TreeSubTreeHeight(left_subtree);
	right_height = ADT_AVL_TreeSubTreeHeight(right_subtree);
    //根据高度决定要放哪边
	if (left_height < right_height) 
    {
		side = AVL_TREE_NODE_RIGHT;
	} 
    else 
    {
		side = AVL_TREE_NODE_LEFT;
	}
    //开始查找
	result = nodePtr->children[side];
	while (result->children[1-side] != NULL) 
    {
		result = result->children[1-side];
	}
    //节点交换
	child = result->children[side];
	ADT_AVL_TreeNodeReplace(avlTreePtr, result, child);
    //更新节点高度
	ADT_AVL_TreeUpdateHeight(result->parent);
    //回传找到的结果
	return result;
}

//平衡二叉树移除节点
void ADT_AVL_TreeRemoveNode(ADT_T avlTreePtr, ADT_NODE_T avlTreeNodePtr)
{
    ADT_NODE_T swap_node;
	ADT_NODE_T balance_startpoint;
	int i;
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //指针检查
    ADT_BASE_PTR_CHECK(avlTreePtr,ADT_AVL_TreeRemoveNode);
    //ID检查
    ADT_BASE_ID_CHECK(avlTreePtr,ADT_AVL_TreeRemoveNode,ADT_AVL_TREE_T_ID);
    //不能是空键
    if(avlTreeNodePtr == NULL)
    {
        return;
    }
    //系统锁定
    ADT_SYS_LOCK(avlTreePtr);
    //先找出交换节点
	swap_node = ADT_AVL_TreeNodeGetReplacement(avlTreePtr, avlTreeNodePtr);
    //查看是否找到
	if (swap_node == NULL) 
    {
		ADT_AVL_TreeNodeReplace(avlTreePtr, avlTreeNodePtr, NULL);
		balance_startpoint = avlTreeNodePtr->parent;

	} 
    else 
    {
		if (swap_node->parent == avlTreeNodePtr) 
        {
			balance_startpoint = swap_node;
		} 
        else 
        {
			balance_startpoint = swap_node->parent;
		}
		for (i=0; i<2; ++i) 
        {
			swap_node->children[i] = avlTreeNodePtr->children[i];
			if (swap_node->children[i] != NULL) 
            {
				swap_node->children[i]->parent = swap_node;
			}
		}
		swap_node->height = avlTreeNodePtr->height;
		ADT_AVL_TreeNodeReplace(avlTreePtr, avlTreeNodePtr, swap_node);
	}
    //销毁节点
    ADT_MEM_FREE(avlTreePtr,avlTreeNodePtr);
    //节点数量减少
	--(avlTreePtr->currentSize);
    //平衡化
	ADT_AVL_TreeBalanceToRoot(avlTreePtr, balance_startpoint);
    //系统解锁
    ADT_SYS_UNLOCK(avlTreePtr);
	return;
}

//平衡二叉树移除指定的键对应的节点
ADT_RESULT ADT_AVL_TreeRemove(ADT_T avlTreePtr, void* key)
{
    ADT_NODE_T node;
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //指针检查
    ADT_BASE_PTR_CHECK(avlTreePtr,ADT_AVL_TreeRemove);
    //ID检查
    ADT_BASE_ID_CHECK(avlTreePtr,ADT_AVL_TreeRemove,ADT_AVL_TREE_T_ID);
    //不能是空键
    if(key == NULL)
    {
        return ADT_RESULT_FAIL;
    }
    //系统锁定
    ADT_SYS_LOCK(avlTreePtr);
    //查找结点
	node = ADT_AVL_TreeLookupNode(avlTreePtr, key);
	if (node == NULL) 
    {
        //系统解锁
        ADT_SYS_UNLOCK(avlTreePtr);
		return ADT_RESULT_FAIL;
	}
    //找到了,移除节点
	ADT_AVL_TreeRemoveNode(avlTreePtr, node);
    //系统解锁
    ADT_SYS_UNLOCK(avlTreePtr);
	return ADT_RESULT_OK;
}

//平衡二叉树使用键查找节点,返回节点指针
ADT_NODE_T ADT_AVL_TreeLookupNode(ADT_T avlTreePtr, void* key)
{
    ADT_NODE_T node;
	int diff;
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //指针检查
    ADT_BASE_PTR_CHECK(avlTreePtr,ADT_AVL_TreeLookupNode);
    //ID检查
    ADT_BASE_ID_CHECK(avlTreePtr,ADT_AVL_TreeLookupNode,ADT_AVL_TREE_T_ID);
    //不能是空键
    if(key == NULL)
    {
        return NULL;
    }
    //系统锁定
    ADT_SYS_LOCK(avlTreePtr);
	node = avlTreePtr->rootNodePtr;
	while (node != NULL) 
    {
		diff = avlTreePtr->compFunc(key, node->key);
		if (diff == 0) 
        {
            //系统解锁
            ADT_SYS_UNLOCK(avlTreePtr);
			return node;
		} 
        else if (diff < 0) 
        {
			node = node->children[AVL_TREE_NODE_LEFT];
		} 
        else 
        {
			node = node->children[AVL_TREE_NODE_RIGHT];
		}
	}
    //系统解锁
    ADT_SYS_UNLOCK(avlTreePtr);
	return NULL;
}

//平衡二叉树使用键查找节点,返回节点值
void* ADT_AVL_TreeLookup(ADT_T avlTreePtr, void* key,ADT_RESULT* resultPtr)
{
    ADT_NODE_T node;
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //指针检查
    ADT_BASE_PTR_CHECK(avlTreePtr,ADT_AVL_TreeLookup);
    //ID检查
    ADT_BASE_ID_CHECK(avlTreePtr,ADT_AVL_TreeLookup,ADT_AVL_TREE_T_ID);
    //不能是空键
    if(key == NULL)
    {
        *resultPtr = ADT_RESULT_FAIL;
        return NULL;
    }
    //系统锁定
    ADT_SYS_LOCK(avlTreePtr);
	node = ADT_AVL_TreeLookupNode(avlTreePtr, key);
    //系统解锁
    ADT_SYS_UNLOCK(avlTreePtr);
	if (node == NULL) 
    {
		*resultPtr = ADT_RESULT_FAIL;
        return NULL;
	} 
    else 
    {
		*resultPtr = ADT_RESULT_OK;
		return node->value;
	}
}

//平衡二叉树获取根节点
ADT_NODE_T ADT_AVL_TreeRootNode(ADT_T avlTreePtr)
{
    //指针检查
    ADT_BASE_PTR_CHECK(avlTreePtr,ADT_AVL_TreeRootNode);
    //ID检查
    ADT_BASE_ID_CHECK(avlTreePtr,ADT_AVL_TreeRootNode,ADT_AVL_TREE_T_ID);
    return avlTreePtr->rootNodePtr;
}

//平衡二叉树获取节点中包含的键
void* ADT_AVL_TreeNodeKey(ADT_NODE_T avlTreeNodePtr)
{
    if(avlTreeNodePtr == NULL)
    {
        return NULL;
    }
    return avlTreeNodePtr->key;
}

//平衡二叉树获取节点中包含的值
void* ADT_AVL_TreeNodeValue(ADT_NODE_T avlTreeNodePtr)
{
    if(avlTreeNodePtr == NULL)
    {
        return NULL;
    }
    return avlTreeNodePtr->value;
}

//平衡二叉树获取节点的子节点
ADT_NODE_T ADT_AVL_TreeNodeChild(ADT_NODE_T avlTreeNodePtr, AVLTreeNodeSide side)
{
    if(avlTreeNodePtr == NULL)
    {
        return NULL;
    }
    if (side == AVL_TREE_NODE_LEFT || side == AVL_TREE_NODE_RIGHT) 
    {
		return avlTreeNodePtr->children[side];
	} 
    else 
    {
		return NULL;
	}
}

//平衡二叉树查找节点的父节点
ADT_NODE_T ADT_AVL_TreeNodeParent(ADT_NODE_T avlTreeNodePtr)
{
    if(avlTreeNodePtr == NULL)
    {
        return NULL;
    }
    return avlTreeNodePtr->parent;
}

//添加子树到数组
static void ADT_AVL_TreeKeyValueToArrayAddSubTree(ADT_NODE_T subtree,void** array,uint32_t* index)
{
	if (subtree == NULL) 
    {
		return;
	}
    //先弄左边,递归调用
	ADT_AVL_TreeKeyValueToArrayAddSubTree(subtree->children[AVL_TREE_NODE_LEFT],array,index);
    //添加当前节点
	array[(*index)] = subtree->key;
	++(*index);
    array[(*index)] = subtree->value;
	++(*index);
    //增加右边
	ADT_AVL_TreeKeyValueToArrayAddSubTree(subtree->children[AVL_TREE_NODE_RIGHT],array, index);
}

//平衡二叉树数据转换为数组
void** ADT_AVL_TreeKeyValueToArray(ADT_T avlTreePtr,uint32_t* arrayLength)
{
    void** array;
	uint32_t index;
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //指针检查
    ADT_BASE_PTR_CHECK(avlTreePtr,ADT_AVL_TreeLookup);
    //ID检查
    ADT_BASE_ID_CHECK(avlTreePtr,ADT_AVL_TreeLookup,ADT_AVL_TREE_T_ID);
    //系统锁定
    ADT_SYS_LOCK(avlTreePtr);
    //申请内存
	array = ADT_MEM_MALLOC(avlTreePtr,sizeof(void*)*(avlTreePtr->currentSize)*2);
	if (array == NULL) 
    {
        *arrayLength = 0;
        //系统解锁
        ADT_SYS_UNLOCK(avlTreePtr);
		return NULL;
	}
	index = 0;
    *arrayLength = (avlTreePtr->currentSize)*2;
    //添加全部的键值对
	ADT_AVL_TreeKeyValueToArrayAddSubTree(avlTreePtr->rootNodePtr, array, &index);
    //系统解锁
    ADT_SYS_UNLOCK(avlTreePtr);
	return array;
}
