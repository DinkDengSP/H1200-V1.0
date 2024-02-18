/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-01-29 14:46:45 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-02-06 14:50:50 +0800
************************************************************************************************/ 
#include "ADT_CircularPtrQueue.h"

//宏定义结构体类型,这种定义的好处是不用出现大量的各种类型名,代码整洁
//理解上差点意思,但是使用者不需要太了解内部结构
#define ADT_T ADT_CIRCULAR_PTR_QUEUE_T
//结构体的类型ID,用于程序运行时的检查
//_COUNTER_  一个从0开始的全局的计数器，每次用于源文件或在源文件中被引用头文件时，自动加1
#define ADT_CIRCULAR_PTR_QUEUE_T_ID         0X20200713

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
    void** queueArrayPtr;       //循环队列缓冲区
    uint16_t frontPos;          //循环队列头,下一次取数据从该位置去取
    uint16_t rearPos;           //循环队列位,插入数据的时候看这个数据
    uint16_t maxSize;           //队列最大容量
    uint16_t currentSize;       //当前有效数据长度
};

//循环队列初始化
ADT_T ADT_CircularPtrQueueCreate(uint16_t bufferSize,ADT_MemMallocFuncPtr memMallocFunc,ADT_MemFreeFuncPtr memFreeFunc,
                                ADT_LockSystem lockSysFuncPtr,ADT_UnLockSystem unlockSysFuncPtr,ADT_SendString sendStrFunc)
{
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //回调函数检查
    ADT_CALL_BACK_CHECK(ADT_CircularPtrQueueCreate,memMallocFunc,memFreeFunc,lockSysFuncPtr,unlockSysFuncPtr,sendStrFunc);
    //参数检查
    if(bufferSize == 0)
    {
        sendStrFunc("Error!! ADT_CircularPtrQueueCreate bufferSize 0!!\r\n");
        return NULL;
    }
    //定义实体
    ADT_T circularPtrQueuePtr = NULL;
    //实体内存申请
    ADT_MALLOC_SELF(circularPtrQueuePtr,memMallocFunc);
    //函数映射
    ADT_CALL_BACK_REGISTER(circularPtrQueuePtr,memMallocFunc,memFreeFunc,lockSysFuncPtr,unlockSysFuncPtr,sendStrFunc);
    //数据缓存申请
    circularPtrQueuePtr->queueArrayPtr = (void**)ADT_MEM_MALLOC(circularPtrQueuePtr,bufferSize*sizeof(void*));
    //系统锁定
    ADT_SYS_LOCK(circularPtrQueuePtr);
    //数据复位
    circularPtrQueuePtr->frontPos = 0;
    circularPtrQueuePtr->rearPos = 0;
    circularPtrQueuePtr->maxSize = bufferSize;
    circularPtrQueuePtr->currentSize = 0;
    //功能辅助
    ADT_SET_IDENTIFY_CODE_SET(circularPtrQueuePtr,ADT_CIRCULAR_PTR_QUEUE_T_ID);
    //系统解锁
    ADT_SYS_UNLOCK(circularPtrQueuePtr);
    //返回结构体指针
    return circularPtrQueuePtr;
}

//释放循环队列
void ADT_CircularPtrQueueFree(ADT_T* circularPtrQueuePtrPtr)
{
    //指针检查
    ADT_BASE_PTR_CHECK(circularPtrQueuePtrPtr,ADT_CircularPtrQueueFree);
    //定义实体
    ADT_T circularPtrQueuePtr = (*circularPtrQueuePtrPtr);
    //ID检查
    ADT_BASE_ID_CHECK(circularPtrQueuePtr,ADT_CircularByteQueueFree,ADT_CIRCULAR_PTR_QUEUE_T_ID);
    //释放缓存
    ADT_MEM_FREE(circularPtrQueuePtr,circularPtrQueuePtr->queueArrayPtr);
    //释放自身
    ADT_FREE_SELF(circularPtrQueuePtr);
    return;
}

//获取队列最大容量
uint16_t ADT_CircularPtrQueueGetCapacity(ADT_T circularPtrQueuePtr)
{
    //指针检查
    ADT_BASE_PTR_CHECK(circularPtrQueuePtr,ADT_CircularPtrQueueGetCapacity);
    //ID检查
    ADT_BASE_ID_CHECK(circularPtrQueuePtr,ADT_CircularByteQueueFree,ADT_CIRCULAR_PTR_QUEUE_T_ID);
    //容量返回
    return circularPtrQueuePtr->maxSize;
}

//获取队列当前总有效数据量
uint16_t ADT_CircularPtrQueueGetCurrentSize(ADT_T circularPtrQueuePtr)
{
    //指针检查
    ADT_BASE_PTR_CHECK(circularPtrQueuePtr,ADT_CircularPtrQueueGetCurrentSize);
    //ID检查
    ADT_BASE_ID_CHECK(circularPtrQueuePtr,ADT_CircularByteQueueFree,ADT_CIRCULAR_PTR_QUEUE_T_ID);
    //当前尺寸返回
    return circularPtrQueuePtr->currentSize;
}

//往循环队列中写入数据
ADT_RESULT ADT_CircularPtrQueueWrite(ADT_T circularPtrQueuePtr,void* ptr)
{
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //指针检查
    ADT_BASE_PTR_CHECK(circularPtrQueuePtr,ADT_CircularPtrQueueWrite);
    //ID检查
    ADT_BASE_ID_CHECK(circularPtrQueuePtr,ADT_CircularPtrQueueWrite,ADT_CIRCULAR_PTR_QUEUE_T_ID);
    //数据检查
    if(ptr == NULL)
    {
        circularPtrQueuePtr->sendStrFunc("Error!! ADT_CircularPtrQueueWrite ptr NULL!!\r\n");
        return ADT_RESULT_FAIL;
    }
    //系统锁定
    ADT_SYS_LOCK(circularPtrQueuePtr);
    if(circularPtrQueuePtr->maxSize == circularPtrQueuePtr->currentSize)
    {
        //当前数据已满.不接收
        //系统解锁
        ADT_SYS_UNLOCK(circularPtrQueuePtr);
        return ADT_RESULT_FAIL;
    }
    else
    {
        //将数据写入尾部
        circularPtrQueuePtr->queueArrayPtr[circularPtrQueuePtr->rearPos++] = ptr;
        circularPtrQueuePtr->rearPos %= circularPtrQueuePtr->maxSize;
        circularPtrQueuePtr->currentSize++;
        //系统解锁
        ADT_SYS_UNLOCK(circularPtrQueuePtr);
        return ADT_RESULT_OK;
    }
}

//从循环队列中读取数据
void* ADT_CircularPtrQueueGet(ADT_T circularPtrQueuePtr)
{
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    void* readPtr = NULL;
    //指针检查
    ADT_BASE_PTR_CHECK(circularPtrQueuePtr,ADT_CircularPtrQueueGet);
    //ID检查
    ADT_BASE_ID_CHECK(circularPtrQueuePtr,ADT_CircularPtrQueueGet,ADT_CIRCULAR_PTR_QUEUE_T_ID);
    //系统锁定
    ADT_SYS_LOCK(circularPtrQueuePtr);
    if(0 == circularPtrQueuePtr->currentSize)
    {
        //系统解锁
        ADT_SYS_UNLOCK(circularPtrQueuePtr);
        return NULL;
    }
    else
    {
        //从头部读取数据
        readPtr = circularPtrQueuePtr->queueArrayPtr[circularPtrQueuePtr->frontPos++];
        circularPtrQueuePtr->frontPos %= circularPtrQueuePtr->maxSize;
        circularPtrQueuePtr->currentSize--;
        //系统解锁
        ADT_SYS_UNLOCK(circularPtrQueuePtr);
        return readPtr;
    }
}


