//----------------------------------------------------
//Copyright (C), 2004-2009,  luoshitian.
//��Ȩ���� (C), 2004-2009,   ������.
//����ģ��:�ж�ģ��
//����:  ������.
//�汾��V1.0.0
//�ļ�����: cm3�쳣����ģ��
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2009-08-30
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��:
//------------------------------------------------------
#ifndef _EXCEPTION_H_
#define _EXCEPTION_H_

#ifdef __cplusplus
extern "C" {
#endif

extern void (*usr_top)(ufast_t ufl_line);
extern void (*usr_bottom)(ufast_t ufl_line);
extern void (*svc_top)(ufast_t ufl_line);
extern void (*svc_bottom)(ufast_t ufl_line);
extern void (*irq_top)(ufast_t ufl_line);
extern void (*irq_bottom)(ufast_t ufl_line);
extern void (*fiq_top)(ufast_t ufl_line);
extern void (*fiq_bottom)(ufast_t ufl_line);
extern void (*undef_top)(ufast_t ufl_line);
extern void (*undef_bottom)(ufast_t ufl_line);
extern void (*abort_top)(ufast_t ufl_line);
extern void (*abort_bottom)(ufast_t ufl_line);

extern void (* engine_irq)(ufast_t ufl_line);

//ϵͳ�쳣��


void exp_init(void);

#ifdef __cplusplus
}
#endif

#endif // _EXCEPTION_H_

