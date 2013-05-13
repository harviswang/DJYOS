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
#include "os_inc.h"

#if (cfg_gui_kernel == 1)

#include "gk_inc.h"
#include "nls.h"
#include <locale.h>
#include <string.h>

//----��ȡ�����ַ�����---------------------------------------------------------
//����: �����������ƻ�ȡ�ַ����룬"C"��Ĭ�ϱ���Ĵ��š�
//����: �������Ʊ���
//����: ƥ����ַ����룬��δ��ƥ����룬ʹ��Ĭ�ϵ�ASCII����
//-----------------------------------------------------------------------------
struct char_encoding_rsc* nls_get_loc_encoding_rsc(const char* loc)
{
    struct char_encoding_rsc* encoding;

    if(strcmp(loc, "C") == 0)
    {
        encoding = nls_search_char_encoding_rsc(cfg_charset_default);
    }else
    {
        encoding = nls_search_char_encoding_rsc(loc);
    }

    return encoding;
}

//----��ʼ��NLSģ��------------------------------------------------------------
//����: ��ʼ��NLSģ��
//����: 1=�ɹ���0=ʧ��
//-----------------------------------------------------------------------------
ptu32_t module_init_nls(ptu32_t para)
{
    struct char_encoding_rsc* encoding;
    
    // ��ȡĬ��locale
    //loc = setlocale(LC_ALL, NULL);

    // ����locale�ҳ���Ӧ���ַ�������Դ
    encoding = nls_get_loc_encoding_rsc("C");
    nls_set_cur_encoding_rsc(encoding);
    
    printf_ng("-module_init_nls\r\n");

    return 1;
}

#endif      //for #if (cfg_gui_kernel == 1)

