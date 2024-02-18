/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-01-29 14:42:27 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-02-06 14:08:57 +0800
************************************************************************************************/ 
#include "ADT_CircularByteQueue.h"

//宏定义结构体类型,这种定义的好处是不用出现大量的各种类型名,代码整洁
//理解上差点意思,但是使用者不需要太了解内部结构
#define ADT_T ADT_CIRCULAR_BYTE_QUEUE_T
//结构体的类型ID,用于程序运行时的检查
//_COUNTER_  一个从0开始的全局的计数器，每次用于源文件或在源文件中被引用头文件时，自动加1
#define ADT_CIRCULAR_BYTE_QUEUE_T_ID         0X20200712

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
    uint8_t *queueArrayBuffer; //循环队列缓冲区
    uint16_t frontPos;         //循环队列头,下一次取数据从该位置去取
    uint16_t rearPos;          //循环队列位,插入数据的时候看这个数据
    uint16_t maxSize;          //队列最大容量
    uint16_t currentSize;      //当前有效数据长度
};

//循环队列初始化,这里的ADT是一个数据类型,是H文件中定义的指向ADT_CIRCULAR_BYTE_QUEUE_T的指针
ADT_T ADT_CircularByteQueueCreate(uint16_t bufferSize,ADT_MemMallocFuncPtr memMallocFunc,ADT_MemFreeFuncPtr memFreeFunc,
                                ADT_LockSystem lockSysFuncPtr,ADT_UnLockSystem unlockSysFuncPtr,ADT_SendString sendStrFunc)
{
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //回调函数检查
    ADT_CALL_BACK_CHECK(ADT_CircularByteQueueCreate,memMallocFunc,memFreeFunc,lockSysFuncPtr,unlockSysFuncPtr,sendStrFunc);
    //参数检查
    if(bufferSize == 0)
    {
        sendStrFunc("Error!! ADT_CircularByteQueueCreate bufferSize 0!!\r\n");
        return NULL;
    }
    //定义实体
    ADT_T circularByteQueuePtr = NULL;
    //实体内存申请
    ADT_MALLOC_SELF(circularByteQueuePtr,memMallocFunc);
    //函数映射
    ADT_CALL_BACK_REGISTER(circularByteQueuePtr,memMallocFunc,memFreeFunc,lockSysFuncPtr,unlockSysFuncPtr,sendStrFunc);
    //数据缓存申请
    circularByteQueuePtr->queueArrayBuffer = (uint8_t*)ADT_MEM_MALLOC(circularByteQueuePtr,bufferSize*sizeof(uint8_t));
    //系统锁定
    ADT_SYS_LOCK(circularByteQueuePtr);
    //数据复位
    circularByteQueuePtr->frontPos = 0;
    circularByteQueuePtr->rearPos = 0;
    circularByteQueuePtr->maxSize = bufferSize;
    circularByteQueuePtr->currentSize = 0;
    //功能辅助
    ADT_SET_IDENTIFY_CODE_SET(circularByteQueuePtr,ADT_CIRCULAR_BYTE_QUEUE_T_ID);
    //系统解锁
    ADT_SYS_UNLOCK(circularByteQueuePtr);
    //返回结构体指针
    return circularByteQueuePtr;
}

//释放申请的内存
void ADT_CircularByteQueueFree(ADT_T* circularByteQueuePtrPtr)
{
    //指针检查
    ADT_BASE_PTR_CHECK(circularByteQueuePtrPtr,ADT_CircularByteQueueFree);
    //定义实体
    ADT_T circularByteQueuePtr = (*circularByteQueuePtrPtr);
    //ID检查
    ADT_BASE_ID_CHECK(circularByteQueuePtr,ADT_CircularByteQueueFree,ADT_CIRCULAR_BYTE_QUEUE_T_ID);
    //释放缓存
    ADT_MEM_FREE(circularByteQueuePtr,circularByteQueuePtr->queueArrayBuffer);
    //释放自身
    ADT_FREE_SELF(circularByteQueuePtr);
    return;
}

//获取循环队列最大容量
uint16_t ADT_CircularByteQueueGetMaxCapacity(ADT_T circularByteQueuePtr)
{
    //指针检查
    ADT_BASE_PTR_CHECK(circularByteQueuePtr,ADT_CircularByteQueueGetMaxCapacity);
    //ID检查
    ADT_BASE_ID_CHECK(circularByteQueuePtr,ADT_CircularByteQueueGetMaxCapacity,ADT_CIRCULAR_BYTE_QUEUE_T_ID);
    //容量返回
    return circularByteQueuePtr->maxSize;
}

//获取当前循环队列已经存储的数据
uint16_t ADT_CircularByteQueueGetCurrentSize(ADT_T circularByteQueuePtr)
{
    //指针检查
    ADT_BASE_PTR_CHECK(circularByteQueuePtr,ADT_CircularByteQueueGetCurrentSize);
    //ID检查
    ADT_BASE_ID_CHECK(circularByteQueuePtr,ADT_CircularByteQueueGetCurrentSize,ADT_CIRCULAR_BYTE_QUEUE_T_ID);
    //当前尺寸返回
    return circularByteQueuePtr->currentSize;
}

//获取当前循环队列中空数据的量
uint16_t ADT_CircularByteQueueGetCurrentEmptySize(ADT_T circularByteQueuePtr)
{
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //指针检查
    ADT_BASE_PTR_CHECK(circularByteQueuePtr,ADT_CircularByteQueueGetCurrentEmptySize);
    //ID检查
    ADT_BASE_ID_CHECK(circularByteQueuePtr,ADT_CircularByteQueueGetCurrentEmptySize,ADT_CIRCULAR_BYTE_QUEUE_T_ID);
    uint16_t emptySize = 0;
    //系统锁定
    ADT_SYS_LOCK(circularByteQueuePtr);
    //计算
    emptySize = circularByteQueuePtr->maxSize - circularByteQueuePtr->currentSize;
    //系统解锁
    ADT_SYS_UNLOCK(circularByteQueuePtr);
    return emptySize;
}

//往循环队列中写入指定数量数据,当空间不足的时候,会将缓冲区数据填满,并返回真实的写入量
void ADT_CircularByteQueueWriteBuffer(ADT_T circularByteQueuePtr,uint8_t* bufferWritePtr,uint16_t length,uint16_t* realWriteByteCount)
{
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    uint16_t writeCount = 0;
    //指针检查
    ADT_BASE_PTR_CHECK(circularByteQueuePtr,ADT_CircularByteQueueWriteBuffer);
    //ID检查
    ADT_BASE_ID_CHECK(circularByteQueuePtr,ADT_CircularByteQueueWriteBuffer,ADT_CIRCULAR_BYTE_QUEUE_T_ID);
    //参数检查
    if(bufferWritePtr == NULL)
    {
        ADT_SHOW_STR(circularByteQueuePtr,"Error!! ADT_CircularByteQueueWriteBuffer bufferWritePtr NULL!!\r\n");
        *realWriteByteCount = 0;
        return;
    }
    //系统锁定
    ADT_SYS_LOCK(circularByteQueuePtr);
    if(circularByteQueuePtr->maxSize == circularByteQueuePtr->currentSize)
    {
        //当前数据已满
        *realWriteByteCount = 0;
    }
    else
    {
        while(circularByteQueuePtr->currentSize < circularByteQueuePtr->maxSize)
        {
            circularByteQueuePtr->queueArrayBuffer[circularByteQueuePtr->rearPos++] = bufferWritePtr[writeCount++];
            circularByteQueuePtr->rearPos %= circularByteQueuePtr->maxSize;
            circularByteQueuePtr->currentSize++;
            if(writeCount == length)
            {
                break;
            }
        }
        *realWriteByteCount = writeCount;
    }
    //系统解锁
    ADT_SYS_UNLOCK(circularByteQueuePtr);
    return;
}

//从循环队列中读取指定数量数据,当数据不足时,已有的数据写到缓冲区,并返回真实读取到的数据
void ADT_CircularByteQueueReadBuffer(ADT_T circularByteQueuePtr,uint8_t* bufferReadPtr,uint16_t length,uint16_t* realReadByteCount)
{
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    uint16_t readCount = 0;
    //指针检查
    ADT_BASE_PTR_CHECK(circularByteQueuePtr,ADT_CircularByteQueueReadBuffer);
    //ID检查
    ADT_BASE_ID_CHECK(circularByteQueuePtr,ADT_CircularByteQueueReadBuffer,ADT_CIRCULAR_BYTE_QUEUE_T_ID);
    if(bufferReadPtr == NULL)
    {
        ADT_SHOW_STR(circularByteQueuePtr,"Error!! ADT_CircularByteQueueReadBuffer bufferReadPtr NULL!!\r\n");
        *realReadByteCount = 0;
        return;
    }
    //系统锁定
    ADT_SYS_LOCK(circularByteQueuePtr);
    if(0 == circularByteQueuePtr->currentSize)
    {
        *realReadByteCount = 0;
    }
    else
    {
        while(circularByteQueuePtr->currentSize > 0)
        {
            bufferReadPtr[readCount++] = circularByteQueuePtr->queueArrayBuffer[circularByteQueuePtr->frontPos++];
            circularByteQueuePtr->frontPos %= circularByteQueuePtr->maxSize;
            circularByteQueuePtr->currentSize--;
            if(readCount == length)
            {
                break;
            }
        }
        *realReadByteCount = readCount;
    }
    //系统解锁
    ADT_SYS_UNLOCK(circularByteQueuePtr);
    return;
}

//清空队列
void ADT_CircularByteQueueClear(ADT_T circularByteQueuePtr)
{
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //指针检查
    ADT_BASE_PTR_CHECK(circularByteQueuePtr,ADT_CircularByteQueueClear);
    //ID检查
    ADT_BASE_ID_CHECK(circularByteQueuePtr,ADT_CircularByteQueueClear,ADT_CIRCULAR_BYTE_QUEUE_T_ID);
    //系统锁定
    ADT_SYS_LOCK(circularByteQueuePtr);
    circularByteQueuePtr->frontPos = 0;
    circularByteQueuePtr->rearPos = 0;
    circularByteQueuePtr->currentSize = 0;
    //系统解锁
    ADT_SYS_UNLOCK(circularByteQueuePtr);
    return;
}


