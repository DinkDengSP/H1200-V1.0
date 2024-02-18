/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-03-09 09:53:06 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-03-09 18:03:14 +0800
************************************************************************************************/ 
#include "ADT_BinomialHeap.h"

//宏定义结构体类型,这种定义的好处是不用出现大量的各种类型名,代码整洁
//理解上差点意思,但是使用者不需要太了解内部结构
#define ADT_T                       ADT_BINOMIAL_HEAP_T
#define ADT_NODE_T                  ADT_BINOMIAL_HEAP_NODE_T
//结构体的类型ID,用于程序运行时的检查
#define ADT_BINOMIAL_HEAP_T_ID      0X20200722

//节点
struct ADT_NODE_T
{
	void* value;
	unsigned short order;
	unsigned short refcount;
	struct ADT_NODE_T **subtrees;
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
    //当前数据长度
    uint32_t num_values;  
    //申请空间的长度
    uint32_t roots_length;
    //堆类型
    BinHeapType heap_type;
    //节点数据定义
    struct ADT_NODE_T** roots;
    //比较函数
    ADT_SortBaseCompare compare_func;
};


//根据设置的堆类型决定比较方法
static int ADT_BinomialHeapCmp(ADT_T binomialHeapPtr,void* data1,void* data2)
{
	if (binomialHeapPtr->heap_type == BIN_HEAP_TYPE_MIN) 
    {
		return binomialHeapPtr->compare_func(data1, data2);
	} 
    else 
    {
		return -(binomialHeapPtr->compare_func(data1, data2));
	}
}

//增加引用
static void ADT_BinomialHeapTreeRef(ADT_NODE_T nodePtr)
{
	if (nodePtr != NULL) 
    {
		++(nodePtr->refcount);
	}
}

//解除引用
static void ADT_BinomialHeapTreeUnRef(ADT_T binomialHeapPtr,ADT_NODE_T nodePtr)
{
    int indexUtil;
    if (nodePtr == NULL)
    {
        return;
    }
    --(nodePtr->refcount);
    if (nodePtr->refcount == 0)
    {
        for (indexUtil = 0; indexUtil < nodePtr->order; ++indexUtil)
        {
            ADT_BinomialHeapTreeUnRef(binomialHeapPtr,nodePtr->subtrees[indexUtil]);
        }
        ADT_MEM_FREE(binomialHeapPtr,nodePtr->subtrees);
        ADT_MEM_FREE(binomialHeapPtr,nodePtr);
    }
}

//两个树节点聚合
static ADT_NODE_T ADT_BinomialHeapTreeMerge(ADT_T binomialHeapPtr,ADT_NODE_T tree1,ADT_NODE_T tree2)
{
	ADT_NODE_T new_tree;
	ADT_NODE_T tmp;
	int indexUtil;

    if (ADT_BinomialHeapCmp(binomialHeapPtr, tree1->value, tree2->value) > 0)
    {
        tmp = tree1;
        tree1 = tree2;
        tree2 = tmp;
    }
    //内存申请
    new_tree = ADT_MEM_MALLOC(binomialHeapPtr,sizeof(*new_tree));
    if (new_tree == NULL)
    {
        return NULL;
    }
    new_tree->refcount = 0;
    new_tree->order = (unsigned short)(tree1->order + 1);
    new_tree->value = tree1->value;
    //内存申请
    new_tree->subtrees =  ADT_MEM_MALLOC(binomialHeapPtr,(sizeof(*(new_tree->subtrees))*(new_tree->order)));
    if (new_tree->subtrees == NULL)
    {
        ADT_MEM_FREE(binomialHeapPtr,new_tree);
        return NULL;
    }
    //数据拷贝
    ADT_BASE_MEM_COPY((void*)(new_tree->subtrees), (void*)(tree1->subtrees),(sizeof(*(tree1->subtrees))*(tree1->order)));
    new_tree->subtrees[new_tree->order - 1] = tree2;
    //循环增加引用
    for (indexUtil = 0; indexUtil < new_tree->order; ++indexUtil)
    {
        ADT_BinomialHeapTreeRef(new_tree->subtrees[indexUtil]);
    }
    return new_tree;
}

//回退聚合
static void ADT_BinomialHeapTreeMergeUndo(ADT_T binomialHeapPtr,ADT_NODE_T *new_roots,uint32_t count)
{
	uint32_t indexUtil;
	for (indexUtil=0; indexUtil<=count; ++indexUtil) 
    {
		ADT_BinomialHeapTreeUnRef(binomialHeapPtr,new_roots[indexUtil]);
	}
    ADT_MEM_FREE(binomialHeapPtr,new_roots);
}

//对两个堆执行聚合
static ADT_RESULT ADT_BinomialHeapMerge(ADT_T binomialHeapPtr, ADT_T otherHeapPtr)
{
    ADT_NODE_T *new_roots;
    uint32_t new_roots_length;
    ADT_NODE_T vals[3];
    int num_vals;
    ADT_NODE_T carry;
    ADT_NODE_T new_carry;
    uint32_t max;
    uint32_t indexUtil;
    if (binomialHeapPtr->roots_length > otherHeapPtr->roots_length)
    {
        max = binomialHeapPtr->roots_length + 1;
    }
    else
    {
        max = otherHeapPtr->roots_length + 1;
    }
    new_roots = ADT_MEM_MALLOC(binomialHeapPtr,sizeof(*new_roots) * max);
    if (new_roots == NULL)
    {
        return ADT_RESULT_FAIL;
    }
    new_roots_length = 0;
    carry = NULL;
    for (indexUtil = 0; indexUtil < max; ++indexUtil)
    {
        num_vals = 0;
        if ((indexUtil < binomialHeapPtr->roots_length)&&(binomialHeapPtr->roots[indexUtil] != NULL))
        {
            vals[num_vals] = binomialHeapPtr->roots[indexUtil];
            ++num_vals;
        }
        if ((indexUtil < otherHeapPtr->roots_length)&&(otherHeapPtr->roots[indexUtil] != NULL))
        {
            vals[num_vals] = otherHeapPtr->roots[indexUtil];
            ++num_vals;
        }
        if (carry != NULL)
        {
            vals[num_vals] = carry;
            ++num_vals;
        }
        if ((num_vals & 1) != 0)
        {
            new_roots[indexUtil] = vals[num_vals - 1];
            ADT_BinomialHeapTreeRef(new_roots[indexUtil]);
            new_roots_length = indexUtil + 1;
        }
        else
        {
            new_roots[indexUtil] = NULL;
        }
        if ((num_vals & 2) != 0)
        {
            new_carry = ADT_BinomialHeapTreeMerge(binomialHeapPtr,vals[0],vals[1]);
            if (new_carry == NULL)
            {
                ADT_BinomialHeapTreeMergeUndo(binomialHeapPtr,new_roots, indexUtil);
                ADT_BinomialHeapTreeUnRef(binomialHeapPtr,carry);
                return ADT_RESULT_FAIL;
            }
        }
        else
        {
            new_carry = NULL;
        }
        ADT_BinomialHeapTreeUnRef(binomialHeapPtr,carry);
        carry = new_carry;
        ADT_BinomialHeapTreeUnRef(binomialHeapPtr,carry);
    }
    for (indexUtil = 0; indexUtil < binomialHeapPtr->roots_length; ++indexUtil)
    {
        if (binomialHeapPtr->roots[indexUtil] != NULL)
        {
            ADT_BinomialHeapTreeUnRef(binomialHeapPtr,binomialHeapPtr->roots[indexUtil]);
        }
    }
    ADT_MEM_FREE(binomialHeapPtr,binomialHeapPtr->roots);
    binomialHeapPtr->roots = new_roots;
    binomialHeapPtr->roots_length = new_roots_length;
    return ADT_RESULT_OK;
}

//创建对象
ADT_T ADT_BinomialHeapCreate(BinHeapType type,ADT_SortBaseCompare sortCompareFunc,
                            ADT_MemMallocFuncPtr memMallocFunc,ADT_MemFreeFuncPtr memFreeFunc,
                            ADT_LockSystem lockSysFuncPtr,ADT_UnLockSystem unlockSysFuncPtr,
                            ADT_SendString sendStrFunc)
{
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //回调函数检查
    ADT_CALL_BACK_CHECK(ADT_BinomialHeapCreate,memMallocFunc,memFreeFunc,lockSysFuncPtr,unlockSysFuncPtr,sendStrFunc);
    //生成变量
    ADT_T binomialHeapPtr = NULL;
    if((type != BIN_HEAP_TYPE_MIN)&&(type != BIN_HEAP_TYPE_MAX))
    {
        return NULL;
    }
    if(sortCompareFunc == NULL)
    {
        return NULL;
    }
    //内存申请
    ADT_MALLOC_SELF(binomialHeapPtr,memMallocFunc);
    //函数映射
    ADT_CALL_BACK_REGISTER(binomialHeapPtr,memMallocFunc,memFreeFunc,lockSysFuncPtr,unlockSysFuncPtr,sendStrFunc);
    //系统锁定
    ADT_SYS_LOCK(binomialHeapPtr);
    //数据设置
    binomialHeapPtr->heap_type = type;
    binomialHeapPtr->num_values = 0;
    binomialHeapPtr->compare_func = sortCompareFunc;
    binomialHeapPtr->roots = NULL;
    binomialHeapPtr->roots_length = 0;
    //功能辅助
    ADT_SET_IDENTIFY_CODE_SET(binomialHeapPtr,ADT_BINOMIAL_HEAP_T_ID);
    //系统解锁
    ADT_SYS_UNLOCK(binomialHeapPtr);
    //返回结构体指针
    return binomialHeapPtr;
}

//销毁
void ADT_BinomialHeapDestroy(ADT_T* binomialHeapPtrPtr)
{
    //指针检查
    ADT_BASE_PTR_CHECK(binomialHeapPtrPtr,ADT_BinomialHeapDestroy);
    //定义实体
    ADT_T binomialHeapPtr = (*binomialHeapPtrPtr);
    //指针检查
    ADT_BASE_PTR_CHECK(binomialHeapPtr,ADT_BinomialHeapDestroy);
    //ID检查
    ADT_BASE_ID_CHECK(binomialHeapPtr,ADT_BinomialHeapDestroy,ADT_BINOMIAL_HEAP_T_ID);
    //释放数据
    ADT_BinomialHeapClear(binomialHeapPtr);
    //释放自身
    ADT_FREE_SELF(binomialHeapPtr);
    return;
}

//释放全部的数据
void ADT_BinomialHeapClear(ADT_T binomialHeapPtr)
{
    uint32_t indexUtil = 0;
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //指针检查
    ADT_BASE_PTR_CHECK(binomialHeapPtr,ADT_BinomialHeapClear);
    if(binomialHeapPtr->num_values == 0)
    {
        //空链表无操作
        return;
    }
    //ID检查
    ADT_BASE_ID_CHECK(binomialHeapPtr,ADT_BinomialHeapClear,ADT_BINOMIAL_HEAP_T_ID);
    //系统锁定
    ADT_SYS_LOCK(binomialHeapPtr);
    for (indexUtil=0; indexUtil<binomialHeapPtr->roots_length; ++indexUtil) 
    {
		ADT_BinomialHeapTreeUnRef(binomialHeapPtr,binomialHeapPtr->roots[indexUtil]);
	}
    ADT_MEM_FREE(binomialHeapPtr,binomialHeapPtr->roots);
    binomialHeapPtr->roots_length = 0;
    //系统解锁
    ADT_SYS_UNLOCK(binomialHeapPtr);
    return;
}



//数据插入
ADT_RESULT ADT_BinomialHeapInsert(ADT_T binomialHeapPtr, void* value)
{
    struct ADT_BINOMIAL_HEAP_T fake_heap;
	ADT_NODE_T new_tree;
	ADT_RESULT result;
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //指针检查
    ADT_BASE_PTR_CHECK(binomialHeapPtr,ADT_BinomialHeapInsert);
    //ID检查
    ADT_BASE_ID_CHECK(binomialHeapPtr,ADT_BinomialHeapInsert,ADT_BINOMIAL_HEAP_T_ID);
    //系统锁定
    ADT_SYS_LOCK(binomialHeapPtr);
	new_tree = ADT_MEM_MALLOC(binomialHeapPtr,sizeof(*new_tree));
	if (new_tree == NULL) 
    {
        //系统解锁
        ADT_SYS_UNLOCK(binomialHeapPtr);
		return ADT_RESULT_FAIL;
	}
	new_tree->value = value;
	new_tree->order = 0;
	new_tree->refcount = 1;
	new_tree->subtrees = NULL;

	fake_heap.heap_type = binomialHeapPtr->heap_type;
	fake_heap.compare_func = binomialHeapPtr->compare_func;
	fake_heap.num_values = 1;
	fake_heap.roots = &new_tree;
	fake_heap.roots_length = 1;

    result = ADT_BinomialHeapMerge(binomialHeapPtr, &fake_heap);
    if (result != ADT_RESULT_FAIL)
    {
        ++binomialHeapPtr->num_values;
    }
    ADT_BinomialHeapTreeUnRef(binomialHeapPtr,new_tree);
    // 系统解锁
    ADT_SYS_UNLOCK(binomialHeapPtr);
    return result;
}

//数据弹出
void* ADT_BinomialHeapPop(ADT_T binomialHeapPtr,ADT_RESULT* resultPtr)
{
    ADT_NODE_T least_tree;
	struct ADT_BINOMIAL_HEAP_T fake_heap;
	void* result;
	uint32_t indexUtil;
	uint32_t least_index;
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //指针检查
    ADT_BASE_PTR_CHECK(binomialHeapPtr,ADT_BinomialHeapInsert);
    //ID检查
    ADT_BASE_ID_CHECK(binomialHeapPtr,ADT_BinomialHeapInsert,ADT_BINOMIAL_HEAP_T_ID);
    if (binomialHeapPtr->num_values == 0)
    {
        *resultPtr = ADT_RESULT_FAIL;
        return NULL;
    }
    //系统锁定
    ADT_SYS_LOCK(binomialHeapPtr);
	least_index = UINT32_MAX;

	for (indexUtil=0; indexUtil < binomialHeapPtr->roots_length; ++indexUtil) 
    {
		if (binomialHeapPtr->roots[indexUtil] == NULL) 
        {
			continue;
		}
		if ((least_index == UINT32_MAX)
		    || (ADT_BinomialHeapCmp(binomialHeapPtr,binomialHeapPtr->roots[indexUtil]->value,
		                            binomialHeapPtr->roots[least_index]->value) < 0)) 
        {
			least_index = indexUtil;
		}
	}
	least_tree = binomialHeapPtr->roots[least_index];
	binomialHeapPtr->roots[least_index] = NULL;

	fake_heap.heap_type = binomialHeapPtr->heap_type;
	fake_heap.compare_func = binomialHeapPtr->compare_func;
	fake_heap.roots = least_tree->subtrees;
	fake_heap.roots_length = least_tree->order;

	if (ADT_RESULT_OK == ADT_BinomialHeapMerge(binomialHeapPtr, &fake_heap)) 
    {
		result = least_tree->value;
		ADT_BinomialHeapTreeUnRef(binomialHeapPtr,least_tree);
		--binomialHeapPtr->num_values;
        *resultPtr = ADT_RESULT_OK;
        //系统解锁
        ADT_SYS_UNLOCK(binomialHeapPtr);
		return result;

	} 
    else 
    {
		binomialHeapPtr->roots[least_index] = least_tree;
		*resultPtr = ADT_RESULT_FAIL;
        //系统解锁
        ADT_SYS_UNLOCK(binomialHeapPtr);
		return NULL;
	}
}

uint32_t ADT_BinomialHeapNumEntries(ADT_T binomialHeapPtr)
{
    //指针检查
    ADT_BASE_PTR_CHECK(binomialHeapPtr,ADT_BinomialHeapNumEntries);
    //ID检查
    ADT_BASE_ID_CHECK(binomialHeapPtr,ADT_BinomialHeapNumEntries,ADT_BINOMIAL_HEAP_T_ID);
    return binomialHeapPtr->num_values;
}