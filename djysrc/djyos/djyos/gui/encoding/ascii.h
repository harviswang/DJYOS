//----------------------------------------------------
//Copyright (C), 2011,  .
//��Ȩ���� (C), 2011,   djy.
//����ģ��: ASCII�ַ���������
//���ߣ�mjh
//�汾��V1.0.0
//�ļ�����: ASCII�ַ��������
//����˵��:
//�޶���ʷ:
//    2. ...
//    1. ����:2011-05-17
//       ����:mjh
//       �°汾�ţ�
//       �޸�˵��:��ʼ�汾
//------------------------------------------------------
#if (cfg_charset_ascii == 1)

#ifndef __GB2312_H_
#define __GB2312_H_

#ifdef __cplusplus
extern "C" {
#endif

#define cn_nls_charset_ascii            "ascii"

ptu32_t module_init_encode_ascii(ptu32_t para);

#ifdef __cplusplus
}
#endif

#endif //__GB2312_H_

#endif          //#if (cfg_charset_ascii == 1)

