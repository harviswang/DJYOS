#ifndef __CPU_PERI_UART__
#define __CPU_PERI_UART__

#ifdef __cplusplus
extern "C" {
#endif

//�Ĵ���λƫ�ƶ���
//�ж�ʹ�ܼĴ���λierƫ�ƶ���
#define bo_uart_ier_rbr     0   //1=��������жϺͽ��ճ�ʱ�ж�
#define bo_uart_ier_thre    1   //1=�������ж�
#define bo_uart_ier_rxl     2   //1=���������״̬�жϣ�״̬�ɴ�lsr4:1����

//�жϱ�ʶ�Ĵ���iirλƫ�ƶ���
#define bo_uart_iir_all     0   //0=������һ���жϹ���,1=���жϹ���
#define bm_uart_iir_id      0xe0//1=�����жϣ�2=�����жϣ�3=��״̬�жϣ�6=��ʱ
#define bo_uart_iir_id      1   //��ʶ������ж�

//fifo���ƼĴ���fcrλƫ�ƶ���
#define bo_uart_fcr_ffen    0   //1=ʹ��fifo����ʹ���Ĵ�������λ�ɷ��ʡ�
#define bo_uart_fcr_rffrs   1   //1=��λrx fifo����λ����Ӳ�����㱾λ
#define bo_uart_fcr_tffrs   2   //1=��λtx fifo����λ����Ӳ�����㱾λ
#define bm_uart_fcr_fflevel 0xc0//fifo����ˮƽ��0=1�ַ���1=4�ַ���2=8�ַ���3=14
#define bo_uart_fcr_fflevel 6   //�շ�fifo����ˮƽѡ��

//��״̬���ƼĴ���lcrλƫ�ƶ���
#define bm_uart_lcr_bw      0x3 //�ַ����ȣ�0~3=5~8λ
#define bo_uart_lcr_bw      0   //�ַ�����λƫ��
#define bo_uart_lcr_st      2   //0=1λֹͣλ��1=2λֹͣλ����5λ�ַ���1.5λ
#define bo_uart_lcr_pe      3   //0=��ֹ��żУ�飬1=ʹ����żУ��
#define bm_uart_lcr_p       0x30//��żУ�飬0=�棬1=ż��2=ǿ��1��3=ǿ��0
#define bo_uart_lcr_p       4   //��żУ��ѡ��
#define bo_uart_lcr_lbc     6   //��Ϸ��Ϳ��ƣ�0=�������ͣ�1=ǿ�Ʒ���0
#define bo_uart_lcr_dlab    7   //1=ѡ��baud�Ĵ���(baud�������Ĵ�����ַ��ͬ)

//��״̬�Ĵ���lsr�Ĵ���λƫ�ƶ���
#define bo_uart_lsr_rdr     0   //1=���վ���
#define bo_uart_lsr_oe      1   //1=�����������lsr�����
#define bo_uart_lsr_pe      2   //1=������ż����
#define bo_uart_lsr_fe      3   //1=����֡����
#define bo_uart_lsr_bi      4   //1=��⵽��Ϸ����ź�
#define bo_uart_lsr_thre    5   //1=����fifo��
#define bo_uart_lsr_temt    6   //1=������λ����
#define bo_uart_lsr_rsfe    7   //1=rx fifo����һ�����մ����ַ�

//����ʹ�ܼĴ���terλƫ�ƶ���
#define bo_uart_ter_txen    7   //1=�����ͣ�0=ֹͣ����

//ע��1.��ͬ��CPU��uart�Ĵ�����ַ��ͬ��
//    2.��������cpu��֧�����мĴ������ο����õ�������
struct st_nxp_uart_reg
{
  union {                   //0x00
      vu32  RBR;                //dlab=0 ����fifo��ַ��thr�ǣ�
      vu32  THR;                //dlab=0 ����fifo��ַ
      vu32  DLL;                //dlab=1 baud�������ֽ�
  }RTD;
  union {                   //0x04
      vu32  DLM;                //dlab=1 baud�������ֽ�
      vu32  IER;                //dlab=0 �ж�ʹ�ܼĴ���
  }DI;
  union {                   //offset = 0x08
      vu32  IIR;                //�ж�id�Ĵ���
      vu32  FCR;                //fifo���ƼĴ���
  }IF;
  vu32  LCR;                //0x0c���߿��ƼĴ���
  vu32  MCR;                //0x010 Modem control Register (R/W)
  vu32  LSR;                //0x014 ��״̬�Ĵ���
  vu32  MSR;                //0x018 Modem status Register (R/ )
  vu32  SCR;                //0x01C ���ٻ���
  vu32  ACR;                //0x020 Auto-baud Control Register (R/W)
  vu32  ICR;                //0x024 IrDA Control Register (R/W) */      
  vu32  FDR;                //0x028 Fractional Divider Register (R/W)
  vu32  RESERVED0;
  vu32  TER;                //0x030 ����ʹ��
  vu32  RESERVED1[6];
  vu32  RS485CTRL;          //0x04C RS-485/EIA-485 Control Register (R/W)
  vu32  ADRMATCH;           //0x050 RS-485/EIA-485 address match Register (R/W)
  vu32  RS485DLY;           //0x054 RS-485/EIA-485 direction control delay Register (R/W)
  vu32  FIFOLVL;            //0x058 FIFO Level Register (R/ )
};

ptu32_t module_init_uart0(ptu32_t para);
ptu32_t module_init_uart1(ptu32_t para);
ptu32_t module_init_uart2(ptu32_t para);
ptu32_t module_init_uart3(ptu32_t para);


#ifdef __cplusplus
}
#endif
#endif /* __CPU_PERI_UART__ */
