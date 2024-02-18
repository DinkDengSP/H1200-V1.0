/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-13 11:21:37 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-13 14:11:41 +0800
************************************************************************************************/ 
#include "BoardDS18B20.h"
#include "DriverConfigBoard.h"

//板上DS18B20初始化
void BoardDS18B20PortInit(void)
{
    //初始化
    MCU_PinInit(PIN_DQ_BOARD_DS18B20,MCU_PIN_MODE_OUTPUT_PP,MCU_PIN_PULL_PULLUP,MCU_PIN_SPEED_FREQ_HIGH,0);
    //写入初始化值
    MCU_PinWriteSingle(PIN_DQ_BOARD_DS18B20,GPIO_PIN_RESET);
}

//复位DS18B20 
static void BoardDS18B20Reset(void)
{
	//设置输出
	BOARD_DS18B20_IO_OUT(); 
	//拉低DQ
	BOARD_DS18B20_DQ_OUT = 0; 
	//拉低750us
	CoreDelayUs(750);    
	//DQ=1 
	BOARD_DS18B20_DQ_OUT = 1; 
	//15US
	CoreDelayUs(15);   
}	

//从DS18B20读取一个位
//返回值：1/0
static uint8_t BoardDS18B20ReadBit(void) 			
{
	uint8_t data;
	//设置输出
	BOARD_DS18B20_IO_OUT();
	BOARD_DS18B20_DQ_OUT = 0; 
	CoreDelayUs(2);
	BOARD_DS18B20_DQ_OUT = 1; 
	//设置输入
	BOARD_DS18B20_IO_IN();
	CoreDelayUs(12);
	//根据总线电平决定数据0还是1
	if(BOARD_DS18B20_DQ_IN)
		data=1;
	else 
		data=0;	 
	CoreDelayUs(50);           
	return data;
}

//从DS18B20读取一个字节
//返回值：读到的数据
static uint8_t BoardDS18B20ReadByte(void)    
{        
    uint8_t indexUtilMain,bitValue,resultDat;
    resultDat=0;
	for (indexUtilMain=1;indexUtilMain<=8;indexUtilMain++) 
	{
        bitValue=BoardDS18B20ReadBit();
        resultDat=(bitValue<<7)|(resultDat>>1);
    }						    
    return resultDat;
}

//写一个字节到DS18B20
//dat：要写入的字节
static void BoardDS18B20WriteByte(uint8_t dat)     
{             
	uint8_t indexMain;
	uint8_t bitWrite;
	//设置输出
	BOARD_DS18B20_IO_OUT();
	for (indexMain=1;indexMain<=8;indexMain++) 
	{
		bitWrite = dat&0x01;
		dat = dat>>1;
		if (bitWrite) 
		{
			// Write 1
			BOARD_DS18B20_DQ_OUT = 0;
			CoreDelayUs(2);                            
			BOARD_DS18B20_DQ_OUT = 1;
			CoreDelayUs(60);             
		}
		else 
		{
			// Write 0
			BOARD_DS18B20_DQ_OUT = 0;
			CoreDelayUs(60);             
			BOARD_DS18B20_DQ_OUT = 1;
			CoreDelayUs(2);                          
		}
	}
	return;
}


//检测存在,不调用进出临界段
static ERROR_SUB BoardDS18B20CheckExistWithOutCritical()
{
    uint8_t retry=0;
    //先发一个硬件复位
    BoardDS18B20Reset();
	//设置输入
	BOARD_DS18B20_IO_IN();	
    //等待输入低电平 0
    while (BOARD_DS18B20_DQ_IN&&retry<DS18B20_CHECK_RETRY_TIME)
	{
		retry++;
		CoreDelayUs(1);
	};	 
	if(retry>=DS18B20_CHECK_RETRY_TIME)
	{
		return ERROR_SUB_TC_DS18B20_CHECK_FAIL;
	}
	else 
    {
        retry=0;
    }
    //等待变成高电平
    while (!BOARD_DS18B20_DQ_IN&&retry<DS18B20_CHECK_RETRY_TIME)
	{
		retry++;
		CoreDelayUs(1);
	};
	if(retry>=DS18B20_CHECK_RETRY_TIME)
	{
		return ERROR_SUB_TC_DS18B20_CHECK_FAIL;
	}
	else
    {
        return ERROR_SUB_OK;
    }
}

//开始温度转换
static ERROR_SUB BoardDS18B20StartWithOutCritical(void)
{   						               
	ERROR_SUB errorCode = ERROR_SUB_OK;	   
	errorCode = BoardDS18B20CheckExistWithOutCritical();	 
    if(errorCode != ERROR_SUB_OK)
    {
        return errorCode;
    }
	//skip rom
	BoardDS18B20WriteByte(DS18B20_COMMAND_SKIP_ROM);
	//convert
	BoardDS18B20WriteByte(DS18B20_CONVERT_TEMP);
    return errorCode;
}


//检测板上DS18B20是否存在
ERROR_SUB BoardDS18B20CheckExist(void)
{
    uint8_t retry=0;
    int32_t lastIntState;
    //进入临界段
    lastIntState = CoreLockIntBasePri();
    //先发一个硬件复位
    BoardDS18B20Reset();
	//设置输入
	BOARD_DS18B20_IO_IN();	
    //等待输入低电平 0
    while (BOARD_DS18B20_DQ_IN&&retry<DS18B20_CHECK_RETRY_TIME)
	{
		retry++;
		CoreDelayUs(1);
	};	 
	if(retry>=DS18B20_CHECK_RETRY_TIME)
	{
        //退出临界段
        CoreUnlockIntBasePri(lastIntState);
		return ERROR_SUB_TC_DS18B20_CHECK_FAIL;
	}
	else 
    {
        retry=0;
    }
    //等待变成高电平
    while (!BOARD_DS18B20_DQ_IN&&retry<DS18B20_CHECK_RETRY_TIME)
	{
		retry++;
		CoreDelayUs(1);
	};
	if(retry>=DS18B20_CHECK_RETRY_TIME)
	{
        //退出临界段
        CoreUnlockIntBasePri(lastIntState);
		return ERROR_SUB_TC_DS18B20_CHECK_FAIL;
	}
	else
    {
        //退出临界段
        CoreUnlockIntBasePri(lastIntState);
        return ERROR_SUB_OK;
    }
}

//读取DS18B20当前温度
ERROR_SUB BoardDS18B20ReadCurrent(float* curentTemp)
{
    uint8_t temp;
	uint8_t TL,TH;
	short tem;
    int32_t lastIntState;
    ERROR_SUB errorCode = ERROR_SUB_OK;	  
    //进入临界段
    lastIntState = CoreLockIntBasePri(); 
	//开始转换
	errorCode = BoardDS18B20StartWithOutCritical(); 
    if(errorCode != ERROR_SUB_OK)
    {
        //退出临界段
        CoreUnlockIntBasePri(lastIntState);
        return errorCode;
    }   
    //等待转换完成               
	BoardDS18B20Reset();
    //检测转换完成
	BoardDS18B20CheckExistWithOutCritical();	 
    //跳过
	BoardDS18B20WriteByte(DS18B20_COMMAND_SKIP_ROM);
    //读取转换
	BoardDS18B20WriteByte(DS18B20_READ_SCRATCHPAD);	    
    // LSB
	TL=BoardDS18B20ReadByte();   
    // MSB
	TH=BoardDS18B20ReadByte(); 
    //退出临界段
    CoreUnlockIntBasePri(lastIntState);
	if(TH>7)
	{
		TH=~TH;
		TL=~TL; 
		temp=0;//温度为负  
	}
	else 
    {
        temp=1;//温度为正	
    }
	//获得高八位	  	  
	tem=TH; 
	tem<<=8;  
    //获得低八位  
	tem+=TL;
    //转换     
	tem=(short)((double)tem*0.625f);
	if(temp)
    {
        *curentTemp = tem;
        (*curentTemp) /= 10.0f;
    }
	else 
    {
        *curentTemp = -tem;
        (*curentTemp) /= 10.0f;
    }
	if((*curentTemp) > 80.0f)
	{
		//板上温度超上限
		return ERROR_SUB_TC_DS18B20_READ_TEMP_LIMIT_HIGH;
	}
	else if((*curentTemp) < -20.0f)
	{
		//板上温度超下限
		return ERROR_SUB_TC_DS18B20_READ_TEMP_LIMIT_LOW;
	}
    return ERROR_SUB_OK;
}








