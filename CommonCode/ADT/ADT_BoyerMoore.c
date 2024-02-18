/***********************************************************************************************
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-03-09 12:00:45 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :
** ModifyRecord2 :
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-03-09 12:00:45 +0800
************************************************************************************************/
#include "ADT_BoyerMoore.h"

//宏定义结构体类型,这种定义的好处是不用出现大量的各种类型名,代码整洁
//理解上差点意思,但是使用者不需要太了解内部结构
#define ADT_T                               ADT_BOYER_MOORE_T
//结构体的类型ID,用于程序运行时的检查
#define ADT_BOYER_MOORE_T_ID                0X2020071F
//辅助支持表长度,字符的种类
#define ADT_BOYER_MOORE_TABLE_LENGTH        256
//比较大的值
#define ADT_BOYER_MOORE_MAX(x, y)           (x)>(y) ? (x):(y)

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
};

//创建
ADT_T ADT_BoyerMooreCreate(ADT_MemMallocFuncPtr memMallocFunc,ADT_MemFreeFuncPtr memFreeFunc,
                            ADT_LockSystem lockSysFuncPtr,ADT_UnLockSystem unlockSysFuncPtr,
                            ADT_SendString sendStrFunc)
{
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //回调函数检查
    ADT_CALL_BACK_CHECK(ADT_BoyerMooreCreate,memMallocFunc,memFreeFunc,lockSysFuncPtr,unlockSysFuncPtr,sendStrFunc);
    //生成变量
    ADT_T bmPtr = NULL;
    //内存申请
    ADT_MALLOC_SELF(bmPtr,memMallocFunc);
    //函数映射
    ADT_CALL_BACK_REGISTER(bmPtr,memMallocFunc,memFreeFunc,lockSysFuncPtr,unlockSysFuncPtr,sendStrFunc);
    //系统锁定
    ADT_SYS_LOCK(bmPtr);
    //功能辅助
    ADT_SET_IDENTIFY_CODE_SET(bmPtr,ADT_BOYER_MOORE_T_ID);
    //系统解锁
    ADT_SYS_UNLOCK(bmPtr);
    //返回结构体指针
    return bmPtr;
}

//销毁
void ADT_BoyerMooreDestroy(ADT_T *bmPtrPtr)
{
    //指针检查
    ADT_BASE_PTR_CHECK(bmPtrPtr,ADT_BoyerMooreDestroy);
    //定义实体
    ADT_T bmPtr = (*bmPtrPtr);
    //指针检查
    ADT_BASE_PTR_CHECK(bmPtr,ADT_BoyerMooreDestroy);
    //ID检查
    ADT_BASE_ID_CHECK(bmPtr,ADT_BoyerMooreDestroy,ADT_BOYER_MOORE_T_ID);
    //释放自身
    ADT_FREE_SELF(bmPtr);
    return;
}

//模式串的坏匹配表
static int32_t* ADT_BoyerMooreBuildBadTable(ADT_T bmPtr,uint8_t* patternString,uint32_t lengthPatternString)
{
    //创建表
    int32_t* badTablePtr = ADT_MEM_MALLOC(bmPtr,sizeof(int32_t)*ADT_BOYER_MOORE_TABLE_LENGTH);
    int32_t indexUtil = 0;
    for (indexUtil = 0; indexUtil < ADT_BOYER_MOORE_TABLE_LENGTH; indexUtil++)
    {
        badTablePtr[indexUtil] = (int32_t)lengthPatternString;
        // 默认初始化全部为匹配字符串长度,因为当主串中的坏字符在模式串中没有出
        // 现时，直接跳过整个模式串的长度就可以了
    }
    for (indexUtil = 0; indexUtil < lengthPatternString - 1; indexUtil++)
    {
        uint8_t k = patternString[indexUtil]; // 记录下当前的字符ASCII码值
        // 这里其实很值得思考一下，bad_table就不多说了，是根据字符的ASCII值存储
        // 坏字符出现最右的位置，这在上面实现坏字符的时候也说过了。不过你仔细思考
        // 一下，为什么这里存的坏字符数值，是最右的那个坏字符相对于模式串最后一个
        // 字符的位置？为什么？首先你要理解i的含义，这个i不是在这里的i，而是在上面
        // 那个pattern函数的循环的那个i，为了方便我们称呼为I，这个I很神奇，虽然I是
        // 在主串上的指针，但是由于在循环中没有使用skip来记录，直接使用I随着j匹配
        // 进行移动，也就意味着，在某种意义上，I也可以直接定位到模式串的相对位置，
        // 理解了这一点，就好理解在本循环中，i的行为了。

        // 其实仔细去想一想，我们分情况来思考，如果模式串的最
        // 后一个字符，也就是匹配开始的第一个字符，出现了坏字符，那么这个时候，直
        // 接移动这个数值，那么正好能让最右的那个字符正对坏字符。那么如果不是第一个
        // 字符出现坏字符呢？这种情况你仔细想一想，这种情况也就意味着出现了好后缀的
        // 情况，假设我们将最右的字符正对坏字符
        badTablePtr[k] = lengthPatternString - 1 - indexUtil;
    }
    return badTablePtr;
}

// 前缀匹配
static ADT_BOOL ADT_BoyerMooreIsPrefix(uint8_t* patternString,uint32_t lengthPatternString, uint32_t startPos)
{
    uint32_t indexUtil = 0;
    uint32_t indexSub = 0;
    // 这里j从模式串第一个字符开始，i从指定的字符位置开始，通过循环判断当前指定的位置p
    // 之后的字符串是否匹配模式串前缀
    for (indexUtil = startPos, indexSub = 0; indexUtil < lengthPatternString; ++indexUtil, ++indexSub)
    {
        if (patternString[indexUtil] != patternString[indexSub])
        {
            return ADT_BOOL_FALSE;
        }
    }
    return ADT_BOOL_TRUE;
}

//后缀匹配
static uint32_t ADT_BoyerMooreSuffixLength(uint8_t* patternString,uint32_t lengthPatternString, uint32_t startPos)
{
    uint32_t len = 0;
    int indexMain = 0;
    int indexSub = 0;
    for (indexMain = startPos, indexSub = lengthPatternString - 1; 
            ((indexMain >= 0)&&(patternString[indexMain] == patternString[indexSub])); 
            indexMain--, indexSub--)
    {
        len += 1;
    }
    return len;
}

// 模式串的好匹配表
static int32_t* ADT_BoyerMooreBuildGoodTable(ADT_T bmPtr,uint8_t* patternString,uint32_t lengthPatternString)
{
    // 创建一个数组，存好后缀数值
    int32_t* goodTablePtr = ADT_MEM_MALLOC(bmPtr,sizeof(int32_t)*lengthPatternString);
    int32_t indexUtil = 0;
    uint32_t slen = 0;
    // 用于记录最新前缀的相对位置，初始化为模式串长度，因为意思就是当前后缀字符串为空
    // 要明白lastPrefixPosition 的含义
    uint32_t lastPrefixPosition = lengthPatternString;
    for (indexUtil = lengthPatternString - 1; indexUtil >= 0; --indexUtil)
    {
        if (ADT_BOOL_TRUE == ADT_BoyerMooreIsPrefix(patternString,lengthPatternString, indexUtil + 1))
        {
            // 如果当前的位置存在前缀匹配，那么记录当前位置
            lastPrefixPosition = indexUtil + 1;
        }
        goodTablePtr[lengthPatternString - 1 - indexUtil] = lastPrefixPosition - indexUtil + lengthPatternString - 1;
    }
    for (indexUtil = 0; indexUtil < lengthPatternString - 1; ++indexUtil)
    {
        // 计算出指定位置匹配的后缀的字符串长度
        slen = ADT_BoyerMooreSuffixLength(patternString,lengthPatternString,indexUtil);
        goodTablePtr[slen] = lengthPatternString - 1 - indexUtil + slen;
    }
    return goodTablePtr;
}

//KMP匹配算法,参数为查找串,查找串长度,目标串,目标串长度,匹配开始序号,存储查找结果的指针
ADT_RESULT ADT_BoyerMooreMatch(ADT_T bmPtr,uint8_t* textStr,uint8_t* patternStr,uint32_t lengthTextStr,
                                    uint32_t lengthPatternStr,uint32_t matchStartPos,uint32_t* matchIndexPtr)
{
    uint32_t indexUtil = 0;
    uint32_t indexSub = 0;
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //指针检查
    ADT_BASE_PTR_CHECK(bmPtr,ADT_BoyerMooreMatch);
    //ID检查
    ADT_BASE_ID_CHECK(bmPtr,ADT_BoyerMooreMatch,ADT_BOYER_MOORE_T_ID);
    //不能是空
    if(textStr == NULL)
    {
        *matchIndexPtr = 0;
        return ADT_RESULT_FAIL;
    }
    //长度不能为0
    if(lengthTextStr == 0)
    {
        *matchIndexPtr = 0;
        return ADT_RESULT_FAIL;
    }
    //不能是空
    if(patternStr == NULL)
    {
        *matchIndexPtr = 0;
        return ADT_RESULT_FAIL;
    }
    //长度不能为0
    if(lengthPatternStr == 0)
    {
        *matchIndexPtr = 0;
        return ADT_RESULT_FAIL;
    }
    //源串要大于等于模式串
    if(lengthTextStr < lengthPatternStr)
    {
        *matchIndexPtr = 0;
        return ADT_RESULT_FAIL;
    }
    //源串要大于起始位置
    if(lengthTextStr <= matchStartPos)
    {
        *matchIndexPtr = 0;
        return ADT_RESULT_FAIL;
    }
    //总长度减去起始长度剩下来的长度要大于等于模式串长度
    if((lengthTextStr - matchStartPos) < lengthPatternStr)
    {
        *matchIndexPtr = 0;
        return ADT_RESULT_FAIL;
    }
    *matchIndexPtr = 0XFFFFFFFF;
    //系统锁定
    ADT_SYS_LOCK(bmPtr);
    //构建两个表
    int32_t* badTablePtr = ADT_BoyerMooreBuildBadTable(bmPtr,patternStr,lengthPatternStr);   
    int32_t* goodTabPtr = ADT_BoyerMooreBuildGoodTable(bmPtr,patternStr,lengthPatternStr); 
    //开始查找
    for (indexUtil = matchStartPos + lengthPatternStr - 1; indexUtil < lengthTextStr;)
    {
        // 这里和上面实现坏字符的时候不一样的地方，我们之前提前求出坏字符以及好后缀
        // 对应的数值数组，所以，我们只要在一边循环中进行比较。还要说明的一点是，这里
        // 没有使用skip记录跳过的位置，直接针对主串中移动的指针i进行移动
        for (indexSub = lengthPatternStr - 1; textStr[indexUtil] == patternStr[indexSub]; indexUtil--, indexSub--)
        {
            if (indexSub == 0)
            { 
                *matchIndexPtr = indexUtil;
                break;
            }
        }
        if(*matchIndexPtr != 0XFFFFFFFF)
        {
            break;
        }
        // 如果出现坏字符，那么这个时候比较坏字符以及好后缀的数组，哪个大用哪个
        indexUtil += ADT_BOYER_MOORE_MAX(goodTabPtr[lengthPatternStr - indexSub - 1], badTablePtr[textStr[indexUtil]]);
    }
    //释放内存
    ADT_MEM_FREE(bmPtr,badTablePtr);
    ADT_MEM_FREE(bmPtr,goodTabPtr);
    ADT_SYS_UNLOCK(bmPtr);
    //完成
    return ADT_RESULT_OK;
}