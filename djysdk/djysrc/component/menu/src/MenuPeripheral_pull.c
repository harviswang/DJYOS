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
// MenuKeyBoard_pull.c
// ģ������:
// ģ��汾:
// ����ʱ��: 2012-12-27
// ����          : Administrator
// =======================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// =======================================================================
#include "MenuPeripheral_pull.h"

#include "MenuPeripheralGeneral.h"
#include "MenuDisplayGeneral.h"
#include "MenuitemApi.h"
// =========================================================================
// ��������:�ϼ���Ӧ����
// �������:operatingDispara����ǰ���ڲ����Ĳ˵���
// ���������
// ����ֵ  :�������µĵ�ǰ���ڲ����Ĳ˵���ʾ����
// ˵��    :�������˵��У������ϼ������˵�������Ӧ���Ӳ˵����л���ǰ��һ���˵���
// =========================================================================
struct menu_displaypara* KeyUpAction_pull(struct menu_displaypara *operatingDispara)
{
  if(NULL==operatingDispara->pre)//���˵�
  {
//    GotoMenuitemInSLayer_pull(operatingDispara, -3);
  }
  else//�Ӳ˵�
  {
    goto_premenuitem(operatingDispara);
  }

  return operatingDispara;
}
// =========================================================================
// ��������:�¼���Ӧ����
// �������:operatingDispara����ǰ���ڲ����Ĳ˵���
// ���������
// ����ֵ  :�������µĵ�ǰ���ڲ����Ĳ˵���
// ˵��    :������ģʽ�У������¼������˵����л����Ӳ˵���û���Ӳ˵��Ļ�ִ��
//              ��ǰ�˵���Ĺ��Ӻ����������Ӳ˵����л�����һ���˵���
// =========================================================================
struct menu_displaypara* KeyDownAction_pull(struct menu_displaypara *operatingDispara)
{
  if(NULL==operatingDispara->pre)//���˵�
  {
    if(GetSonMenuitem(operatingDispara->OperatingMenuitem))
        {
           operatingDispara=goto_submenu(operatingDispara);
        }
    else
        {
        operatingDispara= exe_menuitemfunc(operatingDispara);
        }

  }
  else//�Ӳ˵�
  {
    goto_nextmenuitem(operatingDispara);
  }

  return operatingDispara;
}
// =========================================================================
// ��������:�Ҽ���Ӧ����
// �������:operatingDispara����ǰ���ڲ����Ĳ˵���
// ���������
// ����ֵ  :�������µĵ�ǰ���ڲ����Ĳ˵���
// ˵��    :������ģʽ�У������Ҽ����л�����һ���˵�������Ӳ˵��������ǰ�˵�����
//          �Ӳ˵����չ���Ӳ˵������ִ�е�ǰ�˵���Ӻ���
// =========================================================================
struct menu_displaypara* KeyRightAction_pull(struct menu_displaypara *operatingDispara)
{
  if(NULL==operatingDispara->pre)//���˵�
  {
    goto_nextmenuitem(operatingDispara);
  }
  else//�Ӳ˵�
  {
    if(GetSonMenuitem(operatingDispara->OperatingMenuitem))
        {
           operatingDispara=goto_submenu(operatingDispara);
        }
    else
        {
         operatingDispara=exe_menuitemfunc(operatingDispara);
        }
  }

  return operatingDispara;
}
// =========================================================================
// ��������:�����Ӧ����
// �������:operatingDispara����ǰ���ڲ����Ĳ˵���
// ���������
// ����ֵ    :�������µĵ�ǰ���ڲ����Ĳ˵���
// ˵��        :������ģʽ�У�������������˵�����ǰ��ǰ��һ���˵���Ӳ˵����л��ظ��˵�
// =========================================================================
struct menu_displaypara* KeyLeftAction_pull(struct menu_displaypara* operatingDispara)
{
  if(NULL==operatingDispara->pre)//���˵�
  {
    goto_premenuitem(operatingDispara);
  }
  else//�Ӳ˵�
  {
    operatingDispara=goto_parmenu(operatingDispara);
  }

  return operatingDispara;
}
// =========================================================================
// ��������:ȷ�ϼ���Ӧ����
// �������:operatingDispara����ǰ���ڲ����Ĳ˵���
// ���������
// ����ֵ  :�������µĵ�ǰ���ڲ����Ĳ˵���
// ˵��   :�������˵��У�����ȷ�ϼ�����ǰ�˵������Ӳ˵����չ���Ӳ˵�������
//         ִ�е�ǰ�˵���Ĺ��Ӻ���
// =========================================================================
struct menu_displaypara* KeyEnterAction_pull(struct menu_displaypara* operatingDispara)
{
    if(GetSonMenuitem(operatingDispara->OperatingMenuitem))
        {
           operatingDispara=goto_submenu(operatingDispara);
        }
    else
        {
         operatingDispara=exe_menuitemfunc(operatingDispara);
        }
  return operatingDispara;

}
// =========================================================================
// ��������:esc����Ӧ����
// �������:operatingDispara����ǰ���ڲ����Ĳ˵���
// ���������
// ����ֵ    :�������µĵ�ǰ���ڲ����Ĳ˵���
// ˵��        :�ھŹ����У����ڷ��ؼ������˵�����Ӧ���Ӳ˵����л��Ḹ�˵�
// =========================================================================
struct menu_displaypara*  KeyEscAction_pull(struct menu_displaypara* operatingDispara)
{
  if(NULL==operatingDispara->pre)//���˵�
  {
    printf("This is the main menu,can not Esc any more!\n");
  }
  else//�Ӳ˵�
  {
    operatingDispara=goto_parmenu(operatingDispara);
  }

  return operatingDispara;

}

// =========================================================================
// ��������:���̽�����
// �������:operatingDispara����ǰ���ڲ����Ĳ˵��keyvalue�����ܵļ���ֵ
// ���������
// ����ֵ  :�������µĵ�ǰ���ڲ����Ĳ˵���
// ˵��   :���ݲ�ͬ��ֵ������Ӧ����Ӧ����
// =========================================================================
struct menu_displaypara *keyboardaction_pull(struct menu_displaypara *operatingDispara,void *para)
{
    u8 keyvalue=*(u8 *)(para);
   if(operatingDispara==NULL)
   {
       printf("Invalid parameter!----keyboardaction_pull\n");
       return operatingDispara;
   }
   switch(keyvalue)
    {
      case KEYL://chage current operating menu to previouse// key:LEFT
        operatingDispara= KeyLeftAction_pull(operatingDispara);
         break;
      case KEYR://change current operating menu to next   //key :RIGHT
         operatingDispara=KeyRightAction_pull(operatingDispara);
         break;
      case KETU://execute the menu                       //key :UP
          operatingDispara=KeyUpAction_pull(operatingDispara);
         break;
      case KEYD://hide curent layser menu                //key : DOWN
         operatingDispara=KeyDownAction_pull(operatingDispara);
         break;
      case KEYE://list the current menu's sub menu       //key : ENTER
         operatingDispara=KeyEnterAction_pull(operatingDispara);
         break;
      case KEYESC:
        operatingDispara=KeyEscAction_pull(operatingDispara);
        break;

     default:
         printf("Un defined key action!\n");
         break;
    }

   return operatingDispara;
}


struct menu_displaypara*  menutouchlcd_pull(struct menu_displaypara *operatingDispara,void *para)
{
   struct coordinate_point  mypoint={0,0};
   if((NULL==operatingDispara)||(NULL==operatingDispara->FVisualMenuitem)||\
      (NULL==operatingDispara->LVisualMenuitem)||(NULL==operatingDispara->OperatingMenuitem)||\
      (NULL==operatingDispara->CurWin)||(NULL==para))
   {
           printf("Invalid parameter!-----TouchLcdAction_pull\n");
           return operatingDispara;
   }
   mypoint=*(struct coordinate_point*)para;
   if(NULL==operatingDispara->pre)//���˵�
   {
       operatingDispara=touchlcdactionhorizontal(operatingDispara,mypoint);
   }
   else//�Ӳ˵�
   {
       operatingDispara=touchlcdactionvertical(operatingDispara,mypoint);
   }
   return operatingDispara;
}
