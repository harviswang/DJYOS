
#include "vnc_mirror_display.h"
#include "clip_process.h"
#include "vnc_thread.h"
#include "debug.h"
static rfbScreenInfoPtr vncscreen;
//��ŵ���vncserver��screen������Ӧ�ñ�֤��
//screen��ʧ���˳���ǰɾ��mirrordisplay�������ڴ����

int comp_two_fram_buf(unsigned char* buf1, unsigned char* buf2, int count)
{
    int result = 0;
    int i=0;
    for(i=0;i<count;i++)
    {
        if(*(buf1+i)!=*(buf2+i))
        {
            debug_printf("there must be somthing wrong!---byte=%d\n",i);
            return -1;
         }
    }
    debug_printf("mem ok!---i=%d bytes\n",i);
    return 0;
}


// =========================================================================
// ��������:��һ��bmͼ�������VNC����ʾ����
// �������:src_bitmap,��Ҫ�����ͼ��dst_rect,ͼ������Ҫ����Ĳ���
// ���������
// ����ֵ  :true������ɹ���  false�����ʧ��
// ˵��    :bitmap���Ժܴ󣬵���mirrordisplayȷ����һ���ܹ����ɵ��¡�
// =========================================================================
bool_t vnc_bm_to_screen(struct tagRectangle *dst_rect,\
                        struct tagRectBitmap *src_bitmap,s32 xsrc,s32 ysrc)
{
//Ϊ�˷���ʹ��clip����غ�����ʹ��DJYOS��clip�ṹ���ڷ������ݵ�ʱ���Լ���ת��
//�Ƚ��о��ο��������Ϊ��������
//1,�Ƚ�dst_rect��screen��clip������������Ϊ�ཻints��different
//2,��ints��different������
//3,��screen��clip���ϲ�����
    struct tagClipRect *newclip;//��rectת��Ϊclipģʽ
    bool_t ret=true;
    u8    bytesPP;//ÿ��������ռ�õ��ֽ���
    u32    i,x =0, y = 0, w =0, h =0;
    u8     *winbuf,*framebuf;
    u32    dst_linebytes;

    if(vncscreen == NULL)
    {
        return false;
    }
    if(0 == vncscreen->mirror_display_enable)
    {
        return true;
    }
    //�����µľ����ǲ��ǺϷ�����������һ����ǺϷ���
    //�򵥵��ж�һ�£�����
    if((dst_rect->left>dst_rect->right)||(dst_rect->top>dst_rect->bottom))
    {
        ret=false;
        return ret;
    }
    //8λ���µ���ʱ��֧��
    bytesPP = vncscreen->rfbServerFormat.bitsPerPixel/8;
    if(bytesPP==0)
    {
        ret=false;
        return ret;

    }
   // debug_printf("---bitmmap color= %d\n", src_bitmap->pf_type >>8);
    //Ҫ��д��ĵ�ַ���д���

    //���Ҫд����ʼ���Ѿ�����Lcd����ôֱ�Ӳ�����Ĭ�ϵ�(0 0 width heitht)
    if(!((dst_rect->top >=0)&&(dst_rect->top <= vncscreen->height)&&\
            (dst_rect->left >= 0)&&(dst_rect->left <= vncscreen->width)))
    {
        return false;
    }
        //����dst_rect�����µ�clip
    newclip = get_vncclip_space();
    if(NULL==newclip)
    {
        ret=false;
        return ret;
    }
    newclip->next=newclip;
    newclip->previous=newclip;
    newclip->rect.top=dst_rect->top;
    newclip->rect.left=dst_rect->left;
    //Ҫcopy�������Ѿ�����LCD��ȡ���ʵļ���
    if(dst_rect->right > vncscreen->width)
    {
        newclip->rect.right = vncscreen->width - 1;
    }
    else
    {
        newclip->rect.right = dst_rect->right;
    }

    if(dst_rect->bottom >=(vncscreen->height - cn_vnc_keyboard_height))
    {
        newclip->rect.bottom = vncscreen->height - cn_vnc_keyboard_height -1;
    }
    else
    {
        newclip->rect.bottom= dst_rect->bottom;
    }

    x = newclip->rect.left;
    y = newclip->rect.top;
    w = newclip->rect.right - newclip->rect.left;
    h = newclip->rect.bottom - newclip->rect.top;
    dst_linebytes=w*bytesPP;
    //��ʼ����screen�Ķ����޸ģ�Ҫ���л�����Դ�ı���
    XMutexWait(&vncscreen->frame_mutex);

    //�����Ǻܼ򵥣����½���clipȥ��ԭ���У��õ�different ��ints����different
    //���½���clip�ĺ;��������clip����

    anastomose_vncclip(&(vncscreen->clip_head), newclip);


    if(!check_clip_queque(vncscreen->clip_head))
    {
        debug_printf("@the que is not OK before sort!\n");
    }
    vncscreen->clip_head = combine_vnccliplink(vncscreen->clip_head);
    if(!check_clip_queque(vncscreen->clip_head))
    {
        debug_printf("@the que is not OK after sort!\n");
    }

    if(NULL == vncscreen->clip_head)
    {
      debug_printf("----������ʾ����clip��ȻΪ�գ������ܵ�����---\n");
    }

//����˾��ο�����⣬���ڿ�ʼ�������ݣ�ע����ǣ�mirror�����ظ�ʽ������ʾ������һ����
//��ʱ��֧��8bit���µ����ظ�ʽ
//����һ��������ʾ�����������ݿ���

    winbuf=src_bitmap->bm_bits+y*(src_bitmap->linebytes)+x*bytesPP;
    framebuf=vncscreen->frameBuffer+y*vncscreen->paddedWidthInBytes+x*bytesPP;
    //����Ϊ��λ���п�����ֻ��Ҫ����DST_RECT���ְ�
    for(i=0;i<h;i++)
    {
       memcpy(framebuf,winbuf,w*bytesPP);
       framebuf+=vncscreen->paddedWidthInBytes;
       winbuf+=src_bitmap->linebytes;
    }
//����update�¼�����Ҫ�����������ȼ�,�ͱ��¼������ȼ���һ����
//�ü����¼��в�ͨ��
//1�����update�ȴ�eventpop����ᵼ��softkey�ĸ��Ĳ���ʱ
//2��������ȴ�eventpop����pop���ȼ�ֻ����Ķ������¼������ȼ���������Ĺ�����
//3���������������ǽ����µ����ȼ�����������update�Լ����°�
//    Djy_EventPop(vncscreen->event_id_update,0,NULL,0,0,0,djy_get_event_prio());
//    vncscreen->event_update_prio = djy_get_event_prio();

    XMutexPost(&vncscreen->frame_mutex);
    ret=true;
    return ret;
}

// =============================================================================
// ��������:��װ������ʾ��
// �������:screen��rfbserver����ʾ��
// ���������
// ����ֵ  :true:�ɹ�  false��ʧ��
// ˵��    :�ڰ�װ������ʾ����ʱ�����ǻ��ȡ����ʾ����һЩ��������˿��Գ�ʼ�����ǵ�
//          screen�Ĳ������ȷ�˵�ߴ��Լ����ظ�ʽ���Լ�screen��buf�����룬ͬʱ����Ҳ
//        Ҫ���Լ���clippool������͵ײ��clip��˵����Ĺ�ϵ
// =============================================================================
#define  cn_limit_vncclip          100//max menuitem num
static struct tagClipRect            s_vncclip_rsc[cn_limit_vncclip];
struct tagMemCellPool  *vncclip_pool;
bool_t install_vncserver_mirrordisplay(rfbScreenInfoPtr rfbserver)
{
    bool_t  ret=false;
    struct tagDisplayRsc *base_display;
    struct tagGkWinRsc *desktop;
    struct tagDisplayRsc *mirror_display;//���ǵľ�����ʾ��

    if(NULL==rfbserver)
    {
        return ret;
    }

    //ͨ�������ҵ���ʾ�������ַ����е�out ---TODO
    desktop = GK_ApiGetDesktop("sim_display");

    if(NULL==desktop)//�Ҳ������档
    {
        return ret;
    }
    base_display=desktop->disp;
    if(NULL==base_display)//�Ҳ�������ʾ��
    {
        return ret;
    }
    if((base_display->pixel_format>>8)<8)//��ʱ��֧��8λ���ظ�ʽ
    {
        return ret;
    }
    vncclip_pool = Mb_CreatePool((void*)s_vncclip_rsc,
                                     cn_limit_vncclip,
                                     sizeof(struct tagClipRect),
                                     20,100,
                                     "vncclip_pool");
    if(NULL == vncclip_pool)
    {
        debug_printf("#fatalerror--create the vncclip_pool failed!\n");
        return ret;
    }

    mirror_display=M_MallocLc(sizeof(struct tagDisplayRsc),0);
    if(NULL==mirror_display)
    {
        debug_printf("#fatalerror--malloc mirror display mem failed!\n");
        Mb_DeletePool(vncclip_pool);
        return ret;
    }
    memset(mirror_display,0,sizeof(struct tagDisplayRsc));
    //��ʼ��������ʾ����һЩ�ر����ԣ�������ʾ������һ��--TODO��

    mirror_display->pixel_format = base_display->pixel_format;
    mirror_display->width = base_display->width;
    if(cn_vnc_keyboard_visual_enable)
    {
        mirror_display->height = base_display->height + cn_vnc_keyboard_height;
    }
    else
    {
        mirror_display->height = base_display->height;
    }
    mirror_display->frame_buffer = NULL;
    mirror_display->draw.bm_to_screen=vnc_bm_to_screen;

    ret= GK_InstallDisplayMirror(base_display,mirror_display,"vnc_server_disp");

    if(ret)//��װ�ɹ�
    {   //��������
        rfbserver->width=mirror_display->width;
        rfbserver->height=mirror_display->height;
        rfbserver->rfbServerFormat.bitsPerPixel=(mirror_display->pixel_format>>8);
        rfbserver->frameBuffer=M_MallocLc(rfbserver->width*\
                    (rfbserver->height)*(rfbserver->rfbServerFormat.bitsPerPixel/8),0);

        if(NULL==rfbserver->frameBuffer)
        {
            Rsc_DelNode((struct tagRscNode *)mirror_display);
            free(mirror_display);
            Mb_DeletePool(vncclip_pool);
            debug_printf("#fatalerror--malloc rfbserver frame buf failed!\n");

            return ret;
        }
//������ʾ�����������ʾ������һ��buf��
        //��ʼ��һЩrfbserver�Ĳ���
        debug_printf("fram buf = %08x   %d bytes\n",rfbserver->frameBuffer,\
           rfbserver->width*(rfbserver->height)*(rfbserver->rfbServerFormat.bitsPerPixel/8));
        memset(rfbserver->frameBuffer,0,rfbserver->width*(rfbserver->height)*(rfbserver->rfbServerFormat.bitsPerPixel/8));
        rfbserver->clip_head=NULL;

        //���ظ�ʽ
     //   rfbserver->rfbServerFormat.bitsPerPixel=rfbserver->bitsPerPixel;
        //�о�rfbScreen����ɫ�ʵ������е����࣬���Կ���ֻ��format�ͺ��ˣ���������--TODO
        rfbserver->rfbServerFormat.bigEndian=false;
        rfbserver->rfbServerFormat.trueColour=true;
        rfbserver->paddedWidthInBytes=(rfbserver->rfbServerFormat.bitsPerPixel/8)*rfbserver->width;
        switch(rfbserver->rfbServerFormat.bitsPerPixel)//Ŀǰ���о��⼸�ְɣ����������в���
        {
            case 8://332��ʽ
                debug_printf("the server vmirror_display used 332 format!\n");
                rfbserver->rfbServerFormat.redShift=5;
                rfbserver->rfbServerFormat.greenShift=2;
                rfbserver->rfbServerFormat.blueShift=0;
                rfbserver->rfbServerFormat.redMax=((1<<3)-1);
                rfbserver->rfbServerFormat.greenMax=((1<<3)-1);
                rfbserver->rfbServerFormat.blueMax=((1<<2)-1);
                rfbserver->rfbServerFormat.depth=8;
                break;
            case 16://565��ʽ
                debug_printf("the server vmirror_display used 565 format!\n");
                rfbserver->rfbServerFormat.redShift=11;
                rfbserver->rfbServerFormat.greenShift=5;
                rfbserver->rfbServerFormat.blueShift=0;
                rfbserver->rfbServerFormat.redMax=((1<<5)-1);
                rfbserver->rfbServerFormat.greenMax=((1<<6)-1);
                rfbserver->rfbServerFormat.blueMax=((1<<5)-1);
                rfbserver->rfbServerFormat.depth=16;
                break;
            case 32://���ɫ888
                debug_printf("the server vmirror_display used 888 format!\n");
                rfbserver->rfbServerFormat.redShift=16;
                rfbserver->rfbServerFormat.greenShift=8;
                rfbserver->rfbServerFormat.blueShift=0;
                rfbserver->rfbServerFormat.redMax=((1<<8)-1);
                rfbserver->rfbServerFormat.greenMax=((1<<8)-1);
                rfbserver->rfbServerFormat.blueMax=((1<<8)-1);
                rfbserver->rfbServerFormat.depth=24;
                break;
            default://��Ӧ�ô�����������أ������޲�bug--TODO
                break;
        }

        vncscreen=rfbserver;
        vnc_set_vncclip_pool(vncclip_pool);
        rfbserver->mirror_display = mirror_display;
        return ret;
    }
    else
    {
        Mb_DeletePool(vncclip_pool);
        free(mirror_display);
        debug_printf("#fatalerror--install the mirror display failed!\n");

        return ret;
    }
}

// =============================================================================
// ��������:ж����ʾ��
// �������:rfbserver��rfbserver����ʾ��
// ���������
// ����ֵ:true:�ɹ�  false��ʧ��
// ˵��:�ڰ�װ������ʾ����ʱ�����ǻ��ȡ����ʾ����һЩ��������˿��Գ�ʼ�����ǵ�
//          screen�Ĳ������ȷ�˵�ߴ��Լ����ظ�ʽ���Լ�screen��buf�����룬ͬʱ����Ҳ
//        Ҫ���Լ���clippool������͵ײ��clip��˵����Ĺ�ϵ
// =============================================================================
void  uninstall_vncserver_mirrrordisplay(rfbScreenInfoPtr rfbserver)
{
    XMutexWait(&(rfbserver->frame_mutex));

    free_vnccliplink_space(rfbserver->clip_head);
    Mb_DeletePool(vncclip_pool);
    free(rfbserver->frameBuffer);
    Rsc_DelNode((struct tagRscNode *)(rfbserver->mirror_display));
    free(rfbserver->mirror_display);

    rfbserver->clip_head = NULL;
    rfbserver->frameBuffer = NULL;
    rfbserver->mirror_display = NULL;
    vnc_set_vncclip_pool(NULL);
    vncscreen = NULL;

    XMutexPost(&(rfbserver->frame_mutex));
}


