/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-03-06 14:40:36 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-03-06 17:09:48 +0800
************************************************************************************************/ 
#include "ADT_DynamicArray.h"

//宏定义结构体类型,这种定义的好处是不用出现大量的各种类型名,代码整洁
//理解上差点意思,但是使用者不需要太了解内部结构
#define ADT_T                           ADT_DYNAMIC_ARRAY_T
#define ADT_NODE_T                      ADT_DYNAMIC_ARRAY_NODE_T
//结构体的类型ID,用于程序运行时的检查
//_COUNTER_  一个从0开始的全局的计数器，每次用于源文件或在源文件中被引用头文件时，自动加1
#define ADT_DYNAMIC_ARRAY_T_ID          0X20200719

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
    uint32_t capacity;      //当前动态数组容量
    uint32_t elementSize;   //单个元素的尺寸
    //存放元素的数组
    uint8_t* elementArrayPtr;
};

//生成一个动态数组
static void ADT_DynamicArrayRepInit(ADT_T arrayPtr, uint32_t capacity, uint32_t elementSize,void *elementArrayPtr)
{
    arrayPtr->capacity = capacity;
    arrayPtr->elementSize = elementSize;
    if (capacity > 0) 
    {
        arrayPtr->elementArrayPtr = elementArrayPtr;
    } 
    else 
    {
        arrayPtr->elementArrayPtr = NULL;
    }
}


//动态数组初始化
ADT_T ADT_DynamicArrayCreate(uint32_t initCapacity,uint32_t elementSize,ADT_MemMallocFuncPtr memMallocFunc,ADT_MemFreeFuncPtr memFreeFunc,
                                ADT_LockSystem lockSysFuncPtr,ADT_UnLockSystem unlockSysFuncPtr,ADT_SendString sendStrFunc)
{
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    if(initCapacity == 0)
    {
        sendStrFunc("ADT_DynamicArrayCreate initCapacity 0!!!\r\n");
        return NULL;
    }
    if(elementSize == 0)
    {
        sendStrFunc("ADT_DynamicArrayCreate elementSize 0!!!\r\n");
        return NULL;
    }
    //回调函数检查
    ADT_CALL_BACK_CHECK(ADT_DynamicArrayCreate,memMallocFunc,memFreeFunc,lockSysFuncPtr,unlockSysFuncPtr,sendStrFunc);
    //生成变量
    ADT_T dynamicArrayPtr = NULL;
    //内存申请
    ADT_MALLOC_SELF(dynamicArrayPtr,memMallocFunc);
    //函数映射
    ADT_CALL_BACK_REGISTER(dynamicArrayPtr,memMallocFunc,memFreeFunc,lockSysFuncPtr,unlockSysFuncPtr,sendStrFunc);
    //系统锁定
    ADT_SYS_LOCK(dynamicArrayPtr);
    //初始化
    if (initCapacity > 0) 
    {
        ADT_DynamicArrayRepInit(dynamicArrayPtr, initCapacity, elementSize, dynamicArrayPtr->memMallocFunc(elementSize*initCapacity));
        //数据清空
        ADT_BASE_MEM_CLEAR(dynamicArrayPtr->elementArrayPtr,elementSize*initCapacity);
    } 
    else 
    {
        ADT_DynamicArrayRepInit(dynamicArrayPtr, initCapacity, elementSize, NULL);
    }
    //功能辅助
    ADT_SET_IDENTIFY_CODE_SET(dynamicArrayPtr,ADT_DYNAMIC_ARRAY_T_ID);
    //系统解锁
    ADT_SYS_UNLOCK(dynamicArrayPtr);
    return dynamicArrayPtr;
}

//清空动态数组内部数据
ADT_T ADT_DynamicArrayClear(ADT_T arrayPtr)
{
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //指针检查
    ADT_BASE_PTR_CHECK(arrayPtr,ADT_DynamicArrayClear);
    //ID检查
    ADT_BASE_ID_CHECK(arrayPtr,ADT_DynamicArrayClear,ADT_DYNAMIC_ARRAY_T_ID);
    //数据为0,直接返回
    if(arrayPtr->elementArrayPtr == NULL)
    {
        return arrayPtr;
    }
    //系统锁定
    ADT_SYS_LOCK(arrayPtr);
    //释放内存
    ADT_MEM_FREE(arrayPtr,arrayPtr->elementArrayPtr);
    //设置容量
    arrayPtr->capacity = 0;
    arrayPtr->elementArrayPtr = NULL;
    //系统解锁
    ADT_SYS_UNLOCK(arrayPtr);
    return arrayPtr;
}

//销毁动态数组结构
void ADT_DynamicArrayDestroy(ADT_T* arrayPtrPtr)
{
    //指针检查
    ADT_BASE_PTR_CHECK(arrayPtrPtr,ADT_DynamicArrayDestroy);
    //定义实体
    ADT_T arrayPtr = (*arrayPtrPtr);
    //清空数据
    ADT_DynamicArrayClear(arrayPtr);
    //释放自身
    ADT_FREE_SELF(arrayPtr);
    return;
}

//获取动态数组中元素的数量
uint32_t ADT_DynamicArrayGetElementCapacity(ADT_T arrayPtr)
{
    //指针检查
    ADT_BASE_PTR_CHECK(arrayPtr,ADT_DynamicArrayGetElementCapacity);
    //ID检查
    ADT_BASE_ID_CHECK(arrayPtr,ADT_DynamicArrayGetElementCapacity,ADT_DYNAMIC_ARRAY_T_ID);
    //返回当前尺寸
    return arrayPtr->capacity;
}

//获取动态数组中的单个元素的尺寸
uint32_t ADT_DynamicArrayGetSingleElementSize(ADT_T arrayPtr)
{
    //指针检查
    ADT_BASE_PTR_CHECK(arrayPtr,ADT_DynamicArrayGetSingleElementSize);
    //ID检查
    ADT_BASE_ID_CHECK(arrayPtr,ADT_DynamicArrayGetSingleElementSize,ADT_DYNAMIC_ARRAY_T_ID);
    //返回当前尺寸
    return arrayPtr->elementSize;
}

//获取数组指定元素
void* ADT_DynamicArrayGet(ADT_T arrayPtr,uint32_t index,ADT_RESULT* resultPtr)
{
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //指针检查
    ADT_BASE_PTR_CHECK(arrayPtr,ADT_DynamicArrayGet);
    //ID检查
    ADT_BASE_ID_CHECK(arrayPtr,ADT_DynamicArrayGet,ADT_DYNAMIC_ARRAY_T_ID);
    //检查元素,空元素不处理
    if(index >= arrayPtr->capacity)
    {
        *resultPtr = ADT_RESULT_FAIL;
        return NULL;
    }
    //系统锁定
    ADT_SYS_LOCK(arrayPtr);
    void* elementPtr = arrayPtr->elementArrayPtr + (index*(arrayPtr->elementSize));
    //系统解锁
    ADT_SYS_UNLOCK(arrayPtr);
    *resultPtr = ADT_RESULT_OK;
    //返回设置之前的数值,不存在返回空
    return elementPtr;
}

//压入指定数组元素
ADT_RESULT ADT_DynamicArrayPut(ADT_T arrayPtr,uint32_t index,void* element)
{
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //指针检查
    ADT_BASE_PTR_CHECK(arrayPtr,ADT_DynamicArrayPut);
    //ID检查
    ADT_BASE_ID_CHECK(arrayPtr,ADT_DynamicArrayPut,ADT_DYNAMIC_ARRAY_T_ID);
    //检查元素,空元素不处理
    if(index >= arrayPtr->capacity)
    {
        return ADT_RESULT_FAIL;
    }
    //系统锁定
    ADT_SYS_LOCK(arrayPtr);
    //数据拷贝
    ADT_BASE_MEM_COPY((arrayPtr->elementArrayPtr + (index*(arrayPtr->elementSize))),element,arrayPtr->elementSize);
    //解锁返回
    ADT_SYS_UNLOCK(arrayPtr);
    //返回设置之前的数值,不存在返回空
    return ADT_RESULT_OK;
}

//重新调整尺寸
ADT_RESULT ADT_DynamicArrayResize(ADT_T arrayPtr,uint32_t newCapacity)
{
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //指针检查
    ADT_BASE_PTR_CHECK(arrayPtr,ADT_DynamicArrayResize);
    //ID检查
    ADT_BASE_ID_CHECK(arrayPtr,ADT_DynamicArrayResize,ADT_DYNAMIC_ARRAY_T_ID);
    //系统锁定
    ADT_SYS_LOCK(arrayPtr);
    if (newCapacity == 0) 
    {
        if(arrayPtr->capacity != 0)
        {
            ADT_MEM_FREE(arrayPtr,arrayPtr->elementArrayPtr);
        }
        arrayPtr->elementArrayPtr = NULL;
        //设置容量
        arrayPtr->capacity = newCapacity;
        ADT_SYS_UNLOCK(arrayPtr);
        //返回设置之前的数值,不存在返回空
        return ADT_RESULT_OK;
    } 
    //生成新的容器
    uint8_t* elementArrayNewPtr = ADT_MEM_MALLOC(arrayPtr,(arrayPtr->elementSize)*newCapacity);
    //数据清空
    ADT_BASE_MEM_CLEAR(elementArrayNewPtr,(arrayPtr->elementSize)*newCapacity);
    //数据拷贝
    uint32_t copyLength = newCapacity;
    if(newCapacity > arrayPtr->capacity)
    {
        copyLength = arrayPtr->capacity;
    }
    //数据拷贝
    ADT_BASE_MEM_COPY(elementArrayNewPtr,arrayPtr->elementArrayPtr,copyLength*(arrayPtr->elementSize));
    //原来的数据释放
    ADT_MEM_FREE(arrayPtr,arrayPtr->elementArrayPtr);
    //新的数据关联
    arrayPtr->elementArrayPtr = elementArrayNewPtr;
    //设置容量
    arrayPtr->capacity = newCapacity;
    ADT_SYS_UNLOCK(arrayPtr);
    //返回设置之前的数值,不存在返回空
    return ADT_RESULT_OK;
}

//创建拷贝
ADT_T ADT_DynamicArrayCopy(ADT_T arrayPtr,uint32_t copyLength)
{
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //指针检查
    ADT_BASE_PTR_CHECK(arrayPtr,ADT_DynamicArrayCopy);
    //ID检查
    ADT_BASE_ID_CHECK(arrayPtr,ADT_DynamicArrayCopy,ADT_DYNAMIC_ARRAY_T_ID);
    //系统锁定
    ADT_SYS_LOCK(arrayPtr);
    //新的数组
    ADT_T newArrayPtr = ADT_DynamicArrayCreate(copyLength,arrayPtr->elementSize,arrayPtr->memMallocFunc,arrayPtr->memFreeFunc,
                                arrayPtr->sysLockFunc,arrayPtr->sysUnlockFunc,arrayPtr->sendStrFunc);
    if ((newArrayPtr->capacity >= arrayPtr->capacity) && (arrayPtr->capacity > 0)) 
    {
        //新的空间比原来的大
        ADT_BASE_MEM_COPY(newArrayPtr->elementArrayPtr, arrayPtr->elementArrayPtr, (arrayPtr->elementSize * arrayPtr->capacity));
    } 
    else if ((arrayPtr->capacity > newArrayPtr->capacity) && (newArrayPtr->capacity > 0)) 
    {
        //新的空间比原来的小
        ADT_BASE_MEM_COPY(newArrayPtr->elementArrayPtr, arrayPtr->elementArrayPtr, (newArrayPtr->capacity * newArrayPtr->elementSize));
    }
    ADT_SYS_UNLOCK(arrayPtr);
    //返回设置之前的数值,不存在返回空
    return newArrayPtr;
}