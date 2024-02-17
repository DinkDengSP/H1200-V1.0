/************************************************************************ 
 ** Author       : 邓小俊
 ** Date         : 2024-01-19 13:30:05
 ** Description  : When I Has Free Time ,I Will Write Description Below.
 ** description  : 
 ** LastEditors  : 邓小俊
 ** LastEditTime : 2024-01-22 11:25:39
 ** Email: xiaojun.deng@aliyun.com
 ** Copyright (c) 2024 by 公司名称, All Rights Reserved. 
 ************************************************************************/
module fsmc_write_cmd_process 
#(
    //输入主频
    parameter SYSTEM_CLK_FREQ = 32'd100_000_000
) 
(
    input       sys_clk,//输入时钟
    input       sys_rst_n,//复位信号
    //写入的信号接口
    input       fsmc_write_singal,//写信号
    input[31:0] fsmc_write_addr,//写地址
    input[31:0] fsmc_write_data,//写数据 
    //输出给外部模块的写地址和写数据
    output reg[15:0] peripherals_write_addr,
    output reg[31:0] peripherals_write_data,
    //输出给输出模块a的写信号
    output reg port_out_group_a_write_single,
    //输出给输出模块b的写信号
    output reg port_out_group_b_write_single,
    //输出给输出模块c的写信号
    output reg port_out_group_c_write_single,
    //输出给输出模块d的写信号
    output reg port_out_group_d_write_single,
    //输出给输入模块a的写信号
    output reg port_in_group_a_write_single,
    //输出给输入模块b的写信号
    output reg port_in_group_b_write_single,
    //输出给输入模块c的写信号
    output reg port_in_group_c_write_single,
    //输出给输入模块d的写信号
    output reg port_in_group_d_write_single,
    //输出给步进电机1模块的写信号
    output reg step_motor_1_write_single,
    //输出给步进电机2模块的写信号
    output reg step_motor_2_write_single,
    //输出给步进电机3模块的写信号
    output reg step_motor_3_write_single,
    //输出给步进电机4模块的写信号
    output reg step_motor_4_write_single,
    //输出给步进电机5模块的写信号
    output reg step_motor_5_write_single,
    //输出给步进电机6模块的写信号
    output reg step_motor_6_write_single,
    //输出给步进电机7模块的写信号
    output reg step_motor_7_write_single,
    //输出给步进电机8模块的写信号
    output reg step_motor_8_write_single,
    //输出给步进电机9模块的写信号
    output reg step_motor_9_write_single,
    //输出给步进电机10模块的写信号
    output reg step_motor_10_write_single,
    //输出给步进电机11模块的写信号
    output reg step_motor_11_write_single,
    //输出给步进电机12模块的写信号
    output reg step_motor_12_write_single,
    //输出给步进电机13模块的写信号
    output reg step_motor_13_write_single,
    //输出给步进电机14模块的写信号
    output reg step_motor_14_write_single,
    //输出给步进电机15模块的写信号
    output reg step_motor_15_write_single,
    //输出给步进电机16模块的写信号
    output reg step_motor_16_write_single
);

/*************************************************************************************/
/*********************************参数定义*********************************************/
/*************************************************************************************/
//写入输出端口A
localparam REG_WRITE_ADDR_PORT_OUT_GROUP_A_START        = 32'h00000000;
localparam REG_WRITE_ADDR_PORT_OUT_GROUP_A_END          = 32'h0000000f;
//写入输出端口B
localparam REG_WRITE_ADDR_PORT_OUT_GROUP_B_START        = 32'h00000010;
localparam REG_WRITE_ADDR_PORT_OUT_GROUP_B_END          = 32'h0000001f;
//写入输出端口C
localparam REG_WRITE_ADDR_PORT_OUT_GROUP_C_START        = 32'h00000020;
localparam REG_WRITE_ADDR_PORT_OUT_GROUP_C_END          = 32'h0000002f;
//写入输出端口D
localparam REG_WRITE_ADDR_PORT_OUT_GROUP_D_START        = 32'h00000030;
localparam REG_WRITE_ADDR_PORT_OUT_GROUP_D_END          = 32'h0000003f;
//写入输入端口A 
localparam REG_WRITE_ADDR_PORT_IN_GROUP_A_START         = 32'h00000100;
localparam REG_WRITE_ADDR_PORT_IN_GROUP_A_END           = 32'h0000010f;
//写入输入端口B 
localparam REG_WRITE_ADDR_PORT_IN_GROUP_B_START         = 32'h00000110;
localparam REG_WRITE_ADDR_PORT_IN_GROUP_B_END           = 32'h0000011f;
//写入输入端口C 
localparam REG_WRITE_ADDR_PORT_IN_GROUP_C_START         = 32'h00000120;
localparam REG_WRITE_ADDR_PORT_IN_GROUP_C_END           = 32'h0000012f;
//写入输入端口D 
localparam REG_WRITE_ADDR_PORT_IN_GROUP_D_START         = 32'h00000130;
localparam REG_WRITE_ADDR_PORT_IN_GROUP_D_END           = 32'h0000013f;
//写入步进电机1端口
localparam REG_WRITE_ADDR_STEP_MOTOR_1_START            = 32'h00010000;
localparam REG_WRITE_ADDR_STEP_MOTOR_1_END              = 32'h0001ffff;
//写入步进电机2端口 
localparam REG_WRITE_ADDR_STEP_MOTOR_2_START            = 32'h00020000;
localparam REG_WRITE_ADDR_STEP_MOTOR_2_END              = 32'h0002ffff;
//写入步进电机3端口 
localparam REG_WRITE_ADDR_STEP_MOTOR_3_START            = 32'h00030000;
localparam REG_WRITE_ADDR_STEP_MOTOR_3_END              = 32'h0003ffff;
//写入步进电机4端口 
localparam REG_WRITE_ADDR_STEP_MOTOR_4_START            = 32'h00040000;
localparam REG_WRITE_ADDR_STEP_MOTOR_4_END              = 32'h0004ffff;
//写入步进电机5端口 
localparam REG_WRITE_ADDR_STEP_MOTOR_5_START            = 32'h00050000;
localparam REG_WRITE_ADDR_STEP_MOTOR_5_END              = 32'h0005ffff;
//写入步进电机6端口 
localparam REG_WRITE_ADDR_STEP_MOTOR_6_START            = 32'h00060000;
localparam REG_WRITE_ADDR_STEP_MOTOR_6_END              = 32'h0006ffff;
//写入步进电机7端口 
localparam REG_WRITE_ADDR_STEP_MOTOR_7_START            = 32'h00070000;
localparam REG_WRITE_ADDR_STEP_MOTOR_7_END              = 32'h0007ffff;
//写入步进电机8端口 
localparam REG_WRITE_ADDR_STEP_MOTOR_8_START            = 32'h00080000;
localparam REG_WRITE_ADDR_STEP_MOTOR_8_END              = 32'h0008ffff;
//写入步进电机9端口 
localparam REG_WRITE_ADDR_STEP_MOTOR_9_START            = 32'h00090000;
localparam REG_WRITE_ADDR_STEP_MOTOR_9_END              = 32'h0009ffff;
//写入步进电机10端口    
localparam REG_WRITE_ADDR_STEP_MOTOR_10_START           = 32'h000a0000;
localparam REG_WRITE_ADDR_STEP_MOTOR_10_END             = 32'h000affff;
//写入步进电机11端口    
localparam REG_WRITE_ADDR_STEP_MOTOR_11_START           = 32'h000b0000;
localparam REG_WRITE_ADDR_STEP_MOTOR_11_END             = 32'h000bffff;
//写入步进电机12端口
localparam REG_WRITE_ADDR_STEP_MOTOR_12_START           = 32'h000c0000;
localparam REG_WRITE_ADDR_STEP_MOTOR_12_END             = 32'h000cffff;
//写入步进电机13端口
localparam REG_WRITE_ADDR_STEP_MOTOR_13_START           = 32'h000d0000;
localparam REG_WRITE_ADDR_STEP_MOTOR_13_END             = 32'h000dffff;
//写入步进电机14端口
localparam REG_WRITE_ADDR_STEP_MOTOR_14_START           = 32'h000e0000;
localparam REG_WRITE_ADDR_STEP_MOTOR_14_END             = 32'h000effff;
//写入步进电机15端口
localparam REG_WRITE_ADDR_STEP_MOTOR_15_START           = 32'h000f0000;
localparam REG_WRITE_ADDR_STEP_MOTOR_15_END             = 32'h000fffff;
//写入步进电机16端口
localparam REG_WRITE_ADDR_STEP_MOTOR_16_START           = 32'h00100000;
localparam REG_WRITE_ADDR_STEP_MOTOR_16_END             = 32'h0010ffff;


/*************************************************************************************/    
/**********************************网线定义********************************************/
/*************************************************************************************/



/*************************************************************************************/
/*********************************寄存器定义*******************************************/
/*************************************************************************************/
//延时信号节拍进行采样
reg [2:0]fsmc_write_singal_reg;

/*************************************************************************************/
/**********************************组合逻辑代码****************************************/
/*************************************************************************************/


/*************************************************************************************/
/**********************************时序逻辑代码****************************************/
/*************************************************************************************/
//写入信号的节拍采样
always @(posedge sys_clk or negedge sys_rst_n) 
begin
    if (1'b0 == sys_rst_n) 
        begin
        fsmc_write_singal_reg <= 3'b000;
    end
    else 
        begin
        //节拍采样
        fsmc_write_singal_reg <= {fsmc_write_singal_reg[1:0],fsmc_write_singal};
    end
end

//对写入信号进行分析处理,检测使能控制脉冲,采集写入值
always @(posedge sys_clk or negedge sys_rst_n) 
begin 
    if (1'b0 == sys_rst_n) 
    begin 
        //取消输出写入信号
        peripherals_write_addr <= 16'd0;
        peripherals_write_data <= 32'd0;
    end
    else if(3'b011 == fsmc_write_singal_reg) 
    begin
        //到达采样点,数据采样
        peripherals_write_addr <= fsmc_write_addr[15:0];
        peripherals_write_data <= fsmc_write_data;
    end
    else if(3'b100 == fsmc_write_singal_reg) 
    begin
        //输入的写入信号消失了
        peripherals_write_addr <= 16'd0;
        peripherals_write_data <= 32'd0;
    end
end

//对写入信号进行分析处理,检测使能控制脉冲,port_out_group_a
always @(posedge sys_clk or negedge sys_rst_n) 
begin 
    if (1'b0 == sys_rst_n) 
    begin 
        port_out_group_a_write_single <= 1'b0;
    end
    else if(3'b011 == fsmc_write_singal_reg) 
    begin
        if((fsmc_write_addr >= REG_WRITE_ADDR_PORT_OUT_GROUP_A_START) && (fsmc_write_addr <= REG_WRITE_ADDR_PORT_OUT_GROUP_A_END)) 
        begin
            //信号触发
            port_out_group_a_write_single <= 1'b1;
        end
        else 
        begin
            //信号保持
            port_out_group_a_write_single   <= port_out_group_a_write_single;
        end
    end
    else if(3'b100 == fsmc_write_singal_reg) 
    begin
        //输入的写入信号消失了,取消输出的触发信号
        if(1'b1 == port_out_group_a_write_single) 
        begin
            port_out_group_a_write_single <= 1'b0;
        end
        else
        begin
            //信号保持
            port_out_group_a_write_single <= port_out_group_a_write_single;
        end
    end
    else 
    begin
        //信号保持
        port_out_group_a_write_single   <= port_out_group_a_write_single;
    end
end

//对写入信号进行分析处理,检测使能控制脉冲,port_out_group_b
always @(posedge sys_clk or negedge sys_rst_n) 
begin 
    if (1'b0 == sys_rst_n) 
    begin 
        port_out_group_b_write_single <= 1'b0;
    end
    else if(3'b011 == fsmc_write_singal_reg) 
    begin
        if((fsmc_write_addr >= REG_WRITE_ADDR_PORT_OUT_GROUP_B_START) && (fsmc_write_addr <= REG_WRITE_ADDR_PORT_OUT_GROUP_B_END)) 
        begin
            //信号触发
            port_out_group_b_write_single <= 1'b1;
        end
        else 
        begin
            //信号保持
            port_out_group_b_write_single   <= port_out_group_b_write_single;
        end
    end
    else if(3'b100 == fsmc_write_singal_reg) 
    begin
        //输入的写入信号消失了,取消输出的触发信号
        if(1'b1 == port_out_group_b_write_single) 
        begin
            port_out_group_b_write_single <= 1'b0;
        end
        else
        begin
            //信号保持
            port_out_group_b_write_single <= port_out_group_b_write_single;
        end
    end
    else 
    begin
        //信号保持
        port_out_group_b_write_single   <= port_out_group_b_write_single;
    end
end

//对写入信号进行分析处理,检测使能控制脉冲,port_out_group_c
always @(posedge sys_clk or negedge sys_rst_n) 
begin 
    if (1'b0 == sys_rst_n) 
    begin 
        port_out_group_c_write_single <= 1'b0;
    end
    else if(3'b011 == fsmc_write_singal_reg) 
    begin
        if((fsmc_write_addr >= REG_WRITE_ADDR_PORT_OUT_GROUP_C_START) && (fsmc_write_addr <= REG_WRITE_ADDR_PORT_OUT_GROUP_C_END)) 
        begin
            //信号触发
            port_out_group_c_write_single <= 1'b1;
        end
        else 
        begin
            //信号保持
            port_out_group_c_write_single   <= port_out_group_c_write_single;
        end
    end
    else if(3'b100 == fsmc_write_singal_reg) 
    begin
        //输入的写入信号消失了,取消输出的触发信号
        if(1'b1 == port_out_group_c_write_single) 
        begin
            port_out_group_c_write_single <= 1'b0;
        end
        else
        begin
            //信号保持
            port_out_group_c_write_single <= port_out_group_c_write_single;
        end
    end
    else 
    begin
        //信号保持
        port_out_group_c_write_single   <= port_out_group_c_write_single;
    end
end

//对写入信号进行分析处理,检测使能控制脉冲,port_out_group_d
always @(posedge sys_clk or negedge sys_rst_n) 
begin 
    if (1'b0 == sys_rst_n) 
    begin 
        port_out_group_d_write_single <= 1'b0;
    end
    else if(3'b011 == fsmc_write_singal_reg) 
    begin
        if((fsmc_write_addr >= REG_WRITE_ADDR_PORT_OUT_GROUP_D_START) && (fsmc_write_addr <= REG_WRITE_ADDR_PORT_OUT_GROUP_D_END)) 
        begin
            //信号触发
            port_out_group_d_write_single <= 1'b1;
        end
        else 
        begin
            //信号保持
            port_out_group_d_write_single   <= port_out_group_d_write_single;
        end
    end
    else if(3'b100 == fsmc_write_singal_reg) 
    begin
        //输入的写入信号消失了,取消输出的触发信号
        if(1'b1 == port_out_group_d_write_single) 
        begin
            port_out_group_d_write_single <= 1'b0;
        end
        else
        begin
            //信号保持
            port_out_group_d_write_single <= port_out_group_d_write_single;
        end
    end
    else 
    begin
        //信号保持
        port_out_group_d_write_single   <= port_out_group_d_write_single;
    end
end

//对写入信号进行分析处理,检测使能控制脉冲,port_in_group_a
always @(posedge sys_clk or negedge sys_rst_n) 
begin 
    if (1'b0 == sys_rst_n) 
    begin 
        port_in_group_a_write_single <= 1'b0;
    end
    else if(3'b011 == fsmc_write_singal_reg) 
    begin
        if((fsmc_write_addr >= REG_WRITE_ADDR_PORT_IN_GROUP_A_START) && (fsmc_write_addr <= REG_WRITE_ADDR_PORT_IN_GROUP_A_END)) 
        begin
            //信号触发
            port_in_group_a_write_single <= 1'b1;
        end
        else 
        begin
            //信号保持
            port_in_group_a_write_single   <= port_in_group_a_write_single;
        end
    end
    else if(3'b100 == fsmc_write_singal_reg) 
    begin
        //输入的写入信号消失了,取消输出的触发信号
        if(1'b1 == port_in_group_a_write_single) 
        begin
            port_in_group_a_write_single <= 1'b0;
        end
        else
        begin
            //信号保持
            port_in_group_a_write_single <= port_in_group_a_write_single;
        end
    end
    else 
    begin
        //信号保持
        port_in_group_a_write_single   <= port_in_group_a_write_single;
    end
end

//对写入信号进行分析处理,检测使能控制脉冲,port_in_group_b
always @(posedge sys_clk or negedge sys_rst_n) 
begin 
    if (1'b0 == sys_rst_n) 
    begin 
        port_in_group_b_write_single <= 1'b0;
    end
    else if(3'b011 == fsmc_write_singal_reg) 
    begin
        if((fsmc_write_addr >= REG_WRITE_ADDR_PORT_IN_GROUP_B_START) && (fsmc_write_addr <= REG_WRITE_ADDR_PORT_IN_GROUP_B_END)) 
        begin
            //信号触发
            port_in_group_b_write_single <= 1'b1;
        end
        else 
        begin
            //信号保持
            port_in_group_b_write_single   <= port_in_group_b_write_single;
        end
    end
    else if(3'b100 == fsmc_write_singal_reg) 
    begin
        //输入的写入信号消失了,取消输出的触发信号
        if(1'b1 == port_in_group_b_write_single) 
        begin
            port_in_group_b_write_single <= 1'b0;
        end
        else
        begin
            //信号保持
            port_in_group_b_write_single <= port_in_group_b_write_single;
        end
    end
    else 
    begin
        //信号保持
        port_in_group_b_write_single   <= port_in_group_b_write_single;
    end
end

//对写入信号进行分析处理,检测使能控制脉冲,port_in_group_c
always @(posedge sys_clk or negedge sys_rst_n) 
begin 
    if (1'b0 == sys_rst_n) 
    begin 
        port_in_group_c_write_single <= 1'b0;
    end
    else if(3'b011 == fsmc_write_singal_reg) 
    begin
        if((fsmc_write_addr >= REG_WRITE_ADDR_PORT_IN_GROUP_C_START) && (fsmc_write_addr <= REG_WRITE_ADDR_PORT_IN_GROUP_C_END)) 
        begin
            //信号触发
            port_in_group_c_write_single <= 1'b1;
        end
        else 
        begin
            //信号保持
            port_in_group_c_write_single   <= port_in_group_c_write_single;
        end
    end
    else if(3'b100 == fsmc_write_singal_reg) 
    begin
        //输入的写入信号消失了,取消输出的触发信号
        if(1'b1 == port_in_group_c_write_single) 
        begin
            port_in_group_c_write_single <= 1'b0;
        end
        else
        begin
            //信号保持
            port_in_group_c_write_single <= port_in_group_c_write_single;
        end
    end
    else 
    begin
        //信号保持
        port_in_group_c_write_single   <= port_in_group_c_write_single;
    end
end

//对写入信号进行分析处理,检测使能控制脉冲,port_in_group_d
always @(posedge sys_clk or negedge sys_rst_n) 
begin 
    if (1'b0 == sys_rst_n) 
    begin 
        port_in_group_d_write_single <= 1'b0;
    end
    else if(3'b011 == fsmc_write_singal_reg) 
    begin
        if((fsmc_write_addr >= REG_WRITE_ADDR_PORT_IN_GROUP_D_START) && (fsmc_write_addr <= REG_WRITE_ADDR_PORT_IN_GROUP_D_END)) 
        begin
            //信号触发
            port_in_group_d_write_single <= 1'b1;
        end
        else 
        begin
            //信号保持
            port_in_group_d_write_single   <= port_in_group_d_write_single;
        end
    end
    else if(3'b100 == fsmc_write_singal_reg) 
    begin
        //输入的写入信号消失了,取消输出的触发信号
        if(1'b1 == port_in_group_d_write_single) 
        begin
            port_in_group_d_write_single <= 1'b0;
        end
        else
        begin
            //信号保持
            port_in_group_d_write_single <= port_in_group_d_write_single;
        end
    end
    else 
    begin
        //信号保持
        port_in_group_d_write_single   <= port_in_group_d_write_single;
    end
end

//对写入信号进行分析处理,检测使能控制脉冲,step_motor_1
always @(posedge sys_clk or negedge sys_rst_n) 
begin 
    if (1'b0 == sys_rst_n) 
    begin 
        step_motor_1_write_single <= 1'b0;
    end
    else if(3'b011 == fsmc_write_singal_reg) 
    begin
        if((fsmc_write_addr >= REG_WRITE_ADDR_STEP_MOTOR_1_START) && (fsmc_write_addr <= REG_WRITE_ADDR_STEP_MOTOR_1_END)) 
        begin
            //信号触发
            step_motor_1_write_single <= 1'b1;
        end
        else 
        begin
            //信号保持
            step_motor_1_write_single   <= step_motor_1_write_single;
        end
    end
    else if(3'b100 == fsmc_write_singal_reg) 
    begin
        //输入的写入信号消失了,取消输出的触发信号
        if(1'b1 == step_motor_1_write_single) 
        begin
            step_motor_1_write_single <= 1'b0;
        end
        else
        begin
            //信号保持
            step_motor_1_write_single <= step_motor_1_write_single;
        end
    end
    else 
    begin
        //信号保持
        step_motor_1_write_single   <= step_motor_1_write_single;
    end
end

//对写入信号进行分析处理,检测使能控制脉冲,step_motor_2
always @(posedge sys_clk or negedge sys_rst_n) 
begin 
    if (1'b0 == sys_rst_n) 
    begin 
        step_motor_2_write_single <= 1'b0;
    end
    else if(3'b011 == fsmc_write_singal_reg) 
    begin
        if((fsmc_write_addr >= REG_WRITE_ADDR_STEP_MOTOR_2_START) && (fsmc_write_addr <= REG_WRITE_ADDR_STEP_MOTOR_2_END)) 
        begin
            //信号触发
            step_motor_2_write_single <= 1'b1;
        end
        else 
        begin
            //信号保持
            step_motor_2_write_single   <= step_motor_2_write_single;
        end
    end
    else if(3'b100 == fsmc_write_singal_reg) 
    begin
        //输入的写入信号消失了,取消输出的触发信号
        if(1'b1 == step_motor_2_write_single) 
        begin
            step_motor_2_write_single <= 1'b0;
        end
        else
        begin
            //信号保持
            step_motor_2_write_single <= step_motor_2_write_single;
        end
    end
    else 
    begin
        //信号保持
        step_motor_2_write_single   <= step_motor_2_write_single;
    end
end

//对写入信号进行分析处理,检测使能控制脉冲,step_motor_3
always @(posedge sys_clk or negedge sys_rst_n) 
begin 
    if (1'b0 == sys_rst_n) 
    begin 
        step_motor_3_write_single <= 1'b0;
    end
    else if(3'b011 == fsmc_write_singal_reg) 
    begin
        if((fsmc_write_addr >= REG_WRITE_ADDR_STEP_MOTOR_3_START) && (fsmc_write_addr <= REG_WRITE_ADDR_STEP_MOTOR_3_END)) 
        begin
            //信号触发
            step_motor_3_write_single <= 1'b1;
        end
        else 
        begin
            //信号保持
            step_motor_3_write_single   <= step_motor_3_write_single;
        end
    end
    else if(3'b100 == fsmc_write_singal_reg) 
    begin
        //输入的写入信号消失了,取消输出的触发信号
        if(1'b1 == step_motor_3_write_single) 
        begin
            step_motor_3_write_single <= 1'b0;
        end
        else
        begin
            //信号保持
            step_motor_3_write_single <= step_motor_3_write_single;
        end
    end
    else 
    begin
        //信号保持
        step_motor_3_write_single   <= step_motor_3_write_single;
    end
end

//对写入信号进行分析处理,检测使能控制脉冲,step_motor_4
always @(posedge sys_clk or negedge sys_rst_n) 
begin 
    if (1'b0 == sys_rst_n) 
    begin 
        step_motor_4_write_single <= 1'b0;
    end
    else if(3'b011 == fsmc_write_singal_reg) 
    begin
        if((fsmc_write_addr >= REG_WRITE_ADDR_STEP_MOTOR_4_START) && (fsmc_write_addr <= REG_WRITE_ADDR_STEP_MOTOR_4_END)) 
        begin
            //信号触发
            step_motor_4_write_single <= 1'b1;
        end
        else 
        begin
            //信号保持
            step_motor_4_write_single   <= step_motor_4_write_single;
        end
    end
    else if(3'b100 == fsmc_write_singal_reg) 
    begin
        //输入的写入信号消失了,取消输出的触发信号
        if(1'b1 == step_motor_4_write_single) 
        begin
            step_motor_4_write_single <= 1'b0;
        end
        else
        begin
            //信号保持
            step_motor_4_write_single <= step_motor_4_write_single;
        end
    end
    else 
    begin
        //信号保持
        step_motor_4_write_single   <= step_motor_4_write_single;
    end
end

//对写入信号进行分析处理,检测使能控制脉冲,step_motor_5
always @(posedge sys_clk or negedge sys_rst_n) 
begin 
    if (1'b0 == sys_rst_n) 
    begin 
        step_motor_5_write_single <= 1'b0;
    end
    else if(3'b011 == fsmc_write_singal_reg) 
    begin
        if((fsmc_write_addr >= REG_WRITE_ADDR_STEP_MOTOR_5_START) && (fsmc_write_addr <= REG_WRITE_ADDR_STEP_MOTOR_5_END)) 
        begin
            //信号触发
            step_motor_5_write_single <= 1'b1;
        end
        else 
        begin
            //信号保持
            step_motor_5_write_single   <= step_motor_5_write_single;
        end
    end
    else if(3'b100 == fsmc_write_singal_reg) 
    begin
        //输入的写入信号消失了,取消输出的触发信号
        if(1'b1 == step_motor_5_write_single) 
        begin
            step_motor_5_write_single <= 1'b0;
        end
        else
        begin
            //信号保持
            step_motor_5_write_single <= step_motor_5_write_single;
        end
    end
    else 
    begin
        //信号保持
        step_motor_5_write_single   <= step_motor_5_write_single;
    end
end

//对写入信号进行分析处理,检测使能控制脉冲,step_motor_6
always @(posedge sys_clk or negedge sys_rst_n) 
begin 
    if (1'b0 == sys_rst_n) 
    begin 
        step_motor_6_write_single <= 1'b0;
    end
    else if(3'b011 == fsmc_write_singal_reg) 
    begin
        if((fsmc_write_addr >= REG_WRITE_ADDR_STEP_MOTOR_6_START) && (fsmc_write_addr <= REG_WRITE_ADDR_STEP_MOTOR_6_END)) 
        begin
            //信号触发
            step_motor_6_write_single <= 1'b1;
        end
        else 
        begin
            //信号保持
            step_motor_6_write_single   <= step_motor_6_write_single;
        end
    end
    else if(3'b100 == fsmc_write_singal_reg) 
    begin
        //输入的写入信号消失了,取消输出的触发信号
        if(1'b1 == step_motor_6_write_single) 
        begin
            step_motor_6_write_single <= 1'b0;
        end
        else
        begin
            //信号保持
            step_motor_6_write_single <= step_motor_6_write_single;
        end
    end
    else 
    begin
        //信号保持
        step_motor_6_write_single   <= step_motor_6_write_single;
    end
end

//对写入信号进行分析处理,检测使能控制脉冲,step_motor_7
always @(posedge sys_clk or negedge sys_rst_n) 
begin 
    if (1'b0 == sys_rst_n) 
    begin 
        step_motor_7_write_single <= 1'b0;
    end
    else if(3'b011 == fsmc_write_singal_reg) 
    begin
        if((fsmc_write_addr >= REG_WRITE_ADDR_STEP_MOTOR_7_START) && (fsmc_write_addr <= REG_WRITE_ADDR_STEP_MOTOR_7_END)) 
        begin
            //信号触发
            step_motor_7_write_single <= 1'b1;
        end
        else 
        begin
            //信号保持
            step_motor_7_write_single   <= step_motor_7_write_single;
        end
    end
    else if(3'b100 == fsmc_write_singal_reg) 
    begin
        //输入的写入信号消失了,取消输出的触发信号
        if(1'b1 == step_motor_7_write_single) 
        begin
            step_motor_7_write_single <= 1'b0;
        end
        else
        begin
            //信号保持
            step_motor_7_write_single <= step_motor_7_write_single;
        end
    end
    else 
    begin
        //信号保持
        step_motor_7_write_single   <= step_motor_7_write_single;
    end
end

//对写入信号进行分析处理,检测使能控制脉冲,step_motor_8
always @(posedge sys_clk or negedge sys_rst_n) 
begin 
    if (1'b0 == sys_rst_n) 
    begin 
        step_motor_8_write_single <= 1'b0;
    end
    else if(3'b011 == fsmc_write_singal_reg) 
    begin
        if((fsmc_write_addr >= REG_WRITE_ADDR_STEP_MOTOR_8_START) && (fsmc_write_addr <= REG_WRITE_ADDR_STEP_MOTOR_8_END)) 
        begin
            //信号触发
            step_motor_8_write_single <= 1'b1;
        end
        else 
        begin
            //信号保持
            step_motor_8_write_single   <= step_motor_8_write_single;
        end
    end
    else if(3'b100 == fsmc_write_singal_reg) 
    begin
        //输入的写入信号消失了,取消输出的触发信号
        if(1'b1 == step_motor_8_write_single) 
        begin
            step_motor_8_write_single <= 1'b0;
        end
        else
        begin
            //信号保持
            step_motor_8_write_single <= step_motor_8_write_single;
        end
    end
    else 
    begin
        //信号保持
        step_motor_8_write_single   <= step_motor_8_write_single;
    end
end

//对写入信号进行分析处理,检测使能控制脉冲,step_motor_9
always @(posedge sys_clk or negedge sys_rst_n) 
begin 
    if (1'b0 == sys_rst_n) 
    begin 
        step_motor_9_write_single <= 1'b0;
    end
    else if(3'b011 == fsmc_write_singal_reg) 
    begin
        if((fsmc_write_addr >= REG_WRITE_ADDR_STEP_MOTOR_9_START) && (fsmc_write_addr <= REG_WRITE_ADDR_STEP_MOTOR_9_END)) 
        begin
            //信号触发
            step_motor_9_write_single <= 1'b1;
        end
        else 
        begin
            //信号保持
            step_motor_9_write_single   <= step_motor_9_write_single;
        end
    end
    else if(3'b100 == fsmc_write_singal_reg) 
    begin
        //输入的写入信号消失了,取消输出的触发信号
        if(1'b1 == step_motor_9_write_single) 
        begin
            step_motor_9_write_single <= 1'b0;
        end
        else
        begin
            //信号保持
            step_motor_9_write_single <= step_motor_9_write_single;
        end
    end
    else 
    begin
        //信号保持
        step_motor_9_write_single   <= step_motor_9_write_single;
    end
end

//对写入信号进行分析处理,检测使能控制脉冲,step_motor_10
always @(posedge sys_clk or negedge sys_rst_n) 
begin 
    if (1'b0 == sys_rst_n) 
    begin 
        step_motor_10_write_single <= 1'b0;
    end
    else if(3'b011 == fsmc_write_singal_reg) 
    begin
        if((fsmc_write_addr >= REG_WRITE_ADDR_STEP_MOTOR_10_START) && (fsmc_write_addr <= REG_WRITE_ADDR_STEP_MOTOR_10_END)) 
        begin
            //信号触发
            step_motor_10_write_single <= 1'b1;
        end
        else 
        begin
            //信号保持
            step_motor_10_write_single   <= step_motor_10_write_single;
        end
    end
    else if(3'b100 == fsmc_write_singal_reg) 
    begin
        //输入的写入信号消失了,取消输出的触发信号
        if(1'b1 == step_motor_10_write_single) 
        begin
            step_motor_10_write_single <= 1'b0;
        end
        else
        begin
            //信号保持
            step_motor_10_write_single <= step_motor_10_write_single;
        end
    end
    else 
    begin
        //信号保持
        step_motor_10_write_single   <= step_motor_10_write_single;
    end
end

//对写入信号进行分析处理,检测使能控制脉冲,step_motor_11
always @(posedge sys_clk or negedge sys_rst_n) 
begin 
    if (1'b0 == sys_rst_n) 
    begin 
        step_motor_11_write_single <= 1'b0;
    end
    else if(3'b011 == fsmc_write_singal_reg) 
    begin
        if((fsmc_write_addr >= REG_WRITE_ADDR_STEP_MOTOR_11_START) && (fsmc_write_addr <= REG_WRITE_ADDR_STEP_MOTOR_11_END)) 
        begin
            //信号触发
            step_motor_11_write_single <= 1'b1;
        end
        else 
        begin
            //信号保持
            step_motor_11_write_single   <= step_motor_11_write_single;
        end
    end
    else if(3'b100 == fsmc_write_singal_reg) 
    begin
        //输入的写入信号消失了,取消输出的触发信号
        if(1'b1 == step_motor_11_write_single) 
        begin
            step_motor_11_write_single <= 1'b0;
        end
        else
        begin
            //信号保持
            step_motor_11_write_single <= step_motor_11_write_single;
        end
    end
    else 
    begin
        //信号保持
        step_motor_11_write_single   <= step_motor_11_write_single;
    end
end

//对写入信号进行分析处理,检测使能控制脉冲,step_motor_12
always @(posedge sys_clk or negedge sys_rst_n) 
begin 
    if (1'b0 == sys_rst_n) 
    begin 
        step_motor_12_write_single <= 1'b0;
    end
    else if(3'b011 == fsmc_write_singal_reg) 
    begin
        if((fsmc_write_addr >= REG_WRITE_ADDR_STEP_MOTOR_12_START) && (fsmc_write_addr <= REG_WRITE_ADDR_STEP_MOTOR_12_END)) 
        begin
            //信号触发
            step_motor_12_write_single <= 1'b1;
        end
        else 
        begin
            //信号保持
            step_motor_12_write_single   <= step_motor_12_write_single;
        end
    end
    else if(3'b100 == fsmc_write_singal_reg) 
    begin
        //输入的写入信号消失了,取消输出的触发信号
        if(1'b1 == step_motor_12_write_single) 
        begin
            step_motor_12_write_single <= 1'b0;
        end
        else
        begin
            //信号保持
            step_motor_12_write_single <= step_motor_12_write_single;
        end
    end
    else 
    begin
        //信号保持
        step_motor_12_write_single   <= step_motor_12_write_single;
    end
end

//对写入信号进行分析处理,检测使能控制脉冲,step_motor_13
always @(posedge sys_clk or negedge sys_rst_n) 
begin 
    if (1'b0 == sys_rst_n) 
    begin 
        step_motor_13_write_single <= 1'b0;
    end
    else if(3'b011 == fsmc_write_singal_reg) 
    begin
        if((fsmc_write_addr >= REG_WRITE_ADDR_STEP_MOTOR_13_START) && (fsmc_write_addr <= REG_WRITE_ADDR_STEP_MOTOR_13_END)) 
        begin
            //信号触发
            step_motor_13_write_single <= 1'b1;
        end
        else 
        begin
            //信号保持
            step_motor_13_write_single   <= step_motor_13_write_single;
        end
    end
    else if(3'b100 == fsmc_write_singal_reg) 
    begin
        //输入的写入信号消失了,取消输出的触发信号
        if(1'b1 == step_motor_13_write_single) 
        begin
            step_motor_13_write_single <= 1'b0;
        end
        else
        begin
            //信号保持
            step_motor_13_write_single <= step_motor_13_write_single;
        end
    end
    else 
    begin
        //信号保持
        step_motor_13_write_single   <= step_motor_13_write_single;
    end
end

//对写入信号进行分析处理,检测使能控制脉冲,step_motor_14
always @(posedge sys_clk or negedge sys_rst_n) 
begin 
    if (1'b0 == sys_rst_n) 
    begin 
        step_motor_14_write_single <= 1'b0;
    end
    else if(3'b011 == fsmc_write_singal_reg) 
    begin
        if((fsmc_write_addr >= REG_WRITE_ADDR_STEP_MOTOR_14_START) && (fsmc_write_addr <= REG_WRITE_ADDR_STEP_MOTOR_14_END)) 
        begin
            //信号触发
            step_motor_14_write_single <= 1'b1;
        end
        else 
        begin
            //信号保持
            step_motor_14_write_single   <= step_motor_14_write_single;
        end
    end
    else if(3'b100 == fsmc_write_singal_reg) 
    begin
        //输入的写入信号消失了,取消输出的触发信号
        if(1'b1 == step_motor_14_write_single) 
        begin
            step_motor_14_write_single <= 1'b0;
        end
        else
        begin
            //信号保持
            step_motor_14_write_single <= step_motor_14_write_single;
        end
    end
    else 
    begin
        //信号保持
        step_motor_14_write_single   <= step_motor_14_write_single;
    end
end

//对写入信号进行分析处理,检测使能控制脉冲,step_motor_15
always @(posedge sys_clk or negedge sys_rst_n) 
begin 
    if (1'b0 == sys_rst_n) 
    begin 
        step_motor_15_write_single <= 1'b0;
    end
    else if(3'b011 == fsmc_write_singal_reg) 
    begin
        if((fsmc_write_addr >= REG_WRITE_ADDR_STEP_MOTOR_15_START) && (fsmc_write_addr <= REG_WRITE_ADDR_STEP_MOTOR_15_END)) 
        begin
            //信号触发
            step_motor_15_write_single <= 1'b1;
        end
        else 
        begin
            //信号保持
            step_motor_15_write_single   <= step_motor_15_write_single;
        end
    end
    else if(3'b100 == fsmc_write_singal_reg) 
    begin
        //输入的写入信号消失了,取消输出的触发信号
        if(1'b1 == step_motor_15_write_single) 
        begin
            step_motor_15_write_single <= 1'b0;
        end
        else
        begin
            //信号保持
            step_motor_15_write_single <= step_motor_15_write_single;
        end
    end
    else 
    begin
        //信号保持
        step_motor_15_write_single   <= step_motor_15_write_single;
    end
end

//对写入信号进行分析处理,检测使能控制脉冲,step_motor_16
always @(posedge sys_clk or negedge sys_rst_n) 
begin 
    if (1'b0 == sys_rst_n) 
    begin 
        step_motor_16_write_single <= 1'b0;
    end
    else if(3'b011 == fsmc_write_singal_reg) 
    begin
        if((fsmc_write_addr >= REG_WRITE_ADDR_STEP_MOTOR_16_START) && (fsmc_write_addr <= REG_WRITE_ADDR_STEP_MOTOR_16_END)) 
        begin
            //信号触发
            step_motor_16_write_single <= 1'b1;
        end
        else 
        begin
            //信号保持
            step_motor_16_write_single   <= step_motor_16_write_single;
        end
    end
    else if(3'b100 == fsmc_write_singal_reg) 
    begin
        //输入的写入信号消失了,取消输出的触发信号
        if(1'b1 == step_motor_16_write_single) 
        begin
            step_motor_16_write_single <= 1'b0;
        end
        else
        begin
            //信号保持
            step_motor_16_write_single <= step_motor_16_write_single;
        end
    end
    else 
    begin
        //信号保持
        step_motor_16_write_single   <= step_motor_16_write_single;
    end
end








endmodule









