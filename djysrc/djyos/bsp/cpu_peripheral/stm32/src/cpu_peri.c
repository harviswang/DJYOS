//----------------------------------------------------
//Copyright (C), 2004-2009,  luoshitian.
//版权所有 (C), 2004-2009,   罗侍田.
//所属模块: CPU外设定义
//作者:  罗侍田.
//版本：V1.0.0
//文件描述: 包含CPU所有外设的定义
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2009-01-04
//   作者:  罗侍田.
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------

//dma相关寄存器
struct st_dma_reg volatile * const pg_dma1_reg
                        = (struct st_dma_reg *)0x40020000;
struct st_dma_reg volatile * const pg_dma2_reg
                        = (struct st_dma_reg *)0x40020400;

//dma1的各通道寄存器
struct st_dma_channel_reg volatile * const pg_dma1_channel1_reg
                        = (struct st_dma_channel_reg *)0x40020008;
struct st_dma_channel_reg volatile * const pg_dma1_channel2_reg
                        = (struct st_dma_channel_reg *)0x4002001c;
struct st_dma_channel_reg volatile * const pg_dma1_channel3_reg
                        = (struct st_dma_channel_reg *)0x40020030;
struct st_dma_channel_reg volatile * const pg_dma1_channel4_reg
                        = (struct st_dma_channel_reg *)0x40020044;
struct st_dma_channel_reg volatile * const pg_dma1_channel5_reg
                        = (struct st_dma_channel_reg *)0x40020058;
struct st_dma_channel_reg volatile * const pg_dma1_channel6_reg
                        = (struct st_dma_channel_reg *)0x4002006c;
struct st_dma_channel_reg volatile * const pg_dma1_channel7_reg
                        = (struct st_dma_channel_reg *)0x40020080;
//dma2的各通道寄存器
struct st_dma_channel_reg volatile * const pg_dma2_channel1_reg
                        = (struct st_dma_channel_reg *)0x40020408;
struct st_dma_channel_reg volatile * const pg_dma2_channel2_reg
                        = (struct st_dma_channel_reg *)0x4002041c;
struct st_dma_channel_reg volatile * const pg_dma2_channel3_reg
                        = (struct st_dma_channel_reg *)0x40020430;
struct st_dma_channel_reg volatile * const pg_dma2_channel4_reg
                        = (struct st_dma_channel_reg *)0x40020444;
struct st_dma_channel_reg volatile * const pg_dma2_channel5_reg
                        = (struct st_dma_channel_reg *)0x40020458;

//gpio相关寄存器定义
struct st_gpio_reg volatile * const pg_gpio_rega
                        = (struct st_gpio_reg *)0x40010800;
struct st_gpio_reg volatile * const pg_gpio_regb
                        = (struct st_gpio_reg *)0x40010c00;
struct st_gpio_reg volatile * const pg_gpio_regc
                        = (struct st_gpio_reg *)0x40011000;
struct st_gpio_reg volatile * const pg_gpio_regd
                        = (struct st_gpio_reg *)0x40011400;
struct st_gpio_reg volatile * const pg_gpio_rege
                        = (struct st_gpio_reg *)0x40011800;
struct st_gpio_reg volatile * const pg_gpio_regf
                        = (struct st_gpio_reg *)0x40011c00;
struct st_gpio_reg volatile * const pg_gpio_regg
                        = (struct st_gpio_reg *)0x40012000;

struct st_afio_reg volatile * const pg_afio_reg
                        = (struct st_afio_reg *)0x40010000;
struct st_gpio_reg volatile * const pg_gpio_reg [7] = {
                                (struct st_gpio_reg *)0x40010800,
                                (struct st_gpio_reg *)0x40010c00,
                                (struct st_gpio_reg *)0x40011000,
                                (struct st_gpio_reg *)0x40011400,
                                (struct st_gpio_reg *)0x40011800,
                                (struct st_gpio_reg *)0x40011c00,
                                (struct st_gpio_reg *)0x40012000,
                               };

//串口相关寄存器
struct st_usart_reg volatile * const pg_uart1_reg
                        = (struct st_usart_reg *)0x40013800;
struct st_usart_reg volatile * const pg_uart2_reg
                        = (struct st_usart_reg *)0x40004400;
struct st_usart_reg volatile * const pg_uart3_reg
                        = (struct st_usart_reg *)0x40004800;
struct st_usart_reg volatile * const pg_uart4_reg
                        = (struct st_usart_reg *)0x40004c00;
struct st_usart_reg volatile * const pg_uart5_reg
                        = (struct st_usart_reg *)0x40005000;

//rcc(复位和时钟控制寄存器)
struct st_rcc_reg volatile * const pg_rcc_reg  = (struct st_rcc_reg *)0x40021000;

//片内flash控制寄存器
struct st_inflash_fpec_reg volatile * const pg_inflash_fpec_reg  
                                = (struct st_inflash_fpec_reg *)0x40022000;
struct st_inflash_ob_reg volatile * const pg_inflash_ob_reg  
                                = (struct st_inflash_ob_reg *)0x1FFFF800;

