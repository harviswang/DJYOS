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
// ģ������: DJY�˵�ģ��֮�˵���Ӻ�������
// ģ��汾: V1.00(��ʼ���汾)
// ������Ա: zqf
// ����ʱ��: 20121224
// ˵��          �������Ҫ�������Ĺ��Ӻ���������������ӹ��Ӻ����Ķ��壬����ӵ�
//          ��Ӧ�ĺ������С�
// =======================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// =======================================================================
#include "stdint.h"
#include "MenuFunction.h"
#include "MenuDisplay.h"
// todo : ��ע��
void  Menuitem_func(struct menu_item *menuitem)
{

  bool_t  result=true;
  u8  i=0;
  char *content="Welcome-DJYOS!";

  if((NULL==menuitem)||(NULL==menuitem->myprivate))
  {
      return;
  }
  else
  {
      printf("In menuitem function!--menuitem name=%s\n",menuitem->node.name);
  }

  GK_ApiDrawText(menuitem->myprivate,NULL,NULL,0,30,content,strnlen(content,namesize),CN_COLOR_BLUE,0,CN_TIMEOUT_FOREVER);
  //
  for(i=0;i<3;i++)
  {
      printf("wait_%d second\n",i);
      Djy_EventDelay(1000*mS);
  }
  return;
}

void  ConnectSwitcher(struct menu_item *menuitem)//�϶�·��
{

  printf("In menuitem function!---Begin to connect the switcher!\n");

}
void  CheckEquipment(struct menu_item *menuitem)//�鿴װ����Ϣ
{
    bool_t  result;
    u8      keyvalue;
    int     i=0;
    s32    width=0;
    s32    height=0;

    struct draw_wintext_para para;

    struct tagInputDeviceMsg *input_msg;
    struct tagGkWinRsc  CheckWin_text;
    struct tagGkWinRsc  *Appwin=NULL;
    char*   content="װ������:/װ�ò���:/���RCR :/��������:";

    if((NULL==menuitem)||(NULL==menuitem->myprivate))
    {
       printf("Invalid parameter---CheckEquipment\n");
       return ;
    }
    Appwin=(struct tagGkWinRsc*)(menuitem->myprivate);

    width=Appwin->right-Appwin->left;
    height=Appwin->bottom-Appwin->top;
    result=GK_ApiCreateGkwin(Appwin, &CheckWin_text, 0, 0, width/2+16, height,\
                             0x00202060, CN_WINBUF_NONE, "Checkwin_text",0,0,0, 0);


    para.colorback=0x00202060;
    para.colortext=CN_COLOR_WHITE;
    para.gkwin=&CheckWin_text;
    para.x=0;
    para.y=0;
    para.count=strlen(content);
//    windows_draw_text(&para,content);
    GK_ApiDrawText(&CheckWin_text,NULL,NULL,0,0,"װ�����ƣ�",10,0,CN_COLOR_BLUE,0);
    GK_ApiDrawText(&CheckWin_text,NULL,NULL,0,16,"װ�ò�����",10,0,CN_COLOR_BLUE,0);
    GK_ApiDrawText(&CheckWin_text,NULL,NULL,0,32,"���CRC��",9,0,CN_COLOR_BLUE,0);
    GK_ApiDrawText(&CheckWin_text,NULL,NULL,0,48,"�������ڣ�",10,0,CN_COLOR_BLUE,CN_TIMEOUT_FOREVER);

    for ( ; ; )
    {
//      input_msg = (struct tagInputDeviceMsg *)Djy_GetEventPara(NULL,NULL);
        if(Stddev_ReadDefaultMsg(&input_msg,20*mS))
        {
            //�鿴�Ƿ񵥵㴥����
            if(input_msg->input_type == EN_STDIN_SINGLE_TOUCH)
            {


            }
            else if(input_msg->input_type == EN_STDIN_KEYBOARD)//�鿴�Ƿ����
            {
                //�����ɿ���ȷ�ϣ�������ж��Ƿ����
                if(input_msg->input_data.key_board.key_value[1] != CN_BREAK_CODE)
                {

              //������Ӧ����
                   keyvalue= input_msg->input_data.key_board.key_value[0];
                 if(keyvalue==27)
                   {
                       printf("received a key:%02d\n",keyvalue);
                       break;
                   }
                   else
                   {
                       printf("received a key:%02d\n",keyvalue);
                   }

                }
            }
//          Djy_ParaUsed(0);
        }
//      Djy_WaitEvttPop(Djy_MyEvttId(),1,20*mS);
    }

    GK_ApiDestroyWin(&CheckWin_text);
    return;
}

struct menu_item_function   menufunc_tab[]=//menu item hook function tab
{
 {"Menuitem_func",Menuitem_func},

 {"ConnectSwitcher",ConnectSwitcher},
 {"CheckEquipment",CheckEquipment}

};


void NULL_FUNC(void)
{

   printf("This is an NULL func,you have not supply func yet!\n");
}
const u16 MenuitemFuncSize=sizeof(menufunc_tab)/sizeof(struct menu_item_function);


// =========================================================================
// ��������:��������Ѱ����صĲ˵����Ӻ���
// �������:���Ӻ���������
// ���������
// ����ֵ  :
// ˵��    :Ѱ�ҵ�ͬ���Ĺ��Ӻ���
// =========================================================================
void*   getfunc_byname(char *funcname)
{
   u16 i=0;
   while(i<MenuitemFuncSize)
   {
     if(0==strcmp(funcname,menufunc_tab[i].Funcname))
     {
        break;
     }
     else
     {
       i++;
     }
   }
   if(i==MenuitemFuncSize)
   {

     return NULL_FUNC;
   }
   else
   {
    return menufunc_tab[i].Func;
   }
}







