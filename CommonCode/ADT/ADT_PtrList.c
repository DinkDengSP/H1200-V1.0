/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-03-04 12:32:50 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-03-04 16:10:09 +0800
************************************************************************************************/ 
#include "ADT_PtrList.h"

//宏定义结构体类型,这种定义的好处是不用出现大量的各种类型名,代码整洁
//理解上差点意思,但是使用者不需要太了解内部结构
#define ADT_T                               ADT_PTR_LIST_T
#define ADT_LIST_NODE_T                     ADT_PTR_LIST_NODE_T
//结构体的类型ID,用于程序运行时的检查
#define ADT_PTR_LIST_T_ID                   0X20200717

//节点讯息
struct ADT_LIST_NODE_T
{
    void* nodeData;//节点保存的数据
    struct ADT_LIST_NODE_T* nextNodePtr;//上一个节点
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


//创建对象
ADT_T ADT_PtrListCreate(ADT_MemMallocFuncPtr memMallocFunc,ADT_MemFreeFuncPtr memFreeFunc,
                                ADT_LockSystem lockSysFuncPtr,ADT_UnLockSystem unlockSysFuncPtr,ADT_SendString sendStrFunc)
{
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //回调函数检查
    ADT_CALL_BACK_CHECK(ADT_PtrListCreate,memMallocFunc,memFreeFunc,lockSysFuncPtr,unlockSysFuncPtr,sendStrFunc);
    //生成变量
    ADT_T ptrListPtr = NULL;
    //内存申请
    ADT_MALLOC_SELF(ptrListPtr,memMallocFunc);
    //函数映射
    ADT_CALL_BACK_REGISTER(ptrListPtr,memMallocFunc,memFreeFunc,lockSysFuncPtr,unlockSysFuncPtr,sendStrFunc);
    //系统锁定
    ADT_SYS_LOCK(ptrListPtr);
    //数据复位
    ptrListPtr->currentSize = 0;
    //创建头节点
    ptrListPtr->headNodePtr = NULL;
    //功能辅助
    ADT_SET_IDENTIFY_CODE_SET(ptrListPtr,ADT_PTR_LIST_T_ID);
    //系统解锁
    ADT_SYS_UNLOCK(ptrListPtr);
    //返回结构体指针
    return ptrListPtr;
}

//销毁指针链表
void ADT_PtrListDestroy(ADT_T* ptrListPtrPtr)
{
    //指针检查
    ADT_BASE_PTR_CHECK(ptrListPtrPtr,ADT_PtrListDestroy);
    //定义实体
    ADT_T ptrListPtr = (*ptrListPtrPtr);
    //清空数据
    ADT_PtrListClear(ptrListPtr);
    //释放自身
    ADT_FREE_SELF(ptrListPtr);
    return;
}

//释放全部的指针链表数据
void ADT_PtrListClear(ADT_T ptrListPtr)
{
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //指针检查
    ADT_BASE_PTR_CHECK(ptrListPtr,ADT_PtrListClear);
    //ID检查
    ADT_BASE_ID_CHECK(ptrListPtr,ADT_PtrListClear,ADT_PTR_LIST_T_ID);
    //系统锁定
    ADT_SYS_LOCK(ptrListPtr);
    //循环弹出元素直到没有元素
    while(0 != ptrListPtr->currentSize)
    {
        //保存第一个节点
        ADT_LIST_NODE_T headNodePtr = ptrListPtr->headNodePtr;
        //当前头节点变成原来头节点的下一个元素
        ptrListPtr->headNodePtr = headNodePtr->nextNodePtr;
        //释放删除的这个节点
        ADT_MEM_FREE(ptrListPtr,headNodePtr);
        //节点数量减少
        ptrListPtr->currentSize -= 1; 
    }
    //系统解锁
    ADT_SYS_UNLOCK(ptrListPtr);
    return;
}

//创建一个当前链表的拷贝
ADT_T ADT_PtrListCopy(ADT_T ptrListPtr)
{
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //指针检查
    ADT_BASE_PTR_CHECK(ptrListPtr,ADT_PtrListCopy);
    //ID检查
    ADT_BASE_ID_CHECK(ptrListPtr,ADT_PtrListCopy,ADT_PTR_LIST_T_ID);
    //系统锁定
    ADT_SYS_LOCK(ptrListPtr);
    //生成新的链表
    ADT_T newPtrListPtr = ptrListPtr->memMallocFunc(sizeof(*newPtrListPtr));
    //数据拷贝
    ADT_BASE_MEM_COPY((void*)newPtrListPtr,(void*)ptrListPtr,sizeof(*newPtrListPtr));
    //头节点设定
    if(ptrListPtr->headNodePtr == NULL)
    {
        //拷贝完成,系统解锁
        ADT_SYS_UNLOCK(newPtrListPtr);
        return newPtrListPtr;
    }
    //头节点不为空
    ADT_LIST_NODE_T newNodePtr = ADT_MEM_MALLOC(ptrListPtr,sizeof(*newNodePtr));
    newNodePtr->nodeData = ptrListPtr->headNodePtr->nodeData;
    //绑定新的链表的头节点
    newPtrListPtr->headNodePtr = newNodePtr;
    //辅助节点
    ADT_LIST_NODE_T utilSrcNodePtr = ptrListPtr->headNodePtr;
    ADT_LIST_NODE_T utilCpyNodePtr = newPtrListPtr->headNodePtr;
    //循环检测拷贝
    while(utilSrcNodePtr->nextNodePtr != NULL)
    {
        //创建节点
        newNodePtr = ADT_MEM_MALLOC(ptrListPtr,sizeof(*newNodePtr));
        //源节点步进
        utilSrcNodePtr = utilSrcNodePtr->nextNodePtr;
        //数据拷贝
        newNodePtr->nodeData = utilSrcNodePtr->nodeData;
        newNodePtr->nextNodePtr = utilSrcNodePtr->nextNodePtr;
        //关联数据
        utilCpyNodePtr->nextNodePtr = newNodePtr;
        //关联数据步进
        utilCpyNodePtr = utilCpyNodePtr->nextNodePtr;
    }
    //拷贝完成,解锁
    ADT_SYS_UNLOCK(ptrListPtr);
    return newPtrListPtr;
}

//将两个链表连接起来,并且释放后一个链表
ADT_RESULT ADT_PtrListAppendFreeTail(ADT_T ptrListPtr,ADT_T* ptrListTailPtrPtr)
{
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //指针检查
    ADT_BASE_PTR_CHECK(ptrListPtr,ADT_PtrListAppendFreeTail);
    //ID检查
    ADT_BASE_ID_CHECK(ptrListPtr,ADT_PtrListAppendFreeTail,ADT_PTR_LIST_T_ID);
    //指针检查
    ADT_BASE_PTR_CHECK(ptrListTailPtrPtr,ADT_PtrListAppendFreeTail);
    ADT_T ptrListTailPtr = *ptrListTailPtrPtr;
    //指针检查
    ADT_BASE_PTR_CHECK(ptrListTailPtr,ADT_PtrListAppendFreeTail);
    //ID检查
    ADT_BASE_ID_CHECK(ptrListTailPtr,ADT_PtrListAppendFreeTail,ADT_PTR_LIST_T_ID);
    //两个链表的内存申请释放函数必须一模一样
    if(ptrListPtr->memMallocFunc != ptrListTailPtr->memMallocFunc)
    {
        ptrListPtr->sendStrFunc("ADT_PtrListAppendFreeTail MemMalloc Func Not Equal!!\r\n");
        return ADT_RESULT_FAIL;
    }
    if(ptrListPtr->memFreeFunc != ptrListTailPtr->memFreeFunc)
    {
        ptrListPtr->sendStrFunc("ADT_PtrListAppendFreeTail MemFree Func Not Equal!!\r\n");
        return ADT_RESULT_FAIL;
    }
    //系统锁定
    ADT_SYS_LOCK(ptrListPtr);
    //找到源链表的最后一个元素
    //保存第一个节点
    ADT_LIST_NODE_T headNodePtr = ptrListPtr->headNodePtr;
    //如果本身头元素就是空元素
    if(headNodePtr == NULL)
    {
        //链接下一个链表的头元素
        headNodePtr = ptrListTailPtr->headNodePtr;
        //数据增加
        ptrListPtr->currentSize = ptrListPtr->currentSize + ptrListTailPtr->currentSize;
        //释放链表
        ADT_FREE_SELF(ptrListTailPtr);
        //系统解锁
        ADT_SYS_UNLOCK(ptrListPtr);
        return ADT_RESULT_OK;
    }
    //头元素不是空元素,找到尾巴上的元素
    while(headNodePtr->nextNodePtr != NULL)
    {
        //移动到下一个元素
        headNodePtr = headNodePtr->nextNodePtr;
    }
    //最后一个元素链接新的链表的头部元素
    headNodePtr->nextNodePtr = ptrListTailPtr->headNodePtr;
    //数据增加
    ptrListPtr->currentSize = ptrListPtr->currentSize + ptrListTailPtr->currentSize;
    //释放链表
    ADT_FREE_SELF(ptrListTailPtr);
    //系统解锁
    ADT_SYS_UNLOCK(ptrListPtr);
    return ADT_RESULT_OK;
}

//压入一个链表元素
ADT_RESULT ADT_PtrListPush(ADT_T ptrListPtr,void* element)
{
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //指针检查
    ADT_BASE_PTR_CHECK(ptrListPtr,ADT_PtrListPush);
    //ID检查
    ADT_BASE_ID_CHECK(ptrListPtr,ADT_PtrListPush,ADT_PTR_LIST_T_ID);
    //检查元素,空元素不处理
    if(element == NULL)
    {
        return ADT_RESULT_FAIL;
    }
    //系统锁定
    ADT_SYS_LOCK(ptrListPtr);
    //生成新的节点
    ADT_LIST_NODE_T newNodePtr = ADT_MEM_MALLOC(ptrListPtr,sizeof(*newNodePtr));
    newNodePtr->nodeData = element;
    //新节点的下一个节点只想原来的头节点
    newNodePtr->nextNodePtr = ptrListPtr->headNodePtr;
    //头节点成为新的节点
    ptrListPtr->headNodePtr = newNodePtr;
    //节点数量增加
    ptrListPtr->currentSize += 1;      
    //系统解锁
    ADT_SYS_UNLOCK(ptrListPtr);
    return ADT_RESULT_OK;
}

//弹出一个链表元素
void* ADT_PtrListPop(ADT_T ptrListPtr)
{
    void* elementPtr = NULL;
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //指针检查
    ADT_BASE_PTR_CHECK(ptrListPtr,ADT_PtrListAppend);
    //ID检查
    ADT_BASE_ID_CHECK(ptrListPtr,ADT_PtrListAppend,ADT_PTR_LIST_T_ID);
    //没有数据元素,直接返回空
    if(ptrListPtr->currentSize == 0)
    {
        return NULL;
    }
    //系统锁定
    ADT_SYS_LOCK(ptrListPtr);
    //保存第一个节点
    ADT_LIST_NODE_T headNodePtr = ptrListPtr->headNodePtr;
    //当前头节点变成原来头节点的下一个元素
    ptrListPtr->headNodePtr = headNodePtr->nextNodePtr;
    //节点赋值
    elementPtr = headNodePtr->nodeData;
    //释放删除的这个节点
    ADT_MEM_FREE(ptrListPtr,headNodePtr);
    //节点数量减少
    ptrListPtr->currentSize -= 1; 
    //系统解锁
    ADT_SYS_UNLOCK(ptrListPtr);
    return elementPtr;
}

//链表反转
ADT_T ADT_PtrListReverse(ADT_T ptrListPtr)
{
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //指针检查
    ADT_BASE_PTR_CHECK(ptrListPtr,ADT_PtrListReverse);
    //ID检查
    ADT_BASE_ID_CHECK(ptrListPtr,ADT_PtrListReverse,ADT_PTR_LIST_T_ID);
    //没有数据元素,直接返回空
    if(ptrListPtr->currentSize == 0)
    {
        return ptrListPtr;
    }
    //系统锁定
    ADT_SYS_LOCK(ptrListPtr);
    //头节点开始
    ADT_LIST_NODE_T listNodePtr = ptrListPtr->headNodePtr;
    ADT_LIST_NODE_T headNodePtr = NULL, nextNodePtr;
    //利用了两个指针list和next来遍历链表一次，
    //并使用这两个指针将链表就地反转，head总是指向反转后链表的第一个结点
    for (; listNodePtr; listNodePtr = nextNodePtr) 
    {
        nextNodePtr = listNodePtr->nextNodePtr;
        listNodePtr->nextNodePtr = headNodePtr;
        headNodePtr = listNodePtr;
    }
    ptrListPtr->headNodePtr = headNodePtr;
    while(headNodePtr->nextNodePtr != NULL)
    {
        headNodePtr = headNodePtr->nextNodePtr;
    }
    //反转完成解锁
    ADT_SYS_UNLOCK(ptrListPtr);
    return ptrListPtr;
}

//节点遍历
void ADT_PtrListTraverse(ADT_T ptrListPtr,ADT_AdvanceElementOperation opFuncPtr,void* paramPtr)
{
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //指针检查
    ADT_BASE_PTR_CHECK(ptrListPtr,ADT_PtrListTraverse);
    //ID检查
    ADT_BASE_ID_CHECK(ptrListPtr,ADT_PtrListTraverse,ADT_PTR_LIST_T_ID);
    //没有数据元素,直接返回空
    if(ptrListPtr->currentSize == 0)
    {
        return;
    }
    if(opFuncPtr == NULL)
    {
        return;
    }
    //系统锁定
    ADT_SYS_LOCK(ptrListPtr);
    //头节点开始
    ADT_LIST_NODE_T headNodePtr = ptrListPtr->headNodePtr;
    //头节点执行操作
    opFuncPtr(&(headNodePtr->nodeData),paramPtr);
    //循环调度
    while(headNodePtr->nextNodePtr != NULL)
    {
        headNodePtr = headNodePtr->nextNodePtr;
        opFuncPtr(&(headNodePtr->nodeData),paramPtr);
    }
    //遍历完成,解锁
    ADT_SYS_UNLOCK(ptrListPtr);
    return;
}

//获取指针链表当前有效数据容量
uint32_t ADT_PtrListGetCurrentSize(ADT_T ptrListPtr)
{
    //指针检查
    ADT_BASE_PTR_CHECK(ptrListPtr,ADT_PtrListGetCurrentSize);
    //ID检查
    ADT_BASE_ID_CHECK(ptrListPtr,ADT_PtrListGetCurrentSize,ADT_PTR_LIST_T_ID);
    //返回当前尺寸
    return ptrListPtr->currentSize;
}

//链表转换为指针数组
void** ADT_PtrListToArray(ADT_T ptrListPtr,uint16_t* elementLength)
{
    uint16_t indexUtil = 0;
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //指针检查
    ADT_BASE_PTR_CHECK(ptrListPtr,ADT_PtrListToArray);
    //ID检查
    ADT_BASE_ID_CHECK(ptrListPtr,ADT_PtrListToArray,ADT_PTR_LIST_T_ID);
    //没有数据元素,直接返回空
    if(ptrListPtr->currentSize == 0)
    {
        *elementLength = 0;
        return NULL;
    }
    //系统锁定
    ADT_SYS_LOCK(ptrListPtr);
    //数组内存申请
    void **arrayElement = ptrListPtr->memMallocFunc(ptrListPtr->currentSize*sizeof(void*));
    *elementLength = ptrListPtr->currentSize;
    //头节点开始
    ADT_LIST_NODE_T headNodePtr = ptrListPtr->headNodePtr;
    //头节点执行操作
    arrayElement[indexUtil] = headNodePtr->nodeData;
    //循环调度
    while(headNodePtr->nextNodePtr != NULL)
    {
        indexUtil += 1;
        headNodePtr = headNodePtr->nextNodePtr;
        arrayElement[indexUtil] = headNodePtr->nodeData;
    }
    //遍历完成,解锁
    ADT_SYS_UNLOCK(ptrListPtr);
    return arrayElement;
}

//释放指针数组
void ADT_PtrListFreeArray(ADT_T ptrListPtr,void* arrayPtr)
{
    //指针检查
    ADT_BASE_PTR_CHECK(ptrListPtr,ADT_PtrListToArray);
    //ID检查
    ADT_BASE_ID_CHECK(ptrListPtr,ADT_PtrListToArray,ADT_PTR_LIST_T_ID);
    //没有数据元素,直接返回空
    if(arrayPtr == NULL)
    {
        return;
    }
    //释放内存
    ptrListPtr->memFreeFunc(arrayPtr);
}