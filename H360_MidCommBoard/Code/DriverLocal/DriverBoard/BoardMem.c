/************************************************************************ 
 ** Author       : 邓小俊
 ** Date         : 2024-01-29 11:51:45
 ** Description  : When I Has Free Time ,I Will Write Description Below.
 ** description  : 
 ** LastEditors  : 邓小俊
 ** LastEditTime : 2024-01-29 13:08:48
 ** Email: xiaojun.deng@aliyun.com
 ** Copyright (c) 2024 by 公司名称, All Rights Reserved. 
 ************************************************************************/
#include "BoardMem.h"
#include "tx_byte_pool.h"
#include "DriverConfigBoard.h"

/*--------------------------------------------SDRAM1内存-------------------------------------------------------*/
#if(MEM_MANAGE_BYTE_SIZE_SDRAM1 != 0)
#pragma pack(push) //保存对齐状态 
#pragma pack(4)//四字节对齐
//内存池 
static uint8_t memSDRAM1_ByteArray[MEM_MANAGE_BYTE_SIZE_SDRAM1]@".sdram1";	
#pragma pack (pop)			

//管理内存池
static TX_BYTE_POOL bytePoolMemSDRAM1;

//SDRAM1内存初始化
void BoardMemInitSDRAM1(void)
{
    if(bytePoolMemSDRAM1.tx_byte_pool_id == TX_BYTE_POOL_ID)
    {
        return;
    }
    //清空内存
    UserMemSet(memSDRAM1_ByteArray,0,MEM_MANAGE_BYTE_SIZE_SDRAM1);
	//创建内存池
    tx_byte_pool_create(&bytePoolMemSDRAM1,"bytePoolMemSDRAM1",memSDRAM1_ByteArray,MEM_MANAGE_BYTE_SIZE_SDRAM1);
}

//申请内存
void* BoardMemMallocSDRAM1(uint32_t size)
{
    void* memMallocResult = NULL;
    if(bytePoolMemSDRAM1.tx_byte_pool_id != TX_BYTE_POOL_ID)
    {
        BoardMemInitSDRAM1();
    }
    //申请内存
    int32_t temp = tx_byte_allocate(&bytePoolMemSDRAM1,(void**)(&memMallocResult),size,TX_NO_WAIT);
    //回传结果
    return memMallocResult;
}

//申请内存直到成功
void* BoardMemMallocWhileSuccessSDRAM1(uint32_t size)
{
    void* memMallocResult = NULL;
    if(bytePoolMemSDRAM1.tx_byte_pool_id != TX_BYTE_POOL_ID)
    {
        BoardMemInitSDRAM1();
    }
    //申请内存
    int32_t temp = tx_byte_allocate(&bytePoolMemSDRAM1,(void**)(&memMallocResult),size,TX_WAIT_FOREVER);
    //回传结果
    return memMallocResult;
}

//释放内存
void BoardMemFreeSDRAM1(void* ptr)
{
    if(bytePoolMemSDRAM1.tx_byte_pool_id != TX_BYTE_POOL_ID)
    {
        BoardMemInitSDRAM1();
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
uint8_t BoardMemPerUsedSDRAM1(void)
{   
    ULONG availableBytesCount = 0;
    float userFloat = 0.0;
    if(bytePoolMemSDRAM1.tx_byte_pool_id != TX_BYTE_POOL_ID)
    {
        BoardMemInitSDRAM1();
    }
    tx_byte_pool_info_get(&bytePoolMemSDRAM1,TX_NULL,&availableBytesCount,TX_NULL,TX_NULL,TX_NULL,TX_NULL);
    userFloat = ((MEM_MANAGE_BYTE_SIZE_SDRAM1-availableBytesCount)*100)/(MEM_MANAGE_BYTE_SIZE_SDRAM1); 
    return (uint8_t)userFloat;
}
#endif

/*--------------------------------------------SDRAM2内存-------------------------------------------------------*/
#if(MEM_MANAGE_BYTE_SIZE_SDRAM2 != 0)
#pragma pack(push) //保存对齐状态 
#pragma pack(4)//四字节对齐
//内存池 
static uint8_t memSDRAM2_ByteArray[MEM_MANAGE_BYTE_SIZE_SDRAM2]@".sdram2";	
#pragma pack (pop)			

//管理内存池
static TX_BYTE_POOL bytePoolMemSDRAM2;

//SDRAM2内存初始化
void BoardMemInitSDRAM2(void)
{
    if(bytePoolMemSDRAM2.tx_byte_pool_id == TX_BYTE_POOL_ID)
    {
        return;
    }
    //清空内存
    UserMemSet(memSDRAM2_ByteArray,0,MEM_MANAGE_BYTE_SIZE_SDRAM2);
	//创建内存池
    tx_byte_pool_create(&bytePoolMemSDRAM2,"bytePoolMemSDRAM2",memSDRAM2_ByteArray,MEM_MANAGE_BYTE_SIZE_SDRAM2);
}

//申请内存
void* BoardMemMallocSDRAM2(uint32_t size)
{
    void* memMallocResult = NULL;
    if(bytePoolMemSDRAM2.tx_byte_pool_id != TX_BYTE_POOL_ID)
    {
        BoardMemInitSDRAM2();
    }
    //申请内存
    int32_t temp = tx_byte_allocate(&bytePoolMemSDRAM2,(void**)(&memMallocResult),size,TX_NO_WAIT);
    //回传结果
    return memMallocResult;
}

//申请内存直到成功
void* BoardMemMallocWhileSuccessSDRAM2(uint32_t size)
{
    void* memMallocResult = NULL;
    if(bytePoolMemSDRAM2.tx_byte_pool_id != TX_BYTE_POOL_ID)
    {
        BoardMemInitSDRAM2();
    }
    //申请内存
    int32_t temp = tx_byte_allocate(&bytePoolMemSDRAM2,(void**)(&memMallocResult),size,TX_WAIT_FOREVER);
    //回传结果
    return memMallocResult;
}

//释放内存
void BoardMemFreeSDRAM2(void* ptr)
{
    if(bytePoolMemSDRAM2.tx_byte_pool_id != TX_BYTE_POOL_ID)
    {
        BoardMemInitSDRAM2();
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
uint8_t BoardMemPerUsedSDRAM2(void)
{   
    ULONG availableBytesCount = 0;
    float userFloat = 0.0;
    if(bytePoolMemSDRAM2.tx_byte_pool_id != TX_BYTE_POOL_ID)
    {
        BoardMemInitSDRAM2();
    }
    tx_byte_pool_info_get(&bytePoolMemSDRAM2,TX_NULL,&availableBytesCount,TX_NULL,TX_NULL,TX_NULL,TX_NULL);
    userFloat = ((MEM_MANAGE_BYTE_SIZE_SDRAM2-availableBytesCount)*100)/(MEM_MANAGE_BYTE_SIZE_SDRAM2); 
    return (uint8_t)userFloat;
}
#endif




