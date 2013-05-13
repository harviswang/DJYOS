//----------------------------------------------------
//Copyright (C), 2004-2009,  lst.
//��Ȩ���� (C), 2004-2009,   lst.
//����ģ��: ��������
//���ߣ�lst
//�汾��V1.0.0
//�ļ�����: ������Դ��ʼ��
//����˵��:
//�޶���ʷ:
//    2. ...
//    1. ����:2009-11-21
//       ����:lst
//       �°汾�ţ�
//       �޸�˵��:��ʼ�汾
//------------------------------------------------------
#include "os_inc.h"

#if (cfg_gui_kernel == 1)

#include "font.h"
#include "ascii8x8.h"
#include "ascii8x16.h"
#include "gb2312_16.h"
#include "string.h"

static struct font_rsc *pg_cur_font;


//----��ȡ�������-------------------------------------------------------------
//����: �����������ƻ�ȡ������Դ��"C"��Ĭ������Ĵ��š�
//����: ��������
//����: ƥ����ַ����룬��δ��ƥ����룬ʹ��Ĭ�ϵ�ASCII����
//-----------------------------------------------------------------------------
struct font_rsc* font_get_loc_font_rsc(const char* font_name)
{
    struct font_rsc* font;

    if(strcmp(font_name, "C") == 0){
        
        font = font_search_font_rsc(cfg_font_default);
        
    }else
    {
        font = font_search_font_rsc(font_name);
    }

    return font;   
}

//----��װ����-----------------------------------------------------------------
//����: �������尲װ��ϵͳ��Դ������
//����: font��������������Դָ��
//      name������������
//����: true
//-----------------------------------------------------------------------------
bool_t font_install_font(struct font_rsc *font, char* name)
{
    struct rsc_node *rsc;
    rsc = rsc_search_tree(cn_font_rsc_tree);
    if(rsc == NULL)
        return false;       //�ַ���Դ�����δ����
        
    rsc_add_son(rsc,&font->node,sizeof(struct font_rsc),name);
    if(pg_cur_font == NULL)
        pg_cur_font = font;

    return true;
}

//----��������ģ���ʼ��-------------------------------------------------------
//����: ��ʼ����������ģ��
//����: para: ��Ч
//����: 1=�ɹ���0=ʧ��
//-----------------------------------------------------------------------------
ptu32_t module_init_font(ptu32_t para)
{
    static struct rsc_node font_root_rsc;    // ������Դ

    pg_cur_font = NULL;
    // ����ַ�������Դ���ڵ�
    if(rsc_add_tree_root(&font_root_rsc, 
                            sizeof(struct rsc_node), 
                            cn_font_rsc_tree))
    {
        return 1;
    }else
    {
    	printf_ng("install font module fail\r\n");
        return 0;
    }
}

//----��ȡ��ǰ�ַ�����---------------------------------------------------------
//����: ��ȡ��ǰʹ�õ��ַ�����
//����: ��ǰ�ַ�����
//-----------------------------------------------------------------------------
struct font_rsc* font_get_cur_font_rsc(void)
{
    return pg_cur_font;
}

//----�趨��ǰ�ַ�����---------------------------------------------------------
//����: �����ַ�������Ϊ��ǰʹ�õ��ַ�����,���ַ�����������Ȱ�װ��ϵͳ�У����
//      encoding��Դδ��װ��������ʲô������������ԭ���ĵ�ǰ�ַ�����
//����: encoding��ָ�����ַ�����
//����: NULL���趨ʧ��
//      �趨֮ǰ���ַ�����
//-----------------------------------------------------------------------------
struct font_rsc* font_set_cur_font_rsc(struct font_rsc* font)
{
    struct rsc_node *rsc;
    if(font == NULL)
        return NULL;
    rsc = rsc_search_tree(cn_font_rsc_tree);
    if(rsc == NULL)
        return NULL;       //�ַ���Դ��δ����
    rsc = rsc_search_son(rsc,font->node.name);
    if(rsc != NULL)
    {
        pg_cur_font = (struct font_rsc*)rsc;
    }
    return pg_cur_font;     
}

//----�����ַ�������Դ---------------------------------------------------------
//����: �������������ַ�������Դ
//����: name��ָ�����ַ���������
//����: NULL���޴˱�����Դ
//      Ҫ�ҵ��ַ�������Դ
//-----------------------------------------------------------------------------
struct font_rsc* font_search_font_rsc(const char* name)
{
    struct rsc_node *rsc;
    
    rsc = rsc_search_tree(cn_font_rsc_tree);
    if(rsc == NULL)
        return NULL;       //�ַ���Դ��δ����    

    return (struct font_rsc*)rsc_search_son(rsc, (char*)name);
}

#endif      //for #if (cfg_gui_kernel == 1)

