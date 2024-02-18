/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-04-26 10:18:30 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-05-05 14:36:30 +0800
************************************************************************************************/ 
#ifndef __BOARD_FPGA_STEP_MOTOR_H_
#define __BOARD_FPGA_STEP_MOTOR_H_
#include "BoardFPGA_Base.h"

/*******************************************定义步进电机操作函数类型*****************************************************/
//读取寄存器
//读取配置寄存器
typedef FPGA_STEP_MOTOR_REG_CONFIG (*BoardFPGA_StepMotorReadRegConfigFunc)(void); 
//读取编码器系数寄存器
typedef float (*BoardFPGA_StepMotorReadRegEncoderFactorFunc)(void); 
//读取定频运转速度寄存器
typedef uint32_t (*BoardFPGA_StepMotorReadRegFixSpdCntFunc)(void); 
//读取本次运行总步数寄存器
typedef uint32_t (*BoardFPGA_StepMotorReadRegTotalStepSetFunc)(void); 
//读取设置步数曲线的起始结束位置
typedef FPGA_STEP_MOTOR_REG_PARAM4 (*BoardFPGA_StepMotorReadRegTabSpdIndexRangeFunc)(void); 
//读取配置的停止条件寄存器
typedef FPGA_STEP_MOTOR_REG_PARAM5 (*BoardFPGA_StepMotorReadRegStopConditionFunc)(void);  
//读取当前控制寄存器
typedef FPGA_STEP_MOTOR_REG_CONTROL (*BoardFPGA_StepMotorReadRegControlFunc)(void); 
//读取已经发出的脉冲寄存器
typedef uint32_t (*BoardFPGA_StepMotorReadRegTotalStepSendOutFunc)(void); 
//读取当前编码器位置寄存器
typedef int32_t (*BoardFPGA_StepMotorReadRegEncoderPosFunc)(void); 
//读取当前信号状态和电机步数表索引序号寄存器
typedef FPGA_STEP_MOTOR_REG_STATE3 (*BoardFPGA_StepMotorReadRegSensorStateTabIndexFunc)(void); 
//读取电机状态寄存器
typedef FPGA_STEP_MOTOR_REG_STATE4 (*BoardFPGA_StepMotorReadRegMotorStateFunc)(void); 
//读取当前速度已经发送的脉冲寄存器
typedef uint32_t (*BoardFPGA_StepMotorReadRegCurTabIdxPluseSendOutFunc)(void); 
//读取速度表元素
typedef ERROR_SUB (*BoardFPGA_StepMotorReadSpdTabElemByIdxFunc)(uint16_t index,uint32_t* elemValPtr);
//读取步数表元素
typedef ERROR_SUB (*BoardFPGA_StepMotorReadStpTabElemByIdxFunc)(uint16_t index,uint32_t* elemValPtr);

//写入寄存器
//写入配置寄存器
typedef ERROR_SUB (*BoardFPGA_StepMotorWriteRegConfigFunc)(FPGA_STEP_MOTOR_REG_CONFIG regConfig);
//写入编码器系数寄存器
typedef ERROR_SUB (*BoardFPGA_StepMotorWriteRegEncoderFactorFunc)(float factor);
//写入定频速度计数值寄存器
typedef ERROR_SUB (*BoardFPGA_StepMotorWriteRegFixSpdCntFunc)(uint32_t fixSpdCnt);
//写入本次运行总步数寄存器
typedef ERROR_SUB (*BoardFPGA_StepMotorWriteRegTotalStepSetFunc)(uint32_t totalStep);
//设置步数曲线的起始结束位置
typedef ERROR_SUB (*BoardFPGA_StepMotorWriteRegTabSpdIndexRangeFunc)(FPGA_STEP_MOTOR_REG_PARAM4 tabRange);
//写入停止条件
typedef ERROR_SUB (*BoardFPGA_StepMotorWriteRegStopConditionFunc)(FPGA_STEP_MOTOR_REG_PARAM5 stopCondition);
//写入控制字
typedef ERROR_SUB (*BoardFPGA_StepMotorWriteRegControlFunc)(FPGA_STEP_MOTOR_REG_CONTROL regControl);
//写入速度表
typedef ERROR_SUB (*BoardFPGA_StepMotorWriteRegSpdTabElemFunc)(uint16_t index,uint32_t val);
//写入步数表
typedef ERROR_SUB (*BoardFPGA_StepMotorWriteRegStpTabElemFunc)(uint16_t index,uint32_t val);

/**********************************STEP_MOTOR对外开放API--基础*******************************************/
//读取寄存器
//读取配置寄存器
FPGA_STEP_MOTOR_REG_CONFIG BoardFPGA_StepMotorReadRegConfig(STEP_MOTOR_NO motorNo);
//读取编码器系数寄存器
float BoardFPGA_StepMotorReadRegEncoderFactor(STEP_MOTOR_NO motorNo);
//读取定频运转速度寄存器
uint32_t BoardFPGA_StepMotorReadRegFixSpdCnt(STEP_MOTOR_NO motorNo);
//读取本次运行总步数寄存器
uint32_t BoardFPGA_StepMotorReadRegTotalStepSet(STEP_MOTOR_NO motorNo);
//读取设置步数曲线的起始结束位置
FPGA_STEP_MOTOR_REG_PARAM4 BoardFPGA_StepMotorReadRegTabSpdIndexRange(STEP_MOTOR_NO motorNo);
//读取配置的停止条件寄存器
FPGA_STEP_MOTOR_REG_PARAM5 BoardFPGA_StepMotorReadRegStopCondition(STEP_MOTOR_NO motorNo);
//读取当前控制寄存器
FPGA_STEP_MOTOR_REG_CONTROL BoardFPGA_StepMotorReadRegControl(STEP_MOTOR_NO motorNo);
//读取已经发出的脉冲寄存器
uint32_t BoardFPGA_StepMotorReadRegTotalStepSendOut(STEP_MOTOR_NO motorNo);
//读取当前编码器位置寄存器
int32_t BoardFPGA_StepMotorReadRegEncoderPos(STEP_MOTOR_NO motorNo);
//读取当前信号状态和电机步数表索引序号寄存器
FPGA_STEP_MOTOR_REG_STATE3 BoardFPGA_StepMotorReadRegSensorStateTabIndex(STEP_MOTOR_NO motorNo);
//读取电机状态寄存器
FPGA_STEP_MOTOR_REG_STATE4 BoardFPGA_StepMotorReadRegMotorState(STEP_MOTOR_NO motorNo);
//读取当前速度已经发送的脉冲寄存器
uint32_t BoardFPGA_StepMotorReadRegCurTabIdxPluseSendOut(STEP_MOTOR_NO motorNo);
//读取速度表元素
ERROR_SUB BoardFPGA_StepMotorReadSpdTabElemByIdx(STEP_MOTOR_NO motorNo,uint16_t index,uint32_t* elemValPtr);
//读取步数表元素
ERROR_SUB BoardFPGA_StepMotorReadStpTabElemByIdx(STEP_MOTOR_NO motorNo,uint16_t index,uint32_t* elemValPtr);
//写入寄存器
//写入配置寄存器
ERROR_SUB BoardFPGA_StepMotorWriteRegConfig(STEP_MOTOR_NO motorNo,FPGA_STEP_MOTOR_REG_CONFIG regConfig);
//写入编码器系数寄存器
ERROR_SUB BoardFPGA_StepMotorWriteRegEncoderFactor(STEP_MOTOR_NO motorNo,float factor);
//写入定频速度计数值寄存器
ERROR_SUB BoardFPGA_StepMotorWriteRegFixSpdCnt(STEP_MOTOR_NO motorNo,uint32_t fixSpdCnt);
//写入本次运行总步数寄存器
ERROR_SUB BoardFPGA_StepMotorWriteRegTotalStepSet(STEP_MOTOR_NO motorNo,uint32_t totalStep);
//设置步数曲线的起始结束位置
ERROR_SUB BoardFPGA_StepMotorWriteRegTabSpdIndexRange(STEP_MOTOR_NO motorNo,FPGA_STEP_MOTOR_REG_PARAM4 tabRange);
//写入停止条件
ERROR_SUB BoardFPGA_StepMotorWriteRegStopCondition(STEP_MOTOR_NO motorNo,FPGA_STEP_MOTOR_REG_PARAM5 stopCondition);
//写入控制字
ERROR_SUB BoardFPGA_StepMotorWriteRegControl(STEP_MOTOR_NO motorNo,FPGA_STEP_MOTOR_REG_CONTROL regControl);
//写入速度表
ERROR_SUB BoardFPGA_StepMotorWriteRegSpdTabElem(STEP_MOTOR_NO motorNo,uint16_t index,uint32_t val);
//写入步数表
ERROR_SUB BoardFPGA_StepMotorWriteRegStpTabElem(STEP_MOTOR_NO motorNo,uint16_t index,uint32_t val);

/**********************************STEP_MOTOR对外开放API--集成*******************************************/
//获取电机全部寄存器状态
void BoardFPGA_StepMotorGetAllReg(STEP_MOTOR_NO motorNo,FPGA_STEP_MOTOR_REGS* allRegsPtr);
//获取电机状态寄存器
void BoardFPGA_StepMotorGetReadOnlyReg(STEP_MOTOR_NO motorNo,FPGA_STEP_MOTOR_REG_READONLY* readonlyRegsPtr);
//获取电机可读可写寄存器状态
void BoardFPGA_StepMotorGetWriteReadReg(STEP_MOTOR_NO motorNo,FPGA_STEP_MOTOR_REG_WRITE_READ* writeReadRegsPtr);
//读取电机中断状态
STEP_MOTOR_INT BoardFPGA_StepMotorGetIntSrc(void);
//写入电机配置
ERROR_SUB BoardFPGA_StepMotorWriteMotorAttri(STEP_MOTOR_NO motorNo,FPGA_MOTOR_ATTRI* attriPtr);
//清空电机速度步数表数据
ERROR_SUB BoardFPGA_StepMotorClearSpdStpTab(STEP_MOTOR_NO motorNo);
//电机使能
ERROR_SUB BoardFPGA_StepMotorEnable(STEP_MOTOR_NO motorNo);
//电机禁止
ERROR_SUB BoardFPGA_StepMotorDisable(STEP_MOTOR_NO motorNo);
//编码器使能
ERROR_SUB BoardFPGA_StepMotorEncoderEnable(STEP_MOTOR_NO motorNo);
//编码器禁止
ERROR_SUB BoardFPGA_StepMotorEncoderDisable(STEP_MOTOR_NO motorNo);
//写入编码器和电机控制状态
ERROR_SUB BoardFPGA_StepMotorAndEncoderState(STEP_MOTOR_NO motorNo,FunctionalState motorStateSet,FunctionalState encoderStateSet);
//编码器计数器清空
ERROR_SUB BoardFPGA_StepMotorEncoderPosClear(STEP_MOTOR_NO motorNo);
//步进电机急停
ERROR_SUB BoardFPGA_StepMotorCmdStop(STEP_MOTOR_NO motorNo,FPGA_STEP_MOTOR_REG_STATE4* motorStatePtrAfterStop);
//步进电机进入空闲状态
ERROR_SUB BoardFPGA_StepMotorConvertToIdle(STEP_MOTOR_NO motorNo);
//步进电机定频运转
ERROR_SUB BoardFPGA_StepMotorFixSpdRunStart(STEP_MOTOR_NO motorNo,FPGA_MOTOR_FIX_SPD_CMD* cmd);
//步进电机步数表运转
ERROR_SUB BoardFPGA_StepMotorTabSpdRunStart(STEP_MOTOR_NO motorNo,FPGA_MOTOR_TAB_SPD_CMD* cmd,FunctionalState writePreSet);
// //等待步进电机运行完成,同步的,状态查询
// ERROR_SUB BoardFPGA_StepMotorWaitStopSync(STEP_MOTOR_NO motorNo,uint32_t timeOutMs,FPGA_STEP_MOTOR_REG_STATE4* motorStateReg);
// //等待步进电机运行完成,异步的,消息处理
// ERROR_SUB BoardFPGA_StepMotorWaitStopAsync(STEP_MOTOR_NO motorNo,uint32_t timeOutMs);


// //电机测试配置
// ERROR_SUB BoardFPGA_StepMotorConfigTest(void);
// //电机测试定频运转
// ERROR_SUB BoardFPGA_StepMotorFixSpdTest(void);
// //电机测试步数表运转
// ERROR_SUB BoardFPGA_StepMotorTabSpdTest(void);
// //电机持续运转启动
// ERROR_SUB BoardFPGA_StepMotorTabSpdRunAlways(void);
// //电机急停模拟
// ERROR_SUB BoardFPGA_StepMotorTabSpdRunDacc(void);















#endif



