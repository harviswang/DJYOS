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
#ifndef ____CPU_PERI_IWDG_H___
#define ____CPU_PERI_IWDG_H___

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

#endif //____CPU_PERI_IWDG_H___
