//----------------------------------------------------
//Copyright (C), 2004-2009,  lst.
//��Ȩ���� (C), 2004-2009,   lst.
//����ģ��: uart��������
//���ߣ�lst
//�汾��V1.0.0
//�ļ�����: ����44b0��uart
//����˵��:
//�޶���ʷ:
//1. ����:2009-03-10
//   ����:lst
//   �°汾�ţ�1.0.0
//   �޸�˵��: ��ֲ��44b0��1.0.1��
//------------------------------------------------------
#include <string.h>
#include "stdint.h"
#include "cpu_peri.h"
#include "uart_tiny.h"

#if(CN_CFG_UART0_USED == 1)
static struct uart_UCB tg_uart0_cb;
uint8_t uart0_drv_send_buf[uart0_buf_len];
uint8_t uart0_drv_recv_buf[uart0_buf_len];
#endif

#if(CN_CFG_UART1_USED == 1)
static struct uart_UCB tg_uart1_cb;
uint8_t uart1_drv_send_buf[uart1_buf_len];
uint8_t uart1_drv_recv_buf[uart1_buf_len];
#endif


#if(CN_CFG_UART2_USED == 1)
static struct uart_UCB tg_uart1_cb;
uint8_t uart1_drv_send_buf[uart2_buf_len];
uint8_t uart1_drv_recv_buf[uart2_buf_len];
#endif

#if(CN_CFG_UART3_USED == 1)
static struct uart_UCB tg_uart1_cb;
uint8_t uart1_drv_send_buf[uart3_buf_len];
uint8_t uart1_drv_recv_buf[uart3_buf_len];
#endif


void __uart_pause_send_int(volatile struct st_nxp_uart_reg * const reg)
{
    reg->DI.IER &=  ~(u32)(1<<bo_uart_ier_thre);          //��ֹ�����жϣ�
}
void __uart_resume_send_int(volatile struct st_nxp_uart_reg * const reg)
{
    reg->DI.IER |=  (u32)(1<<bo_uart_ier_thre);           //ʹ�ܷ����жϣ�
}

bool_t uart_set_baud(u32 uart_no,u32 baud)
{
    struct st_nxp_uart_reg *reg;
    u32 temp,temp1;
    u32 mulval,divval;
    u32 err;
    bool_t result = false;
    switch(uart_no)
    {
        case 0 :
            reg = pg_uart0_reg;
            break;
        case 1 :
            reg = pg_uart1_reg;
            break;
        case 2 :
            reg = pg_uart2_reg;
            break;
        case 3 :
            reg = pg_uart3_reg;
            break;
        default : break;
    }
    temp = CN_CFG_PCLK/16/baud;
    temp1 = CN_CFG_PCLK/16%baud;
    if(temp1 > (baud>>1))   //������������ֱȡ�������������ڳ���1�룬���λ��
    {
        err = baud - temp1;
        temp++;
    }else
    {
        err = temp1;
    }
    //������ < CN_CFG_PCLK/16>>6 = 1/64��˵������С������������Ѿ���С��
    //�ڶ���������17xxϵ��uart��Ҫ�󣬷���ϵ������2������С��������
    if((err <= (CN_CFG_PCLK/16>>6)) || (temp <= 2))
    {
        reg->LCR |= 1<<bo_uart_lcr_dlab;
        reg->RTD.DLL= temp;
        reg->DI.DLM= temp>>8;
        reg->FDR= 0;
        reg->LCR &= ~(1<<bo_uart_lcr_dlab);
        if(err > (baud>>6))
            return false;
        else
            return true;
    }

    //��С����Ƶֵ��ÿһ������ֵ������ֱ�����С��1/64
    for(divval = 1; divval < 15; divval++)
    {
        for(mulval = divval+1; mulval < 16; mulval++)
        {
            temp1 = CN_CFG_PCLK * mulval / 16 / (mulval + divval) % baud;
            if(temp1 > (baud >> 1))
            {
                err = baud - temp1;
            }else
            {
                err = temp1;
            }
            if(err <= (CN_CFG_PCLK/16>>6))
            {
                result = true;
                break;
            }
        }
        if(result == true)
            break;
    }
    if(result == true)
    {
        temp = (CN_CFG_PCLK * mulval / 16 / (mulval + divval) + baud -1) / baud;
        reg->LCR |= 1<<bo_uart_lcr_dlab;
        reg->RTD.DLL = temp;
        reg->DI.DLM = temp>>8;
        reg->FDR = divval + (mulval <<4);
        reg->LCR &= ~(1<<bo_uart_lcr_dlab);
        return true;
    }else
        return false;
}

#if(CN_CFG_UART0_USED == 1)

u32  Uart0_Int(ufast_t uart_int_line);

//----��ʼ��uart0ģ��----------------------------------------------------------
//���ܣ���ʼ��uart0ģ�飬��ʼ��״̬Ϊ��ʹ��dma���͵ķ�ʽ��
//������para���ڱ���������Ч
//���أ�true = �ɹ���ʼ����false = ��ʼ��ʧ��
//-----------------------------------------------------------------------------
ptu32_t module_init_uart0(ptu32_t para)
{
    //�������������ź�����ʹ��������������Ϊ0ʱ����д���̣߳���ȡ�߳�ʹ��������
    //���ݽ���trigger_level����ʱ�ͷ��ź�����ʹд���߳̽������
    tg_uart0_cb.send_buf_semp = Lock_SempCreate(1,0,"uart0 tx buf");
    if(tg_uart0_cb.send_buf_semp == NULL)
        goto exit_from_left_buf_semp;
    tg_uart0_cb.recv_buf_semp = Lock_SempCreate(1,0,"uart0 rx buf");
    if(tg_uart0_cb.recv_buf_semp == NULL)
        goto exit_from_right_buf_semp;
    //��ʼ��IO�˿�λuart����
    pg_sysctrl_reg->pconp |= 1<<4;      //ʹ��uart0ģ��
    GPIO_CfgPinFunc(0,2,cn_p0_2_txd0);
    GPIO_CfgPinFunc(0,3,cn_p0_3_rxd0);
    pg_uart0_reg->IF.IIR;     //��һ����������жϱ�־
    pg_uart0_reg->LCR |= 1<<bo_uart_lcr_dlab;
    pg_uart0_reg->IF.FCR = 1<< bo_uart_fcr_ffen;   //ʹ��ifo��
    pg_uart0_reg->IF.FCR = 0x47;  //����4�ֽڴ����жϣ����շ�fifo��
    tg_uart0_cb.baud = 115200;
    uart_set_baud(0,115200);
    pg_uart0_reg->LCR = (3<<bo_uart_lcr_bw)     //8λ�ַ�
                        | (0<<bo_uart_lcr_st)   //1��ֹͣλ
                        | (0<<bo_uart_lcr_pe)   //��ֹ��żУ��
                        | (0<<bo_uart_lcr_lbc)  //��ֹ��Ϸ���
                        | (0<<bo_uart_lcr_dlab);

    pg_uart0_reg->MCR = 0;
    pg_uart0_reg->TER = 1<<bo_uart_ter_txen;
    pg_uart0_reg->DI.IER =  1<< bo_uart_ier_rbr;       //ʹ�ܽ����жϣ�


    tg_uart0_cb.sendding = false;
    Ring_Init(  &tg_uart0_cb.send_ring_buf,
                uart0_drv_send_buf,
                uart0_buf_len);
    Ring_Init(  &tg_uart0_cb.recv_ring_buf,
                uart0_drv_recv_buf,
                uart0_buf_len);

    Int_IsrConnect(cn_int_line_uart0,Uart0_Int);
    Int_SettoAsynSignal(cn_int_line_uart0);

    Int_ClearLine(cn_int_line_uart0);
    Int_RestoreAsynLine(cn_int_line_uart0);
    return true;

exit_from_right_buf_semp:
    Lock_SempDelete(tg_uart0_cb.send_buf_semp);
exit_from_left_buf_semp:
    return false;
}


//----uart0�ж�---------------------------------------------------------------
//����: ����ǽ����źŴ������ж�:
//      1.������fifo�е�������.�����ݴӽ��ռĴ���ȫ��copy������㻺������.
//      2.�������㻺������.�ô��ڿ��ƿ�״̬�ֵ����λ,������uart�����¼�.
//      3.���ʹ��ڽ����¼�.�����¼�����״̬���¼�
//      ����Ƿ����ź��������ж�:
//      1.��鷢��fifo�ж��ٿ�λ
//      2.���豸���ֽӿڶ����������ݣ�
//      3.�Ѷ���������д�뷢��fifo
//      ����ǳ����ź������ж�:�������ڳ����¼�
//����: �жϺ���û�в���.
//����: �жϺ���û�з���ֵ.
//-----------------------------------------------------------------------------
uint32_t Uart0_Int(ufast_t uart_int_line)
{
    uint32_t recv_trans,num;
    uint32_t IIR;
    uint8_t ch[16];
    while(1)
    {
        IIR = pg_uart0_reg->IF.IIR;
        if(IIR & 1)
            break;
        IIR = (IIR>>1)&0x7;
        switch(IIR)
        {
            case 1:     //����fifo�Ĵ�����
            {
                recv_trans = Ring_Read(&tg_uart0_cb.send_ring_buf,ch,16);
                if(recv_trans != 0)
                {
                    for(num = 0; num < recv_trans; num++)
                    {
                        pg_uart0_reg->RTD.THR = ch[num];
                    }
                }
                if(recv_trans < 16)
                    __uart_pause_send_int(pg_uart0_reg);
                if(Ring_Check(&tg_uart0_cb.send_ring_buf) < 16)
                    Lock_SempPost(tg_uart0_cb.send_buf_semp);
            }break;
            case 2:     //���մ���ˮƽ��
            case 6:     //���ճ�ʱ��fifo��������һ�����ݣ�����4�ַ�ʱ����������
            {
                for(num = 0; num<16; num++)
                {
                    if(pg_uart0_reg->LSR & 1)
                        ch[num] = pg_uart0_reg->RTD.THR;
                    else
                        break;
                }
                Ring_Write(&tg_uart0_cb.recv_ring_buf,ch,num);
                Lock_SempPost(tg_uart0_cb.recv_buf_semp);
            }break;
            case 3:     //�����־
            {
            }break;
            default: return 0;
        }
    }
    return 0;
}

void uart0_send(u8 *buf,u32 len)
{
    u32 completed = 0,sended;
    if((len==0) || (buf == NULL))
        return;
    __uart_pause_send_int(pg_uart0_reg);
    if(Ring_IsEmpty(&tg_uart0_cb.send_ring_buf))
    {
        while((pg_uart0_reg->FIFOLVL &0x0f00) != 0x0f00)
        {
            pg_uart0_reg->RTD.THR = buf[completed];
            completed++;
            if(completed == len)
                break;
        }
    }
    if(completed < len)
    {
        while(1)
        {
            sended = Ring_Write(&tg_uart0_cb.send_ring_buf,
                                (uint8_t*)buf+completed,len-completed);
            __uart_resume_send_int(pg_uart0_reg);
            if(sended != len-completed)     //����������û���ͳ�ȫ������
            {
                completed += sended;
                Lock_SempPend(tg_uart0_cb.send_buf_semp,CN_TIMEOUT_FOREVER);
                __uart_pause_send_int(pg_uart0_reg);
            }else
            {
                break;
            }
        }
    }else
    {
        __uart_resume_send_int(pg_uart0_reg);
    }
}


//========================================================
// �������ܣ�����0��������
// �����������
// �����������
// ����ֵ  ����
//========================================================
u32 uart0_read(u8 *buf,u32 len,u32 timeout)
{
    u32 readed,completed=0,start_time,time=0;
    start_time = DjyGetTime();
    while(1)
    {

        readed = Ring_Read(&tg_uart0_cb.recv_ring_buf,buf+completed,len-completed);

        if(readed != len-completed)
        {
            completed += readed;
            if(Lock_SempPend(tg_uart0_cb.recv_buf_semp,timeout-time) == false)
            {
                break;
            }else
            {
                time = DjyGetTime() - start_time;     //�ص�while���ٴζ����ڡ�
            }
        }else
        {
            completed = len;                            //�Ѿ���ɣ�
            break;
        }
    }
    return completed;
}

#endif  //for (CN_CFG_UART0_USED == 1)

#if(CN_CFG_UART1_USED == 1)

u32  Uart1_Int(ufast_t uart_int_line);

//----��ʼ��uart1ģ��----------------------------------------------------------
//���ܣ���ʼ��uart1ģ�飬��ʼ��״̬Ϊ��ʹ��dma���͵ķ�ʽ��
//������para���ڱ���������Ч
//���أ�true = �ɹ���ʼ����false = ��ʼ��ʧ��
//-----------------------------------------------------------------------------
ptu32_t module_init_uart1(ptu32_t para)
{
    //�������������ź�����ʹ��������������Ϊ0ʱ����д���̣߳���ȡ�߳�ʹ��������
    //���ݽ���trigger_level����ʱ�ͷ��ź�����ʹд���߳̽������
    tg_uart1_cb.send_buf_semp = Lock_SempCreate(1,0,"uart1 tx buf");
    if(tg_uart1_cb.send_buf_semp == NULL)
        goto exit_from_left_buf_semp;
    tg_uart1_cb.recv_buf_semp = Lock_SempCreate(1,0,"uart1 rx buf");
    if(tg_uart1_cb.recv_buf_semp == NULL)
        goto exit_from_right_buf_semp;
    //��ʼ��IO�˿�λuart����
    pg_sysctrl_reg->pconp |= 1<<3;      //ʹ��uart1ģ��
    GPIO_CfgPinFunc(0,15,cn_p0_15_txd1);
    GPIO_CfgPinFunc(0,16,cn_p0_16_rxd1);
    pg_uart1_reg->IF.IIR;     //��һ����������жϱ�־
    pg_uart1_reg->LCR |= 1<<bo_uart_lcr_dlab;
    pg_uart1_reg->IF.FCR = 1<< bo_uart_fcr_ffen;   //ʹ��ifo��
    pg_uart1_reg->IF.FCR = 0x47;  //����4�ֽڴ����жϣ����շ�fifo��
    tg_uart1_cb.baud = 115200;
    uart_set_baud(1,115200);
    pg_uart1_reg->LCR = (3<<bo_uart_lcr_bw)     //8λ�ַ�
                        | (0<<bo_uart_lcr_st)   //1��ֹͣλ
                        | (0<<bo_uart_lcr_pe)   //��ֹ��żУ��
                        | (0<<bo_uart_lcr_lbc)  //��ֹ��Ϸ���
                        | (0<<bo_uart_lcr_dlab);

    pg_uart1_reg->MCR = 0;
    pg_uart1_reg->TER = 1<<bo_uart_ter_txen;
    pg_uart1_reg->DI.IER =  1<< bo_uart_ier_rbr;       //ʹ�ܽ����жϣ�


    tg_uart1_cb.sendding = false;
    Ring_Init(  &tg_uart1_cb.send_ring_buf,
                uart1_drv_send_buf,
                uart1_buf_len);
    Ring_Init(  &tg_uart1_cb.recv_ring_buf,
                uart1_drv_recv_buf,
                uart1_buf_len);

    Int_IsrConnect(cn_int_line_uart1,Uart1_Int);
    Int_SettoAsynSignal(cn_int_line_uart1);

    Int_ClearLine(cn_int_line_uart1);
    Int_RestoreAsynLine(cn_int_line_uart1);
    return true;

exit_from_right_buf_semp:
    Lock_SempDelete(tg_uart1_cb.send_buf_semp);
exit_from_left_buf_semp:
    return false;
}


//----uart1�ж�---------------------------------------------------------------
//����: ����ǽ����źŴ������ж�:
//      1.������fifo�е�������.�����ݴӽ��ռĴ���ȫ��copy������㻺������.
//      2.�������㻺������.�ô��ڿ��ƿ�״̬�ֵ����λ,������uart�����¼�.
//      3.���ʹ��ڽ����¼�.�����¼�����״̬���¼�
//      ����Ƿ����ź��������ж�:
//      1.��鷢��fifo�ж��ٿ�λ
//      2.���豸���ֽӿڶ����������ݣ�
//      3.�Ѷ���������д�뷢��fifo
//      ����ǳ����ź������ж�:�������ڳ����¼�
//����: �жϺ���û�в���.
//����: �жϺ���û�з���ֵ.
//-----------------------------------------------------------------------------
uint32_t Uart1_Int(ufast_t uart_int_line)
{
    uint32_t recv_trans,num;
    uint32_t IIR;
    uint8_t ch[16];
    while(1)
    {
        IIR = pg_uart1_reg->IF.IIR;
        if(IIR & 1)
            break;
        IIR = (IIR>>1)&0x7;
        switch(IIR)
        {
            case 1:     //����fifo�Ĵ�����
            {
                recv_trans = Ring_Read(&tg_uart1_cb.send_ring_buf,ch,16);
                if(recv_trans != 0)
                {
                    for(num = 0; num < recv_trans; num++)
                    {
                        pg_uart1_reg->RTD.THR = ch[num];
                    }
                }
                if(recv_trans < 16)
                    __uart_pause_send_int(pg_uart1_reg);
                if(Ring_Check(&tg_uart1_cb.send_ring_buf) < 16)
                    Lock_SempPost(tg_uart1_cb.send_buf_semp);
            }break;
            case 2:     //���մ���ˮƽ��
            case 6:     //���ճ�ʱ��fifo��������һ�����ݣ�����4�ַ�ʱ����������
            {
                for(num = 0; num<16; num++)
                {
                    if(pg_uart1_reg->LSR & 1)
                        ch[num] = pg_uart1_reg->RTD.THR;
                    else
                        break;
                }
                Ring_Write(&tg_uart1_cb.recv_ring_buf,ch,num);
                Lock_SempPost(tg_uart1_cb.recv_buf_semp);
            }break;
            case 3:     //�����־
            {
            }break;
            default: return 0;
        }
    }
    return 0;
}

void uart1_send(u8 *buf,u32 len)
{
    u32 completed = 0,sended;
    if((len==0) || (buf == NULL))
        return;
    __uart_pause_send_int(pg_uart1_reg);
    if(Ring_IsEmpty(&tg_uart1_cb.send_ring_buf))
    {
        while((pg_uart1_reg->FIFOLVL &0x0f00) != 0x0f00)
        {
            pg_uart1_reg->RTD.THR = buf[completed];
            completed++;
            if(completed == len)
                break;
        }
    }
    if(completed < len)
    {
        while(1)
        {
            sended = Ring_Write(&tg_uart1_cb.send_ring_buf,
                                (uint8_t*)buf+completed,len-completed);
            __uart_resume_send_int(pg_uart1_reg);
            if(sended != len-completed)     //����������û���ͳ�ȫ������
            {
                completed += sended;
                Lock_SempPend(tg_uart1_cb.send_buf_semp,CN_TIMEOUT_FOREVER);
                __uart_pause_send_int(pg_uart1_reg);
            }else
            {
                break;
            }
        }
    }else
    {
        __uart_resume_send_int(pg_uart1_reg);
    }
}


//========================================================
// �������ܣ�����0��������
// �����������
// �����������
// ����ֵ  ����
//========================================================
u32 uart1_read(u8 *buf,u32 len,u32 timeout)
{
    u32 readed,completed=0,start_time,time=0;
    start_time = DjyGetTime();
    while(1)
    {
        readed = Ring_Read(&tg_uart1_cb.recv_ring_buf,buf+completed,len-completed);
        if(readed != len-completed)
        {
            completed += readed;
            if(Lock_SempPend(tg_uart1_cb.recv_buf_semp,timeout-time) == false)
            {
                break;
            }else
            {
                time = DjyGetTime() - start_time;     //�ص�while���ٴζ����ڡ�
            }
        }else
        {
            completed = len;                            //�Ѿ���ɣ�
            break;
        }
    }
    return completed;
}
#endif  //for (CN_CFG_UART1_USED == 1)





