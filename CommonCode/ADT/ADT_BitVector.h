/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-03-06 14:44:57 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-03-07 13:57:47 +0800
************************************************************************************************/ 
#ifndef __ADT_BIT_VECTOR_H_
#define __ADT_BIT_VECTOR_H_
#include "ADT_Base.h"

//类型名称宏定义
#define ADT_T ADT_BIT_VECTOR_T

//指定一个数据类型,****对外部文件ADT_CIRCULAR_PTR_QUEUE_T数据类型是一个指向
//****ADT_CIRCULAR_PTR_QUEUE_T结构体的指针(注意这句话)
typedef struct ADT_T *ADT_T;

//创建向量,指定向量初始长度
ADT_T ADT_BitVectorCreate(uint32_t length,ADT_MemMallocFuncPtr memMallocFunc,ADT_MemFreeFuncPtr memFreeFunc,
                                ADT_LockSystem lockSysFuncPtr,ADT_UnLockSystem unlockSysFuncPtr,ADT_SendString sendStrFunc);

//获取向量长度
int ADT_BitVectorGetLength(ADT_T bitVectorPtr);

//获取向量中为1的比特的长度
int ADT_BitVectorGetBitSetCount(ADT_T bitVectorPtr);

//释放向量
void ADT_BitVectorDestroy(ADT_T* bitVectorPtrPtr);

//获取指定bit的值
uint8_t ADT_BitVectorGetBit(ADT_T bitVectorPtr, uint32_t bitIndex,ADT_RESULT* resultPtr);

//设置指定比特位的值
uint8_t ADT_BitVectorPut(ADT_T bitVectorPtr, uint32_t bitIndex, uint8_t bitValue,ADT_RESULT* resultPtr);

//清除指定比特区域的值
ADT_RESULT ADT_BitVectorClearRange(ADT_T bitVectorPtr,uint32_t rangeLow, uint32_t rangeHigh);

//设置指定比特区域的值
ADT_RESULT ADT_BitVectorSetRange(ADT_T bitVectorPtr,uint32_t rangeLow, uint32_t rangeHigh);

//取反指定比特区域的值
ADT_RESULT ADT_BitVectorNotRange(ADT_T bitVectorPtr,uint32_t rangeLow, uint32_t rangeHigh);

//检查srcBitVectorPtr是否是testBitVectorPtr的真子集,testBitVectorPtr为0的位,srcBitVectorPtr也必然为0
//且testBitVectorPtr为1的位,在srcBitVectorPtr中必然有一个不为1
ADT_BOOL ADT_BitVectorLessThen(ADT_T srcBitVectorPtr, ADT_T testBitVectorPtr,ADT_RESULT* resultPtr);

//比较两个位向量是否相等
ADT_BOOL ADT_BitVectorCompareEqual(ADT_T srcBitVectorPtr, ADT_T testBitVectorPtr,ADT_RESULT* resultPtr);

//检查srcBitVectorPtr是否是testBitVectorPtr的子集,testBitVectorPtr为0的位,srcBitVectorPtr也必然为0
//不关注testBitVectorPtr为1的位,在srcBitVectorPtr中是否为1
ADT_BOOL ADT_BitVectorLessOrEqual(ADT_T srcBitVectorPtr, ADT_T testBitVectorPtr,ADT_RESULT* resultPtr);

//遍历位向量
void ADT_BitVectorTraverse(ADT_T srcBitVectorPtr, void opFunc(int bitIndex, int bitValue, void *param), 
                                void *param);

//获取两个集合的并集,为1的地方合在一起
ADT_T ADT_BitVectorOperationOr(ADT_T srcBitVectorPtr, ADT_T testBitVectorPtr,ADT_RESULT* resultPtr);

//获取两个集合的交集
ADT_T ADT_BitVectorOperationAnd(ADT_T srcBitVectorPtr, ADT_T testBitVectorPtr,ADT_RESULT* resultPtr);

//testBitVectorPtr的补集和srcBitVectorPtr按位与
ADT_T ADT_BitVectorOperationMinus(ADT_T srcBitVectorPtr, ADT_T testBitVectorPtr,ADT_RESULT* resultPtr); 

//获取两个集合的异或运算结果ADT_T srcBitVectorPtr, ADT_T testBitVectorPtr
ADT_T ADT_BitVectorOperationXor(ADT_T srcBitVectorPtr, ADT_T testBitVectorPtr,ADT_RESULT* resultPtr); 

//释放宏定义,外部可以使用
#undef ADT_T

#endif



