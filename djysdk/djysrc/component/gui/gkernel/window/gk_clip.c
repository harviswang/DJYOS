//----------------------------------------------------
// Copyright (c) 2014, SHENZHEN PENGRUI SOFT CO LTD. All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:

// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
// 3. As a constituent part of djyos,do not transplant it to other software
//    without specific prior written permission.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------
// Copyright (c) 2014 ����Ȩ����������������޹�˾���С�����Ȩ�˱���һ��Ȩ����
//
// �����Ȩ�����ʹ���߷��������������������£�����ʹ����ʹ�ü���ɢ����
// �����װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б���
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б��Լ�����
//    ������������
// 3. �������Ϊ�����߲���ϵͳ����ɲ��֣�δ����ǰȡ�õ�������ɣ���������ֲ����
//    �����߲���ϵͳ���������С�

// ����������������Ǳ������Ȩ�������Լ�����������״��"as is"���ṩ��
// �������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
//-----------------------------------------------------------------------------

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
#include "stdint.h"
#include "stddef.h"
#include "stdlib.h"
#include "string.h"
#include "pool.h"
#include "gkernel.h"
#include "gk_display.h"
#include "..\include\gk_win.h"
#include "..\include\gk_clip.h"
#include "..\include\gk_draw.h"
struct tagMemCellPool *g_ptClipRectPool;   //�������ڴ��

//----�ͷ�clip����-------------------------------------------------------------
//����: �ͷ�һ��clip���е����нڵ㣬�ö�����һ��˫��ѭ�����С�
//����: clip��ָ����ͷŶ��е�һ���ڵ��ָ��
//����: ��
//-----------------------------------------------------------------------------
struct tagClipRect *__GK_FreeClipQueue(struct tagClipRect *clip)
{
    struct tagClipRect *temp,*temp1;
    if(clip != NULL)
    {
        temp = clip;
        do
        {
            temp1 = temp->next;
            Mb_Free(g_ptClipRectPool,temp);
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
struct tagClipRect * __GK_CombineClip_s(struct tagClipRect *clipq)
{
    struct tagClipRect *clip,*clip1,*tempclip;
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
            Mb_Free(g_ptClipRectPool,clip1);
            clip1 = clip->next;
        }
        else if((clip1->rect.top == clip->rect.top) //���ڼ�������ͬ�����
            &&(clip1->rect.bottom == clip->rect.bottom)
            &&(clip1->rect.left == clip->rect.left)
            &&(clip1->rect.right== clip->rect.right))
        {
            clip->next = clip1->next;
            clip1->next->previous = clip;
            Mb_Free(g_ptClipRectPool,clip1);
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
                Mb_Free(g_ptClipRectPool,tempclip);
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
//����: ���ص����ź�˳��Ķ���
//-----------------------------------------------------------------------------
//is ClipA bottomer and righter than B? 1 yes 0 no
//ˮƽλ���и��ߵ�Ȩ�ر�
static u8 ClipAIsBR2ClipB(struct tagClipRect *ClipA, struct tagClipRect *ClipB)
{
    if((ClipA->rect.top > ClipB->rect.top) || \
    ((ClipA->rect.top == ClipB->rect.top) && (ClipA->rect.left > ClipB->rect.left)))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
struct tagClipRect * __GK_CombineClip(struct tagClipRect *clipq)
{

    struct tagClipRect  *startclip, *endclip,  *maxclip;
    struct tagClipRect  *clippop, *clippopnext, *clippop_pbak, *clippopnext_nbak;
    int    firstTime=0;

    if(clipq == NULL)       //�����ǿյ�
        return NULL;
    else if(clipq->next == clipq)   //����ֻ��һ��clip
        return clipq;
    //������ð�����򷨰�clipq�еļ���������ң����ϡ�������
    //������ܶ�ʱ��ð�ݷ���Ƚ������Ľ�֮��--db
    //startclipΪÿ��ð�ݿ�ʼ�ĵط�
    //endclipΪÿ��ð�ݽ����ĵط�
    //����ķ����ǣ���һ�ν�����maxclip�������������棩��������棬Ȼ��ÿ�ζ�
    //��maxclip->next��ʼ����Ϊ��˫��ѭ��������һ���δ�ģ�ֱ��startclip��
    //endclip��ͬ
    startclip = clipq;
    endclip = startclip->previous;

    do
    {
        clippop = startclip;
        firstTime++;
        do
        {
            clippopnext = clippop->next;
            //clipop��������һ�������£�������Ҫ����AB��λ��
            if(ClipAIsBR2ClipB(clippop, clippopnext))
            {
                if(clippop == clippopnext->next)//�����н���������clip
                {
                    //֮��������������ϣ��Aָ���������maxclip��
                }
                else//��A��B��λ�õ�����ע��A��B�����ڵ�
                {
                    clippop_pbak = clippop->previous;
                    clippopnext_nbak = clippopnext->next;
                    clippop_pbak->next = clippopnext;
                    clippopnext_nbak->previous = clippop;

                    clippop->previous = clippopnext;
                    clippop->next = clippopnext_nbak;
                    clippopnext->next = clippop;
                    clippopnext->previous = clippop_pbak;
                }
            }
            else
            {
                clippop = clippopnext;    //һ�αȽϱ���
            }
        }while(clippopnext != endclip);
        if(firstTime ==1)//��ѭ����һ�ξͿ���֪���������ĸ���
        {
            maxclip = clippop;
        }
        endclip = clippop;
        startclip = maxclip->next;
        endclip = endclip->previous;
        //printf("used times =%d\n",firstTime);
    }while(startclip!= endclip);

    return  __GK_CombineClip_s(startclip);
}



//----����ν���---------------------------------------------------------------
//����: �����������Σ��������ཻ��ľ��Ρ�
//����: rect1������1ָ��
//      rect2������2ָ��
//      result�����ؽ��ָ��
//����: true = �������ཻ��false = ���ཻ
//-----------------------------------------------------------------------------
/*
bool_t __GK_GetRectInts(struct tagRectangle *rect1,struct tagRectangle *rect2,
                     struct tagRectangle *result)
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
*/

//----�ϲ�����ͨ�ð�-----------------------------------------------------------
//����: �Ѹ����ļ���������пɺϲ�������ϲ�����򻯰治ͬ���ǣ������е�clip��
//      ������еġ�������:
//      1��������clip����y��x��������ʹ���б������ң����ϡ�������ġ�
//      2������__gk_combine_clip_s������������
//����: clipq: ָ����ϲ��Ķ�����
//����: ��
//-----------------------------------------------------------------------------
struct tagClipRect * __gk_combine_clip_old(struct tagClipRect *clipq)
{
    struct tagClipRect *min,*now,*start,*temp,*temp_previous,*temp_next;
    struct tagClipRect *work;
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
    return __GK_CombineClip_s(start);
  // check_clip_queque(start);
   //return start;
}
//----����ν���---------------------------------------------------------------
//����: �����������Σ��������ཻ��ľ��Ρ�
//����: rect1������1ָ��
//      rect2������2ָ��
//      result�����ؽ��ָ��
//����: true = �������ཻ��false = ���ཻ
//-----------------------------------------------------------------------------
bool_t __GK_GetRectInts(struct tagRectangle *rect1,struct tagRectangle *rect2,
                        struct tagRectangle *result)
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
bool_t __GK_ScanNewVisibleClip(struct tagDisplayRsc *display)
{
    struct tagGkWinRsc *tempwin;
    struct tagRectangle *rect;
    struct tagClipRect *clip,*clip1,*clip_head = NULL;
    s32 num,rect_left,rect_top,rect_right,rect_bottom,loop,temp;
    u8 *sort_array_x,*sort_array_y;

    num = (display->width+1)*sizeof(u8) + (display->height+1)*sizeof(u8);
    sort_array_x = M_MallocLc(num,0);
    if(sort_array_x == NULL)
        return false;
    sort_array_y = sort_array_x + display->width+1;
    memset(sort_array_x,0,num);
    tempwin = display->z_topmost;
//    rect_left = 0;
//    rect_top = 0;
    while(1)
    {     //ȡ���ڿ��ӱ߿򣬸ñ߿��Ǵ��������ȴ������ƺ�ľ���
        temp = tempwin->limit_left-tempwin->left + tempwin->absx0;
        if(sort_array_x[temp] == 0)
        {
            sort_array_x[temp] = 1;
//            rect_left++;
        }

        temp = tempwin->limit_right-tempwin->left+tempwin->absx0;
        if(sort_array_x[temp] == 0)
        {
            sort_array_x[temp] = 1;
//            rect_left++;
        }

        temp = tempwin->limit_top-tempwin->top + tempwin->absy0;
        if(sort_array_y[temp] == 0)
        {
            sort_array_y[temp] = 1;
//            rect_top++;
        }

        temp = tempwin->limit_bottom-tempwin->top+tempwin->absy0;
        if(sort_array_y[temp] == 0)
        {
            sort_array_y[temp] = 1;
//            rect_top++;
        }

        //ִ��__GK_GetRedrawClipAll����ע���е�step1
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
            clip = (struct tagClipRect*)Mb_Malloc(g_ptClipRectPool,0);
            if(clip != NULL)
            {
                clip->rect.left = rect_left;
                clip->rect.right = rect_right;
                clip->rect.top = rect_top;
                clip->rect.bottom = rect_bottom;
                rect_left = rect_right;
                __GK_ClipConnect(&clip_head,clip);      //��Сclip���뵽��������
                temp++;
            }
            else
            {
                __GK_FreeClipQueue(clip_head);
                free(sort_array_x);
                return false;           //�ڴ�ز��㣬�޷����ɿ��������
            }
        }
        rect_top = rect_bottom;
    }
    free(sort_array_x);

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
                    clip1 = (struct tagClipRect*)Mb_Malloc(g_ptClipRectPool,0);
                    if(clip1 != NULL)
                    {
                        *clip1 = *clip;
                    }
                    else
                    {
                        __GK_FreeClipQueue(clip_head);
                        return false;           //�ڴ�ز��㣬�޷����ɿ��������
                    }
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
                __GK_ClipConnect(&tempwin->visible_clip,clip1);
            }else       //���β���tpwin�Ŀ���ʾ��Χ�ڣ����账��
            {
                clip = clip->next;
            }
        }
        __GK_CombineClip_s(tempwin->visible_clip);//�ϲ�clip,����x��y��˳��ϲ�
        if(tempwin != display->desktop)
            tempwin = tempwin->z_back;
        else
            break;
    }
    return true;
}

//----�����¿�����-------------------------------------------------------------
//����: ȫ�����������µĿ�������У���ԭ���Ŀ�����ŵ�visible_bak�У����ͷ�ԭ��
//      ��visible_bak����:Ȼ�������µĿ���������������������뵽redraw_clip
//      �����С���ɵ��� __GK_GetRedrawClipAll ����ע���е�step1~3��
//����: display����ɨ�����ʾ��
//����: false=ʧ�ܣ�һ������Ϊ���������������
//-----------------------------------------------------------------------------
bool_t __GK_GetVisibleClip(struct tagDisplayRsc *display)
{
    struct tagGkWinRsc *tempwin;
    if(__GK_ScanNewVisibleClip(display) == false)
        return false;
    tempwin = display->z_topmost;
    while(1)
    {
        //����visible_clip
        tempwin->copy_clip = __GK_CopyClipLink(tempwin->visible_clip);
//        if(tempwin->change_flag == CN_GKWIN_CHANGE_ALL)
//        {
//            //ȫ�����������ػ棬
//            __GK_ClipLinkConnect(&tempwin->redraw_clip,tempwin->visible_clip);
//            tempwin->visible_clip = NULL;   //�������Ѿ�ȫ�������ػ���
//            tempwin->change_flag = CN_GKWIN_CHANGE_NONE;
//        }
//        else
        {
            __GK_GetNewClip(tempwin);         //�������������redraw_clip
        }
        if(tempwin != display->desktop)
            tempwin = tempwin->z_back;
        else
            break;
    }
    return true;
}

//----����clip_link---------------------------------------------------------
//����: ���ݼ�������С�
//����: clip_link�������ݵļ��������
//����: ���ݵĶ��У������򷵻�NULL
//-----------------------------------------------------------------------------
struct tagClipRect * __GK_CopyClipLink(struct tagClipRect *clip_link)
{
    struct tagClipRect *clip1,*clip2,*clip_rsc,*result = NULL;

    clip_rsc = clip_link;
    if(clip_rsc == NULL)
        return NULL;

    //������һ���ڵ�
    clip1 = (struct tagClipRect*)Mb_Malloc(g_ptClipRectPool,0);
    if(clip1 == NULL)
        return NULL;
    clip1->rect = clip_rsc->rect;
    result = clip1;
    clip1->next = NULL;
    clip_rsc = clip_rsc->next;
    while(clip_rsc != clip_link)
    {
        clip2 = (struct tagClipRect*)Mb_Malloc(g_ptClipRectPool,0);
        if(clip2 == NULL)
            goto error_exit;
        clip2->rect = clip_rsc->rect;
        clip_rsc = clip_rsc->next;
        clip1->next = clip2;
        clip2->previous = clip1;
        clip2->next = NULL;
        clip1 = clip2;
    }
    //��������ʹcopy_clip�����γɱջ�
    result->previous = clip1;
    clip1->next = result;
    return result;
error_exit:
    clip1 = result;
    do
    {
        clip2 = clip1->next;
        Mb_Free(g_ptClipRectPool,clip1);
        clip1 = clip2;
    }while(clip2 != NULL);
    return NULL;
}



//----���������������һ���ڵ�-------------------------------------------------
//����: ��clip���뵽����clip_link�У��½�����clip_link���ǰ�档
//����: clip_link��Ŀ������еĲ����
//      clip��������Ľڵ�
//����: ��
//-----------------------------------------------------------------------------
void __GK_ClipConnect(struct tagClipRect **clip_link,struct tagClipRect *clip)
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

//----���������������һ���ڵ�-------------------------------------------------
//����: ��clip���뵽����clip_link�У��½�����clip_link���ǰ�档
//����: clip_link��Ŀ������еĲ����
//      clip��������Ľڵ�
//����: ��
//-----------------------------------------------------------------------------

void __GK_ClipLinkConnect(struct tagClipRect **mlink,struct tagClipRect *sub)
{
    struct tagClipRect *temp;

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


//----��ȡ���޸�����-----------------------------------------------------------
//����: ��ȡһ�����ڵı��޸Ĳ��ֵļ������γ�˫��ѭ�����������е�clip����
//      ��->�ң�����->�µ�˳�����У�������changed_msk���顣
//����: gkwin��Ŀ�괰��
//����: ����ָ�룬NULL��ʾ����δ���޸�
//-----------------------------------------------------------------------------
struct tagClipRect *__GK_GetChangedClip(struct tagGkWinRsc *gkwin)
{
    struct tagClipRect *clip=NULL,*clip1,*tempclip,*clip_head = NULL;
    u8 *msk;
    u32 offset_bit,offset,msk_line_words;
    s16 loopx,loopy;
    bool_t start;
    s32 width,height;

    if(gkwin->change_flag == CN_GKWIN_CHANGE_ALL)   //�������ھ����޸�
    {
        clip = (struct tagClipRect *)Mb_Malloc(g_ptClipRectPool,0);
        if(clip != NULL)
        {
            clip->rect.left = gkwin->limit_left + gkwin->absx0;
            clip->rect.top = gkwin->limit_top + gkwin->absy0;
            clip->rect.right = gkwin->limit_right + gkwin->absx0;
            clip->rect.bottom = gkwin->limit_bottom + gkwin->absy0;
            clip->next = clip;
            clip->previous = clip;
            gkwin->change_flag = CN_GKWIN_CHANGE_NONE;
            return clip;            //���������ڵĿ���ʾ������һ�������򷵻�
        }
        else
            return NULL;
    }else if(gkwin->change_flag == CN_GKWIN_CHANGE_NONE)    //û���޸�
    {
        return NULL;
    }else       //�����޸ģ��Ȱ�x����ȡ�ü������ٰ�y����ϲ�
    {
        offset = 0;                 //������ƫ����
        msk = gkwin->changed_msk.bm_bits;
        width = gkwin->wm_bitmap->width;
        height = gkwin->wm_bitmap->height;
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
                        clip =(struct tagClipRect*)Mb_Malloc(g_ptClipRectPool,0);
                        if(clip != NULL)
                        {
                            clip->rect.left = gkwin->absx0 + offset_bit*8;
                            clip->rect.top = loopy + gkwin->absy0;
                            clip->rect.right = clip->rect.left + 8;
                            clip->rect.bottom = clip->rect.top + 8;
                        }
                        else
                        {
                            __GK_FreeClipQueue(clip_head);
                            return NULL;
                        }
                    }else                   //changed block����
                    {
                        clip->rect.right += 8;
                    }
                }else     //������е����һ��Ҳ�Ǳ���д�Ŀ飬����򲻻��ߵ�����
                {
                    if(start == true)     //changed block����
                    {
                        __GK_ClipConnect(&clip_head,clip);
                        start = false;
                    }
                }
                offset_bit++;
            }
            //�������һ���Ǳ���д�飬��Ҫ������Ѹ�������뱻�޸�����
            if(start == true)
                __GK_ClipConnect(&clip_head,clip);
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
                    Mb_Free(g_ptClipRectPool,tempclip);
                }else
                    clip1 = clip1->next;
            }
            clip = clip->next;
        }while(clip != clip_head);
        gkwin->change_flag = CN_GKWIN_CHANGE_NONE;
    }
    memset(msk,0,msk_line_words * gkwin->changed_msk.height);
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
void __GK_ClipLinkSub(struct tagClipRect *src,struct tagClipRect *sub,
                       struct tagClipRect **ins,struct tagClipRect **different)
{
    struct tagClipRect *difftemp = NULL;  //���ص��Ĳ����ݴ�
    struct tagClipRect *worksrc;          //src�о��α��������Ĺ����Ӷ���
    struct tagClipRect *LoopSrc;
    struct tagClipRect *EndLoop;
    struct tagClipRect *workloop;         //�����Ӷ��е�ѭ������ָ��
    struct tagClipRect *subloop;          //�������е�ѭ������ָ��
    struct tagClipRect *clip_ints=NULL;   //�����ռ��ص�������
    struct tagClipRect *temp1;            //��ʱ����
    struct tagRectangle ints_rect,src_rect;
    bool_t EndSub;
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
            *different = NULL;//

          //  *different = sub;
            return;
        }
    }
    //���������С��������ж���Ϊ��
    LoopSrc = src;
    do{
        worksrc = LoopSrc;
        LoopSrc = LoopSrc->next;
        worksrc->next = worksrc;
        worksrc->previous = worksrc;
        subloop = sub;
        do{
            workloop = worksrc;
            EndLoop = worksrc->previous;
            EndSub = false;
            //�˴���ѭ���Ǳ�Ҫ�ģ���sub�е�һ������ʱworksrcֻ��һ�����Σ���
            //�Բ���ѭ�������������ڶ�������ʱ��worksrc���ܷ��ѳ�һ�����С�
            do
            {
                if(__GK_GetRectInts(&workloop->rect,&subloop->rect,&ints_rect))
                {//�����ཻ
                    temp1 = (struct tagClipRect *)Mb_Malloc(g_ptClipRectPool,0);
                    temp1->rect = ints_rect;
                    //���ཻ�������clip_ints
                    __GK_ClipConnect(&clip_ints,temp1);
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
                             (struct tagClipRect *)Mb_Malloc(g_ptClipRectPool,0);
                            temp1->rect.left = src_rect.left;
                            temp1->rect.top = subloop->rect.top;
                            temp1->rect.right = subloop->rect.left;
                            if(src_rect.bottom < subloop->rect.bottom)
                                temp1->rect.bottom = src_rect.bottom;
                            else
                                temp1->rect.bottom = subloop->rect.bottom;
                            __GK_ClipConnect(&workloop,temp1);
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
                             (struct tagClipRect *)Mb_Malloc(g_ptClipRectPool,0);
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
                            __GK_ClipConnect(&workloop,temp1);
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
                             (struct tagClipRect *)Mb_Malloc(g_ptClipRectPool,0);
                            temp1->rect.left = src_rect.left;
                            temp1->rect.top = subloop->rect.bottom;
                            temp1->rect.right = src_rect.right;
                            temp1->rect.bottom = src_rect.bottom;
                            __GK_ClipConnect(&workloop,temp1);
                        }
                        uftemp++;
                   }
                   if(uftemp == 0)                    //Դ���α���ȫ����,
                   {
                        if(worksrc->next == worksrc)    //Դ����ֻ��һ������
                        {
                            Mb_Free(g_ptClipRectPool,worksrc);
                            worksrc = NULL;
//                            workloop = NULL;
                            EndSub = true;
                        }else
                        {
                            temp1 = workloop;
                            workloop = workloop->next;
                            temp1->previous->next = workloop;
                            workloop->previous = temp1->previous;
                            if(worksrc == temp1)
                                worksrc = workloop;
                            if(EndLoop == temp1)
                                EndSub = true;
                            Mb_Free(g_ptClipRectPool,temp1);
                        }
                    }
                   else
                   {
                        workloop = workloop->next;
                   }
                }
                else           //���β��ཻ
                {
                    workloop = workloop->next;
                }
                if(workloop->previous == EndLoop)
                    EndSub = true;
            }while(EndSub == false);
            if(worksrc == NULL)
                break;
            subloop = subloop->next;
        }while(subloop != sub);
        __GK_ClipLinkConnect(&difftemp,worksrc);
    }while(src != LoopSrc);
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
void __GK_GetNewClip(struct tagGkWinRsc *gkwin)
{
    struct tagClipRect *redraw;       //visible_clip-visible_bak����Ĳ���
    struct tagClipRect *res;          //visible_clip��visible_bak�ཻ��clip

    if(gkwin->visible_bak != NULL)
    {
        __GK_ClipLinkSub(gkwin->visible_clip,gkwin->visible_bak,&res,&redraw);
        __GK_ClipLinkConnect(&gkwin->redraw_clip,redraw);
        gkwin->visible_clip = res;
        if(gkwin->dest_blend)
        {
            __GK_ClipLinkSub(gkwin->visible_bak,gkwin->visible_clip,
                              &res,&redraw);
            gkwin->visible_bak = redraw;
            __GK_FreeClipQueue(res);
        }
        else
        {
            gkwin->visible_bak = __GK_FreeClipQueue(gkwin->visible_bak);
        }
    }
    else
    {
        __GK_ClipLinkConnect(&gkwin->redraw_clip,gkwin->visible_clip);
        gkwin->visible_clip = NULL;
    }
}

//----��ȡ�����򽻼�-----------------------------------------------------------
//����: ��ȡsrcclip��������desclip�ཻ���֣����ཻ�Ĳ�����Ȼ����srcclip�С�
//����: srcclip��ԭ����������
//      desclip��Ŀ�����������
//����: ��ȡ�Ŀ�����NULL = û���ཻ����
//-----------------------------------------------------------------------------
struct tagClipRect *__GK_GetClipLinkInts(struct tagClipRect **srcclip,
                                             struct tagClipRect *desclip)
{
    struct tagClipRect *redraw,*res;
    //��ȡ����������ص�����
    __GK_ClipLinkSub(*srcclip,desclip,&redraw,&res);
    *srcclip = res;
    return redraw;
}

//----�����ػ�����-------------------------------------------------------------
//����: ɨ����������������������ȫ�����ڵĿ�����,�Լ����ػ��clip�������͹���:
//      1���ͷ����е�visible_bak����visible_clipת��visible_bak�С�
//      2��������������win��visible_clip�������Ƶ� copy_clip ����ʱ���档
//      3��visible_clip��ȥ��visible_bak���ü���redraw_clip���С�ʣ�ಿ�ֱ�����
//          visible_clip�С�������ڵ�dest_blend == true����visible_bak�б���
//          �ɿɼ���Ϊ���ɼ��Ĳ��֣�����visible_bak�����ͷš�
//      4����changed msk����ȡchanged_clip��
//      5��ȡvisible_clip��changed_clip�Ľ���������redraw_clip���С�
//      6����z_top��ȡvisible_bak,ɨ����󴰿ڵĿ����������ص��Ĳ��֣�����
//         redraw�򡣷��ϴ�������visible_bak���ض�����Ϊ�����ƿ����¡�
//      7����z_top�𣬶�dest_blend == true��win��ɨ��z������������
//         win��visible_clip���������ػ沿���ص��ģ�����redraw_clip���С�
//         �˺�dest_blend != true��win��visible_clip���ͷš�
//      8���ϲ�ǰ����������redraw_clip���С�
//      9����z_top�𣬶���һdest_blend == true��win��visible_clip��ɨ��z��������
//         �����win��redraw_clip���ص��Ĳ��ּ���redraw_clip���У����ϲ�֮��
//����: display����ɨ�����ʾ��
//����: false=ʧ�ܣ�һ������Ϊ���������������
//-----------------------------------------------------------------------------
bool_t __GK_GetRedrawClipAll(struct tagDisplayRsc *display)
{
    struct tagGkWinRsc *tempwin;
    struct tagGkWinRsc *special_win;
    struct tagClipRect *changed_clip,*tempclip1;
    if(display->reset_clip == true)         //���Ƿ���Ҫ�������ɿ�����
    {
        if(__GK_GetVisibleClip(display) == false)     //���step1��step2��step3
            return false;
        display->reset_clip = false;
    }
    tempwin = display->desktop;
    while(1)
    {   //ɨ��desktop��z_topmost֮���ȫ�����ڣ����step4~5����desktop�����
        //���ڲ���ʾ��������step1~3���Ѿ�ȫ��ת�Ƶ� redraw_clip �У����账��
        if(tempwin->visible_clip != NULL)
        {
            //����visible_clip
            if(tempwin->copy_clip == NULL)  //copy_clip�����Ѿ���step1~3��������
                tempwin->copy_clip = __GK_CopyClipLink(tempwin->visible_clip);
            //��������ȫ�����޸ģ�������ơ������ƶ������졢ѹ���ᵼ��ȫ���޸�
            //reset_clip == true�����Ļ���cn_gkwin_change_all�������
            //__gk_get_visible_clip�������Ѿ���ɣ��ڸú�������ɣ����Լ��᲻��
            //�����������Ҷ�����û��Ӱ�졣
            if(tempwin->change_flag == CN_GKWIN_CHANGE_ALL)
            {
                //ȫ�����������ػ�
                __GK_ClipLinkConnect(&tempwin->redraw_clip,
                                       tempwin->visible_clip);
                tempwin->visible_clip = NULL;   //�������Ѿ�ȫ�������ػ���
                tempwin->change_flag = CN_GKWIN_CHANGE_NONE;
            }else
            {   //�в����޸�
                if(tempwin->change_flag == CN_GKWIN_CHANGE_PART)
                {
                    //ȡ���޸���
                    changed_clip = __GK_GetChangedClip(tempwin);
                    //��ȡ���������޸ĵ�����
                    tempclip1 = __GK_GetClipLinkInts(
                                    &tempwin->visible_clip,changed_clip);
                    __GK_FreeClipQueue(changed_clip);
                    //���޸�������뵽redraw_clip��
                    __GK_ClipLinkConnect(&tempwin->redraw_clip,tempclip1);
                    tempwin->change_flag = CN_GKWIN_CHANGE_NONE;
                }
            }
//              tempwin->visible_bak = NULL;
        }
        else
            tempwin->change_flag = CN_GKWIN_CHANGE_NONE;
        //���changed_msk
        __GK_ShadingClear(tempwin);
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
//                tempclip2 = __GK_CopyClipLink(special_win->visible_bak);
                //ȡ�ñ���������Ҫ�ػ������
                tempclip1 =
                    __GK_GetClipLinkInts(&tempwin->visible_clip,
                                          special_win->visible_bak);

//                __GK_FreeClipQueue(tempclip2);
                //���޸�������뵽��������redraw_clip��
                __GK_ClipLinkConnect(&tempwin->redraw_clip,tempclip1);
                if(tempwin != display->desktop)
                    tempwin = tempwin->z_back;
                else
                    break;
            }
            special_win->visible_bak = __GK_FreeClipQueue(special_win->visible_bak);
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
                        __GK_GetClipLinkInts(&tempwin->visible_clip,
                                                special_win->redraw_clip);

                    //���޸�������뵽��������redraw_clip��
                    __GK_ClipLinkConnect(&tempwin->redraw_clip,tempclip1);
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
                            __GK_FreeClipQueue(special_win->visible_clip);
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
        tempwin->redraw_clip = __GK_CombineClip(tempwin->redraw_clip);
        if(tempwin != display->desktop)
            tempwin = tempwin->z_back;
        else
            break;
    }

    //ִ��step9�������������ж�desktop����ѭ����������ͬ
    //��Ҫ������������Ĵ��ڣ��䱳���ı�
    special_win = display->z_topmost;
    do{
        if((special_win->dest_blend)&&(special_win->visible_clip != NULL))
        {//special_win��Ҫ�������ڲ������㣬��ִ������ĳ���
            tempwin = special_win;
            do{
                tempwin = tempwin->z_back;
                //ȡ�ñ������ڵ��޸���
                tempclip1 =
                    __GK_GetClipLinkInts(&special_win->visible_clip,
                                              tempwin->redraw_clip);
                //���޸�������뵽���ڵ�redraw_clip��
                __GK_ClipLinkConnect(&special_win->redraw_clip,tempclip1);
            }while(tempwin != display->desktop);
            //�ͷŵ�����Ҫʹ�õ�����
            special_win->visible_clip =
                            __GK_FreeClipQueue(special_win->visible_clip);
            //�ϲ�redraw_clip
            special_win->redraw_clip =
                            __GK_CombineClip(special_win->redraw_clip);
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
void __GK_GetClipSection( struct tagGkWinRsc *z1,struct tagGkWinRsc *z2)
{
}

