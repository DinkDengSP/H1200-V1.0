/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-21 10:19:22 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-06 11:29:04 +0800
************************************************************************************************/ 
#ifndef __CORE_UTIL_H_
#define __CORE_UTIL_H_
#include "stm32h7xx.h"
#include "CoreDataType.h"

//4字节拆解
#define DWORD_TO_BYTE_HH(dword)                         ((uint8_t)((dword >> 24)&0XFF))
#define DWORD_TO_BYTE_HL(dword)                         ((uint8_t)((dword >> 16)&0XFF))
#define DWORD_TO_BYTE_LH(dword)                         ((uint8_t)((dword >> 8)&0XFF))
#define DWORD_TO_BYTE_LL(dword)                         ((uint8_t)((dword)&0XFF))
//双字节拆解
#define WORD_TO_BYTE_H(word)                            ((uint8_t)((word >> 8)&0XFF))
#define WORD_TO_BYTE_L(word)                            ((uint8_t)((word)&0XFF))

//计算输出指定个数字成为字符串的需要的缓冲区长度
#define LENGTH_BYTES_BUF_TO_HEX_STR(byteCount)          (byteCount*5+2)

//计算结构体长度
#define DIM_STRUCT_LENGTH_BYTE(structName)              (sizeof(structName)/sizeof(uint8_t))
//计算一维数组长度
#define DIM_ARRAY_ELEMENT_COUNT(arrayName)              (sizeof(arrayName)/sizeof(arrayName[0]))
//计算一维字符串数组长度
#define DIM_STR_ELEMENT_COUNT(stringPtr)                ((sizeof(stringPtr)/sizeof(stringPtr[0]))-1)
//计算端口无效电频
#define PIN_INVALID_LEVEL(validLevel)                   ((GPIO_PinState)(!validLevel))

//双字节合成16位数据
uint16_t CoreConvertBigEndianToLittleEndianWord(uint8_t* streamPtr);

//四字节合成32位数据
uint16_t CoreConvertBigEndianToLittleEndianDWord(uint8_t* streamPtr);

//设置默认的中断优先级分组
void CoreNVIC_SetGroupDefault(void);

//芯片内核复位
void CoreSystemReset(void);

//获取Icache是否打开
FunctionalState CoreUtilGet_ICahceState(void);

//获取DCache是否打开
FunctionalState CoreUtilGet_DCahceState(void);

//将当前Cache中的数据都写回SRAM
void CoreUtilCacheWriteThrough(void);

//设定系统ICACHE
void CoreICacheSet(FunctionalState setState);

//设置系统DCACHE
void CoreDCacheSet(FunctionalState setState);

//关中断函数,基于BASEPRI以上的中断被关闭
uint32_t CoreLockIntBasePri(void);

//开中断函数,在上次关的时候是什么状态,打开就是什么状态
void CoreUnlockIntBasePri(uint32_t lastIntState);

//关中断函数系统全局中断
uint32_t CoreLockIntGlobal(void);

//开中断函数,系统全局中断
void CoreUnlockIntGlobal(uint32_t lastIntState);

//清除系统全部中断
void CoreClearSystemAllInt(void);

//锁定系统调度
uint32_t CoreLockSchedule(void);

//解锁调度
void CoreUnlockSchedule(uint32_t oldPriority);

//设置主堆栈地址
void CoreSetMainStackPtr(uint32_t addr);

//内核进入低功耗待机模式
void CoreEnterLowPowerMode(void);

//输出指定元素的数字到hex数组
void CoreConvertByteBufToHexStr(uint8_t* bufPtr,uint8_t count,uint8_t* hexStrPtr,uint16_t strBufLen,uint16_t* resultStrLen);

//拷贝内核中断向量表到指定地址
void CoreCopyIntVectorTable(uint32_t dstAddr);

//清除并且无效化DCACHE
void CoreCleanInvalidateDCacheByAddrAlign4Byte(uint32_t addrAny,uint8_t lengthAny);

//清除DCACHE
void CoreCleanDCacheByAddrAlign4Byte(uint32_t addrAny,uint8_t lengthAny);

//设置结果包无错误
void CoreUtilSetSystemResultPackNoError(SYS_RESULT_PACK* sysResultPackPtr);

//设置结果包错误码
void CoreUtilSetSystemResultPack(SYS_RESULT_PACK* sysResultPackPtr,PROT_RESULT result,ERROR_MAIN errorMain,
                                    ERROR_LEVEL errorLevel,ERROR_SUB errorSub);

//restrict是c99标准引入的，它只可以用于限定和约束指针，并表明指针是访问一个数据对象的唯一且初始的方式.
//即它告诉编译器，所有修改该指针所指向内存中内容的操作都必须通过该指针来修改,而不能通过其它途径(其它变量或指针)来修改;
//这样做的好处是,能帮助编译器进行更好的优化代码,生成更有效率的汇编代码.如 int *restrict ptr, ptr 指向的内存单元只能被 ptr 访问到.
//任何同样指向这个内存单元的其他指针都是未定义的，直白点就是无效指针。
//restrict 的出现是因为 C 语言本身固有的缺陷，C 程序员应当主动地规避这个缺陷，而编译器也会很配合地优化你的代码.
__STATIC_INLINE void UserMemCopy(volatile uint8_t* dst, const uint8_t* __RESTRICT src, uint32_t len)
{
    uint32_t indexUtil;
    for (indexUtil = 0U; indexUtil < len; ++indexUtil)
    {
        dst[indexUtil] = src[indexUtil];
    }
}

//数据设置
__STATIC_INLINE void UserMemSet(volatile uint8_t* dst, uint8_t setValue, uint32_t len)
{
    uint32_t indexUtil;
    for (indexUtil = 0U; indexUtil < len; ++indexUtil)
    {
        dst[indexUtil] = setValue;
    }
}

//数据清空
__STATIC_INLINE void UserMemClear(volatile uint8_t* dst, uint32_t len)
{
    uint32_t indexUtil;
    for (indexUtil = 0U; indexUtil < len; ++indexUtil)
    {
        dst[indexUtil] = 0;
    }
}

__STATIC_INLINE void UserMemMove(void* dst,const void* src,uint32_t len)
{
    uint8_t* tmpdst = (uint8_t*)dst;
    uint8_t* tmpsrc = (uint8_t*)src;

    if (tmpdst <= tmpsrc || tmpdst >= tmpsrc + len)
    {
        while(len--)
        {
            *tmpdst++ = *tmpsrc++; 
        }
    }
    else
    {
        tmpdst = tmpdst + len - 1;
        tmpsrc = tmpsrc + len - 1;
        while(len--)
        {
            *tmpdst-- = *tmpsrc--;
        }
    }
}

//数据清空
__STATIC_INLINE int32_t UserMemCmp(volatile uint8_t* dst, const uint8_t* __RESTRICT src, uint32_t len)
{
    uint32_t indexUtil = 0;
    for(indexUtil = 0; indexUtil < len; indexUtil++)
    {
        if(dst[indexUtil] - src[indexUtil] > 0)
        {
            return 1;
        }
        else if(dst[indexUtil] - src[indexUtil] < 0)
        {
            return -1;
        }
    }
    return 0;
}


#endif













