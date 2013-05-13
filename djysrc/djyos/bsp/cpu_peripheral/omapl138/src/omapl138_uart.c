/*========================================================*
 * �ļ�ģ��˵��:                                                 *
 * omapl138��uart��������ֲ�Զ����߲���ϵͳ����ӵ�а�Ȩ
 * ��S3C2440 uart�����ļ������omapl138����һЩ�޸�
 * �ļ��汾: v1.00                                               *
 * ������Ա: lst                              *
 * ����ʱ��: 2011.06.01                                          *
 * Copyright(c) 2011-2011  ��������������޹�˾             *
 *========================================================*
 * �����޸ļ�¼(���µķ�����ǰ��):							     *
 *  <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>				 *
 *========================================================*
* ���磺
*  V1.02 2004-07-14, ����: ���պ������ӶԱ��ĳ�����Ч��У��
*
*/

//������ԭ���߰�Ȩ����
//----------------------------------------------------
//Copyright (C), 2004-2009,  �����߲���ϵͳ�����Ŷ�
//��Ȩ���� (C), 2004-2009,   �����߲���ϵͳ�����Ŷ�
//����ģ��: uart��������
//����:  ������.
//�汾��V1.0.0
//�ļ�����: 
//����˵��:
//�޶���ʷ:
//1. ����:2009-03-10
//   ����:  ������.
//   �°汾�ţ�1.0.0
//   �޸�˵��: ��ֲ��44b0��1.0.1��
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

//----ʹ�ܽ����ж�-------------------------------------------------------------
//����: ʹ��uart�Ľ����жϡ�
//����: reg,�������ļĴ�����ָ��
//����: ��
//-----------------------------------------------------------------------------
void __uart_recv_int_enable(tag_UartReg volatile *reg)
{
    reg->IER |= (u32)1;
}

//----��ֹ�����ж�-------------------------------------------------------------
//����: ��ֹuart�Ľ����жϡ�
//����: reg,�������ļĴ�����ָ��
//����: ��
//-----------------------------------------------------------------------------
void __uart_recv_int_disable(tag_UartReg volatile *reg)
{
    reg->IER &= ~(u32)1;
}

//----ʹ�ܷ����ж�-------------------------------------------------------------
//����: ʹ��uart�ķ����жϡ�
//����: reg,�������ļĴ�����ָ��
//����: ��
//-----------------------------------------------------------------------------
void __uart_send_int_enable(tag_UartReg volatile *reg)
{
    reg->IER |= (u32)2;
}

//----��ֹ�����ж�-------------------------------------------------------------
//����: ��ֹuart�ķ����жϡ�
//����: reg,�������ļĴ�����ָ��
//����: ��
//-----------------------------------------------------------------------------
void __uart_send_int_disable(tag_UartReg volatile *reg)
{
    reg->IER &= ~(u32)2;
}

//----����baud-----------------------------------------------------------------
//����: ���ô���baud
//����: reg,�������ļĴ�����ָ��
//����: ��
//-----------------------------------------------------------------------------
void __uart_set_baud(tag_UartReg volatile *reg,u32 baud)
{
    u32 temp;
    if((u32)reg == cn_uart0_base)       //����0,����ASYNC3_CLKSRC����
    {
        temp = (u32)(cn_pll0_sysclk2+(baud>>1))/(baud*16);
        reg->DLL = (u8)temp;
        reg->DLH = (u8)(temp>>8);
    }else                               //����1�ʹ���2
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
//----��鴮�ڷ��ͼĴ����Ƿ��-------------------------------------------------
//����: ��鷢�ͼĴ�����״̬������վͷ���true
//�������ޡ�
//����: true = �������գ�false = �ǿ�
//-----------------------------------------------------------------------------
bool_t uart_tx_tran_empty(volatile tag_UartReg *uart_reg)
{
    return (0 != (uart_reg->LSR & 0x40));
}

//----��ʼ��uart0ģ��----------------------------------------------------------
//���ܣ���ʼ��uart0ģ��
//������ģ���ʼ������û�в���
//���أ�1=�ɹ���0=ʧ��
//-----------------------------------------------------------------------------
ptu32_t module_init_uart0(ptu32_t para)
{
    struct semaphore_LCB *left_semp,*right_semp;
    struct  pan_device   *uart_dev;
    uint16_t uart_send_evtt;
    vu32 temp;
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

    Cpucfg_EnableLPSC(cn_PSC0_UART0);
    gpio_cfg_pin_func(8,3,cn_p8_3_uart0_txd);
    gpio_cfg_pin_func(8,4,cn_p8_4_uart0_rxd);
    //ϵͳ��ʼ��ʱ�Ѿ�ʹ�жϴ��ڽ�ֹ״̬�������ٽ�ֹ������жϡ�
    //��ʼ��uartӲ���������ݽṹ
    tg_uart0_CB.my_reg = (tag_UartReg *)cn_uart0_base;
    __uart_set_baud(tg_uart0_CB.my_reg,9600);
    temp = tg_uart0_CB.my_reg->IIR_FCR;     //��һ����������жϱ�־
//    tg_uart0_CB.my_reg->IIR_FCR |= 1;   //set FIFOEN
    tg_uart0_CB.my_reg->IIR_FCR = 0xcf; //����14�ֽڴ����жϣ����շ�fifo��
    tg_uart0_CB.my_reg->LCR = 3 | (0<<2) | (000<<3) | (0<<6) | (0<<7);
    tg_uart0_CB.my_reg->MCR = 0;
    tg_uart0_CB.my_reg->PWREMU_MGMT = 0x7fff;
    tg_uart0_CB.my_reg->MDR = 0;        //16��ʱ��
    tg_uart0_CB.my_reg->IER =  3;       //ʹ���շ��жϣ���ʱ�ж��߿�����δʹ��
    tg_uart0_CB.baud = 9600;

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
                               (dev_write_func) uart_right_write,
                               (dev_read_func ) uart_right_read,
                               (dev_ctrl_func ) uart_ctrl,
                               (dev_write_func ) uart_left_write,
                               (dev_read_func  ) uart_left_read,
                               (dev_ctrl_func  ) uart_ctrl,
                               (ptu32_t)(&tg_uart0_CB)  //uart0�豸��˽������
                               );
    if(uart_dev == NULL)
        goto exit_from_add_device;
    pg_uart0_rhdl = dev_open_right("uart0",0);      //�����־��
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

//----��ʼ��uart1ģ��----------------------------------------------------------
//���ܣ���ʼ��uart1ģ��
//������ģ���ʼ������û�в���
//���أ�1=�ɹ���0=ʧ��
//-----------------------------------------------------------------------------
ptu32_t module_init_uart1(ptu32_t para)
{
    struct semaphore_LCB *left_semp,*right_semp;
    struct  pan_device   *uart_dev;
    uint16_t uart_send_evtt;
    vu32 temp;
    //�������������ź�����ʹ��������������Ϊ0ʱ����д���̣߳���ȡ�߳�ʹ��������
    //���ݽ���trigger_level����ʱ�ͷ��ź�����ʹд���߳̽������
    tg_uart1_CB.send_buf_semp = semp_create(1,0,"uart1 tx buf");
    if(tg_uart1_CB.send_buf_semp == NULL)
        goto exit_from_left_buf_semp;
    tg_uart1_CB.recv_buf_semp = semp_create(1,0,"uart1 rx buf");
    if(tg_uart1_CB.recv_buf_semp == NULL)
        goto exit_from_right_buf_semp;
    //�ֱ𴴽����ֺ����ַ���uart1�豸���ź�������һ������Ϊ1��ʾ�����Ƕ�ռ����
    //�ģ��ڶ�������1��ʾ��ʼ״̬��һյ�źŵƵ�����������Ӧ�ó���ʹ�ã�������Ӳ
    //����������ʹ�á�
    left_semp = semp_create(1,1,"uart1 device left");
    if(left_semp == NULL)
        goto exit_from_left_semp;
    right_semp = semp_create(1,1,"uart1 device right");
    if(right_semp == NULL)
        goto exit_from_right_semp;

    Cpucfg_EnableLPSC(cn_PSC1_UART1);
    gpio_cfg_pin_func(1,0,cn_p1_0_uart1_txd);
    gpio_cfg_pin_func(1,1,cn_p1_1_uart1_rxd);

    //ϵͳ��ʼ��ʱ�Ѿ�ʹ�жϴ��ڽ�ֹ״̬�������ٽ�ֹ������жϡ�
    //��ʼ��uartӲ���������ݽṹ
    tg_uart1_CB.my_reg = (tag_UartReg *)cn_uart1_base;
    __uart_set_baud(tg_uart1_CB.my_reg,9600);
    temp = tg_uart1_CB.my_reg->IIR_FCR;     //��һ����������жϱ�־
//    tg_uart1_CB.my_reg->IIR_FCR |= 1;   //set FIFOEN
    tg_uart1_CB.my_reg->IIR_FCR = 0xcf; //����14�ֽڴ����жϣ����շ�fifo��
    tg_uart1_CB.my_reg->LCR = 3 | (0<<2) | (000<<3) | (0<<6) | (0<<7);
    tg_uart1_CB.my_reg->MCR = 0;
    tg_uart1_CB.my_reg->PWREMU_MGMT = 0x7fff;
    tg_uart1_CB.my_reg->MDR = 0;        //16��ʱ��
    tg_uart1_CB.my_reg->IER =  3;       //ʹ���շ��жϣ���ʱ�ж��߿�����δʹ��
    tg_uart1_CB.baud = 9600;

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
                               (dev_write_func) uart_right_write,
                               (dev_read_func ) uart_right_read,
                               (dev_ctrl_func ) uart_ctrl,
                               (dev_write_func ) uart_left_write,
                               (dev_read_func  ) uart_left_read,
                               (dev_ctrl_func  ) uart_ctrl,
                               (ptu32_t)(&tg_uart1_CB)  //uart1�豸��˽������
                               );
    if(uart_dev == NULL)
        goto exit_from_add_device;
    pg_uart1_rhdl = dev_open_right("uart1",0);      //�����־��
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

//----��ʼ��uart2ģ��----------------------------------------------------------
//���ܣ���ʼ��uart2ģ��
//������ģ���ʼ������û�в���
//���أ�1=�ɹ���0=ʧ��
//-----------------------------------------------------------------------------
ptu32_t module_init_uart2(ptu32_t para)
{
    struct semaphore_LCB *left_semp,*right_semp;
    struct  pan_device   *uart_dev;
    uint16_t uart_send_evtt;
    vu32 temp;
    //�������������ź�����ʹ��������������Ϊ0ʱ����д���̣߳���ȡ�߳�ʹ��������
    //���ݽ���trigger_level����ʱ�ͷ��ź�����ʹд���߳̽������
    tg_uart2_CB.send_buf_semp = semp_create(1,0,"uart2 tx buf");
    if(tg_uart2_CB.send_buf_semp == NULL)
        goto exit_from_left_buf_semp;
    tg_uart2_CB.recv_buf_semp = semp_create(1,0,"uart2 rx buf");
    if(tg_uart2_CB.recv_buf_semp == NULL)
        goto exit_from_right_buf_semp;
    //�ֱ𴴽����ֺ����ַ���uart2�豸���ź�������һ������Ϊ1��ʾ�����Ƕ�ռ����
    //�ģ��ڶ�������1��ʾ��ʼ״̬��һյ�źŵƵ�����������Ӧ�ó���ʹ�ã�������Ӳ
    //����������ʹ�á�
    left_semp = semp_create(1,1,"uart2 device left");
    if(left_semp == NULL)
        goto exit_from_left_semp;
    right_semp = semp_create(1,1,"uart2 device right");
    if(right_semp == NULL)
        goto exit_from_right_semp;

    Cpucfg_EnableLPSC(cn_PSC1_UART2);
    gpio_cfg_pin_func(1,2,cn_p1_2_uart2_txd);
    gpio_cfg_pin_func(1,3,cn_p1_3_uart2_rxd);
    //ϵͳ��ʼ��ʱ�Ѿ�ʹ�жϴ��ڽ�ֹ״̬�������ٽ�ֹ������жϡ�
    //��ʼ��uartӲ���������ݽṹ
    tg_uart2_CB.my_reg = (tag_UartReg *)cn_uart2_base;
    __uart_set_baud(tg_uart2_CB.my_reg,9600);
    temp = tg_uart2_CB.my_reg->IIR_FCR;     //��һ����������жϱ�־
//    tg_uart2_CB.my_reg->IIR_FCR |= 1;   //set FIFOEN
    tg_uart2_CB.my_reg->IIR_FCR = 0xcf; //����14�ֽڴ����жϣ����շ�fifo��
    tg_uart2_CB.my_reg->LCR = 3 | (0<<2) | (000<<3) | (0<<6) | (0<<7);
    tg_uart2_CB.my_reg->MCR = 0;
    tg_uart2_CB.my_reg->PWREMU_MGMT = 0x7fff;
    tg_uart2_CB.my_reg->MDR = 0;        //16��ʱ��
    tg_uart2_CB.my_reg->IER =  3;       //ʹ���շ��жϣ���ʱ�ж��߿�����δʹ��
    tg_uart2_CB.baud = 9600;

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
                               (dev_write_func) uart_right_write,
                               (dev_read_func ) uart_right_read,
                               (dev_ctrl_func ) uart_ctrl,
                               (dev_write_func ) uart_left_write,
                               (dev_read_func  ) uart_left_read,
                               (dev_ctrl_func  ) uart_ctrl,
                               (ptu32_t)(&tg_uart2_CB)  //uart2�豸��˽������
                               );
    if(uart_dev == NULL)
        goto exit_from_add_device;
    pg_uart2_rhdl = dev_open_right("uart2",0);      //�����־��
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

//----uart0�ж�---------------------------------------------------------------
//����: ���գ���ȡfifo�е����ݲ�������д���豸
//      ���ͣ������ֶ����ݲ�����
//����: �жϺ�.
//����: 0.
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
            case 1:     //����fifo�Ĵ�����
            {
                recv_trans = dev_read_right(pg_uart0_rhdl,0,(ptu32_t)ch,16);
                for(num = 0; num < recv_trans; num++)
                {
                    reg->RBR_THR = ch[num];
                }
            }break;
            case 2:     //���մ���ˮƽ��
            case 6:     //���ճ�ʱ��fifo��������һ�����ݣ�����4�ַ�ʱ����������
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
            case 3:     //�����־
            {
            }break;
            default: return 0;
        }
    }
    return 0;
}

//----uart1�ж�---------------------------------------------------------------
//����: ���գ���ȡfifo�е����ݲ�������д���豸
//      ���ͣ������ֶ����ݲ�����
//����: �жϺ�.
//����: 0.
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
            case 1:     //����fifo�Ĵ�����
            {
                recv_trans = dev_read_right(pg_uart1_rhdl,0,(ptu32_t)ch,16);
                for(num = 0; num < recv_trans; num++)
                {
                    reg->RBR_THR = ch[num];
                }
            }break;
            case 2:     //���մ���ˮƽ��
            case 6:     //���ճ�ʱ��fifo��������һ�����ݣ�����4�ַ�ʱ����������
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
            case 3:     //�����־
            {
            }break;
            default: return 0;
        }
    }
    return 0;
}

//----uart2�ж�---------------------------------------------------------------
//����: ���գ���ȡfifo�е����ݲ�������д���豸
//      ���ͣ������ֶ����ݲ�����
//����: �жϺ�.
//����: 0.
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
            case 1:     //����fifo�Ĵ�����
            {
                recv_trans = dev_read_right(pg_uart2_rhdl,0,(ptu32_t)ch,16);
                for(num = 0; num < recv_trans; num++)
                {
                    reg->RBR_THR = ch[num];
                }
            }break;
            case 2:     //���մ���ˮƽ��
            case 6:     //���ճ�ʱ��fifo��������һ�����ݣ�����4�ַ�ʱ����������
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
            case 3:     //�����־
            {
            }break;
            default: return 0;
        }
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
ptu32_t uart_right_write(struct  pan_device *uart_dev,ptu32_t buf,
                            ptu32_t res2,ptu32_t len)
{
    struct uart_UCB *uart_port;
    uint16_t recv_bytes,valid_bytes,error_evtt;
    enum uart_error_no uart_error;

    uart_port = (struct uart_UCB*)dev_get_my_tag(uart_dev);

    //copy����Ӳ����������Э�黺����
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
        //������Ǽ�����д�¼�����ֻ�ܲ�ѯ��ʽ���豸��ȡ�����ˡ�
        djy_event_pop(uart_port->evtt_right_write,enum_para_async,
                                    NULL,0,NULL,0,0,0);
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
        if(result != len-completed)     //����������û���ͳ�ȫ������
        {
            completed += result;
            semp_pend(uart_port->send_buf_semp,cn_timeout_forever);
        }else
            break;
    }
    return result;
}

//----�����豸���ֶ�����------------------------------------------------------
//����: ���ֶ������ϲ��豸,��ȡ���ڽ��ջ��λ�����������
//����: uart_dev,���������豸������ָ��.
//      dst_buf,��������
//      len,���볤��,
//����: ʵ�ʶ�������
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

//----�����豸���ֶ�����------------------------------------------------------
//����: ���ֶ�����Ӳ��,Ӳ�����ͷ����� ser_send_service�Ӵ��ڷ��ͻ��λ�����
//      ��ȡ���ݵ�����Ӳ������������������.
//����: uart_dev,���������豸������ָ��.
//      dst_buf,��������
//      len,���볤��,
//����: ʵ�ʶ�������
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

//----�����豸���ƺ���---------------------------------------------------------
//����: �����豸�Ŀ��ƺ���
//����: uart_handle,�������Ĵ����豸ָ��.
//      cmd,��������
//      data,������cmd����
//����: ������.
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
        case cn_uart_set_baud:  //����Baud
            if(uart_port->baud !=data1)
            {
                uart_port->baud = data1;
                __uart_set_baud((tag_UartReg *)uart_port->my_reg,data1);
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
            __uart_recv_int_disable((tag_UartReg *)uart_port->my_reg);
            break;
        case cn_uart_rx_resume:      //�ָ�����
            __uart_recv_int_enable((tag_UartReg *)uart_port->my_reg);
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
        case cn_uart_recv_hard_level:    //���ý���fifo����ˮƽ
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
        //�ȴ��������͵��¼��ٴη�����
        djy_wait_evtt_pop(djy_my_evtt_id(),1,cn_timeout_forever);
    }
}


