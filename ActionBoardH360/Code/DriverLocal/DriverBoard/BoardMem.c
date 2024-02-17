/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-30 10:08:12 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-31 14:35:30 +0800
************************************************************************************************/ 
#include "BoardMem.h"
#include "DriverConfigBoard.h"
#include "tx_byte_pool.h"

/*--------------------------------------------SramExt内存-------------------------------------------------------*/
#pragma pack(push) //保存对齐状态 
#pragma pack(4)//四字节对齐
//内存池 
static uint8_t memSramExtByteArray[MEM_MANAGE_BYTE_SIZE_SRAM_EXT]@".sramext";	
#pragma pack (pop)			

//管理内存池
static TX_BYTE_POOL bytePoolMemSramExt;

//SramExt内存初始化
void BoardMemInitSramExt(void)
{
    if(bytePoolMemSramExt.tx_byte_pool_id == TX_BYTE_POOL_ID)
    {
        return;
    }
    //清空内存
    UserMemSet(memSramExtByteArray,0,MEM_MANAGE_BYTE_SIZE_SRAM_EXT);
	//创建内存池
    tx_byte_pool_create(&bytePoolMemSramExt,"bytePoolMemSramExt",memSramExtByteArray,MEM_MANAGE_BYTE_SIZE_SRAM_EXT);
}

//申请内存
void* BoardMemMallocSramExt(uint32_t size)
{
    void* memMallocResult = NULL;
    if(bytePoolMemSramExt.tx_byte_pool_id != TX_BYTE_POOL_ID)
    {
        BoardMemInitSramExt();
    }
    //申请内存
    int32_t temp = tx_byte_allocate(&bytePoolMemSramExt,(void**)(&memMallocResult),size,TX_NO_WAIT);
    //回传结果
    return memMallocResult;
}

//申请内存直到成功
void* BoardMemMallocWhileSuccessSramExt(uint32_t size)
{
    void* memMallocResult = NULL;
    if(bytePoolMemSramExt.tx_byte_pool_id != TX_BYTE_POOL_ID)
    {
        BoardMemInitSramExt();
    }
    //申请内存
    int32_t temp = tx_byte_allocate(&bytePoolMemSramExt,(void**)(&memMallocResult),size,TX_WAIT_FOREVER);
    //回传结果
    return memMallocResult;
}

//释放内存
void BoardMemFreeSramExt(void* ptr)
{
    if(bytePoolMemSramExt.tx_byte_pool_id != TX_BYTE_POOL_ID)
    {
        BoardMemInitSramExt();
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
uint8_t BoardMemPerUsedSramExt(void)
{   
    ULONG availableBytesCount = 0;
    float userFloat = 0.0;
    if(bytePoolMemSramExt.tx_byte_pool_id != TX_BYTE_POOL_ID)
    {
        BoardMemInitSramExt();
    }
    tx_byte_pool_info_get(&bytePoolMemSramExt,TX_NULL,&availableBytesCount,TX_NULL,TX_NULL,TX_NULL,TX_NULL);
    userFloat = ((MEM_MANAGE_BYTE_SIZE_SRAM_EXT-availableBytesCount)*100)/(MEM_MANAGE_BYTE_SIZE_SRAM_EXT); 
    return (uint8_t)userFloat;
}








