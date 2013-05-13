//----------------------------------------------------
//Copyright (C), 2011-2011,  luoshitian.
//��Ȩ���� (C), 2004-2010,   ������.
//����ģ��: gui
//����:  ������.
//�汾��V1.0.0
//�ļ�����: gui kernelӦ�ó���ӿڲ���
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2011-10-07
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include "os_inc.h"

#if (cfg_gui_kernel == 1)

#include "gk_inc.h"
#include "gk_api.h"

//----��������-----------------------------------------------------------------
//����: �������棬����ʾ�����������Ҫ�����������ʹ�á�������ʵ��һ����ͨ����
//      �ǳ����ƣ������Ҫ����:
//          1�����治��С����ʾ���ߴ磬�����ڿ��ԡ�
//          2�������rop����ֻ����cn_r3_srccopy�����ڿ������⡣
//          3����������ȼ�ֻ����0��
//          4��һ����ʾ���£�����ֻ��һ������display��desktopָ��ָ��
//      ��һ����ʾ���µ����д��ڶ���������Ӵ��ڡ�
//����: display�������������ʾ��
//      desktop���´��������棬�������ݽṹ�����ڴ��ɵ������ṩ
//      width��height������ĳߴ磬С����ʾ���ߴ������Ϊ��ʾ���ߴ�
//      color����������ʱ������ɫ
//      buf_mode��true=����ģʽ��false=�ǻ���ģʽ
//����: true=�����ɹ���false=����ʧ��
//-----------------------------------------------------------------------------
bool_t gk_api_create_desktop(struct display_rsc *display,
                                        struct gkwin_rsc *desktop,
                                        s32 width,s32 height,u32 color,
                                        bool_t buf_mode)
{
    struct gksc_para_create_desktop para;
    struct gkwin_rsc *result;
    if((NULL == display) || (NULL == desktop))
        return false;
    para.display = display;
    para.desktop = desktop;
    para.width = width;
    para.height = height;
    para.color = color;
    para.buf_mode = buf_mode;
    result = (void*)gk_create_desktop(&para);
    if(result == NULL)
        return false;
    else
        return true;
}

//----��ȡ����ָ��-------------------------------------------------------------
//����: ������ʾ��������������ָ�롣��ָ��ָ��һ�����ڿ��ƿ�
//����: display_name������������ʾ��������
//����: ����ҵ����򷵻�ָ�룬���򷵻�NULL
//-----------------------------------------------------------------------------
struct gkwin_rsc *gk_api_get_desktop(char *display_name)
{
    struct display_rsc *display;
    display = (struct display_rsc *)rsc_search_tree("display");
    if(display != NULL)
        display = (struct display_rsc *)rsc_search_scion(
        		(struct rsc_node*)display,display_name);
    if(display != NULL)
        return display->desktop;
    else
        return NULL;
}
//----��������-----------------------------------------------------------------
//����: ����һ�����ڣ��´����Ĵ���ֻ��һ����������û����䣬Ҳû�б߿��һ��
//      װ�Ρ�
//����: parent��������ָ��
//      newwin���´����Ĵ��ڣ��������ݽṹ�����ڴ��ɵ������ṩ
//      left��top��right��bottom������ĳߴ磬����ڸ�����
//      color����������ʱ������ɫ
//      buf_mode��true=����ģʽ��false=�ǻ���ģʽ
//      name���´��ڵ����֣���copy��newwin��
//      rop_code���´��ڵ�rop����
//      transparentcolor,�趨����͸��ɫ
//����: true=�����ɹ���false=����ʧ��
//-----------------------------------------------------------------------------
bool_t gk_api_create_gkwin(struct gkwin_rsc *parent,
                                        struct gkwin_rsc *newwin,
                                        s32 left,s32 top,s32 right,s32 bottom,
                                        u32 color,bool_t buf_mode,
                                        char *name,u32 rop_code,
                                        u32 transparentcolor)
{
    struct gksc_para_create_gkwin para;
    struct gkwin_rsc *result;
    if((NULL == parent) || (NULL == newwin))
        return false;
    para.parent_gkwin = parent;
    para.gkwin = newwin;
    para.result = &result;
    para.left = left;
    para.top = top;
    para.right = right;
    para.bottom = bottom;
    para.color = color;
    para.buf_mode = buf_mode;
    para.name = name;
    para.rop_code = rop_code;
    para.transparentcolor = transparentcolor;
    gk_syscall_chunnel(cn_gksc_create_gkwin,cn_timeout_forever,
                            &para,sizeof(para),NULL,0);
    if(result != NULL)
        return false;
    else
        return true;
}
//----�첽��䴰��-------------------------------------------------------------
//����: ��ָ����ɫ���һ�����ڡ�
//����: gkwin�������Ĵ���
//      color�������ɫ
//����: ��
//-----------------------------------------------------------------------------
void gk_api_fill_win(struct gkwin_rsc *gkwin,u32 color,u32 sync_time)
{
    struct gksc_para_fill_win para;
    if(NULL == gkwin)
        return;
    para.gkwin = gkwin;
    para.color = color;
    gk_syscall_chunnel(cn_gksc_fill_win,sync_time,&para,sizeof(para),NULL,0);
    return;
}
//----��䴰�ڵ�һ����---------------------------------------------------------
//����: ��ָ����ɫ���һ�����ڵ�һ���֡�
//����: gkwin��������Ŀ�괰��
//      rect��������λ��
//      color��Ҫ�����ɫ
//����: ��
//-----------------------------------------------------------------------------
void gk_api_fill_part_win(struct gkwin_rsc *gkwin,
                            	struct st_rect rect,u32 color,u32 sync_time)
{
    struct gksc_para_fill_part_win para;
    if(NULL == gkwin)
        return;
    para.gkwin = gkwin;
    para.rect = rect;
    para.color = color;
    gk_syscall_chunnel(cn_gksc_fill_part_win,sync_time,&para,
                    	sizeof(para),NULL,0);
    return;
}

//----��ʾ�ı�-----------------------------------------------------------------
//����: ��Ĭ�������Ĭ���ַ�����ʾһ���ı�����
//����: gkwin����ʾ��Ŀ�괰��
//      x��y����ʾλ�ã������gkwin
//      text������ʾ���ַ���
//      count���ַ������ȣ�����������
//����: ��
//-----------------------------------------------------------------------------
void gk_api_draw_text(struct gkwin_rsc *gkwin,s32 x,s32 y,
                        const char *text,u32 count,u32 sync_time)
{
    struct gksc_para_draw_text para;
    if((NULL == gkwin) || (NULL == text) || (0 == count))           
        return;
    para.gkwin = gkwin;
    para.x = x;
    para.y = y;
    para.count = count;
    gk_syscall_chunnel(cn_gksc_draw_text,sync_time,
                        &para,sizeof(para),(void*)text,count);
    return;
}
//----����---------------------------------------------------------------------
//����: �������ء�
//����: gkwin�����Ƶ�Ŀ�괰��
//      x��y�����Ƶ�λ�ã������gkwin
//      color����������ɫ
//      rop2_code����Ԫ��դ������
//����: ��
//-----------------------------------------------------------------------------
void gk_api_set_pixel(struct gkwin_rsc *gkwin,s32 x,s32 y,
                        u32 color,u32 rop2_code,u32 sync_time)
{
    struct gksc_para_set_pixel para;
    if(NULL == gkwin)           
        return;    
    para.gkwin = gkwin;
    para.x = x;
    para.y = y;
    para.color = color;
    para.rop2_code = rop2_code;
    gk_syscall_chunnel(cn_gksc_set_pixel,sync_time,
                        &para,sizeof(para),NULL,0);
    return;    
}
//----�������ؿ�ֱ�ߣ������������˵�-------------------------------------------
//����: ����ֱ�ߣ������������˵�
//����: gkwin�����Ƶ�Ŀ�괰��
//      x1��y1��x2��y2,Ҫ���Ƶ�ֱ�ߵ����յ�����
//      color�����Ƶ���ɫ
//      rop2_code����Ԫ��դ������
//����: ��
//-----------------------------------------------------------------------------
void gk_api_lineto(struct gkwin_rsc *gkwin, s32 x1,s32 y1,
                    s32 x2,s32 y2,u32 color,u32 rop2_code,u32 sync_time)
{
    struct gksc_para_lineto para;
    if(NULL == gkwin)          
        return;
    para.gkwin = gkwin;
    para.x1 = x1;
    para.y1 = y1;
    para.x2 = x2;
    para.y2 = y2;
    para.color = color;
    para.rop2_code = rop2_code;
    gk_syscall_chunnel(cn_gksc_lineto,sync_time,
                        &para,sizeof(para),NULL,0);
    return;    
}
//----�������ؿ�ֱ�ߣ����������˵�---------------------------------------------
//����: ����ֱ�ߣ����������˵�
//����: gkwin�����Ƶ�Ŀ�괰��
//      x1��y1��x2��y2,Ҫ���Ƶ�ֱ�ߵ����յ�����
//      color�����Ƶ���ɫ
//      rop2_code����Ԫ��դ������
//����: ��
//-----------------------------------------------------------------------------
void gk_api_lineto_ie(struct gkwin_rsc *gkwin, s32 x1,s32 y1,
                    s32 x2,s32 y2,u32 color,u32 rop2_code,u32 sync_time)
{
    struct gksc_para_lineto para;
    if(NULL == gkwin)          
        return;
    para.gkwin = gkwin;
    para.x1 = x1;
    para.y1 = y1;
    para.x2 = x2;
    para.y2 = y2;
    para.color = color;
    para.rop2_code = rop2_code;
    gk_syscall_chunnel(cn_gksc_lineto_inc_end,sync_time,
                        &para,sizeof(para),NULL,0);
    return;    
}
//----��bitmap���޹�դ����-----------------------------------------------------
//����: ����bitmap���޹�դ������
//����: gkwin�����Ƶ�Ŀ�괰��
//      bitmap�������Ƶ�λͼ      
//      x��y������λ�õ����Ͻ�����
//����: ��
//-----------------------------------------------------------------------------
void gk_api_draw_bitmap(struct gkwin_rsc *gkwin,
                                struct st_rect_bitmap *bitmap,
                                s32 x,s32 y,u32 sync_time)
{
    struct gksc_para_draw_bitmap para;
    if(NULL == gkwin)         
        return;
    para.gkwin = gkwin;
    para.bitmap = *bitmap;
    para.x = x;
    para.y = y;
    gk_syscall_chunnel(cn_gksc_draw_bitmap,sync_time,
                        &para,sizeof(para),NULL,0);
    return;    
}

//----����դλͼ---------------------------------------------------------------
//����: ����bitmap������դ������
//����: gkwin�����Ƶ�Ŀ�괰��
//      bitmap�������Ƶ�λͼ
//      pattern��Ҫ���õ�ͼ��λͼ    
//      bitmsk��rop4��������Ҫ������λͼ��һ����ɫλͼ
//      transparentcolor��Ҫ���õ�͸��ɫ   
//      x��y�����Ƶ�Ŀ��λ��
//      rop_code����դ������
//����: ��
//-----------------------------------------------------------------------------
void gk_api_draw_bitmap_rop(struct gkwin_rsc *gkwin, 
                                struct st_rect_bitmap *bitmap,
                                struct st_rect_bitmap *pattern,
                                struct st_rect_bitmap *bitmsk,
                                u32 transparentcolor,
                                s32 x,s32 y,u32 rop_code,u32 sync_time)
{
    struct gksc_para_draw_bitmap_rop para;
    if(NULL == gkwin)           
        return;
    para.gkwin = gkwin;
    para.bitmap = *bitmap;
    para.pattern = *pattern;
    para.bitmsk = *bitmsk;
    para.transparentcolor = transparentcolor;
    para.x = x;
    para.y = y;
    para.rop_code = rop_code;
    gk_syscall_chunnel(cn_gksc_draw_bitmap_rop,sync_time,
                        &para,sizeof(para),NULL,0);
    return;    
}
//----�������ؿ��Բ-----------------------------------------------------------
//����: �������ؿ��Բ��
//����: gkwin�����Ƶ�Ŀ�괰��
//      x0��y0��Բ�����ĵ����꣬����ڴ���
//      r��Բ�İ뾶
//      color����Բʹ�õ���ɫ
//      rop2_code����Ԫ��դ������
//����: ��
//-----------------------------------------------------------------------------
void gk_api_draw_circle(struct gkwin_rsc *gkwin,s32 x0,s32 y0,
                    u32 r,u32 color,u32 rop2_code,u32 sync_time)
{
    struct gksc_para_draw_circle para;
    if(NULL == gkwin)          
        return;
    para.gkwin = gkwin;
    para.x0 = x0;
    para.y0 = y0;
    para.r = r;
    para.color = color;
    para.rop2_code = rop2_code;
    gk_syscall_chunnel(cn_gksc_draw_circle,sync_time,
                        &para,sizeof(para),NULL,0);
    return;    
}
//----��Bezier����-------------------------------------------------------------
//����: ��Bezier���ߡ�
//����: gkwin�����Ƶ�Ŀ�괰��
//      x1��y1��x2��y2��x3��y3��x4��y4������Bezier���ߵ��ĸ����Ƶ�
//      color�����Ƶ���ɫ
//      rop2_code����Ԫ��դ������
//����: ��
//-----------------------------------------------------------------------------
void gk_api_draw_bezier(struct gkwin_rsc *gkwin,float x1,float y1,
                	float x2,float y2,float x3,float y3,float x4,float y4,
                    u32 color,u32 rop2_code,u32 sync_time)
{
    struct gksc_para_Bezier para;
    if(NULL == gkwin)          
        return;
    para.gkwin = gkwin;
    para.x1 = x1;
    para.y1 = y1;
    para.x2 = x2;
    para.y2 = y2;
    para.x3 = x3;
    para.y3 = y3;
    para.x4 = x4;
    para.y4 = y4;
    para.color = color;
    para.rop2_code = rop2_code;
    gk_syscall_chunnel(cn_gksc_Bezier,sync_time,
                        &para,sizeof(para),NULL,0);
    return;    
}
//----�ƶ�����-----------------------------------------------------------------
//����: �ƶ����ڡ�
//����: gkwin��Ŀ�괰��
//      left��top�������ƶ�������Ͻ����꣬����ڸ�����
//����: ��
//-----------------------------------------------------------------------------
void gk_api_move_win(struct gkwin_rsc *gkwin, s32 left,s32 top,u32 sync_time)
{
    struct gksc_para_move_win para;
    if(NULL == gkwin)
        return;
    para.gkwin = gkwin;
    para.left = left;
    para.top = top;
    gk_syscall_chunnel(cn_gksc_move_win,sync_time,
                        &para,sizeof(para),NULL,0);
    return;
}
//----�ı䴰��λ�óߴ�---------------------------------------------------------
//����: �ı䴰�ڵ�λ�úͳߴ磬�������������ڸ�����
//����: gkwin��Ŀ�괰��
//      left��top��right��bottom�����ڵ������꣬����ڸ�����
//����: ��
//-----------------------------------------------------------------------------
void gk_api_change_win_area(struct gkwin_rsc *gkwin, s32 left,s32 top,
                                       s32 right,s32 bottom, u32 sync_time)
{
    struct gksc_para_change_win_area para;
    if(NULL == gkwin)
        return;
    para.gkwin = gkwin;
    para.left = left;
    para.top = top;
    para.right = right;
    para.bottom = bottom;
    gk_syscall_chunnel(cn_gksc_change_win_area,sync_time,
                        &para,sizeof(para),NULL,0);
    return;
}

//----���ô��ڵı߽�ģʽ-------------------------------------------------------
//����: ���ô��ڵı߽�ģʽ��
//����: gkwin��Ŀ�괰��
//      mode�� ���ڵı߽�ģʽ��cn_bound_limitΪ���ޣ�cn_bound_unlimitΪ������
//����: ��
//-----------------------------------------------------------------------------
void gk_api_set_bound_mode(struct gkwin_rsc *gkwin, bool_t mode)
{
    struct gksc_para_set_bound_mode para;
    if(NULL == gkwin)
        return;
    para.gkwin = gkwin;
    para.mode = mode;
    gk_syscall_chunnel(cn_gksc_set_bound_mode,cn_timeout_forever,
                        &para,sizeof(para),NULL,0);
    return;
}
//----���ô�����ʾ���ȼ�-------------------------------------------------------
//����: ���ô�����ʾ���ȼ���
//����: gkwin��Ŀ�괰��
//      prio�������ȼ�
//      sync_time������ͼ��ʱ��ͬ�����ڴ�ʱ���ڲ����Ƿ������ɶ�����
//����: ��
//-----------------------------------------------------------------------------
void gk_api_set_prio(struct gkwin_rsc *gkwin, sint16_t prio,u32 sync_time)
{
    struct gksc_para_set_prio para;
    if(NULL == gkwin)
        return;
    para.gkwin = gkwin;
    para.prio = prio;
    gk_syscall_chunnel(cn_gksc_set_prio,sync_time,
                        &para,sizeof(para),NULL,0);
    return;
}
//----���ô��ڵĹ�դ����-------------------------------------------------------
//����: ���ô��ڵĹ�դ���ԡ�
//����: gkwin��Ŀ�괰��
//      bitmsk,����λͼ
//      rop_code,�µĹ�դ������    
//����: true=�ɹ���false=ʧ��
//-----------------------------------------------------------------------------
bool_t gk_api_set_rop_code(struct gkwin_rsc *gkwin,
                            struct st_rect_bitmap *bitmsk,
                            u32 rop_code,u32 sync_time)
{
    struct gksc_para_set_rop_code para;
    if(NULL == gkwin)        
        return false;
    if(rop_code == gkwin->rop_code)
        return true;
    para.gkwin = gkwin;
    para.bitmsk = *bitmsk;
    para.rop_code = rop_code;
    gk_syscall_chunnel(cn_gksc_set_rop_code,sync_time,
                        &para,sizeof(para),NULL,0);
    return true;    
}
//----���ô���ͼ��λͼ---------------------------------------------------------
//����: �ı䴰�ڵ�ͼ��λͼ��
//����: gkwin��Ŀ�괰��
//      pattern���µ�ͼ��λͼ   
//����: true=�ɹ���false=ʧ��
//-----------------------------------------------------------------------------
bool_t gk_api_set_pat_buf(struct gkwin_rsc *gkwin,
                            struct st_rect_bitmap *pattern,
                            u32 sync_time)
{
    struct gksc_para_set_pat_buf para;
    if(NULL == gkwin)        
        return false; 
    para.gkwin = gkwin;
    para.pattern = *pattern;
    gk_syscall_chunnel(cn_gksc_set_pat_buf,sync_time,
                        &para,sizeof(para),NULL,0);
    return true;    
}
//----���ô���͸��ɫ-----------------------------------------------------------
//����: ���ô��ڵ�͸��ɫ��
//����: gkwin��Ŀ�괰��
//      transparentcolor��Ҫ���õ�͸��ɫ 
//����: true=�ɹ���false=ʧ��
//-----------------------------------------------------------------------------
bool_t gk_api_set_transparentcolor(struct gkwin_rsc *gkwin,
                                    	u32 transparentcolor)
{
    struct gksc_para_set_transparentcolor para;
    if(NULL == gkwin)           
        return false;
    if(transparentcolor == gkwin->transparentcolor)
        return true;
    para.gkwin = gkwin;
    para.transparentcolor = transparentcolor;
    gk_syscall_chunnel(cn_gksc_set_transparentcolor,cn_timeout_forever,
                        &para,sizeof(para),NULL,0);
    return true;    
}
//----���ô�������λͼ---------------------------------------------------------
//����: ���ô�������λͼ��
//����: gkwin��Ŀ�괰��
//      bitmsk�����õ�����λͼ
//����: true=�ɹ���false=ʧ��
//-----------------------------------------------------------------------------
bool_t gk_api_set_bitmsk(struct gkwin_rsc *gkwin,
                            	struct st_rect_bitmap *bitmsk,u32 sync_time)
{
    struct gksc_para_set_bitmsk para;
    if(NULL == gkwin)          
        return false;
	para.gkwin = gkwin;
	para.bitmsk = *bitmsk;
	gk_syscall_chunnel(cn_gksc_set_bitmsk,sync_time,
                        &para,sizeof(para),NULL,0);
        return true;
}

//----���ô���ֱ��д������-----------------------------------------------------
//����: ���ô���ֱ��д�����ԡ�
//����: gkwin��Ŀ�괰��
//����: ��
//-----------------------------------------------------------------------------
void gk_api_set_direct_screen(struct gkwin_rsc *gkwin,u32 sync_time)
{
    struct gksc_para_set_direct_screen para;
    if(NULL == gkwin)
        return;
    para.gkwin = gkwin;
    gk_syscall_chunnel(cn_gksc_set_direct_screen,sync_time,
                        &para,sizeof(para),NULL,0);
    return;
}
//----��������ֱ��д������-----------------------------------------------------
//����: ��������ֱ��д�����ԡ�
//����: gkwin��Ŀ�괰��
//����: ��
//-----------------------------------------------------------------------------
void gk_api_unset_direct_screen(struct gkwin_rsc *gkwin,u32 sync_time)
{
    struct gksc_para_unset_direct_screen para;
    if(NULL == gkwin)
        return;
    para.gkwin = gkwin;
    gk_syscall_chunnel(cn_gksc_unset_direct_screen,cn_timeout_forever,
                        &para,sizeof(para),NULL,0);
    return;
}
//----���ٴ���-----------------------------------------------------------------
//����: �ͷŴ�����ռ�õ���Դ��
//����: gkwin��Ŀ�괰��
//����: ��
//-----------------------------------------------------------------------------
void gk_api_destroy_win(struct gkwin_rsc *gkwin)
{
    struct gksc_para_destroy_win para;
    if(NULL == gkwin)           
        return;        
    para.gkwin = gkwin;
    gk_syscall_chunnel(cn_gksc_destroy_win,cn_timeout_forever,
                        &para,sizeof(para),NULL,0);
    return;
}

//----��ѯ�Կ���ʽ-------------------------------------------------------------
//����: ��ѯ�Կ�ʹ�õ���ɫ��ʽ����λͼʱ�����ʹ�ø��Կ���ͬ����ɫ��ʽ�������
//      ��������
//����: display������ѯ���Կ�
//����: ��ɫ��ʽ
//-----------------------------------------------------------------------------
u16 gk_api_get_pixel_format(struct display_rsc *display)
{
    if(display != NULL)
        return display->pixel_format;
    else
        return 0;
}

#endif //   for #if (cfg_gui_kernel == 1)

