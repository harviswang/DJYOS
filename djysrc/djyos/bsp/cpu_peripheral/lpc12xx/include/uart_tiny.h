//----------------------------------------------------
//Copyright (C), 2004-2009,  lst.
//��Ȩ���� (C), 2004-2009,   lst.
//����ģ��: uart��������
//���ߣ�lst
//�汾��V1.0.1
//�ļ�����: ����44b0��uart
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2009-03-10
//   ����: lst
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#ifndef __UART_TINY_H__
#define __UART_TINY_H__

#ifdef __cplusplus
extern "C" {
#endif

//����״̬���ƽṹ
struct uart_UCB
{
    struct ring_buf send_ring_buf;           //���η��ͻ�����.
    struct ring_buf recv_ring_buf;           //���ν��ջ�����.

    struct semaphore_LCB *send_buf_semp;     //���ͻ�������
    struct semaphore_LCB *recv_buf_semp;     //���ջ�������

    u32 baud;
    bool_t sendding;                //��Ϊtrue�����豸������д���������������
                                    //���ͣ�������Ҫ��������
};

#define uart0_buf_len  64
#define uart1_buf_len  64

//���ڴ�������
enum uart_error_no
{
    enum_recv_hard_over,         //����Ӳ�����
    enum_recv_buf_over,          //���ջ��������
};

ptu32_t module_init_uart0(ptu32_t para);
ptu32_t module_init_uart1(ptu32_t para);
bool_t uart_set_baud(u32 uart_no,u32 baud);
void uart0_send(u8 *buf,u32 len);
void uart1_send(u8 *buf,u32 len);
u32 uart0_read(u8 *buf,u32 len,u32 timeout);
u32 uart1_read(u8 *buf,u32 len,u32 timeout);


#ifdef __cplusplus
}
#endif

#endif // __UART_TINY_H__

