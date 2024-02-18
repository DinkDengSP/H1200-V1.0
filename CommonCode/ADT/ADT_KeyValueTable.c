/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-03-05 22:26:48 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-03-06 13:48:22 +0800
************************************************************************************************/ 
#include "ADT_KeyValueTable.h"

//宏定义结构体类型,这种定义的好处是不用出现大量的各种类型名,代码整洁
//理解上差点意思,但是使用者不需要太了解内部结构
#define ADT_T                           ADT_KEY_VALUE_TABLE_T
#define ADT_NODE_T                      ADT_KEY_VALUE_TABLE_NODE_T
//结构体的类型ID,用于程序运行时的检查
//_COUNTER_  一个从0开始的全局的计数器，每次用于源文件或在源文件中被引用头文件时，自动加1
#define ADT_KEY_VALUE_TABLE_T_ID         0X20200718

//预设容量定义
static const uint32_t primes[] = {193, 389, 769, 1543, 3079, 6151, 12289, 24593, 49157, 98317,
                                    196613, 393241, 786433, 1572869, 3145739, 6291469,
                                    12582917, 25165843, 50331653, 100663319, 201326611,
                                    402653189, 805306457, 1610612741, UINT32_MAX};

//节点定义
struct ADT_NODE_T 
{
    struct ADT_NODE_T *link;
    const void *key;
    void *value;
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
    ADT_KeyValueTableCompare tableKeyCompareFunc;
    ADT_KeyValueTableCreateHashCode hashCodeCreateFunc;
    //识别ID
    uint32_t adtTypeID;
    //数据定义
    uint32_t timestamp;
    uint16_t bucketCapacity;   //队列桶容量
    uint16_t currentSize;      //当前有效数据长度
    //类似于链表的结构,这个一定要放在结构体最后,方便内存申请
    struct ADT_NODE_T **buckets;//桶结构,快速搜索
};

//默认的键比较函数
static int32_t ADT_KeyValueTableCompareDefault(const void* x,const void* y)
{
    if(x > y)
    {
        return 1;
    }
    else if(x == y)
    {
        return 0;
    }
    else 
    {
        return -1;
    }
}

//默认的哈希代码生成函数
static uint32_t ADT_KeyValueTableCreateHashCodeDefault(const void* key)
{
    // 右移两位是因为可能每个原子都起始于字边界，因此最右侧两位可能是0。
    return (uint32_t)key >> 2; 
}

//表初始化
ADT_T ADT_KeyValueTableCreate(uint16_t bucketCapacitySet,ADT_MemMallocFuncPtr memMallocFunc,ADT_MemFreeFuncPtr memFreeFunc,
                                ADT_LockSystem lockSysFuncPtr,ADT_UnLockSystem unlockSysFuncPtr,ADT_SendString sendStrFunc,
                                ADT_KeyValueTableCompare compFuncPtr,ADT_KeyValueTableCreateHashCode hashCodeCreateFuncPtr)
{
    uint32_t indexUtil = 0;
    if(bucketCapacitySet == 0)
    {
        return NULL;
    }
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //回调函数检查
    ADT_CALL_BACK_CHECK(ADT_KeyValueTableCreate,memMallocFunc,memFreeFunc,lockSysFuncPtr,unlockSysFuncPtr,sendStrFunc);
    //生成变量
    ADT_T keyValueTablePtr = NULL;
    //内存申请
    ADT_MALLOC_SELF(keyValueTablePtr,memMallocFunc);
    //函数映射
    ADT_CALL_BACK_REGISTER(keyValueTablePtr,memMallocFunc,memFreeFunc,lockSysFuncPtr,unlockSysFuncPtr,sendStrFunc);
    keyValueTablePtr->tableKeyCompareFunc = compFuncPtr;
    if(keyValueTablePtr->tableKeyCompareFunc == NULL)
    {
        keyValueTablePtr->tableKeyCompareFunc = ADT_KeyValueTableCompareDefault;
    }
    keyValueTablePtr->hashCodeCreateFunc = hashCodeCreateFuncPtr;
    if(keyValueTablePtr->hashCodeCreateFunc == NULL)
    {
        keyValueTablePtr->hashCodeCreateFunc = ADT_KeyValueTableCreateHashCodeDefault;
    }
    //系统锁定
    ADT_SYS_LOCK(keyValueTablePtr);
    //计算初始化容量
    for (indexUtil = 1; primes[indexUtil] < bucketCapacitySet; ++indexUtil) 
    {
        asm("nop");
    }
    //内存申请
    keyValueTablePtr->buckets = ADT_MEM_MALLOC(keyValueTablePtr,primes[indexUtil-1]*sizeof(keyValueTablePtr->buckets[0]));
    keyValueTablePtr->bucketCapacity = primes[indexUtil-1];
    //桶赋值
    for (indexUtil = 0; indexUtil < keyValueTablePtr->bucketCapacity; ++indexUtil) 
    {
        keyValueTablePtr->buckets[indexUtil] = NULL;
    }
    keyValueTablePtr->currentSize = 0;
    keyValueTablePtr->timestamp = 0;
    //功能辅助
    ADT_SET_IDENTIFY_CODE_SET(keyValueTablePtr,ADT_KEY_VALUE_TABLE_T_ID);
    //系统解锁
    ADT_SYS_UNLOCK(keyValueTablePtr);
    //返回结构体指针
    return keyValueTablePtr;
}

//销毁表
void ADT_KeyValueTableDestroy(ADT_T* keyValueTablePtrPtr)
{
    //指针检查
    ADT_BASE_PTR_CHECK(keyValueTablePtrPtr,ADT_KeyValueTableDestroy);
    //定义实体
    ADT_T keyValueTablePtr = (*keyValueTablePtrPtr);
    //清空数据
    ADT_KeyValueTableClear(keyValueTablePtr);
    //清空桶
    ADT_MEM_FREE(keyValueTablePtr,keyValueTablePtr->buckets);
    //释放自身
    ADT_FREE_SELF(keyValueTablePtr);
    return;
}

//释放全部的表数据
void ADT_KeyValueTableClear(ADT_T keyValueTablePtr)
{
    uint32_t indexUtil = 0;
    struct ADT_NODE_T *p, *q;
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //指针检查
    ADT_BASE_PTR_CHECK(keyValueTablePtr,ADT_KeyValueTableClear);
    //ID检查
    ADT_BASE_ID_CHECK(keyValueTablePtr,ADT_KeyValueTableClear,ADT_KEY_VALUE_TABLE_T_ID);
    //数据为0,直接返回
    if(keyValueTablePtr->currentSize == 0)
    {
        return;
    }
    //系统锁定
    ADT_SYS_LOCK(keyValueTablePtr);
    //遍历
    for (indexUtil = 0; indexUtil < keyValueTablePtr->bucketCapacity; indexUtil++) 
    {
        for (p = keyValueTablePtr->buckets[indexUtil]; p; p = q) 
        {
            //移动到下一个元素
            q = p->link;
            //释放当前元素
            ADT_MEM_FREE(keyValueTablePtr,p);
        }
    }
    //元素数量清空
    keyValueTablePtr->currentSize = 0;
    //系统解锁
    ADT_SYS_UNLOCK(keyValueTablePtr);
    return;
}


//获取当前表中元素数量
uint32_t ADT_KeyValueTableGetElementCount(ADT_T keyValueTablePtr)
{
    //指针检查
    ADT_BASE_PTR_CHECK(keyValueTablePtr,ADT_KeyValueTableGetElementCount);
    //ID检查
    ADT_BASE_ID_CHECK(keyValueTablePtr,ADT_KeyValueTableGetElementCount,ADT_KEY_VALUE_TABLE_T_ID);
    //返回当前尺寸
    return keyValueTablePtr->currentSize;
}

//添加一个键值对,如果原来就存在这个键,新的值取代旧的值,旧的值被返回,否则返回空
void* ADT_KeyValueTablePut(ADT_T keyValueTablePtr,const void *key, void *value,ADT_RESULT* putResultPtr)
{
    uint32_t indexUtil = 0;
    struct ADT_NODE_T *p;
    void *prev;
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //指针检查
    ADT_BASE_PTR_CHECK(keyValueTablePtr,ADT_KeyValueTablePut);
    //ID检查
    ADT_BASE_ID_CHECK(keyValueTablePtr,ADT_KeyValueTablePut,ADT_KEY_VALUE_TABLE_T_ID);
    //检查元素,空元素不处理
    if(key == NULL)
    {
        *putResultPtr = ADT_RESULT_FAIL;
        return NULL;
    }
    //系统锁定
    ADT_SYS_LOCK(keyValueTablePtr);
    //搜索对应的节点
    //计算哈希代码,计算桶序号
    indexUtil = (keyValueTablePtr->hashCodeCreateFunc(key))%(keyValueTablePtr->bucketCapacity);
    for (p = keyValueTablePtr->buckets[indexUtil]; p; p = p->link) 
    {
        //键匹配
        if ((keyValueTablePtr->tableKeyCompareFunc(key, p->key)) == 0) 
        {
            break;
        }
    }
    //根据搜索结果决定后续操作
    if (p == NULL) 
    {
        //为空,生成新的节点
        p = ADT_MEM_MALLOC(keyValueTablePtr,sizeof(struct ADT_NODE_T));
        //节点赋值
        p->key = key;
        //链接到桶原先的表头
        p->link = keyValueTablePtr->buckets[indexUtil];
        //桶表头链接到新的指针
        keyValueTablePtr->buckets[indexUtil] = p;
        //数量增加
        keyValueTablePtr->currentSize += 1;
        //前一个节点
        prev = NULL;
    } 
    else 
    {
        //已经存在这个节点了
        prev = p->value;
    }
    //数值链接
    p->value = value;
    //设置时间戳
    keyValueTablePtr->timestamp = ADT_BASE_GET_TIME_STAMP();
    //系统解锁
    ADT_SYS_UNLOCK(keyValueTablePtr);
    *putResultPtr = ADT_RESULT_OK;
    //返回设置之前的数值,不存在返回空
    return prev;
}

//获取一个键值对,返回查找值,如果不存在则返回空
void* ADT_KeyValueTableGet(ADT_T keyValueTablePtr,const void *key,ADT_RESULT* putResultPtr)
{
    uint32_t indexUtil = 0;
    struct ADT_NODE_T *p;
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //指针检查
    ADT_BASE_PTR_CHECK(keyValueTablePtr,ADT_KeyValueTableGet);
    //ID检查
    ADT_BASE_ID_CHECK(keyValueTablePtr,ADT_KeyValueTableGet,ADT_KEY_VALUE_TABLE_T_ID);
    //没有数据元素,直接返回空
    if(keyValueTablePtr->currentSize == 0)
    {
        *putResultPtr = ADT_RESULT_FAIL;
        return NULL;
    }
    //键为空
    if(key == NULL)
    {
        *putResultPtr = ADT_RESULT_FAIL;
        return NULL;
    }
    //系统锁定
    ADT_SYS_LOCK(keyValueTablePtr);
    //计算哈希代码,计算桶序号
    indexUtil = (keyValueTablePtr->hashCodeCreateFunc(key))%(keyValueTablePtr->bucketCapacity);
    for (p = keyValueTablePtr->buckets[indexUtil]; p; p = p->link) 
    {
        //键匹配
        if ((keyValueTablePtr->tableKeyCompareFunc(key, p->key)) == 0) 
        {
            break;
        }
    }
    //系统解锁
    ADT_SYS_UNLOCK(keyValueTablePtr);
    *putResultPtr = ADT_RESULT_OK;
    //结果返回
    return p ? p->value : NULL;
}

//删除指定键值对,返回被删除的值,如果不存在则返回空
void* ADT_KeyValueTableRemove(ADT_T keyValueTablePtr,const void *key,ADT_RESULT* putResultPtr)
{
    uint32_t indexUtil = 0;
    struct ADT_NODE_T **pp;
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //指针检查
    ADT_BASE_PTR_CHECK(keyValueTablePtr,ADT_KeyValueTableRemove);
    //ID检查
    ADT_BASE_ID_CHECK(keyValueTablePtr,ADT_KeyValueTableRemove,ADT_KEY_VALUE_TABLE_T_ID);
    //没有数据元素,直接返回空
    if(keyValueTablePtr->currentSize == 0)
    {
        *putResultPtr = ADT_RESULT_FAIL;
        return NULL;
    }
    //键为空
    if(key == NULL)
    {
        *putResultPtr = ADT_RESULT_FAIL;
        return NULL;
    }
    //系统锁定
    ADT_SYS_LOCK(keyValueTablePtr);
    //设置时间戳
    keyValueTablePtr->timestamp = ADT_BASE_GET_TIME_STAMP();
    //计算哈希代码,计算桶序号
    indexUtil = (keyValueTablePtr->hashCodeCreateFunc(key))%(keyValueTablePtr->bucketCapacity);
    //桶头开始查找
    for (pp = &(keyValueTablePtr->buckets[indexUtil]); *pp; pp = &(*pp)->link) 
    {
        if ((keyValueTablePtr->tableKeyCompareFunc(key, (*pp)->key)) == 0) 
        {
            //找到了
            struct ADT_NODE_T *p = *pp;
            //获取VALUE
            void *value = p->value;
            //连接到下一个元素
            *pp = p->link;
            //释放原来的元素
            ADT_MEM_FREE(keyValueTablePtr,p);
            //计数值1
            keyValueTablePtr->currentSize -= 1;
            //系统解锁
            ADT_SYS_UNLOCK(keyValueTablePtr);
            *putResultPtr = ADT_RESULT_OK;
            return value;
        }
    }
    //系统解锁
    ADT_SYS_UNLOCK(keyValueTablePtr);
    //没找到
    *putResultPtr = ADT_RESULT_OK;
    return NULL;
}

//链表转换为指针数组
void** ADT_KeyValueTableToArray(ADT_T keyValueTablePtr,uint16_t* elementLength)
{
    uint32_t indexUtil1 = 0;
    uint32_t indexUtil2 = 0;
    void **array;
    struct ADT_NODE_T *p;
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //指针检查
    ADT_BASE_PTR_CHECK(keyValueTablePtr,ADT_KeyValueTableToArray);
    //ID检查
    ADT_BASE_ID_CHECK(keyValueTablePtr,ADT_KeyValueTableToArray,ADT_KEY_VALUE_TABLE_T_ID);
    //没有数据元素,直接返回空
    if(keyValueTablePtr->currentSize == 0)
    {
        *elementLength = 0;
        return NULL;
    }
    //系统锁定
    ADT_SYS_LOCK(keyValueTablePtr);
    //内存申请
    array = ADT_MEM_MALLOC(keyValueTablePtr,(2*(keyValueTablePtr->currentSize))*sizeof(*array));
    //遍历
    for (indexUtil1 = 0; indexUtil1 < keyValueTablePtr->bucketCapacity; indexUtil1++) 
    {
        //桶查找
        for (p = keyValueTablePtr->buckets[indexUtil1]; p; p = p->link) 
        {
            //键
            array[indexUtil2++] = (void *)p->key;
            //值
            array[indexUtil2++] = p->value;
        }
    }
    //系统解锁
    ADT_SYS_UNLOCK(keyValueTablePtr);
    *elementLength = 2*(keyValueTablePtr->currentSize);
    return array;
}

//释放指针数组
void ADT_KeyValueTableFreeArray(ADT_T keyValueTablePtr,void* arrayPtr)
{
    //指针检查
    ADT_BASE_PTR_CHECK(keyValueTablePtr,ADT_KeyValueTableFreeArray);
    //ID检查
    ADT_BASE_ID_CHECK(keyValueTablePtr,ADT_KeyValueTableFreeArray,ADT_KEY_VALUE_TABLE_T_ID);
    //没有数据元素,直接返回空
    if(arrayPtr == NULL)
    {
        return;
    }
    //释放内存
    keyValueTablePtr->memFreeFunc(arrayPtr);
}

//遍历列表
void ADT_KeyValueTableTraverse(ADT_T keyValueTablePtr,void operationFunc(const void* key, void **value, void *param),
                                    void* param)

{
    uint32_t indexUtil = 0;
    struct ADT_NODE_T *p;
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //指针检查
    ADT_BASE_PTR_CHECK(keyValueTablePtr,ADT_KeyValueTableTraverse);
    //ID检查
    ADT_BASE_ID_CHECK(keyValueTablePtr,ADT_KeyValueTableTraverse,ADT_KEY_VALUE_TABLE_T_ID);
    //没有数据元素,直接返回空
    if(keyValueTablePtr->currentSize == 0)
    {
        return;
    }
    //操作函数为空
    if(operationFunc == NULL)
    {
        return;
    }
    //系统锁定
    ADT_SYS_LOCK(keyValueTablePtr);
    //遍历
    for (indexUtil = 0; indexUtil < keyValueTablePtr->bucketCapacity; indexUtil++) 
    {
        //桶查找
        for (p = keyValueTablePtr->buckets[indexUtil]; p; p = p->link) 
        {
            //操作
            operationFunc(p->key,&(p->value),param);
        }
    }
    //系统解锁
    ADT_SYS_UNLOCK(keyValueTablePtr);
    return;
}
