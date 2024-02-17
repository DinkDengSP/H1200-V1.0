/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-21 10:49:58 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-08-25 14:58:29 +0800
************************************************************************************************/ 
#ifndef __DRIVER_CONFIG_TIMER_H_
#define __DRIVER_CONFIG_TIMER_H_
#include "DriverConfigCore.h"

//总线到定时器主频,当APB1 APB2不是1的时候,定时器的频率为总线频率的两倍
#if(CORE_CLOCK_BUS_APB2 == RCC_HCLK_DIV2)
#define TIM_CLK_APB2_HZ                                 CORE_CLOCK_SYSTEM_MAIN_FREQ
#elif(CORE_CLOCK_BUS_APB2 == RCC_HCLK_DIV4)
#define TIM_CLK_APB2_HZ                                 (CORE_CLOCK_SYSTEM_MAIN_FREQ/2)
#elif(CORE_CLOCK_BUS_APB2 == RCC_HCLK_DIV8)
#define TIM_CLK_APB2_HZ                                 (CORE_CLOCK_SYSTEM_MAIN_FREQ/4)
#elif(CORE_CLOCK_BUS_APB2 == RCC_HCLK_DIV16)
#define TIM_CLK_APB2_HZ                                 (CORE_CLOCK_SYSTEM_MAIN_FREQ/8)
#endif

#if(CORE_CLOCK_BUS_APB1 == RCC_HCLK_DIV2)
#define TIM_CLK_APB1_HZ                                 CORE_CLOCK_SYSTEM_MAIN_FREQ
#elif(CORE_CLOCK_BUS_APB1 == RCC_HCLK_DIV4)
#define TIM_CLK_APB1_HZ                                 (CORE_CLOCK_SYSTEM_MAIN_FREQ/2)
#elif(CORE_CLOCK_BUS_APB1 == RCC_HCLK_DIV8)
#define TIM_CLK_APB1_HZ                                 (CORE_CLOCK_SYSTEM_MAIN_FREQ/4)
#elif(CORE_CLOCK_BUS_APB1 == RCC_HCLK_DIV16)
#define TIM_CLK_APB1_HZ                                 (CORE_CLOCK_SYSTEM_MAIN_FREQ/8)
#endif

//必须打开定时器内部回调
#if(USE_HAL_TIM_REGISTER_CALLBACKS == 0)
#error "Lib Timer Must Use CallBack FuncPtr !!!"
#endif

//TIMER 1 8 9 10 11的时钟来源是APB2总线
#define TIM1_CLK_SRC_HZ                                 TIM_CLK_APB2_HZ
#define TIM8_CLK_SRC_HZ                                 TIM_CLK_APB2_HZ
#define TIM9_CLK_SRC_HZ                                 TIM_CLK_APB2_HZ
#define TIM10_CLK_SRC_HZ                                TIM_CLK_APB2_HZ
#define TIM11_CLK_SRC_HZ                                TIM_CLK_APB2_HZ
//TIMER 2 3 4 5 6 7 12 13 14的时钟来源是APB1总线
#define TIM2_CLK_SRC_HZ                                 TIM_CLK_APB1_HZ
#define TIM3_CLK_SRC_HZ                                 TIM_CLK_APB1_HZ
#define TIM4_CLK_SRC_HZ                                 TIM_CLK_APB1_HZ
#define TIM5_CLK_SRC_HZ                                 TIM_CLK_APB1_HZ
#define TIM6_CLK_SRC_HZ                                 TIM_CLK_APB1_HZ
#define TIM7_CLK_SRC_HZ                                 TIM_CLK_APB1_HZ
#define TIM12_CLK_SRC_HZ                                TIM_CLK_APB1_HZ
#define TIM13_CLK_SRC_HZ                                TIM_CLK_APB1_HZ
#define TIM14_CLK_SRC_HZ                                TIM_CLK_APB1_HZ

//当APB1和APB2分频数为1的时候，TIM1、TIM8~TIM11的时钟为APB2的时钟,TIM2~TIM7、TIM12~TIM14的时钟为APB1的时钟
//而如果APB1和APB2分频数不为1，那么TIM1、TIM8~TIM11的时钟为APB2的时钟的两倍，TIM2~TIM7、TIM12~TIM14的时钟为APB1的时钟的两倍。
//因为系统初始化SystemInit函数里初始化APB1总线时钟为4分频即42M，APB2总线时钟为2分频即84M
//所以TIM1、TIM8~TIM11的时钟为APB2时钟的两倍即168M
//TIM2~TIM7、TIM12~TIM14的时钟为APB1的时钟的两倍即84M

/*------------------------------------------芯片基础定时器6------------------------------------*/
//TIMER6用于基础计时模式 16位计数器
#define MCU_TIMER6_MODE_BASE                            0
//TIMER6用于DAC触发模式
#define MCU_TIMER6_MODE_DAC                             1

//选择定时器使用模式,TIMER6同时用于DAC下溢中断和TIMER6中断,在DAC模式下,不会定义中断回调
#define MCU_TIMER6_MODE_USER                            MCU_TIMER6_MODE_BASE
//是否打印定时器中断讯息
#define MCU_TIMER6_INT_MSH_SHOW                         CONFIG_FUNC_DISABLE


/*------------------------------------------芯片基础定时器7------------------------------------*/
//TIMER7用于基础计时模式 16位计数器
#define MCU_TIMER7_MODE_BASE                            0
//TIMER7用于DAC触发模式
#define MCU_TIMER7_MODE_DAC                             1

//选择定时器使用模式
#define MCU_TIMER7_MODE_USER                            MCU_TIMER7_MODE_BASE
//是否打印定时器中断讯息
#define MCU_TIMER7_INT_MSH_SHOW                         CONFIG_FUNC_DISABLE

/********************************************TIMER1***************************************************/
//各个定时器对应通道的细分数,最大频率,计算公式
#define MCU_TIMER1_MOTOR_DRIVER_DIV                     8
//定义定时器输出的最大频率和最小频率,默认电机最大转速2002.5RPM,最小转速30rpm
//2002.5rpm下,对应rps为33.375,无细分情况下输出频率6675,带细分的情况下乘以细分数
#define MCU_TIMER1_FREQ_MAX                             (6675*MCU_TIMER1_MOTOR_DRIVER_DIV)   
//30RPM下,对应rps为0.5,无细分情况下输出频率100,带细分的情况下乘以细分数
#define MCU_TIMER1_FREQ_MIN                             (100*MCU_TIMER1_MOTOR_DRIVER_DIV)   
//168M分频数,没有细分的情况下输出100-6675频率,分频数为40,6675对应计数值629,100对应计数值42000
//细分变大,分频数相应减小 
#if(MCU_TIMER1_MOTOR_DRIVER_DIV > 40)
#define MCU_TIMER1_DIV                                  1 
#else
#define MCU_TIMER1_DIV                                  (40/MCU_TIMER1_MOTOR_DRIVER_DIV)  
#endif
//频率转换为重载计数值的基本计算系数,也就是定时器实际主频
#define MCU_TIMER1_FREQ_CALC_BASE                       (TIM1_CLK_SRC_HZ/MCU_TIMER1_DIV)
//输出IO
#define MCU_PIN_TIMER1_CH1_PWM                          MCU_PIN_E_9
/***********************************************************************************************/

/********************************************TIMER3***************************************************/
//各个定时器对应通道的细分数,最大频率,计算公式
#define MCU_TIMER3_MOTOR_DRIVER_DIV                     8
//定义定时器输出的最大频率和最小频率,默认电机最大转速2002.5RPM,最小转速30rpm
//2002.5rpm下,对应rps为33.375,无细分情况下输出频率6675,带细分的情况下乘以细分数
#define MCU_TIMER3_FREQ_MAX                             (6675*MCU_TIMER3_MOTOR_DRIVER_DIV)   
//30RPM下,对应rps为0.5,无细分情况下输出频率100,带细分的情况下乘以细分数
#define MCU_TIMER3_FREQ_MIN                             (100*MCU_TIMER3_MOTOR_DRIVER_DIV)   
//84M分频数,没有细分的情况下输出100-6675频率,分频数为20,6675对应计数值629,100对应计数值42000
//细分变大,分频数相应减小 
#if(MCU_TIMER3_MOTOR_DRIVER_DIV > 20)
#define MCU_TIMER3_DIV                                  1 
#else
#define MCU_TIMER3_DIV                                  (20/MCU_TIMER3_MOTOR_DRIVER_DIV)  
#endif 
//频率转换为重载计数值的基本计算系数,也就是定时器实际主频
#define MCU_TIMER3_FREQ_CALC_BASE                       (TIM3_CLK_SRC_HZ/MCU_TIMER3_DIV)
//输出IO
#define MCU_PIN_TIMER3_CH2_PWM                          MCU_PIN_C_7
/***********************************************************************************************/

/********************************************TIMER4***************************************************/
//各个定时器对应通道的细分数,最大频率,计算公式
#define MCU_TIMER4_MOTOR_DRIVER_DIV                     8
//定义定时器输出的最大频率和最小频率,默认电机最大转速2002.5RPM,最小转速30rpm
//2002.5rpm下,对应rps为33.375,无细分情况下输出频率6675,带细分的情况下乘以细分数
#define MCU_TIMER4_FREQ_MAX                             (6675*MCU_TIMER4_MOTOR_DRIVER_DIV)   
//30RPM下,对应rps为0.5,无细分情况下输出频率100,带细分的情况下乘以细分数
#define MCU_TIMER4_FREQ_MIN                             (100*MCU_TIMER4_MOTOR_DRIVER_DIV)   
//84M分频数,没有细分的情况下输出100-6675频率,分频数为20,6675对应计数值629,100对应计数值42000
//细分变大,分频数相应减小 
#if(MCU_TIMER4_MOTOR_DRIVER_DIV > 20)
#define MCU_TIMER4_DIV                                  1 
#else
#define MCU_TIMER4_DIV                                  (20/MCU_TIMER4_MOTOR_DRIVER_DIV)  
#endif 
//频率转换为重载计数值的基本计算系数,也就是定时器实际主频
#define MCU_TIMER4_FREQ_CALC_BASE                       (TIM4_CLK_SRC_HZ/MCU_TIMER4_DIV)
//输出IO
#define MCU_PIN_TIMER4_CH3_PWM                          MCU_PIN_D_14
/***********************************************************************************************/

/********************************************TIMER8***************************************************/
//各个定时器对应通道的细分数,最大频率,计算公式
#define MCU_TIMER8_MOTOR_DRIVER_DIV                     8
//定义定时器输出的最大频率和最小频率,默认电机最大转速2002.5RPM,最小转速30rpm
//2002.5rpm下,对应rps为33.375,无细分情况下输出频率6675,带细分的情况下乘以细分数
#define MCU_TIMER8_FREQ_MAX                             (6675*MCU_TIMER8_MOTOR_DRIVER_DIV)   
//30RPM下,对应rps为0.5,无细分情况下输出频率100,带细分的情况下乘以细分数
#define MCU_TIMER8_FREQ_MIN                             (100*MCU_TIMER8_MOTOR_DRIVER_DIV)   
//168M分频数,没有细分的情况下输出100-6675频率,分频数为40,6675对应计数值629,100对应计数值42000
//细分变大,分频数相应减小 
#if(MCU_TIMER8_MOTOR_DRIVER_DIV > 40)
#define MCU_TIMER8_DIV                                  1 
#else
#define MCU_TIMER8_DIV                                  (40/MCU_TIMER8_MOTOR_DRIVER_DIV)  
#endif
//频率转换为重载计数值的基本计算系数,也就是定时器实际主频
#define MCU_TIMER8_FREQ_CALC_BASE                       (TIM8_CLK_SRC_HZ/MCU_TIMER8_DIV)
//输出IO
#define MCU_PIN_TIMER8_CH2_PWM                          MCU_PIN_I_6
/***********************************************************************************************/

/********************************************TIMER9***************************************************/
//各个定时器对应通道的细分数,最大频率,计算公式
#define MCU_TIMER9_MOTOR_DRIVER_DIV                     8
//定义定时器输出的最大频率和最小频率,默认电机最大转速2002.5RPM,最小转速30rpm
//2002.5rpm下,对应rps为33.375,无细分情况下输出频率6675,带细分的情况下乘以细分数
#define MCU_TIMER9_FREQ_MAX                             (6675*MCU_TIMER9_MOTOR_DRIVER_DIV)   
//30RPM下,对应rps为0.5,无细分情况下输出频率100,带细分的情况下乘以细分数
#define MCU_TIMER9_FREQ_MIN                             (100*MCU_TIMER9_MOTOR_DRIVER_DIV)   
//168M分频数,没有细分的情况下输出100-6675频率,分频数为40,6675对应计数值629,100对应计数值42000
//细分变大,分频数相应减小 
#if(MCU_TIMER9_MOTOR_DRIVER_DIV > 40)
#define MCU_TIMER9_DIV                                  1 
#else
#define MCU_TIMER9_DIV                                  (40/MCU_TIMER9_MOTOR_DRIVER_DIV)  
#endif
//频率转换为重载计数值的基本计算系数,也就是定时器实际主频
#define MCU_TIMER9_FREQ_CALC_BASE                       (TIM9_CLK_SRC_HZ/MCU_TIMER9_DIV)
//输出IO
#define MCU_PIN_TIMER9_CH1_PWM                          MCU_PIN_E_5
/***********************************************************************************************/



/*-------------------------------------------编码器基础信号定义---------------------------------*/
//只在T1计数
#define MODE_MCU_TIMER_ENCODER_TI1                      0
//只在T2计数
#define MODE_MCU_TIMER_ENCODER_TI2                      1
//T1 T2均计数,4倍频模式
#define MODE_MCU_TIMER_ENCODER_TI12                     2
/***********************************************************************************************/


/*-------------------------------------------定时器2用作编码器接口-------------------------------*/
//编码器模式
#define MODE_MCU_TIMER2_ENCODER                         MODE_MCU_TIMER_ENCODER_TI12
//分频数,这个值影响计数,为2的时候,转一圈少一半,最小为1
#define PRESCALER_MCU_TIMER2_ENCODER                    1
//最大计数值,溢出值,定时器2是32位的,不是32位的定时器计数溢出值是UINT16_MAX
#define PERIOD_MCU_TIMER2_ENCODER                       UINT32_MAX
//初始化值
#define INIT_COUNT_MCU_TIMER2_ENCODER                   (PERIOD_MCU_TIMER2_ENCODER/2)
//编码器输入TI的连接通道,直连还是交叉,TI1和TI2必须相同
#define LINK_TI_MCU_TIMER2_ENCODER                      TIM_ICSELECTION_INDIRECTTI
//编码器输入TI的过滤
#define FILTER_TI_MCU_TIMER2_ENCODER                    5
//编码器输入TI分频,表示发生多少次事件产生一个捕获,降频
#define ICPCS_TI_MCU_TIMER2_ENCODER                     TIM_ICPSC_DIV1
//编码器输入TI1的极性
#define ICPOLARITY_TI1_MCU_TIMER2_ENCODER               TIM_ENCODERINPUTPOLARITY_RISING
//编码器输入TI2的极性
#define ICPOLARITY_TI2_MCU_TIMER2_ENCODER               TIM_ENCODERINPUTPOLARITY_RISING
//TI1端口
#define PIN_TI1_MCU_TIMER2_ENCODER                      MCU_PIN_A_0
//TI2端口
#define PIN_TI2_MCU_TIMER2_ENCODER                      MCU_PIN_A_1
//编码器原点信号Z
#define PIN_Z_SIGNAL_MCU_TIMER2_ENCODER                 MCU_PIN_A_2

/*-------------------------------------------定时器5用作编码器接口-------------------------------*/
//编码器模式
#define MODE_MCU_TIMER5_ENCODER                         MODE_MCU_TIMER_ENCODER_TI12
//分频数,这个值影响计数,为2的时候,转一圈少一半,最小为1
#define PRESCALER_MCU_TIMER5_ENCODER                    1
//最大计数值,溢出值,定时器2是32位的,不是32位的定时器计数溢出值是UINT16_MAX
#define PERIOD_MCU_TIMER5_ENCODER                       UINT32_MAX
//初始化值
#define INIT_COUNT_MCU_TIMER5_ENCODER                   (PERIOD_MCU_TIMER5_ENCODER/2)
//编码器输入TI的连接通道,直连还是交叉,TI1和TI2必须相同
#define LINK_TI_MCU_TIMER5_ENCODER                      TIM_ICSELECTION_DIRECTTI
//编码器输入TI的过滤
#define FILTER_TI_MCU_TIMER5_ENCODER                    5
//编码器输入TI分频,表示发生多少次事件产生一个捕获,降频
#define ICPCS_TI_MCU_TIMER5_ENCODER                     TIM_ICPSC_DIV1
//编码器输入TI1的极性
#define ICPOLARITY_TI1_MCU_TIMER5_ENCODER               TIM_ENCODERINPUTPOLARITY_RISING
//编码器输入TI2的极性
#define ICPOLARITY_TI2_MCU_TIMER5_ENCODER               TIM_ENCODERINPUTPOLARITY_RISING
//TI1端口
#define PIN_TI1_MCU_TIMER5_ENCODER                      MCU_PIN_H_10
//TI1端口
#define PIN_TI2_MCU_TIMER5_ENCODER                      MCU_PIN_H_11
//编码器原点信号Z
#define PIN_Z_SIGNAL_MCU_TIMER5_ENCODER                 MCU_PIN_H_12










#endif






















