//----------------------------------------------------
//Copyright (C), 2004-2009,  luoshitian.
//��Ȩ���� (C), 2004-2009,   ������.
//����ģ��: gui
//����:  ������.
//�汾��V1.0.0
//�ļ�����: gui kernel���岿��
//����˵��:
//�޶���ʷ:
//2. ����: 2011-07-18
//   ����:  ������.
//   �°汾��: V1.1.0
//   �޸�˵��: �������ݽṹʵ���Ѿ���Ϊ���ϲ��ṩ�������ļ�һֱû�ģ��޸�֮��
//             ����__gk_vmalloc��4��������ʹ���Դ����������Ҫ��ʱ��driver���Բ�
//             �ṩ�����ڴ�ĺ�����
//1. ����: 2009-11-21
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include "os_inc.h"

#if (cfg_gui_kernel == 1)

#include "gk_inc.h"
#include "gk_api.h"
#include "gk_clip.h"
#include <math.h>
#include <string.h>

struct gkwin_rsc   *pg_focus_win;//��ʾ���㴰��,���е���ʾ���Ƕ�������ڵĲ���
static struct rsc_node tg_display_root_rsc;
static struct rsc_node tg_window_root_rsc;
u32 u32g_default_back_color,u32g_default_color;
u16 u16g_gk_server_evtt,u16g_refresh_evtt;
struct gk_chunnel tg_gk_chunnel;

extern struct mem_cell_pool *pg_clip_rect_pool; //from gk_clip.c

//����������
#define cn_clip_init_num    100
struct clip_rect tg_clip_rect[cn_clip_init_num];
struct gkwin_rsc *pg_z_topmost;

struct semaphore_LCB *pg_usercall_semp;
//������÷�ϣ��gui kernel��������ٷ��أ�ʹ������ź���
struct semaphore_LCB *pg_syscall_semp;
static struct mutex_LCB *pg_gk_server_sync;

//gkwin����Դ���ֵ�λ����z�����ϵ:
// 1������Դ���У������Ӵ��ڰ�z�����У���ǰ�˵��Ǹ�����ָ��ĵ�һ����㡣
// 2��z_prio��ԱӰ��win�����ֵܽ���е�λ�ã�ԽСԽǰ��
// 3��z_prioһ����Ϊ0��<=0���ڸǸ����ڣ���>0�򱻸������ڸ�


//----��ʼ��guiģ��---------------------------------------------------------
//����: ��ʼ��guiģ�飬����Դ���������ӱ�Ҫ����Դ���
//����: ģ���ʼ��������û�в���
//����: 1=�ɹ�
//-----------------------------------------------------------------------------
ptu32_t module_init_gk(ptu32_t para)
{
    rsc_add_tree_root(&tg_display_root_rsc,sizeof(struct rsc_node),"display");
    rsc_add_tree_root(&tg_window_root_rsc,sizeof(struct rsc_node),"gkwindow");

    module_init_char_encoding(0);
    module_init_font(0);
    
    ring_init(&tg_gk_chunnel.ring_syscall,
                        tg_gk_chunnel.syscall_buf,cfg_gk_down_chunnel_deep);
    ring_init(&tg_gk_chunnel.ring_usercall,
                        tg_gk_chunnel.usercall_buf,cfg_gk_up_chunnel_deep);
     
    tg_gk_chunnel.syscall_mutex = mutex_create("gui chunnel to gk mutex");
    tg_gk_chunnel.syscall_semp = semp_create(1,0,"gui chunnel to gk semp");
    tg_gk_chunnel.usercall_semp = semp_create(1,0,"gui chunnel from gk semp");
    pg_gk_server_sync = mutex_create("gk server sync");
//    pg_gk_server_sync = semp_create(1,0,"gk server sync");
    pg_usercall_semp = semp_create(1,0,"gk wait repaint");
    pg_syscall_semp = semp_create(1,0,"gk wait job");
    u16g_gk_server_evtt = djy_evtt_regist(enum_correlative,1,0,10,gk_server,
                                    8120,"gui kernel server");
    pg_clip_rect_pool = mb_create_pool(&tg_clip_rect,
                                  cn_clip_init_num,
                                  sizeof(struct clip_rect),
                                  100,2000, "clip area");

    if(    (pg_clip_rect_pool == NULL)
        || (u16g_gk_server_evtt == cn_invalid_evtt_id)
        || (pg_syscall_semp == NULL)
        || (pg_usercall_semp == NULL)
        || (pg_gk_server_sync == NULL)
        || (tg_gk_chunnel.usercall_semp == NULL)
        || (tg_gk_chunnel.syscall_semp == NULL)
        || (tg_gk_chunnel.syscall_mutex == NULL) )
    {
        goto exit_error;
    }


    semp_set_sync_sort(pg_syscall_semp,sort_time);
    djy_event_pop(u16g_gk_server_evtt,enum_para_async,NULL,0,NULL,0,0,0);


    return 1;

exit_error:
    mb_delete_pool(pg_clip_rect_pool);
    djy_evtt_unregist(u16g_gk_server_evtt);
    semp_delete(pg_syscall_semp);
    semp_delete(pg_usercall_semp);
//    semp_delete(pg_gk_server_sync);
    mutex_delete(pg_gk_server_sync);
    semp_delete(tg_gk_chunnel.usercall_semp);
    semp_delete(tg_gk_chunnel.syscall_semp);
    mutex_delete(tg_gk_chunnel.syscall_mutex);
    
    rsc_del_node(&tg_display_root_rsc);
    rsc_del_node(&tg_window_root_rsc);
    return 0;
}

u32 __gk_cal_buf_size(u32 xsize,u32 ysize,u32 *linesize,u16 pf_type)
{
    u32 size;
    u8 color_bits;
    color_bits = pf_type >>8;
    switch(color_bits)
    {
        case 1:
        {
            *linesize = xsize/8;
            if(xsize%8)
                (*linesize)++;
            size = *linesize*ysize;
        }break;
        case 2:
        {
            *linesize = xsize/4;
            if(xsize%4)
                (*linesize)++;
            size = *linesize*ysize;
        }break;
        case 4:
        {
            *linesize = xsize/2;
            if(xsize%2)
                (*linesize)++;
            size = *linesize*ysize;
        }break;
        case 8:
        {
            *linesize = xsize;
            size = xsize * ysize;
        }break;
        case 16:
        {
            *linesize = 2*xsize;
            size = xsize*ysize*2;           
        }break;
        case 24:
        {
            *linesize = 3*xsize;
            size = xsize*ysize*3;
        }break;
        case 32:
        {
            *linesize = 4*xsize;
            size = xsize*ysize*4;
        }break;
        default:
            size = 0;
            *linesize = 0;
            break;
    }
    return size;

}
//----���䴰�ڻ���-------------------------------------------------------------
//����: ���䴰�ڻ��棬������1�����������bitmap,2������ô�����rop4���ԣ���Ҫ��
//      ��bitmsk_buf��3��pattern buffer��
//      ��Ȼ������������Ҫ���������֣���ʵ�����Ǻϲ�һ��Ӷ������룬Ȼ�����ֹ�
//      �ָ�ɼ����������ġ��������������byte��ָ�룬�����뵣�Ķ������⡣
//����: buf�����ڷ��ؽ���Ľṹָ��
//      xsize��ysize����ʾ����ߴ�
//      rop_code�����ڵ�rop_code���룬Ӱ���Ƿ����buf�е�rop4_msk��Ա��
//����: true=�ɹ���false=ʧ��
//ע: 2416��2D���ٲ�֧��rop4�������ж�rop_code��ֻ�����pat��vbuf����
//-----------------------------------------------------------------------------
bool_t __gk_vmalloc(struct display_rsc *disp,struct bmbuf_info *buf,
                    u32 xsize,u32 ysize,u32 rop_code)
{
    u8 *mem;
    u32 buf_size,linesize,pat_size,rop4_size;
    bool_t result;
    //��Щ�Կ��Է��䴰�ڻ���������Ҫ��������Կ��ṩ��vmalloc��������д�Կ���
    //������ɲο���������ʵ�ַ�����
    //�����Կ��Ի���������Ҫ���ԭ�򣬰���(������):
    //	1���м���������Կ����������Ҫ�����Կ���ͼ�μ��ټ��㣬��������Ի���ĵ�
    //   ַ������߽���ܻ��������ơ�
    // 2����Щϵͳר�Ż����˻�������򣬷��仺������ڸ������ڡ�
    //��������Ҫ������driver�а�vmalloc��ΪNULL�����ɱ�����ʵ���Դ����
    if(disp->vmalloc != NULL)
    {
        result = disp->vmalloc(buf,xsize,ysize,rop_code);
    }else
    {
        buf_size = __gk_cal_buf_size(xsize,ysize,&linesize,buf->vbuf.pf_type);
        buf_size = align_up_sys(buf_size);
        //rop_code�а���4Ԫ��դ�����������rop4_msk
        if(( (rop_code & (cn_rop_ext_en + cn_rop_rop4_en)) 
                        ==(cn_rop_ext_en + cn_rop_rop4_en)) )
        {
            rop4_size = (xsize+7)/8 * ysize;
            rop4_size = align_up_sys(rop4_size);
        }else
            rop4_size = 0;
        pat_size = disp->patx * (disp->pixel_format >>8)>>3; //����1��ռ�����ֽ�
        pat_size = pat_size * disp->paty;
        mem = m_malloc(buf_size + rop4_size + pat_size,0);
        if(mem != NULL)
        {
            buf->vbuf.bm_bits = mem;
            buf->vbuf.width = xsize;
            buf->vbuf.height = ysize;
            buf->vbuf.linebytes = linesize;
            
            buf->rop4_msk.bm_bits = mem + buf_size;
            buf->rop4_msk.width = xsize;
            buf->rop4_msk.height = ysize;
            buf->rop4_msk.linebytes = (xsize+7)/8;
            
            buf->pat = mem + buf_size + rop4_size;
            result = true;
        }else
        {
            buf->vbuf.bm_bits = NULL;
            buf->rop4_msk.bm_bits = NULL;
            buf->pat = NULL;
            buf->vbuf.width = 0;
            buf->vbuf.height = 0;
            buf->vbuf.linebytes = 0;
            result = false;
        }
    }
    return result;
}

//----���·��䴰�ڻ���---------------------------------------------------------
//����: �����ڳߴ�ı䣬���ñ��������·��仺�档�߼��ϣ��������Ĺ���="vfree-
//      ����vmalloc"����ϣ�������һ����������Ϊ�����Ч�ʡ���Ϊ�ڴ��ǰ������
//      �ģ��ڴ�������൱��ʱ�Ĺ�����vmalloc����Ĵ��ڻ��棬���ܱ�ʵ�ʵĴ󣬵�
//      ���ڳߴ��������Ļ��������������·��䡣���û�ͨ���϶����ı䴰�ڳߴ�ʱ��
//      ʵ������һ��һ��ı�ģ�ÿ�ζ����·���Ļ���ϵͳ�����ܴ����ײ�����Ƭ��
//      vrmalloc�����ȼ��buf�еĻ������Ƿ���Ȼ�������������㣬�����·��䡣
//      �����ܻ����xsize��ysize����ָ�롣
//����: buf��ԭ���ڻ���ָ�룬Ҳ���ڷ��ؽ��
//      xsize��ysize����ʾ����ߴ�
//      rop_code�����ڵ�rop_code����
//����: true=�ɹ���false=ʧ��
//-----------------------------------------------------------------------------
bool_t __gk_vrmalloc(struct display_rsc *disp,struct bmbuf_info *buf,
                    u32 xsize,u32 ysize,u32 rop_code)
{
    u8 *mem;
    u32 buf_size,linesize,pat_size,oldsize,newsize,rop4_size;
    bool_t remalloc = true,result;
    //���������lcd_vmalloc������Ӧ���ֵ�ע��
    if(disp->vrmalloc != NULL)
    {
        result = disp->vrmalloc(buf,xsize,ysize,rop_code);
    }else
    {
        buf_size = __gk_cal_buf_size(xsize,ysize,&linesize,buf->vbuf.pf_type);
        buf_size = align_up_sys(buf_size);
        //rop_code�а���4Ԫ��դ�����������rop4_msk
        if( ( (rop_code & (cn_rop_ext_en + cn_rop_rop4_en)) 
                        ==(cn_rop_ext_en + cn_rop_rop4_en)) )
        {
            rop4_size = (xsize+7)/8 * ysize;
            rop4_size = align_up_sys(rop4_size);
        }else
            rop4_size = 0;
        pat_size = disp->patx * (disp->pixel_format >>8)>>3; //����1��ռ�����ֽ�
        pat_size = pat_size * disp->paty;
        newsize = buf_size + rop4_size + pat_size;

        oldsize = m_check_size(buf->vbuf.bm_bits); //��ѯԭָ��ʵ�ʷ�����ڴ�
        if(newsize <= oldsize)
        {
            //�µ��Դ�������ԭʵ�ʳߴ������1/8���ڵĻ���С����С�ڴ����ߴ�ģ�
            //�����·����Դ�
            if(((oldsize - newsize) <= (oldsize>>3)) 
                    || ((oldsize - newsize) <= m_get_page_size() ))
                remalloc = false;       //�������·���
        }
        if(remalloc)
        {
            mem = m_malloc(newsize,0);
            if(mem != NULL)
            {
                m_free(buf->vbuf.bm_bits);
                buf->vbuf.bm_bits = mem;
                buf->vbuf.width = xsize;
                buf->vbuf.height = ysize;
                buf->vbuf.linebytes = linesize;
                
                buf->rop4_msk.bm_bits = mem + buf_size;
                buf->rop4_msk.width = xsize;
                buf->rop4_msk.height = ysize;
                buf->rop4_msk.linebytes = (xsize+7)/8;
                
                buf->pat = mem + buf_size + rop4_size;
                result = true;
            }else
            {
                result = false;   //�·����ڴ�ʧ�ܣ�����ԭ�����䣬���ͷ�ԭ�ڴ档
            }
        }else       //�������·���
        {
            buf->vbuf.width = xsize;
            buf->vbuf.height = ysize;
            buf->vbuf.linebytes = linesize;
            
            buf->rop4_msk.bm_bits = buf->vbuf.bm_bits + buf_size;
            buf->rop4_msk.width = xsize;
            buf->rop4_msk.height = ysize;
            buf->rop4_msk.linebytes = (xsize+7)/8;
            
            buf->pat = buf->vbuf.bm_bits + buf_size + rop4_size;
            result = true;
        }
    }
    return result;
}

//----�ͷŴ��ڻ���-------------------------------------------------------------
//����: �ͷ�һ�����ڵĻ��档
//����: buf�����ڷ��ؽ���Ľṹָ��
//����: ��
//-----------------------------------------------------------------------------
void __gk_vfree(struct display_rsc *disp,struct bmbuf_info *buf)
{
    if(disp->vfree != NULL)
    {
        disp->vfree(buf);
    }else
    {
        m_free(buf->vbuf.bm_bits);  //�����ڴ�ʱ���Դ��rop4_msk��pattern��ͬʱ
                                    //����ģ���ֻ�����m_freeһ�Σ����߾��ͷ�
    }
}
//----ȡz��Ƭ�ε���ʼ����(���ڼ����Ӵ���)--------------------------------------
//����: ÿһ�����ڼ����Ӵ��ڣ�����z����ռ��������һ�Σ�������ȡ�öε���ʼ����(
//      ���˵Ĵ���)��
//����: gkwin������ָ��
//����: ��ʼ����ָ��
//-----------------------------------------------------------------------------
struct gkwin_rsc *__gk_get_zsection_start(struct gkwin_rsc *gkwin)
{
    struct gkwin_rsc *temp,*result;
    result = gkwin;
    temp = result;
    while(1)
    {
        temp = (struct gkwin_rsc *)rsc_get_son(&temp->node);
        if(temp == NULL)        //result�Ѿ�û���Ӵ�����
            break;
        temp = (struct gkwin_rsc *)temp->node.previous;
        if(temp ->z_prio <= 0 )  //�����Ӵ��ھ���resultǰ��
            break;
        else
        {
            result = temp;
        }
    }
    return result;
}
//----ȡz��Ƭ�εĽ�������(���ڼ����Ӵ���)--------------------------------------
//����: ÿһ�����ڼ����Ӵ��ڣ�����z����ռ��������һ�Σ�������ȡ�öεĽ�������(
//      ��ǰ�˵Ĵ���)��
//����: gkwin������ָ��
//����: ��������ָ��
//-----------------------------------------------------------------------------
struct gkwin_rsc *__gk_get_zsection_end(struct gkwin_rsc *gkwin)
{
    struct gkwin_rsc *temp,*result;
    result = gkwin;
    temp = result;
    while(1)
    {
        temp = (struct gkwin_rsc *)rsc_get_son(&temp->node);
        if(temp == NULL)        //result�Ѿ�û���Ӵ�����
            break;
        else if(temp ->z_prio > 0 )  //�����Ӵ��ھ���result���
            break;
        else
        {
            result = temp;
        }
    }
    return result;
}
//----ȡz��Ƭ�ε���ʼ����(���ڼ������ﴰ��)------------------------------------
//����: ÿһ�����ڼ������ﴰ�ڣ�����z����ռ��������һ�Σ�������ȡ�öε���ʼ����(
//      ���˵Ĵ���)��
//����: gkwin������ָ��
//����: ��ʼ����ָ��
//-----------------------------------------------------------------------------
struct gkwin_rsc *__gk_get_zsection_all_start(struct gkwin_rsc *gkwin)
{
    struct gkwin_rsc *temp,*result;
    result = gkwin;
    temp = result;
    while(1)
    {
        temp = (struct gkwin_rsc *)rsc_get_son(&temp->node);
        if(temp == NULL)        //result�Ѿ�û���Ӵ�����
            break;
        temp = (struct gkwin_rsc *)rsc_get_next(&temp->node);
        if(temp ->z_prio <= 0 )  //�����Ӵ��ھ���resultǰ��
            break;
        else
        {
            result = temp;
        }
    }
    return result;
}

//----ȡz��Ƭ�εĽ�������(���ڼ����Ӵ���)--------------------------------------
//����: ÿһ�����ڼ������ﴰ�ڣ�����z����ռ��������һ�Σ�������ȡ�öεĽ�������(
//      ��ǰ�˵Ĵ���)��
//����: gkwin������ָ��
//����: ��������ָ��
//-----------------------------------------------------------------------------
struct gkwin_rsc *__gk_get_zsection_all_end(struct gkwin_rsc *gkwin)
{
    struct gkwin_rsc *temp,*result;
    result = gkwin;
    temp = result;
    while(1)
    {
        temp = (struct gkwin_rsc *)rsc_get_son(&temp->node);
        if(temp == NULL)        //result�Ѿ�û���Ӵ�����
            break;
        else if(temp ->z_prio > 0 )  //�����Ӵ��ھ���result���
            break;
        else
        {
            result = temp;
        }
    }
    return result;
}


//----��������-----------------------------------------------------------------
//����: ������������ʾ���ĵ�һ�����ڣ����е���ʾ���������ȴ���������ܴ������ڣ�
//      ����ĳߴ粻��С����ʾ���ĳߴ磬�����Դ�����ʾ���ߴ硣������ˢ��screen
//      �Ĺ����к���ͨ�����ǵ�ͬ�ġ�
//����: para���������ݽṹ�����Ա������ṹ���崦��
//����: �´��ھ��(ָ��)
//-----------------------------------------------------------------------------
struct gkwin_rsc *gk_create_desktop(struct gksc_para_create_desktop *para)
{
    struct gkwin_rsc *desktop;
    struct display_rsc *display;
    struct rsc_node *rsc;
    struct clip_rect *clip;
    u32 msk_size;
    s32 desktop_x,desktop_y;
    struct bmbuf_info buf_info;
    struct gksc_para_fill_win para_fill;
    if(para->display == NULL)
    {
        return NULL;
    }
    display = para->display;
    if(para->width >= display->width)
        desktop_x = para->width;            //������Դ�����ʾ���ߴ�
    else
        desktop_x = display->width;   //���治��С����ʾ���ߴ�
    if(para->height >= display->height)
        desktop_y = para->height;
    else
        desktop_y = display->height;
    if((display->frame_buffer == NULL) && (para->buf_mode == true))
    {
        return NULL;        //��֡�������ʾ��������������������
    }
    desktop = para->desktop;    //para->desktop�ɵ������ṩ�ڴ棬��ָ�����
    if(para->buf_mode)      //����ģʽ��Ϊÿ������׼��������
    {
        buf_info.vbuf.pf_type = display->pixel_format;
        if( __gk_vmalloc(display,&buf_info,
                        desktop_x,desktop_y,cn_r3_srccopy)) //�����Դ�
        {
            msk_size = ((desktop_x +7)/8+7)/8*((desktop_y +7)/8);
            //����frame bufferʱ����Ҫchanged_msk
            desktop->changed_msk.bm_bits = m_malloc_gbl(msk_size,0);
            if(desktop->changed_msk.bm_bits == NULL)   //�����޸������ڴ�ʧ��
            {
                __gk_vfree(display,&buf_info);
                djy_error_login(enum_gk_no_memory,"�Դ治��");
                return NULL;
            }else
            {///�����޸������ڴ�ɹ������Դ��ٷ���
                memset(desktop->changed_msk.bm_bits,0,msk_size);
                desktop->changed_msk.pf_type = cn_sys_pf_gray1;
                desktop->changed_msk.height = desktop_y/8;
                desktop->changed_msk.width = desktop_x/8;
                desktop->changed_msk.linebytes = ((desktop_x+7)/8+7)/8;
                desktop->wm_bitmap = buf_info.vbuf;
                desktop->bitmsk = buf_info.rop4_msk;
                desktop->pat_buf = buf_info.pat;
            }
        }
        else               //�����Դ�ʧ�ܣ�
        {
            djy_error_login(enum_gk_no_memory,"�Դ治��");
            return NULL;
        }
    }
    else                   //�ǻ���ģʽ������û�л�����
    {
        desktop->wm_bitmap.bm_bits = NULL;
        desktop->wm_bitmap.height = desktop_y;
        desktop->wm_bitmap.width = desktop_x;
        desktop->pat_buf = NULL;
    }
    //gui kernel�ǵ��߳����еģ����赣��mb_query_free��mb_malloc֮ǰ�ᱻ�����߳�
    //��ռ��
    if(mb_query_free(pg_clip_rect_pool) < 2)
    {
        __gk_vfree(display,&buf_info);
        return NULL;
    }
    else
    {
        //��������򣬿������С����ʾ���ߴ��Сһ��
        clip = (struct clip_rect*)mb_malloc(pg_clip_rect_pool,0);
        clip->next = clip;
        clip->previous = clip;
        clip->rect.left = 0;
        clip->rect.top = 0;
        clip->rect.right = display->width;
        clip->rect.bottom = display->height;
        desktop->visible_clip = clip;
        desktop->visible_bak = NULL;
    }
    //�����ʼ�����棬����Ҳ�ǰ����ڹ����
    desktop->z_back = desktop;
    desktop->z_top = desktop;
    strcpy(desktop->win_name,"desktop");
    desktop->redraw_clip = NULL;
    desktop->changed_clip = NULL;
    desktop->disp = display;        //������ʾ��
//    desktop->locked = false;            //���治����
//    desktop->hided = false;            //���治����
    desktop->z_prio = 0;                //��������ȼ�
    desktop->transparentcolor = 0;        //��������������Ч
    desktop->rop_code = cn_r3_srccopy;
    desktop->dest_blend = false;        //��������������Ч
//    desktop->bitmsk.bm_bits = NULL;
//    desktop->pat_buf = NULL;
    desktop->change_flag = cn_gkwin_change_none; //���������ʾ
    desktop->absx0 = 0;
    desktop->absy0 = 0;
    desktop->bound_limit = true;
    desktop->left = 0;
    desktop->top = 0;
    desktop->right = desktop_x;
    desktop->bottom = desktop_y;
    desktop->limit_left = 0;
    desktop->limit_top = 0;
    desktop->limit_right = display->width;
    desktop->limit_bottom = display->height;

    display->z_topmost = desktop;
    display->desktop = desktop;
    rsc = rsc_search_tree("gkwindow");
    rsc_add_son(rsc,&desktop->node,sizeof(struct gkwin_rsc),desktop->win_name);

    //�ø�������ɫ�������
    para_fill.gkwin = desktop;
    para_fill.color = para->color;
    gk_fill_win(&para_fill);
    
    //�����´���ָ��
    return desktop;
}

//----��������-----------------------------------------------------------------
//����: ����һ�����ڣ��´��ڵ����ȼ�Ϊ0��λ���������ȼ�=0�Ĵ��ڵ���ǰ�ˡ�
//����: para���������ݽṹ�����Ա������ṹ���崦��
//����: �´��ھ��(ָ��)
//-----------------------------------------------------------------------------
struct gkwin_rsc *gk_create_win(struct gksc_para_create_gkwin *para)
{
    u32 size;
    s32 width,height;
    struct gkwin_rsc *target_section;  
    struct gkwin_rsc *parent;                       //gkwin�ĸ�����
    struct display_rsc *display;
    //z���б��ƶ��Ĵ��ڶ����˵Ĵ���
    struct gkwin_rsc *move_start;
    //z���б��ƶ��Ĵ��ڶ���ǰ�˵Ĵ���
    struct gkwin_rsc *move_end;      
    struct gkwin_rsc *gkwin;
    struct bmbuf_info buf_info;
    struct gksc_para_fill_win para_fill;
    
    width = para->right - para->left;
    height = para->bottom - para->top;
    display = para->parent_gkwin->disp;
    gkwin = para->gkwin;    //para->gkwin�ɵ������ṩ�ڴ棬��ָ�����
    parent = para->parent_gkwin; 
    if((parent == NULL)||(width < 0) || (height < 0))
    {
        return NULL;
    }
    if((display->frame_buffer == NULL) && (para->buf_mode == true))
    {
        return NULL;        //��֡�������ʾ���������������崰��
    }
    if(mb_query_free(pg_clip_rect_pool) < 2)
    {
        return NULL;
    }
    
    *gkwin = *parent;             //�´��ڿ��������ڵ�����
    if(para->buf_mode)      //����ģʽ��Ϊ����׼��������
    {
        buf_info.vbuf.pf_type = display->pixel_format;
        if( __gk_vmalloc(display,&buf_info,
                        para->right-para->left,
                        para->bottom-para->top,para->rop_code)) //�����Դ�
        {
            size = (((width +7)/8+7)/8 *((height +7)/8));
            //����frame bufferʱ����Ҫchanged_msk
            gkwin->changed_msk.bm_bits = m_malloc_gbl(size,0);
            if(gkwin->changed_msk.bm_bits == NULL)   //�����޸������ڴ�ʧ��
            {
                __gk_vfree(display,&buf_info);
                djy_error_login(enum_gk_no_memory,"�Դ治��");
                return NULL;
            }
            else
            {
                memset(gkwin->changed_msk.bm_bits,0,size);
                gkwin->changed_msk.pf_type = cn_sys_pf_gray1;
                gkwin->changed_msk.height = height/8;
                gkwin->changed_msk.width = width/8;
                gkwin->changed_msk.linebytes = ((width+7)/8+7)/8;
                gkwin->wm_bitmap = buf_info.vbuf;
                gkwin->bitmsk = buf_info.rop4_msk;
                gkwin->pat_buf = buf_info.pat;
            }
        }
        else               //�����Դ�ʧ�ܣ�
        {
            djy_error_login(enum_gk_no_memory,"�Դ治��");
            return NULL;
        }
    }
    else                   //�ǻ���ģʽ������û�л�����
    {
        gkwin->wm_bitmap.bm_bits = NULL;
        gkwin->wm_bitmap.height = height;
        gkwin->wm_bitmap.width =width;
        gkwin->pat_buf = NULL;
    }

    size = strnlen(para->name,cn_gkwin_name_limit);
    if(size == cn_limit_uint32)         //���ֳ��ȳ�������
    {
        size = cn_gkwin_name_limit;     //ǿ���е���������
    }
    memcpy(gkwin->win_name,para->name,size);
    gkwin->win_name[size] = '\0';       //��������\0��

    //�������´��ڵ�����������丸���ڵ�
    gkwin->left = para->left;
    gkwin->top = para->top;
    gkwin->right = para->right;
    gkwin->bottom = para->bottom;
    //���ڵľ���λ�ã����������screen������ԭ��
    gkwin->absx0 = para->left + para->parent_gkwin->absx0;
    gkwin->absy0 = para->top + para->parent_gkwin->absy0;
    gkwin->z_prio = 0;                //�½�����Ĭ��Ϊ0
    display->reset_clip = false;
    
    gkwin->dest_blend = gk_rop_needsno_destination(para->rop_code);
    gkwin->rop_code = para->rop_code;
    //���°��´������ӵ���Դ�����У������뵽z����
    if(rsc_get_son(&para->parent_gkwin->node) == NULL)  //���������Ӵ���
    {
        rsc_add_son(&parent->node,&gkwin->node,
                     sizeof(struct gkwin_rsc),gkwin->win_name);
        //����4����z���а�win���븸�ڵ��ǰ��
        gkwin->z_back = parent;
        gkwin->z_top = parent->z_top;
        parent->z_top->z_back = gkwin;
        parent->z_top = gkwin;
        if(display->z_topmost == parent)
            display->z_topmost = gkwin;
    }
    else
    {//���������Ӵ���
        //ȡz���б��ƶ�����gkwinͬ���Ĵ��ڶο�ʼ�ͽ������ڶ�
        move_end = (struct gkwin_rsc*)rsc_get_son(&parent->node);
        move_start = (struct gkwin_rsc*)rsc_get_previous(&move_end->node);
        target_section = move_end;
        while(1)
        {//ɨ��ͬ�������к�Ŀ�괰�����ȼ���ͬ�Ĵ��ڣ�ɨ����z��Ϊ����   
            //ɨ��˳��:��z���ǰ�����˽���ɨ�裬ֻɨ�ֵܴ��ڣ���ɨ�Ӵ���
            if(target_section->z_prio >= 0)
                break;
            //ɨ��Ŀ�괰������z������һ���ڣ���ֹ����
            if(target_section == move_start)
                break;
            target_section = (struct gkwin_rsc*)rsc_get_next(&target_section->node);
        }
        if(target_section->z_prio == 0)
        {//ͬ�������д��ں�gkwin��prio��ȵĴ���
            rsc_add_node(&target_section->node,&gkwin->node,
                        sizeof(struct gkwin_rsc),gkwin->win_name);
            if(rsc_is_head(&target_section->node))
                rsc_round_next(&parent->node);
            //��ȡtarget_section�����Ӵ�������z��εĽ�������(��ǰ��)
            //�´��ڲ��뵽����ǰ��
            move_end = __gk_get_zsection_all_end(target_section);
            if(move_end == display->z_topmost)
                display->z_topmost = gkwin;
            //�嵽Ŀ�괰��ǰ��
            gkwin->z_back = move_end;
            gkwin->z_top = move_end->z_top;
            move_end->z_top->z_back = gkwin;
            move_end->z_top = gkwin;        
        }
        else //�ֵܴ�����û�����ȼ�=0�Ĵ���
        {   
            //ͬ�������������ȼ�������0�ģ���Դ�������´��ڲ�����ǰ��
            if(target_section->z_prio > 0)
            {
                rsc_add_node(&target_section->node,&gkwin->node,
                            sizeof(struct gkwin_rsc),gkwin->win_name);
            }
            else    //ͬ���������ȼ��������´���(prio = 0)
            {
                //�´�������Դ�����д���ͬ����������
                rsc_insert_node(&target_section->node,&gkwin->node,
                            sizeof(struct gkwin_rsc),gkwin->win_name);
            }
            //�´����Ĵ���gkwin���ȼ�Ϊ0������ͬ���������ȼ�����ηֲ�����
            //���ڿ϶��ڸ�����ǰ��
            if(parent == display->z_topmost)
                display->z_topmost = para->gkwin;
            //�嵽Ŀ�괰��ǰ��
            gkwin->z_back = parent;
            gkwin->z_top = parent->z_top;
            parent->z_top->z_back = gkwin;
            parent->z_top = gkwin;        
        }
    }
    //���ô��ڵĿɼ��߽磬���ܴ��ڻ����ڵ�Ӱ�죬����������Դ���к���ܵ���
    __gk_set_bound(gkwin);

    gkwin->visible_clip = NULL;    
    gkwin->visible_bak = NULL; 
	__gk_scan_new_visible_clip(display);

    para_fill.gkwin = gkwin;
    para_fill.color = para->color;
    gk_fill_win(&para_fill);
    return gkwin;
}

//----���ڸ���-----------------------------------------------------------------
//����: �޸Ĵ������֣��������127�ַ����������ֽ��ضϡ�
//����: gcwin�����޸ĵĴ��ھ��(ָ��)
//      name���µ�����
//����: ��
//-----------------------------------------------------------------------------
void gk_set_name(struct gkwin_rsc *gkwin,char *name)
{
    s32 size;
    size = strnlen(name,cn_gkwin_name_limit);
    if(size == cn_limit_uint32)     //���ֳ��ȳ�������
        size = cn_gkwin_name_limit;   //ǿ���е���������
    memcpy(gkwin->win_name,name,size);//copy���֣�����ܳ�������δcopy��������\0��
    gkwin->win_name[size] = '\0';     //�����(�ӽ�����)
}

//----��������-----------------------------------------------------------------
//����: ����������ڣ���Ҫ��������������޸Ĺ��Ĳ��֡�����״̬�£�������ʾ����Ļ
//      �ϣ��������ݲ���ı䡣
//����: gcwin��Ŀ�괰��
//����: ��
//-----------------------------------------------------------------------------
//void gk_set_unlock(struct gkwin_rsc *gkwin)
//{
//    if(gkwin == gkwin->disp->desktop)   //���治������
//        return;
//    if(gkwin->locked == false)      //������û���������޲���
//        return;
//    gkwin->locked = false;
//    if(gkwin->hided == false)       //�����ڿ��ӣ���Ҫ����ɨ�������
//    {
//        gkwin->disp->reset_clip = true;
//    }
//    return;
//}

//----��������-----------------------------------------------------------------
//����: �������ڣ���Ҫ��������������޸Ĺ��Ĳ���
//����: gcwin��Ŀ�괰��
//����: ��
//-----------------------------------------------------------------------------
//void gk_set_lock(struct gkwin_rsc *gkwin)
//{
//    if(gkwin == gkwin->disp->desktop)   //���治������
//        return;
//    if(gkwin->locked == true)       //���������������޲���
//        return;
//    gkwin->locked = true;
//}

//----���ش���-----------------------------------------------------------------
//����: �Ѵ�������Ϊ���أ�����������ɨ���������������������ڱ��Լ��ڸǲ��֡�
//      ��Ӱ���Ӵ��ڣ���Ҫͬʱ�����Ӵ��ڣ�Ӧ��gui windows�����
//����: gcwin��Ŀ�괰��
//����: ��
//-----------------------------------------------------------------------------
//void gk_set_hide(struct gkwin_rsc *gkwin)
//{
//    if(gkwin == gkwin->disp->desktop)   //���治������
//        return;
//    if(gkwin->hided == true)                    //ԭ���������صģ��޲���
//        return;
//    gkwin->hided = true;
//    gkwin->disp->reset_clip = true;
//}

//----��ʾ����-----------------------------------------------------------------
//����: �Ѵ�������Ϊ���ӣ�����������ɨ�������������������ڡ���Ӱ���Ӵ��ڣ�
//      ��Ҫͬʱ�����Ӵ��ڣ�Ӧ��gui windows�����
//����: gcwin��Ŀ�괰��
//����: ��
//-----------------------------------------------------------------------------
//void gk_set_show(struct gkwin_rsc *gkwin)
//{
//    if(gkwin == gkwin->disp->desktop)   //���治������
//        return;
//    if(gkwin->hided == false)   //ԭ�����ǿ��ӵģ��޲���
//        return;
//    gkwin->hided = false;
//    gkwin->disp->reset_clip = true;
// }
//----�ƶ�����-----------------------------------------------------------------
//����: �ı䴰���ڸ������ڵ����λ�ã������Ӵ��ڵ�����������ڸ����ڵģ����ƶ���
//      ��ʱ�����Ӵ���һ���ƶ���
//����: gcwin��Ŀ�괰��
//      left��top���µ����Ͻ����꣬����ڸ�����
//����: ��
//-----------------------------------------------------------------------------
void gk_move_win(struct gksc_para_move_win *para)
{
    u32 width,height;
    struct gkwin_rsc *current;
    struct display_rsc *disp;
    if((para->left == para->gkwin->left) && (para->top == para->gkwin->top))
        return;                                 //λ��û���޸ģ�ֱ�ӷ��ء�
    disp = para->gkwin->disp;
    if(para->gkwin == disp->desktop)
    {   //Ŀ�괰�������棬�ж�������:������븲����ʾ������
        s32 dtop,dleft;
        if(para->left > 0)
            dleft = 0;    //������߽���븲����ʾ��
        else if((para->gkwin->wm_bitmap.width+para->left) < disp->width) 
            //�ұ߽���븲����ʾ��   
            dleft = disp->width - para->gkwin->wm_bitmap.width;
        else
            dleft = para->left;
        if(para->top > 0)
            dtop = 0;     //�����ϱ߽���븲����ʾ��
        else if((para->gkwin->wm_bitmap.height+para->top) < disp->height)
            //�±߽���븲����ʾ��
            dtop =disp->height - para->gkwin->wm_bitmap.height;
        else
            dtop = para->top;
        if((dtop == para->gkwin->top) && (dleft == para->gkwin->left))
            return;         //����λ��ʵ����δ�����ı�
        else
        {
            para->gkwin->top = dtop;
            para->gkwin->left = dleft;
         }
        para->gkwin->limit_left = 0-para->left;
        para->gkwin->limit_top = 0-para->top;
        para->gkwin->limit_right = para->gkwin->limit_left + disp->width;
        para->gkwin->limit_bottom = para->gkwin->limit_top + disp->height;
    }else
    {
        para->gkwin->absx0 -= para->gkwin->left-para->left;//�޸Ĵ��ھ�������
        para->gkwin->absy0 -= para->gkwin->top-para->top;
        width = para->gkwin->right-para->gkwin->left;
        height = para->gkwin->bottom-para->gkwin->top;
        para->gkwin->left = para->left;                      //�޸Ĵ����������
        para->gkwin->top = para->top;
        para->gkwin->right = para->gkwin->left+width;
        para->gkwin->bottom = para->gkwin->top+height;
        __gk_set_bound(para->gkwin);                        //���ÿ���ʾ�߽�
    }
    current = para->gkwin;
    current->disp->reset_clip = true;
    current->change_flag = cn_gkwin_change_all;
    //����gkwin�������Ӵ���
    while(rsc_trave_scion(&para->gkwin->node,&current->node)) 
    {
        current->absx0 -= current->left -para->left;        //�޸Ĵ��ھ�������
        current->absy0 -= current->top -para->top;
        __gk_set_bound(current);
        para->gkwin->change_flag = cn_gkwin_change_all;
    }
}

//----���ÿ���ʾ�߽�-----------------------------------------------------------
//����: �κδ��ڣ������ʹ�����Ǳ�z��ǰ�˴��ڼ��У��䴰�ڿ���ʾ��ΧҲ�����޵ģ�
//      ��bound_limit=true�ġ�������������ȴ��ڵĿ���ʾ�߽����ơ�����λ�øı�
//      ����Ҫ���ÿ���ʾ�߽硣
//      gkwin�������õĴ���
//����: ��
//-----------------------------------------------------------------------------
void __gk_set_bound(struct gkwin_rsc *gkwin)
{
    struct gkwin_rsc *ancestor;
    s32 gkwin_absx,gkwin_absy,ancestor_absx,ancestor_absy;
    ancestor = gkwin;
    //��ȡ���ȴ����е�һ���߽��ܸ��������Ƶĵ�һ�����ڣ�gkwin�ı߽��ܸô����޶�
    while(ancestor->bound_limit == false)
    {
        ancestor = (struct gkwin_rsc*)rsc_get_parent(&ancestor->node);
    }
    ancestor = (struct gkwin_rsc*)rsc_get_parent(&ancestor->node);
    //����left��limit_left��������ڸ����ڵģ�����Ҫת���ɾ���������ܱȽ�
    gkwin_absx = gkwin->absx0;  //ȡ�����ڵľ������꣬Ҳ��gkwin->left�ľ�������
    gkwin_absy = gkwin->absy0;
    //ȡ�߽����޴��ڵĿɼ��߽�ԭ���������
    ancestor_absx = ancestor->absx0 +ancestor->limit_left - ancestor->left;
    ancestor_absy = ancestor->absy0 +ancestor->limit_top - ancestor->top;

    //
    if(gkwin_absx < ancestor_absx)
       gkwin->limit_left = gkwin->left + ancestor_absx - gkwin_absx;
    else
        gkwin->limit_left = gkwin->left;
    
    if(gkwin_absy < ancestor_absy)
       gkwin->limit_top = gkwin->top + ancestor_absy - gkwin_absy;
    else
        gkwin->limit_top = gkwin->top;
    
    if((gkwin_absx+gkwin->right-gkwin->left) 
            < (ancestor_absx+ancestor->limit_right-ancestor->limit_left))
        gkwin->limit_right = gkwin->right;
    else
        gkwin->limit_right = gkwin->right -
                    (  gkwin_absx+gkwin->right-gkwin->left
                   -(ancestor_absx+ancestor->limit_right-ancestor->limit_left));

    if((gkwin_absy+gkwin->bottom-gkwin->top) 
            < (ancestor_absy+ancestor->limit_bottom-ancestor->limit_top))
        gkwin->limit_bottom = gkwin->bottom;
    else
        gkwin->limit_bottom = gkwin->bottom -
                    (  gkwin_absy+gkwin->bottom-gkwin->top
                   -(ancestor_absy+ancestor->limit_bottom-ancestor->limit_top));

}
//----���ñ߽�ģʽ-------------------------------------------------------------
//����: �趨���ڵ���ʾ�߽��Ƿ��ܸ��������ƣ����ƺ��Ӵ��ڳ��������ڵĲ��ֽ�����
//      ��ʾ��desktop��ֱ���Ӵ���Ĭ�����ޣ����ܸ��ġ�
//����: gkwin��Ŀ�괰��
//      mode��trueΪ���ޣ�falseΪ������
//����: ��
//-----------------------------------------------------------------------------
void gk_set_bound_mode(struct gksc_para_set_bound_mode *para)
{
    struct gkwin_rsc *current;
    if(para->gkwin == NULL)
        return;
    if(para->gkwin == para->gkwin->disp->desktop)   //���治���޸ı߽�ģʽ
        return;
    if(para->gkwin->bound_limit == para->mode)      //ģʽδ�ı�
        return;
    if(&para->gkwin->disp->desktop->node == rsc_get_parent(&para->gkwin->node))
        return;                         //ֱ�ӷ���������Ӵ��ڣ�����ı�
    //Ŀ�괰�ڱ߽�ģʽ�ı䣬�������Ըı�
    para->gkwin->bound_limit = para->mode;
    __gk_set_bound(para->gkwin);
    current = para->gkwin;
    //����gkwin�������Ӵ���
    while(rsc_trave_scion(&para->gkwin->node,&current->node))
    {
        __gk_set_bound(current);
    }
    para->gkwin->disp->reset_clip = true;
}

//----���ô�����ʾ���ȼ�-------------------------------------------------------
//����: ����һ�����ڵ����ȼ������ȼ�������ͬ�������е�z�����λ�ã�����ԽС����
//      z����Խ��ǰ�����ȼ�<=0�����Ǹ����ڣ���֮�������ڸ��ǡ����ڸı����ȼ���
//      ����Z���е�˳����ܻ�ı䣬��Ļ����Ҳ���ܻ�ı䡣
//      ���ڱ��ı����ȼ��Ĵ��ڿ��ܻ����Ӵ��ڣ����ԣ���z���б��ƶ��ģ�����һ����
//      �ڣ�����������һ�δ��ڡ�
//����: gkwin��Ŀ�괰��
//      prio�������ȼ�
//����: ��
//-----------------------------------------------------------------------------
void gk_set_prio(struct gksc_para_set_prio *para)
{
    struct gkwin_rsc *target_section,*gkwin;  
    struct display_rsc *display;
    struct gkwin_rsc *parent;       //gkwin�ĸ�����
    //z���б��ƶ��Ĵ��ڶ����˵Ĵ���
    struct gkwin_rsc *section_start;
    //z���б��ƶ��Ĵ��ڶ���ǰ�˵Ĵ���
    struct gkwin_rsc *section_end,*target_win;     

    gkwin = para->gkwin;
    display = para->gkwin->disp;
    if(gkwin == display->desktop)  //���洰�ڵ����ȼ����ɸı�
        return;

    
    parent = (struct gkwin_rsc *)rsc_get_parent(&gkwin->node); 
    //ȡz���б��ƶ��Ĵ��ڶ���ǰ�˵Ĵ��ڶ�
    section_end = (struct gkwin_rsc*)rsc_get_son(&parent->node);
    section_start = (struct gkwin_rsc*)rsc_get_next(&section_end->node);
    target_section = section_end;
    while(1)
    {//����ͬ�������к����ȼ�����para->prio�ĵĴ��ڣ�ɨ����z��Ϊ����   
        //ɨ��˳��:��z���ǰ�����˽���ɨ�裬ֻɨ�ֵܴ��ڣ���ɨ�Ӵ���
        if(target_section->z_prio >= para->prio)
            break;
        //ɨ��Ŀ�괰������z������һ���ڣ���ֹ����
        if(target_section == section_start)
            break;
        target_section = (struct gkwin_rsc*)rsc_get_previous(&target_section->node);
    }
    if(target_section->z_prio >= para->prio)
    {// �ҵ�gkwinͬ�����������ȼ����ڻ������prio�Ĵ���
        //����Դ�����������ƶ��������ȼ����п���Ҫ�ƶ�
        if((target_section == gkwin) 
                || (target_section == (struct gkwin_rsc*)rsc_get_previous(&gkwin->node)))
        {
            if((gkwin->z_prio <= 0) && (para->prio > 0))    //�Ƶ������ں��
            {
                //��ȡgkwin�������ﴰ������z��εĿ�ʼ�ͽ�������
                section_start = __gk_get_zsection_all_start(gkwin);
                section_end = __gk_get_zsection_all_end(gkwin);
                //����gkwinҪ����Ĵ����Ƿ�����ǰ�˴���,
                //��,��gkwin����Z�����ǰ�˵Ĵ��ڵ���Ϊ����Z�����ǰ�˴���
                if(section_end == display->z_topmost)
                    display->z_topmost = parent;
                //��gkwin����z��δ�z��ȡ����
                section_start->z_back->z_top = section_end->z_top;
                section_end->z_top->z_back = section_start->z_back;
                
                section_start->z_top = parent;
                section_end->z_back = parent->z_back;
                parent->z_back->z_top = section_start;
                parent->z_back = section_end;
                
                gkwin->z_prio = para->prio;
                display->reset_clip = true;
            }
            else if((gkwin->z_prio > 0) && (para->prio <= 0))//�Ƶ�������ǰ��
            {
                //��ȡgkwin�������ﴰ������z��εĿ�ʼ�ͽ�������
                section_start = __gk_get_zsection_all_start(gkwin);
                section_end = __gk_get_zsection_all_end(gkwin);
                //����gkwinҪ����Ĵ����Ƿ�����ǰ�˴���,
                //��,��gkwin����Z�����ǰ�˵Ĵ��ڵ���Ϊ����Z�����ǰ�˴���
                if(parent == display->z_topmost)
                    display->z_topmost = section_end;
                //��gkwin����z��δ�z��ȡ����
                section_start->z_back->z_top = section_end->z_top;
                section_end->z_top->z_back = section_start->z_back;
                
                section_start->z_back = parent;
                section_end->z_top = parent->z_top;
                parent->z_top->z_back = section_end;
                parent->z_top = section_start;
                
                gkwin->z_prio = para->prio;
                display->reset_clip = true;
            }
            else    //��������������ƶ�z��
            {
                gkwin->z_prio = para->prio;
            }
        }
        else    //��Դ�������ƶ�
        {
            rsc_moveto_next(&target_section->node,&gkwin->node);
																  
            if((para->prio <= 0) && (target_section->z_prio > 0))
                target_win = parent;
            else
                target_win = __gk_get_zsection_all_end(target_section);
            //��ȡgkwin�������ﴰ������z��εĿ�ʼ�ͽ�������
            section_start = __gk_get_zsection_all_start(gkwin);
            section_end = __gk_get_zsection_all_end(gkwin);
            //����gkwinҪ����Ĵ����Ƿ�����ǰ�˴���,
            //��,��gkwin����Z�����ǰ�˵Ĵ��ڵ���Ϊ����Z�����ǰ�˴���
            if(target_win == display->z_topmost)
                display->z_topmost = section_end;
            //��gkwin����z��δ�z��ȡ����
            section_start->z_back->z_top = section_end->z_top;
            section_end->z_top->z_back = section_start->z_back;
            
            section_start->z_back = target_win;
            section_end->z_top = target_win->z_top;
            target_win->z_top->z_back = section_end;
            target_win->z_top = section_start;

            gkwin->z_prio = para->prio;
            display->reset_clip = true;
        }
    }
    else
    {//ͬ���������ȼ���������prio
        rsc_moveto_previous(&target_section->node,&gkwin->node);

        if((para->prio > 0) && (target_section->z_prio <=0))
        {
            target_win = parent;
        }
        else
        {
            //�ҵ�new_location����������ɵ�z��������˵Ĵ���
            target_win = __gk_get_zsection_all_start(target_section);
        }
        //���°�gkwinȡ��������move_end_temp���
        //��ȡgkwin�������ﴰ������z��εĿ�ʼ�ͽ�������
        section_start = __gk_get_zsection_all_start(gkwin);
        section_end = __gk_get_zsection_all_end(gkwin);
        
        //��gkwin����z��δ�z��ȡ����
        section_start->z_back->z_top = section_end->z_top;
        section_end->z_top->z_back = section_start->z_back;
        
        section_start->z_top = target_win;
        section_end->z_back = target_win->z_back;
        target_win->z_back->z_top = section_start;
        target_win->z_back = section_end;
        
        gkwin->z_prio = para->prio;
        display->reset_clip = true;
    }
}

//----���ù�դ������---------------------------------------------------------
//����: �ı䴰�ڵĹ�դ�����롣�ǻ��崰�ں�����ֻ֧��cn_r3_srccopy��
//    	�����޸Ĺ�դ�����롣
//����: gkwin��Ŀ�괰��
//      bitmsk��Ҫ���õ�����λͼ 
//      rop_code���¹�դ������
//����: true=�ɹ���false=ʧ��
//-----------------------------------------------------------------------------
bool_t gk_set_rop_code(struct gksc_para_set_rop_code *para)
{
    u32 buf_size;
    struct bmbuf_info buf_info,buf_bak;
    struct gkwin_rsc *mygkwin;
    struct display_rsc *mydisplay;
    bool_t blend;
    
    mygkwin = para->gkwin;
    mydisplay = mygkwin->disp;
    buf_info.vbuf.pf_type = mydisplay->pixel_format;
    //�ǻ��崰�ں�����ֻ֧��cn_r3_srccopy�������޸�rop_code
    if((mygkwin->wm_bitmap.bm_bits == NULL)
        || (mydisplay->desktop == mygkwin)
        || (para->rop_code == mygkwin->rop_code))
    {
        return false;        
    }
    //�����õ�rop_code��rop4����������bitmskΪNULL������������rop_code
    if((para->rop_code & cn_rop_ext_en)
                    && (para->rop_code & cn_rop_rop4_en)
                    && (para->bitmsk.bm_bits == NULL))
    {
    	return false;            
    }
    buf_bak.vbuf = mygkwin->wm_bitmap;
    buf_bak.rop4_msk = mygkwin->bitmsk;
    buf_bak.pat = mygkwin->pat_buf;
    buf_info = buf_bak;
    if(__gk_vrmalloc(mydisplay,&buf_info,mygkwin->wm_bitmap.width,
                        mygkwin->wm_bitmap.height,para->rop_code))
    {
        blend = gk_rop_needsno_destination(para->rop_code);
        mygkwin->rop_code = para->rop_code;
        if(mygkwin->dest_blend != blend)
        {
            mygkwin->dest_blend = blend;
            mydisplay->reset_clip = true;
        }
        mygkwin->change_flag = cn_gkwin_change_all; 
        if(mygkwin->wm_bitmap.bm_bits != buf_info.vbuf.bm_bits)
        {
            buf_size = buf_info.vbuf.linebytes * buf_info.vbuf.height;
            memcpy(buf_info.vbuf.bm_bits,mygkwin->wm_bitmap.bm_bits,buf_size);
            mygkwin->wm_bitmap = buf_info.vbuf;
        }
        
        if(mygkwin->bitmsk.bm_bits != buf_info.rop4_msk.bm_bits)
        {
            buf_size = buf_info.rop4_msk.linebytes * buf_info.rop4_msk.height;
            memcpy(buf_info.rop4_msk.bm_bits,mygkwin->bitmsk.bm_bits,buf_size);
            mygkwin->bitmsk = buf_info.rop4_msk;
        }
        if(mygkwin->pat_buf != buf_info.pat)
        {
            buf_size = mydisplay->patx 
                    *(mydisplay->pixel_format>>8)>>3;//����1��ռ�����ֽ�
            buf_size = buf_size * mydisplay->paty;
            memcpy(buf_info.pat,mygkwin->pat_buf,buf_size);
            mygkwin->pat_buf = buf_info.pat;
        }
        __gk_vfree(mydisplay,&buf_bak);
        return true;
    }else
    {
        djy_error_login(enum_gk_no_memory,"��ʾ�ڴ治��");
        return false;
    }
}
//----���ô���ͼ��λͼ---------------------------------------------------------
//����: �ı䴰�ڵ�ͼ��λͼ���ǻ��崰�ں�����ֻ֧��cn_r3_srccopy��
//    	����Ҫpat_buf��
//����: gkwin��Ŀ�괰��
//      pat_buf���µ�ͼ��λͼ
//����: true=�ɹ���false=ʧ��
//-----------------------------------------------------------------------------
bool_t gk_set_pat_buf(struct gksc_para_set_pat_buf *para)
{
    u32 pat_size;
    //�ǻ��崰�ں�����ֻ֧��cn_r3_srccopy,����Ҫpat_buf
    if((para->gkwin->disp->frame_buffer == NULL)
        || (para->gkwin->disp->desktop == para->gkwin))
        return false;               
    //����1��ռ�����ֽ�    
    pat_size = para->gkwin->disp->patx 
                * (para->gkwin->disp->pixel_format >>8)>>3; 
    pat_size = pat_size * para->gkwin->disp->paty; 
    para->gkwin->disp->format_pattern(para->pattern.bm_bits,&para->pattern);    
    memcpy(para->gkwin->pat_buf,para->pattern.bm_bits,pat_size);
    para->gkwin->dest_blend = 
                	gk_rop_needsno_destination(para->gkwin->rop_code);
    //�ڴ��ڹ�դ������Ҫpat_buf������£�ֻ�ı�pat_buf ����Ҫ����change_flag
    if(gk_rop_needsno_pattern(para->gkwin->rop_code))
        para->gkwin->change_flag = cn_gkwin_change_all;
    return true;
}
//----���ô���͸��ɫ-----------------------------------------------------------
//����: �ı䴰��͸��ɫ
//����: gkwin��Ŀ�괰��
//      transparentcolor�����õ�͸��ɫ,��ɫ��ʽ�����ɫ
//����: true=�ɹ���false=ʧ��
//-----------------------------------------------------------------------------
bool_t gk_set_transparentcolor(struct gksc_para_set_transparentcolor *para)
{
    //���治��Ҫtransparentcolor
    if(para->gkwin->disp->desktop == para->gkwin)
        return false;               
    para->transparentcolor = __gk_convert_color_to_pf(
                                                para->gkwin->wm_bitmap.pf_type,
                                                para->transparentcolor);
    para->gkwin->transparentcolor = para->transparentcolor;
    //����ĳ������ڴ��ڹ�դ��������Ϊ͸����ֻ�ı䵱ǰ͸��ɫ�����
    if((para->gkwin->rop_code & cn_rop_ext_en) 
        && (para->gkwin->rop_code & cn_rop_transparentcolor))
    {
        para->gkwin->change_flag = cn_gkwin_change_all;
        para->gkwin->dest_blend = true;
    }
    return true;
}
//----���ô�������λͼ---------------------------------------------------------
//����: ���ô�������λͼ��ֻ��֧��rop4�Ĵ��ڲ���Ҫ����λͼ
//����: gkwin��Ŀ�괰��
//      bitmsk�����õ�����λͼ
//����: true=�ɹ���false=ʧ��
//-----------------------------------------------------------------------------
bool_t gk_set_bitmsk(struct gksc_para_set_bitmsk *para)
{
    //�ǻ��崰�ڲ���Ҫbitmsk
    if(para->gkwin->disp->frame_buffer == NULL)
        return false;               
    para->gkwin->bitmsk = para->bitmsk;
    para->gkwin->dest_blend = 
                	gk_rop_needsno_destination(para->gkwin->rop_code);
    para->gkwin->change_flag = cn_gkwin_change_all;
    return true;
}
//----���ô���ֱ��д������-----------------------------------------------------
//����: ���ô���ֱ��д�����ԡ�
//����: gkwin��Ŀ�괰��
//����: ��
//-----------------------------------------------------------------------------
void gk_set_direct_screen(struct gksc_para_set_direct_screen *para)
{
    if(para->gkwin->direct_screen == true)
        return;
    else
        para->gkwin->direct_screen = true;
}
//----��������ֱ��д������-----------------------------------------------------
//����: ��������ֱ��д�����ԡ�
//����: gkwin��Ŀ�괰��
//����: ��
//-----------------------------------------------------------------------------
void gk_unset_direct_screen(struct gksc_para_unset_direct_screen *para)
{
    if(para->gkwin->direct_screen == false)
        return;
    else
        para->gkwin->direct_screen = false;
}
//----���ٴ���(�Ӵ���)---------------------------------------------------------
//����: �ͷŴ�����ռ�õ���Դ��
//����: gkwin��Ŀ�괰��
//����: ��
//-----------------------------------------------------------------------------
void __gk_destroy_win(struct gkwin_rsc *gkwin)
{
    struct bmbuf_info buf_info;
    if(gkwin->disp->z_topmost == gkwin)
        gkwin->disp->z_topmost = gkwin->z_back;
    gkwin->z_back->z_top = gkwin->z_top;
    gkwin->z_top->z_back = gkwin->z_back;
    rsc_del_node(&gkwin->node);
    if(gkwin->visible_clip != NULL)
        gkwin->disp->reset_clip = true;
    if(gkwin->disp->frame_buffer != NULL)         
    {
        if(gkwin->changed_msk.bm_bits != NULL)//��֡�����ڻ���ģʽ��
        {
            buf_info.vbuf.bm_bits = gkwin->wm_bitmap.bm_bits;
            buf_info.rop4_msk.bm_bits = gkwin->bitmsk.bm_bits;
            buf_info.pat = gkwin->pat_buf;
            __gk_vfree(gkwin->disp,&buf_info);
            m_free(gkwin->changed_msk.bm_bits);
            gkwin->visible_clip = 
                            __gk_free_clip_queue(gkwin->visible_clip);
            return;
        }
        else 
            return;
    }
    else
        return;
}
//----���ٴ���-------------------------------------------------------------
//����: �ͷŴ�����ռ�õ���Դ�����ٴ��ڣ�ͬʱ���ٴ��ڵ����ﴰ�ڡ�
//����: gkwin��Ŀ�괰��
//����: ��
//-----------------------------------------------------------------------------
void gk_destroy_win(struct gksc_para_destroy_win *para)
{
    struct gkwin_rsc *gkwin;

    while((gkwin = (struct gkwin_rsc *)rsc_get_twig(&para->gkwin->node))!= NULL)
    {
        __gk_destroy_win(gkwin);
    }
    __gk_destroy_win(para->gkwin);

}

//----�ı䴰�ڳߴ��λ��-------------------------------------------------------
//����: �ı�һ�����ڵĳߴ��Լ�λ�ã����û�ĳߴ磬ʵ���Ͼ���һ��gk_move_win��
//����: gcwin��Ŀ�괰��
//      new_area���µĴ��ڳߴ��λ��
//      mode���ĳߴ�󣬴���ͼ����ģʽ��0=���ֲ��䣬1=�����ѹ����
//            ���崰�ڲ���Ч
//����: true = �ɹ���false = ʧ��(һ�����ڴ���䵼��)
//ע: �л�����������£����ֻ���ƶ���λ�ã�����Ҫ���»��ơ�
//    ����޸��˳ߴ磬Ӧ�ó���Ӧ���ػ棬ֱ���ػ漴�ɣ�����ȴ��ػ���Ϣ��
//-----------------------------------------------------------------------------
bool_t gk_change_win_area(struct gksc_para_change_win_area *para)
{
    s32 left,top,right,bottom;
    struct gkwin_rsc *gkwin;
    struct bmbuf_info buf_info;
    struct gkwin_rsc *current;
    struct gksc_para_move_win movwin_para;

    gkwin = para->gkwin;
    left = para->left;
    top = para->top;
    right = para->right;
    bottom = para->bottom;
    if((right < left) || (bottom < top))    //�ߴ����ݲ��Ϸ�
        return false;

    if(((right-left) == (gkwin->right-gkwin->left))
            && ((bottom-top) == (gkwin->bottom - gkwin->top)))
    {
        movwin_para.gkwin = gkwin;
        movwin_para.left = left;
        movwin_para.top = top;
        gk_move_win(&movwin_para);        //ֻ���ƶ��˴��ڣ��ߴ�δ��
        return true;
    }
    if(gkwin->wm_bitmap.bm_bits != NULL)
    {
        buf_info.vbuf = gkwin->wm_bitmap;
        if( !__gk_vrmalloc(gkwin->disp,&buf_info,right-left,
                                    bottom-top,gkwin->rop_code))
        {
            djy_error_login(enum_gk_no_memory,"�Դ治��");
            return false;   //�����Դ�ʧ�ܣ����ڳߴ�δ�ı�(��Ȼ��Ч)
        }else
        {
            gkwin->change_flag = cn_gkwin_change_none;
            gkwin->wm_bitmap = buf_info.vbuf;
            gkwin->bitmsk = buf_info.rop4_msk;
            gkwin->pat_buf = buf_info.pat;
        }
    }
    current = gkwin;
    gkwin->absx0 -= gkwin->left-left;          //�޸Ĵ��ھ�������
    gkwin->absy0 -= gkwin->top-top;
    gkwin->left = left;
    gkwin->right = right;
    gkwin->top = top;
    gkwin->bottom = bottom;
    __gk_set_bound(gkwin);                      //���ÿ���ʾ�߽�
    while(rsc_trave_scion(&gkwin->node,&current->node))//����gkwin�������Ӵ���
    {
        current->absx0 -= current->left -left; //�޸Ĵ��ھ�������
        current->absy0 -= current->top -top;
        __gk_set_bound(current);
        gkwin->change_flag = cn_gkwin_change_all;
    }
    gkwin->disp->reset_clip = true;
    return true;
}

//----���û�����ɫ-------------------------------------------------------------
//����: ���û�����ɫ���׳Ʊ���ɫ���ı�Ҳ�����������ɫ��
//����: gkwin��Ŀ�괰��
//      color���µĻ�����ɫ
//����: ��
//-----------------------------------------------------------------------------
//void gk_set_canvas_color(struct gkwin_rsc *gkwin,u32 color)
//{
//    gkwin->canvas_color = color;
//}

//----ȡ������ɫ-------------------------------------------------------------
//����: ��ȡ������ɫ���׳Ʊ���ɫ���ı�Ҳ�����������ɫ��
//����: gkwin��Ŀ�괰��
//      color���µĻ�����ɫ
//����: ��
//-----------------------------------------------------------------------------
//void gk_get_canvas_color(struct gkwin_rsc *gkwin,u32 *color)
//{
//    *color = gkwin->canvas_color;
//}

//----�������redraw����-------------------------------------------------------
//����: �����д������޸ĵĿ������������ʾ��������������Ѿ�׼���á�
//����: display����ɨ�����ʾ��
//����: ��
//-----------------------------------------------------------------------------
void __gk_output_redraw(struct display_rsc *display)
{
    s32 x,y,x_src,y_src,x_dst,y_dst;
    u32 color;
    struct clip_rect *clip;
    struct display_rsc *mirror,*current;
    struct gkwin_rsc *gkwin,*desktop_gkwin,*topwin,*frame_buf;
    struct gkuc_para_repaint repaint;
    struct st_rect_bitmap *src,*dst;
    struct st_rect *rect;
    struct st_rect_bitmap *bitmap;
    
    frame_buf = display->frame_buffer;          //ȡ֡���崰��
    desktop_gkwin = display->desktop;
    topwin = display->z_topmost;
    if(frame_buf == NULL)   //��֡���壬��֡�������ʾ������Ȼû�д��ڻ���
    {
        //��û�л�����������£����Ʋ����ǲ�������redraw_clip�ģ�ֻ�д��ڹ���
        //�����Ż�Ҫ���ػ档
        gkwin = desktop_gkwin;
        do{
            clip = gkwin->redraw_clip;
            if(clip != NULL)
            {
                repaint.win = (void*)gkwin;
                repaint.redraw_clip = clip;
                gk_usercall_chunnel(cn_gkuc_repaint,pg_usercall_semp,NULL,
                                  &repaint,sizeof(struct gkuc_para_repaint));
                //�ȴ��û�����ػ��������ʱ(1��)����
                semp_pend(pg_usercall_semp,0);
                gkwin->redraw_clip = __gk_free_clip_queue(gkwin->redraw_clip);
            }
            gkwin = gkwin->z_top;
        }while(gkwin != topwin->z_top);
    }else                                           //��֡����
    {
        gkwin = desktop_gkwin;
        do{
            clip = gkwin->redraw_clip;
            if(clip != NULL)
            {
                if(gkwin->wm_bitmap.bm_bits == NULL)   //�޴��ڻ���
                {
                    repaint.win = (void*)gkwin;
                    repaint.redraw_clip = clip;
                    gk_usercall_chunnel(cn_gkuc_repaint,pg_usercall_semp,NULL,
                                    &repaint,sizeof(struct gkuc_para_repaint));
                    //�ȴ��û�����ػ��������ʱ(1��)����
                    semp_pend(pg_usercall_semp,0);   
                }else                                   //�д��ڻ���
                {
                    do
                    {
                        if(!display->draw.blt_bm_to_bm(
                                            &frame_buf->wm_bitmap,
                                            &clip->rect,
                                            &gkwin->wm_bitmap,
                                            clip->rect.left-gkwin->absx0,
                                            clip->rect.top-gkwin->absy0,
                                            gkwin->pat_buf,
                                            &gkwin->bitmsk,
                                            gkwin->rop_code,
                                            gkwin->transparentcolor))
                        {
                            dst = &frame_buf->wm_bitmap;
                            src = &gkwin->wm_bitmap;
                            
                            y_dst = clip->rect.top;
                            if((gkwin->rop_code & cn_rop_ext_en) != 0)
                            {
                                for(y_src = clip->rect.top-gkwin->absy0;
                                    y_src < clip->rect.bottom-gkwin->absy0;
                                    y_src++)
                                {
                                    x_dst = clip->rect.left;
                                    for(x_src = clip->rect.left-gkwin->absx0;
                                        x_src < clip->rect.right-gkwin->absx0;
                                        x_src++)
                                    {
                                        __gk_set_pixel_rop_ext_bm(dst,src,
                                                gkwin->pat_buf,
                                                x_dst,y_dst,x_src,y_src,
                                                gkwin->rop_code,&gkwin->bitmsk,
                                                gkwin->transparentcolor);
                                        x_dst ++;
                                    }
                                    y_dst ++;
                                }                           
                            }
                            else
                            {
                            	for(y_src = clip->rect.top-gkwin->absy0;
                                    y_src < clip->rect.bottom-gkwin->absy0;
                                    y_src++)
                                {
                                    x_dst = clip->rect.left;
                                    for(x_src = clip->rect.left-gkwin->absx0;
                                        x_src < clip->rect.right-gkwin->absx0;
                                        x_src++)
                                    {
                                        __gk_set_pixel_rop3_bm(dst,src,
                                            	gkwin->pat_buf,x_dst,y_dst,
                                            	x_src,y_src,gkwin->rop_code);
                                        x_dst ++; 
                                    }
                                    y_dst ++;
                                 }                                 
                            }
                        }
                        __gk_shading_rect(frame_buf,&clip->rect);
                        clip = clip->next;
                    }while(clip != gkwin->redraw_clip);
                }
                gkwin->redraw_clip = __gk_free_clip_queue(gkwin->redraw_clip);
            }
            gkwin = gkwin->z_top;
        }while(gkwin != topwin->z_top);
        //���°�֡�������������ʾ��
        clip = __gk_get_changed_clip(frame_buf);  //��ȡ֡�����б��޸ĵĿ�����
        if(clip != NULL)
        {
            __gk_shading_clear(frame_buf);
            frame_buf->redraw_clip = clip;
            mirror = (struct display_rsc *)gkwin->disp->node.child;//������ʾ��
            do{
                if(display->framebuf_direct == false)
                {
                    //�����screen
                    if(display->draw.bm_to_screen(&clip->rect,
                                                &frame_buf->wm_bitmap,
                                                clip->rect.left,clip->rect.top))
                    {
                    }
                    else
                    {
                        rect = &clip->rect;
                        bitmap = &frame_buf->wm_bitmap;
                        for(y = rect->top;y < rect->bottom;y++)
                        {
                            for(x = rect->left;x < rect->right;x++)
                            {
                                color = __gk_get_pixel_bm(bitmap,x,y);
                                color =__gk_convert_color_to_24(bitmap->pf_type,
                                                                color);
                                display->draw.set_pixel_screen(x,y,color,
                                                                cn_r2_copypen);
                            }
                        }
                    }
                }
                current = mirror;
                
                while(current != NULL)  //����ȫ�����񴰿�
                {
                    current->draw.bm_to_screen(&clip->rect,
                                                &frame_buf->wm_bitmap,
                                                clip->rect.left,clip->rect.top);                                                
                    current = (struct display_rsc *)rsc_trave_scion(
                                                &mirror->node,&current->node);
                }
                clip = clip->next;
            }while(clip != frame_buf->redraw_clip);
            frame_buf->redraw_clip =
                            __gk_free_clip_queue(frame_buf->redraw_clip);
        }
    }
}

//----��ʾ�����������ʾ��-----------------------------------------------------
//����: ��������ʾ������Ҫ����Ĳ����������ʾ����
//����: ��
//����: ��
//-----------------------------------------------------------------------------
void __gk_redraw_all(void)
{
    struct display_rsc *display,*display_next;
    display = (struct display_rsc *)rsc_get_son(&tg_display_root_rsc);
    display_next = display;
    if(display != NULL)
    {
        do{
            __gk_get_redraw_clip_all(display);//ɨ��ȡ���µ����ػ������
            __gk_output_redraw(display);    //�ػ�
            display_next = (struct display_rsc *)
                                    rsc_get_next(&display_next->node);
        }while(display != display_next);  //ɨ��������ʾ��
    }
}
//----ϵͳ���ùܵ�-------------------------------------------------------------
//����: gui api��gui kernel�Ľӿں�������api������Ļ���Ҫ�󴫵ݵ�gui kernel����
//      mp�汾�У����Կ���ʹ�����ж�ʵ�֡�
//����: param��������ַ
//      size����������
//����: ʵ��д��ܵ�����������0����size��
//-----------------------------------------------------------------------------
u16 gk_syscall_chunnel(u16 command,u32 sync_time,void *param1,u16 size1,
                                                void *param2,u16 size2)
{
    u16 completed = 0;
    u8 buf[2];
    mutex_pend(tg_gk_chunnel.syscall_mutex,1000*mS);   //�ܵ����ʻ���
    while(1)
    {
        if((ring_capacity(&tg_gk_chunnel.ring_syscall)
                    - ring_check(&tg_gk_chunnel.ring_syscall)) <(size1+size2+2))
        {       //�ܵ�������������
            if(!semp_pend(tg_gk_chunnel.syscall_semp,1000*mS))//�ȴ��ܵ��ͷſռ�
                break;                                  //�ȴ��ܵ�ʧ��
            else
                continue;                               //�ٴμ������
        }
        buf[0] = (u8)command;
        buf[1] = (u8)(command>>8);
        completed = ring_write(&tg_gk_chunnel.ring_syscall,buf,2);
        if(size1 != 0)
            completed += ring_write(&tg_gk_chunnel.ring_syscall,param1,size1);
        if(size2 != 0)
            completed += ring_write(&tg_gk_chunnel.ring_syscall,param2,size2);
        break;
    }
    mutex_post(tg_gk_chunnel.syscall_mutex);    //�ܵ����ʻ�����
    if(semp_query_free(pg_syscall_semp) == 0)
        semp_post(pg_syscall_semp);
    if(0 != sync_time)      //�趨�˵ȴ�ʱ��
    {
        mutex_pend(pg_gk_server_sync,sync_time);
        mutex_post(pg_gk_server_sync);
    }
    return completed;
}


//----�û����ùܵ�-------------------------------------------------------------
//����: ��������gk_syscall_chunnle�������������������gui kernel��Ҫ�ϲ�Ӧ�ó���
//      ���ĳЩ����ʱ�����ñ��������Ѳ���Ҫ��ͨ���ܵ�������ȥ��
//����: param��������ַ
//      size����������
//����: ʵ��д��ܵ�����������0����size��
//ע: usercall_bufֻ��gkwin������д�����������軥��������
//-----------------------------------------------------------------------------
u16 gk_usercall_chunnel(u16 usercall_id,struct semaphore_LCB *semp,
                        void **result,void *param,u16 size)
{
    u16 completed = 0,loop;
    u8 buf[100];    //�����г���100�ֽڵ�����ġ�
    while(1)
    {
        if((ring_capacity(&tg_gk_chunnel.ring_usercall)
                    - ring_check(&tg_gk_chunnel.ring_usercall)) < size)
        {       //�ܵ�������������
            if(!semp_pend(tg_gk_chunnel.usercall_semp,0))//�ȴ��ܵ�����ռ�
                break;                                  //�ȴ��ܵ�ʧ��
            else
                continue;                               //�ٴμ������
        }
        buf[0] = (u8)usercall_id;
        buf[1] = (u8)(usercall_id>>8);
        for(loop = 0; loop < sizeof(void*); loop++)
        {
            buf[2+loop] = ((u8*)&semp)[loop] ;
        }
        for(loop = 0; loop < sizeof(void*); loop++)
        {
            buf[2 + sizeof(void*) +loop] = ((u8*)&result)[loop] ;
        }
        for(loop = 0; loop < size-2+2*sizeof(void*); loop++)
        {
            buf[2+2*sizeof(void*)+loop] = ((u8*)param)[loop] ;
        }
        completed = ring_write(&tg_gk_chunnel.ring_usercall,buf,
                                size+2+sizeof(void*));
    }
    return completed;
}

//----gui kernel������---------------------------------------------------------
//����: �ӻ�����ȡ����ʾ���󣬲�����֮��
//����: ��
//����: ��
//-----------------------------------------------------------------------------
u8 draw_chunnel_buf[cfg_gk_down_chunnel_deep];
void gk_server(void)
{
    u16 command;
    u32 num,offset;
    mutex_pend(pg_gk_server_sync,cn_timeout_forever);
    djy_set_event_prio(248);
    while(1)
    {
        //һ�ζ�ȡȫ�������Ϊ����ʱ�л��������������Թܵ��е����ݿ϶���������
        //��������ڰ�������Ŀ��ܡ�
        num = ring_read(&tg_gk_chunnel.ring_syscall,(u8*)draw_chunnel_buf,cfg_gk_down_chunnel_deep);

        if(num == 0)
        {
            //���������ִ����󣬼����û��win buffer��Ҫˢ��screen��
            __gk_redraw_all();

            int_save_asyn_signal();
            mutex_post(pg_gk_server_sync);
            mutex_pend(pg_gk_server_sync,cn_timeout_forever);
            int_restore_asyn_signal();
            semp_pend(pg_syscall_semp,cn_timeout_forever);
            continue;
        }
        if(semp_query_used(tg_gk_chunnel.syscall_semp) != 0)
            semp_post(tg_gk_chunnel.syscall_semp);    //�ͷŹܵ�
        offset = 0;
        while(num != offset)
        {
            //���ڹܵ��е����ݿ��ܲ����룬�ʱ��������copy����������ֱ����ָ��
            //ָ���ǿ������ת��
            command = draw_chunnel_buf[offset] + ((u16)draw_chunnel_buf[offset+1]<<8);
            //sizeof(u16)������2�������cn_byte_bits>=16�Ļ����ϣ�sizeof(u16)=1.
            offset += sizeof(u16);
//            sync_draw = draw_chunnel_buf[offset];
//            offset += 1;  
            switch(command)
            {
                case cn_gksc_create_gkwin:
                {
                    struct gksc_para_create_gkwin para;
                    memcpy(&para,draw_chunnel_buf+offset,
                            sizeof(struct gksc_para_create_gkwin));
                    *(para.result) = (void*)gk_create_win(&para);
                    offset += sizeof(struct gksc_para_create_gkwin);
                } break;
                case cn_gksc_set_pixel:
                {
                    struct gksc_para_set_pixel para;
                    memcpy(&para,draw_chunnel_buf+offset,sizeof(struct gksc_para_set_pixel));
                    gk_set_pixel(&para);
                    offset += sizeof(struct gksc_para_set_pixel);
                } break;
                case cn_gksc_lineto:
                {
                    struct gksc_para_lineto para;
                    memcpy(&para,draw_chunnel_buf+offset,sizeof(struct gksc_para_lineto));
                    gk_lineto(&para);
                    offset += sizeof(struct gksc_para_lineto);
                } break;
                case cn_gksc_lineto_inc_end:
                {
                    struct gksc_para_lineto para;
                    memcpy(&para,draw_chunnel_buf+offset,sizeof(struct gksc_para_lineto));
                    gk_lineto_ie(&para);
                    offset += sizeof(struct gksc_para_lineto);
                } break;
                case cn_gksc_draw_bitmap:
                {
                    struct gksc_para_draw_bitmap para;
                    memcpy(&para,draw_chunnel_buf+offset,
                            sizeof(struct gksc_para_draw_bitmap));
                    gk_draw_bitmap(&para);
                    offset += sizeof(struct gksc_para_draw_bitmap);
                } break;
                case cn_gksc_draw_bitmap_rop:
                {
                    struct gksc_para_draw_bitmap_rop para;
                    memcpy(&para,draw_chunnel_buf+offset,
                            sizeof(struct gksc_para_draw_bitmap_rop));
                    gk_draw_bitmap_rop(&para);
                    offset += sizeof(struct gksc_para_draw_bitmap_rop);
                } break;
                case cn_gksc_fill_win:
                {
                    struct gksc_para_fill_win para;
                    memcpy(&para,draw_chunnel_buf+offset,
                            sizeof(struct gksc_para_fill_win));
                    gk_fill_win(&para);
                    offset += sizeof(struct gksc_para_fill_win);
                } break;
                case cn_gksc_set_rop_code:
                {
                    struct gksc_para_set_rop_code para;
                    memcpy(&para,draw_chunnel_buf+offset,
                            sizeof(struct gksc_para_set_rop_code));
                    gk_set_rop_code(&para);
                    offset += sizeof(struct gksc_para_set_rop_code);
                } break;
                case cn_gksc_set_pat_buf:
                {
                    struct gksc_para_set_pat_buf para;
                    memcpy(&para,draw_chunnel_buf+offset,
                            sizeof(struct gksc_para_set_pat_buf));
                    gk_set_pat_buf(&para);                    
                    offset += sizeof(struct gksc_para_set_pat_buf);
                } break;
                case cn_gksc_draw_text:
                {
                    struct gksc_para_draw_text para;
                    memcpy(&para,draw_chunnel_buf+offset,
                            sizeof(struct gksc_para_draw_text));
                    offset += sizeof(struct gksc_para_draw_text);
                    gk_draw_text(&para,(char*)draw_chunnel_buf+offset);                    
                    offset += para.count;
                }break;
                case cn_gksc_set_transparentcolor:
                {
                    struct gksc_para_set_transparentcolor para;
                    memcpy(&para,draw_chunnel_buf+offset,
                            sizeof(struct gksc_para_set_transparentcolor));
                    gk_set_transparentcolor(&para);                    
                    offset += sizeof(struct gksc_para_set_transparentcolor);
                } break;
                case cn_gksc_set_bitmsk:
                {
                    struct gksc_para_set_bitmsk para;
                    memcpy(&para,draw_chunnel_buf+offset,
                            sizeof(struct gksc_para_set_bitmsk));
                    gk_set_bitmsk(&para);                    
                    offset += sizeof(struct gksc_para_set_bitmsk);
                } break;
                case cn_gksc_destroy_win:
                {
                    struct gksc_para_destroy_win para;
                    memcpy(&para,draw_chunnel_buf+offset,
                            sizeof(struct gksc_para_destroy_win));
                    gk_destroy_win(&para);                    
                    offset += sizeof(struct gksc_para_destroy_win);
                } break;
                case cn_gksc_fill_part_win:
                {
                    struct gksc_para_fill_part_win para;
                    memcpy(&para,draw_chunnel_buf+offset,
                            sizeof(struct gksc_para_fill_part_win));
                    gk_fill_part_win(&para);
                    offset += sizeof(struct gksc_para_fill_part_win);
                } break;
                case cn_gksc_set_prio:
                {
                    struct gksc_para_set_prio para;
                    memcpy(&para,draw_chunnel_buf+offset,
                            sizeof(struct gksc_para_set_prio));
                    gk_set_prio(&para);
                    offset += sizeof(struct gksc_para_set_prio);
                } break;
                case cn_gksc_set_bound_mode:
                {
                    struct gksc_para_set_bound_mode para;
                    memcpy(&para,draw_chunnel_buf+offset,
                            sizeof(struct gksc_para_set_bound_mode));
                    gk_set_bound_mode(&para);
                    offset += sizeof(struct gksc_para_set_bound_mode);
                }break;
                case cn_gksc_move_win:
                {
                    struct gksc_para_move_win para;
                    memcpy(&para,draw_chunnel_buf+offset,
                            sizeof(struct gksc_para_move_win));
                    gk_move_win(&para);
                    offset += sizeof(struct gksc_para_move_win);
                }break;
                case cn_gksc_change_win_area:
                {
                    struct gksc_para_change_win_area para;
                    memcpy(&para,draw_chunnel_buf+offset,
                            sizeof(struct gksc_para_change_win_area));
                    gk_change_win_area(&para);
                    offset += sizeof(struct gksc_para_change_win_area);
                }break;
                case cn_gksc_draw_circle:
                {
                    struct gksc_para_draw_circle para;
                    memcpy(&para,draw_chunnel_buf+offset,
                            sizeof(struct gksc_para_draw_circle));
                    gk_draw_circle(&para);
                    offset += sizeof(struct gksc_para_draw_circle);
                } break;
                case cn_gksc_Bezier:
                {
                    struct gksc_para_Bezier para;
                    memcpy(&para,draw_chunnel_buf+offset,
                            sizeof(struct gksc_para_Bezier));
                    gk_Bezier(&para);
                    offset += sizeof(struct gksc_para_Bezier);
                } break;
                case cn_gksc_set_direct_screen:
                {
                    struct gksc_para_set_direct_screen para;
                    memcpy(&para,draw_chunnel_buf+offset,
                            sizeof(struct gksc_para_set_direct_screen));
                    gk_set_direct_screen(&para);
                    offset += sizeof(struct gksc_para_set_direct_screen);
                } break;
                case cn_gksc_unset_direct_screen:
                {
                    struct gksc_para_unset_direct_screen para;
                    memcpy(&para,draw_chunnel_buf+offset,
                            sizeof(struct gksc_para_unset_direct_screen));
                    gk_unset_direct_screen(&para);
                    offset += sizeof(struct gksc_para_unset_direct_screen);
                } break;
                default: break;                
            }   //for switch(command)
        }   //for while(num != offset)
    }   //for while(1)
    
}

#endif //   for #if (cfg_gui_kernel == 1)

