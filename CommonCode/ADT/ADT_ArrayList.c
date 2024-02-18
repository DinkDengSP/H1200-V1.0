/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-03-09 09:51:58 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-03-09 15:48:05 +0800
************************************************************************************************/ 
#include "ADT_ArrayList.h"

//宏定义结构体类型,这种定义的好处是不用出现大量的各种类型名,代码整洁
//理解上差点意思,但是使用者不需要太了解内部结构
#define ADT_T                               ADT_ARRAY_LIST_T
//结构体的类型ID,用于程序运行时的检查
#define ADT_ARRAY_LIST_T_ID                 0X20200720

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
    uint32_t length;  
    //申请的空间长度
    uint32_t _alloced;
    //节点数据定义
    void** data;
    //比较函数
    ADT_SortBaseCompare compareFunc;
};


//创建对象
ADT_T ADT_ArrayListCreate(uint32_t initLength,ADT_SortBaseCompare sortCompareFunc,
                            ADT_MemMallocFuncPtr memMallocFunc,ADT_MemFreeFuncPtr memFreeFunc,
                            ADT_LockSystem lockSysFuncPtr,ADT_UnLockSystem unlockSysFuncPtr,
                            ADT_SendString sendStrFunc)
{
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //回调函数检查
    ADT_CALL_BACK_CHECK(ADT_ArrayListCreate,memMallocFunc,memFreeFunc,lockSysFuncPtr,unlockSysFuncPtr,sendStrFunc);
    //生成变量
    ADT_T arrayListPtr = NULL;
    if(initLength == 0)
    {
        return NULL;
    }
    if(sortCompareFunc == NULL)
    {
        return NULL;
    }
    //内存申请
    ADT_MALLOC_SELF(arrayListPtr,memMallocFunc);
    //函数映射
    ADT_CALL_BACK_REGISTER(arrayListPtr,memMallocFunc,memFreeFunc,lockSysFuncPtr,unlockSysFuncPtr,sendStrFunc);
    //系统锁定
    ADT_SYS_LOCK(arrayListPtr);
    //数据复位
    arrayListPtr->_alloced = initLength;
    //创建头节点
    arrayListPtr->length = 0;
    //绑定比较函数
    arrayListPtr->compareFunc = sortCompareFunc;
    //内存申请
    arrayListPtr->data = (void**)ADT_MEM_MALLOC(arrayListPtr,initLength*sizeof(void*));
    //功能辅助
    ADT_SET_IDENTIFY_CODE_SET(arrayListPtr,ADT_ARRAY_LIST_T_ID);
    //系统解锁
    ADT_SYS_UNLOCK(arrayListPtr);
    //查看内存申请
    if (arrayListPtr->data == NULL) 
    {
		ADT_FREE_SELF(arrayListPtr);
		arrayListPtr = NULL;
	}
    //返回结构体指针
    return arrayListPtr;
}

//释放全部的数据
void ADT_ArrayListClear(ADT_T arrayListPtr)
{
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //指针检查
    ADT_BASE_PTR_CHECK(arrayListPtr,ADT_ArrayListClear);
    if(arrayListPtr->length == 0)
    {
        //空链表无操作
        return;
    }
    //ID检查
    ADT_BASE_ID_CHECK(arrayListPtr,ADT_ArrayListClear,ADT_ARRAY_LIST_T_ID);
    //系统锁定
    ADT_SYS_LOCK(arrayListPtr);
    //元素数量归零
    arrayListPtr->length = 0;
    //系统解锁
    ADT_SYS_UNLOCK(arrayListPtr);
    return;
}

//销毁
void ADT_ArrayListDestroy(ADT_T* arrayListPtrPtr)
{
    //指针检查
    ADT_BASE_PTR_CHECK(arrayListPtrPtr,ADT_ArrayListDestroy);
    //定义实体
    ADT_T arrayListPtr = (*arrayListPtrPtr);
    //指针检查
    ADT_BASE_PTR_CHECK(arrayListPtr,ADT_ArrayListDestroy);
    //ID检查
    ADT_BASE_ID_CHECK(arrayListPtr,ADT_ArrayListDestroy,ADT_ARRAY_LIST_T_ID);
    //清空数据
    ADT_MEM_FREE(arrayListPtr,arrayListPtr->data);
    //释放自身
    ADT_FREE_SELF(arrayListPtr);
    return;
}

//获取指定位置元素
void* ADT_ArrayListGetIndex(ADT_T arrayListPtr,uint32_t index,ADT_RESULT* resultPtr)
{
    void* element = NULL;
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //指针检查
    ADT_BASE_PTR_CHECK(arrayListPtr,ADT_ArrayListGetIndex);
    //ID检查
    ADT_BASE_ID_CHECK(arrayListPtr,ADT_ArrayListGetIndex,ADT_ARRAY_LIST_T_ID);
    //不能是空键
    if(index >= arrayListPtr->length)
    {
        *resultPtr = ADT_RESULT_FAIL;
        return NULL;
    }
    //系统锁定
    ADT_SYS_LOCK(arrayListPtr);
    element = arrayListPtr->data[index];
    //系统解锁
    ADT_SYS_UNLOCK(arrayListPtr);
    *resultPtr = ADT_RESULT_OK;
	return element;
}

//动态扩容
static ADT_RESULT ADT_ArrayListEnlarge(ADT_T arrayListPtr)
{
	void** newDataPtr = NULL;
	uint32_t newsize = 0;
    //数据增加一倍
	newsize = arrayListPtr->_alloced * 2;
    //申请新的数据
	newDataPtr = (void**)ADT_MEM_MALLOC(arrayListPtr,sizeof(void*) * newsize);

	if (newDataPtr == NULL) 
    {
		return ADT_RESULT_FAIL;
	} 
    else 
    {
        //数据拷贝
        UserMemCopy((void*)newDataPtr,(void*)(arrayListPtr->data),(sizeof(void*)*(arrayListPtr->length)));
        //释放原来的
        ADT_MEM_FREE(arrayListPtr,arrayListPtr->data);
        //指向最新的
        arrayListPtr->data = newDataPtr;
        //尺寸增加
		arrayListPtr->_alloced = newsize;
		return ADT_RESULT_OK;
	}
}

//指定位置插入元素
ADT_RESULT ADT_ArrayListInsert(ADT_T arrayListPtr, uint32_t index,void* data)
{
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //指针检查
    ADT_BASE_PTR_CHECK(arrayListPtr,ADT_ArrayListInsert);
    //ID检查
    ADT_BASE_ID_CHECK(arrayListPtr,ADT_ArrayListInsert,ADT_ARRAY_LIST_T_ID);
    //不能是空键
    if(index > arrayListPtr->length)
    {
        return ADT_RESULT_FAIL;
    }
    //系统锁定
    ADT_SYS_LOCK(arrayListPtr);
    if (arrayListPtr->length + 1 > arrayListPtr->_alloced)
    {
        //扩容
        if (ADT_RESULT_FAIL == ADT_ArrayListEnlarge(arrayListPtr))
        {
            //系统解锁
            ADT_SYS_UNLOCK(arrayListPtr);
            return ADT_RESULT_FAIL;
        }
    }
    //内存移动
    ADT_MEM_MOVE((void*)&(arrayListPtr->data[index + 1]),(void*)(&arrayListPtr->data[index]),
                        (arrayListPtr->length - index) * sizeof(void*));

    //数据插入
    arrayListPtr->data[index] = data;
    //数量增加
    ++(arrayListPtr->length);
    //系统解锁
    ADT_SYS_UNLOCK(arrayListPtr);
    return ADT_RESULT_OK;
}

//尾部增加元素
ADT_RESULT ADT_ArrayListAppend(ADT_T arrayListPtr, void* data)
{
    return ADT_ArrayListInsert(arrayListPtr, arrayListPtr->length, data);
}

//头部增加元素
ADT_RESULT ADT_ArrayListPrepend(ADT_T arrayListPtr, void* data)
{
    return ADT_ArrayListInsert(arrayListPtr, 0, data);
}

//移除指定区域元素
ADT_RESULT ADT_ArrayListRemoveRange(ADT_T arrayListPtr, uint32_t index,uint32_t length)
{
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //指针检查
    ADT_BASE_PTR_CHECK(arrayListPtr,ADT_ArrayListRemoveRange);
    //ID检查
    ADT_BASE_ID_CHECK(arrayListPtr,ADT_ArrayListRemoveRange,ADT_ARRAY_LIST_T_ID);
    //参数检查
    if ((index > arrayListPtr->length) || (index + length) > arrayListPtr->length)
    {
        return ADT_RESULT_FAIL;
    }
    //系统锁定
    ADT_SYS_LOCK(arrayListPtr);
    //内存拷贝
    ADT_MEM_MOVE((void*)&arrayListPtr->data[index],(void*)&arrayListPtr->data[index + length],
	                (arrayListPtr->length - (index + length))*sizeof(void*));
    //长度减少
	arrayListPtr->length -= length;
    //系统解锁
    ADT_SYS_UNLOCK(arrayListPtr);
    return ADT_RESULT_OK;
}

//移除指定位置元素
ADT_RESULT ADT_ArrayListRemove(ADT_T arrayListPtr, uint32_t index)
{
    return ADT_ArrayListRemoveRange(arrayListPtr, index, 1);
}

//查找指定元素位置
ADT_RESULT ADT_ArrayListIndexOf(ADT_T arrayListPtr,void* data,uint32_t* indexPtr)
{
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //指针检查
    ADT_BASE_PTR_CHECK(arrayListPtr,ADT_ArrayListIndexOf);
    //ID检查
    ADT_BASE_ID_CHECK(arrayListPtr,ADT_ArrayListIndexOf,ADT_ARRAY_LIST_T_ID);
    //系统锁定
    ADT_SYS_LOCK(arrayListPtr);
    uint32_t indexUtil = 0;
    for (indexUtil = 0; indexUtil < arrayListPtr->length; ++indexUtil)
    {
        if (arrayListPtr->compareFunc(arrayListPtr->data[indexUtil], data) == 0)
        {
            *indexPtr = indexUtil;
            ADT_SYS_UNLOCK(arrayListPtr);
            return ADT_RESULT_OK;
        }
            
    }
    //系统解锁
    ADT_SYS_UNLOCK(arrayListPtr);
    *indexPtr = 0XFFFFFFFF;
	return ADT_RESULT_FAIL;
}

//内部排序算法
static void ADT_ArrayListsSortInternal(ADT_T arrayListPtr,void** list_data,uint32_t list_length)
{
    void* pivot;
    void* tmp;
    unsigned int indexUtil;
    unsigned int list1_length;
    unsigned int list2_length;
    if (list_length <= 1)
    {
        return;
    }
    pivot = list_data[list_length - 1];
    list1_length = 0;
    for (indexUtil = 0; indexUtil < list_length - 1; ++indexUtil)
    {
        if (arrayListPtr->compareFunc(list_data[indexUtil], pivot) < 0)
        {
            tmp = list_data[indexUtil];
            list_data[indexUtil] = list_data[list1_length];
            list_data[list1_length] = tmp;
            ++list1_length;
        }
        else
        {
            /* This should be in list 2.  This is already in the
             * right position. */
        }
    }
    list2_length = list_length - list1_length - 1;
    list_data[list_length - 1] = list_data[list1_length];
    list_data[list1_length] = pivot;
    //递归调用
    ADT_ArrayListsSortInternal(arrayListPtr,list_data, list1_length);
    ADT_ArrayListsSortInternal(arrayListPtr,&list_data[list1_length + 1], list2_length);
}

//元素排序
ADT_RESULT ADT_ArrayListSort(ADT_T arrayListPtr)
{
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //指针检查
    ADT_BASE_PTR_CHECK(arrayListPtr,ADT_ArrayListIndexOf);
    //ID检查
    ADT_BASE_ID_CHECK(arrayListPtr,ADT_ArrayListIndexOf,ADT_ARRAY_LIST_T_ID);
    if(arrayListPtr->length == 0)
    {
        return ADT_RESULT_FAIL;
    }
    //系统锁定
    ADT_SYS_LOCK(arrayListPtr);
    //排序
    ADT_ArrayListsSortInternal(arrayListPtr,arrayListPtr->data, arrayListPtr->length);
    //系统解锁
    ADT_SYS_UNLOCK(arrayListPtr);
	return ADT_RESULT_OK;
}