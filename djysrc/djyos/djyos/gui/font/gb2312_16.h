//----------------------------------------------------
//Copyright (C), 2011,  .
//��Ȩ���� (C), 2011,   djy.
//����ģ��: GB2312-80��׼��������
//���ߣ�mjh
//�汾��V1.0.0
//�ļ�����: GB2312-80�������
//����˵��:
//�޶���ʷ:
//    2. ...
//    1. ����:2011-05-17
//       ����:mjh
//       �°汾�ţ�
//       �޸�˵��:��ʼ�汾
//------------------------------------------------------
#if (cfg_gui_kernel == 1)
#if (cfg_gb2312_8x16_1616 == 1)

#ifndef __GB2312_16_H_
#define __GB2312_16_H_

#ifdef __cplusplus
extern "C" {
#endif


//ascii:8*16,����16*16
#define cn_font_gb2312_8x16_1616    "gb2312_8_16_1616"

ptu32_t module_init_font_gb2312_1616(ptu32_t para);


#ifdef __cplusplus
}
#endif

#endif //__GB2312_16_H_

#endif      //#if (cfg_gb2312_8x16_1616 == 1)
#endif //   for #if (cfg_gui_kernel == 1)

