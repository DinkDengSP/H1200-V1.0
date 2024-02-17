/************************************************************************ 
 ** Author       : 邓小俊
 ** Date         : 2024-01-16 11:35:28
 ** Description  : When I Has Free Time ,I Will Write Description Below.
 ** description  : 
 ** LastEditors  : 邓小俊
 ** LastEditTime : 2024-01-16 14:14:48
 ** Email: xiaojun.deng@aliyun.com
 ** Copyright (c) 2024 by 公司名称, All Rights Reserved. 
 ************************************************************************/
#include "BoardMem.h"
#include "tx_byte_pool.h"
#include "DriverConfigBoard.h"

/*--------------------------------------------SDRAM内存-------------------------------------------------------*/
#if(MEM_MANAGE_BYTE_SIZE_SDRAM != 0)
#pragma pack(push) //保存对齐状态 
#pragma pack(4)//四字节对齐
//内存池 
static uint8_t memSDRAM_ByteArray[MEM_MANAGE_BYTE_SIZE_SDRAM]@".sdram";	
#pragma pack (pop)			

//管理内存池
static TX_BYTE_POOL bytePoolMemSDRAM;

//SDRAM内存初始化
void BoardMemInitSDRAM(void)
{
    if(bytePoolMemSDRAM.tx_byte_pool_id == TX_BYTE_POOL_ID)
    {
        return;
    }
    //清空内存
    UserMemSet(memSDRAM_ByteArray,0,MEM_MANAGE_BYTE_SIZE_SDRAM);
	//创建内存池
    tx_byte_pool_create(&bytePoolMemSDRAM,"bytePoolMemSDRAM",memSDRAM_ByteArray,MEM_MANAGE_BYTE_SIZE_SDRAM);
}

//申请内存
void* BoardMemMallocSDRAM(uint32_t size)
{
    void* memMallocResult = NULL;
    if(bytePoolMemSDRAM.tx_byte_pool_id != TX_BYTE_POOL_ID)
    {
        BoardMemInitSDRAM();
    }
    //申请内存
    int32_t temp = tx_byte_allocate(&bytePoolMemSDRAM,(void**)(&memMallocResult),size,TX_NO_WAIT);
    //回传结果
    return memMallocResult;
}

//申请内存直到成功
void* BoardMemMallocWhileSuccessSDRAM(uint32_t size)
{
    void* memMallocResult = NULL;
    if(bytePoolMemSDRAM.tx_byte_pool_id != TX_BYTE_POOL_ID)
    {
        BoardMemInitSDRAM();
    }
    //申请内存
    int32_t temp = tx_byte_allocate(&bytePoolMemSDRAM,(void**)(&memMallocResult),size,TX_WAIT_FOREVER);
    //回传结果
    return memMallocResult;
}

//释放内存
void BoardMemFreeSDRAM(void* ptr)
{
    if(bytePoolMemSDRAM.tx_byte_pool_id != TX_BYTE_POOL_ID)
    {
        BoardMemInitSDRAM();
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
uint8_t BoardMemPerUsedSDRAM(void)
{   
    ULONG availableBytesCount = 0;
    float userFloat = 0.0;
    if(bytePoolMemSDRAM.tx_byte_pool_id != TX_BYTE_POOL_ID)
    {
        BoardMemInitSDRAM();
    }
    tx_byte_pool_info_get(&bytePoolMemSDRAM,TX_NULL,&availableBytesCount,TX_NULL,TX_NULL,TX_NULL,TX_NULL);
    userFloat = ((MEM_MANAGE_BYTE_SIZE_SDRAM-availableBytesCount)*100)/(MEM_MANAGE_BYTE_SIZE_SDRAM); 
    return (uint8_t)userFloat;
}



#endif


