//----------------------------------------------------
// Copyright (c) 2014, SHENZHEN PENGRUI SOFT CO LTD. All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:

// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------
// Copyright (c) 2014 ����Ȩ����������������޹�˾���С�����Ȩ�˱���һ��Ȩ����
//
// �����Ȩ�����ʹ���߷������������������£�����ʹ����ʹ�ü���ɢ����
// �����װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б���
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б��Լ�����
//    ������������

// ����������������Ǳ������Ȩ�������Լ�����������״��"as is"���ṩ��
// �������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
//-----------------------------------------------------------------------------
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
#ifndef __CPU_PERI_UART_H__
#define __CPU_PERI_UART_H__

#ifdef __cplusplus
extern "C" {
#endif


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


typedef struct
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
}tagUartReg;


#define CN_UART1    0
#define CN_UART2    1
#define CN_UART3    2
#define CN_UART4    3
#define CN_UART5    4
#define CN_UART_NUM 5

ptu32_t ModuleInstall_UART(ptu32_t para);

#ifdef __cplusplus
}
#endif

#endif // __CPU_PERI_UART_H__

