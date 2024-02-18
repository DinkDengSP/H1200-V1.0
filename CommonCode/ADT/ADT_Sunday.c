/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-03-08 14:17:36 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-03-08 16:17:41 +0800
************************************************************************************************/ 
#include "ADT_Sunday.h"

//子串匹配
static ADT_BOOL ADT_SundaySubMatch(uint8_t* string,uint32_t matchStart,uint8_t* testString,uint32_t lengthTestString)
{
	uint32_t indexUtil = 0;
	while((indexUtil < lengthTestString)&&((*(string+matchStart+indexUtil)) == (*(testString+indexUtil))))
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

//字符是否处于模式串中
int32_t ADT_SundayCheckByteInPattern(uint8_t *patternString,int lengthPatternString,uint8_t byteValue)
{
	int32_t indexUtil = lengthPatternString-1;
	while((indexUtil >= 0)&&(*(patternString+indexUtil) != byteValue))
    {
        indexUtil--;
    }
	return indexUtil;
}


//SUNDAY匹配算法,参数为查找串,查找串长度,目标串,目标串长度,匹配开始序号,存储查找结果的指针
ADT_RESULT ADT_SundayMatch(uint8_t* textStr,uint8_t* patternStr,uint32_t lengthTextStr,
                                    uint32_t lengthPatternStr,uint32_t matchStartPos,uint32_t* matchIndexPtr)
{
    uint32_t indexUtil = matchStartPos;
    //参数检查
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
    uint32_t indexUtilMax = lengthTextStr - lengthPatternStr + 1;
    int step;
    uint8_t next;
    while (indexUtil < indexUtilMax)
    {
        if (ADT_BOOL_TRUE == ADT_SundaySubMatch(textStr,indexUtil, patternStr, lengthPatternStr))
        {
            //找到了
            break;
        }
        if (indexUtil + lengthPatternStr < lengthTextStr)
        {
            next = textStr[indexUtil + lengthPatternStr];
            step = ADT_SundayCheckByteInPattern(patternStr, lengthPatternStr, next);
            if (step >= 0)
            {
                indexUtil += lengthPatternStr - step;
            }
            else
            {
                indexUtil = indexUtil + lengthPatternStr + 1;
            }
        }
        else
        {
            break;
        }
    }
    //查看查找结果
    if(indexUtil < indexUtilMax)
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