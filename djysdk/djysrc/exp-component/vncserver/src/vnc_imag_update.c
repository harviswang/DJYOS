// =============================================================================
// Copyright (C) 2012-2020 ��԰�̱��Զ������޹�˾ All Rights Reserved
// ģ������: VNCServer
// ģ��汾: V1.00(��ʼ���汾)
// �ļ���     ��frameupdate.c
// ������Ա: Administrator
// ����ʱ��: 2013-3-8
// =============================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <V1.00> <2013-3-8>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// =============================================================================

#include "vnc_image_update.h"
#include "vnc_socket.h"
#include "rfb.h"
#include "debug.h"

#include "vnc_thread.h"
#include "vnc_mirror_display.h"
#include "clip_process.h"
#include "vnc_keyboard.h"
#include "color_remap.h"
// =============================================================================
// ��������:�ж��Ƿ�÷�������
// �������:���жϵ�cl
// ���������
// ����ֵ  :true:���Է���  false��û�б�Ҫ����
// ˵��    :���ͻ����й�����֤��׼���ý������ݣ�
// =============================================================================
bool_t  ready_to_send_frame(rfbClientPtr cl)
{
    if((cl->HasRequest == 1)&&(cl->client_dirty == 0 ))
    {
        return true;
    }
    else
    {
        return false;
    }
}
// =============================================================================
// ��������:��screen��clbuffer����ˢ�¸�cl������Ϊublen
// �������:�����͵�cl
// ���������
// ����    :true:���ͳɹ�  false������ʧ��
// ˵��    :�ڲ�ʹ�ã������������
// =============================================================================
bool_t rfbSendUpdateBuf(rfbClientPtr cl)
{
    if (WriteExact(cl, cl->screen->cl_buffer, cl->screen->ublen) < 0)
    {
        debug_printf("#error:rfbSendUpdateBuf: write\n");
        return false;
    }

    cl->screen->ublen = 0;
    return true;
}

// =============================================================================
// ��������:�ж�һ��clip�ǲ��Ǹ�client�ĸ���Ȥ������
// �������:cl���ͻ��ˣ� clip ���жϵ�clip
// ���������
// ����ֵ :true:����Ȥ  false��������Ȥ
// ˵��:�жϸ�clip��cl����Ȥ�����ǲ����н���
// =============================================================================
static  bool_t vnc_check_clip_in_client(rfbClientPtr cl, struct tagClipRect *clip)
{
//���оݱȽϼ򵥣��������ж���ʼ�㣬ʵ����Ӧ�ÿ�������clip�Ƿ��н���--TODO
//	Ĭ�ϵ����ཻ�����
	u8 h_crossflag = 1;
	u8 v_crossflag = 1;
	if((((u16)clip->rect.left)> (cl->modifiedregion.x + cl->modifiedregion.w))||\
			(((u16)clip->rect.right) < cl->modifiedregion.x)) //�ж�ˮƽ�Ƿ񽻼�
	{
		h_crossflag = 0;
	}
	else
	{
		h_crossflag = 1;
	}

	if((((u16)clip->rect.top)> (cl->modifiedregion.y + cl->modifiedregion.h))||\
			(((u16)clip->rect.bottom) < cl->modifiedregion.y)) //�жϴ�ֱ�Ƿ񽻼�
	{
		v_crossflag = 0;
	}
	else
	{
		v_crossflag = 1;
	}
	if((h_crossflag)&(v_crossflag))
	{
		return true;
	}
	else
	{
		return false;
	}

}
// =============================================================================
// ��������:����cl�ĸ���Ȥ�����򣬵�����clip����Ҫ�ǽ���������Ȥ�Ĳ�������
// �������:cl���ͻ��ˣ� clip ���жϵ�clip
// �������:rec,�洢�����õ�λ��
// ����ֵ :
// ˵��:�������Ҫ������������洢ԭ���ľ��μ���
// =============================================================================
static void vnc_adjust_clip_in_client(rfbClientPtr cl,u16 x, \
		u16 y, u16 w, u16 h,rfbRectangle *rec)
{
	if(x < (cl->modifiedregion.x))
	{
		rec->x = cl->modifiedregion.x;
	}
	else
	{
		rec->x = x;
	}
	if(y < (cl->modifiedregion.y))
	{
		rec->y = cl->modifiedregion.y;
	}
	else
	{
		rec->y = y;
	}


	if( (x + w) >(cl->modifiedregion.x + cl->modifiedregion.w))
	{
		rec->w = cl->modifiedregion.x + cl->modifiedregion.w - x;
	}
	else
	{
		rec->w = w;
	}
	if( (y + h) >(cl->modifiedregion.y + cl->modifiedregion.h))
	{
		rec->h = cl->modifiedregion.y + cl->modifiedregion.h - x;
	}
	else
	{
		rec->h = h;
	}
}
// =============================================================================
// ��������:�ж�cip�����ڸ�cl�ĸ���Ȥ�������ж��ٸ�
// �������:cl���ͻ���
// ���������
// ����ֵ ����server��cl����Χ�ж���clip
// ˵��:
// =============================================================================
static  u16 vnc_check_clipnumbers_in_client(rfbClientPtr cl)
{
	u16 ret = 0;
	struct tagClipRect * pclip;

	pclip = cl->screen->clip_head;
	if(NULL == pclip)
	{
		return ret;
	}
	do{
		if(vnc_check_clip_in_client(cl,pclip))
		{
			ret++;
		}
		pclip = pclip->next;
	}while(cl->screen->clip_head != pclip );

	return ret;
}


// =============================================================================
// ��������:��һ�����ο���raw(Դ�룬�Ǽ���)����ʽ���͸�cl
// �������:�����͵�cl,x,y,w,h,���screen��ͼ�񴰿�λ����Ϣ��
// ���������
// ����ֵ  :true:���ͳɹ�  false������ʧ��
// ˵��    :screen��cl�����ظ�ʽ�ȵ�ת���ڴ˽��У���һ�η��͵�ʱ��clbuffer���Ѿ�����
//          ͼ����µ���Ϣ
// =============================================================================
bool_t  send_rect_enraw(rfbClientPtr cl, int x, int y, int w, int h)
{
    rfbFramebufferUpdateRectHeader rect;
    int nlines;
    int bytesPerLine = w * (cl->format.bitsPerPixel / 8);
    char *fbptr = (cl->screen->frameBuffer + (cl->screen->paddedWidthInBytes * y)
          	         + (x * (cl->screen->rfbServerFormat.bitsPerPixel / 8)));
    if (cl->screen->ublen + sz_rfbFramebufferUpdateRectHeader > CL_BUFF_SIZE)
    {
        if (!rfbSendUpdateBuf(cl))
            return false;
    }

 //����cl��clip������������ֹ����cl�ĸ���Ȥ����
    vnc_adjust_clip_in_client(cl, x, y, w, h,&(rect.r));
    rect.r.x = Swap16IfLE(rect.r.x);
    rect.r.y = Swap16IfLE(rect.r.y);
    rect.r.w = Swap16IfLE(rect.r.w);
    rect.r.h = Swap16IfLE(rect.r.h);
    rect.encoding = Swap32IfLE(rfbEncodingRaw);

//    ��������ͷ��Ϣ
    memcpy(&(cl->screen->cl_buffer[cl->screen->ublen]), \
    		(char *)&rect,sz_rfbFramebufferUpdateRectHeader);
    cl->screen->ublen += sz_rfbFramebufferUpdateRectHeader;
    nlines = (CL_BUFF_SIZE - cl->screen->ublen) / bytesPerLine;

    while (true)
    {
        if (nlines > h)
            nlines = h;

        if(NULL != cl->translateFn)
        {
        	(cl->translateFn)(&(cl->screen->rfbServerFormat),\
             &cl->format, fbptr, &(cl->screen->cl_buffer[cl->screen->ublen]),\
             cl->screen->paddedWidthInBytes, w, nlines);
        }

        cl->screen->ublen += nlines * bytesPerLine;
        h -= nlines;
//ÿ�ζ�����CL_BUFF_SIZE�����ݣ����õ���������
        if (h == 0)
            return true;

        if (!rfbSendUpdateBuf(cl))
        {
             return false;
        }
        fbptr += (cl->screen->paddedWidthInBytes * nlines);

        nlines = (CL_BUFF_SIZE - cl->screen->ublen) / bytesPerLine;
        if (nlines == 0) 
        {//һ�е����ݶ����Ͳ��ˣ�����������ܲ��˵�
//          debug_printf("send buffer  for %d  bytes per line\n", bytesPerLine);
            return false;
        }
    }//end while
}//end send_rect_enraw
// =============================================================================
// ��������:������ͼ�񶼸��¸�cl�ͻ���
// �������:�ͻ���cl
// ������� ��
// ����ֵ   :true,���³ɹ���false����ʧ��
// ˵��     :�ڲ�ʹ�ú���������������֮��İ�ȫ���
// =============================================================================
static bool_t  SendFrambuf2cl(rfbClientPtr cl)
{
	int clip_numbers=0;
	rfbScreenInfoPtr screen;
	rfbFramebufferUpdateMsg *Msg;
	bool_t ret=true;

	//clҪ���ò��а�
	if(cl->client_dirty)
	{
		return ret;
	}

	clip_numbers=0x01;
	screen=cl->screen;
	screen->ublen=0;
	Msg=(rfbFramebufferUpdateMsg *)&(screen->cl_buffer[screen->ublen]);
	Msg->type=rfbFramebufferUpdate;
	Msg->nRects=Swap16IfLE(clip_numbers);//���͵�������buffer
	Msg->pad=0xFF;


	screen->ublen+=sz_rfbFramebufferUpdateMsg;
//	ret=send_rect_enraw(cl,0,0,screen->width,screen->height);
	ret = send_rect_enraw(cl, cl->modifiedregion.x, cl->modifiedregion.y,\
			cl->modifiedregion.w, cl->modifiedregion.h);
	if(ret==false)
	{
	  return ret;
	 // rfbCloseClient(cl);//�ڸ��³����о����������������
	}
	ret=rfbSendUpdateBuf(cl);

      cl->InitDone=TRUE;
    //  debug_printf("send the whole buf data end!\n");
    return ret;
}

// =============================================================================
// ��������:���껺���еĸ���clip���¸�cl�ͻ���
// �������:�ͻ���cl
// ���������
// ����ֵ  :true,���³ɹ���false����ʧ��
// ˵��    :�ڲ�ʹ�ú���������������֮��İ�ȫ���
// =============================================================================
static bool_t  SendFramClip2cl(rfbClientPtr cl)
{
	int clip_numbers=0;
	int  x,y,w,h,i;
	rfbScreenInfoPtr screen;
	rfbFramebufferUpdateMsg *Msg;
	struct tagClipRect *clip;
	bool_t ret=true;

//clҪ���ò��а�
	if(cl->client_dirty)
	{
		return ret;
	}

	screen=cl->screen;
	screen->ublen=0;
    clip_numbers=vnc_check_clipnumbers_in_client(cl);
	if(0 == clip_numbers)
	{
		return ret;
	}
	Msg=(rfbFramebufferUpdateMsg *)&(screen->cl_buffer[screen->ublen]);
	Msg->type=rfbFramebufferUpdate;
	Msg->nRects=Swap16IfLE(clip_numbers);
	Msg->pad=0xFF;
	screen->ublen+=sz_rfbFramebufferUpdateMsg;
	clip=screen->clip_head;

  //  debug_printf("clip_numbers= %d\n",clip_numbers);
	for(i=0;i<clip_numbers;i++)//�������е�clip,���η��ͳ�ȥ
	{
		//�����clip���Ǹ���Ȥ������ô���÷��͵�
		if(vnc_check_clip_in_client(cl,clip))
		{
			x = clip->rect.left;
			y = clip->rect.top;
			w = clip->rect.right-clip->rect.left;
		    h = clip->rect.bottom-clip->rect.top;
		    ret = send_rect_enraw(cl,x,y,w,h);
		    if(ret==false)
		    {
		    	return ret;
		    }
		}
	    clip = clip->next;
	}
	ret=rfbSendUpdateBuf(cl);
	return ret ;
}

// =============================================================================
// ��������:���µ�key���Ӿ����͸��ͻ���
// �������:�ͻ���cl
// ���������
// ����ֵ  :true ���ͳɹ� false ʧ��(һ������Ϊдsocketʧ��)
// ˵��    :�ڲ�ʹ�ú���������������֮��İ�ȫ���
// =============================================================================
static bool_t  sendkeylist2cl(rfbClientPtr cl)
{
	u16 key_num=0;
	u16 key_num_bak = 0;
	u8  key_number = 0, i =0;
	u16  x,y,w,h;
	rfbScreenInfoPtr screen;
	rfbFramebufferUpdateMsg *Msg;
//	struct tagClipRect *clip;
	bool_t ret=true;

//clҪ���ò��а�
	if(cl->client_dirty)
	{
		return ret;
	}

	screen=cl->screen;
	screen->ublen=0;
	key_num=vnc_get_keychanged_numbers(screen);
	if(0 == key_num)
	{
		return ret;
	}
	Msg=(rfbFramebufferUpdateMsg *)&(screen->cl_buffer[screen->ublen]);
	Msg->type=rfbFramebufferUpdate;
	Msg->nRects=Swap16IfLE(key_num);
	Msg->pad=0xFF;
	screen->ublen+=sz_rfbFramebufferUpdateMsg;
	for(i=0; i<cn_vnc_key_num; i++)//�������е�key
	{
		if(vnc_check_key_changed(screen,i))//�ü������˱仯
		{
			vnc_get_keyposition_by_number(screen, i, &x, &y);
			w = cn_vnc_key_width;
		    h = cn_vnc_key_height;
		    ret = send_rect_enraw(cl,x,y,w,h);
		    if(ret == false)
		    {
		    	return ret;
		    }
		    key_num_bak++;
		}
		if(key_num_bak == key_num)
		{
			break;//���͹��ˣ�Ҫ�˳�ȥ
		}
	}
	ret=rfbSendUpdateBuf(cl);
	return ret ;
}

// =============================================================================
// ��������:Ϊcl���и�������
// �������:vncserver��screen
// ���������
// ����ֵ  :true,���³ɹ���false����clû�и��³ɹ����Ὣ������clɾ����
// ˵��    :�������ݣ���������Image����������̡��źŵƵ�
// =============================================================================
bool_t UpdateFrame2Clist(rfbScreenInfoPtr screen)
{
	   rfbClientPtr cl = NULL, cl_next = NULL;
       int clip_numbers=0; 
       bool_t ret=true;
       bool_t bNoClip=false;

//�;�����ʾ������ʹ��frambuf��Դ
       XMutexWait(&(screen->frame_mutex));
       cl=screen->rfbClientHead;
       
       if(cl)
       {
    	   vnc_changed_keyboard_visual(screen);//�ڴ�ˢ��keyvisual��buf
       }
       while(cl)//�������е�cl�����и���
       {
    	   if(ready_to_send_frame(cl))//���Է������ݣ��ðɣ��Ǿͷ��Ͱ�
    	   {
    		   screen->ublen = 0;
    		   cl_next=cl->next;
 //cl�б�ɾ����Σ�գ���ɾ��֮ǰ�����ȱ���cl->next��
//����ǵ�һ�Σ���ô��Ҫ�ó�ʼ�����������������ǵľ�����ʾ��,
//���п��������ǵ�����̡��źŵ�֮��ģ�
    		   if(!cl->InitDone)
    		   {
     			   ret=SendFrambuf2cl(cl);
     			   if(false==ret)
     			   {
     				   cl->client_dirty = 1;
     			   }                
               }
    		   else//���͸��µ�����
    		   {
                 //�����źŵƸ�������
                 //���Ͱ�����������
#if  CN_CFG_KEYBOARD ==1
    			   ret = sendkeylist2cl(cl);
    			   if(false==ret)
    			   {
      				  //�ɶ˿ڼ��ӳ�������cl
      				   cl->client_dirty = 1;
    			   }
#endif
                 //ѭ������clip����
    			   ret=SendFramClip2cl(cl);
    			   if(false==ret)
    			   {
      				  //�ɶ˿ڼ��ӳ�������cl
      				   cl->client_dirty = 1;
    			   }

    		   }//����������ʽ������ͼ��or������ʽ��
          }//���Է�������
          cl=cl_next;

      }//while����

       //��� key����
       vnc_reset_keyboard_statelist(screen);
      //���clip����
      if(NULL!=screen->clip_head)
      {
    	  free_vnccliplink_space(screen->clip_head);
    	  screen->clip_head = NULL;
      }
      //�ͷ�frame��Դ
      XMutexPost(&(screen->frame_mutex));
      return ret;
}






