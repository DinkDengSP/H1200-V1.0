/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-03 16:12:17 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-06 08:54:17 +0800
************************************************************************************************/ 
#include "UserMemManage.h"

static const UserMemInitFunc UserMemInitFuncArray[] = {
#if(MEM_MANAGE_BYTE_SIZE_ITCM_SRAM != 0)
    MCU_MemInitItcmSram,
#endif
#if(MEM_MANAGE_BYTE_SIZE_DTCM_SRAM != 0)
    MCU_MemInitDtcmSram,
#endif
#if(MEM_MANAGE_BYTE_SIZE_AXI_SRAM != 0)
    MCU_MemInitAxiSram,
#endif
#if(MEM_MANAGE_BYTE_SIZE_SRAM != 0)
    BoardMemInitSRAM,
#endif
};

static const UserMemMallocFunc UserMemMallocFuncArray[] = {
#if(MEM_MANAGE_BYTE_SIZE_ITCM_SRAM != 0)
    MCU_MemMallocItcmSram,
#endif
#if(MEM_MANAGE_BYTE_SIZE_DTCM_SRAM != 0)
    MCU_MemMallocDtcmSram,
#endif
#if(MEM_MANAGE_BYTE_SIZE_AXI_SRAM != 0)
    MCU_MemMallocAxiSram,
#endif
#if(MEM_MANAGE_BYTE_SIZE_SRAM != 0)
    BoardMemMallocSRAM,
#endif
};

static const UserMemMallocWhileSuccessFunc UserMemMallocWhileSuccessFuncArray[] = {
#if(MEM_MANAGE_BYTE_SIZE_ITCM_SRAM != 0)
    MCU_MemMallocWhileSuccessItcmSram,
#endif
#if(MEM_MANAGE_BYTE_SIZE_DTCM_SRAM != 0)
    MCU_MemMallocWhileSuccessDtcmSram,
#endif
#if(MEM_MANAGE_BYTE_SIZE_AXI_SRAM != 0)
    MCU_MemMallocWhileSuccessAxiSram,
#endif
#if(MEM_MANAGE_BYTE_SIZE_SRAM != 0)
    BoardMemMallocWhileSuccessSRAM,
#endif
};

static const UserMemFreeFunc UserMemFreeFuncArray[] = {
#if(MEM_MANAGE_BYTE_SIZE_ITCM_SRAM != 0)
    MCU_MemFreeItcmSram,
#endif
#if(MEM_MANAGE_BYTE_SIZE_DTCM_SRAM != 0)
    MCU_MemFreeDtcmSram,
#endif
#if(MEM_MANAGE_BYTE_SIZE_AXI_SRAM != 0)
    MCU_MemFreeAxiSram,
#endif
#if(MEM_MANAGE_BYTE_SIZE_SRAM != 0)
    BoardMemFreeSRAM,
#endif
};

static const UserMemPerUsedFunc UserMemPerUsedFuncArray[] = {
#if(MEM_MANAGE_BYTE_SIZE_ITCM_SRAM != 0)
    MCU_MemPerUsedItcmSram,
#endif
#if(MEM_MANAGE_BYTE_SIZE_DTCM_SRAM != 0)
    MCU_MemPerUsedDtcmSram,
#endif
#if(MEM_MANAGE_BYTE_SIZE_AXI_SRAM != 0)
    MCU_MemPerUsedAxiSram,
#endif
#if(MEM_MANAGE_BYTE_SIZE_SRAM != 0)
    BoardMemPerUsedSRAM,
#endif
};


//动态内存初始化
void UserMemInit(MEM_REGION locate)
{
    UserMemInitFuncArray[locate]();
}

//申请内存
void* UserMemMalloc(MEM_REGION locate,uint32_t size)
{
    void* targetPtr;
    targetPtr = UserMemMallocFuncArray[locate](size);
    return targetPtr;
}


//申请内存
void* UserMemMallocWhileSuccess(MEM_REGION locate,uint32_t size)
{
    void* targetPtr;
    targetPtr = UserMemMallocWhileSuccessFuncArray[locate](size);
    return targetPtr;
}

//释放内存
void UserMemFree(MEM_REGION locate,void* ptr)
{
    if(ptr == NULL)
    {
        return;
    }
    UserMemFreeFuncArray[locate](ptr);
}


//获取当前动态内存使用率
uint8_t UserMemPerUsed(MEM_REGION locate)
{
    uint8_t used;
    used = UserMemPerUsedFuncArray[locate]();
    return used;
}















