//----------------------------------------------------
//Copyright (C), 2004-2009,  lst.
//版权所有 (C), 2004-2009,   lst.
//所属模块: uart驱动程序
//作者：lst
//版本：V1.0.0
//文件描述: 驱动44b0的uart
//其他说明:
//修订历史:
//1. 日期:2009-03-10
//   作者:lst
//   新版本号：1.0.0
//   修改说明: 移植字44b0的1.0.1版
//------------------------------------------------------
#include <string.h>
#include "os_inc.h"
#include "cpu_peri.h"
#include "gpio_api.h"

#if(cfg_uart1_used == 1)
static djy_handle_t   pg_uart1_rhdl;
static struct uart_UCB tg_uart1_cb;
uint16_t u16g_evtt_uart1_error;
uint16_t u16g_evtt_uart1_hard_recv;
uint8_t uart1_drv_send_buf[uart1_buf_len];
uint8_t uart1_drv_recv_buf[uart1_buf_len];
uint8_t uart1_dma_send_buf[32];
uint8_t uart1_dma_recv_buf[32];
bool_t uart1_first_dma_send;
#endif

#if(cfg_uart2_used == 1)
static djy_handle_t   pg_uart2_rhdl;
static struct uart_UCB tg_uart2_cb;
uint16_t u16g_evtt_uart2_error;
uint16_t u16g_evtt_uart2_hard_recv;
uint8_t uart2_drv_send_buf[uart2_buf_len];
uint8_t uart2_drv_recv_buf[uart2_buf_len];
uint8_t uart2_dma_send_buf[32];
uint8_t uart2_dma_recv_buf[32];
bool_t uart2_first_dma_send;
#endif

#if(cfg_uart3_used == 1)
static djy_handle_t   pg_uart3_rhdl;
static struct uart_UCB tg_uart3_cb;
uint16_t u16g_evtt_uart3_error;
uint16_t u16g_evtt_uart3_hard_recv;
uint8_t uart3_drv_send_buf[uart3_buf_len];
uint8_t uart3_drv_recv_buf[uart3_buf_len];
uint8_t uart3_dma_send_buf[32];
uint8_t uart3_dma_recv_buf[32];
bool_t uart3_first_dma_send;
#endif

#if(cfg_uart4_used == 1)
static djy_handle_t   pg_uart4_rhdl;
static struct uart_UCB tg_uart4_cb;
uint16_t u16g_evtt_uart4_error;
uint16_t u16g_evtt_uart4_hard_recv;
uint8_t uart4_drv_send_buf[uart4_buf_len];
uint8_t uart4_drv_recv_buf[uart4_buf_len];
uint8_t uart4_dma_send_buf[32];
uint8_t uart4_dma_recv_buf[32];
bool_t uart4_first_dma_send;
#endif

#if(cfg_uart5_used == 1)   //uart5不支持dma传输
static djy_handle_t   pg_uart5_rhdl;
static struct uart_UCB tg_uart5_cb;
uint16_t u16g_evtt_uart5_error;
uint16_t u16g_evtt_uart5_hard_recv;
uint8_t uart5_drv_send_buf[uart5_buf_len];
uint8_t uart5_drv_recv_buf[uart5_buf_len];
#endif

#if(cfg_uart1_used == 1)
//----初始化uart0模块----------------------------------------------------------
//功能：初始化uart0模块，初始化状态为不使用dma传送的方式。
//参数：模块初始化函数没有参数
//返回：true = 成功初始化，false = 初始化失败
//-----------------------------------------------------------------------------
ptu32_t module_init_uart1(ptu32_t para)
{
    struct semaphore_LCB *left_semp,*right_semp;
    struct  pan_device   *uart_dev;
    uint32_t temp1,temp2;
    //保护缓冲区的信号量，使缓冲区中数据量为0时阻塞写入线程，读取线程使缓冲区中
    //数据降至trigger_level以下时释放信号量，使写入线程解除阻塞
    tg_uart1_cb.send_buf_semp = semp_create(1,0,"uart1 tx buf");
    if(tg_uart1_cb.send_buf_semp == NULL)
        goto exit_from_left_buf_semp;
    tg_uart1_cb.recv_buf_semp = semp_create(1,0,"uart1 rx buf");
    if(tg_uart1_cb.recv_buf_semp == NULL)
        goto exit_from_right_buf_semp;
    left_semp = semp_create(1,1,"uart1 device left");
    if(left_semp == NULL)
        goto exit_from_left_semp;
    right_semp = semp_create(1,1,"uart1 device right");
    if(right_semp == NULL)
        goto exit_from_right_semp;
    //初始化IO端口位uart功能
    //Ports  :  GPA10 GPA9 GPA8 GPA7 GPA6 GPA5 GPA4 GPA3 GPA2 GPA1 GPA0
    //Signal :   RXD1 TXD1  xx   xx   xx   xx   xx   xx   xx   xx   xx
    bb_rcc_apb2enr_uart1en = 1;     //uart1时钟使能
    gpio_power_on(cn_gpio_a);
    gpio_cfg_pin_func(cn_gpio_a,9,cn_gpio_mode_peri_out_pp_10Mhz);
    gpio_cfg_pin_func(cn_gpio_a,10,cn_gpio_mode_in_floating);
    //系统初始化时已经使中断处于禁止状态，无需再禁止和清除中断。
    //初始化uart硬件控制数据结构
    pg_uart1_reg->CR1 = 0x20ac;
    pg_uart1_reg->CR2 = 0x0;
    pg_uart1_reg->CR3 = 0x0000;
    temp1 = cn_pclk2/(16*115200);
    temp2 = cn_pclk2%(16*115200);
    temp2 = temp2/115200;
    pg_uart1_reg->BRR =  ((u16)temp1<<4)+(u16)temp2;
    tg_uart1_cb.baud = 115200;

    //初始化通用串口数据结构
    tg_uart1_cb.recv_trigger_level = uart1_buf_len>>4;  //缓冲接收触发水平为1/16
    tg_uart1_cb.send_trigger_level = uart1_buf_len>>4;  //缓冲发送触发水平为1/16
    tg_uart1_cb.timeout = 100*mS;   //超时时间=100
    tg_uart1_cb.sendding = false;
    ring_init(  &tg_uart1_cb.send_ring_buf,
                uart1_drv_send_buf,
                uart1_buf_len);
    ring_init(  &tg_uart1_cb.recv_ring_buf,
                uart1_drv_recv_buf,
                uart1_buf_len);

    //以下建立uart1设备
    uart_dev = dev_add_device(     dev_get_root_device(),"uart1",
                                   right_semp,left_semp,
                                   (dev_write_func) uart_hard_write,
                                   (dev_read_func ) uart_hard_read,
                                   (dev_ctrl_func ) uart1_ctrl,
                                   (dev_write_func ) uart1_app_write,
                                   (dev_read_func  ) uart1_app_read,
                                   (dev_ctrl_func  ) uart1_ctrl,
                                   (ptu32_t)(&tg_uart1_cb)
                                   );    //"uart1"是一个根设备
    if(uart_dev == NULL)
        goto exit_from_add_device;
    pg_uart1_rhdl = dev_open_right("uart1",0);      //打开右手句柄
    u16g_evtt_uart1_error = djy_evtt_regist(enum_correlative,
                            cn_prio_real,0,5,uart_error_service,10,"uart1 err");
    if(u16g_evtt_uart1_error == cn_invalid_evtt_id)
        goto exit_from_error_evtt;
    u16g_evtt_uart1_hard_recv = djy_evtt_regist(enum_correlative,cn_prio_real,0,
                                   5,uart1_hard_recv_service,32,"uart1 recv");
    if(u16g_evtt_uart1_hard_recv == cn_invalid_evtt_id)
        goto exit_from_hard_recv_evtt;

    djy_event_pop(u16g_evtt_uart1_hard_recv,enum_para_async,NULL,0,NULL,0,0,0);

    int_isr_connect(cn_int_line_USART1,uart1_int);
    int_setto_asyn_signal(cn_int_line_USART1);

//    bb_uart1_sr_txe = 0;
    int_echo_line(cn_int_line_USART1);
    int_restore_asyn_line(cn_int_line_USART1);
    return true;

exit_from_hard_recv_evtt:
    djy_evtt_unregist(u16g_evtt_uart1_error);
exit_from_error_evtt:
    dev_delete_device(uart_dev);
exit_from_add_device:
    semp_delete(right_semp);
exit_from_right_semp:
    semp_delete(left_semp);
exit_from_left_semp:
    semp_delete(tg_uart1_cb.recv_buf_semp);
exit_from_right_buf_semp:
    semp_delete(tg_uart1_cb.send_buf_semp);
exit_from_left_buf_semp:
    return false;
}

//----uart1中断---------------------------------------------------------------
//功能: 如果是接收信号触发的中断:
//      1.检查接收fifo中的数据量.把数据从接收寄存器全部copy到物理层缓冲区中.
//      2.如果物理层缓冲区满.置串口控制块状态字的溢出位,并发出uart错误事件.
//      3.发送串口接收事件.两个事件都是状态量事件
//      如果是发送信号引发的中断:
//      1.检查发送fifo有多少空位
//      2.从设备右手接口读出适量数据，
//      3.把读出的数据写入发送fifo
//      如果是出错信号引发中断:弹出串口出错事件
//参数: 中断函数没有参数.
//返回: 中断函数没有返回值.
//-----------------------------------------------------------------------------
uint32_t uart1_int(ufast_t uart_int_line)
{
    u16 num;
    u8 ch;
    enum uart_error_no parameter;
    if(bb_uart1_sr_rxne == 1)
    {
        ch = (u8)pg_uart1_reg->DR;
        num = dev_write_right(pg_uart1_rhdl,(ptu32_t)&ch,0,1);
        if(num != 1)
        {
            parameter = enum_recv_buf_over;
            djy_event_pop(u16g_evtt_uart1_error,false,NULL,0,&parameter,
                                sizeof(parameter),0,0);
        }else
        {
            djy_event_pop(u16g_evtt_uart1_hard_recv,false,NULL,0,NULL,0,0,0);
        }
    }
    if(bb_uart1_sr_txe == 1)
    {
        num = dev_read_right(pg_uart1_rhdl,0,(ptu32_t)&ch,1);
        if(num != 0)
            pg_uart1_reg->DR = ch;
        else
        {

            bb_uart1_cr1_txeie = 0;
            tg_uart1_cb.sendding = false;
        }
    }
    if(bb_uart1_sr_ore == 1)
    {
        ch = (u8)pg_uart1_reg->DR;
        num = pg_uart1_reg->SR;     //读一下sr寄存器
        parameter = enum_recv_hard_over;
        djy_event_pop(u16g_evtt_uart1_error,false,NULL,0,&parameter,
                            sizeof(parameter),0,0);
    }
    if(bb_uart1_sr_idle)
    {
        pg_uart1_reg->DR;
        int_tap_line(cn_int_line_DMA1_Ch5);
    }
    return 0;
}

//----dma1 ch4中断-------------------------------------------------------------
//功能: 当uart1配置成dma方式是，dma1 ch4负责发送，ch5负责接收
//参数: 中断函数没有参数.
//返回: 中断函数没有返回值.
//-----------------------------------------------------------------------------
uint32_t dma1ch4_int(ufast_t int_line)
{
    uint16_t num;
    pg_dma1_reg->IFCR |= 0x0000f000;
    num = dev_read_right(pg_uart1_rhdl,0,(ptu32_t)uart1_dma_send_buf,32);
    if(num == 0)
        tg_uart1_cb.sendding = false;
    else
    {
        bb_dma1_ch4_ccr_en = 0;    //暂停dma传输
        pg_dma1_channel4_reg->CMAR = (u32)uart1_dma_send_buf;
        pg_dma1_channel4_reg->CNDTR = num;
        bb_dma1_ch4_ccr_en = 1;    //继续dma传输
    }
    return 0;
}

//----dma1 ch5中断-------------------------------------------------------------
//功能: 当uart1配置成dma方式是，dma1 ch4负责发送，ch5负责接收
//参数: 中断函数没有参数.
//返回: 中断函数没有返回值.
//-----------------------------------------------------------------------------
uint32_t dma1ch5_int(ufast_t int_line)
{
    u32 recvs,num;
    enum uart_error_no parameter;
    pg_dma1_reg->IFCR |= 0x000f0000;
    bb_dma1_ch5_ccr_en = 0;    //暂停dma传输
    recvs = 32-pg_dma1_channel5_reg->CNDTR;
    if(recvs == 32)     //dma缓冲区满，可能有数据丢失
    {
        parameter = enum_recv_hard_over;
        djy_event_pop(u16g_evtt_uart1_error,false,NULL,0,&parameter,
                            sizeof(parameter),0,0);
    }else
    {
        num = dev_write_right(pg_uart1_rhdl,(ptu32_t)uart1_dma_recv_buf,0,recvs);
        if(num != recvs)
        {
            parameter = enum_recv_buf_over;
            djy_event_pop(u16g_evtt_uart1_error,false,NULL,0,&parameter,
                            sizeof(parameter),0,0);
        }else
            djy_event_pop(u16g_evtt_uart1_hard_recv,false,NULL,0,NULL,0,0,0);
    }
    pg_dma1_channel5_reg->CNDTR = 32;
    pg_dma1_channel5_reg->CMAR = (u32)uart1_dma_recv_buf;
    bb_dma1_ch5_ccr_en = 1;    //继续dma传输
    return 0;
}

//----设置不使用dma------------------------------------------------------------
//功能: uart1不使用dma收发，每收发一个字节就必须中断一次，不建议在高速连续通信
//      中使用。
//参数: 无
//返回: 无
//-----------------------------------------------------------------------------
void __uart1_set_dma_unused(void)
{
    pg_uart1_reg->CR3 &= ~0x00c0;     //设置不使用dma传输
    int_restore_asyn_line(cn_int_line_USART1);
    if(tg_uart1_cb.dma_used == cn_dma_used)
    {
        int_save_asyn_line(cn_int_line_DMA1_Ch4);
        int_save_asyn_line(cn_int_line_DMA1_Ch4);
        tg_uart1_cb.dma_used = cn_dma_unused;
    }
    bb_uart1_cr1_rxneie = 1;
    bb_uart1_cr1_txeie = 1;
    bb_uart1_cr1_idleie = 0;
}

//----设置使用dma------------------------------------------------------------
//功能: 设置uart1使用dma收发，dma4发，dma5收，将重新初始化所有dma寄存器，因为:
//      1、不使用dma期间可能有其他设备使用dma，从而更改了设置。
//      2、即使uart使用dma期间，也可能软件模块配合的问题导致其他设备使用dma.
//参数: 无
//返回: 无
//-----------------------------------------------------------------------------
void __uart1_set_dma_used(void)
{
    bb_rcc_ahbenr_dma1en = 1;
    bb_dma1_ch4_ccr_en = 0;    //停止dma通道
    bb_dma1_ch5_ccr_en = 0;    //停止dma通道
    bb_uart1_cr1_rxneie = 0;
    bb_uart1_cr1_txeie = 0;
    bb_uart1_cr1_idleie = 1;
    pg_dma1_reg->IFCR |= 0x000ff000;        //清所有中断状态
    pg_dma1_channel4_reg->CCR = 0x00000092; //发送dma
    pg_dma1_channel4_reg->CPAR = (u32)&(pg_uart1_reg->DR);

    pg_dma1_channel5_reg->CCR = 0x00000086; //接收dma
    pg_dma1_channel5_reg->CPAR = (u32)&(pg_uart1_reg->DR);
    pg_dma1_channel5_reg->CNDTR = 32;
    pg_dma1_channel5_reg->CMAR = (u32)uart1_dma_recv_buf;

    int_isr_connect(cn_int_line_DMA1_Ch4,dma1ch4_int);
    int_isr_connect(cn_int_line_DMA1_Ch5,dma1ch5_int);

    int_setto_asyn_signal(cn_int_line_DMA1_Ch4);
    int_setto_asyn_signal(cn_int_line_DMA1_Ch5);
    int_restore_asyn_line(cn_int_line_DMA1_Ch4);
    int_restore_asyn_line(cn_int_line_DMA1_Ch5);

    pg_uart1_reg->CR3 |= 0x00c0;  //设置串口使用dma收发
    tg_uart1_cb.dma_used = cn_dma_used;
    bb_dma1_ch5_ccr_en = 1;    //启动dma通道
    uart1_first_dma_send = true;
}

//----暂停发送------------------------------------------------------------
//功能: 用于左手写设备过程中要操作发送缓冲区，防止发送中断访问发送缓冲区。
//参数: 无
//返回: 无
//-----------------------------------------------------------------------------
void __uart1_pause_send_int(void)
{
    if(tg_uart1_cb.dma_used == true)
    {
        int_save_asyn_line(cn_int_line_DMA1_Ch4);
    }else
    {
        int_save_asyn_line(cn_int_line_USART1);
    }
}

//----恢复发送------------------------------------------------------------
//功能: 用于左手写设备过程中操作发送缓冲区完成，可恢复发送
//参数: 无
//返回: 无
//-----------------------------------------------------------------------------
void __uart1_resume_send_int(void)
{
    if(tg_uart1_cb.dma_used == true)
    {
        int_restore_asyn_line(cn_int_line_DMA1_Ch4);
    }else
    {
        int_restore_asyn_line(cn_int_line_USART1);
    }
}

//----暂停接收------------------------------------------------------------
//功能: 用于左手写设备过程中要操作发送缓冲区，防止发送中断访问发送缓冲区。
//参数: 无
//返回: 无
//-----------------------------------------------------------------------------
void __uart1_pause_recv_int(void)
{
    if(tg_uart1_cb.dma_used == true)
    {
        int_save_asyn_line(cn_int_line_DMA1_Ch5);
    }else
    {
        int_save_asyn_line(cn_int_line_USART1);
    }
}

//----恢复接收------------------------------------------------------------
//功能: 用于左手写设备过程中操作发送缓冲区完成，可恢复发送
//参数: 无
//返回: 无
//-----------------------------------------------------------------------------
void __uart1_resume_recv_int(void)
{
    if(tg_uart1_cb.dma_used == true)
    {
        int_restore_asyn_line(cn_int_line_DMA1_Ch5);
    }else
    {
        int_restore_asyn_line(cn_int_line_USART1);
    }
}

//----启动发送------------------------------------------------------------
//功能: 左手写入数据后，如果发送器未工作，需要发送数据以启动发送器
//参数: 无
//返回: 无
//-----------------------------------------------------------------------------
void __uart1_start_send(void)
{
    u32 num,addr;
    if(tg_uart1_cb.dma_used == true)
    {
        num = dev_read_right(pg_uart1_rhdl,0,(ptu32_t)uart1_dma_send_buf,32);
        if(uart1_first_dma_send)
        {
            pg_uart1_reg->DR = uart1_dma_send_buf[0];
            uart1_first_dma_send = false;
            addr = (u32)uart1_dma_send_buf+1;
            num--;
        }else
            addr = (u32)uart1_dma_send_buf;

        if(num > 0)
        {
            bb_dma1_ch4_ccr_en = 0;    //暂停dma传输
            pg_dma1_channel4_reg->CMAR = addr;
            pg_dma1_channel4_reg->CNDTR = num;
            bb_dma1_ch4_ccr_en = 1;    //继续dma传输
            tg_uart1_cb.sendding = true;
        }
    }else
    {
        bb_uart1_cr1_txeie = 1;
        dev_read_right(pg_uart1_rhdl,0,(ptu32_t)&pg_uart1_reg->DR,1);
        tg_uart1_cb.sendding = true;
    }
}

//----启动串口1------------------------------------------------------------
//功能: 使串口开始工作
//参数: 无
//返回: 无
//-----------------------------------------------------------------------------
void __uart1_start(void)
{
    bb_rcc_apb2enr_uart1en = 1;    //uart1时钟使能
    bb_uart1_cr1_ue = 1;
}

//----停止串口1------------------------------------------------------------
//功能: 使串口停止工作
//参数: 无
//返回: 无
//-----------------------------------------------------------------------------
void __uart1_stop(void)
{
    bb_rcc_apb2enr_uart1en = 0;    //uart1时钟使能
    bb_uart1_cr1_ue = 0;
}

//----uart1设备左手写函数-------------------------------------------------------
//功能: 从左手写入uart设备,一般由uart的使用者调用.
//      1.把数据写入串口serial_DCB_t的环形发送缓冲区中.
//      2.如果串口硬件空闲,便把数据写入硬件发送缓冲区,并启动发送,否则不操作
//参数: uart_dev,被操作的设备的指针.
//      src_buf,数据源指针
//      len,要发送的序列长度
//返回: 实际写入环形缓冲区的字符数
//-----------------------------------------------------------------------------
ptu32_t uart1_app_write(struct  pan_device *uart_dev,ptu32_t src_buf,
                            ptu32_t res,ptu32_t len)
{
    uint32_t result;
    uint32_t completed = 0;

    if(len == 0)
        return 0;
    while(1)
    {
        __uart1_pause_send_int();
        result = ring_write(&tg_uart1_cb.send_ring_buf,
                            (uint8_t*)src_buf+completed,len-completed);
        if(tg_uart1_cb.sendding == false)
            __uart1_start_send();
        __uart1_resume_send_int();
        if(result != len-completed)     //缓冲区满，没有送出全部数据
        {
            completed += result;
            semp_pend(tg_uart1_cb.send_buf_semp,cn_timeout_forever);
        }else
            break;
    }
    return result;
}

//----串口1设备左手读函数------------------------------------------------------
//功能: 左手读用于上层设备,读取串口接收环形缓冲区的数据
//参数: uart_dev,被操作的设备的指针.
//      des_buf,读缓冲区
//      len,读入长度,
//返回: 实际读出长度
//----------------------------------------------------------------------------
ptu32_t uart1_app_read(struct  pan_device *uart_dev,ptu32_t res,
                                        ptu32_t des_buf,ptu32_t len)
{
    uint32_t result;
    __uart1_pause_recv_int();
    result = ring_read(&tg_uart1_cb.recv_ring_buf,(uint8_t*)des_buf,len);
    __uart1_resume_recv_int();
    return result;
}

//----串口1设备控制函数--------------------------------------------------------
//功能: 串口设备的控制函数
//参数: uart_dev,被操作的设备的指针.
//      cmd,操作类型
//      data,含义依cmd而定
//返回: 无意义.
//-----------------------------------------------------------------------------
ptu32_t uart1_ctrl(struct  pan_device *uart_dev,uint32_t cmd,
                   uint32_t data1,uint32_t data2)
{
    u16 temp1,temp2;

    switch(cmd)
    {
        case cn_uart_connect_recv_evtt:
            tg_uart1_cb.recv_target_evtt = (uint16_t)data1;
            break;
        case cn_uart_disconnect_recv_evtt:
            tg_uart1_cb.recv_target_evtt = cn_invalid_evtt_id;
            break;
        case cn_uart_start:
            __uart1_start();
            break;
        case cn_uart_stop:
            __uart1_stop();
            break;
        case cn_uart_set_baud:  //设置Baud
            if(tg_uart1_cb.baud !=data1)
            {
                temp1 = cn_pclk2/(16*data1);
                temp2 = cn_pclk2%(16*data1);
                temp2 = temp2/data1;
                pg_uart1_reg->BRR =  (temp1<<4)+temp2;
                tg_uart1_cb.baud = data1;
            }
            break;
        case cn_uart_half_duplex_send:     //启动发送
        //用IO控制半双工通信的发送使能的话,在此转换收发.
            break;
        case cn_uart_half_duplex_recv:     //启动接收
        //用IO控制半双工通信的发送使能的话,在此转换收发.
            break;
        case cn_uart_setto_all_duplex:
        //用IO控制半双工通信的发送使能的话,在此转换收发,注意要查询UTRSTAT寄存器
        //的Transmit shifter empty位判断是否已经完成.
            break;
        case cn_uart_overtime:   //data1为毫秒数
            tg_uart1_cb.timeout = data1;
            break;
        case cn_uart_recv_soft_level:    //设置接收软件缓冲区触发水平
            tg_uart1_cb.recv_trigger_level = (uint16_t)data1;
            break;
        case cn_uart_send_soft_level:    //设置接收软件缓冲区触发水平
            tg_uart1_cb.send_trigger_level = (uint16_t)data1;
            break;
        case cn_uart_en_rts:
            pg_uart1_reg->CR3 |= 0x100;
            break;
        case cn_uart_dis_rts:
            pg_uart1_reg->CR3 &= ~0x100;
            break;
        case cn_uart_en_cts:
            pg_uart1_reg->CR3 |= 0x200;
            break;
        case cn_uart_dis_cts:
            pg_uart1_reg->CR3 &= ~0x200;
            break;
        case cn_uart_dma_used:
            __uart1_set_dma_used();
            break;
        case cn_uart_dma_unused:
            __uart1_set_dma_unused();
            break;
        default: break;
    }
    return 0;
}

//----串行接收服务-------------------------------------------------------------
//功能: 事件服务函数,当串口硬件收到有效数据后，将收到的数据写入设备的同时，发送
//      本事件，事件的两个参数分别是:
//      1、所属串口的右手设备句柄.
//      2、本次接收到的数据数量
//参数: my_event，本事件指针.
//返回: 事件服务函数可以不返回,即使返回也没有返回值.
//-----------------------------------------------------------------------------
void uart1_hard_recv_service(void)
{
    uint32_t valid_bytes=0;
    u32 recvs;
    u32 wakeup;

    while(1)
    {
        //等待自身类型的事件再次发生，新事件在串口中断或dma中断服务函数中弹出。
        wakeup = djy_wait_evtt_pop(djy_my_evtt_id(),1,
                                    tg_uart1_cb.timeout);
        if(wakeup == cn_sync_timeout)
        {
            if(tg_uart1_cb.dma_used == true)
            {
                bb_dma1_ch5_ccr_en = 0;    //暂停dma传输
                recvs = 32-pg_dma1_channel5_reg->CNDTR;
                dev_write_right(pg_uart1_rhdl,(ptu32_t)uart1_dma_recv_buf,0,recvs);
                pg_dma1_channel5_reg->CNDTR = 32;
                pg_dma1_channel5_reg->CMAR = (u32)uart1_dma_recv_buf;
                bb_dma1_ch5_ccr_en = 1;    //继续dma传输
                valid_bytes += recvs;
            }
            if(valid_bytes != 0)
            {
                //如果不登记右手写事件，就只能查询方式从设备读取数据了。
                djy_event_pop(tg_uart1_cb.recv_target_evtt,
                                false,NULL,0,NULL,0,0,0);
                valid_bytes = 0;

            }
        }else
        {
            valid_bytes = ring_check(&tg_uart1_cb.recv_ring_buf);
            if(valid_bytes >= tg_uart1_cb.recv_trigger_level)
            {
                //如果不登记右手写事件，就只能查询方式从设备读取数据了。
                djy_event_pop(tg_uart1_cb.recv_target_evtt,
                                false,NULL,0,NULL,0,0,0);
                valid_bytes = 0;
            }
        }
    }
}

#endif  //for (cfg_uart1_used == 1)

#if(cfg_uart2_used == 1)
//----初始化uart0模块----------------------------------------------------------
//功能：初始化uart0模块，初始化状态为不使用dma传送的方式。
//参数：模块初始化函数没有参数
//返回：true = 成功初始化，false = 初始化失败
//-----------------------------------------------------------------------------
ptu32_t module_init_uart2(ptu32_t para)
{
    struct semaphore_LCB *left_semp,*right_semp;
    struct  pan_device   *uart_dev;
    uint32_t temp1,temp2;
    //保护缓冲区的信号量，使缓冲区中数据量为0时阻塞写入线程，读取线程使缓冲区中
    //数据降至trigger_level以下时释放信号量，使写入线程解除阻塞
    tg_uart2_cb.send_buf_semp = semp_create(1,0,"uart2 tx buf");
    if(tg_uart2_cb.send_buf_semp == NULL)
        goto exit_from_left_buf_semp;
    tg_uart2_cb.recv_buf_semp = semp_create(1,0,"uart2 rx buf");
    if(tg_uart2_cb.recv_buf_semp == NULL)
        goto exit_from_right_buf_semp;
    left_semp = semp_create(1,1,"uart2 device left");
    if(left_semp == NULL)
        goto exit_from_left_semp;
    right_semp = semp_create(1,1,"uart2 device right");
    if(right_semp == NULL)
        goto exit_from_right_semp;
    //初始化IO端口位uart功能
    //Ports  :  GPA10 GPA9 GPA8 GPA7 GPA6 GPA5 GPA4 GPA3 GPA2 GPA1 GPA0
    //Signal :   xx    xx  xx   xx   xx   xx   xx   RXD1 TXD1  xx   xx
    bb_rcc_apb1enr_uart2en = 1;     //uart2时钟使能
    gpio_power_on(cn_gpio_a);
    gpio_cfg_pin_func(cn_gpio_a,2,cn_gpio_mode_peri_out_pp_10Mhz);
    gpio_cfg_pin_func(cn_gpio_a,3,cn_gpio_mode_in_floating);
    //系统初始化时已经使中断处于禁止状态，无需再禁止和清除中断。
    //初始化uart硬件控制数据结构
    pg_uart2_reg->CR1 = 0x20ac;
    pg_uart2_reg->CR2 = 0x0;
    pg_uart2_reg->CR3 = 0x0000;
    temp1 = cn_pclk1/(16*115200);
    temp2 = cn_pclk1%(16*115200);
    temp2 = temp2/115200;
    pg_uart2_reg->BRR =  ((u16)temp1<<4)+(u16)temp2;
    tg_uart2_cb.baud = 115200;

    //初始化通用串口数据结构
    tg_uart2_cb.recv_trigger_level = uart2_buf_len>>4;  //缓冲接收触发水平为1/16
    tg_uart2_cb.send_trigger_level = uart2_buf_len>>4;  //缓冲发送触发水平为1/16
    tg_uart2_cb.timeout = 100*mS;   //超时时间=100
    tg_uart2_cb.sendding = false;
    ring_init(  &tg_uart2_cb.send_ring_buf,
                uart2_drv_send_buf,
                uart2_buf_len);
    ring_init(  &tg_uart2_cb.recv_ring_buf,
                uart2_drv_recv_buf,
                uart2_buf_len);

    //以下建立uart1设备
    uart_dev = dev_add_device(     dev_get_root_device(),"uart2",
                                   right_semp,left_semp,
                                   (dev_write_func) uart_hard_write,
                                   (dev_read_func ) uart_hard_read,
                                   (dev_ctrl_func ) uart2_ctrl,
                                   (dev_write_func ) uart2_app_write,
                                   (dev_read_func  ) uart2_app_read,
                                   (dev_ctrl_func  ) uart2_ctrl,
                                   (ptu32_t)(&tg_uart2_cb)
                                   );    //"uart2"是一个根设备
    if(uart_dev == NULL)
        goto exit_from_add_device;
    pg_uart2_rhdl = dev_open_right("uart2",0);      //打开右手句柄

    u16g_evtt_uart2_error = djy_evtt_regist(enum_correlative,
                            cn_prio_real,0,5,uart_error_service,10,"uart2 err");
    if(u16g_evtt_uart2_error == cn_invalid_evtt_id)
        goto exit_from_error_evtt;
    u16g_evtt_uart2_hard_recv = djy_evtt_regist(enum_correlative,cn_prio_real,0,
                                   5,uart2_hard_recv_service,32,"uart2 recv");
    if(u16g_evtt_uart2_hard_recv == cn_invalid_evtt_id)
        goto exit_from_hard_recv_evtt;

    djy_event_pop(u16g_evtt_uart2_hard_recv,enum_para_async,NULL,0,NULL,0,0,0);
    
    int_isr_connect(cn_int_line_USART2,uart2_int);
    int_setto_asyn_signal(cn_int_line_USART2);

//    bb_uart2_sr_txe = 0;
    int_echo_line(cn_int_line_USART2);
    int_restore_asyn_line(cn_int_line_USART2);
    return true;

exit_from_hard_recv_evtt:
    djy_evtt_unregist(u16g_evtt_uart2_error);
exit_from_error_evtt:
    dev_delete_device(uart_dev);
exit_from_add_device:
    semp_delete(right_semp);
exit_from_right_semp:
    semp_delete(left_semp);
exit_from_left_semp:
    semp_delete(tg_uart2_cb.recv_buf_semp);
exit_from_right_buf_semp:
    semp_delete(tg_uart2_cb.send_buf_semp);
exit_from_left_buf_semp:
    return false;
}

//----uart1中断---------------------------------------------------------------
//功能: 如果是接收信号触发的中断:
//      1.检查接收fifo中的数据量.把数据从接收寄存器全部copy到物理层缓冲区中.
//      2.如果物理层缓冲区满.置串口控制块状态字的溢出位,并发出uart错误事件.
//      3.发送串口接收事件.两个事件都是状态量事件
//      如果是发送信号引发的中断:
//      1.检查发送fifo有多少空位
//      2.从设备右手接口读出适量数据，
//      3.把读出的数据写入发送fifo
//      如果是出错信号引发中断:弹出串口出错事件
//参数: 中断函数没有参数.
//返回: 中断函数没有返回值.
//-----------------------------------------------------------------------------
uint32_t uart2_int(ufast_t uart_int_line)
{
    u16 num;
    u8 ch;
    enum uart_error_no parameter;
    if(bb_uart2_sr_rxne == 1)
    {
        ch = (u8)pg_uart2_reg->DR;
        num = dev_write_right(pg_uart2_rhdl,(ptu32_t)&ch,0,1);
        if(num != 1)
        {
            parameter = enum_recv_buf_over;
            djy_event_pop(u16g_evtt_uart2_error,false,NULL,0,&parameter,
                                sizeof(parameter),0,0);
        }else
        {
            djy_event_pop(u16g_evtt_uart2_hard_recv,false,NULL,0,NULL,0,0,0);
        }
    }
    if(bb_uart2_sr_txe == 1)
    {
        num = dev_read_right(pg_uart2_rhdl,0,(ptu32_t)&ch,1);
        if(num != 0)
            pg_uart2_reg->DR = ch;
        else
        {

            bb_uart2_cr1_txeie = 0;
            tg_uart2_cb.sendding = false;
        }
    }
    if(bb_uart2_sr_ore == 1)
    {
        ch = (u8)pg_uart2_reg->DR;
        num = pg_uart2_reg->SR;     //读一下sr寄存器
        parameter = enum_recv_hard_over;
        djy_event_pop(u16g_evtt_uart2_error,false,NULL,0,&parameter,
                            sizeof(parameter),0,0);
    }
    if(bb_uart2_sr_idle)
    {
        pg_uart2_reg->DR;
        int_tap_line(cn_int_line_DMA1_Ch6);
    }
    return 0;
}

//----dma1 ch4中断-------------------------------------------------------------
//功能: 当uart2配置成dma方式是，dma1 ch7负责发送，ch6负责接收
//参数: 中断函数没有参数.
//返回: 中断函数没有返回值.
//-----------------------------------------------------------------------------
uint32_t dma1ch7_int(ufast_t int_line)
{
    uint16_t num;
    pg_dma1_reg->IFCR |= 0x0f000000;
    num = dev_read_right(pg_uart2_rhdl,0,(ptu32_t)uart2_dma_send_buf,32);
    if(num == 0)
        tg_uart2_cb.sendding = false;
    else
    {
        bb_dma1_ch7_ccr_en = 0;    //暂停dma传输
        pg_dma1_channel7_reg->CMAR = (u32)uart2_dma_send_buf;
        pg_dma1_channel7_reg->CNDTR = num;
        bb_dma1_ch7_ccr_en = 1;    //继续dma传输
    }
    return 0;
}

//----dma1 ch5中断-------------------------------------------------------------
//功能: 当uart2配置成dma方式是，dma1 ch7负责发送，ch6负责接收
//参数: 中断函数没有参数.
//返回: 中断函数没有返回值.
//-----------------------------------------------------------------------------
uint32_t dma1ch6_int(ufast_t int_line)
{
    u32 recvs,num;
    enum uart_error_no parameter;
    pg_dma1_reg->IFCR |= 0x00f00000;
    bb_dma1_ch6_ccr_en = 0;    //暂停dma传输
    recvs = 32-pg_dma1_channel6_reg->CNDTR;
    if(recvs == 32)     //dma缓冲区满，可能有数据丢失
    {
        parameter = enum_recv_hard_over;
        djy_event_pop(u16g_evtt_uart2_error,false,NULL,0,&parameter,
                            sizeof(parameter),0,0);
    }else
    {
        num = dev_write_right(pg_uart2_rhdl,(ptu32_t)uart2_dma_recv_buf,0,recvs);
        if(num != recvs)
        {
            parameter = enum_recv_buf_over;
            djy_event_pop(u16g_evtt_uart2_error,false,NULL,0,&parameter,
                            sizeof(parameter),0,0);
        }else
            djy_event_pop(u16g_evtt_uart2_hard_recv,false,NULL,0,NULL,0,0,0);
    }
    pg_dma1_channel6_reg->CNDTR = 32;
    pg_dma1_channel6_reg->CMAR = (u32)uart2_dma_recv_buf;
    bb_dma1_ch6_ccr_en = 1;    //继续dma传输
    return 0;
}

//----设置不使用dma------------------------------------------------------------
//功能: uart2不使用dma收发，每收发一个字节就必须中断一次，不建议在高速连续通信
//      中使用。
//参数: 无
//返回: 无
//-----------------------------------------------------------------------------
void __uart2_set_dma_unused(void)
{
    pg_uart2_reg->CR3 &= ~0x00c0;     //设置不使用dma传输
    int_restore_asyn_line(cn_int_line_USART2);
    if(tg_uart2_cb.dma_used == cn_dma_used)
    {
        int_save_asyn_line(cn_int_line_DMA1_Ch6);
        int_save_asyn_line(cn_int_line_DMA1_Ch7);
        tg_uart2_cb.dma_used = cn_dma_unused;
    }
    bb_uart2_cr1_rxneie = 1;
    bb_uart2_cr1_txeie = 1;
    bb_uart2_cr1_idleie = 0;
}

//----设置使用dma------------------------------------------------------------
//功能: 设置uart2使用dma收发，dma4发，dma5收，将重新初始化所有dma寄存器，因为:
//      1、不使用dma期间可能有其他设备使用dma，从而更改了设置。
//      2、即使uart使用dma期间，也可能软件模块配合的问题导致其他设备使用dma.
//参数: 无
//返回: 无
//-----------------------------------------------------------------------------
void __uart2_set_dma_used(void)
{
    bb_rcc_ahbenr_dma1en = 1;
    bb_dma1_ch7_ccr_en = 0;    //停止dma通道
    bb_dma1_ch6_ccr_en = 0;    //停止dma通道
    bb_uart2_cr1_rxneie = 0;
    bb_uart2_cr1_txeie = 0;
    bb_uart2_cr1_idleie = 1;
    pg_dma1_reg->IFCR |= 0x0ff00000;        //清所有中断状态
    pg_dma1_channel7_reg->CCR = 0x00000092; //发送dma
    pg_dma1_channel7_reg->CPAR = (u32)&(pg_uart2_reg->DR);

    pg_dma1_channel6_reg->CCR = 0x00000086; //接收dma
    pg_dma1_channel6_reg->CPAR = (u32)&(pg_uart2_reg->DR);
    pg_dma1_channel6_reg->CNDTR = 32;
    pg_dma1_channel6_reg->CMAR = (u32)uart2_dma_recv_buf;

    int_isr_connect(cn_int_line_DMA1_Ch6,dma1ch6_int);
    int_isr_connect(cn_int_line_DMA1_Ch7,dma1ch7_int);

    int_setto_asyn_signal(cn_int_line_DMA1_Ch6);
    int_setto_asyn_signal(cn_int_line_DMA1_Ch7);
    int_restore_asyn_line(cn_int_line_DMA1_Ch6);
    int_restore_asyn_line(cn_int_line_DMA1_Ch7);

    pg_uart2_reg->CR3 |= 0x00c0;  //设置串口使用dma收发
    tg_uart2_cb.dma_used = cn_dma_used;
    bb_dma1_ch6_ccr_en = 1;    //启动dma通道
    uart2_first_dma_send = true;
}

//----暂停发送------------------------------------------------------------
//功能: 用于左手写设备过程中要操作发送缓冲区，防止发送中断访问发送缓冲区。
//参数: 无
//返回: 无
//-----------------------------------------------------------------------------
void __uart2_pause_send_int(void)
{
    if(tg_uart2_cb.dma_used == true)
    {
        int_save_asyn_line(cn_int_line_DMA1_Ch7);
    }else
    {
        int_save_asyn_line(cn_int_line_USART2);
    }
}

//----恢复发送------------------------------------------------------------
//功能: 用于左手写设备过程中操作发送缓冲区完成，可恢复发送
//参数: 无
//返回: 无
//-----------------------------------------------------------------------------
void __uart2_resume_send_int(void)
{
    if(tg_uart2_cb.dma_used == true)
    {
        int_restore_asyn_line(cn_int_line_DMA1_Ch7);
    }else
    {
        int_restore_asyn_line(cn_int_line_USART2);
    }
}

//----暂停接收------------------------------------------------------------
//功能: 用于左手写设备过程中要操作发送缓冲区，防止发送中断访问发送缓冲区。
//参数: 无
//返回: 无
//-----------------------------------------------------------------------------
void __uart2_pause_recv_int(void)
{
    if(tg_uart2_cb.dma_used == true)
    {
        int_save_asyn_line(cn_int_line_DMA1_Ch6);
    }else
    {
        int_save_asyn_line(cn_int_line_USART2);
    }
}

//----恢复接收------------------------------------------------------------
//功能: 用于左手写设备过程中操作发送缓冲区完成，可恢复发送
//参数: 无
//返回: 无
//-----------------------------------------------------------------------------
void __uart2_resume_recv_int(void)
{
    if(tg_uart2_cb.dma_used == true)
    {
        int_restore_asyn_line(cn_int_line_DMA1_Ch6);
    }else
    {
        int_restore_asyn_line(cn_int_line_USART2);
    }
}

//----启动发送------------------------------------------------------------
//功能: 左手写入数据后，如果发送器未工作，需要发送数据以启动发送器
//参数: 无
//返回: 无
//-----------------------------------------------------------------------------
void __uart2_start_send(void)
{
    u32 num,addr;
    if(tg_uart2_cb.dma_used == true)
    {
        num = dev_read_right(pg_uart2_rhdl,0,(ptu32_t)uart2_dma_send_buf,32);
        if(uart2_first_dma_send)
        {
            pg_uart2_reg->DR = uart2_dma_send_buf[0];
            uart2_first_dma_send = false;
            addr = (u32)uart2_dma_send_buf+1;
            num--;
        }else
            addr = (u32)uart2_dma_send_buf;

        if(num > 0)
        {
            bb_dma1_ch7_ccr_en = 0;    //暂停dma传输
            pg_dma1_channel7_reg->CMAR = addr;
            pg_dma1_channel7_reg->CNDTR = num;
            bb_dma1_ch7_ccr_en = 1;    //继续dma传输
            tg_uart2_cb.sendding = true;
        }
    }else
    {
        bb_uart2_cr1_txeie = 1;
        dev_read_right(pg_uart2_rhdl,0,(ptu32_t)&pg_uart2_reg->DR,1);
        tg_uart2_cb.sendding = true;
    }
}

//----启动串口2------------------------------------------------------------
//功能: 使串口开始工作
//参数: 无
//返回: 无
//-----------------------------------------------------------------------------
void __uart2_start(void)
{
    bb_rcc_apb1enr_uart2en = 1;    //uart2时钟使能
    bb_uart2_cr1_ue = 1;
}

//----停止串口2------------------------------------------------------------
//功能: 使串口停止工作
//参数: 无
//返回: 无
//-----------------------------------------------------------------------------
void __uart2_stop(void)
{
    bb_rcc_apb1enr_uart2en = 0;    //uart2时钟使能
    bb_uart2_cr1_ue = 0;
}

//----uart1设备左手写函数-------------------------------------------------------
//功能: 从左手写入uart设备,一般由uart的使用者调用.
//      1.把数据写入串口serial_DCB_t的环形发送缓冲区中.
//      2.如果串口硬件空闲,便把数据写入硬件发送缓冲区,并启动发送,否则不操作
//参数: uart_dev,被操作的设备的指针.
//      src_buf,数据源指针
//      len,要发送的序列长度
//返回: 实际写入环形缓冲区的字符数
//-----------------------------------------------------------------------------
ptu32_t uart2_app_write(struct  pan_device *uart_dev,ptu32_t src_buf,
                            ptu32_t res,ptu32_t len)
{
    uint32_t result;
    uint32_t completed = 0;

    if(len == 0)
        return 0;
    while(1)
    {
        __uart2_pause_send_int();
        result = ring_write(&tg_uart2_cb.send_ring_buf,
                            (uint8_t*)src_buf+completed,len-completed);
        if(tg_uart2_cb.sendding == false)
            __uart2_start_send();
        __uart2_resume_send_int();
        if(result != len-completed)     //缓冲区满，没有送出全部数据
        {
            completed += result;
            semp_pend(tg_uart2_cb.send_buf_semp,cn_timeout_forever);
        }else
            break;
    }
    return result;
}

//----串口2设备左手读函数------------------------------------------------------
//功能: 左手读用于上层设备,读取串口接收环形缓冲区的数据
//参数: uart_dev,被操作的设备的指针.
//      des_buf,读缓冲区
//      len,读入长度,
//返回: 实际读出长度
//----------------------------------------------------------------------------
ptu32_t uart2_app_read(struct  pan_device *uart_dev,ptu32_t res,
                                        ptu32_t des_buf,ptu32_t len)
{
    uint32_t result;
    __uart2_pause_recv_int();
    result = ring_read(&tg_uart2_cb.recv_ring_buf,(uint8_t*)des_buf,len);
    __uart2_resume_recv_int();
    return result;
}

//----串口2设备控制函数--------------------------------------------------------
//功能: 串口设备的控制函数
//参数: uart_dev,被操作的设备的指针.
//      cmd,操作类型
//      data,含义依cmd而定
//返回: 无意义.
//-----------------------------------------------------------------------------
ptu32_t uart2_ctrl(struct  pan_device *uart_dev,uint32_t cmd,
                   uint32_t data1,uint32_t data2)
{
    u16 temp1,temp2;

    switch(cmd)
    {
        case cn_uart_connect_recv_evtt:
            tg_uart2_cb.recv_target_evtt = (uint16_t)data1;
            break;
        case cn_uart_disconnect_recv_evtt:
            tg_uart2_cb.recv_target_evtt = cn_invalid_evtt_id;
            break;
        case cn_uart_start:
            __uart2_start();
            break;
        case cn_uart_stop:
            __uart2_stop();
            break;
        case cn_uart_set_baud:  //设置Baud
            if(tg_uart2_cb.baud !=data1)
            {
                temp1 = cn_pclk1/(16*data1);
                temp2 = cn_pclk1%(16*data1);
                temp2 = temp2/data1;
                pg_uart2_reg->BRR =  (temp1<<4)+temp2;
                tg_uart2_cb.baud = data1;
            }
            break;
        case cn_uart_half_duplex_send:     //启动发送
        //用IO控制半双工通信的发送使能的话,在此转换收发.
            break;
        case cn_uart_half_duplex_recv:     //启动接收
        //用IO控制半双工通信的发送使能的话,在此转换收发.
            break;
        case cn_uart_setto_all_duplex:
        //用IO控制半双工通信的发送使能的话,在此转换收发,注意要查询UTRSTAT寄存器
        //的Transmit shifter empty位判断是否已经完成.
            break;
        case cn_uart_overtime:   //data1为毫秒数
            tg_uart2_cb.timeout = data1;
            break;
        case cn_uart_recv_soft_level:    //设置接收软件缓冲区触发水平
            tg_uart2_cb.recv_trigger_level = (uint16_t)data1;
            break;
        case cn_uart_send_soft_level:    //设置接收软件缓冲区触发水平
            tg_uart2_cb.send_trigger_level = (uint16_t)data1;
            break;
        case cn_uart_en_rts:
            pg_uart2_reg->CR3 |= 0x100;
            break;
        case cn_uart_dis_rts:
            pg_uart2_reg->CR3 &= ~0x100;
            break;
        case cn_uart_en_cts:
            pg_uart2_reg->CR3 |= 0x200;
            break;
        case cn_uart_dis_cts:
            pg_uart2_reg->CR3 &= ~0x200;
            break;
        case cn_uart_dma_used:
            __uart2_set_dma_used();
            break;
        case cn_uart_dma_unused:
            __uart2_set_dma_unused();
            break;
        default: break;
    }
    return 0;
}

//----串行接收服务-------------------------------------------------------------
//功能: 事件服务函数,当串口硬件收到有效数据后，将收到的数据写入设备的同时，发送
//      本事件，事件的两个参数分别是:
//      1、所属串口的右手设备句柄.
//      2、本次接收到的数据数量
//参数: my_event，本事件指针.
//返回: 事件服务函数可以不返回,即使返回也没有返回值.
//-----------------------------------------------------------------------------
void uart2_hard_recv_service(void)
{
    uint32_t valid_bytes=0;
    u32 recvs;
    u32 wakeup;

    while(1)
    {
        //等待自身类型的事件再次发生，新事件在串口中断或dma中断服务函数中弹出。
        wakeup = djy_wait_evtt_pop(djy_my_evtt_id(),1,
                                    tg_uart2_cb.timeout);
        if(wakeup == cn_sync_timeout)
        {
            if(tg_uart2_cb.dma_used == true)
            {
                bb_dma1_ch6_ccr_en = 0;    //暂停dma传输
                recvs = 32-pg_dma1_channel6_reg->CNDTR;
                dev_write_right(pg_uart2_rhdl,(ptu32_t)uart2_dma_recv_buf,0,recvs);
                pg_dma1_channel6_reg->CNDTR = 32;
                pg_dma1_channel6_reg->CMAR = (u32)uart2_dma_recv_buf;
                bb_dma1_ch6_ccr_en = 1;    //继续dma传输
                valid_bytes += recvs;
            }
            if(valid_bytes != 0)
            {
                //如果不登记右手写事件，就只能查询方式从设备读取数据了。
                djy_event_pop(tg_uart2_cb.recv_target_evtt,
                                false,NULL,0,NULL,0,0,0);
                valid_bytes = 0;
            }
        }else
        {
            valid_bytes = ring_check(&tg_uart2_cb.recv_ring_buf);
            if(valid_bytes >= tg_uart2_cb.recv_trigger_level)
            {
                //如果不登记右手写事件，就只能查询方式从设备读取数据了。
                djy_event_pop(tg_uart2_cb.recv_target_evtt,
                                false,NULL,0,NULL,0,0,0);
                valid_bytes = 0;
            }
        }
    }
}

#endif  //for (cfg_uart2_used == 1)

#if(cfg_uart3_used == 1)
//----初始化uart0模块----------------------------------------------------------
//功能：初始化uart0模块，初始化状态为不使用dma传送的方式。
//参数：模块初始化函数没有参数
//返回：true = 成功初始化，false = 初始化失败
//-----------------------------------------------------------------------------
ptu32_t module_init_uart3(ptu32_t para)
{
    struct semaphore_LCB *left_semp,*right_semp;
    struct  pan_device   *uart_dev;
    uint32_t temp1,temp2;
    //保护缓冲区的信号量，使缓冲区中数据量为0时阻塞写入线程，读取线程使缓冲区中
    //数据降至trigger_level以下时释放信号量，使写入线程解除阻塞
    tg_uart3_cb.send_buf_semp = semp_create(1,0,"uart3 tx buf");
    if(tg_uart3_cb.send_buf_semp == NULL)
        goto exit_from_left_buf_semp;
    tg_uart3_cb.recv_buf_semp = semp_create(1,0,"uart3 rx buf");
    if(tg_uart3_cb.recv_buf_semp == NULL)
        goto exit_from_right_buf_semp;
    left_semp = semp_create(1,1,"uart3 device left");
    if(left_semp == NULL)
        goto exit_from_left_semp;
    right_semp = semp_create(1,1,"uart3 device right");
    if(right_semp == NULL)
        goto exit_from_right_semp;
    //初始化IO端口位uart功能
    bb_rcc_apb1enr_uart3en = 1;     //uart3时钟使能
    gpio_power_on(cn_gpio_b);
    gpio_cfg_pin_func(cn_gpio_b,10,cn_gpio_mode_peri_out_pp_10Mhz);
    gpio_cfg_pin_func(cn_gpio_b,11,cn_gpio_mode_in_floating);
    //系统初始化时已经使中断处于禁止状态，无需再禁止和清除中断。
    //初始化uart硬件控制数据结构
    pg_uart3_reg->CR1 = 0x20ac;
    pg_uart3_reg->CR2 = 0x0;
    pg_uart3_reg->CR3 = 0x0000;
    temp1 = cn_pclk1/(16*115200);
    temp2 = cn_pclk1%(16*115200);
    temp2 = temp2/115200;
    pg_uart3_reg->BRR =  ((u16)temp1<<4)+(u16)temp2;
    tg_uart3_cb.baud = 115200;

    //初始化通用串口数据结构
    tg_uart3_cb.recv_trigger_level = uart3_buf_len>>4;  //缓冲接收触发水平为1/16
    tg_uart3_cb.send_trigger_level = uart3_buf_len>>4;  //缓冲发送触发水平为1/16
    tg_uart3_cb.timeout = 100*mS;   //超时时间=100
    tg_uart3_cb.sendding = false;
    ring_init(  &tg_uart3_cb.send_ring_buf,
                uart3_drv_send_buf,
                uart3_buf_len);
    ring_init(  &tg_uart3_cb.recv_ring_buf,
                uart3_drv_recv_buf,
                uart3_buf_len);

    //以下建立uart1设备
    uart_dev = dev_add_device(     dev_get_root_device(),"uart3",
                                   right_semp,left_semp,
                                   (dev_write_func) uart_hard_write,
                                   (dev_read_func ) uart_hard_read,
                                   (dev_ctrl_func ) uart3_ctrl,
                                   (dev_write_func ) uart3_app_write,
                                   (dev_read_func  ) uart3_app_read,
                                   (dev_ctrl_func  ) uart3_ctrl,
                                   (ptu32_t)(&tg_uart3_cb)
                                   );    //"uart3"是一个根设备
    if(uart_dev == NULL)
        goto exit_from_add_device;
    pg_uart3_rhdl = dev_open_right("uart3",0);      //打开右手句柄
    u16g_evtt_uart3_error = djy_evtt_regist(enum_correlative,
                            cn_prio_real,0,5,uart_error_service,10,"uart3 err");
    if(u16g_evtt_uart3_error == cn_invalid_evtt_id)
        goto exit_from_error_evtt;
    u16g_evtt_uart3_hard_recv = djy_evtt_regist(enum_correlative,cn_prio_real,0,
                                   5,uart3_hard_recv_service,32,"uart3 recv");
    if(u16g_evtt_uart3_hard_recv == cn_invalid_evtt_id)
        goto exit_from_hard_recv_evtt;

    djy_event_pop(u16g_evtt_uart3_hard_recv,enum_para_async,NULL,0,NULL,0,0,0);

    int_isr_connect(cn_int_line_USART3,uart3_int);
    int_setto_asyn_signal(cn_int_line_USART3);

//    bb_uart3_sr_txe = 0;
    int_echo_line(cn_int_line_USART3);
    int_restore_asyn_line(cn_int_line_USART3);
    return true;

exit_from_hard_recv_evtt:
    djy_evtt_unregist(u16g_evtt_uart3_error);
exit_from_error_evtt:
    dev_delete_device(uart_dev);
exit_from_add_device:
    semp_delete(right_semp);
exit_from_right_semp:
    semp_delete(left_semp);
exit_from_left_semp:
    semp_delete(tg_uart3_cb.recv_buf_semp);
exit_from_right_buf_semp:
    semp_delete(tg_uart3_cb.send_buf_semp);
exit_from_left_buf_semp:
    return false;
}

//----uart1中断---------------------------------------------------------------
//功能: 如果是接收信号触发的中断:
//      1.检查接收fifo中的数据量.把数据从接收寄存器全部copy到物理层缓冲区中.
//      2.如果物理层缓冲区满.置串口控制块状态字的溢出位,并发出uart错误事件.
//      3.发送串口接收事件.两个事件都是状态量事件
//      如果是发送信号引发的中断:
//      1.检查发送fifo有多少空位
//      2.从设备右手接口读出适量数据，
//      3.把读出的数据写入发送fifo
//      如果是出错信号引发中断:弹出串口出错事件
//参数: 中断函数没有参数.
//返回: 中断函数没有返回值.
//-----------------------------------------------------------------------------
uint32_t uart3_int(ufast_t uart_int_line)
{
    u16 num;
    u8 ch;
    enum uart_error_no parameter;
    if(bb_uart3_sr_rxne == 1)
    {
        ch = (u8)pg_uart3_reg->DR;
        num = dev_write_right(pg_uart3_rhdl,(ptu32_t)&ch,0,1);
        if(num != 1)
        {
            parameter = enum_recv_buf_over;
            djy_event_pop(u16g_evtt_uart3_error,false,NULL,0,&parameter,
                                sizeof(parameter),0,0);
        }else
        {
            djy_event_pop(u16g_evtt_uart3_hard_recv,false,NULL,0,NULL,0,0,0);
        }
    }
    if(bb_uart3_sr_txe == 1)
    {
        num = dev_read_right(pg_uart3_rhdl,0,(ptu32_t)&ch,1);
        if(num != 0)
            pg_uart3_reg->DR = ch;
        else
        {

            bb_uart3_cr1_txeie = 0;
            tg_uart3_cb.sendding = false;
        }
    }
    if(bb_uart3_sr_ore == 1)
    {
        ch = (u8)pg_uart3_reg->DR;
        num = pg_uart3_reg->SR;     //读一下sr寄存器
        parameter = enum_recv_hard_over;
        djy_event_pop(u16g_evtt_uart3_error,false,NULL,0,&parameter,
                            sizeof(parameter),0,0);
    }
    if(bb_uart3_sr_idle)
    {
        pg_uart3_reg->DR;
        int_tap_line(cn_int_line_DMA1_Ch3);
    }
    return 0;
}

//----dma1 ch2中断-------------------------------------------------------------
//功能: 当uart3配置成dma方式是，dma1 ch2负责发送，ch3负责接收
//参数: 中断函数没有参数.
//返回: 中断函数没有返回值.
//-----------------------------------------------------------------------------
uint32_t dma1ch2_int(ufast_t int_line)
{
    uint16_t num;
    pg_dma1_reg->IFCR |= 0x000000f0;
    num = dev_read_right(pg_uart3_rhdl,0,(ptu32_t)uart3_dma_send_buf,32);
    if(num == 0)
        tg_uart3_cb.sendding = false;
    else
    {
        bb_dma1_ch2_ccr_en = 0;    //暂停dma传输
        pg_dma1_channel2_reg->CMAR = (u32)uart3_dma_send_buf;
        pg_dma1_channel2_reg->CNDTR = num;
        bb_dma1_ch2_ccr_en = 1;    //继续dma传输
    }
    return 0;
}

//----dma1 ch3中断-------------------------------------------------------------
//功能: 当uart3配置成dma方式是，dma1 ch2负责发送，ch3负责接收
//参数: 中断函数没有参数.
//返回: 中断函数没有返回值.
//-----------------------------------------------------------------------------
uint32_t dma1ch3_int(ufast_t int_line)
{
    u32 recvs,num;
    enum uart_error_no parameter;
    pg_dma1_reg->IFCR |= 0x00000f00;
    bb_dma1_ch3_ccr_en = 0;    //暂停dma传输
    recvs = 32-pg_dma1_channel3_reg->CNDTR;
    if(recvs == 32)     //dma缓冲区满，可能有数据丢失
    {
        parameter = enum_recv_hard_over;
        djy_event_pop(u16g_evtt_uart3_error,false,NULL,0,&parameter,
                            sizeof(parameter),0,0);
    }else
    {
        num = dev_write_right(pg_uart3_rhdl,(ptu32_t)uart3_dma_recv_buf,0,recvs);
        if(num != recvs)
        {
            parameter = enum_recv_buf_over;
            djy_event_pop(u16g_evtt_uart3_error,false,NULL,0,&parameter,
                            sizeof(parameter),0,0);
        }else
            djy_event_pop(u16g_evtt_uart3_hard_recv,false,NULL,0,NULL,0,0,0);
    }
    pg_dma1_channel3_reg->CNDTR = 32;
    pg_dma1_channel3_reg->CMAR = (u32)uart3_dma_recv_buf;
    bb_dma1_ch3_ccr_en = 1;    //继续dma传输
    return 0;
}

//----设置不使用dma------------------------------------------------------------
//功能: uart3不使用dma收发，每收发一个字节就必须中断一次，不建议在高速连续通信
//      中使用。
//参数: 无
//返回: 无
//-----------------------------------------------------------------------------
void __uart3_set_dma_unused(void)
{
    pg_uart3_reg->CR3 &= ~0x00c0;     //设置不使用dma传输
    int_restore_asyn_line(cn_int_line_USART1);
    if(tg_uart3_cb.dma_used == cn_dma_used)
    {
        int_save_asyn_line(cn_int_line_DMA1_Ch2);
        int_save_asyn_line(cn_int_line_DMA1_Ch3);
        tg_uart3_cb.dma_used = cn_dma_unused;
    }
    bb_uart3_cr1_rxneie = 1;
    bb_uart3_cr1_txeie = 1;
    bb_uart3_cr1_idleie = 0;
}

//----设置使用dma------------------------------------------------------------
//功能: 设置uart3使用dma收发，dma4发，dma5收，将重新初始化所有dma寄存器，因为:
//      1、不使用dma期间可能有其他设备使用dma，从而更改了设置。
//      2、即使uart使用dma期间，也可能软件模块配合的问题导致其他设备使用dma.
//参数: 无
//返回: 无
//-----------------------------------------------------------------------------
void __uart3_set_dma_used(void)
{
    bb_rcc_ahbenr_dma1en = 1;
    bb_dma1_ch2_ccr_en = 0;    //停止dma通道
    bb_dma1_ch3_ccr_en = 0;    //停止dma通道
    bb_uart3_cr1_rxneie = 0;
    bb_uart3_cr1_txeie = 0;
    bb_uart3_cr1_idleie = 1;
    pg_dma1_reg->IFCR |= 0x000ff000;        //清所有中断状态
    pg_dma1_channel2_reg->CCR = 0x00000092; //发送dma
    pg_dma1_channel2_reg->CPAR = (u32)&(pg_uart3_reg->DR);

    pg_dma1_channel3_reg->CCR = 0x00000086; //接收dma
    pg_dma1_channel3_reg->CPAR = (u32)&(pg_uart3_reg->DR);
    pg_dma1_channel3_reg->CNDTR = 32;
    pg_dma1_channel3_reg->CMAR = (u32)uart3_dma_recv_buf;

    int_isr_connect(cn_int_line_DMA1_Ch2,dma1ch2_int);
    int_isr_connect(cn_int_line_DMA1_Ch3,dma1ch3_int);

    int_setto_asyn_signal(cn_int_line_DMA1_Ch2);
    int_setto_asyn_signal(cn_int_line_DMA1_Ch3);
    int_restore_asyn_line(cn_int_line_DMA1_Ch2);
    int_restore_asyn_line(cn_int_line_DMA1_Ch3);

    pg_uart3_reg->CR3 |= 0x00c0;  //设置串口使用dma收发
    tg_uart3_cb.dma_used = cn_dma_used;
    bb_dma1_ch3_ccr_en = 1;    //启动dma通道
    uart3_first_dma_send = true;
}

//----暂停发送------------------------------------------------------------
//功能: 用于左手写设备过程中要操作发送缓冲区，防止发送中断访问发送缓冲区。
//参数: 无
//返回: 无
//-----------------------------------------------------------------------------
void __uart3_pause_send_int(void)
{
    if(tg_uart3_cb.dma_used == true)
    {
        int_save_asyn_line(cn_int_line_DMA1_Ch2);
    }else
    {
        int_save_asyn_line(cn_int_line_USART3);
    }
}

//----恢复发送------------------------------------------------------------
//功能: 用于左手写设备过程中操作发送缓冲区完成，可恢复发送
//参数: 无
//返回: 无
//-----------------------------------------------------------------------------
void __uart3_resume_send_int(void)
{
    if(tg_uart3_cb.dma_used == true)
    {
        int_restore_asyn_line(cn_int_line_DMA1_Ch2);
    }else
    {
        int_restore_asyn_line(cn_int_line_USART3);
    }
}

//----暂停接收------------------------------------------------------------
//功能: 用于左手写设备过程中要操作发送缓冲区，防止发送中断访问发送缓冲区。
//参数: 无
//返回: 无
//-----------------------------------------------------------------------------
void __uart3_pause_recv_int(void)
{
    if(tg_uart3_cb.dma_used == true)
    {
        int_save_asyn_line(cn_int_line_DMA1_Ch3);
    }else
    {
        int_save_asyn_line(cn_int_line_USART3);
    }
}

//----恢复接收------------------------------------------------------------
//功能: 用于左手写设备过程中操作发送缓冲区完成，可恢复发送
//参数: 无
//返回: 无
//-----------------------------------------------------------------------------
void __uart3_resume_recv_int(void)
{
    if(tg_uart3_cb.dma_used == true)
    {
        int_restore_asyn_line(cn_int_line_DMA1_Ch3);
    }else
    {
        int_restore_asyn_line(cn_int_line_USART3);
    }
}

//----启动发送------------------------------------------------------------
//功能: 左手写入数据后，如果发送器未工作，需要发送数据以启动发送器
//参数: 无
//返回: 无
//-----------------------------------------------------------------------------
void __uart3_start_send(void)
{
    u32 num,addr;
    if(tg_uart3_cb.dma_used == true)
    {
        num = dev_read_right(pg_uart3_rhdl,0,(ptu32_t)uart3_dma_send_buf,32);
        if(uart3_first_dma_send)
        {
            pg_uart3_reg->DR = uart3_dma_send_buf[0];
            uart3_first_dma_send = false;
            addr = (u32)uart3_dma_send_buf+1;
            num--;
        }else
            addr = (u32)uart3_dma_send_buf;

        if(num > 0)
        {
            bb_dma1_ch2_ccr_en = 0;    //暂停dma传输
            pg_dma1_channel2_reg->CMAR = addr;
            pg_dma1_channel2_reg->CNDTR = num;
            bb_dma1_ch2_ccr_en = 1;    //继续dma传输
            tg_uart3_cb.sendding = true;
        }
    }else
    {
        bb_uart3_cr1_txeie = 1;
        dev_read_right(pg_uart3_rhdl,0,(ptu32_t)&pg_uart3_reg->DR,1);
        tg_uart3_cb.sendding = true;
    }
}

//----启动串口1------------------------------------------------------------
//功能: 使串口开始工作
//参数: 无
//返回: 无
//-----------------------------------------------------------------------------
void __uart3_start(void)
{
    bb_rcc_apb1enr_uart3en = 1;    //uart3时钟使能
    bb_uart3_cr1_ue = 1;
}

//----停止串口1------------------------------------------------------------
//功能: 使串口停止工作
//参数: 无
//返回: 无
//-----------------------------------------------------------------------------
void __uart3_stop(void)
{
    bb_rcc_apb1enr_uart3en = 0;    //uart3时钟使能
    bb_uart3_cr1_ue = 0;
}

//----uart1设备左手写函数-------------------------------------------------------
//功能: 从左手写入uart设备,一般由uart的使用者调用.
//      1.把数据写入串口serial_DCB_t的环形发送缓冲区中.
//      2.如果串口硬件空闲,便把数据写入硬件发送缓冲区,并启动发送,否则不操作
//参数: uart_dev,被操作的设备的指针.
//      src_buf,数据源指针
//      len,要发送的序列长度
//返回: 实际写入环形缓冲区的字符数
//-----------------------------------------------------------------------------
ptu32_t uart3_app_write(struct  pan_device *uart_dev,ptu32_t src_buf,
                            ptu32_t res,ptu32_t len)
{
    uint32_t result;
    uint32_t completed = 0;

    if(len == 0)
        return 0;
    while(1)
    {
        __uart3_pause_send_int();
        result = ring_write(&tg_uart3_cb.send_ring_buf,
                            (uint8_t*)src_buf+completed,len-completed);
        if(tg_uart3_cb.sendding == false)
            __uart3_start_send();
        __uart3_resume_send_int();
        if(result != len-completed)     //缓冲区满，没有送出全部数据
        {
            completed += result;
            semp_pend(tg_uart3_cb.send_buf_semp,cn_timeout_forever);
        }else
            break;
    }
    return result;
}

//----串口1设备左手读函数------------------------------------------------------
//功能: 左手读用于上层设备,读取串口接收环形缓冲区的数据
//参数: uart_dev,被操作的设备的指针.
//      des_buf,读缓冲区
//      len,读入长度,
//返回: 实际读出长度
//----------------------------------------------------------------------------
ptu32_t uart3_app_read(struct  pan_device *uart_dev,ptu32_t res,
                                        ptu32_t des_buf,ptu32_t len)
{
    uint32_t result;
    __uart3_pause_recv_int();
    result = ring_read(&tg_uart3_cb.recv_ring_buf,(uint8_t*)des_buf,len);
    __uart3_resume_recv_int();
    return result;
}

//----串口1设备控制函数--------------------------------------------------------
//功能: 串口设备的控制函数
//参数: uart_dev,被操作的设备的指针.
//      cmd,操作类型
//      data,含义依cmd而定
//返回: 无意义.
//-----------------------------------------------------------------------------
ptu32_t uart3_ctrl(struct  pan_device *uart_dev,uint32_t cmd,
                   uint32_t data1,uint32_t data2)
{
    u16 temp1,temp2;

    switch(cmd)
    {
        case cn_uart_connect_recv_evtt:
            tg_uart3_cb.recv_target_evtt = (uint16_t)data1;
            break;
        case cn_uart_disconnect_recv_evtt:
            tg_uart3_cb.recv_target_evtt = cn_invalid_evtt_id;
            break;
        case cn_uart_start:
            __uart3_start();
            break;
        case cn_uart_stop:
            __uart3_stop();
            break;
        case cn_uart_set_baud:  //设置Baud
            if(tg_uart3_cb.baud !=data1)
            {
                temp1 = cn_pclk1/(16*data1);
                temp2 = cn_pclk1%(16*data1);
                temp2 = temp2/data1;
                pg_uart3_reg->BRR =  (temp1<<4)+temp2;
                tg_uart3_cb.baud = data1;
            }
            break;
        case cn_uart_half_duplex_send:     //启动发送
        //用IO控制半双工通信的发送使能的话,在此转换收发.
            break;
        case cn_uart_half_duplex_recv:     //启动接收
        //用IO控制半双工通信的发送使能的话,在此转换收发.
            break;
        case cn_uart_setto_all_duplex:
        //用IO控制半双工通信的发送使能的话,在此转换收发,注意要查询UTRSTAT寄存器
        //的Transmit shifter empty位判断是否已经完成.
            break;
        case cn_uart_overtime:   //data1为毫秒数
            tg_uart3_cb.timeout = data1;
            break;
        case cn_uart_recv_soft_level:    //设置接收软件缓冲区触发水平
            tg_uart3_cb.recv_trigger_level = (uint16_t)data1;
            break;
        case cn_uart_send_soft_level:    //设置接收软件缓冲区触发水平
            tg_uart3_cb.send_trigger_level = (uint16_t)data1;
            break;
        case cn_uart_en_rts:
            pg_uart3_reg->CR3 |= 0x100;
            break;
        case cn_uart_dis_rts:
            pg_uart3_reg->CR3 &= ~0x100;
            break;
        case cn_uart_en_cts:
            pg_uart3_reg->CR3 |= 0x200;
            break;
        case cn_uart_dis_cts:
            pg_uart3_reg->CR3 &= ~0x200;
            break;
        case cn_uart_dma_used:
            __uart3_set_dma_used();
            break;
        case cn_uart_dma_unused:
            __uart3_set_dma_unused();
            break;
        default: break;
    }
    return 0;
}

//----串行接收服务-------------------------------------------------------------
//功能: 事件服务函数,当串口硬件收到有效数据后，将收到的数据写入设备的同时，发送
//      本事件，事件的两个参数分别是:
//      1、所属串口的右手设备句柄.
//      2、本次接收到的数据数量
//参数: my_event，本事件指针.
//返回: 事件服务函数可以不返回,即使返回也没有返回值.
//-----------------------------------------------------------------------------
void uart3_hard_recv_service(void)
{
    uint32_t valid_bytes=0;
    u32 recvs;
    u32 wakeup;

    while(1)
    {
        //等待自身类型的事件再次发生，新事件在串口中断或dma中断服务函数中弹出。
        wakeup = djy_wait_evtt_pop(djy_my_evtt_id(),1,
                                    tg_uart3_cb.timeout);
        if(wakeup == cn_sync_timeout)
        {
            if(tg_uart3_cb.dma_used == true)
            {
                bb_dma1_ch3_ccr_en = 0;    //暂停dma传输
                recvs = 32-pg_dma1_channel3_reg->CNDTR;
                dev_write_right(pg_uart3_rhdl,(ptu32_t)uart3_dma_recv_buf,0,recvs);
                pg_dma1_channel3_reg->CNDTR = 32;
                pg_dma1_channel3_reg->CMAR = (u32)uart3_dma_recv_buf;
                bb_dma1_ch3_ccr_en = 1;    //继续dma传输
                valid_bytes += recvs;
            }
            if(valid_bytes != 0)
            {
                //如果不登记右手写事件，就只能查询方式从设备读取数据了。
                djy_event_pop(tg_uart3_cb.recv_target_evtt,
                                false,NULL,0,NULL,0,0,0);
                valid_bytes = 0;
            }
        }else
        {
            valid_bytes = ring_check(&tg_uart3_cb.recv_ring_buf);
            if(valid_bytes >= tg_uart3_cb.recv_trigger_level)
            {
                //如果不登记右手写事件，就只能查询方式从设备读取数据了。
                djy_event_pop(tg_uart3_cb.recv_target_evtt,
                                false,NULL,0,NULL,0,0,0);
                valid_bytes = 0;
            }
        }
    }
}

#endif  //for (cfg_uart3_used == 1)

#if(cfg_uart4_used == 1)
//----初始化uart4模块----------------------------------------------------------
//功能：初始化uart4模块，初始化状态为不使用dma传送的方式。
//参数：模块初始化函数没有参数
//返回：true = 成功初始化，false = 初始化失败
//-----------------------------------------------------------------------------
ptu32_t module_init_uart4(ptu32_t para)
{
    struct semaphore_LCB *left_semp,*right_semp;
    struct  pan_device   *uart_dev;
    uint32_t temp1,temp2;
    //保护缓冲区的信号量，使缓冲区中数据量为0时阻塞写入线程，读取线程使缓冲区中
    //数据降至trigger_level以下时释放信号量，使写入线程解除阻塞
    tg_uart4_cb.send_buf_semp = semp_create(1,0,"uart4 tx buf");
    if(tg_uart4_cb.send_buf_semp == NULL)
        goto exit_from_left_buf_semp;
    tg_uart4_cb.recv_buf_semp = semp_create(1,0,"uart4 rx buf");
    if(tg_uart4_cb.recv_buf_semp == NULL)
        goto exit_from_right_buf_semp;
    left_semp = semp_create(1,1,"uart4 device left");
    if(left_semp == NULL)
        goto exit_from_left_semp;
    right_semp = semp_create(1,1,"uart4 device right");
    if(right_semp == NULL)
        goto exit_from_right_semp;
    //初始化IO端口位uart功能
    bb_rcc_apb1enr_uart4en = 1;     //uart4时钟使能
    gpio_power_on(cn_gpio_c);
    gpio_cfg_pin_func(cn_gpio_c,10,cn_gpio_mode_peri_out_pp_10Mhz);
    gpio_cfg_pin_func(cn_gpio_c,11,cn_gpio_mode_in_floating);
    //系统初始化时已经使中断处于禁止状态，无需再禁止和清除中断。
    //初始化uart硬件控制数据结构
    pg_uart4_reg->CR1 = 0x20ac;
    pg_uart4_reg->CR2 = 0x0;
    pg_uart4_reg->CR3 = 0x0000;
    temp1 = cn_pclk1/(16*115200);
    temp2 = cn_pclk1%(16*115200);
    temp2 = temp2/115200;
    pg_uart4_reg->BRR =  ((u16)temp1<<4)+(u16)temp2;
    tg_uart4_cb.baud = 115200;

    //初始化通用串口数据结构
    tg_uart4_cb.recv_trigger_level = uart4_buf_len>>4;  //缓冲接收触发水平为1/16
    tg_uart4_cb.send_trigger_level = uart4_buf_len>>4;  //缓冲发送触发水平为1/16
    tg_uart4_cb.timeout = 100*mS;   //超时时间=100
    tg_uart4_cb.sendding = false;
    ring_init(  &tg_uart4_cb.send_ring_buf,
                uart4_drv_send_buf,
                uart4_buf_len);
    ring_init(  &tg_uart4_cb.recv_ring_buf,
                uart4_drv_recv_buf,
                uart4_buf_len);

    //以下建立uart1设备
    uart_dev = dev_add_device(     dev_get_root_device(),"uart4",
                                   right_semp,left_semp,
                                   (dev_write_func) uart_hard_write,
                                   (dev_read_func ) uart_hard_read,
                                   (dev_ctrl_func ) uart4_ctrl,
                                   (dev_write_func ) uart4_app_write,
                                   (dev_read_func  ) uart4_app_read,
                                   (dev_ctrl_func  ) uart4_ctrl,
                                   (ptu32_t)(&tg_uart4_cb)
                                   );    //"uart4"是一个根设备
    if(uart_dev == NULL)
        goto exit_from_add_device;
    pg_uart4_rhdl = dev_open_right("uart4",0);      //打开右手句柄
    u16g_evtt_uart4_error = djy_evtt_regist(enum_correlative,
                            cn_prio_real,0,5,uart_error_service,10,"uart4 err");
    if(u16g_evtt_uart4_error == cn_invalid_evtt_id)
        goto exit_from_error_evtt;
    u16g_evtt_uart4_hard_recv = djy_evtt_regist(enum_correlative,cn_prio_real,0,
                                   5,uart4_hard_recv_service,32,"uart4 recv");
    if(u16g_evtt_uart4_hard_recv == cn_invalid_evtt_id)
        goto exit_from_hard_recv_evtt;

    djy_event_pop(u16g_evtt_uart4_hard_recv,enum_para_async,NULL,0,NULL,0,0,0);

    int_isr_connect(cn_int_line_UART4,uart4_int);
    int_setto_asyn_signal(cn_int_line_UART4);

//    bb_uart4_sr_txe = 0;
    int_echo_line(cn_int_line_UART4);
    int_restore_asyn_line(cn_int_line_UART4);
    
    return true;

exit_from_hard_recv_evtt:
    djy_evtt_unregist(u16g_evtt_uart4_error);
exit_from_error_evtt:
    dev_delete_device(uart_dev);
exit_from_add_device:
    semp_delete(right_semp);
exit_from_right_semp:
    semp_delete(left_semp);
exit_from_left_semp:
    semp_delete(tg_uart4_cb.recv_buf_semp);
exit_from_right_buf_semp:
    semp_delete(tg_uart4_cb.send_buf_semp);
exit_from_left_buf_semp:
    return false;
}

//----uart4中断---------------------------------------------------------------
//功能: 如果是接收信号触发的中断:
//      1.检查接收fifo中的数据量.把数据从接收寄存器全部copy到物理层缓冲区中.
//      2.如果物理层缓冲区满.置串口控制块状态字的溢出位,并发出uart错误事件.
//      3.发送串口接收事件.两个事件都是状态量事件
//      如果是发送信号引发的中断:
//      1.检查发送fifo有多少空位
//      2.从设备右手接口读出适量数据，
//      3.把读出的数据写入发送fifo
//      如果是出错信号引发中断:弹出串口出错事件
//参数: 中断函数没有参数.
//返回: 中断函数没有返回值.
//-----------------------------------------------------------------------------
uint32_t uart4_int(ufast_t uart_int_line)
{
    u16 num;
    u8 ch;
    enum uart_error_no parameter;
    if(bb_uart4_sr_rxne == 1)
    {
        ch = (u8)pg_uart4_reg->DR;
        num = dev_write_right(pg_uart4_rhdl,(ptu32_t)&ch,0,1);
        if(num != 1)
        {
            parameter = enum_recv_buf_over;
            djy_event_pop(u16g_evtt_uart4_error,false,NULL,0,&parameter,
                                sizeof(parameter),0,0);
        }else
        {
            djy_event_pop(u16g_evtt_uart4_hard_recv,false,NULL,0,NULL,0,0,0);
        }
    }
    if(bb_uart4_sr_txe == 1)
    {
        num = dev_read_right(pg_uart4_rhdl,0,(ptu32_t)&ch,1);
        if(num != 0)
            pg_uart4_reg->DR = ch;
        else
        {

            bb_uart4_cr1_txeie = 0;
            tg_uart4_cb.sendding = false;
        }
    }
    if(bb_uart4_sr_ore == 1)
    {
        ch = (u8)pg_uart4_reg->DR;
        num = pg_uart4_reg->SR;     //读一下sr寄存器
        parameter = enum_recv_hard_over;
        djy_event_pop(u16g_evtt_uart4_error,false,NULL,0,&parameter,
                            sizeof(parameter),0,0);
    }
    if(bb_uart4_sr_idle)
    {
        pg_uart4_reg->DR;
        int_tap_line(cn_int_line_DMA2_Ch3);
    }
    return 0;
}

//----dma2 ch5中断-------------------------------------------------------------
//功能: 当uart4配置成dma方式是，dma2 ch5负责发送，ch3负责接收
//参数: 中断函数没有参数.
//返回: 中断函数没有返回值.
//-----------------------------------------------------------------------------
uint32_t dma2ch5_int(ufast_t int_line)
{
    uint16_t num;
    if(bb_dma2_isr_tcif5 == 0)
    {
        pg_dma2_reg->IFCR |= 0x0000f000;    //是ch4产生的中断
        return 0;
    }
    pg_dma2_reg->IFCR |= 0x000f0000;
    num = dev_read_right(pg_uart4_rhdl,0,(ptu32_t)uart4_dma_send_buf,32);
    if(num == 0)
        tg_uart4_cb.sendding = false;
    else
    {
        bb_dma2_ch5_ccr_en = 0;    //暂停dma传输
        pg_dma2_channel5_reg->CMAR = (u32)uart4_dma_send_buf;
        pg_dma2_channel5_reg->CNDTR = num;
        bb_dma2_ch5_ccr_en = 1;    //继续dma传输
    }
    return 0;
}

//----dma2 ch3中断-------------------------------------------------------------
//功能: 当uart4配置成dma方式是，dma2 ch5负责发送，ch3负责接收
//参数: 中断函数没有参数.
//返回: 中断函数没有返回值.
//-----------------------------------------------------------------------------
uint32_t dma2ch3_int(ufast_t int_line)
{
    u32 recvs,num;
    enum uart_error_no parameter;
    pg_dma2_reg->IFCR |= 0x00000f00;
    bb_dma2_ch3_ccr_en = 0;    //暂停dma传输
    recvs = 32-pg_dma2_channel3_reg->CNDTR;
    if(recvs == 32)     //dma缓冲区满，可能有数据丢失
    {
        parameter = enum_recv_hard_over;
        djy_event_pop(u16g_evtt_uart4_error,false,NULL,0,&parameter,
                            sizeof(parameter),0,0);
    }else
    {
        num = dev_write_right(pg_uart4_rhdl,(ptu32_t)uart4_dma_recv_buf,0,recvs);
        if(num != recvs)
        {
            parameter = enum_recv_buf_over;
            djy_event_pop(u16g_evtt_uart4_error,false,NULL,0,&parameter,
                            sizeof(parameter),0,0);
        }else
            djy_event_pop(u16g_evtt_uart4_hard_recv,false,NULL,0,NULL,0,0,0);
    }
    pg_dma2_channel3_reg->CNDTR = 32;
    pg_dma2_channel3_reg->CMAR = (u32)uart4_dma_recv_buf;
    bb_dma2_ch3_ccr_en = 1;    //继续dma传输
    return 0;
}

//----设置不使用dma------------------------------------------------------------
//功能: uart4不使用dma收发，每收发一个字节就必须中断一次，不建议在高速连续通信
//      中使用。
//参数: 无
//返回: 无
//-----------------------------------------------------------------------------
void __uart4_set_dma_unused(void)
{
    pg_uart4_reg->CR3 &= ~0x00c0;     //设置不使用dma传输
    int_restore_asyn_line(cn_int_line_UART4);
    if(tg_uart4_cb.dma_used == cn_dma_used)
    {
        int_save_asyn_line(cn_int_line_DMA2_Ch3);
        int_save_asyn_line(cn_int_line_DMA2_Ch4_5);
        tg_uart4_cb.dma_used = cn_dma_unused;
    }
    bb_uart4_cr1_rxneie = 1;
    bb_uart4_cr1_txeie = 1;
    bb_uart4_cr1_idleie = 0;
}

//----设置使用dma------------------------------------------------------------
//功能: 设置uart4使用dma收发，dma4发，dma5收，将重新初始化所有dma寄存器，因为:
//      1、不使用dma期间可能有其他设备使用dma，从而更改了设置。
//      2、即使uart使用dma期间，也可能软件模块配合的问题导致其他设备使用dma.
//参数: 无
//返回: 无
//-----------------------------------------------------------------------------
void __uart4_set_dma_used(void)
{
    bb_rcc_ahbenr_dma2en = 1;
    bb_dma2_ch3_ccr_en = 0;    //停止dma通道
    bb_dma2_ch5_ccr_en = 0;    //停止dma通道
    bb_uart4_cr1_rxneie = 0;
    bb_uart4_cr1_txeie = 0;
    bb_uart4_cr1_idleie = 1;
    pg_dma2_reg->IFCR |= 0x000f0f00;        //清所有中断状态
    pg_dma2_channel5_reg->CCR = 0x00000092; //发送dma
    pg_dma2_channel5_reg->CPAR = (u32)&(pg_uart4_reg->DR);

    pg_dma2_channel3_reg->CCR = 0x00000086; //接收dma
    pg_dma2_channel3_reg->CPAR = (u32)&(pg_uart4_reg->DR);
    pg_dma2_channel3_reg->CNDTR = 32;
    pg_dma2_channel3_reg->CMAR = (u32)uart4_dma_recv_buf;

    int_isr_connect(cn_int_line_DMA2_Ch3,dma2ch3_int);
    int_isr_connect(cn_int_line_DMA2_Ch4_5,dma2ch5_int);

    int_setto_asyn_signal(cn_int_line_DMA2_Ch3);
    int_setto_asyn_signal(cn_int_line_DMA2_Ch4_5);
    int_restore_asyn_line(cn_int_line_DMA2_Ch3);
    int_restore_asyn_line(cn_int_line_DMA2_Ch4_5);

    pg_uart4_reg->CR3 |= 0x00c0;  //设置串口使用dma收发
    tg_uart4_cb.dma_used = cn_dma_used;
    bb_dma2_ch3_ccr_en = 1;    //启动接收dma通道
    uart4_first_dma_send = true;
}

//----暂停发送------------------------------------------------------------
//功能: 用于左手写设备过程中要操作发送缓冲区，防止发送中断访问发送缓冲区。
//参数: 无
//返回: 无
//-----------------------------------------------------------------------------
void __uart4_pause_send_int(void)
{
    if(tg_uart4_cb.dma_used == true)
    {
        int_save_asyn_line(cn_int_line_DMA2_Ch4_5);
    }else
    {
        int_save_asyn_line(cn_int_line_UART4);
    }
}

//----恢复发送------------------------------------------------------------
//功能: 用于左手写设备过程中操作发送缓冲区完成，可恢复发送
//参数: 无
//返回: 无
//-----------------------------------------------------------------------------
void __uart4_resume_send_int(void)
{
    if(tg_uart4_cb.dma_used == true)
    {
        int_restore_asyn_line(cn_int_line_DMA2_Ch4_5);
    }else
    {
        int_restore_asyn_line(cn_int_line_UART4);
    }
}

//----暂停接收------------------------------------------------------------
//功能: 用于左手写设备过程中要操作发送缓冲区，防止发送中断访问发送缓冲区。
//参数: 无
//返回: 无
//-----------------------------------------------------------------------------
void __uart4_pause_recv_int(void)
{
    if(tg_uart4_cb.dma_used == true)
    {
        int_save_asyn_line(cn_int_line_DMA2_Ch3);
    }else
    {
        int_save_asyn_line(cn_int_line_UART4);
    }
}

//----恢复接收------------------------------------------------------------
//功能: 用于左手写设备过程中操作发送缓冲区完成，可恢复发送
//参数: 无
//返回: 无
//-----------------------------------------------------------------------------
void __uart4_resume_recv_int(void)
{
    if(tg_uart4_cb.dma_used == true)
    {
        int_restore_asyn_line(cn_int_line_DMA2_Ch3);
    }else
    {
        int_restore_asyn_line(cn_int_line_UART4);
    }
}

//----启动发送------------------------------------------------------------
//功能: 左手写入数据后，如果发送器未工作，需要发送数据以启动发送器
//参数: 无
//返回: 无
//-----------------------------------------------------------------------------
void __uart4_start_send(void)
{
    u32 num,addr;
    if(tg_uart4_cb.dma_used == true)
    {
        num = dev_read_right(pg_uart4_rhdl,0,(ptu32_t)uart4_dma_send_buf,32);
        if(uart4_first_dma_send)
        {
            pg_uart4_reg->DR = uart4_dma_send_buf[0];
            uart4_first_dma_send = false;
            addr = (u32)uart4_dma_send_buf+1;
            num--;
        }else
            addr = (u32)uart4_dma_send_buf;

        if(num > 0)
        {
            bb_dma2_ch5_ccr_en = 0;    //暂停dma传输
            pg_dma2_channel5_reg->CMAR = addr;
            pg_dma2_channel5_reg->CNDTR = num;
            bb_dma2_ch5_ccr_en = 1;    //继续dma传输
            tg_uart4_cb.sendding = true;
        }
    }else
    {
        bb_uart4_cr1_txeie = 1;
        dev_read_right(pg_uart4_rhdl,0,(ptu32_t)&pg_uart4_reg->DR,1);
        tg_uart4_cb.sendding = true;
    }
}

//----启动串口1------------------------------------------------------------
//功能: 使串口开始工作
//参数: 无
//返回: 无
//-----------------------------------------------------------------------------
void __uart4_start(void)
{
    bb_rcc_apb1enr_uart4en = 1;    //uart4时钟使能
    bb_uart4_cr1_ue = 1;
}

//----停止串口1------------------------------------------------------------
//功能: 使串口停止工作
//参数: 无
//返回: 无
//-----------------------------------------------------------------------------
void __uart4_stop(void)
{
    bb_rcc_apb1enr_uart4en = 0;    //uart4时钟不使能
    bb_uart4_cr1_ue = 0;
}

//----uart1设备左手写函数-------------------------------------------------------
//功能: 从左手写入uart设备,一般由uart的使用者调用.
//      1.把数据写入串口serial_DCB_t的环形发送缓冲区中.
//      2.如果串口硬件空闲,便把数据写入硬件发送缓冲区,并启动发送,否则不操作
//参数: uart_dev,被操作的设备的指针.
//      src_buf,数据源指针
//      len,要发送的序列长度
//返回: 实际写入环形缓冲区的字符数
//-----------------------------------------------------------------------------
ptu32_t uart4_app_write(struct  pan_device *uart_dev,ptu32_t src_buf,
                            ptu32_t res,ptu32_t len)
{
    uint32_t result;
    uint32_t completed = 0;

    if(len == 0)
        return 0;
    while(1)
    {
        __uart4_pause_send_int();
        result = ring_write(&tg_uart4_cb.send_ring_buf,
                            (uint8_t*)src_buf+completed,len-completed);
        if(tg_uart4_cb.sendding == false)
            __uart4_start_send();
        __uart4_resume_send_int();
        if(result != len-completed)     //缓冲区满，没有送出全部数据
        {
            completed += result;
            semp_pend(tg_uart4_cb.send_buf_semp,cn_timeout_forever);
        }else
            break;
    }
    return result;
}

//----串口1设备左手读函数------------------------------------------------------
//功能: 左手读用于上层设备,读取串口接收环形缓冲区的数据
//参数: uart_dev,被操作的设备的指针.
//      des_buf,读缓冲区
//      len,读入长度,
//返回: 实际读出长度
//----------------------------------------------------------------------------
ptu32_t uart4_app_read(struct  pan_device *uart_dev,ptu32_t res,
                                        ptu32_t des_buf,ptu32_t len)
{
    uint32_t result;
    __uart4_pause_recv_int();
    result = ring_read(&tg_uart4_cb.recv_ring_buf,(uint8_t*)des_buf,len);
    __uart4_resume_recv_int();
    return result;
}

//----串口1设备控制函数--------------------------------------------------------
//功能: 串口设备的控制函数
//参数: uart_dev,被操作的设备的指针.
//      cmd,操作类型
//      data,含义依cmd而定
//返回: 无意义.
//-----------------------------------------------------------------------------
ptu32_t uart4_ctrl(struct  pan_device *uart_dev,uint32_t cmd,
                   uint32_t data1,uint32_t data2)
{
    u16 temp1,temp2;

    switch(cmd)
    {
        case cn_uart_connect_recv_evtt:
            tg_uart4_cb.recv_target_evtt = (uint16_t)data1;
            break;
        case cn_uart_disconnect_recv_evtt:
            tg_uart4_cb.recv_target_evtt = cn_invalid_evtt_id;
            break;
        case cn_uart_start:
            __uart4_start();
            break;
        case cn_uart_stop:
            __uart4_stop();
            break;
        case cn_uart_set_baud:  //设置Baud
            if(tg_uart4_cb.baud !=data1)
            {
                temp1 = cn_pclk1/(16*data1);
                temp2 = cn_pclk1%(16*data1);
                temp2 = temp2/data1;
                pg_uart4_reg->BRR =  (temp1<<4)+temp2;
                tg_uart4_cb.baud = data1;
            }
            break;
        case cn_uart_half_duplex_send:     //启动发送
        //用IO控制半双工通信的发送使能的话,在此转换收发.
            break;
        case cn_uart_half_duplex_recv:     //启动接收
        //用IO控制半双工通信的发送使能的话,在此转换收发.
            break;
        case cn_uart_setto_all_duplex:
        //用IO控制半双工通信的发送使能的话,在此转换收发,注意要查询UTRSTAT寄存器
        //的Transmit shifter empty位判断是否已经完成.
            break;
        case cn_uart_overtime:   //data1为毫秒数
            tg_uart4_cb.timeout = data1;
            break;
        case cn_uart_recv_soft_level:    //设置接收软件缓冲区触发水平
            tg_uart4_cb.recv_trigger_level = (uint16_t)data1;
            break;
        case cn_uart_send_soft_level:    //设置接收软件缓冲区触发水平
            tg_uart4_cb.send_trigger_level = (uint16_t)data1;
            break;
        case cn_uart_en_rts:
            pg_uart4_reg->CR3 |= 0x100;
            break;
        case cn_uart_dis_rts:
            pg_uart4_reg->CR3 &= ~0x100;
            break;
        case cn_uart_en_cts:
            pg_uart4_reg->CR3 |= 0x200;
            break;
        case cn_uart_dis_cts:
            pg_uart4_reg->CR3 &= ~0x200;
            break;
        case cn_uart_dma_used:
            __uart4_set_dma_used();
            break;
        case cn_uart_dma_unused:
            __uart4_set_dma_unused();
            break;
        default: break;
    }
    return 0;
}

//----串行接收服务-------------------------------------------------------------
//功能: 事件服务函数,当串口硬件收到有效数据后，将收到的数据写入设备的同时，发送
//      本事件，事件的两个参数分别是:
//      1、所属串口的右手设备句柄.
//      2、本次接收到的数据数量
//参数: my_event，本事件指针.
//返回: 事件服务函数可以不返回,即使返回也没有返回值.
//-----------------------------------------------------------------------------
void uart4_hard_recv_service(void)
{
    uint32_t valid_bytes=0;
    u32 recvs;
    u32 wakeup;

    while(1)
    {
        //等待自身类型的事件再次发生，新事件在串口中断或dma中断服务函数中弹出。
        wakeup = djy_wait_evtt_pop(djy_my_evtt_id(),1,
                                    tg_uart4_cb.timeout);
        if(wakeup == cn_sync_timeout)
        {
            if(tg_uart4_cb.dma_used == true)
            {
                bb_dma2_ch3_ccr_en = 0;    //暂停dma传输
                recvs = 32-pg_dma2_channel3_reg->CNDTR;
                dev_write_right(pg_uart4_rhdl,(ptu32_t)uart4_dma_recv_buf,0,recvs);
                pg_dma2_channel3_reg->CNDTR = 32;
                pg_dma2_channel3_reg->CMAR = (u32)uart4_dma_recv_buf;
                bb_dma2_ch3_ccr_en = 1;    //继续dma传输
                valid_bytes += recvs;
            }
            if(valid_bytes != 0)
            {
                //如果不登记右手写事件，就只能查询方式从设备读取数据了。
                djy_event_pop(tg_uart4_cb.recv_target_evtt,
                                false,NULL,0,NULL,0,0,0);
                valid_bytes = 0;
            }
        }else
        {
            valid_bytes = ring_check(&tg_uart4_cb.recv_ring_buf);
            if(valid_bytes >= tg_uart4_cb.recv_trigger_level)
            {
                //如果不登记右手写事件，就只能查询方式从设备读取数据了。
                djy_event_pop(tg_uart4_cb.recv_target_evtt,
                                false,NULL,0,NULL,0,0,0);
                valid_bytes = 0;
            }
        }
    }
}

#endif  //for (cfg_uart4_used == 1)

#if(cfg_uart5_used == 1)
//----初始化uart5模块----------------------------------------------------------
//功能：初始化uart5模块，uart5不支持dma传送的方式。
//参数：模块初始化函数没有参数
//返回：true = 成功初始化，false = 初始化失败
//-----------------------------------------------------------------------------
ptu32_t module_init_uart5(ptu32_t para)
{
    struct semaphore_LCB *left_semp,*right_semp;
    struct  pan_device   *uart_dev;
    uint32_t temp1,temp2;
    //保护缓冲区的信号量，使缓冲区中数据量为0时阻塞写入线程，读取线程使缓冲区中
    //数据降至trigger_level以下时释放信号量，使写入线程解除阻塞
    tg_uart5_cb.send_buf_semp = semp_create(1,0,"uart5 tx buf");
    if(tg_uart5_cb.send_buf_semp == NULL)
        goto exit_from_left_buf_semp;
    tg_uart5_cb.recv_buf_semp = semp_create(1,0,"uart5 rx buf");
    if(tg_uart5_cb.recv_buf_semp == NULL)
        goto exit_from_right_buf_semp;
    left_semp = semp_create(1,1,"uart5 device left");
    if(left_semp == NULL)
        goto exit_from_left_semp;
    right_semp = semp_create(1,1,"uart5 device right");
    if(right_semp == NULL)
        goto exit_from_right_semp;
    //初始化IO端口位uart功能
    bb_rcc_apb1enr_uart5en = 1;    //uart5时钟使能
    gpio_power_on(cn_gpio_c);
    gpio_power_on(cn_gpio_d);
    gpio_cfg_pin_func(cn_gpio_c,12,cn_gpio_mode_peri_out_pp_10Mhz);
    gpio_cfg_pin_func(cn_gpio_d,2,cn_gpio_mode_in_floating);
    //系统初始化时已经使中断处于禁止状态，无需再禁止和清除中断。
    //初始化uart硬件控制数据结构
    pg_uart5_reg->CR1 = 0x20ac;
    pg_uart5_reg->CR2 = 0x0;
    pg_uart5_reg->CR3 = 0x0000;
    temp1 = cn_pclk2/(16*115200);
    temp2 = cn_pclk2%(16*115200);
    temp2 = temp2/115200;
    pg_uart5_reg->BRR =  ((u16)temp1<<4)+(u16)temp2;
    tg_uart5_cb.baud = 115200;

    //初始化通用串口数据结构
    tg_uart5_cb.recv_trigger_level = uart5_buf_len>>4;  //缓冲接收触发水平为1/16
    tg_uart5_cb.send_trigger_level = uart5_buf_len>>4;  //缓冲发送触发水平为1/16
    tg_uart5_cb.timeout = 100*mS;   //超时时间=100
    tg_uart5_cb.sendding = false;
    ring_init(  &tg_uart5_cb.send_ring_buf,
                uart5_drv_send_buf,
                uart5_buf_len);
    ring_init(  &tg_uart5_cb.recv_ring_buf,
                uart5_drv_recv_buf,
                uart5_buf_len);

    //以下建立uart1设备
    uart_dev = dev_add_device(     dev_get_root_device(),"uart5",
                                   right_semp,left_semp,
                                   (dev_write_func) uart_hard_write,
                                   (dev_read_func ) uart_hard_read,
                                   (dev_ctrl_func ) uart5_ctrl,
                                   (dev_write_func ) uart5_app_write,
                                   (dev_read_func  ) uart5_app_read,
                                   (dev_ctrl_func  ) uart5_ctrl,
                                   (ptu32_t)(&tg_uart5_cb)
                                   );    //"uart5"是一个根设备
    if(uart_dev == NULL)
        goto exit_from_add_device;
    pg_uart5_rhdl = dev_open_right("uart5",0);      //打开右手句柄
    u16g_evtt_uart5_error = djy_evtt_regist(enum_correlative,
                            cn_prio_real,0,5,uart_error_service,10,"uart5 err");
    if(u16g_evtt_uart5_error == cn_invalid_evtt_id)
        goto exit_from_error_evtt;
    u16g_evtt_uart5_hard_recv = djy_evtt_regist(enum_correlative,cn_prio_real,0,
                                   5,uart5_hard_recv_service,32,"uart5 recv");
    if(u16g_evtt_uart5_hard_recv == cn_invalid_evtt_id)
        goto exit_from_hard_recv_evtt;

    djy_event_pop(u16g_evtt_uart5_hard_recv,enum_para_async,NULL,0,NULL,0,0,0);

    int_isr_connect(cn_int_line_UART5,uart5_int);
    int_setto_asyn_signal(cn_int_line_UART5);

//    bb_uart5_sr_txe = 0;
    int_echo_line(cn_int_line_UART5);
    int_restore_asyn_line(cn_int_line_UART5);
    return true;

exit_from_hard_recv_evtt:
    djy_evtt_unregist(u16g_evtt_uart5_error);
exit_from_error_evtt:
    dev_delete_device(uart_dev);
exit_from_add_device:
    semp_delete(right_semp);
exit_from_right_semp:
    semp_delete(left_semp);
exit_from_left_semp:
    semp_delete(tg_uart5_cb.recv_buf_semp);
exit_from_right_buf_semp:
    semp_delete(tg_uart5_cb.send_buf_semp);
exit_from_left_buf_semp:
    return false;
}

//----uart5中断---------------------------------------------------------------
//功能: 如果是接收信号触发的中断:
//      1.检查接收fifo中的数据量.把数据从接收寄存器全部copy到物理层缓冲区中.
//      2.如果物理层缓冲区满.置串口控制块状态字的溢出位,并发出uart错误事件.
//      3.发送串口接收事件.两个事件都是状态量事件
//      如果是发送信号引发的中断:
//      1.检查发送fifo有多少空位
//      2.从设备右手接口读出适量数据，
//      3.把读出的数据写入发送fifo
//      如果是出错信号引发中断:弹出串口出错事件
//参数: 中断函数没有参数.
//返回: 中断函数没有返回值.
//-----------------------------------------------------------------------------
uint32_t uart5_int(ufast_t uart_int_line)
{
    u16 num;
    u8 ch;
    enum uart_error_no parameter;
    if(bb_uart5_sr_rxne == 1)
    {
        ch = (u8)pg_uart5_reg->DR;
        num = dev_write_right(pg_uart5_rhdl,(ptu32_t)&ch,0,1);
        if(num != 1)
        {
            parameter = enum_recv_buf_over;
            djy_event_pop(u16g_evtt_uart5_error,false,NULL,0,&parameter,
                                sizeof(parameter),0,0);
        }else
        {
            djy_event_pop(u16g_evtt_uart5_hard_recv,false,NULL,0,NULL,0,0,0);
        }
    }
    if(bb_uart5_sr_txe == 1)
    {
        num = dev_read_right(pg_uart5_rhdl,0,(ptu32_t)&ch,1);
        if(num != 0)
            pg_uart5_reg->DR = ch;
        else
        {

            bb_uart5_cr1_txeie = 0;
            tg_uart5_cb.sendding = false;
        }
    }
    if(bb_uart5_sr_ore == 1)
    {
        ch = (u8)pg_uart5_reg->DR;
        num = pg_uart5_reg->SR;     //读一下sr寄存器
        parameter = enum_recv_hard_over;
        djy_event_pop(u16g_evtt_uart5_error,false,NULL,0,&parameter,
                            sizeof(parameter),0,0);
    }
    return 0;
}

//----暂停发送------------------------------------------------------------
//功能: 用于左手写设备过程中要操作发送缓冲区，防止发送中断访问发送缓冲区。
//参数: 无
//返回: 无
//-----------------------------------------------------------------------------
void __uart5_pause_send_int(void)
{
    int_save_asyn_line(cn_int_line_UART5);
}

//----恢复发送------------------------------------------------------------
//功能: 用于左手写设备过程中操作发送缓冲区完成，可恢复发送
//参数: 无
//返回: 无
//-----------------------------------------------------------------------------
void __uart5_resume_send_int(void)
{
    int_restore_asyn_line(cn_int_line_UART5);
}

//----暂停接收------------------------------------------------------------
//功能: 用于左手写设备过程中要操作发送缓冲区，防止发送中断访问发送缓冲区。
//参数: 无
//返回: 无
//-----------------------------------------------------------------------------
void __uart5_pause_recv_int(void)
{
    int_save_asyn_line(cn_int_line_UART5);
}

//----恢复接收------------------------------------------------------------
//功能: 用于左手写设备过程中操作发送缓冲区完成，可恢复发送
//参数: 无
//返回: 无
//-----------------------------------------------------------------------------
void __uart5_resume_recv_int(void)
{
    int_restore_asyn_line(cn_int_line_UART5);
}

//----启动发送------------------------------------------------------------
//功能: 左手写入数据后，如果发送器未工作，需要发送数据以启动发送器
//参数: 无
//返回: 无
//-----------------------------------------------------------------------------
void __uart5_start_send(void)
{
    bb_uart5_cr1_txeie = 1;
    dev_read_right(pg_uart5_rhdl,0,(ptu32_t)&pg_uart5_reg->DR,1);
    tg_uart5_cb.sendding = true;
}

//----启动串口1------------------------------------------------------------
//功能: 使串口开始工作
//参数: 无
//返回: 无
//-----------------------------------------------------------------------------
void __uart5_start(void)
{
    bb_rcc_apb1enr_uart5en = 1;    //uart5时钟使能
    bb_uart5_cr1_ue = 1;
}

//----停止串口1------------------------------------------------------------
//功能: 使串口停止工作
//参数: 无
//返回: 无
//-----------------------------------------------------------------------------
void __uart5_stop(void)
{
    bb_rcc_apb1enr_uart5en = 0;    //uart5时钟使能
    bb_uart5_cr1_ue = 0;
}

//----uart1设备左手写函数-------------------------------------------------------
//功能: 从左手写入uart设备,一般由uart的使用者调用.
//      1.把数据写入串口serial_DCB_t的环形发送缓冲区中.
//      2.如果串口硬件空闲,便把数据写入硬件发送缓冲区,并启动发送,否则不操作
//参数: uart_dev,被操作的设备的指针.
//      src_buf,数据源指针
//      len,要发送的序列长度
//返回: 实际写入环形缓冲区的字符数
//-----------------------------------------------------------------------------
ptu32_t uart5_app_write(struct  pan_device *uart_dev,ptu32_t src_buf,
                            ptu32_t res,ptu32_t len)
{
    uint32_t result;
    uint32_t completed = 0;

    if(len == 0)
        return 0;
    while(1)
    {
        __uart5_pause_send_int();
        result = ring_write(&tg_uart5_cb.send_ring_buf,
                            (uint8_t*)src_buf+completed,len-completed);
        if(tg_uart5_cb.sendding == false)
            __uart5_start_send();
        __uart5_resume_send_int();
        if(result != len-completed)     //缓冲区满，没有送出全部数据
        {
            completed += result;
            semp_pend(tg_uart5_cb.send_buf_semp,cn_timeout_forever);
        }else
            break;
    }
    return result;
}

//----串口1设备左手读函数------------------------------------------------------
//功能: 左手读用于上层设备,读取串口接收环形缓冲区的数据
//参数: uart_dev,被操作的设备的指针.
//      des_buf,读缓冲区
//      len,读入长度,
//返回: 实际读出长度
//----------------------------------------------------------------------------
ptu32_t uart5_app_read(struct  pan_device *uart_dev,ptu32_t res,
                                        ptu32_t des_buf,ptu32_t len)
{
    uint32_t result;
    __uart5_pause_recv_int();
    result = ring_read(&tg_uart5_cb.recv_ring_buf,(uint8_t*)des_buf,len);
    __uart5_resume_recv_int();
    return result;
}

//----串口1设备控制函数--------------------------------------------------------
//功能: 串口设备的控制函数
//参数: uart_dev,被操作的设备的指针.
//      cmd,操作类型
//      data,含义依cmd而定
//返回: 无意义.
//-----------------------------------------------------------------------------
ptu32_t uart5_ctrl(struct  pan_device *uart_dev,uint32_t cmd,
                   uint32_t data1,uint32_t data2)
{
    u16 temp1,temp2;

    switch(cmd)
    {
        case cn_uart_connect_recv_evtt:
            tg_uart5_cb.recv_target_evtt = (uint16_t)data1;
            break;
        case cn_uart_disconnect_recv_evtt:
            tg_uart5_cb.recv_target_evtt = cn_invalid_evtt_id;
            break;
        case cn_uart_start:
            __uart5_start();
            break;
        case cn_uart_stop:
            __uart5_stop();
            break;
        case cn_uart_set_baud:  //设置Baud
            if(tg_uart5_cb.baud !=data1)
            {
                temp1 = cn_pclk1/(16*data1);
                temp2 = cn_pclk1%(16*data1);
                temp2 = temp2/data1;
                pg_uart5_reg->BRR =  (temp1<<4)+temp2;
                tg_uart5_cb.baud = data1;
            }
            break;
        case cn_uart_half_duplex_send:     //启动发送
        //用IO控制半双工通信的发送使能的话,在此转换收发.
            break;
        case cn_uart_half_duplex_recv:     //启动接收
        //用IO控制半双工通信的发送使能的话,在此转换收发.
            break;
        case cn_uart_setto_all_duplex:
        //用IO控制半双工通信的发送使能的话,在此转换收发,注意要查询UTRSTAT寄存器
        //的Transmit shifter empty位判断是否已经完成.
            break;
        case cn_uart_overtime:   //data1为毫秒数
            tg_uart5_cb.timeout = data1;
            break;
        case cn_uart_recv_soft_level:    //设置接收软件缓冲区触发水平
            tg_uart5_cb.recv_trigger_level = (uint16_t)data1;
            break;
        case cn_uart_send_soft_level:    //设置接收软件缓冲区触发水平
            tg_uart5_cb.send_trigger_level = (uint16_t)data1;
            break;
        case cn_uart_en_rts:
            pg_uart5_reg->CR3 |= 0x100;
            break;
        case cn_uart_dis_rts:
            pg_uart5_reg->CR3 &= ~0x100;
            break;
        case cn_uart_en_cts:
            pg_uart5_reg->CR3 |= 0x200;
            break;
        case cn_uart_dis_cts:
            pg_uart5_reg->CR3 &= ~0x200;
            break;
        default: break;
    }
    return 0;
}

//----串行接收服务-------------------------------------------------------------
//功能: 事件服务函数,当串口硬件收到有效数据后，将收到的数据写入设备的同时，发送
//      本事件，事件的两个参数分别是:
//      1、所属串口的右手设备句柄.
//      2、本次接收到的数据数量
//参数: my_event，本事件指针.
//返回: 事件服务函数可以不返回,即使返回也没有返回值.
//-----------------------------------------------------------------------------
void uart5_hard_recv_service(void)
{
    uint32_t valid_bytes=0;
    u32 wakeup;

    while(1)
    {
        //等待自身类型的事件再次发生，新事件在串口中断或dma中断服务函数中弹出。
        wakeup = djy_wait_evtt_pop(djy_my_evtt_id(),1,
                                    tg_uart5_cb.timeout);
        if(wakeup == cn_sync_timeout)
        {
            if(valid_bytes != 0)
            {
                //如果不登记右手写事件，就只能查询方式从设备读取数据了。
                djy_event_pop(tg_uart5_cb.recv_target_evtt,
                                false,NULL,0,NULL,0,0,0);
                valid_bytes = 0;
            }
        }else
        {
            valid_bytes = ring_check(&tg_uart5_cb.recv_ring_buf);
            if(valid_bytes >= tg_uart5_cb.recv_trigger_level)
            {
                //如果不登记右手写事件，就只能查询方式从设备读取数据了。
                djy_event_pop(tg_uart5_cb.recv_target_evtt,
                                false,NULL,0,NULL,0,0,0);
                valid_bytes = 0;
            }
        }
    }
}

#endif  //for (cfg_uart5_used == 1)

void uart_error_service(void)
{
    while(1)
    {
    	djy_wait_evtt_pop(djy_my_evtt_id(),1,cn_timeout_forever);
    }
}

//----uart设备右手写函数-------------------------------------------------------
//功能: 从右手写入uart设备,由uart设备连接的串口模块调用.
//      1.把数据写入串口serial_DCB_t的环形发送缓冲区中.
//      2.如果写入设备的数据未达到触发水平，则继续收集数据
//      3.达到出发水平后，按设定的方式把数据传递给上层设备
//参数: uart_dev,被操作的设备的指针.
//      buf，数据源指针。
//      len，数据量(bytes)
//返回: 实际写入环形缓冲区的字符数
//-----------------------------------------------------------------------------
ptu32_t uart_hard_write(struct  pan_device *uart_dev,ptu32_t buf,
                            ptu32_t res2,ptu32_t len)
{
    struct uart_UCB *uart_port;
    uint16_t recv_bytes;

    uart_port = (struct uart_UCB*)dev_get_my_tag(uart_dev);
    //copy整个硬件缓冲区到协议缓冲区
    recv_bytes = ring_write(&uart_port->recv_ring_buf, (uint8_t*)buf,len);
    return (ptu32_t)recv_bytes;

}

//----串口设备右手读函数------------------------------------------------------
//功能: 右手读用于硬件,硬件发送服务函数 ser_send_service从串口发送环形缓冲区
//      读取数据到串口硬件缓冲区并启动发送.
//参数: uart_dev,被操作的设备的指针.
//      des_buf,读缓冲区
//      len,读入长度,
//返回: 实际读出长度
//----------------------------------------------------------------------------
ptu32_t uart_hard_read(struct  pan_device *uart_dev,ptu32_t res1,
                                        ptu32_t des_buf,ptu32_t len)
{
    struct uart_UCB *uart_port;
    uint32_t result;

    uart_port = (struct uart_UCB*)dev_get_my_tag(uart_dev);
    result=(ptu32_t)ring_read(&uart_port->send_ring_buf,(uint8_t*)des_buf,len);
    if(ring_check(&uart_port->send_ring_buf) <= uart_port->send_trigger_level)
        semp_post(uart_port->send_buf_semp);
    return result;
}

