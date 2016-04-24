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
//�ļ�����:�������ؼ�ʵ��
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2015-03-23
//   ����:  LiuWei.
//   �°汾�ţ�V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------

#include    "gdd.h"
#include    "../include/gdd_private.h"
/*============================================================================*/

static  void progressbar_paint(HWND hwnd,PROGRESSBAR_DATA *pPB)
{
    HDC hdc;
    RECT m_rc[2];
    RECT rc0;
    int gfill_mode=0;

    hdc =BeginPaint(hwnd);
    if(NULL!=hdc)
    {
        GetClientRect(hwnd,&rc0);

        if(hwnd->Style&PBS_VER)
        {
        	gfill_mode =GFILL_L_R;
        	if(pPB->Flag&PBF_ORG_BOTTOM)
        	{
        		MakeProgressRect(m_rc,&rc0,pPB->Range,pPB->Pos,PBM_BOTTOM);
        	}
        	else
        	{
            	MakeProgressRect(m_rc,&rc0,pPB->Range,pPB->Pos,PBM_TOP);
        	}
        }
        else
        {
        	gfill_mode =GFILL_U_D;
        	if(pPB->Flag&PBF_ORG_RIGHT)
        	{
        		MakeProgressRect(m_rc,&rc0,pPB->Range,pPB->Pos,PBM_RIGHT);
        	}
        	else
        	{
        		MakeProgressRect(m_rc,&rc0,pPB->Range,pPB->Pos,PBM_LEFT);
        	}

        }

        if(hwnd->Style&PBS_FLAT)
        {
			SetFillColor(hdc,pPB->FGColor);
			FillRect(hdc,&m_rc[0]);
			SetFillColor(hdc,pPB->BGColor);
			FillRect(hdc,&m_rc[1]);
        }
        else
        {

            GradientFillRect(hdc,&m_rc[0],
            				AlphaBlendColor(pPB->FGColor,RGB(250,250,250),100),
    						AlphaBlendColor(pPB->FGColor,RGB(10,10,10),160),
							gfill_mode);

            GradientFillRect(hdc,&m_rc[1],
    						AlphaBlendColor(pPB->BGColor,RGB(250,250,250),100),
    						AlphaBlendColor(pPB->BGColor,RGB(10,10,10),160),
							gfill_mode);

        }

        if(pPB->Flag&PBF_SHOWTEXT)
        {
        	SetTextColor(hdc,pPB->TextColor);
        	DrawText(hdc,hwnd->Text,-1,&rc0,pPB->DrawTextFlag);
        }
        EndPaint(hwnd,hdc);
    }
}

u32 progressbar_proc(MSG *pMsg)
{
    HWND hwnd;
    RECT rc;
    PROGRESSBAR_DATA *pPB;

    hwnd =pMsg->hwnd;
    switch(pMsg->Code)
    {
        case    MSG_CREATE:
        		pPB =(PROGRESSBAR_DATA*)malloc(sizeof(PROGRESSBAR_DATA));
        		if(pPB!=NULL)
        		{
        			if(pMsg->Param1==NULL)
        			{
        				pPB->Flag =0;
						pPB->Range =100;
						pPB->Pos   =0;
						pPB->FGColor =RGB(200,0,0);
						pPB->BGColor =RGB(0,0,200);
						pPB->DrawTextFlag =DT_VCENTER|DT_CENTER;
        			}
        			else
        			{
        				PROGRESSBAR_DATA *pPB_Usr;

        				pPB_Usr =(PROGRESSBAR_DATA*)pMsg->Param1;
        				pPB->Flag =pPB_Usr->Flag;
        				pPB->Range =pPB_Usr->Range;
        				pPB->Pos =pPB_Usr->Pos;
        				pPB->FGColor =pPB_Usr->FGColor;
        				pPB->BGColor =pPB_Usr->BGColor;
        				pPB->DrawTextFlag =pPB_Usr->DrawTextFlag;

        			}
        			SetWindowPrivateData(hwnd,(void*)pPB);
					return TRUE;
        		}
        		return FALSE;
                ////
        case	PBM_SETDATA:
        		if(pMsg->Param1!=NULL)
        		{
					pPB =(PROGRESSBAR_DATA*)GetWindowPrivateData(hwnd);
					if(pPB!=NULL)
					{
						memcpy(pPB,(char*)pMsg->Param1,sizeof(PROGRESSBAR_DATA));
						InvalidateWindow(pMsg->hwnd);
						return TRUE;
					}
        		}
        		return FALSE;
        		////
        case	PBM_GETDATA:
        		if(pMsg->Param1!=NULL)
        		{
					pPB =(PROGRESSBAR_DATA*)GetWindowPrivateData(hwnd);
					if(pPB!=NULL)
					{
						memcpy((char*)pMsg->Param1,pPB,sizeof(PROGRESSBAR_DATA));
						InvalidateWindow(pMsg->hwnd);
						return TRUE;
					}
        		}
        		return FALSE;
        		////
        case	PBM_SETRANGE:
        		pPB =(PROGRESSBAR_DATA*)GetWindowPrivateData(hwnd);
        		if(pPB!=NULL)
        		{
        			pPB->Range =(u32)pMsg->Param1;
        			InvalidateWindow(pMsg->hwnd);
        		}
        		return TRUE;
        		////
        case	PBM_GETRANGE:
				pPB =(PROGRESSBAR_DATA*)GetWindowPrivateData(hwnd);
				if(pPB!=NULL)
				{
					return pPB->Range;
				}
				return 0;
				////
        case	PBM_SETPOS:
        		pPB =(PROGRESSBAR_DATA*)GetWindowPrivateData(hwnd);
        		if(pPB!=NULL)
        		{
        			pPB->Pos =(u32)pMsg->Param1;
        			InvalidateWindow(pMsg->hwnd);
        		}
        		return TRUE;
        		////
        case	PBM_GETPOS:
				pPB =(PROGRESSBAR_DATA*)GetWindowPrivateData(hwnd);
				if(pPB!=NULL)
				{
					return pPB->Pos;
				}
				return 0;
				////
        case    MSG_PAINT:
        		pPB =(PROGRESSBAR_DATA*)GetWindowPrivateData(hwnd);
        		progressbar_paint(pMsg->hwnd,pPB);
                return TRUE;
                ////
        case    MSG_DESTROY:
        		pPB =(PROGRESSBAR_DATA*)GetWindowPrivateData(hwnd);
        		if(pPB!=NULL)
        		{
        			free(pPB);
        			SetWindowPrivateData(hwnd,NULL);
        		}
                return TRUE;
                ////

        default:
                return DefWindowProc(pMsg);

    }
    return FALSE;

}
/*============================================================================*/
