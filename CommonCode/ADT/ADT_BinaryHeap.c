/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-03-09 09:52:32 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-03-09 17:09:05 +0800
************************************************************************************************/ 
#include "ADT_BinaryHeap.h"

//宏定义结构体类型,这种定义的好处是不用出现大量的各种类型名,代码整洁
//理解上差点意思,但是使用者不需要太了解内部结构
#define ADT_T                       ADT_BINARY_HEAP_T
//结构体的类型ID,用于程序运行时的检查
#define ADT_BINARY_HEAP_T_ID        0X20200721

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
    //当前数据长度
    uint32_t num_values;  
    //申请空间的长度
    uint32_t alloced_size;
    //堆类型
    BinHeapType heap_type;
    //节点数据定义
    void** values;
    //比较函数
    ADT_SortBaseCompare compare_func;
};

//创建对象
ADT_T ADT_BinaryHeapCreate(BinHeapType type,ADT_SortBaseCompare sortCompareFunc,
                            ADT_MemMallocFuncPtr memMallocFunc,ADT_MemFreeFuncPtr memFreeFunc,
                            ADT_LockSystem lockSysFuncPtr,ADT_UnLockSystem unlockSysFuncPtr,
                            ADT_SendString sendStrFunc)
{
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //回调函数检查
    ADT_CALL_BACK_CHECK(ADT_BinaryHeapCreate,memMallocFunc,memFreeFunc,lockSysFuncPtr,unlockSysFuncPtr,sendStrFunc);
    //生成变量
    ADT_T binaryHeapPtr = NULL;
    if((type != BIN_HEAP_TYPE_MIN)&&(type != BIN_HEAP_TYPE_MAX))
    {
        return NULL;
    }
    if(sortCompareFunc == NULL)
    {
        return NULL;
    }
    //内存申请
    ADT_MALLOC_SELF(binaryHeapPtr,memMallocFunc);
    //函数映射
    ADT_CALL_BACK_REGISTER(binaryHeapPtr,memMallocFunc,memFreeFunc,lockSysFuncPtr,unlockSysFuncPtr,sendStrFunc);
    //系统锁定
    ADT_SYS_LOCK(binaryHeapPtr);
    //数据设置
    binaryHeapPtr->heap_type = type;
    binaryHeapPtr->num_values = 0;
    //初始长度16
    binaryHeapPtr->alloced_size = 16;
    binaryHeapPtr->compare_func = sortCompareFunc;
    //内存申请
    binaryHeapPtr->values = (void**)ADT_MEM_MALLOC(binaryHeapPtr,(sizeof(void*)*(binaryHeapPtr->alloced_size)));
    //功能辅助
    ADT_SET_IDENTIFY_CODE_SET(binaryHeapPtr,ADT_BINARY_HEAP_T_ID);
    //系统解锁
    ADT_SYS_UNLOCK(binaryHeapPtr);
    //查看内存申请
    if (binaryHeapPtr->values == NULL) 
    {
		ADT_FREE_SELF(binaryHeapPtr);
		binaryHeapPtr = NULL;
	}
    //返回结构体指针
    return binaryHeapPtr;
}

//销毁
void ADT_BinaryHeapDestroy(ADT_T* binaryHeapPtrPtr)
{
    //指针检查
    ADT_BASE_PTR_CHECK(binaryHeapPtrPtr,ADT_BinaryHeapDestroy);
    //定义实体
    ADT_T binaryHeapPtr = (*binaryHeapPtrPtr);
    //指针检查
    ADT_BASE_PTR_CHECK(binaryHeapPtr,ADT_BinaryHeapDestroy);
    //ID检查
    ADT_BASE_ID_CHECK(binaryHeapPtr,ADT_BinaryHeapDestroy,ADT_BINARY_HEAP_T_ID);
    //清空数据
    ADT_MEM_FREE(binaryHeapPtr,binaryHeapPtr->values);
    //释放自身
    ADT_FREE_SELF(binaryHeapPtr);
    return;
}

//释放全部的数据
void ADT_BinaryHeapClear(ADT_T binaryHeapPtr)
{
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //指针检查
    ADT_BASE_PTR_CHECK(binaryHeapPtr,ADT_BinaryHeapClear);
    if(binaryHeapPtr->num_values == 0)
    {
        //空链表无操作
        return;
    }
    //ID检查
    ADT_BASE_ID_CHECK(binaryHeapPtr,ADT_BinaryHeapClear,ADT_BINARY_HEAP_T_ID);
    //系统锁定
    ADT_SYS_LOCK(binaryHeapPtr);
    //元素数量归零
    binaryHeapPtr->num_values = 0;
    //系统解锁
    ADT_SYS_UNLOCK(binaryHeapPtr);
    return;
}

//根据设置的堆类型决定比较方法
static int ADT_BinaryHeapCmp(ADT_T binaryHeapPtr,void* data1,void* data2)
{
	if (binaryHeapPtr->heap_type == BIN_HEAP_TYPE_MIN) 
    {
		return binaryHeapPtr->compare_func(data1, data2);
	} 
    else 
    {
		return -(binaryHeapPtr->compare_func(data1, data2));
	}
}

//数据插入
ADT_RESULT ADT_BinaryHeapInsert(ADT_T binaryHeapPtr, void* value)
{
    void** new_values;
	unsigned int index;
	unsigned int new_size;
	unsigned int parent;
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //指针检查
    ADT_BASE_PTR_CHECK(binaryHeapPtr,ADT_BinaryHeapInsert);
    //ID检查
    ADT_BASE_ID_CHECK(binaryHeapPtr,ADT_BinaryHeapInsert,ADT_BINARY_HEAP_T_ID);
    //系统锁定
    ADT_SYS_LOCK(binaryHeapPtr);
    if (binaryHeapPtr->num_values >= binaryHeapPtr->alloced_size)
    {
        new_size = binaryHeapPtr->alloced_size * 2;
        new_values = (void**)ADT_MEM_MALLOC(binaryHeapPtr,(sizeof(void*)*new_size));
        if (new_values == NULL) 
        {
            //系统解锁
            ADT_SYS_UNLOCK(binaryHeapPtr);
            return ADT_RESULT_FAIL;
        } 
        else 
        {
            //数据拷贝
            UserMemCopy((void*)new_values,(void*)(binaryHeapPtr->values),(sizeof(void*)*new_size));
            //释放原来的
            ADT_MEM_FREE(binaryHeapPtr,binaryHeapPtr->values);
            //指向最新的
            binaryHeapPtr->values = new_values;
        }
        binaryHeapPtr->alloced_size = new_size;
        binaryHeapPtr->values = new_values;
    }
    index = binaryHeapPtr->num_values;
    ++(binaryHeapPtr->num_values);
    while (index > 0)
    {
        parent = (index - 1) / 2;
        if (ADT_BinaryHeapCmp(binaryHeapPtr, binaryHeapPtr->values[parent], value) < 0)
        {
            break;
        }
        else
        {
            binaryHeapPtr->values[index] = binaryHeapPtr->values[parent];
            index = parent;
        }
    }
    binaryHeapPtr->values[index] = value;
    //系统解锁
    ADT_SYS_UNLOCK(binaryHeapPtr);
    return ADT_RESULT_OK;
}

//数据弹出
void* ADT_BinaryHeapPop(ADT_T binaryHeapPtr,ADT_RESULT* resultPtr)
{
    void* result;
	void* new_value;
	unsigned int index;
	unsigned int next_index;
	unsigned int child1, child2;
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //指针检查
    ADT_BASE_PTR_CHECK(binaryHeapPtr,ADT_BinaryHeapInsert);
    //ID检查
    ADT_BASE_ID_CHECK(binaryHeapPtr,ADT_BinaryHeapInsert,ADT_BINARY_HEAP_T_ID);
    if (binaryHeapPtr->num_values == 0)
    {
        *resultPtr = ADT_RESULT_FAIL;
        return NULL;
    }
    //系统锁定
    ADT_SYS_LOCK(binaryHeapPtr);
    result = binaryHeapPtr->values[0];
    new_value = binaryHeapPtr->values[binaryHeapPtr->num_values - 1];
    //数量减少
    --(binaryHeapPtr->num_values);
    index = 0;
    //重新排布
    for (;;)
    {
        child1 = index * 2 + 1;
        child2 = index * 2 + 2;
        if ((child1 < binaryHeapPtr->num_values)&&
                (ADT_BinaryHeapCmp(binaryHeapPtr,new_value,binaryHeapPtr->values[child1]) > 0))
        {
            if ((child2 < binaryHeapPtr->num_values)&&
                    (ADT_BinaryHeapCmp(binaryHeapPtr,binaryHeapPtr->values[child1],binaryHeapPtr->values[child2]) > 0))
            {
                next_index = child2;
            }
            else
            {
                next_index = child1;
            }
        }
        else if ((child2 < binaryHeapPtr->num_values)&&
                        (ADT_BinaryHeapCmp(binaryHeapPtr,new_value,binaryHeapPtr->values[child2]) > 0))
        {
            next_index = child2;
        }
        else
        {
            binaryHeapPtr->values[index] = new_value;
            break;
        }
        binaryHeapPtr->values[index] = binaryHeapPtr->values[next_index];
        index = next_index;
    }
    *resultPtr = ADT_RESULT_OK;
    //系统解锁
    ADT_SYS_UNLOCK(binaryHeapPtr);
    return result;
}

uint32_t ADT_BinaryHeapNumEntries(ADT_T binaryHeapPtr)
{
    //指针检查
    ADT_BASE_PTR_CHECK(binaryHeapPtr,ADT_BinaryHeapNumEntries);
    //ID检查
    ADT_BASE_ID_CHECK(binaryHeapPtr,ADT_BinaryHeapNumEntries,ADT_BINARY_HEAP_T_ID);
    return binaryHeapPtr->num_values;
}


