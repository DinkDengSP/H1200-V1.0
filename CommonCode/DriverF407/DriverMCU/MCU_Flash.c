/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-23 09:44:56 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-08-28 17:36:20 +0800
************************************************************************************************/ 
#include "MCU_Flash.h"
#include "MCU_LogShow.h"
#include "MCU_Random.h"
#include "DriverConfigMCU.H"

//FLASH 扇区的起始地址
#define ADDR_FLASH_SECTOR_0     ((uint32_t)0x08000000) 	//扇区0起始地址, 16 Kbytes  
#define ADDR_FLASH_SECTOR_1     ((uint32_t)0x08004000) 	//扇区1起始地址, 16 Kbytes  
#define ADDR_FLASH_SECTOR_2     ((uint32_t)0x08008000) 	//扇区2起始地址, 16 Kbytes  
#define ADDR_FLASH_SECTOR_3     ((uint32_t)0x0800C000) 	//扇区3起始地址, 16 Kbytes  
#define ADDR_FLASH_SECTOR_4     ((uint32_t)0x08010000) 	//扇区4起始地址, 64 Kbytes  
#define ADDR_FLASH_SECTOR_5     ((uint32_t)0x08020000) 	//扇区5起始地址, 128 Kbytes  
#define ADDR_FLASH_SECTOR_6     ((uint32_t)0x08040000) 	//扇区6起始地址, 128 Kbytes  
#define ADDR_FLASH_SECTOR_7     ((uint32_t)0x08060000) 	//扇区7起始地址, 128 Kbytes  
#define ADDR_FLASH_SECTOR_8     ((uint32_t)0x08080000) 	//扇区8起始地址, 128 Kbytes  
#define ADDR_FLASH_SECTOR_9     ((uint32_t)0x080A0000) 	//扇区9起始地址, 128 Kbytes  
#define ADDR_FLASH_SECTOR_10    ((uint32_t)0x080C0000) 	//扇区10起始地址,128 Kbytes  
#define ADDR_FLASH_SECTOR_11    ((uint32_t)0x080E0000) 	//扇区11起始地址,128 Kbytes    

//读出字
uint32_t MCU_FlashReadWord(uint32_t addr)
{
    return *(__IO uint32_t *)addr; 
}

//从指定地址开始读出指定长度的数据
void MCU_FlashReadBuffer(uint32_t readAddr,uint32_t *bufferPtr,uint32_t wordNumber)
{
    uint32_t indexUtil = 0;
	for(indexUtil = 0;indexUtil < wordNumber;indexUtil++)
	{
        //读取4个字节.
		bufferPtr[indexUtil] = MCU_FlashReadWord(readAddr);
        //偏移4个字节.
		readAddr += 4;	
	}
}  

//从指定地址开始读出指定长度的数据
void MCU_FlashReadBufferBytes(uint32_t readAddr,uint8_t *bufferPtr,uint32_t byteCount)
{
    uint32_t lastWord = 0XFFFFFFFF;
    uint32_t dwordCount = byteCount/4;
    uint8_t legacyBytes = byteCount % 4;
    //4字节读取
    MCU_FlashReadBuffer(readAddr,(uint32_t*)bufferPtr,dwordCount);
    if(legacyBytes != 0)
    {
        //四字节不整除
        MCU_FlashReadBuffer(readAddr+(4*dwordCount),&lastWord,1);
        //补上最后四字节,小端
        if(legacyBytes > 0)bufferPtr[4*dwordCount] = lastWord;
        if(legacyBytes > 1)bufferPtr[(4*dwordCount)+1] = (uint8_t)(lastWord>>8);
        if(legacyBytes > 2)bufferPtr[(4*dwordCount)+2] = (uint8_t)(lastWord>>16);
    }
}  

//通过Flash地址计算扇区号码和bank号码
static void MCU_FlashCalcBankAndSectorWithAddr(uint32_t flashAddr,uint8_t* bankNoPtr,uint32_t* sectorNoPtr)
{
	if(flashAddr < ADDR_FLASH_SECTOR_1)
    {
        *bankNoPtr = FLASH_BANK_1;
        *sectorNoPtr = FLASH_SECTOR_0;
    }
	else if(flashAddr < ADDR_FLASH_SECTOR_2)
    {
        *bankNoPtr = FLASH_BANK_1;
        *sectorNoPtr = FLASH_SECTOR_1;
    }
	else if(flashAddr < ADDR_FLASH_SECTOR_3)
    {
        *bankNoPtr = FLASH_BANK_1;
        *sectorNoPtr = FLASH_SECTOR_2;
    }
	else if(flashAddr < ADDR_FLASH_SECTOR_4)
    {
        *bankNoPtr = FLASH_BANK_1;
        *sectorNoPtr = FLASH_SECTOR_3;
    }
	else if(flashAddr < ADDR_FLASH_SECTOR_5)
    {
        *bankNoPtr = FLASH_BANK_1;
        *sectorNoPtr = FLASH_SECTOR_4;
    }
	else if(flashAddr < ADDR_FLASH_SECTOR_6)
    {
        *bankNoPtr = FLASH_BANK_1;
        *sectorNoPtr = FLASH_SECTOR_5;
    }
	else if(flashAddr < ADDR_FLASH_SECTOR_7)
    {
        *bankNoPtr = FLASH_BANK_1;
        *sectorNoPtr = FLASH_SECTOR_6;
    }
	else if(flashAddr < ADDR_FLASH_SECTOR_8)
    {
        *bankNoPtr = FLASH_BANK_1;
        *sectorNoPtr = FLASH_SECTOR_7;
    }
	else if(flashAddr < ADDR_FLASH_SECTOR_9)
    {
        *bankNoPtr = FLASH_BANK_1;
        *sectorNoPtr = FLASH_SECTOR_8;
    }
	else if(flashAddr < ADDR_FLASH_SECTOR_10)
    {
        *bankNoPtr = FLASH_BANK_1;
        *sectorNoPtr = FLASH_SECTOR_9;
    }
	else if(flashAddr < ADDR_FLASH_SECTOR_11)
    {
        *bankNoPtr = FLASH_BANK_1;
        *sectorNoPtr = FLASH_SECTOR_10;
    }
	else
    {
        *bankNoPtr = FLASH_BANK_1;
        *sectorNoPtr = FLASH_SECTOR_11;
    }
}

//比较Flash空间和buffer空间的数据是否对应的上,决定该空间是否需要擦除
MCU_FLASH_CMP_RESULT MCU_FlashCompare(uint32_t flashAddr, uint8_t* bufferPtr, uint32_t size)
{
    uint32_t indexUtil;
	uint8_t flashEqualFlag;	
	uint8_t readByteTemp;
	//如果偏移地址超过芯片容量，输出错误
    if (flashAddr + size > FLASH_BASE + MCU_FLASH_BANK_MAX_SIZE)
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
        //读取FLASH数据
		readByteTemp = *(uint8_t *)flashAddr;
        //检查是否相等
		if (readByteTemp != (*bufferPtr))
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
        //flash地址增加,buffer地址增加
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

//清除指定FLASH区域全部错误
static void MCU_FlashClearAllError(void)
{
#if defined(FLASH_SR_RDERR)
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP|FLASH_FLAG_OPERR|FLASH_FLAG_WRPERR|FLASH_FLAG_PGAERR|
                            FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR|FLASH_FLAG_RDERR|FLASH_FLAG_BSY);
#else
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP|FLASH_FLAG_OPERR|FLASH_FLAG_WRPERR|FLASH_FLAG_PGAERR|
                            FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR|FLASH_FLAG_BSY);
#endif
}

//flash操作前置
static uint32_t MCU_FlashOperationPrev(void)
{
    uint32_t lastIntState;
    //关中断
    lastIntState = CoreLockIntGlobal();
    //刷新缓存
    FLASH_FlushCaches();
    //Flash解锁
 	HAL_FLASH_Unlock();
    //清除指定区域全部错误
    MCU_FlashClearAllError();
    //返回中断状态
    return lastIntState;
}

//flash操作后置
static void MCU_FlashOperationAfter(uint32_t lastIntState)
{
    //Flash重新上锁
    HAL_FLASH_Lock();
    //刷新缓存
    FLASH_FlushCaches();
    /* 开中断 */
    CoreUnlockIntGlobal(lastIntState);
}

//擦除指定扇区
static ERROR_SUB MCU_FlashEraseSector(uint8_t bankNo,uint32_t sectorNo)
{
    FLASH_EraseInitTypeDef FlashEraseInit;
    HAL_StatusTypeDef halState;
    uint32_t sectorError;
    uint32_t lastIntState;
    //擦除对象
    FlashEraseInit.TypeErase = FLASH_TYPEERASE_SECTORS; //擦除类型，扇区擦除 
    FlashEraseInit.Sector = sectorNo;                   //要擦除的扇区
    FlashEraseInit.Banks = bankNo;                      //操作BANK
    FlashEraseInit.NbSectors = 1;                       //一次只擦除一个扇区
    FlashEraseInit.VoltageRange = FLASH_VOLTAGE_RANGE_3;//电压范围，VCC=2.7~3.6V之间!!
    //擦写flash前置操作
    lastIntState = MCU_FlashOperationPrev();
    //发出擦除指令
    halState = HAL_FLASHEx_Erase(&FlashEraseInit,&sectorError);
    if(halState != HAL_OK)
	{
        //擦写flash后置操作
        MCU_FlashOperationAfter(lastIntState);
        //日志显示
        MCU_LogShowHalState(halState,sectorError);
        //显示错误码
        MCU_LogPrintf("HAL_FLASHEx_Erase sectorError : 0X%08X\r\n",sectorError);
        return ERROR_SUB_MCU_FLASH_ERASE_CMD;
    }    
    //没有异常,等待擦除操作完成
    halState = FLASH_WaitForLastOperation(MCU_FLASH_WAIT_OP_TIME);   
    //擦写flash后置操作
    MCU_FlashOperationAfter(lastIntState);
    if(halState != HAL_OK)
	{
        //日志显示
        MCU_LogShowHalState(halState,0);
        return ERROR_SUB_MCU_FLASH_ERASE_TIME_OUT;
    }    
    return ERROR_SUB_OK;
}


//FLASH写入,不带擦除
static ERROR_SUB MCU_FlashWriteNoCheck(uint32_t startAddr,uint32_t *bufferPtr,uint32_t wordNumber)
{
    HAL_StatusTypeDef flashState;
    uint32_t lastIntState;
    //地址计数辅助          
	uint32_t addressUtil = startAddr;		
    //地址计数结束		
	uint32_t endAddr = startAddr + (wordNumber*4);
    //擦写flash前置操作
    lastIntState = MCU_FlashOperationPrev();
    while(addressUtil < endAddr)
    {
        flashState = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,addressUtil,(uint64_t)(*bufferPtr));
        if(flashState != HAL_OK)
	    {
            //擦写flash后置操作
            MCU_FlashOperationAfter(lastIntState);
            //日志显示
            MCU_LogShowHalState(flashState,0);
            return ERROR_SUB_MCU_FLASH_PAGE_WRITE;
        }    
        addressUtil += 4;
        bufferPtr += 1;
    } 
    //擦写flash后置操作
    MCU_FlashOperationAfter(lastIntState);
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
ERROR_SUB MCU_FlashEraseRangeWithCheck(uint32_t startAddr,uint32_t wordNumber)
{
    ERROR_SUB errorCodeSub = ERROR_SUB_OK;
    //地址计数辅助          
	uint32_t addressUtil = startAddr;		
    //地址计数结束		
	uint32_t endAddr = startAddr + (wordNumber*4);	
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
ERROR_SUB MCU_FlashWrite(uint32_t startAddr,uint32_t *bufferPtr,uint32_t wordNumber)
{
    ERROR_SUB errorCodeSub = ERROR_SUB_OK;
    //非法地址,数据必须大于基础地址,且四字节对齐
    if((startAddr < FLASH_BASE)||(startAddr%4))
    {
        errorCodeSub = ERROR_SUB_MCU_FLASH_ADDR_LIMIT;
        MCU_LogShowErrorSubCode(errorCodeSub);
        return errorCodeSub;
    }
    uint8_t* bufferPtrLocal = (uint8_t*)bufferPtr;
    MCU_FLASH_CMP_RESULT compareResult = MCU_FLASH_CMP_EQU;
    //检查要写入的数据和对应的写入空间是否匹配,要是完全一样就不用写入了
    compareResult = MCU_FlashCompare(startAddr,bufferPtrLocal,wordNumber*4);
    if(compareResult == MCU_FLASH_CMP_ERASE)
    {
        //需要擦除之后才能写入
        //擦除指定区域
        errorCodeSub = MCU_FlashEraseRangeWithCheck(startAddr,wordNumber);
        if(errorCodeSub != ERROR_SUB_OK)
        {
            MCU_LogShowErrorSubCode(errorCodeSub);
            return errorCodeSub;
        } 
        //写入数据
        errorCodeSub = MCU_FlashWriteNoCheck(startAddr,bufferPtr,wordNumber);
        if(errorCodeSub != ERROR_SUB_OK)
        {
            MCU_LogShowErrorSubCode(errorCodeSub);
            return errorCodeSub;
        } 
    }
    else if(compareResult == MCU_FLASH_CMP_EQU)
    {
        //写入数据和保存数据完全相等,不需要写入
        return ERROR_SUB_OK;
    }
    else if(compareResult == MCU_FLASH_CMP_WRITE)
    {
        //直接写入不需要擦除
        //写入数据
        errorCodeSub = MCU_FlashWriteNoCheck(startAddr,bufferPtr,wordNumber);
        if(errorCodeSub != ERROR_SUB_OK)
        {
            MCU_LogShowErrorSubCode(errorCodeSub);
            return errorCodeSub;
        } 
    }
    else if(compareResult == MCU_FLASH_CMP_PARAM_ERR)
    {
        //参数错误
        return ERROR_SUB_MCU_FLASH_COMPARE_PARAM;
    }
    return ERROR_SUB_OK;
}


//MCU Flash Check
ERROR_SUB MCU_FlashCheck(void)
{
    
    uint32_t srcDat = 0;
    uint32_t writeDat = MCU_RandomGetNextUnsigned(0,0XFFFFFFFF);
    uint32_t readDat = 0;
    uint32_t testAddr = MCU_RandomGetNextUnsigned(ADDR_FLASH_SECTOR_5+4,FLASH_BASE+MCU_FLASH_BANK_MAX_SIZE -1);
    //四字节对齐
    testAddr /= 4;
    testAddr *= 4;
    //自检开始
    MCU_LogPrintf("%s Start,Addr: 0X%08X,WriteData: 0X%08X\r\n",__func__,testAddr,writeDat);
    //读取源
    srcDat = MCU_FlashReadWord(testAddr);
    //写入新的数据
    MCU_FlashWrite(testAddr,&writeDat,1);
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
    MCU_FlashWrite(testAddr,&srcDat,1);
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





