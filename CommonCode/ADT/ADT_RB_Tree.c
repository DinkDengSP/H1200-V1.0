/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-03-07 16:36:55 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-03-08 13:37:03 +0800
************************************************************************************************/ 
#include "ADT_RB_Tree.h"

//宏定义结构体类型,这种定义的好处是不用出现大量的各种类型名,代码整洁
//理解上差点意思,但是使用者不需要太了解内部结构
#define ADT_T                               ADT_RB_TREE_T
#define ADT_NODE_T                          ADT_RB_TREE_NODE_T
//结构体的类型ID,用于程序运行时的检查
#define ADT_RB_TREE_T_ID                    0X2020071D

//节点讯息
struct ADT_NODE_T
{
    void* key;//节点的键
    void* value;//节点的值
    struct ADT_NODE_T* children[2];//子节点
    struct ADT_NODE_T* parent;//父节点
    RBTreeNodeColor color;//节点颜色
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

//获取当前节点是父节点的左节点还是右节点
static RBTreeNodeSide ADT_RB_TreeNodeSide(ADT_NODE_T nodePtr)
{
	if (nodePtr->parent->children[RB_TREE_NODE_LEFT] == nodePtr) 
    {
		return RB_TREE_NODE_LEFT;
	} 
    else 
    {
		return RB_TREE_NODE_RIGHT;
	}
}
//获取指定节点的对称节点
static ADT_NODE_T ADT_RB_TreeNodeSibling(ADT_NODE_T nodePtr)
{
	RBTreeNodeSide side;
	side = ADT_RB_TreeNodeSide(nodePtr);
	return nodePtr->parent->children[1 - side];
}

//获取指定节点的叔叔节点
ADT_NODE_T ADT_RB_TreeNodeUncle(ADT_NODE_T nodePtr)
{
	return ADT_RB_TreeNodeSibling(nodePtr->parent);
}

//交换两个节点
static void ADT_RB_TreeNodeReplace(ADT_T rbTreePtr, ADT_NODE_T node1Ptr,ADT_NODE_T node2Ptr)
{
	int side;
    //节点不为空,设置2的父节点是1的父节点
	if (node2Ptr != NULL) 
    {
		node2Ptr->parent = node1Ptr->parent;
	}
    //检查1的父节点是否为空,是否是头节点
	if (node1Ptr->parent == NULL) 
    {
		rbTreePtr->rootNodePtr = node2Ptr;
	} 
    else 
    {
		side = ADT_RB_TreeNodeSide(node1Ptr);
        //设置子节点关联
		node1Ptr->parent->children[side] = node2Ptr;
	}
}


/* 节点旋转,旋转方式如下
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

static ADT_NODE_T ADT_RB_TreeRotate(ADT_T rbTreePtr, ADT_NODE_T nodePtr,RBTreeNodeSide direction)
{
	ADT_NODE_T newRootNodePtr;

	//设置节点的子节点获取,进行旋转必须
	newRootNodePtr = nodePtr->children[1-direction];
    //节点交换
	ADT_RB_TreeNodeReplace(rbTreePtr, nodePtr, newRootNodePtr);
    //指针指向重设
	nodePtr->children[1-direction] = newRootNodePtr->children[direction];
	newRootNodePtr->children[direction] = nodePtr;
    //更新父节点指针
	nodePtr->parent = newRootNodePtr;
    //更新子节点
	if (nodePtr->children[1-direction] != NULL) 
    {
		nodePtr->children[1-direction]->parent = nodePtr;
	}
	return newRootNodePtr;
}

//创建红黑二叉树
ADT_T ADT_RB_TreeCreate(ADT_AVLTreeCompareFunc compFuncPtr,ADT_MemMallocFuncPtr memMallocFunc,ADT_MemFreeFuncPtr memFreeFunc,
                                ADT_LockSystem lockSysFuncPtr,ADT_UnLockSystem unlockSysFuncPtr,ADT_SendString sendStrFunc)
{
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //回调函数检查
    ADT_CALL_BACK_CHECK(ADT_RB_TreeCreate,memMallocFunc,memFreeFunc,lockSysFuncPtr,unlockSysFuncPtr,sendStrFunc);
    //生成变量
    ADT_T rbTreePtr = NULL;
    if(compFuncPtr == NULL)
    {
        return NULL;
    }
    //内存申请
    ADT_MALLOC_SELF(rbTreePtr,memMallocFunc);
    //函数映射
    ADT_CALL_BACK_REGISTER(rbTreePtr,memMallocFunc,memFreeFunc,lockSysFuncPtr,unlockSysFuncPtr,sendStrFunc);
    //系统锁定
    ADT_SYS_LOCK(rbTreePtr);
    //数据复位
    rbTreePtr->currentSize = 0;
    //创建头节点
    rbTreePtr->rootNodePtr = NULL;
    //绑定比较函数
    rbTreePtr->compFunc = compFuncPtr;
    //功能辅助
    ADT_SET_IDENTIFY_CODE_SET(rbTreePtr,ADT_RB_TREE_T_ID);
    //系统解锁
    ADT_SYS_UNLOCK(rbTreePtr);
    //返回结构体指针
    return rbTreePtr;
}

//删除子树
static void ADT_RB_TreeFreeSubTree(ADT_T rbTreePtr,ADT_NODE_T nodePtr)
{
	if (nodePtr != NULL) 
    {
        //递归调用
		ADT_RB_TreeFreeSubTree(rbTreePtr,nodePtr->children[RB_TREE_NODE_LEFT]);
		ADT_RB_TreeFreeSubTree(rbTreePtr,nodePtr->children[RB_TREE_NODE_RIGHT]);
		//释放当前节点
	    ADT_MEM_FREE(rbTreePtr,nodePtr);
	}
}

//清空红黑二叉树
void ADT_RB_TreeClear(ADT_T rbTreePtr)
{
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //指针检查
    ADT_BASE_PTR_CHECK(rbTreePtr,ADT_RB_TreeClear);
    if(rbTreePtr->currentSize == 0)
    {
        rbTreePtr->rootNodePtr = NULL;
        //空链表无操作
        return;
    }
    //ID检查
    ADT_BASE_ID_CHECK(rbTreePtr,ADT_RB_TreeClear,ADT_RB_TREE_T_ID);
    //系统锁定
    ADT_SYS_LOCK(rbTreePtr);
    //递归释放
    ADT_RB_TreeFreeSubTree(rbTreePtr,rbTreePtr->rootNodePtr);
    //元素数量归零
    rbTreePtr->rootNodePtr = NULL;
    rbTreePtr->currentSize = 0;
    //系统解锁
    ADT_SYS_UNLOCK(rbTreePtr);
    return;
}

//红黑二叉树销毁
void ADT_RB_TreeDestroy(ADT_T* rbTreePtrPtr)
{
    //指针检查
    ADT_BASE_PTR_CHECK(rbTreePtrPtr,ADT_RB_TreeDestroy);
    //定义实体
    ADT_T rbTreePtr = (*rbTreePtrPtr);
    //指针检查
    ADT_BASE_PTR_CHECK(rbTreePtr,ADT_RB_TreeDestroy);
    //ID检查
    ADT_BASE_ID_CHECK(rbTreePtr,ADT_RB_TreeDestroy,ADT_RB_TREE_T_ID);
    //清空数据
    ADT_RB_TreeClear(rbTreePtr);
    //释放自身
    ADT_FREE_SELF(rbTreePtr);
    return;
}

//红黑二叉树获取元素数量
int ADT_RB_TreeNumEntries(ADT_T rbTreePtr)
{
    //指针检查
    ADT_BASE_PTR_CHECK(rbTreePtr,ADT_RB_TreeNumEntries);
    //ID检查
    ADT_BASE_ID_CHECK(rbTreePtr,ADT_RB_TreeNumEntries,ADT_RB_TREE_T_ID);
    return rbTreePtr->currentSize;
}

//声明插入函数
static void ADT_RB_TreeInsertCase1(ADT_T rbTreePtr, ADT_NODE_T rbTreeNodePtr);
static void ADT_RB_TreeInsertCase2(ADT_T rbTreePtr, ADT_NODE_T rbTreeNodePtr);
static void ADT_RB_TreeInsertCase3(ADT_T rbTreePtr, ADT_NODE_T rbTreeNodePtr);
static void ADT_RB_TreeInsertCase4(ADT_T rbTreePtr, ADT_NODE_T rbTreeNodePtr);
static void ADT_RB_TreeInsertCase5(ADT_T rbTreePtr, ADT_NODE_T rbTreeNodePtr);

//第一步插入,保证根节点是黑色
static void ADT_RB_TreeInsertCase1(ADT_T rbTreePtr, ADT_NODE_T rbTreeNodePtr)
{
	if (rbTreeNodePtr->parent == NULL) 
    {
        //根节点是黑节点
		rbTreeNodePtr->color = RB_TREE_NODE_BLACK;
	} 
    else 
    {
		ADT_RB_TreeInsertCase2(rbTreePtr, rbTreeNodePtr);
	}
}

//如果新节点的父节点是红色的，这个与红黑树条件冲突，因为两个孩子每个红色节点都是黑色的
static void ADT_RB_TreeInsertCase2(ADT_T rbTreePtr, ADT_NODE_T rbTreeNodePtr)
{
    //运行到这里,肯定不是根节点,有父节点
	if (rbTreeNodePtr->parent->color != RB_TREE_NODE_BLACK) 
    {
		ADT_RB_TreeInsertCase3(rbTreePtr, rbTreeNodePtr);
	}
}

//如果 parent 和 uncle 都是红色的，重新绘制它们重新涂成红色,并将祖父母涂成黑色
static void ADT_RB_TreeInsertCase3(ADT_T rbTreePtr, ADT_NODE_T rbTreeNodePtr)
{
	ADT_NODE_T grandparent;
	ADT_NODE_T uncle;

	grandparent = rbTreeNodePtr->parent->parent;
	uncle = ADT_RB_TreeNodeUncle(rbTreeNodePtr);

	if (uncle != NULL && uncle->color == RB_TREE_NODE_RED) 
    {
		rbTreeNodePtr->parent->color = RB_TREE_NODE_BLACK;
		uncle->color = RB_TREE_NODE_BLACK;
		grandparent->color = RB_TREE_NODE_RED;
		ADT_RB_TreeInsertCase1(rbTreePtr, grandparent);
	} 
    else 
    {
		ADT_RB_TreeInsertCase4(rbTreePtr, rbTreeNodePtr);
	}
}


/* 如果父母是红色的，但叔叔是黑色的，我们需要做一些旋转以保持树平衡并符合树
 * 状况。 如果节点相对于其父节点位于相反的一侧,由于父母是相对于其祖父母的，因此旋转
 * 父母。 无论哪种方式，我们将继续第五步
 *
 * eg.
 *
 *         B                              B
 *        / \                            / \
 *       R   B          ->     node ->  R   B
 *        \                            /
 *         R  <- node                 R
 *
 */

void ADT_RB_TreeInsertCase4(ADT_T rbTreePtr, ADT_NODE_T rbTreeNodePtr)
{
	ADT_NODE_T nextNodePtr;
	RBTreeNodeSide side;
    //请注意，此时，根据案例 3 的暗示，我们知道父母是红色的，但叔叔是黑色的。 我们因此
    //只需要检查节点在相对的哪一侧到它的父级，以及父级相对于的那一侧到祖父母。
	side = ADT_RB_TreeNodeSide(rbTreeNodePtr);
	if (side != ADT_RB_TreeNodeSide(rbTreeNodePtr->parent)) 
    {
		//轮换后，我们继续案例5，但是父节点将在底部
		nextNodePtr = rbTreeNodePtr->parent;
        //以相反的方向围绕父级旋转到一边
		ADT_RB_TreeRotate(rbTreePtr, rbTreeNodePtr->parent, (RBTreeNodeSide)(1-side));
	} 
    else 
    {
		nextNodePtr = rbTreeNodePtr;
	}
	ADT_RB_TreeInsertCase5(rbTreePtr, nextNodePtr);
}

/* 情况 5：节点相对于其父节点与父母是相对于它的祖父母位于同一侧。 该节点及其父节点是
 * 红色，但叔叔是黑色的。现在，在祖父母处旋转并重新着色父母和祖父母分别为黑色和红色。
 *
 *               G/B                 P/B
 *              /   \               /   \
 *           P/R     U/B    ->   N/R     G/R
 *          /   \                       /   \
 *       N/R      ?                   ?      U/B
 *
 */
void ADT_RB_TreeInsertCase5(ADT_T rbTreePtr, ADT_NODE_T rbTreeNodePtr)
{
	ADT_NODE_T parent;
	ADT_NODE_T grandparent;
	RBTreeNodeSide side;

	parent = rbTreeNodePtr->parent;
	grandparent = parent->parent;
    //相对于父母，我们是哪一边？ 这将决定我们旋转的方向
	side = ADT_RB_TreeNodeSide(rbTreeNodePtr);
	//在祖父母处旋转，与侧面相反的方向
	ADT_RB_TreeRotate(rbTreePtr, grandparent, (RBTreeNodeSide)(1-side));
    //重新上色
	parent->color = RB_TREE_NODE_BLACK;
	grandparent->color = RB_TREE_NODE_RED;
}

//红黑二叉树插入节点
ADT_NODE_T ADT_RB_TreeInsert(ADT_T rbTreePtr, void* key, void* value)
{
    ADT_NODE_T nodePtr;
	ADT_NODE_T *rover;
	ADT_NODE_T parent;
	RBTreeNodeSide side;
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //指针检查
    ADT_BASE_PTR_CHECK(rbTreePtr,ADT_RB_TreeInsert);
    //ID检查
    ADT_BASE_ID_CHECK(rbTreePtr,ADT_RB_TreeInsert,ADT_RB_TREE_T_ID);
    //不能是空键
    if(key == NULL)
    {
        return NULL;
    }
    //系统锁定
    ADT_SYS_LOCK(rbTreePtr);
    //生成新的节点
	nodePtr = ADT_MEM_MALLOC(rbTreePtr,sizeof(*nodePtr));
	if (nodePtr == NULL) 
    {
        //系统解锁
        ADT_SYS_UNLOCK(rbTreePtr);
		return NULL;
	}
    //设置节点
	nodePtr->key = key;
	nodePtr->value = value;
	nodePtr->color = RB_TREE_NODE_RED;
	nodePtr->children[RB_TREE_NODE_LEFT] = NULL;
	nodePtr->children[RB_TREE_NODE_RIGHT] = NULL;

	//首先，执行正常的二叉树式插入
	parent = NULL;
	rover = &(rbTreePtr->rootNodePtr);
    //先找到插入点
	while (*rover != NULL) 
    {
		parent = *rover;
		//选择走哪条路，左孩子还是右孩子
		if (rbTreePtr->compFunc(value, (*rover)->value) < 0) 
        {
			side = RB_TREE_NODE_LEFT;
		} 
        else 
        {
			side = RB_TREE_NODE_RIGHT;
		}
		rover = &(*rover)->children[side];
	}
    //在找到的位置进行插入
	*rover = nodePtr;
	nodePtr->parent = parent;
	ADT_RB_TreeInsertCase1(rbTreePtr, nodePtr);
    //更新计数器
	++(rbTreePtr->currentSize);
    //系统解锁
    ADT_SYS_UNLOCK(rbTreePtr);
	return nodePtr;
}

//红黑树删除节点之后对不上来的子节点进行处理
static void ADT_RB_TreeDeleteFixUp(ADT_T rbTreePtr, ADT_NODE_T node, ADT_NODE_T parent)
{
    ADT_NODE_T other = NULL;
    while (((node == NULL) || (node->color == RB_TREE_NODE_BLACK)) && (node != rbTreePtr->rootNodePtr))
    {
        if (parent->children[RB_TREE_NODE_LEFT] == node)
        { 
            // left child
            other = parent->children[RB_TREE_NODE_RIGHT];
            if ((other != NULL)&&(other->color == RB_TREE_NODE_RED))
            {
                /* Case 1: x的兄弟w是红色的 */
                other->color = RB_TREE_NODE_BLACK;
                parent->color = RB_TREE_NODE_RED;
                ADT_RB_TreeRotate(rbTreePtr, parent,RB_TREE_NODE_LEFT);
                other = parent->children[RB_TREE_NODE_RIGHT];
            }

            if (((other->children[RB_TREE_NODE_LEFT] == NULL)||(other->children[RB_TREE_NODE_LEFT]->color == RB_TREE_NODE_BLACK))&&
                ((other->children[RB_TREE_NODE_RIGHT] == NULL)||(other->children[RB_TREE_NODE_RIGHT]->color == RB_TREE_NODE_BLACK)))
            {
                /* Case 2: x的兄弟w是黑色，且w的俩个孩子也都是黑色的 */
                other->color = RB_TREE_NODE_RED;
                node = parent;
                parent = node->parent;
            }
            else
            {
                if ((other->children[RB_TREE_NODE_RIGHT] == NULL)||(other->children[RB_TREE_NODE_RIGHT]->color == RB_TREE_NODE_BLACK))
                {
                    /* Case 3: x的兄弟w是黑色的，并且w的左孩子是红色，右孩子为黑色 */
                    other->children[RB_TREE_NODE_LEFT]->color = RB_TREE_NODE_BLACK;
                    other->color = RB_TREE_NODE_RED;
                    ADT_RB_TreeRotate(rbTreePtr, other,RB_TREE_NODE_RIGHT);
                    other = parent->children[RB_TREE_NODE_RIGHT];
                }
                /* Case 4: x的兄弟w是黑色的；并且w的右孩子是红色的，左孩子任意颜色 */
                other->color = parent->color;
                parent->color = RB_TREE_NODE_BLACK;
                other->children[RB_TREE_NODE_RIGHT]->color = RB_TREE_NODE_BLACK;
                ADT_RB_TreeRotate(rbTreePtr, parent,RB_TREE_NODE_LEFT);
                node = rbTreePtr->rootNodePtr;
                break;
            }
        }
        else
        {
            other = parent->children[RB_TREE_NODE_LEFT];
            if ((other != NULL)&&(other->color == RB_TREE_NODE_RED))
            {
                /* Case 1: x的兄弟w是红色的 */
                other->color = RB_TREE_NODE_BLACK;
                parent->color = RB_TREE_NODE_RED;
                ADT_RB_TreeRotate(rbTreePtr, parent,RB_TREE_NODE_RIGHT);
                other = parent->children[RB_TREE_NODE_LEFT];
            }

            if (((other->children[RB_TREE_NODE_LEFT] == NULL)||(other->children[RB_TREE_NODE_LEFT]->color == RB_TREE_NODE_BLACK)) &&
                ((other->children[RB_TREE_NODE_RIGHT] == NULL)||(other->children[RB_TREE_NODE_RIGHT]->color == RB_TREE_NODE_BLACK)))
            {
                /* Case 2: x的兄弟w是黑色，且w的俩个孩子也都是黑色的 */
                other->color = RB_TREE_NODE_RED;
                node = parent;
                parent = node->parent;
            }
            else
            {
                if ((other->children[RB_TREE_NODE_LEFT] == NULL)||(other->children[RB_TREE_NODE_LEFT]->color == RB_TREE_NODE_BLACK))
                {
                    /* Case 3: x的兄弟w是黑色的，并且w的左孩子是红色，右孩子为黑色 */
                    other->children[RB_TREE_NODE_RIGHT]->color = RB_TREE_NODE_BLACK;
                    other->color = RB_TREE_NODE_RED;
                    ADT_RB_TreeRotate(rbTreePtr, other,RB_TREE_NODE_LEFT);
                    other = parent->children[RB_TREE_NODE_LEFT];
                }

                /* Case 4: x的兄弟w是黑色的；并且w的右孩子是红色的，左孩子任意颜色 */
                other->color = parent->color;
                parent->color = RB_TREE_NODE_BLACK;
                other->children[RB_TREE_NODE_LEFT]->color = RB_TREE_NODE_BLACK;
                ADT_RB_TreeRotate(rbTreePtr, parent,RB_TREE_NODE_RIGHT);
                node = rbTreePtr->rootNodePtr;
                break;
            }
        }
    }
    if (node != NULL)
    {
        node->color = RB_TREE_NODE_BLACK;
    }
}

//红黑二叉树移除节点
void ADT_RB_TreeRemoveNode(ADT_T rbTreePtr, ADT_NODE_T rbTreeNodePtr)
{
    RBTreeNodeColor color = RB_TREE_NODE_RED;
    ADT_NODE_T node = rbTreeNodePtr;
    ADT_NODE_T child = NULL;
    ADT_NODE_T parent = NULL;
    ADT_NODE_T replace = NULL;
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //指针检查
    ADT_BASE_PTR_CHECK(rbTreePtr,ADT_RB_TreeRemoveNode);
    //ID检查
    ADT_BASE_ID_CHECK(rbTreePtr,ADT_RB_TreeRemoveNode,ADT_RB_TREE_T_ID);
    //不能是空键
    if(rbTreeNodePtr == NULL)
    {
        return;
    }
    //当前尺寸为0
    if(rbTreePtr->currentSize == 0)
    {
        return;
    }
    //节点为空
    if(rbTreeNodePtr == NULL)
    {
        return;
    }
    //节点键为空
    if(rbTreeNodePtr->key == NULL)
    {
        return;
    }
    parent = node->parent;
    //没有父节点且不是头节点
    if ((parent == NULL) && (node != rbTreePtr->rootNodePtr))
    {
        return;
    }
    else if ((parent != NULL)&&(parent->children[RB_TREE_NODE_LEFT] != node)&&(parent->children[RB_TREE_NODE_RIGHT] != node))
    {
        //没有和父节点关联
        return;
    }
    //系统锁定
    ADT_SYS_LOCK(rbTreePtr);
    if ((node->children[RB_TREE_NODE_LEFT] != NULL)&&(node->children[RB_TREE_NODE_RIGHT] != NULL))
    {
        //被删节点的后继节点, 用它来取代"被删节点"的位置，然后再将"被删节点"去掉
        replace = node->children[RB_TREE_NODE_RIGHT];
        while (replace->children[RB_TREE_NODE_LEFT] != NULL)
        {
            replace = replace->children[RB_TREE_NODE_LEFT];
        }
        if (node->parent != NULL)
        {
            if (node->parent->children[RB_TREE_NODE_LEFT] == node)
            {
                node->parent->children[RB_TREE_NODE_LEFT] = replace;
            }
            else
            {
                node->parent->children[RB_TREE_NODE_RIGHT] = replace;
            }
        }
        else
        {
            rbTreePtr->rootNodePtr = replace;
        }
        child = replace->children[RB_TREE_NODE_RIGHT];
        parent = replace->parent;
        color = replace->color;
        if (parent == node)
        {
            parent = replace;
        }
        else
        {
            if (child != NULL)
            {
                child->parent = parent;
            }
            parent->children[RB_TREE_NODE_LEFT] = child;
            replace->children[RB_TREE_NODE_RIGHT] = node->children[RB_TREE_NODE_RIGHT];
            node->children[RB_TREE_NODE_RIGHT]->parent = replace;
        }
        replace->parent = node->parent;
        replace->color = node->color;
        replace->children[RB_TREE_NODE_LEFT] = node->children[RB_TREE_NODE_LEFT];
        node->children[RB_TREE_NODE_LEFT]->parent = replace;
        //整理
        if (color == RB_TREE_NODE_BLACK)
        {
            ADT_RB_TreeDeleteFixUp(rbTreePtr, child, parent);
        }
        --(rbTreePtr->currentSize);
        //设置链接
        node->children[RB_TREE_NODE_RIGHT] = NULL;
        node->children[RB_TREE_NODE_LEFT] = NULL;
        node->parent = NULL;
        node->color = RB_TREE_NODE_RED;
        //释放节点
        ADT_MEM_FREE(rbTreePtr,node);
        //系统解锁
        ADT_SYS_UNLOCK(rbTreePtr);
        return;
    }
    if (node->children[RB_TREE_NODE_LEFT] != NULL)
    {
        child = node->children[RB_TREE_NODE_LEFT];
    }
    else
    {
        child = node->children[RB_TREE_NODE_RIGHT];
    }
    parent = node->parent;
    color = node->color;
    if (child != NULL)
    {
        child->parent = parent;
    }
    if (parent != NULL)
    {
        if (parent->children[RB_TREE_NODE_LEFT] == node)
        {
            parent->children[RB_TREE_NODE_LEFT] = child;
        }
        else
        {
            parent->children[AVL_TREE_NODE_RIGHT] = child;
        }
    }
    else
    {
        rbTreePtr->rootNodePtr = child;
    }
    if (color == RB_TREE_NODE_BLACK)
    {
        ADT_RB_TreeDeleteFixUp(rbTreePtr, child, parent);
    }
    --(rbTreePtr->currentSize);
    //设置节点
    node->children[RB_TREE_NODE_RIGHT] = NULL;
    node->children[RB_TREE_NODE_LEFT] = NULL;
    node->parent = NULL;
    node->color = RB_TREE_NODE_RED;
    //释放节点
    ADT_MEM_FREE(rbTreePtr,node);
    //系统解锁
    ADT_SYS_UNLOCK(rbTreePtr);
    return;
}

//红黑二叉树移除节点
ADT_RESULT ADT_RB_TreeRemove(ADT_T rbTreePtr, void* key)
{
    ADT_NODE_T nodePtr;
	//ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //指针检查
    ADT_BASE_PTR_CHECK(rbTreePtr,ADT_RB_TreeRemove);
    //ID检查
    ADT_BASE_ID_CHECK(rbTreePtr,ADT_RB_TreeRemove,ADT_RB_TREE_T_ID);
    //不能是空键
    if(key == NULL)
    {
		return ADT_RESULT_FAIL;
    }
    //系统锁定
    ADT_SYS_LOCK(rbTreePtr);
	nodePtr = ADT_RB_TreeLookupNode(rbTreePtr, key);
	if (nodePtr == NULL) 
    {
		//系统解锁
        ADT_SYS_UNLOCK(rbTreePtr);
		return ADT_RESULT_FAIL;
	}
	ADT_RB_TreeRemoveNode(rbTreePtr, nodePtr);
    //系统解锁
    ADT_SYS_UNLOCK(rbTreePtr);
	return ADT_RESULT_OK;
}

//红黑二叉树搜索节点
ADT_NODE_T ADT_RB_TreeLookupNode(ADT_T rbTreePtr, void* key)
{
    ADT_NODE_T nodePtr;
	RBTreeNodeSide side;
	int diff;
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //指针检查
    ADT_BASE_PTR_CHECK(rbTreePtr,ADT_RB_TreeLookupNode);
    //ID检查
    ADT_BASE_ID_CHECK(rbTreePtr,ADT_RB_TreeLookupNode,ADT_RB_TREE_T_ID);
    //不能是空键
    if(key == NULL)
    {
        return NULL;
    }
    //系统锁定
    ADT_SYS_LOCK(rbTreePtr);
	nodePtr = rbTreePtr->rootNodePtr;
	//往下搜索树
	while (nodePtr != NULL) 
    {
		diff = rbTreePtr->compFunc(key, nodePtr->key);
		if (diff == 0) 
        {
            //系统解锁
            ADT_SYS_UNLOCK(rbTreePtr);
			return nodePtr;
		} 
        else if (diff < 0) 
        {
			side = RB_TREE_NODE_LEFT;
		} 
        else 
        {
			side = RB_TREE_NODE_RIGHT;
		}
		nodePtr = nodePtr->children[side];
	}
    //系统解锁
    ADT_SYS_UNLOCK(rbTreePtr);
	return NULL;
}

//红黑二叉树搜索节点并得到节点值
void* ADT_RB_TreeLookup(ADT_T rbTreePtr, void* key,ADT_RESULT* resultPtr)
{
    ADT_NODE_T nodePtr;
	//ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //指针检查
    ADT_BASE_PTR_CHECK(rbTreePtr,ADT_RB_TreeLookup);
    //ID检查
    ADT_BASE_ID_CHECK(rbTreePtr,ADT_RB_TreeLookup,ADT_RB_TREE_T_ID);
    //不能是空键
    if(key == NULL)
    {
        *resultPtr = ADT_RESULT_FAIL;
        return NULL;
    }
    //系统锁定
    ADT_SYS_LOCK(rbTreePtr);
	nodePtr = ADT_RB_TreeLookupNode(rbTreePtr, key);
    //系统解锁
    ADT_SYS_UNLOCK(rbTreePtr);
	if (nodePtr == NULL) 
    {
		*resultPtr = ADT_RESULT_FAIL;
        return NULL;
	} 
    else 
    {
		*resultPtr = ADT_RESULT_OK;
		return nodePtr->value;
	}
}

//红黑二叉树获取根节点
ADT_NODE_T ADT_RB_TreeRootNode(ADT_T rbTreePtr)
{
    //指针检查
    ADT_BASE_PTR_CHECK(rbTreePtr,ADT_RB_TreeRootNode);
    //ID检查
    ADT_BASE_ID_CHECK(rbTreePtr,ADT_RB_TreeRootNode,ADT_RB_TREE_T_ID);
    return rbTreePtr->rootNodePtr;
}

//红黑二叉树获取节点的键
void* ADT_RB_TreeNodeKey(ADT_NODE_T rbTreeNodePtr)
{
    if(rbTreeNodePtr == NULL)
    {
        return NULL;
    }
    return rbTreeNodePtr->key;
}

//红黑二叉树获取节点的值
void* ADT_RB_TreeNodeValue(ADT_NODE_T rbTreeNodePtr)
{
    if(rbTreeNodePtr == NULL)
    {
        return NULL;
    }
    return rbTreeNodePtr->value;
}

//红黑二叉树获取节点的子节点
ADT_NODE_T ADT_RB_TreeNodeChild(ADT_NODE_T rbTreeNodePtr, RBTreeNodeSide side)
{
    if(rbTreeNodePtr == NULL)
    {
        return NULL;
    }
    if (side == RB_TREE_NODE_LEFT || side == RB_TREE_NODE_RIGHT) 
    {
		return rbTreeNodePtr->children[side];
	} 
    else 
    {
		return NULL;
	}
}

//红黑二叉树获取节点的父节点
ADT_NODE_T ADT_RB_TreeNodeParent(ADT_NODE_T rbTreeNodePtr)
{
    if(rbTreeNodePtr == NULL)
    {
        return NULL;
    }
    return rbTreeNodePtr->parent;
}

//添加子树到数组
static void ADT_RB_TreeKeyValueToArrayAddSubTree(ADT_NODE_T subtree,void** array,uint32_t* index)
{
	if (subtree == NULL) 
    {
		return;
	}
    //先弄左边,递归调用
	ADT_RB_TreeKeyValueToArrayAddSubTree(subtree->children[AVL_TREE_NODE_LEFT],array,index);
    //添加当前节点
	array[(*index)] = subtree->key;
	++(*index);
    array[(*index)] = subtree->value;
	++(*index);
    //增加右边
	ADT_RB_TreeKeyValueToArrayAddSubTree(subtree->children[AVL_TREE_NODE_RIGHT],array, index);
}

//红黑二叉树键转换为数组
void** ADT_RB_TreeToArray(ADT_T rbTreePtr,uint32_t* arrayLength)
{
    void** array;
	uint32_t index;
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //指针检查
    ADT_BASE_PTR_CHECK(rbTreePtr,ADT_RB_TreeToArray);
    //ID检查
    ADT_BASE_ID_CHECK(rbTreePtr,ADT_RB_TreeToArray,ADT_RB_TREE_T_ID);
    //系统锁定
    ADT_SYS_LOCK(rbTreePtr);
    //申请内存
	array = ADT_MEM_MALLOC(rbTreePtr,sizeof(void*)*(rbTreePtr->currentSize)*2);
	if (array == NULL) 
    {
        *arrayLength = 0;
        //系统解锁
        ADT_SYS_UNLOCK(rbTreePtr);
		return NULL;
	}
	index = 0;
    *arrayLength = (rbTreePtr->currentSize)*2;
    //添加全部的键值对
	ADT_RB_TreeKeyValueToArrayAddSubTree(rbTreePtr->rootNodePtr, array, &index);
    //系统解锁
    ADT_SYS_UNLOCK(rbTreePtr);
	return array;
}