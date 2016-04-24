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
//�ļ�����: ���ؼ�ʵ��
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2015-03-31
//   ����:  LiuWei.
//   �°汾�ţ�V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------

#include    "gdd.h"
#include    "../include/gdd_private.h"
/*============================================================================*/
/*
LVM_INSERTCOLUMN  �����У�wParam Ϊ���ͣ�ָ���кţ�lParam Ϊָ��LV_COLUMN�ṹ��ָ��
LVM_SETCOLUMN  �����У�����ͬ��
LVM_INSERTITEM  ������Ŀ������Ŀ��wParam Ϊ0��lParam Ϊָ��LV_ITEM�ṹ��ָ��
LVM_SETITEM  ������Ŀ������Ŀ������ͬ��
LVM_GETITEM  ȡ����Ŀ������Ŀ������ͬ��
LVM_GETNEXTITEM  ȡ����һ����Ŀ������Ŀ����������ȡ�ù��ѡ�����Ŀ
LVM_DELETEITEM  ɾ����Ŀ������Ŀ��wParam Ϊ���ͣ�ָ����Ŀ�����ţ�lParam Ϊ0
LVM_DELETEALLITEMS  ɾ��������Ŀ��wParam �� lParam ��Ϊ0
LVM_SETTEXTCOLOR  ����������ɫ��wParam Ϊ0��lParam Ϊ��ɫ��RGBֵ
LVM_SETTEXTBKCOLOR  �������ֱ���ɫ������ͬ��
LVM_SETBKCOLOR  ���ñ���ɫ������ͬ��
*/

typedef	struct{

	list_t	list_col;

}LISTVIEW_DATA;


u32 listview_proc(MSG *pMsg)
{
    HWND hwnd;
    u32 res;

    hwnd =pMsg->hwnd;
    switch(pMsg->Code)
    {
    	case	MSG_CREATE:
    			break;
        default:
                 return DefWindowProc(pMsg);

    }

    return FALSE;
}
