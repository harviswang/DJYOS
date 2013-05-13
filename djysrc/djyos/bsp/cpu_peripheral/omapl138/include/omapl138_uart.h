/*========================================================*
 * 文件模块说明: omapl138的uart驱动，移植自都江堰操作系统作者拥有版权
 * 的S3C2440 uart驱动文件，针对omapl138做了一些修改
 * 文件版本: v1.00                                               *
 * 开发人员: lst                              *
 * 创建时间: 2011.06.01                                          *
 * Copyright(c) 2011-2011  深圳鹏瑞软件有限公司             *
 *========================================================*
 * 程序修改记录(最新的放在最前面):							     *
 *  <版本号> <修改日期>, <修改人员>: <修改功能概述>				 *
 *========================================================*
* 例如：
*  V1.02 2004-07-14, 张三: 接收函数增加对报文长度有效性校核
*
*/
//以下是原作者版权声明
//----------------------------------------------------
//Copyright (C), 2004-2009,  都江堰操作系统开发团队
//版权所有 (C), 2004-2009,   都江堰操作系统开发团队
//所属模块: uart驱动程序
//作者:  罗侍田.
//版本：V1.0.0
//文件描述: 
//其他说明:
//修订历史:
//1. 日期:2009-03-10
//   作者:  罗侍田.
//   新版本号：1.0.0
//   修改说明: 移植字44b0的1.0.1版
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

//串口状态控制结构
struct uart_UCB
{
    struct ring_buf send_ring_buf;           //环形发送缓冲区.
    struct ring_buf recv_ring_buf;           //环形接收缓冲区.

    struct semaphore_LCB *send_buf_semp;     //发送缓冲区锁
    struct semaphore_LCB *recv_buf_semp;     //接收缓冲区锁

    u16 evtt_right_write;      //设备右手被写后弹出的事件类型
    u16 evtt_left_write;       //设备左手被写后弹出的事件类型
    u16 recv_trigger_level;    //串口接收触发水平,即收到多少数据时弹出事件
    u32 timeout;               //未到触发水平,持续设定时间内没有收到数据,
                                    //也弹出事件.
    u16 send_trigger_level;    //发送同步触发水平，当发送环形缓冲区满，将
                                    //用send_buf_semp阻塞当前线程，直到发送缓冲
                                    //区数据量降至send_trigger_level时释放之。
    tag_UartReg volatile *my_reg;
    u32 baud;
    ufast_t status;
    ufast_t serial_no;
};

//通信状态变量各位定义
#define cn_ser_tx_busy         (1<<0)
#define cn_ser_rxbuf_empty     (1<<1)
#define cn_ser_receive_over    (1<<2)

//串口错误类型
enum uart_error_no
{
    enum_recv_fifo_over,         //接收硬件fifo溢出
    enum_recv_buf_over,          //接收缓冲区溢出
};

//串口设备控制命令常数，用于uart_ctrl函数。
#define cn_uart_connect_recv_evtt     0  //设备被从右手写入的数据超过软件触发水
                                         //平时弹出本类型事件
#define cn_uart_disconnect_recv_evtt  1  //断开右手事件
#define cn_uart_connect_send_evtt     2  //设备被从左手写入数据时弹出本类型事件
#define cn_uart_disconnect_send_evtt  3  //断开左手事件
#define cn_uart_start                 4  //启动串口
#define cn_uart_stop                  5  //停止串口
#define cn_uart_set_baud              6  //设置uartBaud.
#define cn_uart_send_data             7  //发送数据
#define cn_uart_recv_data             8  //接收数据
#define cn_uart_completed_send        9  //完成发送工作
#define cn_uart_rx_pause              10 //暂停接收数据到缓冲区
#define cn_uart_rx_resume             11 //重新开始接收数据到缓冲区
#define cn_uart_overtime              12 //设置接收超时时间，毫秒数
#define cn_uart_recv_soft_level       13 //设置接收软件缓冲区触发水平
#define cn_uart_send_soft_level       14 //设置接收软件缓冲区触发水平
#define cn_uart_recv_hard_level       15 //设置接收fifo触发水平
#define cn_uart_send_hard_level       16 //设置发送fifo触发水平

ptu32_t module_init_uart0(ptu32_t para);
ptu32_t module_init_uart1(ptu32_t para);
ptu32_t module_init_uart2(ptu32_t para);
ptu32_t module_init_uart3(ptu32_t para);

#ifdef __cplusplus
}
#endif

#endif //__CPU_PERI_UART_H__


