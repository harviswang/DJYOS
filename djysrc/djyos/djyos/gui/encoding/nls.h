//----------------------------------------------------
//Copyright (C), 2011,  .
//��Ȩ���� (C), 2011,   djy.
//����ģ��: National Language Support
//���ߣ�mjh
//�汾��V1.0.0
//�ļ�����: NLS
//����˵��:
//�޶���ʷ:
//    2. ...
//    1. ����:2011-05-17
//       ����:mjh
//       �°汾�ţ�
//       �޸�˵��:��ʼ�汾
//------------------------------------------------------
#if (cfg_gui_kernel == 1)

#ifndef __NLS_MODULE_H_
#define __NLS_MODULE_H_

#define cn_nls_charset_default          cn_nls_charset_gb2312

struct char_encoding_rsc* nls_get_loc_encoding_rsc(const char* loc);
ptu32_t module_init_nls(ptu32_t para);

#endif

#endif      //for #if (cfg_gui_kernel == 1)

