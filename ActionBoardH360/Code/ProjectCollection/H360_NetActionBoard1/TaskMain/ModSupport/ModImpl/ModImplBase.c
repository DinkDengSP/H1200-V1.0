/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-27 17:08:07 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-12-04 20:38:05 +0800
************************************************************************************************/ 
#include "ModImplBase.h"
#include "ModImplLiquidAutoConfig.h"

//IO急停操作
static void SrvImplBaseUtilEmStopWithOutput(void)
{
    uint16_t indexUtil = 0;
    for(indexUtil = 0; indexUtil < BOARD_PORT_OUT_COUNT; indexUtil++)
    {
        //排除自动稀释相关的IO,与废液相关的IO
        if((indexUtil == (uint16_t)(OUTPUT_LIQUID_PUMP_DP101-H360_SYS_PORT_OUT_BOARD1_CPLD_CDC1))||
            (indexUtil == (uint16_t)(OUTPUT_LIQUID_PUMP_DP304-H360_SYS_PORT_OUT_BOARD1_CPLD_CDC1))||
            (indexUtil == (uint16_t)(OUTPUT_LIQUID_PUMP_DP103-H360_SYS_PORT_OUT_BOARD1_CPLD_CDC1))||
            (indexUtil == (uint16_t)(OUTPUT_LIQUID_VALVE_V235-H360_SYS_PORT_OUT_BOARD1_CPLD_CDC1))||
            (indexUtil == (uint16_t)(OUTPUT_LIQUID_VALVE_V240-H360_SYS_PORT_OUT_BOARD1_CPLD_CDC1))||
            (indexUtil == (uint16_t)(OUTPUT_LIQUID_VALVE_V237-H360_SYS_PORT_OUT_BOARD1_CPLD_CDC1))||
            (indexUtil == (uint16_t)(OUTPUT_LIQUID_VALVE_V307-H360_SYS_PORT_OUT_BOARD1_CPLD_CDC1))||
            (indexUtil == (uint16_t)(OUTPUT_LIQUID_PUMP_DP1-H360_SYS_PORT_OUT_BOARD1_CPLD_CDC1)))
        {
            continue;
        }
        else
        {
            BoardPortOutInitSingle((BOARD_PORT_OUT)indexUtil);
        }
    }
}

//系统急停实现
void SrvImplBaseUtilEmStop(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //构建完成无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    //12个电机全部急停
    IPC_SrvStepMotorEmergencyStop(STEP_MOTOR_NO1);
    IPC_SrvStepMotorEmergencyStop(STEP_MOTOR_NO2);
    IPC_SrvStepMotorEmergencyStop(STEP_MOTOR_NO3);
    IPC_SrvStepMotorEmergencyStop(STEP_MOTOR_NO4);
    IPC_SrvStepMotorEmergencyStop(STEP_MOTOR_NO5);
    IPC_SrvStepMotorEmergencyStop(STEP_MOTOR_NO6);
    IPC_SrvStepMotorEmergencyStop(STEP_MOTOR_NO7);
    IPC_SrvStepMotorEmergencyStop(STEP_MOTOR_NO8);
    IPC_SrvStepMotorEmergencyStop(STEP_MOTOR_NO9);
    IPC_SrvStepMotorEmergencyStop(STEP_MOTOR_NO10);
    IPC_SrvStepMotorEmergencyStop(STEP_MOTOR_NO11);
    IPC_SrvStepMotorEmergencyStop(STEP_MOTOR_NO12);
    //IO急停
    SrvImplBaseUtilEmStopWithOutput();
}



//等待泵操作稳定
void ModLiquidUtilWaitPumpStable(void)
{
    CoreDelayMs(TIME_MS_WAIT_STABLE_PUMP);
}

//等待阀操作稳定
void ModLiquidUtilWaitValveStable(void)
{
    CoreDelayMs(TIME_MS_WAIT_STABLE_VALVE);
}


/***************************************操作互斥*************************************************/
//DP2打开计数器
static uint8_t dp2OpenCount = 0;
//DP305打开计数器
static uint8_t dp305OpenCount = 0;
//dp306打开计数器
static uint8_t dp306OpenCount = 0;
//V309打开计数器
static uint8_t v309OpenCount = 0;
//V232打开计数器
static uint8_t v232OpenCount = 0;

//打开宏定义
#define LIQUID_SYNC_LOCK_OPEN(lockCount,port,state)              uint32_t lastIntState = 0;/*申请锁定*/\
                                                                 lastIntState = CoreLockIntBasePri();/*进入互斥区间*/\
                                                                 lockCount++;/*锁增加*/\
                                                                 CoreUnlockIntBasePri(lastIntState);/*退出互斥*/\
                                                                 H360SysActPortOutWrite(port,state);/*写入想写的状态*/
//关闭宏定义
#define LIQUID_SYNC_UNLOCK_CLOSE(lockCount,port,state)           uint32_t lastIntState = 0;/*申请锁定*/\
                                                                 lastIntState = CoreLockIntBasePri();/*进入互斥区间*/\
                                                                 if(lockCount > 0)/*锁大于0*/\
                                                                 {\
                                                                     lockCount--;/*锁减少*/\
                                                                 }\
                                                                 CoreUnlockIntBasePri(lastIntState);/*退出互斥*/\
                                                                 if(lockCount == 0)/*锁清零了*/\
                                                                 {\
                                                                     H360SysActPortOutWrite(port,PIN_INVALID_LEVEL(state));\
                                                                 }

/***************************************************A液 B液 清洗针公用******************************************/
//打开DP2
void ModLiquidUtilOpenPumpDP2(void)
{
    LIQUID_SYNC_LOCK_OPEN(dp2OpenCount,OUTPUT_LIQUID_PUMP_DP2,VALID_LEVEL_OUTPUT_LIQUID_PUMP_DP2);
}

//关闭DP2
void ModLiquidUtilClosePumpDP2(void)
{
    LIQUID_SYNC_UNLOCK_CLOSE(dp2OpenCount,OUTPUT_LIQUID_PUMP_DP2,VALID_LEVEL_OUTPUT_LIQUID_PUMP_DP2);
}

/***************************************************缓冲液使用*************************************************/
//关闭V201
void ModLiquidUtilCloseValveV201(void)
{
    H360SysActPortOutWrite(OUTPUT_LIQUID_VALVE_V201,PIN_INVALID_LEVEL(VALID_LEVEL_OUTPUT_LIQUID_VALVE_V201));
}
//选择V201,1瓶
void ModLiquidUtilValveV201SelectBottle1(void)
{
    H360SysActPortOutWrite(OUTPUT_LIQUID_VALVE_V201,BOTTLE_1_OUTPUT_LIQUID_VALVE_V201);
}
//选择V201,2瓶
void ModLiquidUtilValveV201SelectBottle2(void)
{
    H360SysActPortOutWrite(OUTPUT_LIQUID_VALVE_V201,BOTTLE_2_OUTPUT_LIQUID_VALVE_V201);
}

//关闭V202,A液关闭注液,防止回流
void ModLiquidUtilCloseValveV202(void)
{
    H360SysActPortOutWrite(OUTPUT_LIQUID_VALVE_V202,PIN_INVALID_LEVEL(VALID_LEVEL_OUTPUT_LIQUID_VALVE_V202));
}
//打开V202,A液准备注液
void ModLiquidUtilOpenValveV202(void)
{
    H360SysActPortOutWrite(OUTPUT_LIQUID_VALVE_V202,VALID_LEVEL_OUTPUT_LIQUID_VALVE_V202);
}

//关闭V300,A液关闭注液
void ModLiquidUtilCloseValveV300(void)
{
    H360SysActPortOutWrite(OUTPUT_LIQUID_VALVE_V300,PIN_INVALID_LEVEL(VALID_LEVEL_OUTPUT_LIQUID_VALVE_V300));
}
//打开V300,A液准备注液
void ModLiquidUtilOpenValveV300(void)
{
    H360SysActPortOutWrite(OUTPUT_LIQUID_VALVE_V300,VALID_LEVEL_OUTPUT_LIQUID_VALVE_V300);
}

/*************************************************激发液使用**************************************************/
//打开V203
void ModLiquidUtilOpenValveV203(void)
{
    H360SysActPortOutWrite(OUTPUT_LIQUID_VALVE_V203,VALID_LEVEL_OUTPUT_LIQUID_VALVE_V203);
}
//关闭V203
void ModLiquidUtilCloseValveV203(void)
{
    H360SysActPortOutWrite(OUTPUT_LIQUID_VALVE_V203,PIN_INVALID_LEVEL(VALID_LEVEL_OUTPUT_LIQUID_VALVE_V203));
}
//选择V203,1瓶
void ModLiquidUtilValveV203SelectBottle1(void)
{
    H360SysActPortOutWrite(OUTPUT_LIQUID_VALVE_V203,BOTTLE_1_OUTPUT_LIQUID_VALVE_V203);
}
//选择V203,2瓶
void ModLiquidUtilValveV203SelectBottle2(void)
{
    H360SysActPortOutWrite(OUTPUT_LIQUID_VALVE_V203,BOTTLE_2_OUTPUT_LIQUID_VALVE_V203);
}

//关闭V204,B液关闭注液,防止回流
void ModLiquidUtilCloseValveV204(void)
{
    H360SysActPortOutWrite(OUTPUT_LIQUID_VALVE_V204,PIN_INVALID_LEVEL(VALID_LEVEL_OUTPUT_LIQUID_VALVE_V204));
}
//打开V204,B液准备注液
void ModLiquidUtilOpenValveV204(void)
{
    H360SysActPortOutWrite(OUTPUT_LIQUID_VALVE_V204,VALID_LEVEL_OUTPUT_LIQUID_VALVE_V204);
}

//打开V308
void ModLiquidUtilOpenValveV308(void)
{
    H360SysActPortOutWrite(OUTPUT_LIQUID_VALVE_V308,VALID_LEVEL_OUTPUT_LIQUID_VALVE_V308);
}
//关闭V308
void ModLiquidUtilCloseValveV308(void)
{
    H360SysActPortOutWrite(OUTPUT_LIQUID_VALVE_V308,PIN_INVALID_LEVEL(VALID_LEVEL_OUTPUT_LIQUID_VALVE_V308));
}

//打开V215
void ModLiquidUtilOpenValveV215(void)
{
    H360SysActPortOutWrite(OUTPUT_LIQUID_VALVE_V215,VALID_LEVEL_OUTPUT_LIQUID_VALVE_V215);
}
//关闭V215
void ModLiquidUtilCloseValveV215(void)
{
    H360SysActPortOutWrite(OUTPUT_LIQUID_VALVE_V215,PIN_INVALID_LEVEL(VALID_LEVEL_OUTPUT_LIQUID_VALVE_V215));
}

/**********************************************三针与清洗针公用***********************************************/
//打开DP305,内外针清洗针纯水泵
void ModLiquidUtilOpenPumpDP305(void)
{
    LIQUID_SYNC_LOCK_OPEN(dp305OpenCount,OUTPUT_LIQUID_PUMP_DP305,VALID_LEVEL_OUTPUT_LIQUID_PUMP_DP305);
}
//关闭DP305,内外针清洗针纯水泵
void ModLiquidUtilClosePumpDP305(void)
{
    LIQUID_SYNC_UNLOCK_CLOSE(dp305OpenCount,OUTPUT_LIQUID_PUMP_DP305,VALID_LEVEL_OUTPUT_LIQUID_PUMP_DP305);
}

//打开DP306,内外针清洗液泵
void ModLiquidUtilOpenPumpDP306(void)
{
    LIQUID_SYNC_LOCK_OPEN(dp306OpenCount,OUTPUT_LIQUID_PUMP_DP306,VALID_LEVEL_OUTPUT_LIQUID_PUMP_DP306);
}
//关闭DP306,内外针清洗液泵
void ModLiquidUtilClosePumpDP306(void)
{
    LIQUID_SYNC_UNLOCK_CLOSE(dp306OpenCount,OUTPUT_LIQUID_PUMP_DP306,VALID_LEVEL_OUTPUT_LIQUID_PUMP_DP306);
}

//关闭V309,三针内针三通阀
void ModLiquidUtilCloseValveV309(void)
{
    LIQUID_SYNC_UNLOCK_CLOSE(v309OpenCount,OUTPUT_LIQUID_VALVE_V309,VALID_LEVEL_OUTPUT_LIQUID_VALVE_V309);
}
//切换到正常运行状态,阀和清洗液导通
void ModLiquidUtilValveV309SwitchLiquidClean(void)
{
    LIQUID_SYNC_UNLOCK_CLOSE(v309OpenCount,OUTPUT_LIQUID_VALVE_V309,VALID_LEVEL_OUTPUT_LIQUID_VALVE_V309);
}
//切换到清洗维护状态,阀和纯水导通
void ModLiquidUtilValveV309SwitchLiquidPureWater(void)
{
    LIQUID_SYNC_LOCK_OPEN(v309OpenCount,OUTPUT_LIQUID_VALVE_V309,VALID_LEVEL_OUTPUT_LIQUID_VALVE_V309);
}

//关闭V232,三针外针三通阀
void ModLiquidUtilCloseValveV232(void)
{
    LIQUID_SYNC_UNLOCK_CLOSE(v232OpenCount,OUTPUT_LIQUID_VALVE_V232,VALID_LEVEL_OUTPUT_LIQUID_VALVE_V232);
}
//切换到正常运行状态,阀和纯水导通
void ModLiquidUtilValveV232SwitchPureWater(void)
{
    LIQUID_SYNC_UNLOCK_CLOSE(v232OpenCount,OUTPUT_LIQUID_VALVE_V232,VALID_LEVEL_OUTPUT_LIQUID_VALVE_V232);
}
//切换到清洗维护状态,阀和清洗液导通
void ModLiquidUtilValveV232SwitchLiquidClean(void)
{
    LIQUID_SYNC_LOCK_OPEN(v232OpenCount,OUTPUT_LIQUID_VALVE_V232,VALID_LEVEL_OUTPUT_LIQUID_VALVE_V232);
}


/*************************************************清洗针使用*****************************************************/
//关闭V301,针准备排液
void ModLiquidUtilCloseValveV301(void)
{
    H360SysActPortOutWrite(OUTPUT_LIQUID_VALVE_V301,PIN_INVALID_LEVEL(VALID_LEVEL_OUTPUT_LIQUID_VALVE_V301));
}
//打开V301,针准备吸液
void ModLiquidUtilOpenValveV301(void)
{
    H360SysActPortOutWrite(OUTPUT_LIQUID_VALVE_V301,VALID_LEVEL_OUTPUT_LIQUID_VALVE_V301);
}


//关闭V302,针准备排液
void ModLiquidUtilCloseValveV302(void)
{
    H360SysActPortOutWrite(OUTPUT_LIQUID_VALVE_V302,PIN_INVALID_LEVEL(VALID_LEVEL_OUTPUT_LIQUID_VALVE_V302));
}
//打开V302,针准备吸液
void ModLiquidUtilOpenValveV302(void)
{
    H360SysActPortOutWrite(OUTPUT_LIQUID_VALVE_V302,VALID_LEVEL_OUTPUT_LIQUID_VALVE_V302);
}


//关闭V303,针准备排液
void ModLiquidUtilCloseValveV303(void)
{
    H360SysActPortOutWrite(OUTPUT_LIQUID_VALVE_V303,PIN_INVALID_LEVEL(VALID_LEVEL_OUTPUT_LIQUID_VALVE_V303));
}
//打开V303,针准备吸液
void ModLiquidUtilOpenValveV303(void)
{
    H360SysActPortOutWrite(OUTPUT_LIQUID_VALVE_V303,VALID_LEVEL_OUTPUT_LIQUID_VALVE_V303);
}

//关闭V304,针准备排液
void ModLiquidUtilCloseValveV304(void)
{
    H360SysActPortOutWrite(OUTPUT_LIQUID_VALVE_V304,PIN_INVALID_LEVEL(VALID_LEVEL_OUTPUT_LIQUID_VALVE_V304));
}
//打开V304,针准备吸液
void ModLiquidUtilOpenValveV304(void)
{
    H360SysActPortOutWrite(OUTPUT_LIQUID_VALVE_V304,VALID_LEVEL_OUTPUT_LIQUID_VALVE_V304);
}


//关闭V305,针准备排液
void ModLiquidUtilCloseValveV305(void)
{
    H360SysActPortOutWrite(OUTPUT_LIQUID_VALVE_V305,PIN_INVALID_LEVEL(VALID_LEVEL_OUTPUT_LIQUID_VALVE_V305));
}
//打开V305,针准备吸液
void ModLiquidUtilOpenValveV305(void)
{
    H360SysActPortOutWrite(OUTPUT_LIQUID_VALVE_V305,VALID_LEVEL_OUTPUT_LIQUID_VALVE_V305);
}


//关闭V310
void ModLiquidUtilCloseValveV310(void)
{
    H360SysActPortOutWrite(OUTPUT_LIQUID_VALVE_V310,PIN_INVALID_LEVEL(VALID_LEVEL_OUTPUT_LIQUID_VALVE_V310));
}
//打开V310,清洗针导通清洗液
void ModLiquidUtilValveV310SwitchLiquidClean(void)
{
    H360SysActPortOutWrite(OUTPUT_LIQUID_VALVE_V310,VALID_LEVEL_OUTPUT_LIQUID_VALVE_V310);
}

//关闭V233
void ModLiquidUtilCloseValveV233(void)
{
    H360SysActPortOutWrite(OUTPUT_LIQUID_VALVE_V233,PIN_INVALID_LEVEL(VALID_LEVEL_OUTPUT_LIQUID_VALVE_V233));
}
//打开V233,清洗针导通纯水
void ModLiquidUtilValveV233SwitchPureWater(void)
{
    H360SysActPortOutWrite(OUTPUT_LIQUID_VALVE_V233,VALID_LEVEL_OUTPUT_LIQUID_VALVE_V233);
}


/*****************************************样本针****************************/
//打开V218
void ModLiquidUtilOpenValveV218(void)
{
    H360SysActPortOutWrite(OUTPUT_LIQUID_VALVE_V218,VALID_LEVEL_OUTPUT_LIQUID_VALVE_V218);
}
//关闭V218
void ModLiquidUtilCloseValveV218(void)
{
    H360SysActPortOutWrite(OUTPUT_LIQUID_VALVE_V218,PIN_INVALID_LEVEL(VALID_LEVEL_OUTPUT_LIQUID_VALVE_V218));
}

//打开V221
void ModLiquidUtilOpenValveV221(void)
{
    H360SysActPortOutWrite(OUTPUT_LIQUID_VALVE_V221,VALID_LEVEL_OUTPUT_LIQUID_VALVE_V221);
}   
//关闭V221
void ModLiquidUtilCloseValveV221(void)
{
    H360SysActPortOutWrite(OUTPUT_LIQUID_VALVE_V221,PIN_INVALID_LEVEL(VALID_LEVEL_OUTPUT_LIQUID_VALVE_V221));
}

/***************************************************磁珠针*****************************************/
//打开V220,磁珠针内壁清洗阀
void ModLiquidUtilOpenValveV220(void)
{
    H360SysActPortOutWrite(OUTPUT_LIQUID_VALVE_V220,VALID_LEVEL_OUTPUT_LIQUID_VALVE_V220);
}
//关闭V220
void ModLiquidUtilCloseValveV220(void)
{
    H360SysActPortOutWrite(OUTPUT_LIQUID_VALVE_V220,PIN_INVALID_LEVEL(VALID_LEVEL_OUTPUT_LIQUID_VALVE_V220));
}

//打开V217,磁珠针外壁清洗阀
void ModLiquidUtilOpenValveV217(void)
{
    H360SysActPortOutWrite(OUTPUT_LIQUID_VALVE_V217,VALID_LEVEL_OUTPUT_LIQUID_VALVE_V217);
}
//关闭V217
void ModLiquidUtilCloseValveV217(void)
{
    H360SysActPortOutWrite(OUTPUT_LIQUID_VALVE_V217,PIN_INVALID_LEVEL(VALID_LEVEL_OUTPUT_LIQUID_VALVE_V217));
}
/***************************************************试剂针*****************************************/
//打开V219,磁珠针内壁清洗阀
void ModLiquidUtilOpenValveV219(void)
{
    H360SysActPortOutWrite(OUTPUT_LIQUID_VALVE_V219,VALID_LEVEL_OUTPUT_LIQUID_VALVE_V219);
}
//关闭V219
void ModLiquidUtilCloseValveV219(void)
{
    H360SysActPortOutWrite(OUTPUT_LIQUID_VALVE_V219,PIN_INVALID_LEVEL(VALID_LEVEL_OUTPUT_LIQUID_VALVE_V219));
}

//打开V216,磁珠针外壁清洗阀
void ModLiquidUtilOpenValveV216(void)
{
    H360SysActPortOutWrite(OUTPUT_LIQUID_VALVE_V216,VALID_LEVEL_OUTPUT_LIQUID_VALVE_V216);
}
//关闭V216
void ModLiquidUtilCloseValveV216(void)
{
    H360SysActPortOutWrite(OUTPUT_LIQUID_VALVE_V216,PIN_INVALID_LEVEL(VALID_LEVEL_OUTPUT_LIQUID_VALVE_V216));
}





















