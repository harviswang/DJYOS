/*========================================================*
 * 文件模块说明:                                                 *
 * omapl138的uart驱动，移植自都江堰操作系统作者拥有版权
 * 的S3C2440 uart驱动文件，针对omapl138做了一些修改
 * 文件版本: v1.00                                               *
 * 开发人员: lst                              *
 * 创建时间: 2011.06.01                                          *
 * Copyright(c) 2011-2011  深圳鹏瑞软件有限公司             *
 *========================================================*
 * 程序修改记录(最新的放在最前面):							     *
 *  <版本号> <修改日期>, <修改人员>: <修改功能概述>				 *
 *========================================================*
* 例如：
*  V1.02 2004-07-14, 张三: 接收函数增加对报文长度有效性校核
*
*/

//以下是原作者版权声明
//----------------------------------------------------
//Copyright (C), 2004-2009,  都江堰操作系统开发团队
//版权所有 (C), 2004-2009,   都江堰操作系统开发团队
//所属模块: uart驱动程序
//作者:  罗侍田.
//版本：V1.0.0
//文件描述: 
//其他说明:
//修订历史:
//1. 日期:2009-03-10
//   作者:  罗侍田.
//   新版本号：1.0.0
//   修改说明: 移植字44b0的1.0.1版
//------------------------------------------------------
#include <string.h>
#include "os_inc.h"
#include "cpu_peri.h"

ptu32_t uart_ctrl(struct  pan_device *uart_dev,u32 cmd,
                   ptu32_t data1,ptu32_t data2);
ptu32_t uart_left_write(struct  pan_device *uart_dev,ptu32_t src_buf,
                            ptu32_t res,ptu32_t len);
ptu32_t uart_right_write(struct  pan_device *uart_dev,ptu32_t src_buf,
                            ptu32_t res,ptu32_t len);
ptu32_t uart_left_read(struct  pan_device *uart_dev,ptu32_t src_buf,
                            ptu32_t res,ptu32_t len);
ptu32_t uart_right_read(struct  pan_device *uart_dev,ptu32_t src_buf,
                            ptu32_t res,ptu32_t len);
u32  uart0_int(ufast_t uart_int_line);
u32  uart1_int(ufast_t uart_int_line);
u32  uart2_int(ufast_t uart_int_line);
u32  uart3_int(ufast_t uart_int_line);
void uart0_error_service(void);
void uart1_error_service(void);
void uart2_error_service(void);
void uart3_error_service(void);
void uart_send_service(void);

extern struct st_int_reg volatile * const pg_int_reg;

static djy_handle_t pg_uart0_rhdl;
static djy_handle_t pg_uart1_rhdl;
static djy_handle_t pg_uart2_rhdl;
static struct uart_UCB tg_uart0_CB,tg_uart1_CB,tg_uart2_CB;

uint16_t u16g_evtt_uart0_error;
uint16_t u16g_evtt_uart1_error;
uint16_t u16g_evtt_uart2_error;

uint8_t uart0_ptcl_send_buf[uart0_buf_len];
uint8_t uart1_ptcl_send_buf[uart1_buf_len];
uint8_t uart2_ptcl_send_buf[uart2_buf_len];
uint8_t uart0_ptcl_recv_buf[uart0_buf_len];
uint8_t uart1_ptcl_recv_buf[uart1_buf_len];
uint8_t uart2_ptcl_recv_buf[uart2_buf_len];

//----使能接收中断-------------------------------------------------------------
//功能: 使能uart的接收中断。
//参数: reg,被操作的寄存器组指针
//返回: 无
//-----------------------------------------------------------------------------
void __uart_recv_int_enable(tag_UartReg volatile *reg)
{
    reg->IER |= (u32)1;
}

//----禁止接收中断-------------------------------------------------------------
//功能: 禁止uart的接收中断。
//参数: reg,被操作的寄存器组指针
//返回: 无
//-----------------------------------------------------------------------------
void __uart_recv_int_disable(tag_UartReg volatile *reg)
{
    reg->IER &= ~(u32)1;
}

//----使能发送中断-------------------------------------------------------------
//功能: 使能uart的发送中断。
//参数: reg,被操作的寄存器组指针
//返回: 无
//-----------------------------------------------------------------------------
void __uart_send_int_enable(tag_UartReg volatile *reg)
{
    reg->IER |= (u32)2;
}

//----禁止发送中断-------------------------------------------------------------
//功能: 禁止uart的发送中断。
//参数: reg,被操作的寄存器组指针
//返回: 无
//-----------------------------------------------------------------------------
void __uart_send_int_disable(tag_UartReg volatile *reg)
{
    reg->IER &= ~(u32)2;
}

//----设置baud-----------------------------------------------------------------
//功能: 设置串口baud
//参数: reg,被操作的寄存器组指针
//返回: 无
//-----------------------------------------------------------------------------
void __uart_set_baud(tag_UartReg volatile *reg,u32 baud)
{
    u32 temp;
    if((u32)reg == cn_uart0_base)       //串口0,不受ASYNC3_CLKSRC控制
    {
        temp = (u32)(cn_pll0_sysclk2+(baud>>1))/(baud*16);
        reg->DLL = (u8)temp;
        reg->DLH = (u8)(temp>>8);
    }else                               //串口1和串口2
    {
        if(g_ptSysconfig0Reg->CFGCHIP[3] & 0x10)   //test ASYNC3_CLKSRC bit
        {
            temp = (u32)(cn_pll1_sysclk2+(baud>>1))/(baud*16);
            reg->DLL = (u8)temp;
            reg->DLH = (u8)(temp>>8);
        }else
        {
            temp = (u32)(cn_pll0_sysclk2+(baud>>1))/(baud*16);
            reg->DLL = (u8)temp;
            reg->DLH = (u8)(temp>>8);
        }
    }
    
}
//----检查串口发送寄存器是否空-------------------------------------------------
//功能: 检查发送寄存器的状态，如果空就返回true
//参数：无。
//返回: true = 缓冲区空，false = 非空
//-----------------------------------------------------------------------------
bool_t uart_tx_tran_empty(volatile tag_UartReg *uart_reg)
{
    return (0 != (uart_reg->LSR & 0x40));
}

//----初始化uart0模块----------------------------------------------------------
//功能：初始化uart0模块
//参数：模块初始化函数没有参数
//返回：1=成功，0=失败
//-----------------------------------------------------------------------------
ptu32_t module_init_uart0(ptu32_t para)
{
    struct semaphore_LCB *left_semp,*right_semp;
    struct  pan_device   *uart_dev;
    uint16_t uart_send_evtt;
    vu32 temp;
    //保护缓冲区的信号量，使缓冲区中数据量为0时阻塞写入线程，读取线程使缓冲区中
    //数据降至trigger_level以下时释放信号量，使写入线程解除阻塞
    tg_uart0_CB.send_buf_semp = semp_create(1,0,"uart0 tx buf");
    if(tg_uart0_CB.send_buf_semp == NULL)
        goto exit_from_left_buf_semp;
    tg_uart0_CB.recv_buf_semp = semp_create(1,0,"uart0 rx buf");
    if(tg_uart0_CB.recv_buf_semp == NULL)
        goto exit_from_right_buf_semp;
    //分别创建左手和右手访问uart0设备的信号量，第一个参数为1表示串口是独占访问
    //的，第二个参数1表示初始状态有一盏信号灯点亮。左手由应用程序使用，右手由硬
    //件操作函数使用。
    left_semp = semp_create(1,1,"uart0 device left");
    if(left_semp == NULL)
        goto exit_from_left_semp;
    right_semp = semp_create(1,1,"uart0 device right");
    if(right_semp == NULL)
        goto exit_from_right_semp;

    Cpucfg_EnableLPSC(cn_PSC0_UART0);
    gpio_cfg_pin_func(8,3,cn_p8_3_uart0_txd);
    gpio_cfg_pin_func(8,4,cn_p8_4_uart0_rxd);
    //系统初始化时已经使中断处于禁止状态，无需再禁止和清除中断。
    //初始化uart硬件控制数据结构
    tg_uart0_CB.my_reg = (tag_UartReg *)cn_uart0_base;
    __uart_set_baud(tg_uart0_CB.my_reg,9600);
    temp = tg_uart0_CB.my_reg->IIR_FCR;     //读一下清除所有中断标志
//    tg_uart0_CB.my_reg->IIR_FCR |= 1;   //set FIFOEN
    tg_uart0_CB.my_reg->IIR_FCR = 0xcf; //接收14字节触发中断，清收发fifo。
    tg_uart0_CB.my_reg->LCR = 3 | (0<<2) | (000<<3) | (0<<6) | (0<<7);
    tg_uart0_CB.my_reg->MCR = 0;
    tg_uart0_CB.my_reg->PWREMU_MGMT = 0x7fff;
    tg_uart0_CB.my_reg->MDR = 0;        //16倍时钟
    tg_uart0_CB.my_reg->IER =  3;       //使能收发中断，此时中断线控制尚未使能
    tg_uart0_CB.baud = 9600;

    //初始化通用串口数据结构
    tg_uart0_CB.recv_trigger_level = 0;  //缓冲接收触发水平为0
    tg_uart0_CB.send_trigger_level = uart0_buf_len>>4;  //缓冲发送触发水平为1/16
    tg_uart0_CB.timeout = (u32)500*mS;  //超时时间=500
    tg_uart0_CB.serial_no = 0;  //串口号是0
    ring_init(  &tg_uart0_CB.send_ring_buf,
                uart0_ptcl_send_buf,
                uart0_buf_len);
    ring_init(  &tg_uart0_CB.recv_ring_buf,
                uart0_ptcl_recv_buf,
                uart0_buf_len);
    tg_uart0_CB.status = cn_ser_rxbuf_empty;

    //以下建立uart0设备
    uart_dev = dev_add_device(dev_get_root_device(),"uart0",
                               right_semp,left_semp,
                               (dev_write_func) uart_right_write,
                               (dev_read_func ) uart_right_read,
                               (dev_ctrl_func ) uart_ctrl,
                               (dev_write_func ) uart_left_write,
                               (dev_read_func  ) uart_left_read,
                               (dev_ctrl_func  ) uart_ctrl,
                               (ptu32_t)(&tg_uart0_CB)  //uart0设备的私有数据
                               );
    if(uart_dev == NULL)
        goto exit_from_add_device;
    pg_uart0_rhdl = dev_open_right("uart0",0);      //打开右手句柄
    u16g_evtt_uart0_error = djy_evtt_regist(
                           enum_correlative,cn_prio_real,0,5,
                           uart0_error_service,1024,NULL);
    if(u16g_evtt_uart0_error == cn_invalid_evtt_id)
        goto exit_from_error_evtt;
    uart_send_evtt = djy_evtt_regist(enum_correlative,cn_prio_real,0,
                                   5,uart_send_service,20,NULL);
    if(uart_send_evtt == cn_invalid_evtt_id)
        goto exit_from_send_evtt;
    dev_ctrl_right(pg_uart0_rhdl,cn_uart_connect_send_evtt,uart_send_evtt,0);
    int_isr_connect(cn_int_line_uart0,uart0_int);
    int_setto_asyn_signal(cn_int_line_uart0);
    int_echo_line(cn_int_line_uart0);       //清掉初始化产生的发送fifo空的中断
    int_restore_asyn_line(cn_int_line_uart0);
    return 1;
exit_from_send_evtt:
    djy_evtt_unregist(u16g_evtt_uart0_error);
exit_from_error_evtt:
    dev_delete_device(uart_dev);
exit_from_add_device:
    semp_delete(right_semp);
exit_from_right_semp:
    semp_delete(left_semp);
exit_from_left_semp:
    semp_delete(tg_uart0_CB.recv_buf_semp);
exit_from_right_buf_semp:
    semp_delete(tg_uart0_CB.send_buf_semp);
exit_from_left_buf_semp:
    return 0;
}

//----初始化uart1模块----------------------------------------------------------
//功能：初始化uart1模块
//参数：模块初始化函数没有参数
//返回：1=成功，0=失败
//-----------------------------------------------------------------------------
ptu32_t module_init_uart1(ptu32_t para)
{
    struct semaphore_LCB *left_semp,*right_semp;
    struct  pan_device   *uart_dev;
    uint16_t uart_send_evtt;
    vu32 temp;
    //保护缓冲区的信号量，使缓冲区中数据量为0时阻塞写入线程，读取线程使缓冲区中
    //数据降至trigger_level以下时释放信号量，使写入线程解除阻塞
    tg_uart1_CB.send_buf_semp = semp_create(1,0,"uart1 tx buf");
    if(tg_uart1_CB.send_buf_semp == NULL)
        goto exit_from_left_buf_semp;
    tg_uart1_CB.recv_buf_semp = semp_create(1,0,"uart1 rx buf");
    if(tg_uart1_CB.recv_buf_semp == NULL)
        goto exit_from_right_buf_semp;
    //分别创建左手和右手访问uart1设备的信号量，第一个参数为1表示串口是独占访问
    //的，第二个参数1表示初始状态有一盏信号灯点亮。左手由应用程序使用，右手由硬
    //件操作函数使用。
    left_semp = semp_create(1,1,"uart1 device left");
    if(left_semp == NULL)
        goto exit_from_left_semp;
    right_semp = semp_create(1,1,"uart1 device right");
    if(right_semp == NULL)
        goto exit_from_right_semp;

    Cpucfg_EnableLPSC(cn_PSC1_UART1);
    gpio_cfg_pin_func(1,0,cn_p1_0_uart1_txd);
    gpio_cfg_pin_func(1,1,cn_p1_1_uart1_rxd);

    //系统初始化时已经使中断处于禁止状态，无需再禁止和清除中断。
    //初始化uart硬件控制数据结构
    tg_uart1_CB.my_reg = (tag_UartReg *)cn_uart1_base;
    __uart_set_baud(tg_uart1_CB.my_reg,9600);
    temp = tg_uart1_CB.my_reg->IIR_FCR;     //读一下清除所有中断标志
//    tg_uart1_CB.my_reg->IIR_FCR |= 1;   //set FIFOEN
    tg_uart1_CB.my_reg->IIR_FCR = 0xcf; //接收14字节触发中断，清收发fifo。
    tg_uart1_CB.my_reg->LCR = 3 | (0<<2) | (000<<3) | (0<<6) | (0<<7);
    tg_uart1_CB.my_reg->MCR = 0;
    tg_uart1_CB.my_reg->PWREMU_MGMT = 0x7fff;
    tg_uart1_CB.my_reg->MDR = 0;        //16倍时钟
    tg_uart1_CB.my_reg->IER =  3;       //使能收发中断，此时中断线控制尚未使能
    tg_uart1_CB.baud = 9600;

    //初始化通用串口数据结构
    tg_uart1_CB.recv_trigger_level = 0;  //缓冲接收触发水平为0
    tg_uart1_CB.send_trigger_level = uart1_buf_len>>4;  //缓冲发送触发水平为1/16
    tg_uart1_CB.timeout = (u32)500*mS;  //超时时间=500
    tg_uart1_CB.serial_no = 1;  //串口号是1
    ring_init(  &tg_uart1_CB.send_ring_buf,
                uart1_ptcl_send_buf,
                uart1_buf_len);
    ring_init(  &tg_uart1_CB.recv_ring_buf,
                uart1_ptcl_recv_buf,
                uart1_buf_len);
    tg_uart1_CB.status = cn_ser_rxbuf_empty;

    //以下建立uart1设备
    uart_dev = dev_add_device(dev_get_root_device(),"uart1",
                               right_semp,left_semp,
                               (dev_write_func) uart_right_write,
                               (dev_read_func ) uart_right_read,
                               (dev_ctrl_func ) uart_ctrl,
                               (dev_write_func ) uart_left_write,
                               (dev_read_func  ) uart_left_read,
                               (dev_ctrl_func  ) uart_ctrl,
                               (ptu32_t)(&tg_uart1_CB)  //uart1设备的私有数据
                               );
    if(uart_dev == NULL)
        goto exit_from_add_device;
    pg_uart1_rhdl = dev_open_right("uart1",0);      //打开右手句柄
    u16g_evtt_uart1_error = djy_evtt_regist(
                           enum_correlative,cn_prio_real,0,5,
                           uart1_error_service,1024,NULL);
    if(u16g_evtt_uart1_error == cn_invalid_evtt_id)
        goto exit_from_error_evtt;
    uart_send_evtt = djy_evtt_regist(enum_correlative,cn_prio_real,0,
                                   5,uart_send_service,20,NULL);
    if(uart_send_evtt == cn_invalid_evtt_id)
        goto exit_from_send_evtt;
    dev_ctrl_right(pg_uart1_rhdl,cn_uart_connect_send_evtt,uart_send_evtt,0);
    int_isr_connect(cn_int_line_uart1,uart1_int);
    int_setto_asyn_signal(cn_int_line_uart1);
    int_echo_line(cn_int_line_uart1);       //清掉初始化产生的发送fifo空的中断
    int_restore_asyn_line(cn_int_line_uart1);
    return 1;
exit_from_send_evtt:
    djy_evtt_unregist(u16g_evtt_uart1_error);
exit_from_error_evtt:
    dev_delete_device(uart_dev);
exit_from_add_device:
    semp_delete(right_semp);
exit_from_right_semp:
    semp_delete(left_semp);
exit_from_left_semp:
    semp_delete(tg_uart1_CB.recv_buf_semp);
exit_from_right_buf_semp:
    semp_delete(tg_uart1_CB.send_buf_semp);
exit_from_left_buf_semp:
    return 0;
}

//----初始化uart2模块----------------------------------------------------------
//功能：初始化uart2模块
//参数：模块初始化函数没有参数
//返回：1=成功，0=失败
//-----------------------------------------------------------------------------
ptu32_t module_init_uart2(ptu32_t para)
{
    struct semaphore_LCB *left_semp,*right_semp;
    struct  pan_device   *uart_dev;
    uint16_t uart_send_evtt;
    vu32 temp;
    //保护缓冲区的信号量，使缓冲区中数据量为0时阻塞写入线程，读取线程使缓冲区中
    //数据降至trigger_level以下时释放信号量，使写入线程解除阻塞
    tg_uart2_CB.send_buf_semp = semp_create(1,0,"uart2 tx buf");
    if(tg_uart2_CB.send_buf_semp == NULL)
        goto exit_from_left_buf_semp;
    tg_uart2_CB.recv_buf_semp = semp_create(1,0,"uart2 rx buf");
    if(tg_uart2_CB.recv_buf_semp == NULL)
        goto exit_from_right_buf_semp;
    //分别创建左手和右手访问uart2设备的信号量，第一个参数为1表示串口是独占访问
    //的，第二个参数1表示初始状态有一盏信号灯点亮。左手由应用程序使用，右手由硬
    //件操作函数使用。
    left_semp = semp_create(1,1,"uart2 device left");
    if(left_semp == NULL)
        goto exit_from_left_semp;
    right_semp = semp_create(1,1,"uart2 device right");
    if(right_semp == NULL)
        goto exit_from_right_semp;

    Cpucfg_EnableLPSC(cn_PSC1_UART2);
    gpio_cfg_pin_func(1,2,cn_p1_2_uart2_txd);
    gpio_cfg_pin_func(1,3,cn_p1_3_uart2_rxd);
    //系统初始化时已经使中断处于禁止状态，无需再禁止和清除中断。
    //初始化uart硬件控制数据结构
    tg_uart2_CB.my_reg = (tag_UartReg *)cn_uart2_base;
    __uart_set_baud(tg_uart2_CB.my_reg,9600);
    temp = tg_uart2_CB.my_reg->IIR_FCR;     //读一下清除所有中断标志
//    tg_uart2_CB.my_reg->IIR_FCR |= 1;   //set FIFOEN
    tg_uart2_CB.my_reg->IIR_FCR = 0xcf; //接收14字节触发中断，清收发fifo。
    tg_uart2_CB.my_reg->LCR = 3 | (0<<2) | (000<<3) | (0<<6) | (0<<7);
    tg_uart2_CB.my_reg->MCR = 0;
    tg_uart2_CB.my_reg->PWREMU_MGMT = 0x7fff;
    tg_uart2_CB.my_reg->MDR = 0;        //16倍时钟
    tg_uart2_CB.my_reg->IER =  3;       //使能收发中断，此时中断线控制尚未使能
    tg_uart2_CB.baud = 9600;

    //初始化通用串口数据结构
    tg_uart2_CB.recv_trigger_level = 0;  //缓冲接收触发水平为0
    tg_uart2_CB.send_trigger_level = uart2_buf_len>>4;  //缓冲发送触发水平为1/16
    tg_uart2_CB.timeout = (u32)500*mS;  //超时时间=500
    tg_uart2_CB.serial_no = 2;  //串口号是2
    ring_init(  &tg_uart2_CB.send_ring_buf,
                uart2_ptcl_send_buf,
                uart2_buf_len);
    ring_init(  &tg_uart2_CB.recv_ring_buf,
                uart2_ptcl_recv_buf,
                uart2_buf_len);
    tg_uart2_CB.status = cn_ser_rxbuf_empty;

    //以下建立uart2设备
    uart_dev = dev_add_device(dev_get_root_device(),"uart2",
                               right_semp,left_semp,
                               (dev_write_func) uart_right_write,
                               (dev_read_func ) uart_right_read,
                               (dev_ctrl_func ) uart_ctrl,
                               (dev_write_func ) uart_left_write,
                               (dev_read_func  ) uart_left_read,
                               (dev_ctrl_func  ) uart_ctrl,
                               (ptu32_t)(&tg_uart2_CB)  //uart2设备的私有数据
                               );
    if(uart_dev == NULL)
        goto exit_from_add_device;
    pg_uart2_rhdl = dev_open_right("uart2",0);      //打开右手句柄
    u16g_evtt_uart2_error = djy_evtt_regist(
                           enum_correlative,cn_prio_real,0,5,
                           uart2_error_service,1024,NULL);
    if(u16g_evtt_uart2_error == cn_invalid_evtt_id)
        goto exit_from_error_evtt;
    uart_send_evtt = djy_evtt_regist(enum_correlative,cn_prio_real,0,
                                   5,uart_send_service,20,NULL);
    if(uart_send_evtt == cn_invalid_evtt_id)
        goto exit_from_send_evtt;
    dev_ctrl_right(pg_uart2_rhdl,cn_uart_connect_send_evtt,uart_send_evtt,0);
    int_isr_connect(cn_int_line_uart2,uart2_int);
    int_setto_asyn_signal(cn_int_line_uart2);
    int_echo_line(cn_int_line_uart2);       //清掉初始化产生的发送fifo空的中断
    int_restore_asyn_line(cn_int_line_uart2);
    return 1;
exit_from_send_evtt:
    djy_evtt_unregist(u16g_evtt_uart2_error);
exit_from_error_evtt:
    dev_delete_device(uart_dev);
exit_from_add_device:
    semp_delete(right_semp);
exit_from_right_semp:
    semp_delete(left_semp);
exit_from_left_semp:
    semp_delete(tg_uart2_CB.recv_buf_semp);
exit_from_right_buf_semp:
    semp_delete(tg_uart2_CB.send_buf_semp);
exit_from_left_buf_semp:
    return 0;
}

void uart0_error_service(void)
{
    while(1)
    {
        djy_wait_evtt_pop(djy_my_evtt_id(),1,cn_timeout_forever);
    }
}

void uart1_error_service(void)
{
    volatile uint32_t temp;
    while(1)
    {
        djy_wait_evtt_pop(djy_my_evtt_id(),1,cn_timeout_forever);
    }
}

void uart2_error_service(void)
{
    volatile uint32_t temp;
    while(1)
    {
        djy_wait_evtt_pop(djy_my_evtt_id(),1,cn_timeout_forever);
    }
}

//----uart0中断---------------------------------------------------------------
//功能: 接收，读取fifo中的数据并从右手写入设备
//      发送，从右手读数据并发送
//参数: 中断号.
//返回: 0.
//-----------------------------------------------------------------------------
uint32_t uart0_int(ufast_t uart_int_line)
{
    uint32_t recv_trans,num;
    uint32_t IIR;
    tag_UartReg *reg;
    uint8_t ch[20];
    reg =  (tag_UartReg *)tg_uart0_CB.my_reg;
    while(1)
    {
        IIR = reg->IIR_FCR;
        if(IIR & 1)
            break;
        IIR = (IIR>>1)&0x7;
        switch(IIR)
        {
            case 1:     //发送fifo寄存器空
            {
                recv_trans = dev_read_right(pg_uart0_rhdl,0,(ptu32_t)ch,16);
                for(num = 0; num < recv_trans; num++)
                {
                    reg->RBR_THR = ch[num];
                }
            }break;
            case 2:     //接收触发水平到
            case 6:     //接收超时，fifo中至少有一个数据，连续4字符时间无新数据
            {
                for(num = 0; num<20; num++)
                {
                    if(reg->LSR & 1)
                        ch[num] = reg->RBR_THR;
                    else
                        break;
                }
                dev_write_right(pg_uart0_rhdl,(ptu32_t)ch,0,num);
            }break;
            case 3:     //出错标志
            {
            }break;
            default: return 0;
        }
    }
    return 0;
}

//----uart1中断---------------------------------------------------------------
//功能: 接收，读取fifo中的数据并从右手写入设备
//      发送，从右手读数据并发送
//参数: 中断号.
//返回: 0.
//-----------------------------------------------------------------------------
uint32_t uart1_int(ufast_t uart_int_line)
{
    uint32_t recv_trans,num;
    uint32_t IIR;
    tag_UartReg *reg;
    uint8_t ch[20];
    reg = (tag_UartReg *)tg_uart1_CB.my_reg;
    while(1)
    {
        IIR = reg->IIR_FCR;
        if(IIR & 1)
            break;
        IIR = (IIR>>1)&0x7;
        switch(IIR)
        {
            case 1:     //发送fifo寄存器空
            {
                recv_trans = dev_read_right(pg_uart1_rhdl,0,(ptu32_t)ch,16);
                for(num = 0; num < recv_trans; num++)
                {
                    reg->RBR_THR = ch[num];
                }
            }break;
            case 2:     //接收触发水平到
            case 6:     //接收超时，fifo中至少有一个数据，连续4字符时间无新数据
            {
                for(num = 0; num<20; num++)
                {
                    if(reg->LSR & 1)
                        ch[num] = reg->RBR_THR;
                    else
                        break;
                }
                dev_write_right(pg_uart1_rhdl,(ptu32_t)ch,0,num);
            }break;
            case 3:     //出错标志
            {
            }break;
            default: return 0;
        }
    }
    return 0;
}

//----uart2中断---------------------------------------------------------------
//功能: 接收，读取fifo中的数据并从右手写入设备
//      发送，从右手读数据并发送
//参数: 中断号.
//返回: 0.
//-----------------------------------------------------------------------------
uint32_t uart2_int(ufast_t uart_int_line)
{
    uint32_t recv_trans,num;
    uint32_t IIR;
    tag_UartReg *reg;
    uint8_t ch[20];
    reg = (tag_UartReg *)tg_uart2_CB.my_reg;
    while(1)
    {
        IIR = reg->IIR_FCR;
        if(IIR & 1)
            break;
        IIR = (IIR>>1)&0x7;
        switch(IIR)
        {
            case 1:     //发送fifo寄存器空
            {
                recv_trans = dev_read_right(pg_uart2_rhdl,0,(ptu32_t)ch,16);
                for(num = 0; num < recv_trans; num++)
                {
                    reg->RBR_THR = ch[num];
                }
            }break;
            case 2:     //接收触发水平到
            case 6:     //接收超时，fifo中至少有一个数据，连续4字符时间无新数据
            {
                for(num = 0; num<20; num++)
                {
                    if(reg->LSR & 1)
                        ch[num] = reg->RBR_THR;
                    else
                        break;
                }
                dev_write_right(pg_uart2_rhdl,(ptu32_t)ch,0,num);
            }break;
            case 3:     //出错标志
            {
            }break;
            default: return 0;
        }
    }
    return 0;
}

//----uart设备右手写函数-------------------------------------------------------
//功能: 从右手写入uart设备,由uart设备连接的串口模块调用.
//      1.把数据写入串口serial_DCB_t的环形发送缓冲区中.
//      2.如果写入设备的数据未达到触发水平，则继续收集数据
//      3.达到出发水平后，按设定的方式把数据传递给上层设备
//参数: uart_dev,被操作的设备的右手指针.
//      buf，数据源指针。
//      len，数据量(bytes)
//返回: 实际写入环形缓冲区的字符数
//-----------------------------------------------------------------------------
ptu32_t uart_right_write(struct  pan_device *uart_dev,ptu32_t buf,
                            ptu32_t res2,ptu32_t len)
{
    struct uart_UCB *uart_port;
    uint16_t recv_bytes,valid_bytes,error_evtt;
    enum uart_error_no uart_error;

    uart_port = (struct uart_UCB*)dev_get_my_tag(uart_dev);

    //copy整个硬件缓冲区到协议缓冲区
    recv_bytes = ring_write(&uart_port->recv_ring_buf, (uint8_t*)buf,len);
    if(recv_bytes != len)
    {
        switch(uart_port->serial_no)
        {
            case 0:
                error_evtt = u16g_evtt_uart0_error;
                break;
            case 1:
                error_evtt = u16g_evtt_uart1_error;
                break;
            case 2:
                error_evtt = u16g_evtt_uart2_error;
                break;
            default:
                error_evtt = u16g_evtt_uart0_error;
        }
        uart_error = enum_recv_buf_over;
        djy_event_pop(error_evtt,enum_para_async,NULL,0,&uart_error,
                                            sizeof(uart_error),0,0);
    }
    valid_bytes = ring_check(&uart_port->recv_ring_buf);
    if(valid_bytes >= uart_port->recv_trigger_level)
    {
        //如果不登记右手写事件，就只能查询方式从设备读取数据了。
        djy_event_pop(uart_port->evtt_right_write,enum_para_async,
                                    NULL,0,NULL,0,0,0);
    }

    return (ptu32_t)recv_bytes;


}

//----uart设备左手写函数-------------------------------------------------------
//功能: 从左手写入uart设备,一般由uart的使用者调用.
//      1.把数据写入串口serial_DCB_t的环形发送缓冲区中.
//      2.如果串口硬件空闲,便把数据写入硬件发送缓冲区,并启动发送,否则不操作
//参数: uart_dev,被操作的设备的左手指针.
//      src_buf,数据源指针
//      len,要发送的序列长度
//返回: 实际写入环形缓冲区的字符数
//-----------------------------------------------------------------------------
ptu32_t uart_left_write(struct  pan_device *uart_dev,ptu32_t src_buf,
                            ptu32_t res,ptu32_t len)
{
    uint32_t result;
    struct uart_UCB *uart_port;
    uint32_t completed = 0;
    
    if(len == 0)
        return 0;
    uart_port = (struct uart_UCB*)dev_get_my_tag(uart_dev);
    while(1)
    {
        __uart_send_int_disable((tag_UartReg *)uart_port->my_reg);
        result = ring_write(&uart_port->send_ring_buf,
                            (uint8_t*)src_buf+completed,len-completed);
        __uart_send_int_enable((tag_UartReg *)uart_port->my_reg);
        djy_event_pop(uart_port->evtt_left_write,enum_para_async,NULL,0,
                                            &uart_dev,sizeof(uart_dev),false,0);
        if(result != len-completed)     //缓冲区满，没有送出全部数据
        {
            completed += result;
            semp_pend(uart_port->send_buf_semp,cn_timeout_forever);
        }else
            break;
    }
    return result;
}

//----串口设备左手读函数------------------------------------------------------
//功能: 左手读用于上层设备,读取串口接收环形缓冲区的数据
//参数: uart_dev,被操作的设备的左手指针.
//      dst_buf,读缓冲区
//      len,读入长度,
//返回: 实际读出长度
//----------------------------------------------------------------------------
ptu32_t uart_left_read(struct  pan_device *uart_dev,ptu32_t res,
                                        ptu32_t dst_buf,ptu32_t len)
{
    struct uart_UCB *uart_port;
    uint32_t result;
    uart_port = (struct uart_UCB*)dev_get_my_tag(uart_dev);
    __uart_recv_int_disable((tag_UartReg *)uart_port->my_reg);
    result = ring_read(&uart_port->recv_ring_buf,(uint8_t*)dst_buf,len);
    __uart_recv_int_enable((tag_UartReg *)uart_port->my_reg);
    return result;
}

//----串口设备右手读函数------------------------------------------------------
//功能: 右手读用于硬件,硬件发送服务函数 ser_send_service从串口发送环形缓冲区
//      读取数据到串口硬件缓冲区并启动发送.
//参数: uart_dev,被操作的设备的右手指针.
//      dst_buf,读缓冲区
//      len,读入长度,
//返回: 实际读出长度
//----------------------------------------------------------------------------
ptu32_t uart_right_read(struct  pan_device *uart_dev,ptu32_t res1,
                                        ptu32_t dst_buf,ptu32_t len)
{
    struct uart_UCB *uart_port;
    uint32_t result;
    uart_port = (struct uart_UCB*)dev_get_my_tag(uart_dev);
    result = (ptu32_t)ring_read(&uart_port->send_ring_buf,(uint8_t *)dst_buf,len);
    if(ring_check(&uart_port->send_ring_buf) <= uart_port->send_trigger_level)
        semp_post(uart_port->send_buf_semp);
    return result;
}

//----串口设备控制函数---------------------------------------------------------
//功能: 串口设备的控制函数
//参数: uart_handle,被操作的串口设备指针.
//      cmd,操作类型
//      data,含义依cmd而定
//返回: 无意义.
//-----------------------------------------------------------------------------
ptu32_t uart_ctrl(struct pan_device *uart_dev,uint32_t cmd,
                   uint32_t data1,uint32_t data2)
{
    struct uart_UCB *uart_port;


    uart_port = (struct uart_UCB*)dev_get_my_tag(uart_dev);
    switch(cmd)
    {
        case cn_uart_connect_recv_evtt:
            uart_port->evtt_right_write = (uint16_t)data1;
            break;
        case cn_uart_disconnect_recv_evtt:
            uart_port->evtt_right_write = cn_invalid_evtt_id;
            break;
        case cn_uart_connect_send_evtt:
            uart_port->evtt_left_write = (uint16_t)data1;
            break;
        case cn_uart_disconnect_send_evtt:
            uart_port->evtt_left_write = cn_invalid_evtt_id;
            break;
        case cn_uart_start:
            __uart_recv_int_enable((tag_UartReg *)uart_port->my_reg);
            __uart_send_int_enable((tag_UartReg *)uart_port->my_reg);
            break;
        case cn_uart_stop:
            __uart_recv_int_disable((tag_UartReg *)uart_port->my_reg);
            __uart_send_int_disable((tag_UartReg *)uart_port->my_reg);
            break;
        case cn_uart_set_baud:  //设置Baud
            if(uart_port->baud !=data1)
            {
                uart_port->baud = data1;
                __uart_set_baud((tag_UartReg *)uart_port->my_reg,data1);
            }
            break;
        case cn_uart_send_data:     //启动发送
        //用IO控制半双工通信的发送使能的话,在此转换收发.
            break;
        case cn_uart_recv_data:     //启动接收
        //用IO控制半双工通信的发送使能的话,在此转换收发.
            break;
        case cn_uart_completed_send:
        //用IO控制半双工通信的发送使能的话,在此转换收发,注意要查询UTRSTAT寄存器
        //的Transmit shifter empty位判断是否已经完成.
            break;
        case cn_uart_rx_pause:      //暂停接收
            __uart_recv_int_disable((tag_UartReg *)uart_port->my_reg);
            break;
        case cn_uart_rx_resume:      //恢复接收
            __uart_recv_int_enable((tag_UartReg *)uart_port->my_reg);
            break;
        case cn_uart_recv_soft_level:    //设置接收软件缓冲区触发水平
            uart_port->recv_trigger_level = (uint16_t)data1;
            break;
        case cn_uart_send_soft_level:    //设置接收软件缓冲区触发水平
            uart_port->send_trigger_level = (uint16_t)data1;
            break;
        case cn_uart_overtime:   //data1为毫秒数
            uart_port->timeout = data1;
            break;
        case cn_uart_recv_hard_level:    //设置接收fifo触发水平
        {
            uart_port->my_reg->IIR_FCR &= 0x3f;
            switch (data1)
            {
                case 1:
                    uart_port->my_reg->IIR_FCR |= 0x00;
                    break;
                case 4:
                    uart_port->my_reg->IIR_FCR |= 0x40;
                    break;
                case 8:
                    uart_port->my_reg->IIR_FCR |= 0x80;
                    break;
                case 14:
                    uart_port->my_reg->IIR_FCR |= 0xc0;
                    break;
                default :break;
            }
            break;
        }
        default: break;
    }
    return 0;
}

//----串行发送服务-------------------------------------------------------------
//功能: 事件服务函数,当应用程序从设备左手写入待发送的数据时，就会弹出本事件.
//参数: 事件服务函数没有参数.
//返回: 事件服务函数可以不返回,即使返回也没有返回值.
//-----------------------------------------------------------------------------
void uart_send_service(void)
{
    struct uart_UCB *uart_port;
    struct pan_device *ser_dev;
    djy_handle_t uart_rhdl;
    void *my_para;
    uint8_t ch[16];
    ufast_t trans,num;

    while(1)
    {
        my_para = djy_get_event_para(NULL);
        ser_dev = (struct  pan_device *)(*(void**)my_para);
        uart_port = (struct uart_UCB *)dev_get_my_tag(ser_dev);
        __uart_send_int_disable((tag_UartReg *)uart_port->my_reg);
        if( uart_tx_tran_empty((tag_UartReg *)uart_port->my_reg))
        {
            if(uart_port->serial_no == 0)
                uart_rhdl = pg_uart0_rhdl;
            else if(uart_port->serial_no == 1)
                uart_rhdl = pg_uart1_rhdl;
            else if(uart_port->serial_no == 2)
                uart_rhdl = pg_uart2_rhdl;
            trans = dev_read_right(uart_rhdl,0,(ptu32_t)ch,16);
            for(num = 0; num < trans; num++)
            {
                uart_port->my_reg->RBR_THR = ch[num];
            }
        }
        __uart_send_int_enable((tag_UartReg *)uart_port->my_reg);
        djy_task_completed(0);
        //等待自身类型的事件再次发生。
        djy_wait_evtt_pop(djy_my_evtt_id(),1,cn_timeout_forever);
    }
}


