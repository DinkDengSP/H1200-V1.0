/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-02-04 23:33:44 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-02-06 15:41:47 +0800
************************************************************************************************/ 
#include "ADT_ByteStack.h"

//宏定义结构体类型,这种定义的好处是不用出现大量的各种类型名,代码整洁
//理解上差点意思,但是使用者不需要太了解内部结构
#define ADT_T ADT_BYTE_STACK_T
//结构体的类型ID,用于程序运行时的检查
#define ADT_BYTE_STACK_T_ID         0X20200711

/*----------------------------------使用数组实现ADT_STACK-------------------------------------------------*/
#if(ADT_BYTE_STACK_USE == ADT_BYTE_STACK_USE_ARRAY)
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
    uint8_t* stackDataBufPtr;   //栈缓冲区
    uint32_t maxSize;           //栈最大容量
    uint32_t currentSize;       //当前有效数据长度
    int32_t currentElementIndex;//当前栈顶数据元素的序号
};


//创建字节栈
ADT_T ADT_ByteStackCreate(uint32_t maxCapacity,ADT_MemMallocFuncPtr memMallocFunc,ADT_MemFreeFuncPtr memFreeFunc,
                                ADT_LockSystem lockSysFuncPtr,ADT_UnLockSystem unlockSysFuncPtr,ADT_SendString sendStrFunc)
{
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //回调函数检查
    ADT_CALL_BACK_CHECK(ADT_ByteStackCreate,memMallocFunc,memFreeFunc,lockSysFuncPtr,unlockSysFuncPtr,sendStrFunc);
    //参数检查
    if(maxCapacity == 0)
    {
        sendStrFunc("Error!! ADT_ByteStackCreate bufferSize 0!!\r\n");
        return NULL;
    }
    ADT_T byteStackPtr = NULL;
    //内存申请
    ADT_MALLOC_SELF(byteStackPtr,memMallocFunc);
    //函数映射
    ADT_CALL_BACK_REGISTER(byteStackPtr,memMallocFunc,memFreeFunc,lockSysFuncPtr,unlockSysFuncPtr,sendStrFunc);
    //数据缓存申请
    byteStackPtr->stackDataBufPtr = (uint8_t*)ADT_MEM_MALLOC(byteStackPtr,maxCapacity*sizeof(uint8_t));
    //系统锁定
    ADT_SYS_LOCK(byteStackPtr);
    //数据复位
    byteStackPtr->maxSize = maxCapacity;
    byteStackPtr->currentSize = 0;
    byteStackPtr->currentElementIndex = 1;
    //功能辅助
    ADT_SET_IDENTIFY_CODE_SET(byteStackPtr,ADT_BYTE_STACK_T_ID);
    //系统解锁
    ADT_SYS_UNLOCK(byteStackPtr);
    //返回结构体指针
    return byteStackPtr;
}

//销毁字节栈
void ADT_ByteStackFree(ADT_T *byteStackPtrPtr)
{
    //指针检查
    ADT_BASE_PTR_CHECK(byteStackPtrPtr,ADT_ByteStackFree);
    //定义实体
    ADT_T byteStackPtr = (*byteStackPtrPtr);
    //ID检查
    ADT_BASE_ID_CHECK(byteStackPtr,ADT_ByteStackFree,ADT_BYTE_STACK_T_ID);
    //释放缓存
    ADT_MEM_FREE(byteStackPtr,byteStackPtr->stackDataBufPtr);
    //释放自身
    ADT_FREE_SELF(byteStackPtr);
    return;
}

//检查字节栈是否为空
ADT_BOOL ADT_ByteStackCheckIsEmpty(ADT_T byteStackPtr)
{
    //指针检查
    ADT_BASE_PTR_CHECK(byteStackPtr,ADT_ByteStackCheckIsEmpty);
    //ID检查
    ADT_BASE_ID_CHECK(byteStackPtr,ADT_ByteStackCheckIsEmpty,ADT_BYTE_STACK_T_ID);
    //数据检查
    if(byteStackPtr->currentSize == 0)
    {
        return ADT_BOOL_TRUE;
    }
    else
    {
        return ADT_BOOL_FALSE;
    }
}

//检查字节栈是否为满
ADT_BOOL ADT_ByteStackCheckIsFull(ADT_T byteStackPtr)
{
    //指针检查
    ADT_BASE_PTR_CHECK(byteStackPtr,ADT_ByteStackCheckIsFull);
    //ID检查
    ADT_BASE_ID_CHECK(byteStackPtr,ADT_ByteStackCheckIsFull,ADT_BYTE_STACK_T_ID);
    //数据检查
    if(byteStackPtr->currentSize == byteStackPtr->maxSize )
    {
        return ADT_BOOL_TRUE;
    }
    else
    {
        return ADT_BOOL_FALSE;
    }
}

//获取字节栈最大容量
uint32_t ADT_ByteStackGetMaxCapacity(ADT_T byteStackPtr)
{
    //指针检查
    ADT_BASE_PTR_CHECK(byteStackPtr,ADT_ByteStackGetMaxCapacity);
    //ID检查
    ADT_BASE_ID_CHECK(byteStackPtr,ADT_ByteStackGetMaxCapacity,ADT_BYTE_STACK_T_ID);
    //返回容量
    return byteStackPtr->maxSize;
}

//获取字节栈当前有效数据容量
uint32_t ADT_ByteStackGetCurrentSize(ADT_T byteStackPtr)
{
    //指针检查
    ADT_BASE_PTR_CHECK(byteStackPtr,ADT_ByteStackGetCurrentSize);
    //ID检查
    ADT_BASE_ID_CHECK(byteStackPtr,ADT_ByteStackGetCurrentSize,ADT_BYTE_STACK_T_ID);
    //返回当前尺寸
    return byteStackPtr->currentSize;
}

//数据压栈
ADT_RESULT ADT_ByteStackPush(ADT_T byteStackPtr, uint8_t pushDataByte)
{
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //指针检查
    ADT_BASE_PTR_CHECK(byteStackPtr,ADT_ByteStackPush);
    //ID检查
    ADT_BASE_ID_CHECK(byteStackPtr,ADT_ByteStackPush,ADT_BYTE_STACK_T_ID);
    //系统锁定
    ADT_SYS_LOCK(byteStackPtr);
    //检查队列是否满
    if(ADT_BOOL_TRUE == ADT_ByteStackCheckIsFull(byteStackPtr))
    {
        //系统解锁
        ADT_SYS_UNLOCK(byteStackPtr);
        return ADT_RESULT_FAIL;
    }
    //压栈
    byteStackPtr->stackDataBufPtr[byteStackPtr->currentElementIndex + 1] = pushDataByte;
    //计数增加
    byteStackPtr->currentElementIndex += 1;
    byteStackPtr->currentSize += 1;
    //系统解锁
    ADT_SYS_UNLOCK(byteStackPtr);
    return ADT_RESULT_OK;
}

//数据出栈
ADT_RESULT ADT_ByteStackPop(ADT_T byteStackPtr,uint8_t* popDataBytePtr)
{
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //指针检查
    ADT_BASE_PTR_CHECK(byteStackPtr,ADT_ByteStackPop);
    //ID检查
    ADT_BASE_ID_CHECK(byteStackPtr,ADT_ByteStackPop,ADT_BYTE_STACK_T_ID);
    //系统锁定
    ADT_SYS_LOCK(byteStackPtr);
    //检查队列是否空
    if(ADT_BOOL_TRUE == ADT_ByteStackCheckIsEmpty(byteStackPtr))
    {
        //系统解锁
        ADT_SYS_UNLOCK(byteStackPtr);
        return ADT_RESULT_FAIL;
    }
    *popDataBytePtr = byteStackPtr->stackDataBufPtr[byteStackPtr->currentElementIndex];
    byteStackPtr->currentElementIndex -= 1;
    byteStackPtr->currentSize -= 1;
    //系统解锁
    ADT_SYS_UNLOCK(byteStackPtr);
    return ADT_RESULT_OK;
}




#endif

/*----------------------------------使用链表实现ADT_STACK-------------------------------------------------*/
#if(ADT_BYTE_STACK_USE == ADT_BYTE_STACK_USE_LIST)
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
		uint8_t itemValue;
		struct ListNode* nextNodePtr;
	} *headNodePtr;
};

//创建字节栈
ADT_T ADT_ByteStackCreate(uint32_t maxCapacity,ADT_MemMallocFuncPtr memMallocFunc,ADT_MemFreeFuncPtr memFreeFunc,
                                ADT_LockSystem lockSysFuncPtr,ADT_UnLockSystem unlockSysFuncPtr,ADT_SendString sendStrFunc)
{
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //回调函数检查
    ADT_CALL_BACK_CHECK(ADT_ByteStackCreate,memMallocFunc,memFreeFunc,lockSysFuncPtr,unlockSysFuncPtr,sendStrFunc);
    //参数检查
    if(maxCapacity == 0)
    {
        sendStrFunc("Error!! ADT_ByteStackCreate bufferSize 0!!\r\n");
        return NULL;
    }
    ADT_T byteStackPtr = NULL;
    //内存申请
    ADT_MALLOC_SELF(byteStackPtr,memMallocFunc);
    //函数映射
    ADT_CALL_BACK_REGISTER(byteStackPtr,memMallocFunc,memFreeFunc,lockSysFuncPtr,unlockSysFuncPtr,sendStrFunc);
    //系统锁定
    ADT_SYS_LOCK(byteStackPtr);
    //数据复位
    byteStackPtr->maxSize = maxCapacity;
    byteStackPtr->currentSize = 0;
    byteStackPtr->headNodePtr = NULL;
    //功能辅助
    ADT_SET_IDENTIFY_CODE_SET(byteStackPtr,ADT_BYTE_STACK_T_ID);
    //系统解锁
    ADT_SYS_UNLOCK(byteStackPtr);
    //返回结构体指针
    return byteStackPtr;
}

//销毁字节栈
void ADT_ByteStackFree(ADT_T *byteStackPtrPtr)
{
    //指针检查
    ADT_BASE_PTR_CHECK(byteStackPtrPtr,ADT_ByteStackFree);
    //定义实体
    ADT_T byteStackPtr = (*byteStackPtrPtr);
    //ID检查
    ADT_BASE_ID_CHECK(byteStackPtr,ADT_ByteStackFree,ADT_BYTE_STACK_T_ID);
    uint8_t popDataVal = NULL;
    //弹出所有元素
    while(ADT_RESULT_OK == ADT_ByteStackPop(byteStackPtr,&popDataVal));
    //释放自身
    ADT_FREE_SELF(byteStackPtr);
    return;
}


//检查字节栈是否为空
ADT_BOOL ADT_ByteStackCheckIsEmpty(ADT_T byteStackPtr)
{
    //指针检查
    ADT_BASE_PTR_CHECK(byteStackPtr,ADT_ByteStackCheckIsEmpty);
    //ID检查
    ADT_BASE_ID_CHECK(byteStackPtr,ADT_ByteStackCheckIsEmpty,ADT_BYTE_STACK_T_ID);
    //数据检查
    if(byteStackPtr->currentSize == 0)
    {
        return ADT_BOOL_TRUE;
    }
    else
    {
        return ADT_BOOL_FALSE;
    }
}


//检查字节栈是否为满
ADT_BOOL ADT_ByteStackCheckIsFull(ADT_T byteStackPtr)
{
    //指针检查
    ADT_BASE_PTR_CHECK(byteStackPtr,ADT_ByteStackCheckIsEmpty);
    //ID检查
    ADT_BASE_ID_CHECK(byteStackPtr,ADT_ByteStackCheckIsEmpty,ADT_BYTE_STACK_T_ID);
    //数据检查
    if(byteStackPtr->currentSize == byteStackPtr->maxSize)
    {
        return ADT_BOOL_TRUE;
    }
    else
    {
        return ADT_BOOL_FALSE;
    }
}

//获取字节栈最大容量
uint32_t ADT_ByteStackGetMaxCapacity(ADT_T byteStackPtr)
{
    //指针检查
    ADT_BASE_PTR_CHECK(byteStackPtr,ADT_ByteStackGetMaxCapacity);
    //ID检查
    ADT_BASE_ID_CHECK(byteStackPtr,ADT_ByteStackGetMaxCapacity,ADT_BYTE_STACK_T_ID);
    //返回容量
    return byteStackPtr->maxSize;
}

//获取字节栈当前有效数据容量
uint32_t ADT_ByteStackGetCurrentSize(ADT_T byteStackPtr)
{
    //指针检查
    ADT_BASE_PTR_CHECK(byteStackPtr,ADT_ByteStackGetCurrentSize);
    //ID检查
    ADT_BASE_ID_CHECK(byteStackPtr,ADT_ByteStackGetCurrentSize,ADT_BYTE_STACK_T_ID);
    //返回当前尺寸
    return byteStackPtr->currentSize;
}

//数据压栈
ADT_RESULT ADT_ByteStackPush(ADT_T byteStackPtr, uint8_t pushDataByte)
{
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //指针检查
    ADT_BASE_PTR_CHECK(byteStackPtr,ADT_ByteStackPush);
    //ID检查
    ADT_BASE_ID_CHECK(byteStackPtr,ADT_ByteStackPush,ADT_BYTE_STACK_T_ID);
    //内存申请
    struct ListNode* newElementPtr = (struct ListNode*)ADT_MEM_MALLOC(byteStackPtr,sizeof(struct ListNode));
    //系统锁定
    ADT_SYS_LOCK(byteStackPtr);
    //检查队列是否满
    if(ADT_BOOL_TRUE == ADT_ByteStackCheckIsFull(byteStackPtr))
    {
        //系统解锁
        ADT_SYS_UNLOCK(byteStackPtr);
        //内存释放
        ADT_MEM_FREE(byteStackPtr,newElementPtr);
        return ADT_RESULT_FAIL;
    }
    //压栈,设置新节点为新的头节点
    newElementPtr->itemValue = pushDataByte;
    newElementPtr->nextNodePtr = byteStackPtr->headNodePtr;
    byteStackPtr->headNodePtr = newElementPtr;
    //计数增加
    byteStackPtr->currentSize += 1;
    //系统解锁
    ADT_SYS_UNLOCK(byteStackPtr);
    return ADT_RESULT_OK;
}

//数据出栈
ADT_RESULT ADT_ByteStackPop(ADT_T byteStackPtr,uint8_t* popDataBytePtr)
{
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //指针检查
    ADT_BASE_PTR_CHECK(byteStackPtr,ADT_ByteStackPop);
    //ID检查
    ADT_BASE_ID_CHECK(byteStackPtr,ADT_ByteStackPop,ADT_BYTE_STACK_T_ID);
    //系统锁定
    ADT_SYS_LOCK(byteStackPtr);
    //检查队列是否空
    if(ADT_BOOL_TRUE == ADT_ByteStackCheckIsEmpty(byteStackPtr))
    {
        //系统解锁
        ADT_SYS_UNLOCK(byteStackPtr);
        return ADT_RESULT_FAIL;
    }
    struct ListNode* headNodePtr = byteStackPtr->headNodePtr;
    *popDataBytePtr = headNodePtr->itemValue;
    byteStackPtr->headNodePtr = headNodePtr->nextNodePtr;
    //尺寸降低
    byteStackPtr->currentSize -= 1;    
    //系统解锁
    ADT_SYS_UNLOCK(byteStackPtr);
    //释放内存
    ADT_MEM_FREE(byteStackPtr,headNodePtr);
    return ADT_RESULT_OK;
}
#endif


