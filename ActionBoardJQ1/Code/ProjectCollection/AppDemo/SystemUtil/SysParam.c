/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-29 14:07:30 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-09-22 10:13:18 +0800
************************************************************************************************/ 
#include "SysParam.h"
#include "DriverConfigSystem.h"
#include "DriverHeaderSystem.h"
#include "SrvTaskHeader.h"

SYS_PARAM* sysParamPtr = NULL;

//App写入单个参数的指针
typedef ERROR_SUB (*SysParamWriteParamSingleFuncPtr)(uint8_t subIndex,int32_t writeParam);
//App读取单个参数的指针
typedef ERROR_SUB (*SysParamReadParamSingleFuncPtr)(uint8_t subIndex,int32_t* readParam);

//初始化系统参数为默认参数
static void SysParamSetDefault(void);

//初始化系统参数
void SysParamInit(void)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //首先申请内存
    sysParamPtr = NULL;
    //申请内存
    sysParamPtr = UserMemMallocWhileSuccess(MEM_REGION_SYS_PARAM,LENGTH_SYS_PARAM);
    //从Flash中加载参数
    errorSub = BOARD_PARAM_READ(ADDR_BOARD_SYS_PARAM_START,(uint8_t*)sysParamPtr,LENGTH_SYS_PARAM);
    if(errorSub != ERROR_SUB_OK)
    {
        SystemPrintf("%s,BOARD_PARAM_READ Failed: 0X%08X\r\n",__func__,errorSub);
        while(1);
    }
    //数据校验
    if(sysParamPtr->paramFlag == SYS_PARAM_DEFAULT_FLAG)
    {
        //校验通过
        return;
    }
    //校验不通过,设定为默认配置
    SysParamSetDefault();
}


//初始化系统参数为默认参数
static void SysParamSetDefault(void)
{
    //设定为默认配置
    sysParamPtr->paramFlag = SYS_PARAM_DEFAULT_FLAG;
    //直线电机坐标参数集合
    UserMemCopy((void*)(&(sysParamPtr->laterallyMotorParam)),(void*)(&laterallyMotorParamDefault),LENGTH_SYS_PARAM_LATERALLY_MOTOR);
    //升降电机坐标参数集合
    UserMemCopy((void*)(&(sysParamPtr->updownMotorParam)),(void*)(&updownMotorParamDefault),LENGTH_SYS_PARAM_UPDOWN_MOTOR);
    //顶杆电机坐标参数集合
    UserMemCopy((void*)(&(sysParamPtr->rodMotorParam)),(void*)(&rodMotorParamDefault),LENGTH_SYS_PARAM_ROD_MOTOR);
    //皮带电机坐标参数集合
    UserMemCopy((void*)(&(sysParamPtr->beltMotorParam)),(void*)(&beltMotorParamDefault),LENGTH_SYS_PARAM_BELT_MOTOR);
    //辅助参数默认参数
    UserMemCopy((void*)(&(sysParamPtr->utilParam)),(void*)(&utilParamDefault),LENGTH_SYS_PARAM_UTIL);
    //将设定好的值写回外置Flash
    BOARD_PARAM_WRITE(ADDR_BOARD_SYS_PARAM_START,(uint8_t*)sysParamPtr,LENGTH_SYS_PARAM);
}

//通用的写入Flash的函数
static void SysParamWriteSingleCommon(int32_t* targetParamPtr,int32_t writeParam)
{
    *targetParamPtr = writeParam;
    int32_t targetAddr = (int32_t)((char*)targetParamPtr);
    int32_t baseAddr = (int32_t)((char*)(sysParamPtr));
    //计算偏移
    int32_t offset = targetAddr - baseAddr;
    if(offset < 0)
    {
        //偏移必须是正数
        offset = 0 - offset;
    }
    //写入参数
    BOARD_PARAM_WRITE(ADDR_BOARD_SYS_PARAM_START+offset,(uint8_t*)targetParamPtr,4);
}

/*----------------------------------------------------------------------------------------------------*/
//读写直线电机参数
ERROR_SUB SysParamReadParamLaterallyMotorParam(uint8_t subIndex,int32_t* readParam)
{
    switch(subIndex)
    {
        case INDEX_SUB_PARAM_LATERALLY_MOTOR_RESET_CORRECT:
            *readParam = sysParamPtr->laterallyMotorParam.resetCorrect;
            break;
        case INDEX_SUB_PARAM_LATERALLY_MOTOR_DIR_SWITCH_CORRECT:
            *readParam = sysParamPtr->laterallyMotorParam.dirSwitchCorrect;
            break;
        case INDEX_SUB_PARAM_LATERALLY_MOTOR_IDLE_POS:
            *readParam = sysParamPtr->laterallyMotorParam.idlePos;
            break;
        case INDEX_SUB_PARAM_LATERALLY_MOTOR_RACK_STORE1_POS:
            *readParam = sysParamPtr->laterallyMotorParam.rackStore1Pos;
            break;
        case INDEX_SUB_PARAM_LATERALLY_MOTOR_RACK_STORE2_POS:
            *readParam = sysParamPtr->laterallyMotorParam.rackStore2Pos;
            break;
        case INDEX_SUB_PARAM_LATERALLY_MOTOR_RACK_STORE3_POS:
            *readParam = sysParamPtr->laterallyMotorParam.rackStore3Pos;
            break;
        case INDEX_SUB_PARAM_LATERALLY_MOTOR_RACK_STORE4_POS:
            *readParam = sysParamPtr->laterallyMotorParam.rackStore4Pos;
            break;
        case INDEX_SUB_PARAM_LATERALLY_MOTOR_RACK_STORE5_POS:
            *readParam = sysParamPtr->laterallyMotorParam.rackStore5Pos;
            break;
        case INDEX_SUB_PARAM_LATERALLY_MOTOR_RACK_STORE6_POS:
            *readParam = sysParamPtr->laterallyMotorParam.rackStore6Pos;
            break;
        case INDEX_SUB_PARAM_LATERALLY_MOTOR_RACK_STORE7_POS:
            *readParam = sysParamPtr->laterallyMotorParam.rackStore7Pos;
            break;
        case INDEX_SUB_PARAM_LATERALLY_MOTOR_RACK_STORE8_POS:
            *readParam = sysParamPtr->laterallyMotorParam.rackStore8Pos;
            break;
        case INDEX_SUB_PARAM_LATERALLY_MOTOR_RACK_STORE9_POS:
            *readParam = sysParamPtr->laterallyMotorParam.rackStore9Pos;
            break;
        case INDEX_SUB_PARAM_LATERALLY_MOTOR_RACK_STORE10_POS:
            *readParam = sysParamPtr->laterallyMotorParam.rackStore10Pos;
            break;
        case INDEX_SUB_PARAM_LATERALLY_MOTOR_RACK_STORE11_POS:
            *readParam = sysParamPtr->laterallyMotorParam.rackStore11Pos;
            break;
        case INDEX_SUB_PARAM_LATERALLY_MOTOR_RACK_STORE12_POS:
            *readParam = sysParamPtr->laterallyMotorParam.rackStore12Pos;
            break;
        case INDEX_SUB_PARAM_LATERALLY_MOTOR_RACK_STORE13_POS:
            *readParam = sysParamPtr->laterallyMotorParam.rackStore13Pos;
            break;
        case INDEX_SUB_PARAM_LATERALLY_MOTOR_RACK_STORE14_POS:
            *readParam = sysParamPtr->laterallyMotorParam.rackStore14Pos;
            break;
        case INDEX_SUB_PARAM_LATERALLY_MOTOR_RACK_STORE15_POS:
            *readParam = sysParamPtr->laterallyMotorParam.rackStore15Pos;
            break;
        case INDEX_SUB_PARAM_LATERALLY_MOTOR_RACK_STORE16_POS:
            *readParam = sysParamPtr->laterallyMotorParam.rackStore16Pos;
            break;
        case INDEX_SUB_PARAM_LATERALLY_MOTOR_RACK_STORE17_POS:
            *readParam = sysParamPtr->laterallyMotorParam.rackStore17Pos;
            break;
        case INDEX_SUB_PARAM_LATERALLY_MOTOR_RACK_STORE18_POS:
            *readParam = sysParamPtr->laterallyMotorParam.rackStore18Pos;
            break;
        case INDEX_SUB_PARAM_LATERALLY_MOTOR_RACK_STORE19_POS:
            *readParam = sysParamPtr->laterallyMotorParam.rackStore19Pos;
            break;
        case INDEX_SUB_PARAM_LATERALLY_MOTOR_RACK_STORE20_POS:
            *readParam = sysParamPtr->laterallyMotorParam.rackStore20Pos;
            break;
        case INDEX_SUB_PARAM_LATERALLY_MOTOR_RACK_STORE21_POS:
            *readParam = sysParamPtr->laterallyMotorParam.rackStore21Pos;
            break;
        case INDEX_SUB_PARAM_LATERALLY_MOTOR_RACK_STORE22_POS:
            *readParam = sysParamPtr->laterallyMotorParam.rackStore22Pos;
            break;
        case INDEX_SUB_PARAM_LATERALLY_MOTOR_RACK_STORE23_POS:
            *readParam = sysParamPtr->laterallyMotorParam.rackStore23Pos;
            break;
        case INDEX_SUB_PARAM_LATERALLY_MOTOR_C21_INTER_POS:
            *readParam = sysParamPtr->laterallyMotorParam.c21InterPos;
            break;
        default://不支持的参数
            *readParam = 0;
            return ERROR_SUB_SYS_PARAM_READ_SUB_INDEX;
    }
    //返回结果
    return ERROR_SUB_OK;
}

ERROR_SUB SysParamWriteParamLaterallyMotorParam(uint8_t subIndex,int32_t writeParam)
{
    int32_t* targetParamPtr = NULL;
    switch(subIndex)
    {
        case INDEX_SUB_PARAM_LATERALLY_MOTOR_RESET_CORRECT:
            targetParamPtr = &(sysParamPtr->laterallyMotorParam.resetCorrect);
            break;
        case INDEX_SUB_PARAM_LATERALLY_MOTOR_DIR_SWITCH_CORRECT:
            targetParamPtr = &(sysParamPtr->laterallyMotorParam.dirSwitchCorrect);
            break;
        case INDEX_SUB_PARAM_LATERALLY_MOTOR_IDLE_POS:
            targetParamPtr = &(sysParamPtr->laterallyMotorParam.idlePos);
            break;
        case INDEX_SUB_PARAM_LATERALLY_MOTOR_RACK_STORE1_POS:
            targetParamPtr = &(sysParamPtr->laterallyMotorParam.rackStore1Pos);
            break;
        case INDEX_SUB_PARAM_LATERALLY_MOTOR_RACK_STORE2_POS:
            targetParamPtr = &(sysParamPtr->laterallyMotorParam.rackStore2Pos);
            break;
        case INDEX_SUB_PARAM_LATERALLY_MOTOR_RACK_STORE3_POS:
            targetParamPtr = &(sysParamPtr->laterallyMotorParam.rackStore3Pos);
            break;
        case INDEX_SUB_PARAM_LATERALLY_MOTOR_RACK_STORE4_POS:
            targetParamPtr = &(sysParamPtr->laterallyMotorParam.rackStore4Pos);
            break;
        case INDEX_SUB_PARAM_LATERALLY_MOTOR_RACK_STORE5_POS:
            targetParamPtr = &(sysParamPtr->laterallyMotorParam.rackStore5Pos);
            break;
        case INDEX_SUB_PARAM_LATERALLY_MOTOR_RACK_STORE6_POS:
            targetParamPtr = &(sysParamPtr->laterallyMotorParam.rackStore6Pos);
            break;
        case INDEX_SUB_PARAM_LATERALLY_MOTOR_RACK_STORE7_POS:
            targetParamPtr = &(sysParamPtr->laterallyMotorParam.rackStore7Pos);
            break;
        case INDEX_SUB_PARAM_LATERALLY_MOTOR_RACK_STORE8_POS:
            targetParamPtr = &(sysParamPtr->laterallyMotorParam.rackStore8Pos);
            break;
        case INDEX_SUB_PARAM_LATERALLY_MOTOR_RACK_STORE9_POS:
            targetParamPtr = &(sysParamPtr->laterallyMotorParam.rackStore9Pos);
            break;
        case INDEX_SUB_PARAM_LATERALLY_MOTOR_RACK_STORE10_POS:
            targetParamPtr = &(sysParamPtr->laterallyMotorParam.rackStore10Pos);
            break;
        case INDEX_SUB_PARAM_LATERALLY_MOTOR_RACK_STORE11_POS:
            targetParamPtr = &(sysParamPtr->laterallyMotorParam.rackStore11Pos);
            break;
        case INDEX_SUB_PARAM_LATERALLY_MOTOR_RACK_STORE12_POS:
            targetParamPtr = &(sysParamPtr->laterallyMotorParam.rackStore12Pos);
            break;
        case INDEX_SUB_PARAM_LATERALLY_MOTOR_RACK_STORE13_POS:
            targetParamPtr = &(sysParamPtr->laterallyMotorParam.rackStore13Pos);
            break;
        case INDEX_SUB_PARAM_LATERALLY_MOTOR_RACK_STORE14_POS:
            targetParamPtr = &(sysParamPtr->laterallyMotorParam.rackStore14Pos);
            break;
        case INDEX_SUB_PARAM_LATERALLY_MOTOR_RACK_STORE15_POS:
            targetParamPtr = &(sysParamPtr->laterallyMotorParam.rackStore15Pos);
            break;
        case INDEX_SUB_PARAM_LATERALLY_MOTOR_RACK_STORE16_POS:
            targetParamPtr = &(sysParamPtr->laterallyMotorParam.rackStore16Pos);
            break;
        case INDEX_SUB_PARAM_LATERALLY_MOTOR_RACK_STORE17_POS:
            targetParamPtr = &(sysParamPtr->laterallyMotorParam.rackStore17Pos);
            break;
        case INDEX_SUB_PARAM_LATERALLY_MOTOR_RACK_STORE18_POS:
            targetParamPtr = &(sysParamPtr->laterallyMotorParam.rackStore18Pos);
            break;
        case INDEX_SUB_PARAM_LATERALLY_MOTOR_RACK_STORE19_POS:
            targetParamPtr = &(sysParamPtr->laterallyMotorParam.rackStore19Pos);
            break;
        case INDEX_SUB_PARAM_LATERALLY_MOTOR_RACK_STORE20_POS:
            targetParamPtr = &(sysParamPtr->laterallyMotorParam.rackStore20Pos);
            break;
        case INDEX_SUB_PARAM_LATERALLY_MOTOR_RACK_STORE21_POS:
            targetParamPtr = &(sysParamPtr->laterallyMotorParam.rackStore21Pos);
            break;
        case INDEX_SUB_PARAM_LATERALLY_MOTOR_RACK_STORE22_POS:
            targetParamPtr = &(sysParamPtr->laterallyMotorParam.rackStore22Pos);
            break;
        case INDEX_SUB_PARAM_LATERALLY_MOTOR_RACK_STORE23_POS:
            targetParamPtr = &(sysParamPtr->laterallyMotorParam.rackStore23Pos);
            break;
        case INDEX_SUB_PARAM_LATERALLY_MOTOR_C21_INTER_POS:
            targetParamPtr = &(sysParamPtr->laterallyMotorParam.c21InterPos);
            break;
        default:
            return ERROR_SUB_SYS_PARAM_WRITE_SUB_INDEX;//不支持的参数
    }
    //参数写入Flash
    SysParamWriteSingleCommon(targetParamPtr,writeParam);
    //返回结果
    return ERROR_SUB_OK;
}
/*----------------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------------*/
//读写升降电机参数
ERROR_SUB SysParamReadParamUpdownMotorParam(uint8_t subIndex,int32_t* readParam)
{
    switch(subIndex)
    {
        case INDEX_SUB_PARAM_UPDOWN_MOTOR_RESET_CORRECT:
            *readParam = sysParamPtr->updownMotorParam.resetCorrect;
            break;
        case INDEX_SUB_PARAM_UPDOWN_MOTOR_DIR_SWITCH_CORRECT:
            *readParam = sysParamPtr->updownMotorParam.dirSwitchCorrect;
            break;
        case INDEX_SUB_PARAM_UPDOWN_MOTOR_IDLE_POS:
            *readParam = sysParamPtr->updownMotorParam.idlePos;
            break;
        case INDEX_SUB_PARAM_UPDOWN_MOTOR_LOAD_RACK_STORE1_POS:
            *readParam = sysParamPtr->updownMotorParam.loadRackStore1Pos;
            break;
        case INDEX_SUB_PARAM_UPDOWN_MOTOR_UNLOAD_RACK_STORE1_POS:
            *readParam = sysParamPtr->updownMotorParam.unloadRackStore1Pos;
            break;
        case INDEX_SUB_PARAM_UPDOWN_MOTOR_LOAD_RACK_STORE2_POS:
            *readParam = sysParamPtr->updownMotorParam.loadRackStore2Pos;
            break;
        case INDEX_SUB_PARAM_UPDOWN_MOTOR_UNLOAD_RACK_STORE2_POS:
            *readParam = sysParamPtr->updownMotorParam.unloadRackStore2Pos;
            break;
        case INDEX_SUB_PARAM_UPDOWN_MOTOR_LOAD_RACK_STORE3_POS:
            *readParam = sysParamPtr->updownMotorParam.loadRackStore3Pos;
            break;
        case INDEX_SUB_PARAM_UPDOWN_MOTOR_UNLOAD_RACK_STORE3_POS:
            *readParam = sysParamPtr->updownMotorParam.unloadRackStore3Pos;
            break;
        case INDEX_SUB_PARAM_UPDOWN_MOTOR_LOAD_RACK_STORE4_POS:
            *readParam = sysParamPtr->updownMotorParam.loadRackStore4Pos;
            break;
        case INDEX_SUB_PARAM_UPDOWN_MOTOR_UNLOAD_RACK_STORE4_POS:
            *readParam = sysParamPtr->updownMotorParam.unloadRackStore4Pos;
            break;
        case INDEX_SUB_PARAM_UPDOWN_MOTOR_LOAD_RACK_STORE5_POS:
            *readParam = sysParamPtr->updownMotorParam.loadRackStore5Pos;
            break;
        case INDEX_SUB_PARAM_UPDOWN_MOTOR_UNLOAD_RACK_STORE5_POS:
            *readParam = sysParamPtr->updownMotorParam.unloadRackStore5Pos;
            break;
        case INDEX_SUB_PARAM_UPDOWN_MOTOR_LOAD_RACK_STORE6_POS:
            *readParam = sysParamPtr->updownMotorParam.loadRackStore6Pos;
            break;
        case INDEX_SUB_PARAM_UPDOWN_MOTOR_UNLOAD_RACK_STORE6_POS:
            *readParam = sysParamPtr->updownMotorParam.unloadRackStore6Pos;
            break;
        case INDEX_SUB_PARAM_UPDOWN_MOTOR_LOAD_RACK_STORE7_POS:
            *readParam = sysParamPtr->updownMotorParam.loadRackStore7Pos;
            break;
        case INDEX_SUB_PARAM_UPDOWN_MOTOR_UNLOAD_RACK_STORE7_POS:
            *readParam = sysParamPtr->updownMotorParam.unloadRackStore7Pos;
            break;
        case INDEX_SUB_PARAM_UPDOWN_MOTOR_LOAD_RACK_STORE8_POS:
            *readParam = sysParamPtr->updownMotorParam.loadRackStore8Pos;
            break;
        case INDEX_SUB_PARAM_UPDOWN_MOTOR_UNLOAD_RACK_STORE8_POS:
            *readParam = sysParamPtr->updownMotorParam.unloadRackStore8Pos;
            break;
        case INDEX_SUB_PARAM_UPDOWN_MOTOR_LOAD_RACK_STORE9_POS:
            *readParam = sysParamPtr->updownMotorParam.loadRackStore9Pos;
            break;
        case INDEX_SUB_PARAM_UPDOWN_MOTOR_UNLOAD_RACK_STORE9_POS:
            *readParam = sysParamPtr->updownMotorParam.unloadRackStore9Pos;
            break;
        case INDEX_SUB_PARAM_UPDOWN_MOTOR_LOAD_RACK_STORE10_POS:
            *readParam = sysParamPtr->updownMotorParam.loadRackStore10Pos;
            break;
        case INDEX_SUB_PARAM_UPDOWN_MOTOR_UNLOAD_RACK_STORE10_POS:
            *readParam = sysParamPtr->updownMotorParam.unloadRackStore10Pos;
            break;
        case INDEX_SUB_PARAM_UPDOWN_MOTOR_LOAD_RACK_STORE11_POS:
            *readParam = sysParamPtr->updownMotorParam.loadRackStore11Pos;
            break;
        case INDEX_SUB_PARAM_UPDOWN_MOTOR_UNLOAD_RACK_STORE11_POS:
            *readParam = sysParamPtr->updownMotorParam.unloadRackStore11Pos;
            break;
        case INDEX_SUB_PARAM_UPDOWN_MOTOR_LOAD_RACK_STORE12_POS:
            *readParam = sysParamPtr->updownMotorParam.loadRackStore12Pos;
            break;
        case INDEX_SUB_PARAM_UPDOWN_MOTOR_UNLOAD_RACK_STORE12_POS:
            *readParam = sysParamPtr->updownMotorParam.unloadRackStore12Pos;
            break;
        case INDEX_SUB_PARAM_UPDOWN_MOTOR_LOAD_RACK_STORE13_POS:
            *readParam = sysParamPtr->updownMotorParam.loadRackStore13Pos;
            break;
        case INDEX_SUB_PARAM_UPDOWN_MOTOR_UNLOAD_RACK_STORE13_POS:
            *readParam = sysParamPtr->updownMotorParam.unloadRackStore13Pos;
            break;
        case INDEX_SUB_PARAM_UPDOWN_MOTOR_LOAD_RACK_STORE14_POS:
            *readParam = sysParamPtr->updownMotorParam.loadRackStore14Pos;
            break;
        case INDEX_SUB_PARAM_UPDOWN_MOTOR_UNLOAD_RACK_STORE14_POS:
            *readParam = sysParamPtr->updownMotorParam.unloadRackStore14Pos;
            break;
        case INDEX_SUB_PARAM_UPDOWN_MOTOR_LOAD_RACK_STORE15_POS:
            *readParam = sysParamPtr->updownMotorParam.loadRackStore15Pos;
            break;
        case INDEX_SUB_PARAM_UPDOWN_MOTOR_UNLOAD_RACK_STORE15_POS:
            *readParam = sysParamPtr->updownMotorParam.unloadRackStore15Pos;
            break;
        case INDEX_SUB_PARAM_UPDOWN_MOTOR_LOAD_RACK_STORE16_POS:
            *readParam = sysParamPtr->updownMotorParam.loadRackStore16Pos;
            break;
        case INDEX_SUB_PARAM_UPDOWN_MOTOR_UNLOAD_RACK_STORE16_POS:
            *readParam = sysParamPtr->updownMotorParam.unloadRackStore16Pos;
            break;
        case INDEX_SUB_PARAM_UPDOWN_MOTOR_LOAD_RACK_STORE17_POS:
            *readParam = sysParamPtr->updownMotorParam.loadRackStore17Pos;
            break;
        case INDEX_SUB_PARAM_UPDOWN_MOTOR_UNLOAD_RACK_STORE17_POS:
            *readParam = sysParamPtr->updownMotorParam.unloadRackStore17Pos;
            break;
        case INDEX_SUB_PARAM_UPDOWN_MOTOR_LOAD_RACK_STORE18_POS:
            *readParam = sysParamPtr->updownMotorParam.loadRackStore18Pos;
            break;
        case INDEX_SUB_PARAM_UPDOWN_MOTOR_UNLOAD_RACK_STORE18_POS:
            *readParam = sysParamPtr->updownMotorParam.unloadRackStore18Pos;
            break;
        case INDEX_SUB_PARAM_UPDOWN_MOTOR_LOAD_RACK_STORE19_POS:
            *readParam = sysParamPtr->updownMotorParam.loadRackStore19Pos;
            break;
        case INDEX_SUB_PARAM_UPDOWN_MOTOR_UNLOAD_RACK_STORE19_POS:
            *readParam = sysParamPtr->updownMotorParam.unloadRackStore19Pos;
            break;
        case INDEX_SUB_PARAM_UPDOWN_MOTOR_LOAD_RACK_STORE20_POS:
            *readParam = sysParamPtr->updownMotorParam.loadRackStore20Pos;
            break;
        case INDEX_SUB_PARAM_UPDOWN_MOTOR_UNLOAD_RACK_STORE20_POS:
            *readParam = sysParamPtr->updownMotorParam.unloadRackStore20Pos;
            break;
        case INDEX_SUB_PARAM_UPDOWN_MOTOR_LOAD_RACK_STORE21_POS:
            *readParam = sysParamPtr->updownMotorParam.loadRackStore21Pos;
            break;
        case INDEX_SUB_PARAM_UPDOWN_MOTOR_UNLOAD_RACK_STORE21_POS:
            *readParam = sysParamPtr->updownMotorParam.unloadRackStore21Pos;
            break;
        case INDEX_SUB_PARAM_UPDOWN_MOTOR_LOAD_RACK_STORE22_POS:
            *readParam = sysParamPtr->updownMotorParam.loadRackStore22Pos;
            break;
        case INDEX_SUB_PARAM_UPDOWN_MOTOR_UNLOAD_RACK_STORE22_POS:
            *readParam = sysParamPtr->updownMotorParam.unloadRackStore22Pos;
            break;
        case INDEX_SUB_PARAM_UPDOWN_MOTOR_LOAD_RACK_STORE23_POS:
            *readParam = sysParamPtr->updownMotorParam.loadRackStore23Pos;
            break;
        case INDEX_SUB_PARAM_UPDOWN_MOTOR_UNLOAD_RACK_STORE23_POS:
            *readParam = sysParamPtr->updownMotorParam.unloadRackStore23Pos;
            break;
        case INDEX_SUB_PARAM_UPDOWN_MOTOR_LOADC21_POS:
            *readParam = sysParamPtr->updownMotorParam.loadC21Pos;
            break;
        case INDEX_SUB_PARAM_UPDOWN_MOTOR_UNLOADC21_POS:
            *readParam = sysParamPtr->updownMotorParam.unloadC21Pos;
            break;
        default://不支持的参数
            *readParam = 0;
            return ERROR_SUB_SYS_PARAM_READ_SUB_INDEX;
    }
    //返回结果
    return ERROR_SUB_OK;
}

ERROR_SUB SysParamWriteParamUpdownMotorParam(uint8_t subIndex,int32_t writeParam)
{
    int32_t* targetParamPtr = NULL;
    switch(subIndex)
    {
        case INDEX_SUB_PARAM_UPDOWN_MOTOR_RESET_CORRECT:
            targetParamPtr = &(sysParamPtr->updownMotorParam.resetCorrect);
            break;
        case INDEX_SUB_PARAM_UPDOWN_MOTOR_DIR_SWITCH_CORRECT:
            targetParamPtr = &(sysParamPtr->updownMotorParam.dirSwitchCorrect);
            break;
        case INDEX_SUB_PARAM_UPDOWN_MOTOR_IDLE_POS:
            targetParamPtr = &(sysParamPtr->updownMotorParam.idlePos);
            break;
        case INDEX_SUB_PARAM_UPDOWN_MOTOR_LOAD_RACK_STORE1_POS:
            targetParamPtr = &(sysParamPtr->updownMotorParam.loadRackStore1Pos);
            break;
        case INDEX_SUB_PARAM_UPDOWN_MOTOR_UNLOAD_RACK_STORE1_POS:
            targetParamPtr = &(sysParamPtr->updownMotorParam.unloadRackStore1Pos);
            break;
        case INDEX_SUB_PARAM_UPDOWN_MOTOR_LOAD_RACK_STORE2_POS:
            targetParamPtr = &(sysParamPtr->updownMotorParam.loadRackStore2Pos);
            break;
        case INDEX_SUB_PARAM_UPDOWN_MOTOR_UNLOAD_RACK_STORE2_POS:
            targetParamPtr = &(sysParamPtr->updownMotorParam.unloadRackStore2Pos);
            break;
        case INDEX_SUB_PARAM_UPDOWN_MOTOR_LOAD_RACK_STORE3_POS:
            targetParamPtr = &(sysParamPtr->updownMotorParam.loadRackStore3Pos);
            break;
        case INDEX_SUB_PARAM_UPDOWN_MOTOR_UNLOAD_RACK_STORE3_POS:
            targetParamPtr = &(sysParamPtr->updownMotorParam.unloadRackStore3Pos);
            break;
        case INDEX_SUB_PARAM_UPDOWN_MOTOR_LOAD_RACK_STORE4_POS:
            targetParamPtr = &(sysParamPtr->updownMotorParam.loadRackStore4Pos);
            break;
        case INDEX_SUB_PARAM_UPDOWN_MOTOR_UNLOAD_RACK_STORE4_POS:
            targetParamPtr = &(sysParamPtr->updownMotorParam.unloadRackStore4Pos);
            break;
        case INDEX_SUB_PARAM_UPDOWN_MOTOR_LOAD_RACK_STORE5_POS:
            targetParamPtr = &(sysParamPtr->updownMotorParam.loadRackStore5Pos);
            break;
        case INDEX_SUB_PARAM_UPDOWN_MOTOR_UNLOAD_RACK_STORE5_POS:
            targetParamPtr = &(sysParamPtr->updownMotorParam.unloadRackStore5Pos);
            break;
        case INDEX_SUB_PARAM_UPDOWN_MOTOR_LOAD_RACK_STORE6_POS:
            targetParamPtr = &(sysParamPtr->updownMotorParam.loadRackStore6Pos);
            break;
        case INDEX_SUB_PARAM_UPDOWN_MOTOR_UNLOAD_RACK_STORE6_POS:
            targetParamPtr = &(sysParamPtr->updownMotorParam.unloadRackStore6Pos);
            break;
        case INDEX_SUB_PARAM_UPDOWN_MOTOR_LOAD_RACK_STORE7_POS:
            targetParamPtr = &(sysParamPtr->updownMotorParam.loadRackStore7Pos);
            break;
        case INDEX_SUB_PARAM_UPDOWN_MOTOR_UNLOAD_RACK_STORE7_POS:
            targetParamPtr = &(sysParamPtr->updownMotorParam.unloadRackStore7Pos);
            break;
        case INDEX_SUB_PARAM_UPDOWN_MOTOR_LOAD_RACK_STORE8_POS:
            targetParamPtr = &(sysParamPtr->updownMotorParam.loadRackStore8Pos);
            break;
        case INDEX_SUB_PARAM_UPDOWN_MOTOR_UNLOAD_RACK_STORE8_POS:
            targetParamPtr = &(sysParamPtr->updownMotorParam.unloadRackStore8Pos);
            break;
        case INDEX_SUB_PARAM_UPDOWN_MOTOR_LOAD_RACK_STORE9_POS:
            targetParamPtr = &(sysParamPtr->updownMotorParam.loadRackStore9Pos);
            break;
        case INDEX_SUB_PARAM_UPDOWN_MOTOR_UNLOAD_RACK_STORE9_POS:
            targetParamPtr = &(sysParamPtr->updownMotorParam.unloadRackStore9Pos);
            break;
        case INDEX_SUB_PARAM_UPDOWN_MOTOR_LOAD_RACK_STORE10_POS:
            targetParamPtr = &(sysParamPtr->updownMotorParam.loadRackStore10Pos);
            break;
        case INDEX_SUB_PARAM_UPDOWN_MOTOR_UNLOAD_RACK_STORE10_POS:
            targetParamPtr = &(sysParamPtr->updownMotorParam.unloadRackStore10Pos);
            break;
        case INDEX_SUB_PARAM_UPDOWN_MOTOR_LOAD_RACK_STORE11_POS:
            targetParamPtr = &(sysParamPtr->updownMotorParam.loadRackStore11Pos);
            break;
        case INDEX_SUB_PARAM_UPDOWN_MOTOR_UNLOAD_RACK_STORE11_POS:
            targetParamPtr = &(sysParamPtr->updownMotorParam.unloadRackStore11Pos);
            break;
        case INDEX_SUB_PARAM_UPDOWN_MOTOR_LOAD_RACK_STORE12_POS:
            targetParamPtr = &(sysParamPtr->updownMotorParam.loadRackStore12Pos);
            break;
        case INDEX_SUB_PARAM_UPDOWN_MOTOR_UNLOAD_RACK_STORE12_POS:
            targetParamPtr = &(sysParamPtr->updownMotorParam.unloadRackStore12Pos);
            break;
        case INDEX_SUB_PARAM_UPDOWN_MOTOR_LOAD_RACK_STORE13_POS:
            targetParamPtr = &(sysParamPtr->updownMotorParam.loadRackStore13Pos);
            break;
        case INDEX_SUB_PARAM_UPDOWN_MOTOR_UNLOAD_RACK_STORE13_POS:
            targetParamPtr = &(sysParamPtr->updownMotorParam.unloadRackStore13Pos);
            break;
        case INDEX_SUB_PARAM_UPDOWN_MOTOR_LOAD_RACK_STORE14_POS:
            targetParamPtr = &(sysParamPtr->updownMotorParam.loadRackStore14Pos);
            break;
        case INDEX_SUB_PARAM_UPDOWN_MOTOR_UNLOAD_RACK_STORE14_POS:
            targetParamPtr = &(sysParamPtr->updownMotorParam.unloadRackStore14Pos);
            break;
        case INDEX_SUB_PARAM_UPDOWN_MOTOR_LOAD_RACK_STORE15_POS:
            targetParamPtr = &(sysParamPtr->updownMotorParam.loadRackStore15Pos);
            break;
        case INDEX_SUB_PARAM_UPDOWN_MOTOR_UNLOAD_RACK_STORE15_POS:
            targetParamPtr = &(sysParamPtr->updownMotorParam.unloadRackStore15Pos);
            break;
        case INDEX_SUB_PARAM_UPDOWN_MOTOR_LOAD_RACK_STORE16_POS:
            targetParamPtr = &(sysParamPtr->updownMotorParam.loadRackStore16Pos);
            break;
        case INDEX_SUB_PARAM_UPDOWN_MOTOR_UNLOAD_RACK_STORE16_POS:
            targetParamPtr = &(sysParamPtr->updownMotorParam.unloadRackStore16Pos);
            break;
        case INDEX_SUB_PARAM_UPDOWN_MOTOR_LOAD_RACK_STORE17_POS:
            targetParamPtr = &(sysParamPtr->updownMotorParam.loadRackStore17Pos);
            break;
        case INDEX_SUB_PARAM_UPDOWN_MOTOR_UNLOAD_RACK_STORE17_POS:
            targetParamPtr = &(sysParamPtr->updownMotorParam.unloadRackStore17Pos);
            break;
        case INDEX_SUB_PARAM_UPDOWN_MOTOR_LOAD_RACK_STORE18_POS:
            targetParamPtr = &(sysParamPtr->updownMotorParam.loadRackStore18Pos);
            break;
        case INDEX_SUB_PARAM_UPDOWN_MOTOR_UNLOAD_RACK_STORE18_POS:
            targetParamPtr = &(sysParamPtr->updownMotorParam.unloadRackStore18Pos);
            break;
        case INDEX_SUB_PARAM_UPDOWN_MOTOR_LOAD_RACK_STORE19_POS:
            targetParamPtr = &(sysParamPtr->updownMotorParam.loadRackStore19Pos);
            break;
        case INDEX_SUB_PARAM_UPDOWN_MOTOR_UNLOAD_RACK_STORE19_POS:
            targetParamPtr = &(sysParamPtr->updownMotorParam.unloadRackStore19Pos);
            break;
        case INDEX_SUB_PARAM_UPDOWN_MOTOR_LOAD_RACK_STORE20_POS:
            targetParamPtr = &(sysParamPtr->updownMotorParam.loadRackStore20Pos);
            break;
        case INDEX_SUB_PARAM_UPDOWN_MOTOR_UNLOAD_RACK_STORE20_POS:
            targetParamPtr = &(sysParamPtr->updownMotorParam.unloadRackStore20Pos);
            break;
        case INDEX_SUB_PARAM_UPDOWN_MOTOR_LOAD_RACK_STORE21_POS:
            targetParamPtr = &(sysParamPtr->updownMotorParam.loadRackStore21Pos);
            break;
        case INDEX_SUB_PARAM_UPDOWN_MOTOR_UNLOAD_RACK_STORE21_POS:
            targetParamPtr = &(sysParamPtr->updownMotorParam.unloadRackStore21Pos);
            break;
        case INDEX_SUB_PARAM_UPDOWN_MOTOR_LOAD_RACK_STORE22_POS:
            targetParamPtr = &(sysParamPtr->updownMotorParam.loadRackStore22Pos);
            break;
        case INDEX_SUB_PARAM_UPDOWN_MOTOR_UNLOAD_RACK_STORE22_POS:
            targetParamPtr = &(sysParamPtr->updownMotorParam.unloadRackStore22Pos);
            break;
        case INDEX_SUB_PARAM_UPDOWN_MOTOR_LOAD_RACK_STORE23_POS:
            targetParamPtr = &(sysParamPtr->updownMotorParam.loadRackStore23Pos);
            break;
        case INDEX_SUB_PARAM_UPDOWN_MOTOR_UNLOAD_RACK_STORE23_POS:
            targetParamPtr = &(sysParamPtr->updownMotorParam.unloadRackStore23Pos);
            break;
        case INDEX_SUB_PARAM_UPDOWN_MOTOR_LOADC21_POS:
            targetParamPtr = &(sysParamPtr->updownMotorParam.loadC21Pos);
            break;
        case INDEX_SUB_PARAM_UPDOWN_MOTOR_UNLOADC21_POS:
            targetParamPtr = &(sysParamPtr->updownMotorParam.unloadC21Pos);
            break;
        default:
            return ERROR_SUB_SYS_PARAM_WRITE_SUB_INDEX;//不支持的参数
    }
    //参数写入Flash
    SysParamWriteSingleCommon(targetParamPtr,writeParam);
    //返回结果
    return ERROR_SUB_OK;
}
/*----------------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------------*/
//读写顶杆电机参数
ERROR_SUB SysParamReadParamRodMotorParam(uint8_t subIndex,int32_t* readParam)
{
    switch(subIndex)
    {
        case INDEX_SUB_PARAM_ROD_MOTOR_RESET_CORRECT:
            *readParam = sysParamPtr->rodMotorParam.resetCorrect;
            break;
        case INDEX_SUB_PARAM_ROD_MOTOR_DIR_SWITCH_CORRECT:
            *readParam = sysParamPtr->rodMotorParam.dirSwitchCorrect;
            break;
        case INDEX_SUB_PARAM_ROD_MOTOR_IDLE_POS:
            *readParam = sysParamPtr->rodMotorParam.idlePos;
            break;
        case INDEX_SUB_PARAM_ROD_MOTOR_LOAD_RACK_STORE1_POS:
            *readParam = sysParamPtr->rodMotorParam.loadRackStore1Pos;
            break;
        case INDEX_SUB_PARAM_ROD_MOTOR_UNLOAD_RACK_STORE1_POS:
            *readParam = sysParamPtr->rodMotorParam.unloadRackStore1Pos;
            break;
        case INDEX_SUB_PARAM_ROD_MOTOR_LOAD_RACK_STORE2_POS:
            *readParam = sysParamPtr->rodMotorParam.loadRackStore2Pos;
            break;
        case INDEX_SUB_PARAM_ROD_MOTOR_UNLOAD_RACK_STORE2_POS:
            *readParam = sysParamPtr->rodMotorParam.unloadRackStore2Pos;
            break;
        case INDEX_SUB_PARAM_ROD_MOTOR_LOAD_RACK_STORE3_POS:
            *readParam = sysParamPtr->rodMotorParam.loadRackStore3Pos;
            break;
        case INDEX_SUB_PARAM_ROD_MOTOR_UNLOAD_RACK_STORE3_POS:
            *readParam = sysParamPtr->rodMotorParam.unloadRackStore3Pos;
            break;
        case INDEX_SUB_PARAM_ROD_MOTOR_LOAD_RACK_STORE4_POS:
            *readParam = sysParamPtr->rodMotorParam.loadRackStore4Pos;
            break;
        case INDEX_SUB_PARAM_ROD_MOTOR_UNLOAD_RACK_STORE4_POS:
            *readParam = sysParamPtr->rodMotorParam.unloadRackStore4Pos;
            break;
        case INDEX_SUB_PARAM_ROD_MOTOR_LOAD_RACK_STORE5_POS:
            *readParam = sysParamPtr->rodMotorParam.loadRackStore5Pos;
            break;
        case INDEX_SUB_PARAM_ROD_MOTOR_UNLOAD_RACK_STORE5_POS:
            *readParam = sysParamPtr->rodMotorParam.unloadRackStore5Pos;
            break;
        case INDEX_SUB_PARAM_ROD_MOTOR_LOAD_RACK_STORE6_POS:
            *readParam = sysParamPtr->rodMotorParam.loadRackStore6Pos;
            break;
        case INDEX_SUB_PARAM_ROD_MOTOR_UNLOAD_RACK_STORE6_POS:
            *readParam = sysParamPtr->rodMotorParam.unloadRackStore6Pos;
            break;
        case INDEX_SUB_PARAM_ROD_MOTOR_LOAD_RACK_STORE7_POS:
            *readParam = sysParamPtr->rodMotorParam.loadRackStore7Pos;
            break;
        case INDEX_SUB_PARAM_ROD_MOTOR_UNLOAD_RACK_STORE7_POS:
            *readParam = sysParamPtr->rodMotorParam.unloadRackStore7Pos;
            break;
        case INDEX_SUB_PARAM_ROD_MOTOR_LOAD_RACK_STORE8_POS:
            *readParam = sysParamPtr->rodMotorParam.loadRackStore8Pos;
            break;
        case INDEX_SUB_PARAM_ROD_MOTOR_UNLOAD_RACK_STORE8_POS:
            *readParam = sysParamPtr->rodMotorParam.unloadRackStore8Pos;
            break;
        case INDEX_SUB_PARAM_ROD_MOTOR_LOAD_RACK_STORE9_POS:
            *readParam = sysParamPtr->rodMotorParam.loadRackStore9Pos;
            break;
        case INDEX_SUB_PARAM_ROD_MOTOR_UNLOAD_RACK_STORE9_POS:
            *readParam = sysParamPtr->rodMotorParam.unloadRackStore9Pos;
            break;
        case INDEX_SUB_PARAM_ROD_MOTOR_LOAD_RACK_STORE10_POS:
            *readParam = sysParamPtr->rodMotorParam.loadRackStore10Pos;
            break;
        case INDEX_SUB_PARAM_ROD_MOTOR_UNLOAD_RACK_STORE10_POS:
            *readParam = sysParamPtr->rodMotorParam.unloadRackStore10Pos;
            break;
        case INDEX_SUB_PARAM_ROD_MOTOR_LOAD_RACK_STORE11_POS:
            *readParam = sysParamPtr->rodMotorParam.loadRackStore11Pos;
            break;
        case INDEX_SUB_PARAM_ROD_MOTOR_UNLOAD_RACK_STORE11_POS:
            *readParam = sysParamPtr->rodMotorParam.unloadRackStore11Pos;
            break;
        case INDEX_SUB_PARAM_ROD_MOTOR_LOAD_RACK_STORE12_POS:
            *readParam = sysParamPtr->rodMotorParam.loadRackStore12Pos;
            break;
        case INDEX_SUB_PARAM_ROD_MOTOR_UNLOAD_RACK_STORE12_POS:
            *readParam = sysParamPtr->rodMotorParam.unloadRackStore12Pos;
            break;
        case INDEX_SUB_PARAM_ROD_MOTOR_LOAD_RACK_STORE13_POS:
            *readParam = sysParamPtr->rodMotorParam.loadRackStore13Pos;
            break;
        case INDEX_SUB_PARAM_ROD_MOTOR_UNLOAD_RACK_STORE13_POS:
            *readParam = sysParamPtr->rodMotorParam.unloadRackStore13Pos;
            break;
        case INDEX_SUB_PARAM_ROD_MOTOR_LOAD_RACK_STORE14_POS:
            *readParam = sysParamPtr->rodMotorParam.loadRackStore14Pos;
            break;
        case INDEX_SUB_PARAM_ROD_MOTOR_UNLOAD_RACK_STORE14_POS:
            *readParam = sysParamPtr->rodMotorParam.unloadRackStore14Pos;
            break;
        case INDEX_SUB_PARAM_ROD_MOTOR_LOAD_RACK_STORE15_POS:
            *readParam = sysParamPtr->rodMotorParam.loadRackStore15Pos;
            break;
        case INDEX_SUB_PARAM_ROD_MOTOR_UNLOAD_RACK_STORE15_POS:
            *readParam = sysParamPtr->rodMotorParam.unloadRackStore15Pos;
            break;
        case INDEX_SUB_PARAM_ROD_MOTOR_LOAD_RACK_STORE16_POS:
            *readParam = sysParamPtr->rodMotorParam.loadRackStore16Pos;
            break;
        case INDEX_SUB_PARAM_ROD_MOTOR_UNLOAD_RACK_STORE16_POS:
            *readParam = sysParamPtr->rodMotorParam.unloadRackStore16Pos;
            break;
        case INDEX_SUB_PARAM_ROD_MOTOR_LOAD_RACK_STORE17_POS:
            *readParam = sysParamPtr->rodMotorParam.loadRackStore17Pos;
            break;
        case INDEX_SUB_PARAM_ROD_MOTOR_UNLOAD_RACK_STORE17_POS:
            *readParam = sysParamPtr->rodMotorParam.unloadRackStore17Pos;
            break;
        case INDEX_SUB_PARAM_ROD_MOTOR_LOAD_RACK_STORE18_POS:
            *readParam = sysParamPtr->rodMotorParam.loadRackStore18Pos;
            break;
        case INDEX_SUB_PARAM_ROD_MOTOR_UNLOAD_RACK_STORE18_POS:
            *readParam = sysParamPtr->rodMotorParam.unloadRackStore18Pos;
            break;
        case INDEX_SUB_PARAM_ROD_MOTOR_LOAD_RACK_STORE19_POS:
            *readParam = sysParamPtr->rodMotorParam.loadRackStore19Pos;
            break;
        case INDEX_SUB_PARAM_ROD_MOTOR_UNLOAD_RACK_STORE19_POS:
            *readParam = sysParamPtr->rodMotorParam.unloadRackStore19Pos;
            break;
        case INDEX_SUB_PARAM_ROD_MOTOR_LOAD_RACK_STORE20_POS:
            *readParam = sysParamPtr->rodMotorParam.loadRackStore20Pos;
            break;
        case INDEX_SUB_PARAM_ROD_MOTOR_UNLOAD_RACK_STORE20_POS:
            *readParam = sysParamPtr->rodMotorParam.unloadRackStore20Pos;
            break;
        case INDEX_SUB_PARAM_ROD_MOTOR_LOAD_RACK_STORE21_POS:
            *readParam = sysParamPtr->rodMotorParam.loadRackStore21Pos;
            break;
        case INDEX_SUB_PARAM_ROD_MOTOR_UNLOAD_RACK_STORE21_POS:
            *readParam = sysParamPtr->rodMotorParam.unloadRackStore21Pos;
            break;
        case INDEX_SUB_PARAM_ROD_MOTOR_LOAD_RACK_STORE22_POS:
            *readParam = sysParamPtr->rodMotorParam.loadRackStore22Pos;
            break;
        case INDEX_SUB_PARAM_ROD_MOTOR_UNLOAD_RACK_STORE22_POS:
            *readParam = sysParamPtr->rodMotorParam.unloadRackStore22Pos;
            break;
        case INDEX_SUB_PARAM_ROD_MOTOR_LOAD_RACK_STORE23_POS:
            *readParam = sysParamPtr->rodMotorParam.loadRackStore23Pos;
            break;
        case INDEX_SUB_PARAM_ROD_MOTOR_UNLOAD_RACK_STORE23_POS:
            *readParam = sysParamPtr->rodMotorParam.unloadRackStore23Pos;
            break;
        case INDEX_SUB_PARAM_ROD_MOTOR_LOADC21_POS:
            *readParam = sysParamPtr->rodMotorParam.loadC21Pos;
            break;
        case INDEX_SUB_PARAM_ROD_MOTOR_UNLOADC21_POS:
            *readParam = sysParamPtr->rodMotorParam.unloadC21Pos;
            break;
        default://不支持的参数
            *readParam = 0;
            return ERROR_SUB_SYS_PARAM_READ_SUB_INDEX;
    }
    //返回结果
    return ERROR_SUB_OK;
}

ERROR_SUB SysParamWriteParamRodMotorParam(uint8_t subIndex,int32_t writeParam)
{
    int32_t* targetParamPtr = NULL;
    switch(subIndex)
    {
        case INDEX_SUB_PARAM_ROD_MOTOR_RESET_CORRECT:
            targetParamPtr = &(sysParamPtr->rodMotorParam.resetCorrect);
            break;
        case INDEX_SUB_PARAM_ROD_MOTOR_DIR_SWITCH_CORRECT:
            targetParamPtr = &(sysParamPtr->rodMotorParam.dirSwitchCorrect);
            break;
        case INDEX_SUB_PARAM_ROD_MOTOR_IDLE_POS:
            targetParamPtr = &(sysParamPtr->rodMotorParam.idlePos);
            break;
        case INDEX_SUB_PARAM_ROD_MOTOR_LOAD_RACK_STORE1_POS:
            targetParamPtr = &(sysParamPtr->rodMotorParam.loadRackStore1Pos);
            break;
        case INDEX_SUB_PARAM_ROD_MOTOR_UNLOAD_RACK_STORE1_POS:
            targetParamPtr = &(sysParamPtr->rodMotorParam.unloadRackStore1Pos);
            break;
        case INDEX_SUB_PARAM_ROD_MOTOR_LOAD_RACK_STORE2_POS:
            targetParamPtr = &(sysParamPtr->rodMotorParam.loadRackStore2Pos);
            break;
        case INDEX_SUB_PARAM_ROD_MOTOR_UNLOAD_RACK_STORE2_POS:
            targetParamPtr = &(sysParamPtr->rodMotorParam.unloadRackStore2Pos);
            break;
        case INDEX_SUB_PARAM_ROD_MOTOR_LOAD_RACK_STORE3_POS:
            targetParamPtr = &(sysParamPtr->rodMotorParam.loadRackStore3Pos);
            break;
        case INDEX_SUB_PARAM_ROD_MOTOR_UNLOAD_RACK_STORE3_POS:
            targetParamPtr = &(sysParamPtr->rodMotorParam.unloadRackStore3Pos);
            break;
        case INDEX_SUB_PARAM_ROD_MOTOR_LOAD_RACK_STORE4_POS:
            targetParamPtr = &(sysParamPtr->rodMotorParam.loadRackStore4Pos);
            break;
        case INDEX_SUB_PARAM_ROD_MOTOR_UNLOAD_RACK_STORE4_POS:
            targetParamPtr = &(sysParamPtr->rodMotorParam.unloadRackStore4Pos);
            break;
        case INDEX_SUB_PARAM_ROD_MOTOR_LOAD_RACK_STORE5_POS:
            targetParamPtr = &(sysParamPtr->rodMotorParam.loadRackStore5Pos);
            break;
        case INDEX_SUB_PARAM_ROD_MOTOR_UNLOAD_RACK_STORE5_POS:
            targetParamPtr = &(sysParamPtr->rodMotorParam.unloadRackStore5Pos);
            break;
        case INDEX_SUB_PARAM_ROD_MOTOR_LOAD_RACK_STORE6_POS:
            targetParamPtr = &(sysParamPtr->rodMotorParam.loadRackStore6Pos);
            break;
        case INDEX_SUB_PARAM_ROD_MOTOR_UNLOAD_RACK_STORE6_POS:
            targetParamPtr = &(sysParamPtr->rodMotorParam.unloadRackStore6Pos);
            break;
        case INDEX_SUB_PARAM_ROD_MOTOR_LOAD_RACK_STORE7_POS:
            targetParamPtr = &(sysParamPtr->rodMotorParam.loadRackStore7Pos);
            break;
        case INDEX_SUB_PARAM_ROD_MOTOR_UNLOAD_RACK_STORE7_POS:
            targetParamPtr = &(sysParamPtr->rodMotorParam.unloadRackStore7Pos);
            break;
        case INDEX_SUB_PARAM_ROD_MOTOR_LOAD_RACK_STORE8_POS:
            targetParamPtr = &(sysParamPtr->rodMotorParam.loadRackStore8Pos);
            break;
        case INDEX_SUB_PARAM_ROD_MOTOR_UNLOAD_RACK_STORE8_POS:
            targetParamPtr = &(sysParamPtr->rodMotorParam.unloadRackStore8Pos);
            break;
        case INDEX_SUB_PARAM_ROD_MOTOR_LOAD_RACK_STORE9_POS:
            targetParamPtr = &(sysParamPtr->rodMotorParam.loadRackStore9Pos);
            break;
        case INDEX_SUB_PARAM_ROD_MOTOR_UNLOAD_RACK_STORE9_POS:
            targetParamPtr = &(sysParamPtr->rodMotorParam.unloadRackStore9Pos);
            break;
        case INDEX_SUB_PARAM_ROD_MOTOR_LOAD_RACK_STORE10_POS:
            targetParamPtr = &(sysParamPtr->rodMotorParam.loadRackStore10Pos);
            break;
        case INDEX_SUB_PARAM_ROD_MOTOR_UNLOAD_RACK_STORE10_POS:
            targetParamPtr = &(sysParamPtr->rodMotorParam.unloadRackStore10Pos);
            break;
        case INDEX_SUB_PARAM_ROD_MOTOR_LOAD_RACK_STORE11_POS:
            targetParamPtr = &(sysParamPtr->rodMotorParam.loadRackStore11Pos);
            break;
        case INDEX_SUB_PARAM_ROD_MOTOR_UNLOAD_RACK_STORE11_POS:
            targetParamPtr = &(sysParamPtr->rodMotorParam.unloadRackStore11Pos);
            break;
        case INDEX_SUB_PARAM_ROD_MOTOR_LOAD_RACK_STORE12_POS:
            targetParamPtr = &(sysParamPtr->rodMotorParam.loadRackStore12Pos);
            break;
        case INDEX_SUB_PARAM_ROD_MOTOR_UNLOAD_RACK_STORE12_POS:
            targetParamPtr = &(sysParamPtr->rodMotorParam.unloadRackStore12Pos);
            break;
        case INDEX_SUB_PARAM_ROD_MOTOR_LOAD_RACK_STORE13_POS:
            targetParamPtr = &(sysParamPtr->rodMotorParam.loadRackStore13Pos);
            break;
        case INDEX_SUB_PARAM_ROD_MOTOR_UNLOAD_RACK_STORE13_POS:
            targetParamPtr = &(sysParamPtr->rodMotorParam.unloadRackStore13Pos);
            break;
        case INDEX_SUB_PARAM_ROD_MOTOR_LOAD_RACK_STORE14_POS:
            targetParamPtr = &(sysParamPtr->rodMotorParam.loadRackStore14Pos);
            break;
        case INDEX_SUB_PARAM_ROD_MOTOR_UNLOAD_RACK_STORE14_POS:
            targetParamPtr = &(sysParamPtr->rodMotorParam.unloadRackStore14Pos);
            break;
        case INDEX_SUB_PARAM_ROD_MOTOR_LOAD_RACK_STORE15_POS:
            targetParamPtr = &(sysParamPtr->rodMotorParam.loadRackStore15Pos);
            break;
        case INDEX_SUB_PARAM_ROD_MOTOR_UNLOAD_RACK_STORE15_POS:
            targetParamPtr = &(sysParamPtr->rodMotorParam.unloadRackStore15Pos);
            break;
        case INDEX_SUB_PARAM_ROD_MOTOR_LOAD_RACK_STORE16_POS:
            targetParamPtr = &(sysParamPtr->rodMotorParam.loadRackStore16Pos);
            break;
        case INDEX_SUB_PARAM_ROD_MOTOR_UNLOAD_RACK_STORE16_POS:
            targetParamPtr = &(sysParamPtr->rodMotorParam.unloadRackStore16Pos);
            break;
        case INDEX_SUB_PARAM_ROD_MOTOR_LOAD_RACK_STORE17_POS:
            targetParamPtr = &(sysParamPtr->rodMotorParam.loadRackStore17Pos);
            break;
        case INDEX_SUB_PARAM_ROD_MOTOR_UNLOAD_RACK_STORE17_POS:
            targetParamPtr = &(sysParamPtr->rodMotorParam.unloadRackStore17Pos);
            break;
        case INDEX_SUB_PARAM_ROD_MOTOR_LOAD_RACK_STORE18_POS:
            targetParamPtr = &(sysParamPtr->rodMotorParam.loadRackStore18Pos);
            break;
        case INDEX_SUB_PARAM_ROD_MOTOR_UNLOAD_RACK_STORE18_POS:
            targetParamPtr = &(sysParamPtr->rodMotorParam.unloadRackStore18Pos);
            break;
        case INDEX_SUB_PARAM_ROD_MOTOR_LOAD_RACK_STORE19_POS:
            targetParamPtr = &(sysParamPtr->rodMotorParam.loadRackStore19Pos);
            break;
        case INDEX_SUB_PARAM_ROD_MOTOR_UNLOAD_RACK_STORE19_POS:
            targetParamPtr = &(sysParamPtr->rodMotorParam.unloadRackStore19Pos);
            break;
        case INDEX_SUB_PARAM_ROD_MOTOR_LOAD_RACK_STORE20_POS:
            targetParamPtr = &(sysParamPtr->rodMotorParam.loadRackStore20Pos);
            break;
        case INDEX_SUB_PARAM_ROD_MOTOR_UNLOAD_RACK_STORE20_POS:
            targetParamPtr = &(sysParamPtr->rodMotorParam.unloadRackStore20Pos);
            break;
        case INDEX_SUB_PARAM_ROD_MOTOR_LOAD_RACK_STORE21_POS:
            targetParamPtr = &(sysParamPtr->rodMotorParam.loadRackStore21Pos);
            break;
        case INDEX_SUB_PARAM_ROD_MOTOR_UNLOAD_RACK_STORE21_POS:
            targetParamPtr = &(sysParamPtr->rodMotorParam.unloadRackStore21Pos);
            break;
        case INDEX_SUB_PARAM_ROD_MOTOR_LOAD_RACK_STORE22_POS:
            targetParamPtr = &(sysParamPtr->rodMotorParam.loadRackStore22Pos);
            break;
        case INDEX_SUB_PARAM_ROD_MOTOR_UNLOAD_RACK_STORE22_POS:
            targetParamPtr = &(sysParamPtr->rodMotorParam.unloadRackStore22Pos);
            break;
        case INDEX_SUB_PARAM_ROD_MOTOR_LOAD_RACK_STORE23_POS:
            targetParamPtr = &(sysParamPtr->rodMotorParam.loadRackStore23Pos);
            break;
        case INDEX_SUB_PARAM_ROD_MOTOR_UNLOAD_RACK_STORE23_POS:
            targetParamPtr = &(sysParamPtr->rodMotorParam.unloadRackStore23Pos);
            break;
        case INDEX_SUB_PARAM_ROD_MOTOR_LOADC21_POS:
            targetParamPtr = &(sysParamPtr->rodMotorParam.loadC21Pos);
            break;
        case INDEX_SUB_PARAM_ROD_MOTOR_UNLOADC21_POS:
            targetParamPtr = &(sysParamPtr->rodMotorParam.unloadC21Pos);
            break;
        default:
            return ERROR_SUB_SYS_PARAM_WRITE_SUB_INDEX;//不支持的参数
    }
    //参数写入Flash
    SysParamWriteSingleCommon(targetParamPtr,writeParam);
    //返回结果
    return ERROR_SUB_OK;
}
/*----------------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------------*/
//读写皮带电机参数
ERROR_SUB SysParamReadParamBeltMotorParam(uint8_t subIndex,int32_t* readParam)
{
    switch(subIndex)
    {
        case INDEX_SUB_PARAM_BELT_MOTOR_RESET_CORRECT:
            *readParam = sysParamPtr->beltMotorParam.resetCorrect;
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_DIR_SWITCH_CORRECT:
            *readParam = sysParamPtr->beltMotorParam.dirSwitchCorrect;
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_IDLE_POS:
            *readParam = sysParamPtr->beltMotorParam.idlePos;
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_LOAD_RACK_STORE1_POS:
            *readParam = sysParamPtr->beltMotorParam.loadRackStore1Pos;
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_UNLOAD_RACK_STORE1_POS:
            *readParam = sysParamPtr->beltMotorParam.unloadRackStore1Pos;
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_LOAD_RACK_STORE2_POS:
            *readParam = sysParamPtr->beltMotorParam.loadRackStore2Pos;
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_UNLOAD_RACK_STORE2_POS:
            *readParam = sysParamPtr->beltMotorParam.unloadRackStore2Pos;
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_LOAD_RACK_STORE3_POS:
            *readParam = sysParamPtr->beltMotorParam.loadRackStore3Pos;
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_UNLOAD_RACK_STORE3_POS:
            *readParam = sysParamPtr->beltMotorParam.unloadRackStore3Pos;
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_LOAD_RACK_STORE4_POS:
            *readParam = sysParamPtr->beltMotorParam.loadRackStore4Pos;
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_UNLOAD_RACK_STORE4_POS:
            *readParam = sysParamPtr->beltMotorParam.unloadRackStore4Pos;
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_LOAD_RACK_STORE5_POS:
            *readParam = sysParamPtr->beltMotorParam.loadRackStore5Pos;
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_UNLOAD_RACK_STORE5_POS:
            *readParam = sysParamPtr->beltMotorParam.unloadRackStore5Pos;
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_LOAD_RACK_STORE6_POS:
            *readParam = sysParamPtr->beltMotorParam.loadRackStore6Pos;
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_UNLOAD_RACK_STORE6_POS:
            *readParam = sysParamPtr->beltMotorParam.unloadRackStore6Pos;
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_LOAD_RACK_STORE7_POS:
            *readParam = sysParamPtr->beltMotorParam.loadRackStore7Pos;
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_UNLOAD_RACK_STORE7_POS:
            *readParam = sysParamPtr->beltMotorParam.unloadRackStore7Pos;
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_LOAD_RACK_STORE8_POS:
            *readParam = sysParamPtr->beltMotorParam.loadRackStore8Pos;
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_UNLOAD_RACK_STORE8_POS:
            *readParam = sysParamPtr->beltMotorParam.unloadRackStore8Pos;
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_LOAD_RACK_STORE9_POS:
            *readParam = sysParamPtr->beltMotorParam.loadRackStore9Pos;
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_UNLOAD_RACK_STORE9_POS:
            *readParam = sysParamPtr->beltMotorParam.unloadRackStore9Pos;
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_LOAD_RACK_STORE10_POS:
            *readParam = sysParamPtr->beltMotorParam.loadRackStore10Pos;
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_UNLOAD_RACK_STORE10_POS:
            *readParam = sysParamPtr->beltMotorParam.unloadRackStore10Pos;
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_LOAD_RACK_STORE11_POS:
            *readParam = sysParamPtr->beltMotorParam.loadRackStore11Pos;
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_UNLOAD_RACK_STORE11_POS:
            *readParam = sysParamPtr->beltMotorParam.unloadRackStore11Pos;
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_LOAD_RACK_STORE12_POS:
            *readParam = sysParamPtr->beltMotorParam.loadRackStore12Pos;
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_UNLOAD_RACK_STORE12_POS:
            *readParam = sysParamPtr->beltMotorParam.unloadRackStore12Pos;
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_LOAD_RACK_STORE13_POS:
            *readParam = sysParamPtr->beltMotorParam.loadRackStore13Pos;
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_UNLOAD_RACK_STORE13_POS:
            *readParam = sysParamPtr->beltMotorParam.unloadRackStore13Pos;
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_LOAD_RACK_STORE14_POS:
            *readParam = sysParamPtr->beltMotorParam.loadRackStore14Pos;
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_UNLOAD_RACK_STORE14_POS:
            *readParam = sysParamPtr->beltMotorParam.unloadRackStore14Pos;
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_LOAD_RACK_STORE15_POS:
            *readParam = sysParamPtr->beltMotorParam.loadRackStore15Pos;
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_UNLOAD_RACK_STORE15_POS:
            *readParam = sysParamPtr->beltMotorParam.unloadRackStore15Pos;
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_LOAD_RACK_STORE16_POS:
            *readParam = sysParamPtr->beltMotorParam.loadRackStore16Pos;
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_UNLOAD_RACK_STORE16_POS:
            *readParam = sysParamPtr->beltMotorParam.unloadRackStore16Pos;
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_LOAD_RACK_STORE17_POS:
            *readParam = sysParamPtr->beltMotorParam.loadRackStore17Pos;
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_UNLOAD_RACK_STORE17_POS:
            *readParam = sysParamPtr->beltMotorParam.unloadRackStore17Pos;
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_LOAD_RACK_STORE18_POS:
            *readParam = sysParamPtr->beltMotorParam.loadRackStore18Pos;
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_UNLOAD_RACK_STORE18_POS:
            *readParam = sysParamPtr->beltMotorParam.unloadRackStore18Pos;
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_LOAD_RACK_STORE19_POS:
            *readParam = sysParamPtr->beltMotorParam.loadRackStore19Pos;
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_UNLOAD_RACK_STORE19_POS:
            *readParam = sysParamPtr->beltMotorParam.unloadRackStore19Pos;
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_LOAD_RACK_STORE20_POS:
            *readParam = sysParamPtr->beltMotorParam.loadRackStore20Pos;
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_UNLOAD_RACK_STORE20_POS:
            *readParam = sysParamPtr->beltMotorParam.unloadRackStore20Pos;
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_LOAD_RACK_STORE21_POS:
            *readParam = sysParamPtr->beltMotorParam.loadRackStore21Pos;
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_UNLOAD_RACK_STORE21_POS:
            *readParam = sysParamPtr->beltMotorParam.unloadRackStore21Pos;
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_LOAD_RACK_STORE22_POS:
            *readParam = sysParamPtr->beltMotorParam.loadRackStore22Pos;
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_UNLOAD_RACK_STORE22_POS:
            *readParam = sysParamPtr->beltMotorParam.unloadRackStore22Pos;
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_LOAD_RACK_STORE23_POS:
            *readParam = sysParamPtr->beltMotorParam.loadRackStore23Pos;
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_UNLOAD_RACK_STORE23_POS:
            *readParam = sysParamPtr->beltMotorParam.unloadRackStore23Pos;
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_LOADC21_POS:
            *readParam = sysParamPtr->beltMotorParam.loadC21Pos;
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_UNLOADC21_POS:
            *readParam = sysParamPtr->beltMotorParam.unloadC21Pos;
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_BAR_SCAN_START_POS:
            *readParam = sysParamPtr->beltMotorParam.barscanStartPos;
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_BAR_SCAN_END_POS:
            *readParam = sysParamPtr->beltMotorParam.barscanEndPos;
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_TUBE1_SCAN_POS:
            *readParam = sysParamPtr->beltMotorParam.tube1ScanPos;
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_TUBE2_SCAN_POS:
            *readParam = sysParamPtr->beltMotorParam.tube2ScanPos;
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_TUBE3_SCAN_POS:
            *readParam = sysParamPtr->beltMotorParam.tube3ScanPos;
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_TUBE4_SCAN_POS:
            *readParam = sysParamPtr->beltMotorParam.tube4ScanPos;
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_TUBE5_SCAN_POS:
            *readParam = sysParamPtr->beltMotorParam.tube5ScanPos;
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_TUBE6_SCAN_POS:
            *readParam = sysParamPtr->beltMotorParam.tube6ScanPos;
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_TUBE7_SCAN_POS:
            *readParam = sysParamPtr->beltMotorParam.tube7ScanPos;
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_TUBE8_SCAN_POS:
            *readParam = sysParamPtr->beltMotorParam.tube8ScanPos;
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_TUBE9_SCAN_POS:
            *readParam = sysParamPtr->beltMotorParam.tube9ScanPos;
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_TUBE10_SCAN_POS:
            *readParam = sysParamPtr->beltMotorParam.tube10ScanPos;
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_ADJUST_START_POS:
            *readParam = sysParamPtr->beltMotorParam.adjustStartPos;
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_ADJUST_OVER_POS:
            *readParam = sysParamPtr->beltMotorParam.adjustOverPos;
            break;
        default://不支持的参数
            *readParam = 0;
            return ERROR_SUB_SYS_PARAM_READ_SUB_INDEX;
    }
    //返回结果
    return ERROR_SUB_OK;
}

ERROR_SUB SysParamWriteParamBeltMotorParam(uint8_t subIndex,int32_t writeParam)
{
    int32_t* targetParamPtr = NULL;
    switch(subIndex)
    {
        case INDEX_SUB_PARAM_BELT_MOTOR_RESET_CORRECT:
            targetParamPtr = &(sysParamPtr->beltMotorParam.resetCorrect);
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_DIR_SWITCH_CORRECT:
            targetParamPtr = &(sysParamPtr->beltMotorParam.dirSwitchCorrect);
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_IDLE_POS:
            targetParamPtr = &(sysParamPtr->beltMotorParam.idlePos);
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_LOAD_RACK_STORE1_POS:
            targetParamPtr = &(sysParamPtr->beltMotorParam.loadRackStore1Pos);
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_UNLOAD_RACK_STORE1_POS:
            targetParamPtr = &(sysParamPtr->beltMotorParam.unloadRackStore1Pos);
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_LOAD_RACK_STORE2_POS:
            targetParamPtr = &(sysParamPtr->beltMotorParam.loadRackStore2Pos);
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_UNLOAD_RACK_STORE2_POS:
            targetParamPtr = &(sysParamPtr->beltMotorParam.unloadRackStore2Pos);
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_LOAD_RACK_STORE3_POS:
            targetParamPtr = &(sysParamPtr->beltMotorParam.loadRackStore3Pos);
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_UNLOAD_RACK_STORE3_POS:
            targetParamPtr = &(sysParamPtr->beltMotorParam.unloadRackStore3Pos);
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_LOAD_RACK_STORE4_POS:
            targetParamPtr = &(sysParamPtr->beltMotorParam.loadRackStore4Pos);
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_UNLOAD_RACK_STORE4_POS:
            targetParamPtr = &(sysParamPtr->beltMotorParam.unloadRackStore4Pos);
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_LOAD_RACK_STORE5_POS:
            targetParamPtr = &(sysParamPtr->beltMotorParam.loadRackStore5Pos);
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_UNLOAD_RACK_STORE5_POS:
            targetParamPtr = &(sysParamPtr->beltMotorParam.unloadRackStore5Pos);
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_LOAD_RACK_STORE6_POS:
            targetParamPtr = &(sysParamPtr->beltMotorParam.loadRackStore6Pos);
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_UNLOAD_RACK_STORE6_POS:
            targetParamPtr = &(sysParamPtr->beltMotorParam.unloadRackStore6Pos);
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_LOAD_RACK_STORE7_POS:
            targetParamPtr = &(sysParamPtr->beltMotorParam.loadRackStore7Pos);
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_UNLOAD_RACK_STORE7_POS:
            targetParamPtr = &(sysParamPtr->beltMotorParam.unloadRackStore7Pos);
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_LOAD_RACK_STORE8_POS:
            targetParamPtr = &(sysParamPtr->beltMotorParam.loadRackStore8Pos);
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_UNLOAD_RACK_STORE8_POS:
            targetParamPtr = &(sysParamPtr->beltMotorParam.unloadRackStore8Pos);
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_LOAD_RACK_STORE9_POS:
            targetParamPtr = &(sysParamPtr->beltMotorParam.loadRackStore9Pos);
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_UNLOAD_RACK_STORE9_POS:
            targetParamPtr = &(sysParamPtr->beltMotorParam.unloadRackStore9Pos);
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_LOAD_RACK_STORE10_POS:
            targetParamPtr = &(sysParamPtr->beltMotorParam.loadRackStore10Pos);
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_UNLOAD_RACK_STORE10_POS:
            targetParamPtr = &(sysParamPtr->beltMotorParam.unloadRackStore10Pos);
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_LOAD_RACK_STORE11_POS:
            targetParamPtr = &(sysParamPtr->beltMotorParam.loadRackStore11Pos);
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_UNLOAD_RACK_STORE11_POS:
            targetParamPtr = &(sysParamPtr->beltMotorParam.unloadRackStore11Pos);
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_LOAD_RACK_STORE12_POS:
            targetParamPtr = &(sysParamPtr->beltMotorParam.loadRackStore12Pos);
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_UNLOAD_RACK_STORE12_POS:
            targetParamPtr = &(sysParamPtr->beltMotorParam.unloadRackStore12Pos);
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_LOAD_RACK_STORE13_POS:
            targetParamPtr = &(sysParamPtr->beltMotorParam.loadRackStore13Pos);
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_UNLOAD_RACK_STORE13_POS:
            targetParamPtr = &(sysParamPtr->beltMotorParam.unloadRackStore13Pos);
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_LOAD_RACK_STORE14_POS:
            targetParamPtr = &(sysParamPtr->beltMotorParam.loadRackStore14Pos);
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_UNLOAD_RACK_STORE14_POS:
            targetParamPtr = &(sysParamPtr->beltMotorParam.unloadRackStore14Pos);
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_LOAD_RACK_STORE15_POS:
            targetParamPtr = &(sysParamPtr->beltMotorParam.loadRackStore15Pos);
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_UNLOAD_RACK_STORE15_POS:
            targetParamPtr = &(sysParamPtr->beltMotorParam.unloadRackStore15Pos);
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_LOAD_RACK_STORE16_POS:
            targetParamPtr = &(sysParamPtr->beltMotorParam.loadRackStore16Pos);
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_UNLOAD_RACK_STORE16_POS:
            targetParamPtr = &(sysParamPtr->beltMotorParam.unloadRackStore16Pos);
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_LOAD_RACK_STORE17_POS:
            targetParamPtr = &(sysParamPtr->beltMotorParam.loadRackStore17Pos);
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_UNLOAD_RACK_STORE17_POS:
            targetParamPtr = &(sysParamPtr->beltMotorParam.unloadRackStore17Pos);
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_LOAD_RACK_STORE18_POS:
            targetParamPtr = &(sysParamPtr->beltMotorParam.loadRackStore18Pos);
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_UNLOAD_RACK_STORE18_POS:
            targetParamPtr = &(sysParamPtr->beltMotorParam.unloadRackStore18Pos);
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_LOAD_RACK_STORE19_POS:
            targetParamPtr = &(sysParamPtr->beltMotorParam.loadRackStore19Pos);
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_UNLOAD_RACK_STORE19_POS:
            targetParamPtr = &(sysParamPtr->beltMotorParam.unloadRackStore19Pos);
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_LOAD_RACK_STORE20_POS:
            targetParamPtr = &(sysParamPtr->beltMotorParam.loadRackStore20Pos);
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_UNLOAD_RACK_STORE20_POS:
            targetParamPtr = &(sysParamPtr->beltMotorParam.unloadRackStore20Pos);
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_LOAD_RACK_STORE21_POS:
            targetParamPtr = &(sysParamPtr->beltMotorParam.loadRackStore21Pos);
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_UNLOAD_RACK_STORE21_POS:
            targetParamPtr = &(sysParamPtr->beltMotorParam.unloadRackStore21Pos);
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_LOAD_RACK_STORE22_POS:
            targetParamPtr = &(sysParamPtr->beltMotorParam.loadRackStore22Pos);
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_UNLOAD_RACK_STORE22_POS:
            targetParamPtr = &(sysParamPtr->beltMotorParam.unloadRackStore22Pos);
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_LOAD_RACK_STORE23_POS:
            targetParamPtr = &(sysParamPtr->beltMotorParam.loadRackStore23Pos);
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_UNLOAD_RACK_STORE23_POS:
            targetParamPtr = &(sysParamPtr->beltMotorParam.unloadRackStore23Pos);
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_LOADC21_POS:
            targetParamPtr = &(sysParamPtr->beltMotorParam.loadC21Pos);
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_UNLOADC21_POS:
            targetParamPtr = &(sysParamPtr->beltMotorParam.unloadC21Pos);
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_BAR_SCAN_START_POS:
            targetParamPtr = &(sysParamPtr->beltMotorParam.barscanStartPos);
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_BAR_SCAN_END_POS:
            targetParamPtr = &(sysParamPtr->beltMotorParam.barscanEndPos);
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_TUBE1_SCAN_POS:
            targetParamPtr = &(sysParamPtr->beltMotorParam.tube1ScanPos);
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_TUBE2_SCAN_POS:
            targetParamPtr = &(sysParamPtr->beltMotorParam.tube2ScanPos);
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_TUBE3_SCAN_POS:
            targetParamPtr = &(sysParamPtr->beltMotorParam.tube3ScanPos);
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_TUBE4_SCAN_POS:
            targetParamPtr = &(sysParamPtr->beltMotorParam.tube4ScanPos);
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_TUBE5_SCAN_POS:
            targetParamPtr = &(sysParamPtr->beltMotorParam.tube5ScanPos);
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_TUBE6_SCAN_POS:
            targetParamPtr = &(sysParamPtr->beltMotorParam.tube6ScanPos);
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_TUBE7_SCAN_POS:
            targetParamPtr = &(sysParamPtr->beltMotorParam.tube7ScanPos);
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_TUBE8_SCAN_POS:
            targetParamPtr = &(sysParamPtr->beltMotorParam.tube8ScanPos);
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_TUBE9_SCAN_POS:
            targetParamPtr = &(sysParamPtr->beltMotorParam.tube9ScanPos);
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_TUBE10_SCAN_POS:
            targetParamPtr = &(sysParamPtr->beltMotorParam.tube10ScanPos);
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_ADJUST_START_POS:
            targetParamPtr = &(sysParamPtr->beltMotorParam.adjustStartPos);
            break;
        case INDEX_SUB_PARAM_BELT_MOTOR_ADJUST_OVER_POS:
            targetParamPtr = &(sysParamPtr->beltMotorParam.adjustOverPos);
            break;
        default:
            return ERROR_SUB_SYS_PARAM_WRITE_SUB_INDEX;//不支持的参数
    }
    //参数写入Flash
    SysParamWriteSingleCommon(targetParamPtr,writeParam);
    //返回结果
    return ERROR_SUB_OK;
}
/*----------------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------------*/
//读写辅助参数
ERROR_SUB SysParamReadParamUtilParam(uint8_t subIndex,int32_t* readParam)
{
    switch(subIndex)
    {
        case INDEX_SUB_PARAM_UTIL_SCAN_MODE:
            *readParam = sysParamPtr->utilParam.scanMode;
            break;
        default://不支持的参数
            *readParam = 0;
            return ERROR_SUB_SYS_PARAM_READ_SUB_INDEX;
    }
    //返回结果
    return ERROR_SUB_OK;
}

ERROR_SUB SysParamWriteParamUtilParam(uint8_t subIndex,int32_t writeParam)
{
    int32_t* targetParamPtr = NULL;
    switch(subIndex)
    {
        case INDEX_SUB_PARAM_UTIL_SCAN_MODE:
            targetParamPtr = &(sysParamPtr->utilParam.scanMode);
            break;
        default:
            return ERROR_SUB_SYS_PARAM_WRITE_SUB_INDEX;//不支持的参数
    }
    //参数写入Flash
    SysParamWriteSingleCommon(targetParamPtr,writeParam);
    //返回结果
    return ERROR_SUB_OK;
}
/*----------------------------------------------------------------------------------------------------*/

/****************************************参数读取函数指针数组**************************************************/
static const SysParamReadParamSingleFuncPtr SysParamReadParamSingleArray[] = {
    SysParamReadParamLaterallyMotorParam,
    SysParamReadParamUpdownMotorParam,
    SysParamReadParamRodMotorParam,
    SysParamReadParamBeltMotorParam,
    SysParamReadParamUtilParam,
};

/****************************************参数写入函数指针数组**************************************************/
static const SysParamWriteParamSingleFuncPtr SysParamWriteParamSingleArray[] = {
    SysParamWriteParamLaterallyMotorParam,
    SysParamWriteParamUpdownMotorParam,
    SysParamWriteParamRodMotorParam,
    SysParamWriteParamBeltMotorParam,
    SysParamWriteParamUtilParam,
};


//读取参数
ERROR_SUB SysParamReadSingle(INDEX_MAIN_SYS_PARAM mainIndex,uint8_t subIndex,int32_t* paramRead)
{
    if(mainIndex >= DIM_ARRAY_ELEMENT_COUNT(SysParamReadParamSingleArray))
    {
        return ERROR_SUB_SYS_PARAM_READ_MAIN_INDEX;
    }
    return SysParamReadParamSingleArray[mainIndex](subIndex,paramRead);
}

//写入参数
ERROR_SUB SysParamWriteSingle(INDEX_MAIN_SYS_PARAM mainIndex,uint8_t subIndex,int32_t paramWrite)
{
    if(mainIndex >= DIM_ARRAY_ELEMENT_COUNT(SysParamWriteParamSingleArray))
    {
        return ERROR_SUB_SYS_PARAM_WRITE_MAIN_INDEX;
    }
    return SysParamWriteParamSingleArray[mainIndex](subIndex,paramWrite);
}



