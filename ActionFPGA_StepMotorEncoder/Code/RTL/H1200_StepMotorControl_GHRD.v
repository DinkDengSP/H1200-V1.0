/************************************************************************ 
 ** Author       : 邓小俊
 ** Date         : 2024-01-19 13:30:05
 ** Description  : When I Has Free Time ,I Will Write Description Below.
 ** description  : 
 ** LastEditors  : 邓小俊
 ** LastEditTime : 2024-01-25 13:04:46
 ** Email: xiaojun.deng@aliyun.com
 ** Copyright (c) 2024 by 公司名称, All Rights Reserved. 
 ************************************************************************/
module H1200_StepMotorControl_GHRD 
#(
    parameter SYS_MOTOR_VALID_COUNT = 16//支持的电机数量
)
(
    //通用时钟复位
    input           sys_clk_50m  ,  //输入时钟
    input           sys_clk_24m  ,  //输入时钟
    input           sys_rst_n,      //复位,低电平有效
    //LED灯光
    output          led_breath_1,
    //FSMC通讯接口
    input           fsmc_comm_nwe   ,//写入信号
    input           fsmc_comm_noe   ,//读取信号
    input           fsmc_comm_ne    ,//片选信号
    input           fsmc_comm_nadv  ,//数据地址选择信号
    output          fsmc_comm_nwait ,//设备忙等待信号
    input[15:0]     fsmc_comm_addr  ,//输入地址
    inout[15:0]     fsmc_comm_data,   //输入输出数据
    //步进电机端口
    output[SYS_MOTOR_VALID_COUNT:1]    motor_out_enable,
    output[SYS_MOTOR_VALID_COUNT:1]    motor_out_dir,
    output[SYS_MOTOR_VALID_COUNT:1]    motor_out_pluse,
    //步进电机编码器输入
    input[SYS_MOTOR_VALID_COUNT:1]     motor_encoder_a,
    input[SYS_MOTOR_VALID_COUNT:1]     motor_encoder_b,
    //步进电机输入复位IO
    input[SYS_MOTOR_VALID_COUNT:1]     motor_sensor_in_reset,
    //步进电机输入急停IO
    input[SYS_MOTOR_VALID_COUNT:1]     motor_sensor_in_emergency_stop,
    //步进电机输入辅助急停IO1
    input[SYS_MOTOR_VALID_COUNT:1]     motor_sensor_in_util_stop,
    //输入端口
    input[31:0]     port_in_group_a,//输入端口A
    input[31:0]     port_in_group_b,//输入端口B
    input[31:0]     port_in_group_c,//输入端口C
    input[31:0]     port_in_group_d,//输入端口D
    //输出端口
    output[31:0]    port_out_group_a,//输出端口A
    output[31:0]    port_out_group_b,//输出端口B
    output[31:0]    port_out_group_c,//输出端口C
    output[31:0]    port_out_group_d,//输出端口D
    //中断输出
    output[3:0]     motor_int_out
);

/*************************************************************************************/
/*********************************参数定义*********************************************/
/*************************************************************************************/
localparam SYS_XTAL_FREQ                =     32'd50_000_000;//外部晶振输入频率
localparam SYS_PLL_FSMC_COMM_FREQ       =     32'd70_000_000;//系统锁相环时钟定义
localparam SYS_PLL_FSMC_CMD_FREQ        =     32'd100_000_000;//系统锁相环时钟定义
localparam SYS_PLL_SDRAM_FREQ           =     32'd100_000_000;//系统锁相环时钟定义
localparam SYS_PLL_STEP_MOTOR_FREQ      =     32'd60_000_000;//系统锁相环时钟定义
localparam SYS_PLL_PORT_IN_FREQ         =     32'd50_000_000;//端口输入时钟定义
localparam SYS_PLL_PORT_OUT_FREQ        =     32'd50_000_000;//端口输出时钟定义
localparam SYS_PLL_OTHER_PERIPH_FREQ    =     32'd50_000_000;//系统锁相环时钟定义

/*************************************************************************************/    
/**********************************网线定义********************************************/
/*************************************************************************************/
//PLL输出的FSMC通讯时钟
wire clk_fsmc_interface; 
//PLL输出的FSMC指令处理时钟
wire clk_fsmc_command;
//PLL输出的FSMC指令处理时钟
wire clk_sdram;
//pll输出的端口输入时钟
wire clk_port_in;
//pll输出的端口输出时钟
wire clk_port_out;
//locked信号拉高,锁相环开始稳定输出时钟
wire locked_clk_pll_50m; 
//复位信号 
wire rst_clk_pll_50m_n; 

//PLL输出的步进电机时钟,A组
wire clk_step_motor_group_a;
//PLL输出的步进电机时钟,B组
wire clk_step_motor_group_b;
//PLL输出的步进电机时钟,C组
wire clk_step_motor_group_c;
//PLL输出的步进电机时钟,D组
wire clk_step_motor_group_d;
//PLL输出的其余外设时钟
wire clk_other_peripherals;
//locked信号拉高,锁相环开始稳定输出时钟
wire locked_clk_pll_24m; 
//复位信号 
wire rst_clk_pll_24m_n;  

//FSMC输出写信号
wire        fsmc_write_singal_wire;
wire[31:0]  fsmc_write_addr_wire;
wire[31:0]  fsmc_write_data_wire;
//FSMC输出的写信号
wire [15:0]    fsmc_cmd_write_addr_wire;
wire [31:0]    fsmc_cmd_write_data_wire;

//实际端口输入信号
wire [31:0]    port_in_group_a_wire;
wire [31:0]    port_in_group_b_wire;
wire [31:0]    port_in_group_c_wire;
wire [31:0]    port_in_group_d_wire;

//实际端口输出信号
wire [31:0]    port_out_group_a_wire;
wire [31:0]    port_out_group_b_wire;
wire [31:0]    port_out_group_c_wire;
wire [31:0]    port_out_group_d_wire;


//端口A输入信号连线
wire           port_in_group_a_write_single_wire;
wire [((2*32)-1):0]    port_in_group_a_readable_reg_extern_wire;

//端口B输入信号连线
wire           port_in_group_b_write_single_wire;
wire [((2*32)-1):0]    port_in_group_b_readable_reg_extern_wire;

//端口C输入信号连线
wire           port_in_group_c_write_single_wire;
wire [((2*32)-1):0]    port_in_group_c_readable_reg_extern_wire;

//端口D输入信号连线
wire           port_in_group_d_write_single_wire;
wire [((2*32)-1):0]    port_in_group_d_readable_reg_extern_wire;

//端口A输出信号连线
wire           port_out_group_a_write_single_wire;
wire [((3*32)-1):0]    port_out_group_a_readable_reg_extern_wire;

//端口B输出信号连线
wire           port_out_group_b_write_single_wire;
wire [((3*32)-1):0]    port_out_group_b_readable_reg_extern_wire;

//端口C输出信号连线
wire           port_out_group_c_write_single_wire;
wire [((3*32)-1):0]    port_out_group_c_readable_reg_extern_wire;

//端口D输出信号连线
wire           port_out_group_d_write_single_wire;
wire [((3*32)-1):0]    port_out_group_d_readable_reg_extern_wire;

//步进电机1信号连线
wire           step_motor_1_write_single_wire;
wire [((14*32)):0]    step_motor_1_readable_reg_extern_wire;

//步进电机2信号连线
wire           step_motor_2_write_single_wire;
wire [((14*32)):0]    step_motor_2_readable_reg_extern_wire;

//步进电机3信号连线
wire           step_motor_3_write_single_wire;
wire [((14*32)):0]    step_motor_3_readable_reg_extern_wire;

//步进电机4信号连线
wire           step_motor_4_write_single_wire;
wire [((14*32)):0]    step_motor_4_readable_reg_extern_wire;

//步进电机5信号连线
wire           step_motor_5_write_single_wire;
wire [((14*32)):0]    step_motor_5_readable_reg_extern_wire;

//步进电机6信号连线
wire           step_motor_6_write_single_wire;
wire [((14*32)):0]    step_motor_6_readable_reg_extern_wire;

//步进电机7信号连线
wire           step_motor_7_write_single_wire;
wire [((14*32)):0]    step_motor_7_readable_reg_extern_wire;

//步进电机8信号连线
wire           step_motor_8_write_single_wire;
wire [((14*32)):0]    step_motor_8_readable_reg_extern_wire;

//步进电机9信号连线
wire           step_motor_9_write_single_wire;
wire [((14*32)):0]    step_motor_9_readable_reg_extern_wire;

//步进电机10信号连线
wire           step_motor_10_write_single_wire;
wire [((14*32)):0]    step_motor_10_readable_reg_extern_wire;

//步进电机11信号连线
wire           step_motor_11_write_single_wire;
wire [((14*32)):0]    step_motor_11_readable_reg_extern_wire;

//步进电机12信号连线
wire           step_motor_12_write_single_wire;
wire [((14*32)):0]    step_motor_12_readable_reg_extern_wire;

//步进电机13信号连线
wire           step_motor_13_write_single_wire;
wire [((14*32)):0]    step_motor_13_readable_reg_extern_wire;

//步进电机14信号连线
wire           step_motor_14_write_single_wire;
wire [((14*32)):0]    step_motor_14_readable_reg_extern_wire;

//步进电机15信号连线
wire           step_motor_15_write_single_wire;
wire [((14*32)):0]    step_motor_15_readable_reg_extern_wire;

//步进电机16信号连线
wire           step_motor_16_write_single_wire;
wire [((14*32)):0]    step_motor_16_readable_reg_extern_wire;

/*************************************************************************************/
/*********************************寄存器定义*******************************************/
/*************************************************************************************/



/*************************************************************************************/
/**********************************组合逻辑代码****************************************/
/*************************************************************************************/
//系统复位与锁相环locked相与,作为其它模块的复位信号 
//任意一个低电平都是复位
assign  rst_clk_pll_50m_n = sys_rst_n & locked_clk_pll_50m;  
//任意一个低电平都是复位
assign  rst_clk_pll_24m_n = sys_rst_n & locked_clk_pll_24m; 
//输入组合逻辑
assign port_in_group_a_wire = {motor_sensor_in_emergency_stop[SYS_MOTOR_VALID_COUNT:1],
                                motor_sensor_in_reset[SYS_MOTOR_VALID_COUNT:1]};
assign port_in_group_b_wire = {port_in_group_b[(32-SYS_MOTOR_VALID_COUNT-1):0],
                                motor_sensor_in_util_stop[SYS_MOTOR_VALID_COUNT:1]};
assign port_in_group_c_wire = {motor_encoder_a[SYS_MOTOR_VALID_COUNT:1],
                                motor_encoder_b[SYS_MOTOR_VALID_COUNT:1]};
assign port_in_group_d_wire = port_in_group_d;
//输出组合逻辑
assign port_out_group_a = port_out_group_a_wire;
assign port_out_group_b = port_out_group_b_wire;
assign port_out_group_c = port_out_group_c_wire;
assign port_out_group_d = port_out_group_d_wire;

/*************************************************************************************/
/***********************************模块实例化*****************************************/
/*************************************************************************************/
//实例化PLL锁相环
clk_50m_pll_ip  clk_50m_pll_ip_inst( 
    .areset         (~sys_rst_n),//锁相环高电平复位,所以复位信号取反
    .inclk0         (sys_clk_50m),//输入时钟 
    .c0             (clk_fsmc_interface),//输出时钟70M
    .c1             (clk_fsmc_command),//输出时钟100M
    .c2             (clk_sdram),//输出时钟100,相位-90
    .c3             (clk_port_in),//端口输入时钟,50M
    .c4             (clk_port_out),//端口输出时钟,50M
    .locked         (locked_clk_pll_50m) //锁相环锁定,高电平有效
); 

//实例化PLL锁相环
clk_24m_pll_ip  clk_24m_pll_ip_inst( 
    .areset         (~sys_rst_n),//锁相环高电平复位,所以复位信号取反
    .inclk0         (sys_clk_24m),//输入时钟 
    .c0             (clk_step_motor_group_a),//步进电机输出时钟a,60M
    .c1             (clk_step_motor_group_b),//步进电机输出时钟b,60M
    .c2             (clk_step_motor_group_c),//步进电机输出时钟c,60M
    .c3             (clk_step_motor_group_d),//步进电机输出时钟d,60M
    .c4             (clk_other_peripherals),//输出时钟,其他外设
    .locked         (locked_clk_pll_24m) //锁相环锁定,高电平有效
); 

//实例化LED呼吸灯
led_breath 
#(
    .CLK_FREQ           (SYS_PLL_OTHER_PERIPH_FREQ),//系统时钟参数定义
    .LED_VALID_LEVEL    (1'b0)                      //LED有效电平
)led_breath_1_inst
(
    .sys_clk    (clk_other_peripherals),//系统时钟
    .sys_rst_n  (rst_clk_pll_24m_n),    //全局复位
    .led_out    (led_breath_1)          //输出信号，控制led灯
);


//实例化FSMC通讯模块
fsmc_comm_interface 
#(
    .SYSTEM_CLK_FREQ                    (SYS_PLL_FSMC_COMM_FREQ), //输入时钟    
    .WRITE_SINGLE_HOLD_TIME_CYCLE       (16'd10)//写入信号的保持时间,时钟周期
)
fsmc_comm_interface_inst
(
    .sys_clk            (clk_fsmc_interface),//输入时钟
    .sys_rst_n          (rst_clk_pll_50m_n),//复位,低电平有效 
    .fsmc_comm_nwe      (fsmc_comm_nwe ),//写入信号
    .fsmc_comm_noe      (fsmc_comm_noe ),//读取信号
    .fsmc_comm_ne       (fsmc_comm_ne  ),//片选信号
    .fsmc_comm_addr     (fsmc_comm_addr),//输入地址
    .fsmc_comm_data     (fsmc_comm_data),//输入输出数据
    //输出中断
    .motor_int_out      (motor_int_out),
    //IO口输出寄存器
    .port_out_group_a_readable_regs  (port_out_group_a_readable_reg_extern_wire),
    .port_out_group_b_readable_regs  (port_out_group_b_readable_reg_extern_wire),
    .port_out_group_c_readable_regs  (port_out_group_c_readable_reg_extern_wire),
    .port_out_group_d_readable_regs  (port_out_group_d_readable_reg_extern_wire),
    //IO口输入寄存器
    .port_in_group_a_readable_regs   (port_in_group_a_readable_reg_extern_wire),
    .port_in_group_b_readable_regs   (port_in_group_b_readable_reg_extern_wire),
    .port_in_group_c_readable_regs   (port_in_group_c_readable_reg_extern_wire),
    .port_in_group_d_readable_regs   (port_in_group_d_readable_reg_extern_wire),
    //输出写信号
    .fsmc_write_singal  (fsmc_write_singal_wire),
    .fsmc_write_addr    (fsmc_write_addr_wire),
    .fsmc_write_data    (fsmc_write_data_wire),
    //步进电机1导入寄存器
    .step_motor_1_readable_regs     (step_motor_1_readable_reg_extern_wire),
    //步进电机2导入寄存器   
    .step_motor_2_readable_regs     (step_motor_2_readable_reg_extern_wire),
    //步进电机3导入寄存器   
    .step_motor_3_readable_regs     (step_motor_3_readable_reg_extern_wire),
    //步进电机4导入寄存器   
    .step_motor_4_readable_regs     (step_motor_4_readable_reg_extern_wire),
    //步进电机5导入寄存器   
    .step_motor_5_readable_regs     (step_motor_5_readable_reg_extern_wire),
    //步进电机6导入寄存器   
    .step_motor_6_readable_regs     (step_motor_6_readable_reg_extern_wire),
    //步进电机7导入寄存器   
    .step_motor_7_readable_regs     (step_motor_7_readable_reg_extern_wire),
    //步进电机8导入寄存器   
    .step_motor_8_readable_regs     (step_motor_8_readable_reg_extern_wire),
    //步进电机9导入寄存器   
    .step_motor_9_readable_regs     (step_motor_9_readable_reg_extern_wire),
    //步进电机10导入寄存器
    .step_motor_10_readable_regs    (step_motor_10_readable_reg_extern_wire),
    //步进电机11导入寄存器  
    .step_motor_11_readable_regs    (step_motor_11_readable_reg_extern_wire),
    //步进电机12导入寄存器  
    .step_motor_12_readable_regs    (step_motor_12_readable_reg_extern_wire),
    //步进电机13导入寄存器  
    .step_motor_13_readable_regs    (step_motor_13_readable_reg_extern_wire),
    //步进电机14导入寄存器  
    .step_motor_14_readable_regs    (step_motor_14_readable_reg_extern_wire),
    //步进电机15导入寄存器  
    .step_motor_15_readable_regs    (step_motor_15_readable_reg_extern_wire),
    //步进电机16导入寄存器  
    .step_motor_16_readable_regs    (step_motor_16_readable_reg_extern_wire)
);


//FSMC写入指令解析模块
fsmc_write_cmd_process 
#(
    .SYSTEM_CLK_FREQ                    (SYS_PLL_FSMC_CMD_FREQ) //输入时钟    
)
fsmc_write_cmd_process_inst
(
    .sys_clk            (clk_fsmc_command),//输入时钟
    .sys_rst_n          (rst_clk_pll_50m_n),//复位,低电平有效 
    //输出写信号
    .fsmc_write_singal  (fsmc_write_singal_wire),
    .fsmc_write_addr    (fsmc_write_addr_wire),
    .fsmc_write_data    (fsmc_write_data_wire),
    //输出给外部模块的写地址和写数据
    .peripherals_write_addr (fsmc_cmd_write_addr_wire),
    .peripherals_write_data (fsmc_cmd_write_data_wire),
    //输出给输出模块的写信号
    .port_out_group_a_write_single  (port_out_group_a_write_single_wire),
    .port_out_group_b_write_single  (port_out_group_b_write_single_wire),
    .port_out_group_c_write_single  (port_out_group_c_write_single_wire),
    .port_out_group_d_write_single  (port_out_group_d_write_single_wire),
    //IO口输入寄存器
    .port_in_group_a_write_single  (port_in_group_a_write_single_wire),
    .port_in_group_b_write_single  (port_in_group_b_write_single_wire),
    .port_in_group_c_write_single  (port_in_group_c_write_single_wire),
    .port_in_group_d_write_single  (port_in_group_d_write_single_wire),
    //步进电机1
    .step_motor_1_write_single  (step_motor_1_write_single_wire),
    //步进电机2
    .step_motor_2_write_single  (step_motor_2_write_single_wire),
    //步进电机3
    .step_motor_3_write_single  (step_motor_3_write_single_wire),
    //步进电机4
    .step_motor_4_write_single  (step_motor_4_write_single_wire),
    //步进电机5
    .step_motor_5_write_single  (step_motor_5_write_single_wire),
    //步进电机6
    .step_motor_6_write_single  (step_motor_6_write_single_wire),
    //步进电机7
    .step_motor_7_write_single  (step_motor_7_write_single_wire),
    //步进电机8
    .step_motor_8_write_single  (step_motor_8_write_single_wire),
    //步进电机9
    .step_motor_9_write_single  (step_motor_9_write_single_wire),
    //步进电机10
    .step_motor_10_write_single  (step_motor_10_write_single_wire),
    //步进电机11
    .step_motor_11_write_single  (step_motor_11_write_single_wire),
    //步进电机12
    .step_motor_12_write_single  (step_motor_12_write_single_wire),
    //步进电机13
    .step_motor_13_write_single  (step_motor_13_write_single_wire),
    //步进电机14
    .step_motor_14_write_single  (step_motor_14_write_single_wire),
    //步进电机15
    .step_motor_15_write_single  (step_motor_15_write_single_wire),
    //步进电机16
    .step_motor_16_write_single  (step_motor_16_write_single_wire)
);

//实例化输入模块A
gpio_port_in 
#(
    .SYS_CLK_FREQ                       (SYS_PLL_PORT_IN_FREQ), //输入时钟    
    .GPIO_PORT_IN_MODE_REG_INIT_VAL     (32'hffff_ffff),//端口模式寄存器的初始值
    .GPIO_PORT_IN_SAMPLE_REG_INIT_VAL   (32'h0000_0000)//端口状态寄存器的初始值
)gpio_port_in_group_a_inst
(
    //通用时钟复位
    .sys_clk                (clk_port_in),//输入时钟
    .sys_rst_n              (rst_clk_pll_50m_n),//复位,低电平有效
    //输入的写信号      
    .write_single           (port_in_group_a_write_single_wire),
    .write_addr             (fsmc_cmd_write_addr_wire[3:0]),
    .write_data             (fsmc_cmd_write_data_wire),
    //输出的寄存器,寄存器有两个,一个是写入的输入模式寄存器
    //另一个是输出的当前状态寄存器
    .readable_reg_extern    (port_in_group_a_readable_reg_extern_wire), 
    //输入端口
    .port_really_in         (port_in_group_a_wire)
);

//实例化输入模块B
gpio_port_in 
#(
    .SYS_CLK_FREQ                       (SYS_PLL_PORT_IN_FREQ), //输入时钟    
    .GPIO_PORT_IN_MODE_REG_INIT_VAL     (32'hffff_ffff),//端口模式寄存器的初始值
    .GPIO_PORT_IN_SAMPLE_REG_INIT_VAL   (32'h0000_0000)//端口状态寄存器的初始值
)gpio_port_in_group_b_inst
(
    //通用时钟复位
    .sys_clk                (clk_port_in),//输入时钟
    .sys_rst_n              (rst_clk_pll_50m_n),//复位,低电平有效
    //输入的写信号      
    .write_single           (port_in_group_b_write_single_wire),
    .write_addr             (fsmc_cmd_write_addr_wire[3:0]),
    .write_data             (fsmc_cmd_write_data_wire),
    //输出的寄存器,寄存器有两个,一个是写入的输入模式寄存器
    //另一个是输出的当前状态寄存器
    .readable_reg_extern    (port_in_group_b_readable_reg_extern_wire), 
    //输入端口
    .port_really_in         (port_in_group_b_wire)
);

//实例化输入模块C
gpio_port_in 
#(
    .SYS_CLK_FREQ                       (SYS_PLL_PORT_IN_FREQ), //输入时钟    
    .GPIO_PORT_IN_MODE_REG_INIT_VAL     (32'hffff_ffff),//端口模式寄存器的初始值
    .GPIO_PORT_IN_SAMPLE_REG_INIT_VAL   (32'h0000_0000)//端口状态寄存器的初始值
)gpio_port_in_group_c_inst
(
    //通用时钟复位
    .sys_clk                (clk_port_in),//输入时钟
    .sys_rst_n              (rst_clk_pll_50m_n),//复位,低电平有效
    //输入的写信号      
    .write_single           (port_in_group_c_write_single_wire),
    .write_addr             (fsmc_cmd_write_addr_wire[3:0]),
    .write_data             (fsmc_cmd_write_data_wire),
    //输出的寄存器,寄存器有两个,一个是写入的输入模式寄存器
    //另一个是输出的当前状态寄存器
    .readable_reg_extern    (port_in_group_c_readable_reg_extern_wire), 
    //输入端口
    .port_really_in         (port_in_group_c_wire)
);

//实例化输入模块D
gpio_port_in 
#(
    .SYS_CLK_FREQ                       (SYS_PLL_PORT_IN_FREQ), //输入时钟    
    .GPIO_PORT_IN_MODE_REG_INIT_VAL     (32'hffff_ffff),//端口模式寄存器的初始值
    .GPIO_PORT_IN_SAMPLE_REG_INIT_VAL   (32'h0000_0000)//端口状态寄存器的初始值
)gpio_port_in_group_d_inst
(
    //通用时钟复位
    .sys_clk                (clk_port_in),//输入时钟
    .sys_rst_n              (rst_clk_pll_50m_n),//复位,低电平有效
    //输入的写信号      
    .write_single           (port_in_group_d_write_single_wire),
    .write_addr             (fsmc_cmd_write_addr_wire[3:0]),
    .write_data             (fsmc_cmd_write_data_wire),
    //输出的寄存器,寄存器有两个,一个是写入的输入模式寄存器
    //另一个是输出的当前状态寄存器
    .readable_reg_extern    (port_in_group_d_readable_reg_extern_wire), 
    //输入端口
    .port_really_in         (port_in_group_d_wire)
);


//实例化输出模块A
gpio_port_out 
#(
    .SYS_CLK_FREQ                        (SYS_PLL_PORT_OUT_FREQ), //输入时钟    
    .GPIO_PORT_OUT_MODE_REG_INIT_VAL     (32'hffff_ffff),//端口模式寄存器的初始值
    .GPIO_PORT_OUT_STATE_REG_INIT_VAL    (32'h0000_0000),//端口状态寄存器的初始值
    .GPIO_PORT_OUT_REALLY_REG_INIT_VAL   (32'h0000_0000)//端口实际输出寄存器
)gpio_port_out_group_a_inst
(
    //通用时钟复位
    .sys_clk                (clk_port_out),//输入时钟
    .sys_rst_n              (rst_clk_pll_50m_n),//复位,低电平有效
    //输入的写信号      
    .write_single           (port_out_group_a_write_single_wire),
    .write_addr             (fsmc_cmd_write_addr_wire[3:0]),
    .write_data             (fsmc_cmd_write_data_wire),
    //输出的寄存器,寄存器有两个,一个是写入的输入模式寄存器
    //另一个是输出的当前状态寄存器
    .readable_reg_extern    (port_out_group_a_readable_reg_extern_wire), 
    //输入端口
    .port_really_out         (port_out_group_a_wire)
);

//实例化输出模块B
gpio_port_out 
#(
    .SYS_CLK_FREQ                        (SYS_PLL_PORT_OUT_FREQ), //输入时钟    
    .GPIO_PORT_OUT_MODE_REG_INIT_VAL     (32'hffff_ffff),//端口模式寄存器的初始值
    .GPIO_PORT_OUT_STATE_REG_INIT_VAL    (32'h0000_0000),//端口状态寄存器的初始值
    .GPIO_PORT_OUT_REALLY_REG_INIT_VAL   (32'h0000_0000)//端口实际输出寄存器
)gpio_port_out_group_b_inst
(
    //通用时钟复位
    .sys_clk                (clk_port_out),//输入时钟
    .sys_rst_n              (rst_clk_pll_50m_n),//复位,低电平有效
    //输入的写信号      
    .write_single           (port_out_group_b_write_single_wire),
    .write_addr             (fsmc_cmd_write_addr_wire[3:0]),
    .write_data             (fsmc_cmd_write_data_wire),
    //输出的寄存器,寄存器有两个,一个是写入的输入模式寄存器
    //另一个是输出的当前状态寄存器
    .readable_reg_extern    (port_out_group_b_readable_reg_extern_wire), 
    //输入端口
    .port_really_out         (port_out_group_b_wire)
);

//实例化输出模块C
gpio_port_out 
#(
    .SYS_CLK_FREQ                        (SYS_PLL_PORT_OUT_FREQ), //输入时钟    
    .GPIO_PORT_OUT_MODE_REG_INIT_VAL     (32'hffff_ffff),//端口模式寄存器的初始值
    .GPIO_PORT_OUT_STATE_REG_INIT_VAL    (32'h0000_0000),//端口状态寄存器的初始值
    .GPIO_PORT_OUT_REALLY_REG_INIT_VAL   (32'h0000_0000)//端口实际输出寄存器
)gpio_port_out_group_c_inst
(
    //通用时钟复位
    .sys_clk                (clk_port_out),//输入时钟
    .sys_rst_n              (rst_clk_pll_50m_n),//复位,低电平有效
    //输入的写信号      
    .write_single           (port_out_group_c_write_single_wire),
    .write_addr             (fsmc_cmd_write_addr_wire[3:0]),
    .write_data             (fsmc_cmd_write_data_wire),
    //输出的寄存器,寄存器有两个,一个是写入的输入模式寄存器
    //另一个是输出的当前状态寄存器
    .readable_reg_extern    (port_out_group_c_readable_reg_extern_wire), 
    //输入端口
    .port_really_out         (port_out_group_c_wire)
);

//实例化输出模块D
gpio_port_out 
#(
    .SYS_CLK_FREQ                        (SYS_PLL_PORT_OUT_FREQ), //输入时钟    
    .GPIO_PORT_OUT_MODE_REG_INIT_VAL     (32'hffff_ffff),//端口模式寄存器的初始值
    .GPIO_PORT_OUT_STATE_REG_INIT_VAL    (32'h0000_0000),//端口状态寄存器的初始值
    .GPIO_PORT_OUT_REALLY_REG_INIT_VAL   (32'h0000_0000)//端口实际输出寄存器
)gpio_port_out_group_d_inst
(
    //通用时钟复位
    .sys_clk                (clk_port_out),//输入时钟
    .sys_rst_n              (rst_clk_pll_50m_n),//复位,低电平有效
    //输入的写信号      
    .write_single           (port_out_group_d_write_single_wire),
    .write_addr             (fsmc_cmd_write_addr_wire[3:0]),
    .write_data             (fsmc_cmd_write_data_wire),
    //输出的寄存器,寄存器有两个,一个是写入的输入模式寄存器
    //另一个是输出的当前状态寄存器
    .readable_reg_extern    (port_out_group_d_readable_reg_extern_wire), 
    //输入端口
    .port_really_out         (port_out_group_d_wire)
);

//实例化步进电机模块1
step_motor_control
#(
    .SYS_CLK_FREQ           (SYS_PLL_STEP_MOTOR_FREQ) //输入时钟    
)step_motor_control_1_inst
(
    //通用时钟复位
    .sys_clk                (clk_step_motor_group_a),//输入时钟
    .sys_rst_n              (rst_clk_pll_24m_n),//复位,低电平有效
    //输入的写信号      
    .write_single           (step_motor_1_write_single_wire),
    .write_addr             (fsmc_cmd_write_addr_wire),
    .write_data             (fsmc_cmd_write_data_wire),
    //输出寄存器
    .readable_reg_extern    (step_motor_1_readable_reg_extern_wire),
    //电机输出端口
    .motor_out_enable       (motor_out_enable[1]),
    .motor_out_dir          (motor_out_dir[1]),
    .motor_out_pluse        (motor_out_pluse[1]),
    //电机编码器端口
    .motor_encoder_a        (motor_encoder_a[1]),
    .motor_encoder_b        (motor_encoder_b[1]),
    //电机传感器端口
    .motor_sensor_in_reset              (motor_sensor_in_reset[1]),
    .motor_sensor_in_emergency_stop     (motor_sensor_in_emergency_stop[1]),
    .motor_sensor_in_util_stop          (motor_sensor_in_util_stop[1])
);

//实例化步进电机模块2
step_motor_control
#(
    .SYS_CLK_FREQ           (SYS_PLL_STEP_MOTOR_FREQ) //输入时钟    
)step_motor_control_2_inst
(
    //通用时钟复位
    .sys_clk                (clk_step_motor_group_a),//输入时钟
    .sys_rst_n              (rst_clk_pll_24m_n),//复位,低电平有效
    //输入的写信号      
    .write_single           (step_motor_2_write_single_wire),
    .write_addr             (fsmc_cmd_write_addr_wire),
    .write_data             (fsmc_cmd_write_data_wire),
    //输出寄存器
    .readable_reg_extern    (step_motor_2_readable_reg_extern_wire),
    //电机输出端口
    .motor_out_enable       (motor_out_enable[2]),
    .motor_out_dir          (motor_out_dir[2]),
    .motor_out_pluse        (motor_out_pluse[2]),
    //电机编码器端口
    .motor_encoder_a        (motor_encoder_a[2]),
    .motor_encoder_b        (motor_encoder_b[2]),
    //电机传感器端口
    .motor_sensor_in_reset              (motor_sensor_in_reset[2]),
    .motor_sensor_in_emergency_stop     (motor_sensor_in_emergency_stop[2]),
    .motor_sensor_in_util_stop          (motor_sensor_in_util_stop[2])
);

//实例化步进电机模块3
step_motor_control
#(
    .SYS_CLK_FREQ           (SYS_PLL_STEP_MOTOR_FREQ) //输入时钟    
)step_motor_control_3_inst
(
    //通用时钟复位
    .sys_clk                (clk_step_motor_group_a),//输入时钟
    .sys_rst_n              (rst_clk_pll_24m_n),//复位,低电平有效
    //输入的写信号      
    .write_single           (step_motor_3_write_single_wire),
    .write_addr             (fsmc_cmd_write_addr_wire),
    .write_data             (fsmc_cmd_write_data_wire),
    //输出寄存器
    .readable_reg_extern    (step_motor_3_readable_reg_extern_wire),
    //电机输出端口
    .motor_out_enable       (motor_out_enable[3]),
    .motor_out_dir          (motor_out_dir[3]),
    .motor_out_pluse        (motor_out_pluse[3]),
    //电机编码器端口
    .motor_encoder_a        (motor_encoder_a[3]),
    .motor_encoder_b        (motor_encoder_b[3]),
    //电机传感器端口
    .motor_sensor_in_reset              (motor_sensor_in_reset[3]),
    .motor_sensor_in_emergency_stop     (motor_sensor_in_emergency_stop[3]),
    .motor_sensor_in_util_stop          (motor_sensor_in_util_stop[3])
);

//实例化步进电机模块4
step_motor_control
#(
    .SYS_CLK_FREQ           (SYS_PLL_STEP_MOTOR_FREQ) //输入时钟    
)step_motor_control_4_inst
(
    //通用时钟复位
    .sys_clk                (clk_step_motor_group_a),//输入时钟
    .sys_rst_n              (rst_clk_pll_24m_n),//复位,低电平有效
    //输入的写信号      
    .write_single           (step_motor_4_write_single_wire),
    .write_addr             (fsmc_cmd_write_addr_wire),
    .write_data             (fsmc_cmd_write_data_wire),
    //输出寄存器
    .readable_reg_extern    (step_motor_4_readable_reg_extern_wire),
    //电机输出端口
    .motor_out_enable       (motor_out_enable[4]),
    .motor_out_dir          (motor_out_dir[4]),
    .motor_out_pluse        (motor_out_pluse[4]),
    //电机编码器端口
    .motor_encoder_a        (motor_encoder_a[4]),
    .motor_encoder_b        (motor_encoder_b[4]),
    //电机传感器端口
    .motor_sensor_in_reset              (motor_sensor_in_reset[4]),
    .motor_sensor_in_emergency_stop     (motor_sensor_in_emergency_stop[4]),
    .motor_sensor_in_util_stop          (motor_sensor_in_util_stop[4])
);

//实例化步进电机模块5
step_motor_control
#(
    .SYS_CLK_FREQ           (SYS_PLL_STEP_MOTOR_FREQ) //输入时钟    
)step_motor_control_5_inst
(
    //通用时钟复位
    .sys_clk                (clk_step_motor_group_b),//输入时钟
    .sys_rst_n              (rst_clk_pll_24m_n),//复位,低电平有效
    //输入的写信号      
    .write_single           (step_motor_5_write_single_wire),
    .write_addr             (fsmc_cmd_write_addr_wire),
    .write_data             (fsmc_cmd_write_data_wire),
    //输出寄存器
    .readable_reg_extern    (step_motor_5_readable_reg_extern_wire),
    //电机输出端口
    .motor_out_enable       (motor_out_enable[5]),
    .motor_out_dir          (motor_out_dir[5]),
    .motor_out_pluse        (motor_out_pluse[5]),
    //电机编码器端口
    .motor_encoder_a        (motor_encoder_a[5]),
    .motor_encoder_b        (motor_encoder_b[5]),
    //电机传感器端口
    .motor_sensor_in_reset              (motor_sensor_in_reset[5]),
    .motor_sensor_in_emergency_stop     (motor_sensor_in_emergency_stop[5]),
    .motor_sensor_in_util_stop          (motor_sensor_in_util_stop[5])
);

//实例化步进电机模块6
step_motor_control
#(
    .SYS_CLK_FREQ           (SYS_PLL_STEP_MOTOR_FREQ) //输入时钟    
)step_motor_control_6_inst
(
    //通用时钟复位
    .sys_clk                (clk_step_motor_group_b),//输入时钟
    .sys_rst_n              (rst_clk_pll_24m_n),//复位,低电平有效
    //输入的写信号      
    .write_single           (step_motor_6_write_single_wire),
    .write_addr             (fsmc_cmd_write_addr_wire),
    .write_data             (fsmc_cmd_write_data_wire),
    //输出寄存器
    .readable_reg_extern    (step_motor_6_readable_reg_extern_wire),
    //电机输出端口
    .motor_out_enable       (motor_out_enable[6]),
    .motor_out_dir          (motor_out_dir[6]),
    .motor_out_pluse        (motor_out_pluse[6]),
    //电机编码器端口
    .motor_encoder_a        (motor_encoder_a[6]),
    .motor_encoder_b        (motor_encoder_b[6]),
    //电机传感器端口
    .motor_sensor_in_reset              (motor_sensor_in_reset[6]),
    .motor_sensor_in_emergency_stop     (motor_sensor_in_emergency_stop[6]),
    .motor_sensor_in_util_stop          (motor_sensor_in_util_stop[6])
);

//实例化步进电机模块7
step_motor_control
#(
    .SYS_CLK_FREQ           (SYS_PLL_STEP_MOTOR_FREQ) //输入时钟    
)step_motor_control_7_inst
(
    //通用时钟复位
    .sys_clk                (clk_step_motor_group_b),//输入时钟
    .sys_rst_n              (rst_clk_pll_24m_n),//复位,低电平有效
    //输入的写信号      
    .write_single           (step_motor_7_write_single_wire),
    .write_addr             (fsmc_cmd_write_addr_wire),
    .write_data             (fsmc_cmd_write_data_wire),
    //输出寄存器
    .readable_reg_extern    (step_motor_7_readable_reg_extern_wire),
    //电机输出端口
    .motor_out_enable       (motor_out_enable[7]),
    .motor_out_dir          (motor_out_dir[7]),
    .motor_out_pluse        (motor_out_pluse[7]),
    //电机编码器端口
    .motor_encoder_a        (motor_encoder_a[7]),
    .motor_encoder_b        (motor_encoder_b[7]),
    //电机传感器端口
    .motor_sensor_in_reset              (motor_sensor_in_reset[7]),
    .motor_sensor_in_emergency_stop     (motor_sensor_in_emergency_stop[7]),
    .motor_sensor_in_util_stop          (motor_sensor_in_util_stop[7])
);

//实例化步进电机模块8
step_motor_control
#(
    .SYS_CLK_FREQ           (SYS_PLL_STEP_MOTOR_FREQ) //输入时钟    
)step_motor_control_8_inst
(
    //通用时钟复位
    .sys_clk                (clk_step_motor_group_b),//输入时钟
    .sys_rst_n              (rst_clk_pll_24m_n),//复位,低电平有效
    //输入的写信号      
    .write_single           (step_motor_8_write_single_wire),
    .write_addr             (fsmc_cmd_write_addr_wire),
    .write_data             (fsmc_cmd_write_data_wire),
    //输出寄存器
    .readable_reg_extern    (step_motor_8_readable_reg_extern_wire),
    //电机输出端口
    .motor_out_enable       (motor_out_enable[8]),
    .motor_out_dir          (motor_out_dir[8]),
    .motor_out_pluse        (motor_out_pluse[8]),
    //电机编码器端口
    .motor_encoder_a        (motor_encoder_a[8]),
    .motor_encoder_b        (motor_encoder_b[8]),
    //电机传感器端口
    .motor_sensor_in_reset              (motor_sensor_in_reset[8]),
    .motor_sensor_in_emergency_stop     (motor_sensor_in_emergency_stop[8]),
    .motor_sensor_in_util_stop          (motor_sensor_in_util_stop[8])
);

//实例化步进电机模块9
step_motor_control
#(
    .SYS_CLK_FREQ           (SYS_PLL_STEP_MOTOR_FREQ) //输入时钟    
)step_motor_control_9_inst
(
    //通用时钟复位
    .sys_clk                (clk_step_motor_group_c),//输入时钟
    .sys_rst_n              (rst_clk_pll_24m_n),//复位,低电平有效
    //输入的写信号      
    .write_single           (step_motor_9_write_single_wire),
    .write_addr             (fsmc_cmd_write_addr_wire),
    .write_data             (fsmc_cmd_write_data_wire),
    //输出寄存器
    .readable_reg_extern    (step_motor_9_readable_reg_extern_wire),
    //电机输出端口
    .motor_out_enable       (motor_out_enable[9]),
    .motor_out_dir          (motor_out_dir[9]),
    .motor_out_pluse        (motor_out_pluse[9]),
    //电机编码器端口
    .motor_encoder_a        (motor_encoder_a[9]),
    .motor_encoder_b        (motor_encoder_b[9]),
    //电机传感器端口
    .motor_sensor_in_reset              (motor_sensor_in_reset[9]),
    .motor_sensor_in_emergency_stop     (motor_sensor_in_emergency_stop[9]),
    .motor_sensor_in_util_stop          (motor_sensor_in_util_stop[9])
);

//实例化步进电机模块10
step_motor_control
#(
    .SYS_CLK_FREQ           (SYS_PLL_STEP_MOTOR_FREQ) //输入时钟    
)step_motor_control_10_inst
(
    //通用时钟复位
    .sys_clk                (clk_step_motor_group_c),//输入时钟
    .sys_rst_n              (rst_clk_pll_24m_n),//复位,低电平有效
    //输入的写信号      
    .write_single           (step_motor_10_write_single_wire),
    .write_addr             (fsmc_cmd_write_addr_wire),
    .write_data             (fsmc_cmd_write_data_wire),
    //输出寄存器
    .readable_reg_extern    (step_motor_10_readable_reg_extern_wire),
    //电机输出端口
    .motor_out_enable       (motor_out_enable[10]),
    .motor_out_dir          (motor_out_dir[10]),
    .motor_out_pluse        (motor_out_pluse[10]),
    //电机编码器端口
    .motor_encoder_a        (motor_encoder_a[10]),
    .motor_encoder_b        (motor_encoder_b[10]),
    //电机传感器端口
    .motor_sensor_in_reset              (motor_sensor_in_reset[10]),
    .motor_sensor_in_emergency_stop     (motor_sensor_in_emergency_stop[10]),
    .motor_sensor_in_util_stop          (motor_sensor_in_util_stop[10])
);

//实例化步进电机模块11
step_motor_control
#(
    .SYS_CLK_FREQ           (SYS_PLL_STEP_MOTOR_FREQ) //输入时钟    
)step_motor_control_11_inst
(
    //通用时钟复位
    .sys_clk                (clk_step_motor_group_c),//输入时钟
    .sys_rst_n              (rst_clk_pll_24m_n),//复位,低电平有效
    //输入的写信号      
    .write_single           (step_motor_11_write_single_wire),
    .write_addr             (fsmc_cmd_write_addr_wire),
    .write_data             (fsmc_cmd_write_data_wire),
    //输出寄存器
    .readable_reg_extern    (step_motor_11_readable_reg_extern_wire),
    //电机输出端口
    .motor_out_enable       (motor_out_enable[11]),
    .motor_out_dir          (motor_out_dir[11]),
    .motor_out_pluse        (motor_out_pluse[11]),
    //电机编码器端口
    .motor_encoder_a        (motor_encoder_a[11]),
    .motor_encoder_b        (motor_encoder_b[11]),
    //电机传感器端口
    .motor_sensor_in_reset              (motor_sensor_in_reset[11]),
    .motor_sensor_in_emergency_stop     (motor_sensor_in_emergency_stop[11]),
    .motor_sensor_in_util_stop          (motor_sensor_in_util_stop[11])
);

//实例化步进电机模块12
step_motor_control
#(
    .SYS_CLK_FREQ           (SYS_PLL_STEP_MOTOR_FREQ) //输入时钟    
)step_motor_control_12_inst
(
    //通用时钟复位
    .sys_clk                (clk_step_motor_group_c),//输入时钟
    .sys_rst_n              (rst_clk_pll_24m_n),//复位,低电平有效
    //输入的写信号      
    .write_single           (step_motor_12_write_single_wire),
    .write_addr             (fsmc_cmd_write_addr_wire),
    .write_data             (fsmc_cmd_write_data_wire),
    //输出寄存器
    .readable_reg_extern    (step_motor_12_readable_reg_extern_wire),
    //电机输出端口
    .motor_out_enable       (motor_out_enable[12]),
    .motor_out_dir          (motor_out_dir[12]),
    .motor_out_pluse        (motor_out_pluse[12]),
    //电机编码器端口
    .motor_encoder_a        (motor_encoder_a[12]),
    .motor_encoder_b        (motor_encoder_b[12]),
    //电机传感器端口
    .motor_sensor_in_reset              (motor_sensor_in_reset[12]),
    .motor_sensor_in_emergency_stop     (motor_sensor_in_emergency_stop[12]),
    .motor_sensor_in_util_stop          (motor_sensor_in_util_stop[12])
);

//实例化步进电机模块13
step_motor_control
#(
    .SYS_CLK_FREQ           (SYS_PLL_STEP_MOTOR_FREQ) //输入时钟    
)step_motor_control_13_inst
(
    //通用时钟复位
    .sys_clk                (clk_step_motor_group_d),//输入时钟
    .sys_rst_n              (rst_clk_pll_24m_n),//复位,低电平有效
    //输入的写信号      
    .write_single           (step_motor_13_write_single_wire),
    .write_addr             (fsmc_cmd_write_addr_wire),
    .write_data             (fsmc_cmd_write_data_wire),
    //输出寄存器
    .readable_reg_extern    (step_motor_13_readable_reg_extern_wire),
    //电机输出端口
    .motor_out_enable       (motor_out_enable[13]),
    .motor_out_dir          (motor_out_dir[13]),
    .motor_out_pluse        (motor_out_pluse[13]),
    //电机编码器端口
    .motor_encoder_a        (motor_encoder_a[13]),
    .motor_encoder_b        (motor_encoder_b[13]),
    //电机传感器端口
    .motor_sensor_in_reset              (motor_sensor_in_reset[13]),
    .motor_sensor_in_emergency_stop     (motor_sensor_in_emergency_stop[13]),
    .motor_sensor_in_util_stop          (motor_sensor_in_util_stop[13])
);

//实例化步进电机模块14
step_motor_control
#(
    .SYS_CLK_FREQ           (SYS_PLL_STEP_MOTOR_FREQ) //输入时钟    
)step_motor_control_14_inst
(
    //通用时钟复位
    .sys_clk                (clk_step_motor_group_d),//输入时钟
    .sys_rst_n              (rst_clk_pll_24m_n),//复位,低电平有效
    //输入的写信号      
    .write_single           (step_motor_14_write_single_wire),
    .write_addr             (fsmc_cmd_write_addr_wire),
    .write_data             (fsmc_cmd_write_data_wire),
    //输出寄存器
    .readable_reg_extern    (step_motor_14_readable_reg_extern_wire),
    //电机输出端口
    .motor_out_enable       (motor_out_enable[14]),
    .motor_out_dir          (motor_out_dir[14]),
    .motor_out_pluse        (motor_out_pluse[14]),
    //电机编码器端口
    .motor_encoder_a        (motor_encoder_a[14]),
    .motor_encoder_b        (motor_encoder_b[14]),
    //电机传感器端口
    .motor_sensor_in_reset              (motor_sensor_in_reset[14]),
    .motor_sensor_in_emergency_stop     (motor_sensor_in_emergency_stop[14]),
    .motor_sensor_in_util_stop          (motor_sensor_in_util_stop[14])
);

//实例化步进电机模块15
step_motor_control
#(
    .SYS_CLK_FREQ           (SYS_PLL_STEP_MOTOR_FREQ) //输入时钟    
)step_motor_control_15_inst
(
    //通用时钟复位
    .sys_clk                (clk_step_motor_group_d),//输入时钟
    .sys_rst_n              (rst_clk_pll_24m_n),//复位,低电平有效
    //输入的写信号      
    .write_single           (step_motor_15_write_single_wire),
    .write_addr             (fsmc_cmd_write_addr_wire),
    .write_data             (fsmc_cmd_write_data_wire),
    //输出寄存器
    .readable_reg_extern    (step_motor_15_readable_reg_extern_wire),
    //电机输出端口
    .motor_out_enable       (motor_out_enable[15]),
    .motor_out_dir          (motor_out_dir[15]),
    .motor_out_pluse        (motor_out_pluse[15]),
    //电机编码器端口
    .motor_encoder_a        (motor_encoder_a[15]),
    .motor_encoder_b        (motor_encoder_b[15]),
    //电机传感器端口
    .motor_sensor_in_reset              (motor_sensor_in_reset[15]),
    .motor_sensor_in_emergency_stop     (motor_sensor_in_emergency_stop[15]),
    .motor_sensor_in_util_stop          (motor_sensor_in_util_stop[15])
);

//实例化步进电机模块16
step_motor_control
#(
    .SYS_CLK_FREQ           (SYS_PLL_STEP_MOTOR_FREQ) //输入时钟    
)step_motor_control_16_inst
(
    //通用时钟复位
    .sys_clk                (clk_step_motor_group_d),//输入时钟
    .sys_rst_n              (rst_clk_pll_24m_n),//复位,低电平有效
    //输入的写信号      
    .write_single           (step_motor_16_write_single_wire),
    .write_addr             (fsmc_cmd_write_addr_wire),
    .write_data             (fsmc_cmd_write_data_wire),
    //输出寄存器
    .readable_reg_extern    (step_motor_16_readable_reg_extern_wire),
    //电机输出端口
    .motor_out_enable       (motor_out_enable[16]),
    .motor_out_dir          (motor_out_dir[16]),
    .motor_out_pluse        (motor_out_pluse[16]),
    //电机编码器端口
    .motor_encoder_a        (motor_encoder_a[16]),
    .motor_encoder_b        (motor_encoder_b[16]),
    //电机传感器端口
    .motor_sensor_in_reset              (motor_sensor_in_reset[16]),
    .motor_sensor_in_emergency_stop     (motor_sensor_in_emergency_stop[16]),
    .motor_sensor_in_util_stop          (motor_sensor_in_util_stop[16])
);


endmodule

