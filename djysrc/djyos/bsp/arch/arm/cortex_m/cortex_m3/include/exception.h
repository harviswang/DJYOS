//----------------------------------------------------
//Copyright (C), 2004-2009,  lst.
//��Ȩ���� (C), 2004-2009,   lst.
//����ģ��:�ж�ģ��
//���ߣ�lst
//�汾��V1.0.0
//�ļ�����: cm3�쳣����ģ��
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2009-08-30
//   ����: lst
//   �°汾��: V1.0.0
//   �޸�˵��:
//------------------------------------------------------
#ifndef _EXCEPTION_H_
#define _EXCEPTION_H_

#ifdef __cplusplus
extern "C" {
#endif

//ϵͳ�쳣��
#define cn_sysvect_reset            1
#define cn_sysvect_nmi              2
#define cn_sysvect_hard_fault       3
#define cn_sysvect_memmanage_fault  4
#define cn_sysvect_bus_fault        5
#define cn_sysvect_usage_fault      6
#define cn_sysvect_svc              11
#define cn_sysvect_debug            12
#define cn_sysvect_pendsv           14
#define cn_sysvect_systick          15

extern u32 u32g_exp_table[];
void exp_init(void);
void exp_connect_nmi(void (*esr)(void));
void exp_connect_systick(void (*tick)(void));
void exp_esr_tick(void);

#ifdef __cplusplus
}
#endif

#endif // _EXCEPTION_H_

