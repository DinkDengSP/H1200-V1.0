/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-21 10:19:22 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-08-21 13:56:28 +0800
************************************************************************************************/ 
#include "MCU_Mem.h"
#include "tx_byte_pool.h"


/*--------------------------------------------ItcmSram内存-------------------------------------------------------*/
#if(MEM_MANAGE_BYTE_SIZE_ITCM_SRAM != 0)

#pragma pack(push) //保存对齐状态 
#pragma pack(4)//四字节对齐
//内存池 
static uint8_t memItcmSramByteArray[MEM_MANAGE_BYTE_SIZE_ITCM_SRAM]@".itcmFuncSection";	
#pragma pack (pop)			

//管理内存池
static TX_BYTE_POOL bytePoolMemItcmSram;

//ItcmSram内存初始化
void MCU_MemInitItcmSram(void)
{
    if(bytePoolMemItcmSram.tx_byte_pool_id == TX_BYTE_POOL_ID)
    {
        return;
    }
    //清空内存
    UserMemSet(memItcmSramByteArray,0,MEM_MANAGE_BYTE_SIZE_ITCM_SRAM);
	//创建内存池
    tx_byte_pool_create(&bytePoolMemItcmSram,"bytePoolMemItcmSram",memItcmSramByteArray,MEM_MANAGE_BYTE_SIZE_ITCM_SRAM);
}

//申请内存
void* MCU_MemMallocItcmSram(uint32_t size)
{
    void* memMallocResult = NULL;
    if(bytePoolMemItcmSram.tx_byte_pool_id != TX_BYTE_POOL_ID)
    {
        MCU_MemInitItcmSram();
    }
    //申请内存
    int32_t temp = tx_byte_allocate(&bytePoolMemItcmSram,(void**)(&memMallocResult),size,TX_NO_WAIT);
    //回传结果
    return memMallocResult;
}

//申请内存直到成功
void* MCU_MemMallocWhileSuccessItcmSram(uint32_t size)
{
    void* memMallocResult = NULL;
    if(bytePoolMemItcmSram.tx_byte_pool_id != TX_BYTE_POOL_ID)
    {
        MCU_MemInitItcmSram();
    }
    //申请内存
    int32_t temp = tx_byte_allocate(&bytePoolMemItcmSram,(void**)(&memMallocResult),size,TX_WAIT_FOREVER);
    //回传结果
    return memMallocResult;
}

//释放内存
void MCU_MemFreeItcmSram(void* ptr)
{
    if(bytePoolMemItcmSram.tx_byte_pool_id != TX_BYTE_POOL_ID)
    {
        MCU_MemInitItcmSram();
    }
    if(ptr == NULL)
    {
        return;
    }
    else
    {
        //释放内存
        tx_byte_release(ptr);
    }
}

//获取当前动态内存使用率
uint8_t MCU_MemPerUsedItcmSram(void)
{   
    ULONG availableBytesCount = 0;
    float userFloat = 0.0;
    if(bytePoolMemItcmSram.tx_byte_pool_id != TX_BYTE_POOL_ID)
    {
        MCU_MemInitItcmSram();
    }
    tx_byte_pool_info_get(&bytePoolMemItcmSram,TX_NULL,&availableBytesCount,TX_NULL,TX_NULL,TX_NULL,TX_NULL);
    userFloat = ((MEM_MANAGE_BYTE_SIZE_ITCM_SRAM-availableBytesCount)*100)/(MEM_MANAGE_BYTE_SIZE_ITCM_SRAM); 
    return (uint8_t)userFloat;
}
#endif


/*--------------------------------------------DtcmSram内存-------------------------------------------------------*/
#if(MEM_MANAGE_BYTE_SIZE_DTCM_SRAM != 0)

#pragma pack(push) //保存对齐状态 
#pragma pack(4)//四字节对齐
//内存池 
static uint8_t memDtcmSramByteArray[MEM_MANAGE_BYTE_SIZE_DTCM_SRAM]@".dtcm_ram";	
#pragma pack (pop)			

//管理内存池
static TX_BYTE_POOL bytePoolMemDtcmSram;

//DtcmSram内存初始化
void MCU_MemInitDtcmSram(void)
{
    if(bytePoolMemDtcmSram.tx_byte_pool_id == TX_BYTE_POOL_ID)
    {
        return;
    }
    //清空内存
    UserMemSet(memDtcmSramByteArray,0,MEM_MANAGE_BYTE_SIZE_DTCM_SRAM);
	//创建内存池
    tx_byte_pool_create(&bytePoolMemDtcmSram,"bytePoolMemDtcmSram",memDtcmSramByteArray,MEM_MANAGE_BYTE_SIZE_DTCM_SRAM);
}

//申请内存
void* MCU_MemMallocDtcmSram(uint32_t size)
{
    void* memMallocResult = NULL;
    if(bytePoolMemDtcmSram.tx_byte_pool_id != TX_BYTE_POOL_ID)
    {
        MCU_MemInitDtcmSram();
    }
    //申请内存
    int32_t temp = tx_byte_allocate(&bytePoolMemDtcmSram,(void**)(&memMallocResult),size,TX_NO_WAIT);
    //回传结果
    return memMallocResult;
}

//申请内存直到成功
void* MCU_MemMallocWhileSuccessDtcmSram(uint32_t size)
{
    void* memMallocResult = NULL;
    if(bytePoolMemDtcmSram.tx_byte_pool_id != TX_BYTE_POOL_ID)
    {
        MCU_MemInitDtcmSram();
    }
    //申请内存
    int32_t temp = tx_byte_allocate(&bytePoolMemDtcmSram,(void**)(&memMallocResult),size,TX_WAIT_FOREVER);
    //回传结果
    return memMallocResult;
}

//释放内存
void MCU_MemFreeDtcmSram(void* ptr)
{
    if(bytePoolMemDtcmSram.tx_byte_pool_id != TX_BYTE_POOL_ID)
    {
        MCU_MemInitDtcmSram();
    }
    if(ptr == NULL)
    {
        return;
    }
    else
    {
        //释放内存
        tx_byte_release(ptr);
    }
}

//获取当前动态内存使用率
uint8_t MCU_MemPerUsedDtcmSram(void)
{   
    ULONG availableBytesCount = 0;
    float userFloat = 0.0;
    if(bytePoolMemDtcmSram.tx_byte_pool_id != TX_BYTE_POOL_ID)
    {
        MCU_MemInitDtcmSram();
    }
    tx_byte_pool_info_get(&bytePoolMemDtcmSram,TX_NULL,&availableBytesCount,TX_NULL,TX_NULL,TX_NULL,TX_NULL);
    userFloat = ((MEM_MANAGE_BYTE_SIZE_DTCM_SRAM-availableBytesCount)*100)/(MEM_MANAGE_BYTE_SIZE_DTCM_SRAM); 
    return (uint8_t)userFloat;
}
#endif


/*--------------------------------------------AxiSram内存-------------------------------------------------------*/
#if(MEM_MANAGE_BYTE_SIZE_AXI_SRAM != 0)

#pragma pack(push) //保存对齐状态 
#pragma pack(4)//四字节对齐
//内存池 
static uint8_t memAxiSramByteArray[MEM_MANAGE_BYTE_SIZE_AXI_SRAM]@".axi_sram";	
#pragma pack (pop)			

//管理内存池
static TX_BYTE_POOL bytePoolMemAxiSram;

//AxiSram内存初始化
void MCU_MemInitAxiSram(void)
{
    if(bytePoolMemAxiSram.tx_byte_pool_id == TX_BYTE_POOL_ID)
    {
        return;
    }
    //清空内存
    UserMemSet(memAxiSramByteArray,0,MEM_MANAGE_BYTE_SIZE_AXI_SRAM);
	//创建内存池
    tx_byte_pool_create(&bytePoolMemAxiSram,"bytePoolMemAxiSram",memAxiSramByteArray,MEM_MANAGE_BYTE_SIZE_AXI_SRAM);
}

//申请内存
void* MCU_MemMallocAxiSram(uint32_t size)
{
    void* memMallocResult = NULL;
    if(bytePoolMemAxiSram.tx_byte_pool_id != TX_BYTE_POOL_ID)
    {
        MCU_MemInitAxiSram();
    }
    //申请内存
    int32_t temp = tx_byte_allocate(&bytePoolMemAxiSram,(void**)(&memMallocResult),size,TX_NO_WAIT);
    //回传结果
    return memMallocResult;
}

//申请内存直到成功
void* MCU_MemMallocWhileSuccessAxiSram(uint32_t size)
{
    void* memMallocResult = NULL;
    if(bytePoolMemAxiSram.tx_byte_pool_id != TX_BYTE_POOL_ID)
    {
        MCU_MemInitAxiSram();
    }
    //申请内存
    int32_t temp = tx_byte_allocate(&bytePoolMemAxiSram,(void**)(&memMallocResult),size,TX_WAIT_FOREVER);
    //回传结果
    return memMallocResult;
}

//释放内存
void MCU_MemFreeAxiSram(void* ptr)
{
    if(bytePoolMemAxiSram.tx_byte_pool_id != TX_BYTE_POOL_ID)
    {
        MCU_MemInitAxiSram();
    }
    if(ptr == NULL)
    {
        return;
    }
    else
    {
        //释放内存
        tx_byte_release(ptr);
    }
}

//获取当前动态内存使用率
uint8_t MCU_MemPerUsedAxiSram(void)
{   
    ULONG availableBytesCount = 0;
    float userFloat = 0.0;
    if(bytePoolMemAxiSram.tx_byte_pool_id != TX_BYTE_POOL_ID)
    {
        MCU_MemInitAxiSram();
    }
    tx_byte_pool_info_get(&bytePoolMemAxiSram,TX_NULL,&availableBytesCount,TX_NULL,TX_NULL,TX_NULL,TX_NULL);
    userFloat = ((MEM_MANAGE_BYTE_SIZE_AXI_SRAM-availableBytesCount)*100)/(MEM_MANAGE_BYTE_SIZE_AXI_SRAM); 
    return (uint8_t)userFloat;
}
#endif


/*--------------------------------------------D2Sram1内存-------------------------------------------------------*/
#if(MEM_MANAGE_BYTE_SIZE_D2_SRAM1 != 0)

#pragma pack(push) //保存对齐状态 
#pragma pack(4)//四字节对齐
//内存池 
static uint8_t memD2Sram1ByteArray[MEM_MANAGE_BYTE_SIZE_D2_SRAM1]@".d2_sram1";	
#pragma pack (pop)			

//管理内存池
static TX_BYTE_POOL bytePoolMemD2Sram1;

//D2Sram1内存初始化
void MCU_MemInitD2Sram1(void)
{
    if(bytePoolMemD2Sram1.tx_byte_pool_id == TX_BYTE_POOL_ID)
    {
        return;
    }
    //清空内存
    UserMemSet(memD2Sram1ByteArray,0,MEM_MANAGE_BYTE_SIZE_D2_SRAM1);
	//创建内存池
    tx_byte_pool_create(&bytePoolMemD2Sram1,"bytePoolMemD2Sram1",memD2Sram1ByteArray,MEM_MANAGE_BYTE_SIZE_D2_SRAM1);
}

//申请内存
void* MCU_MemMallocD2Sram1(uint32_t size)
{
    void* memMallocResult = NULL;
    if(bytePoolMemD2Sram1.tx_byte_pool_id != TX_BYTE_POOL_ID)
    {
        MCU_MemInitD2Sram1();
    }
    //申请内存
    int32_t temp = tx_byte_allocate(&bytePoolMemD2Sram1,(void**)(&memMallocResult),size,TX_NO_WAIT);
    //回传结果
    return memMallocResult;
}

//申请内存直到成功
void* MCU_MemMallocWhileSuccessD2Sram1(uint32_t size)
{
    void* memMallocResult = NULL;
    if(bytePoolMemD2Sram1.tx_byte_pool_id != TX_BYTE_POOL_ID)
    {
        MCU_MemInitD2Sram1();
    }
    //申请内存
    int32_t temp = tx_byte_allocate(&bytePoolMemD2Sram1,(void**)(&memMallocResult),size,TX_WAIT_FOREVER);
    //回传结果
    return memMallocResult;
}

//释放内存
void MCU_MemFreeD2Sram1(void* ptr)
{
    if(bytePoolMemD2Sram1.tx_byte_pool_id != TX_BYTE_POOL_ID)
    {
        MCU_MemInitD2Sram1();
    }
    if(ptr == NULL)
    {
        return;
    }
    else
    {
        //释放内存
        tx_byte_release(ptr);
    }
}

//获取当前动态内存使用率
uint8_t MCU_MemPerUsedD2Sram1(void)
{   
    ULONG availableBytesCount = 0;
    float userFloat = 0.0;
    if(bytePoolMemD2Sram1.tx_byte_pool_id != TX_BYTE_POOL_ID)
    {
        MCU_MemInitD2Sram1();
    }
    tx_byte_pool_info_get(&bytePoolMemD2Sram1,TX_NULL,&availableBytesCount,TX_NULL,TX_NULL,TX_NULL,TX_NULL);
    userFloat = ((MEM_MANAGE_BYTE_SIZE_D2_SRAM1-availableBytesCount)*100)/(MEM_MANAGE_BYTE_SIZE_D2_SRAM1); 
    return (uint8_t)userFloat;
}
#endif


/*--------------------------------------------D3Sram4内存-------------------------------------------------------*/
#if(MEM_MANAGE_BYTE_SIZE_D3_SRAM4 != 0)

#pragma pack(push) //保存对齐状态 
#pragma pack(4)//四字节对齐
//内存池 
static uint8_t memD3Sram4ByteArray[MEM_MANAGE_BYTE_SIZE_D3_SRAM4]@".d3_sram4";	
#pragma pack (pop)			

//管理内存池
static TX_BYTE_POOL bytePoolMemD3Sram4;

//D3Sram4内存初始化
void MCU_MemInitD3Sram4(void)
{
    if(bytePoolMemD3Sram4.tx_byte_pool_id == TX_BYTE_POOL_ID)
    {
        return;
    }
    //清空内存
    UserMemSet(memD3Sram4ByteArray,0,MEM_MANAGE_BYTE_SIZE_D3_SRAM4);
	//创建内存池
    tx_byte_pool_create(&bytePoolMemD3Sram4,"bytePoolMemD3Sram4",memD3Sram4ByteArray,MEM_MANAGE_BYTE_SIZE_D3_SRAM4);
}

//申请内存
void* MCU_MemMallocD3Sram4(uint32_t size)
{
    void* memMallocResult = NULL;
    if(bytePoolMemD3Sram4.tx_byte_pool_id != TX_BYTE_POOL_ID)
    {
        MCU_MemInitD3Sram4();
    }
    //申请内存
    int32_t temp = tx_byte_allocate(&bytePoolMemD3Sram4,(void**)(&memMallocResult),size,TX_NO_WAIT);
    //回传结果
    return memMallocResult;
}

//申请内存直到成功
void* MCU_MemMallocWhileSuccessD3Sram4(uint32_t size)
{
    void* memMallocResult = NULL;
    if(bytePoolMemD3Sram4.tx_byte_pool_id != TX_BYTE_POOL_ID)
    {
        MCU_MemInitD3Sram4();
    }
    //申请内存
    int32_t temp = tx_byte_allocate(&bytePoolMemD3Sram4,(void**)(&memMallocResult),size,TX_WAIT_FOREVER);
    //回传结果
    return memMallocResult;
}

//释放内存
void MCU_MemFreeD3Sram4(void* ptr)
{
    if(bytePoolMemD3Sram4.tx_byte_pool_id != TX_BYTE_POOL_ID)
    {
        MCU_MemInitD3Sram4();
    }
    if(ptr == NULL)
    {
        return;
    }
    else
    {
        //释放内存
        tx_byte_release(ptr);
    }
}

//获取当前动态内存使用率
uint8_t MCU_MemPerUsedD3Sram4(void)
{   
    ULONG availableBytesCount = 0;
    float userFloat = 0.0;
    if(bytePoolMemD3Sram4.tx_byte_pool_id != TX_BYTE_POOL_ID)
    {
        MCU_MemInitD3Sram4();
    }
    tx_byte_pool_info_get(&bytePoolMemD3Sram4,TX_NULL,&availableBytesCount,TX_NULL,TX_NULL,TX_NULL,TX_NULL);
    userFloat = ((MEM_MANAGE_BYTE_SIZE_D3_SRAM4-availableBytesCount)*100)/(MEM_MANAGE_BYTE_SIZE_D3_SRAM4); 
    return (uint8_t)userFloat;
}
#endif



//一键初始化全部的SRAM
void MCU_MemInitAll(void)
{
#if(MEM_MANAGE_BYTE_SIZE_AXI_SRAM != 0)
    MCU_MemInitAxiSram();
#endif
#if(MEM_MANAGE_BYTE_SIZE_ITCM_SRAM != 0)
    MCU_MemInitItcmSram();
#endif
#if(MEM_MANAGE_BYTE_SIZE_DTCM_SRAM != 0)
    MCU_MemInitDtcmSram();
#endif
#if(MEM_MANAGE_BYTE_SIZE_D2_SRAM1 != 0)
    MCU_MemInitD2Sram1();
#endif
#if(MEM_MANAGE_BYTE_SIZE_D3_SRAM4 != 0)
    MCU_MemInitD3Sram4();
#endif
}



