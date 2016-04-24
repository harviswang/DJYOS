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

//����ģ��: GDD
//����:  LiuWei.
//�汾��V1.0.0
//�ļ�����: ��ͼ�����
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2014-11-10
//   ����:  LiuWei.
//   �°汾�ţ�V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include    "gdd.h"
#include    "./include/gdd_private.h"
/*============================================================================*/



static  BOOL    BeginDraw(HDC hdc)
{
    return TRUE;
}

static  void    EndDraw(HDC hdc)
{

}


static  void    _LPtoDP(HDC hdc,POINT *pt,int count)
{

    switch(hdc->DCType)
    {
        case    DC_TYPE_PAINT:
        case    DC_TYPE_CLIENT:
                while(count--)
                {
                    pt->x +=(hdc->hwnd->CliRect.left - hdc->hwnd->WinRect.left);
                    pt->y +=(hdc->hwnd->CliRect.top - hdc->hwnd->WinRect.top);
                    pt++;
                }
                break;
                ////

        case    DC_TYPE_WINDOW:
                while(count--)
                {
                    //pt->x +=(hdc->hwnd->CliRect.left - hdc->hwnd->WinRect.left);
                    //pt->y +=(hdc->hwnd->CliRect.top - hdc->hwnd->WinRect.top);
                    //pt++;
                }
                break;

        default:
                break;
                ////
    }
}
/*============================================================================*/

u32 AlphaBlendColor(u32 bk_c,u32 fr_c,u8 alpha)
{
    u8 r1,g1,b1;
    u8 r2,g2,b2;

    r1 =bk_c>>16;
    g1 =bk_c>>8;
    b1 =bk_c;

    r2 =fr_c>>16;
    g2 =fr_c>>8;
    b2 =fr_c;


    r1 =(((r2-r1)*alpha)>>8) + r1;
    g1 =(((g2-g1)*alpha)>>8) + g1;
    b1 =(((b2-b1)*alpha)>>8) + b1;

    return   RGB(r1,g1,b1);

}
/*============================================================================*/

void    UpdateDisplay(void)
{
    GK_ApiSyncShow(1000*mS);
}

/*============================================================================*/
/*============================================================================*/

void    _InitDC(DC *pdc,struct tagGkWinRsc *gk_win,HWND hwnd,int dc_type)
{

    pdc->pGkWin     =gk_win;
    pdc->hwnd       =hwnd;
    pdc->DCType     =dc_type;
    pdc->pFontDef   =Font_GetCurFont();
    pdc->pFont      =pdc->pFontDef;
    pdc->pCharset	=Charset_NlsGetCurCharset();

    pdc->CurX       =0;
    pdc->CurY       =0;
    pdc->DrawColor  =RGB(255,255,255);
    pdc->FillColor  =RGB(1,1,1);
    pdc->TextColor  =RGB(255,255,255);
    pdc->SyncTime   =100*mS;
    pdc->RopCode    =CN_R2_COPYPEN;

}

bool_t  DeleteDC(HDC hdc)
{
    if(hdc != NULL)
    {
//        GUI_DeleteDefFont(hdc->pFontDef);
        free(hdc);
        return TRUE;
    }
    return FALSE;
}


//----���õ�ǰ��դ��-------------------------------------------------------------
//����: ��.
//������hdc: ��ͼ�����ľ��.
//     rop_code: �µĹ�դ��.
//���أ��ɵĹ�դ��.
//------------------------------------------------------------------------------
u32 SetRopCode(HDC hdc,u32 rop_code)
{
    u32 old=0;
    if(hdc!=NULL)
    {
        old =hdc->RopCode;
        hdc->RopCode =rop_code;
    }
    return old;
}

//----��õ�ǰ��դ��-------------------------------------------------------------
//����: ��.
//������hdc: ��ͼ�����ľ��.
//���أ���ǰ��դ��.
//------------------------------------------------------------------------------
u32 GetRopCode(HDC hdc)
{
    u32 old=0;
    if(hdc!=NULL)
    {
        old =hdc->RopCode;
    }
    return old;

}

//----���õ�ǰ����λ��-----------------------------------------------------------
//����: �ú���Ϊ�ڲ�����,����麯�������ĺϷ���.
//������hdc: ��ͼ�����ľ��.
//      x,y: �µ�����λ��.
//     old_pt: ����ɵ�����λ��,����ò���ΪNULL,�ղ�����ɵ�����λ��.
//���أ���.
//------------------------------------------------------------------------------
static  void    _MoveTo(HDC hdc,int x,int y,POINT *old_pt)
{

    if(old_pt!=NULL)
    {
        old_pt->x =hdc->CurX;
        old_pt->y =hdc->CurY;
    }

    hdc->CurX =x;
    hdc->CurY =y;
}

//----���õ�ǰ����λ��-----------------------------------------------------------
//����: ��ǰ����λ�û�Ӱ��LineTo���ߵ���ʼλ��.
//������hdc: ��ͼ�����ľ��.
//      x,y: �µ�����λ��.
//     old_pt: ����ɵ�����λ��,����ò���ΪNULL,�ղ�����ɵ�����λ��.
//���أ���.
//------------------------------------------------------------------------------
void    MoveTo(HDC hdc,int x,int y,POINT *old_pt)
{
    if(hdc!=NULL)
    {
        _MoveTo(hdc,x,y,old_pt);
    }
}

//----���õ�ǰ������ɫ-----------------------------------------------------------
//����: ��ǰ������ɫ��Ӱ�����л����ຯ��,��DrawRect,DrawCircle...
//������hdc: ��ͼ�����ľ��.
//      color: �µĻ�����ɫ.
//���أ��ɵĻ�����ɫ.
//------------------------------------------------------------------------------
u32 SetDrawColor(HDC hdc,u32 color)
{
    u32 old;

    if(hdc!=NULL)
    {
        old =hdc->DrawColor;
        hdc->DrawColor = color;
    }
    else
    {
        old =0;
    }
    return old;
}

//----��õ�ǰ������ɫ-----------------------------------------------------------
//����: ��.
//������hdc: ��ͼ�����ľ��.
//���أ���ǰ������ɫ.
//------------------------------------------------------------------------------
u32 GetDrawColor(HDC hdc)
{
    u32 old;

    if(hdc!=NULL)
    {
        old =hdc->DrawColor;
    }
    else
    {
        old =0;
    }
    return old;
}

//----���õ�ǰ�����ɫ-----------------------------------------------------------
//����: ��ǰ�����ɫ��Ӱ����������ຯ��,��FillRect,FillCircle...
//������hdc: ��ͼ�����ľ��.
//      color: �µ������ɫ.
//���أ��ɵĻ�����ɫ.
//------------------------------------------------------------------------------
u32 SetFillColor(HDC hdc,u32 color)
{
    u32 old;

    if(hdc!=NULL)
    {
        old =hdc->FillColor;
        hdc->FillColor = color;
    }
    else
    {
        old =0;
    }
    return old;
}

//----��õ�ǰ�����ɫ-----------------------------------------------------------
//����: ��.
//������hdc: ��ͼ�����ľ��.
//���أ���ǰ�����ɫ.
//------------------------------------------------------------------------------
u32 GetFillColor(HDC hdc)
{
    u32 old;

    if(hdc!=NULL)
    {
        old =hdc->FillColor;
    }
    else
    {
        old =0;
    }
    return old;
}

//----���õ�ǰ������ɫ-----------------------------------------------------------
//����: ��ǰ������ɫ��Ӱ���������ֻ����ຯ��,��TextOut,DrawText...
//������hdc: ��ͼ�����ľ��.
//      color: �µ�������ɫ.
//���أ��ɵ�������ɫ.
//------------------------------------------------------------------------------
u32 SetTextColor(HDC hdc,u32 color)
{
    u32 old;

    if(hdc!=NULL)
    {
        old =hdc->TextColor;
        hdc->TextColor = color;
    }
    else
    {
        old =0;
    }
    return old;
}

//----��õ�ǰ������ɫ-----------------------------------------------------------
//����: ��.
//������hdc: ��ͼ�����ľ��.
//���أ���ǰ������ɫ.
//------------------------------------------------------------------------------
u32 GetTextColor(HDC hdc)
{
    u32 old;

    if(hdc!=NULL)
    {
        old =hdc->TextColor;
    }
    else
    {
        old =0;
    }
    return old;
}

//----���õ�ǰͬ��ʱ��-----------------------------------------------------------
//����: ��
//������hdc: ��ͼ�����ľ��.
//      sync_time: �µ�ͬ��ʱ��.
//���أ��ɵ�ͬ��ʱ��.
//------------------------------------------------------------------------------
u32 SetSyncTime(HDC hdc,u32 sync_time)
{
    u32 old;

    if(hdc!=NULL)
    {
        old =hdc->SyncTime;
        hdc->SyncTime = sync_time;
    }
    else
    {
        old =0;
    }
    return old;
}

//----��õ�ǰͬ��ʱ��-----------------------------------------------------------
//����: ��
//������hdc: ��ͼ�����ľ��.
//���أ���ǰͬ��ʱ��.
//------------------------------------------------------------------------------
u32 GetSyncTime(HDC hdc)
{
    u32 old;

    if(hdc!=NULL)
    {
        old =hdc->SyncTime;
    }
    else
    {
        old =0;
    }
    return old;
}

//----���õ�ǰ����---------------------------------------------------------------
//����: ��
//������hdc: ��ͼ�����ľ��.
//      hFont: ��ǰ������.
//���أ��ɵ�������.
//------------------------------------------------------------------------------
HFONT   SetFont(HDC hdc,HFONT hFont)
{
    HFONT old;

    if(hdc!=NULL)
    {
        old =hdc->pFont;
        hdc->pFont = hFont;
    }
    else
    {
        old =NULL;
    }
    return old;

}

//----��õ�ǰ����---------------------------------------------------------------
//����: ��
//������hdc: ��ͼ�����ľ��.
//���أ���ǰ������.
//------------------------------------------------------------------------------
HFONT   GetFont(HDC hdc)
{
    HFONT old;

    if(hdc!=NULL)
    {
        old =hdc->pFont;
    }
    else
    {
        old =NULL;
    }
    return old;

}

//----����ָ��λ��������ɫ-------------------------------------------------------
//����: ��
//������hdc: ��ͼ�����ľ��.
//      x,y: ����.
//      color:��ɫֵ.
//���أ���.
//------------------------------------------------------------------------------
void SetPixel(HDC hdc,int x,int y,u32 color)
{
    POINT pt;
    if(BeginDraw(hdc))
    {
        pt.x =x;
        pt.y =y;
        _LPtoDP(hdc,&pt,1);
        GK_ApiSetPixel(hdc->pGkWin,pt.x,pt.y,color,hdc->RopCode,hdc->SyncTime);
        EndDraw(hdc);
    }
}

//----����----------------------------------------------------------------------
//����: ���Ƶ������ؿ������ֱ��,ʹ��DrawColor��Ϊ��ɫֵ.
//������hdc: ��ͼ�����ľ��.
//      x0,y0: ��ʼ����.
//      x1,y1: ��������.
//���أ���.
//------------------------------------------------------------------------------
void    DrawLine(HDC hdc,int x0,int y0,int x1,int y1)
{
    POINT pt[2];

    if(BeginDraw(hdc))
    {
        pt[0].x =x0;
        pt[0].y =y0;
        pt[1].x =x1;
        pt[1].y =y1;
        _LPtoDP(hdc,pt,2);

        GK_ApiLinetoIe(hdc->pGkWin,pt[0].x,pt[0].y,pt[1].x,pt[1].y,
                hdc->DrawColor,hdc->RopCode,hdc->SyncTime);
    }
}

//----ʹ�õ�ǰλ�û���-----------------------------------------------------------
//����: ���Ƶ������ؿ������ֱ��,ʹ��DrawColor��Ϊ��ɫֵ,ʹ�õ�ǰλ����Ϊ��ʼ����,
//     ������ɺ�,���������������Ϊ��ǰ����λ��.
//������hdc: ��ͼ�����ľ��.
//      x,y: ��������.
//���أ���.
//------------------------------------------------------------------------------
void    DrawLineTo(HDC hdc,int x,int y)
{
    POINT pt[2];

    if(BeginDraw(hdc))
    {
        pt[0].x =hdc->CurX;
        pt[0].y =hdc->CurY;
        pt[1].x =x;
        pt[1].y =y;

        _LPtoDP(hdc,pt,2);

        GK_ApiLinetoIe(hdc->pGkWin,pt[0].x,pt[0].y,pt[1].x,pt[1].y,
                hdc->DrawColor,hdc->RopCode,hdc->SyncTime);

        _MoveTo(hdc,x,y,NULL);
        EndDraw(hdc);
    }
}

//----��ָ��λ�û����ַ���-------------------------------------------------------
//����: ��ָ��λ�û����ַ���,ʹ��TextColor��Ϊ��ɫֵ,֧�ֻس��뻻�з�.
//������hdc: ��ͼ�����ľ��.
//      x,y: ����λ��.
//      text: �ַ���.
//      count: Ҫ���Ƶ��ַ�����,�ò���С��0ʱ,�����������ַ���.
//���أ�TRUE:�ɹ�; FALSE:ʧ��.
//------------------------------------------------------------------------------
BOOL    TextOut(HDC hdc,int x,int y,const char *text,int count)
{
	    POINT pt;

	    if(BeginDraw(hdc))
	    {
	        pt.x =x;
	        pt.y =y;
	        _LPtoDP(hdc,&pt,1);

	        x =pt.x;
	        y =pt.y;

	        if(count<0)
	        {
	          	count = strlen(text);
	        }

	        if(text!=NULL)
	        {
	           GK_ApiDrawText(hdc->pGkWin,hdc->pFont,hdc->pCharset,
	                                  x,y,text,count,hdc->TextColor,
	                                  hdc->RopCode,hdc->SyncTime);

	        }

	        EndDraw(hdc);
	        return TRUE;
	    }

	    return FALSE;
}

//----�����ַ�����ռ���ؿ��-----------------------------------------------------
//����: �ú���Ϊ�ڲ�����,����麯�������Ϸ���.
//������ft: ʹ�õ�������
//     text: �ַ���.
//     count: Ҫ������ַ�����,�ò���С��0ʱ,�����������ַ�����ռ���ؿ��.
//���أ��ַ������ؿ��.
//------------------------------------------------------------------------------
static  int _GetTextWidth(HDC hdc,const char *text,int count)
{
    int width;
    struct tagCharset *Set;
    struct tagFontRsc *Font;
    s32 len, loop;
    u32 wc;

    width=0;
    Set = hdc->pCharset;
    if(Set!=NULL)
    {
		Font = hdc->pFont;
		if(Font!=NULL)
		{
			for(loop = 0; loop < count;)
			{
				len= Set->mb_to_ucs4(&wc, text, count);
				if(len == -1)
				{ // ��Ч�ַ�
					text++;
					loop++;
				}
				else if(len == 0)
				{
					break;
				}
				else
				{ // ��Ч�ַ�
					text += len;
					loop += len;
					width += Font->GetCharWidth(wc);
				}
			}
		}
    }
    return width;
}

//----�����ַ�����ռ���δ�С-----------------------------------------------------
//����: �ú���Ϊ�ڲ�����,����麯�������Ϸ���.
//������ft: ʹ�õ�������
//     text: �ַ���.
//     count: Ҫ������ַ�����,�ò���С��0ʱ,�����������ַ�����ռ���ؿ��.
//     rect: ������ľ���.
//     flag: �ַ����Ʊ��.
//���أ���.
//------------------------------------------------------------------------------
static  void _AdjustTextRect(HDC hdc,const char *text,int count, RECT *prc,u32 flag)
{
    s32 Height;
    s32 i;

    if(count<0)
    {
    	count =strlen(text);
    }

    Height = Font_GetFontLineHeight(hdc->pFont);
    if(1)
    {
        switch(flag&DT_ALIGN_V_MASK)
        {
            case    DT_VCENTER:
                    prc->top += ((RectH(prc) - Height)>>1);
                    prc->bottom = prc->top + Height;
                    break;
                    ////
            case    DT_TOP:
                    break;
                    ////
            case    DT_BOTTOM:
                    prc->top += (RectH(prc) - Height);
                    prc->bottom =prc->top + Height;
                    break;
                    ////
            default:
                    break;

        }

        switch(flag&DT_ALIGN_H_MASK)
        {
            case    DT_CENTER:
                    i =_GetTextWidth(hdc,text,count);
                    i =(RectW(prc)-i)>>1;
                    prc->left  += i;
                    prc->right -= i;
                    break;
                    ////
            case    DT_LEFT:
                    prc->left  += 1;
                    break;
                    ////
            case    DT_RIGHT:
                    i =_GetTextWidth(hdc,text,count);
                    prc->left = prc->right-i-1;
                    break;
                    ////
            default:
                    break;

        }


    }
}

//----����һ���ַ����ж�����-----------------------------------------------------
//����: ��
//������str: �ַ���ָ��
//���أ��ַ�������.
//------------------------------------------------------------------------------
static	s32  __GetStrLineCount(const char *str)
{
	s32 count;
	char ch;

	count=0;
	while(1)
	{
		ch=*str;
		if(ch=='\n')
		{
			count++;
		}

		if(ch=='\0')
		{
			if(*(str-1)!='\n')
			{
				count++;
			}
			break;
		}

		str++;

	}
	return count;
}

//----�ھ��η�Χ�ڻ����ַ���-----------------------------------------------------
//����: ��ָ�����η�Χ�ڻ����ַ���,ʹ��TextColor��Ϊ��ɫֵ,֧�ֻس��뻻�з�,
//      �ú�������ָ���Ƿ�����ַ����߿�ͱ���,��ǰָ�����뷽ʽ�����.
//������hdc: ��ͼ�����ľ��.
//      x,y: ����λ��.
//      count: Ҫ���Ƶ��ַ�����,�ò���С��0ʱ,�����������ַ���.
//      prc: Ҫ���Ƶ����ھ���λ�úʹ�С.
//      flag: ���Ʊ�־����.
//���أ�TRUE:�ɹ�; FALSE:ʧ��.
//------------------------------------------------------------------------------
BOOL    DrawText(HDC hdc,const char *text,int count,const RECT *prc,u32 flag)
{

    RECT rc,rc0;
    int line_count;

    if(NULL==prc)
    {
        return FALSE;
    }

    if(NULL==text)
    {
        return FALSE;
    }

    if(count<0)
    {
    	count = strlen(text);
    }

    line_count =__GetStrLineCount(text);

    if(BeginDraw(hdc))
    {
    	if(line_count<=1)
    	{
    		//����
			_CopyRect(&rc0,prc);
			_CopyRect(&rc,prc);
			_InflateRect(&rc,-1,-1);
			_AdjustTextRect(hdc,text,count,&rc,flag);

			if(flag&DT_BORDER)
			{
				DrawRect(hdc,&rc0);
				InflateRect(&rc0,-1,-1);
			}

			if(flag&DT_BKGND)
			{
				FillRect(hdc,&rc0);
			}

		   TextOut(hdc,rc.left,rc.top,text,count);

    	}
    	else
    	{
    		//����
    		s32 x0,y0,i,line_h;
    		char *p0,*p1;

    		line_h = Font_GetFontLineHeight(hdc->pFont);

			_CopyRect(&rc0,prc);
			_CopyRect(&rc,prc);
			_InflateRect(&rc,-1,-1);

			if(flag&DT_BORDER)
			{
				DrawRect(hdc,&rc0);
				InflateRect(&rc0,-1,-1);
			}

			if(flag&DT_BKGND)
			{
				FillRect(hdc,&rc0);
			}

			y0 =rc.top;
            switch(flag&DT_ALIGN_V_MASK)
            {
                case    DT_VCENTER:
                		y0 += (RectH(&rc)-(line_count*line_h))>>1;
                        break;
                        ////
                case    DT_TOP:
                        break;
                        ////
                case    DT_BOTTOM:
                        y0 += (RectH(&rc) - (line_count*line_h));
                        break;
                        ////
                default:
                        break;

            }

            p0 =(char*)text;
            while(p0!=NULL)
            {
            	p1 =strchr(p0,'\n');
            	if(p1!=NULL)
            	{
            		i =(s32)((u32)p1-(u32)p0);
            	}
            	else
            	{
            		//��������һ��.
            		i =strlen(p0);
            	}

            	if(i<=0)
            	{
            		break;
            	}

            	//i += 1;
            	x0 =rc.left;
                switch(flag&DT_ALIGN_H_MASK)
                {
                    case    DT_CENTER:
                            x0 =(RectW(&rc)-_GetTextWidth(hdc,p0,i))>>1;
                            break;
                            ////
                    case    DT_LEFT:
                            x0  += 1;
                            break;
                            ////
                    case    DT_RIGHT:
                            x0 =(RectW(&rc)-_GetTextWidth(hdc,p0,i));
                            break;
                            ////
                    default:
                            break;

                }

                TextOut(hdc,x0,y0,p0,i);

                if(p1==NULL)
                {	//���һ����ʾ���,���˳�
                	break;
                }

                y0 += line_h;
                p0 += i+1;

            }


    	}

        EndDraw(hdc);
        return TRUE;
    }

    return FALSE;

}

//----���ƾ���------------------------------------------------------------------
//����: ʹ��DrawColor����һ�����ľ���.
//������hdc: ��ͼ�����ľ��.
//      prc: ���β���.
//���أ���.
//------------------------------------------------------------------------------
void    DrawRect(HDC hdc,const RECT *prc)
{
    int x0,y0;
    int x1,y1;
    POINT pt;

    if(prc != NULL)
    {
        if(BeginDraw(hdc))
        {
            pt.x =prc->left;
            pt.y =prc->top;
            _LPtoDP(hdc,&pt,1);

            x0 =pt.x;
            y0 =pt.y;
            x1 =x0+RectW(prc)-1;
            y1 =y0+RectH(prc)-1;

            //宸�
            GK_ApiLinetoIe(hdc->pGkWin,x0,y0,x0,y1,hdc->DrawColor,hdc->RopCode,hdc->SyncTime);
            //涓�
            GK_ApiLinetoIe(hdc->pGkWin,x0,y0,x1,y0,hdc->DrawColor,hdc->RopCode,hdc->SyncTime);
            //鍙�
            GK_ApiLinetoIe(hdc->pGkWin,x1,y0,x1,y1,hdc->DrawColor,hdc->RopCode,hdc->SyncTime);
            //涓�
            GK_ApiLinetoIe(hdc->pGkWin,x0,y1,x1,y1,hdc->DrawColor,hdc->RopCode,hdc->SyncTime);

            EndDraw(hdc);
        }
    }

}

//----������------------------------------------------------------------------
//����: ʹ��FillColor���һ��ʵ�ľ���.
//������hdc: ��ͼ�����ľ��.
//      prc: ���β���.
//���أ���.
//------------------------------------------------------------------------------
void    FillRect(HDC hdc,const RECT *prc)
{
    struct tagRectangle gk_rc;
    POINT pt;

    if(hdc!=NULL)
    {
        if(prc!=NULL)
        {
            if(BeginDraw(hdc))
            {
                pt.x =prc->left;
                pt.y =prc->top;
                _LPtoDP(hdc,&pt,1);

                gk_rc.left = pt.x;
                gk_rc.top = pt.y;
                gk_rc.right = pt.x+RectW(prc);
                gk_rc.bottom = pt.y+RectH(prc);

                GK_ApiFillRect(hdc->pGkWin,&gk_rc,hdc->FillColor,hdc->FillColor,
                            CN_FILLRECT_MODE_N,hdc->SyncTime);
                EndDraw(hdc);
            }
        }
    }

}

//----����������------------------------------------------------------------------
//����: ʹ��Color1��Ϊ��ʼɫ,Color2��Ϊ����ɫ,����������.
//������hdc: ��ͼ�����ľ��.
//      prc: ���β���.
//      Color1: ��ʼ��ɫ.
//      Color2: ������ɫ.
//      mode: �������ģʽ,����������ֵ:
//            GFILL_L_R:���������
//            GFILL_U_D:���ϵ���
//            GFILL_LU_RD:�����ϵ�����
//            GFILL_RU_LD:�����ϵ�����
//���أ���.
//------------------------------------------------------------------------------
void    GradientFillRect(HDC hdc,const RECT *prc,u32 Color1,u32 Color2,u32 mode)
{
    struct tagRectangle gk_rc;
    RECT rc;

    if(hdc!=NULL)
    {
        if(prc!=NULL)
        {
            if(BeginDraw(hdc))
            {
                _CopyRect(&rc,prc);

                _LPtoDP(hdc,(POINT*)&rc,2);

                gk_rc.left = rc.left;
                gk_rc.top = rc.top;
                gk_rc.right = rc.right;
                gk_rc.bottom = rc.bottom;

                switch(mode)
                {
                    case GFILL_L_R:
                            mode =CN_FILLRECT_MODE_H;
                            break;
                    case GFILL_U_D:
                            mode =CN_FILLRECT_MODE_V;
                            break;
                    case GFILL_LU_RD:
                            mode =CN_FILLRECT_MODE_SP;
                            break;
                    case GFILL_RU_LD:
                            mode =CN_FILLRECT_MODE_SN;
                            break;
                    default:
                            mode =CN_FILLRECT_MODE_N;
                            break;

                }
                GK_ApiFillRect(hdc->pGkWin,&gk_rc,Color1,Color2,
                            mode,hdc->SyncTime);
                EndDraw(hdc);
            }
        }
    }

}

/*============================================================================*/

void    DrawCircle(HDC hdc,int cx,int cy,int r)
{

}

void    FillCircle(HDC hdc,int cx,int cy,int r)
{

}


//----��������------------------------------------------------------------------
//����: ʹ��DrawColor��������.
//������hdc: ��ͼ�����ľ��.
//      pt: ���������.
//      count: ���������.
//���أ���.
//------------------------------------------------------------------------------
void    DrawPolyLine(HDC hdc,const POINT *pt,int count)
{
    int i;

    if(count>1)
    {
        for(i=1;i<count;i++)
        {
            DrawLine(hdc,pt[i-1].x,pt[i-1].y,pt[i].x,pt[i].y);
        }
    }
}

//----������Ͽ�------------------------------------------------------------------
//����: ʹ��DrawColor������Ͽ�߿�,ʹ��TextColor������Ͽ�����.
//������hdc: ��ͼ�����ľ��.
//      prc: ��Ͽ���β���.
//      Text:��Ͽ�����.
//���أ���.
//------------------------------------------------------------------------------

void    DrawGroupBox(HDC hdc,const RECT *prc,const char *Text)
{
    int i,text_w,text_h,text_offset;
    u32 old_color;

    POINT pt[6];
    RECT rc;

    rc.left =0;
    rc.top =0;
    rc.right =0;
    rc.bottom =0;
    _AdjustTextRect(hdc,Text,-1,&rc,DT_CENTER|DT_VCENTER);

    text_w =RectW(&rc);
    text_h =RectH(&rc);
    text_offset =8;

    pt[0].x =prc->left+text_offset;
    pt[0].y =prc->top+(RectH(&rc)>>1);

    pt[1].x =pt[0].x-text_offset;
    pt[1].y =pt[0].y;

    pt[2].x =pt[1].x;
    pt[2].y =prc->top+RectH(prc)-1;

    pt[3].x =pt[2].x+RectW(prc)-1;
    pt[3].y =pt[2].y;

    pt[4].x =pt[3].x;
    pt[4].y =pt[0].y;

    pt[5].x =pt[0].x+text_w;
    pt[5].y =pt[0].y;

    ////Bottom
    for(i=0;i<6;i++)
    {
        pt[i].x+=1;
        pt[i].y+=1;
    }

    old_color=GetDrawColor(hdc);
    SetDrawColor(hdc,AlphaBlendColor(RGB(255,255,255),old_color,128));
    DrawPolyLine(hdc,pt,6);

    ////Top
    for(i=0;i<6;i++)
    {
        pt[i].x-=1;
        pt[i].y-=1;
    }
    SetDrawColor(hdc,old_color);
    DrawPolyLine(hdc,pt,6);

    SetRect(&rc,pt[0].x+1,prc->top,text_w,text_h);
    DrawText(hdc,Text,-1,&rc,DT_CENTER|DT_VCENTER);


}

/*============================================================================*/
/*============================================================================*/
/*============================================================================*/
/*============================================================================*/
/*============================================================================*/


