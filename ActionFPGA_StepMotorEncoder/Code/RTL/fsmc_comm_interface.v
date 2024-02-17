/************************************************************************ 
 ** Author       : 邓小俊
 ** Date         : 2024-01-19 13:30:05
 ** Description  : When I Has Free Time ,I Will Write Description Below.
 ** description  : 
 ** LastEditors  : 邓小俊
 ** LastEditTime : 2024-01-22 12:53:58
 ** Email: xiaojun.deng@aliyun.com
 ** Copyright (c) 2024 by 公司名称, All Rights Reserved. 
 ************************************************************************/
module fsmc_comm_interface 
#(
    //输入主频
    parameter SYSTEM_CLK_FREQ = 32'd70_000_000,
    //写入信号的保持时间,时钟周期
    parameter WRITE_SINGLE_HOLD_TIME_CYCLE = 16'd3
) 
(
    input           sys_clk  ,//输入时钟
    input           sys_rst_n,//复位,低电平有效
    //FSMC通讯接口
    input           fsmc_comm_nwe   ,//写入信号
    input           fsmc_comm_noe   ,//读取信号
    input           fsmc_comm_ne    ,//片选信号
    input[15:0]     fsmc_comm_addr  ,//输入地址
    inout[15:0]     fsmc_comm_data  ,//输入输出数据
    //输出写信号
    output reg     fsmc_write_singal,//写信号
    output reg[31:0] fsmc_write_addr,//写地址
    output reg[31:0] fsmc_write_data,//写数据
    //输出中断
    output wire[3:0] motor_int_out,
    //IO口输出寄存器
    input[((3*32)-1):0]   port_out_group_a_readable_regs,
    //IO口输出寄存器
    input[((3*32)-1):0]   port_out_group_b_readable_regs,
    //IO口输出寄存器
    input[((3*32)-1):0]   port_out_group_c_readable_regs,
    //IO口输出寄存器
    input[((3*32)-1):0]   port_out_group_d_readable_regs,
    //IO口输入寄存器
    input[((2*32)-1):0]   port_in_group_a_readable_regs,
    //IO口输入寄存器
    input[((2*32)-1):0]   port_in_group_b_readable_regs,
    //IO口输入寄存器
    input[((2*32)-1):0]   port_in_group_c_readable_regs,
    //IO口输入寄存器
    input[((2*32)-1):0]   port_in_group_d_readable_regs,
    //步进电机1导入寄存器
    input[((14*32)):0]   step_motor_1_readable_regs,
    //步进电机2导入寄存器
    input[((14*32)):0]   step_motor_2_readable_regs,
    //步进电机3导入寄存器
    input[((14*32)):0]   step_motor_3_readable_regs,
    //步进电机4导入寄存器
    input[((14*32)):0]   step_motor_4_readable_regs,
    //步进电机5导入寄存器
    input[((14*32)):0]   step_motor_5_readable_regs,
    //步进电机6导入寄存器
    input[((14*32)):0]   step_motor_6_readable_regs,
    //步进电机7导入寄存器
    input[((14*32)):0]   step_motor_7_readable_regs,
    //步进电机8导入寄存器
    input[((14*32)):0]   step_motor_8_readable_regs,
    //步进电机9导入寄存器
    input[((14*32)):0]   step_motor_9_readable_regs,
    //步进电机10导入寄存器
    input[((14*32)):0]   step_motor_10_readable_regs,
    //步进电机11导入寄存器
    input[((14*32)):0]   step_motor_11_readable_regs,
    //步进电机12导入寄存器
    input[((14*32)):0]   step_motor_12_readable_regs,
    //步进电机13导入寄存器
    input[((14*32)):0]   step_motor_13_readable_regs,
    //步进电机14导入寄存器
    input[((14*32)):0]   step_motor_14_readable_regs,
    //步进电机15导入寄存器
    input[((14*32)):0]   step_motor_15_readable_regs,
    //步进电机16导入寄存器
    input[((14*32)):0]   step_motor_16_readable_regs
);

/*********************************参数定义*******************************************/
//识别ID        
localparam FSMC_FPGA_CHIP_ID_LOW                                = 16'h1121;
localparam FSMC_FPGA_CHIP_ID_HIGH                               = 16'h1989;
//版本号        
localparam FSMC_FPGA_SOFT_VERSION_SUB                           = 16'd0002;
localparam FSMC_FPGA_SOFT_VERSION_MAIN                          = 16'd0001;

//读取寄存器地址定义
//读取设备版本号
localparam READ_ADDR_FPGA_SOFT_VERSION_LOW                      = 16'h0000;
localparam READ_ADDR_FPGA_SOFT_VERSION_HIGH                     = 16'h0001;
//电机中断寄存器
localparam READ_ADDR_STEP_MOTOR_INT_LOW                         = 16'h0002;
localparam READ_ADDR_STEP_MOTOR_INT_HIGH                        = 16'h0003;

//输入端口A模式寄存器
localparam REG_READ_ADDR_PORT_IN_GROUP_A_MODE_LOW               = 16'h0020;
localparam REG_READ_ADDR_PORT_IN_GROUP_A_MODE_HIGH              = 16'h0021;
//输入端口A状态寄存器
localparam REG_READ_ADDR_PORT_IN_GROUP_A_STATE_LOW              = 16'h0022;
localparam REG_READ_ADDR_PORT_IN_GROUP_A_STATE_HIGH             = 16'h0023;

//输入端口B模式寄存器
localparam REG_READ_ADDR_PORT_IN_GROUP_B_MODE_LOW               = 16'h0030;
localparam REG_READ_ADDR_PORT_IN_GROUP_B_MODE_HIGH              = 16'h0031;
//输入端口B状态寄存器
localparam REG_READ_ADDR_PORT_IN_GROUP_B_STATE_LOW              = 16'h0032;
localparam REG_READ_ADDR_PORT_IN_GROUP_B_STATE_HIGH             = 16'h0033;

//输入端口C模式寄存器
localparam REG_READ_ADDR_PORT_IN_GROUP_C_MODE_LOW               = 16'h0040;
localparam REG_READ_ADDR_PORT_IN_GROUP_C_MODE_HIGH              = 16'h0041;
//输入端口C状态寄存器
localparam REG_READ_ADDR_PORT_IN_GROUP_C_STATE_LOW              = 16'h0042;
localparam REG_READ_ADDR_PORT_IN_GROUP_C_STATE_HIGH             = 16'h0043;

//输入端口D模式寄存器
localparam REG_READ_ADDR_PORT_IN_GROUP_D_MODE_LOW               = 16'h0050;
localparam REG_READ_ADDR_PORT_IN_GROUP_D_MODE_HIGH              = 16'h0051;
//输入端口D状态寄存器
localparam REG_READ_ADDR_PORT_IN_GROUP_D_STATE_LOW              = 16'h0052;
localparam REG_READ_ADDR_PORT_IN_GROUP_D_STATE_HIGH             = 16'h0053;

//输出端口A模式寄存器
localparam REG_READ_ADDR_PORT_OUT_GROUP_A_MODE_LOW              = 16'h0100;
localparam REG_READ_ADDR_PORT_OUT_GROUP_A_MODE_HIGH             = 16'h0101;
//输出端口A状态寄存器
localparam REG_READ_ADDR_PORT_OUT_GROUP_A_SET_LOW               = 16'h0102;
localparam REG_READ_ADDR_PORT_OUT_GROUP_A_SET_HIGH              = 16'h0103;
//输出端口A实际输出电平寄存器
localparam REG_READ_ADDR_PORT_OUT_GROUP_A_REAL_LOW              = 16'h0104;
localparam REG_READ_ADDR_PORT_OUT_GROUP_A_REAL_HIGH             = 16'h0105;

//输出端口B模式寄存器
localparam REG_READ_ADDR_PORT_OUT_GROUP_B_MODE_LOW              = 16'h0110;
localparam REG_READ_ADDR_PORT_OUT_GROUP_B_MODE_HIGH             = 16'h0111;
//输出端口B状态寄存器
localparam REG_READ_ADDR_PORT_OUT_GROUP_B_SET_LOW               = 16'h0112;
localparam REG_READ_ADDR_PORT_OUT_GROUP_B_SET_HIGH              = 16'h0113;
//输出端口B实际输出电平寄存器
localparam REG_READ_ADDR_PORT_OUT_GROUP_B_REAL_LOW              = 16'h0114;
localparam REG_READ_ADDR_PORT_OUT_GROUP_B_REAL_HIGH             = 16'h0115;

//输出端口C模式寄存器
localparam REG_READ_ADDR_PORT_OUT_GROUP_C_MODE_LOW              = 16'h0120;
localparam REG_READ_ADDR_PORT_OUT_GROUP_C_MODE_HIGH             = 16'h0121;
//输出端口C状态寄存器
localparam REG_READ_ADDR_PORT_OUT_GROUP_C_SET_LOW               = 16'h0122;
localparam REG_READ_ADDR_PORT_OUT_GROUP_C_SET_HIGH              = 16'h0123;
//输出端口C实际输出电平寄存器
localparam REG_READ_ADDR_PORT_OUT_GROUP_C_REAL_LOW              = 16'h0124;
localparam REG_READ_ADDR_PORT_OUT_GROUP_C_REAL_HIGH             = 16'h0125;

//输出端口D模式寄存器
localparam REG_READ_ADDR_PORT_OUT_GROUP_D_MODE_LOW              = 16'h0130;
localparam REG_READ_ADDR_PORT_OUT_GROUP_D_MODE_HIGH             = 16'h0131;
//输出端口D状态寄存器
localparam REG_READ_ADDR_PORT_OUT_GROUP_D_SET_LOW               = 16'h0132;
localparam REG_READ_ADDR_PORT_OUT_GROUP_D_SET_HIGH              = 16'h0133;
//输出端口D实际输出电平寄存器
localparam REG_READ_ADDR_PORT_OUT_GROUP_D_REAL_LOW              = 16'h0134;
localparam REG_READ_ADDR_PORT_OUT_GROUP_D_REAL_HIGH             = 16'h0135;

//步进电机1配置寄存器
//配置寄存器
localparam REG_READ_ADDR_STEP_MOTOR1_CONFIG_1_LOW               = 16'h0200;
localparam REG_READ_ADDR_STEP_MOTOR1_CONFIG_1_HIGH              = 16'h0201;
//参数寄存器
localparam REG_READ_ADDR_STEP_MOTOR1_PARAM1_LOW                 = 16'h020a;
localparam REG_READ_ADDR_STEP_MOTOR1_PARAM1_HIGH                = 16'h020b;
localparam REG_READ_ADDR_STEP_MOTOR1_PARAM2_LOW                 = 16'h020c;
localparam REG_READ_ADDR_STEP_MOTOR1_PARAM2_HIGH                = 16'h020d;
localparam REG_READ_ADDR_STEP_MOTOR1_PARAM3_LOW                 = 16'h020e;
localparam REG_READ_ADDR_STEP_MOTOR1_PARAM3_HIGH                = 16'h020f;
localparam REG_READ_ADDR_STEP_MOTOR1_PARAM4_LOW                 = 16'h0210;
localparam REG_READ_ADDR_STEP_MOTOR1_PARAM4_HIGH                = 16'h0211;
localparam REG_READ_ADDR_STEP_MOTOR1_PARAM5_LOW                 = 16'h0212;
localparam REG_READ_ADDR_STEP_MOTOR1_PARAM5_HIGH                = 16'h0213;
//控制寄存器
localparam REG_READ_ADDR_STEP_MOTOR1_CONTROL1_LOW               = 16'h0230;
localparam REG_READ_ADDR_STEP_MOTOR1_CONTROL1_HIGH              = 16'h0231;
//状态寄存器
localparam REG_READ_ADDR_STEP_MOTOR1_STATE1_LOW                 = 16'h0250;
localparam REG_READ_ADDR_STEP_MOTOR1_STATE1_HIGH                = 16'h0251;
localparam REG_READ_ADDR_STEP_MOTOR1_STATE2_LOW                 = 16'h0252;
localparam REG_READ_ADDR_STEP_MOTOR1_STATE2_HIGH                = 16'h0253;
localparam REG_READ_ADDR_STEP_MOTOR1_STATE3_LOW                 = 16'h0254;
localparam REG_READ_ADDR_STEP_MOTOR1_STATE3_HIGH                = 16'h0255;
localparam REG_READ_ADDR_STEP_MOTOR1_STATE4_LOW                 = 16'h0256;
localparam REG_READ_ADDR_STEP_MOTOR1_STATE4_HIGH                = 16'h0257;
localparam REG_READ_ADDR_STEP_MOTOR1_STATE5_LOW                 = 16'h0258;
localparam REG_READ_ADDR_STEP_MOTOR1_STATE5_HIGH                = 16'h0259;
localparam REG_READ_ADDR_STEP_MOTOR1_SPD_TAB_ELEM_LOW           = 16'h025a;
localparam REG_READ_ADDR_STEP_MOTOR1_SPD_TAB_ELEM_HIGH          = 16'h025b;
localparam REG_READ_ADDR_STEP_MOTOR1_STP_TAB_ELEM_LOW           = 16'h025c;
localparam REG_READ_ADDR_STEP_MOTOR1_STP_TAB_ELEM_HIGH          = 16'h025d;


//步进电机2配置寄存器
//配置寄存器
localparam REG_READ_ADDR_STEP_MOTOR2_CONFIG_1_LOW               = 16'h0300;
localparam REG_READ_ADDR_STEP_MOTOR2_CONFIG_1_HIGH              = 16'h0301;
//参数寄存器
localparam REG_READ_ADDR_STEP_MOTOR2_PARAM1_LOW                 = 16'h030a;
localparam REG_READ_ADDR_STEP_MOTOR2_PARAM1_HIGH                = 16'h030b;
localparam REG_READ_ADDR_STEP_MOTOR2_PARAM2_LOW                 = 16'h030c;
localparam REG_READ_ADDR_STEP_MOTOR2_PARAM2_HIGH                = 16'h030d;
localparam REG_READ_ADDR_STEP_MOTOR2_PARAM3_LOW                 = 16'h030e;
localparam REG_READ_ADDR_STEP_MOTOR2_PARAM3_HIGH                = 16'h030f;
localparam REG_READ_ADDR_STEP_MOTOR2_PARAM4_LOW                 = 16'h0310;
localparam REG_READ_ADDR_STEP_MOTOR2_PARAM4_HIGH                = 16'h0311;
localparam REG_READ_ADDR_STEP_MOTOR2_PARAM5_LOW                 = 16'h0312;
localparam REG_READ_ADDR_STEP_MOTOR2_PARAM5_HIGH                = 16'h0313;
//控制寄存器
localparam REG_READ_ADDR_STEP_MOTOR2_CONTROL1_LOW               = 16'h0330;
localparam REG_READ_ADDR_STEP_MOTOR2_CONTROL1_HIGH              = 16'h0331;
//状态寄存器
localparam REG_READ_ADDR_STEP_MOTOR2_STATE1_LOW                 = 16'h0350;
localparam REG_READ_ADDR_STEP_MOTOR2_STATE1_HIGH                = 16'h0351;
localparam REG_READ_ADDR_STEP_MOTOR2_STATE2_LOW                 = 16'h0352;
localparam REG_READ_ADDR_STEP_MOTOR2_STATE2_HIGH                = 16'h0353;
localparam REG_READ_ADDR_STEP_MOTOR2_STATE3_LOW                 = 16'h0354;
localparam REG_READ_ADDR_STEP_MOTOR2_STATE3_HIGH                = 16'h0355;
localparam REG_READ_ADDR_STEP_MOTOR2_STATE4_LOW                 = 16'h0356;
localparam REG_READ_ADDR_STEP_MOTOR2_STATE4_HIGH                = 16'h0357;
localparam REG_READ_ADDR_STEP_MOTOR2_STATE5_LOW                 = 16'h0358;
localparam REG_READ_ADDR_STEP_MOTOR2_STATE5_HIGH                = 16'h0359;
localparam REG_READ_ADDR_STEP_MOTOR2_SPD_TAB_ELEM_LOW           = 16'h035a;
localparam REG_READ_ADDR_STEP_MOTOR2_SPD_TAB_ELEM_HIGH          = 16'h035b;
localparam REG_READ_ADDR_STEP_MOTOR2_STP_TAB_ELEM_LOW           = 16'h035c;
localparam REG_READ_ADDR_STEP_MOTOR2_STP_TAB_ELEM_HIGH          = 16'h035d;


//步进电机3配置寄存器
//配置寄存器
localparam REG_READ_ADDR_STEP_MOTOR3_CONFIG_1_LOW               = 16'h0400;
localparam REG_READ_ADDR_STEP_MOTOR3_CONFIG_1_HIGH              = 16'h0401;
//参数寄存器
localparam REG_READ_ADDR_STEP_MOTOR3_PARAM1_LOW                 = 16'h040a;
localparam REG_READ_ADDR_STEP_MOTOR3_PARAM1_HIGH                = 16'h040b;
localparam REG_READ_ADDR_STEP_MOTOR3_PARAM2_LOW                 = 16'h040c;
localparam REG_READ_ADDR_STEP_MOTOR3_PARAM2_HIGH                = 16'h040d;
localparam REG_READ_ADDR_STEP_MOTOR3_PARAM3_LOW                 = 16'h040e;
localparam REG_READ_ADDR_STEP_MOTOR3_PARAM3_HIGH                = 16'h040f;
localparam REG_READ_ADDR_STEP_MOTOR3_PARAM4_LOW                 = 16'h0410;
localparam REG_READ_ADDR_STEP_MOTOR3_PARAM4_HIGH                = 16'h0411;
localparam REG_READ_ADDR_STEP_MOTOR3_PARAM5_LOW                 = 16'h0412;
localparam REG_READ_ADDR_STEP_MOTOR3_PARAM5_HIGH                = 16'h0413;
//控制寄存器
localparam REG_READ_ADDR_STEP_MOTOR3_CONTROL1_LOW               = 16'h0430;
localparam REG_READ_ADDR_STEP_MOTOR3_CONTROL1_HIGH              = 16'h0431;
//状态寄存器
localparam REG_READ_ADDR_STEP_MOTOR3_STATE1_LOW                 = 16'h0450;
localparam REG_READ_ADDR_STEP_MOTOR3_STATE1_HIGH                = 16'h0451;
localparam REG_READ_ADDR_STEP_MOTOR3_STATE2_LOW                 = 16'h0452;
localparam REG_READ_ADDR_STEP_MOTOR3_STATE2_HIGH                = 16'h0453;
localparam REG_READ_ADDR_STEP_MOTOR3_STATE3_LOW                 = 16'h0454;
localparam REG_READ_ADDR_STEP_MOTOR3_STATE3_HIGH                = 16'h0455;
localparam REG_READ_ADDR_STEP_MOTOR3_STATE4_LOW                 = 16'h0456;
localparam REG_READ_ADDR_STEP_MOTOR3_STATE4_HIGH                = 16'h0457;
localparam REG_READ_ADDR_STEP_MOTOR3_STATE5_LOW                 = 16'h0458;
localparam REG_READ_ADDR_STEP_MOTOR3_STATE5_HIGH                = 16'h0459;
localparam REG_READ_ADDR_STEP_MOTOR3_SPD_TAB_ELEM_LOW           = 16'h045a;
localparam REG_READ_ADDR_STEP_MOTOR3_SPD_TAB_ELEM_HIGH          = 16'h045b;
localparam REG_READ_ADDR_STEP_MOTOR3_STP_TAB_ELEM_LOW           = 16'h045c;
localparam REG_READ_ADDR_STEP_MOTOR3_STP_TAB_ELEM_HIGH          = 16'h045d;


//步进电机4配置寄存器
//配置寄存器
localparam REG_READ_ADDR_STEP_MOTOR4_CONFIG_1_LOW               = 16'h0500;
localparam REG_READ_ADDR_STEP_MOTOR4_CONFIG_1_HIGH              = 16'h0501;
//参数寄存器
localparam REG_READ_ADDR_STEP_MOTOR4_PARAM1_LOW                 = 16'h050a;
localparam REG_READ_ADDR_STEP_MOTOR4_PARAM1_HIGH                = 16'h050b;
localparam REG_READ_ADDR_STEP_MOTOR4_PARAM2_LOW                 = 16'h050c;
localparam REG_READ_ADDR_STEP_MOTOR4_PARAM2_HIGH                = 16'h050d;
localparam REG_READ_ADDR_STEP_MOTOR4_PARAM3_LOW                 = 16'h050e;
localparam REG_READ_ADDR_STEP_MOTOR4_PARAM3_HIGH                = 16'h050f;
localparam REG_READ_ADDR_STEP_MOTOR4_PARAM4_LOW                 = 16'h0510;
localparam REG_READ_ADDR_STEP_MOTOR4_PARAM4_HIGH                = 16'h0511;
localparam REG_READ_ADDR_STEP_MOTOR4_PARAM5_LOW                 = 16'h0512;
localparam REG_READ_ADDR_STEP_MOTOR4_PARAM5_HIGH                = 16'h0513;
//控制寄存器
localparam REG_READ_ADDR_STEP_MOTOR4_CONTROL1_LOW               = 16'h0530;
localparam REG_READ_ADDR_STEP_MOTOR4_CONTROL1_HIGH              = 16'h0531;
//状态寄存器
localparam REG_READ_ADDR_STEP_MOTOR4_STATE1_LOW                 = 16'h0550;
localparam REG_READ_ADDR_STEP_MOTOR4_STATE1_HIGH                = 16'h0551;
localparam REG_READ_ADDR_STEP_MOTOR4_STATE2_LOW                 = 16'h0552;
localparam REG_READ_ADDR_STEP_MOTOR4_STATE2_HIGH                = 16'h0553;
localparam REG_READ_ADDR_STEP_MOTOR4_STATE3_LOW                 = 16'h0554;
localparam REG_READ_ADDR_STEP_MOTOR4_STATE3_HIGH                = 16'h0555;
localparam REG_READ_ADDR_STEP_MOTOR4_STATE4_LOW                 = 16'h0556;
localparam REG_READ_ADDR_STEP_MOTOR4_STATE4_HIGH                = 16'h0557;
localparam REG_READ_ADDR_STEP_MOTOR4_STATE5_LOW                 = 16'h0558;
localparam REG_READ_ADDR_STEP_MOTOR4_STATE5_HIGH                = 16'h0559;
localparam REG_READ_ADDR_STEP_MOTOR4_SPD_TAB_ELEM_LOW           = 16'h055a;
localparam REG_READ_ADDR_STEP_MOTOR4_SPD_TAB_ELEM_HIGH          = 16'h055b;
localparam REG_READ_ADDR_STEP_MOTOR4_STP_TAB_ELEM_LOW           = 16'h055c;
localparam REG_READ_ADDR_STEP_MOTOR4_STP_TAB_ELEM_HIGH          = 16'h055d;


//步进电机5配置寄存器
//配置寄存器
localparam REG_READ_ADDR_STEP_MOTOR5_CONFIG_1_LOW               = 16'h0600;
localparam REG_READ_ADDR_STEP_MOTOR5_CONFIG_1_HIGH              = 16'h0601;
//参数寄存器
localparam REG_READ_ADDR_STEP_MOTOR5_PARAM1_LOW                 = 16'h060a;
localparam REG_READ_ADDR_STEP_MOTOR5_PARAM1_HIGH                = 16'h060b;
localparam REG_READ_ADDR_STEP_MOTOR5_PARAM2_LOW                 = 16'h060c;
localparam REG_READ_ADDR_STEP_MOTOR5_PARAM2_HIGH                = 16'h060d;
localparam REG_READ_ADDR_STEP_MOTOR5_PARAM3_LOW                 = 16'h060e;
localparam REG_READ_ADDR_STEP_MOTOR5_PARAM3_HIGH                = 16'h060f;
localparam REG_READ_ADDR_STEP_MOTOR5_PARAM4_LOW                 = 16'h0610;
localparam REG_READ_ADDR_STEP_MOTOR5_PARAM4_HIGH                = 16'h0611;
localparam REG_READ_ADDR_STEP_MOTOR5_PARAM5_LOW                 = 16'h0612;
localparam REG_READ_ADDR_STEP_MOTOR5_PARAM5_HIGH                = 16'h0613;
//控制寄存器
localparam REG_READ_ADDR_STEP_MOTOR5_CONTROL1_LOW               = 16'h0630;
localparam REG_READ_ADDR_STEP_MOTOR5_CONTROL1_HIGH              = 16'h0631;
//状态寄存器
localparam REG_READ_ADDR_STEP_MOTOR5_STATE1_LOW                 = 16'h0650;
localparam REG_READ_ADDR_STEP_MOTOR5_STATE1_HIGH                = 16'h0651;
localparam REG_READ_ADDR_STEP_MOTOR5_STATE2_LOW                 = 16'h0652;
localparam REG_READ_ADDR_STEP_MOTOR5_STATE2_HIGH                = 16'h0653;
localparam REG_READ_ADDR_STEP_MOTOR5_STATE3_LOW                 = 16'h0654;
localparam REG_READ_ADDR_STEP_MOTOR5_STATE3_HIGH                = 16'h0655;
localparam REG_READ_ADDR_STEP_MOTOR5_STATE4_LOW                 = 16'h0656;
localparam REG_READ_ADDR_STEP_MOTOR5_STATE4_HIGH                = 16'h0657;
localparam REG_READ_ADDR_STEP_MOTOR5_STATE5_LOW                 = 16'h0658;
localparam REG_READ_ADDR_STEP_MOTOR5_STATE5_HIGH                = 16'h0659;
localparam REG_READ_ADDR_STEP_MOTOR5_SPD_TAB_ELEM_LOW           = 16'h065a;
localparam REG_READ_ADDR_STEP_MOTOR5_SPD_TAB_ELEM_HIGH          = 16'h065b;
localparam REG_READ_ADDR_STEP_MOTOR5_STP_TAB_ELEM_LOW           = 16'h065c;
localparam REG_READ_ADDR_STEP_MOTOR5_STP_TAB_ELEM_HIGH          = 16'h065d;


//步进电机6配置寄存器
//配置寄存器
localparam REG_READ_ADDR_STEP_MOTOR6_CONFIG_1_LOW               = 16'h0700;
localparam REG_READ_ADDR_STEP_MOTOR6_CONFIG_1_HIGH              = 16'h0701;
//参数寄存器
localparam REG_READ_ADDR_STEP_MOTOR6_PARAM1_LOW                 = 16'h070a;
localparam REG_READ_ADDR_STEP_MOTOR6_PARAM1_HIGH                = 16'h070b;
localparam REG_READ_ADDR_STEP_MOTOR6_PARAM2_LOW                 = 16'h070c;
localparam REG_READ_ADDR_STEP_MOTOR6_PARAM2_HIGH                = 16'h070d;
localparam REG_READ_ADDR_STEP_MOTOR6_PARAM3_LOW                 = 16'h070e;
localparam REG_READ_ADDR_STEP_MOTOR6_PARAM3_HIGH                = 16'h070f;
localparam REG_READ_ADDR_STEP_MOTOR6_PARAM4_LOW                 = 16'h0710;
localparam REG_READ_ADDR_STEP_MOTOR6_PARAM4_HIGH                = 16'h0711;
localparam REG_READ_ADDR_STEP_MOTOR6_PARAM5_LOW                 = 16'h0712;
localparam REG_READ_ADDR_STEP_MOTOR6_PARAM5_HIGH                = 16'h0713;
//控制寄存器
localparam REG_READ_ADDR_STEP_MOTOR6_CONTROL1_LOW               = 16'h0730;
localparam REG_READ_ADDR_STEP_MOTOR6_CONTROL1_HIGH              = 16'h0731;
//状态寄存器
localparam REG_READ_ADDR_STEP_MOTOR6_STATE1_LOW                 = 16'h0750;
localparam REG_READ_ADDR_STEP_MOTOR6_STATE1_HIGH                = 16'h0751;
localparam REG_READ_ADDR_STEP_MOTOR6_STATE2_LOW                 = 16'h0752;
localparam REG_READ_ADDR_STEP_MOTOR6_STATE2_HIGH                = 16'h0753;
localparam REG_READ_ADDR_STEP_MOTOR6_STATE3_LOW                 = 16'h0754;
localparam REG_READ_ADDR_STEP_MOTOR6_STATE3_HIGH                = 16'h0755;
localparam REG_READ_ADDR_STEP_MOTOR6_STATE4_LOW                 = 16'h0756;
localparam REG_READ_ADDR_STEP_MOTOR6_STATE4_HIGH                = 16'h0757;
localparam REG_READ_ADDR_STEP_MOTOR6_STATE5_LOW                 = 16'h0758;
localparam REG_READ_ADDR_STEP_MOTOR6_STATE5_HIGH                = 16'h0759;
localparam REG_READ_ADDR_STEP_MOTOR6_SPD_TAB_ELEM_LOW           = 16'h075a;
localparam REG_READ_ADDR_STEP_MOTOR6_SPD_TAB_ELEM_HIGH          = 16'h075b;
localparam REG_READ_ADDR_STEP_MOTOR6_STP_TAB_ELEM_LOW           = 16'h075c;
localparam REG_READ_ADDR_STEP_MOTOR6_STP_TAB_ELEM_HIGH          = 16'h075d;


//步进电机7配置寄存器
//配置寄存器
localparam REG_READ_ADDR_STEP_MOTOR7_CONFIG_1_LOW               = 16'h0800;
localparam REG_READ_ADDR_STEP_MOTOR7_CONFIG_1_HIGH              = 16'h0801;
//参数寄存器
localparam REG_READ_ADDR_STEP_MOTOR7_PARAM1_LOW                 = 16'h080a;
localparam REG_READ_ADDR_STEP_MOTOR7_PARAM1_HIGH                = 16'h080b;
localparam REG_READ_ADDR_STEP_MOTOR7_PARAM2_LOW                 = 16'h080c;
localparam REG_READ_ADDR_STEP_MOTOR7_PARAM2_HIGH                = 16'h080d;
localparam REG_READ_ADDR_STEP_MOTOR7_PARAM3_LOW                 = 16'h080e;
localparam REG_READ_ADDR_STEP_MOTOR7_PARAM3_HIGH                = 16'h080f;
localparam REG_READ_ADDR_STEP_MOTOR7_PARAM4_LOW                 = 16'h0810;
localparam REG_READ_ADDR_STEP_MOTOR7_PARAM4_HIGH                = 16'h0811;
localparam REG_READ_ADDR_STEP_MOTOR7_PARAM5_LOW                 = 16'h0812;
localparam REG_READ_ADDR_STEP_MOTOR7_PARAM5_HIGH                = 16'h0813;
//控制寄存器
localparam REG_READ_ADDR_STEP_MOTOR7_CONTROL1_LOW               = 16'h0830;
localparam REG_READ_ADDR_STEP_MOTOR7_CONTROL1_HIGH              = 16'h0831;
//状态寄存器
localparam REG_READ_ADDR_STEP_MOTOR7_STATE1_LOW                 = 16'h0850;
localparam REG_READ_ADDR_STEP_MOTOR7_STATE1_HIGH                = 16'h0851;
localparam REG_READ_ADDR_STEP_MOTOR7_STATE2_LOW                 = 16'h0852;
localparam REG_READ_ADDR_STEP_MOTOR7_STATE2_HIGH                = 16'h0853;
localparam REG_READ_ADDR_STEP_MOTOR7_STATE3_LOW                 = 16'h0854;
localparam REG_READ_ADDR_STEP_MOTOR7_STATE3_HIGH                = 16'h0855;
localparam REG_READ_ADDR_STEP_MOTOR7_STATE4_LOW                 = 16'h0856;
localparam REG_READ_ADDR_STEP_MOTOR7_STATE4_HIGH                = 16'h0857;
localparam REG_READ_ADDR_STEP_MOTOR7_STATE5_LOW                 = 16'h0858;
localparam REG_READ_ADDR_STEP_MOTOR7_STATE5_HIGH                = 16'h0859;
localparam REG_READ_ADDR_STEP_MOTOR7_SPD_TAB_ELEM_LOW           = 16'h085a;
localparam REG_READ_ADDR_STEP_MOTOR7_SPD_TAB_ELEM_HIGH          = 16'h085b;
localparam REG_READ_ADDR_STEP_MOTOR7_STP_TAB_ELEM_LOW           = 16'h085c;
localparam REG_READ_ADDR_STEP_MOTOR7_STP_TAB_ELEM_HIGH          = 16'h085d;


//步进电机8配置寄存器
//配置寄存器
localparam REG_READ_ADDR_STEP_MOTOR8_CONFIG_1_LOW               = 16'h0900;
localparam REG_READ_ADDR_STEP_MOTOR8_CONFIG_1_HIGH              = 16'h0901;
//参数寄存器
localparam REG_READ_ADDR_STEP_MOTOR8_PARAM1_LOW                 = 16'h090a;
localparam REG_READ_ADDR_STEP_MOTOR8_PARAM1_HIGH                = 16'h090b;
localparam REG_READ_ADDR_STEP_MOTOR8_PARAM2_LOW                 = 16'h090c;
localparam REG_READ_ADDR_STEP_MOTOR8_PARAM2_HIGH                = 16'h090d;
localparam REG_READ_ADDR_STEP_MOTOR8_PARAM3_LOW                 = 16'h090e;
localparam REG_READ_ADDR_STEP_MOTOR8_PARAM3_HIGH                = 16'h090f;
localparam REG_READ_ADDR_STEP_MOTOR8_PARAM4_LOW                 = 16'h0910;
localparam REG_READ_ADDR_STEP_MOTOR8_PARAM4_HIGH                = 16'h0911;
localparam REG_READ_ADDR_STEP_MOTOR8_PARAM5_LOW                 = 16'h0912;
localparam REG_READ_ADDR_STEP_MOTOR8_PARAM5_HIGH                = 16'h0913;
//控制寄存器
localparam REG_READ_ADDR_STEP_MOTOR8_CONTROL1_LOW               = 16'h0930;
localparam REG_READ_ADDR_STEP_MOTOR8_CONTROL1_HIGH              = 16'h0931;
//状态寄存器
localparam REG_READ_ADDR_STEP_MOTOR8_STATE1_LOW                 = 16'h0950;
localparam REG_READ_ADDR_STEP_MOTOR8_STATE1_HIGH                = 16'h0951;
localparam REG_READ_ADDR_STEP_MOTOR8_STATE2_LOW                 = 16'h0952;
localparam REG_READ_ADDR_STEP_MOTOR8_STATE2_HIGH                = 16'h0953;
localparam REG_READ_ADDR_STEP_MOTOR8_STATE3_LOW                 = 16'h0954;
localparam REG_READ_ADDR_STEP_MOTOR8_STATE3_HIGH                = 16'h0955;
localparam REG_READ_ADDR_STEP_MOTOR8_STATE4_LOW                 = 16'h0956;
localparam REG_READ_ADDR_STEP_MOTOR8_STATE4_HIGH                = 16'h0957;
localparam REG_READ_ADDR_STEP_MOTOR8_STATE5_LOW                 = 16'h0958;
localparam REG_READ_ADDR_STEP_MOTOR8_STATE5_HIGH                = 16'h0959;
localparam REG_READ_ADDR_STEP_MOTOR8_SPD_TAB_ELEM_LOW           = 16'h095a;
localparam REG_READ_ADDR_STEP_MOTOR8_SPD_TAB_ELEM_HIGH          = 16'h095b;
localparam REG_READ_ADDR_STEP_MOTOR8_STP_TAB_ELEM_LOW           = 16'h095c;
localparam REG_READ_ADDR_STEP_MOTOR8_STP_TAB_ELEM_HIGH          = 16'h095d;


//步进电机9配置寄存器
//配置寄存器
localparam REG_READ_ADDR_STEP_MOTOR9_CONFIG_1_LOW               = 16'h0a00;
localparam REG_READ_ADDR_STEP_MOTOR9_CONFIG_1_HIGH              = 16'h0a01;
//参数寄存器
localparam REG_READ_ADDR_STEP_MOTOR9_PARAM1_LOW                 = 16'h0a0a;
localparam REG_READ_ADDR_STEP_MOTOR9_PARAM1_HIGH                = 16'h0a0b;
localparam REG_READ_ADDR_STEP_MOTOR9_PARAM2_LOW                 = 16'h0a0c;
localparam REG_READ_ADDR_STEP_MOTOR9_PARAM2_HIGH                = 16'h0a0d;
localparam REG_READ_ADDR_STEP_MOTOR9_PARAM3_LOW                 = 16'h0a0e;
localparam REG_READ_ADDR_STEP_MOTOR9_PARAM3_HIGH                = 16'h0a0f;
localparam REG_READ_ADDR_STEP_MOTOR9_PARAM4_LOW                 = 16'h0a10;
localparam REG_READ_ADDR_STEP_MOTOR9_PARAM4_HIGH                = 16'h0a11;
localparam REG_READ_ADDR_STEP_MOTOR9_PARAM5_LOW                 = 16'h0a12;
localparam REG_READ_ADDR_STEP_MOTOR9_PARAM5_HIGH                = 16'h0a13;
//控制寄存器
localparam REG_READ_ADDR_STEP_MOTOR9_CONTROL1_LOW               = 16'h0a30;
localparam REG_READ_ADDR_STEP_MOTOR9_CONTROL1_HIGH              = 16'h0a31;
//状态寄存器
localparam REG_READ_ADDR_STEP_MOTOR9_STATE1_LOW                 = 16'h0a50;
localparam REG_READ_ADDR_STEP_MOTOR9_STATE1_HIGH                = 16'h0a51;
localparam REG_READ_ADDR_STEP_MOTOR9_STATE2_LOW                 = 16'h0a52;
localparam REG_READ_ADDR_STEP_MOTOR9_STATE2_HIGH                = 16'h0a53;
localparam REG_READ_ADDR_STEP_MOTOR9_STATE3_LOW                 = 16'h0a54;
localparam REG_READ_ADDR_STEP_MOTOR9_STATE3_HIGH                = 16'h0a55;
localparam REG_READ_ADDR_STEP_MOTOR9_STATE4_LOW                 = 16'h0a56;
localparam REG_READ_ADDR_STEP_MOTOR9_STATE4_HIGH                = 16'h0a57;
localparam REG_READ_ADDR_STEP_MOTOR9_STATE5_LOW                 = 16'h0a58;
localparam REG_READ_ADDR_STEP_MOTOR9_STATE5_HIGH                = 16'h0a59;
localparam REG_READ_ADDR_STEP_MOTOR9_SPD_TAB_ELEM_LOW           = 16'h0a5a;
localparam REG_READ_ADDR_STEP_MOTOR9_SPD_TAB_ELEM_HIGH          = 16'h0a5b;
localparam REG_READ_ADDR_STEP_MOTOR9_STP_TAB_ELEM_LOW           = 16'h0a5c;
localparam REG_READ_ADDR_STEP_MOTOR9_STP_TAB_ELEM_HIGH          = 16'h0a5d;


//步进电机10配置寄存器
//配置寄存器
localparam REG_READ_ADDR_STEP_MOTOR10_CONFIG_1_LOW               = 16'h0b00;
localparam REG_READ_ADDR_STEP_MOTOR10_CONFIG_1_HIGH              = 16'h0b01;
//参数寄存器
localparam REG_READ_ADDR_STEP_MOTOR10_PARAM1_LOW                 = 16'h0b0a;
localparam REG_READ_ADDR_STEP_MOTOR10_PARAM1_HIGH                = 16'h0b0b;
localparam REG_READ_ADDR_STEP_MOTOR10_PARAM2_LOW                 = 16'h0b0c;
localparam REG_READ_ADDR_STEP_MOTOR10_PARAM2_HIGH                = 16'h0b0d;
localparam REG_READ_ADDR_STEP_MOTOR10_PARAM3_LOW                 = 16'h0b0e;
localparam REG_READ_ADDR_STEP_MOTOR10_PARAM3_HIGH                = 16'h0b0f;
localparam REG_READ_ADDR_STEP_MOTOR10_PARAM4_LOW                 = 16'h0b10;
localparam REG_READ_ADDR_STEP_MOTOR10_PARAM4_HIGH                = 16'h0b11;
localparam REG_READ_ADDR_STEP_MOTOR10_PARAM5_LOW                 = 16'h0b12;
localparam REG_READ_ADDR_STEP_MOTOR10_PARAM5_HIGH                = 16'h0b13;
//控制寄存器
localparam REG_READ_ADDR_STEP_MOTOR10_CONTROL1_LOW               = 16'h0b30;
localparam REG_READ_ADDR_STEP_MOTOR10_CONTROL1_HIGH              = 16'h0b31;
//状态寄存器
localparam REG_READ_ADDR_STEP_MOTOR10_STATE1_LOW                 = 16'h0b50;
localparam REG_READ_ADDR_STEP_MOTOR10_STATE1_HIGH                = 16'h0b51;
localparam REG_READ_ADDR_STEP_MOTOR10_STATE2_LOW                 = 16'h0b52;
localparam REG_READ_ADDR_STEP_MOTOR10_STATE2_HIGH                = 16'h0b53;
localparam REG_READ_ADDR_STEP_MOTOR10_STATE3_LOW                 = 16'h0b54;
localparam REG_READ_ADDR_STEP_MOTOR10_STATE3_HIGH                = 16'h0b55;
localparam REG_READ_ADDR_STEP_MOTOR10_STATE4_LOW                 = 16'h0b56;
localparam REG_READ_ADDR_STEP_MOTOR10_STATE4_HIGH                = 16'h0b57;
localparam REG_READ_ADDR_STEP_MOTOR10_STATE5_LOW                 = 16'h0b58;
localparam REG_READ_ADDR_STEP_MOTOR10_STATE5_HIGH                = 16'h0b59;
localparam REG_READ_ADDR_STEP_MOTOR10_SPD_TAB_ELEM_LOW           = 16'h0b5a;
localparam REG_READ_ADDR_STEP_MOTOR10_SPD_TAB_ELEM_HIGH          = 16'h0b5b;
localparam REG_READ_ADDR_STEP_MOTOR10_STP_TAB_ELEM_LOW           = 16'h0b5c;
localparam REG_READ_ADDR_STEP_MOTOR10_STP_TAB_ELEM_HIGH          = 16'h0b5d;


//步进电机11配置寄存器
//配置寄存器
localparam REG_READ_ADDR_STEP_MOTOR11_CONFIG_1_LOW               = 16'h0c00;
localparam REG_READ_ADDR_STEP_MOTOR11_CONFIG_1_HIGH              = 16'h0c01;
//参数寄存器
localparam REG_READ_ADDR_STEP_MOTOR11_PARAM1_LOW                 = 16'h0c0a;
localparam REG_READ_ADDR_STEP_MOTOR11_PARAM1_HIGH                = 16'h0c0b;
localparam REG_READ_ADDR_STEP_MOTOR11_PARAM2_LOW                 = 16'h0c0c;
localparam REG_READ_ADDR_STEP_MOTOR11_PARAM2_HIGH                = 16'h0c0d;
localparam REG_READ_ADDR_STEP_MOTOR11_PARAM3_LOW                 = 16'h0c0e;
localparam REG_READ_ADDR_STEP_MOTOR11_PARAM3_HIGH                = 16'h0c0f;
localparam REG_READ_ADDR_STEP_MOTOR11_PARAM4_LOW                 = 16'h0c10;
localparam REG_READ_ADDR_STEP_MOTOR11_PARAM4_HIGH                = 16'h0c11;
localparam REG_READ_ADDR_STEP_MOTOR11_PARAM5_LOW                 = 16'h0c12;
localparam REG_READ_ADDR_STEP_MOTOR11_PARAM5_HIGH                = 16'h0c13;
//控制寄存器
localparam REG_READ_ADDR_STEP_MOTOR11_CONTROL1_LOW               = 16'h0c30;
localparam REG_READ_ADDR_STEP_MOTOR11_CONTROL1_HIGH              = 16'h0c31;
//状态寄存器
localparam REG_READ_ADDR_STEP_MOTOR11_STATE1_LOW                 = 16'h0c50;
localparam REG_READ_ADDR_STEP_MOTOR11_STATE1_HIGH                = 16'h0c51;
localparam REG_READ_ADDR_STEP_MOTOR11_STATE2_LOW                 = 16'h0c52;
localparam REG_READ_ADDR_STEP_MOTOR11_STATE2_HIGH                = 16'h0c53;
localparam REG_READ_ADDR_STEP_MOTOR11_STATE3_LOW                 = 16'h0c54;
localparam REG_READ_ADDR_STEP_MOTOR11_STATE3_HIGH                = 16'h0c55;
localparam REG_READ_ADDR_STEP_MOTOR11_STATE4_LOW                 = 16'h0c56;
localparam REG_READ_ADDR_STEP_MOTOR11_STATE4_HIGH                = 16'h0c57;
localparam REG_READ_ADDR_STEP_MOTOR11_STATE5_LOW                 = 16'h0c58;
localparam REG_READ_ADDR_STEP_MOTOR11_STATE5_HIGH                = 16'h0c59;
localparam REG_READ_ADDR_STEP_MOTOR11_SPD_TAB_ELEM_LOW           = 16'h0c5a;
localparam REG_READ_ADDR_STEP_MOTOR11_SPD_TAB_ELEM_HIGH          = 16'h0c5b;
localparam REG_READ_ADDR_STEP_MOTOR11_STP_TAB_ELEM_LOW           = 16'h0c5c;
localparam REG_READ_ADDR_STEP_MOTOR11_STP_TAB_ELEM_HIGH          = 16'h0c5d;


//步进电机12配置寄存器
//配置寄存器
localparam REG_READ_ADDR_STEP_MOTOR12_CONFIG_1_LOW               = 16'h0d00;
localparam REG_READ_ADDR_STEP_MOTOR12_CONFIG_1_HIGH              = 16'h0d01;
//参数寄存器
localparam REG_READ_ADDR_STEP_MOTOR12_PARAM1_LOW                 = 16'h0d0a;
localparam REG_READ_ADDR_STEP_MOTOR12_PARAM1_HIGH                = 16'h0d0b;
localparam REG_READ_ADDR_STEP_MOTOR12_PARAM2_LOW                 = 16'h0d0c;
localparam REG_READ_ADDR_STEP_MOTOR12_PARAM2_HIGH                = 16'h0d0d;
localparam REG_READ_ADDR_STEP_MOTOR12_PARAM3_LOW                 = 16'h0d0e;
localparam REG_READ_ADDR_STEP_MOTOR12_PARAM3_HIGH                = 16'h0d0f;
localparam REG_READ_ADDR_STEP_MOTOR12_PARAM4_LOW                 = 16'h0d10;
localparam REG_READ_ADDR_STEP_MOTOR12_PARAM4_HIGH                = 16'h0d11;
localparam REG_READ_ADDR_STEP_MOTOR12_PARAM5_LOW                 = 16'h0d12;
localparam REG_READ_ADDR_STEP_MOTOR12_PARAM5_HIGH                = 16'h0d13;
//控制寄存器
localparam REG_READ_ADDR_STEP_MOTOR12_CONTROL1_LOW               = 16'h0d30;
localparam REG_READ_ADDR_STEP_MOTOR12_CONTROL1_HIGH              = 16'h0d31;
//状态寄存器
localparam REG_READ_ADDR_STEP_MOTOR12_STATE1_LOW                 = 16'h0d50;
localparam REG_READ_ADDR_STEP_MOTOR12_STATE1_HIGH                = 16'h0d51;
localparam REG_READ_ADDR_STEP_MOTOR12_STATE2_LOW                 = 16'h0d52;
localparam REG_READ_ADDR_STEP_MOTOR12_STATE2_HIGH                = 16'h0d53;
localparam REG_READ_ADDR_STEP_MOTOR12_STATE3_LOW                 = 16'h0d54;
localparam REG_READ_ADDR_STEP_MOTOR12_STATE3_HIGH                = 16'h0d55;
localparam REG_READ_ADDR_STEP_MOTOR12_STATE4_LOW                 = 16'h0d56;
localparam REG_READ_ADDR_STEP_MOTOR12_STATE4_HIGH                = 16'h0d57;
localparam REG_READ_ADDR_STEP_MOTOR12_STATE5_LOW                 = 16'h0d58;
localparam REG_READ_ADDR_STEP_MOTOR12_STATE5_HIGH                = 16'h0d59;
localparam REG_READ_ADDR_STEP_MOTOR12_SPD_TAB_ELEM_LOW           = 16'h0d5a;
localparam REG_READ_ADDR_STEP_MOTOR12_SPD_TAB_ELEM_HIGH          = 16'h0d5b;
localparam REG_READ_ADDR_STEP_MOTOR12_STP_TAB_ELEM_LOW           = 16'h0d5c;
localparam REG_READ_ADDR_STEP_MOTOR12_STP_TAB_ELEM_HIGH          = 16'h0d5d;


//步进电机13配置寄存器
//配置寄存器
localparam REG_READ_ADDR_STEP_MOTOR13_CONFIG_1_LOW               = 16'h0e00;
localparam REG_READ_ADDR_STEP_MOTOR13_CONFIG_1_HIGH              = 16'h0e01;
//参数寄存器
localparam REG_READ_ADDR_STEP_MOTOR13_PARAM1_LOW                 = 16'h0e0a;
localparam REG_READ_ADDR_STEP_MOTOR13_PARAM1_HIGH                = 16'h0e0b;
localparam REG_READ_ADDR_STEP_MOTOR13_PARAM2_LOW                 = 16'h0e0c;
localparam REG_READ_ADDR_STEP_MOTOR13_PARAM2_HIGH                = 16'h0e0d;
localparam REG_READ_ADDR_STEP_MOTOR13_PARAM3_LOW                 = 16'h0e0e;
localparam REG_READ_ADDR_STEP_MOTOR13_PARAM3_HIGH                = 16'h0e0f;
localparam REG_READ_ADDR_STEP_MOTOR13_PARAM4_LOW                 = 16'h0e10;
localparam REG_READ_ADDR_STEP_MOTOR13_PARAM4_HIGH                = 16'h0e11;
localparam REG_READ_ADDR_STEP_MOTOR13_PARAM5_LOW                 = 16'h0e12;
localparam REG_READ_ADDR_STEP_MOTOR13_PARAM5_HIGH                = 16'h0e13;
//控制寄存器
localparam REG_READ_ADDR_STEP_MOTOR13_CONTROL1_LOW               = 16'h0e30;
localparam REG_READ_ADDR_STEP_MOTOR13_CONTROL1_HIGH              = 16'h0e31;
//状态寄存器
localparam REG_READ_ADDR_STEP_MOTOR13_STATE1_LOW                 = 16'h0e50;
localparam REG_READ_ADDR_STEP_MOTOR13_STATE1_HIGH                = 16'h0e51;
localparam REG_READ_ADDR_STEP_MOTOR13_STATE2_LOW                 = 16'h0e52;
localparam REG_READ_ADDR_STEP_MOTOR13_STATE2_HIGH                = 16'h0e53;
localparam REG_READ_ADDR_STEP_MOTOR13_STATE3_LOW                 = 16'h0e54;
localparam REG_READ_ADDR_STEP_MOTOR13_STATE3_HIGH                = 16'h0e55;
localparam REG_READ_ADDR_STEP_MOTOR13_STATE4_LOW                 = 16'h0e56;
localparam REG_READ_ADDR_STEP_MOTOR13_STATE4_HIGH                = 16'h0e57;
localparam REG_READ_ADDR_STEP_MOTOR13_STATE5_LOW                 = 16'h0e58;
localparam REG_READ_ADDR_STEP_MOTOR13_STATE5_HIGH                = 16'h0e59;
localparam REG_READ_ADDR_STEP_MOTOR13_SPD_TAB_ELEM_LOW           = 16'h0e5a;
localparam REG_READ_ADDR_STEP_MOTOR13_SPD_TAB_ELEM_HIGH          = 16'h0e5b;
localparam REG_READ_ADDR_STEP_MOTOR13_STP_TAB_ELEM_LOW           = 16'h0e5c;
localparam REG_READ_ADDR_STEP_MOTOR13_STP_TAB_ELEM_HIGH          = 16'h0e5d;


//步进电机14配置寄存器
//配置寄存器
localparam REG_READ_ADDR_STEP_MOTOR14_CONFIG_1_LOW               = 16'h0f00;
localparam REG_READ_ADDR_STEP_MOTOR14_CONFIG_1_HIGH              = 16'h0f01;
//参数寄存器
localparam REG_READ_ADDR_STEP_MOTOR14_PARAM1_LOW                 = 16'h0f0a;
localparam REG_READ_ADDR_STEP_MOTOR14_PARAM1_HIGH                = 16'h0f0b;
localparam REG_READ_ADDR_STEP_MOTOR14_PARAM2_LOW                 = 16'h0f0c;
localparam REG_READ_ADDR_STEP_MOTOR14_PARAM2_HIGH                = 16'h0f0d;
localparam REG_READ_ADDR_STEP_MOTOR14_PARAM3_LOW                 = 16'h0f0e;
localparam REG_READ_ADDR_STEP_MOTOR14_PARAM3_HIGH                = 16'h0f0f;
localparam REG_READ_ADDR_STEP_MOTOR14_PARAM4_LOW                 = 16'h0f10;
localparam REG_READ_ADDR_STEP_MOTOR14_PARAM4_HIGH                = 16'h0f11;
localparam REG_READ_ADDR_STEP_MOTOR14_PARAM5_LOW                 = 16'h0f12;
localparam REG_READ_ADDR_STEP_MOTOR14_PARAM5_HIGH                = 16'h0f13;
//控制寄存器
localparam REG_READ_ADDR_STEP_MOTOR14_CONTROL1_LOW               = 16'h0f30;
localparam REG_READ_ADDR_STEP_MOTOR14_CONTROL1_HIGH              = 16'h0f31;
//状态寄存器
localparam REG_READ_ADDR_STEP_MOTOR14_STATE1_LOW                 = 16'h0f50;
localparam REG_READ_ADDR_STEP_MOTOR14_STATE1_HIGH                = 16'h0f51;
localparam REG_READ_ADDR_STEP_MOTOR14_STATE2_LOW                 = 16'h0f52;
localparam REG_READ_ADDR_STEP_MOTOR14_STATE2_HIGH                = 16'h0f53;
localparam REG_READ_ADDR_STEP_MOTOR14_STATE3_LOW                 = 16'h0f54;
localparam REG_READ_ADDR_STEP_MOTOR14_STATE3_HIGH                = 16'h0f55;
localparam REG_READ_ADDR_STEP_MOTOR14_STATE4_LOW                 = 16'h0f56;
localparam REG_READ_ADDR_STEP_MOTOR14_STATE4_HIGH                = 16'h0f57;
localparam REG_READ_ADDR_STEP_MOTOR14_STATE5_LOW                 = 16'h0f58;
localparam REG_READ_ADDR_STEP_MOTOR14_STATE5_HIGH                = 16'h0f59;
localparam REG_READ_ADDR_STEP_MOTOR14_SPD_TAB_ELEM_LOW           = 16'h0f5a;
localparam REG_READ_ADDR_STEP_MOTOR14_SPD_TAB_ELEM_HIGH          = 16'h0f5b;
localparam REG_READ_ADDR_STEP_MOTOR14_STP_TAB_ELEM_LOW           = 16'h0f5c;
localparam REG_READ_ADDR_STEP_MOTOR14_STP_TAB_ELEM_HIGH          = 16'h0f5d;


//步进电机15配置寄存器
//配置寄存器
localparam REG_READ_ADDR_STEP_MOTOR15_CONFIG_1_LOW               = 16'h1000;
localparam REG_READ_ADDR_STEP_MOTOR15_CONFIG_1_HIGH              = 16'h1001;
//参数寄存器
localparam REG_READ_ADDR_STEP_MOTOR15_PARAM1_LOW                 = 16'h100a;
localparam REG_READ_ADDR_STEP_MOTOR15_PARAM1_HIGH                = 16'h100b;
localparam REG_READ_ADDR_STEP_MOTOR15_PARAM2_LOW                 = 16'h100c;
localparam REG_READ_ADDR_STEP_MOTOR15_PARAM2_HIGH                = 16'h100d;
localparam REG_READ_ADDR_STEP_MOTOR15_PARAM3_LOW                 = 16'h100e;
localparam REG_READ_ADDR_STEP_MOTOR15_PARAM3_HIGH                = 16'h100f;
localparam REG_READ_ADDR_STEP_MOTOR15_PARAM4_LOW                 = 16'h1010;
localparam REG_READ_ADDR_STEP_MOTOR15_PARAM4_HIGH                = 16'h1011;
localparam REG_READ_ADDR_STEP_MOTOR15_PARAM5_LOW                 = 16'h1012;
localparam REG_READ_ADDR_STEP_MOTOR15_PARAM5_HIGH                = 16'h1013;
//控制寄存器
localparam REG_READ_ADDR_STEP_MOTOR15_CONTROL1_LOW               = 16'h1030;
localparam REG_READ_ADDR_STEP_MOTOR15_CONTROL1_HIGH              = 16'h1031;
//状态寄存器
localparam REG_READ_ADDR_STEP_MOTOR15_STATE1_LOW                 = 16'h1050;
localparam REG_READ_ADDR_STEP_MOTOR15_STATE1_HIGH                = 16'h1051;
localparam REG_READ_ADDR_STEP_MOTOR15_STATE2_LOW                 = 16'h1052;
localparam REG_READ_ADDR_STEP_MOTOR15_STATE2_HIGH                = 16'h1053;
localparam REG_READ_ADDR_STEP_MOTOR15_STATE3_LOW                 = 16'h1054;
localparam REG_READ_ADDR_STEP_MOTOR15_STATE3_HIGH                = 16'h1055;
localparam REG_READ_ADDR_STEP_MOTOR15_STATE4_LOW                 = 16'h1056;
localparam REG_READ_ADDR_STEP_MOTOR15_STATE4_HIGH                = 16'h1057;
localparam REG_READ_ADDR_STEP_MOTOR15_STATE5_LOW                 = 16'h1058;
localparam REG_READ_ADDR_STEP_MOTOR15_STATE5_HIGH                = 16'h1059;
localparam REG_READ_ADDR_STEP_MOTOR15_SPD_TAB_ELEM_LOW           = 16'h105a;
localparam REG_READ_ADDR_STEP_MOTOR15_SPD_TAB_ELEM_HIGH          = 16'h105b;
localparam REG_READ_ADDR_STEP_MOTOR15_STP_TAB_ELEM_LOW           = 16'h105c;
localparam REG_READ_ADDR_STEP_MOTOR15_STP_TAB_ELEM_HIGH          = 16'h105d;


//步进电机16配置寄存器
//配置寄存器
localparam REG_READ_ADDR_STEP_MOTOR16_CONFIG_1_LOW               = 16'h1100;
localparam REG_READ_ADDR_STEP_MOTOR16_CONFIG_1_HIGH              = 16'h1101;
//参数寄存器
localparam REG_READ_ADDR_STEP_MOTOR16_PARAM1_LOW                 = 16'h110a;
localparam REG_READ_ADDR_STEP_MOTOR16_PARAM1_HIGH                = 16'h110b;
localparam REG_READ_ADDR_STEP_MOTOR16_PARAM2_LOW                 = 16'h110c;
localparam REG_READ_ADDR_STEP_MOTOR16_PARAM2_HIGH                = 16'h110d;
localparam REG_READ_ADDR_STEP_MOTOR16_PARAM3_LOW                 = 16'h110e;
localparam REG_READ_ADDR_STEP_MOTOR16_PARAM3_HIGH                = 16'h110f;
localparam REG_READ_ADDR_STEP_MOTOR16_PARAM4_LOW                 = 16'h1110;
localparam REG_READ_ADDR_STEP_MOTOR16_PARAM4_HIGH                = 16'h1111;
localparam REG_READ_ADDR_STEP_MOTOR16_PARAM5_LOW                 = 16'h1112;
localparam REG_READ_ADDR_STEP_MOTOR16_PARAM5_HIGH                = 16'h1113;
//控制寄存器
localparam REG_READ_ADDR_STEP_MOTOR16_CONTROL1_LOW               = 16'h1130;
localparam REG_READ_ADDR_STEP_MOTOR16_CONTROL1_HIGH              = 16'h1131;
//状态寄存器
localparam REG_READ_ADDR_STEP_MOTOR16_STATE1_LOW                 = 16'h1150;
localparam REG_READ_ADDR_STEP_MOTOR16_STATE1_HIGH                = 16'h1151;
localparam REG_READ_ADDR_STEP_MOTOR16_STATE2_LOW                 = 16'h1152;
localparam REG_READ_ADDR_STEP_MOTOR16_STATE2_HIGH                = 16'h1153;
localparam REG_READ_ADDR_STEP_MOTOR16_STATE3_LOW                 = 16'h1154;
localparam REG_READ_ADDR_STEP_MOTOR16_STATE3_HIGH                = 16'h1155;
localparam REG_READ_ADDR_STEP_MOTOR16_STATE4_LOW                 = 16'h1156;
localparam REG_READ_ADDR_STEP_MOTOR16_STATE4_HIGH                = 16'h1157;
localparam REG_READ_ADDR_STEP_MOTOR16_STATE5_LOW                 = 16'h1158;
localparam REG_READ_ADDR_STEP_MOTOR16_STATE5_HIGH                = 16'h1159;
localparam REG_READ_ADDR_STEP_MOTOR16_SPD_TAB_ELEM_LOW           = 16'h115a;
localparam REG_READ_ADDR_STEP_MOTOR16_SPD_TAB_ELEM_HIGH          = 16'h115b;
localparam REG_READ_ADDR_STEP_MOTOR16_STP_TAB_ELEM_LOW           = 16'h115c;
localparam REG_READ_ADDR_STEP_MOTOR16_STP_TAB_ELEM_HIGH          = 16'h115d;

//读取设备ID
localparam REG_READ_ADDR_FPGA_CHIP_ID_LOW                       = 16'hfff0;
localparam REG_READ_ADDR_FPGA_CHIP_ID_HIGH                      = 16'hfff1;
//读取设备是否忙
localparam REG_READ_ADDR_FPGA_CHIP_BUSY                         = 16'hfff2;

//写入寄存器地址定义
//写入地址低位
localparam REG_WRITE_ADDR_REG_LOW                               = 16'hfffb;
//写入地址高位
localparam REG_WRITE_ADDR_REG_HIGH                              = 16'hfffc;
//写入数据低位
localparam REG_WRITE_ADDR_DATA_LOW                              = 16'hfffd;
//写入数据高位
localparam REG_WRITE_ADDR_DATA_HIGH                             = 16'hfffe;

/**********************************网线定义*******************************************/
//读取信号
wire fsmc_comm_read_wire;
//写入信号
wire fsmc_comm_write_wire;

//电机中断寄存器
wire[31:0] step_motor_int_wire;

/*********************************寄存器定义*****************************************/
//地址信号锁存
reg[15:0] fsmc_comm_addr_reg_read;
//读取数据寄存器
reg [15:0] fsmc_comm_data_out_reg;

//地址信号锁存
reg[15:0] fsmc_comm_addr_reg_write;

//写入数据缓存,低16位数据
reg [15:0] fsmc_comm_data_write_addr_low_reg;
reg [15:0] fsmc_comm_data_write_addr_high_reg;
reg [15:0] fsmc_comm_data_write_data_low_reg;
//写信号保持时间计数
reg[15:0] fsmc_write_singal_hold_clk_count;

//锁存地址下拉信号
reg [1:0]fsmc_comm_write_reg;

/*************************************************************************************/
/**********************************组合逻辑代码****************************************/
/*************************************************************************************/
//读有效信号
assign fsmc_comm_read_wire = (fsmc_comm_ne | fsmc_comm_noe);
//写有效信号
assign fsmc_comm_write_wire = (fsmc_comm_ne | fsmc_comm_nwe);
//输出信号,输出状态,不读取的时候输出高阻态,读取的时候输出寄存器值
assign fsmc_comm_data = (!fsmc_comm_read_wire) ? fsmc_comm_data_out_reg : 16'hzzzz;
//输出电机中断
assign motor_int_out[0] = (|step_motor_int_wire[3:0]);
assign motor_int_out[1] = (|step_motor_int_wire[7:4]);
assign motor_int_out[2] = (|step_motor_int_wire[11:8]);
assign motor_int_out[3] = (|step_motor_int_wire[15:12]);

//关联电机中断线
assign step_motor_int_wire = {16'd0,
                              step_motor_16_readable_regs[14*32],
                              step_motor_15_readable_regs[14*32],
                              step_motor_14_readable_regs[14*32],
                              step_motor_13_readable_regs[14*32],
                              step_motor_12_readable_regs[14*32],
                              step_motor_11_readable_regs[14*32],
                              step_motor_10_readable_regs[14*32],
                              step_motor_9_readable_regs[14*32],
                              step_motor_8_readable_regs[14*32],
                              step_motor_7_readable_regs[14*32],
                              step_motor_6_readable_regs[14*32],
                              step_motor_5_readable_regs[14*32],
                              step_motor_4_readable_regs[14*32],
                              step_motor_3_readable_regs[14*32],
                              step_motor_2_readable_regs[14*32],
                              step_motor_1_readable_regs[14*32]
                            };

/*************************************************************************************/
/**********************************时序逻辑代码****************************************/
/*************************************************************************************/
//读取地址信号锁存逻辑
always @(negedge sys_clk or negedge fsmc_comm_ne or negedge sys_rst_n)
begin
    if(1'b0 == sys_rst_n) begin
        fsmc_comm_addr_reg_read <= 16'hffff;
    end
    else if(1'b0 == fsmc_comm_ne)
    begin
        fsmc_comm_addr_reg_read <= fsmc_comm_addr;
    end
    else begin
        fsmc_comm_addr_reg_read <= 16'hffff;
    end
end

//读取逻辑
always @(negedge sys_clk or negedge sys_rst_n)
begin
    if(1'b0 == sys_rst_n) begin
        //信号复位
        fsmc_comm_data_out_reg <= 16'h0000;
    end
    else begin
        //读取下降沿写入数据
        case(fsmc_comm_addr_reg_read)
            READ_ADDR_FPGA_SOFT_VERSION_LOW: begin
                //读取设备版本号,低位
                fsmc_comm_data_out_reg <= FSMC_FPGA_SOFT_VERSION_SUB;
            end
            READ_ADDR_FPGA_SOFT_VERSION_HIGH: begin
                //读取设备版本号,高位
                fsmc_comm_data_out_reg <= FSMC_FPGA_SOFT_VERSION_MAIN;
            end
            REG_READ_ADDR_FPGA_CHIP_ID_LOW: begin
                //读取设备ID,低位
                fsmc_comm_data_out_reg <= FSMC_FPGA_CHIP_ID_LOW;
            end
            REG_READ_ADDR_FPGA_CHIP_ID_HIGH: begin
                //读取设备ID,高位
                fsmc_comm_data_out_reg <= FSMC_FPGA_CHIP_ID_HIGH;
            end
            REG_READ_ADDR_FPGA_CHIP_BUSY: begin
                //读取是否忙
                fsmc_comm_data_out_reg <= {15'd0,fsmc_write_singal};
            end
            /********************************电机中断寄存器******************************/
            READ_ADDR_STEP_MOTOR_INT_LOW: begin
                //输入端口A模式寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_int_wire[15:0];
            end
            READ_ADDR_STEP_MOTOR_INT_HIGH: begin
                //输入端口A模式寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_int_wire[31:16];
            end
            /********************************端口输入状态寄存器******************************/
            //输入端口A
            REG_READ_ADDR_PORT_IN_GROUP_A_MODE_LOW: begin
                //输入端口A模式寄存器,低位
                fsmc_comm_data_out_reg <= port_in_group_a_readable_regs[((0*32)+15):(0*32)];
            end
            REG_READ_ADDR_PORT_IN_GROUP_A_MODE_HIGH: begin
                //输入端口A模式寄存器,高位
                fsmc_comm_data_out_reg <= port_in_group_a_readable_regs[((0*32)+31):((0*32)+16)];
            end
            REG_READ_ADDR_PORT_IN_GROUP_A_STATE_LOW: begin
                //输入端口A状态寄存器,低位
                fsmc_comm_data_out_reg <= port_in_group_a_readable_regs[((1*32)+15):(1*32)];
            end
            REG_READ_ADDR_PORT_IN_GROUP_A_STATE_HIGH: begin
                //输入端口A状态寄存器,高位
                fsmc_comm_data_out_reg <= port_in_group_a_readable_regs[((1*32)+31):((1*32)+16)];
            end
            //输入端口B
            REG_READ_ADDR_PORT_IN_GROUP_B_MODE_LOW: begin
                //输入端口B模式寄存器,低位
                fsmc_comm_data_out_reg <= port_in_group_b_readable_regs[((0*32)+15):(0*32)];
            end
            REG_READ_ADDR_PORT_IN_GROUP_B_MODE_HIGH: begin
                //输入端口B模式寄存器,高位
                fsmc_comm_data_out_reg <= port_in_group_b_readable_regs[((0*32)+31):((0*32)+16)];
            end
            REG_READ_ADDR_PORT_IN_GROUP_B_STATE_LOW: begin
                //输入端口B状态寄存器,低位
                fsmc_comm_data_out_reg <= port_in_group_b_readable_regs[((1*32)+15):(1*32)];
            end
            REG_READ_ADDR_PORT_IN_GROUP_B_STATE_HIGH: begin
                //输入端口B状态寄存器,高位
                fsmc_comm_data_out_reg <= port_in_group_b_readable_regs[((1*32)+31):((1*32)+16)];
            end
            //输入端口C
            REG_READ_ADDR_PORT_IN_GROUP_C_MODE_LOW: begin
                //输入端口C模式寄存器,低位
                fsmc_comm_data_out_reg <= port_in_group_c_readable_regs[((0*32)+15):(0*32)];
            end
            REG_READ_ADDR_PORT_IN_GROUP_C_MODE_HIGH: begin
                //输入端口C模式寄存器,高位
                fsmc_comm_data_out_reg <= port_in_group_c_readable_regs[((0*32)+31):((0*32)+16)];
            end
            REG_READ_ADDR_PORT_IN_GROUP_C_STATE_LOW: begin
                //输入端口C状态寄存器,低位
                fsmc_comm_data_out_reg <= port_in_group_c_readable_regs[((1*32)+15):(1*32)];
            end
            REG_READ_ADDR_PORT_IN_GROUP_C_STATE_HIGH: begin
                //输入端口C状态寄存器,高位
                fsmc_comm_data_out_reg <= port_in_group_c_readable_regs[((1*32)+31):((1*32)+16)];
            end
            //输入端口D
            REG_READ_ADDR_PORT_IN_GROUP_D_MODE_LOW: begin
                //输入端口D模式寄存器,低位
                fsmc_comm_data_out_reg <= port_in_group_d_readable_regs[((0*32)+15):(0*32)];
            end
            REG_READ_ADDR_PORT_IN_GROUP_D_MODE_HIGH: begin
                //输入端口D模式寄存器,高位
                fsmc_comm_data_out_reg <= port_in_group_d_readable_regs[((0*32)+31):((0*32)+16)];
            end
            REG_READ_ADDR_PORT_IN_GROUP_D_STATE_LOW: begin
                //输入端口D状态寄存器,低位
                fsmc_comm_data_out_reg <= port_in_group_d_readable_regs[((1*32)+15):(1*32)];
            end
            REG_READ_ADDR_PORT_IN_GROUP_D_STATE_HIGH: begin
                //输入端口D状态寄存器,高位
                fsmc_comm_data_out_reg <= port_in_group_d_readable_regs[((1*32)+31):((1*32)+16)];
            end
            /********************************端口输出状态寄存器******************************/
            //输出端口A
            REG_READ_ADDR_PORT_OUT_GROUP_A_MODE_LOW: begin
                //输出端口A输出模式寄存器,低位
                fsmc_comm_data_out_reg <= port_out_group_a_readable_regs[((0*32)+15):(0*32)];
            end
            REG_READ_ADDR_PORT_OUT_GROUP_A_MODE_HIGH: begin
                //输出端口A输出模式寄存器,高位
                fsmc_comm_data_out_reg <= port_out_group_a_readable_regs[((0*32)+31):((0*32)+16)];
            end
            REG_READ_ADDR_PORT_OUT_GROUP_A_SET_LOW: begin
                //输出端口A状态设置寄存器,低位
                fsmc_comm_data_out_reg <= port_out_group_a_readable_regs[((1*32)+15):(1*32)];
            end
            REG_READ_ADDR_PORT_OUT_GROUP_A_SET_HIGH: begin
                //输出端口A状态设置寄存器,高位
                fsmc_comm_data_out_reg <= port_out_group_a_readable_regs[((1*32)+31):((1*32)+16)];
            end
            REG_READ_ADDR_PORT_OUT_GROUP_A_REAL_LOW: begin
                //输出端口A实际输出寄存器,低位
                fsmc_comm_data_out_reg <= port_out_group_a_readable_regs[((2*32)+15):(2*32)];
            end
            REG_READ_ADDR_PORT_OUT_GROUP_A_REAL_HIGH: begin
                //输出端口A实际输出寄存器,高位
                fsmc_comm_data_out_reg <= port_out_group_a_readable_regs[((2*32)+31):((2*32)+16)];
            end
            //输出端口B
            REG_READ_ADDR_PORT_OUT_GROUP_B_MODE_LOW: begin
                //输出端口B输出模式寄存器,低位
                fsmc_comm_data_out_reg <= port_out_group_b_readable_regs[((0*32)+15):(0*32)];
            end
            REG_READ_ADDR_PORT_OUT_GROUP_B_MODE_HIGH: begin
                //输出端口B输出模式寄存器,高位
                fsmc_comm_data_out_reg <= port_out_group_b_readable_regs[((0*32)+31):((0*32)+16)];
            end
            REG_READ_ADDR_PORT_OUT_GROUP_B_SET_LOW: begin
                //输出端口B状态设置寄存器,低位
                fsmc_comm_data_out_reg <= port_out_group_b_readable_regs[((1*32)+15):(1*32)];
            end
            REG_READ_ADDR_PORT_OUT_GROUP_B_SET_HIGH: begin
                //输出端口B状态设置寄存器,高位
                fsmc_comm_data_out_reg <= port_out_group_b_readable_regs[((1*32)+31):((1*32)+16)];
            end
            REG_READ_ADDR_PORT_OUT_GROUP_B_REAL_LOW: begin
                //输出端口B实际输出寄存器,低位
                fsmc_comm_data_out_reg <= port_out_group_b_readable_regs[((2*32)+15):(2*32)];
            end
            REG_READ_ADDR_PORT_OUT_GROUP_B_REAL_HIGH: begin
                //输出端口B实际输出寄存器,高位
                fsmc_comm_data_out_reg <= port_out_group_b_readable_regs[((2*32)+31):((2*32)+16)];
            end
            //输出端口C
            REG_READ_ADDR_PORT_OUT_GROUP_C_MODE_LOW: begin
                //输出端口C输出模式寄存器,低位
                fsmc_comm_data_out_reg <= port_out_group_c_readable_regs[((0*32)+15):(0*32)];
            end
            REG_READ_ADDR_PORT_OUT_GROUP_C_MODE_HIGH: begin
                //输出端口C输出模式寄存器,高位
                fsmc_comm_data_out_reg <= port_out_group_c_readable_regs[((0*32)+31):((0*32)+16)];
            end
            REG_READ_ADDR_PORT_OUT_GROUP_C_SET_LOW: begin
                //输出端口C状态设置寄存器,低位
                fsmc_comm_data_out_reg <= port_out_group_c_readable_regs[((1*32)+15):(1*32)];
            end
            REG_READ_ADDR_PORT_OUT_GROUP_C_SET_HIGH: begin
                //输出端口C状态设置寄存器,高位
                fsmc_comm_data_out_reg <= port_out_group_c_readable_regs[((1*32)+31):((1*32)+16)];
            end
            REG_READ_ADDR_PORT_OUT_GROUP_C_REAL_LOW: begin
                //输出端口C实际输出寄存器,低位
                fsmc_comm_data_out_reg <= port_out_group_c_readable_regs[((2*32)+15):(2*32)];
            end
            REG_READ_ADDR_PORT_OUT_GROUP_C_REAL_HIGH: begin
                //输出端口C实际输出寄存器,高位
                fsmc_comm_data_out_reg <= port_out_group_c_readable_regs[((2*32)+31):((2*32)+16)];
            end
            //输出端口D
            REG_READ_ADDR_PORT_OUT_GROUP_D_MODE_LOW: begin
                //输出端口D输出模式寄存器,低位
                fsmc_comm_data_out_reg <= port_out_group_d_readable_regs[((0*32)+15):(0*32)];
            end
            REG_READ_ADDR_PORT_OUT_GROUP_D_MODE_HIGH: begin
                //输出端口D输出模式寄存器,高位
                fsmc_comm_data_out_reg <= port_out_group_d_readable_regs[((0*32)+31):((0*32)+16)];
            end
            REG_READ_ADDR_PORT_OUT_GROUP_D_SET_LOW: begin
                //输出端口D状态设置寄存器,低位
                fsmc_comm_data_out_reg <= port_out_group_d_readable_regs[((1*32)+15):(1*32)];
            end
            REG_READ_ADDR_PORT_OUT_GROUP_D_SET_HIGH: begin
                //输出端口D状态设置寄存器,高位
                fsmc_comm_data_out_reg <= port_out_group_d_readable_regs[((1*32)+31):((1*32)+16)];
            end
            REG_READ_ADDR_PORT_OUT_GROUP_D_REAL_LOW: begin
                //输出端口D实际输出寄存器,低位
                fsmc_comm_data_out_reg <= port_out_group_d_readable_regs[((2*32)+15):(2*32)];
            end
            REG_READ_ADDR_PORT_OUT_GROUP_D_REAL_HIGH: begin
                //输出端口D实际输出寄存器,高位
                fsmc_comm_data_out_reg <= port_out_group_d_readable_regs[((2*32)+31):((2*32)+16)];
            end
            /********************************步进电机1可读寄存器******************************/
            REG_READ_ADDR_STEP_MOTOR1_CONFIG_1_LOW: begin
                //步进电机1配置寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_1_readable_regs[((0*32)+15):(0*32)];
            end
            REG_READ_ADDR_STEP_MOTOR1_CONFIG_1_HIGH: begin
                //步进电机1配置寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_1_readable_regs[((0*32)+31):((0*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR1_PARAM1_LOW: begin
                //步进电机1参数寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_1_readable_regs[((1*32)+15):(1*32)];
            end
            REG_READ_ADDR_STEP_MOTOR1_PARAM1_HIGH: begin
                //步进电机1参数寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_1_readable_regs[((1*32)+31):((1*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR1_PARAM2_LOW: begin
                //步进电机1参数寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_1_readable_regs[((2*32)+15):(2*32)];
            end
            REG_READ_ADDR_STEP_MOTOR1_PARAM2_HIGH: begin
                //步进电机1参数寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_1_readable_regs[((2*32)+31):((2*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR1_PARAM3_LOW: begin
                //步进电机1参数寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_1_readable_regs[((3*32)+15):(3*32)];
            end
            REG_READ_ADDR_STEP_MOTOR1_PARAM3_HIGH: begin
                //步进电机1参数寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_1_readable_regs[((3*32)+31):((3*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR1_PARAM4_LOW: begin
                //步进电机1参数寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_1_readable_regs[((4*32)+15):(4*32)];
            end
            REG_READ_ADDR_STEP_MOTOR1_PARAM4_HIGH: begin
                //步进电机1参数寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_1_readable_regs[((4*32)+31):((4*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR1_PARAM5_LOW: begin
                //步进电机1参数寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_1_readable_regs[((5*32)+15):(5*32)];
            end
            REG_READ_ADDR_STEP_MOTOR1_PARAM5_HIGH: begin
                //步进电机1参数寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_1_readable_regs[((5*32)+31):((5*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR1_CONTROL1_LOW: begin
                //步进电机1控制寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_1_readable_regs[((6*32)+15):(6*32)];
            end
            REG_READ_ADDR_STEP_MOTOR1_CONTROL1_HIGH: begin
                //步进电机1控制寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_1_readable_regs[((6*32)+31):((6*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR1_STATE1_LOW: begin
                //步进电机1状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_1_readable_regs[((7*32)+15):(7*32)];
            end
            REG_READ_ADDR_STEP_MOTOR1_STATE1_HIGH: begin
                //步进电机1状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_1_readable_regs[((7*32)+31):((7*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR1_STATE2_LOW: begin
                //步进电机1状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_1_readable_regs[((8*32)+15):(8*32)];
            end
            REG_READ_ADDR_STEP_MOTOR1_STATE2_HIGH: begin
                //步进电机1状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_1_readable_regs[((8*32)+31):((8*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR1_STATE3_LOW: begin
                //步进电机1状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_1_readable_regs[((9*32)+15):(9*32)];
            end
            REG_READ_ADDR_STEP_MOTOR1_STATE3_HIGH: begin
                //步进电机1状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_1_readable_regs[((9*32)+31):((9*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR1_STATE4_LOW: begin
                //步进电机1状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_1_readable_regs[((10*32)+15):(10*32)];
            end
            REG_READ_ADDR_STEP_MOTOR1_STATE4_HIGH: begin
                //步进电机1状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_1_readable_regs[((10*32)+31):((10*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR1_STATE5_LOW: begin
                //步进电机1状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_1_readable_regs[((11*32)+15):(11*32)];
            end
            REG_READ_ADDR_STEP_MOTOR1_STATE5_HIGH: begin
                //步进电机1状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_1_readable_regs[((11*32)+31):((11*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR1_SPD_TAB_ELEM_LOW: begin
                //步进电机1状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_1_readable_regs[((12*32)+15):(12*32)];
            end
            REG_READ_ADDR_STEP_MOTOR1_SPD_TAB_ELEM_HIGH: begin
                //步进电机1状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_1_readable_regs[((12*32)+31):((12*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR1_STP_TAB_ELEM_LOW: begin
                //步进电机1状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_1_readable_regs[((13*32)+15):(13*32)];
            end
            REG_READ_ADDR_STEP_MOTOR1_STP_TAB_ELEM_HIGH: begin
                //步进电机1状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_1_readable_regs[((13*32)+31):((13*32)+16)];
            end
            /********************************步进电机2可读寄存器******************************/
            REG_READ_ADDR_STEP_MOTOR2_CONFIG_1_LOW: begin
                //步进电机2配置寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_2_readable_regs[((0*32)+15):(0*32)];
            end
            REG_READ_ADDR_STEP_MOTOR2_CONFIG_1_HIGH: begin
                //步进电机2配置寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_2_readable_regs[((0*32)+31):((0*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR2_PARAM1_LOW: begin
                //步进电机2参数寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_2_readable_regs[((1*32)+15):(1*32)];
            end
            REG_READ_ADDR_STEP_MOTOR2_PARAM1_HIGH: begin
                //步进电机2参数寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_2_readable_regs[((1*32)+31):((1*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR2_PARAM2_LOW: begin
                //步进电机2参数寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_2_readable_regs[((2*32)+15):(2*32)];
            end
            REG_READ_ADDR_STEP_MOTOR2_PARAM2_HIGH: begin
                //步进电机2参数寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_2_readable_regs[((2*32)+31):((2*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR2_PARAM3_LOW: begin
                //步进电机2参数寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_2_readable_regs[((3*32)+15):(3*32)];
            end
            REG_READ_ADDR_STEP_MOTOR2_PARAM3_HIGH: begin
                //步进电机2参数寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_2_readable_regs[((3*32)+31):((3*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR2_PARAM4_LOW: begin
                //步进电机2参数寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_2_readable_regs[((4*32)+15):(4*32)];
            end
            REG_READ_ADDR_STEP_MOTOR2_PARAM4_HIGH: begin
                //步进电机2参数寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_2_readable_regs[((4*32)+31):((4*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR2_PARAM5_LOW: begin
                //步进电机2参数寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_2_readable_regs[((5*32)+15):(5*32)];
            end
            REG_READ_ADDR_STEP_MOTOR2_PARAM5_HIGH: begin
                //步进电机2参数寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_2_readable_regs[((5*32)+31):((5*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR2_CONTROL1_LOW: begin
                //步进电机2控制寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_2_readable_regs[((6*32)+15):(6*32)];
            end
            REG_READ_ADDR_STEP_MOTOR2_CONTROL1_HIGH: begin
                //步进电机2控制寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_2_readable_regs[((6*32)+31):((6*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR2_STATE1_LOW: begin
                //步进电机2状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_2_readable_regs[((7*32)+15):(7*32)];
            end
            REG_READ_ADDR_STEP_MOTOR2_STATE1_HIGH: begin
                //步进电机2状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_2_readable_regs[((7*32)+31):((7*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR2_STATE2_LOW: begin
                //步进电机2状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_2_readable_regs[((8*32)+15):(8*32)];
            end
            REG_READ_ADDR_STEP_MOTOR2_STATE2_HIGH: begin
                //步进电机2状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_2_readable_regs[((8*32)+31):((8*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR2_STATE3_LOW: begin
                //步进电机2状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_2_readable_regs[((9*32)+15):(9*32)];
            end
            REG_READ_ADDR_STEP_MOTOR2_STATE3_HIGH: begin
                //步进电机2状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_2_readable_regs[((9*32)+31):((9*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR2_STATE4_LOW: begin
                //步进电机2状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_2_readable_regs[((10*32)+15):(10*32)];
            end
            REG_READ_ADDR_STEP_MOTOR2_STATE4_HIGH: begin
                //步进电机2状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_2_readable_regs[((10*32)+31):((10*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR2_STATE5_LOW: begin
                //步进电机2状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_2_readable_regs[((11*32)+15):(11*32)];
            end
            REG_READ_ADDR_STEP_MOTOR2_STATE5_HIGH: begin
                //步进电机2状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_2_readable_regs[((11*32)+31):((11*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR2_SPD_TAB_ELEM_LOW: begin
                //步进电机2状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_2_readable_regs[((12*32)+15):(12*32)];
            end
            REG_READ_ADDR_STEP_MOTOR2_SPD_TAB_ELEM_HIGH: begin
                //步进电机2状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_2_readable_regs[((12*32)+31):((12*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR2_STP_TAB_ELEM_LOW: begin
                //步进电机2状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_2_readable_regs[((13*32)+15):(13*32)];
            end
            REG_READ_ADDR_STEP_MOTOR2_STP_TAB_ELEM_HIGH: begin
                //步进电机2状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_2_readable_regs[((13*32)+31):((13*32)+16)];
            end
            /********************************步进电机3可读寄存器******************************/
            REG_READ_ADDR_STEP_MOTOR3_CONFIG_1_LOW: begin
                //步进电机3配置寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_3_readable_regs[((0*32)+15):(0*32)];
            end
            REG_READ_ADDR_STEP_MOTOR3_CONFIG_1_HIGH: begin
                //步进电机3配置寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_3_readable_regs[((0*32)+31):((0*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR3_PARAM1_LOW: begin
                //步进电机3参数寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_3_readable_regs[((1*32)+15):(1*32)];
            end
            REG_READ_ADDR_STEP_MOTOR3_PARAM1_HIGH: begin
                //步进电机3参数寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_3_readable_regs[((1*32)+31):((1*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR3_PARAM2_LOW: begin
                //步进电机3参数寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_3_readable_regs[((2*32)+15):(2*32)];
            end
            REG_READ_ADDR_STEP_MOTOR3_PARAM2_HIGH: begin
                //步进电机3参数寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_3_readable_regs[((2*32)+31):((2*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR3_PARAM3_LOW: begin
                //步进电机3参数寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_3_readable_regs[((3*32)+15):(3*32)];
            end
            REG_READ_ADDR_STEP_MOTOR3_PARAM3_HIGH: begin
                //步进电机3参数寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_3_readable_regs[((3*32)+31):((3*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR3_PARAM4_LOW: begin
                //步进电机3参数寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_3_readable_regs[((4*32)+15):(4*32)];
            end
            REG_READ_ADDR_STEP_MOTOR3_PARAM4_HIGH: begin
                //步进电机3参数寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_3_readable_regs[((4*32)+31):((4*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR3_PARAM5_LOW: begin
                //步进电机3参数寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_3_readable_regs[((5*32)+15):(5*32)];
            end
            REG_READ_ADDR_STEP_MOTOR3_PARAM5_HIGH: begin
                //步进电机3参数寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_3_readable_regs[((5*32)+31):((5*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR3_CONTROL1_LOW: begin
                //步进电机3控制寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_3_readable_regs[((6*32)+15):(6*32)];
            end
            REG_READ_ADDR_STEP_MOTOR3_CONTROL1_HIGH: begin
                //步进电机3控制寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_3_readable_regs[((6*32)+31):((6*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR3_STATE1_LOW: begin
                //步进电机3状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_3_readable_regs[((7*32)+15):(7*32)];
            end
            REG_READ_ADDR_STEP_MOTOR3_STATE1_HIGH: begin
                //步进电机3状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_3_readable_regs[((7*32)+31):((7*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR3_STATE2_LOW: begin
                //步进电机3状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_3_readable_regs[((8*32)+15):(8*32)];
            end
            REG_READ_ADDR_STEP_MOTOR3_STATE2_HIGH: begin
                //步进电机3状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_3_readable_regs[((8*32)+31):((8*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR3_STATE3_LOW: begin
                //步进电机3状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_3_readable_regs[((9*32)+15):(9*32)];
            end
            REG_READ_ADDR_STEP_MOTOR3_STATE3_HIGH: begin
                //步进电机3状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_3_readable_regs[((9*32)+31):((9*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR3_STATE4_LOW: begin
                //步进电机3状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_3_readable_regs[((10*32)+15):(10*32)];
            end
            REG_READ_ADDR_STEP_MOTOR3_STATE4_HIGH: begin
                //步进电机3状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_3_readable_regs[((10*32)+31):((10*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR3_STATE5_LOW: begin
                //步进电机3状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_3_readable_regs[((11*32)+15):(11*32)];
            end
            REG_READ_ADDR_STEP_MOTOR3_STATE5_HIGH: begin
                //步进电机3状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_3_readable_regs[((11*32)+31):((11*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR3_SPD_TAB_ELEM_LOW: begin
                //步进电机3状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_3_readable_regs[((12*32)+15):(12*32)];
            end
            REG_READ_ADDR_STEP_MOTOR3_SPD_TAB_ELEM_HIGH: begin
                //步进电机3状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_3_readable_regs[((12*32)+31):((12*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR3_STP_TAB_ELEM_LOW: begin
                //步进电机3状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_3_readable_regs[((13*32)+15):(13*32)];
            end
            REG_READ_ADDR_STEP_MOTOR3_STP_TAB_ELEM_HIGH: begin
                //步进电机3状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_3_readable_regs[((13*32)+31):((13*32)+16)];
            end
            /********************************步进电机4可读寄存器******************************/
            REG_READ_ADDR_STEP_MOTOR4_CONFIG_1_LOW: begin
                //步进电机4配置寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_4_readable_regs[((0*32)+15):(0*32)];
            end
            REG_READ_ADDR_STEP_MOTOR4_CONFIG_1_HIGH: begin
                //步进电机4配置寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_4_readable_regs[((0*32)+31):((0*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR4_PARAM1_LOW: begin
                //步进电机4参数寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_4_readable_regs[((1*32)+15):(1*32)];
            end
            REG_READ_ADDR_STEP_MOTOR4_PARAM1_HIGH: begin
                //步进电机4参数寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_4_readable_regs[((1*32)+31):((1*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR4_PARAM2_LOW: begin
                //步进电机4参数寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_4_readable_regs[((2*32)+15):(2*32)];
            end
            REG_READ_ADDR_STEP_MOTOR4_PARAM2_HIGH: begin
                //步进电机4参数寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_4_readable_regs[((2*32)+31):((2*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR4_PARAM3_LOW: begin
                //步进电机4参数寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_4_readable_regs[((3*32)+15):(3*32)];
            end
            REG_READ_ADDR_STEP_MOTOR4_PARAM3_HIGH: begin
                //步进电机4参数寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_4_readable_regs[((3*32)+31):((3*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR4_PARAM4_LOW: begin
                //步进电机4参数寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_4_readable_regs[((4*32)+15):(4*32)];
            end
            REG_READ_ADDR_STEP_MOTOR4_PARAM4_HIGH: begin
                //步进电机4参数寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_4_readable_regs[((4*32)+31):((4*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR4_PARAM5_LOW: begin
                //步进电机4参数寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_4_readable_regs[((5*32)+15):(5*32)];
            end
            REG_READ_ADDR_STEP_MOTOR4_PARAM5_HIGH: begin
                //步进电机4参数寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_4_readable_regs[((5*32)+31):((5*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR4_CONTROL1_LOW: begin
                //步进电机4控制寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_4_readable_regs[((6*32)+15):(6*32)];
            end
            REG_READ_ADDR_STEP_MOTOR4_CONTROL1_HIGH: begin
                //步进电机4控制寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_4_readable_regs[((6*32)+31):((6*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR4_STATE1_LOW: begin
                //步进电机4状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_4_readable_regs[((7*32)+15):(7*32)];
            end
            REG_READ_ADDR_STEP_MOTOR4_STATE1_HIGH: begin
                //步进电机4状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_4_readable_regs[((7*32)+31):((7*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR4_STATE2_LOW: begin
                //步进电机4状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_4_readable_regs[((8*32)+15):(8*32)];
            end
            REG_READ_ADDR_STEP_MOTOR4_STATE2_HIGH: begin
                //步进电机4状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_4_readable_regs[((8*32)+31):((8*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR4_STATE3_LOW: begin
                //步进电机4状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_4_readable_regs[((9*32)+15):(9*32)];
            end
            REG_READ_ADDR_STEP_MOTOR4_STATE3_HIGH: begin
                //步进电机4状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_4_readable_regs[((9*32)+31):((9*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR4_STATE4_LOW: begin
                //步进电机4状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_4_readable_regs[((10*32)+15):(10*32)];
            end
            REG_READ_ADDR_STEP_MOTOR4_STATE4_HIGH: begin
                //步进电机4状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_4_readable_regs[((10*32)+31):((10*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR4_STATE5_LOW: begin
                //步进电机4状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_4_readable_regs[((11*32)+15):(11*32)];
            end
            REG_READ_ADDR_STEP_MOTOR4_STATE5_HIGH: begin
                //步进电机4状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_4_readable_regs[((11*32)+31):((11*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR4_SPD_TAB_ELEM_LOW: begin
                //步进电机4状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_4_readable_regs[((12*32)+15):(12*32)];
            end
            REG_READ_ADDR_STEP_MOTOR4_SPD_TAB_ELEM_HIGH: begin
                //步进电机4状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_4_readable_regs[((12*32)+31):((12*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR4_STP_TAB_ELEM_LOW: begin
                //步进电机4状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_4_readable_regs[((13*32)+15):(13*32)];
            end
            REG_READ_ADDR_STEP_MOTOR4_STP_TAB_ELEM_HIGH: begin
                //步进电机4状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_4_readable_regs[((13*32)+31):((13*32)+16)];
            end
            /********************************步进电机5可读寄存器******************************/
            REG_READ_ADDR_STEP_MOTOR5_CONFIG_1_LOW: begin
                //步进电机5配置寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_5_readable_regs[((0*32)+15):(0*32)];
            end
            REG_READ_ADDR_STEP_MOTOR5_CONFIG_1_HIGH: begin
                //步进电机5配置寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_5_readable_regs[((0*32)+31):((0*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR5_PARAM1_LOW: begin
                //步进电机5参数寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_5_readable_regs[((1*32)+15):(1*32)];
            end
            REG_READ_ADDR_STEP_MOTOR5_PARAM1_HIGH: begin
                //步进电机5参数寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_5_readable_regs[((1*32)+31):((1*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR5_PARAM2_LOW: begin
                //步进电机5参数寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_5_readable_regs[((2*32)+15):(2*32)];
            end
            REG_READ_ADDR_STEP_MOTOR5_PARAM2_HIGH: begin
                //步进电机5参数寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_5_readable_regs[((2*32)+31):((2*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR5_PARAM3_LOW: begin
                //步进电机5参数寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_5_readable_regs[((3*32)+15):(3*32)];
            end
            REG_READ_ADDR_STEP_MOTOR5_PARAM3_HIGH: begin
                //步进电机5参数寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_5_readable_regs[((3*32)+31):((3*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR5_PARAM4_LOW: begin
                //步进电机5参数寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_5_readable_regs[((4*32)+15):(4*32)];
            end
            REG_READ_ADDR_STEP_MOTOR5_PARAM4_HIGH: begin
                //步进电机5参数寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_5_readable_regs[((4*32)+31):((4*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR5_PARAM5_LOW: begin
                //步进电机5参数寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_5_readable_regs[((5*32)+15):(5*32)];
            end
            REG_READ_ADDR_STEP_MOTOR5_PARAM5_HIGH: begin
                //步进电机5参数寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_5_readable_regs[((5*32)+31):((5*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR5_CONTROL1_LOW: begin
                //步进电机5控制寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_5_readable_regs[((6*32)+15):(6*32)];
            end
            REG_READ_ADDR_STEP_MOTOR5_CONTROL1_HIGH: begin
                //步进电机5控制寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_5_readable_regs[((6*32)+31):((6*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR5_STATE1_LOW: begin
                //步进电机5状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_5_readable_regs[((7*32)+15):(7*32)];
            end
            REG_READ_ADDR_STEP_MOTOR5_STATE1_HIGH: begin
                //步进电机5状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_5_readable_regs[((7*32)+31):((7*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR5_STATE2_LOW: begin
                //步进电机5状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_5_readable_regs[((8*32)+15):(8*32)];
            end
            REG_READ_ADDR_STEP_MOTOR5_STATE2_HIGH: begin
                //步进电机5状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_5_readable_regs[((8*32)+31):((8*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR5_STATE3_LOW: begin
                //步进电机5状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_5_readable_regs[((9*32)+15):(9*32)];
            end
            REG_READ_ADDR_STEP_MOTOR5_STATE3_HIGH: begin
                //步进电机5状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_5_readable_regs[((9*32)+31):((9*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR5_STATE4_LOW: begin
                //步进电机5状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_5_readable_regs[((10*32)+15):(10*32)];
            end
            REG_READ_ADDR_STEP_MOTOR5_STATE4_HIGH: begin
                //步进电机5状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_5_readable_regs[((10*32)+31):((10*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR5_STATE5_LOW: begin
                //步进电机5状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_5_readable_regs[((11*32)+15):(11*32)];
            end
            REG_READ_ADDR_STEP_MOTOR5_STATE5_HIGH: begin
                //步进电机5状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_5_readable_regs[((11*32)+31):((11*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR5_SPD_TAB_ELEM_LOW: begin
                //步进电机5状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_5_readable_regs[((12*32)+15):(12*32)];
            end
            REG_READ_ADDR_STEP_MOTOR5_SPD_TAB_ELEM_HIGH: begin
                //步进电机5状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_5_readable_regs[((12*32)+31):((12*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR5_STP_TAB_ELEM_LOW: begin
                //步进电机5状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_5_readable_regs[((13*32)+15):(13*32)];
            end
            REG_READ_ADDR_STEP_MOTOR5_STP_TAB_ELEM_HIGH: begin
                //步进电机5状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_5_readable_regs[((13*32)+31):((13*32)+16)];
            end
            /********************************步进电机6可读寄存器******************************/
            REG_READ_ADDR_STEP_MOTOR6_CONFIG_1_LOW: begin
                //步进电机6配置寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_6_readable_regs[((0*32)+15):(0*32)];
            end
            REG_READ_ADDR_STEP_MOTOR6_CONFIG_1_HIGH: begin
                //步进电机6配置寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_6_readable_regs[((0*32)+31):((0*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR6_PARAM1_LOW: begin
                //步进电机6参数寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_6_readable_regs[((1*32)+15):(1*32)];
            end
            REG_READ_ADDR_STEP_MOTOR6_PARAM1_HIGH: begin
                //步进电机6参数寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_6_readable_regs[((1*32)+31):((1*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR6_PARAM2_LOW: begin
                //步进电机6参数寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_6_readable_regs[((2*32)+15):(2*32)];
            end
            REG_READ_ADDR_STEP_MOTOR6_PARAM2_HIGH: begin
                //步进电机6参数寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_6_readable_regs[((2*32)+31):((2*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR6_PARAM3_LOW: begin
                //步进电机6参数寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_6_readable_regs[((3*32)+15):(3*32)];
            end
            REG_READ_ADDR_STEP_MOTOR6_PARAM3_HIGH: begin
                //步进电机6参数寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_6_readable_regs[((3*32)+31):((3*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR6_PARAM4_LOW: begin
                //步进电机6参数寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_6_readable_regs[((4*32)+15):(4*32)];
            end
            REG_READ_ADDR_STEP_MOTOR6_PARAM4_HIGH: begin
                //步进电机6参数寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_6_readable_regs[((4*32)+31):((4*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR6_PARAM5_LOW: begin
                //步进电机6参数寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_6_readable_regs[((5*32)+15):(5*32)];
            end
            REG_READ_ADDR_STEP_MOTOR6_PARAM5_HIGH: begin
                //步进电机6参数寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_6_readable_regs[((5*32)+31):((5*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR6_CONTROL1_LOW: begin
                //步进电机6控制寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_6_readable_regs[((6*32)+15):(6*32)];
            end
            REG_READ_ADDR_STEP_MOTOR6_CONTROL1_HIGH: begin
                //步进电机6控制寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_6_readable_regs[((6*32)+31):((6*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR6_STATE1_LOW: begin
                //步进电机6状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_6_readable_regs[((7*32)+15):(7*32)];
            end
            REG_READ_ADDR_STEP_MOTOR6_STATE1_HIGH: begin
                //步进电机6状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_6_readable_regs[((7*32)+31):((7*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR6_STATE2_LOW: begin
                //步进电机6状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_6_readable_regs[((8*32)+15):(8*32)];
            end
            REG_READ_ADDR_STEP_MOTOR6_STATE2_HIGH: begin
                //步进电机6状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_6_readable_regs[((8*32)+31):((8*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR6_STATE3_LOW: begin
                //步进电机6状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_6_readable_regs[((9*32)+15):(9*32)];
            end
            REG_READ_ADDR_STEP_MOTOR6_STATE3_HIGH: begin
                //步进电机6状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_6_readable_regs[((9*32)+31):((9*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR6_STATE4_LOW: begin
                //步进电机6状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_6_readable_regs[((10*32)+15):(10*32)];
            end
            REG_READ_ADDR_STEP_MOTOR6_STATE4_HIGH: begin
                //步进电机6状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_6_readable_regs[((10*32)+31):((10*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR6_STATE5_LOW: begin
                //步进电机6状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_6_readable_regs[((11*32)+15):(11*32)];
            end
            REG_READ_ADDR_STEP_MOTOR6_STATE5_HIGH: begin
                //步进电机6状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_6_readable_regs[((11*32)+31):((11*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR6_SPD_TAB_ELEM_LOW: begin
                //步进电机6状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_6_readable_regs[((12*32)+15):(12*32)];
            end
            REG_READ_ADDR_STEP_MOTOR6_SPD_TAB_ELEM_HIGH: begin
                //步进电机6状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_6_readable_regs[((12*32)+31):((12*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR6_STP_TAB_ELEM_LOW: begin
                //步进电机6状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_6_readable_regs[((13*32)+15):(13*32)];
            end
            REG_READ_ADDR_STEP_MOTOR6_STP_TAB_ELEM_HIGH: begin
                //步进电机6状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_6_readable_regs[((13*32)+31):((13*32)+16)];
            end
            /********************************步进电机7可读寄存器******************************/
            REG_READ_ADDR_STEP_MOTOR7_CONFIG_1_LOW: begin
                //步进电机7配置寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_7_readable_regs[((0*32)+15):(0*32)];
            end
            REG_READ_ADDR_STEP_MOTOR7_CONFIG_1_HIGH: begin
                //步进电机7配置寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_7_readable_regs[((0*32)+31):((0*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR7_PARAM1_LOW: begin
                //步进电机7参数寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_7_readable_regs[((1*32)+15):(1*32)];
            end
            REG_READ_ADDR_STEP_MOTOR7_PARAM1_HIGH: begin
                //步进电机7参数寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_7_readable_regs[((1*32)+31):((1*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR7_PARAM2_LOW: begin
                //步进电机7参数寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_7_readable_regs[((2*32)+15):(2*32)];
            end
            REG_READ_ADDR_STEP_MOTOR7_PARAM2_HIGH: begin
                //步进电机7参数寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_7_readable_regs[((2*32)+31):((2*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR7_PARAM3_LOW: begin
                //步进电机7参数寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_7_readable_regs[((3*32)+15):(3*32)];
            end
            REG_READ_ADDR_STEP_MOTOR7_PARAM3_HIGH: begin
                //步进电机7参数寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_7_readable_regs[((3*32)+31):((3*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR7_PARAM4_LOW: begin
                //步进电机7参数寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_7_readable_regs[((4*32)+15):(4*32)];
            end
            REG_READ_ADDR_STEP_MOTOR7_PARAM4_HIGH: begin
                //步进电机7参数寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_7_readable_regs[((4*32)+31):((4*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR7_PARAM5_LOW: begin
                //步进电机7参数寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_7_readable_regs[((5*32)+15):(5*32)];
            end
            REG_READ_ADDR_STEP_MOTOR7_PARAM5_HIGH: begin
                //步进电机7参数寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_7_readable_regs[((5*32)+31):((5*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR7_CONTROL1_LOW: begin
                //步进电机7控制寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_7_readable_regs[((6*32)+15):(6*32)];
            end
            REG_READ_ADDR_STEP_MOTOR7_CONTROL1_HIGH: begin
                //步进电机7控制寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_7_readable_regs[((6*32)+31):((6*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR7_STATE1_LOW: begin
                //步进电机7状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_7_readable_regs[((7*32)+15):(7*32)];
            end
            REG_READ_ADDR_STEP_MOTOR7_STATE1_HIGH: begin
                //步进电机7状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_7_readable_regs[((7*32)+31):((7*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR7_STATE2_LOW: begin
                //步进电机7状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_7_readable_regs[((8*32)+15):(8*32)];
            end
            REG_READ_ADDR_STEP_MOTOR7_STATE2_HIGH: begin
                //步进电机7状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_7_readable_regs[((8*32)+31):((8*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR7_STATE3_LOW: begin
                //步进电机7状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_7_readable_regs[((9*32)+15):(9*32)];
            end
            REG_READ_ADDR_STEP_MOTOR7_STATE3_HIGH: begin
                //步进电机7状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_7_readable_regs[((9*32)+31):((9*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR7_STATE4_LOW: begin
                //步进电机7状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_7_readable_regs[((10*32)+15):(10*32)];
            end
            REG_READ_ADDR_STEP_MOTOR7_STATE4_HIGH: begin
                //步进电机7状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_7_readable_regs[((10*32)+31):((10*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR7_STATE5_LOW: begin
                //步进电机7状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_7_readable_regs[((11*32)+15):(11*32)];
            end
            REG_READ_ADDR_STEP_MOTOR7_STATE5_HIGH: begin
                //步进电机7状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_7_readable_regs[((11*32)+31):((11*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR7_SPD_TAB_ELEM_LOW: begin
                //步进电机7状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_7_readable_regs[((12*32)+15):(12*32)];
            end
            REG_READ_ADDR_STEP_MOTOR7_SPD_TAB_ELEM_HIGH: begin
                //步进电机7状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_7_readable_regs[((12*32)+31):((12*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR7_STP_TAB_ELEM_LOW: begin
                //步进电机7状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_7_readable_regs[((13*32)+15):(13*32)];
            end
            REG_READ_ADDR_STEP_MOTOR7_STP_TAB_ELEM_HIGH: begin
                //步进电机7状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_7_readable_regs[((13*32)+31):((13*32)+16)];
            end
            /********************************步进电机8可读寄存器******************************/
            REG_READ_ADDR_STEP_MOTOR8_CONFIG_1_LOW: begin
                //步进电机8配置寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_8_readable_regs[((0*32)+15):(0*32)];
            end
            REG_READ_ADDR_STEP_MOTOR8_CONFIG_1_HIGH: begin
                //步进电机8配置寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_8_readable_regs[((0*32)+31):((0*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR8_PARAM1_LOW: begin
                //步进电机8参数寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_8_readable_regs[((1*32)+15):(1*32)];
            end
            REG_READ_ADDR_STEP_MOTOR8_PARAM1_HIGH: begin
                //步进电机8参数寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_8_readable_regs[((1*32)+31):((1*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR8_PARAM2_LOW: begin
                //步进电机8参数寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_8_readable_regs[((2*32)+15):(2*32)];
            end
            REG_READ_ADDR_STEP_MOTOR8_PARAM2_HIGH: begin
                //步进电机8参数寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_8_readable_regs[((2*32)+31):((2*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR8_PARAM3_LOW: begin
                //步进电机8参数寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_8_readable_regs[((3*32)+15):(3*32)];
            end
            REG_READ_ADDR_STEP_MOTOR8_PARAM3_HIGH: begin
                //步进电机8参数寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_8_readable_regs[((3*32)+31):((3*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR8_PARAM4_LOW: begin
                //步进电机8参数寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_8_readable_regs[((4*32)+15):(4*32)];
            end
            REG_READ_ADDR_STEP_MOTOR8_PARAM4_HIGH: begin
                //步进电机8参数寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_8_readable_regs[((4*32)+31):((4*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR8_PARAM5_LOW: begin
                //步进电机8参数寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_8_readable_regs[((5*32)+15):(5*32)];
            end
            REG_READ_ADDR_STEP_MOTOR8_PARAM5_HIGH: begin
                //步进电机8参数寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_8_readable_regs[((5*32)+31):((5*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR8_CONTROL1_LOW: begin
                //步进电机8控制寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_8_readable_regs[((6*32)+15):(6*32)];
            end
            REG_READ_ADDR_STEP_MOTOR8_CONTROL1_HIGH: begin
                //步进电机8控制寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_8_readable_regs[((6*32)+31):((6*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR8_STATE1_LOW: begin
                //步进电机8状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_8_readable_regs[((7*32)+15):(7*32)];
            end
            REG_READ_ADDR_STEP_MOTOR8_STATE1_HIGH: begin
                //步进电机8状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_8_readable_regs[((7*32)+31):((7*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR8_STATE2_LOW: begin
                //步进电机8状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_8_readable_regs[((8*32)+15):(8*32)];
            end
            REG_READ_ADDR_STEP_MOTOR8_STATE2_HIGH: begin
                //步进电机8状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_8_readable_regs[((8*32)+31):((8*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR8_STATE3_LOW: begin
                //步进电机8状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_8_readable_regs[((9*32)+15):(9*32)];
            end
            REG_READ_ADDR_STEP_MOTOR8_STATE3_HIGH: begin
                //步进电机8状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_8_readable_regs[((9*32)+31):((9*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR8_STATE4_LOW: begin
                //步进电机8状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_8_readable_regs[((10*32)+15):(10*32)];
            end
            REG_READ_ADDR_STEP_MOTOR8_STATE4_HIGH: begin
                //步进电机8状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_8_readable_regs[((10*32)+31):((10*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR8_STATE5_LOW: begin
                //步进电机8状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_8_readable_regs[((11*32)+15):(11*32)];
            end
            REG_READ_ADDR_STEP_MOTOR8_STATE5_HIGH: begin
                //步进电机8状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_8_readable_regs[((11*32)+31):((11*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR8_SPD_TAB_ELEM_LOW: begin
                //步进电机8状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_8_readable_regs[((12*32)+15):(12*32)];
            end
            REG_READ_ADDR_STEP_MOTOR8_SPD_TAB_ELEM_HIGH: begin
                //步进电机8状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_8_readable_regs[((12*32)+31):((12*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR8_STP_TAB_ELEM_LOW: begin
                //步进电机8状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_8_readable_regs[((13*32)+15):(13*32)];
            end
            REG_READ_ADDR_STEP_MOTOR8_STP_TAB_ELEM_HIGH: begin
                //步进电机8状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_8_readable_regs[((13*32)+31):((13*32)+16)];
            end
            /********************************步进电机9可读寄存器******************************/
            REG_READ_ADDR_STEP_MOTOR9_CONFIG_1_LOW: begin
                //步进电机9配置寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_9_readable_regs[((0*32)+15):(0*32)];
            end
            REG_READ_ADDR_STEP_MOTOR9_CONFIG_1_HIGH: begin
                //步进电机9配置寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_9_readable_regs[((0*32)+31):((0*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR9_PARAM1_LOW: begin
                //步进电机9参数寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_9_readable_regs[((1*32)+15):(1*32)];
            end
            REG_READ_ADDR_STEP_MOTOR9_PARAM1_HIGH: begin
                //步进电机9参数寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_9_readable_regs[((1*32)+31):((1*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR9_PARAM2_LOW: begin
                //步进电机9参数寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_9_readable_regs[((2*32)+15):(2*32)];
            end
            REG_READ_ADDR_STEP_MOTOR9_PARAM2_HIGH: begin
                //步进电机9参数寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_9_readable_regs[((2*32)+31):((2*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR9_PARAM3_LOW: begin
                //步进电机9参数寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_9_readable_regs[((3*32)+15):(3*32)];
            end
            REG_READ_ADDR_STEP_MOTOR9_PARAM3_HIGH: begin
                //步进电机9参数寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_9_readable_regs[((3*32)+31):((3*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR9_PARAM4_LOW: begin
                //步进电机9参数寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_9_readable_regs[((4*32)+15):(4*32)];
            end
            REG_READ_ADDR_STEP_MOTOR9_PARAM4_HIGH: begin
                //步进电机9参数寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_9_readable_regs[((4*32)+31):((4*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR9_PARAM5_LOW: begin
                //步进电机9参数寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_9_readable_regs[((5*32)+15):(5*32)];
            end
            REG_READ_ADDR_STEP_MOTOR9_PARAM5_HIGH: begin
                //步进电机9参数寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_9_readable_regs[((5*32)+31):((5*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR9_CONTROL1_LOW: begin
                //步进电机9控制寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_9_readable_regs[((6*32)+15):(6*32)];
            end
            REG_READ_ADDR_STEP_MOTOR9_CONTROL1_HIGH: begin
                //步进电机9控制寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_9_readable_regs[((6*32)+31):((6*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR9_STATE1_LOW: begin
                //步进电机9状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_9_readable_regs[((7*32)+15):(7*32)];
            end
            REG_READ_ADDR_STEP_MOTOR9_STATE1_HIGH: begin
                //步进电机9状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_9_readable_regs[((7*32)+31):((7*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR9_STATE2_LOW: begin
                //步进电机9状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_9_readable_regs[((8*32)+15):(8*32)];
            end
            REG_READ_ADDR_STEP_MOTOR9_STATE2_HIGH: begin
                //步进电机9状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_9_readable_regs[((8*32)+31):((8*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR9_STATE3_LOW: begin
                //步进电机9状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_9_readable_regs[((9*32)+15):(9*32)];
            end
            REG_READ_ADDR_STEP_MOTOR9_STATE3_HIGH: begin
                //步进电机9状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_9_readable_regs[((9*32)+31):((9*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR9_STATE4_LOW: begin
                //步进电机9状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_9_readable_regs[((10*32)+15):(10*32)];
            end
            REG_READ_ADDR_STEP_MOTOR9_STATE4_HIGH: begin
                //步进电机9状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_9_readable_regs[((10*32)+31):((10*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR9_STATE5_LOW: begin
                //步进电机9状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_9_readable_regs[((11*32)+15):(11*32)];
            end
            REG_READ_ADDR_STEP_MOTOR9_STATE5_HIGH: begin
                //步进电机9状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_9_readable_regs[((11*32)+31):((11*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR9_SPD_TAB_ELEM_LOW: begin
                //步进电机9状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_9_readable_regs[((12*32)+15):(12*32)];
            end
            REG_READ_ADDR_STEP_MOTOR9_SPD_TAB_ELEM_HIGH: begin
                //步进电机9状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_9_readable_regs[((12*32)+31):((12*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR9_STP_TAB_ELEM_LOW: begin
                //步进电机9状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_9_readable_regs[((13*32)+15):(13*32)];
            end
            REG_READ_ADDR_STEP_MOTOR9_STP_TAB_ELEM_HIGH: begin
                //步进电机9状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_9_readable_regs[((13*32)+31):((13*32)+16)];
            end
            /********************************步进电机90可读寄存器******************************/
            REG_READ_ADDR_STEP_MOTOR10_CONFIG_1_LOW: begin
                //步进电机10配置寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_10_readable_regs[((0*32)+15):(0*32)];
            end
            REG_READ_ADDR_STEP_MOTOR10_CONFIG_1_HIGH: begin
                //步进电机10配置寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_10_readable_regs[((0*32)+31):((0*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR10_PARAM1_LOW: begin
                //步进电机10参数寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_10_readable_regs[((1*32)+15):(1*32)];
            end
            REG_READ_ADDR_STEP_MOTOR10_PARAM1_HIGH: begin
                //步进电机10参数寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_10_readable_regs[((1*32)+31):((1*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR10_PARAM2_LOW: begin
                //步进电机10参数寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_10_readable_regs[((2*32)+15):(2*32)];
            end
            REG_READ_ADDR_STEP_MOTOR10_PARAM2_HIGH: begin
                //步进电机10参数寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_10_readable_regs[((2*32)+31):((2*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR10_PARAM3_LOW: begin
                //步进电机10参数寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_10_readable_regs[((3*32)+15):(3*32)];
            end
            REG_READ_ADDR_STEP_MOTOR10_PARAM3_HIGH: begin
                //步进电机10参数寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_10_readable_regs[((3*32)+31):((3*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR10_PARAM4_LOW: begin
                //步进电机10参数寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_10_readable_regs[((4*32)+15):(4*32)];
            end
            REG_READ_ADDR_STEP_MOTOR10_PARAM4_HIGH: begin
                //步进电机10参数寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_10_readable_regs[((4*32)+31):((4*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR10_PARAM5_LOW: begin
                //步进电机10参数寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_10_readable_regs[((5*32)+15):(5*32)];
            end
            REG_READ_ADDR_STEP_MOTOR10_PARAM5_HIGH: begin
                //步进电机10参数寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_10_readable_regs[((5*32)+31):((5*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR10_CONTROL1_LOW: begin
                //步进电机10控制寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_10_readable_regs[((6*32)+15):(6*32)];
            end
            REG_READ_ADDR_STEP_MOTOR10_CONTROL1_HIGH: begin
                //步进电机10控制寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_10_readable_regs[((6*32)+31):((6*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR10_STATE1_LOW: begin
                //步进电机10状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_10_readable_regs[((7*32)+15):(7*32)];
            end
            REG_READ_ADDR_STEP_MOTOR10_STATE1_HIGH: begin
                //步进电机10状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_10_readable_regs[((7*32)+31):((7*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR10_STATE2_LOW: begin
                //步进电机10状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_10_readable_regs[((8*32)+15):(8*32)];
            end
            REG_READ_ADDR_STEP_MOTOR10_STATE2_HIGH: begin
                //步进电机10状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_10_readable_regs[((8*32)+31):((8*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR10_STATE3_LOW: begin
                //步进电机10状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_10_readable_regs[((9*32)+15):(9*32)];
            end
            REG_READ_ADDR_STEP_MOTOR10_STATE3_HIGH: begin
                //步进电机10状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_10_readable_regs[((9*32)+31):((9*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR10_STATE4_LOW: begin
                //步进电机10状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_10_readable_regs[((10*32)+15):(10*32)];
            end
            REG_READ_ADDR_STEP_MOTOR10_STATE4_HIGH: begin
                //步进电机10状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_10_readable_regs[((10*32)+31):((10*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR10_STATE5_LOW: begin
                //步进电机10状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_10_readable_regs[((11*32)+15):(11*32)];
            end
            REG_READ_ADDR_STEP_MOTOR10_STATE5_HIGH: begin
                //步进电机10状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_10_readable_regs[((11*32)+31):((11*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR10_SPD_TAB_ELEM_LOW: begin
                //步进电机10状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_10_readable_regs[((12*32)+15):(12*32)];
            end
            REG_READ_ADDR_STEP_MOTOR10_SPD_TAB_ELEM_HIGH: begin
                //步进电机10状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_10_readable_regs[((12*32)+31):((12*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR10_STP_TAB_ELEM_LOW: begin
                //步进电机10状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_10_readable_regs[((13*32)+15):(13*32)];
            end
            REG_READ_ADDR_STEP_MOTOR10_STP_TAB_ELEM_HIGH: begin
                //步进电机10状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_10_readable_regs[((13*32)+31):((13*32)+16)];
            end
            /********************************步进电机11可读寄存器******************************/
            REG_READ_ADDR_STEP_MOTOR11_CONFIG_1_LOW: begin
                //步进电机11配置寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_11_readable_regs[((0*32)+15):(0*32)];
            end
            REG_READ_ADDR_STEP_MOTOR11_CONFIG_1_HIGH: begin
                //步进电机11配置寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_11_readable_regs[((0*32)+31):((0*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR11_PARAM1_LOW: begin
                //步进电机11参数寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_11_readable_regs[((1*32)+15):(1*32)];
            end
            REG_READ_ADDR_STEP_MOTOR11_PARAM1_HIGH: begin
                //步进电机11参数寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_11_readable_regs[((1*32)+31):((1*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR11_PARAM2_LOW: begin
                //步进电机11参数寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_11_readable_regs[((2*32)+15):(2*32)];
            end
            REG_READ_ADDR_STEP_MOTOR11_PARAM2_HIGH: begin
                //步进电机11参数寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_11_readable_regs[((2*32)+31):((2*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR11_PARAM3_LOW: begin
                //步进电机11参数寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_11_readable_regs[((3*32)+15):(3*32)];
            end
            REG_READ_ADDR_STEP_MOTOR11_PARAM3_HIGH: begin
                //步进电机11参数寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_11_readable_regs[((3*32)+31):((3*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR11_PARAM4_LOW: begin
                //步进电机11参数寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_11_readable_regs[((4*32)+15):(4*32)];
            end
            REG_READ_ADDR_STEP_MOTOR11_PARAM4_HIGH: begin
                //步进电机11参数寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_11_readable_regs[((4*32)+31):((4*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR11_PARAM5_LOW: begin
                //步进电机11参数寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_11_readable_regs[((5*32)+15):(5*32)];
            end
            REG_READ_ADDR_STEP_MOTOR11_PARAM5_HIGH: begin
                //步进电机11参数寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_11_readable_regs[((5*32)+31):((5*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR11_CONTROL1_LOW: begin
                //步进电机11控制寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_11_readable_regs[((6*32)+15):(6*32)];
            end
            REG_READ_ADDR_STEP_MOTOR11_CONTROL1_HIGH: begin
                //步进电机11控制寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_11_readable_regs[((6*32)+31):((6*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR11_STATE1_LOW: begin
                //步进电机11状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_11_readable_regs[((7*32)+15):(7*32)];
            end
            REG_READ_ADDR_STEP_MOTOR11_STATE1_HIGH: begin
                //步进电机11状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_11_readable_regs[((7*32)+31):((7*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR11_STATE2_LOW: begin
                //步进电机11状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_11_readable_regs[((8*32)+15):(8*32)];
            end
            REG_READ_ADDR_STEP_MOTOR11_STATE2_HIGH: begin
                //步进电机11状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_11_readable_regs[((8*32)+31):((8*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR11_STATE3_LOW: begin
                //步进电机11状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_11_readable_regs[((9*32)+15):(9*32)];
            end
            REG_READ_ADDR_STEP_MOTOR11_STATE3_HIGH: begin
                //步进电机11状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_11_readable_regs[((9*32)+31):((9*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR11_STATE4_LOW: begin
                //步进电机11状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_11_readable_regs[((10*32)+15):(10*32)];
            end
            REG_READ_ADDR_STEP_MOTOR11_STATE4_HIGH: begin
                //步进电机11状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_11_readable_regs[((10*32)+31):((10*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR11_STATE5_LOW: begin
                //步进电机11状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_11_readable_regs[((11*32)+15):(11*32)];
            end
            REG_READ_ADDR_STEP_MOTOR11_STATE5_HIGH: begin
                //步进电机11状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_11_readable_regs[((11*32)+31):((11*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR11_SPD_TAB_ELEM_LOW: begin
                //步进电机11状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_11_readable_regs[((12*32)+15):(12*32)];
            end
            REG_READ_ADDR_STEP_MOTOR11_SPD_TAB_ELEM_HIGH: begin
                //步进电机11状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_11_readable_regs[((12*32)+31):((12*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR11_STP_TAB_ELEM_LOW: begin
                //步进电机11状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_11_readable_regs[((13*32)+15):(13*32)];
            end
            REG_READ_ADDR_STEP_MOTOR11_STP_TAB_ELEM_HIGH: begin
                //步进电机11状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_11_readable_regs[((13*32)+31):((13*32)+16)];
            end
            /********************************步进电机12可读寄存器******************************/
            REG_READ_ADDR_STEP_MOTOR12_CONFIG_1_LOW: begin
                //步进电机12配置寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_12_readable_regs[((0*32)+15):(0*32)];
            end
            REG_READ_ADDR_STEP_MOTOR12_CONFIG_1_HIGH: begin
                //步进电机12配置寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_12_readable_regs[((0*32)+31):((0*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR12_PARAM1_LOW: begin
                //步进电机12参数寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_12_readable_regs[((1*32)+15):(1*32)];
            end
            REG_READ_ADDR_STEP_MOTOR12_PARAM1_HIGH: begin
                //步进电机12参数寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_12_readable_regs[((1*32)+31):((1*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR12_PARAM2_LOW: begin
                //步进电机12参数寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_12_readable_regs[((2*32)+15):(2*32)];
            end
            REG_READ_ADDR_STEP_MOTOR12_PARAM2_HIGH: begin
                //步进电机12参数寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_12_readable_regs[((2*32)+31):((2*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR12_PARAM3_LOW: begin
                //步进电机12参数寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_12_readable_regs[((3*32)+15):(3*32)];
            end
            REG_READ_ADDR_STEP_MOTOR12_PARAM3_HIGH: begin
                //步进电机12参数寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_12_readable_regs[((3*32)+31):((3*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR12_PARAM4_LOW: begin
                //步进电机12参数寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_12_readable_regs[((4*32)+15):(4*32)];
            end
            REG_READ_ADDR_STEP_MOTOR12_PARAM4_HIGH: begin
                //步进电机12参数寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_12_readable_regs[((4*32)+31):((4*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR12_PARAM5_LOW: begin
                //步进电机12参数寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_12_readable_regs[((5*32)+15):(5*32)];
            end
            REG_READ_ADDR_STEP_MOTOR12_PARAM5_HIGH: begin
                //步进电机12参数寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_12_readable_regs[((5*32)+31):((5*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR12_CONTROL1_LOW: begin
                //步进电机12控制寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_12_readable_regs[((6*32)+15):(6*32)];
            end
            REG_READ_ADDR_STEP_MOTOR12_CONTROL1_HIGH: begin
                //步进电机12控制寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_12_readable_regs[((6*32)+31):((6*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR12_STATE1_LOW: begin
                //步进电机12状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_12_readable_regs[((7*32)+15):(7*32)];
            end
            REG_READ_ADDR_STEP_MOTOR12_STATE1_HIGH: begin
                //步进电机12状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_12_readable_regs[((7*32)+31):((7*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR12_STATE2_LOW: begin
                //步进电机12状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_12_readable_regs[((8*32)+15):(8*32)];
            end
            REG_READ_ADDR_STEP_MOTOR12_STATE2_HIGH: begin
                //步进电机12状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_12_readable_regs[((8*32)+31):((8*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR12_STATE3_LOW: begin
                //步进电机12状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_12_readable_regs[((9*32)+15):(9*32)];
            end
            REG_READ_ADDR_STEP_MOTOR12_STATE3_HIGH: begin
                //步进电机12状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_12_readable_regs[((9*32)+31):((9*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR12_STATE4_LOW: begin
                //步进电机12状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_12_readable_regs[((10*32)+15):(10*32)];
            end
            REG_READ_ADDR_STEP_MOTOR12_STATE4_HIGH: begin
                //步进电机12状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_12_readable_regs[((10*32)+31):((10*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR12_STATE5_LOW: begin
                //步进电机12状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_12_readable_regs[((11*32)+15):(11*32)];
            end
            REG_READ_ADDR_STEP_MOTOR12_STATE5_HIGH: begin
                //步进电机12状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_12_readable_regs[((11*32)+31):((11*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR12_SPD_TAB_ELEM_LOW: begin
                //步进电机12状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_12_readable_regs[((12*32)+15):(12*32)];
            end
            REG_READ_ADDR_STEP_MOTOR12_SPD_TAB_ELEM_HIGH: begin
                //步进电机12状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_12_readable_regs[((12*32)+31):((12*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR12_STP_TAB_ELEM_LOW: begin
                //步进电机12状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_12_readable_regs[((13*32)+15):(13*32)];
            end
            REG_READ_ADDR_STEP_MOTOR12_STP_TAB_ELEM_HIGH: begin
                //步进电机12状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_12_readable_regs[((13*32)+31):((13*32)+16)];
            end
            /********************************步进电机13可读寄存器******************************/
            REG_READ_ADDR_STEP_MOTOR13_CONFIG_1_LOW: begin
                //步进电机13配置寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_13_readable_regs[((0*32)+15):(0*32)];
            end
            REG_READ_ADDR_STEP_MOTOR13_CONFIG_1_HIGH: begin
                //步进电机13配置寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_13_readable_regs[((0*32)+31):((0*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR13_PARAM1_LOW: begin
                //步进电机13参数寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_13_readable_regs[((1*32)+15):(1*32)];
            end
            REG_READ_ADDR_STEP_MOTOR13_PARAM1_HIGH: begin
                //步进电机13参数寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_13_readable_regs[((1*32)+31):((1*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR13_PARAM2_LOW: begin
                //步进电机13参数寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_13_readable_regs[((2*32)+15):(2*32)];
            end
            REG_READ_ADDR_STEP_MOTOR13_PARAM2_HIGH: begin
                //步进电机13参数寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_13_readable_regs[((2*32)+31):((2*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR13_PARAM3_LOW: begin
                //步进电机13参数寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_13_readable_regs[((3*32)+15):(3*32)];
            end
            REG_READ_ADDR_STEP_MOTOR13_PARAM3_HIGH: begin
                //步进电机13参数寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_13_readable_regs[((3*32)+31):((3*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR13_PARAM4_LOW: begin
                //步进电机13参数寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_13_readable_regs[((4*32)+15):(4*32)];
            end
            REG_READ_ADDR_STEP_MOTOR13_PARAM4_HIGH: begin
                //步进电机13参数寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_13_readable_regs[((4*32)+31):((4*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR13_PARAM5_LOW: begin
                //步进电机13参数寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_13_readable_regs[((5*32)+15):(5*32)];
            end
            REG_READ_ADDR_STEP_MOTOR13_PARAM5_HIGH: begin
                //步进电机13参数寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_13_readable_regs[((5*32)+31):((5*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR13_CONTROL1_LOW: begin
                //步进电机13控制寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_13_readable_regs[((6*32)+15):(6*32)];
            end
            REG_READ_ADDR_STEP_MOTOR13_CONTROL1_HIGH: begin
                //步进电机13控制寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_13_readable_regs[((6*32)+31):((6*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR13_STATE1_LOW: begin
                //步进电机13状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_13_readable_regs[((7*32)+15):(7*32)];
            end
            REG_READ_ADDR_STEP_MOTOR13_STATE1_HIGH: begin
                //步进电机13状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_13_readable_regs[((7*32)+31):((7*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR13_STATE2_LOW: begin
                //步进电机13状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_13_readable_regs[((8*32)+15):(8*32)];
            end
            REG_READ_ADDR_STEP_MOTOR13_STATE2_HIGH: begin
                //步进电机13状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_13_readable_regs[((8*32)+31):((8*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR13_STATE3_LOW: begin
                //步进电机13状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_13_readable_regs[((9*32)+15):(9*32)];
            end
            REG_READ_ADDR_STEP_MOTOR13_STATE3_HIGH: begin
                //步进电机13状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_13_readable_regs[((9*32)+31):((9*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR13_STATE4_LOW: begin
                //步进电机13状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_13_readable_regs[((10*32)+15):(10*32)];
            end
            REG_READ_ADDR_STEP_MOTOR13_STATE4_HIGH: begin
                //步进电机13状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_13_readable_regs[((10*32)+31):((10*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR13_STATE5_LOW: begin
                //步进电机13状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_13_readable_regs[((11*32)+15):(11*32)];
            end
            REG_READ_ADDR_STEP_MOTOR13_STATE5_HIGH: begin
                //步进电机13状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_13_readable_regs[((11*32)+31):((11*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR13_SPD_TAB_ELEM_LOW: begin
                //步进电机13状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_13_readable_regs[((12*32)+15):(12*32)];
            end
            REG_READ_ADDR_STEP_MOTOR13_SPD_TAB_ELEM_HIGH: begin
                //步进电机13状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_13_readable_regs[((12*32)+31):((12*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR13_STP_TAB_ELEM_LOW: begin
                //步进电机13状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_13_readable_regs[((13*32)+15):(13*32)];
            end
            REG_READ_ADDR_STEP_MOTOR13_STP_TAB_ELEM_HIGH: begin
                //步进电机13状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_13_readable_regs[((13*32)+31):((13*32)+16)];
            end
            /********************************步进电机14可读寄存器******************************/
            REG_READ_ADDR_STEP_MOTOR14_CONFIG_1_LOW: begin
                //步进电机14配置寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_14_readable_regs[((0*32)+15):(0*32)];
            end
            REG_READ_ADDR_STEP_MOTOR14_CONFIG_1_HIGH: begin
                //步进电机14配置寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_14_readable_regs[((0*32)+31):((0*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR14_PARAM1_LOW: begin
                //步进电机14参数寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_14_readable_regs[((1*32)+15):(1*32)];
            end
            REG_READ_ADDR_STEP_MOTOR14_PARAM1_HIGH: begin
                //步进电机14参数寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_14_readable_regs[((1*32)+31):((1*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR14_PARAM2_LOW: begin
                //步进电机14参数寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_14_readable_regs[((2*32)+15):(2*32)];
            end
            REG_READ_ADDR_STEP_MOTOR14_PARAM2_HIGH: begin
                //步进电机14参数寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_14_readable_regs[((2*32)+31):((2*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR14_PARAM3_LOW: begin
                //步进电机14参数寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_14_readable_regs[((3*32)+15):(3*32)];
            end
            REG_READ_ADDR_STEP_MOTOR14_PARAM3_HIGH: begin
                //步进电机14参数寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_14_readable_regs[((3*32)+31):((3*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR14_PARAM4_LOW: begin
                //步进电机14参数寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_14_readable_regs[((4*32)+15):(4*32)];
            end
            REG_READ_ADDR_STEP_MOTOR14_PARAM4_HIGH: begin
                //步进电机14参数寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_14_readable_regs[((4*32)+31):((4*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR14_PARAM5_LOW: begin
                //步进电机14参数寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_14_readable_regs[((5*32)+15):(5*32)];
            end
            REG_READ_ADDR_STEP_MOTOR14_PARAM5_HIGH: begin
                //步进电机14参数寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_14_readable_regs[((5*32)+31):((5*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR14_CONTROL1_LOW: begin
                //步进电机14控制寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_14_readable_regs[((6*32)+15):(6*32)];
            end
            REG_READ_ADDR_STEP_MOTOR14_CONTROL1_HIGH: begin
                //步进电机14控制寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_14_readable_regs[((6*32)+31):((6*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR14_STATE1_LOW: begin
                //步进电机14状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_14_readable_regs[((7*32)+15):(7*32)];
            end
            REG_READ_ADDR_STEP_MOTOR14_STATE1_HIGH: begin
                //步进电机14状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_14_readable_regs[((7*32)+31):((7*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR14_STATE2_LOW: begin
                //步进电机14状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_14_readable_regs[((8*32)+15):(8*32)];
            end
            REG_READ_ADDR_STEP_MOTOR14_STATE2_HIGH: begin
                //步进电机14状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_14_readable_regs[((8*32)+31):((8*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR14_STATE3_LOW: begin
                //步进电机14状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_14_readable_regs[((9*32)+15):(9*32)];
            end
            REG_READ_ADDR_STEP_MOTOR14_STATE3_HIGH: begin
                //步进电机14状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_14_readable_regs[((9*32)+31):((9*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR14_STATE4_LOW: begin
                //步进电机14状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_14_readable_regs[((10*32)+15):(10*32)];
            end
            REG_READ_ADDR_STEP_MOTOR14_STATE4_HIGH: begin
                //步进电机14状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_14_readable_regs[((10*32)+31):((10*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR14_STATE5_LOW: begin
                //步进电机14状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_14_readable_regs[((11*32)+15):(11*32)];
            end
            REG_READ_ADDR_STEP_MOTOR14_STATE5_HIGH: begin
                //步进电机14状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_14_readable_regs[((11*32)+31):((11*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR14_SPD_TAB_ELEM_LOW: begin
                //步进电机14状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_14_readable_regs[((12*32)+15):(12*32)];
            end
            REG_READ_ADDR_STEP_MOTOR14_SPD_TAB_ELEM_HIGH: begin
                //步进电机14状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_14_readable_regs[((12*32)+31):((12*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR14_STP_TAB_ELEM_LOW: begin
                //步进电机14状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_14_readable_regs[((13*32)+15):(13*32)];
            end
            REG_READ_ADDR_STEP_MOTOR14_STP_TAB_ELEM_HIGH: begin
                //步进电机14状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_14_readable_regs[((13*32)+31):((13*32)+16)];
            end
            /********************************步进电机15可读寄存器******************************/
            REG_READ_ADDR_STEP_MOTOR15_CONFIG_1_LOW: begin
                //步进电机15配置寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_15_readable_regs[((0*32)+15):(0*32)];
            end
            REG_READ_ADDR_STEP_MOTOR15_CONFIG_1_HIGH: begin
                //步进电机15配置寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_15_readable_regs[((0*32)+31):((0*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR15_PARAM1_LOW: begin
                //步进电机15参数寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_15_readable_regs[((1*32)+15):(1*32)];
            end
            REG_READ_ADDR_STEP_MOTOR15_PARAM1_HIGH: begin
                //步进电机15参数寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_15_readable_regs[((1*32)+31):((1*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR15_PARAM2_LOW: begin
                //步进电机15参数寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_15_readable_regs[((2*32)+15):(2*32)];
            end
            REG_READ_ADDR_STEP_MOTOR15_PARAM2_HIGH: begin
                //步进电机15参数寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_15_readable_regs[((2*32)+31):((2*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR15_PARAM3_LOW: begin
                //步进电机15参数寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_15_readable_regs[((3*32)+15):(3*32)];
            end
            REG_READ_ADDR_STEP_MOTOR15_PARAM3_HIGH: begin
                //步进电机15参数寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_15_readable_regs[((3*32)+31):((3*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR15_PARAM4_LOW: begin
                //步进电机15参数寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_15_readable_regs[((4*32)+15):(4*32)];
            end
            REG_READ_ADDR_STEP_MOTOR15_PARAM4_HIGH: begin
                //步进电机15参数寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_15_readable_regs[((4*32)+31):((4*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR15_PARAM5_LOW: begin
                //步进电机15参数寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_15_readable_regs[((5*32)+15):(5*32)];
            end
            REG_READ_ADDR_STEP_MOTOR15_PARAM5_HIGH: begin
                //步进电机15参数寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_15_readable_regs[((5*32)+31):((5*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR15_CONTROL1_LOW: begin
                //步进电机15控制寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_15_readable_regs[((6*32)+15):(6*32)];
            end
            REG_READ_ADDR_STEP_MOTOR15_CONTROL1_HIGH: begin
                //步进电机15控制寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_15_readable_regs[((6*32)+31):((6*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR15_STATE1_LOW: begin
                //步进电机15状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_15_readable_regs[((7*32)+15):(7*32)];
            end
            REG_READ_ADDR_STEP_MOTOR15_STATE1_HIGH: begin
                //步进电机15状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_15_readable_regs[((7*32)+31):((7*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR15_STATE2_LOW: begin
                //步进电机15状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_15_readable_regs[((8*32)+15):(8*32)];
            end
            REG_READ_ADDR_STEP_MOTOR15_STATE2_HIGH: begin
                //步进电机15状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_15_readable_regs[((8*32)+31):((8*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR15_STATE3_LOW: begin
                //步进电机15状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_15_readable_regs[((9*32)+15):(9*32)];
            end
            REG_READ_ADDR_STEP_MOTOR15_STATE3_HIGH: begin
                //步进电机15状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_15_readable_regs[((9*32)+31):((9*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR15_STATE4_LOW: begin
                //步进电机15状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_15_readable_regs[((10*32)+15):(10*32)];
            end
            REG_READ_ADDR_STEP_MOTOR15_STATE4_HIGH: begin
                //步进电机15状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_15_readable_regs[((10*32)+31):((10*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR15_STATE5_LOW: begin
                //步进电机15状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_15_readable_regs[((11*32)+15):(11*32)];
            end
            REG_READ_ADDR_STEP_MOTOR15_STATE5_HIGH: begin
                //步进电机15状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_15_readable_regs[((11*32)+31):((11*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR15_SPD_TAB_ELEM_LOW: begin
                //步进电机15状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_15_readable_regs[((12*32)+15):(12*32)];
            end
            REG_READ_ADDR_STEP_MOTOR15_SPD_TAB_ELEM_HIGH: begin
                //步进电机15状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_15_readable_regs[((12*32)+31):((12*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR15_STP_TAB_ELEM_LOW: begin
                //步进电机15状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_15_readable_regs[((13*32)+15):(13*32)];
            end
            REG_READ_ADDR_STEP_MOTOR15_STP_TAB_ELEM_HIGH: begin
                //步进电机15状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_15_readable_regs[((13*32)+31):((13*32)+16)];
            end
            /********************************步进电机16可读寄存器******************************/
            REG_READ_ADDR_STEP_MOTOR16_CONFIG_1_LOW: begin
                //步进电机16配置寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_16_readable_regs[((0*32)+15):(0*32)];
            end
            REG_READ_ADDR_STEP_MOTOR16_CONFIG_1_HIGH: begin
                //步进电机16配置寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_16_readable_regs[((0*32)+31):((0*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR16_PARAM1_LOW: begin
                //步进电机16参数寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_16_readable_regs[((1*32)+15):(1*32)];
            end
            REG_READ_ADDR_STEP_MOTOR16_PARAM1_HIGH: begin
                //步进电机16参数寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_16_readable_regs[((1*32)+31):((1*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR16_PARAM2_LOW: begin
                //步进电机16参数寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_16_readable_regs[((2*32)+15):(2*32)];
            end
            REG_READ_ADDR_STEP_MOTOR16_PARAM2_HIGH: begin
                //步进电机16参数寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_16_readable_regs[((2*32)+31):((2*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR16_PARAM3_LOW: begin
                //步进电机16参数寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_16_readable_regs[((3*32)+15):(3*32)];
            end
            REG_READ_ADDR_STEP_MOTOR16_PARAM3_HIGH: begin
                //步进电机16参数寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_16_readable_regs[((3*32)+31):((3*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR16_PARAM4_LOW: begin
                //步进电机16参数寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_16_readable_regs[((4*32)+15):(4*32)];
            end
            REG_READ_ADDR_STEP_MOTOR16_PARAM4_HIGH: begin
                //步进电机16参数寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_16_readable_regs[((4*32)+31):((4*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR16_PARAM5_LOW: begin
                //步进电机16参数寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_16_readable_regs[((5*32)+15):(5*32)];
            end
            REG_READ_ADDR_STEP_MOTOR16_PARAM5_HIGH: begin
                //步进电机16参数寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_16_readable_regs[((5*32)+31):((5*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR16_CONTROL1_LOW: begin
                //步进电机16控制寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_16_readable_regs[((6*32)+15):(6*32)];
            end
            REG_READ_ADDR_STEP_MOTOR16_CONTROL1_HIGH: begin
                //步进电机16控制寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_16_readable_regs[((6*32)+31):((6*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR16_STATE1_LOW: begin
                //步进电机16状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_16_readable_regs[((7*32)+15):(7*32)];
            end
            REG_READ_ADDR_STEP_MOTOR16_STATE1_HIGH: begin
                //步进电机16状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_16_readable_regs[((7*32)+31):((7*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR16_STATE2_LOW: begin
                //步进电机16状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_16_readable_regs[((8*32)+15):(8*32)];
            end
            REG_READ_ADDR_STEP_MOTOR16_STATE2_HIGH: begin
                //步进电机16状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_16_readable_regs[((8*32)+31):((8*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR16_STATE3_LOW: begin
                //步进电机16状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_16_readable_regs[((9*32)+15):(9*32)];
            end
            REG_READ_ADDR_STEP_MOTOR16_STATE3_HIGH: begin
                //步进电机16状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_16_readable_regs[((9*32)+31):((9*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR16_STATE4_LOW: begin
                //步进电机16状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_16_readable_regs[((10*32)+15):(10*32)];
            end
            REG_READ_ADDR_STEP_MOTOR16_STATE4_HIGH: begin
                //步进电机16状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_16_readable_regs[((10*32)+31):((10*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR16_STATE5_LOW: begin
                //步进电机16状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_16_readable_regs[((11*32)+15):(11*32)];
            end
            REG_READ_ADDR_STEP_MOTOR16_STATE5_HIGH: begin
                //步进电机16状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_16_readable_regs[((11*32)+31):((11*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR16_SPD_TAB_ELEM_LOW: begin
                //步进电机16状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_16_readable_regs[((12*32)+15):(12*32)];
            end
            REG_READ_ADDR_STEP_MOTOR16_SPD_TAB_ELEM_HIGH: begin
                //步进电机16状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_16_readable_regs[((12*32)+31):((12*32)+16)];
            end
            REG_READ_ADDR_STEP_MOTOR16_STP_TAB_ELEM_LOW: begin
                //步进电机16状态寄存器,低位
                fsmc_comm_data_out_reg <= step_motor_16_readable_regs[((13*32)+15):(13*32)];
            end
            REG_READ_ADDR_STEP_MOTOR16_STP_TAB_ELEM_HIGH: begin
                //步进电机16状态寄存器,高位
                fsmc_comm_data_out_reg <= step_motor_16_readable_regs[((13*32)+31):((13*32)+16)];
            end
            /***************************************不支持的指令****************************************/
            default: begin
                //不识别的指令
                fsmc_comm_data_out_reg <= fsmc_comm_data_out_reg;
            end
        endcase
    end
end


//写入信号锁存延迟一个节拍
always @( posedge sys_clk or negedge sys_rst_n)
begin 
    if(1'b0 == sys_rst_n) begin
        fsmc_comm_write_reg <= 2'd0;
    end
    else begin
        fsmc_comm_write_reg <= {fsmc_comm_write_reg[0],fsmc_comm_write_wire};
    end
end 

//写入地址信号锁存
always @(posedge sys_clk or negedge sys_rst_n)
begin
    if(1'b0 == sys_rst_n) begin
        fsmc_comm_addr_reg_write <= 16'hffff;
    end
    else if(1'b0 == fsmc_comm_ne)begin
        //检测到ne拉低,获取地址信号
        fsmc_comm_addr_reg_write <= fsmc_comm_addr;
    end
    else begin
        fsmc_comm_addr_reg_write <= 16'hffff;
    end
end

//写入逻辑
always @(posedge sys_clk or negedge sys_rst_n)
begin
    if(1'b0 == sys_rst_n) 
    begin
        //信号复位
        fsmc_comm_data_write_addr_low_reg       <= 16'd0;
        fsmc_comm_data_write_addr_high_reg      <= 16'd0;
        fsmc_comm_data_write_data_low_reg       <= 16'd0;
        fsmc_write_addr         <= 32'd0;
        fsmc_write_data         <= 32'd0;
        fsmc_write_singal       <= 1'b0;
        fsmc_write_singal_hold_clk_count <= 16'd0;
    end
    else if (fsmc_comm_write_reg == 2'b10)
    begin
        //识别到写入信号
        case(fsmc_comm_addr_reg_write)
            REG_WRITE_ADDR_REG_LOW : 
            begin
                //写入地址低字节
                fsmc_comm_data_write_addr_low_reg <= fsmc_comm_data;
            end
            REG_WRITE_ADDR_REG_HIGH: 
            begin
                //写入地址高字节
                fsmc_comm_data_write_addr_high_reg <= fsmc_comm_data;
            end
            REG_WRITE_ADDR_DATA_LOW: 
            begin
                //写入数据低字节
                fsmc_comm_data_write_data_low_reg <= fsmc_comm_data;
            end
            REG_WRITE_ADDR_DATA_HIGH: 
            begin
                //写入数据高字节
                fsmc_write_addr <= {fsmc_comm_data_write_addr_high_reg,fsmc_comm_data_write_addr_low_reg};
                fsmc_write_data <= {fsmc_comm_data,fsmc_comm_data_write_data_low_reg};
                //设置写入标记,执行写入
                fsmc_write_singal <= 1'b1;
                //保持时间清零
                fsmc_write_singal_hold_clk_count <= 16'd0;
            end
            default: 
            begin
                //原样保持不动
                fsmc_comm_data_write_addr_low_reg <= fsmc_comm_data_write_addr_low_reg;
                fsmc_comm_data_write_addr_high_reg <= fsmc_comm_data_write_addr_high_reg;
                fsmc_comm_data_write_data_low_reg <= fsmc_comm_data_write_data_low_reg;
                fsmc_write_singal <= fsmc_write_singal;
            end
        endcase
    end
    else begin
        if(1'b1 == fsmc_write_singal) 
        begin
            if(fsmc_write_singal_hold_clk_count > WRITE_SINGLE_HOLD_TIME_CYCLE) 
            begin
                fsmc_write_singal <= 1'b0;
                fsmc_write_singal_hold_clk_count <= 16'd0;
            end
            else 
            begin
                fsmc_write_singal_hold_clk_count <= fsmc_write_singal_hold_clk_count + 1'b1;
            end
        end
        else 
        begin
            //原样保持不动
            fsmc_comm_data_write_addr_low_reg <= fsmc_comm_data_write_addr_low_reg;
            fsmc_comm_data_write_addr_high_reg <= fsmc_comm_data_write_addr_high_reg;
            fsmc_comm_data_write_data_low_reg <= fsmc_comm_data_write_data_low_reg;
            fsmc_write_singal <= fsmc_write_singal;
            fsmc_write_singal_hold_clk_count <= fsmc_write_singal_hold_clk_count;
        end
    end
end

/*************************************************************************************/
/***********************************模块实例化*****************************************/
/*************************************************************************************/


endmodule









