//----------------------------------------------------
//Copyright (C), 2004-2009,  lst.
//��Ȩ���� (C), 2004-2009,   lst.
//����ģ��:�ж�ģ��
//���ߣ�lst
//�汾��V1.0.0
//�ļ�����: �����жϺ�
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2009--9-28
//   ����: lst
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#ifndef __CPU_PERI_WDT_H__
#define __CPU_PERI_WDT_H__

#ifdef __cplusplus
extern "C" {
#endif

struct st_wdt_reg
{
    vu32  wdmod;
    vu32 wdtc;
    vu32  wdfeed;
    vu32 wdtv;
    vu32 wdclksel;
 };

#ifdef __cplusplus
}
#endif

#endif //__CPU_PERI_WDT_H__
