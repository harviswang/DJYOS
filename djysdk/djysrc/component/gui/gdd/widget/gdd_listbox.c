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
//�ļ�����: �б��ؼ�ʵ��
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2015-03-25
//   ����:  LiuWei.
//   �°汾�ţ�V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------

#include    "gdd.h"
#include    "../include/gdd_private.h"
/*============================================================================*/

typedef struct{

	list_t node;
	u32 Flag;
	u32 Data;
	char szText[4];

}LISTBOX_ITEM;

typedef struct{
	list_t list;
	s32 ItemNum;
	s32 TopIndex;
	s32 CurSel;
	s32 ItemHeight;
	s32	xpos,ypos;

}LISTBOX_DATA;


static	LISTBOX_ITEM*	listbox_get_item(LISTBOX_DATA *pLB,u32 idx)
{
	LISTBOX_ITEM *item=NULL;
	list_t *n;
	int i;

	if(idx<pLB->ItemNum)
	{
		i=0;
		n =pLB->list.next;
		while(1)
		{
			if(n==&pLB->list)
			{
				break;
			}
			if(i==idx)
			{
				item =(LISTBOX_ITEM*)list_entry(n,LISTBOX_ITEM,node);
				break;
			}

			i++;
			n =n->next;

		}

	}
	return item;
}

static	s32  listbox_add_string(LISTBOX_DATA *pLB,s32 idx,const char *text)
{
	s32 i;
	list_t *n;
	LISTBOX_ITEM *item;

	if(pLB->ItemNum >= (s32)0x7FFFFFFE)
	{
		return (s32)-1;
	}

	i=strlen(text)+1;
	item =(LISTBOX_ITEM*)malloc(sizeof(LISTBOX_ITEM)+i);
	if(item == NULL)
	{
		return (s32)-1;
	}
	strcpy(item->szText,text);
	item->Flag =0;
	item->Data =0;

	i=0;
	n=pLB->list.next;
	while(1)
	{
		if(n == &pLB->list)
		{
			break;
		}
		n =n->next;

		if(i == idx)
		{
			break;
		}
		i++;
	}

	list_insert_before(n,&item->node);

	pLB->ItemNum++;

	if(pLB->CurSel<0)
	{
		pLB->CurSel=0;
	}

	if(pLB->TopIndex<0)
	{
		pLB->TopIndex=0;
	}

	return i;
}

static	void listbox_del_string(LISTBOX_DATA *pLB,s32 idx)
{
	LISTBOX_ITEM *item;
	if(pLB->ItemNum>0)
	{
		if(idx>=pLB->ItemNum)
		{
			idx=pLB->ItemNum-1;
		}

		item =listbox_get_item(pLB,idx);
		if(item!=NULL)
		{
			list_remove(&item->node);
			free(item);
		}

		pLB->ItemNum--;

		if(pLB->CurSel>=pLB->ItemNum)
		{
			pLB->CurSel=pLB->ItemNum-1;
		}

		if(pLB->TopIndex>=pLB->ItemNum)
		{
			pLB->TopIndex=pLB->ItemNum-1;
		}

	}
}

static	void listbox_reset_content(LISTBOX_DATA *pLB)
{
	list_t *n,*next;
	LISTBOX_ITEM *item;

	n=pLB->list.next;
	while(1)
	{
		if(n==&pLB->list)
		{
			break;
		}
		next =n->next;

		item =(LISTBOX_ITEM*)list_entry(n,LISTBOX_ITEM,node);
		free(item);

		n=next;

	}

	pLB->ItemNum =0;
	pLB->TopIndex =0;
	pLB->CurSel =-1;
}

static	u32 listbox_set_cur_sel(HWND hwnd,LISTBOX_DATA *pLB,s32 idx)
{

	if(pLB->ItemNum>0)
	{
		if(idx<0)
		{
			idx=pLB->ItemNum-1;
		}

		if(idx>=pLB->ItemNum)
		{
			idx=pLB->ItemNum-1;
		}

		if(pLB->CurSel!=idx)
		{
			pLB->CurSel=idx;
			SendMessage(GetParent(hwnd),MSG_NOTIFY,(LBN_SELCHANGE<<16)|((u16)hwnd->WinId),(ptu32_t)hwnd);
		}
	}
	return pLB->CurSel;
}

static	u32 listbox_set_top_index(LISTBOX_DATA *pLB,s32 idx)
{
	if(idx<0)
	{
		idx =pLB->ItemNum-1;
	}

	if(idx<pLB->ItemNum)
	{
		pLB->TopIndex=idx;
	}
	return pLB->TopIndex;
}

static	BOOL listbox_set_item_data(LISTBOX_DATA *pLB,s32 idx,u32 data)
{
	LISTBOX_ITEM *item;

	item =listbox_get_item(pLB,idx);
	if(item!=NULL)
	{
		item->Data =data;
		return TRUE;
	}
	return FALSE;
}

static	u32 listbox_get_item_data(LISTBOX_DATA *pLB,s32 idx)
{
	LISTBOX_ITEM *item;

	item=listbox_get_item(pLB,idx);
	if(item!=NULL)
	{
		return item->Data;
	}
	return 0;
}

static  void listbox_paint(HWND hwnd,LISTBOX_DATA *pLB)
{
	HDC hdc;
	RECT rc,rc0;
	list_t *n;
	LISTBOX_ITEM *item;
	s32 i;

	hdc =BeginPaint(hwnd);

	GetClientRect(hwnd,&rc0);
	if(GetWindowStyle(hwnd)&LBS_FLAT)
	{
		SetFillColor(hdc,RGB(200,200,200));
		FillRect(hdc,&rc0);
	}
	else
	{
		GradientFillRect(hdc,&rc0,
    					RGB(240,240,240),
						RGB(100,100,100),
						GFILL_U_D);
	}

	i=0;
	n=pLB->list.next;
	while(1)
	{
		if(n==&pLB->list)
		{	//�Ѿ������һ������.
			break;
		}

		if(i==pLB->TopIndex)
		{	//��TopIndex��ʼ����.
			break;
		}

		i++;
		n=n->next;
	}

	if(n!=&pLB->list)
	{
		SetRect(&rc,0,0,RectW(&rc0),pLB->ItemHeight);
		while(1)
		{	//ѭ�����Ʊ���.
			if(n == &pLB->list)
			{	//�Ѿ������һ������.
				break;
			}

			if(rc.top >= rc0.bottom)
			{	//�Ѿ����ڴ��ڷ�Χ��.
				break;
			}

			item =(LISTBOX_ITEM*)list_entry(n,LISTBOX_ITEM,node);
			if(i == pLB->CurSel)
			{	//Ϊ��ǰѡ����Ŀ.
				SetDrawColor(hdc,RGB(255,100,255));
				SetFillColor(hdc,RGB(128,0,160));
				SetTextColor(hdc,RGB(0,255,0));
				DrawText(hdc,item->szText,-1,&rc,DT_LEFT|DT_VCENTER|DT_BORDER|DT_BKGND);
			}
			else
			{
				SetTextColor(hdc,RGB(1,1,1));
				DrawText(hdc,item->szText,-1,&rc,DT_LEFT|DT_VCENTER);
			}

			OffsetRect(&rc,0,RectH(&rc));
			i++;
			n =n->next;

		}
	}

	EndPaint(hwnd,hdc);
}

static	void	listbox_lbutton_down(HWND hwnd,LISTBOX_DATA *pLB,int x,int y)
{
	RECT rc,rc0;
	LISTBOX_ITEM *item;
	int i;
	POINT pt;

	pt.x =x;
	pt.y =y;

	pLB->xpos =x;
	pLB->ypos =y;

	GetClientRect(hwnd,&rc0);
	SetRect(&rc,0,0,RectW(&rc0),pLB->ItemHeight);

	item =listbox_get_item(pLB,pLB->TopIndex);
	if(item!=NULL)
	{
		i=pLB->TopIndex;
		while(i<pLB->ItemNum)
		{
			if(rc.top>rc0.right)
			{
				break;
			}

			if(PtInRect(&rc,&pt))
			{
				listbox_set_cur_sel(hwnd,pLB,i);
				break;
			}

			OffsetRect(&rc,0,RectH(&rc));
			i++;
		}
	}

}

static	BOOL	listbox_mouse_move(LISTBOX_DATA *pLB,s32 x,s32 y)
{
	int i;

	if((y-pLB->ypos)>pLB->ItemHeight)
	{
		pLB->xpos =x;
		pLB->ypos =y;

		i=pLB->TopIndex;
		if(i>0)
		{
			i--;
		}
		listbox_set_top_index(pLB,i);
		return TRUE;
	}

	if((y-pLB->ypos)<(-(s32)pLB->ItemHeight))
	{
		pLB->xpos =x;
		pLB->ypos =y;

		i=pLB->TopIndex;
		if(i<pLB->ItemNum)
		{
			i++;
		}
		listbox_set_top_index(pLB,i);
		return TRUE;
	}

	return FALSE;

}

static	u32	listbox_get_text_len(LISTBOX_DATA *pLB,s32 idx)
{
	LISTBOX_ITEM *item;

	item =listbox_get_item(pLB,idx);
	return strlen(item->szText)+1;

}
static	BOOL	listbox_get_text(LISTBOX_DATA *pLB,s32 idx,char *buf)
{
	LISTBOX_ITEM *item;

	if(buf!=NULL)
	{
		item =listbox_get_item(pLB,idx);
		if(item!=NULL)
		{
			strcpy(buf,item->szText);
			return TRUE;
		}
		buf[0]='\0';
	}
	return FALSE;
}

u32 listbox_proc(MSG *pMsg)
{
    HWND hwnd;
    LISTBOX_DATA *pLB;
    u32 res;

    hwnd =pMsg->hwnd;
    switch(pMsg->Code)
    {
        case    MSG_CREATE:
        		pLB =(LISTBOX_DATA*)malloc(sizeof(LISTBOX_DATA));
        		if(pLB!=NULL)
        		{
        			list_init(&pLB->list);
        			pLB->ItemNum =0;
        			pLB->TopIndex =0;
        			pLB->CurSel =-1;
        			pLB->ItemHeight =20;
        			SetWindowPrivateData(hwnd,(void*)pLB);
					return TRUE;
        		}
        		return FALSE;
                ////

        case	MSG_LBUTTON_DOWN:
        		pLB =(LISTBOX_DATA*)GetWindowPrivateData(hwnd);
        		listbox_lbutton_down(hwnd,pLB,(s16)LO16(pMsg->Param2),(s16)HI16(pMsg->Param2));
        		InvalidateWindow(hwnd);
        		return TRUE;
        		////

        case	MSG_MOUSE_MOVE:
        		pLB =(LISTBOX_DATA*)GetWindowPrivateData(hwnd);
        		if(listbox_mouse_move(pLB,(s16)LO16(pMsg->Param2),(s16)HI16(pMsg->Param2)))
        		{
        			InvalidateWindow(hwnd);
        		}
        		return TRUE;
        		////

        case	LBM_ADDSTRING:
        		pLB =(LISTBOX_DATA*)GetWindowPrivateData(hwnd);
        		res =listbox_add_string(pLB,pMsg->Param1,(const char*)pMsg->Param2);
        		InvalidateWindow(hwnd);
        		return res;
        		////

        case	LBM_DELSTRING:
				pLB =(LISTBOX_DATA*)GetWindowPrivateData(hwnd);
				listbox_del_string(pLB,pMsg->Param1);
				InvalidateWindow(hwnd);
				return TRUE;
				////
        case	LBM_RESETCONTENT:
				pLB =(LISTBOX_DATA*)GetWindowPrivateData(hwnd);
				listbox_reset_content(pLB);
				InvalidateWindow(hwnd);
				return TRUE;
				////

        case	LBM_GETCOUNT:
        		pLB =(LISTBOX_DATA*)GetWindowPrivateData(hwnd);
        		return pLB->ItemNum;
         		////

        case	LBM_SETCURSEL:
       			pLB =(LISTBOX_DATA*)GetWindowPrivateData(hwnd);
       			res	=listbox_set_cur_sel(hwnd,pLB,pMsg->Param1);
       			InvalidateWindow(hwnd);
       			return res;
       			////

        case	LBM_GETCURSEL:
        		pLB =(LISTBOX_DATA*)GetWindowPrivateData(hwnd);
        		return pLB->CurSel;
         		////

        case	LBM_SETTOPINDEX:
       			pLB =(LISTBOX_DATA*)GetWindowPrivateData(hwnd);
       			res =listbox_set_top_index(pLB,pMsg->Param1);
       			InvalidateWindow(hwnd);
       			return res;
       			////

        case	LBM_GETTOPINDEX:
         		pLB =(LISTBOX_DATA*)GetWindowPrivateData(hwnd);
        		return pLB->TopIndex;
         		////

        case	LBM_SETITEMHEIGHT:
        		pLB =(LISTBOX_DATA*)GetWindowPrivateData(hwnd);
        		pLB->ItemHeight =pMsg->Param1;
        		InvalidateWindow(hwnd);
        		return TRUE;
         		////

        case	LBM_GETITEMHEIGHT:
        		pLB =(LISTBOX_DATA*)GetWindowPrivateData(hwnd);
        		return pLB->ItemHeight;
         		////

        case	LBM_SETITEMDATA:
        		pLB =(LISTBOX_DATA*)GetWindowPrivateData(hwnd);
        		return listbox_set_item_data(pLB,pMsg->Param1,pMsg->Param2);
        		////

        case	LBM_GETITEMDATA:
    			pLB =(LISTBOX_DATA*)GetWindowPrivateData(hwnd);
    			return listbox_get_item_data(pLB,pMsg->Param1);
    			////

        case	LBM_GETTEXTLEN:
   				pLB =(LISTBOX_DATA*)GetWindowPrivateData(hwnd);
   				return	listbox_get_text_len(pLB,pMsg->Param1);
   				////

        case	LBM_GETTEXT:
   				pLB =(LISTBOX_DATA*)GetWindowPrivateData(hwnd);
   				listbox_get_text(pLB,pMsg->Param1,(char*)pMsg->Param2);
   				return TRUE;
   				////

        case    MSG_PAINT:
        		pLB =(LISTBOX_DATA*)GetWindowPrivateData(hwnd);
        		listbox_paint(pMsg->hwnd,pLB);
                return TRUE;
                ////

        case    MSG_DESTROY:
        		pLB =(LISTBOX_DATA*)GetWindowPrivateData(hwnd);
        		if(pLB!=NULL)
        		{
        			SetWindowPrivateData(hwnd,NULL);
        			listbox_reset_content(pLB);
        			free(pLB);
        		}
                return TRUE;
                ////

        default:
                return DefWindowProc(pMsg);

    }
    return FALSE;

}
/*============================================================================*/
