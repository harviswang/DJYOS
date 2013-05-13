//----------------------------------------------------
//Copyright (C), 2004-2009,  lst.
//版权所有 (C), 2004-2009,   lst.
//所属模块: uart驱动程序
//作者：lst
//版本：V1.0.0
//文件描述: 驱动2440的uart
//其他说明:
//修订历史:
//1. 日期:2009-03-10
//   作者:lst
//   新版本号：1.0.0
//   修改说明: 移植自44b0的1.0.1版
//------------------------------------------------------
#include <string.h>
#include "os_inc.h"
#include "cpu_peri.h"
extern struct int_reg volatile * const pg_int_reg;

struct  pan_device   *uart_dev;
static djy_handle_t pg_uart0_rhdl;
static djy_handle_t pg_uart1_rhdl;
static djy_handle_t pg_uart2_rhdl;
static struct uart_UCB tg_uart0_CB,tg_uart1_CB,tg_uart2_CB;

uint16_t u16g_evtt_uart0_error;
uint16_t u16g_evtt_uart1_error;
uint16_t u16g_evtt_uart2_error;

u8 uart0_ptcl_send_buf[uart0_buf_len];
u8 uart1_ptcl_send_buf[uart1_buf_len];
u8 uart2_ptcl_send_buf[uart2_buf_len];
u8 uart0_ptcl_recv_buf[uart0_buf_len];
u8 uart1_ptcl_recv_buf[uart1_buf_len];
u8 uart2_ptcl_recv_buf[uart2_buf_len];

//----启动uart的中断源---------------------------------------------------------
//功能: 2410中，uart中断可能被收、发、err三个源触发，本函数选择性地启动其中一个
//      或数个中断源，具体由rsc_msk中被置1的位确定。
//参数: rsc_msk，置1的位被操作的中断源，只有bit0~bit8有效
//返回: 无
//-----------------------------------------------------------------------------
void __uart_sub_int_enable(u32 rsc_msk)
{
    pg_int_reg->INTSUBMSK &= ~(rsc_msk & BIT_ALL_UART);
}

//----禁止uart的中断源---------------------------------------------------------
//功能: 2410中，uart中断可能被收、发、err三个源触发，本函数选择性地禁止其中一个
//      或数个中断源，具体由rsc_msk中被置1的位确定。
//参数: rsc_msk，置1的位被操作的中断源，只有bit0~bit8有效
//返回: 无
//-----------------------------------------------------------------------------
void __uart_sub_int_disable(u32 rsc_msk)
{
    pg_int_reg->INTSUBMSK |= (rsc_msk & BIT_ALL_UART);
}

//----检查串口发送缓冲区是否满-------------------------------------------------
//功能: 检查发送fifo缓冲区的状态，如果fifo满就返回true
//参数：无。
//返回: true = 缓冲区满，false = 非满
//-----------------------------------------------------------------------------
bool_t uart_tx_fifo_full(struct uart_reg *uart_reg)
{
    return (uart_reg->UFSTAT &0x4000);
}

//----检查串口发送fifo是否空---------------------------------------------------
//功能: 检查发送fifo缓冲区的状态，如果fifo空就返回true
//参数：无。
//返回: true = 缓冲区空，false = 非空
//-----------------------------------------------------------------------------
u32 uart_get_tx_fifo_num(struct uart_reg *uart_reg)
{
    return (uart_reg->UFSTAT &0x3f00)>> 8;
}

//----检查串口发送寄存器是否空-------------------------------------------------
//功能: 检查发送寄存器的状态，如果空就返回true
//参数：无。
//返回: true = 缓冲区空，false = 非空
//-----------------------------------------------------------------------------
bool_t uart_tx_tran_empty(volatile struct uart_reg *uart_reg)
{
    return (0!=(uart_reg->UTRSTAT & 0x04));
}



void uart0_error_service(void)
{
    volatile u32 temp;
    while(1)
    {
        temp = tg_uart0_CB.my_reg->UERSTAT;
        djy_wait_evtt_pop(djy_my_evtt_id(),1,cn_timeout_forever);
    }
}

void uart1_error_service(void)
{
    volatile u32 temp;
    while(1)
    {
        temp = tg_uart1_CB.my_reg->UERSTAT;
        djy_wait_evtt_pop(djy_my_evtt_id(),1,cn_timeout_forever);
    }
}

void uart2_error_service(void)
{
    volatile u32 temp;
    while(1)
    {
        temp = tg_uart2_CB.my_reg->UERSTAT;
        djy_wait_evtt_pop(djy_my_evtt_id(),1,cn_timeout_forever);
    }
}

//----uart0中断---------------------------------------------------------------
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
u32 uart0_int(ufast_t uart_int_line)
{
    u32 recv_trans,num;
    u32 fifo_reg;
    u8 ch[64];
    enum uart_error_no parameter;
    if(pg_int_reg->SUBSRCPND & BIT_SUB_RXD0)
    {
        pg_int_reg->SUBSRCPND = BIT_SUB_RXD0;
        fifo_reg = tg_uart0_CB.my_reg->UFSTAT;
        recv_trans = fifo_reg & 0x3f;
        for(num = 0; num < recv_trans; num++)
        //fifo缓冲区有数据,直到处理完成
            ch[num] = tg_uart0_CB.my_reg->URXH;
        dev_write_right(pg_uart0_rhdl,(ptu32_t)ch,0,recv_trans);
        if( fifo_reg & cn_rx_over)
        {
            tg_uart0_CB.my_reg->UFCON |= cn_rx_fifo_reset;
            parameter = enum_recv_fifo_over;
            djy_event_pop(u16g_evtt_uart0_error,enum_para_async,NULL,0,&parameter,
                                    sizeof(parameter),0,0);
        }
    }
    if(pg_int_reg->SUBSRCPND & BIT_SUB_TXD0)
    {
        pg_int_reg->SUBSRCPND = BIT_SUB_TXD0;
        fifo_reg = tg_uart0_CB.my_reg->UFSTAT;
        recv_trans = 64-((fifo_reg>>8) & 0x3f);
        recv_trans = dev_read_right(pg_uart0_rhdl,0,(ptu32_t)ch,recv_trans);
        for(num = 0; num < recv_trans; num++)
        {
            tg_uart0_CB.my_reg->UTXH = ch[num];
        }
    }
    if(pg_int_reg->SUBSRCPND & BIT_SUB_ERR0)
    {
        pg_int_reg->SUBSRCPND = BIT_SUB_ERR0;
        djy_event_pop(u16g_evtt_uart0_error,enum_para_async,NULL,0,NULL,0,0,0);
    }
    return 0;
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
u32 uart1_int(ufast_t uart_int_line)
{
    u32 recv_trans,num;
    u32 fifo_reg;
    u8 ch[64];
    enum uart_error_no parameter;
    if(pg_int_reg->SUBSRCPND & BIT_SUB_RXD1)
    {
        pg_int_reg->SUBSRCPND = BIT_SUB_RXD1;
        fifo_reg = tg_uart1_CB.my_reg->UFSTAT;
        recv_trans = fifo_reg & 0x3f;
        for(num = 0; num < recv_trans; num++)
        //fifo缓冲区有数据,直到处理完成
            ch[num] = tg_uart1_CB.my_reg->URXH;
        dev_write_right(pg_uart1_rhdl,(ptu32_t)ch,0,recv_trans);
        if( fifo_reg & cn_rx_over)
        {
            tg_uart1_CB.my_reg->UFCON |= cn_rx_fifo_reset;
            parameter = enum_recv_fifo_over;
            djy_event_pop(u16g_evtt_uart1_error,enum_para_async,NULL,0,&parameter,
                                    sizeof(parameter),0,0);
        }
    }
    if(pg_int_reg->SUBSRCPND & BIT_SUB_TXD1)
    {
        pg_int_reg->SUBSRCPND = BIT_SUB_TXD1;
        fifo_reg = tg_uart1_CB.my_reg->UFSTAT;
        recv_trans = 64-((fifo_reg>>8) & 0x3f);
        recv_trans = dev_read_right(pg_uart1_rhdl,0,(ptu32_t)ch,recv_trans);
        for(num = 0; num < recv_trans; num++)
        {
            tg_uart1_CB.my_reg->UTXH = ch[num];
        }
    }
    if(pg_int_reg->SUBSRCPND & BIT_SUB_ERR1)
    {
        pg_int_reg->SUBSRCPND = BIT_SUB_ERR1;
        djy_event_pop(u16g_evtt_uart1_error,enum_para_async,NULL,0,NULL,0,0,0);
    }
    return 0;
}


//----uart2中断---------------------------------------------------------------
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
u32 uart2_int(ufast_t uart_int_line)
{
    u32 recv_trans,num;
    u32 fifo_reg;
    u8 ch[64];
    enum uart_error_no parameter;
    if(pg_int_reg->SUBSRCPND & BIT_SUB_RXD2)
    {
        pg_int_reg->SUBSRCPND = BIT_SUB_RXD2;
        fifo_reg = tg_uart2_CB.my_reg->UFSTAT;
        recv_trans = fifo_reg & 0x3f;
        for(num = 0; num < recv_trans; num++)
        //fifo缓冲区有数据,直到处理完成
            ch[num] = tg_uart2_CB.my_reg->URXH;
        dev_write_right(pg_uart2_rhdl,(ptu32_t)ch,0,recv_trans);
        if( fifo_reg & cn_rx_over)
        {
            tg_uart2_CB.my_reg->UFCON |= cn_rx_fifo_reset;
            parameter = enum_recv_fifo_over;
            djy_event_pop(u16g_evtt_uart2_error,enum_para_async,NULL,0,&parameter,
                                    sizeof(parameter),0,0);
        }
    }
    if(pg_int_reg->SUBSRCPND & BIT_SUB_TXD2)
    {
        pg_int_reg->SUBSRCPND = BIT_SUB_TXD2;
        fifo_reg = tg_uart2_CB.my_reg->UFSTAT;
        recv_trans = 64-((fifo_reg>>8) & 0x3f);
        recv_trans = dev_read_right(pg_uart2_rhdl,0,(ptu32_t)ch,recv_trans);
        for(num = 0; num < recv_trans; num++)
        {
            tg_uart2_CB.my_reg->UTXH = ch[num];
        }
    }
    if(pg_int_reg->SUBSRCPND & BIT_SUB_ERR2)
    {
        pg_int_reg->SUBSRCPND = BIT_SUB_ERR2;
        djy_event_pop(u16g_evtt_uart2_error,enum_para_async,NULL,0,NULL,0,0,0);
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
ptu32_t uart_hard_write(struct  pan_device *uart_dev,ptu32_t buf,
                            ptu32_t res2,ptu32_t len)
{
    struct uart_UCB *uart_port;
    uint16_t recv_bytes,valid_bytes,error_evtt;
    enum uart_error_no uart_error;

    uart_port = (struct uart_UCB*)dev_get_my_tag(uart_dev);

    //copy整个硬件缓冲区到协议缓冲区
    recv_bytes = ring_write(&uart_port->recv_ring_buf, (u8*)buf,len);
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
        djy_event_pop(error_evtt,enum_para_async,NULL,0,&uart_error,sizeof(uart_error),
                                            0,0);
    }
    valid_bytes = ring_check(&uart_port->recv_ring_buf);
    if(valid_bytes >= uart_port->recv_trigger_level)
    {
        //如果不登记右手写事件，就只能查询方式从设备读取数据了。
        djy_event_pop(uart_port->evtt_right_write,enum_para_async,NULL,0,NULL,0,0,0);
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
ptu32_t uart_app_write(struct  pan_device *uart_dev,ptu32_t src_buf,
                            ptu32_t res,ptu32_t len)
{
    struct uart_UCB *uart_port;   
    u32 completed = 0,sended;
    u8 *buf;

	buf = (u8*)src_buf;
    if((len==0))
        return 0;
    uart_port = (struct uart_UCB*)dev_get_my_tag(uart_dev);  
    
    __uart_sub_int_disable(uart_port->tx_int_line);
    if(ring_is_empty(&uart_port->send_ring_buf))
    {
		sended = uart_get_tx_fifo_num((struct uart_reg *)uart_port->my_reg);
        while(sended <=16)
        {
            uart_port->my_reg->UTXH = buf[completed];
            completed++;
            if(completed == len)
                break;
        }
    }
    
    if(completed < len)
    {
        while(1)
        {
            sended = ring_write(&uart_port->send_ring_buf,
                                (uint8_t*)buf+completed,len-completed);
            __uart_sub_int_enable(uart_port->tx_int_line);
            if(sended != len-completed)     //缓冲区满，没有送出全部数据
            {
                completed += sended;
                semp_pend(uart_port->send_buf_semp,cn_timeout_forever);
                __uart_sub_int_disable(uart_port->tx_int_line);
            }else
            {
                break;
            }
        }
    }
    else
        __uart_sub_int_enable(uart_port->tx_int_line);
    

    return sended;
}

//----串口设备左手读函数------------------------------------------------------
//功能: 左手读用于上层设备,读取串口接收环形缓冲区的数据
//参数: uart_dev,被操作的设备的左手指针.
//      dst_buf,读缓冲区
//      len,读入长度,
//返回: 实际读出长度
//----------------------------------------------------------------------------
ptu32_t uart_app_read(struct  pan_device *uart_dev,ptu32_t res,
                                        ptu32_t dst_buf,ptu32_t len)
{
    struct uart_UCB *uart_port;
    u32 result;
    uart_port = (struct uart_UCB*)dev_get_my_tag(uart_dev);
    __uart_sub_int_disable(uart_port->rx_int_line);
    result = ring_read(&uart_port->recv_ring_buf,(u8*)dst_buf,len);
    __uart_sub_int_enable(uart_port->rx_int_line);
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
ptu32_t uart_hard_read(struct  pan_device *uart_dev,ptu32_t res1,
                                        ptu32_t dst_buf,ptu32_t len)
{
    struct uart_UCB *uart_port;
    static bool_t full = false;
    u32 result;
    uart_port = (struct uart_UCB*)dev_get_my_tag(uart_dev);
    if(ring_if_full(&uart_port->send_ring_buf))
        full = true;
    result = (ptu32_t)ring_read(&uart_port->send_ring_buf,(u8 *)dst_buf,len);
    if((ring_check(&uart_port->send_ring_buf) <= uart_port->send_trigger_level)
            && (full == true))
    {
        full = false;
        semp_post(uart_port->send_buf_semp);
    }
    return result;
}

//----串口设备控制函数---------------------------------------------------------
//功能: 串口设备的控制函数
//参数: uart_handle,被操作的串口设备指针.
//      cmd,操作类型
//      data,含义依cmd而定
//返回: 无意义.
//-----------------------------------------------------------------------------
ptu32_t uart_ctrl(struct pan_device *uart_dev,u32 cmd,
                   u32 data1,u32 data2)
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
            __uart_sub_int_enable(uart_port->tx_int_line);
            __uart_sub_int_enable(uart_port->rx_int_line);
            break;
        case cn_uart_stop:
            __uart_sub_int_disable(uart_port->tx_int_line);
            __uart_sub_int_disable(uart_port->rx_int_line);
            break;
        case cn_uart_set_baud:  //设置Baud
            if(uart_port->baud !=data1)
            {
                uart_port->baud = data1;
                uart_port->my_reg->UBRDIV = ((cn_pclk<<2)/data1 -32)>>6;
//                pg_uart1_reg->UBRDIV =((cn_pclk<<2)/data -32)>>6;
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
            __uart_sub_int_disable(uart_port->rx_int_line);
            break;
        case cn_uart_rx_resume:      //恢复接收
            __uart_sub_int_enable(uart_port->rx_int_line);
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
        case cn_uart_send_hard_level:    //设置发送fifo触发水平
        {
            uart_port->my_reg->UFCON &= 0x3f;
            switch (data1)
            {
                case 0:
                    break;
                case 4:
                    uart_port->my_reg->UFCON |= 0x40;
                    break;
                case 8:
                    uart_port->my_reg->UFCON |= 0x80;
                    break;
                case 12:
                    uart_port->my_reg->UFCON |= 0xc0;
                    break;
                default :break;
            }
            break;
        }
        case cn_uart_recv_hard_level:    //设置接收fifo触发水平
        {
            uart_port->my_reg->UFCON &= 0xcf;
            switch (data1)
            {
                case 0:
                    break;
                case 4:
                    uart_port->my_reg->UFCON |= 0x10;
                    break;
                case 8:
                    uart_port->my_reg->UFCON |= 0x20;
                    break;
                case 12:
                    uart_port->my_reg->UFCON |= 0x30;
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
    u8 ch[64];
    ufast_t trans,num;
    u32 time,sending_num;
    while(1)
    {
	   // for(time=0;time<10000;time++);
        
        my_para = djy_get_event_para(NULL);
        ser_dev = (struct  pan_device *)(*(void**)my_para);
        uart_port = (struct uart_UCB *)dev_get_my_tag(ser_dev);
        
        sending_num = uart_get_tx_fifo_num((struct uart_reg *)uart_port->my_reg);
        //16是串口发送fifo触发深度，必须与module_init_uart0中设置一致
        if(sending_num <=16)
        {   
            __uart_sub_int_disable(uart_port->tx_int_line);
            if(uart_port->serial_no == 0)
                uart_rhdl = pg_uart0_rhdl;
            else if(uart_port->serial_no == 1)
                uart_rhdl = pg_uart1_rhdl;
            else
                uart_rhdl = pg_uart2_rhdl;
            trans = dev_read_right(uart_rhdl,0,(ptu32_t)ch,64-sending_num);
            __uart_sub_int_enable(uart_port->tx_int_line);
            for(num = 0; num < trans; num++)
            {
                uart_port->my_reg->UTXH = ch[num];
            }
            
        }
        
        djy_task_completed(0);
        //等待自身类型的事件再次发生，由于本类型事件已经被设置成mark事件，且事件
        //服务函数中一直不调用y_clear_mark，因此新弹出的事件只会触发本事件继续执
        //行。新事件在uart_left_write函数中弹出。
        djy_wait_evtt_pop(djy_my_evtt_id(),1,cn_timeout_forever);
    }
}

//----初始化uart0模块----------------------------------------------------------
//功能：初始化uart0模块
//参数：模块初始化函数没有参数
//返回：true = 成功初始化，false = 初始化失败
//-----------------------------------------------------------------------------
ptu32_t module_init_uart0(ptu32_t para)
{
    struct semaphore_LCB *left_semp,*right_semp;
    struct  pan_device   *uart_dev;
    uint16_t uart_send_evtt;
    u32 temp;
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
    //GPH和UART口公用接口，初始化IO端口位，使用uart功能
    //Ports  :  GPH10 GPH9 GPH8 GPH7 GPH6 GPH5 GPH4 GPH3 GPH2 GPH1  GPH0
    //Signal :   xx    xx   xx   xx   xx   xx   xx  RXD0 TXD0 nRTS0 nCTS0
    //Binary :   xx    xx   xx   xx   xx   xx   xx   xx   xx   xx   xx
    temp = pg_gpio_reg->GPHCON;
    temp &= ~0xff;
    temp |= 0xaa;
    pg_gpio_reg->GPHCON = temp;
    pg_gpio_reg->GPHUP |= 0xf;   // The pull up function is disabled GPH[3:0]
    //系统初始化时已经使中断处于禁止状态，无需再禁止和清除中断。
    //初始化uart硬件控制数据结构
    tg_uart0_CB.my_reg = (struct uart_reg *)0x50000000;
    tg_uart0_CB.my_reg->ULCON = 0x3;
    tg_uart0_CB.my_reg->UCON =  0xc5;
    tg_uart0_CB.my_reg->UFCON = 0x57;   //bit0=1使能fifo.
    tg_uart0_CB.my_reg->UMCON = 0x0;
    tg_uart0_CB.my_reg->UBRDIV = ((cn_pclk<<2)/115200 -32)>>6;
    temp = tg_uart0_CB.my_reg->UERSTAT;     //读一下清除错误状态
    tg_uart0_CB.baud = 115200;
    tg_uart0_CB.rx_int_line = BIT_SUB_RXD0;
    tg_uart0_CB.tx_int_line = BIT_SUB_TXD0;

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
                               (dev_write_func) uart_hard_write,
                               (dev_read_func ) uart_hard_read,
                               (dev_ctrl_func ) uart_ctrl,
                               (dev_write_func ) uart_app_write,
                               (dev_read_func  ) uart_app_read,
                               (dev_ctrl_func  ) uart_ctrl,
                               (ptu32_t)(&tg_uart0_CB)  //uart0设备的私有数据
                               );
    if(uart_dev == NULL)
        goto exit_from_add_device;
    pg_uart0_rhdl = dev_open_right("uart0",0);      //打开右手句柄
    u16g_evtt_uart0_error = djy_evtt_regist(
                           enum_correlative,cn_prio_real,0,5,uart0_error_service,1024,NULL);
    if(u16g_evtt_uart0_error == cn_invalid_evtt_id)
        goto exit_from_error_evtt;
    uart_send_evtt = djy_evtt_regist(enum_correlative,cn_prio_real,0,
                                   1,uart_send_service,20,NULL);
    if(uart_send_evtt == cn_invalid_evtt_id)
        goto exit_from_send_evtt;
    dev_ctrl_right(pg_uart0_rhdl,cn_uart_connect_send_evtt,uart_send_evtt,0);
    int_isr_connect(cn_int_line_uart0,uart0_int);
    int_setto_asyn_signal(cn_int_line_uart0);
    pg_int_reg->SUBSRCPND = BIT_SUB_TXD0+BIT_SUB_RXD0+BIT_SUB_ERR0;
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
ptu32_t module_init_uart1(ptu32_t para)
{
    struct semaphore_LCB *left_semp,*right_semp;
    struct  pan_device   *uart_dev;
    uint16_t uart_send_evtt;
    u32 temp;
    //保护缓冲区的信号量，使缓冲区中数据量为0时阻塞写入线程，读取线程使缓冲区中
    //数据降至trigger_level以下时释放信号量，使写入线程解除阻塞
    tg_uart1_CB.send_buf_semp = semp_create(1,0,"uart1 tx buf");
    if(tg_uart1_CB.send_buf_semp == NULL)
        goto exit_from_left_buf_semp;
    tg_uart1_CB.recv_buf_semp = semp_create(1,0,"uart1 rx buf");
    if(tg_uart1_CB.recv_buf_semp == NULL)
        goto exit_from_right_buf_semp;
    left_semp = semp_create(1,1,"uart1 device left");
    if(left_semp == NULL)
        goto exit_from_left_semp;
    right_semp = semp_create(1,1,"uart1 device right");
    if(right_semp == NULL)
        goto exit_from_right_semp;
    //初始化IO端口位uart功能
    //Ports  :  GPH10 GPH9 GPH8 GPH7 GPH6 GPH5 GPH4 GPH3 GPH2 GPH1 GPH0
    //Signal :   xx    xx   xx   xx   xx  RXD1 TXD1  xx   xx   xx   xx
    //Binary :   xx    xx   xx   xx   xx   10   10   xx   xx   xx   xx
    temp = pg_gpio_reg->GPHCON;
    temp &= ~0xf00;
    temp |= 0xa00;
    pg_gpio_reg->GPHCON = temp;
    pg_gpio_reg->GPHUP |= 0x30;   // The pull up function is disabled GPH[5:4]
    //系统初始化时已经使中断处于禁止状态，无需再禁止和清除中断。
    //初始化uart硬件控制数据结构
    tg_uart1_CB.my_reg = (struct uart_reg *)0x50004000;
    tg_uart1_CB.my_reg->UFCON = 0x57;   //bit0=1使能fifo.
    tg_uart1_CB.my_reg->UMCON = 0x0;
    tg_uart1_CB.my_reg->ULCON = 0x3;
    tg_uart1_CB.my_reg->UCON =  0xc5;
    tg_uart1_CB.my_reg->UBRDIV = ((cn_pclk<<2)/115200 -32)>>6;
    temp = tg_uart1_CB.my_reg->UERSTAT;     //读一下清除错误状态
    tg_uart1_CB.baud = 115200;
    tg_uart1_CB.rx_int_line = BIT_SUB_RXD1;
    tg_uart1_CB.tx_int_line = BIT_SUB_TXD1;

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
                               (dev_write_func) uart_hard_write,
                               (dev_read_func ) uart_hard_read,
                               (dev_ctrl_func ) uart_ctrl,
                               (dev_write_func ) uart_app_write,
                               (dev_read_func  ) uart_app_read,
                               (dev_ctrl_func  ) uart_ctrl,
                               (ptu32_t)(&tg_uart1_CB)  //uart1设备的私有数据
                               );
    if(uart_dev == NULL)
        goto exit_from_add_device;
    pg_uart1_rhdl = dev_open_right("uart1",0);      //打开右手句柄
    u16g_evtt_uart1_error = djy_evtt_regist(enum_correlative,cn_prio_real,
                           0,5,uart1_error_service,1024,NULL);
    if(u16g_evtt_uart1_error == cn_invalid_evtt_id)
        goto exit_from_error_evtt;
    uart_send_evtt = djy_evtt_regist(enum_correlative,cn_prio_real,0,
                                   1,uart_send_service,20,NULL);
    if(uart_send_evtt == cn_invalid_evtt_id)
        goto exit_from_send_evtt;
    dev_ctrl_right(pg_uart1_rhdl,cn_uart_connect_send_evtt,uart_send_evtt,0);
    int_isr_connect(cn_int_line_uart1,uart1_int);
    int_setto_asyn_signal(cn_int_line_uart1);
    pg_int_reg->SUBSRCPND = BIT_SUB_TXD1+BIT_SUB_RXD1+BIT_SUB_ERR1;
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

ptu32_t module_init_uart2(ptu32_t para)
{
    struct semaphore_LCB *left_semp,*right_semp;
    struct  pan_device   *uart_dev;
    uint16_t uart_send_evtt;
    u32 temp;
    //保护缓冲区的信号量，使缓冲区中数据量为0时阻塞写入线程，读取线程使缓冲区中
    //数据降至trigger_level以下时释放信号量，使写入线程解除阻塞
    tg_uart2_CB.send_buf_semp = semp_create(1,0,"uart2 tx buf");
    if(tg_uart2_CB.send_buf_semp == NULL)
        goto exit_from_left_buf_semp;
    tg_uart2_CB.recv_buf_semp = semp_create(1,0,"uart2 rx buf");
    if(tg_uart2_CB.recv_buf_semp == NULL)
        goto exit_from_right_buf_semp;
    left_semp = semp_create(1,1,"uart2 device left");
    if(left_semp == NULL)
        goto exit_from_left_semp;
    right_semp = semp_create(1,1,"uart2 device right");
    if(right_semp == NULL)
        goto exit_from_right_semp;
    //初始化IO端口位uart功能
    //Ports  :  GPH10 GPH9 GPH8 GPH7 GPH6 GPH5 GPH4 GPH3 GPH2 GPH1 GPH0
    //Signal :   xx    xx   xx  RXD2 TXD2  xx   xx   xx   xx   xx   xx
    //Binary :   xx    xx   xx   10   10   xx   xx   xx   xx   xx   xx
    temp = pg_gpio_reg->GPHCON;
    temp &= ~0xf000;
    temp |= 0xa000;
    pg_gpio_reg->GPHCON = temp;
    pg_gpio_reg->GPHUP |= 0xc0;   // The pull up function is disabled GPH[7:6]
    //系统初始化时已经使中断处于禁止状态，无需再禁止和清除中断。
    //初始化uart硬件控制数据结构
    tg_uart2_CB.my_reg = (struct uart_reg *)0x50008000;
    tg_uart2_CB.my_reg->UFCON = 0x57;   //bit0=1使能fifo.
    tg_uart2_CB.my_reg->UMCON = 0x0;
    tg_uart2_CB.my_reg->ULCON = 0x3;
    tg_uart2_CB.my_reg->UCON =  0xc5;
    tg_uart2_CB.my_reg->UBRDIV = ((cn_pclk<<2)/115200 -32)>>6;
    temp = tg_uart2_CB.my_reg->UERSTAT;     //读一下清除错误状态
    tg_uart2_CB.baud = 115200;
    tg_uart2_CB.rx_int_line = BIT_SUB_RXD2;
    tg_uart2_CB.tx_int_line = BIT_SUB_TXD2;

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
                               (dev_write_func) uart_hard_write,
                               (dev_read_func ) uart_hard_read,
                               (dev_ctrl_func ) uart_ctrl,
                               (dev_write_func ) uart_app_write,
                               (dev_read_func  ) uart_app_read,
                               (dev_ctrl_func  ) uart_ctrl,
                               (ptu32_t)(&tg_uart2_CB)  //uart2设备的私有数据
                               );    //"uart2"是一个根设备
    if(uart_dev == NULL)
        goto exit_from_add_device;
    pg_uart2_rhdl = dev_open_right("uart2",0);      //打开右手句柄
    u16g_evtt_uart2_error = djy_evtt_regist(enum_correlative,cn_prio_real,
                           0,5,uart2_error_service,1024,NULL);
    if(u16g_evtt_uart2_error == cn_invalid_evtt_id)
        goto exit_from_error_evtt;
    uart_send_evtt = djy_evtt_regist(enum_correlative,cn_prio_real,0,
                                   1,uart_send_service,20,NULL);
    if(uart_send_evtt == cn_invalid_evtt_id)
        goto exit_from_send_evtt;
    dev_ctrl_right(pg_uart2_rhdl,cn_uart_connect_send_evtt,uart_send_evtt,0);
    int_isr_connect(cn_int_line_uart2,uart2_int);
    int_setto_asyn_signal(cn_int_line_uart2);
    pg_int_reg->SUBSRCPND = BIT_SUB_TXD2+BIT_SUB_RXD2+BIT_SUB_ERR2;
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
