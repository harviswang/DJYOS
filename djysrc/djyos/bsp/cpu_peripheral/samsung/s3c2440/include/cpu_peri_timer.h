//----------------------------------------------------
//Copyright (C), 2004-2009,  lst.
//��Ȩ���� (C), 2004-2009,   lst.
//����ģ��: Ӳ����ʱ��
//���ߣ�lst
//�汾��V1.0.0
//�ļ�����: Ӳ����ʱ������
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2009-03-10
//   ����: lst
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#ifndef __timer_hard_h__
#define __timer_hard_h__

#ifdef __cplusplus
extern "C" {
#endif

struct timer_reg
{
    u32 TCFG0;     //0x51000000
    u32 TCFG1;     //0x51000004
    u32 TCON;      //0x51000008
    u32 TCNTB0;    //0x5100000C
    u32 TCMPB0;    //0x51000010
    u32 TCNTO0;    //0x51000014
    u32 TCNTB1;    //0x51000018
    u32 TCMPB1;    //0x5100001C
    u32 TCNTO1;    //0x51000020
    u32 TCNTB2;    //0x51000024
    u32 TCMPB2;    //0x51000028
    u32 TCNTO2;    //0x5100002C
    u32 TCNTB3;    //0x51000030
    u32 TCMPB3;    //0x51000034
    u32 TCNTO3;    //0x51000038
    u32 TCNTB4;    //0x5100003C
    u32 TCNTO4;    //0x51000040
};

extern struct timer_reg volatile * const pg_timer_reg;

void timer_set_clk_source(ufast_t timer,ufast_t value);
void timer_set_precale(ufast_t group,uint16_t value);
void timer_set_counter(ufast_t timer,uint16_t value);
void timer_set_compare(ufast_t timer,uint16_t value);
void timer_set_type(ufast_t timer,ufast_t type);
void timer_reload(ufast_t timer);
void timer_start(ufast_t timer);
void timer_stop(ufast_t timer);
uint16_t timer_read(ufast_t timer);

#ifdef __cplusplus
}
#endif

#endif //__timer_hard_h__
