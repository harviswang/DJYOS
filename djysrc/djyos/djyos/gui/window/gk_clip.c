//----------------------------------------------------
//Copyright (C), 2004-2010,  luoshitian.
//��Ȩ���� (C), 2004-2010,   ������.
//����ģ��: gui
//����:  ������.
//�汾��V1.0.0
//�ļ�����: gui kernel�����������
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2010-04-07
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include "os_inc.h"

#if (cfg_gui_kernel == 1)

#include "gk_inc.h"

struct mem_cell_pool *pg_clip_rect_pool;   //�������ڴ��

//----�ͷ�clip����-------------------------------------------------------------
//����: �ͷ�һ��clip���е����нڵ㣬�ö�����һ��˫��ѭ�����С�
//����: clip��ָ����ͷŶ��е�һ���ڵ��ָ��
//����: ��
//-----------------------------------------------------------------------------
struct clip_rect *__gk_free_clip_queue(struct clip_rect *clip)
{
    struct clip_rect *temp,*temp1;
    if(clip != NULL)
    {
        temp = clip;
        do
        {
            temp1 = temp->next;
            mb_free(pg_clip_rect_pool,temp);
            temp = temp1;
        }while(temp != clip);
    }
    return NULL;
}

//----�ϲ�����򻯰�-----------------------------------------------------------
//����: �Ѹ����ļ���������пɺϲ�������ϲ����򻯰����˼�ǣ������е������Ѿ���
//      ��->�ң�����->�µ�˳�����У��ϲ��󣬶����clip�����ͷš�
//����: clipq: ָ����ϲ��Ķ�����
//����: ��
//-----------------------------------------------------------------------------
struct clip_rect * __gk_combine_clip_s(struct clip_rect *clipq)
{
    struct clip_rect *clip,*clip1,*tempclip;
    if(clipq == NULL)
        return NULL;
    //�ϲ�������
    clip = clipq;
    clip1 = clipq->next;
    //ִ��ˮƽ�ϲ�,����clip�ǰ����Ⱥ���ݵķ�ʽ���ֵģ����ˮƽ�ϲ�ֻ��Ҫ��
    //�����ھ����Ƿ���Ժϲ�����
    while(clip1 != clipq)
    {
        if((clip1->rect.top == clip->rect.top)
            &&(clip1->rect.bottom == clip->rect.bottom)
            &&(clip1->rect.left == clip->rect.right)) //�ɺϲ�
        {
            clip->rect.right = clip1->rect.right;
            clip->next = clip1->next;
            clip1->next->previous = clip;
            mb_free(pg_clip_rect_pool,clip1);
            clip1 = clip->next;
        }
        else if((clip1->rect.top == clip->rect.top) //���ڼ�������ͬ�����
            &&(clip1->rect.bottom == clip->rect.bottom)
            &&(clip1->rect.left == clip->rect.left)
            &&(clip1->rect.right== clip->rect.right))
        {
            clip->next = clip1->next;
            clip1->next->previous = clip;
            mb_free(pg_clip_rect_pool,clip1);
            clip1 = clip->next;
        }else
        {
            clip = clip1;
            clip1 = clip1->next;
        }
    }
    //ִ�д�ֱ�ϲ����ɺϲ��ľ��ο��ܲ����ڣ���������ѭ��������ɡ�
    clip = clipq;
    do{
        clip1 = clip->next;
        while(clip1 != clip)
        {
            if((clip1->rect.left == clip->rect.left)
                &&(clip1->rect.right == clip->rect.right)
                &&(clip->rect.bottom == clip1->rect.top)) //�ɺϲ�
            {
                clip->rect.bottom = clip1->rect.bottom;
                clip1->previous->next = clip1->next;
                clip1->next->previous = clip1->previous;
                tempclip = clip1;
                clip1 = clip1->next;
                mb_free(pg_clip_rect_pool,tempclip);
            }else
                clip1 = clip1->next;
        }
        clip = clip->next;
    }while(clip != clipq);
    return clipq;
}

//----�ϲ�����ͨ�ð�-----------------------------------------------------------
//����: �Ѹ����ļ���������пɺϲ�������ϲ�����򻯰治ͬ���ǣ������е�clip��
//      ������еġ�������:
//      1��������clip����y��x��������ʹ���б������ң����ϡ�������ġ�
//      2������__gk_combine_clip_s������������
//����: clipq: ָ����ϲ��Ķ�����
//����: ��
//-----------------------------------------------------------------------------
struct clip_rect * __gk_combine_clip(struct clip_rect *clipq)
{
    struct clip_rect *min,*now,*start,*temp,*temp_previous,*temp_next;
    struct clip_rect *work;
    s32 top,left,i=0;
    if(clipq == NULL)       //�����ǿյ�
        return NULL;
    else if(clipq->next == clipq)   //����ֻ��һ��clip
        return clipq;
    //������ð�����򷨰�clipq�еļ���������ң����ϡ�������
    //������ܶ�ʱ��ð�ݷ���Ƚ������Ľ�֮��--db
    min = clipq;
    start = clipq;
    now = clipq;
    temp = clipq;
    do{
        top = min->rect.top;
        left = min->rect.left;
        work = min->next;
        do{
            if((work->rect.top < top)
                || ((work->rect.top == top) && (work->rect.left <= left)))
            {
                min = work;
                top = work->rect.top;
                left = work->rect.left;
            }
            work = work->next;
        }while(work != start);
        if(min != start)  //���Ǵ�����ĵ�һ�����轻��min��temp��λ��
        {
            if(temp->next != min)   //Ҫ������min��temp������
            {
                i++;
                if(i == 1)
                    start = min;
                now = temp;
                now->previous->next = min;
                now->next->previous = min;
                min->previous->next = now;
                min->next->previous = now;
                temp_next = min->next;
                temp_previous = min->previous;
                min->previous = now->previous;
                min->next = now->next;
                now->next = temp_next;
                now->previous = temp_previous;
            }
            else                    //Ҫ������min��temp����
            {
                i++;
                if(i == 1)
                    start = min;
                now->previous->next = min;
                min->next->previous = now;
                min->previous = now->previous;
                now->next = min->next;
                now->previous = min;
                min->next = now;
            }
            temp = min;
        }
    }while(min != start);
    return __gk_combine_clip_s(start);
}

//----����ν���---------------------------------------------------------------
//����: �����������Σ��������ཻ��ľ��Ρ�
//����: rect1������1ָ��
//      rect2������2ָ��
//      result�����ؽ��ָ��
//����: true = �������ཻ��false = ���ཻ
//-----------------------------------------------------------------------------
bool_t __gk_get_rect_ints(struct st_rect *rect1,struct st_rect *rect2,
                     struct st_rect *result)
{
    s16 max_left,max_top,min_right,min_bottom;
    //�����������Ͻǵ��������ֵ
    max_left = rect1->left > rect2->left? rect1->left:rect2->left;
    max_top = rect1->top > rect2->top? rect1->top:rect2->top;
    //�����������½ǵ���С����ֵ
    min_right = rect1->right > rect2->right?rect2->right:rect1->right;
    min_bottom = rect1->bottom > rect2->bottom?rect2->bottom:rect1->bottom;
    if((min_right <= max_left) || (min_bottom <= max_top))
        return false;
    else
    {
        result->left = max_left;
        result->top = max_top;
        result->right = min_right;
        result->bottom = min_bottom;
        return true;
    }
}

//----ɨ�������---------------------------------------------------------------
//����: 1����visible_clip���ݵ�visible_bak�С�
//      2�����д��������µ�visible_clip
//����: display����ɨ�����ʾ��
//����: false=ʧ�ܣ�һ������Ϊ���������������
//-----------------------------------------------------------------------------
bool_t __gk_scan_new_visible_clip(struct display_rsc *display)
{
    struct gkwin_rsc *tempwin;
    struct st_rect *rect;
    struct clip_rect *clip,*clip1,*clip_head = NULL;
    u32 num,rect_left,rect_top,rect_right,rect_bottom,loop,temp;
    u8 *sort_array_x,*sort_array_y;

    sort_array_x = m_malloc((display->width+1)*sizeof(u8) 
                            + (display->height+1)*sizeof(u8),0);
    if(sort_array_x == NULL)
        return false;
    sort_array_y = sort_array_x + display->width+1;
    for(num = 0; num <= display->width; num++)
        sort_array_x[num] = 0;
    for(num = 0; num <= display->height; num++)
        sort_array_y[num] = 0;
    tempwin = display->z_topmost;
    rect_left = 0;
    rect_top = 0;
    while(1)
    {     //ȡ���ڿ��ӱ߿򣬸ñ߿��Ǵ��������ȴ������ƺ�ľ���
        temp = tempwin->limit_left-tempwin->left + tempwin->absx0;
        if(sort_array_x[temp] == 0)
        {
            sort_array_x[temp] = 1;
            rect_left++;
        }
        
        temp = tempwin->limit_right-tempwin->left+tempwin->absx0;
        if(sort_array_x[temp] == 0)
        {
            sort_array_x[temp] = 1;
            rect_left++;
        }
        
        temp = tempwin->limit_top-tempwin->top + tempwin->absy0;
        if(sort_array_y[temp] == 0)
        {
            sort_array_y[temp] = 1;
            rect_top++;
        }
        
        temp = tempwin->limit_bottom-tempwin->top+tempwin->absy0;
        if(sort_array_y[temp] == 0)
        {
            sort_array_y[temp] = 1;
            rect_top++;
        }
        
        //ִ��__gk_get_redraw_clip_all����ע���е�step1
        //���洰��ԭ���Ŀ�����
        tempwin->visible_bak = tempwin->visible_clip;
        tempwin->visible_clip = NULL;
        if(tempwin != display->desktop)
            tempwin = tempwin->z_back;
        else
            break;
    }
    //���������µĴ��ڣ���Щ���ڿ϶�û�п�����
    while(1)
    {
        tempwin = tempwin->z_back;
        if(tempwin == display->z_topmost)
            break;
        tempwin->visible_bak = tempwin->visible_clip;
        tempwin->visible_clip = NULL;
    }
    if(mb_query_free(pg_clip_rect_pool) < (rect_left-1)*(rect_top-1))
    {
        m_free(sort_array_x);
        return false;           //�ڴ�ز��㣬�޷����ɿ��������
    }
    //�������ң����ϵ��µ�˳��(˳���ܸı�)�����д�ֱ�ߡ�ˮƽ��Χ�ɵ�Сclip
    //����˫��������clip_head������ͷ
    rect_top = 0;
    temp = 0;
    for(rect_bottom =1;rect_bottom <= display->height; rect_bottom++)
    {
        if(sort_array_y[rect_bottom] == 0)
            continue;
        rect_left = 0;
        for(rect_right =1;rect_right <= display->width; rect_right++)
        {
            if(sort_array_x[rect_right] == 0)
                continue;
            clip = (struct clip_rect*)mb_malloc(pg_clip_rect_pool,0);
            clip->rect.left = rect_left;
            clip->rect.right = rect_right;
            clip->rect.top = rect_top;
            clip->rect.bottom = rect_bottom;
            rect_left = rect_right;
            __gk_clip_connect(&clip_head,clip);      //��Сclip���뵽��������
            temp++;
        }
        rect_top = rect_bottom;
    }
    m_free(sort_array_x);

    //�����ж�Сclip�Ĺ������������Ǽ��뵽����win��new_clip������
    tempwin = display->z_topmost;
    while(1)
    {
        clip = clip_head;
        for(loop = temp; loop >0; loop--)
        {
            rect = &(clip->rect);
            if((rect->left>=tempwin->limit_left-tempwin->left+tempwin->absx0)
               &&(rect->top>=tempwin->limit_top-tempwin->top+tempwin->absy0)
               &&(rect->right<=tempwin->limit_right-tempwin->left+tempwin->absx0)
               &&(rect->bottom<=tempwin->limit_bottom-tempwin->top+tempwin->absy0))
            {   //������tempwin�Ŀ���ʾ��Χ��,�����������账��
                //����alpha��͸�������򽫼��봰�ڿ����򣬵�������ʱ������ɾ����
                if(tempwin->dest_blend)
                {
                    clip1 = (struct clip_rect*)mb_malloc(pg_clip_rect_pool,0);
                    *clip1 = *clip;
                }else   //������͸����alpha��������봰�ڿ����򣬴���ʱ����ɾ��
                {
                    if(clip == clip_head)
                        clip_head = clip_head->next;
                    clip1 = clip;
                    clip->previous->next = clip->next;
                    clip->next->previous = clip->previous;
                    temp--;
                }
                clip = clip->next;
                //��Сclip���뵽visible_clip������
                __gk_clip_connect(&tempwin->visible_clip,clip1);
            }else       //���β���tpwin�Ŀ���ʾ��Χ�ڣ����账��
            {
                clip = clip->next;
            }
        }
        __gk_combine_clip_s(tempwin->visible_clip);//�ϲ�clip,����x��y��˳��ϲ�
        if(tempwin != display->desktop)
            tempwin = tempwin->z_back;
        else
            break;
    }
    return true;
}

//----�ָ�������---------------------------------------------------------------
//����: �ͷŸ����ڵ�visible_clip������visible_bak�ָ���visible_clip��
//----�����¿�����-------------------------------------------------------------
//����: ȫ�����������µĿ�������У���ԭ���Ŀ�����ŵ�visible_bak�У����ͷ�ԭ��
//      ��visible_bak����:Ȼ�������µĿ���������������������뵽redraw_clip
//      �����С���ɵ���__gk_get_redraw_clip_all����ע���е�step1~3��
//����: display����ɨ�����ʾ��
//����: false=ʧ�ܣ�һ������Ϊ���������������
//-----------------------------------------------------------------------------
bool_t __gk_get_visible_clip(struct display_rsc *display)
{
    struct gkwin_rsc *tempwin;
    if(__gk_scan_new_visible_clip(display) == false)
        return false;
    tempwin = display->z_topmost;
    while(1)
    {
        //����visible_clip
        __gk_copy_visible_clip(tempwin);
//        if(tempwin->change_flag == cn_gkwin_change_all)
//        {
//            //ȫ�����������ػ棬
//            __gk_cliplink_connect(&tempwin->redraw_clip,tempwin->visible_clip);
//            tempwin->visible_clip = NULL;   //�������Ѿ�ȫ�������ػ���
//            tempwin->change_flag = cn_gkwin_change_none;
//        }
//        else
        {
            __gk_get_new_clip(tempwin);         //�������������redraw_clip 
        }
        if(tempwin != display->desktop)
            tempwin = tempwin->z_back;
        else
            break;
    }
    return true;
}

//----����visible_clip---------------------------------------------------------
//����: ����һ����ʾ�������д��ڵ�ȫ��������
//����: display��Ŀ����ʾ��
//����: ��
//-----------------------------------------------------------------------------
void __gk_copy_visible_clip(struct gkwin_rsc *gkwin)
{
    struct clip_rect *clip1,*clip2,*clip_rsc;
    
    clip_rsc = gkwin->visible_clip;
    if(clip_rsc != NULL)
    {
        //������һ���ڵ�
        clip1 = (struct clip_rect*)mb_malloc(pg_clip_rect_pool,0);
        clip1->rect = clip_rsc->rect;
        gkwin->copy_clip = clip1;
        clip_rsc = clip_rsc->next;
        while(clip_rsc != gkwin->visible_clip)
        {
            clip2 = (struct clip_rect*)mb_malloc(pg_clip_rect_pool,0);
            clip2->rect = clip_rsc->rect;
            clip_rsc = clip_rsc->next;
            clip1->next = clip2;
            clip2->previous = clip1;
            clip1 = clip2;
        }
        //��������ʹcopy_clip�����γɱջ�
        gkwin->copy_clip->previous = clip1;
        clip1->next = gkwin->copy_clip;
    }
}

//----���������������һ�����������-------------------------------------------
//����: ��sub���뵽����mlink�С�
//����: clip_link��Ŀ�����
//      sub��������Ķ���
//����: ��
//-----------------------------------------------------------------------------
void __gk_cliplink_connect(struct clip_rect **mlink,struct clip_rect *sub)
{
    struct clip_rect *temp;
    if(*mlink == NULL)
        *mlink = sub;
    else if(sub != NULL)
    {
        temp = sub->previous;
        sub->previous = (*mlink)->previous;
        (*mlink)->previous->next = sub;
        (*mlink)->previous = temp;
        temp->next = (*mlink);
    }
}
//----���������������һ���ڵ�-------------------------------------------------
//����: ��clip���뵽����clip_link�У��½�����clip_link���ǰ�档
//����: clip_link��Ŀ������еĲ����
//      clip��������Ľڵ�
//����: ��
//-----------------------------------------------------------------------------
void __gk_clip_connect(struct clip_rect **clip_link,struct clip_rect *clip)
{
    if(*clip_link == NULL)
    {
        *clip_link = clip;
        clip->next = clip;
        clip->previous = clip;
    }else
    {
        clip->next = *clip_link;
        clip ->previous = (*clip_link)->previous;
        (*clip_link)->previous->next = clip;
        (*clip_link)->previous = clip;
    }
}

//----��ȡ���޸�����-----------------------------------------------------------
//����: ��ȡһ�����ڵı��޸Ĳ��ֵļ������γ�˫��ѭ�����������е�clip����
//      ��->�ң�����->�µ�˳�����У�������changed_msk���顣
//����: gkwin��Ŀ�괰��
//����: ����ָ�룬NULL��ʾ����δ���޸�
//-----------------------------------------------------------------------------
struct clip_rect *__gk_get_changed_clip(struct gkwin_rsc *gkwin)
{
    struct clip_rect *clip=NULL,*clip1,*tempclip,*clip_head = NULL;
    u8 *msk;
    u32 offset_bit,offset,msk_line_words;
    s16 loopx,loopy,loopz,loop=0;
    bool_t start;
    s32 width,height;

    if(gkwin->change_flag == cn_gkwin_change_all)   //�������ھ����޸�
    {
        clip = (struct clip_rect *)mb_malloc(pg_clip_rect_pool,0);
        clip->rect.left = gkwin->limit_left + gkwin->absx0;
        clip->rect.top = gkwin->limit_top + gkwin->absy0;
        clip->rect.right = gkwin->limit_right;
        clip->rect.bottom = gkwin->limit_bottom;
        clip->next = clip;
        clip->previous = clip;
        gkwin->change_flag = cn_gkwin_change_none;
        return clip;            //���������ڵĿ���ʾ������һ�������򷵻�
    }else if(gkwin->change_flag == cn_gkwin_change_none)    //û���޸�
    {
        return NULL;            
    }else       //�����޸ģ��Ȱ�x����ȡ�ü������ٰ�y����ϲ�
    {
        offset = 0;                 //������ƫ����
        msk = gkwin->changed_msk.bm_bits;
        width = gkwin->wm_bitmap.width;
        height = gkwin->wm_bitmap.height;
        msk_line_words = gkwin->changed_msk.linebytes;
        for(loopy = 0; loopy < height; loopy +=8)
        {
            start = false;      //��ʼһ�������ı��޸�����
            offset_bit = 0;     //��ʼ���λƫ��
            for(loopx = 0; loopx < width; loopx+=8)
            {
                if(msk[offset+offset_bit/8]&(u8)(1<<(8-(offset_bit%8)-1)))
                {   //�ÿ��changed flagΪtrue��
                    if(start == false)      //changed block��ʼ���޸�
                    {
                        start = true;
                        clip =(struct clip_rect*)mb_malloc(pg_clip_rect_pool,0);
                        clip->rect.left = gkwin->absx0 + offset_bit*8;
                        clip->rect.top = loopy + gkwin->absy0;
                        clip->rect.right = clip->rect.left + 8;
                        clip->rect.bottom = clip->rect.top + 8;
                    }else                   //changed block����
                    {
                        clip->rect.right += 8;
                    }
                }else     //������е����һ��Ҳ�Ǳ���д�Ŀ飬����򲻻��ߵ�����
                {
                    if(start == true)     //changed block����
                    {
                        __gk_clip_connect(&clip_head,clip);
                        start = false;
                    }
                }
                offset_bit++;
            }
            //�������һ���Ǳ���д�飬��Ҫ������Ѹ�������뱻�޸�����
            if(start == true)   
                __gk_clip_connect(&clip_head,clip);
            for(loopz = 0; loopz < msk_line_words; loopz++)  //changed_msk����
                msk[loopz+loop] = 0;
            loop += msk_line_words;
            offset += msk_line_words;
        }

        //���ˣ�x������״clip�Ѿ����ɣ�������y����ϲ�
        clip = clip_head;
        do{
            clip1 = clip->next;
            while(clip1 != clip)
            {
                if((clip1->rect.left == clip->rect.left)
                    &&(clip1->rect.right == clip->rect.right)
                    &&((clip->rect.bottom)== clip1->rect.top))//�ɺϲ�
                {
                    clip->rect.bottom += 8;
                    clip1->previous->next = clip1->next;
                    clip1->next->previous = clip1->previous;
                    tempclip = clip1;
                    clip1 = clip1->next;
                    mb_free(pg_clip_rect_pool,tempclip);
                }else
                    clip1 = clip1->next;
            }
            clip = clip->next;
        }while(clip != clip_head);
        gkwin->change_flag = cn_gkwin_change_none;
    }
    return clip_head;
}

//----��ȡ����������ص�����---------------------------------------------------
//����: ��ȡsrc�����е����о�����sub�������ص��Ĳ��֣���ins�������أ�
//      src�з��ص��Ĳ���Ҳ�γ��µļ�������У���different���ء�
//����: src�������Ķ���ָ�룬ִ�к󣬱����н����ͷŻ�ת��
//      sub����������ָ��
//      ins��src��sub�ཻ�Ĳ���
//      different��src��sub���ཻ�Ĳ���
//����: ��
//-----------------------------------------------------------------------------
void __gk_cliplink_sub(struct clip_rect *src,struct clip_rect *sub,
                       struct clip_rect **ins,struct clip_rect **different)
{
    struct clip_rect *difftemp = NULL;  //���ص��Ĳ����ݴ�
    struct clip_rect *worksrc;          //src�о��α��������Ĺ����Ӷ���
    struct clip_rect *end;
    struct clip_rect *workloop;         //�����Ӷ��е�ѭ������ָ��
    struct clip_rect *subloop;          //�������е�ѭ������ָ��
    struct clip_rect *clip_ints=NULL;   //�����ռ��ص�������
    struct clip_rect *temp1;            //��ʱ����
    struct st_rect ints_rect,src_rect;
    bool_t endloop;
    ufast_t uftemp;
    //��������Ϊ��
    if(sub == NULL)
    {   //����������Ϊ��
        if(src == NULL)
        {//src��sub�ཻ�����ཻ�Ĳ��ֶ�Ϊ��
            *ins = NULL;
            *different = NULL;
            return;
        }else
        {//src��sub�ཻ����Ϊ�ա����ཻ�Ĳ���Ϊsrc
            *ins = NULL;
            *different = src;
            return;
        }
    }else //�������в�Ϊ��
    {   //����������Ϊ��
        if(src == NULL)
        {//src��sub�ཻ�����ཻ�Ĳ��ֶ�Ϊ��
            *ins = NULL;
            *different = NULL;
            return;
        }
    }
    //���������С��������ж���Ϊ��
    src->previous->next = NULL;
    while(src != NULL)
    {
        worksrc = src;
        src = src->next;
        worksrc->next = worksrc;
        worksrc->previous = worksrc;
        subloop = sub;
        do{
            workloop = worksrc;
            end = worksrc->previous;
            endloop = false;
            //�˴���ѭ���Ǳ�Ҫ�ģ���sub�е�һ������ʱworksrcֻ��һ�����Σ���
            //�Բ���ѭ�������������ڶ�������ʱ��worksrc���ܷ��ѳ�һ�����С�
            do{
                if(workloop == end)
                    endloop = true;
                if(__gk_get_rect_ints(&workloop->rect,&subloop->rect,&ints_rect))
                {//�����ཻ
                    temp1 = (struct clip_rect *)mb_malloc(pg_clip_rect_pool,0);
                    temp1->rect = ints_rect;
                    //���ཻ�������clip_ints
                    __gk_clip_connect(&clip_ints,temp1);
                    src_rect = workloop->rect;
                    uftemp = 0;
                    //������ȡworkloop����subloop�в��ཻ�Ĳ��֣�ֱ�����src����
                    //��workloop��λ��
                    if(subloop->rect.top > src_rect.top)        //�ϲ�����
                    {   //�׸����Σ�����ԭ���
                        workloop->rect.bottom = subloop->rect.top;   
                        workloop = workloop->next;
                        uftemp++;
                    }
                    if(subloop->rect.left > src_rect.left)      //�󲿾���
                    {   //�׸����Σ�����ԭ���
                        if(uftemp == 0)     
                        {
                            workloop->rect.right = subloop->rect.left;
                            if(src_rect.bottom < subloop->rect.bottom)
                                workloop->rect.bottom = src_rect.bottom;
                            else
                                workloop->rect.bottom = subloop->rect.bottom;
                            workloop = workloop->next;
                        }else
                        {
                            temp1 = 
                             (struct clip_rect *)mb_malloc(pg_clip_rect_pool,0);
                            temp1->rect.left = src_rect.left;
                            temp1->rect.top = subloop->rect.top;
                            temp1->rect.right = subloop->rect.left;
                            if(src_rect.bottom < subloop->rect.bottom)
                                temp1->rect.bottom = src_rect.bottom;
                            else
                                temp1->rect.bottom = subloop->rect.bottom;
                            __gk_clip_connect(&workloop,temp1);
                        }
                        uftemp++;
                    }
                    if(subloop->rect.right < src_rect.right)    //�Ҳ�����
                    {   //�׸����Σ�����ԭ���
                        if(uftemp == 0)     
                        {
                            workloop->rect.left = subloop->rect.right;
                            if(src_rect.bottom < subloop->rect.bottom)
                                workloop->rect.bottom = src_rect.bottom;
                            else
                                workloop->rect.bottom = subloop->rect.bottom;
                            workloop = workloop->next;
                        }else
                        {
                            temp1 = 
                             (struct clip_rect *)mb_malloc(pg_clip_rect_pool,0);
                            temp1->rect.left = subloop->rect.right;
                            if(src_rect.top < subloop->rect.top)
                                temp1->rect.top = subloop->rect.top;
                            else
                                temp1->rect.top = src_rect.top;
                            temp1->rect.right = src_rect.right;
                            if(src_rect.bottom < subloop->rect.bottom)
                                temp1->rect.bottom = src_rect.bottom;
                            else
                                temp1->rect.bottom = subloop->rect.bottom;
                            __gk_clip_connect(&workloop,temp1);
                        }
                        uftemp++;
                    }
                    if(subloop->rect.bottom < src_rect.bottom)      //�²�����
                    {   //�׸����Σ�����ԭ���
                        if(uftemp == 0)     
                        {
                            workloop->rect.top = subloop->rect.bottom;
                            workloop = workloop->next;
                        }else
                        {
                            temp1 = 
                             (struct clip_rect *)mb_malloc(pg_clip_rect_pool,0);
                            temp1->rect.left = src_rect.left;
                            temp1->rect.top = subloop->rect.bottom;
                            temp1->rect.right = src_rect.right;
                            temp1->rect.bottom = src_rect.bottom;
                            __gk_clip_connect(&workloop,temp1);
                        }
                        uftemp++;
                   }
                   if(uftemp == 0)                    //Դ���α���ȫ����,
                   {
                        if(worksrc->next == worksrc)    //Դ����ֻ��һ������
                        {
                            mb_free(pg_clip_rect_pool,worksrc);
                            worksrc = NULL;
                            workloop = NULL;
                        }else
                        {
                            temp1 = workloop;
                            workloop = workloop->next;
                            temp1->previous->next = workloop;
                            workloop->previous = temp1->previous;
                            if(worksrc == temp1)
                                worksrc = workloop;
                            mb_free(pg_clip_rect_pool,temp1);
                        }
                    }
                }
                else           //���β��ཻ
                    workloop = workloop->next;
            }while(!endloop);
            if(worksrc == NULL)
                break;
            subloop = subloop->next;
        }while(subloop != sub);
        __gk_cliplink_connect(&difftemp,worksrc);
    }
    *different = difftemp;
    *ins = clip_ints;
}
//----ȡ������������-----------------------------------------------------------
//����: ������visible_clip���Ҳ���visible_bak�����򣬼���redraw_clip��
//      ͬʱ������visible_clip��visible_bak�еĲ��֣�����visible_clip��
//      ���dest_blend==true�������visible_bak���ֵĲ�������visible_bak
//      �����ͷ�visiblvisible_bak
//����: gkwin��Ŀ�괰��
//����: ��
//-----------------------------------------------------------------------------
void __gk_get_new_clip(struct gkwin_rsc *gkwin)
{
    struct clip_rect *redraw;       //visible_clip-visible_bak����Ĳ���
    struct clip_rect *res;          //visible_clip��visible_bak�ཻ��clip
    
    if(gkwin->visible_bak != NULL)
    {
        __gk_cliplink_sub(gkwin->visible_clip,gkwin->visible_bak,&res,&redraw);
        __gk_cliplink_connect(&gkwin->redraw_clip,redraw);
        gkwin->visible_clip = res;
        if(gkwin->dest_blend)
        {
            __gk_cliplink_sub(gkwin->visible_bak,gkwin->visible_clip,
                              &res,&redraw);
            gkwin->visible_bak = redraw;
            __gk_free_clip_queue(res);
        }
        else
        {
            __gk_free_clip_queue(gkwin->visible_bak);
            gkwin->visible_bak = NULL;
        }
    }
    else 
    {        
        __gk_cliplink_connect(&gkwin->redraw_clip,gkwin->visible_clip);
        gkwin->visible_clip = NULL;
    }
}

//----��ȡ�����򽻼�-----------------------------------------------------------
//����: ��ȡsrcclip��������desclip�ཻ���֣����ཻ�Ĳ�����Ȼ����srcclip�С�
//����: srcclip��ԭ����������
//      desclip��Ŀ�����������
//����: ��ȡ�Ŀ�����NULL = û���ཻ����
//-----------------------------------------------------------------------------
struct clip_rect *__gk_get_cliplink_ints(struct clip_rect **srcclip,
                                             struct clip_rect *desclip)
{
    struct clip_rect *redraw,*res;
    //��ȡ����������ص�����
    __gk_cliplink_sub(*srcclip,desclip,&redraw,&res);
    *srcclip = res;
    return redraw;
}

u32 clip_sum;
void tongji(struct display_rsc *display)
{
    struct gkwin_rsc *gkwin;
    struct clip_rect *clip;
    clip_sum = 0;
    gkwin = display->z_topmost;
    while(1)
    {
        gkwin->bak_num = 0;
        gkwin->vis_num = 0;
        gkwin->changed_num = 0;
        gkwin->copy_num = 0;
        gkwin->redraw_num = 0;
        
        clip = gkwin->visible_bak;
        if(clip != NULL)
        {
            do
            {
                clip_sum++;
                gkwin->bak_num++;
                clip = clip->next;
            }while(clip != gkwin->visible_bak);
        }
        else
            gkwin->bak_num = 0;
        
        clip = gkwin->visible_clip;
        if(clip != NULL)
        {
            do
            {
                clip_sum++;
                gkwin->vis_num++;
                clip = clip->next;
            }while(clip != gkwin->visible_clip);
        }
        else
            gkwin->vis_num = 0;
        
        clip = gkwin->changed_clip;
        if(clip != NULL)
        {
            do
            {
                clip_sum++;
                gkwin->changed_num++;
                clip = clip->next;
            }while(clip != gkwin->changed_clip);
        }
        else
            gkwin->changed_num = 0;
        
        clip = gkwin->copy_clip;
        if(clip != NULL)
        {
            do
            {
                clip_sum++;
                gkwin->copy_num++;
                clip = clip->next;
            }while(clip != gkwin->copy_clip);
        }
        else
            gkwin->copy_num = 0;
        
        clip = gkwin->redraw_clip;
        if(clip != NULL)
        {
            do
            {
                clip_sum++;
                gkwin->redraw_num++;
                clip = clip->next;
            }while(clip != gkwin->redraw_clip);
        }
        else
            gkwin->redraw_num = 0;
        gkwin = gkwin->z_back;
        if(gkwin == display->z_topmost)
            break;
    }
}
//----�����ػ�����-------------------------------------------------------------
//����: ɨ����������������������ȫ�����ڵĿ�����,�Լ����ػ��clip�������͹���:
//      1���ͷ����е�visible_bak����visible_clipת��visible_bak�С�
//      2��������������win��visible_clip��
//      3��visible_clip��ȥ��visible_bak���ü���redraw_clip���С�ʣ�ಿ�ֱ�����
//          visible_clip�С�������ڵ�dest_blend == true����visible_bak�б���
//          �ɿɼ���Ϊ���ɼ��Ĳ��֣�����visible_bak�����ͷš�
//      4����changed msk����ȡchanged_clip��
//      5��ȡvisible_clip��changed_clip�Ľ���������redraw_clip���С�
//      6����z_top��ȡvisible_bak,ɨ����󴰿ڵĿ����������ص��Ĳ��֣�����
//         redraw��
//      7����z_top�𣬶�dest_blend == true��win��ɨ��z������������
//         win��visible_clip�������ػ沿���ص��ģ�����redraw_clip���С�
//         �˺�dest_blend != true��win��visible_clip���ͷš�
//      8���ϲ�ǰ����������redraw_clip���С�
//      9����z_top�𣬶���һdest_blend == true��win��visible_clip��ɨ��z��������
//         �����win��redraw_clip���ص��Ĳ��ּ���redraw_clip���У����ϲ�֮��
//����: display����ɨ�����ʾ��
//����: false=ʧ�ܣ�һ������Ϊ���������������
//-----------------------------------------------------------------------------
bool_t __gk_get_redraw_clip_all(struct display_rsc *display)
{
    struct gkwin_rsc *tempwin;         
    struct gkwin_rsc *special_win;
    struct clip_rect *changed_clip,*tempclip1;
    if(display->reset_clip == true)         //���Ƿ���Ҫ�������ɿ�����
    {
        __gk_get_visible_clip(display);     //���step1��step2��step3
        display->reset_clip = false;
    }
    tempwin = display->desktop;
    while(1)
    {   //ɨ��desktop��z_topmost֮���ȫ�����ڣ����step4~5����desktop�����
        //���ڲ���ʾ�����账��
        if(tempwin->visible_clip != NULL)
        {
            //����visible_clip
            if(tempwin->copy_clip == NULL)  //copy_clip�����Ѿ���step1~3��������
                __gk_copy_visible_clip(tempwin);
            //��������ȫ�����޸ģ�������ơ������ƶ������졢ѹ���ᵼ��ȫ���޸�
            //reset_clip == true�����Ļ���cn_gkwin_change_all�������
            //__gk_get_visible_clip�������Ѿ���ɣ��ڸú�������ɣ����Լ��᲻��
            //�����������Ҷ�����û��Ӱ�졣
            if(tempwin->change_flag == cn_gkwin_change_all) 
            {
                //ȫ�����������ػ�
				__gk_cliplink_connect(&tempwin->redraw_clip,
                                       tempwin->visible_clip);
                tempwin->visible_clip = NULL;   //�������Ѿ�ȫ�������ػ���
                tempwin->change_flag = cn_gkwin_change_none;
            }else
            {   //�в����޸�
                if(tempwin->change_flag == cn_gkwin_change_part)
                {
                    //ȡ���޸���
                    changed_clip = __gk_get_changed_clip(tempwin);
                    //��ȡ���������޸ĵ�����
                    tempclip1 = __gk_get_cliplink_ints(
                                    &tempwin->visible_clip,changed_clip);
                    __gk_free_clip_queue(changed_clip);
                    //���޸�������뵽redraw_clip��
                    __gk_cliplink_connect(&tempwin->redraw_clip,tempclip1);
                    tempwin->change_flag = cn_gkwin_change_none;
                }
            }
//              tempwin->visible_bak = NULL;
        }
        //���changed_msk
        __gk_shading_clear(tempwin);
        if(tempwin != display->z_topmost)
            tempwin = tempwin->z_top;
        else
            break;
    }

    //ִ��step6���ı�λ�õġ�dest_blend=true�Ĵ��ڣ��ı�ǰ�ڵ����������ڵ�
    //�����򣬼���redraw��
    special_win = display->z_topmost;
    while(1)
    {   
        //�����ƶ��������ᵼ��visible_bak != NULL
        if((special_win->dest_blend) && (special_win->visible_bak != NULL))
        {
            tempwin = special_win->z_back;
            while(1)
            {
                //ȡ�ñ���������Ҫ�ػ������
                tempclip1 =
                	__gk_get_cliplink_ints(&special_win->visible_bak,
                                            tempwin->visible_clip);
                
                //���޸�������뵽��������redraw_clip��
                __gk_cliplink_connect(&tempwin->redraw_clip,tempclip1);
                if(tempwin != display->desktop)
                    tempwin = tempwin->z_back;
                else
                    break;
            }
            special_win->visible_bak = __gk_free_clip_queue(special_win->visible_bak);
        }
        if(special_win != display->desktop)
            special_win = special_win->z_back;
        else
            break;
    }
    
    //ִ��step7��������Ҫ�����������㣬�˴��������иı�
    special_win = display->z_topmost;
    while(1)
    {   //hjj,step7��step9�����Ⱥ�ִ�е����������__gk_get_cliplink_ints_res
        //��Բ�����Ӱ��(���ܵ��²���ֵ�ı�)��ʹ��ʱӦ��������
        if(special_win->dest_blend)
        {//special_win��Ҫ�������������ִ������ĳ���
            if(special_win->redraw_clip != NULL)
            {
                tempwin = special_win->z_back;
                while(1)
                {
                    //ȡ�ñ���������Ҫ�ػ������
                    tempclip1 =
                    	__gk_get_cliplink_ints(&tempwin->visible_clip,
                                                special_win->redraw_clip);
                    
                    //���޸�������뵽��������redraw_clip��
                    __gk_cliplink_connect(&tempwin->redraw_clip,tempclip1);
                    if(tempwin != display->desktop)
                        tempwin = tempwin->z_back;
                    else
                        break;
                }
            }
        }
        else
        {//�ͷŵ�����Ҫ������������Ĵ��ڵĿ�����
            special_win->visible_clip = 
                            __gk_free_clip_queue(special_win->visible_clip);
        }
        if(special_win != display->desktop)
            special_win = special_win->z_back;
        else
            break;
    }
    
    //ִ��step8���ϲ�ǰ����������redraw_clip����
    tempwin = display->z_topmost;
    while(1)
    {
        //�ϲ�redraw_clip
        tempwin->redraw_clip = __gk_combine_clip(tempwin->redraw_clip);
        if(tempwin != display->desktop)
            tempwin = tempwin->z_back;
        else
            break;
    }

    //ִ��step9�������������ж�desktop����ѭ����������ͬ
    //��Ҫ������������Ĵ��ڣ��䱳���ı�
    special_win = display->z_topmost;
    do{
        if(special_win->dest_blend)            
        {//special_win��Ҫ�������ڲ������㣬��ִ������ĳ���
            tempwin = special_win;
            do{ 
                tempwin = tempwin->z_back;
                //ȡ�ñ������ڵ��޸���
                tempclip1 =
                	__gk_get_cliplink_ints(&special_win->visible_clip,
                                              tempwin->redraw_clip);
                //���޸�������뵽���ڵ�redraw_clip��
                __gk_cliplink_connect(&special_win->redraw_clip,tempclip1);
            }while(tempwin != display->desktop);
            //�ͷŵ�����Ҫʹ�õ�����
            special_win->visible_clip = 
                            __gk_free_clip_queue(special_win->visible_clip);
            //�ϲ�redraw_clip
            special_win->redraw_clip =
                            __gk_combine_clip(special_win->redraw_clip);
        }
        special_win = special_win->z_back;
    }while(special_win != display->desktop);

    //�ָ�visible_clip
    tempwin = display->z_topmost;
    while(1)
    {
        tempwin->visible_clip = tempwin->copy_clip;
        tempwin->copy_clip = NULL;
        if(tempwin != display->desktop)
            tempwin = tempwin->z_back;
        else
            break;
    }
    return true;
}
//----���ɿ�����---------------------------------------------------------------
//����: ����z1(��)��z2(��)֮��Ĵ��ڵĿ�����
//����: z1��z���п���Ĵ��ڣ���������
//      z2��z���п�ǰ�Ĵ��ڣ���������
//����: ��
//-----------------------------------------------------------------------------
void __gk_get_clip_section( struct gkwin_rsc *z1,struct gkwin_rsc *z2)
{
}

#endif //   for #if (cfg_gui_kernel == 1)

