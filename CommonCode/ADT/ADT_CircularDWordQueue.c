/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-02-27 13:35:27 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-02-27 13:45:16 +0800
************************************************************************************************/ 
#include "ADT_CircularDWordQueue.h"

//宏定义结构体类型,这种定义的好处是不用出现大量的各种类型名,代码整洁
//理解上差点意思,但是使用者不需要太了解内部结构
#define ADT_T ADT_CIRCULAR_DWORD_QUEUE_T
//结构体的类型ID,用于程序运行时的检查
//_COUNTER_  一个从0开始的全局的计数器，每次用于源文件或在源文件中被引用头文件时，自动加1
#define ADT_CIRCULAR_DWORD_QUEUE_T_ID         0X20200715

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
    uint32_t *queueArrayBuffer; //循环队列缓冲区
    uint16_t frontPos;         //循环队列头,下一次取数据从该位置去取
    uint16_t rearPos;          //循环队列位,插入数据的时候看这个数据
    uint16_t maxSize;          //队列最大容量
    uint16_t currentSize;      //当前有效数据长度
    //满载操作
    ADT_FULL_OPTION fullOption;
};

//循环队列初始化,这里的ADT是一个数据类型,是H文件中定义的指向ADT_CIRCULAR_DWORD_QUEUE_T的指针
ADT_T ADT_CircularDWordQueueCreate(uint16_t bufferSize,ADT_FULL_OPTION fullOption,ADT_MemMallocFuncPtr memMallocFunc,ADT_MemFreeFuncPtr memFreeFunc,
                                ADT_LockSystem lockSysFuncPtr,ADT_UnLockSystem unlockSysFuncPtr,ADT_SendString sendStrFunc)
{
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //回调函数检查
    ADT_CALL_BACK_CHECK(ADT_CircularDWordQueueCreate,memMallocFunc,memFreeFunc,lockSysFuncPtr,unlockSysFuncPtr,sendStrFunc);
    //参数检查
    if(bufferSize == 0)
    {
        sendStrFunc("Error!! ADT_CircularDWordQueueCreate bufferSize 0!!\r\n");
        return NULL;
    }
    //定义实体
    ADT_T circularDWordQueuePtr = NULL;
    //实体内存申请
    ADT_MALLOC_SELF(circularDWordQueuePtr,memMallocFunc);
    //函数映射
    ADT_CALL_BACK_REGISTER(circularDWordQueuePtr,memMallocFunc,memFreeFunc,lockSysFuncPtr,unlockSysFuncPtr,sendStrFunc);
    //数据缓存申请
    circularDWordQueuePtr->queueArrayBuffer = (uint32_t*)ADT_MEM_MALLOC(circularDWordQueuePtr,bufferSize*sizeof(uint32_t));
    //系统锁定
    ADT_SYS_LOCK(circularDWordQueuePtr);
    //数据复位
    circularDWordQueuePtr->frontPos = 0;
    circularDWordQueuePtr->rearPos = 0;
    circularDWordQueuePtr->maxSize = bufferSize;
    circularDWordQueuePtr->currentSize = 0;
    //满载操作
    circularDWordQueuePtr->fullOption = fullOption;
    //功能辅助
    ADT_SET_IDENTIFY_CODE_SET(circularDWordQueuePtr,ADT_CIRCULAR_DWORD_QUEUE_T_ID);
    //系统解锁
    ADT_SYS_UNLOCK(circularDWordQueuePtr);
    //返回结构体指针
    return circularDWordQueuePtr;
}

//释放申请的内存
void ADT_CircularDWordQueueFree(ADT_T* circularDWordQueuePtrPtr)
{
    //指针检查
    ADT_BASE_PTR_CHECK(circularDWordQueuePtrPtr,ADT_CircularDWordQueueFree);
    //定义实体
    ADT_T circularDWordQueuePtr = (*circularDWordQueuePtrPtr);
    //ID检查
    ADT_BASE_ID_CHECK(circularDWordQueuePtr,ADT_CircularDWordQueueFree,ADT_CIRCULAR_DWORD_QUEUE_T_ID);
    //释放缓存
    ADT_MEM_FREE(circularDWordQueuePtr,circularDWordQueuePtr->queueArrayBuffer);
    //释放自身
    ADT_FREE_SELF(circularDWordQueuePtr);
    return;
}

//获取循环队列最大容量
uint16_t ADT_CircularDWordQueueGetMaxCapacity(ADT_T circularDWordQueuePtr)
{
    //指针检查
    ADT_BASE_PTR_CHECK(circularDWordQueuePtr,ADT_CircularDWordQueueGetMaxCapacity);
    //ID检查
    ADT_BASE_ID_CHECK(circularDWordQueuePtr,ADT_CircularDWordQueueGetMaxCapacity,ADT_CIRCULAR_DWORD_QUEUE_T_ID);
    //容量返回
    return circularDWordQueuePtr->maxSize;
}

//获取当前循环队列已经存储的数据
uint16_t ADT_CircularDWordQueueGetCurrentSize(ADT_T circularDWordQueuePtr)
{
    //指针检查
    ADT_BASE_PTR_CHECK(circularDWordQueuePtr,ADT_CircularDWordQueueGetCurrentSize);
    //ID检查
    ADT_BASE_ID_CHECK(circularDWordQueuePtr,ADT_CircularDWordQueueGetCurrentSize,ADT_CIRCULAR_DWORD_QUEUE_T_ID);
    //当前尺寸返回
    return circularDWordQueuePtr->currentSize;
}

//获取当前循环队列中空数据的量
uint16_t ADT_CircularDWordQueueGetCurrentEmptySize(ADT_T circularDWordQueuePtr)
{
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //指针检查
    ADT_BASE_PTR_CHECK(circularDWordQueuePtr,ADT_CircularDWordQueueGetCurrentEmptySize);
    //ID检查
    ADT_BASE_ID_CHECK(circularDWordQueuePtr,ADT_CircularDWordQueueGetCurrentEmptySize,ADT_CIRCULAR_DWORD_QUEUE_T_ID);
    uint16_t emptySize = 0;
    //系统锁定
    ADT_SYS_LOCK(circularDWordQueuePtr);
    //计算
    emptySize = circularDWordQueuePtr->maxSize - circularDWordQueuePtr->currentSize;
    //系统解锁
    ADT_SYS_UNLOCK(circularDWordQueuePtr);
    return emptySize;
}

//往循环队列中写入指定数量数据,满载分为两种情况,弹出旧数据或者拒绝新数据
void ADT_CircularDWordQueueWriteBuffer(ADT_T circularDWordQueuePtr,uint32_t* bufferWritePtr,uint16_t length,uint16_t* realWriteCount)
{
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    uint16_t writeCount = 0;
    //指针检查
    ADT_BASE_PTR_CHECK(circularDWordQueuePtr,ADT_CircularDWordQueueWriteBuffer);
    //ID检查
    ADT_BASE_ID_CHECK(circularDWordQueuePtr,ADT_CircularDWordQueueWriteBuffer,ADT_CIRCULAR_DWORD_QUEUE_T_ID);
    //参数检查
    if(bufferWritePtr == NULL)
    {
        ADT_SHOW_STR(circularDWordQueuePtr,"Error!! ADT_CircularDWordQueueWriteBuffer bufferWritePtr NULL!!\r\n");
        *realWriteCount = 0;
        return;
    }
    //系统锁定
    ADT_SYS_LOCK(circularDWordQueuePtr);
    //写入之前先检查是否满载
    if((circularDWordQueuePtr->fullOption == ADT_FULL_OPTION_BLOCK)&&
            (circularDWordQueuePtr->maxSize == circularDWordQueuePtr->currentSize))
    {
        //当前数据已满,不再写入
        *realWriteCount = 0;
        //系统解锁
        ADT_SYS_UNLOCK(circularDWordQueuePtr);
        return;
    }
    //数据写入
    for(writeCount = 0;writeCount < length;writeCount++)
    {
        //检查是否写满了,并且设置将旧数据弹出
        if((circularDWordQueuePtr->maxSize == circularDWordQueuePtr->currentSize)&&
                (circularDWordQueuePtr->fullOption == ADT_FULL_OPTION_POP))
        {
            //队列头减小
            circularDWordQueuePtr->frontPos++;
            //控制不能溢出
            circularDWordQueuePtr->frontPos %= circularDWordQueuePtr->maxSize;
            //当前尺寸缩一个
            circularDWordQueuePtr->currentSize -= 1;;
        }
        //数据写入
        circularDWordQueuePtr->queueArrayBuffer[circularDWordQueuePtr->rearPos++] = bufferWritePtr[writeCount];
        circularDWordQueuePtr->rearPos %= circularDWordQueuePtr->maxSize;
        circularDWordQueuePtr->currentSize++;
        //写入数量增加
        *realWriteCount += 1;
        //检查满载并且设置堵塞
        if((circularDWordQueuePtr->fullOption == ADT_FULL_OPTION_BLOCK)&&
            (circularDWordQueuePtr->maxSize == circularDWordQueuePtr->currentSize))
        {
            break;
        }
    }
    //系统解锁
    ADT_SYS_UNLOCK(circularDWordQueuePtr);
    return;
}

//往循环队列中写入单个字数据
ADT_RESULT ADT_CircularDWordQueueWriteOnce(ADT_T circularByteQueuePtr,uint32_t dwordWrite)
{
    uint16_t readWriteByte = 0;
    ADT_CircularDWordQueueWriteBuffer(circularByteQueuePtr,&dwordWrite,1,&readWriteByte);
    if(readWriteByte == 0)
    {
        return ADT_RESULT_FAIL;
    }
    return ADT_RESULT_OK;
}

//从循环队列中读取指定数量数据,当数据不足时,已有的数据写到缓冲区,并返回真实读取到的数据
void ADT_CircularDWordQueueReadBuffer(ADT_T circularDWordQueuePtr,uint32_t* bufferReadPtr,uint16_t length,uint16_t* realReadCount)
{
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    uint16_t readCount = 0;
    //指针检查
    ADT_BASE_PTR_CHECK(circularDWordQueuePtr,ADT_CircularDWordQueueReadBuffer);
    //ID检查
    ADT_BASE_ID_CHECK(circularDWordQueuePtr,ADT_CircularDWordQueueReadBuffer,ADT_CIRCULAR_DWORD_QUEUE_T_ID);
    if(bufferReadPtr == NULL)
    {
        ADT_SHOW_STR(circularDWordQueuePtr,"Error!! ADT_CircularDWordQueueReadBuffer bufferReadPtr NULL!!\r\n");
        *realReadCount = 0;
        return;
    }
    //系统锁定
    ADT_SYS_LOCK(circularDWordQueuePtr);
    if(0 == circularDWordQueuePtr->currentSize)
    {
        *realReadCount = 0;
    }
    else
    {
        while(circularDWordQueuePtr->currentSize > 0)
        {
            bufferReadPtr[readCount++] = circularDWordQueuePtr->queueArrayBuffer[circularDWordQueuePtr->frontPos++];
            circularDWordQueuePtr->frontPos %= circularDWordQueuePtr->maxSize;
            circularDWordQueuePtr->currentSize--;
            if(readCount == length)
            {
                break;
            }
        }
        *realReadCount = readCount;
    }
    //系统解锁
    ADT_SYS_UNLOCK(circularDWordQueuePtr);
    return;
}

//从循环队列中读取单个字数据
ADT_RESULT ADT_CircularDWordQueueReadOnce(ADT_T circularByteQueuePtr,uint32_t* readBytePtr)
{
    uint16_t readReadCount = 0;
    ADT_CircularDWordQueueReadBuffer(circularByteQueuePtr,readBytePtr,1,&readReadCount);
    if(readReadCount == 0)
    {
        return ADT_RESULT_FAIL;
    }
    return ADT_RESULT_OK;
}

//清空队列
void ADT_CircularDWordQueueClear(ADT_T circularDWordQueuePtr)
{
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //指针检查
    ADT_BASE_PTR_CHECK(circularDWordQueuePtr,ADT_CircularDWordQueueClear);
    //ID检查
    ADT_BASE_ID_CHECK(circularDWordQueuePtr,ADT_CircularDWordQueueClear,ADT_CIRCULAR_DWORD_QUEUE_T_ID);
    //系统锁定
    ADT_SYS_LOCK(circularDWordQueuePtr);
    circularDWordQueuePtr->frontPos = 0;
    circularDWordQueuePtr->rearPos = 0;
    circularDWordQueuePtr->currentSize = 0;
    //系统解锁
    ADT_SYS_UNLOCK(circularDWordQueuePtr);
    return;
}

