/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-04-26 10:15:43 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-05-10 16:25:33 +0800
************************************************************************************************/ 
#ifndef __BOARD_FPGA_TYPE_H_
#define __BOARD_FPGA_TYPE_H_
#include "BoardFPGA_Reg.h"


//FPGA版本号
typedef union FPGA_SOFT_VERSION
{
    uint32_t valueAll;
    struct
    {//位段
        //小端模式下least significant bits,这个是字节的最低位
        uint16_t lowWord     : 16;
        //大端模式下most significant bits,这个是字节的最低位
        uint16_t highWord    : 16;
    }valueBits;
}FPGA_SOFT_VERSION;

//端口有效电平,端口是高电平有效还是低电平有效
typedef union FPGA_PORT_VALID_LEVEL
{
    uint32_t valueAll;
    struct
    {//位段
        //小端模式下least significant bits,这个是字节的最低位
        GPIO_PinState pin0     : 1;
        GPIO_PinState pin1     : 1;
        GPIO_PinState pin2     : 1;
        GPIO_PinState pin3     : 1;
        GPIO_PinState pin4     : 1;
        GPIO_PinState pin5     : 1;
        GPIO_PinState pin6     : 1;
        GPIO_PinState pin7     : 1;
        GPIO_PinState pin8     : 1;
        GPIO_PinState pin9     : 1;
        GPIO_PinState pin10    : 1;
        GPIO_PinState pin11    : 1;
        GPIO_PinState pin12    : 1;
        GPIO_PinState pin13    : 1;
        GPIO_PinState pin14    : 1;
        GPIO_PinState pin15    : 1;
        GPIO_PinState pin16    : 1;
        GPIO_PinState pin17    : 1;
        GPIO_PinState pin18    : 1;
        GPIO_PinState pin19    : 1;
        GPIO_PinState pin20    : 1;
        GPIO_PinState pin21    : 1;
        GPIO_PinState pin22    : 1;
        GPIO_PinState pin23    : 1;
        GPIO_PinState pin24    : 1;
        GPIO_PinState pin25    : 1;
        GPIO_PinState pin26    : 1;
        GPIO_PinState pin27    : 1;
        GPIO_PinState pin28    : 1;
        GPIO_PinState pin29    : 1;
        GPIO_PinState pin30    : 1;
        //大端模式下most significant bits,这个是字节的最低位
        GPIO_PinState pin31    : 1;
    }valueBits;
}FPGA_PORT_VALID_LEVEL;

//输入端口状态,状态值是有效无效而不是高低电平
typedef union FPGA_PORT_IN_STATE
{
    uint32_t stateAll;
    struct
    {//位段
        //小端模式下least significant bits,这个是字节的最低位
        PIN_STATE pin0     : 1;
        PIN_STATE pin1     : 1;
        PIN_STATE pin2     : 1;
        PIN_STATE pin3     : 1;
        PIN_STATE pin4     : 1;
        PIN_STATE pin5     : 1;
        PIN_STATE pin6     : 1;
        PIN_STATE pin7     : 1;
        PIN_STATE pin8     : 1;
        PIN_STATE pin9     : 1;
        PIN_STATE pin10    : 1;
        PIN_STATE pin11    : 1;
        PIN_STATE pin12    : 1;
        PIN_STATE pin13    : 1;
        PIN_STATE pin14    : 1;
        PIN_STATE pin15    : 1;
        PIN_STATE pin16    : 1;
        PIN_STATE pin17    : 1;
        PIN_STATE pin18    : 1;
        PIN_STATE pin19    : 1;
        PIN_STATE pin20    : 1;
        PIN_STATE pin21    : 1;
        PIN_STATE pin22    : 1;
        PIN_STATE pin23    : 1;
        PIN_STATE pin24    : 1;
        PIN_STATE pin25    : 1;
        PIN_STATE pin26    : 1;
        PIN_STATE pin27    : 1;
        PIN_STATE pin28    : 1;
        PIN_STATE pin29    : 1;
        PIN_STATE pin30    : 1;
        //大端模式下most significant bits,这个是字节的最低位
        PIN_STATE pin31    : 1;
    }stateBits;
}FPGA_PORT_IN_STATE;

//设置输出端口值,输出有效还是无效,不是输出高低电平
typedef union FPGA_PORT_OUT_SET
{
    uint32_t valAll;
    struct
    {//位段
        //小端模式下least significant bits,这个是字节的最低位
        PIN_STATE pin0     : 1;
        PIN_STATE pin1     : 1;
        PIN_STATE pin2     : 1;
        PIN_STATE pin3     : 1;
        PIN_STATE pin4     : 1;
        PIN_STATE pin5     : 1;
        PIN_STATE pin6     : 1;
        PIN_STATE pin7     : 1;
        PIN_STATE pin8     : 1;
        PIN_STATE pin9     : 1;
        PIN_STATE pin10    : 1;
        PIN_STATE pin11    : 1;
        PIN_STATE pin12    : 1;
        PIN_STATE pin13    : 1;
        PIN_STATE pin14    : 1;
        PIN_STATE pin15    : 1;
        PIN_STATE pin16    : 1;
        PIN_STATE pin17    : 1;
        PIN_STATE pin18    : 1;
        PIN_STATE pin19    : 1;
        PIN_STATE pin20    : 1;
        PIN_STATE pin21    : 1;
        PIN_STATE pin22    : 1;
        PIN_STATE pin23    : 1;
        PIN_STATE pin24    : 1;
        PIN_STATE pin25    : 1;
        PIN_STATE pin26    : 1;
        PIN_STATE pin27    : 1;
        PIN_STATE pin28    : 1;
        PIN_STATE pin29    : 1;
        PIN_STATE pin30    : 1;
        //大端模式下most significant bits,这个是字节的最低位
        PIN_STATE pin31    : 1;
    }valBits;
}FPGA_PORT_OUT_SET;

//端口输出的真实值,是当前端口的电平值
typedef union FPGA_PORT_OUT_REALLY
{
    uint32_t valAll;
    struct
    {//位段
        //小端模式下least significant bits,这个是字节的最低位
        GPIO_PinState pin0     : 1;
        GPIO_PinState pin1     : 1;
        GPIO_PinState pin2     : 1;
        GPIO_PinState pin3     : 1;
        GPIO_PinState pin4     : 1;
        GPIO_PinState pin5     : 1;
        GPIO_PinState pin6     : 1;
        GPIO_PinState pin7     : 1;
        GPIO_PinState pin8     : 1;
        GPIO_PinState pin9     : 1;
        GPIO_PinState pin10    : 1;
        GPIO_PinState pin11    : 1;
        GPIO_PinState pin12    : 1;
        GPIO_PinState pin13    : 1;
        GPIO_PinState pin14    : 1;
        GPIO_PinState pin15    : 1;
        GPIO_PinState pin16    : 1;
        GPIO_PinState pin17    : 1;
        GPIO_PinState pin18    : 1;
        GPIO_PinState pin19    : 1;
        GPIO_PinState pin20    : 1;
        GPIO_PinState pin21    : 1;
        GPIO_PinState pin22    : 1;
        GPIO_PinState pin23    : 1;
        GPIO_PinState pin24    : 1;
        GPIO_PinState pin25    : 1;
        GPIO_PinState pin26    : 1;
        GPIO_PinState pin27    : 1;
        GPIO_PinState pin28    : 1;
        GPIO_PinState pin29    : 1;
        GPIO_PinState pin30    : 1;
        //大端模式下most significant bits,这个是字节的最低位
        GPIO_PinState pin31    : 1;
    }valBits;
}FPGA_PORT_OUT_REALLY;

//配置寄存器
typedef union FPGA_STEP_MOTOR_REG_CONFIG
{
    uint32_t valAll;
    struct
    {//位段
        //小端模式下least significant bits,这个是字节的最低位
        GPIO_PinState resetSensorValidLevel         : 1;
        GPIO_PinState emStopSensorValidLevel        : 1;
        GPIO_PinState utilStopSensorValidLevel     : 1;
        GPIO_PinState reserveBit                    : 1;
        GPIO_PinState motorEnableValidLevel         : 1;
        GPIO_PinState motorDirCwLevel               : 1;
        GPIO_PinState motorPluseIdleLevel           : 1;
        GPIO_PinState encoderValidLevelA            : 1;
        GPIO_PinState encoderValidLevelB            : 1;
        GPIO_PinState encoderValidLevelZ            : 1;
        //大端模式下most significant bits,这个是字节的最低位
        uint32_t regReserveVal                      : 22;
    }valBits;
}FPGA_STEP_MOTOR_REG_CONFIG;

//参数寄存器1
typedef union FPGA_STEP_MOTOR_REG_PARAM1
{
    float factorFloat;
    struct
    {
        uint32_t factorUint32    : 32;
    }valBits;
}FPGA_STEP_MOTOR_REG_PARAM1;

//参数寄存器4
typedef union FPGA_STEP_MOTOR_REG_PARAM4
{
    uint32_t valAll;
    struct
    {//位段
        //小端模式下least significant bits,这个是字节的最低位
        uint16_t tabSpdStartIdx    : 16;
        //大端模式下most significant bits,这个是字节的最低位
        uint16_t tabSpdStopIdx     : 16;
    }valBits;
}FPGA_STEP_MOTOR_REG_PARAM4;

//参数寄存器4
typedef union FPGA_STEP_MOTOR_REG_PARAM5
{
    uint32_t valAll;
    struct
    {//位段
        //小端模式下least significant bits,这个是字节的最低位
        FunctionalState resetSensorValidStop         : 1;
        FunctionalState resetSensorInvalidStop       : 1;
        FunctionalState emStopSensorValidStop        : 1;
        FunctionalState utilStopSensorValidStop      : 1;
        FunctionalState reserveBit                   : 1;
        FunctionalState pluseSendOverStop            : 1;
        //大端模式下most significant bits,这个是字节的最低位
        uint32_t reserveVal1                        : 26;
    }valBits;
}FPGA_STEP_MOTOR_REG_PARAM5;


//控制寄存器1
typedef union FPGA_STEP_MOTOR_REG_CONTROL
{
    uint32_t valAll;
    struct
    {//位段
        //小端模式下least significant bits,这个是字节的最低位
        FunctionalState encoderWorkStateSet     : 1;
        FunctionalState motorWorkStateSet       : 1;
        FunctionalState spdStpTablePreWriteSet  : 1;
        FunctionalState encoderPosClearSet      : 1;
        uint16_t reserveVal1                    : 12;
        //大端模式下most significant bits,这个是字节的最低位
        STEP_MOTOR_CMD motorCmd                 : 16;
    }valBits;
}FPGA_STEP_MOTOR_REG_CONTROL;

//状态寄存器3
typedef union FPGA_STEP_MOTOR_REG_STATE2
{
    uint32_t encoderPosUint32;
    struct
    {//位段
        //小端模式下least significant bits,这个是字节的最低位
        int32_t encoderPosInt32      : 32;
    }valBits;
}FPGA_STEP_MOTOR_REG_STATE2;

//状态寄存器3
typedef union FPGA_STEP_MOTOR_REG_STATE3
{
    uint32_t valAll;
    struct
    {//位段
        //小端模式下least significant bits,这个是字节的最低位
        PIN_STATE resetSensorState      : 1;
        PIN_STATE emStopSensorState     : 1;
        PIN_STATE utilStopSensorState   : 1;
        PIN_STATE reserveBit            : 1;
        uint16_t reserveVal1            : 12;
        //大端模式下most significant bits,这个是字节的最低位
        uint16_t tabIndexCurrent        : 16;
    }valBits;
}FPGA_STEP_MOTOR_REG_STATE3;

//状态寄存器4
typedef union FPGA_STEP_MOTOR_REG_STATE4
{
    uint32_t valAll;
    struct
    {//位段
        //小端模式下least significant bits,这个是字节的最低位
        STEP_MOTOR_STATE motorState      : 16;
        //大端模式下most significant bits,这个是字节的最低位
        STEP_MOTOR_STOP_REASON stopReason: 16;
    }valBits;
}FPGA_STEP_MOTOR_REG_STATE4;

//步进电机可读可写的寄存器列表
typedef struct FPGA_STEP_MOTOR_REG_WRITE_READ
{
    FPGA_STEP_MOTOR_REG_CONFIG motorConfig;//配置寄存器
    float encoderFactor;//编码器系数寄存器
    uint32_t fixSpdClkCnt;//定频输出的速度计数周期
    uint32_t totalPluseSet;//本次输出的总脉冲数
    FPGA_STEP_MOTOR_REG_PARAM4 spdTableIndex;//步数表输出的起始结束序号
    FPGA_STEP_MOTOR_REG_PARAM5 stopConditionSet;//停止条件设定
    FPGA_STEP_MOTOR_REG_CONTROL controlSet;
}FPGA_STEP_MOTOR_REG_WRITE_READ;

//步进电机只读的寄存器列表
typedef struct FPGA_STEP_MOTOR_REG_READONLY
{
    uint32_t motorPluseSendOutAll;//已经发出的脉冲数
    int32_t encoderCurrentPos;//编码器当前坐标
    FPGA_STEP_MOTOR_REG_STATE3 sensorStateAndTabIndexCurrent;
    FPGA_STEP_MOTOR_REG_STATE4 motorStateAndStopReason;
    uint32_t motorPluseSendOutCurrentIndex;//当前速度表项已发送脉冲
}FPGA_STEP_MOTOR_REG_READONLY;

//步进电机寄存器集合
typedef struct FPGA_STEP_MOTOR_REGS
{
    FPGA_STEP_MOTOR_REG_WRITE_READ writeReadRegs;
    FPGA_STEP_MOTOR_REG_READONLY readOnlyRegs;
}FPGA_STEP_MOTOR_REGS;

//电机中断源
typedef union STEP_MOTOR_INT
{
    uint32_t valAll;
    struct
    {//位段
        //小端模式下least significant bits,这个是字节的最低位
        ITStatus motor1     : 1;
        ITStatus motor2     : 1;
        ITStatus motor3     : 1;
        ITStatus motor4     : 1;
        ITStatus motor5     : 1;
        ITStatus motor6     : 1;
        ITStatus motor7     : 1;
        ITStatus motor8     : 1;
        ITStatus motor9     : 1;
        ITStatus motor10    : 1;
        ITStatus motor11    : 1;
        ITStatus motor12    : 1;
        ITStatus motor13    : 1;
        ITStatus motor14    : 1;
        ITStatus motor15    : 1;
        ITStatus motor16    : 1;
        ITStatus motor17    : 1;
        ITStatus motor18    : 1;
        ITStatus motor19    : 1;
        ITStatus motor20    : 1;
        ITStatus motor21    : 1;
        ITStatus motor22    : 1;
        ITStatus motor23    : 1;
        ITStatus motor24    : 1;
        ITStatus motor25    : 1;
        ITStatus motor26    : 1;
        ITStatus motor27    : 1;
        ITStatus motor28    : 1;
        ITStatus motor29    : 1;
        ITStatus motor30    : 1;
        ITStatus motor31    : 1;
        //大端模式下most significant bits,这个是字节的最低位
        ITStatus motor32    : 1;
    }valBits;
}STEP_MOTOR_INT;


//电机配置属性
typedef struct FPGA_MOTOR_ATTRI
{
    FPGA_STEP_MOTOR_REG_CONFIG configReg;
    float encoderFactor;
}FPGA_MOTOR_ATTRI;

//电机定频运动参数
typedef struct FPGA_MOTOR_FIX_SPD_CMD
{
    STEP_MOTOR_DIR motorDir                 ;//电机方向
    float rpmSet                            ;//设置转速
    uint32_t motorDiv                       ;//电机细分数
    uint32_t stepCount                      ;//输出步数
    FunctionalState clearEncoderPosSet      ;//清除编码器坐标
    FPGA_STEP_MOTOR_REG_PARAM5 stopCondition;//停止条件
}FPGA_MOTOR_FIX_SPD_CMD;

//电机表驱动运动参数
typedef struct FPGA_MOTOR_TAB_SPD_CMD
{
    STEP_MOTOR_DIR motorDir                 ;//电机方向
    uint32_t motorDiv                       ;//电机细分数
    float* rpmSetArrayPtr                   ;//速度表指针
    uint32_t* stepSetArrayPtr               ;//步数表指针
    uint16_t tableLength                    ;//表长度
    uint32_t stepCountTotal                 ;//总步数
    FunctionalState clearEncoderPosSet      ;//清除编码器坐标
    FPGA_STEP_MOTOR_REG_PARAM5 stopCondition;//停止条件
}FPGA_MOTOR_TAB_SPD_CMD;


//FPGA中断回调函数
typedef void (*BoardFPGA_StepMotorIntCallBack)(STEP_MOTOR_NO motorNo,STEP_MOTOR_STATE motorState,STEP_MOTOR_STOP_REASON stopReason);



#endif


