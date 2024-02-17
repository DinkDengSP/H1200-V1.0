/************************************************************************ 
 ** Author       : 邓小俊
 ** Date         : 2024-01-19 13:30:05
 ** Description  : When I Has Free Time ,I Will Write Description Below.
 ** description  : 
 ** LastEditors  : 邓小俊
 ** LastEditTime : 2024-01-19 13:57:44
 ** Email: xiaojun.deng@aliyun.com
 ** Copyright (c) 2024 by 公司名称, All Rights Reserved. 
 ************************************************************************/
//LED控制电路,单路
module led_flashing 
#(
    parameter       CLK_FREQ            = 32'd50_000_000;//系统时钟参数定义
    parameter       LED_VALID_LEVEL     = 1'b0;          //LED有效电平
    parameter       LED_FLASH_PERIOD_MS = 16'd199        //LED闪烁周期
)
(
    input wire      sys_clk ,      //系统时钟
    input wire      sys_rst_n ,    //全局复位
    input wire      flash_enable,  //是否打开闪烁
    output reg      led_out        //输出信号，控制led灯
);

/********************************参数定义****************************************/
//LED无效信号
localparam LED_INVALID_LEVEL = ~(LED_VALID_LEVEL);
//1MS的计数时间
localparam CLK_COUNT_ONE_MS    = ((CLK_FREQ/1000) - 1); 
//LED切换的时间点
localparam LED_FLASH_TOOGLE_TIME_MS = (LED_FLASH_PERIOD_MS/2);
//状态机相关变量
localparam LED_FLASHING_STATE_IDLE  = 3'b001;
localparam LED_FLASHING_STATE_OFF   = 3'b010;
localparam LED_FLASHING_STATE_ON    = 3'b100;
/*************************************************************************************/    
/**********************************网线定义********************************************/
/*************************************************************************************/



/*************************************************************************************/
/*********************************寄存器定义*******************************************/
/*************************************************************************************/
//捕获使能信号
reg[1:0] flash_enable_reg;
//当前状态变量
reg[2:0] led_flashing_state_current;

//ms计数单元
reg[19:0] clk_count;
reg[15:0] ms_count;

/*************************************************************************************/
/**********************************组合逻辑代码****************************************/
/*************************************************************************************/



/*************************************************************************************/
/**********************************时序逻辑代码****************************************/
/*************************************************************************************/
//使能信号设定
always @(posedge sys_clk or negedge sys_rst_n) 
begin
    if(1'b0 == sys_rst_n)begin
        flash_enable_reg <= 2'b00;
    end
    else begin
        flash_enable_reg <= {flash_enable_reg[0],flash_enable};
    end
end

//时钟计数器
always @(posedge sys_clk or negedge sys_rst_n) 
begin
    if(1'b0 == sys_rst_n)begin
        clk_count <= 20'd0;
    end
    else if(LED_FLASHING_STATE_IDLE == led_flashing_state_current) begin
        //空闲状态不计数
        clk_count <= 20'd0;
    end
    else begin
        if(clk_count == CLK_COUNT_ONE_MS) begin
            clk_count <= 20'd0;
        end
        else begin
            clk_count <= clk_count + 1'b1;
        end
    end
end

//ms计数器
always @(posedge sys_clk or negedge sys_rst_n) 
begin
    if(1'b0 == sys_rst_n)begin
        ms_count <= 16'd0;
    end
    else if(LED_FLASHING_STATE_IDLE == led_flashing_state_current) begin
        //空闲状态不计数
        ms_count <= 16'd0;
    end
    else begin
        if((ms_count == LED_FLASH_PERIOD_MS)&&(clk_count == CLK_COUNT_ONE_MS)) begin
            //周期计数清零
            ms_count <= 16'd0;
        end
        else if(clk_count == CLK_COUNT_ONE_MS) begin
            ms_count <= ms_count + 1'b1;
        end
        else begin
            ms_count <= ms_count;
        end
    end
end


//第一段状态机，描述当前状态state如何根据输入跳转到下一状态
always@(posedge sys_clk or negedge sys_rst_n)
begin
    if(1'b0 == sys_rst_n ) begin
        //任何情况下只要按复位就回到初始状态
        led_flashing_state_current <= LED_FLASHING_STATE_IDLE;
    end
    else begin
        case(led_flashing_state_current)
            LED_FLASHING_STATE_IDLE : begin
                //空闲状态的转换条件
                if(2'b01 == flash_enable_reg) begin
                    //接收到使能,转入STOP模式
                    led_flashing_state_current <= LED_FLASHING_STATE_OFF;
                end
                else begin
                    //状态保持
                    led_flashing_state_current <= led_flashing_state_current;
                end
            end 
            LED_FLASHING_STATE_OFF : begin
                //关闭状态下的转换条件
                if(2'b00 == flash_enable_reg) begin
                    //接收到停止,转入空闲状态
                    led_flashing_state_current <= LED_FLASHING_STATE_IDLE;
                end
                else if(ms_count == LED_FLASH_TOOGLE_TIME_MS) begin
                    //时间达到周期的一半,转入打开
                    led_flashing_state_current <= LED_FLASHING_STATE_ON;
                end
                else begin
                    //状态保持
                    led_flashing_state_current <= led_flashing_state_current;
                end
            end
            LED_FLASHING_STATE_ON : begin
                //打开状态下的转换条件
                if(2'b00 == flash_enable_reg) begin
                    //接收到停止,转入空闲状态
                    led_flashing_state_current <= LED_FLASHING_STATE_IDLE;
                end
                else if(ms_count == LED_FLASH_PERIOD_MS) begin
                    //时间达到周期的,转入关闭
                    led_flashing_state_current <= LED_FLASHING_STATE_OFF;
                end
                else begin
                    //状态保持
                    led_flashing_state_current <= led_flashing_state_current;
                end
            end
            //如果状态机跳转到编码的状态之外也回到初始状态
            default: begin
                //状态保持
                led_flashing_state_current <= led_flashing_state_current;
            end
        endcase
    end
end

//第二段状态机，描述当前状态state和输入pi_money如何影响po_cola输出
always@(posedge sys_clk or negedge sys_rst_n)
begin
    if(1'b0 == sys_rst_n ) begin
        //初始化关闭LED
        led_out <= LED_INVALID_LEVEL;
    end
    else begin
        if(led_flashing_state_current == LED_FLASHING_STATE_IDLE) begin
            //LED关闭
            led_out <= LED_INVALID_LEVEL;
        end
        else if(led_flashing_state_current == LED_FLASHING_STATE_OFF) begin
            //LED关闭
            led_out <= LED_INVALID_LEVEL;
        end
        else begin
            //LED打开
            led_out <= LED_VALID_LEVEL;
        end
    end
end



endmodule






