/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-23 09:44:56 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-30 13:53:20 +0800
************************************************************************************************/ 
#include "UserMemManage.h"

//函数指针类型定义
typedef void (*UserMemInitFuncPtr)();
typedef void* (*UserMemMallocFuncPtr)(uint32_t size);
typedef void* (*UserMemMallocWhileSuccessFuncPtr)(uint32_t size);
typedef void (*UserMemFreeFuncPtr)(void* ptr);
typedef uint8_t (*UserMemPerUsedFuncPtr)(void);




static const UserMemInitFuncPtr UserMemInitFuncPtrArray[] = {
    MCU_MemInitSramIn,
#if(MEM_MANAGE_BYTE_SIZE_SRAM_EXT != 0)
    BoardMemInitSramExt,
#endif
};

static const UserMemMallocFuncPtr UserMemMallocFuncPtrArray[] = {
    MCU_MemMallocSramIn,
#if(MEM_MANAGE_BYTE_SIZE_SRAM_EXT != 0)
    BoardMemMallocSramExt,
#endif
};

static const UserMemMallocWhileSuccessFuncPtr UserMemMallocWhileSuccessFuncPtrArray[] = {
    MCU_MemMallocWhileSuccessSramIn,
#if(MEM_MANAGE_BYTE_SIZE_SRAM_EXT != 0)
    BoardMemMallocWhileSuccessSramExt,
#endif
};

static const UserMemFreeFuncPtr UserMemFreeFuncPtrArray[] = {
    MCU_MemFreeSramIn,
#if(MEM_MANAGE_BYTE_SIZE_SRAM_EXT != 0)
    BoardMemFreeSramExt,
#endif
};

static const UserMemPerUsedFuncPtr UserMemPerUsedFuncPtrArray[] = {
    MCU_MemPerUsedSramIn,
#if(MEM_MANAGE_BYTE_SIZE_SRAM_EXT != 0)
    BoardMemPerUsedSramExt,
#endif
};


//动态内存初始化
void UserMemInit(MEM_REGION locate)
{
    UserMemInitFuncPtrArray[locate]();
}

//申请内存
void* UserMemMalloc(MEM_REGION locate,uint32_t size)
{
    void* targetPtr;
    targetPtr = UserMemMallocFuncPtrArray[locate](size);
    return targetPtr;
}


//申请内存
void* UserMemMallocWhileSuccess(MEM_REGION locate,uint32_t size)
{
    void* targetPtr;
    targetPtr = UserMemMallocWhileSuccessFuncPtrArray[locate](size);
    return targetPtr;
}

//释放内存
void UserMemFree(MEM_REGION locate,void* ptr)
{
    if(ptr == NULL)
    {
        return;
    }
    UserMemFreeFuncPtrArray[locate](ptr);
}


//获取当前动态内存使用率
uint8_t UserMemPerUsed(MEM_REGION locate)
{
    uint8_t used;
    used = UserMemPerUsedFuncPtrArray[locate]();
    return used;
}









