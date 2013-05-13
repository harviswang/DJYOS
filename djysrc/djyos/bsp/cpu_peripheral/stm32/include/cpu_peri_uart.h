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

#define cn_dma_unused   0
#define cn_dma_used     1
//����״̬���ƽṹ
struct uart_UCB
{
    struct ring_buf send_ring_buf;           //���η��ͻ�����.
    struct ring_buf recv_ring_buf;           //���ν��ջ�����.

    struct semaphore_LCB *send_buf_semp;     //���ͻ�������
    struct semaphore_LCB *recv_buf_semp;     //���ջ�������

    uint16_t recv_target_evtt;      //�豸���ֱ�д�󵯳����¼�����
    uint16_t recv_trigger_level;    //���ڽ��մ���ˮƽ,���յ���������ʱ�����¼�
    uint32_t timeout;               //���ջ������������ݣ���δ������ˮƽ,������
                                    //��ʱ����û���յ�������,Ҳ�����¼�.��λ=us
    uint16_t send_trigger_level;    //����ͬ������ˮƽ�������ͻ��λ�����������
                                    //��send_buf_semp������ǰ�̣߳�ֱ�����ͻ���
                                    //������������send_trigger_levelʱ�ͷ�֮��
    uint32_t baud;
    ufast_t dma_used;               //0=��ʹ��dma��1=��dmaͨ��
    bool_t sendding;                //��Ϊtrue�����豸������д���������������
                                    //���ͣ�������Ҫ��������
};
//���ڴ�������
enum uart_error_no
{
    enum_recv_hard_over,         //����Ӳ�����
    enum_recv_buf_over,          //���ջ��������
};

//ͬ��/�첽����λ��bo_Ϊλƫ�ƣ�1λ����λ����ַ��bb_ǰ׺����λ�����룬bm_ǰ׺
//uart1 sr�Ĵ���λ����
#define bb_uart1_sr_base   (0x42000000 + 0x13800*32)           //λ����ַ
#define bb_uart1_sr_pe     (*(vu32*)(bb_uart1_sr_base+4*0))
#define bb_uart1_sr_fe     (*(vu32*)(bb_uart1_sr_base+4*1))
#define bb_uart1_sr_ne     (*(vu32*)(bb_uart1_sr_base+4*2))
#define bb_uart1_sr_ore    (*(vu32*)(bb_uart1_sr_base+4*3))
#define bb_uart1_sr_idle   (*(vu32*)(bb_uart1_sr_base+4*4))
#define bb_uart1_sr_rxne   (*(vu32*)(bb_uart1_sr_base+4*5))
#define bb_uart1_sr_tc     (*(vu32*)(bb_uart1_sr_base+4*6))
#define bb_uart1_sr_txe    (*(vu32*)(bb_uart1_sr_base+4*7))
#define bb_uart1_sr_lbd    (*(vu32*)(bb_uart1_sr_base+4*8))
#define bb_uart1_sr_cts    (*(vu32*)(bb_uart1_sr_base+4*9))

//uart2 sr�Ĵ���λ����
#define bb_uart2_sr_base   (0x42000000 + 0x4400*32)           //λ����ַ
#define bb_uart2_sr_pe     (*(vu32*)(bb_uart2_sr_base+4*0))
#define bb_uart2_sr_fe     (*(vu32*)(bb_uart2_sr_base+4*1))
#define bb_uart2_sr_ne     (*(vu32*)(bb_uart2_sr_base+4*2))
#define bb_uart2_sr_ore    (*(vu32*)(bb_uart2_sr_base+4*3))
#define bb_uart2_sr_idle   (*(vu32*)(bb_uart2_sr_base+4*4))
#define bb_uart2_sr_rxne   (*(vu32*)(bb_uart2_sr_base+4*5))
#define bb_uart2_sr_tc     (*(vu32*)(bb_uart2_sr_base+4*6))
#define bb_uart2_sr_txe    (*(vu32*)(bb_uart2_sr_base+4*7))
#define bb_uart2_sr_lbd    (*(vu32*)(bb_uart2_sr_base+4*8))
#define bb_uart2_sr_cts    (*(vu32*)(bb_uart2_sr_base+4*9))

//uart3 sr�Ĵ���λ����
#define bb_uart3_sr_base   (0x42000000 + 0x4800*32)           //λ����ַ
#define bb_uart3_sr_pe     (*(vu32*)(bb_uart3_sr_base+4*0))
#define bb_uart3_sr_fe     (*(vu32*)(bb_uart3_sr_base+4*1))
#define bb_uart3_sr_ne     (*(vu32*)(bb_uart3_sr_base+4*2))
#define bb_uart3_sr_ore    (*(vu32*)(bb_uart3_sr_base+4*3))
#define bb_uart3_sr_idle   (*(vu32*)(bb_uart3_sr_base+4*4))
#define bb_uart3_sr_rxne   (*(vu32*)(bb_uart3_sr_base+4*5))
#define bb_uart3_sr_tc     (*(vu32*)(bb_uart3_sr_base+4*6))
#define bb_uart3_sr_txe    (*(vu32*)(bb_uart3_sr_base+4*7))
#define bb_uart3_sr_lbd    (*(vu32*)(bb_uart3_sr_base+4*8))
#define bb_uart3_sr_cts    (*(vu32*)(bb_uart3_sr_base+4*9))

//uart4 sr�Ĵ���λ����
#define bb_uart4_sr_base    (0x42000000 + 0x4c00*32)           //λ����ַ
#define bb_uart4_sr_pe      (*(vu32*)(bb_uart4_sr_base+4*0))
#define bb_uart4_sr_fe      (*(vu32*)(bb_uart4_sr_base+4*1))
#define bb_uart4_sr_ne      (*(vu32*)(bb_uart4_sr_base+4*2))
#define bb_uart4_sr_ore     (*(vu32*)(bb_uart4_sr_base+4*3))
#define bb_uart4_sr_idle    (*(vu32*)(bb_uart4_sr_base+4*4))
#define bb_uart4_sr_rxne    (*(vu32*)(bb_uart4_sr_base+4*5))
#define bb_uart4_sr_tc      (*(vu32*)(bb_uart4_sr_base+4*6))
#define bb_uart4_sr_txe     (*(vu32*)(bb_uart4_sr_base+4*7))
#define bb_uart4_sr_lbd     (*(vu32*)(bb_uart4_sr_base+4*8))
#define bb_uart4_sr_cts     (*(vu32*)(bb_uart4_sr_base+4*9))

//uart5 sr�Ĵ���λ����
#define bb_uart5_sr_base    (0x42000000 + 0x5000*32)           //λ����ַ
#define bb_uart5_sr_pe      (*(vu32*)(bb_uart5_sr_base+4*0))
#define bb_uart5_sr_fe      (*(vu32*)(bb_uart5_sr_base+4*1))
#define bb_uart5_sr_ne      (*(vu32*)(bb_uart5_sr_base+4*2))
#define bb_uart5_sr_ore     (*(vu32*)(bb_uart5_sr_base+4*3))
#define bb_uart5_sr_idle    (*(vu32*)(bb_uart5_sr_base+4*4))
#define bb_uart5_sr_rxne    (*(vu32*)(bb_uart5_sr_base+4*5))
#define bb_uart5_sr_tc      (*(vu32*)(bb_uart5_sr_base+4*6))
#define bb_uart5_sr_txe     (*(vu32*)(bb_uart5_sr_base+4*7))
#define bb_uart5_sr_lbd     (*(vu32*)(bb_uart5_sr_base+4*8))
#define bb_uart5_sr_cts     (*(vu32*)(bb_uart5_sr_base+4*9))

//uart1 cr1�Ĵ���λ����
#define bb_uart1_cr1_base      (0x42000000 + 0x1380c*32)           //λ����ַ
#define bb_uart1_cr1_sbk       (*(vu32*)(bb_uart1_cr1_base+4*0))
#define bb_uart1_cr1_rwu       (*(vu32*)(bb_uart1_cr1_base+4*1))
#define bb_uart1_cr1_re        (*(vu32*)(bb_uart1_cr1_base+4*2))
#define bb_uart1_cr1_te        (*(vu32*)(bb_uart1_cr1_base+4*3))
#define bb_uart1_cr1_idleie    (*(vu32*)(bb_uart1_cr1_base+4*4))
#define bb_uart1_cr1_rxneie    (*(vu32*)(bb_uart1_cr1_base+4*5))
#define bb_uart1_cr1_tcie      (*(vu32*)(bb_uart1_cr1_base+4*6))
#define bb_uart1_cr1_txeie     (*(vu32*)(bb_uart1_cr1_base+4*7))
#define bb_uart1_cr1_peie      (*(vu32*)(bb_uart1_cr1_base+4*8))
#define bb_uart1_cr1_ps        (*(vu32*)(bb_uart1_cr1_base+4*9))
#define bb_uart1_cr1_pce       (*(vu32*)(bb_uart1_cr1_base+4*10))
#define bb_uart1_cr1_wake      (*(vu32*)(bb_uart1_cr1_base+4*11))
#define bb_uart1_cr1_m         (*(vu32*)(bb_uart1_cr1_base+4*12))
#define bb_uart1_cr1_ue        (*(vu32*)(bb_uart1_cr1_base+4*13))

//uart2 cr1�Ĵ���λ����
#define bb_uart2_cr1_base      (0x42000000 + 0x440c*32)           //λ����ַ
#define bb_uart2_cr1_sbk       (*(vu32*)(bb_uart2_cr1_base+4*0))
#define bb_uart2_cr1_rwu       (*(vu32*)(bb_uart2_cr1_base+4*1))
#define bb_uart2_cr1_re        (*(vu32*)(bb_uart2_cr1_base+4*2))
#define bb_uart2_cr1_te        (*(vu32*)(bb_uart2_cr1_base+4*3))
#define bb_uart2_cr1_idleie    (*(vu32*)(bb_uart2_cr1_base+4*4))
#define bb_uart2_cr1_rxneie    (*(vu32*)(bb_uart2_cr1_base+4*5))
#define bb_uart2_cr1_tcie      (*(vu32*)(bb_uart2_cr1_base+4*6))
#define bb_uart2_cr1_txeie     (*(vu32*)(bb_uart2_cr1_base+4*7))
#define bb_uart2_cr1_peie      (*(vu32*)(bb_uart2_cr1_base+4*8))
#define bb_uart2_cr1_ps        (*(vu32*)(bb_uart2_cr1_base+4*9))
#define bb_uart2_cr1_pce       (*(vu32*)(bb_uart2_cr1_base+4*10))
#define bb_uart2_cr1_wake      (*(vu32*)(bb_uart2_cr1_base+4*11))
#define bb_uart2_cr1_m         (*(vu32*)(bb_uart2_cr1_base+4*12))
#define bb_uart2_cr1_ue        (*(vu32*)(bb_uart2_cr1_base+4*13))

//uart3 cr1�Ĵ���λ����
#define bb_uart3_cr1_base      (0x42000000 + 0x480c*32)           //λ����ַ
#define bb_uart3_cr1_sbk       (*(vu32*)(bb_uart3_cr1_base+4*0))
#define bb_uart3_cr1_rwu       (*(vu32*)(bb_uart3_cr1_base+4*1))
#define bb_uart3_cr1_re        (*(vu32*)(bb_uart3_cr1_base+4*2))
#define bb_uart3_cr1_te        (*(vu32*)(bb_uart3_cr1_base+4*3))
#define bb_uart3_cr1_idleie    (*(vu32*)(bb_uart3_cr1_base+4*4))
#define bb_uart3_cr1_rxneie    (*(vu32*)(bb_uart3_cr1_base+4*5))
#define bb_uart3_cr1_tcie      (*(vu32*)(bb_uart3_cr1_base+4*6))
#define bb_uart3_cr1_txeie     (*(vu32*)(bb_uart3_cr1_base+4*7))
#define bb_uart3_cr1_peie      (*(vu32*)(bb_uart3_cr1_base+4*8))
#define bb_uart3_cr1_ps        (*(vu32*)(bb_uart3_cr1_base+4*9))
#define bb_uart3_cr1_pce       (*(vu32*)(bb_uart3_cr1_base+4*10))
#define bb_uart3_cr1_wake      (*(vu32*)(bb_uart3_cr1_base+4*11))
#define bb_uart3_cr1_m         (*(vu32*)(bb_uart3_cr1_base+4*12))
#define bb_uart3_cr1_ue        (*(vu32*)(bb_uart3_cr1_base+4*13))

//uart4 cr1�Ĵ���λ����
#define bb_uart4_cr1_base       (0x42000000 + 0x4c0c*32)           //λ����ַ
#define bb_uart4_cr1_sbk        (*(vu32*)(bb_uart4_cr1_base+4*0))
#define bb_uart4_cr1_rwu        (*(vu32*)(bb_uart4_cr1_base+4*1))
#define bb_uart4_cr1_re         (*(vu32*)(bb_uart4_cr1_base+4*2))
#define bb_uart4_cr1_te         (*(vu32*)(bb_uart4_cr1_base+4*3))
#define bb_uart4_cr1_idleie     (*(vu32*)(bb_uart4_cr1_base+4*4))
#define bb_uart4_cr1_rxneie     (*(vu32*)(bb_uart4_cr1_base+4*5))
#define bb_uart4_cr1_tcie       (*(vu32*)(bb_uart4_cr1_base+4*6))
#define bb_uart4_cr1_txeie      (*(vu32*)(bb_uart4_cr1_base+4*7))
#define bb_uart4_cr1_peie       (*(vu32*)(bb_uart4_cr1_base+4*8))
#define bb_uart4_cr1_ps         (*(vu32*)(bb_uart4_cr1_base+4*9))
#define bb_uart4_cr1_pce        (*(vu32*)(bb_uart4_cr1_base+4*10))
#define bb_uart4_cr1_wake       (*(vu32*)(bb_uart4_cr1_base+4*11))
#define bb_uart4_cr1_m          (*(vu32*)(bb_uart4_cr1_base+4*12))
#define bb_uart4_cr1_ue         (*(vu32*)(bb_uart4_cr1_base+4*13))

//uart5 cr1�Ĵ���λ����
#define bb_uart5_cr1_base       (0x42000000 + 0x500c*32)           //λ����ַ
#define bb_uart5_cr1_sbk        (*(vu32*)(bb_uart5_cr1_base+4*0))
#define bb_uart5_cr1_rwu        (*(vu32*)(bb_uart5_cr1_base+4*1))
#define bb_uart5_cr1_re         (*(vu32*)(bb_uart5_cr1_base+4*2))
#define bb_uart5_cr1_te         (*(vu32*)(bb_uart5_cr1_base+4*3))
#define bb_uart5_cr1_idleie     (*(vu32*)(bb_uart5_cr1_base+4*4))
#define bb_uart5_cr1_rxneie     (*(vu32*)(bb_uart5_cr1_base+4*5))
#define bb_uart5_cr1_tcie       (*(vu32*)(bb_uart5_cr1_base+4*6))
#define bb_uart5_cr1_txeie      (*(vu32*)(bb_uart5_cr1_base+4*7))
#define bb_uart5_cr1_peie       (*(vu32*)(bb_uart5_cr1_base+4*8))
#define bb_uart5_cr1_ps         (*(vu32*)(bb_uart5_cr1_base+4*9))
#define bb_uart5_cr1_pce        (*(vu32*)(bb_uart5_cr1_base+4*10))
#define bb_uart5_cr1_wake       (*(vu32*)(bb_uart5_cr1_base+4*11))
#define bb_uart5_cr1_m          (*(vu32*)(bb_uart5_cr1_base+4*12))
#define bb_uart5_cr1_ue         (*(vu32*)(bb_uart5_cr1_base+4*13))
//uasrt1��0x40013800
//uasrt2��0x40004400
//uasrt3��0x40004800
//uasrt4��0x40004c00
//uasrt5��0x40005000
struct st_usart_reg
{
  vu16 SR;      //״̬�Ĵ�����b31��10��������Ӳ��ǿ��Ϊ0
                //rc��b9��CTS��cts��־��0=cts�ޱ仯��1=cts�б仯
                //rc��b8��LBD��LIN break����־��0=û�м�⵽LIN break��1=��֮
                //r��b7��TXE���������ݼĴ����գ�0 = �ǿգ�1=��
                //rc��TC��b6��������ɱ�־��0=δ��ɣ�1=���
                //r��RXNE��b5������ready��0=��ready��1=ready
                //r��IDLE��b4�����߿��б�־��0=�ǿ��У�1=����
                //r��ORE��b3�����������־��0=�������1=���
                //r��NE��b2�����������־��0=��������1=����������SR�ٶ�DR������
                //           ����־���жϣ���Ϊ����RXNE��־һ����֡�
                //r��FE��b1��֡������NEһ����
                //r��PE��b0��У�����0=�޴�1=�д���SR�ٶ�DR������
  u16 RESERVED0;
  vu16 DR;      //�շ����ݼĴ�������9λ��Ч��һ����ַ�������Ĵ�����
  u16 RESERVED1;
  vu16 BRR;     //Baud�Ĵ�����16λ����������4λ��С����Baud = pclk/(16*BRR)
  u16 RESERVED2;
  vu16 CR1;     //bit31~14,������Ӳ��ǿ��Ϊ0
                //rw��UE��b13��ʹ��λ��0=��ֹuart��1=����
                //rw��M��b12���ֳ���0=8����λ��n��ֹͣλ��1=9����λ��1ֹͣλ
                //rw��WAKE��b11�����ͨ�Ż��ѷ�����0=�������ߣ�1=��ַ���
                //rw��PCE��b10��У��ʹ�ܣ�0=��ֹ��1=ʹ��
                //rw��PS��b9��У��ѡ��0=żУ�飬1=��У��
                //rw��PEIE��b8��PE�ж�ʹ�ܣ�����RS�Ĵ����е�PEλ�Ƿ�����ж�
                //                0=��ֹ��1=����
                //rw��TXEIE��b7��ͬ�ϣ�����SR��TXEλ
                //rw��TCIE��b6��ͬ�ϣ�����SR��TCλ
                //rw��RXNEIE��b5��ͬ�ϣ�����SR��RXNEλ
                //rw��IDLEIE��b4��ͬ�ϣ�����SR��IDLEλ
                //rw��TE��b3������ʹ�ܣ�0=��ֹ��1=����
                //rw��RE��b2������ʹ�ܣ�0=��ֹ��1=����
                //rw��RWU��b1�����뾲Ĭģʽ��0=����ģʽ��1=��Ĭģʽ��
                //    ���ڶ��ͨ�ţ���������1�ֽں���ܽ��뾲Ĭģʽ
                //rw��SBK��b0��0=�޶�����1=��Ҫ���ͶϿ�֡������LINģʽ
  u16 RESERVED3;
  vu16 CR2;     //bit31~15,������Ӳ��ǿ��Ϊ0
                //rw��LINEN��b14��LINģʽʹ�ܣ�0=��ֹ��1=ʹ��
                //rw��STOP��b13~12��stopλ����CR1�е�MλΪ0����Ч��
                //          00=1bit��01=0.5bit��10=2bit��11=1.5bit
                //rw��CLKEN��b11��ʱ��ʹ�ܣ�0=SCLK���Ž�ֹ��1=��������ͬ��ͨ��
                //rw��CPOL��b10��SCLK�ϵ�ʱ�Ӽ��ԣ�0=���߿���ʱ�ͣ�1=��
                //rw��CPHA��b9��SCLKʱ����λ��0=SCLK��һ�����ز�����1=�ڶ�����
                //rw��LBCL��b8��ͬ��ģʽ�£��Ƿ������һ���ֽڵ�MSBʱ������
                //          0=�������1=���
                //rw��b7��������ǿ��0
                //rw��LBDIE��b6��LIN break����ж�ʹ�ܣ�0=��ֹ��1=ʹ��
                //rw��LBDL��b5��LIN break��ⳤ�ȣ�0=10λ��1=11λ
                //rw��b4��������ǿ��0
                //rw��ADD[3:0]��b3~0�����ͨ��ģʽ�¸�USART�ڵ�ĵ�ַ
  u16 RESERVED4;
  vu16 CR3;     //bit31~11,������Ӳ��ǿ��Ϊ0
                //rw��CTSIE��b10��CTS�ж�ʹ�ܣ�0=��ֹ��1=SR��CTS��λ���ж�
                //rw��CTSE��b9��CTSӲ������ʹ�ܣ�0=��ֹ��1=ʹ��
                //rw��RTSE��b8��RTSӲ������ʹ�ܣ�0=��ֹ��1=ʹ��
                //rw��DMAT��b7��DMAʹ�ܷ��ͣ�0=��ֹ��1=ʹ��
                //rw��DMAR��b6��DMAʹ�ܽ��գ�0=��ֹ��1=ʹ��
                //rw��SCEN��b5�����ܿ�ģʽʹ�ܣ�0=��ֹ��1=ʹ��
                //rw��NACK��b4�����ܿ�NACKʹ�ܣ�0=У������ǲ���NACK��1=��֮
                //rw��HDSEL��b3��0=ѡ���˫��ѡ��1=��ѡ
                //rw��IRLP��b2��0=����ģʽ��1=����͹���ģʽ
                //rw��IREN��b1������ģʽʹ�ܣ�0=��ֹ��1=ʹ��
                //rw��EIE��b0�������ж�ʹ�ܣ�0=��ֹ��1=ʹ��
                //rw��
                //rw��
  u16 RESERVED5;
  vu16 GTPR;    //����ʱ���Ԥ��Ƶ�Ĵ������������ܿ�ģʽ
  u16 RESERVED6;
} ;

#define uart1_buf_len  64
#define uart2_buf_len  64
#define uart3_buf_len  64
#define uart4_buf_len  64
#define uart5_buf_len  64

//�����豸���������������uart_ctrl������
#define cn_uart_connect_recv_evtt       0   //�豸��������д������ݳ����������
                                            //ˮƽʱ�����������¼�
#define cn_uart_disconnect_recv_evtt    1   //�Ͽ������¼�
#define cn_uart_start                   2   //��������
#define cn_uart_stop                    3   //ֹͣ����
#define cn_uart_set_baud                4   //����uartBaud.
#define cn_uart_recv_soft_level         5   //���ý����������������ˮƽ
#define cn_uart_send_soft_level         6   //���÷����������������ˮƽ
#define cn_uart_overtime                7   //���ý��ճ�ʱʱ�䣬������
#define cn_uart_half_duplex_send        8   //��������
#define cn_uart_half_duplex_recv        9   //��������
#define cn_uart_setto_all_duplex        10  //����Ϊȫ˫����ʽ
#define cn_uart_en_rts                  11  //ʹ��rts����
#define cn_uart_dis_rts                 12  //��ֹrts����
#define cn_uart_en_cts                  13  //ʹ��cts����
#define cn_uart_dis_cts                 14  //��ֹcts����
#define cn_uart_dma_used                15  //����ʹ��dma���գ�uart5��֧��
#define cn_uart_dma_unused              16  //���ò�ʹ��dma����

ptu32_t uart_hard_write(struct  pan_device *uart_dev,ptu32_t src_buf,
                            ptu32_t res,ptu32_t len);
ptu32_t uart_hard_read(struct  pan_device *uart_dev,ptu32_t src_buf,
                            ptu32_t res,ptu32_t len);
void uart_error_service(void);
void uart1_hard_recv_service(void);

ptu32_t module_init_uart1(ptu32_t para);
uint32_t  uart1_int(ufast_t uart_int_line);
void __uart1_stop(void);
void __uart1_start(void);
void __uart1_start_send(void);
void __uart1_pause_recv_int(void);
void __uart1_resume_recv_int(void);
void __uart1_resume_send_int(void);
void __uart1_pause_send_int(void);
void __uart1_set_dma_used(void);
void __uart1_set_dma_unused(void);
uint32_t dma1ch5_int(ufast_t int_line);
uint32_t dma1ch4_int(ufast_t int_line);
uint32_t uart1_int(ufast_t uart_int_line);
ptu32_t uart1_ctrl(struct  pan_device *uart_dev,uint32_t cmd,
                   ptu32_t data1,ptu32_t data2);
ptu32_t uart1_app_write(struct  pan_device *uart_dev,ptu32_t src_buf,
                            ptu32_t res,ptu32_t len);
ptu32_t uart1_app_read(struct  pan_device *uart_dev,ptu32_t src_buf,
                            ptu32_t res,ptu32_t len);
void uart2_hard_recv_service(void);

ptu32_t module_init_uart2(ptu32_t para);
uint32_t  uart2_int(ufast_t uart_int_line);
void __uart2_stop(void);
void __uart2_start(void);
void __uart2_start_send(void);
void __uart2_pause_recv_int(void);
void __uart2_resume_recv_int(void);
void __uart2_resume_send_int(void);
void __uart2_pause_send_int(void);
void __uart2_set_dma_used(void);
void __uart2_set_dma_unused(void);
uint32_t dma1ch6_int(ufast_t int_line);
uint32_t dma1ch7_int(ufast_t int_line);
uint32_t uart2_int(ufast_t uart_int_line);
ptu32_t uart2_ctrl(struct  pan_device *uart_dev,uint32_t cmd,
                   ptu32_t data1,ptu32_t data2);
ptu32_t uart2_app_write(struct  pan_device *uart_dev,ptu32_t src_buf,
                            ptu32_t res,ptu32_t len);
ptu32_t uart2_app_read(struct  pan_device *uart_dev,ptu32_t src_buf,
                            ptu32_t res,ptu32_t len);
void uart3_hard_recv_service(void);

ptu32_t module_init_uart3(ptu32_t para);
uint32_t  uart3_int(ufast_t uart_int_line);
void __uart3_stop(void);
void __uart3_start(void);
void __uart3_start_send(void);
void __uart3_pause_recv_int(void);
void __uart3_resume_recv_int(void);
void __uart3_resume_send_int(void);
void __uart3_pause_send_int(void);
void __uart3_set_dma_used(void);
void __uart3_set_dma_unused(void);
uint32_t dma1ch3_int(ufast_t int_line);
uint32_t dma1ch2_int(ufast_t int_line);
uint32_t uart3_int(ufast_t uart_int_line);
ptu32_t uart3_ctrl(struct  pan_device *uart_dev,uint32_t cmd,
                   ptu32_t data1,ptu32_t data2);
ptu32_t uart3_app_write(struct  pan_device *uart_dev,ptu32_t src_buf,
                            ptu32_t res,ptu32_t len);
ptu32_t uart3_app_read(struct  pan_device *uart_dev,ptu32_t src_buf,
                            ptu32_t res,ptu32_t len);
void uart4_hard_recv_service(void);

ptu32_t module_init_uart4(ptu32_t para);
uint32_t  uart4_int(ufast_t uart_int_line);
void __uart4_stop(void);
void __uart4_start(void);
void __uart4_start_send(void);
void __uart4_pause_recv_int(void);
void __uart4_resume_recv_int(void);
void __uart4_resume_send_int(void);
void __uart4_pause_send_int(void);
void __uart4_set_dma_used(void);
void __uart4_set_dma_unused(void);
uint32_t dma2ch3_int(ufast_t int_line);
uint32_t dma2ch5_int(ufast_t int_line);
uint32_t uart4_int(ufast_t uart_int_line);
ptu32_t uart4_ctrl(struct  pan_device *uart_dev,uint32_t cmd,
                   ptu32_t data1,ptu32_t data2);
ptu32_t uart4_app_write(struct  pan_device *uart_dev,ptu32_t src_buf,
                            ptu32_t res,ptu32_t len);
ptu32_t uart4_app_read(struct  pan_device *uart_dev,ptu32_t src_buf,
                            ptu32_t res,ptu32_t len);
void uart5_hard_recv_service(void);

ptu32_t module_init_uart5(ptu32_t para);
uint32_t  uart5_int(ufast_t uart_int_line);
void __uart5_stop(void);
void __uart5_start(void);
void __uart5_start_send(void);
void __uart5_pause_recv_int(void);
void __uart5_resume_recv_int(void);
void __uart5_resume_send_int(void);
void __uart5_pause_send_int(void);
uint32_t uart5_int(ufast_t uart_int_line);
ptu32_t uart5_ctrl(struct  pan_device *uart_dev,uint32_t cmd,
                   ptu32_t data1,ptu32_t data2);
ptu32_t uart5_app_write(struct  pan_device *uart_dev,ptu32_t src_buf,
                            ptu32_t res,ptu32_t len);
ptu32_t uart5_app_read(struct  pan_device *uart_dev,ptu32_t src_buf,
                            ptu32_t res,ptu32_t len);

#ifdef __cplusplus
}
#endif

#endif // _UART_H_

