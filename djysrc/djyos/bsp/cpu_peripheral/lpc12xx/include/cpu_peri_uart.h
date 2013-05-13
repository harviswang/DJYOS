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

/* @brief UART_databit_type definitions
  * @{
  */
#define UART_CFG_DATABIT_5   ((u8)(0))       /* UART 5 bit data mode */
#define UART_CFG_DATABIT_6   ((u8)(1<<0))    /* UART 6 bit data mode */
#define UART_CFG_DATABIT_7   ((u8)(2<<0))    /* UART 7 bit data mode */
#define UART_CFG_DATABIT_8   ((u8)(3<<0))    /* UART 8 bit data mode */
#define PARAM_UART_DATABIT(databit) ((databit==UART_CFG_DATABIT_5) || \
                                     (databit==UART_CFG_DATABIT_6) || \
                                     (databit==UART_CFG_DATABIT_7) || \
                                     (databit==UART_CFG_DATABIT_8))
/**
  * @}
  */

/*@brief UART_stopbit_type definitions
  * @{
  */
#define UART_CFG_STOPBIT_1   ((u8)(0))      /* UART 1 Stop Bits Select */
#define UART_CFG_STOPBIT_2   ((u8)(1<<2))   /* UART Two Stop Bits Select */
#define PARAM_UART_STOPBIT(stopbit) ((stopbit==UART_CFG_STOPBIT_1) || \
                                     (stopbit==UART_CFG_STOPBIT_2))
/**
  * @}
  */

/* @brief UART_parity_type definitions
  * @{
  */
#define UART_CFG_PARITY_NONE   ((u8)(0))     /* No parity */
#define UART_CFG_PARITY_ODD    ((u8)(1<<3))  /* Odd parity */
#define UART_CFG_PARITY_EVEN   ((u8)(3<<3))  /* Even parity */
#define UART_CFG_PARITY_MARK   ((u8)(5<<3))  /* Forced "1" stick parity */
#define UART_CFG_PARITY_SPACE  ((u8)(7<<3))  /* Forced "0" stick parity */
#define PARAM_UART_PARITY(parity)   ((parity==UART_CFG_PARITY_NONE) || \
                                     (parity==UART_CFG_PARITY_ODD)  || \
                                     (parity==UART_CFG_PARITY_EVEN) || \
                                     (parity==UART_CFG_PARITY_MARK) || \
                                     (parity==UART_CFG_PARITY_SPACE))
/**
  * @}
  */

/* @brief FIFO_level_type definitions
  * @{
  */
#define UART_CFG_FIFOTRG_1   ((u8)(0))    /* UART FIFO trigger level 0: 1 char */
#define UART_CFG_FIFOTRG_4   ((u8)(1<<6)) /* UART FIFO trigger level 1: 4 char */
#define UART_CFG_FIFOTRG_8   ((u8)(2<<6)) /* UART FIFO trigger level 2: 8 char */
#define UART_CFG_FIFOTRG_14  ((u8)(3<<6)) /* UART FIFO trigger level 3: 14 char */
#define PARAM_UART_FIFO_LEVEL(fifo) ((fifo==UART_CFG_FIFOTRG_1) || \
                                     (fifo==UART_CFG_FIFOTRG_4) || \
                                     (fifo==UART_CFG_FIFOTRG_8) || \
                                     (fifo==UART_CFG_FIFOTRG_14))
/**
  * @}
  */

/*@brief FIFO_dmamode_type definitions
  * @{
  */
#define UART_CFG_DMAMODE_DISABLE  ((u8)(0))     /* UART DMA mode disable */
#define UART_CFG_DMAMODE_ENABLE   ((u8)(1<<3))  /* UART DMA mode enable  */
#define PARAM_UART_FIFO_DMA(fifo)   ((fifo==UART_CFG_DMAMODE_DISABLE) ||\
                                     (fifo==UART_CFG_DMAMODE_ENABLE))
/**
  * @}
  */

/*@brief TXD_break_forced_type definitions
  * @{
  */
#define UART_CFG_TXDBREAK_DISABLE  ((u8)(0))      /* TXD Break disable */
#define UART_CFG_TXDBREAK_ENABLE   ((u8)(1<<6))   /* TXD Break enable */
#define PARAM_UART_TXDBREAK(type)   ((type==UART_CFG_TXDBREAK_DISABLE) ||\
                                     (type==UART_CFG_TXDBREAK_ENABLE))
/**
  * @}
  */

/*@brief UART_interrupt_type definitions
  * @{
  */
#define UART_INTCFG_RBR  ((u16)(1<<0)) /* RBR Interrupt enable*/
#define UART_INTCFG_THRE ((u16)(1<<1)) /* THR Interrupt enable*/
#define UART_INTCFG_RLS  ((u16)(1<<2)) /* RX line status interrupt enable*/
#define UART_MSINT_EN    ((u32)(1<<3)) /*!< Modem status interrupt enable */
#define UART_CTSINT_EN   ((u32)(1<<7)) /*!< CTS1 signal transition interrupt enable */
#define UART_INTCFG_ABEO ((u16)(1<<8)) /* the end of auto-baud interrupt */
#define UART_INTCFG_ABTO ((u16)(1<<9)) /* the auto-baud time-out interrupt */
#define PARAM_UART_INTCFG(intcfg)   ((intcfg == UART_INTCFG_RBR)  ||\
                                     (intcfg == UART_INTCFG_THRE) ||\
                                     (intcfg == UART_INTCFG_RLS)  ||\
                                     (intcfg == UART_MSINT_EN)    ||\
                                     (intcfg == UART_CTSINT_EN)   ||\
                                     (intcfg == UART_INTCFG_ABEO) ||\
                                     (intcfg == UART_INTCFG_ABTO))
/**
  * @}
  */

/*@brief UART_int_status_type definitions
  * @{
  */

#define UART_INTSTAT_PEND              ((u16)(1<<0))  /* Active low */
#define UART_INTSTAT_RX_LINE_STAT      ((u16)(3<<1))  /* Receive line status*/
#define UART_INTSTAT_RX_DATA_AVAILABLE ((u16)(2<<1))  /* Receive data available*/
#define UART_INTSTAT_RX_TIMEOUT        ((u16)(6<<1))  /* Character time-out indicator*/
#define UART_INTSTAT_TX_READY          ((u16)(1<<1))  /* THRE interrupt*/
#define UART_INTSTAT_MODEM             ((u16)(0<<1))  /* Modem interrupt*/
#define UART_INTSTAT_AUTO_END          ((u16)(1<<8))  /* End of auto-baud interrupt */
#define UART_INTSTAT_AUTO_TIMOUT       ((u16)(1<<9))  /* Autobaud time-out interrupt */

/**
  * @}
  */

/*@brief UART_line_status_type definitions
  * @{
  */
#define UART_LS_RX_DATA_READY ((u8)(1<<0)) /*Receive data ready*/
#define UART_LS_RX_FIFO_FULL  ((u8)(1<<1)) /*Overrun error: RX fifo overflow*/
#define UART_LS_PARITY_ERROR  ((u8)(1<<2)) /*Parity error*/
#define UART_LS_FRAMING_ERROR ((u8)(1<<3)) /*Framing error*/
#define UART_LS_BREAK_INT     ((u8)(1<<4)) /*Break interrupt*/
#define UART_LS_TX_EMPTY      ((u8)(1<<5)) /*Transmit holding register THR empty*/
#define UART_LS_TX_EMPTY_ALL  ((u8)(1<<6)) /*Transmitter THR and TSR all empty*/
#define UART_LS_RX_FIFO_ERROR ((u8)(1<<7)) /*Error in RX FIFO or others*/

/**
  * @}
  */

/*@brief UART_auto_baudrate_mode_type definitions
  * @{
  */
#define UART_CFG_AUTOBAUD_MODE0  ((u8)(0))     /* UART Auto baudrate Mode 0 */
#define UART_CFG_AUTOBAUD_MODE1  ((u8)(1<<1)) /* UART Auto baudrate Mode 1 */

#define PARAM_UART_AB_MODE(ABmode) ((ABmode==UART_CFG_AUTOBAUD_MODE0) ||\
                                    (ABmode==UART_CFG_AUTOBAUD_MODE1))

/**
  * @}
  */

/*@brief UART_auto_baudrate_restart_type definitions
  * @{
  */
#define UART_CFG_ABRESTART_DISABLE  ((u8)(0))     /* Auto baudrate disable */
#define UART_CFG_ABRESTART_ENABLE   ((u8)(1<<2)) /* Auto baudrate enable */

#define PARAM_UART_AB_RESTART(type) ((type==UART_CFG_ABRESTART_DISABLE) ||\
                                     (type==UART_CFG_ABRESTART_ENABLE))


/**
  * @}
  */

/*@brief UART_modem_status_type definitions
  * @{
  */
#define UART_MODEM_STAT_DELTA_CTS  ((u8)(1<<0))   /* State change of CTS */
#define UART_MODEM_STAT_DELTA_DSR  ((u8)(1<<1))   /* State change of DSR */
#define UART_MODEM_STAT_LO2HI_RI   ((u8)(1<<2))   /* Low to high transition of RI */
#define UART_MODEM_STAT_DELTA_DCD  ((u8)(1<<3))   /* State change of input DCD */
#define UART_MODEM_STAT_CTS        ((u8)(1<<4))   /* Clear To Send State */
#define UART_MODEM_STAT_DSR        ((u8)(1<<5))   /* Data Set Ready State */
#define UART_MODEM_STAT_RI         ((u8)(1<<6))   /* Ring Indicator State */
#define UART_MODEM_STAT_DCD        ((u8)(1<<7))   /* Data Carrier Detect State */

/**
  * @}
  */

/*@brief UART_modem_DTR_pin_type definitions
  * @{
  */
#define UART_MODEM_PIN_DTR_DISABLE ((u8)(0))
#define UART_MODEM_PIN_DTR_ENABLE  ((u8)(1<<0)) /* Modem output pin DTR */
#define PARAM_MODEM_PIN_DTR(x) ((x==UART_MODEM_PIN_DTR_DISABLE) || \
                                (x==UART_MODEM_PIN_DTR_ENABLE))

/**
  * @}
  */

/*@brief UART_modem_RTS_pin_type definitions
  * @{
  */

#define UART_MODEM_PIN_RTS_DISABLE ((u8)(0))
#define UART_MODEM_PIN_RTS_ENABLE  ((u8)(1<<1)) /* Modem output pin RTS */
#define PARAM_MODEM_PIN_RTS(x) ((x==UART_MODEM_PIN_RTS_DISABLE) || \
                                (x==UART_MODEM_PIN_RTS_ENABLE))

/**
  * @}
  */

/*@brief UART_modem_loopback_type definitions
  * @{
  */

#define UART_MODEM_MODE_LOOPBACK_DISABLE ((u8)(0))
#define UART_MODEM_MODE_LOOPBACK_ENABLE  ((u8)(1<<4))  /* Loop back mode select */
#define PARAM_MODEM_MODE_LOOPBACK(x) ((x==UART_MODEM_MODE_LOOPBACK_DISABLE) || \
                                      (x==UART_MODEM_MODE_LOOPBACK_ENABLE))

/**
  * @}
  */

/*@brief UART_modem_autoRTS_type definitions
  * @{
  */
#define UART_MODEM_MODE_AUTO_RTS_DISABLE ((u8)(0))
#define UART_MODEM_MODE_AUTO_RTS_ENABLE  ((u8)(1<<6)) /* Enable AutoRTS flow-control*/
#define PARAM_MODEM_MODE_AUTO_RTS(x) ((x==UART_MODEM_MODE_AUTO_RTS_DISABLE) || \
                                      (x==UART_MODEM_MODE_AUTO_RTS_ENABLE))

/**
  * @}
  */

/*@brief UART_modem_autoCTS_type definitions
  * @{
  */
#define UART_MODEM_MODE_AUTO_CTS_DISABLE ((u8)(0))
#define UART_MODEM_MODE_AUTO_CTS_ENABLE  ((u8)(1<<7)) /* Enable AutoCTS flow-control*/
#define PARAM_MODEM_MODE_AUTO_CTS(x) ((x==UART_MODEM_MODE_AUTO_CTS_DISABLE) || \
                                      (x==UART_MODEM_MODE_AUTO_CTS_ENABLE))

/**
  * @}
  */

/*@brief UART_485_nmm_type definitions
  * @{
  */
#define UART_RS485_NORMAL_MULTIDROP_DISABLE ((u8)(0))
#define UART_RS485_NORMAL_MULTIDROP_ENABLE  ((u8)(1<<0))  /* Normal Multi-drop Mode */
#define PARAM_UART_RS485_NORMAL_MULTIDROP(x) ((x==UART_RS485_NORMAL_MULTIDROP_DISABLE) || \
                                             (x==UART_RS485_NORMAL_MULTIDROP_ENABLE))

/**
  * @}
  */

/*@brief UART_485_receiver_type definitions
  * @{
  */
#define UART_RS485_RECEIVER_ENABLE   ((u8)(0))
#define UART_RS485_RECEIVER_DISABLE  ((u8)(1<<1))  /* The receiver is disabled */
#define PARAM_UART_RS485_RECEIVER(x) ((x==UART_RS485_RECEIVER_ENABLE) || \
                                 (x==UART_RS485_RECEIVER_DISABLE))

/**
  * @}
  */

/*@brief UART_485_add_type definitions
  * @{
  */

#define UART_RS485_AUTO_ADDRRESS_DISABLE ((u8)(0))
#define UART_RS485_AUTO_ADDRRESS_ENABLE  ((u8)(1<<2))  /* Auto Address Detect */
#define PARAM_UART_RS485_AUTO_ADDRRESS(x) ((x==UART_RS485_AUTO_ADDRRESS_DISABLE) || \
                                      (x==UART_RS485_AUTO_ADDRRESS_ENABLE))

/**
  * @}
  */

/*@brief UART_485_dirpin_ctrl_type definitions
  * @{
  */
#define UART_RS485_DIRPIN_RTS  ((u8)(0))
#define UART_RS485_DIRPIN_DTR  ((u8)(1<<3))  /* Direction control */
#define PARAM_UART_RS485_DIRPIN_SELECT(x) ((x==UART_RS485_DIRPIN_RTS) || \
                                      (x==UART_RS485_DIRPIN_DTR))

/**
  * @}
  */

/*@brief UART_485_autodir_type definitions
  * @{
  */
#define UART_RS485_AUTO_DIRECTION_DISABLE ((u8)(0))
#define UART_RS485_AUTO_DIRECTION_ENABLE  ((u8)(1<<4)) /* Auto Direction Control */
#define PARAM_UART_RS485_AUTO_DIRECTION(x) ((x==UART_RS485_AUTO_DIRECTION_DISABLE) || \
                                           (x==UART_RS485_AUTO_DIRECTION_ENABLE))

/**
  * @}
  */

/*@brief UART_485_dirpin_level_type definitions
  * @{
  */
#define UART_RS485_DIRPIN_LOW   ((u8)(0))
#define UART_RS485_DIRPIN_HIGH  ((u8)(1<<5))  /* Reverses the polarity */
#define PARAM_UART_RS485_DIRPIN_LEVEL(x) ((x==UART_RS485_DIRPIN_LOW) || \
                                          (x==UART_RS485_DIRPIN_HIGH))

/**
  * @}
  */

/*@brief UART_irda_mode_type definitions
  * @{
  */
#define UART_IRDA_DISABLE       ((u8)(0))    /* IrDA mode disable */
#define UART_IRDA_ENABLE        ((u8)(1<<0)) /* IrDA mode enable */
#define PARAM_UART_IRDA_MODE(x) ((x==UART_IRDA_DISABLE) || \
                                 (x==UART_IRDA_ENABLE))

/**
  * @}
  */

/*@brief UART_irda_input_invert_type definitions
  * @{
  */
#define UART_IRDA_INPUTINVERT_NOT   ((u8)(0))    /* IrDA serial not input inverted */
#define UART_IRDA_INPUTINVERTED     ((u8)(1<<1)) /* IrDA serial input inverted */
#define PARAM_UART_IRDA_INPUT(x) ((x==UART_IRDA_INPUTINVERT_NOT) || \
                                  (x==UART_IRDA_INPUTINVERTED))

/**
  * @}
  */

/*@brief UART_irda_fixed_pulse_type definitions
  * @{
  */
#define UART_IRDA_PULSEDIV_NORMAL     ((u8)(0))
#define UART_IRDA_PULSEDIV_2          ((u8)(0x1<<2))
#define UART_IRDA_PULSEDIV_4          ((u8)(0x3<<2))
#define UART_IRDA_PULSEDIV_8          ((u8)(0x5<<2))
#define UART_IRDA_PULSEDIV_16         ((u8)(0x7<<2))
#define UART_IRDA_PULSEDIV_32         ((u8)(0x9<<2))
#define UART_IRDA_PULSEDIV_64         ((u8)(0xB<<2))
#define UART_IRDA_PULSEDIV_128        ((u8)(0xD<<2))
#define UART_IRDA_PULSEDIV_256        ((u8)(0xF<<2))
#define PARAM_UART_IRDA_PULSEDIV(x) ((x==UART_IRDA_PULSEDIV_NORMAL) || \
                                     (x==UART_IRDA_PULSEDIV_2)      || \
                                     (x==UART_IRDA_PULSEDIV_4)      || \
                                     (x==UART_IRDA_PULSEDIV_8)      || \
                                     (x==UART_IRDA_PULSEDIV_16)     || \
                                     (x==UART_IRDA_PULSEDIV_64)     || \
                                     (x==UART_IRDA_PULSEDIV_128)    || \
                                     (x==UART_IRDA_PULSEDIV_256))
/**
  * @}
  */

/*@brief UART_process_time_delay definitions
  * @{
  */

#define UART_BLOKING_TIMEOUT          (0xFFFFFFFFUL)
#define UART_NO_TIMEOUT               (0x1UL)
#define PARAM_UART_PROCESS_DELAY(x) ((x==UART_BLOKING_TIMEOUT) || \
                                     (x==UART_NO_TIMEOUT))
    /* Private Types -------------------------------------------------------------- */
    /** @defgroup UART_Private_Types
     * @{
     */
    /* Macro to determine if it is valid UART port number */
#define PARAM_UARTx(x)     ((((u32 *)x)==((u32 *)LPC_UART0)) || \
                                (((u32 *)x)==((u32 *)LPC_UART1)))
#define PARAM_UART_MODEM(x) (((u32 *)x)==((u32 *)LPC_UART0))
#define PARAM_UART_485(x)   (((u32 *)x)==((u32 *)LPC_UART0))
#define PARAM_UART_IRDA(x)  (((u32 *)x)==((u32 *)LPC_UART1))

    /* Macro to define bit mask */
#define UART_RBR_MASKBIT    ((u8)0xFF) /* Received Buffer mask bit  */
#define UART_THR_MASKBIT    ((u8)0xFF) /* Transmit Holding mask bit  */
#define UART_IER_BITMASK    ((u32)(0x307)) /* IER normal bit mask */
#define UART_IER_BITMASK2   ((u32)(0x38F)) /* IER include modem bit mask */
#define UART_IIR_BITMASK    ((u32)(0x3CF)) /* IIR bit mask */
#define UART_FCR_BITMASK    ((u8)(0xCF))   /* FCR bit mask */
#define UART_LCR_BITMASK    ((u8)(0xFF))   /* LCR bit mask */
#define UART_MCR_BITMASK    ((u8)(0xF3))   /* MCR bit mask */
#define UART_LSR_BITMASK    ((u8)(0xFF))   /* LSR bit mask */
#define UART_MSR_BITMASK    ((u8)(0xFF))   /* MSR register bit mask */
#define UART_SCR_BIMASK     ((u8)(0xFF))   /* UART Scratch Pad bit mask */
#define UART_ACR_BITMASK    ((u32)(0x307)) /* ACR bit mask */
#define UART_ICR_BITMASK    ((u32)(0x3F))  /* IRDA bit mask */
#define UART_FDR_BITMASK    ((u32)(0xFF))  /* FDR bit mask */
#define UART_TER_BITMASK    ((u8)(0x80))   /* TER bit mask */
#define UART_RS485CTRL_BITMASK     ((u32)(0x3F))  /* 485 CTRL bit mask */
#define UART_RS485ADRMATCH_BITMASK ((u8)(0xFF))   /* 485 ADR bit mask */
#define UART_RS485DLY_BITMASK      ((u8)(0xFF))   /* 485 DLY bit mask */
#define UART_FIFOLVL_BITMASK       ((u32)(0x0F0F))/* 485 FIFOLVL bit mask */

    /* Macro to define control bit */
#define UART_LCR_DLAB_EN        ((u8)(1<<7))   /* DivisorLatchesAccess enable*/
#define UART_FCR_FIFO_EN        ((u8)(1<<0))   /* FIFO enable */
#define UART_FCR_RX_RS          ((u8)(1<<1))   /* FIFO RX reset */
#define UART_FCR_TX_RS          ((u8)(1<<2))   /* FIFO TX reset */
#define UART_TER_TXEN           ((u8)(1<<7))   /* Transmit enable bit */
#define UART_ACR_START          ((u32)(1<<0))  /* Auto baudrate Start */
#define UART_ACR_ABEOINT_CLR    ((u32)(1<<8))  /* Auto baudrate end INT clear */
#define UART_ACR_ABTOINT_CLR    ((u32)(1<<9))  /* Auto baudrate Ttime-out INT clear */

    /* Macro to define caculate */
#define UART_LOAD_DLL(div)        ((div) & 0xFF)
#define UART_LOAD_DLM(div)        (((div) >> 8) & 0xFF)
#define UART_FDR_DIVADDVAL(n)     ((u32)(n&0x0F))      /* Pre-scaler divisor */
#define UART_FDR_MULVAL(n)        ((u32)((n<<4)&0xF0)) /* Multiplier value */
#define UART_FIFOLVL_RXFIFOLVL(n) ((u32)(n&0x0F))      /*receiver FIFO */
#define UART_FIFOLVL_TXFIFOLVL(n) ((u32)((n>>8)&0x0F)) /*transmitter FIFO */

    /* Macro to define parameter */
#define UART_ACCEPTED_BAUDRATE_ERROR  (5) /* Acceptable baudrate error, 1/2(n) percent */
#define UART_TX_FIFO_SIZE             (16)
#define UART_DEVICE_NUMBER            (2)

ptu32_t module_init_uart0(ptu32_t para);
ptu32_t module_init_uart1(ptu32_t para);

#ifdef __cplusplus
}
#endif
#endif /* __CPU_PERI_UART__ */

