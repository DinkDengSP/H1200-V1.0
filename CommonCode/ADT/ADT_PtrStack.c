/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-02-04 23:38:52 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-02-06 16:51:03 +0800
************************************************************************************************/ 
#include "ADT_PtrStack.h"

//宏定义结构体类型,这种定义的好处是不用出现大量的各种类型名,代码整洁
//理解上差点意思,但是使用者不需要太了解内部结构
#define ADT_T ADT_PTR_STACK_T
//结构体的类型ID,用于程序运行时的检查
#define ADT_PTR_STACK_T_ID         0X20200714

/*----------------------------------使用数组实现ADT_STACK-------------------------------------------------*/
#if(ADT_PTR_STACK_USE == ADT_PTR_STACK_USE_ARRAY)
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
    //数据定义
    void** stackDataBufPtr;     //栈缓冲区
    uint32_t maxSize;           //栈最大容量
    uint32_t currentSize;       //当前有效数据长度
    int32_t currentElementIndex;//当前栈顶数据元素的序号
};


//创建字节栈
ADT_T ADT_PtrStackCreate(uint32_t maxCapacity,ADT_MemMallocFuncPtr memMallocFunc,ADT_MemFreeFuncPtr memFreeFunc,
                                ADT_LockSystem lockSysFuncPtr,ADT_UnLockSystem unlockSysFuncPtr,ADT_SendString sendStrFunc)
{
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //回调函数检查
    ADT_CALL_BACK_CHECK(ADT_PtrStackCreate,memMallocFunc,memFreeFunc,lockSysFuncPtr,unlockSysFuncPtr,sendStrFunc);
    //参数检查
    if(maxCapacity == 0)
    {
        sendStrFunc("Error!! ADT_PtrStackCreate bufferSize 0!!\r\n");
        return NULL;
    }
    ADT_T ptrStackPtr = NULL;
    //内存申请
    ADT_MALLOC_SELF(ptrStackPtr,memMallocFunc);
    //函数映射
    ADT_CALL_BACK_REGISTER(ptrStackPtr,memMallocFunc,memFreeFunc,lockSysFuncPtr,unlockSysFuncPtr,sendStrFunc);
    //数据缓存申请
    ptrStackPtr->stackDataBufPtr = (void**)ADT_MEM_MALLOC(ptrStackPtr,maxCapacity*sizeof(void*));
    //系统锁定
    ADT_SYS_LOCK(ptrStackPtr);
    //数据复位
    ptrStackPtr->maxSize = maxCapacity;
    ptrStackPtr->currentSize = 0;
    ptrStackPtr->currentElementIndex = 1;
    //功能辅助
    ADT_SET_IDENTIFY_CODE_SET(ptrStackPtr,ADT_PTR_STACK_T_ID);
    //系统解锁
    ADT_SYS_UNLOCK(ptrStackPtr);
    //返回结构体指针
    return ptrStackPtr;
}

//销毁字节栈
void ADT_PtrStackFree(ADT_T *ptrStackPtrPtr)
{
    //指针检查
    ADT_BASE_PTR_CHECK(ptrStackPtrPtr,ADT_PtrStackFree);
    //定义实体
    ADT_T ptrStackPtr = (*ptrStackPtrPtr);
    //ID检查
    ADT_BASE_ID_CHECK(ptrStackPtr,ADT_PtrStackFree,ADT_PTR_STACK_T_ID);
    //释放缓存
    ADT_MEM_FREE(ptrStackPtr,ptrStackPtr->stackDataBufPtr);
    //释放自身
    ADT_FREE_SELF(ptrStackPtr);
    return;
}

//检查字节栈是否为空
ADT_BOOL ADT_PtrStackCheckIsEmpty(ADT_T ptrStackPtr)
{
    //指针检查
    ADT_BASE_PTR_CHECK(ptrStackPtr,ADT_PtrStackCheckIsEmpty);
    //ID检查
    ADT_BASE_ID_CHECK(ptrStackPtr,ADT_PtrStackCheckIsEmpty,ADT_PTR_STACK_T_ID);
    //数据检查
    if(ptrStackPtr->currentSize == 0)
    {
        return ADT_BOOL_TRUE;
    }
    else
    {
        return ADT_BOOL_FALSE;
    }
}

//检查字节栈是否为满
ADT_BOOL ADT_PtrStackCheckIsFull(ADT_T ptrStackPtr)
{
    //指针检查
    ADT_BASE_PTR_CHECK(ptrStackPtr,ADT_PtrStackCheckIsFull);
    //ID检查
    ADT_BASE_ID_CHECK(ptrStackPtr,ADT_PtrStackCheckIsFull,ADT_PTR_STACK_T_ID);
    //数据检查
    if(ptrStackPtr->currentSize == ptrStackPtr->maxSize )
    {
        return ADT_BOOL_TRUE;
    }
    else
    {
        return ADT_BOOL_FALSE;
    }
}

//获取字节栈最大容量
uint32_t ADT_PtrStackGetMaxCapacity(ADT_T ptrStackPtr)
{
    //指针检查
    ADT_BASE_PTR_CHECK(ptrStackPtr,ADT_PtrStackGetMaxCapacity);
    //ID检查
    ADT_BASE_ID_CHECK(ptrStackPtr,ADT_PtrStackGetMaxCapacity,ADT_PTR_STACK_T_ID);
    //返回容量
    return ptrStackPtr->maxSize;
}

//获取字节栈当前有效数据容量
uint32_t ADT_PtrStackGetCurrentSize(ADT_T ptrStackPtr)
{
    //指针检查
    ADT_BASE_PTR_CHECK(ptrStackPtr,ADT_PtrStackGetCurrentSize);
    //ID检查
    ADT_BASE_ID_CHECK(ptrStackPtr,ADT_PtrStackGetCurrentSize,ADT_PTR_STACK_T_ID);
    //返回当前尺寸
    return ptrStackPtr->currentSize;
}

//数据压栈
ADT_RESULT ADT_PtrStackPush(ADT_T ptrStackPtr, void* pushDataPtr)
{
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //指针检查
    ADT_BASE_PTR_CHECK(ptrStackPtr,ADT_PtrStackPush);
    //ID检查
    ADT_BASE_ID_CHECK(ptrStackPtr,ADT_PtrStackPush,ADT_PTR_STACK_T_ID);
    //数据检查
    if(pushDataPtr == NULL)
    {
        ptrStackPtr->sendStrFunc("Error!! ADT_PtrStackPush pushDataPtr NULL!!\r\n");
        return ADT_RESULT_FAIL;
    }
    //系统锁定
    ADT_SYS_LOCK(ptrStackPtr);
    //检查队列是否满
    if(ADT_BOOL_TRUE == ADT_PtrStackCheckIsFull(ptrStackPtr))
    {
        //系统解锁
        ADT_SYS_UNLOCK(ptrStackPtr);
        return ADT_RESULT_FAIL;
    }
    //压栈
    ptrStackPtr->stackDataBufPtr[ptrStackPtr->currentElementIndex + 1] = pushDataPtr;
    //计数增加
    ptrStackPtr->currentElementIndex += 1;
    ptrStackPtr->currentSize += 1;
    //系统解锁
    ADT_SYS_UNLOCK(ptrStackPtr);
    return ADT_RESULT_OK;
}

//数据出栈
ADT_RESULT ADT_PtrStackPop(ADT_T ptrStackPtr,void** popDataPtrPtr)
{
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //指针检查
    ADT_BASE_PTR_CHECK(ptrStackPtr,ADT_PtrStackPop);
    //ID检查
    ADT_BASE_ID_CHECK(ptrStackPtr,ADT_PtrStackPop,ADT_PTR_STACK_T_ID);
    //系统锁定
    ADT_SYS_LOCK(ptrStackPtr);
    //检查队列是否空
    if(ADT_BOOL_TRUE == ADT_PtrStackCheckIsEmpty(ptrStackPtr))
    {
        //系统解锁
        ADT_SYS_UNLOCK(ptrStackPtr);
        return ADT_RESULT_FAIL;
    }
    *popDataPtrPtr = ptrStackPtr->stackDataBufPtr[ptrStackPtr->currentElementIndex];
    ptrStackPtr->currentElementIndex -= 1;
    ptrStackPtr->currentSize -= 1;
    //系统解锁
    ADT_SYS_UNLOCK(ptrStackPtr);
    return ADT_RESULT_OK;
}




#endif

/*----------------------------------使用链表实现ADT_STACK-------------------------------------------------*/
#if(ADT_PTR_STACK_USE == ADT_PTR_STACK_USE_LIST)
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
    uint32_t maxSize;           //栈最大容量
    uint32_t currentSize;       //当前有效数据长度
    //节点数据定义
    struct ListNode{
		void* itemValue;
		struct ListNode* nextNodePtr;
	} *headNodePtr;
};

//创建字节栈
ADT_T ADT_PtrStackCreate(uint32_t maxCapacity,ADT_MemMallocFuncPtr memMallocFunc,ADT_MemFreeFuncPtr memFreeFunc,
                                ADT_LockSystem lockSysFuncPtr,ADT_UnLockSystem unlockSysFuncPtr,ADT_SendString sendStrFunc)
{
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //回调函数检查
    ADT_CALL_BACK_CHECK(ADT_PtrStackCreate,memMallocFunc,memFreeFunc,lockSysFuncPtr,unlockSysFuncPtr,sendStrFunc);
    //参数检查
    if(maxCapacity == 0)
    {
        sendStrFunc("Error!! ADT_PtrStackCreate bufferSize 0!!\r\n");
        return NULL;
    }
    ADT_T ptrStackPtr = NULL;
    //内存申请
    ADT_MALLOC_SELF(ptrStackPtr,memMallocFunc);
    //函数映射
    ADT_CALL_BACK_REGISTER(ptrStackPtr,memMallocFunc,memFreeFunc,lockSysFuncPtr,unlockSysFuncPtr,sendStrFunc);
    //系统锁定
    ADT_SYS_LOCK(ptrStackPtr);
    //数据复位
    ptrStackPtr->maxSize = maxCapacity;
    ptrStackPtr->currentSize = 0;
    ptrStackPtr->headNodePtr = NULL;
    //功能辅助
    ADT_SET_IDENTIFY_CODE_SET(ptrStackPtr,ADT_PTR_STACK_T_ID);
    //系统解锁
    ADT_SYS_UNLOCK(ptrStackPtr);
    //返回结构体指针
    return ptrStackPtr;
}

//销毁字节栈
void ADT_PtrStackFree(ADT_T *ptrStackPtrPtr)
{
    //指针检查
    ADT_BASE_PTR_CHECK(ptrStackPtrPtr,ADT_PtrStackFree);
    //定义实体
    ADT_T ptrStackPtr = (*ptrStackPtrPtr);
    //ID检查
    ADT_BASE_ID_CHECK(ptrStackPtr,ADT_PtrStackFree,ADT_PTR_STACK_T_ID);
    void* popDataVal = NULL;
    //弹出所有元素
    while(ADT_RESULT_OK == ADT_PtrStackPop(ptrStackPtr,&popDataVal));
    //释放自身
    ADT_FREE_SELF(ptrStackPtr);
    return;
}


//检查字节栈是否为空
ADT_BOOL ADT_PtrStackCheckIsEmpty(ADT_T ptrStackPtr)
{
    //指针检查
    ADT_BASE_PTR_CHECK(ptrStackPtr,ADT_PtrStackCheckIsEmpty);
    //ID检查
    ADT_BASE_ID_CHECK(ptrStackPtr,ADT_PtrStackCheckIsEmpty,ADT_PTR_STACK_T_ID);
    //数据检查
    if(ptrStackPtr->currentSize == 0)
    {
        return ADT_BOOL_TRUE;
    }
    else
    {
        return ADT_BOOL_FALSE;
    }
}


//检查字节栈是否为满
ADT_BOOL ADT_PtrStackCheckIsFull(ADT_T ptrStackPtr)
{
    //指针检查
    ADT_BASE_PTR_CHECK(ptrStackPtr,ADT_PtrStackCheckIsEmpty);
    //ID检查
    ADT_BASE_ID_CHECK(ptrStackPtr,ADT_PtrStackCheckIsEmpty,ADT_PTR_STACK_T_ID);
    //数据检查
    if(ptrStackPtr->currentSize == ptrStackPtr->maxSize)
    {
        return ADT_BOOL_TRUE;
    }
    else
    {
        return ADT_BOOL_FALSE;
    }
}

//获取字节栈最大容量
uint32_t ADT_PtrStackGetMaxCapacity(ADT_T ptrStackPtr)
{
    //指针检查
    ADT_BASE_PTR_CHECK(ptrStackPtr,ADT_PtrStackGetMaxCapacity);
    //ID检查
    ADT_BASE_ID_CHECK(ptrStackPtr,ADT_PtrStackGetMaxCapacity,ADT_PTR_STACK_T_ID);
    //返回容量
    return ptrStackPtr->maxSize;
}

//获取字节栈当前有效数据容量
uint32_t ADT_PtrStackGetCurrentSize(ADT_T ptrStackPtr)
{
    //指针检查
    ADT_BASE_PTR_CHECK(ptrStackPtr,ADT_PtrStackGetCurrentSize);
    //ID检查
    ADT_BASE_ID_CHECK(ptrStackPtr,ADT_PtrStackGetCurrentSize,ADT_PTR_STACK_T_ID);
    //返回当前尺寸
    return ptrStackPtr->currentSize;
}

//数据压栈
ADT_RESULT ADT_PtrStackPush(ADT_T ptrStackPtr, void* pushDataPtr)
{
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //指针检查
    ADT_BASE_PTR_CHECK(ptrStackPtr,ADT_PtrStackPush);
    //ID检查
    ADT_BASE_ID_CHECK(ptrStackPtr,ADT_PtrStackPush,ADT_PTR_STACK_T_ID);
    //数据检查
    if(pushDataPtr == NULL)
    {
        ptrStackPtr->sendStrFunc("Error!! ADT_PtrStackPush pushDataPtr NULL!!\r\n");
        return ADT_RESULT_FAIL;
    }
    //内存申请
    struct ListNode* newElementPtr = (struct ListNode*)ADT_MEM_MALLOC(ptrStackPtr,sizeof(struct ListNode));
    //系统锁定
    ADT_SYS_LOCK(ptrStackPtr);
    //检查队列是否满
    if(ADT_BOOL_TRUE == ADT_PtrStackCheckIsFull(ptrStackPtr))
    {
        //系统解锁
        ADT_SYS_UNLOCK(ptrStackPtr);
        //内存释放
        ADT_MEM_FREE(ptrStackPtr,newElementPtr);
        return ADT_RESULT_FAIL;
    }
    //压栈,设置新节点为新的头节点
    newElementPtr->itemValue = pushDataPtr;
    newElementPtr->nextNodePtr = ptrStackPtr->headNodePtr;
    ptrStackPtr->headNodePtr = newElementPtr;
    //计数增加
    ptrStackPtr->currentSize += 1;
    //系统解锁
    ADT_SYS_UNLOCK(ptrStackPtr);
    return ADT_RESULT_OK;
}

//数据出栈
ADT_RESULT ADT_PtrStackPop(ADT_T ptrStackPtr,void** popDataPtrPtr)
{
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //指针检查
    ADT_BASE_PTR_CHECK(ptrStackPtr,ADT_PtrStackPop);
    //ID检查
    ADT_BASE_ID_CHECK(ptrStackPtr,ADT_PtrStackPop,ADT_PTR_STACK_T_ID);
    //系统锁定
    ADT_SYS_LOCK(ptrStackPtr);
    //检查队列是否空
    if(ADT_BOOL_TRUE == ADT_PtrStackCheckIsEmpty(ptrStackPtr))
    {
        //系统解锁
        ADT_SYS_UNLOCK(ptrStackPtr);
        return ADT_RESULT_FAIL;
    }
    struct ListNode* headNodePtr = ptrStackPtr->headNodePtr;
    *popDataPtrPtr = headNodePtr->itemValue;
    ptrStackPtr->headNodePtr = headNodePtr->nextNodePtr;
    //尺寸降低
    ptrStackPtr->currentSize -= 1;    
    //系统解锁
    ADT_SYS_UNLOCK(ptrStackPtr);
    //释放内存
    ADT_MEM_FREE(ptrStackPtr,headNodePtr);
    return ADT_RESULT_OK;
}
#endif
