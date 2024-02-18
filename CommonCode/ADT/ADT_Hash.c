/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-03-09 09:59:13 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-03-09 10:17:39 +0800
************************************************************************************************/ 
#include "ADT_Hash.h"

//数值的哈希计算
uint32_t ADT_HashCalcValue(void* valuePtr)
{
    uint32_t *location;
    //获取指向
	location = (uint32_t *)valuePtr;
    //得到指向的数据
	return (uint32_t)*location;
}

//指针地址的哈希计算
uint32_t ADT_HashCalcPtr(void* ptr)
{
    return (uint32_t)ptr;
}

//字符串的哈希代码计算
uint32_t ADT_HashCalcString(void* stringPtr)
{
    uint32_t result = 5381;
	uint8_t* p;
	p = (uint8_t *)stringPtr;
    //计算
	while (*p != '\0') 
    {
		result = (result << 5) + result + (*p);
		++p;
	}
	return result;
}

//字符转小写
static uint8_t CharToLower(uint8_t c)
{
	if ((c >= 'A') && (c <= 'Z'))
    {
        return c + ('a' - 'A');
    }
	return c;
}
 
//字符转大写
static uint8_t CharToUpper(uint8_t c)
{
	if ((c >= 'a') && (c <= 'z'))
    {
        return c + ('A' - 'a');
    }
	return c;
}

//字符串的哈希代码计算,不区分大小写,按小写计算
uint32_t ADT_HashCalcStringNoCaseLower(void* stringPtr)
{
    uint32_t result = 5381;
	uint8_t* p;
	p = (uint8_t *)stringPtr;
    //计算
	while (*p != '\0') 
    {
		result = (result << 5) + result + CharToLower(*p);
		++p;
	}
	return result;
}

//字符串的哈希代码计算,不区分大小写
uint32_t ADT_HashCalcStringNoCaseUpper(void* stringPtr)
{
    uint32_t result = 5381;
	uint8_t* p;
	p = (uint8_t *)stringPtr;
    //计算
	while (*p != '\0') 
    {
		result = (result << 5) + result + CharToUpper(*p);
		++p;
	}
	return result;
}

//数组的哈希代码计算
uint32_t ADT_HashCalcBuffer(uint8_t* bufStartPtr,uint32_t length)
{
    uint32_t result = 5381;
	uint8_t* p;
	p = (uint8_t *)bufStartPtr;
    uint32_t indexUtil = 0;
    for(indexUtil = 0; indexUtil < length; indexUtil++)
    {
        result = (result << 5) + result + (*p);
		++p;
    }
    return result;
}