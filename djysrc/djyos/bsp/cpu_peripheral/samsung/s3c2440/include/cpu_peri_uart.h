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
#ifndef _UART_H_
#define _UART_H_

#ifdef __cplusplus
extern "C" {
#endif

//UTRSTAT״̬λ����
#define UartTXShiftEmpty    (1<<2)  //1:������λ�Ĵ�����,0:�ǿ�
#define UartTxBufEmpty      (1<<1)  //1:���ͻ�������,0:�ǿ�
#define UartRxBufNotEmpty   (1<<0)  //1:���ܻ�����������,0:������
//UFCON FIFO���ƼĴ���
#define cn_tx_fifo_reset    (1<<2)  //д1��λ����fifo,������0
#define cn_rx_fifo_reset    (1<<1)  //д1��λ����fifo,������0
//UFSTAT FIFO״̬�Ĵ���
#define cn_rx_over          (1<<6)  //1:����fifo���,0:����

//ͨ��״̬������λ����
#define cn_ser_tx_busy         (1<<0)
#define cn_ser_rxbuf_empty     (1<<1)
#define cn_ser_receive_over    (1<<2)

//�жϿ���λ����2410���жϿ������У�ÿ������ֻ������һ���жϣ������ж�ԭ����ͨ��
//��ѯ SUBSRCPND �Ĵ�����Ӧ��λ���õ��ģ����� INTSUBMSK ����Ӧλ�ɶ����ؽ�ֹ
//����/�����жϡ�
#define BIT_SUB_ERR2    (0x1<<8)
#define BIT_SUB_TXD2    (0x1<<7)
#define BIT_SUB_RXD2    (0x1<<6)
#define BIT_SUB_ERR1    (0x1<<5)
#define BIT_SUB_TXD1    (0x1<<4)
#define BIT_SUB_RXD1    (0x1<<3)
#define BIT_SUB_ERR0    (0x1<<2)
#define BIT_SUB_TXD0    (0x1<<1)
#define BIT_SUB_RXD0    (0x1<<0)
#define BIT_ALL_UART    (0x1ff)

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
    uint16_t send_trigger_level;    //����ͬ������ˮƽ�������ͻ��λ�����������
                                    //��send_buf_semp������ǰ�̣߳�ֱ�����ͻ���
                                    //������������send_trigger_levelʱ�ͷ�֮��
    struct uart_reg volatile *my_reg;
    u32 baud;
    u32 rx_int_line,tx_int_line;    //���ͺͽ����жϺ�
    ufast_t status;
    ufast_t serial_no;
};
//���ڴ�������
enum uart_error_no
{
    enum_recv_fifo_over,         //����Ӳ��fifo���
    enum_recv_buf_over,          //���ջ��������
};

struct uart_reg
{
    u32 ULCON;     //0x50000000 0x50004000 0x50008000 Line control
    u32 UCON;      //0x50000004 0x50004004 0x50008004 Control
    u32 UFCON;     //0x50000008 0x50004008 0x50008008 FIFO control
    u32 UMCON;     //0x5000000c 0x5000400c 0x5000800c Modem control
    u32 UTRSTAT;   //0x50000010 0x50004010 0x50008010 Tx/Rx status
    u32 UERSTAT;   //0x50000014 0x50004014 0x50008014 Rx error status
    u32 UFSTAT;    //0x50000018 0x50004018 0x50008018 FIFO status
    u32 UMSTAT;    //0x5000001c 0x5000401c 0x5000801c Modem status

#ifdef __BIG_ENDIAN
    u8  reserve;
    u8  reserve1;
    u8  reserve2;
    u8  UTXH;      //0x50000023 0x50004023 0x50008023
    u8  reserve3;
    u8  reserve4;
    u8  reserve5;
    u8  URXH;      //0x50000027 0x50004027 0x50008027

#else //Little Endian
    u8  UTXH;      //0x50000020 0x50004020 0x50008020
    u8  reserve;   //UTXH��URXH���ֽڷ��ʼĴ���,reserve��ռλ����.
    u8  reserve1;
    u8  reserve2;
    u8  URXH;      //0x50000024 0x50004024 0x50008024
    u8  reserve3;
    u8  reserve4;
    u8  reserve5;
 #endif
    u32 UBRDIV;    //0x50000028 0x50004028 0x50008028 Baud rate divisor
};

#define uart0_buf_len  512
#define uart1_buf_len  512
#define uart2_buf_len  512

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

ptu32_t uart_ctrl(struct  pan_device *uart_dev,u32 cmd,
                   ptu32_t data1,ptu32_t data2);
ptu32_t uart_app_write(struct  pan_device *uart_dev,ptu32_t src_buf,
                            ptu32_t res,ptu32_t len);
ptu32_t uart_hard_write(struct  pan_device *uart_dev,ptu32_t src_buf,
                            ptu32_t res,ptu32_t len);
ptu32_t uart_app_read(struct  pan_device *uart_dev,ptu32_t src_buf,
                            ptu32_t res,ptu32_t len);
ptu32_t uart_hard_read(struct  pan_device *uart_dev,ptu32_t src_buf,
                            ptu32_t res,ptu32_t len);
u32  uart0_int(ufast_t uart_int_line);
u32  uart1_int(ufast_t uart_int_line);
u32  uart2_int(ufast_t uart_int_line);
ptu32_t module_init_uart0(ptu32_t para);
ptu32_t module_init_uart1(ptu32_t para);
ptu32_t module_init_uart2(ptu32_t para);
void uart0_error_service(void);
void uart1_error_service(void);
void uart2_error_service(void);
void uart_send_service(void);

extern uint16_t u16g_evtt_uart0_error;
extern uint16_t u16g_evtt_uart1_error;

#ifdef __cplusplus
}
#endif

#endif // _UART_H_

