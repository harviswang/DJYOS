//----------------------------------------------------
//Copyright (C), 2011,  mjh.
//��Ȩ���� (C), 2011,   mjh.
//����ģ��: �ַ���������
//���ߣ�mjh
//�汾��V1.0.0
//�ļ�����: �ַ�������Դ��ʼ��
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

#include "ascii.h"
#include "utf8.h"
#include "gb2312.h"

static struct char_encoding_rsc *pg_cur_char_encoding;    // ������Դ
//----��װ�ַ�����-----------------------------------------------------------------
//����: �����ַ����밲װ��ϵͳ��Դ������
//����: encoding���������ַ�������Դָ��
//      name�������ַ�������
//����: true
//-----------------------------------------------------------------------------
bool_t nls_install_char_encoding(struct char_encoding_rsc *encoding, char* name)
{
    struct rsc_node *rsc;
    rsc = rsc_search_tree(cn_char_encoding_rsc_tree);
    if(rsc == NULL)
        return false;       //�ַ���Դ�����δ����
        
    rsc_add_son(rsc,&encoding->node,sizeof(struct char_encoding_rsc),name);    

    return true;
}

//----�ַ�����ģ���ʼ��-------------------------------------------------------
//����: ��ʼ���ַ�����ģ��
//-----------------------------------------------------------------------------
ptu32_t module_init_char_encoding(ptu32_t para)
{
    static struct rsc_node encoding_root_rsc;    // ������Դ

    pg_cur_char_encoding = NULL;
    // ����ַ�������Դ���ڵ�
    if(rsc_add_tree_root(&encoding_root_rsc, 
                            sizeof(struct rsc_node), 
                            cn_char_encoding_rsc_tree))
    {
        return 1;
    }else
    {
    	printf_ng("install charset module fail\r\n");
        return 0;
    }
}

//----��ȡ��ǰ�ַ�����---------------------------------------------------------
//����: ��ȡ��ǰʹ�õ��ַ�����
//����: ��ǰ�ַ�����
//-----------------------------------------------------------------------------
struct char_encoding_rsc* nls_get_cur_encoding_rsc(void)
{
    return pg_cur_char_encoding;
}

//----�趨��ǰ�ַ�����---------------------------------------------------------
//����: �����ַ�������Ϊ��ǰʹ�õ��ַ�����,���ַ�����������Ȱ�װ��ϵͳ�У����
//      encoding��Դδ��װ��������ʲô������������ԭ���ĵ�ǰ�ַ�����
//����: encoding��ָ�����ַ�����
//����: NULL���趨ʧ��
//      �趨֮ǰ���ַ�����
//-----------------------------------------------------------------------------
struct char_encoding_rsc* nls_set_cur_encoding_rsc(
                                struct char_encoding_rsc* encoding)
{
    struct rsc_node *rsc;
    if(encoding == NULL)
        return NULL;
    rsc = rsc_search_tree(cn_char_encoding_rsc_tree);
    if(rsc == NULL)
        return NULL;       //�ַ���Դ��δ����    
    rsc = rsc_search_son(rsc,encoding->node.name);
    if(rsc != NULL)
    {
        pg_cur_char_encoding = (struct char_encoding_rsc*)rsc;
    }
    return pg_cur_char_encoding;     
}

//----�����ַ�������Դ---------------------------------------------------------
//����: �������������ַ�������Դ
//����: name��ָ�����ַ���������
//����: NULL���޴˱�����Դ
//      Ҫ�ҵ��ַ�������Դ
//-----------------------------------------------------------------------------
struct char_encoding_rsc* nls_search_char_encoding_rsc(const char* name)
{
    struct rsc_node *rsc;
    
    rsc = rsc_search_tree(cn_char_encoding_rsc_tree);
    if(rsc == NULL)
        return NULL;       //�ַ���Դ��δ����    

    return (struct char_encoding_rsc*)rsc_search_son(rsc, (char *)name);
}

#endif      //for #if (cfg_gui_kernel == 1)

