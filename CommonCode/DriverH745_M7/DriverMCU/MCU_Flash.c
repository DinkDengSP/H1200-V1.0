/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-19 17:06:01 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2024-01-02 15:08:38 +0800
************************************************************************************************/ 
#include "MCU_Flash.h"
#include "MCU_LogShow.h"
#include "DriverConfigMCU.h"
#include "MCU_Random.h"

//获取某个地址所在的flash扇区,仅用于BANK1！！
//addr:flash地址
//返回值:0~11,即addr所在的扇区
static uint16_t MCU_FlashGetFlashBank1Sector(uint32_t addr)
{
	if(addr<ADDR_FLASH_SECTOR_1_BANK1)
        return FLASH_SECTOR_0;
	else if(addr<ADDR_FLASH_SECTOR_2_BANK1)
        return FLASH_SECTOR_1;
	else if(addr<ADDR_FLASH_SECTOR_3_BANK1)
        return FLASH_SECTOR_2;
	else if(addr<ADDR_FLASH_SECTOR_4_BANK1)
        return FLASH_SECTOR_3;
	else if(addr<ADDR_FLASH_SECTOR_5_BANK1)
        return FLASH_SECTOR_4;
	else if(addr<ADDR_FLASH_SECTOR_6_BANK1)
        return FLASH_SECTOR_5;
	else if(addr<ADDR_FLASH_SECTOR_7_BANK1)
        return FLASH_SECTOR_6;
	return FLASH_SECTOR_7;	
}

//获取某个地址所在的flash扇区,仅用于BANK2！！
//addr:flash地址
//返回值:0~11,即addr所在的扇区
static uint16_t MCU_FlashGetFlashBank2Sector(uint32_t addr)
{
	if(addr<ADDR_FLASH_SECTOR_1_BANK2)
        return FLASH_SECTOR_0;
	else if(addr<ADDR_FLASH_SECTOR_2_BANK2)
        return FLASH_SECTOR_1;
	else if(addr<ADDR_FLASH_SECTOR_3_BANK2)
        return FLASH_SECTOR_2;
	else if(addr<ADDR_FLASH_SECTOR_4_BANK2)
        return FLASH_SECTOR_3;
	else if(addr<ADDR_FLASH_SECTOR_5_BANK2)
        return FLASH_SECTOR_4;
	else if(addr<ADDR_FLASH_SECTOR_6_BANK2)
        return FLASH_SECTOR_5;
	else if(addr<ADDR_FLASH_SECTOR_7_BANK2)
        return FLASH_SECTOR_6;
	return FLASH_SECTOR_7;	
}

//通过地址计算扇区编号
static uint8_t MCU_FlashCalcBank(uint32_t flashAddr)
{
    if(flashAddr < ADDR_FLASH_SECTOR_0_BANK2)
    {
        return FLASH_BANK_1;
    }
    else
    {
        return FLASH_BANK_2;
    }
}

//读出字
uint32_t MCU_FlashReadWord(uint32_t addr)
{
    return *(__IO uint32_t *)addr; 
}

//从指定地址开始读出指定长度的数据
void MCU_FlashReadBuffer(uint32_t readAddr,uint32_t *bufferPtr,uint32_t dwordNumber)
{
    uint32_t indexUtil = 0;
	for(indexUtil = 0;indexUtil < dwordNumber;indexUtil++)
	{
        //读取4个字节.
		bufferPtr[indexUtil] = MCU_FlashReadWord(readAddr);
        //偏移4个字节.
		readAddr += 4;	
	}
}  

//比较Flash空间和buffer空间的数据是否对应的上,决定该空间是否需要擦除
MCU_FLASH_CMP_RESULT MCU_FlashCompare(uint32_t flashAddr, uint8_t* bufferPtr, uint32_t size)
{
    uint32_t indexUtil;
	uint8_t flashEqualFlag;	
	uint8_t readByteTemp;
	//如果偏移地址超过芯片容量，输出错误
    if (flashAddr + size > MCU_FLASH_BASE + 2*MCU_FLASH_BANK_SIZE)
    {
        /*　函数参数错误　*/
        return MCU_FLASH_CMP_PARAM_ERR;		
    }
	//长度为0时返回正确
    if (size == 0)
    {
        /* Flash内容和待写入的数据相等 */
        return MCU_FLASH_CMP_EQU;		
    }
    /* 先假设所有字节和待写入的数据相等，如果遇到任何一个不相等，则设置为 0 */
	flashEqualFlag = 1;			
	for (indexUtil = 0; indexUtil < size; indexUtil++)
	{
		readByteTemp = *(__IO uint8_t *)flashAddr;

		if (readByteTemp != *bufferPtr)
		{
			if (readByteTemp != 0xFF)
			{
                //需要擦除后再写
				return MCU_FLASH_CMP_ERASE;		
			}
			else
			{
                //不相等,需要写,但是不需要擦除
				flashEqualFlag = 0;	
			}
		}
		flashAddr++;
		bufferPtr++;
	}
    /* Flash内容和待写入的数据相等，不需要擦除和写操作 */
	if (flashEqualFlag == 1)
	{
		return MCU_FLASH_CMP_EQU;	
	}
	else
	{
        //Flash不需要擦除，直接写
		return MCU_FLASH_CMP_WRITE;	
	}
} 	

//通过Flash地址计算扇区号码和bank号码
static void MCU_FlashCalcBankAndSectorWithAddr(uint32_t flashAddr,uint8_t* bankNoPtr,uint32_t* sectorNoPtr)
{
    //计算bank
    *bankNoPtr = MCU_FlashCalcBank(flashAddr);
    if(*bankNoPtr == FLASH_BANK_1)
    {
        *sectorNoPtr = MCU_FlashGetFlashBank1Sector(flashAddr);
    }
    else 
    {
        *sectorNoPtr = MCU_FlashGetFlashBank2Sector(flashAddr);
    }
}

//清除指定FLASH区域全部错误
static void MCU_FlashClearAllErrorByBankNo(uint8_t bankNo)
{
    if(bankNo == FLASH_BANK_1)
    {
        __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_ALL_ERRORS_BANK1);
    }
    else
    {
        __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_ALL_ERRORS_BANK2);
    }
}

//擦除指定扇区
static ERROR_SUB MCU_FlashEraseSector(uint8_t bankNo,uint32_t sectorNo)
{
    FLASH_EraseInitTypeDef FlashEraseInit;
    HAL_StatusTypeDef flashState;
    uint32_t lastIntState;
    //擦除对象
    FlashEraseInit.TypeErase = FLASH_TYPEERASE_SECTORS; //擦除类型，扇区擦除 
    FlashEraseInit.Sector = sectorNo;                   //要擦除的扇区
    FlashEraseInit.Banks = bankNo;                      //操作BANK
    FlashEraseInit.NbSectors = 1;                       //一次只擦除一个扇区
    FlashEraseInit.VoltageRange = FLASH_VOLTAGE_RANGE_4;//64bit操作
    /* 关中断 */
    lastIntState = CoreLockIntGlobal();
    //关闭Cache
    CoreDCacheSet(DISABLE);
    CoreICacheSet(DISABLE);
    //清除无效的D-Cache
    SCB_CleanInvalidateDCache();  
    //Flash解锁
    if(FLASH_BANK_1 == bankNo)
    {
        flashState = HAL_FLASHEx_Unlock_Bank1();
    }
    else
    {
        flashState = HAL_FLASHEx_Unlock_Bank2();
    }
    if(flashState != HAL_OK)
    {
        //Flash重新上锁
        if(FLASH_BANK_1 == bankNo)
        {
            flashState = HAL_FLASHEx_Lock_Bank1();
        }
        else
        {
            flashState = HAL_FLASHEx_Lock_Bank2();
        }
        //打开Cache
        CoreDCacheSet(CORE_DCACHE_SET);
        CoreICacheSet(CORE_ICACHE_SET);
        /* 开中断 */
        CoreUnlockIntGlobal(lastIntState);
        //日志显示
        MCU_LogShowHalState(flashState,HAL_FLASH_GetError());
        return ERROR_SUB_MCU_FLASH_UNLOCK;
    }
    //清除指定区域全部错误
    MCU_FlashClearAllErrorByBankNo(bankNo);
    //发出擦除指令
    flashState = HAL_FLASHEx_Erase(&FlashEraseInit,&sectorNo);
    if(flashState != HAL_OK)
	{
        //Flash重新上锁
        if(FLASH_BANK_1 == bankNo)
        {
            flashState = HAL_FLASHEx_Lock_Bank1();
        }
        else
        {
            flashState = HAL_FLASHEx_Lock_Bank2();
        }
        //打开Cache
        CoreDCacheSet(CORE_DCACHE_SET);
        CoreICacheSet(CORE_ICACHE_SET);
        /* 开中断 */
        CoreUnlockIntGlobal(lastIntState);
        //日志显示
        MCU_LogShowHalState(flashState,HAL_FLASH_GetError());
        return ERROR_SUB_MCU_FLASH_ERASE_CMD;
    }      
    //Flash重新上锁
    if(FLASH_BANK_1 == bankNo)
    {
        flashState = HAL_FLASHEx_Lock_Bank1();
    }
    else
    {
        flashState = HAL_FLASHEx_Lock_Bank2();
    }
    //打开Cache
    CoreDCacheSet(CORE_DCACHE_SET);
    CoreICacheSet(CORE_ICACHE_SET);
    /* 开中断 */
    CoreUnlockIntGlobal(lastIntState);
    return ERROR_SUB_OK;
}

//FLASH写入,不带擦除
static ERROR_SUB MCU_FlashWriteNoCheck(uint32_t startAddr,uint8_t *bufferPtr,uint32_t byteCount)
{
    HAL_StatusTypeDef flashState;
    uint32_t lastIntState;
    uint32_t indexUtil = 0;
    uint64_t localFlashDataUtil[4];
    /* 关中断 */
    lastIntState = CoreLockIntGlobal();
    //关闭Cache
    CoreDCacheSet(DISABLE);
    CoreICacheSet(DISABLE);
    //清除无效的D-Cache
    SCB_CleanInvalidateDCache(); 
    //Flash解锁
 	flashState = HAL_FLASH_Unlock();	
    if(flashState != HAL_OK)
	{
        //Flash重新上锁
        HAL_FLASH_Lock();
        //打开Cache
        CoreDCacheSet(CORE_DCACHE_SET);
        CoreICacheSet(CORE_ICACHE_SET);
        /* 开中断 */
        CoreUnlockIntGlobal(lastIntState);
        //日志显示
        MCU_LogShowHalState(flashState,HAL_FLASH_GetError());
        return ERROR_SUB_MCU_FLASH_UNLOCK;
    }  
    for (indexUtil = 0; indexUtil < (byteCount / 32); indexUtil++)   
    {
        //数据拷贝便于对齐
        UserMemCopy((uint8_t *)localFlashDataUtil, bufferPtr, 32);
        //指针增长
        bufferPtr += 32;
        //编程
        flashState = HAL_FLASH_Program(FLASH_TYPEPROGRAM_FLASHWORD,startAddr,(uint32_t)localFlashDataUtil);
        if(flashState != HAL_OK)
	    {
            //Flash重新上锁
            HAL_FLASH_Lock();
            //打开Cache
            CoreDCacheSet(CORE_DCACHE_SET);
            CoreICacheSet(CORE_ICACHE_SET);
            /* 开中断 */
            CoreUnlockIntGlobal(lastIntState);
            //日志显示
            MCU_LogShowHalState(flashState,HAL_FLASH_GetError());
            return ERROR_SUB_MCU_FLASH_PAGE_WRITE;
        }    
        //STM32H7的Flash编程时，务必保证要编程的地址是32字节对齐的，即此地址对32求余为0。
        //并且编程的数据必须32字节整数倍
        startAddr += 32;
    } 
    //判定长度是否为32字节整数倍
    if ((byteCount % 32) != 0)
    {
        //填充处理
        localFlashDataUtil[0] = 0XFFFFFFFFFFFFFFFF;
        localFlashDataUtil[1] = 0XFFFFFFFFFFFFFFFF;
        localFlashDataUtil[2] = 0XFFFFFFFFFFFFFFFF;
        localFlashDataUtil[3] = 0XFFFFFFFFFFFFFFFF;
        UserMemCopy((uint8_t *)localFlashDataUtil, bufferPtr, (byteCount % 32));
        //编程
        flashState = HAL_FLASH_Program(FLASH_TYPEPROGRAM_FLASHWORD,startAddr,(uint32_t)localFlashDataUtil);
        if(flashState != HAL_OK)
	    {
            //Flash重新上锁
            HAL_FLASH_Lock();
            //打开Cache
            CoreDCacheSet(CORE_DCACHE_SET);
            CoreICacheSet(CORE_ICACHE_SET);
            /* 开中断 */
            CoreUnlockIntGlobal(lastIntState);
            //日志显示
            MCU_LogShowHalState(flashState,HAL_FLASH_GetError());
            return ERROR_SUB_MCU_FLASH_PAGE_WRITE;
        } 
    }
    //Flash重新上锁
    HAL_FLASH_Lock();
    //打开Cache
    CoreDCacheSet(CORE_DCACHE_SET);
    CoreICacheSet(CORE_ICACHE_SET);
    /* 开中断 */
    CoreUnlockIntGlobal(lastIntState);
    return ERROR_SUB_OK;
}

//擦除指定地址的扇区，外部需要关闭中断
ERROR_SUB MCU_FlashEraseSectorByAddr(uint32_t flashAddr)
{
    //计算bank
    uint8_t bankNo = 0;
    //计算扇区号码
    uint32_t sectorNo =0;
    //计算bank编号和扇区编号
    MCU_FlashCalcBankAndSectorWithAddr(flashAddr,&bankNo,&sectorNo);
    //擦除
    return MCU_FlashEraseSector(bankNo,sectorNo);
}

//擦除使用的FLASH空间，查看是否擦成功
ERROR_SUB MCU_FlashEraseRangeWithCheck(uint32_t startAddr,uint32_t byteCount)
{
    ERROR_SUB errorCodeSub = ERROR_SUB_OK;
    //地址计数辅助          
	uint32_t addressUtil = startAddr;		
    //地址计数结束		
	uint32_t endAddr = startAddr + byteCount;	
    //扫清一切障碍.(对非FFFFFFFF的地方,先擦除)
    while(addressUtil < endAddr)		
    {
        //有非0XFFFFFFFF的地方,要擦除这个扇区
        if(MCU_FlashReadWord(addressUtil)!=0XFFFFFFFF)
        {   
            errorCodeSub = MCU_FlashEraseSectorByAddr(addressUtil);
            if(errorCodeSub != ERROR_SUB_OK)
            {
                MCU_LogShowErrorSubCode(errorCodeSub);
                return errorCodeSub;
            }                    
        }
        addressUtil += 4;   
    }
    //擦除完毕，重新读取
    addressUtil = startAddr;
    while(addressUtil<endAddr)
    {
        //擦除完成后检验对应地址是不是空数据
        if(MCU_FlashReadWord(addressUtil) != 0XFFFFFFFF)
        { 
            errorCodeSub = ERROR_SUB_MCU_FLASH_ERASE_CHECK;
            MCU_LogShowErrorSubCode(errorCodeSub);
            return errorCodeSub;
        }
        addressUtil+=4;
    } 
    return ERROR_SUB_OK;
}

//从指定地址开始写入指定长度的数据  	  
ERROR_SUB MCU_FlashWrite(uint32_t startAddr,uint8_t *bufferPtr,uint32_t byteCount)
{
    ERROR_SUB errorCodeSub = ERROR_SUB_OK;
    //非法地址
    if((startAddr < MCU_FLASH_BASE)||(startAddr%32 != 0))
    {
        //必须是32字节对齐的地址
        errorCodeSub = ERROR_SUB_MCU_FLASH_ADDR_LIMIT;
        MCU_LogShowErrorSubCode(errorCodeSub);
        return errorCodeSub;
    }
    //擦除指定区域
    errorCodeSub = MCU_FlashEraseRangeWithCheck(startAddr,byteCount);
    if(errorCodeSub != ERROR_SUB_OK)
    {
        MCU_LogShowErrorSubCode(errorCodeSub);
        return errorCodeSub;
    } 
    //写入数据
    errorCodeSub = MCU_FlashWriteNoCheck(startAddr,bufferPtr,byteCount);
    if(errorCodeSub != ERROR_SUB_OK)
    {
        MCU_LogShowErrorSubCode(errorCodeSub);
        return errorCodeSub;
    } 
    return ERROR_SUB_OK;
}



//MCU Flash Check
ERROR_SUB MCU_FlashCheck(void)
{
    uint32_t srcDat = 0;
    uint32_t writeDat = MCU_RandomGetNextUnsigned(0,0XFFFFFFFF);
    uint32_t readDat = 0;
    uint32_t testAddr = MCU_RandomGetNextUnsigned(ADDR_FLASH_SECTOR_6_BANK1+4,ADDR_FLASH_SECTOR_7_BANK1 -1);
    //32字节对齐
    testAddr /= 32;
    testAddr *= 32;
    //自检开始
    MCU_LogPrintf("%s Start,Addr: 0X%08X,WriteData: 0X%08X\r\n",__func__,testAddr,writeDat);
    //读取源
    srcDat = MCU_FlashReadWord(testAddr);
    //写入新的数据
    MCU_FlashWrite(testAddr,(uint8_t*)&writeDat,4);
    //读取出来
    readDat = MCU_FlashReadWord(testAddr);
    //比较数据
    if(readDat != writeDat)
    {
        //错误日志打印
        MCU_LogShowErrorSubCode(ERROR_SUB_MCU_FLASH_CHECK);
        return ERROR_SUB_MCU_FLASH_CHECK;
    }
    //原始数据写回去
    MCU_FlashWrite(testAddr,(uint8_t*)&srcDat,4);
    //设置异常值
    readDat = 0XAA5555AA;
    //读取出来
    readDat = MCU_FlashReadWord(testAddr);
    //比较数据
    if(readDat != srcDat)
    {
        //错误日志打印
        MCU_LogShowErrorSubCode(ERROR_SUB_MCU_FLASH_CHECK);
        return ERROR_SUB_MCU_FLASH_CHECK;
    }
    return ERROR_SUB_OK;
}















