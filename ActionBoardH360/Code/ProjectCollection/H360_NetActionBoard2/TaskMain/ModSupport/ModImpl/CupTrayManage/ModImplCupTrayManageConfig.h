/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-22 09:01:17 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-12-11 17:42:27 +0800
************************************************************************************************/ 
#ifndef __MOD_IMPL_CUP_TRAY_MANAGE_CONFIG_H_
#define __MOD_IMPL_CUP_TRAY_MANAGE_CONFIG_H_
#include "ModImplBase.h"


/***********************************************步进电机配置****************************************************/
//本机ID
//新杯托盘栈管理电机
#define CUP_TRAY_MANAGE_STEP_MOTOR_NEW_TRAY_UPDOWN_LOCAL_ID                         STEP_MOTOR_NO5
//空杯托盘栈管理电机                    
#define CUP_TRAY_MANAGE_STEP_MOTOR_EMPTY_TRAY_UPDOWN_LOCAL_ID                       STEP_MOTOR_NO6
//杯栈管理推杆                  
#define CUP_TRAY_MANAGE_STEP_MOTOR_PUSH_LOCAL_ID                                    STEP_MOTOR_NO3
//杯栈管理夹手                  
#define CUP_TRAY_MANAGE_STEP_MOTOR_CLAMP_LOCAL_ID                                   STEP_MOTOR_NO4

//系统ID
//新杯托盘栈管理电机
#define CUP_TRAY_MANAGE_STEP_MOTOR_NEW_TRAY_UPDOWN_SYS_ID                           H360_SYS_BOARD2_SM5
//空杯托盘栈管理电机                    
#define CUP_TRAY_MANAGE_STEP_MOTOR_EMPTY_TRAY_UPDOWN_SYS_ID                         H360_SYS_BOARD2_SM6
//杯栈管理推杆                  
#define CUP_TRAY_MANAGE_STEP_MOTOR_PUSH_SYS_ID                                      H360_SYS_BOARD2_SM3
//杯栈管理夹手                  
#define CUP_TRAY_MANAGE_STEP_MOTOR_CLAMP_SYS_ID                                     H360_SYS_BOARD2_SM4


/***********************************************输入讯号配置****************************************************/
//新杯栈电机原点
#define INPUT_CUP_TRAY_MANAGE_NEW_TRAY_UPDOWN_ORGIN                           H360_SYS_PORT_IN_BOARD2_ARM_SEN5_PE6
#define VALID_LEVEL_INPUT_CUP_TRAY_MANAGE_NEW_TRAY_UPDOWN_ORGIN               GPIO_PIN_RESET

//空杯栈电机原点
#define INPUT_CUP_TRAY_MANAGE_EMPTY_TRAY_UPDOWN_ORGIN                         H360_SYS_PORT_IN_BOARD2_ARM_SEN6_PE5
#define VALID_LEVEL_INPUT_CUP_TRAY_MANAGE_EMPTY_TRAY_UPDOWN_ORGIN             GPIO_PIN_RESET

//推杆电机原点
#define INPUT_CUP_TRAY_MANAGE_PUSH_ORGIN                                      H360_SYS_PORT_IN_BOARD2_ARM_SEN3_PC13
#define VALID_LEVEL_INPUT_CUP_TRAY_MANAGE_PUSH_ORGIN                          GPIO_PIN_RESET

//夹手电机原点
#define INPUT_CUP_TRAY_MANAGE_CLAMP_ORGIN                                     H360_SYS_PORT_IN_BOARD2_ARM_SEN4_PI8
#define VALID_LEVEL_INPUT_CUP_TRAY_MANAGE_CLAMP_ORGIN                         GPIO_PIN_SET

//杯栈管理门打开到位传感器
#define INPUT_CUP_TRAY_MANAGE_DOOR_OPEN_FULL                                        H360_SYS_PORT_IN_BOARD2_ARM_SEN11_PI6
//杯栈管理门打开到位传感器有效电平,代表门打开到位了
#define VALID_LEVEL_INPUT_CUP_TRAY_MANAGE_DOOR_OPEN_FULL                            GPIO_PIN_RESET

//新杯托盘栈上升按键,打开杯盘后才能按下的按键
#define INPUT_CUP_TRAY_MANAGE_NEW_TARY_UPDOWN_KEY                                   H360_SYS_PORT_IN_BOARD2_SLAVE_ARM_SEN11_PC1
//新杯托盘栈上升按键有效电平,代表按键按下   
#define VALID_LEVEL_INPUT_CUP_TRAY_MANAGE_NEW_TARY_UPDOWN_KEY                       GPIO_PIN_SET

//空杯托盘栈上升按键,打开杯盘后才能按下的按键   
#define INPUT_CUP_TRAY_MANAGE_EMPTY_TARY_UPDOWN_KEY                                 H360_SYS_PORT_IN_BOARD2_SLAVE_ARM_SEN12_PC2
//空杯托盘栈上升按键有效电平,代表按键按下   
#define VALID_LEVEL_INPUT_CUP_TRAY_MANAGE_EMPTY_TARY_UPDOWN_KEY                     GPIO_PIN_SET
      
//新杯托盘栈顶部检测有无传感器
#define INPUT_CUP_TRAY_MANAGE_NEW_TARY_CHECK_TOP_EXIST                              H360_SYS_PORT_IN_BOARD2_ARM_SEN15_PB8
//新杯托盘栈顶部检测有无传感器有效电平,代表托盘存在         
#define VALID_LEVEL_INPUT_CUP_TRAY_MANAGE_NEW_TARY_CHECK_TOP_EXIST                  GPIO_PIN_SET

//空杯托盘栈检测有无传感器          
#define INPUT_CUP_TRAY_MANAGE_EMPTY_TARY_CHECK_EXIST                                H360_SYS_PORT_IN_BOARD2_ARM_SEN13_PI4
//空杯托盘栈检测有无传感器有效电平,代表托盘存在         
#define VALID_LEVEL_INPUT_CUP_TRAY_MANAGE_EMPTY_TARY_CHECK_EXIST                    GPIO_PIN_SET

//测试位杯盘存在传感器          
#define INPUT_CUP_TRAY_MANAGE_TEST_TARY_CHECK_EXIST                                 H360_SYS_PORT_IN_BOARD2_ARM_SEN10_PI7
//测试位杯盘存在传感器有效电平,代表托盘存在         
#define VALID_LEVEL_INPUT_CUP_TRAY_MANAGE_TEST_TARY_CHECK_EXIST                     GPIO_PIN_RESET

//垃圾桶1存在传感器         
#define INPUT_CUP_TRAY_MANAGE_GARBAGE1_CHECK_EXIST                                  H360_SYS_PORT_IN_BOARD2_SLAVE_ARM_SEN7_PH6
//垃圾桶1存在传感器有效电平,代表垃圾桶存在          
#define VALID_LEVEL_INPUT_CUP_TRAY_MANAGE_GARBAGE1_CHECK_EXIST                      GPIO_PIN_SET

//垃圾桶2存在传感器         
#define INPUT_CUP_TRAY_MANAGE_GARBAGE2_CHECK_EXIST                                  H360_SYS_PORT_IN_BOARD2_SLAVE_ARM_SEN8_PB11
//垃圾桶2存在传感器有效电平,代表垃圾桶存在          
#define VALID_LEVEL_INPUT_CUP_TRAY_MANAGE_GARBAGE2_CHECK_EXIST                      GPIO_PIN_SET

//垃圾桶1过载传感器         
#define INPUT_CUP_TRAY_MANAGE_GARBAGE1_OVERLOAD                                     H360_SYS_PORT_IN_BOARD2_SLAVE_ARM_SEN6_PH7
//垃圾桶1过载传感器有效电平,代表已经过载            
#define VALID_LEVEL_INPUT_CUP_TRAY_MANAGE_GARBAGE1_OVERLOAD                         GPIO_PIN_RESET

//垃圾桶2过载传感器         
#define INPUT_CUP_TRAY_MANAGE_GARBAGE2_OVERLOAD                                     H360_SYS_PORT_IN_BOARD2_SLAVE_ARM_SEN5_PH8
//垃圾桶2过载传感器有效电平,代表已经过载            
#define VALID_LEVEL_INPUT_CUP_TRAY_MANAGE_GARBAGE2_OVERLOAD                         GPIO_PIN_RESET

/***********************************************输出讯号配置****************************************************/
//新杯盘栈上升按键灯
#define OUTPUT_CUP_TRAY_MANAGE_NEW_TARY_UPDOWN_KEY_LIGHT                            H360_SYS_PORT_OUT_BOARD2_SLAVE_SEN26_PH3
//新杯盘栈上升按键灯有效电平,点亮           
#define VALID_LEVEL_OUTPUT_CUP_TRAY_MANAGE_NEW_TARY_UPDOWN_KEY_LIGHT                GPIO_PIN_SET

//空杯盘栈上升按键灯            
#define OUTPUT_CUP_TRAY_MANAGE_EMPTY_TARY_UPDOWN_KEY_LIGHT                          H360_SYS_PORT_OUT_BOARD2_SLAVE_SEN27_PH2
//空杯盘栈上升按键灯有效电平,点亮           
#define VALID_LEVEL_OUTPUT_CUP_TRAY_MANAGE_EMPTY_TARY_UPDOWN_KEY_LIGHT              GPIO_PIN_SET

//垃圾桶1上升           
#define OUTPUT_CUP_TRAY_MANAGE_GARBAGE1_UP                                          H360_SYS_PORT_OUT_BOARD2_SLAVE_DC5_RUBBISH_MOTOR4_PI2
//垃圾桶1上升有效电平,上升有效          
#define VALID_LEVEL_OUTPUT_CUP_TRAY_MANAGE_GARBAGE1_UP                              GPIO_PIN_RESET

//垃圾桶1下降           
#define OUTPUT_CUP_TRAY_MANAGE_GARBAGE1_DOWN                                        H360_SYS_PORT_OUT_BOARD2_SLAVE_DC4_RUBBISH_MOTOR3_PI3
//垃圾桶1下降有效电平,下降有效          
#define VALID_LEVEL_OUTPUT_CUP_TRAY_MANAGE_GARBAGE1_DOWN                            GPIO_PIN_RESET

//垃圾桶2上升           
#define OUTPUT_CUP_TRAY_MANAGE_GARBAGE2_UP                                          H360_SYS_PORT_OUT_BOARD2_SLAVE_DC3_RUBBISH_MOTOR2_PD6
//垃圾桶2上升有效电平,上升有效          
#define VALID_LEVEL_OUTPUT_CUP_TRAY_MANAGE_GARBAGE2_UP                              GPIO_PIN_RESET

//垃圾桶2下降           
#define OUTPUT_CUP_TRAY_MANAGE_GARBAGE2_DOWN                                        H360_SYS_PORT_OUT_BOARD2_SLAVE_DC2_RUBBISH_MOTOR1_PG9
//垃圾桶2下降有效电平,下降有效          
#define VALID_LEVEL_OUTPUT_CUP_TRAY_MANAGE_GARBAGE2_DOWN                            GPIO_PIN_RESET

/***********************************************特殊配置****************************************************/
//新杯托盘栈的最大存量
#define NEW_TRAY_STACK_MAX_COUNT                                                    10
//空杯托盘栈的最大存量                                  
#define EMPTY_TRAY_STACK_MAX_COUNT                                                  9

//新杯栈维护按键按下升起来的层数                                    
#define TRAY_COUNT_NEW_STACK_KEY                                                    8
#define TRAY_COUNT_EMPTY_STACK_KEY                                                  8

/****************************************电机运行曲线配置***************************************************/
/*不同的目标位置,新杯盘栈升降的运转曲线 */
#define S_CURVE_CUP_TRAY_MANAGE_NEW_TRAY_DEBUG                                      SM_CURVE_0//调试曲线
#define S_CURVE_CUP_TRAY_MANAGE_NEW_TRAY_CHECK_COUNT                                SM_CURVE_1//初始化检测杯盘数量速度曲线
#define S_CURVE_CUP_TRAY_MANAGE_NEW_TRAY_UP                                         SM_CURVE_2//杯盘上升速度曲线
#define S_CURVE_CUP_TRAY_MANAGE_NEW_TRAY_DOWN                                       SM_CURVE_3//杯盘下降速度曲线
#define S_CURVE_CUP_TRAY_MANAGE_NEW_TRAY_ADJUST                                     SM_CURVE_4//杯盘调增速度曲线

/*不同的目标位置,空杯盘栈升降的运转曲线 */      
#define S_CURVE_CUP_TRAY_MANAGE_EMPTY_TRAY_DEBUG                                    SM_CURVE_0//调试曲线
#define S_CURVE_CUP_TRAY_MANAGE_EMPTY_TRAY_CHECK_COUNT                              SM_CURVE_1//初始化检测杯盘数量速度曲线
#define S_CURVE_CUP_TRAY_MANAGE_EMPTY_TRAY_UP                                       SM_CURVE_2//杯盘上升速度曲线
#define S_CURVE_CUP_TRAY_MANAGE_EMPTY_TRAY_DOWN                                     SM_CURVE_3//杯盘下降速度曲线
#define S_CURVE_CUP_TRAY_MANAGE_EMPTY_TRAY_ADJUST                                   SM_CURVE_4//杯盘调增速度曲线


/*不同的目标位置,推杆的运转曲线 */      
#define S_CURVE_CUP_TRAY_MANAGE_PUSH_DEBUG                                          SM_CURVE_0//调试曲线
#define S_CURVE_CUP_TRAY_MANAGE_PUSH_FAST                                           SM_CURVE_1//推杆快速推进
#define S_CURVE_CUP_TRAY_MANAGE_PUSH_SLOW                                           SM_CURVE_2//推杆慢速推进
#define S_CURVE_CUP_TRAY_MANAGE_PUSH_RETURN                                         SM_CURVE_3//推杆回零


/*不同的目标位置,夹手的运转曲线 */      
#define S_CURVE_CUP_TRAY_MANAGE_CLAMP_DEBUG                                         SM_CURVE_0//调试曲线
#define S_CURVE_CUP_TRAY_MANAGE_CLAMP_CLOSE                                         SM_CURVE_1//夹紧速度
#define S_CURVE_CUP_TRAY_MANAGE_CLAMP_OPEN                                          SM_CURVE_2//释放速度
#define S_CURVE_CUP_TRAY_MANAGE_CLAMP_RETURN_ZERO                                   SM_CURVE_3//回零速度

/********************************************运行中等待时间配置*****************************************************/
//垃圾桶升降时间
#define TIME_COUNT_CUP_TRAY_MANAGE_GARBAGE_UPDOWN                                   (10000/MOD_TASK_CUP_TRAY_MANAGE_FSM_PERIOD_MS)
//新杯栈上升的稳定时间              
#define TIMS_MS_WAIT_NEW_TRAY_STACK_STABLE                                          1000
//空杯栈回收的稳定时间              
#define TIMS_MS_WAIT_EMPTY_TRAY_STACK_STABLE                                        700
//推手推进等待稳定时间              
#define TIMS_MS_WAIT_PUSH_TRAY_STABLE                                               100
//空杯栈回收下降时间                
#define TIMS_MS_WAIT_EMPTY_TRAY_STACK_DOWN                                          200
/********************************************固定的参数配置*****************************************************/
//推杆二次推进补充偏移
#define CUP_TRAY_MANAGE_OFFSET_PUSH_TRAY_TIGHT_SECOND                               50


#endif


















