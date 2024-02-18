/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-02-03 15:45:29 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-03-09 16:45:24 +0800
************************************************************************************************/ 
#ifndef __ADT_BASE_H_
#define __ADT_BASE_H_
#include "stdio.h"
#include "stdlib.h"
#include "stdarg.h"
#include "time.h"
#include "stddef.h"
#include "stdint.h"
#include "MCU_RTT.h"

/*------------------------------------枚举定义------------------------------------*/
//抽象数据类型的执行结果
typedef enum ADT_RESULT
{
    ADT_RESULT_OK   = 0,//成功
    ADT_RESULT_FAIL = 1,//失败
}ADT_RESULT;

//比对是否相等
typedef enum ADT_CMP_RESULT
{
    ADT_CMP_EQUAL       = 0X00,
    ADT_CMP_NOT_EQUAL   = 0X01,
}ADT_CMP_RESULT;

//数据元素是否为空
typedef enum ADT_BOOL
{
    ADT_BOOL_FALSE = 0X00,
    ADT_BOOL_TRUE  = 0X01,
}ADT_BOOL;

//当数据结构满了之后是将旧数据弹出还是将新数据拒绝
typedef enum ADT_FULL_OPTION
{
    ADT_FULL_OPTION_BLOCK   = 0X00,//数据满载之后不接收新数据
    ADT_FULL_OPTION_POP     = 0X01,//数据满载之后接受新数据,将旧数据弹出
}ADT_FULL_OPTION;

//平衡二叉树子节点查找方向
typedef enum
{
	AVL_TREE_NODE_LEFT = 0,
	AVL_TREE_NODE_RIGHT = 1
} AVLTreeNodeSide;

//红黑数颜色定义
typedef enum
{
	RB_TREE_NODE_RED    = 0,
	RB_TREE_NODE_BLACK  = 1,
} RBTreeNodeColor;

//红黑树节点方向定义
typedef enum 
{
	RB_TREE_NODE_LEFT = 0,
	RB_TREE_NODE_RIGHT = 1
} RBTreeNodeSide;

typedef enum 
{
	BIN_HEAP_TYPE_MIN = 0,
	BIN_HEAP_TYPE_MAX = 1,
} BinHeapType;

/*---------------------------------实现接口定义,需要对接应用程序---------------------*/
//日志输出
#define ADT_BASE_ERROR_MSG(str)         MCU_RTT_SendStringInt(str)
//内存拷贝
#define ADT_BASE_MEM_COPY(dst,src,len)  UserMemCopy(dst,src,len)
//内存清空
#define ADT_BASE_MEM_CLEAR(dst,len)     UserMemClear(dst,len)
//内存设置
#define ADT_MEM_SET(dst,value,len)      UserMemSet(dst,value,len)
//内存移动
#define ADT_MEM_MOVE(dst,src,len)       UserMemMove(dst,src,len)
//获取当前时间戳
#define ADT_BASE_GET_TIME_STAMP()       ((uint32_t)tx_time_get())

/*---------------------------------------函数原型----------------------------------*/
//内存申请原型
//申请
typedef void* (*ADT_MemMallocFuncPtr)(uint32_t size);
//释放
typedef void (*ADT_MemFreeFuncPtr)(void* ptr);

//系统锁定原型
//锁定
typedef uint32_t (*ADT_LockSystem)(void);
//释放
typedef void (*ADT_UnLockSystem)(uint32_t lastSysSta);

//异常信息打印
//释放
typedef void (*ADT_SendString)(uint8_t* stringPtr);

//比较两个元素是否相等的函数类型
typedef ADT_CMP_RESULT (*ADT_BaseElementCmp)(void* cmpSrc,void* cmpDst);
//判定链表数据是否满足条件
typedef ADT_CMP_RESULT (*ADT_BaseElementMeetCondition)(void* elementPtr,uint32_t utilData);
//对列表元素进行操作的函数,用于列表遍历
typedef void (*ADT_BaseElementOperation)(void** nodeDataPtr);
//对列表元素进行操作的函数,带参数
typedef void (*ADT_AdvanceElementOperation)(void** nodeDataPtr,void* param);
//表结构中进行两个元素比对的函数
typedef int32_t (*ADT_KeyValueTableCompare)(const void* x,const void* y);
//排序算法比较函数
typedef int32_t (*ADT_SortBaseCompare)(void* x,void* y);
//针对特定键值生成哈希代码
typedef uint32_t (*ADT_KeyValueTableCreateHashCode)(const void* key);
//平衡二叉树键比对函数
typedef int (*ADT_AVLTreeCompareFunc)(void* value1, void* value2);
//红黑树键比对函数
typedef int (*ADT_RBTreeCompareFunc)(void* value1, void* value2);
/*--------------------------------------辅助通用功能----------------------------*/
//参数转换为字符串
#define ADT_TO_STRING(param)                                            #param
//字符串拼接
#define ADT_PTR_CHECK_JOINER(perfixStr,funcName,line,ptrName,suffix)    ADT_TO_STRING(perfixStr##_##funcName##_##line##_##ptrName##_##suffix)
//创建指针异常字符串
#define ADT_PTR_ERR_STR(ptrName,funcName,line)                          ADT_PTR_CHECK_JOINER(ADT_PtrError,funcName,line,ptrName,NULL\r\n)
//数据指针检查
#define ADT_BASE_PTR_CHECK(ptrName,funcName)                            if(ptrName == NULL)\
                                                                        {                                     \
                                                                            ADT_BASE_ERROR_MSG(ADT_PTR_ERR_STR(ptrName,funcName,__LINE__));  \
                                                                            while(1);                          \
                                                                        };

//字符串拼接
#define ADT_ID_CHECK_JOINER(perfixStr,funcName,line,ptrName,suffix)     ADT_TO_STRING(perfixStr##_##funcName##_##line##_##ptrName##_##suffix)
//创建ID匹配异常字符串
#define ADT_ID_ERR_STR(funcName,ptrName,line)                           ADT_ID_CHECK_JOINER(ADT_IdError,funcName,line,ptrName,IdNotMatch\r\n)
//数据指针内部ID检查
#define ADT_BASE_ID_CHECK(ptrName,funcName,idDefined)                   if(ptrName->adtTypeID != idDefined)\
                                                                        {                                     \
                                                                            ADT_BASE_ERROR_MSG(ADT_ID_ERR_STR(funcName,ptrName,__LINE__));  \
                                                                            while(1);                          \
                                                                        };                                     

//ADT系统锁定前置
#define ADT_SYS_LOCK_ALLOC(lastStaName)                                 int lastState = 0;
//ADT锁定
#define ADT_SYS_LOCK(ptrEntity)                                         lastState = ptrEntity->sysLockFunc();
//ADT解锁
#define ADT_SYS_UNLOCK(ptrEntity)                                       ptrEntity->sysUnlockFunc(lastState)
//ADT内存申请
#define ADT_MEM_MALLOC(ptrEntity,size)                                  (ptrEntity->memMallocFunc(size))
//ADT内存释放
#define ADT_MEM_FREE(ptrEntity,memPtr)                                  (ptrEntity->memFreeFunc(memPtr))
//ADT日志发送
#define ADT_SHOW_STR(ptrEntity,str)                                     ptrEntity->sendStrFunc(str)
//CALL BACK错误拼接
#define ADT_CALLBACL_CHECK_JOINER(perfixStr,funcName,ptrName,suffix)    ADT_TO_STRING(perfixStr##_##funcName##_##ptrName##_##suffix)
#define ADT_CALLBACK_ERR_STR(funcName,callBackFuncName)                 ADT_CALLBACL_CHECK_JOINER(ADT_CallBackError,callBackFuncName,ptrName,NULL\r\n)
//ADT 回调函数检查
#define ADT_CALL_BACK_CHECK(funcName,memMallocFunc,memFreeFunc,lockSysFuncPtr,unlockSysFuncPtr,sendStrFunc) if(sendStrFunc == NULL)             \
                                                                        {                                                                       \
                                                                            ADT_BASE_ERROR_MSG(ADT_CALLBACK_ERR_STR(funcName,sendStrFunc));     \
                                                                            return NULL;                                                        \
                                                                        }                                                                       \
                                                                        if(memMallocFunc == NULL)                                               \
                                                                        {                                                                       \
                                                                            ADT_BASE_ERROR_MSG(ADT_CALLBACK_ERR_STR(funcName,memMallocFunc));   \
                                                                            return NULL;                                                        \
                                                                        }                                                                       \
                                                                        if(memFreeFunc == NULL)                                                 \
                                                                        {                                                                       \
                                                                            ADT_BASE_ERROR_MSG(ADT_CALLBACK_ERR_STR(funcName,memFreeFunc));     \
                                                                            return NULL;                                                        \
                                                                        }                                                                       \
                                                                        if(lockSysFuncPtr == NULL)                                              \
                                                                        {                                                                       \
                                                                            ADT_BASE_ERROR_MSG(ADT_CALLBACK_ERR_STR(funcName,lockSysFuncPtr));  \
                                                                            return NULL;                                                        \
                                                                        }                                                                       \
                                                                        if(unlockSysFuncPtr == NULL)                                            \
                                                                        {                                                                       \
                                                                            ADT_BASE_ERROR_MSG(ADT_CALLBACK_ERR_STR(funcName,unlockSysFuncPtr));\
                                                                            return NULL;                                                        \
                                                                        }                                                                       \
//函数注册
#define ADT_CALL_BACK_REGISTER(ptrEntity,memMallocFunc,memFreeFunc,lockSysFuncPtr,unlockSysFuncPtr,sendStrFunc) ptrEntity->memMallocFunc = memMallocFunc;\
                                                                        ptrEntity->memFreeFunc = memFreeFunc;           \
                                                                        ptrEntity->sysLockFunc = lockSysFuncPtr;        \
                                                                        ptrEntity->sysUnlockFunc = unlockSysFuncPtr;    \
                                                                        ptrEntity->sendStrFunc = sendStrFunc;           \
//ADT自身内存申请
#define ADT_MALLOC_SELF(ptrEntity,memMallocFunc)                        ptrEntity = memMallocFunc(sizeof(*ptrEntity));
//ADT释放自身内存
#define ADT_FREE_SELF(ptrEntity)                                        ptrEntity->memFreeFunc(ptrEntity)
//设置对象ID
#define ADT_SET_IDENTIFY_CODE_SET(ptrEntity,identifyCode)               ptrEntity->adtTypeID = identifyCode
/*-------------------------------------ADT_BYTE_STACK实现配置---------------------------------------------*/
//字节栈使用数组实现
#define ADT_BYTE_STACK_USE_ARRAY            0
//字节栈使用链表实现    
#define ADT_BYTE_STACK_USE_LIST             1
//定义字节栈实现方式    
#define ADT_BYTE_STACK_USE                  ADT_BYTE_STACK_USE_ARRAY

/*-------------------------------------ADT_PTR_STACK实现配置---------------------------------------------*/
//指针栈使用数组实现
#define ADT_PTR_STACK_USE_ARRAY             0
//指针栈使用链表实现    
#define ADT_PTR_STACK_USE_LIST              1
//用户选择指针栈实现方式    
#define ADT_PTR_STACK_USE                   ADT_PTR_STACK_USE_LIST

/*-------------------------------------ADT_CIRCULAR_LIST实现配置---------------------------------------------*/

#endif


