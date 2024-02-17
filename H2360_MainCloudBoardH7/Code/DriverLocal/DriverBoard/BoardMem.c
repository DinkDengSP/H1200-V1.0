/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-03 16:12:17 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-06 13:50:11 +0800
************************************************************************************************/ 
#include "BoardMem.h"
#include "tx_byte_pool.h"
#include "DriverConfigBoard.h"

/*--------------------------------------------SRAM内存-------------------------------------------------------*/
#if(MEM_MANAGE_BYTE_SIZE_SRAM != 0)
#pragma pack(push) //保存对齐状态 
#pragma pack(4)//四字节对齐
//内存池 
static uint8_t memSRAM_ByteArray[MEM_MANAGE_BYTE_SIZE_SRAM]@".ext_sram";	
#pragma pack (pop)			

//管理内存池
static TX_BYTE_POOL bytePoolMemSRAM;

//SRAM内存初始化
void BoardMemInitSRAM(void)
{
    if(bytePoolMemSRAM.tx_byte_pool_id == TX_BYTE_POOL_ID)
    {
        return;
    }
    //清空内存
    UserMemSet(memSRAM_ByteArray,0,MEM_MANAGE_BYTE_SIZE_SRAM);
	//创建内存池
    tx_byte_pool_create(&bytePoolMemSRAM,"bytePoolMemSRAM",memSRAM_ByteArray,MEM_MANAGE_BYTE_SIZE_SRAM);
}

//申请内存
void* BoardMemMallocSRAM(uint32_t size)
{
    void* memMallocResult = NULL;
    if(bytePoolMemSRAM.tx_byte_pool_id != TX_BYTE_POOL_ID)
    {
        BoardMemInitSRAM();
    }
    //申请内存
    int32_t temp = tx_byte_allocate(&bytePoolMemSRAM,(void**)(&memMallocResult),size,TX_NO_WAIT);
    //回传结果
    return memMallocResult;
}

//申请内存直到成功
void* BoardMemMallocWhileSuccessSRAM(uint32_t size)
{
    void* memMallocResult = NULL;
    if(bytePoolMemSRAM.tx_byte_pool_id != TX_BYTE_POOL_ID)
    {
        BoardMemInitSRAM();
    }
    //申请内存
    int32_t temp = tx_byte_allocate(&bytePoolMemSRAM,(void**)(&memMallocResult),size,TX_WAIT_FOREVER);
    //回传结果
    return memMallocResult;
}

//释放内存
void BoardMemFreeSRAM(void* ptr)
{
    if(bytePoolMemSRAM.tx_byte_pool_id != TX_BYTE_POOL_ID)
    {
        BoardMemInitSRAM();
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
uint8_t BoardMemPerUsedSRAM(void)
{   
    ULONG availableBytesCount = 0;
    float userFloat = 0.0;
    if(bytePoolMemSRAM.tx_byte_pool_id != TX_BYTE_POOL_ID)
    {
        BoardMemInitSRAM();
    }
    tx_byte_pool_info_get(&bytePoolMemSRAM,TX_NULL,&availableBytesCount,TX_NULL,TX_NULL,TX_NULL,TX_NULL);
    userFloat = ((MEM_MANAGE_BYTE_SIZE_SRAM-availableBytesCount)*100)/(MEM_MANAGE_BYTE_SIZE_SRAM); 
    return (uint8_t)userFloat;
}
#endif












