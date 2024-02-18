/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-04-26 10:19:09 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-05-06 14:07:25 +0800
************************************************************************************************/ 
#ifndef __BOARD_FPGA_MACRO_H_
#define __BOARD_FPGA_MACRO_H_
#include "DriverHeaderMCU.h"
#include "DriverConfigBoard.h"

//寄存器地址转换为FMC地址,因为FMC地址16位对其,所以寄存器地址要左移一位
#define BOARD_FPGA_CALC_ADDR(regaddr)           ((uint32_t *)(BOARD_FPGA_BASE_ADDR + (regaddr << 1)))
//电机rpm转换为速度计数值,(BOARD_FPGA_STEP_MOTOR_FREQ*0.3)/(rpm*div),0.3是rpm转rps系数 60->min-second 200->r-step 60/200=0.3
#define BOARD_FPGA_RPM_TO_CNT(rpm,div)          (uint32_t)((20000000.0f)/(((float)rpm)*((float)div)))
//速度计数值转换为电机rpm
#define BOARD_FPGA_CNT_TO_RPM(cnt,div)          (float)((20000000.0f)/(((float)cnt)*((float)div)))


//电机写入是否忙
typedef enum FPGA_WRITE_STATE
{
    FPGA_WRITE_STATE_IDLE   = 0X00,//写入空闲
    FPGA_WRITE_STATE_BUSY   = 0X01,//写入忙
}FPGA_WRITE_STATE;

//FPGA输入端口
typedef enum FPGA_IN_PORT
{
    FPGA_IN_PORT_A    = 0X00,//输入端口A
    FPGA_IN_PORT_B    = 0X01,//输入端口B
    FPGA_IN_PORT_C    = 0X02,//输入端口C
    FPGA_IN_PORT_D    = 0X03,//输入端口D
}FPGA_IN_PORT;

//FPGA输入端口引脚序号
typedef enum FPGA_IN_PIN
{
    FPGA_IN_PIN_0    = 0x00000001,
    FPGA_IN_PIN_1    = 0x00000002,
    FPGA_IN_PIN_2    = 0x00000004,
    FPGA_IN_PIN_3    = 0x00000008,
    FPGA_IN_PIN_4    = 0x00000010,
    FPGA_IN_PIN_5    = 0x00000020,
    FPGA_IN_PIN_6    = 0x00000040,
    FPGA_IN_PIN_7    = 0x00000080,
    FPGA_IN_PIN_8    = 0x00000100,
    FPGA_IN_PIN_9    = 0x00000200,
    FPGA_IN_PIN_10   = 0x00000400,
    FPGA_IN_PIN_11   = 0x00000800,
    FPGA_IN_PIN_12   = 0x00001000,
    FPGA_IN_PIN_13   = 0x00002000,
    FPGA_IN_PIN_14   = 0x00004000,
    FPGA_IN_PIN_15   = 0x00008000,
    FPGA_IN_PIN_16   = 0X00010000,
    FPGA_IN_PIN_17   = 0X00020000,
    FPGA_IN_PIN_18   = 0X00040000,
    FPGA_IN_PIN_19   = 0X00080000,
    FPGA_IN_PIN_20   = 0X00100000,
    FPGA_IN_PIN_21   = 0X00200000,
    FPGA_IN_PIN_22   = 0X00400000,
    FPGA_IN_PIN_23   = 0X00800000,
    FPGA_IN_PIN_24   = 0X01000000,
    FPGA_IN_PIN_25   = 0X02000000,
    FPGA_IN_PIN_26   = 0X04000000,
    FPGA_IN_PIN_27   = 0X08000000,
    FPGA_IN_PIN_28   = 0X10000000,
    FPGA_IN_PIN_29   = 0X20000000,
    FPGA_IN_PIN_30   = 0X40000000,
    FPGA_IN_PIN_31   = 0X80000000,
}FPGA_IN_PIN;

//FPGA输出端口
typedef enum FPGA_OUT_PORT
{
    FPGA_OUT_PORT_A   = 0X00,//输出端口A
    FPGA_OUT_PORT_B   = 0X01,//输出端口B
    FPGA_OUT_PORT_C   = 0X02,//输出端口C
    FPGA_OUT_PORT_D   = 0X03,//输出端口D
}FPGA_OUT_PORT;

//FPFA输出端口引脚序号
typedef enum FPGA_OUT_PIN
{
    FPGA_OUT_PIN_0    = 0x00000001,
    FPGA_OUT_PIN_1    = 0x00000002,
    FPGA_OUT_PIN_2    = 0x00000004,
    FPGA_OUT_PIN_3    = 0x00000008,
    FPGA_OUT_PIN_4    = 0x00000010,
    FPGA_OUT_PIN_5    = 0x00000020,
    FPGA_OUT_PIN_6    = 0x00000040,
    FPGA_OUT_PIN_7    = 0x00000080,
    FPGA_OUT_PIN_8    = 0x00000100,
    FPGA_OUT_PIN_9    = 0x00000200,
    FPGA_OUT_PIN_10   = 0x00000400,
    FPGA_OUT_PIN_11   = 0x00000800,
    FPGA_OUT_PIN_12   = 0x00001000,
    FPGA_OUT_PIN_13   = 0x00002000,
    FPGA_OUT_PIN_14   = 0x00004000,
    FPGA_OUT_PIN_15   = 0x00008000,
    FPGA_OUT_PIN_16   = 0X00010000,
    FPGA_OUT_PIN_17   = 0X00020000,
    FPGA_OUT_PIN_18   = 0X00040000,
    FPGA_OUT_PIN_19   = 0X00080000,
    FPGA_OUT_PIN_20   = 0X00100000,
    FPGA_OUT_PIN_21   = 0X00200000,
    FPGA_OUT_PIN_22   = 0X00400000,
    FPGA_OUT_PIN_23   = 0X00800000,
    FPGA_OUT_PIN_24   = 0X01000000,
    FPGA_OUT_PIN_25   = 0X02000000,
    FPGA_OUT_PIN_26   = 0X04000000,
    FPGA_OUT_PIN_27   = 0X08000000,
    FPGA_OUT_PIN_28   = 0X10000000,
    FPGA_OUT_PIN_29   = 0X20000000,
    FPGA_OUT_PIN_30   = 0X40000000,
    FPGA_OUT_PIN_31   = 0X80000000,
}FPGA_OUT_PIN;



#endif




