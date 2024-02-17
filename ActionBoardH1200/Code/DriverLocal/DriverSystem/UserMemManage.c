/************************************************************************ 
 ** Author       : 邓小俊
 ** Date         : 2024-01-16 11:35:28
 ** Description  : When I Has Free Time ,I Will Write Description Below.
 ** description  : 
 ** LastEditors  : 邓小俊
 ** LastEditTime : 2024-01-16 13:15:06
 ** Email: xiaojun.deng@aliyun.com
 ** Copyright (c) 2024 by 公司名称, All Rights Reserved. 
 ************************************************************************/
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
#if(MEM_MANAGE_BYTE_SIZE_D2_SRAM1 != 0)
    MCU_MemInitD2Sram1,
#endif
#if(MEM_MANAGE_BYTE_SIZE_D3_SRAM4 != 0)
    MCU_MemInitD3Sram4,
#endif
#if(MEM_MANAGE_BYTE_SIZE_SDRAM != 0)
    BoardMemInitSDRAM,
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
#if(MEM_MANAGE_BYTE_SIZE_D2_SRAM1 != 0)
    MCU_MemMallocD2Sram1,
#endif
#if(MEM_MANAGE_BYTE_SIZE_D3_SRAM4 != 0)
    MCU_MemMallocD3Sram4,
#endif
#if(MEM_MANAGE_BYTE_SIZE_SDRAM != 0)
    BoardMemMallocSDRAM,
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
#if(MEM_MANAGE_BYTE_SIZE_D2_SRAM1 != 0)
    MCU_MemMallocWhileSuccessD2Sram1,
#endif
#if(MEM_MANAGE_BYTE_SIZE_D3_SRAM4 != 0)
    MCU_MemMallocWhileSuccessD3Sram4,
#endif
#if(MEM_MANAGE_BYTE_SIZE_SDRAM != 0)
    BoardMemMallocWhileSuccessSDRAM,
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
#if(MEM_MANAGE_BYTE_SIZE_D2_SRAM1 != 0)
    MCU_MemFreeD2Sram1,
#endif
#if(MEM_MANAGE_BYTE_SIZE_D3_SRAM4 != 0)
    MCU_MemFreeD3Sram4,
#endif
#if(MEM_MANAGE_BYTE_SIZE_SDRAM != 0)
    BoardMemFreeSDRAM,
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
#if(MEM_MANAGE_BYTE_SIZE_D2_SRAM1 != 0)
    MCU_MemPerUsedD2Sram1,
#endif
#if(MEM_MANAGE_BYTE_SIZE_D3_SRAM4 != 0)
    MCU_MemPerUsedD3Sram4,
#endif
#if(MEM_MANAGE_BYTE_SIZE_SDRAM != 0)
    BoardMemPerUsedSDRAM,
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

