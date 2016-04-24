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
//�ļ�����: ��ť�ؼ�ʵ��
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2014-11-10
//   ����:  LiuWei.
//   �°汾�ţ�V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include    "gdd.h"
#include    "../include/gdd_private.h"

/*============================================================================*/

static  int _get_button_type(HWND hwnd)
{
    return hwnd->Style&BS_TYPE_MASK;
}

static  void button_paint(MSG *pMsg)
{
    HWND hwnd;
    HDC hdc;
    RECT rc;

    hwnd=pMsg->hwnd;
    hdc =BeginPaint(hwnd);
    if(NULL!=hdc)
    {
        GetClientRect(hwnd,&rc);


        if(hwnd->Style&BS_PUSHED)
        {
            SetTextColor(hdc,RGB(255,255,255));

            SetFillColor(hdc,RGB(180,0,240));
            FillRect(hdc,&rc);
            OffsetRect(&rc,1,1);
        }
        else
        {
            SetTextColor(hdc,RGB(1,1,1));
            switch(hwnd->Style&BS_SURFACE_MASK)
            {
                case    BS_NICE:
                        GradientFillRect(hdc,&rc,
                                RGB(210,210,210),RGB(150,150,150),GFILL_U_D);
                        break;

                case    BS_SIMPLE:
                        SetDrawColor(hdc,RGB(220,220,220));
                        DrawLine(hdc,0,0,0,RectH(&rc)-1); //L
                        DrawLine(hdc,0,0,RectW(&rc)-1,0);   //U

                        SetDrawColor(hdc,RGB(80,80,80));
                        DrawLine(hdc,RectW(&rc)-1,0,RectW(&rc)-1,RectH(&rc)-1); //R
                        DrawLine(hdc,0,RectH(&rc)-1,RectW(&rc)-1,RectH(&rc)-1); //D

                        InflateRect(&rc,-1,-1);
                        SetFillColor(hdc,RGB(150,150,150));
                        FillRect(hdc,&rc);
                        break;

                case    BS_FLAT:
                default:
                        SetFillColor(hdc,RGB(140,140,140));
                        FillRect(hdc,&rc);
                        break;

            }


        }

        DrawText(hdc,hwnd->Text,-1,&rc,DT_VCENTER|DT_CENTER);
        EndPaint(hwnd,hdc);
    }


}

u32 button_proc(MSG *pMsg)
{
    HWND hwnd;
    RECT rc;

    hwnd =pMsg->hwnd;
    switch(pMsg->Code)
    {
        case    MSG_CREATE:
                GetWindowRect(hwnd,&rc);
                printf("btn[%04XH]: MSG_CREATE.\r\n",hwnd->WinId);
                return 1;
                ////
        case    MSG_LBUTTON_DOWN:
                {
//                    int x,y;
//                    x =LO16(pMsg->Param2);
//                    y =HI16(pMsg->Param2);
                    switch(_get_button_type(hwnd))
                    {
                        case    BS_NORMAL:  //���水ť
                                hwnd->Style |= BS_PUSHED;
                                SendMessage(GetParent(hwnd),MSG_NOTIFY,(BTN_DOWN<<16)|(hwnd->WinId),(ptu32_t)hwnd);
                                break;
                                ////
                        case    BS_HOLD:    //������ť
                                hwnd->Style ^= BS_PUSHED;
                                if(hwnd->Style&BS_PUSHED)
                                {
                                    SendMessage(GetParent(hwnd),MSG_NOTIFY,(BTN_DOWN<<16)|(hwnd->WinId),(ptu32_t)hwnd);
                                }
                                else
                                {
                                    SendMessage(GetParent(hwnd),MSG_NOTIFY,(BTN_UP<<16)|(hwnd->WinId),(ptu32_t)hwnd);
                                }
                                break;
                                ////

                    }

                    InvalidateWindow(hwnd);
                }
                break;
                ////
        case    MSG_LBUTTON_UP:
                {
//                    int x,y;
//                    x =LO16(pMsg->Param2);
//                    y =HI16(pMsg->Param2);
                    switch(_get_button_type(hwnd))
                    {
                        case    BS_NORMAL:
                                hwnd->Style &= ~BS_PUSHED;
                                SendMessage(GetParent(hwnd),MSG_NOTIFY,(BTN_UP<<16)|(hwnd->WinId),(ptu32_t)hwnd);
                                break;
                                ////
                        case    BS_HOLD:
                                break;
                                ////

                    }
                    InvalidateWindow(hwnd);

                }
                break;
                ////
        case    MSG_MOUSE_MOVE:
                {
//                    int x,y;
//                    x =LO16(pMsg->Param2);
//                    y =HI16(pMsg->Param2);
                    //printf("btn: MSG_MOUSE_MOVE: %d,%d\r\n",x,y);
                }
                break;
                ////

        case    MSG_PAINT:
            //  printf("btn[%04XH]: MSG_PAINT.\r\n",hwnd->WinId);
                button_paint(pMsg);
                return 1;
                ////

        case    MSG_DESTROY:
                printf("btn[%04XH]: MSG_DESTROY.\r\n",hwnd->WinId);
                return 1;
                ////

        default:
                return DefWindowProc(pMsg);

    }
    return 0;

}
/*============================================================================*/




