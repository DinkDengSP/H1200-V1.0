/************************************************************************ 
 ** Author       : 邓小俊
 ** Date         : 2024-01-19 13:30:05
 ** Description  : When I Has Free Time ,I Will Write Description Below.
 ** description  : 
 ** LastEditors  : 邓小俊
 ** LastEditTime : 2024-01-19 14:15:29
 ** Email: xiaojun.deng@aliyun.com
 ** Copyright (c) 2024 by 公司名称, All Rights Reserved. 
 ************************************************************************/
module gpio_port_in 
#(
    parameter SYS_CLK_FREQ                      = 32'd50_000_000,//输入时钟
    parameter GPIO_PORT_IN_MODE_REG_INIT_VAL    = 32'hffff_ffff,//端口模式寄存器的初始值
    parameter GPIO_PORT_IN_SAMPLE_REG_INIT_VAL  = 32'h0000_0000//端口状态寄存器的初始值
)
(
    //通用时钟复位
    input           sys_clk  ,      //输入时钟
    input           sys_rst_n,      //复位,低电平有效
    //输入的写信号
    input           write_single,   //写入信号
    input [3:0]     write_addr,     //写入地址
    input [31:0]    write_data,     //写入数据
    //输出的寄存器,寄存器有两个,
    //一个是写入的输入模式寄存器,决定是否对输入信号进行翻转
    //另一个是输出的当前状态寄存器,输入信号经过模式处理的信号
    output [((2*32)-1):0]   readable_reg_extern,    //外部可读寄存器
    //输入端口
    input[31:0] port_really_in                      //输入端口
);

/*************************************************************************************/
/*********************************参数定义*********************************************/
/*************************************************************************************/
//地址,写入端口模式寄存器
localparam REG_WRITE_ADDR_GPIO_PORT_IN_MODE = 4'h0;


/*************************************************************************************/    
/**********************************网线定义********************************************/
/*************************************************************************************/


/*************************************************************************************/
/*********************************寄存器定义*******************************************/
/*************************************************************************************/
//延时信号节拍进行采样
reg [2:0]write_singal_reg;
//记录输入的地址
reg[3:0] local_write_addr_reg;
reg[31:0] local_write_data_reg;

//端口当前配置寄存器
reg[31:0] local_port_mode_reg;
//端口当前状态寄存器
reg[31:0] local_port_state_reg;

/*************************************************************************************/
/**********************************组合逻辑代码****************************************/
/*************************************************************************************/
//组合内部寄存器信号输出
assign readable_reg_extern = {local_port_state_reg,local_port_mode_reg};

/*************************************************************************************/
/**********************************时序逻辑代码****************************************/
/*************************************************************************************/
//对外部发送过来的写信号做锁存
always @(posedge sys_clk or negedge sys_rst_n) 
begin 
    if (1'b0 == sys_rst_n) 
    begin 
        write_singal_reg <= 3'b000;
        local_write_addr_reg <= 4'd0;
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

//对写入信号做处理,写入到相应的寄存器
always @(posedge sys_clk or negedge sys_rst_n) 
begin 
    if (1'b0 == sys_rst_n) 
    begin 
        local_port_mode_reg <= GPIO_PORT_IN_MODE_REG_INIT_VAL;
    end
    else if(3'b011 == write_singal_reg) begin
        case(local_write_addr_reg)
            REG_WRITE_ADDR_GPIO_PORT_IN_MODE: begin
                //写入模式寄存器
                local_port_mode_reg <= local_write_data_reg;
            end
            default : begin
                //不知道做什么
            end
        endcase
    end
    else begin
        //不知道做什么
    end
end

//对输入信号进行采样
always @(posedge sys_clk or negedge sys_rst_n) 
begin 
    if (1'b0 == sys_rst_n) 
    begin 
        //设置采样寄存器初始值
        local_port_state_reg <= GPIO_PORT_IN_SAMPLE_REG_INIT_VAL;
    end
    else begin
        //输入采样,当模式寄存器为0,输入什么电平就显示什么值,
        //当模式寄存器为1,电平反转,输入电平0,寄存器写1,相当于设置有效电平
        //输入端口0
        if(1'b1 == local_port_mode_reg[0]) begin
            local_port_state_reg[0] <= port_really_in[0];
        end
        else begin
            local_port_state_reg[0] <= ~(port_really_in[0]);
        end

        //输入端口1
        if(1'b1 == local_port_mode_reg[1]) begin
            local_port_state_reg[1] <= port_really_in[1];
        end
        else begin
            local_port_state_reg[1] <= ~(port_really_in[1]);
        end
        
        //输入端口2
        if(1'b1 == local_port_mode_reg[2]) begin
            local_port_state_reg[2] <= port_really_in[2];
        end
        else begin
            local_port_state_reg[2] <= ~(port_really_in[2]);
        end
        
        //输入端口3
        if(1'b1 == local_port_mode_reg[3]) begin
            local_port_state_reg[3] <= port_really_in[3];
        end
        else begin
            local_port_state_reg[3] <= ~(port_really_in[3]);
        end
        
        //输入端口4
        if(1'b1 == local_port_mode_reg[4]) begin
            local_port_state_reg[4] <= port_really_in[4];
        end
        else begin
            local_port_state_reg[4] <= ~(port_really_in[4]);
        end
        
        //输入端口5
        if(1'b1 == local_port_mode_reg[5]) begin
            local_port_state_reg[5] <= port_really_in[5];
        end
        else begin
            local_port_state_reg[5] <= ~(port_really_in[5]);
        end
        
        //输入端口6
        if(1'b1 == local_port_mode_reg[6]) begin
            local_port_state_reg[6] <= port_really_in[6];
        end
        else begin
            local_port_state_reg[6] <= ~(port_really_in[6]);
        end
        
        //输入端口7
        if(1'b1 == local_port_mode_reg[7]) begin
            local_port_state_reg[7] <= port_really_in[7];
        end
        else begin
            local_port_state_reg[7] <= ~(port_really_in[7]);
        end
        
        //输入端口8
        if(1'b1 == local_port_mode_reg[8]) begin
            local_port_state_reg[8] <= port_really_in[8];
        end
        else begin
            local_port_state_reg[8] <= ~(port_really_in[8]);
        end
        
        //输入端口9
        if(1'b1 == local_port_mode_reg[9]) begin
            local_port_state_reg[9] <= port_really_in[9];
        end
        else begin
            local_port_state_reg[9] <= ~(port_really_in[9]);
        end
        
        //输入端口10
        if(1'b1 == local_port_mode_reg[10]) begin
            local_port_state_reg[10] <= port_really_in[10];
        end
        else begin
            local_port_state_reg[10] <= ~(port_really_in[10]);
        end
        
        //输入端口11
        if(1'b1 == local_port_mode_reg[11]) begin
            local_port_state_reg[11] <= port_really_in[11];
        end
        else begin
            local_port_state_reg[11] <= ~(port_really_in[11]);
        end
        
        //输入端口12
        if(1'b1 == local_port_mode_reg[12]) begin
            local_port_state_reg[12] <= port_really_in[12];
        end
        else begin
            local_port_state_reg[12] <= ~(port_really_in[12]);
        end
        
        //输入端口13
        if(1'b1 == local_port_mode_reg[13]) begin
            local_port_state_reg[13] <= port_really_in[13];
        end
        else begin
            local_port_state_reg[13] <= ~(port_really_in[13]);
        end

        
        //输入端口14
        if(1'b1 == local_port_mode_reg[14]) begin
            local_port_state_reg[14] <= port_really_in[14];
        end
        else begin
            local_port_state_reg[14] <= ~(port_really_in[14]);
        end
        
        //输入端口15
        if(1'b1 == local_port_mode_reg[15]) begin
            local_port_state_reg[15] <= port_really_in[15];
        end
        else begin
            local_port_state_reg[15] <= ~(port_really_in[15]);
        end
        
        //输入端口16
        if(1'b1 == local_port_mode_reg[16]) begin
            local_port_state_reg[16] <= port_really_in[16];
        end
        else begin
            local_port_state_reg[16] <= ~(port_really_in[16]);
        end
        
        //输入端口17
        if(1'b1 == local_port_mode_reg[17]) begin
            local_port_state_reg[17] <= port_really_in[17];
        end
        else begin
            local_port_state_reg[17] <= ~(port_really_in[17]);
        end
        
        //输入端口18
        if(1'b1 == local_port_mode_reg[18]) begin
            local_port_state_reg[18] <= port_really_in[18];
        end
        else begin
            local_port_state_reg[18] <= ~(port_really_in[18]);
        end
        
        //输入端口19
        if(1'b1 == local_port_mode_reg[19]) begin
            local_port_state_reg[19] <= port_really_in[19];
        end
        else begin
            local_port_state_reg[19] <= ~(port_really_in[19]);
        end
        
        //输入端口20
        if(1'b1 == local_port_mode_reg[20]) begin
            local_port_state_reg[20] <= port_really_in[20];
        end
        else begin
            local_port_state_reg[20] <= ~(port_really_in[20]);
        end
        
        //输入端口21
        if(1'b1 == local_port_mode_reg[21]) begin
            local_port_state_reg[21] <= port_really_in[21];
        end
        else begin
            local_port_state_reg[21] <= ~(port_really_in[21]);
        end
        
        //输入端口22
        if(1'b1 == local_port_mode_reg[22]) begin
            local_port_state_reg[22] <= port_really_in[22];
        end
        else begin
            local_port_state_reg[22] <= ~(port_really_in[22]);
        end
        
        //输入端口23
        if(1'b1 == local_port_mode_reg[23]) begin
            local_port_state_reg[23] <= port_really_in[23];
        end
        else begin
            local_port_state_reg[23] <= ~(port_really_in[23]);
        end
        
        //输入端口24
        if(1'b1 == local_port_mode_reg[24]) begin
            local_port_state_reg[24] <= port_really_in[24];
        end
        else begin
            local_port_state_reg[24] <= ~(port_really_in[24]);
        end
        
        //输入端口25
        if(1'b1 == local_port_mode_reg[25]) begin
            local_port_state_reg[25] <= port_really_in[25];
        end
        else begin
            local_port_state_reg[25] <= ~(port_really_in[25]);
        end
        
        //输入端口26
        if(1'b1 == local_port_mode_reg[26]) begin
            local_port_state_reg[26] <= port_really_in[26];
        end
        else begin
            local_port_state_reg[26] <= ~(port_really_in[26]);
        end
        
        //输入端口27
        if(1'b1 == local_port_mode_reg[27]) begin
            local_port_state_reg[27] <= port_really_in[27];
        end
        else begin
            local_port_state_reg[27] <= ~(port_really_in[27]);
        end
        
        //输入端口28
        if(1'b1 == local_port_mode_reg[28]) begin
            local_port_state_reg[28] <= port_really_in[28];
        end
        else begin
            local_port_state_reg[28] <= ~(port_really_in[28]);
        end
        
        //输入端口29
        if(1'b1 == local_port_mode_reg[29]) begin
            local_port_state_reg[29] <= port_really_in[29];
        end
        else begin
            local_port_state_reg[29] <= ~(port_really_in[29]);
        end
        
        //输入端口30
        if(1'b1 == local_port_mode_reg[30]) begin
            local_port_state_reg[30] <= port_really_in[30];
        end
        else begin
            local_port_state_reg[30] <= ~(port_really_in[30]);
        end

        //输入端口31
        if(1'b1 == local_port_mode_reg[31]) begin
            local_port_state_reg[31] <= port_really_in[31];
        end
        else begin
            local_port_state_reg[31] <= ~(port_really_in[31]);
        end
    end
end

    
endmodule








