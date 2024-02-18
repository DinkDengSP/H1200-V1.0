/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-03-09 09:59:13 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-03-09 09:59:32 +0800
************************************************************************************************/ 
#ifndef __ADT_HASH_H_
#define __ADT_HASH_H_
#include "ADT_Base.h"

//数值的哈希计算
uint32_t ADT_HashCalcValue(void* valuePtr);

//指针地址的哈希计算
uint32_t ADT_HashCalcPtr(void* ptr);

//字符串的哈希代码计算
uint32_t ADT_HashCalcString(void* stringPtr);

//字符串的哈希代码计算,不区分大小写
uint32_t ADT_HashCalcStringNoCaseLower(void* stringPtr);

//字符串的哈希代码计算,不区分大小写
uint32_t ADT_HashCalcStringNoCaseUpper(void* stringPtr);

//数组的哈希代码计算
uint32_t ADT_HashCalcBuffer(uint8_t* bufStartPtr,uint32_t length);


#endif




