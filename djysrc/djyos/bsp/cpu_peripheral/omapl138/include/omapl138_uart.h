/*========================================================*
 * �ļ�ģ��˵��: omapl138��uart��������ֲ�Զ����߲���ϵͳ����ӵ�а�Ȩ
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
#ifndef __CPU_PERI_UART_H__
#define __CPU_PERI_UART_H__

#ifdef __cplusplus
extern "C" {
#endif

//base address
#define cn_uart0_base   0x01c42000
#define cn_uart1_base   0x01d0c000
#define cn_uart2_base   0x01d0d000
typedef struct         //offset
{
    vu32 RBR_THR;         // 0x0000
    vu32 IER;             // 0x0004
    vu32 IIR_FCR;         // 0x0008
    vu32 LCR;             // 0x000C
    vu32 MCR;             // 0x0010
    vu32 LSR;             // 0x0014
    vu32 MSR;             // 0x0018
    vu32 SCR;             // 0x001C
    vu32 DLL;             // 0x0020
    vu32 DLH;             // 0x0024
    vu32 REV_ID1;         // 0x0028
    vu32 REV_ID2;         // 0x002C
    vu32 PWREMU_MGMT;     // 0x0030
    vu32 MDR;             // 0x0034
}tag_UartReg;


#define uart0_buf_len  512
#define uart1_buf_len  512
#define uart2_buf_len  512

//����״̬���ƽṹ
struct uart_UCB
{
    struct ring_buf send_ring_buf;           //���η��ͻ�����.
    struct ring_buf recv_ring_buf;           //���ν��ջ�����.

    struct semaphore_LCB *send_buf_semp;     //���ͻ�������
    struct semaphore_LCB *recv_buf_semp;     //���ջ�������

    u16 evtt_right_write;      //�豸���ֱ�д�󵯳����¼�����
    u16 evtt_left_write;       //�豸���ֱ�д�󵯳����¼�����
    u16 recv_trigger_level;    //���ڽ��մ���ˮƽ,���յ���������ʱ�����¼�
    u32 timeout;               //δ������ˮƽ,�����趨ʱ����û���յ�����,
                                    //Ҳ�����¼�.
    u16 send_trigger_level;    //����ͬ������ˮƽ�������ͻ��λ�����������
                                    //��send_buf_semp������ǰ�̣߳�ֱ�����ͻ���
                                    //������������send_trigger_levelʱ�ͷ�֮��
    tag_UartReg volatile *my_reg;
    u32 baud;
    ufast_t status;
    ufast_t serial_no;
};

//ͨ��״̬������λ����
#define cn_ser_tx_busy         (1<<0)
#define cn_ser_rxbuf_empty     (1<<1)
#define cn_ser_receive_over    (1<<2)

//���ڴ�������
enum uart_error_no
{
    enum_recv_fifo_over,         //����Ӳ��fifo���
    enum_recv_buf_over,          //���ջ��������
};

//�����豸���������������uart_ctrl������
#define cn_uart_connect_recv_evtt     0  //�豸��������д������ݳ����������ˮ
                                         //ƽʱ�����������¼�
#define cn_uart_disconnect_recv_evtt  1  //�Ͽ������¼�
#define cn_uart_connect_send_evtt     2  //�豸��������д������ʱ�����������¼�
#define cn_uart_disconnect_send_evtt  3  //�Ͽ������¼�
#define cn_uart_start                 4  //��������
#define cn_uart_stop                  5  //ֹͣ����
#define cn_uart_set_baud              6  //����uartBaud.
#define cn_uart_send_data             7  //��������
#define cn_uart_recv_data             8  //��������
#define cn_uart_completed_send        9  //��ɷ��͹���
#define cn_uart_rx_pause              10 //��ͣ�������ݵ�������
#define cn_uart_rx_resume             11 //���¿�ʼ�������ݵ�������
#define cn_uart_overtime              12 //���ý��ճ�ʱʱ�䣬������
#define cn_uart_recv_soft_level       13 //���ý����������������ˮƽ
#define cn_uart_send_soft_level       14 //���ý����������������ˮƽ
#define cn_uart_recv_hard_level       15 //���ý���fifo����ˮƽ
#define cn_uart_send_hard_level       16 //���÷���fifo����ˮƽ

ptu32_t module_init_uart0(ptu32_t para);
ptu32_t module_init_uart1(ptu32_t para);
ptu32_t module_init_uart2(ptu32_t para);
ptu32_t module_init_uart3(ptu32_t para);

#ifdef __cplusplus
}
#endif

#endif //__CPU_PERI_UART_H__


