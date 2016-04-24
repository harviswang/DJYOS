//----------------------------------------------------
// Copyright (c) 2014, SHENZHEN PENGRUI SOFT CO LTD. All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:

// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.

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
// �����Ȩ�����ʹ���߷������������������£�����ʹ����ʹ�ü���ɢ����
// �����װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б���
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б��Լ�����
//    ������������

// ����������������Ǳ������Ȩ�������Լ�����������״��"as is"���ṩ��
// �������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
//-----------------------------------------------------------------------------
// MenuDisplay_pull.c
// ģ������:
// ģ��汾:
// ����ʱ��: 2013-1-6
// ����    : Administrator
// =======================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// =======================================================================

#include  "MenuDisplay_pull.h"
#include  "MenuDisplayGeneral.h"
#include  "MenuSpaceGeneral.h"
#include  "MenuitemApi.h"

// =========================================================================
// �������ܣ���״̬��(���ұ�)(����ģʽ)
// �������:dispara���������ʾ����
// ���������
// ����ֵ  :
// ˵��    :һ�����dispara�����в˵�����Ϊ��Ļ��ԭ�򲻿���ͬʱ��ʾ��������״̬��
//          ����ǰ�����Ĳ˵�����ͬ�����в˵��е�λ��
// =========================================================================
void drawstatecontent_pull_rightside(struct menu_displaypara  *dispara)
{
    u8  numOp=0;
    u8  numTotal=0;
    s32 lengthT=0;
    s32 LengthE=0;
    struct tagRectangle  state_area={0,0,0,0};
    numOp=CalMenuitemNumber(dispara->OperatingMenuitem);
    numTotal=CalMenuitemNum(dispara->FVisualMenuitem);
    dispara=getmainmenudispara(dispara);
    //��ԭ��ɫ
       state_area.left=dispara->CurWin->right-dispara->CurWin->left-cn_column_space;
       state_area.right=dispara->CurWin->right-dispara->CurWin->left;

       state_area.top=0;
       state_area.bottom=dispara->CurWin->bottom-dispara->CurWin->top;
       GK_ApiFillRect(dispara->CurWin, &state_area, color_back,
                      0,CN_FILLRECT_MODE_N,0);
       //��־��ǰ�Ŀ�����
       lengthT=state_area.bottom-state_area.top;
       //��߾��ȣ�
       lengthT=lengthT*1000;

       LengthE=(lengthT+numTotal-1)/numTotal;

       state_area.top=(numOp-1)*LengthE;
       state_area.top=(state_area.top)/1000;

       state_area.bottom=(numOp)*LengthE;
       state_area.bottom=(state_area.bottom)/1000;

       GK_ApiFillRect(dispara->CurWin, &state_area, color_light,
                      0,CN_FILLRECT_MODE_N,0);
       GK_ApiSyncShow(CN_TIMEOUT_FOREVER);
        return;
}
// =========================================================================
// �������ܣ���״̬��(��ױ�)
// �������:dispara���������ʾ����
// ���������
// ����ֵ  :
// ˵��    :һ�����dispara�����в˵�����Ϊ��Ļ��ԭ�򲻿���ͬʱ��ʾ��������״̬��
//          ����ǰ�����Ĳ˵�����ͬ�����в˵��е�λ��
// =========================================================================
//void DrawStateContentBottom(struct menu_displaypara  *dispara)
//{
//
//       struct tagRectangle  state_area={0,0,0,0};
//
//
//     u8  numOp=0;
//     u8  numTotal=0;
//     s32 lengthT=0;
//     s32 LengthE=0;
//
//
//     numOp=CalMenuitemNumber(dispara->OperatingMenuitem);
//     numTotal=CalMenuitemNum(dispara->FVisualMenuitem);
//
//    //��״̬�����������һ��ҳ�ڵ���
////      dispara=SearchLatestPageNode(dispara);//����ҳ�ڵ����������Ҫ��һ���Ŀ���
//      //��ԭ��ɫ
//       state_area.left=0;
//     state_area.right=dispara->CurWin->right-dispara->CurWin->left;
//
//     state_area.top=dispara->CurWin->bottom-dispara->CurWin->top-cn_column_space;
//     state_area.bottom=dispara->CurWin->bottom-dispara->CurWin->top;
//        GK_ApiFillRect(dispara->CurWin, &state_area, color_back,
//                       0,CN_FILLRECT_MODE_N,0);
//     //��־��ǰ�Ŀ�����
//     lengthT=state_area.right-state_area.left;
//     //��߾��ȣ�
//     lengthT=lengthT*1000;
//
//     LengthE=(lengthT+numTotal-1)/numTotal;
//
//     state_area.left=(numOp-1)*LengthE;
//     state_area.left=(state_area.left)/1000;
//
//     state_area.right=(numOp)*LengthE;
//     state_area.right=(state_area.right)/1000;
//        GK_ApiFillRect(dispara->CurWin, &state_area, color_light,
//                       0,CN_FILLRECT_MODE_N,0);
//      GK_ApiSyncShow(CN_TIMEOUT_FOREVER);
//        return;
//}

// =========================================================================
// ��������:������ʾ�����ķ�����
// �������:����ʾ����ʾ����dispara
// ˵��          :
//          ������ʾ������
//          Ŀǰ������ʾ�������õ�����������������ķ�ʽ���Ӳ˵������������ķ�ʽ
// =========================================================================
void  menu_display_pull(struct menu_displaypara  *dispara)
{

   if((NULL==dispara)||(NULL==dispara->OperatingMenuitem)||(NULL==dispara->CurWin))
   {
     printf("Invalid parameter---menu_display_pull!\n");
     return ;
   }

   if(NULL==dispara->pre)//���˵���ˮƽ��ʾ���ǲ��û�״̬����
   {
      display_menu_horizontal(dispara);
      drawstatecontent_pull_rightside(dispara);
   }
   else//�Ӳ˵�������ģʽ
   {
      display_menu_vertical(dispara);
      drawstatecontent_pull_rightside(dispara);
   }
   printf("Operatingmenu=%s\n",dispara->OperatingMenuitem->node.name);

    return ;
}

