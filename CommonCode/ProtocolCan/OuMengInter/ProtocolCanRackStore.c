/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-09-11 13:33:37 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-09-12 14:25:45 +0800
************************************************************************************************/ 
#include "ProtocolCanRackStore.h"
#include "ProtocolCanOumengInterBase.h"
#include "DriverConfigSystem.h"
#include SYS_LOG_INCLUDE_FILE

#define MODULE_NO_CAN_MASTER_RACK_STORE             0X05
#define BOARD_ID_CAN_MASTER_RACK_STORE              0X02

//通道配置数组
//参数分别是 指令  是否需要结果  超时时间  期望返回最小参数长度 附带参数长度
//附带参数长度为不定长的时候,指示参数长度为0,由上级调用者手动申请内存
static const CAN_PROT_CMD_CONFIG canRackStoreMasterCmdConfigArray[] = {
    {CAN_MASTER_CMD_RACK_STORE_READ_SENSOR_SINGLE ,        ENABLE,     1000,      1,      1       },//输入单个读取
    {CAN_MASTER_CMD_RACK_STORE_READ_SENSOR_ALL    ,        ENABLE,     1000,     23,      0       },//输入区间读取
};

/*********************************************************发送指令等待ACK********************************************************************/
//读取样本架仓指定仓位单个传感器状态
void CanMasterRackStoreSendReadSingleSensorByPosWhileAck(RACK_STORE_HOUSE rackHousePos,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_ACT_RACK_STORE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_RACK_STORE_READ_SENSOR_SINGLE;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(MODULE_NO_CAN_MASTER_RACK_STORE,BOARD_ID_CAN_MASTER_RACK_STORE,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode != PROT_RESULT_FAIL)
    {
        CAN_PROT_LOG("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
	
	CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canRackStoreMasterCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canRackStoreMasterCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        CAN_PROT_LOG("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
		return;
    }
	
	//创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(MODULE_NO_CAN_MASTER_RACK_STORE,BOARD_ID_CAN_MASTER_RACK_STORE,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)rackHousePos;
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//读取样本架仓指定仓位全部传感器状态
void CanMasterRackStoreSendReadAllSensorStateWhileAck(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_ACT_RACK_STORE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_RACK_STORE_READ_SENSOR_ALL;
	CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(MODULE_NO_CAN_MASTER_RACK_STORE,BOARD_ID_CAN_MASTER_RACK_STORE,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode != PROT_RESULT_FAIL)
    {
        CAN_PROT_LOG("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
	
	CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canRackStoreMasterCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canRackStoreMasterCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        CAN_PROT_LOG("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
		return;
    }
	
	//创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(MODULE_NO_CAN_MASTER_RACK_STORE,BOARD_ID_CAN_MASTER_RACK_STORE,singalChannel,channelCmdCode,canCmdConfigPtr);
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

/*********************************************************等待指定指令完成返回********************************************************************/
//读取样本架仓指定仓位单个传感器状态
void CanMasterRackStoreWaitReadSingleSensorByPosResult(SYS_RESULT_PACK* resultPackPtr,PIN_STATE* sensorStatePtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_ACT_RACK_STORE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_RACK_STORE_READ_SENSOR_SINGLE;
	uint8_t targetModule = MODULE_NO_CAN_MASTER_RACK_STORE;
    uint8_t targetBoardID = BOARD_ID_CAN_MASTER_RACK_STORE;
	//保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
	//创建结果包
    CanProtCmdWaitResult(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
	if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,ProtocolCanWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
		//释放内存
        CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
	
	//结果解析
    if(sensorStatePtr != NULL)
    {
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
			*sensorStatePtr = (PIN_STATE)(returnDataBufferPtr[0]);
        }
    }
	//内存释放
    CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//读取样本架仓指定仓位全部传感器状态
void CanMasterRackStoreWaitReadAllSensorStateResult(SYS_RESULT_PACK* resultPackPtr,RACK_STORE_SENSOR_SET* sensorStateSetPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_ACT_RACK_STORE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_RACK_STORE_READ_SENSOR_ALL;
	uint8_t targetModule = MODULE_NO_CAN_MASTER_RACK_STORE;
    uint8_t targetBoardID = BOARD_ID_CAN_MASTER_RACK_STORE;
	//保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
	//创建结果包
    CanProtCmdWaitResult(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
	if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,ProtocolCanWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
		//释放内存
        CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
	
	//结果解析
    if(sensorStateSetPtr != NULL)
    {
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
			UserMemCopy((uint8_t*)sensorStateSetPtr,returnDataBufferPtr,DIM_STRUCT_LENGTH_BYTE(RACK_STORE_SENSOR_SET));
        }
    }
	//内存释放
    CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

/*********************************************************发送指令等待完成返回********************************************************************/
//读取样本架仓指定仓位单个传感器状态
void CanMasterRackStoreSendReadSingleSensorByPosWhileReturn(RACK_STORE_HOUSE rackHousePos,SYS_RESULT_PACK* resultPackPtr,PIN_STATE* sensorStatePtr)
{
    //发送指令等待ACK
    CanMasterRackStoreSendReadSingleSensorByPosWhileAck(rackHousePos,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterRackStoreSendReadSingleSensorByPosWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterRackStoreWaitReadSingleSensorByPosResult(resultPackPtr,sensorStatePtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterRackStoreWaitReadSingleSensorByPosResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//读取样本架仓指定仓位全部传感器状态
void CanMasterRackStoreSendReadAllSensorStateWhileReturn(SYS_RESULT_PACK* resultPackPtr,RACK_STORE_SENSOR_SET* sensorStateSetPtr)
{
    //发送指令等待ACK
   CanMasterRackStoreSendReadAllSensorStateWhileAck(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterRackStoreSendReadAllSensorStateWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterRackStoreWaitReadAllSensorStateResult(resultPackPtr,sensorStateSetPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterRackStoreWaitReadAllSensorStateResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}
