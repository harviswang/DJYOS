//----------------------------------------------------
//Copyright (C), 2004-2009,  lst.
//版权所有 (C), 2004-2009,   lst.
//所属模块: uart驱动程序
//作者：lst
//版本：V1.0.1
//文件描述: 驱动44b0的uart
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2009-03-10
//   作者: lst
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#ifndef _UART_H_
#define _UART_H_

#ifdef __cplusplus
extern "C" {
#endif

#define cn_dma_unused   0
#define cn_dma_used     1
//串口状态控制结构
struct uart_UCB
{
    struct ring_buf send_ring_buf;           //环形发送缓冲区.
    struct ring_buf recv_ring_buf;           //环形接收缓冲区.

    struct semaphore_LCB *send_buf_semp;     //发送缓冲区锁
    struct semaphore_LCB *recv_buf_semp;     //接收缓冲区锁

    uint16_t recv_target_evtt;      //设备右手被写后弹出的事件类型
    uint16_t recv_trigger_level;    //串口接收触发水平,即收到多少数据时弹出事件
    uint32_t timeout;               //接收缓冲区中有数据，但未到触发水平,持续设
                                    //定时间内没有收到新数据,也弹出事件.单位=us
    uint16_t send_trigger_level;    //发送同步触发水平，当发送环形缓冲区满，将
                                    //用send_buf_semp阻塞当前线程，直到发送缓冲
                                    //区数据量降至send_trigger_level时释放之。
    uint32_t baud;
    ufast_t dma_used;               //0=不使用dma，1=用dma通信
    bool_t sendding;                //若为true，则设备从左手写入后，无需重新启动
                                    //发送，否则，需要启动发送
};
//串口错误类型
enum uart_error_no
{
    enum_recv_hard_over,         //接收硬件溢出
    enum_recv_buf_over,          //接收缓冲区溢出
};

//同步/异步串口位域，bo_为位偏移，1位的用位带地址，bb_前缀，多位用掩码，bm_前缀
//uart1 sr寄存器位定义
#define bb_uart1_sr_base   (0x42000000 + 0x13800*32)           //位带基址
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

//uart2 sr寄存器位定义
#define bb_uart2_sr_base   (0x42000000 + 0x4400*32)           //位带基址
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

//uart3 sr寄存器位定义
#define bb_uart3_sr_base   (0x42000000 + 0x4800*32)           //位带基址
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

//uart4 sr寄存器位定义
#define bb_uart4_sr_base    (0x42000000 + 0x4c00*32)           //位带基址
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

//uart5 sr寄存器位定义
#define bb_uart5_sr_base    (0x42000000 + 0x5000*32)           //位带基址
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

//uart1 cr1寄存器位定义
#define bb_uart1_cr1_base      (0x42000000 + 0x1380c*32)           //位带基址
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

//uart2 cr1寄存器位定义
#define bb_uart2_cr1_base      (0x42000000 + 0x440c*32)           //位带基址
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

//uart3 cr1寄存器位定义
#define bb_uart3_cr1_base      (0x42000000 + 0x480c*32)           //位带基址
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

//uart4 cr1寄存器位定义
#define bb_uart4_cr1_base       (0x42000000 + 0x4c0c*32)           //位带基址
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

//uart5 cr1寄存器位定义
#define bb_uart5_cr1_base       (0x42000000 + 0x500c*32)           //位带基址
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
//uasrt1：0x40013800
//uasrt2：0x40004400
//uasrt3：0x40004800
//uasrt4：0x40004c00
//uasrt5：0x40005000
struct st_usart_reg
{
  vu16 SR;      //状态寄存器，b31：10，保留，硬件强制为0
                //rc，b9：CTS，cts标志，0=cts无变化，1=cts有变化
                //rc，b8：LBD，LIN break检测标志，0=没有检测到LIN break，1=反之
                //r，b7，TXE，发送数据寄存器空，0 = 非空，1=空
                //rc，TC，b6，发送完成标志，0=未完成，1=完成
                //r，RXNE，b5，接收ready，0=非ready，1=ready
                //r，IDLE，b4，总线空闲标志，0=非空闲，1=空闲
                //r，ORE，b3，接收溢出标志，0=无溢出，1=溢出
                //r，NE，b2，噪声错误标志，0=有噪声，1=无噪声，读SR再读DR可清零
                //           本标志无中断，因为它与RXNE标志一起出现。
                //r，FE，b1，帧错误，与NE一样。
                //r，PE，b0，校验错误，0=无错，1=有错，读SR再读DR可清零
  u16 RESERVED0;
  vu16 DR;      //收发数据寄存器，低9位有效，一个地址的两个寄存器。
  u16 RESERVED1;
  vu16 BRR;     //Baud寄存器，16位定点数，低4位是小数，Baud = pclk/(16*BRR)
  u16 RESERVED2;
  vu16 CR1;     //bit31~14,保留，硬件强制为0
                //rw，UE，b13，使能位，0=禁止uart，1=允许
                //rw，M，b12，字长，0=8数据位，n个停止位，1=9数据位，1停止位
                //rw，WAKE，b11，多机通信唤醒方法，0=空闲总线，1=地址标记
                //rw，PCE，b10，校验使能，0=禁止，1=使能
                //rw，PS，b9，校验选择，0=偶校验，1=奇校验
                //rw，PEIE，b8，PE中断使能，控制RS寄存器中的PE位是否产生中断
                //                0=禁止，1=允许
                //rw，TXEIE，b7，同上，监视SR的TXE位
                //rw，TCIE，b6，同上，监视SR的TC位
                //rw，RXNEIE，b5，同上，监视SR的RXNE位
                //rw，IDLEIE，b4，同上，监视SR的IDLE位
                //rw，TE，b3，发送使能，0=禁止，1=允许
                //rw，RE，b2，接收使能，0=禁止，1=允许
                //rw，RWU，b1，进入静默模式，0=正常模式，1=静默模式，
                //    用于多机通信，必须先收1字节后才能进入静默模式
                //rw，SBK，b0，0=无动作，1=将要发送断开帧，用于LIN模式
  u16 RESERVED3;
  vu16 CR2;     //bit31~15,保留，硬件强制为0
                //rw，LINEN，b14，LIN模式使能，0=禁止，1=使能
                //rw，STOP，b13~12，stop位数，CR1中的M位为0才有效。
                //          00=1bit，01=0.5bit，10=2bit，11=1.5bit
                //rw，CLKEN，b11，时钟使能，0=SCLK引脚禁止，1=允许，用于同步通信
                //rw，CPOL，b10，SCLK上的时钟极性，0=总线空闲时低，1=高
                //rw，CPHA，b9，SCLK时钟相位，0=SCLK第一个边沿采样，1=第二边沿
                //rw，LBCL，b8，同步模式下，是否发送最后一个字节的MSB时钟脉冲
                //          0=不输出，1=输出
                //rw，b7，保留，强制0
                //rw，LBDIE，b6，LIN break检测中断使能，0=禁止，1=使能
                //rw，LBDL，b5，LIN break检测长度，0=10位，1=11位
                //rw，b4，保留，强制0
                //rw，ADD[3:0]，b3~0，多机通信模式下该USART节点的地址
  u16 RESERVED4;
  vu16 CR3;     //bit31~11,保留，硬件强制为0
                //rw，CTSIE，b10，CTS中断使能，0=禁止，1=SR中CTS置位就中断
                //rw，CTSE，b9，CTS硬件流控使能，0=禁止，1=使能
                //rw，RTSE，b8，RTS硬件流控使能，0=禁止，1=使能
                //rw，DMAT，b7，DMA使能发送，0=禁止，1=使能
                //rw，DMAR，b6，DMA使能接收，0=禁止，1=使能
                //rw，SCEN，b5，智能卡模式使能，0=禁止，1=使能
                //rw，NACK，b4，智能卡NACK使能，0=校验错误是不发NACK，1=反之
                //rw，HDSEL，b3，0=选择半双工选择，1=不选
                //rw，IRLP，b2，0=正常模式，1=红外低功耗模式
                //rw，IREN，b1，红外模式使能，0=禁止，1=使能
                //rw，EIE，b0，错误中断使能，0=禁止，1=使能
                //rw，
                //rw，
  u16 RESERVED5;
  vu16 GTPR;    //保护时间和预分频寄存器，用于智能卡模式
  u16 RESERVED6;
} ;

#define uart1_buf_len  64
#define uart2_buf_len  64
#define uart3_buf_len  64
#define uart4_buf_len  64
#define uart5_buf_len  64

//串口设备控制命令常数，用于uart_ctrl函数。
#define cn_uart_connect_recv_evtt       0   //设备被从右手写入的数据超过软件触发
                                            //水平时弹出本类型事件
#define cn_uart_disconnect_recv_evtt    1   //断开右手事件
#define cn_uart_start                   2   //启动串口
#define cn_uart_stop                    3   //停止串口
#define cn_uart_set_baud                4   //设置uartBaud.
#define cn_uart_recv_soft_level         5   //设置接收软件缓冲区触发水平
#define cn_uart_send_soft_level         6   //设置发送软件缓冲区触发水平
#define cn_uart_overtime                7   //设置接收超时时间，毫秒数
#define cn_uart_half_duplex_send        8   //发送数据
#define cn_uart_half_duplex_recv        9   //接收数据
#define cn_uart_setto_all_duplex        10  //设置为全双工方式
#define cn_uart_en_rts                  11  //使能rts流控
#define cn_uart_dis_rts                 12  //禁止rts流控
#define cn_uart_en_cts                  13  //使能cts流控
#define cn_uart_dis_cts                 14  //禁止cts流控
#define cn_uart_dma_used                15  //设置使用dma接收，uart5不支持
#define cn_uart_dma_unused              16  //设置不使用dma接收

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

