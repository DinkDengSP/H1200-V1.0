/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-03-06 14:44:49 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-03-07 15:39:57 +0800
************************************************************************************************/ 
#include "ADT_BitVector.h"

//宏定义结构体类型,这种定义的好处是不用出现大量的各种类型名,代码整洁
//理解上差点意思,但是使用者不需要太了解内部结构
#define ADT_T                               ADT_BIT_VECTOR_T
//结构体的类型ID,用于程序运行时的检查
#define ADT_BIT_VECTOR_T_ID                 0X2020071B

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
    uint32_t vectorBitLength;//位向量的长度
    uint8_t *bytes;//存储数据的指针
    uint32_t *words;//存储数据的指针,与bytes指向相同的地方,访问方法不同,加速访问
};

//一个word中量子比特的数量
#define ADT_BIT_VECTOR_BIT_PER_WORD             (8 * sizeof(uint32_t))
//存储指定长度的bit需要几个word
#define ADT_BIT_VECTOR_N_WORDS(len)             ((((len) + ADT_BIT_VECTOR_BIT_PER_WORD -1) & (~(ADT_BIT_VECTOR_BIT_PER_WORD-1))) / ADT_BIT_VECTOR_BIT_PER_WORD)
//存储指定长度的bit需要几个BYTE
#define ADT_BIT_VECTOR_N_BYTES(len)             ((((len) + 8 - 1)&(~(8-1))) / 8)
//高四位的匹配掩码
static uint8_t msbmask[] = {0xFF, 0xFE, 0xFC, 0xF8,0xF0, 0xE0, 0xC0, 0x80};
//低四位的匹配掩码
static uint8_t lsbmask[] = {0x01, 0x03, 0x07, 0x0F,0x1F, 0x3F, 0x7F, 0xFF};


//创建向量,指定向量初始长度
ADT_T ADT_BitVectorCreate(uint32_t length,ADT_MemMallocFuncPtr memMallocFunc,ADT_MemFreeFuncPtr memFreeFunc,
                                ADT_LockSystem lockSysFuncPtr,ADT_UnLockSystem unlockSysFuncPtr,ADT_SendString sendStrFunc)
{
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //回调函数检查
    ADT_CALL_BACK_CHECK(ADT_BitVectorCreate,memMallocFunc,memFreeFunc,lockSysFuncPtr,unlockSysFuncPtr,sendStrFunc);
    //生成变量
    ADT_T bitVectorPtr = NULL;
    //检查数据长度
    if(length == 0)
    {
        return NULL;
    }
    //内存申请
    ADT_MALLOC_SELF(bitVectorPtr,memMallocFunc);
    //函数映射
    ADT_CALL_BACK_REGISTER(bitVectorPtr,memMallocFunc,memFreeFunc,lockSysFuncPtr,unlockSysFuncPtr,sendStrFunc);
    //系统锁定
    ADT_SYS_LOCK(bitVectorPtr);
    //数据复位
    bitVectorPtr->vectorBitLength = length;
    //创建头节点
    bitVectorPtr->words = ADT_MEM_MALLOC(bitVectorPtr,(ADT_BIT_VECTOR_N_WORDS(length)*sizeof(uint32_t)));
    bitVectorPtr->bytes = (uint8_t*)(bitVectorPtr->words);
    //初始化全部为0
    ADT_BASE_MEM_CLEAR(bitVectorPtr->bytes,ADT_BIT_VECTOR_N_BYTES(bitVectorPtr->vectorBitLength));
    //功能辅助
    ADT_SET_IDENTIFY_CODE_SET(bitVectorPtr,ADT_BIT_VECTOR_T_ID);
    //系统解锁
    ADT_SYS_UNLOCK(bitVectorPtr);
    //返回结构体指针
    return bitVectorPtr;
}

//创建一个拷贝对象
static ADT_T ADT_BitVectorCreateObjectCopy(ADT_T bitVectorPtr)
{
    ADT_T newBitVectorPtr;
    newBitVectorPtr = ADT_BitVectorCreate(bitVectorPtr->vectorBitLength,bitVectorPtr->memMallocFunc,
                                            bitVectorPtr->memFreeFunc,bitVectorPtr->sysLockFunc,
                                            bitVectorPtr->sysUnlockFunc,bitVectorPtr->sendStrFunc);
    if (bitVectorPtr->vectorBitLength > 0) 
    {
        ADT_BASE_MEM_COPY(newBitVectorPtr->bytes, bitVectorPtr->bytes, ADT_BIT_VECTOR_N_BYTES(bitVectorPtr->vectorBitLength));
    }
    return newBitVectorPtr;
}

//获取向量长度
int ADT_BitVectorGetLength(ADT_T bitVectorPtr)
{
    //指针检查
    ADT_BASE_PTR_CHECK(bitVectorPtr,ADT_BitVectorGetLength);
    //ID检查
    ADT_BASE_ID_CHECK(bitVectorPtr,ADT_BitVectorGetLength,ADT_BIT_VECTOR_T_ID);
    //返回当前尺寸
    return bitVectorPtr->vectorBitLength;
}

//获取向量中为1的比特的长度
int ADT_BitVectorGetBitSetCount(ADT_T bitVectorPtr)
{
    int length = 0, n;
    //0-15每个数字包含几个1
    static char count[] = {0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4};
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //指针检查
    ADT_BASE_PTR_CHECK(bitVectorPtr,ADT_BitVectorGetBitSetCount);
    //ID检查
    ADT_BASE_ID_CHECK(bitVectorPtr,ADT_BitVectorGetBitSetCount,ADT_BIT_VECTOR_T_ID);
    //系统锁定
    ADT_SYS_LOCK(bitVectorPtr);
    for (n = ADT_BIT_VECTOR_N_BYTES(bitVectorPtr->vectorBitLength); --n >= 0; ) 
    {
        //获取数值
        unsigned char c = bitVectorPtr->bytes[n];
        //快速计算
        length += count[c&0xF] + count[c>>4];
    }
    //系统解锁
    ADT_SYS_UNLOCK(bitVectorPtr);
    return length;
}

//释放向量
void ADT_BitVectorDestroy(ADT_T* bitVectorPtrPtr)
{
    //指针检查
    ADT_BASE_PTR_CHECK(bitVectorPtrPtr,ADT_BitVectorDestroy);
    //定义实体
    ADT_T bitVectorPtr = (*bitVectorPtrPtr);
    //指针检查
    ADT_BASE_PTR_CHECK(bitVectorPtr,ADT_BitVectorDestroy);
    //ID检查
    ADT_BASE_ID_CHECK(bitVectorPtr,ADT_BitVectorDestroy,ADT_BIT_VECTOR_T_ID);
    //释放buffer
    ADT_MEM_FREE(bitVectorPtr,bitVectorPtr->words);
    //释放自身
    ADT_FREE_SELF(bitVectorPtr);
    return;
}

//获取指定bit的值
uint8_t ADT_BitVectorGetBit(ADT_T bitVectorPtr, uint32_t bitIndex,ADT_RESULT* resultPtr)
{
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //指针检查
    ADT_BASE_PTR_CHECK(bitVectorPtr,ADT_BitVectorGetBit);
    //ID检查
    ADT_BASE_ID_CHECK(bitVectorPtr,ADT_BitVectorGetBit,ADT_BIT_VECTOR_T_ID);
    //检查位置
    if(bitIndex >= bitVectorPtr->vectorBitLength)
    {
        //系统解锁
        ADT_SYS_UNLOCK(bitVectorPtr);
        *resultPtr = ADT_RESULT_FAIL;
        return 0;
    }
    //系统锁定
    ADT_SYS_LOCK(bitVectorPtr);
    uint8_t bitValue = 0;
    bitValue = (((bitVectorPtr->bytes[bitIndex/8])>>(bitIndex%8)) && 1);
    //系统解锁
    ADT_SYS_UNLOCK(bitVectorPtr);
    *resultPtr = ADT_RESULT_OK;
    return bitValue;
}

//设置指定比特位的值
uint8_t ADT_BitVectorPut(ADT_T bitVectorPtr, uint32_t bitIndex, uint8_t bitValue,ADT_RESULT* resultPtr)
{
    uint8_t prev;
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //指针检查
    ADT_BASE_PTR_CHECK(bitVectorPtr,ADT_BitVectorPut);
    //ID检查
    ADT_BASE_ID_CHECK(bitVectorPtr,ADT_BitVectorPut,ADT_BIT_VECTOR_T_ID);
    //检查位置
    if(bitIndex >= bitVectorPtr->vectorBitLength)
    {
        //系统解锁
        ADT_SYS_UNLOCK(bitVectorPtr);
        *resultPtr = ADT_RESULT_FAIL;
        return 0;
    }
    //系统锁定
    ADT_SYS_LOCK(bitVectorPtr);
    prev = (((bitVectorPtr->bytes[bitIndex/8])>>(bitIndex%8)) && 1);
    if (bitValue == 1) 
    {
        bitVectorPtr->bytes[bitIndex/8] |= (1<<(bitIndex%8));
    } 
    else 
    {
        bitVectorPtr->bytes[bitIndex/8] &= (~(1<<(bitIndex%8)));
    }
    //系统解锁
    ADT_SYS_UNLOCK(bitVectorPtr);
    *resultPtr = ADT_RESULT_OK;
    return prev;
}

//清除指定比特区域的值
ADT_RESULT ADT_BitVectorClearRange(ADT_T bitVectorPtr,uint32_t rangeLow, uint32_t rangeHigh)
{
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //指针检查
    ADT_BASE_PTR_CHECK(bitVectorPtr,ADT_BitVectorClearRange);
    //ID检查
    ADT_BASE_ID_CHECK(bitVectorPtr,ADT_BitVectorClearRange,ADT_BIT_VECTOR_T_ID);
    //检查位置
    if(rangeHigh >= bitVectorPtr->vectorBitLength)
    {
        return ADT_RESULT_FAIL;
    }
    if(rangeLow > bitVectorPtr->vectorBitLength)
    {
        return ADT_RESULT_FAIL;
    }
    if(rangeLow > rangeHigh)
    {
        return ADT_RESULT_FAIL;
    }
    //系统锁定
    ADT_SYS_LOCK(bitVectorPtr);
    if (rangeLow/8 < rangeHigh/8) 
    {
        int i;
        bitVectorPtr->bytes[rangeLow/8] &= ~msbmask[rangeLow%8];
        for (i = rangeLow/8+1; i < rangeHigh/8; i++) 
        {
            bitVectorPtr->bytes[i] = 0;
        }
        bitVectorPtr->bytes[rangeHigh/8] &= ~lsbmask[rangeHigh%8];
    } 
    else 
    {
        bitVectorPtr->bytes[rangeLow/8] &= ~(msbmask[rangeLow%8] & lsbmask[rangeHigh%8]);
    }
    //系统解锁
    ADT_SYS_UNLOCK(bitVectorPtr);
    return ADT_RESULT_OK;
}

//设置指定比特区域的值
ADT_RESULT ADT_BitVectorSetRange(ADT_T bitVectorPtr,uint32_t rangeLow, uint32_t rangeHigh)
{
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //指针检查
    ADT_BASE_PTR_CHECK(bitVectorPtr,ADT_BitVectorSetRange);
    //ID检查
    ADT_BASE_ID_CHECK(bitVectorPtr,ADT_BitVectorSetRange,ADT_BIT_VECTOR_T_ID);
    //检查位置
    if(rangeHigh >= bitVectorPtr->vectorBitLength)
    {
        return ADT_RESULT_FAIL;
    }
    if(rangeLow > bitVectorPtr->vectorBitLength)
    {
        return ADT_RESULT_FAIL;
    }
    if(rangeLow > rangeHigh)
    {
        return ADT_RESULT_FAIL;
    }
    //系统锁定
    ADT_SYS_LOCK(bitVectorPtr);
    if (rangeLow/8 < rangeHigh/8) 
    {
        bitVectorPtr->bytes[rangeLow/8] |= msbmask[rangeLow%8];
        int i;
        for (i = rangeLow/8 + 1; i < rangeHigh/8; i++) 
        {
            bitVectorPtr->bytes[i] = 0xFF;
        }
        bitVectorPtr->bytes[rangeHigh/8] |= lsbmask[rangeHigh%8];
    } 
    else 
    {
        bitVectorPtr->bytes[rangeLow/8] |= (msbmask[rangeLow%8] & lsbmask[rangeHigh%8]);
    }
    //系统解锁
    ADT_SYS_UNLOCK(bitVectorPtr);
    return ADT_RESULT_OK;
}

//取反指定比特区域的值
ADT_RESULT ADT_BitVectorNotRange(ADT_T bitVectorPtr,uint32_t rangeLow, uint32_t rangeHigh)
{
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //指针检查
    ADT_BASE_PTR_CHECK(bitVectorPtr,ADT_BitVectorNotRange);
    //ID检查
    ADT_BASE_ID_CHECK(bitVectorPtr,ADT_BitVectorNotRange,ADT_BIT_VECTOR_T_ID);
    //检查位置
    if(rangeHigh >= bitVectorPtr->vectorBitLength)
    {
        return ADT_RESULT_FAIL;
    }
    if(rangeLow > bitVectorPtr->vectorBitLength)
    {
        return ADT_RESULT_FAIL;
    }
    if(rangeLow > rangeHigh)
    {
        return ADT_RESULT_FAIL;
    }
    //系统锁定
    ADT_SYS_LOCK(bitVectorPtr);
    if (rangeLow/8 < rangeHigh/8) 
    {
        int i;
        bitVectorPtr->bytes[rangeLow/8] ^= msbmask[rangeLow%8];
        for (i = rangeLow/8+1; i < rangeHigh/8; i++) 
        {
            bitVectorPtr->bytes[i] ^= 0xFF;
        }
        bitVectorPtr->bytes[rangeHigh/8] ^= lsbmask[rangeHigh%8];
    } 
    else 
    {
        bitVectorPtr->bytes[rangeLow/8] ^= (msbmask[rangeLow%8] & lsbmask[rangeHigh%8]);
    }
    //系统解锁
    ADT_SYS_UNLOCK(bitVectorPtr);
    return ADT_RESULT_OK;
}

//检查srcBitVectorPtr是否是testBitVectorPtr的真子集,testBitVectorPtr为0的位,srcBitVectorPtr也必然为0
//且testBitVectorPtr为1的位,在srcBitVectorPtr中必然有一个不为1
ADT_BOOL ADT_BitVectorLessThen(ADT_T srcBitVectorPtr, ADT_T testBitVectorPtr,ADT_RESULT* resultPtr)
{
    int i, lt = 0;
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //指针检查
    ADT_BASE_PTR_CHECK(srcBitVectorPtr,ADT_BitVectorLessThen);
    //ID检查
    ADT_BASE_ID_CHECK(srcBitVectorPtr,ADT_BitVectorLessThen,ADT_BIT_VECTOR_T_ID);
    //指针检查
    ADT_BASE_PTR_CHECK(testBitVectorPtr,ADT_BitVectorLessThen);
    //ID检查
    ADT_BASE_ID_CHECK(testBitVectorPtr,ADT_BitVectorLessThen,ADT_BIT_VECTOR_T_ID);
    if(testBitVectorPtr->vectorBitLength != srcBitVectorPtr->vectorBitLength)
    {
        //向量长度不等
        *resultPtr = ADT_RESULT_FAIL;
        return ADT_BOOL_FALSE;
    }
    //系统锁定
    ADT_SYS_LOCK(srcBitVectorPtr);
    for (i = ADT_BIT_VECTOR_N_WORDS(srcBitVectorPtr->vectorBitLength); --i >= 0;) 
    {
        if ((srcBitVectorPtr->words[i] & ~testBitVectorPtr->words[i]) != 0) 
        {
            //系统解锁
            ADT_SYS_UNLOCK(srcBitVectorPtr);
            //直接不包含
            *resultPtr = ADT_RESULT_OK;
            return ADT_BOOL_FALSE;
        } 
        else if (srcBitVectorPtr->words[i] != testBitVectorPtr->words[i]) 
        {
            //存在不等的地方,testBitVectorPtr为1的位,在srcBitVectorPtr中必然有一个不为1
            lt |= 1;
        }
    }
    //系统解锁
    ADT_SYS_UNLOCK(srcBitVectorPtr);
    //直接不包含
    *resultPtr = ADT_RESULT_OK;
    if(lt == 0)
    {
        return ADT_BOOL_FALSE;
    }
    return ADT_BOOL_TRUE;
}

//比较两个位向量是否相等
ADT_BOOL ADT_BitVectorCompareEqual(ADT_T srcBitVectorPtr, ADT_T testBitVectorPtr,ADT_RESULT* resultPtr)
{
    int i = 0;
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //指针检查
    ADT_BASE_PTR_CHECK(srcBitVectorPtr,ADT_BitVectorLessThen);
    //ID检查
    ADT_BASE_ID_CHECK(srcBitVectorPtr,ADT_BitVectorLessThen,ADT_BIT_VECTOR_T_ID);
    //指针检查
    ADT_BASE_PTR_CHECK(testBitVectorPtr,ADT_BitVectorLessThen);
    //ID检查
    ADT_BASE_ID_CHECK(testBitVectorPtr,ADT_BitVectorLessThen,ADT_BIT_VECTOR_T_ID);
    if(testBitVectorPtr->vectorBitLength != srcBitVectorPtr->vectorBitLength)
    {
        //向量长度不等
        *resultPtr = ADT_RESULT_FAIL;
        return ADT_BOOL_FALSE;
    }
    //系统锁定
    ADT_SYS_LOCK(srcBitVectorPtr);
    for (i = ADT_BIT_VECTOR_N_WORDS(srcBitVectorPtr->vectorBitLength); --i >= 0;) 
    {
        if (srcBitVectorPtr->words[i] != testBitVectorPtr->words[i]) 
        {
            //系统解锁
            ADT_SYS_UNLOCK(srcBitVectorPtr);
            //直接不包含
            *resultPtr = ADT_RESULT_OK;
            return ADT_BOOL_FALSE;
        }
    }
    //系统解锁
    ADT_SYS_UNLOCK(srcBitVectorPtr);
    //直接不包含
    *resultPtr = ADT_RESULT_OK;
    return ADT_BOOL_TRUE;
}

//检查srcBitVectorPtr是否是testBitVectorPtr的子集,testBitVectorPtr为0的位,srcBitVectorPtr也必然为0
//不关注testBitVectorPtr为1的位,在srcBitVectorPtr中是否为1
ADT_BOOL ADT_BitVectorLessOrEqual(ADT_T srcBitVectorPtr, ADT_T testBitVectorPtr,ADT_RESULT* resultPtr)
{
    int i = 0;
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //指针检查
    ADT_BASE_PTR_CHECK(srcBitVectorPtr,ADT_BitVectorLessThen);
    //ID检查
    ADT_BASE_ID_CHECK(srcBitVectorPtr,ADT_BitVectorLessThen,ADT_BIT_VECTOR_T_ID);
    //指针检查
    ADT_BASE_PTR_CHECK(testBitVectorPtr,ADT_BitVectorLessThen);
    //ID检查
    ADT_BASE_ID_CHECK(testBitVectorPtr,ADT_BitVectorLessThen,ADT_BIT_VECTOR_T_ID);
    if(testBitVectorPtr->vectorBitLength != srcBitVectorPtr->vectorBitLength)
    {
        //向量长度不等
        *resultPtr = ADT_RESULT_FAIL;
        return ADT_BOOL_FALSE;
    }
    //系统锁定
    ADT_SYS_LOCK(srcBitVectorPtr);
    for (i = ADT_BIT_VECTOR_N_WORDS(srcBitVectorPtr->vectorBitLength); --i >= 0;) 
    {
        if ((srcBitVectorPtr->words[i] & ~testBitVectorPtr->words[i]) != 0) 
        {
            //系统解锁
            ADT_SYS_UNLOCK(srcBitVectorPtr);
            //直接不包含
            *resultPtr = ADT_RESULT_OK;
            return ADT_BOOL_FALSE;
        }
    }
    //系统解锁
    ADT_SYS_UNLOCK(srcBitVectorPtr);
    //直接不包含
    *resultPtr = ADT_RESULT_OK;
    return ADT_BOOL_TRUE;
}

//遍历位向量
void ADT_BitVectorTraverse(ADT_T srcBitVectorPtr, void opFunc(int bitIndex, int bitValue, void *param), 
                                void *param)
{
    int n;
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //指针检查
    ADT_BASE_PTR_CHECK(srcBitVectorPtr,ADT_BitVectorTraverse);
    //ID检查
    ADT_BASE_ID_CHECK(srcBitVectorPtr,ADT_BitVectorTraverse,ADT_BIT_VECTOR_T_ID);
    //检查位置
    if(opFunc == NULL)
    {
        return;
    }
    //系统锁定
    ADT_SYS_LOCK(srcBitVectorPtr);
    for (n = 0; n < srcBitVectorPtr->vectorBitLength; n++) 
    {
        opFunc(n, ((srcBitVectorPtr->bytes[n/8]>>(n%8))&1), param);
    }
    //系统解锁
    ADT_SYS_UNLOCK(srcBitVectorPtr);
    return;
}

//获取两个集合的并集,为1的地方合在一起
ADT_T ADT_BitVectorOperationOr(ADT_T srcBitVectorPtr, ADT_T testBitVectorPtr,ADT_RESULT* resultPtr)
{
    int i = 0;
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //指针检查
    ADT_BASE_PTR_CHECK(srcBitVectorPtr,ADT_BitVectorLessThen);
    //ID检查
    ADT_BASE_ID_CHECK(srcBitVectorPtr,ADT_BitVectorLessThen,ADT_BIT_VECTOR_T_ID);
    //指针检查
    ADT_BASE_PTR_CHECK(testBitVectorPtr,ADT_BitVectorLessThen);
    //ID检查
    ADT_BASE_ID_CHECK(testBitVectorPtr,ADT_BitVectorLessThen,ADT_BIT_VECTOR_T_ID);
    if(testBitVectorPtr->vectorBitLength != srcBitVectorPtr->vectorBitLength)
    {
        //向量长度不等
        *resultPtr = ADT_RESULT_FAIL;
        return NULL;
    }
    //系统锁定
    ADT_SYS_LOCK(srcBitVectorPtr);
    //创建一个新的集合
    ADT_T newBitVectorPtr = ADT_BitVectorCreateObjectCopy(srcBitVectorPtr);
    for (i = ADT_BIT_VECTOR_N_WORDS(srcBitVectorPtr->vectorBitLength); --i >= 0;) 
    {
        newBitVectorPtr->words[i] = (srcBitVectorPtr->words[i])|(testBitVectorPtr->words[i]); 
    } 
    //系统解锁
    ADT_SYS_UNLOCK(srcBitVectorPtr);
    *resultPtr = ADT_RESULT_OK;
    return newBitVectorPtr;
}

//获取两个集合的交集
ADT_T ADT_BitVectorOperationAnd(ADT_T srcBitVectorPtr, ADT_T testBitVectorPtr,ADT_RESULT* resultPtr)
{
    int i = 0;
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //指针检查
    ADT_BASE_PTR_CHECK(srcBitVectorPtr,ADT_BitVectorLessThen);
    //ID检查
    ADT_BASE_ID_CHECK(srcBitVectorPtr,ADT_BitVectorLessThen,ADT_BIT_VECTOR_T_ID);
    //指针检查
    ADT_BASE_PTR_CHECK(testBitVectorPtr,ADT_BitVectorLessThen);
    //ID检查
    ADT_BASE_ID_CHECK(testBitVectorPtr,ADT_BitVectorLessThen,ADT_BIT_VECTOR_T_ID);
    if(testBitVectorPtr->vectorBitLength != srcBitVectorPtr->vectorBitLength)
    {
        //向量长度不等
        *resultPtr = ADT_RESULT_FAIL;
        return NULL;
    }
    //系统锁定
    ADT_SYS_LOCK(srcBitVectorPtr);
    //创建一个新的集合
    ADT_T newBitVectorPtr = ADT_BitVectorCreateObjectCopy(srcBitVectorPtr);
    for (i = ADT_BIT_VECTOR_N_WORDS(srcBitVectorPtr->vectorBitLength); --i >= 0;) 
    {
        newBitVectorPtr->words[i] = (srcBitVectorPtr->words[i])&(testBitVectorPtr->words[i]); 
    } 
    //系统解锁
    ADT_SYS_UNLOCK(srcBitVectorPtr);
    *resultPtr = ADT_RESULT_OK;
    return newBitVectorPtr;
}

//testBitVectorPtr的补集和srcBitVectorPtr按位与
ADT_T ADT_BitVectorOperationMinus(ADT_T srcBitVectorPtr, ADT_T testBitVectorPtr,ADT_RESULT* resultPtr)
{
    int i = 0;
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //指针检查
    ADT_BASE_PTR_CHECK(srcBitVectorPtr,ADT_BitVectorLessThen);
    //ID检查
    ADT_BASE_ID_CHECK(srcBitVectorPtr,ADT_BitVectorLessThen,ADT_BIT_VECTOR_T_ID);
    //指针检查
    ADT_BASE_PTR_CHECK(testBitVectorPtr,ADT_BitVectorLessThen);
    //ID检查
    ADT_BASE_ID_CHECK(testBitVectorPtr,ADT_BitVectorLessThen,ADT_BIT_VECTOR_T_ID);
    if(testBitVectorPtr->vectorBitLength != srcBitVectorPtr->vectorBitLength)
    {
        //向量长度不等
        *resultPtr = ADT_RESULT_FAIL;
        return NULL;
    }
    //系统锁定
    ADT_SYS_LOCK(srcBitVectorPtr);
    //创建一个新的集合
    ADT_T newBitVectorPtr = ADT_BitVectorCreateObjectCopy(srcBitVectorPtr);
    for (i = ADT_BIT_VECTOR_N_WORDS(srcBitVectorPtr->vectorBitLength); --i >= 0;) 
    {
        newBitVectorPtr->words[i] = (srcBitVectorPtr->words[i])&(~(testBitVectorPtr->words[i])); 
    } 
    //系统解锁
    ADT_SYS_UNLOCK(srcBitVectorPtr);
    *resultPtr = ADT_RESULT_OK;
    return newBitVectorPtr;
}

//获取两个集合的异或运算结果ADT_T srcBitVectorPtr, ADT_T testBitVectorPtr
ADT_T ADT_BitVectorOperationXor(ADT_T srcBitVectorPtr, ADT_T testBitVectorPtr,ADT_RESULT* resultPtr)
{
    int i = 0;
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //指针检查
    ADT_BASE_PTR_CHECK(srcBitVectorPtr,ADT_BitVectorLessThen);
    //ID检查
    ADT_BASE_ID_CHECK(srcBitVectorPtr,ADT_BitVectorLessThen,ADT_BIT_VECTOR_T_ID);
    //指针检查
    ADT_BASE_PTR_CHECK(testBitVectorPtr,ADT_BitVectorLessThen);
    //ID检查
    ADT_BASE_ID_CHECK(testBitVectorPtr,ADT_BitVectorLessThen,ADT_BIT_VECTOR_T_ID);
    if(testBitVectorPtr->vectorBitLength != srcBitVectorPtr->vectorBitLength)
    {
        //向量长度不等
        *resultPtr = ADT_RESULT_FAIL;
        return NULL;
    }
    //系统锁定
    ADT_SYS_LOCK(srcBitVectorPtr);
    //创建一个新的集合
    ADT_T newBitVectorPtr = ADT_BitVectorCreateObjectCopy(srcBitVectorPtr);
    for (i = ADT_BIT_VECTOR_N_WORDS(srcBitVectorPtr->vectorBitLength); --i >= 0;) 
    {
        newBitVectorPtr->words[i] = (srcBitVectorPtr->words[i])^((testBitVectorPtr->words[i])); 
    } 
    //系统解锁
    ADT_SYS_UNLOCK(srcBitVectorPtr);
    *resultPtr = ADT_RESULT_OK;
    return newBitVectorPtr;
}