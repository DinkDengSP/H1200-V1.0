/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-23 09:44:56 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-08-23 14:04:19 +0800
************************************************************************************************/ 
#include "MCU_Mem.h"
#include "DriverConfigMCU.h"
#include "tx_byte_pool.h"

/*--------------------------------------------SramIn内存-------------------------------------------------------*/
#pragma pack(push) //保存对齐状态 
#pragma pack(4)//四字节对齐
//内存池 
static uint8_t memSramInByteArray[MEM_MANAGE_BYTE_SIZE_SRAM_IN]@".sramin";	
#pragma pack (pop)			

//管理内存池
static TX_BYTE_POOL bytePoolMemSramIn;

//SramIn内存初始化
void MCU_MemInitSramIn(void)
{
    if(bytePoolMemSramIn.tx_byte_pool_id == TX_BYTE_POOL_ID)
    {
        return;
    }
    //清空内存
    UserMemSet(memSramInByteArray,0,MEM_MANAGE_BYTE_SIZE_SRAM_IN);
	//创建内存池
    tx_byte_pool_create(&bytePoolMemSramIn,"bytePoolMemSramIn",memSramInByteArray,MEM_MANAGE_BYTE_SIZE_SRAM_IN);
}

//申请内存
void* MCU_MemMallocSramIn(uint32_t size)
{
    void* memMallocResult = NULL;
    if(bytePoolMemSramIn.tx_byte_pool_id != TX_BYTE_POOL_ID)
    {
        MCU_MemInitSramIn();
    }
    //申请内存
    int32_t temp = tx_byte_allocate(&bytePoolMemSramIn,(void**)(&memMallocResult),size,TX_NO_WAIT);
    //回传结果
    return memMallocResult;
}

//申请内存直到成功
void* MCU_MemMallocWhileSuccessSramIn(uint32_t size)
{
    void* memMallocResult = NULL;
    if(bytePoolMemSramIn.tx_byte_pool_id != TX_BYTE_POOL_ID)
    {
        MCU_MemInitSramIn();
    }
    //申请内存
    int32_t temp = tx_byte_allocate(&bytePoolMemSramIn,(void**)(&memMallocResult),size,TX_WAIT_FOREVER);
    //回传结果
    return memMallocResult;
}

//释放内存
void MCU_MemFreeSramIn(void* ptr)
{
    if(bytePoolMemSramIn.tx_byte_pool_id != TX_BYTE_POOL_ID)
    {
        MCU_MemInitSramIn();
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
uint8_t MCU_MemPerUsedSramIn(void)
{   
    ULONG availableBytesCount = 0;
    float userFloat = 0.0;
    if(bytePoolMemSramIn.tx_byte_pool_id != TX_BYTE_POOL_ID)
    {
        MCU_MemInitSramIn();
    }
    tx_byte_pool_info_get(&bytePoolMemSramIn,TX_NULL,&availableBytesCount,TX_NULL,TX_NULL,TX_NULL,TX_NULL);
    userFloat = ((MEM_MANAGE_BYTE_SIZE_SRAM_IN-availableBytesCount)*100)/(MEM_MANAGE_BYTE_SIZE_SRAM_IN); 
    return (uint8_t)userFloat;
}








