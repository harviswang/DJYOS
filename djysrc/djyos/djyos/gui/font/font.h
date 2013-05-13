//----------------------------------------------------
//Copyright (C), 2004-2009,  lst.
//��Ȩ����  (C), 2004-2009,   lst.
//����ģ��: ��������
//���ߣ�lst
//�汾��V1.0.0
//�ļ�����: ������Դͷ�ļ�
//����˵��:
//�޶���ʷ:
//    2. ...
//    1. ����:2009-11-21
//       ����:lst
//       �°汾�ţ�
//       �޸�˵��:��ʼ�汾
//------------------------------------------------------
#if (cfg_gui_kernel == 1)
#ifndef __FONT_H_
#define __FONT_H_

#ifdef __cplusplus
extern "C" {
#endif

#define cn_font_rsc_tree            "font"

/* �������� */
#define cn_font_type_dot            1       // ��������
#define cn_font_type_truetype       2       //ʸ������
struct font_info
{
    u8 type;
};

struct font_rsc
{
    struct  rsc_node node;

    struct font_info fi;

    bool_t (*load_font)(u8 *zk_addr);      //��������

    void (*unload_font)(void);      //ж������

// ��ȡucs4����Ϊcharcode�ַ�����ʾ���󣬰ѵ�����䵽font_bitmap�У�������Ӧ����
// ��font_bitmap�����ڴ档
// ���font_bitmap�����е�bm_bits����ΪNULL���򱾺����˻�Ϊ��ѯ��������
// font_bitmap�����з���ͼ����������������Լ���bm_bits�����ڴ�ߴ磬����һ����
// ����ʹ�ö��ֳߴ������ʱ���ر����á�
// resv,����������
// ����: true = �ɹ�ִ�У�false=��֧���ַ�
    bool_t (*get_char_bitmap)(u32 charcode, u32 size, u32 resv,
                              struct st_rect_bitmap *font_bitmap);
};

bool_t font_install_font(struct font_rsc *font,char *name);

ptu32_t module_init_font(ptu32_t para);

struct font_rsc* font_get_cur_font_rsc(void);
struct font_rsc* font_set_cur_font_rsc(struct font_rsc* font);
struct font_rsc* font_search_font_rsc(const char* name);

#ifdef __cplusplus
}
#endif

#endif //__FONT_H_
#endif //   for #if (cfg_gui_kernel == 1)

