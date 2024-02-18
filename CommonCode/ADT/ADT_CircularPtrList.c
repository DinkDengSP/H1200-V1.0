/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-03-03 11:51:46 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-06-06 16:52:35 +0800
************************************************************************************************/ 
#include "ADT_CircularPtrList.h"

//宏定义结构体类型,这种定义的好处是不用出现大量的各种类型名,代码整洁
//理解上差点意思,但是使用者不需要太了解内部结构
#define ADT_T                               ADT_CIRCULAR_PTR_LIST_T
#define ADT_LIST_NODE_T                     ADT_CIRCULAR_PTR_LIST_NODE_T
//结构体的类型ID,用于程序运行时的检查
#define ADT_CIRCULAR_PTR_LIST_T_ID          0X20200716

//节点讯息
struct ADT_LIST_NODE_T
{
    void* nodeData;//节点保存的数据
    struct ADT_LIST_NODE_T* prevNodePtr;//上一个节点
    struct ADT_LIST_NODE_T* nextNodePtr;//下一个节点
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
    uint32_t currentSize;       //当前有效数据长度
    //节点数据定义
    struct ADT_LIST_NODE_T* headNodePtr;
};

//循环指针链表初始化
ADT_T ADT_CircularPtrListCreate(ADT_MemMallocFuncPtr memMallocFunc,ADT_MemFreeFuncPtr memFreeFunc,
                                    ADT_LockSystem lockSysFuncPtr,ADT_UnLockSystem unlockSysFuncPtr,ADT_SendString sendStrFunc)
{
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //回调函数检查
    ADT_CALL_BACK_CHECK(ADT_CircularPtrListCreate,memMallocFunc,memFreeFunc,lockSysFuncPtr,unlockSysFuncPtr,sendStrFunc);
    //生成变量
    ADT_T circularPtrListPtr = NULL;
    //内存申请
    ADT_MALLOC_SELF(circularPtrListPtr,memMallocFunc);
    //函数映射
    ADT_CALL_BACK_REGISTER(circularPtrListPtr,memMallocFunc,memFreeFunc,lockSysFuncPtr,unlockSysFuncPtr,sendStrFunc);
    //系统锁定
    ADT_SYS_LOCK(circularPtrListPtr);
    //数据复位
    circularPtrListPtr->currentSize = 0;
    //创建头节点
    circularPtrListPtr->headNodePtr = NULL;
    ADT_MALLOC_SELF((circularPtrListPtr->headNodePtr),memMallocFunc);
    //头节点的前驱和后向都指向自己
    circularPtrListPtr->headNodePtr->nextNodePtr = circularPtrListPtr->headNodePtr;
    circularPtrListPtr->headNodePtr->prevNodePtr = circularPtrListPtr->headNodePtr;
    //初始化为空节点
    circularPtrListPtr->headNodePtr->nodeData = NULL;
    //功能辅助
    ADT_SET_IDENTIFY_CODE_SET(circularPtrListPtr,ADT_CIRCULAR_PTR_LIST_T_ID);
    //系统解锁
    ADT_SYS_UNLOCK(circularPtrListPtr);
    //返回结构体指针
    return circularPtrListPtr;
}

//循环指针链表检查链表是否为空
ADT_BOOL ADT_CircularPtrListIsEmpty(ADT_T circularPtrListPtr)
{
    //指针检查
    ADT_BASE_PTR_CHECK(circularPtrListPtr,ADT_CircularPtrListIsEmpty);
    //ID检查
    ADT_BASE_ID_CHECK(circularPtrListPtr,ADT_CircularPtrListIsEmpty,ADT_CIRCULAR_PTR_LIST_T_ID);
    //数据检查
    if(circularPtrListPtr->currentSize == 0)
    {
        return ADT_BOOL_TRUE;
    }
    else
    {
        return ADT_BOOL_FALSE;
    }
}

//循环指针链表获取链表元素个数
uint32_t ADT_CircularPtrListGetNodeCount(ADT_T circularPtrListPtr)
{
    //指针检查
    ADT_BASE_PTR_CHECK(circularPtrListPtr,ADT_CircularPtrListGetNodeCount);
    //ID检查
    ADT_BASE_ID_CHECK(circularPtrListPtr,ADT_CircularPtrListGetNodeCount,ADT_CIRCULAR_PTR_LIST_T_ID);
    //返回当前尺寸
    return circularPtrListPtr->currentSize;
}

//循环指针链表清空
void ADT_CircularPtrListClear(ADT_T circularPtrListPtr)
{
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //指针检查
    ADT_BASE_PTR_CHECK(circularPtrListPtr,ADT_CircularPtrListClear);
    if(circularPtrListPtr->currentSize == 0)
    {
        //空链表无操作
        return;
    }
    //ID检查
    ADT_BASE_ID_CHECK(circularPtrListPtr,ADT_CircularPtrListClear,ADT_CIRCULAR_PTR_LIST_T_ID);
    //系统锁定
    ADT_SYS_LOCK(circularPtrListPtr);
    //临时辅助
    ADT_LIST_NODE_T currentNodePtr = circularPtrListPtr->headNodePtr->nextNodePtr;
    while (currentNodePtr != circularPtrListPtr->headNodePtr)
	{
        //保存当前节点的下一个节点地址
		ADT_LIST_NODE_T nextNodePtr = currentNodePtr->nextNodePtr;
        //释放当前节点
        ADT_MEM_FREE(circularPtrListPtr,currentNodePtr);
        //当前节点指向下一个节点
		currentNodePtr = nextNodePtr;
        //节点数量减少
        circularPtrListPtr->currentSize -= 1;
        if(circularPtrListPtr->currentSize == 0)
        {
            break;
        }
	}
    //头节点的前驱和后向都指向自己
    circularPtrListPtr->headNodePtr->nextNodePtr = circularPtrListPtr->headNodePtr;
    circularPtrListPtr->headNodePtr->prevNodePtr = circularPtrListPtr->headNodePtr;
    //初始化为空节点
    circularPtrListPtr->headNodePtr->nodeData = NULL;
    //节点数量设置为0
    circularPtrListPtr->currentSize = 0;
    //系统解锁
    ADT_SYS_UNLOCK(circularPtrListPtr);
    return;
}

//循环指针链表销毁
void ADT_CircularPtrListDestroy(ADT_T* circularPtrListPtrPtr)
{
    //指针检查
    ADT_BASE_PTR_CHECK(circularPtrListPtrPtr,ADT_CircularPtrListDestroy);
    //定义实体
    ADT_T circularPtrListPtr = (*circularPtrListPtrPtr);
    //清空数据
    ADT_CircularPtrListClear(circularPtrListPtr);
    //释放头节点
    ADT_MEM_FREE(circularPtrListPtr,circularPtrListPtr->headNodePtr);
    //释放自身
    ADT_FREE_SELF(circularPtrListPtr);
    return;
}

//循环指针链表查找指定元素节点
ADT_LIST_NODE_T ADT_CircularPtrListFindNodeWithData(ADT_T circularPtrListPtr,void* element,ADT_BaseElementCmp compareFuncPtr)
{
    uint32_t findLoopCount = 0;
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //指针检查
    ADT_BASE_PTR_CHECK(circularPtrListPtr,ADT_CircularPtrListFindNodeWithData);
    //ID检查
    ADT_BASE_ID_CHECK(circularPtrListPtr,ADT_CircularPtrListFindNodeWithData,ADT_CIRCULAR_PTR_LIST_T_ID);
    //系统锁定
    ADT_SYS_LOCK(circularPtrListPtr);
    //临时辅助
    ADT_LIST_NODE_T currentNodePtr = circularPtrListPtr->headNodePtr->nextNodePtr;
    //当cur指针指向头结点时，说明链表已经查找完毕
	while (currentNodePtr != circularPtrListPtr->headNodePtr)
	{
		if(ADT_CMP_EQUAL == compareFuncPtr(currentNodePtr->nodeData,element))
        {
            //系统解锁
            ADT_SYS_UNLOCK(circularPtrListPtr);
            //直接返回
            return currentNodePtr;
        }
        //进步到下一个节点
		currentNodePtr = currentNodePtr->nextNodePtr;
        //防止死循环
        findLoopCount++;
        if(findLoopCount >= circularPtrListPtr->currentSize)
        {
            //系统解锁
            ADT_SYS_UNLOCK(circularPtrListPtr);
            return NULL;
        }
	}
    //系统解锁
    ADT_SYS_UNLOCK(circularPtrListPtr);
    //没找到
    return NULL;
}

//循环指针链表插入元素到头节点
ADT_RESULT ADT_CircularPtrListInsertNodeFront(ADT_T circularPtrListPtr,void* element)
{
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //指针检查
    ADT_BASE_PTR_CHECK(circularPtrListPtr,ADT_CircularPtrListInsertNodeFront);
    //ID检查
    ADT_BASE_ID_CHECK(circularPtrListPtr,ADT_CircularPtrListInsertNodeFront,ADT_CIRCULAR_PTR_LIST_T_ID);
    //创建节点
    ADT_LIST_NODE_T newNodePtr = ADT_MEM_MALLOC(circularPtrListPtr,sizeof(*newNodePtr));
    newNodePtr->nodeData = element;
    //系统锁定
    ADT_SYS_LOCK(circularPtrListPtr);
    //获取原来的头节点
    ADT_LIST_NODE_T frontNodePtr = circularPtrListPtr->headNodePtr->nextNodePtr;
    //建立新结点与头结点之间的双向关系
    circularPtrListPtr->headNodePtr->nextNodePtr = newNodePtr;
    newNodePtr->prevNodePtr = circularPtrListPtr->headNodePtr;
    //建立新结点与front结点之间的双向关系
    newNodePtr->nextNodePtr = frontNodePtr;
    frontNodePtr->prevNodePtr = newNodePtr;    
    //节点数量增加
    circularPtrListPtr->currentSize += 1;      
    //系统解锁
    ADT_SYS_UNLOCK(circularPtrListPtr);
    return ADT_RESULT_OK;
}

//循环指针链表插入元素到尾节点
ADT_RESULT ADT_CircularPtrListInsertNodeEnd(ADT_T circularPtrListPtr,void* element)
{
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //指针检查
    ADT_BASE_PTR_CHECK(circularPtrListPtr,ADT_CircularPtrListInsertNodeEnd);
    //ID检查
    ADT_BASE_ID_CHECK(circularPtrListPtr,ADT_CircularPtrListInsertNodeEnd,ADT_CIRCULAR_PTR_LIST_T_ID);
    //创建节点
    ADT_LIST_NODE_T newNodePtr = ADT_MEM_MALLOC(circularPtrListPtr,sizeof(*newNodePtr));
    newNodePtr->nodeData = element;
    //系统锁定
    ADT_SYS_LOCK(circularPtrListPtr);
    //获取原来的头节点
    ADT_LIST_NODE_T tailNodePtr = circularPtrListPtr->headNodePtr->prevNodePtr;
    //建立新结点与头结点之间的双向关系
    newNodePtr->nextNodePtr = circularPtrListPtr->headNodePtr;
	circularPtrListPtr->headNodePtr->prevNodePtr = newNodePtr;
    //建立新结点与tail结点之间的双向关系
    tailNodePtr->nextNodePtr = newNodePtr;
	newNodePtr->prevNodePtr = tailNodePtr;  
    //节点数量增加
    circularPtrListPtr->currentSize += 1;     
    //系统解锁
    ADT_SYS_UNLOCK(circularPtrListPtr);
    return ADT_RESULT_OK;
}

//检查指定节点是否存在列表中
static ADT_RESULT ADT_CircularPtrListCheckNodeExist(ADT_T circularPtrListPtr,ADT_LIST_NODE_T findNodePtr)
{
    uint32_t findLoopCount = 0;
    //临时辅助
    ADT_LIST_NODE_T currentNodePtr = circularPtrListPtr->headNodePtr->nextNodePtr;
    //当cur指针指向头结点时，说明链表已经查找完毕
	while (currentNodePtr != circularPtrListPtr->headNodePtr)
	{
		if(currentNodePtr == findNodePtr)
        {
            //直接返回
            return ADT_RESULT_OK;
        }
        //进步到下一个节点
		currentNodePtr = currentNodePtr->nextNodePtr;
        //防止死循环
        findLoopCount++;
        if(findLoopCount >= circularPtrListPtr->currentSize)
        {
            //没找到
            return ADT_RESULT_FAIL;
        }
	}
    //没找到
    return ADT_RESULT_FAIL;
}

//循环指针链表插入元素到指定节点后方
ADT_RESULT ADT_CircularPtrListInsertNodeCurrent(ADT_T circularPtrListPtr,ADT_LIST_NODE_T currentNodePtr,void* element)
{
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //指针检查
    ADT_BASE_PTR_CHECK(circularPtrListPtr,ADT_CircularPtrListInsertNodeCurrent);
    //ID检查
    ADT_BASE_ID_CHECK(circularPtrListPtr,ADT_CircularPtrListInsertNodeCurrent,ADT_CIRCULAR_PTR_LIST_T_ID);
    //当前节点不能是空节点
    if(currentNodePtr == NULL)
    {
        return ADT_RESULT_FAIL;
    }
    //系统锁定
    ADT_SYS_LOCK(circularPtrListPtr);
    //确定当前节点存在于列表中
    if(ADT_RESULT_FAIL == ADT_CircularPtrListCheckNodeExist(circularPtrListPtr,currentNodePtr))
    {
        //系统解锁
        ADT_SYS_UNLOCK(circularPtrListPtr);
        return ADT_RESULT_FAIL;
    }
    //创建节点
    ADT_LIST_NODE_T newNodePtr = ADT_MEM_MALLOC(circularPtrListPtr,sizeof(*newNodePtr));
    newNodePtr->nodeData = element;
    //获取当前节点的上一节点对象
    ADT_LIST_NODE_T prevNodePtr = currentNodePtr->prevNodePtr;
    //建立新结点与before结点之间的双向关系
	prevNodePtr->nextNodePtr = newNodePtr;
	newNodePtr->prevNodePtr = prevNodePtr;
    //建立新结点与pos指向结点之间的双向关系
    newNodePtr->nextNodePtr = currentNodePtr;
	currentNodePtr->prevNodePtr = newNodePtr;
    //节点数量增加
    circularPtrListPtr->currentSize += 1; 
    //系统解锁
    ADT_SYS_UNLOCK(circularPtrListPtr);
    return ADT_RESULT_OK;
}

//查看顶部元素
ADT_LIST_NODE_T ADT_CircularPtrListPeekTop(ADT_T circularPtrListPtr)
{
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //指针检查
    ADT_BASE_PTR_CHECK(circularPtrListPtr,ADT_CircularPtrListPeekTop);
    //ID检查
    ADT_BASE_ID_CHECK(circularPtrListPtr,ADT_CircularPtrListPeekTop,ADT_CIRCULAR_PTR_LIST_T_ID);
    //系统锁定
    ADT_SYS_LOCK(circularPtrListPtr);
    //不为空才能获取到
    if(circularPtrListPtr->currentSize == 0)
    {
        //系统解锁
        ADT_SYS_UNLOCK(circularPtrListPtr);
        return NULL;
    }
    //保存第一个节点
    ADT_LIST_NODE_T frontNodePtr = circularPtrListPtr->headNodePtr->nextNodePtr;
    //系统解锁
    ADT_SYS_UNLOCK(circularPtrListPtr);
    //返回顶部节点
    return frontNodePtr;
}

//顶部弹出一个元素
void* ADT_CircularPtrListPopTop(ADT_T circularPtrListPtr)
{
    void* elementPtr = NULL;
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //指针检查
    ADT_BASE_PTR_CHECK(circularPtrListPtr,ADT_CircularPtrListPopTop);
    //ID检查
    ADT_BASE_ID_CHECK(circularPtrListPtr,ADT_CircularPtrListPopTop,ADT_CIRCULAR_PTR_LIST_T_ID);
    //系统锁定
    ADT_SYS_LOCK(circularPtrListPtr);
    //不为空才能弹出
    if(circularPtrListPtr->currentSize == 0)
    {
        //系统解锁
        ADT_SYS_UNLOCK(circularPtrListPtr);
        return NULL;
    }
    //保存第一个节点
    ADT_LIST_NODE_T frontNodePtr = circularPtrListPtr->headNodePtr->nextNodePtr;
    //保存新的头节点
    ADT_LIST_NODE_T newFrontNodePtr = frontNodePtr->nextNodePtr;
    //建立头结点与newfront结点之间的双向关系
    circularPtrListPtr->headNodePtr->nextNodePtr = newFrontNodePtr;
	newFrontNodePtr->prevNodePtr = circularPtrListPtr->headNodePtr;
    //节点赋值
    elementPtr = frontNodePtr->nodeData;
    //释放删除的这个节点
    ADT_MEM_FREE(circularPtrListPtr,frontNodePtr);
    //节点数量减少
    circularPtrListPtr->currentSize -= 1; 
    //系统解锁
    ADT_SYS_UNLOCK(circularPtrListPtr);
    return elementPtr;
}

//底部弹出一个元素
void* ADT_CircularPtrListPopButtom(ADT_T circularPtrListPtr)
{
    void* elementPtr = NULL;
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //指针检查
    ADT_BASE_PTR_CHECK(circularPtrListPtr,ADT_CircularPtrListPopButtom);
    //ID检查
    ADT_BASE_ID_CHECK(circularPtrListPtr,ADT_CircularPtrListPopButtom,ADT_CIRCULAR_PTR_LIST_T_ID);
    //系统锁定
    ADT_SYS_LOCK(circularPtrListPtr);
    //不为空才能弹出
    if(circularPtrListPtr->currentSize == 0)
    {
        //系统解锁
        ADT_SYS_UNLOCK(circularPtrListPtr);
        return NULL;
    }
    //保存尾巴上节点
    ADT_LIST_NODE_T tailNodePtr = circularPtrListPtr->headNodePtr->prevNodePtr;
    //保存新尾节点
    ADT_LIST_NODE_T newTailNodePtr = tailNodePtr->prevNodePtr;
    //建立头结点与newfront结点之间的双向关系
    newTailNodePtr->nextNodePtr = circularPtrListPtr->headNodePtr;
	circularPtrListPtr->headNodePtr->prevNodePtr = newTailNodePtr;
    //节点赋值
    elementPtr = tailNodePtr->nodeData;
    //释放删除的这个节点
    ADT_MEM_FREE(circularPtrListPtr,tailNodePtr);
    //节点数量减少
    circularPtrListPtr->currentSize -= 1; 
    //系统解锁
    ADT_SYS_UNLOCK(circularPtrListPtr);
    return elementPtr;
}

//获取节点数据中保存的数据值
void* ADT_CircularPtrListGetNodeVal(ADT_LIST_NODE_T listNodePtr)
{
    if(listNodePtr == NULL)
    {
        return NULL;
    }
    return listNodePtr->nodeData;
}

//循环指针链表删除头节点
ADT_RESULT ADT_CircularPtrListDeleteNodeFront(ADT_T circularPtrListPtr)
{
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //指针检查
    ADT_BASE_PTR_CHECK(circularPtrListPtr,ADT_CircularPtrListDeleteNodeFront);
    //ID检查
    ADT_BASE_ID_CHECK(circularPtrListPtr,ADT_CircularPtrListDeleteNodeFront,ADT_CIRCULAR_PTR_LIST_T_ID);
    //系统锁定
    ADT_SYS_LOCK(circularPtrListPtr);
    //不为空才能删除
    if(circularPtrListPtr->currentSize == 0)
    {
        //系统解锁
        ADT_SYS_UNLOCK(circularPtrListPtr);
        return ADT_RESULT_OK;
    }
    //保存第一个节点
    ADT_LIST_NODE_T frontNodePtr = circularPtrListPtr->headNodePtr->nextNodePtr;
    //保存新的头节点,新的头节点是原来头节点的下一个节点
    ADT_LIST_NODE_T newFrontNodePtr = frontNodePtr->nextNodePtr;
    //建立头结点与newfront结点之间的双向关系
    circularPtrListPtr->headNodePtr->nextNodePtr = newFrontNodePtr;
	newFrontNodePtr->prevNodePtr = circularPtrListPtr->headNodePtr;
    //释放删除的这个节点
    ADT_MEM_FREE(circularPtrListPtr,frontNodePtr);
    //节点数量减少
    circularPtrListPtr->currentSize -= 1; 
    //系统解锁
    ADT_SYS_UNLOCK(circularPtrListPtr);
    return ADT_RESULT_OK;
}

//循环指针链表删除尾节点
ADT_RESULT ADT_CircularPtrListDeleteNodeEnd(ADT_T circularPtrListPtr)
{
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //指针检查
    ADT_BASE_PTR_CHECK(circularPtrListPtr,ADT_CircularPtrListDeleteNodeEnd);
    //ID检查
    ADT_BASE_ID_CHECK(circularPtrListPtr,ADT_CircularPtrListDeleteNodeEnd,ADT_CIRCULAR_PTR_LIST_T_ID);
    //系统锁定
    ADT_SYS_LOCK(circularPtrListPtr);
    //不为空才能删除
    if(circularPtrListPtr->currentSize == 0)
    {
        //系统解锁
        ADT_SYS_UNLOCK(circularPtrListPtr);
        return ADT_RESULT_OK;
    }
    //保存尾巴上节点
    ADT_LIST_NODE_T tailNodePtr = circularPtrListPtr->headNodePtr->prevNodePtr;
    //保存新尾节点,新的尾节点是原来尾节点的上一个节点
    ADT_LIST_NODE_T newTailNodePtr = tailNodePtr->prevNodePtr;
    //建立头结点与newfront结点之间的双向关系
    newTailNodePtr->nextNodePtr = circularPtrListPtr->headNodePtr;
	circularPtrListPtr->headNodePtr->prevNodePtr = newTailNodePtr;
    //释放删除的这个节点
    ADT_MEM_FREE(circularPtrListPtr,tailNodePtr);
    //节点数量减少
    circularPtrListPtr->currentSize -= 1; 
    //系统解锁
    ADT_SYS_UNLOCK(circularPtrListPtr);
    return ADT_RESULT_OK;
}

//循环指针链表删除指定节点
ADT_RESULT ADT_CircularPtrListDeleteNodeCurrent(ADT_T circularPtrListPtr,ADT_LIST_NODE_T currentNodePtr)
{
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //指针检查
    ADT_BASE_PTR_CHECK(circularPtrListPtr,ADT_CircularPtrListDeleteNodeCurrent);
    //ID检查
    ADT_BASE_ID_CHECK(circularPtrListPtr,ADT_CircularPtrListDeleteNodeCurrent,ADT_CIRCULAR_PTR_LIST_T_ID);
    //当前节点不能是空节点
    if(currentNodePtr == NULL)
    {
        return ADT_RESULT_FAIL;
    }
    //系统锁定
    ADT_SYS_LOCK(circularPtrListPtr);
    //不为空才能删除
    if(circularPtrListPtr->currentSize == 0)
    {
        //系统解锁
        ADT_SYS_UNLOCK(circularPtrListPtr);
        return ADT_RESULT_OK;
    }
    //不能是头节点自身
    if(currentNodePtr == circularPtrListPtr->headNodePtr)
    {
        //系统解锁
        ADT_SYS_UNLOCK(circularPtrListPtr);
        return ADT_RESULT_OK;
    }
    //确定当前节点存在于列表中
    if(ADT_RESULT_FAIL == ADT_CircularPtrListCheckNodeExist(circularPtrListPtr,currentNodePtr))
    {
        //系统解锁
        ADT_SYS_UNLOCK(circularPtrListPtr);
        return ADT_RESULT_FAIL;
    }
    //记录当前节点的上一个节点
    ADT_LIST_NODE_T prevNodePtr = currentNodePtr->prevNodePtr;
    //记录当前节点的下一个节点
    ADT_LIST_NODE_T nextNodePtr = currentNodePtr->nextNodePtr;
    //建立before结点与after结点之间的双向关系
	prevNodePtr->nextNodePtr = nextNodePtr;
	nextNodePtr->prevNodePtr = prevNodePtr;
    //释放当前
    ADT_MEM_FREE(circularPtrListPtr,currentNodePtr);
    //节点数量减少
    circularPtrListPtr->currentSize -= 1; 
    //系统解锁
    ADT_SYS_UNLOCK(circularPtrListPtr);
    return ADT_RESULT_OK;
}

//节点遍历
void ADT_CircularPtrListTraverse(ADT_T circularPtrListPtr,ADT_BaseElementOperation opFuncPtr)
{
    uint32_t findLoopCount = 0;
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //指针检查
    ADT_BASE_PTR_CHECK(circularPtrListPtr,ADT_CircularPtrListTraverse);
    //ID检查
    ADT_BASE_ID_CHECK(circularPtrListPtr,ADT_CircularPtrListTraverse,ADT_CIRCULAR_PTR_LIST_T_ID);
    if(opFuncPtr == NULL)
    {
        return;
    }
    //系统锁定
    ADT_SYS_LOCK(circularPtrListPtr);
    //不为空才能执行
    if(circularPtrListPtr->currentSize == 0)
    {
        //系统解锁
        ADT_SYS_UNLOCK(circularPtrListPtr);
        return;
    }
    //临时辅助
    ADT_LIST_NODE_T currentNodePtr = circularPtrListPtr->headNodePtr->nextNodePtr;
    //当cur指针指向头结点时，说明链表已经查找完毕
	while (currentNodePtr != circularPtrListPtr->headNodePtr)
	{
		//对节点数据执行指定操作
        opFuncPtr(&(currentNodePtr->nodeData));
        //进步到下一个节点
		currentNodePtr = currentNodePtr->nextNodePtr;
        //防止死循环
        findLoopCount++;
        if(findLoopCount >= circularPtrListPtr->currentSize)
        {
            //系统解锁
            ADT_SYS_UNLOCK(circularPtrListPtr);
            return;
        }
	}
    //系统解锁
    ADT_SYS_UNLOCK(circularPtrListPtr);
    //没找到
    return;
}

//节点条件遍历
void ADT_CircularPtrListTraverseCondition(ADT_T circularPtrListPtr,void* element,ADT_BaseElementCmp compareFuncPtr,
                                                ADT_BaseElementOperation opFuncPtr)
{
    uint32_t findLoopCount = 0;
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //指针检查
    ADT_BASE_PTR_CHECK(circularPtrListPtr,ADT_CircularPtrListTraverseCondition);
    //ID检查
    ADT_BASE_ID_CHECK(circularPtrListPtr,ADT_CircularPtrListTraverseCondition,ADT_CIRCULAR_PTR_LIST_T_ID);
    if(compareFuncPtr == NULL)
    {
        return;
    }
    //系统锁定
    ADT_SYS_LOCK(circularPtrListPtr);
    //临时辅助
    ADT_LIST_NODE_T currentNodePtr = circularPtrListPtr->headNodePtr->nextNodePtr;
    //当cur指针指向头结点时，说明链表已经查找完毕
	while (currentNodePtr != circularPtrListPtr->headNodePtr)
	{
		if(ADT_CMP_EQUAL == compareFuncPtr(currentNodePtr->nodeData,element))
        {
            if(opFuncPtr != NULL)
            {
                //找到条件匹配节点,执行操作
                opFuncPtr(&(currentNodePtr->nodeData));
            }
        }
        //进步到下一个节点
		currentNodePtr = currentNodePtr->nextNodePtr;
        //防止死循环
        findLoopCount++;
        if(findLoopCount >= circularPtrListPtr->currentSize)
        {
            //系统解锁
            ADT_SYS_UNLOCK(circularPtrListPtr);
            return;
        }
	}
    //系统解锁
    ADT_SYS_UNLOCK(circularPtrListPtr);
    //没找到
    return;
}

//节点条件遍历并删除节点,满足条件的节点执行指定操作并最终删除掉这个节点
void ADT_CircularPtrListTraverseConditionDelete(ADT_T circularPtrListPtr,void* element,ADT_BaseElementCmp compareFuncPtr,
                                                ADT_BaseElementOperation opFuncPtr)
{
    uint32_t findLoopCount = 0;
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //指针检查
    ADT_BASE_PTR_CHECK(circularPtrListPtr,ADT_CircularPtrListTraverseConditionDelete);
    //ID检查
    ADT_BASE_ID_CHECK(circularPtrListPtr,ADT_CircularPtrListTraverseConditionDelete,ADT_CIRCULAR_PTR_LIST_T_ID);
    if(compareFuncPtr == NULL)
    {
        return;
    }
    //系统锁定
    ADT_SYS_LOCK(circularPtrListPtr);
    //临时辅助
    ADT_LIST_NODE_T currentNodePtr = circularPtrListPtr->headNodePtr->nextNodePtr;
    //当cur指针指向头结点时，说明链表已经查找完毕
	while (currentNodePtr != circularPtrListPtr->headNodePtr)
	{
		if(ADT_CMP_EQUAL == compareFuncPtr(currentNodePtr->nodeData,element))
        {
            //找到条件匹配节点,执行操作
            if(opFuncPtr != NULL)
            {
                opFuncPtr(&(currentNodePtr->nodeData));
            }
            //执行完成,删除节点
            //记录当前节点的上一个节点
            ADT_LIST_NODE_T prevNodePtr = currentNodePtr->prevNodePtr;
            //记录当前节点的下一个节点
            ADT_LIST_NODE_T nextNodePtr = currentNodePtr->nextNodePtr;
            //建立before结点与after结点之间的双向关系
            prevNodePtr->nextNodePtr = nextNodePtr;
            nextNodePtr->prevNodePtr = prevNodePtr;
            //释放当前
            ADT_MEM_FREE(circularPtrListPtr,currentNodePtr);
            //节点数量减少
            circularPtrListPtr->currentSize -= 1; 
            //进步到下一个节点
            currentNodePtr = nextNodePtr;
        }
        else
        {
            //进步到下一个节点
		    currentNodePtr = currentNodePtr->nextNodePtr;
        }
        //防止死循环
        findLoopCount++;
        if(findLoopCount >= circularPtrListPtr->currentSize)
        {
            //系统解锁
            ADT_SYS_UNLOCK(circularPtrListPtr);
            return;
        }
	}
    //系统解锁
    ADT_SYS_UNLOCK(circularPtrListPtr);
    //没找到
    return;
}

//节点遍历并删除满足条件的节点
void ADT_CircularPtrListTraverseDelete(ADT_T circularPtrListPtr,ADT_BaseElementMeetCondition meetConditionFuncPtr,uint32_t utilData)
{
    uint32_t findLoopCount = 0;
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //指针检查
    ADT_BASE_PTR_CHECK(circularPtrListPtr,ADT_CircularPtrListTraverseConditionDelete);
    //ID检查
    ADT_BASE_ID_CHECK(circularPtrListPtr,ADT_CircularPtrListTraverseConditionDelete,ADT_CIRCULAR_PTR_LIST_T_ID);
    if(meetConditionFuncPtr == NULL)
    {
        return;
    }
    //系统锁定
    ADT_SYS_LOCK(circularPtrListPtr);
    //临时辅助
    ADT_LIST_NODE_T currentNodePtr = circularPtrListPtr->headNodePtr->nextNodePtr;
    //当cur指针指向头结点时，说明链表已经查找完毕
	while (currentNodePtr != circularPtrListPtr->headNodePtr)
	{
		if(ADT_CMP_EQUAL == meetConditionFuncPtr(currentNodePtr->nodeData,utilData))
        {
            //条件匹配,删除节点
            //记录当前节点的上一个节点
            ADT_LIST_NODE_T prevNodePtr = currentNodePtr->prevNodePtr;
            //记录当前节点的下一个节点
            ADT_LIST_NODE_T nextNodePtr = currentNodePtr->nextNodePtr;
            //建立before结点与after结点之间的双向关系
            prevNodePtr->nextNodePtr = nextNodePtr;
            nextNodePtr->prevNodePtr = prevNodePtr;
            //释放当前
            ADT_MEM_FREE(circularPtrListPtr,currentNodePtr);
            //节点数量减少
            circularPtrListPtr->currentSize -= 1; 
            //进步到下一个节点
            currentNodePtr = nextNodePtr;
        }
        else
        {
            //进步到下一个节点
		    currentNodePtr = currentNodePtr->nextNodePtr;
        }
        //防止死循环
        findLoopCount++;
        if(findLoopCount >= circularPtrListPtr->currentSize)
        {
            //系统解锁
            ADT_SYS_UNLOCK(circularPtrListPtr);
            return;
        }
	}
    //系统解锁
    ADT_SYS_UNLOCK(circularPtrListPtr);
    //没找到
    return;
}