/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-16 13:31:54 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-16 17:17:52 +0800
************************************************************************************************/ 
#include "MCU_Mem.h"
#include "DriverConfigMCU.h"
#include "tx_byte_pool.h"

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












