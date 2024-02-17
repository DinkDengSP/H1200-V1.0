/************************************************************************ 
 ** Author       : 邓小俊
 ** Date         : 2024-01-19 13:30:05
 ** Description  : When I Has Free Time ,I Will Write Description Below.
 ** description  : 
 ** LastEditors  : 邓小俊
 ** LastEditTime : 2024-01-22 13:59:54
 ** Email: xiaojun.deng@aliyun.com
 ** Copyright (c) 2024 by 公司名称, All Rights Reserved. 
 ************************************************************************/
module step_motor_control 
#(
    parameter SYS_CLK_FREQ                      = 32'd60_000_000,//输入时钟
    parameter CONFIG1_REG_INIT_VAL              = 32'hffffffef,//配置寄存器初始值
    parameter PARAM1_REG_INIT_VAL               = 32'h3F800000,//配置寄存器1初始值1.0
    parameter PARAM2_REG_INIT_VAL               = 32'hffffffff,//配置寄存器2初始值
    parameter PARAM3_REG_INIT_VAL               = 32'hffffffff,//配置寄存器3初始值
    parameter PARAM4_REG_INIT_VAL               = 32'h00000000,//配置寄存器4初始值
    parameter PARAM5_REG_INIT_VAL               = 32'h00000000,//配置寄存器5初始值
    parameter CONTROL_REG_INIT_VAL              = 32'h00000000,//控制寄存器初始值
    parameter SENSOR_INPUT_DEBOUNCE_CLK         = 32'd59//传感器采样消抖系数
) 
(
    //通用时钟复位
    input           sys_clk  ,//输入时钟
    input           sys_rst_n,//复位,低电平有效
    //输入的写信号
    input           write_single,
    input [15:0]    write_addr,
    input [31:0]    write_data,
    //输出的寄存器
    output [((14*32)):0]   readable_reg_extern,
    //输入复位IO
    input motor_sensor_in_reset,
    //输入急停IO
    input motor_sensor_in_emergency_stop,
    //输入辅助急停IO
    input motor_sensor_in_util_stop,
    //步进电机输出
    output reg motor_out_enable,
    output reg motor_out_dir,
    output motor_out_pluse,
    //编码器输入
    input motor_encoder_a,
    input motor_encoder_b
);

/*************************************************************************************/
/*********************************参数定义*********************************************/
/*************************************************************************************/
//步数表和速度表的长度
localparam LENGTH_SPD_STP_TABLE             = 16'd1024;
//地址
//配置寄存器
localparam REG_WRITE_ADDR_CONFIG_1          = 16'h0000;
//参数寄存器
localparam REG_WRITE_ADDR_PARAM_1           = 16'h0001;
localparam REG_WRITE_ADDR_PARAM_2           = 16'h0002;
localparam REG_WRITE_ADDR_PARAM_3           = 16'h0003;
localparam REG_WRITE_ADDR_PARAM_4           = 16'h0004;
localparam REG_WRITE_ADDR_PARAM_5           = 16'h0005;
//控制寄存器
localparam REG_WRITE_ADDR_CONTROL_1         = 16'h0006;
//读取速度表
localparam REG_WRITE_ADDR_READ_SPD_TABLE    = 16'h0010;
//读取步数表
localparam REG_WRITE_ADDR_READ_STP_TABLE    = 16'h0011;
//速度表
localparam REG_WRITE_ADDR_SPD_TABLE_BASE    = 16'h0020;
localparam REG_WRITE_ADDR_SPD_TABLE_MAX     = REG_WRITE_ADDR_SPD_TABLE_BASE + LENGTH_SPD_STP_TABLE;
//步数表
localparam REG_WRITE_ADDR_STP_TABLE_BASE    = 16'h8000;
localparam REG_WRITE_ADDR_STP_TABLE_MAX     = REG_WRITE_ADDR_STP_TABLE_BASE + LENGTH_SPD_STP_TABLE;

//ram表操作指令
localparam CMD_TABLE_IDLE                   = 3'b001;//空闲
localparam CMD_TABLE_WRITE                  = 3'b010;//写入
localparam CMD_TABLE_READ                   = 3'b100;//读取

//步进电机状态
//电机空闲
localparam STATE_IDLE                       = 16'h0001;
//定频正向脉冲发送,正在运行 
localparam STATE_FIX_SPD_CW_RUN             = 16'h0002;
//定频反向脉冲发送,正在运行 
localparam STATE_FIX_SPD_CCW_RUN            = 16'h0004;
//步数表正向脉冲发送,正在运行   
localparam STATE_TAB_SPD_CW_RUN             = 16'h0008;
//步数表反向脉冲发送,正在运行   
localparam STATE_TAB_SPD_CCW_RUN            = 16'h0010;
//步进电机条件停止
localparam STATE_STOP_CONDITION             = 16'h0020;
//步进电机指令停止
localparam STATE_STOP_CMD                   = 16'h0040;

//步进电机停止原因
//正常运转没有停止
localparam STOP_REASON_NONE                                 = 16'h0000;
//正向定频运转
//速度参数错误
localparam STOP_REASON_FIX_SPD_CW_PARAM_SPD                 = 16'h0001;
//步数参数错误
localparam STOP_REASON_FIX_SPD_CW_PARAM_STP                 = 16'h0002;
//指令急停
localparam STOP_REASON_FIX_SPD_CW_CMD                       = 16'h0003;
//复位信号有效导致停止
localparam STOP_REASON_FIX_SPD_CW_SENSOR_RESET_VALID        = 16'h0004;
//复位信号无效导致停止
localparam STOP_REASON_FIX_SPD_CW_SENSOR_RESET_INVALID      = 16'h0005;
//急停信号有效导致停止
localparam STOP_REASON_FIX_SPD_CW_SENSOR_EM_STOP_VALID      = 16'h0006;
//辅助急停有效导致停止
localparam STOP_REASON_FIX_SPD_CW_SENSOR_UTIL_STOP_VALID    = 16'h0007;
//脉冲发送完成停止
localparam STOP_REASON_FIX_SPD_CW_PLUSE_OVER                = 16'h0009;
//反向定频运转
//速度参数错误
localparam STOP_REASON_FIX_SPD_CCW_PARAM_SPD                = 16'h0010;
//步数参数错误
localparam STOP_REASON_FIX_SPD_CCW_PARAM_STP                = 16'h0020;
//指令急停
localparam STOP_REASON_FIX_SPD_CCW_CMD                      = 16'h0030;
//复位信号有效导致停止
localparam STOP_REASON_FIX_SPD_CCW_SENSOR_RESET_VALID       = 16'h0040;
//复位信号无效导致停止
localparam STOP_REASON_FIX_SPD_CCW_SENSOR_RESET_INVALID     = 16'h0050;
//急停信号有效导致停止
localparam STOP_REASON_FIX_SPD_CCW_SENSOR_EM_STOP_VALID     = 16'h0060;
//辅助急停有效导致停止
localparam STOP_REASON_FIX_SPD_CCW_SENSOR_UTIL_STOP_VALID   = 16'h0070;
//脉冲发送完成停止
localparam STOP_REASON_FIX_SPD_CCW_PLUSE_OVER               = 16'h0090;
//步数表正向运转
//速度参数错误
localparam STOP_REASON_TAB_SPD_CW_PARAM_SPD                 = 16'h0100;
//步数参数错误  
localparam STOP_REASON_TAB_SPD_CW_PARAM_STP                 = 16'h0200;
//指令急停  
localparam STOP_REASON_TAB_SPD_CW_CMD                       = 16'h0300;
//复位信号有效导致停止  
localparam STOP_REASON_TAB_SPD_CW_SENSOR_RESET_VALID        = 16'h0400;
//复位信号无效导致停止  
localparam STOP_REASON_TAB_SPD_CW_SENSOR_RESET_INVALID      = 16'h0500;
//急停信号有效导致停止  
localparam STOP_REASON_TAB_SPD_CW_SENSOR_EM_STOP_VALID      = 16'h0600;
//辅助急停有效导致停止 
localparam STOP_REASON_TAB_SPD_CW_SENSOR_UTIL_STOP_VALID    = 16'h0700;
//脉冲发送完成停止  
localparam STOP_REASON_TAB_SPD_CW_PLUSE_OVER                = 16'h0900;
//启动序号错误
localparam STOP_REASON_TAB_SPD_CW_PARAM_START_INDEX         = 16'h0a00;
//停止序号错误
localparam STOP_REASON_TAB_SPD_CW_PARAM_STOP_INDEX          = 16'h0b00;
//启停信号阈值错误
localparam STOP_REASON_TAB_SPD_CW_PARAM_INDEX_RANGE         = 16'h0c00;
//步数表反向运转
//速度参数错误
localparam STOP_REASON_TAB_SPD_CCW_PARAM_SPD                = 16'h1000;
//步数参数错误  
localparam STOP_REASON_TAB_SPD_CCW_PARAM_STP                = 16'h2000;
//指令急停  
localparam STOP_REASON_TAB_SPD_CCW_CMD                      = 16'h3000;
//复位信号有效导致停止  
localparam STOP_REASON_TAB_SPD_CCW_SENSOR_RESET_VALID       = 16'h4000;
//复位信号无效导致停止  
localparam STOP_REASON_TAB_SPD_CCW_SENSOR_RESET_INVALID     = 16'h5000;
//急停信号有效导致停止  
localparam STOP_REASON_TAB_SPD_CCW_SENSOR_EM_STOP_VALID     = 16'h6000;
//辅助急停有效导致停止 
localparam STOP_REASON_TAB_SPD_CCW_SENSOR_UTIL_STOP_VALID   = 16'h7000;
//脉冲发送完成停止  
localparam STOP_REASON_TAB_SPD_CCW_PLUSE_OVER               = 16'h9000;
//启动序号错误
localparam STOP_REASON_TAB_SPD_CCW_PARAM_START_INDEX        = 16'ha000;
//停止序号错误
localparam STOP_REASON_TAB_SPD_CCW_PARAM_STOP_INDEX         = 16'hb000;
//启停信号阈值错误
localparam STOP_REASON_TAB_SPD_CCW_PARAM_INDEX_RANGE        = 16'hc000;

//电机基础逻辑状态
//空闲状态
localparam BASE_LOGIC_IDLE                  = 4'b0001;
//启动脉冲已发送状态
localparam BASE_LOGIC_START                 = 4'b0010; 
//下一步步数计算状态
localparam BASE_LOGIC_NEXT_STEP             = 4'b0100;  
//检测到忙状态
localparam BASE_LOGIC_BUSY                  = 4'b1000;

/**************************步进电机指令***********************************************/
//无指令
localparam CMD_NONE      = 16'h0000;
//电机转入空闲状态
localparam CMD_IDLE      = 16'h0001;
//电机指令停止
localparam CMD_STOP      = 16'h0002;
//电机进入定频正向数据发送状态
localparam CMD_FIX_CW    = 16'h0004;
//电机进入定频反向数据发送状态
localparam CMD_FIX_CCW   = 16'h0008;
//电机进入表驱动正向发送状态
localparam CMD_TAB_CW    = 16'h0010;
//电机进入表驱动反向数据发送状态
localparam CMD_TAB_CCW   = 16'h0020;

/*************************************************************************************/    
/**********************************网线定义********************************************/
/*************************************************************************************/
//编码器启停信号
wire control_encoder_start_wire;

//ram块数据读取,外部展示
wire[31:0] spd_tab_data_wire;
wire[31:0] stp_tab_data_wire;

//ram块当前速度表项
wire[31:0] spd_tab_logic_current_item_wire;
//ram块当前步数表项
wire[31:0] stp_tab_logic_current_item_wire;

//电机脉冲发送器模块
wire base_logic_busy_state;
//传感器状态
wire sensor_reset_state_wire;
wire sensor_em_stop_state_wire;
wire sensor_util_stop_state_wire;

/*************************************************************************************/
/*********************************寄存器定义*******************************************/
/*************************************************************************************/
//延时信号节拍进行采样
reg [2:0]write_singal_reg;
//记录输入的地址
reg[15:0] local_write_addr_reg;
reg[31:0] local_write_data_reg;

//速度表RAM块信号
reg spd_tab_aclr;
//写入A,指令写入
reg spd_tab_wren_a;
reg[9:0] spd_tab_address_a;
reg[31:0] spd_tab_write_data_a;
//读取数据
reg[9:0] spd_tab_address_b;
reg[31:0] spd_tab_read_data_b;

//步数表RAM块信号
reg stp_tab_aclr;
//写入A,指令写入
reg stp_tab_wren_a;
reg[9:0] stp_tab_address_a;
reg[31:0] stp_tab_write_data_a;
//读取数据
reg[9:0] stp_tab_address_b;
reg[31:0] stp_tab_read_data_b;

//电机配置寄存器,信号有效电平设置
reg[31:0] config_sensor_valid_level_reg;
//电机参数寄存器1,编码器信号映射为电机步数的比例关系
reg[31:0] param_encoder_pos_calc_factor_fp_reg1;
//电机参数寄存器2,定频输出速度设置
reg[31:0] param_fix_spd_set_reg2;
//电机参数寄存器3,本次总运行步数设置
reg[31:0] param_output_pluse_max_count_set_reg3;
//电机参数寄存器4,表驱动输出起始结束位置设置
reg[31:0] param_tab_spd_index_start_stop_set_reg4;
//电机参数寄存器5,停止条件设置
reg[31:0] param_motor_stop_condition_set_reg5;
//电机控制寄存器
reg[31:0] control_reg;
//电机中断寄存器
reg int_reg;
//电机状态寄存器,当前已发送步数状态
reg[31:0] state_motor_pluse_send_count_all_reg1;
//编码器当前计数值,编码器当前位置状态
wire signed[31:0] state_encoder_position_current_reg2;
//状态寄存器3,信号状态与当前表索引状态
reg[31:0] state_sensor_state_and_tab_index_reg3;
//状态寄存器4,步进电机当前状态
reg[31:0] state_motor_status_now_reg4;
//状态寄存器5,当前速度已输出脉冲值状态
reg[31:0] state_motor_pluse_send_current_spd_reg5;


//步进电机速度表写入寄存器
reg[2:0] spd_tab_cmd;
reg[15:0] spd_tab_address_cmd;
reg[31:0] spd_tab_write_data_cmd;

//步进电机步数表写入寄存器
reg[2:0] stp_tab_cmd;
reg[15:0] stp_tab_address_cmd;
reg[31:0] stp_tab_write_data_cmd;

//外部指令对编码器数据更新标记
reg encoder_position_clear_flag_reg;
//外部指令对电机指令写入新的值标记
reg cmd_change_flag_reg;

//电机基础控制逻辑
reg[31:0] base_logic_cnt;
reg base_logic_start;
reg[3:0] base_logic_state;
//上一次发出的速度
reg[31:0] last_spd_record;

/*************************************************************************************/
/**********************************组合逻辑代码****************************************/
/*************************************************************************************/
//组合内部寄存器信号输出
assign readable_reg_extern = {int_reg,stp_tab_data_wire,spd_tab_data_wire,state_motor_pluse_send_current_spd_reg5,state_motor_status_now_reg4,
                                state_sensor_state_and_tab_index_reg3,state_encoder_position_current_reg2,state_motor_pluse_send_count_all_reg1,
                                control_reg,param_motor_stop_condition_set_reg5,param_tab_spd_index_start_stop_set_reg4,
                                param_output_pluse_max_count_set_reg3,param_fix_spd_set_reg2,param_encoder_pos_calc_factor_fp_reg1,
                                config_sensor_valid_level_reg};

//关联编码器控制信号
assign control_encoder_start_wire = control_reg[0];

/*************************************************************************************/
/**********************************IP实例化****************************************/
/*************************************************************************************/
//步进电机速度表RAM块
step_motor_table_ram table_ram_spd_table_inst(
    .aclr            (spd_tab_aclr),
    .address_a       (spd_tab_address_a),
    .address_b       (state_sensor_state_and_tab_index_reg3[25:16]),
    .clock           (sys_clk),
    .data_a          (spd_tab_write_data_a),
    .data_b          (32'd0),
    .wren_a          (spd_tab_wren_a),
    .wren_b          (1'b0),
    .q_a             (spd_tab_data_wire),
    .q_b             (spd_tab_logic_current_item_wire)
);

//步进电机速度表RAM块
step_motor_table_ram table_ram_stp_table_inst(
    .aclr            (stp_tab_aclr),
    .address_a       (stp_tab_address_a),
    .address_b       (state_sensor_state_and_tab_index_reg3[25:16]),
    .clock           (sys_clk),
    .data_a          (stp_tab_write_data_a),
    .data_b          (32'd0),
    .wren_a          (stp_tab_wren_a),
    .wren_b          (1'b0),
    .q_a             (stp_tab_data_wire),
    .q_b             (stp_tab_logic_current_item_wire)
);

//步进电机编码器实例化
step_motor_encoder 
#(
    .SYS_CLK_FREQ           (SYS_CLK_FREQ),//输入时钟
    .FILTER_CLK_SET         (6'd9)//滤波器设定
)step_motor_encoder_inst
(
    .sys_clk                        (sys_clk),//输入时钟
    .sys_rst_n                      (sys_rst_n),//复位,低电平有效
    .motor_encoder_a                (motor_encoder_a),//输入信号A相
    .motor_encoder_b                (motor_encoder_b),//输入信号B相
    .encoder_a_valid_level          (config_sensor_valid_level_reg[7]),//编码器A相有效信号
    .encoder_b_valid_level          (config_sensor_valid_level_reg[8]),//编码器B相有效信号
    .encoder_start                  (control_encoder_start_wire),//编码器启停
    .encoder_position_calc_factor_fp(param_encoder_pos_calc_factor_fp_reg1),//编码器修改当前计数值
    .encoder_position_clear_singal  (encoder_position_clear_flag_reg),//编码器修改计数值信号
    .encoder_position               (state_encoder_position_current_reg2)//输出的编码器计数值
);

//步进电机基础脉冲发送逻辑
step_motor_base_logic step_motor_base_logic_inst(
    //通用时钟复位
    .sys_clk                        (sys_clk),
    .sys_rst_n                      (sys_rst_n),
    //外部控制信号
    .trig_start                     (base_logic_start),
    .spd_cnt_period                 (base_logic_cnt),
    .step_motor_pluse_idle_level    (config_sensor_valid_level_reg[6]),
    //外部输出信号
    .busy_state                     (base_logic_busy_state),
    .pluse_out                      (motor_out_pluse)
);

//复位信号采样
sensor_debounce 
#(
    .SYS_CLK_FREQ   (SYS_CLK_FREQ),
    .DEBOUNCE_CLK   (SENSOR_INPUT_DEBOUNCE_CLK)
)
sensor_debounce_reset_inst
(
    .sys_clk            (sys_clk),
    .sys_rst_n          (sys_rst_n),
    .sensor_input       (motor_sensor_in_reset),
    .sensor_valid_level (config_sensor_valid_level_reg[0]),
    .sensor_debounce_val(sensor_reset_state_wire)
);

//急停信号采样
sensor_debounce 
#(
    .SYS_CLK_FREQ   (SYS_CLK_FREQ),
    .DEBOUNCE_CLK   (SENSOR_INPUT_DEBOUNCE_CLK)
)
sensor_debounce_em_stop_inst
(
    .sys_clk            (sys_clk),
    .sys_rst_n          (sys_rst_n),
    .sensor_input       (motor_sensor_in_emergency_stop),
    .sensor_valid_level (config_sensor_valid_level_reg[1]),
    .sensor_debounce_val(sensor_em_stop_state_wire)
);

//辅助急停信号采样
sensor_debounce 
#(
    .SYS_CLK_FREQ   (SYS_CLK_FREQ),
    .DEBOUNCE_CLK   (SENSOR_INPUT_DEBOUNCE_CLK)
)
sensor_debounce_util_stop_inst
(
    .sys_clk            (sys_clk),
    .sys_rst_n          (sys_rst_n),
    .sensor_input       (motor_sensor_in_util_stop),
    .sensor_valid_level (config_sensor_valid_level_reg[2]),
    .sensor_debounce_val(sensor_util_stop_state_wire)
);

/*************************************************************************************/
/**********************************时序逻辑代码****************************************/
/*************************************************************************************/
//对外部发送过来的写信号做锁存
always @(posedge sys_clk or negedge sys_rst_n) 
begin 
    if (1'b0 == sys_rst_n) 
    begin 
        write_singal_reg <= 3'b000;
        local_write_addr_reg <= 16'd0;
        local_write_data_reg <= 32'd0;
    end
    else 
    begin
        //加载写入使能信号
        write_singal_reg <= {write_singal_reg[1:0],write_single};
        //加载总线数据
        local_write_addr_reg <= write_addr;
        local_write_data_reg <= write_data;
    end
end


//寄存器写入config_reg
always @(posedge sys_clk or negedge sys_rst_n) 
begin
    if (1'b0 == sys_rst_n) 
    begin 
        //寄存器列表初始化
        //配置
        config_sensor_valid_level_reg <= CONFIG1_REG_INIT_VAL;
    end
    else if((3'b011 == write_singal_reg)&&(REG_WRITE_ADDR_CONFIG_1 == local_write_addr_reg)) begin
        //数据写入
        config_sensor_valid_level_reg <= local_write_data_reg;
    end
    else begin
        //状态保持
        config_sensor_valid_level_reg <= config_sensor_valid_level_reg;
    end
end

//寄存器写入 param_encoder_pos_calc_factor_fp_reg1
always @(posedge sys_clk or negedge sys_rst_n) 
begin
    if (1'b0 == sys_rst_n) 
    begin 
        //寄存器列表初始化
        //配置
        param_encoder_pos_calc_factor_fp_reg1 <= PARAM1_REG_INIT_VAL;
    end
    else if((3'b011 == write_singal_reg)&&(REG_WRITE_ADDR_PARAM_1 == local_write_addr_reg)) begin
        //数据写入
        param_encoder_pos_calc_factor_fp_reg1 <= local_write_data_reg;
    end
    else begin
        //状态保持
        param_encoder_pos_calc_factor_fp_reg1 <= param_encoder_pos_calc_factor_fp_reg1;
    end
end

//寄存器写入 param_fix_spd_set_reg2
always @(posedge sys_clk or negedge sys_rst_n) 
begin
    if (1'b0 == sys_rst_n) 
    begin 
        //寄存器列表初始化
        //配置
        param_fix_spd_set_reg2 <= PARAM2_REG_INIT_VAL;
    end
    else if((3'b011 == write_singal_reg)&&(REG_WRITE_ADDR_PARAM_2 == local_write_addr_reg)) begin
        //数据写入
        param_fix_spd_set_reg2 <= local_write_data_reg;
    end
    else begin
        //状态保持
        param_fix_spd_set_reg2 <= param_fix_spd_set_reg2;
    end
end

//寄存器写入 param_output_pluse_max_count_set_reg3
always @(posedge sys_clk or negedge sys_rst_n) 
begin
    if (1'b0 == sys_rst_n) 
    begin 
        //寄存器列表初始化
        //配置
        param_output_pluse_max_count_set_reg3 <= PARAM3_REG_INIT_VAL;
    end
    else if((3'b011 == write_singal_reg)&&(REG_WRITE_ADDR_PARAM_3 == local_write_addr_reg)) begin
        //数据写入
        param_output_pluse_max_count_set_reg3 <= local_write_data_reg;
    end
    else begin
        //状态保持
        param_output_pluse_max_count_set_reg3 <= param_output_pluse_max_count_set_reg3;
    end
end

//寄存器写入 param_tab_spd_index_start_stop_set_reg4
always @(posedge sys_clk or negedge sys_rst_n) 
begin
    if (1'b0 == sys_rst_n) 
    begin 
        //寄存器列表初始化
        //配置
        param_tab_spd_index_start_stop_set_reg4 <= PARAM4_REG_INIT_VAL;
    end
    else if((3'b011 == write_singal_reg)&&(REG_WRITE_ADDR_PARAM_4 == local_write_addr_reg)) begin
        //数据写入
        param_tab_spd_index_start_stop_set_reg4 <= local_write_data_reg;
    end
    else begin
        //状态保持
        param_tab_spd_index_start_stop_set_reg4 <= param_tab_spd_index_start_stop_set_reg4;
    end
end

//寄存器写入 param_motor_stop_condition_set_reg5
always @(posedge sys_clk or negedge sys_rst_n) 
begin
    if (1'b0 == sys_rst_n) 
    begin 
        //寄存器列表初始化
        //配置
        param_motor_stop_condition_set_reg5 <= PARAM5_REG_INIT_VAL;
    end
    else if((3'b011 == write_singal_reg)&&(REG_WRITE_ADDR_PARAM_5 == local_write_addr_reg)) begin
        //数据写入
        param_motor_stop_condition_set_reg5 <= local_write_data_reg;
    end
    else begin
        //状态保持
        param_motor_stop_condition_set_reg5 <= param_motor_stop_condition_set_reg5;
    end
end

//寄存器写入 control_reg
always @(posedge sys_clk or negedge sys_rst_n) 
begin
    if (1'b0 == sys_rst_n) 
    begin 
        //寄存器列表初始化
        control_reg <= CONTROL_REG_INIT_VAL;
        cmd_change_flag_reg <= 1'b0;
    end
    else if((3'b011 == write_singal_reg)&&(REG_WRITE_ADDR_CONTROL_1 == local_write_addr_reg)) begin
        //检查写入信号和原本的寄存器信号是否匹配
        if(control_reg[31:16] != local_write_data_reg[31:16])begin
            //设置写入标记
            cmd_change_flag_reg <= 1'b1;
        end
        else begin
            cmd_change_flag_reg <= cmd_change_flag_reg;
        end
        if((control_reg[3] == 1'b0)&&(local_write_data_reg[3] == 1'b1)) begin
            encoder_position_clear_flag_reg <= 1'b1;
        end
        else begin
            encoder_position_clear_flag_reg <= encoder_position_clear_flag_reg;
        end
        //数据写入
        control_reg <= local_write_data_reg;
    end
    else begin
        //状态保持
        control_reg <= control_reg;
        //指令信号只产生一次
        if(1'b1 == cmd_change_flag_reg)begin
            cmd_change_flag_reg <= 1'b0;
        end
        else begin
            cmd_change_flag_reg <= cmd_change_flag_reg;
        end
        if(1'b1 == encoder_position_clear_flag_reg)begin
            encoder_position_clear_flag_reg <= 1'b0;
        end
        else begin
            encoder_position_clear_flag_reg <= encoder_position_clear_flag_reg;
        end
    end
end

//寄存器对速度表进行操作
always @(posedge sys_clk or negedge sys_rst_n) 
begin
    if (1'b0 == sys_rst_n) 
    begin 
        spd_tab_cmd <= CMD_TABLE_IDLE;
        spd_tab_address_cmd <= 16'd0;
        spd_tab_write_data_cmd <= 32'd0;
    end
    else if(3'b011 == write_singal_reg) begin
        if(REG_WRITE_ADDR_READ_SPD_TABLE == local_write_addr_reg) begin
            //读取速度表数据
            spd_tab_cmd <= CMD_TABLE_READ;
            //地址是写入数据的低16位
            spd_tab_address_cmd <= local_write_data_reg[15:0];
            //写入数据保持不变
            spd_tab_write_data_cmd <= spd_tab_write_data_cmd;
        end
        else if((local_write_addr_reg >= REG_WRITE_ADDR_SPD_TABLE_BASE)&&
                    (local_write_addr_reg < REG_WRITE_ADDR_SPD_TABLE_MAX)) begin
            //写入电机速度表
            spd_tab_cmd <= CMD_TABLE_WRITE;
            spd_tab_address_cmd <= local_write_addr_reg - REG_WRITE_ADDR_SPD_TABLE_BASE;
            spd_tab_write_data_cmd <= local_write_data_reg;
        end
        else begin
            //不知道做什么
            spd_tab_cmd <= spd_tab_cmd;
            spd_tab_address_cmd <= spd_tab_address_cmd;
            spd_tab_write_data_cmd <= spd_tab_write_data_cmd;
        end
    end
    else if(3'b100 == write_singal_reg) begin
        //切换空闲
        spd_tab_cmd <= CMD_TABLE_IDLE;
        spd_tab_address_cmd <= spd_tab_address_cmd;
        spd_tab_write_data_cmd <= spd_tab_write_data_cmd;
    end
    else begin
        //状态保持
        spd_tab_cmd <= spd_tab_cmd;
        spd_tab_address_cmd <= spd_tab_address_cmd;
        spd_tab_write_data_cmd <= spd_tab_write_data_cmd;
    end
end

//寄存器对步数表进行操作
always @(posedge sys_clk or negedge sys_rst_n) 
begin
    if (1'b0 == sys_rst_n) 
    begin 
        stp_tab_cmd <= CMD_TABLE_IDLE;
        stp_tab_address_cmd <= 16'd0;
        stp_tab_write_data_cmd <= 32'd0;
    end
    else if(3'b011 == write_singal_reg) begin
        if(REG_WRITE_ADDR_READ_STP_TABLE == local_write_addr_reg) begin
            //读取步数表数据
            stp_tab_cmd <= CMD_TABLE_READ;
            //地址是写入数据的低16位
            stp_tab_address_cmd <= local_write_data_reg[15:0];
            //写入数据保持不变
            stp_tab_write_data_cmd <= stp_tab_write_data_cmd;
        end
        else if((local_write_addr_reg >= REG_WRITE_ADDR_STP_TABLE_BASE)&&
                    (local_write_addr_reg < REG_WRITE_ADDR_STP_TABLE_MAX)) begin
            //写入电机步数表
            stp_tab_cmd <= CMD_TABLE_WRITE;
            stp_tab_address_cmd <= local_write_addr_reg - REG_WRITE_ADDR_STP_TABLE_BASE;
            stp_tab_write_data_cmd <= local_write_data_reg;
        end
        else begin
            //不知道做什么
            stp_tab_cmd <= stp_tab_cmd;
            stp_tab_address_cmd <= stp_tab_address_cmd;
            stp_tab_write_data_cmd <= stp_tab_write_data_cmd;
        end
    end
    else if(3'b100 == write_singal_reg) begin
        //切换空闲
        stp_tab_cmd <= CMD_TABLE_IDLE;
        stp_tab_address_cmd <= stp_tab_address_cmd;
        stp_tab_write_data_cmd <= stp_tab_write_data_cmd;
    end
    else begin
        //状态保持
        stp_tab_cmd <= stp_tab_cmd;
        stp_tab_address_cmd <= stp_tab_address_cmd;
        stp_tab_write_data_cmd <= stp_tab_write_data_cmd;
    end
end

//速度表RAM块读写,指令方式
always @(posedge sys_clk or negedge sys_rst_n) 
begin 
    if (1'b0 == sys_rst_n) begin 
        //初始化清除RAM块
        spd_tab_aclr <= 1'b1;
        //初始化设置写入值为0
        spd_tab_wren_a <= 1'b0;
        spd_tab_address_a <= 10'd0;
        spd_tab_write_data_a <= 32'd0;
    end
    else begin
        spd_tab_aclr <= 1'b0;
        case(spd_tab_cmd)
            CMD_TABLE_IDLE: begin
                //空闲状态
                spd_tab_wren_a <= 1'b0;
                spd_tab_address_a <= spd_tab_address_a;
                spd_tab_write_data_a <= spd_tab_write_data_a;
            end
            CMD_TABLE_WRITE: begin
                //写入
                spd_tab_wren_a <= 1'b1;
                spd_tab_address_a <= spd_tab_address_cmd[9:0];
                spd_tab_write_data_a <= spd_tab_write_data_cmd;
            end
            CMD_TABLE_READ: begin
                //读取
                spd_tab_wren_a <= 1'b0;
                spd_tab_address_a <= spd_tab_address_cmd[9:0];
            end
            default : begin
                spd_tab_wren_a <= 1'b0;
                spd_tab_address_a <= spd_tab_address_a;
                spd_tab_write_data_a <= spd_tab_write_data_a;
            end
        endcase
    end
end

//步数表RAM块读写,指令方式
always @(posedge sys_clk or negedge sys_rst_n) 
begin 
    if (1'b0 == sys_rst_n) begin 
        //初始化清除RAM块
        stp_tab_aclr <= 1'b1;
        //初始化设置写入值为0
        stp_tab_wren_a <= 1'b0;
        stp_tab_address_a <= 10'd0;
        stp_tab_write_data_a <= 32'd0;
    end
    else begin
        stp_tab_aclr <= 1'b0;
        case(stp_tab_cmd)
            CMD_TABLE_IDLE: begin
                //空闲状态
                stp_tab_wren_a <= 1'b0;
                stp_tab_address_a <= stp_tab_address_a;
                stp_tab_write_data_a <= stp_tab_write_data_a;
            end
            CMD_TABLE_WRITE: begin
                //写入
                stp_tab_wren_a <= 1'b1;
                stp_tab_address_a <= stp_tab_address_cmd[9:0];
                stp_tab_write_data_a <= stp_tab_write_data_cmd;
            end
            CMD_TABLE_READ: begin
                //读取
                stp_tab_wren_a <= 1'b0;
                stp_tab_address_a <= stp_tab_address_cmd[9:0];
            end
            default : begin
                stp_tab_wren_a <= 1'b0;
                stp_tab_address_a <= stp_tab_address_a;
                stp_tab_write_data_a <= stp_tab_write_data_a;
            end
        endcase
    end
end


/***************************************电机端口控制********************************************/
//电机使能禁止控制
always @(posedge sys_clk or negedge sys_rst_n) 
begin
    if (1'b0 == sys_rst_n) begin 
        //初始化电机不使能
        motor_out_enable <= (~config_sensor_valid_level_reg[4]);
    end
    else begin
        //电机存在的情况下,才能受控
        if(1'b0 == control_reg[1]) begin
            //电机禁止
            motor_out_enable <= (~config_sensor_valid_level_reg[4]);
        end
        else begin
            //电机使能
            motor_out_enable <= config_sensor_valid_level_reg[4];
        end
    end
end


/****************************************信号采样*******************************************/
//不需要的信号处理
always @(posedge sys_clk or negedge sys_rst_n) 
begin
    if (1'b0 == sys_rst_n) begin 
        //初始化状态无效
        state_sensor_state_and_tab_index_reg3[15:4] <= 12'd0;
    end
    else begin
        //原点传感器高电平有效
        state_sensor_state_and_tab_index_reg3[15:4] <= 12'd0;
    end
end

//复位信号采样
always @(posedge sys_clk or negedge sys_rst_n) 
begin
    if (1'b0 == sys_rst_n) begin 
        //初始化状态无效
        state_sensor_state_and_tab_index_reg3[0] <= 1'b0;
    end
    else begin
        state_sensor_state_and_tab_index_reg3[0] <= sensor_reset_state_wire;
    end
end

//急停信号采样
always @(posedge sys_clk or negedge sys_rst_n) 
begin
    if (1'b0 == sys_rst_n) begin 
        //初始化状态无效
        state_sensor_state_and_tab_index_reg3[1] <= 1'b0;
    end
    else begin
        state_sensor_state_and_tab_index_reg3[1] <= sensor_em_stop_state_wire;
    end
end

//辅助急停信号采样
always @(posedge sys_clk or negedge sys_rst_n) 
begin
    if (1'b0 == sys_rst_n) begin 
        //初始化状态无效
        state_sensor_state_and_tab_index_reg3[2] <= 1'b0;
    end
    else begin
        //原点传感器低电平有效
        state_sensor_state_and_tab_index_reg3[2] <= sensor_util_stop_state_wire;
    end
end


//根据状态机设置模块中断
always @(posedge sys_clk or negedge sys_rst_n) 
begin 
    if (1'b0 == sys_rst_n) 
    begin 
        //初始化无中断
        int_reg <= 1'b0;
    end
    else if(1'b0 == control_reg[1]) 
    begin
        //电机不使能的情况下,无中断
        int_reg <= 1'b0;
    end
    else 
    begin
        //电机存在且使能的情况下,才能执行状态机变化
        case(state_motor_status_now_reg4[15:0])
            STATE_IDLE: 
            begin
                //电机空闲
                int_reg <= 1'b0;
            end
            STATE_FIX_SPD_CW_RUN: 
            begin
                //定频正向脉冲发送
                int_reg <= 1'b0;
            end
            STATE_FIX_SPD_CCW_RUN: 
            begin
                //定频反向脉冲发送
                int_reg <= 1'b0;
            end
            STATE_TAB_SPD_CW_RUN: 
            begin
                //步数表正向脉冲发送
                int_reg <= 1'b0;
            end
            STATE_TAB_SPD_CCW_RUN: 
            begin
                //步数表反向脉冲发送
                int_reg <= 1'b0;
            end
            STATE_STOP_CONDITION: 
            begin
                //步进电机条件停止,触发中断
                int_reg <= 1'b1;
            end
            STATE_STOP_CMD: 
            begin
                //步进电机指令停止,不触发中断
                int_reg <= 1'b0;
            end
            default: 
            begin
                //默认状态
                int_reg <= 1'b0;
            end
        endcase
    end
end


/****************************************状态机以及状态切换标记*********************************************/
//步进电机状态机,初始化以及状态转换条件
always @(posedge sys_clk or negedge sys_rst_n) 
begin 
    if (1'b0 == sys_rst_n) begin 
        //初始化进入空闲状态
        state_motor_status_now_reg4[15:0] <= STATE_IDLE;
        //无错误
        state_motor_status_now_reg4[31:16] <= STOP_REASON_NONE;
    end
    else if(1'b0 == control_reg[1]) begin
        //初始化进入空闲状态
        state_motor_status_now_reg4[15:0] <= STATE_IDLE;
        //无错误
        state_motor_status_now_reg4[31:16] <= STOP_REASON_NONE;
    end
    else begin
        //电机存在且使能的情况下,才能执行状态机变化
        case(state_motor_status_now_reg4[15:0])
            STATE_IDLE: begin
                //电机空闲,指令启动
                if(cmd_change_flag_reg == 1'b1) begin
                    //收到新的指令码
                    case(control_reg[31:16])
                        CMD_FIX_CW: begin
                            //检查各种条件合理就进入FIX CW
                            if(32'd0 == param_fix_spd_set_reg2)begin
                                //定频输出的速度必须不等于0,切入指令错误停止
                                state_motor_status_now_reg4[15:0] <= STATE_STOP_CONDITION;
                                //速度参数错误
                                state_motor_status_now_reg4[31:16] <= STOP_REASON_FIX_SPD_CW_PARAM_SPD;
                            end
                            else if(32'd0 == param_output_pluse_max_count_set_reg3) begin
                                //定频输出的步数必须不等于0,切入指令错误停止
                                state_motor_status_now_reg4[15:0] <= STATE_STOP_CONDITION;
                                //速度参数错误
                                state_motor_status_now_reg4[31:16] <= STOP_REASON_FIX_SPD_CW_PARAM_STP;
                            end
                            else begin
                                //切入CW 定频输出
                                state_motor_status_now_reg4[15:0] <= STATE_FIX_SPD_CW_RUN;
                                //错误清零
                                state_motor_status_now_reg4[31:16] <= STOP_REASON_NONE;
                            end
                        end
                        CMD_FIX_CCW: begin
                            //检查各种条件合理就进入FIX CCW
                            if(32'd0 == param_fix_spd_set_reg2)begin
                                //定频输出的速度必须不等于0,切入指令错误停止
                                state_motor_status_now_reg4[15:0] <= STATE_STOP_CONDITION;
                                //速度参数错误
                                state_motor_status_now_reg4[31:16] <= STOP_REASON_FIX_SPD_CCW_PARAM_SPD;
                            end
                            else if(32'd0 == param_output_pluse_max_count_set_reg3) begin
                                //定频输出的步数必须不等于0,切入指令错误停止
                                state_motor_status_now_reg4[15:0] <= STATE_STOP_CONDITION;
                                //速度参数错误
                                state_motor_status_now_reg4[31:16] <= STOP_REASON_FIX_SPD_CCW_PARAM_STP;
                            end
                            else begin
                                //切入CCW 定频输出
                                state_motor_status_now_reg4[15:0] <= STATE_FIX_SPD_CCW_RUN;
                                //错误清零
                                state_motor_status_now_reg4[31:16] <= STOP_REASON_NONE;
                            end
                        end
                        CMD_TAB_CW: begin
                            //检查各种条件合理就进入TAB_SPD_CW
                            if(param_tab_spd_index_start_stop_set_reg4[15:0] >= LENGTH_SPD_STP_TABLE)begin
                                //表输出的序号必须小于表长度,切入指令错误停止
                                state_motor_status_now_reg4[15:0] <= STATE_STOP_CONDITION;
                                //速度参数错误
                                state_motor_status_now_reg4[31:16] <= STOP_REASON_TAB_SPD_CW_PARAM_START_INDEX;
                            end
                            else if(param_tab_spd_index_start_stop_set_reg4[31:16] >= LENGTH_SPD_STP_TABLE) begin
                                //表输出的序号必须小于表长度,切入指令错误停止
                                state_motor_status_now_reg4[15:0] <= STATE_STOP_CONDITION;
                                //速度参数错误
                                state_motor_status_now_reg4[31:16] <= STOP_REASON_TAB_SPD_CW_PARAM_STOP_INDEX;
                            end
                            else if(param_tab_spd_index_start_stop_set_reg4[15:0] > param_tab_spd_index_start_stop_set_reg4[31:16]) begin
                                //起始位置必须小于等于结束位置切入指令错误停止
                                state_motor_status_now_reg4[15:0] <= STATE_STOP_CONDITION;
                                //速度参数错误
                                state_motor_status_now_reg4[31:16] <= STOP_REASON_TAB_SPD_CW_PARAM_INDEX_RANGE;
                            end
                            else begin
                                //切入CW表输出
                                state_motor_status_now_reg4[15:0] <= STATE_TAB_SPD_CW_RUN;
                                //错误清零
                                state_motor_status_now_reg4[31:16] <= STOP_REASON_NONE;
                            end
                        end
                        CMD_TAB_CCW: begin
                            //检查各种条件合理就进入TAB_SPD_CW
                            if(param_tab_spd_index_start_stop_set_reg4[15:0] >= LENGTH_SPD_STP_TABLE)begin
                                //表起始输出的序号必须小于表长度,切入指令错误停止
                                state_motor_status_now_reg4[15:0] <= STATE_STOP_CONDITION;
                                //速度参数错误
                                state_motor_status_now_reg4[31:16] <= STOP_REASON_TAB_SPD_CCW_PARAM_START_INDEX;
                            end
                            else if(param_tab_spd_index_start_stop_set_reg4[31:16] >= LENGTH_SPD_STP_TABLE) begin
                                //表结束输出的序号必须小于表长度,切入指令错误停止
                                state_motor_status_now_reg4[15:0] <= STATE_STOP_CONDITION;
                                //速度参数错误
                                state_motor_status_now_reg4[31:16] <= STOP_REASON_TAB_SPD_CCW_PARAM_STOP_INDEX;
                            end
                            else if(param_tab_spd_index_start_stop_set_reg4[15:0] > param_tab_spd_index_start_stop_set_reg4[31:16]) begin
                                //起始位置必须小于等于结束位置,切入指令错误停止
                                state_motor_status_now_reg4[15:0] <= STATE_STOP_CONDITION;
                                //速度参数错误
                                state_motor_status_now_reg4[31:16] <= STOP_REASON_TAB_SPD_CCW_PARAM_INDEX_RANGE;
                            end
                            else begin
                                //切入CCW 表输出
                                state_motor_status_now_reg4[15:0] <= STATE_TAB_SPD_CCW_RUN;
                                //错误清零
                                state_motor_status_now_reg4[31:16] <= STOP_REASON_NONE;
                            end
                        end
                        default: begin
                            //保持空闲
                            state_motor_status_now_reg4[15:0] <= state_motor_status_now_reg4[15:0];
                            state_motor_status_now_reg4[31:16] <= state_motor_status_now_reg4[31:16];
                        end
                    endcase
                end
                else begin
                    //没有信号驱动,状态保持
                    state_motor_status_now_reg4[15:0] <= state_motor_status_now_reg4[15:0];
                    state_motor_status_now_reg4[31:16] <= state_motor_status_now_reg4[31:16];
                end
            end
            STATE_FIX_SPD_CW_RUN: begin
                //定频正向脉冲发送,首先原点传感器
                if((param_motor_stop_condition_set_reg5[0] == 1'b1)&&(state_sensor_state_and_tab_index_reg3[0] == 1'b1))begin
                    //设置了原点传感器有效停止条件并且检测到原点传感器有效
                    state_motor_status_now_reg4[15:0] <= STATE_STOP_CONDITION;
                    state_motor_status_now_reg4[31:16] <= STOP_REASON_FIX_SPD_CW_SENSOR_RESET_VALID;
                end
                else if((param_motor_stop_condition_set_reg5[1] == 1'b1)&&(state_sensor_state_and_tab_index_reg3[0] == 1'b0))begin
                    //设置了原点传感器无效停止且检测到原点传感器无效
                    state_motor_status_now_reg4[15:0] <= STATE_STOP_CONDITION;
                    state_motor_status_now_reg4[31:16] <= STOP_REASON_FIX_SPD_CW_SENSOR_RESET_INVALID;
                end
                else if((param_motor_stop_condition_set_reg5[2] == 1'b1)&&(state_sensor_state_and_tab_index_reg3[1] == 1'b1)) begin
                    //设置了急停有效停止且急停有效
                    state_motor_status_now_reg4[15:0] <= STATE_STOP_CONDITION;
                    state_motor_status_now_reg4[31:16] <= STOP_REASON_FIX_SPD_CW_SENSOR_EM_STOP_VALID;
                end
                else if((param_motor_stop_condition_set_reg5[3] == 1'b1)&&(state_sensor_state_and_tab_index_reg3[2] == 1'b1)) begin
                    //设置了辅助急停有效停止且辅助急停有效
                    state_motor_status_now_reg4[15:0] <= STATE_STOP_CONDITION;
                    state_motor_status_now_reg4[31:16] <= STOP_REASON_FIX_SPD_CW_SENSOR_UTIL_STOP_VALID;
                end
                else if((param_motor_stop_condition_set_reg5[5] == 1'b1)&&(state_motor_pluse_send_count_all_reg1[31:0] == param_output_pluse_max_count_set_reg3[31:0])) begin
                    //设置脉冲发送完成停止且发出的脉冲数量等于设置的脉冲数量
                    state_motor_status_now_reg4[15:0] <= STATE_STOP_CONDITION;
                    state_motor_status_now_reg4[31:16] <= STOP_REASON_FIX_SPD_CW_PLUSE_OVER;
                end
                else if((cmd_change_flag_reg == 1'b1)&&(control_reg[31:16] == CMD_STOP)) begin
                    //收到指令急停
                    state_motor_status_now_reg4[15:0] <= STATE_STOP_CMD;
                    state_motor_status_now_reg4[31:16] <= STOP_REASON_FIX_SPD_CW_CMD;
                end
                else begin
                    state_motor_status_now_reg4[15:0] <= state_motor_status_now_reg4[15:0];
                    state_motor_status_now_reg4[31:16] <= state_motor_status_now_reg4[31:16];
                end
            end
            STATE_FIX_SPD_CCW_RUN: begin
                //定频反向脉冲发送,首先原点传感器
                if((param_motor_stop_condition_set_reg5[0] == 1'b1)&&(state_sensor_state_and_tab_index_reg3[0] == 1'b1))begin
                    //设置了原点传感器有效停止条件并且检测到原点传感器有效
                    state_motor_status_now_reg4[15:0] <= STATE_STOP_CONDITION;
                    state_motor_status_now_reg4[31:16] <= STOP_REASON_FIX_SPD_CCW_SENSOR_RESET_VALID;
                end
                else if((param_motor_stop_condition_set_reg5[1] == 1'b1)&&(state_sensor_state_and_tab_index_reg3[0] == 1'b0))begin
                    //设置了原点传感器无效停止且检测到原点传感器无效
                    state_motor_status_now_reg4[15:0] <= STATE_STOP_CONDITION;
                    state_motor_status_now_reg4[31:16] <= STOP_REASON_FIX_SPD_CCW_SENSOR_RESET_INVALID;
                end
                else if((param_motor_stop_condition_set_reg5[2] == 1'b1)&&(state_sensor_state_and_tab_index_reg3[1] == 1'b1)) begin
                    //设置了急停有效停止且急停有效
                    state_motor_status_now_reg4[15:0] <= STATE_STOP_CONDITION;
                    state_motor_status_now_reg4[31:16] <= STOP_REASON_FIX_SPD_CCW_SENSOR_EM_STOP_VALID;
                end
                else if((param_motor_stop_condition_set_reg5[3] == 1'b1)&&(state_sensor_state_and_tab_index_reg3[2] == 1'b1)) begin
                    //设置了辅助急停有效停止且辅助急停有效
                    state_motor_status_now_reg4[15:0] <= STATE_STOP_CONDITION;
                    state_motor_status_now_reg4[31:16] <= STOP_REASON_FIX_SPD_CCW_SENSOR_UTIL_STOP_VALID;
                end
                else if((param_motor_stop_condition_set_reg5[5] == 1'b1)&&(state_motor_pluse_send_count_all_reg1[31:0] == param_output_pluse_max_count_set_reg3[31:0])) begin
                    //设置脉冲发送完成停止且发出的脉冲数量等于设置的脉冲数量
                    state_motor_status_now_reg4[15:0] <= STATE_STOP_CONDITION;
                    state_motor_status_now_reg4[31:16] <= STOP_REASON_FIX_SPD_CCW_PLUSE_OVER;
                end
                else if((cmd_change_flag_reg == 1'b1)&&(control_reg[31:16] == CMD_STOP)) begin
                    //收到指令急停
                    state_motor_status_now_reg4[15:0] <= STATE_STOP_CMD;
                    state_motor_status_now_reg4[31:16] <= STOP_REASON_FIX_SPD_CCW_CMD;
                end
                else begin
                    state_motor_status_now_reg4[15:0] <= state_motor_status_now_reg4[15:0];
                    state_motor_status_now_reg4[31:16] <= state_motor_status_now_reg4[31:16];
                end
            end
            STATE_TAB_SPD_CW_RUN: begin
                //步数表正向脉冲发送,首先原点传感器
                if((param_motor_stop_condition_set_reg5[0] == 1'b1)&&(state_sensor_state_and_tab_index_reg3[0] == 1'b1))begin
                    //设置了原点传感器有效停止条件并且检测到原点传感器有效
                    state_motor_status_now_reg4[15:0] <= STATE_STOP_CONDITION;
                    state_motor_status_now_reg4[31:16] <= STOP_REASON_TAB_SPD_CW_SENSOR_RESET_VALID;
                end
                else if((param_motor_stop_condition_set_reg5[1] == 1'b1)&&(state_sensor_state_and_tab_index_reg3[0] == 1'b0))begin
                    //设置了原点传感器无效停止且检测到原点传感器无效
                    state_motor_status_now_reg4[15:0] <= STATE_STOP_CONDITION;
                    state_motor_status_now_reg4[31:16] <= STOP_REASON_TAB_SPD_CW_SENSOR_RESET_INVALID;
                end
                else if((param_motor_stop_condition_set_reg5[2] == 1'b1)&&(state_sensor_state_and_tab_index_reg3[1] == 1'b1)) begin
                    //设置了急停有效停止且急停有效
                    state_motor_status_now_reg4[15:0] <= STATE_STOP_CONDITION;
                    state_motor_status_now_reg4[31:16] <= STOP_REASON_TAB_SPD_CW_SENSOR_EM_STOP_VALID;
                end
                else if((param_motor_stop_condition_set_reg5[3] == 1'b1)&&(state_sensor_state_and_tab_index_reg3[2] == 1'b1)) begin
                    //设置了辅助急停有效停止且辅助急停有效
                    state_motor_status_now_reg4[15:0] <= STATE_STOP_CONDITION;
                    state_motor_status_now_reg4[31:16] <= STOP_REASON_TAB_SPD_CW_SENSOR_UTIL_STOP_VALID;
                end
                else if((param_motor_stop_condition_set_reg5[5] == 1'b1)&&(state_motor_pluse_send_count_all_reg1[31:0] == param_output_pluse_max_count_set_reg3[31:0])) begin
                    //设置脉冲发送完成停止且发出的脉冲数量等于设置的脉冲数量
                    state_motor_status_now_reg4[15:0] <= STATE_STOP_CONDITION;
                    state_motor_status_now_reg4[31:16] <= STOP_REASON_TAB_SPD_CW_PLUSE_OVER;
                end
                else if((cmd_change_flag_reg == 1'b1)&&(control_reg[31:16] == CMD_STOP)) begin
                    //收到指令急停
                    state_motor_status_now_reg4[15:0] <= STATE_STOP_CMD;
                    state_motor_status_now_reg4[31:16] <= STOP_REASON_TAB_SPD_CW_CMD;
                end
                else if((32'd0 == spd_tab_logic_current_item_wire[31:0])&&(1'b0 == control_reg[2])) begin
                    //检测到当前RAM表的速度为0,停止,没开启预写入功能
                    state_motor_status_now_reg4[15:0] <= STATE_STOP_CONDITION;
                    state_motor_status_now_reg4[31:16] <= STOP_REASON_TAB_SPD_CW_PARAM_SPD;
                end
                else if((32'd0 == stp_tab_logic_current_item_wire[31:0])&&(1'b0 == control_reg[2])) begin
                    //检测到当前RAM表的步数为0,停止,没开启预写入功能
                    state_motor_status_now_reg4[15:0] <= STATE_STOP_CONDITION;
                    state_motor_status_now_reg4[31:16] <= STOP_REASON_TAB_SPD_CW_PARAM_STP;
                end
                else begin
                    state_motor_status_now_reg4[15:0] <= state_motor_status_now_reg4[15:0];
                    state_motor_status_now_reg4[31:16] <= state_motor_status_now_reg4[31:16];
                end
            end
            STATE_TAB_SPD_CCW_RUN: begin
                //步数表反向脉冲发送,首先原点传感器
                if((param_motor_stop_condition_set_reg5[0] == 1'b1)&&(state_sensor_state_and_tab_index_reg3[0] == 1'b1))begin
                    //设置了原点传感器有效停止条件并且检测到原点传感器有效
                    state_motor_status_now_reg4[15:0] <= STATE_STOP_CONDITION;
                    state_motor_status_now_reg4[31:16] <= STOP_REASON_TAB_SPD_CCW_SENSOR_RESET_VALID;
                end
                else if((param_motor_stop_condition_set_reg5[1] == 1'b1)&&(state_sensor_state_and_tab_index_reg3[0] == 1'b0))begin
                    //设置了原点传感器无效停止且检测到原点传感器无效
                    state_motor_status_now_reg4[15:0] <= STATE_STOP_CONDITION;
                    state_motor_status_now_reg4[31:16] <= STOP_REASON_TAB_SPD_CCW_SENSOR_RESET_INVALID;
                end
                else if((param_motor_stop_condition_set_reg5[2] == 1'b1)&&(state_sensor_state_and_tab_index_reg3[1] == 1'b1)) begin
                    //设置了急停有效停止且急停有效
                    state_motor_status_now_reg4[15:0] <= STATE_STOP_CONDITION;
                    state_motor_status_now_reg4[31:16] <= STOP_REASON_TAB_SPD_CCW_SENSOR_EM_STOP_VALID;
                end
                else if((param_motor_stop_condition_set_reg5[3] == 1'b1)&&(state_sensor_state_and_tab_index_reg3[2] == 1'b1)) begin
                    //设置了辅助急停有效停止且辅助急停有效
                    state_motor_status_now_reg4[15:0] <= STATE_STOP_CONDITION;
                    state_motor_status_now_reg4[31:16] <= STOP_REASON_TAB_SPD_CCW_SENSOR_UTIL_STOP_VALID;
                end
                else if((param_motor_stop_condition_set_reg5[5] == 1'b1)&&(state_motor_pluse_send_count_all_reg1[31:0] == param_output_pluse_max_count_set_reg3[31:0])) begin
                    //设置脉冲发送完成停止且发出的脉冲数量等于设置的脉冲数量
                    state_motor_status_now_reg4[15:0] <= STATE_STOP_CONDITION;
                    state_motor_status_now_reg4[31:16] <= STOP_REASON_TAB_SPD_CCW_PLUSE_OVER;
                end
                else if((cmd_change_flag_reg == 1'b1)&&(control_reg[31:16] == CMD_STOP)) begin
                    //收到指令急停
                    state_motor_status_now_reg4[15:0] <= STATE_STOP_CMD;
                    state_motor_status_now_reg4[31:16] <= STOP_REASON_TAB_SPD_CCW_CMD;
                end
                else if((32'd0 == spd_tab_logic_current_item_wire[31:0])&&(1'b0 == control_reg[2])) begin
                    //检测到当前RAM表的速度为0,停止,没开启预写入功能
                    state_motor_status_now_reg4[15:0] <= STATE_STOP_CONDITION;
                    state_motor_status_now_reg4[31:16] <= STOP_REASON_TAB_SPD_CCW_PARAM_SPD;
                end
                else if((32'd0 == stp_tab_logic_current_item_wire[31:0])&&(1'b0 == control_reg[2])) begin
                    //检测到当前RAM表的步数为0,停止,没开启预写入功能
                    state_motor_status_now_reg4[15:0] <= STATE_STOP_CONDITION;
                    state_motor_status_now_reg4[31:16] <= STOP_REASON_TAB_SPD_CCW_PARAM_STP;
                end
                else begin
                    //状态保持
                    state_motor_status_now_reg4[15:0] <= state_motor_status_now_reg4[15:0];
                    state_motor_status_now_reg4[31:16] <= state_motor_status_now_reg4[31:16];
                end
            end
            STATE_STOP_CONDITION: begin
                //定频反向脉冲发送停止,复位传感器有效
                if(cmd_change_flag_reg == 1'b1) begin
                    if(control_reg[31:16] == CMD_IDLE) begin
                        //切换空闲
                        state_motor_status_now_reg4[15:0] <= STATE_IDLE;
                        state_motor_status_now_reg4[31:16] <= STOP_REASON_NONE;
                    end
                    else if(control_reg[31:16] == CMD_STOP) begin
                        //切换指令停止,这样可以清楚中断又不会丢失系统保留信息
                        state_motor_status_now_reg4[15:0] <= STATE_STOP_CMD;
                        //错误原因保留原来的状态
                        state_motor_status_now_reg4[31:16] <= state_motor_status_now_reg4[31:16];
                    end
                    else begin
                        state_motor_status_now_reg4[15:0] <= state_motor_status_now_reg4[15:0];
                        state_motor_status_now_reg4[31:16] <= state_motor_status_now_reg4[31:16];
                    end
                end
                else begin
                    state_motor_status_now_reg4[15:0] <= state_motor_status_now_reg4[15:0];
                    state_motor_status_now_reg4[31:16] <= state_motor_status_now_reg4[31:16];
                end
            end
            STATE_STOP_CMD: begin
                //电机急停
                if(cmd_change_flag_reg == 1'b1) begin
                    if(control_reg[31:16] == CMD_IDLE) begin
                        //切换空闲
                        state_motor_status_now_reg4[15:0] <= STATE_IDLE;
                        state_motor_status_now_reg4[31:16] <= STOP_REASON_NONE;
                    end
                    else begin
                        state_motor_status_now_reg4[15:0] <= state_motor_status_now_reg4[15:0];
                        state_motor_status_now_reg4[31:16] <= state_motor_status_now_reg4[31:16];
                    end
                end
                else begin
                    state_motor_status_now_reg4[15:0] <= state_motor_status_now_reg4[15:0];
                    state_motor_status_now_reg4[31:16] <= state_motor_status_now_reg4[31:16];
                end
            end
            default: begin
                //默认状态
                state_motor_status_now_reg4[15:0] <= state_motor_status_now_reg4[15:0];
                state_motor_status_now_reg4[31:16] <= state_motor_status_now_reg4[31:16];
            end
        endcase
    end
end

//步进电机状态机,不同状态下电机的工作选择
//空闲状态下的状态机执行过程
always @(posedge sys_clk or negedge sys_rst_n) 
begin 
    if (1'b0 == sys_rst_n) begin 
        //复位初始化使用的变量
        //本次指令一共输出的脉冲值
        state_motor_pluse_send_count_all_reg1 <= 32'd0;
        //速度步数表的序号
        state_sensor_state_and_tab_index_reg3[31:16] <= 16'd0;
        //当前速度下已经输出的脉冲值
        state_motor_pluse_send_current_spd_reg5 <= 32'd0;
        //方向信号默认初始化为正方向信号
        motor_out_dir = config_sensor_valid_level_reg[5];
        //启动信号清除
        base_logic_start <= 1'b0;
        //速度计数值清除
        base_logic_cnt <= 32'd0;
        //基础状态为空闲
        base_logic_state <= BASE_LOGIC_IDLE;
        //上次的速度
        last_spd_record <= 32'd0;
    end
    else begin
        case(state_motor_status_now_reg4[15:0])
            STATE_IDLE: begin
                //空闲状态下清零这些标记
                state_motor_pluse_send_count_all_reg1 <= 32'd0;
                //空闲状态下速度步数表的序号等于设置的起始序号
                state_sensor_state_and_tab_index_reg3[31:16] <= param_tab_spd_index_start_stop_set_reg4[15:0];
                state_motor_pluse_send_current_spd_reg5 <= 32'd0;
                //启动信号清除
                base_logic_start <= 1'b0;
                //速度计数值清除
                base_logic_cnt <= 32'd0;
                //基础状态为空闲
                base_logic_state <= BASE_LOGIC_IDLE;
                //上次的速度
                last_spd_record <= 32'd0;
            end
            STATE_STOP_CONDITION: begin
                //条件满足停止
                state_motor_pluse_send_count_all_reg1 <= state_motor_pluse_send_count_all_reg1;
                state_sensor_state_and_tab_index_reg3[31:16] <= state_sensor_state_and_tab_index_reg3[31:16];
                state_motor_pluse_send_current_spd_reg5 <= state_motor_pluse_send_current_spd_reg5;
                //启动信号清除
                base_logic_start <= 1'b0;
                //速度计数值清除
                base_logic_cnt <= 32'd0;
                //基础状态为空闲
                base_logic_state <= BASE_LOGIC_IDLE;
                //上次的速度
                last_spd_record <= 32'd0;
            end
            STATE_STOP_CMD: begin
                //指令停止
                state_motor_pluse_send_count_all_reg1 <= state_motor_pluse_send_count_all_reg1;
                state_sensor_state_and_tab_index_reg3[31:16] <= state_sensor_state_and_tab_index_reg3[31:16];
                state_motor_pluse_send_current_spd_reg5 <= state_motor_pluse_send_current_spd_reg5;
                //启动信号清除
                base_logic_start <= 1'b0;
                //速度计数值清除
                base_logic_cnt <= 32'd0;
                //基础状态为空闲
                base_logic_state <= BASE_LOGIC_IDLE;
                //上次的速度
                last_spd_record <= 32'd0;
            end
            STATE_FIX_SPD_CW_RUN: begin
                //定频正向脉冲发送,方向信号设置为正方向
                motor_out_dir = config_sensor_valid_level_reg[5];
                //索引为0
                state_sensor_state_and_tab_index_reg3[31:16] <= 16'd0;
                //根据基础状态决定操作
                case(base_logic_state)
                    BASE_LOGIC_IDLE: begin
                        //加载定频速度值
                        base_logic_cnt[31:0] <= param_fix_spd_set_reg2[31:0];
                        if(32'd0 == base_logic_cnt[31:0]) begin
                            base_logic_state <= base_logic_state;
                        end
                        else begin
                            //检测忙信号,不忙启动脉冲发送
                            if(1'b0 == base_logic_busy_state)begin
                                //发送启动脉冲
                                base_logic_start <= 1'b1;
                                //进入启动状态
                                base_logic_state <= BASE_LOGIC_START;
                            end
                            else begin
                                base_logic_state <= base_logic_state;
                            end
                        end
                    end
                    BASE_LOGIC_START: begin
                        //启动状态下检测到busy高电平,进入忙状态
                        base_logic_start <= base_logic_start;
                        if(1'b1 == base_logic_busy_state) begin
                            //发送的脉冲值增加
                            state_motor_pluse_send_count_all_reg1 <= state_motor_pluse_send_count_all_reg1 + 1'b1;
                            //当前步数发送的脉冲值也增加
                            state_motor_pluse_send_current_spd_reg5 <= state_motor_pluse_send_current_spd_reg5 + 1'b1;
                            //进入下一步
                            base_logic_state <= BASE_LOGIC_NEXT_STEP;
                        end
                        else begin
                            //保持不变
                            base_logic_state <= base_logic_state;
                        end
                    end
                    BASE_LOGIC_NEXT_STEP: begin
                        base_logic_start <= 1'b0;
                        //进入忙状态
                        base_logic_state <= BASE_LOGIC_BUSY;
                    end
                    BASE_LOGIC_BUSY: begin
                        //忙状态检测空闲,进入空闲状态,准备下一次脉冲发送
                        base_logic_start <= 1'b0;
                        //在这个状态下就可以进行下一步的步数切换了
                        if(1'b0 == base_logic_busy_state) begin
                            //进入空闲状态准备下一次脉冲发送
                            base_logic_state <= BASE_LOGIC_IDLE;
                        end
                        else begin
                            //保持不变
                            base_logic_state <= base_logic_state;
                        end
                    end
                endcase
            end
            STATE_FIX_SPD_CCW_RUN: begin
                //定频反向脉冲发送
                motor_out_dir = (~config_sensor_valid_level_reg[5]);
                //索引为0
                state_sensor_state_and_tab_index_reg3[31:16] <= 16'd0;
                //根据基础状态决定操作
                case(base_logic_state)
                    BASE_LOGIC_IDLE: begin
                        //加载定频速度值
                        base_logic_cnt[31:0] <= param_fix_spd_set_reg2[31:0];
                        if(32'd0 == base_logic_cnt[31:0]) begin
                            base_logic_state <= base_logic_state;
                        end
                        else begin
                            //检测忙信号,不忙启动脉冲发送
                            if(1'b0 == base_logic_busy_state)begin
                                //发送启动脉冲
                                base_logic_start <= 1'b1;
                                //进入启动状态
                                base_logic_state <= BASE_LOGIC_START;
                            end
                            else begin
                                base_logic_state <= base_logic_state;
                            end
                        end
                    end
                    BASE_LOGIC_START: begin
                        //启动状态下检测到busy高电平,进入忙状态
                        base_logic_start <= base_logic_start;
                        if(1'b1 == base_logic_busy_state) begin
                            //发送的脉冲值增加
                            state_motor_pluse_send_count_all_reg1 <= state_motor_pluse_send_count_all_reg1 + 1'b1;
                            //当前步数发送的脉冲值也增加
                            state_motor_pluse_send_current_spd_reg5 <= state_motor_pluse_send_current_spd_reg5 + 1'b1;
                            //进入下一步
                            base_logic_state <= BASE_LOGIC_NEXT_STEP;
                        end
                        else begin
                            //保持不变
                            base_logic_state <= base_logic_state;
                        end
                    end
                    BASE_LOGIC_NEXT_STEP: begin
                        base_logic_start <= 1'b0;
                        //进入忙状态
                        base_logic_state <= BASE_LOGIC_BUSY;
                    end
                    BASE_LOGIC_BUSY: begin
                        //忙状态检测空闲,进入空闲状态,准备下一次脉冲发送
                        base_logic_start <= 1'b0;
                        //在这个状态下就可以进行下一步的步数切换了
                        if(1'b0 == base_logic_busy_state) begin
                            //进入空闲状态准备下一次脉冲发送
                            base_logic_state <= BASE_LOGIC_IDLE;
                        end
                        else begin
                            //保持不变
                            base_logic_state <= base_logic_state;
                        end
                    end
                endcase
            end
            STATE_TAB_SPD_CW_RUN: begin
                //步数表正向脉冲发送
                motor_out_dir = config_sensor_valid_level_reg[5];
                //根据基础状态决定操作
                case(base_logic_state)
                    BASE_LOGIC_IDLE: begin
                        if(1'b0 == control_reg[2]) begin
                            //没开预加载
                            if(base_logic_cnt[31:0] != spd_tab_logic_current_item_wire[31:0]) begin
                                //加载参数
                                base_logic_cnt[31:0] <= spd_tab_logic_current_item_wire[31:0];
                            end
                            else begin
                                //保持不变
                                base_logic_cnt[31:0] <= base_logic_cnt[31:0];
                            end
                            if(base_logic_cnt[31:0] != spd_tab_logic_current_item_wire[31:0]) begin
                                //数据不匹配,等下一次
                                base_logic_state <= base_logic_state;
                            end
                            else begin
                                //数据匹配,下一步
                                if(32'd0 == spd_tab_logic_current_item_wire[31:0]) begin
                                    //当前速度表输出信息为0,不能发送,等待状态机切换异常
                                    base_logic_state <= base_logic_state;
                                end
                                else if(32'd0 == stp_tab_logic_current_item_wire[31:0]) begin
                                    //当前步数表输出信息为0,不能发送,等待状态机切换异常
                                    base_logic_state <= base_logic_state;
                                end
                                else if(32'd0 == base_logic_cnt[31:0]) begin
                                    //加载的速度为0,状态保持
                                    base_logic_state <= base_logic_state;
                                end
                                else begin
                                    if(1'b0 == base_logic_busy_state)begin
                                        //发送启动脉冲
                                        base_logic_start <= 1'b1;
                                        //进入启动状态
                                        base_logic_state <= BASE_LOGIC_START;
                                        //记录上一次的速度值
                                        last_spd_record[31:0] <= base_logic_cnt[31:0];
                                    end
                                    else begin
                                        //状态不空闲,保持不变
                                        base_logic_state <= base_logic_state;
                                    end
                                end
                            end
                        end
                        else begin
                            //开了预加载
                            if((32'd0 == spd_tab_logic_current_item_wire[31:0])||(32'd0 == stp_tab_logic_current_item_wire[31:0])) begin
                                //速度表或者步数表任意一个为0,启用预加载功能
                                if(base_logic_cnt[31:0] != last_spd_record[31:0]) begin
                                    //加载参数
                                    base_logic_cnt[31:0] <= last_spd_record[31:0];
                                end
                                else begin
                                    //保持不变
                                    base_logic_cnt[31:0] <= base_logic_cnt[31:0];
                                end
                                if(base_logic_cnt[31:0] != last_spd_record[31:0]) begin
                                    //数据不匹配,等下一次
                                    base_logic_state <= base_logic_state;
                                end
                                else begin
                                    if(32'd0 == base_logic_cnt[31:0]) begin
                                        //加载的速度为0,状态保持
                                        base_logic_state <= base_logic_state;
                                    end
                                    else begin
                                        if(1'b0 == base_logic_busy_state)begin
                                            //发送启动脉冲
                                            base_logic_start <= 1'b1;
                                            //进入启动状态
                                            base_logic_state <= BASE_LOGIC_START;
                                            //记录上一次的速度值
                                            last_spd_record[31:0] <= base_logic_cnt[31:0];
                                        end
                                        else begin
                                            //状态不空闲,保持不变
                                            base_logic_state <= base_logic_state;
                                        end
                                    end
                                end
                            end
                            else begin
                                //速度表和步数表均不为0
                                if(base_logic_cnt[31:0] != spd_tab_logic_current_item_wire[31:0]) begin
                                    //加载参数
                                    base_logic_cnt[31:0] <= spd_tab_logic_current_item_wire[31:0];
                                end
                                else begin
                                    //保持不变
                                    base_logic_cnt[31:0] <= base_logic_cnt[31:0];
                                end
                                if(base_logic_cnt[31:0] != spd_tab_logic_current_item_wire[31:0]) begin
                                    //数据不匹配,等下一次
                                    base_logic_state <= base_logic_state;
                                end
                                else begin
                                    if(32'd0 == base_logic_cnt[31:0]) begin
                                        //加载的速度为0,状态保持
                                        base_logic_state <= base_logic_state;
                                    end
                                    else begin
                                        if(1'b0 == base_logic_busy_state)begin
                                            //发送启动脉冲
                                            base_logic_start <= 1'b1;
                                            //进入启动状态
                                            base_logic_state <= BASE_LOGIC_START;
                                            //记录上一次的速度值
                                            last_spd_record[31:0] <= base_logic_cnt[31:0];
                                        end
                                        else begin
                                            //状态不空闲,保持不变
                                            base_logic_state <= base_logic_state;
                                        end
                                    end
                                end
                            end
                        end
                    end//BASE_LOGIC_IDLE
                    BASE_LOGIC_START: begin
                        //启动状态下检测到busy高电平,进入忙状态
                        base_logic_start <= base_logic_start;
                        if(1'b1 == base_logic_busy_state) begin
                            base_logic_state <= BASE_LOGIC_NEXT_STEP;
                            //发送的脉冲值增加
                            state_motor_pluse_send_count_all_reg1 <= state_motor_pluse_send_count_all_reg1 + 1'b1;
                            //当前步数发送的脉冲值也增加
                            state_motor_pluse_send_current_spd_reg5 <= state_motor_pluse_send_current_spd_reg5 + 1'b1;
                        end
                        else begin
                            //保持不变
                            base_logic_state <= base_logic_state;
                        end
                    end
                    BASE_LOGIC_NEXT_STEP: begin
                        base_logic_start <= 1'b0;
                        //进入忙状态
                        base_logic_state <= BASE_LOGIC_BUSY;
                        //如果当前步数设置为0且开了预写入,那么暂时不增长步数序号
                        if((32'd0 == stp_tab_logic_current_item_wire[31:0])&&(1'b1 == control_reg[2])) begin
                            state_sensor_state_and_tab_index_reg3[31:16] <= state_sensor_state_and_tab_index_reg3[31:16];
                            state_motor_pluse_send_current_spd_reg5[31:0] <= state_motor_pluse_send_current_spd_reg5[31:0];
                        end
                        //如果当前速度下的步数已经达到步数表的上限步数
                        else if(state_motor_pluse_send_current_spd_reg5[31:0] >= stp_tab_logic_current_item_wire[31:0]) begin
                            //如果当前索引还没达到步数表的最大索引
                            if(state_sensor_state_and_tab_index_reg3[31:16] < param_tab_spd_index_start_stop_set_reg4[31:16])begin
                                //索引增加,进一步
                                state_sensor_state_and_tab_index_reg3[31:16] <= state_sensor_state_and_tab_index_reg3[31:16] + 1'b1;
                                //当前步数已经发出的脉冲清零
                                state_motor_pluse_send_current_spd_reg5[31:0] <= 32'd0;
                            end
                            else begin
                                //当前索引已经到达了步数表的最大索引,还停留在原来的位置
                                state_sensor_state_and_tab_index_reg3[31:16] <= state_sensor_state_and_tab_index_reg3[31:16];
                                state_motor_pluse_send_current_spd_reg5[31:0] <= state_motor_pluse_send_current_spd_reg5[31:0];
                            end
                        end
                        else begin
                            state_sensor_state_and_tab_index_reg3[31:16] <= state_sensor_state_and_tab_index_reg3[31:16];
                            state_motor_pluse_send_current_spd_reg5[31:0] <= state_motor_pluse_send_current_spd_reg5[31:0];
                        end
                    end
                    BASE_LOGIC_BUSY: begin
                        //忙状态检测空闲,进入空闲状态,准备下一次脉冲发送
                        base_logic_start <= 1'b0;
                        if(1'b0 == base_logic_busy_state) begin
                            //进入空闲状态准备下一次脉冲发送
                            base_logic_state <= BASE_LOGIC_IDLE;
                            //加载下一次需要输出的值
                            base_logic_cnt[31:0] <= spd_tab_logic_current_item_wire[31:0];
                        end
                        else begin
                            //保持不变
                            base_logic_state <= base_logic_state;
                        end
                    end
                endcase
            end
            STATE_TAB_SPD_CCW_RUN: begin
                //步数表反向脉冲发送
                motor_out_dir = (~config_sensor_valid_level_reg[5]);
                //根据基础状态决定操作
                case(base_logic_state)
                    BASE_LOGIC_IDLE: begin
                        if(1'b0 == control_reg[2]) begin
                            //没开预加载
                            if(base_logic_cnt[31:0] != spd_tab_logic_current_item_wire[31:0]) begin
                                //加载参数
                                base_logic_cnt[31:0] <= spd_tab_logic_current_item_wire[31:0];
                            end
                            else begin
                                //保持不变
                                base_logic_cnt[31:0] <= base_logic_cnt[31:0];
                            end
                            if(base_logic_cnt[31:0] != spd_tab_logic_current_item_wire[31:0]) begin
                                //数据不匹配,等下一次
                                base_logic_state <= base_logic_state;
                            end
                            else begin
                                //数据匹配,下一步
                                if(32'd0 == spd_tab_logic_current_item_wire[31:0]) begin
                                    //当前速度表输出信息为0,不能发送,等待状态机切换异常
                                    base_logic_state <= base_logic_state;
                                end
                                else if(32'd0 == stp_tab_logic_current_item_wire[31:0]) begin
                                    //当前步数表输出信息为0,不能发送,等待状态机切换异常
                                    base_logic_state <= base_logic_state;
                                end
                                else if(32'd0 == base_logic_cnt[31:0]) begin
                                    //加载的速度为0,状态保持
                                    base_logic_state <= base_logic_state;
                                end
                                else begin
                                    if(1'b0 == base_logic_busy_state)begin
                                        //发送启动脉冲
                                        base_logic_start <= 1'b1;
                                        //进入启动状态
                                        base_logic_state <= BASE_LOGIC_START;
                                        //记录上一次的速度值
                                        last_spd_record[31:0] <= base_logic_cnt[31:0];
                                    end
                                    else begin
                                        //状态不空闲,保持不变
                                        base_logic_state <= base_logic_state;
                                    end
                                end
                            end
                        end
                        else begin
                            //开了预加载
                            if((32'd0 == spd_tab_logic_current_item_wire[31:0])||(32'd0 == stp_tab_logic_current_item_wire[31:0])) begin
                                //速度表或者步数表任意一个为0,启用预加载功能
                                if(base_logic_cnt[31:0] != last_spd_record[31:0]) begin
                                    //加载参数
                                    base_logic_cnt[31:0] <= last_spd_record[31:0];
                                end
                                else begin
                                    //保持不变
                                    base_logic_cnt[31:0] <= base_logic_cnt[31:0];
                                end
                                if(base_logic_cnt[31:0] != last_spd_record[31:0]) begin
                                    //数据不匹配,等下一次
                                    base_logic_state <= base_logic_state;
                                end
                                else begin
                                    if(32'd0 == base_logic_cnt[31:0]) begin
                                        //加载的速度为0,状态保持
                                        base_logic_state <= base_logic_state;
                                    end
                                    else begin
                                        if(1'b0 == base_logic_busy_state)begin
                                            //发送启动脉冲
                                            base_logic_start <= 1'b1;
                                            //进入启动状态
                                            base_logic_state <= BASE_LOGIC_START;
                                            //记录上一次的速度值
                                            last_spd_record[31:0] <= base_logic_cnt[31:0];
                                        end
                                        else begin
                                            //状态不空闲,保持不变
                                            base_logic_state <= base_logic_state;
                                        end
                                    end
                                end
                            end
                            else begin
                                //速度表和步数表均不为0
                                if(base_logic_cnt[31:0] != spd_tab_logic_current_item_wire[31:0]) begin
                                    //加载参数
                                    base_logic_cnt[31:0] <= spd_tab_logic_current_item_wire[31:0];
                                end
                                else begin
                                    //保持不变
                                    base_logic_cnt[31:0] <= base_logic_cnt[31:0];
                                end
                                if(base_logic_cnt[31:0] != spd_tab_logic_current_item_wire[31:0]) begin
                                    //数据不匹配,等下一次
                                    base_logic_state <= base_logic_state;
                                end
                                else begin
                                    if(32'd0 == base_logic_cnt[31:0]) begin
                                        //加载的速度为0,状态保持
                                        base_logic_state <= base_logic_state;
                                    end
                                    else begin
                                        if(1'b0 == base_logic_busy_state)begin
                                            //发送启动脉冲
                                            base_logic_start <= 1'b1;
                                            //进入启动状态
                                            base_logic_state <= BASE_LOGIC_START;
                                            //记录上一次的速度值
                                            last_spd_record[31:0] <= base_logic_cnt[31:0];
                                        end
                                        else begin
                                            //状态不空闲,保持不变
                                            base_logic_state <= base_logic_state;
                                        end
                                    end
                                end
                            end
                        end
                    end//BASE_LOGIC_IDLE
                    BASE_LOGIC_START: begin
                        //启动状态下检测到busy高电平,进入忙状态
                        base_logic_start <= base_logic_start;
                        if(1'b1 == base_logic_busy_state) begin
                            base_logic_state <= BASE_LOGIC_NEXT_STEP;
                            //发送的脉冲值增加
                            state_motor_pluse_send_count_all_reg1 <= state_motor_pluse_send_count_all_reg1 + 1'b1;
                            //当前步数发送的脉冲值也增加
                            state_motor_pluse_send_current_spd_reg5 <= state_motor_pluse_send_current_spd_reg5 + 1'b1;
                        end
                        else begin
                            //保持不变
                            base_logic_state <= base_logic_state;
                        end
                    end
                    BASE_LOGIC_NEXT_STEP: begin
                        base_logic_start <= 1'b0;
                        //进入忙状态
                        base_logic_state <= BASE_LOGIC_BUSY;
                        //如果当前步数设置为0且开了预写入,那么暂时不增长步数序号
                        if((32'd0 == stp_tab_logic_current_item_wire[31:0])&&(1'b1 == control_reg[2])) begin
                            state_sensor_state_and_tab_index_reg3[31:16] <= state_sensor_state_and_tab_index_reg3[31:16];
                            state_motor_pluse_send_current_spd_reg5[31:0] <= state_motor_pluse_send_current_spd_reg5[31:0];
                        end
                        //如果当前速度下的步数已经达到步数表的上限步数
                        else if(state_motor_pluse_send_current_spd_reg5[31:0] >= stp_tab_logic_current_item_wire[31:0]) begin
                            //如果当前索引还没达到步数表的最大索引
                            if(state_sensor_state_and_tab_index_reg3[31:16] < param_tab_spd_index_start_stop_set_reg4[31:16])begin
                                //索引增加,进一步
                                state_sensor_state_and_tab_index_reg3[31:16] <= state_sensor_state_and_tab_index_reg3[31:16] + 1'b1;
                                //当前步数已经发出的脉冲清零
                                state_motor_pluse_send_current_spd_reg5[31:0] <= 32'd0;
                            end
                            else begin
                                //当前索引已经到达了步数表的最大索引,还停留在原来的位置
                                state_sensor_state_and_tab_index_reg3[31:16] <= state_sensor_state_and_tab_index_reg3[31:16];
                                state_motor_pluse_send_current_spd_reg5[31:0] <= state_motor_pluse_send_current_spd_reg5[31:0];
                            end
                        end
                        else begin
                            state_sensor_state_and_tab_index_reg3[31:16] <= state_sensor_state_and_tab_index_reg3[31:16];
                            state_motor_pluse_send_current_spd_reg5[31:0] <= state_motor_pluse_send_current_spd_reg5[31:0];
                        end
                    end
                    BASE_LOGIC_BUSY: begin
                        //忙状态检测空闲,进入空闲状态,准备下一次脉冲发送
                        base_logic_start <= 1'b0;
                        if(1'b0 == base_logic_busy_state) begin
                            //进入空闲状态准备下一次脉冲发送
                            base_logic_state <= BASE_LOGIC_IDLE;
                            //加载下一次需要输出的值
                            base_logic_cnt[31:0] <= spd_tab_logic_current_item_wire[31:0];
                        end
                        else begin
                            //保持不变
                            base_logic_state <= base_logic_state;
                        end
                    end
                endcase
            end
            default: begin
                //默认状态下的标记保持
                state_motor_pluse_send_count_all_reg1 <= state_motor_pluse_send_count_all_reg1;
                state_sensor_state_and_tab_index_reg3[31:16] <= state_sensor_state_and_tab_index_reg3[31:16];
                state_motor_pluse_send_current_spd_reg5 <= state_motor_pluse_send_current_spd_reg5;
                //启动信号清除
                base_logic_start <= 1'b0;
                //速度计数值清除
                base_logic_cnt <= 32'd0;
                //基础状态为空闲
                base_logic_state <= BASE_LOGIC_IDLE;
                //上次的速度
                last_spd_record <= 32'd0;
            end
        endcase
    end
end



endmodule






