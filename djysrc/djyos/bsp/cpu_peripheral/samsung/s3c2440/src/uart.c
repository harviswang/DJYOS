//----------------------------------------------------
//Copyright (C), 2004-2009,  lst.
//��Ȩ���� (C), 2004-2009,   lst.
//����ģ��: uart��������
//���ߣ�lst
//�汾��V1.0.0
//�ļ�����: ����2440��uart
//����˵��:
//�޶���ʷ:
//1. ����:2009-03-10
//   ����:lst
//   �°汾�ţ�1.0.0
//   �޸�˵��: ��ֲ��44b0��1.0.1��
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

//----����uart���ж�Դ---------------------------------------------------------
//����: 2410�У�uart�жϿ��ܱ��ա�����err����Դ������������ѡ���Ե���������һ��
//      �������ж�Դ��������rsc_msk�б���1��λȷ����
//����: rsc_msk����1��λ���������ж�Դ��ֻ��bit0~bit8��Ч
//����: ��
//-----------------------------------------------------------------------------
void __uart_sub_int_enable(u32 rsc_msk)
{
    pg_int_reg->INTSUBMSK &= ~(rsc_msk & BIT_ALL_UART);
}

//----��ֹuart���ж�Դ---------------------------------------------------------
//����: 2410�У�uart�жϿ��ܱ��ա�����err����Դ������������ѡ���Եؽ�ֹ����һ��
//      �������ж�Դ��������rsc_msk�б���1��λȷ����
//����: rsc_msk����1��λ���������ж�Դ��ֻ��bit0~bit8��Ч
//����: ��
//-----------------------------------------------------------------------------
void __uart_sub_int_disable(u32 rsc_msk)
{
    pg_int_reg->INTSUBMSK |= (rsc_msk & BIT_ALL_UART);
}

//----��鴮�ڷ��ͻ������Ƿ���-------------------------------------------------
//����: ��鷢��fifo��������״̬�����fifo���ͷ���true
//�������ޡ�
//����: true = ����������false = ����
//-----------------------------------------------------------------------------
bool_t uart_tx_fifo_full(struct uart_reg *uart_reg)
{
    return (uart_reg->UFSTAT &0x4000);
}

//----��鴮�ڷ���fifo�Ƿ��---------------------------------------------------
//����: ��鷢��fifo��������״̬�����fifo�վͷ���true
//�������ޡ�
//����: true = �������գ�false = �ǿ�
//-----------------------------------------------------------------------------
u32 uart_get_tx_fifo_num(struct uart_reg *uart_reg)
{
    return (uart_reg->UFSTAT &0x3f00)>> 8;
}

//----��鴮�ڷ��ͼĴ����Ƿ��-------------------------------------------------
//����: ��鷢�ͼĴ�����״̬������վͷ���true
//�������ޡ�
//����: true = �������գ�false = �ǿ�
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

//----uart0�ж�---------------------------------------------------------------
//����: ����ǽ����źŴ������ж�:
//      1.������fifo�е�������.�����ݴӽ��ռĴ���ȫ��copy������㻺������.
//      2.�������㻺������.�ô��ڿ��ƿ�״̬�ֵ����λ,������uart�����¼�.
//      3.���ʹ��ڽ����¼�.�����¼�����״̬���¼�
//      ����Ƿ����ź��������ж�:
//      1.��鷢��fifo�ж��ٿ�λ
//      2.���豸���ֽӿڶ����������ݣ�
//      3.�Ѷ���������д�뷢��fifo
//      ����ǳ����ź������ж�:�������ڳ����¼�
//����: �жϺ���û�в���.
//����: �жϺ���û�з���ֵ.
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
        //fifo������������,ֱ���������
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

//----uart1�ж�---------------------------------------------------------------
//����: ����ǽ����źŴ������ж�:
//      1.������fifo�е�������.�����ݴӽ��ռĴ���ȫ��copy������㻺������.
//      2.�������㻺������.�ô��ڿ��ƿ�״̬�ֵ����λ,������uart�����¼�.
//      3.���ʹ��ڽ����¼�.�����¼�����״̬���¼�
//      ����Ƿ����ź��������ж�:
//      1.��鷢��fifo�ж��ٿ�λ
//      2.���豸���ֽӿڶ����������ݣ�
//      3.�Ѷ���������д�뷢��fifo
//      ����ǳ����ź������ж�:�������ڳ����¼�
//����: �жϺ���û�в���.
//����: �жϺ���û�з���ֵ.
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
        //fifo������������,ֱ���������
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


//----uart2�ж�---------------------------------------------------------------
//����: ����ǽ����źŴ������ж�:
//      1.������fifo�е�������.�����ݴӽ��ռĴ���ȫ��copy������㻺������.
//      2.�������㻺������.�ô��ڿ��ƿ�״̬�ֵ����λ,������uart�����¼�.
//      3.���ʹ��ڽ����¼�.�����¼�����״̬���¼�
//      ����Ƿ����ź��������ж�:
//      1.��鷢��fifo�ж��ٿ�λ
//      2.���豸���ֽӿڶ����������ݣ�
//      3.�Ѷ���������д�뷢��fifo
//      ����ǳ����ź������ж�:�������ڳ����¼�
//����: �жϺ���û�в���.
//����: �жϺ���û�з���ֵ.
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
        //fifo������������,ֱ���������
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

//----uart�豸����д����-------------------------------------------------------
//����: ������д��uart�豸,��uart�豸���ӵĴ���ģ�����.
//      1.������д�봮��serial_DCB_t�Ļ��η��ͻ�������.
//      2.���д���豸������δ�ﵽ����ˮƽ��������ռ�����
//      3.�ﵽ����ˮƽ�󣬰��趨�ķ�ʽ�����ݴ��ݸ��ϲ��豸
//����: uart_dev,���������豸������ָ��.
//      buf������Դָ�롣
//      len��������(bytes)
//����: ʵ��д�뻷�λ��������ַ���
//-----------------------------------------------------------------------------
ptu32_t uart_hard_write(struct  pan_device *uart_dev,ptu32_t buf,
                            ptu32_t res2,ptu32_t len)
{
    struct uart_UCB *uart_port;
    uint16_t recv_bytes,valid_bytes,error_evtt;
    enum uart_error_no uart_error;

    uart_port = (struct uart_UCB*)dev_get_my_tag(uart_dev);

    //copy����Ӳ����������Э�黺����
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
        //������Ǽ�����д�¼�����ֻ�ܲ�ѯ��ʽ���豸��ȡ�����ˡ�
        djy_event_pop(uart_port->evtt_right_write,enum_para_async,NULL,0,NULL,0,0,0);
    }

    return (ptu32_t)recv_bytes;


}

//----uart�豸����д����-------------------------------------------------------
//����: ������д��uart�豸,һ����uart��ʹ���ߵ���.
//      1.������д�봮��serial_DCB_t�Ļ��η��ͻ�������.
//      2.�������Ӳ������,�������д��Ӳ�����ͻ�����,����������,���򲻲���
//����: uart_dev,���������豸������ָ��.
//      src_buf,����Դָ��
//      len,Ҫ���͵����г���
//����: ʵ��д�뻷�λ��������ַ���
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
            if(sended != len-completed)     //����������û���ͳ�ȫ������
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

//----�����豸���ֶ�����------------------------------------------------------
//����: ���ֶ������ϲ��豸,��ȡ���ڽ��ջ��λ�����������
//����: uart_dev,���������豸������ָ��.
//      dst_buf,��������
//      len,���볤��,
//����: ʵ�ʶ�������
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

//----�����豸���ֶ�����------------------------------------------------------
//����: ���ֶ�����Ӳ��,Ӳ�����ͷ����� ser_send_service�Ӵ��ڷ��ͻ��λ�����
//      ��ȡ���ݵ�����Ӳ������������������.
//����: uart_dev,���������豸������ָ��.
//      dst_buf,��������
//      len,���볤��,
//����: ʵ�ʶ�������
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

//----�����豸���ƺ���---------------------------------------------------------
//����: �����豸�Ŀ��ƺ���
//����: uart_handle,�������Ĵ����豸ָ��.
//      cmd,��������
//      data,������cmd����
//����: ������.
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
        case cn_uart_set_baud:  //����Baud
            if(uart_port->baud !=data1)
            {
                uart_port->baud = data1;
                uart_port->my_reg->UBRDIV = ((cn_pclk<<2)/data1 -32)>>6;
//                pg_uart1_reg->UBRDIV =((cn_pclk<<2)/data -32)>>6;
            }
            break;
        case cn_uart_send_data:     //��������
        //��IO���ư�˫��ͨ�ŵķ���ʹ�ܵĻ�,�ڴ�ת���շ�.
            break;
        case cn_uart_recv_data:     //��������
        //��IO���ư�˫��ͨ�ŵķ���ʹ�ܵĻ�,�ڴ�ת���շ�.
            break;
        case cn_uart_completed_send:
        //��IO���ư�˫��ͨ�ŵķ���ʹ�ܵĻ�,�ڴ�ת���շ�,ע��Ҫ��ѯUTRSTAT�Ĵ���
        //��Transmit shifter emptyλ�ж��Ƿ��Ѿ����.
            break;
        case cn_uart_rx_pause:      //��ͣ����
            __uart_sub_int_disable(uart_port->rx_int_line);
            break;
        case cn_uart_rx_resume:      //�ָ�����
            __uart_sub_int_enable(uart_port->rx_int_line);
            break;
        case cn_uart_recv_soft_level:    //���ý����������������ˮƽ
            uart_port->recv_trigger_level = (uint16_t)data1;
            break;
        case cn_uart_send_soft_level:    //���ý����������������ˮƽ
            uart_port->send_trigger_level = (uint16_t)data1;
            break;
        case cn_uart_overtime:   //data1Ϊ������
            uart_port->timeout = data1;
            break;
        case cn_uart_send_hard_level:    //���÷���fifo����ˮƽ
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
        case cn_uart_recv_hard_level:    //���ý���fifo����ˮƽ
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

//----���з��ͷ���-------------------------------------------------------------
//����: �¼�������,��Ӧ�ó�����豸����д������͵�����ʱ���ͻᵯ�����¼�.
//����: �¼�������û�в���.
//����: �¼����������Բ�����,��ʹ����Ҳû�з���ֵ.
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
        //16�Ǵ��ڷ���fifo������ȣ�������module_init_uart0������һ��
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
        //�ȴ��������͵��¼��ٴη��������ڱ������¼��Ѿ������ó�mark�¼������¼�
        //��������һֱ������y_clear_mark������µ������¼�ֻ�ᴥ�����¼�����ִ
        //�С����¼���uart_left_write�����е�����
        djy_wait_evtt_pop(djy_my_evtt_id(),1,cn_timeout_forever);
    }
}

//----��ʼ��uart0ģ��----------------------------------------------------------
//���ܣ���ʼ��uart0ģ��
//������ģ���ʼ������û�в���
//���أ�true = �ɹ���ʼ����false = ��ʼ��ʧ��
//-----------------------------------------------------------------------------
ptu32_t module_init_uart0(ptu32_t para)
{
    struct semaphore_LCB *left_semp,*right_semp;
    struct  pan_device   *uart_dev;
    uint16_t uart_send_evtt;
    u32 temp;
    //�������������ź�����ʹ��������������Ϊ0ʱ����д���̣߳���ȡ�߳�ʹ��������
    //���ݽ���trigger_level����ʱ�ͷ��ź�����ʹд���߳̽������
    tg_uart0_CB.send_buf_semp = semp_create(1,0,"uart0 tx buf");
    if(tg_uart0_CB.send_buf_semp == NULL)
        goto exit_from_left_buf_semp;
    tg_uart0_CB.recv_buf_semp = semp_create(1,0,"uart0 rx buf");
    if(tg_uart0_CB.recv_buf_semp == NULL)
        goto exit_from_right_buf_semp;
    //�ֱ𴴽����ֺ����ַ���uart0�豸���ź�������һ������Ϊ1��ʾ�����Ƕ�ռ����
    //�ģ��ڶ�������1��ʾ��ʼ״̬��һյ�źŵƵ�����������Ӧ�ó���ʹ�ã�������Ӳ
    //����������ʹ�á�
    left_semp = semp_create(1,1,"uart0 device left");
    if(left_semp == NULL)
        goto exit_from_left_semp;
    right_semp = semp_create(1,1,"uart0 device right");
    if(right_semp == NULL)
        goto exit_from_right_semp;
    //GPH��UART�ڹ��ýӿڣ���ʼ��IO�˿�λ��ʹ��uart����
    //Ports  :  GPH10 GPH9 GPH8 GPH7 GPH6 GPH5 GPH4 GPH3 GPH2 GPH1  GPH0
    //Signal :   xx    xx   xx   xx   xx   xx   xx  RXD0 TXD0 nRTS0 nCTS0
    //Binary :   xx    xx   xx   xx   xx   xx   xx   xx   xx   xx   xx
    temp = pg_gpio_reg->GPHCON;
    temp &= ~0xff;
    temp |= 0xaa;
    pg_gpio_reg->GPHCON = temp;
    pg_gpio_reg->GPHUP |= 0xf;   // The pull up function is disabled GPH[3:0]
    //ϵͳ��ʼ��ʱ�Ѿ�ʹ�жϴ��ڽ�ֹ״̬�������ٽ�ֹ������жϡ�
    //��ʼ��uartӲ���������ݽṹ
    tg_uart0_CB.my_reg = (struct uart_reg *)0x50000000;
    tg_uart0_CB.my_reg->ULCON = 0x3;
    tg_uart0_CB.my_reg->UCON =  0xc5;
    tg_uart0_CB.my_reg->UFCON = 0x57;   //bit0=1ʹ��fifo.
    tg_uart0_CB.my_reg->UMCON = 0x0;
    tg_uart0_CB.my_reg->UBRDIV = ((cn_pclk<<2)/115200 -32)>>6;
    temp = tg_uart0_CB.my_reg->UERSTAT;     //��һ���������״̬
    tg_uart0_CB.baud = 115200;
    tg_uart0_CB.rx_int_line = BIT_SUB_RXD0;
    tg_uart0_CB.tx_int_line = BIT_SUB_TXD0;

    //��ʼ��ͨ�ô������ݽṹ
    tg_uart0_CB.recv_trigger_level = 0;  //������մ���ˮƽΪ0
    tg_uart0_CB.send_trigger_level = uart0_buf_len>>4;  //���巢�ʹ���ˮƽΪ1/16
    tg_uart0_CB.timeout = (u32)500*mS;  //��ʱʱ��=500
    tg_uart0_CB.serial_no = 0;  //���ں���0
    ring_init(  &tg_uart0_CB.send_ring_buf,
                uart0_ptcl_send_buf,
                uart0_buf_len);
    ring_init(  &tg_uart0_CB.recv_ring_buf,
                uart0_ptcl_recv_buf,
                uart0_buf_len);
    tg_uart0_CB.status = cn_ser_rxbuf_empty;

    //���½���uart0�豸
    uart_dev = dev_add_device(dev_get_root_device(),"uart0",
                               right_semp,left_semp,
                               (dev_write_func) uart_hard_write,
                               (dev_read_func ) uart_hard_read,
                               (dev_ctrl_func ) uart_ctrl,
                               (dev_write_func ) uart_app_write,
                               (dev_read_func  ) uart_app_read,
                               (dev_ctrl_func  ) uart_ctrl,
                               (ptu32_t)(&tg_uart0_CB)  //uart0�豸��˽������
                               );
    if(uart_dev == NULL)
        goto exit_from_add_device;
    pg_uart0_rhdl = dev_open_right("uart0",0);      //�����־��
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
    int_echo_line(cn_int_line_uart0);       //�����ʼ�������ķ���fifo�յ��ж�
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
    //�������������ź�����ʹ��������������Ϊ0ʱ����д���̣߳���ȡ�߳�ʹ��������
    //���ݽ���trigger_level����ʱ�ͷ��ź�����ʹд���߳̽������
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
    //��ʼ��IO�˿�λuart����
    //Ports  :  GPH10 GPH9 GPH8 GPH7 GPH6 GPH5 GPH4 GPH3 GPH2 GPH1 GPH0
    //Signal :   xx    xx   xx   xx   xx  RXD1 TXD1  xx   xx   xx   xx
    //Binary :   xx    xx   xx   xx   xx   10   10   xx   xx   xx   xx
    temp = pg_gpio_reg->GPHCON;
    temp &= ~0xf00;
    temp |= 0xa00;
    pg_gpio_reg->GPHCON = temp;
    pg_gpio_reg->GPHUP |= 0x30;   // The pull up function is disabled GPH[5:4]
    //ϵͳ��ʼ��ʱ�Ѿ�ʹ�жϴ��ڽ�ֹ״̬�������ٽ�ֹ������жϡ�
    //��ʼ��uartӲ���������ݽṹ
    tg_uart1_CB.my_reg = (struct uart_reg *)0x50004000;
    tg_uart1_CB.my_reg->UFCON = 0x57;   //bit0=1ʹ��fifo.
    tg_uart1_CB.my_reg->UMCON = 0x0;
    tg_uart1_CB.my_reg->ULCON = 0x3;
    tg_uart1_CB.my_reg->UCON =  0xc5;
    tg_uart1_CB.my_reg->UBRDIV = ((cn_pclk<<2)/115200 -32)>>6;
    temp = tg_uart1_CB.my_reg->UERSTAT;     //��һ���������״̬
    tg_uart1_CB.baud = 115200;
    tg_uart1_CB.rx_int_line = BIT_SUB_RXD1;
    tg_uart1_CB.tx_int_line = BIT_SUB_TXD1;

    //��ʼ��ͨ�ô������ݽṹ
    tg_uart1_CB.recv_trigger_level = 0;  //������մ���ˮƽΪ0
    tg_uart1_CB.send_trigger_level = uart1_buf_len>>4;  //���巢�ʹ���ˮƽΪ1/16
    tg_uart1_CB.timeout = (u32)500*mS;  //��ʱʱ��=500
    tg_uart1_CB.serial_no = 1;  //���ں���1
    ring_init(  &tg_uart1_CB.send_ring_buf,
                uart1_ptcl_send_buf,
                uart1_buf_len);
    ring_init(  &tg_uart1_CB.recv_ring_buf,
                uart1_ptcl_recv_buf,
                uart1_buf_len);
    tg_uart1_CB.status = cn_ser_rxbuf_empty;

    //���½���uart1�豸
    uart_dev = dev_add_device(dev_get_root_device(),"uart1",
                               right_semp,left_semp,
                               (dev_write_func) uart_hard_write,
                               (dev_read_func ) uart_hard_read,
                               (dev_ctrl_func ) uart_ctrl,
                               (dev_write_func ) uart_app_write,
                               (dev_read_func  ) uart_app_read,
                               (dev_ctrl_func  ) uart_ctrl,
                               (ptu32_t)(&tg_uart1_CB)  //uart1�豸��˽������
                               );
    if(uart_dev == NULL)
        goto exit_from_add_device;
    pg_uart1_rhdl = dev_open_right("uart1",0);      //�����־��
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
    int_echo_line(cn_int_line_uart1);       //�����ʼ�������ķ���fifo�յ��ж�
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
    //�������������ź�����ʹ��������������Ϊ0ʱ����д���̣߳���ȡ�߳�ʹ��������
    //���ݽ���trigger_level����ʱ�ͷ��ź�����ʹд���߳̽������
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
    //��ʼ��IO�˿�λuart����
    //Ports  :  GPH10 GPH9 GPH8 GPH7 GPH6 GPH5 GPH4 GPH3 GPH2 GPH1 GPH0
    //Signal :   xx    xx   xx  RXD2 TXD2  xx   xx   xx   xx   xx   xx
    //Binary :   xx    xx   xx   10   10   xx   xx   xx   xx   xx   xx
    temp = pg_gpio_reg->GPHCON;
    temp &= ~0xf000;
    temp |= 0xa000;
    pg_gpio_reg->GPHCON = temp;
    pg_gpio_reg->GPHUP |= 0xc0;   // The pull up function is disabled GPH[7:6]
    //ϵͳ��ʼ��ʱ�Ѿ�ʹ�жϴ��ڽ�ֹ״̬�������ٽ�ֹ������жϡ�
    //��ʼ��uartӲ���������ݽṹ
    tg_uart2_CB.my_reg = (struct uart_reg *)0x50008000;
    tg_uart2_CB.my_reg->UFCON = 0x57;   //bit0=1ʹ��fifo.
    tg_uart2_CB.my_reg->UMCON = 0x0;
    tg_uart2_CB.my_reg->ULCON = 0x3;
    tg_uart2_CB.my_reg->UCON =  0xc5;
    tg_uart2_CB.my_reg->UBRDIV = ((cn_pclk<<2)/115200 -32)>>6;
    temp = tg_uart2_CB.my_reg->UERSTAT;     //��һ���������״̬
    tg_uart2_CB.baud = 115200;
    tg_uart2_CB.rx_int_line = BIT_SUB_RXD2;
    tg_uart2_CB.tx_int_line = BIT_SUB_TXD2;

    //��ʼ��ͨ�ô������ݽṹ
    tg_uart2_CB.recv_trigger_level = 0;  //������մ���ˮƽΪ0
    tg_uart2_CB.send_trigger_level = uart2_buf_len>>4;  //���巢�ʹ���ˮƽΪ1/16
    tg_uart2_CB.timeout = (u32)500*mS;  //��ʱʱ��=500
    tg_uart2_CB.serial_no = 2;  //���ں���2
    ring_init(  &tg_uart2_CB.send_ring_buf,
                uart2_ptcl_send_buf,
                uart2_buf_len);
    ring_init(  &tg_uart2_CB.recv_ring_buf,
                uart2_ptcl_recv_buf,
                uart2_buf_len);
    tg_uart2_CB.status = cn_ser_rxbuf_empty;

    //���½���uart2�豸
    uart_dev = dev_add_device(dev_get_root_device(),"uart2",
                               right_semp,left_semp,
                               (dev_write_func) uart_hard_write,
                               (dev_read_func ) uart_hard_read,
                               (dev_ctrl_func ) uart_ctrl,
                               (dev_write_func ) uart_app_write,
                               (dev_read_func  ) uart_app_read,
                               (dev_ctrl_func  ) uart_ctrl,
                               (ptu32_t)(&tg_uart2_CB)  //uart2�豸��˽������
                               );    //"uart2"��һ�����豸
    if(uart_dev == NULL)
        goto exit_from_add_device;
    pg_uart2_rhdl = dev_open_right("uart2",0);      //�����־��
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
    int_echo_line(cn_int_line_uart2);       //�����ʼ�������ķ���fifo�յ��ж�
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
