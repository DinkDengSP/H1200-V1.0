/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-03-08 14:17:00 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-03-08 16:11:15 +0800
************************************************************************************************/ 
#include "ADT_KMP.h"

//宏定义结构体类型,这种定义的好处是不用出现大量的各种类型名,代码整洁
//理解上差点意思,但是使用者不需要太了解内部结构
#define ADT_T                               ADT_KMP_T
//结构体的类型ID,用于程序运行时的检查
#define ADT_KMP_T_ID                        0X2020071E

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

//创建字节栈
ADT_T ADT_KMP_Create(ADT_MemMallocFuncPtr memMallocFunc,ADT_MemFreeFuncPtr memFreeFunc,
                    ADT_LockSystem lockSysFuncPtr,ADT_UnLockSystem unlockSysFuncPtr,
                    ADT_SendString sendStrFunc)
{
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //回调函数检查
    ADT_CALL_BACK_CHECK(ADT_KMP_Create,memMallocFunc,memFreeFunc,lockSysFuncPtr,unlockSysFuncPtr,sendStrFunc);
    //生成变量
    ADT_T kmpPtr = NULL;
    //内存申请
    ADT_MALLOC_SELF(kmpPtr,memMallocFunc);
    //函数映射
    ADT_CALL_BACK_REGISTER(kmpPtr,memMallocFunc,memFreeFunc,lockSysFuncPtr,unlockSysFuncPtr,sendStrFunc);
    //系统锁定
    ADT_SYS_LOCK(kmpPtr);
    //功能辅助
    ADT_SET_IDENTIFY_CODE_SET(kmpPtr,ADT_KMP_T_ID);
    //系统解锁
    ADT_SYS_UNLOCK(kmpPtr);
    //返回结构体指针
    return kmpPtr;
}

//销毁字节栈
void ADT_KMP_Destroy(ADT_T *kmpPtrPtr)
{
    //指针检查
    ADT_BASE_PTR_CHECK(kmpPtrPtr,ADT_KMP_Destroy);
    //定义实体
    ADT_T kmpPtr = (*kmpPtrPtr);
    //指针检查
    ADT_BASE_PTR_CHECK(kmpPtr,ADT_KMP_Destroy);
    //ID检查
    ADT_BASE_ID_CHECK(kmpPtr,ADT_KMP_Destroy,ADT_KMP_T_ID);
    //释放自身
    ADT_FREE_SELF(kmpPtr);
    return;
}

//检查testString是不是string的开始前缀
static ADT_BOOL ADT_KMP_IsPrefix(uint8_t* string,uint8_t* testString,uint32_t lengthTestString)
{
	uint32_t indexUtil = 0;
    //检查testString是不是string的开始前缀
	while((indexUtil < lengthTestString)&&((*(string+indexUtil))==(*(testString+indexUtil))))
    {
        indexUtil++;
    }
	if(indexUtil == lengthTestString)
    {
        //是前缀,开头就能匹配上
        return ADT_BOOL_TRUE;
    }
    //不是
	return ADT_BOOL_FALSE;
}

//找出一个字符串前缀信息,生成模式串
static uint32_t ADT_KMP_GetLongestPrefix(uint8_t* string,uint32_t lengthString)
{
	uint32_t retval=0;
	uint32_t indexUtil = 0;
	for(indexUtil = 1;indexUtil < lengthString; indexUtil++)
    {
        if(ADT_BOOL_TRUE == ADT_KMP_IsPrefix(string,string+indexUtil,lengthString-indexUtil))
        {
			retval=lengthString - indexUtil;
			break;
		}
    }
	return retval;
}

//创建模式串
uint32_t* ADT_KMP_CreateJumpTable(ADT_T kmpPtr,uint8_t* patternString,uint32_t lengthPatternString)
{
    //申请模式串空间
	uint32_t* jumptable = (uint32_t*)ADT_MEM_MALLOC(kmpPtr,sizeof(uint32_t)*lengthPatternString);
	uint32_t indexUtil = 0;
	for(indexUtil = 0; indexUtil < lengthPatternString; indexUtil++)
    {
        jumptable[indexUtil] = ADT_KMP_GetLongestPrefix(patternString,indexUtil+1);
    }
	return jumptable;
}

//匹配
static uint32_t ADT_KMP_PatternMatch(uint8_t* string,uint32_t matchStart,uint8_t* patternString,uint32_t lengthPatternString)
{
	int matchsize=0;
    //检查匹配序号
	while((matchsize < lengthPatternString)&&(*(string+matchStart+matchsize)) == (*(patternString+matchsize)))
    {
        matchsize++;
    }
	//返回匹配序号	
	return matchsize;
}

//KMP匹配算法,参数为查找串,查找串长度,目标串,目标串长度,匹配开始序号,存储查找结果的指针
ADT_RESULT ADT_KMP_Match(ADT_T kmpPtr,uint8_t* textStr,uint8_t* patternStr,uint32_t lengthTextStr,
                                    uint32_t lengthPatternStr,uint32_t matchStartPos,uint32_t* matchIndexPtr)
{
    uint32_t indexUtil = matchStartPos;
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //指针检查
    ADT_BASE_PTR_CHECK(kmpPtr,ADT_KMP_Match);
    //ID检查
    ADT_BASE_ID_CHECK(kmpPtr,ADT_KMP_Match,ADT_KMP_T_ID);
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
    //系统锁定
    ADT_SYS_LOCK(kmpPtr);
    //首先生成模式串
    uint32_t* jumpTablePtr = ADT_KMP_CreateJumpTable(kmpPtr,patternStr,lengthPatternStr);
    //开始查找
	uint32_t solution = 0;
	while(indexUtil < (lengthTextStr - lengthPatternStr  + 1))
    {
		solution = ADT_KMP_PatternMatch(textStr,indexUtil,patternStr,lengthPatternStr);
		if(solution != 0)
        {
			if(solution == lengthPatternStr)
            {
                //找到了
                break;
            }
            //没找到,模式串增长
			indexUtil += solution - jumpTablePtr[solution-1];
		}
        else
        {
            indexUtil++;
        }
	}
    //释放内存
    ADT_MEM_FREE(kmpPtr,jumpTablePtr);
    ADT_SYS_UNLOCK(kmpPtr);
    //查看查找结果
    if(indexUtil < (lengthTextStr - lengthPatternStr  + 1))
    {
        //找到了
        *matchIndexPtr = indexUtil;
    }
    else
    {
        //没找到
        *matchIndexPtr = UINT32_MAX;
    }
    return ADT_RESULT_OK;
}