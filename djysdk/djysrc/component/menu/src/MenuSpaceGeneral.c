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
// ģ������: DJY�˵�ģ��֮��ʾ�ռ�����ͨ�ú�������
// ģ��汾: V1.00(��ʼ���汾)
// ������Ա: zqf
// ����ʱ��: 20121224
// =======================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// =======================================================================
#include "MenuSpaceGeneral.h"
#include "MenuitemApi.h"
//Ĭ�ϵ���ɫ


// =========================================================================
// �������ܣ���ʼ��һ�����νṹ��
// ���������
// ���������
// ����ֵ  �� һ���ڲ���ԱȫΪ0�ľ��νṹ��
// =========================================================================
struct tagRectangle  zero_strect(void)
{
  struct tagRectangle result={0,0,0,0};
  return result;
}


// =========================================================================
// �������ܣ�Ϊ��ʾ��������ռ䣬�������ʼ��������Ա����
// ���������
// ���������
// ����ֵ  �� ������ʼ������ʾ������ָ��
// =========================================================================
struct menu_displaypara *mallocdisplaypara(void)
{

   struct menu_displaypara  *result;

   result=(struct menu_displaypara *)M_MallocLc(sizeof(struct menu_displaypara),0);
   if(NULL==result)
   {
      printf("not enough mem----mallocdisplaypara\n");
      return result;
   }
/*
   //��ʼ������ָ��
   result->next=NULL;
   result->pre=NULL;

   //��ʼ������ָ��
   result->AppWin=NULL;
   result->CurWin=NULL;


   //��ʼ���˵���ָ��
   result->OperatingMenuitem=NULL;
   result->FVisualMenuitem=NULL;
   result->LVisualMenuitem=NULL;
    //��ʼ����ʾ�ռ�
   result->MenuitemSpace.height=0;
   result->MenuitemSpace.width=0;

   //˽������
   result->myprivate=NULL;
   //������Ա������ʾ���������Ĺ����г�ʼ��
   */
    memset(result, 0, sizeof(struct menu_displaypara));
   //��ʼ����ɫ��ʹ��Ĭ����ɫ
   result->DisplayColor.menu_color=cn_menu_color;
   result->DisplayColor.menu_lighted_color=cn_menu_lighted_color;
   result->DisplayColor.text_color=cn_menu_text_color;

   return result;
}

// =========================================================================
// ��������:�ͷ���ʾ�����ռ䣬�������Ա��ԭ
// �������:���ͷ���ʾ����
// ���������
// ����ֵ  :��
// =========================================================================

void deldisplaypara(struct menu_displaypara *menudispara)
{

    struct menu_displaypara  *temp1;//����Ҫɾ������ʾ����
    struct menu_displaypara  *temp2;//ɾ���Ķ���

    temp1=menudispara;

    if( NULL==menudispara)
    {
        printf("This is an NULL menudispara,no need to del!----nDelDisplaypara\n");
        return ;
    }
    while(NULL!=temp1->next)//�ҵ����һ�����Ӻ���ǰɾ��
    {
         temp1=temp1->next;
    }
    while( menudispara!=temp1)//ɾ�����к���Ľڵ�
    {
        if(NULL!=temp1->CurWin)//���ٴ���
        {
           GK_ApiDestroyWin(temp1->CurWin);//�ú���������ִ�к�������������
           free(temp1->CurWin);//ɾ��������ռ�õ��ڴ�
        }

        temp2=temp1;
        temp1=temp1->pre;
        free(temp2);
    }
    //ɾ����ǰ�ڵ�
    if(NULL!=temp1->CurWin)//
    {
        GK_ApiDestroyWin(temp1->CurWin);
        //����ɾ����ǰ���ڣ��������ܵ�,��ΪҪ�����ͷ��ڴ棬��Ȼ�Ļ����ͷ��ڴ������
        //��Ȼ������ڴ�����
        free(temp1->CurWin);//�ͷŴ���ռ�õ��ڴ�
        temp1->CurWin=NULL;
    }
    free(temp1);
    return;
}
// =========================================================================
// ��������:����ʾ�������뵽������
// �������:head:�������   newpara:���������ʾ����
// ���������
// ����ֵ  :��
// =========================================================================
void adddisplaypara(struct menu_displaypara * head,\
        struct menu_displaypara * newpara)
{
    if((NULL==head)||(NULL==newpara))
    {
     printf("You have supplied an null para--adddisplaypara!\n");
     return ;
    }
    while(NULL!=head->next)
        {
      head=head->next;
    }
    head->next=newpara;
    newpara->pre=head;
    //�û��п�������ӵ�һ����������ע�͵�һ����,���Բ��ù�newpara->next�����
}
// =========================================================================
// ��������:����ʾ�������������Ƴ�
// �������:��Ҫ�Ƴ�����ʾ����:operatingDispara
// ���������
// ����ֵ    :��
//  ˵��      :��Ȼ�Ӹ��������Ƴ��˴���ʾ���������ǲ�û��ɾ������ʾ����
// =========================================================================
void removedisplaypara(struct menu_displaypara * operatingDispara)
{
    if(NULL==operatingDispara)
    {
     printf("Invalid parameter--removedisplaypara!\n");
     return ;
    }
    if((NULL==operatingDispara->pre)&&(NULL==operatingDispara->next))//ֻ��һ��
    {
        //return;
    }
    else if (NULL==operatingDispara->pre)
    {
      operatingDispara->next->pre=NULL;
    }
    else if(NULL==operatingDispara->next)
    {
        operatingDispara->pre->next=NULL;
    }
    else
    {
        operatingDispara->pre->next=operatingDispara->next;
        operatingDispara->next->pre=operatingDispara->pre;
    }
    return;
}

// =========================================================================
// ��������:�����ṩ���ַ�������������ַ�����λͼ�Ŀ�͸�
// �������:text�������Ե��ַ�����count:�ַ����ĳ��ȣ�������������
// ���������
// ����ֵ  :��
// =========================================================================
struct menu_item_space cal_textarea(const char *text, u32 count )
{


    s32 len, src_len,size_bak;
    u32 wc;
    struct tagFontRsc* cur_font;
    struct tagGkscParaDrawBitmapRop bitmap_para;
    struct menu_item_space   result={0,0};

    cur_font = Font_GetCurFont();
    if(cur_font == NULL)
        return result;
    src_len = count;

    result.height=0;
    result.width=0;

    bitmap_para.bitmap.bm_bits = NULL;
    size_bak = 0;

    for(; src_len > 0;)
    {
        len= mbtowc(&wc, text, src_len);
        if(len == -1)
        { // ��Ч�ַ�
            src_len--;
            text++;
        }else
        { // ��Ч�ַ�
            text += len;
            src_len -= len;

            // ȡ����
            bitmap_para.bitmap.bm_bits = NULL;      //NULL=��ѯbitmap.
            cur_font->GetBitmap(wc,0,0,&(bitmap_para.bitmap));

            result.width+=bitmap_para.bitmap.width;//�����ۼ�

            if(bitmap_para.bitmap.height>result.height)//�߶�ȡ��ߵ�һ��
            {
                result.height=bitmap_para.bitmap.height;
            }

        }
    }

  return result;
}


// =========================================================================
// ��������:�����ṩ�Ĳ˵����ͬ��˵����ڵ�ǰ�ַ����µ�ռ�õ����ռ�
// �������:����Ĳ˵���
// ���������
// ����ֵ  :����õ��Ĳ˵����ڸõ�ǰ�ַ����µ�map��С
// ˵��    ����ռ�ָ���ǲ˵������Դ�ڵ����֣���Ȼ������Ӧ�ı�ʶ��
// =========================================================================
struct menu_item_space cal_menuitemarea(struct menu_item *menuitem)
{
    u8 size=0;
    struct menu_item *temp=NULL;
    struct menu_item_space result={0,0};
    struct menu_item_space tempSpace={0,0};

    if(NULL==menuitem)
    {
      printf("Invalid parameter--cal_menuitemarea\n");
      return result;
    }
    temp=menuitem;
    do{
        size=strlen(temp->node.name,namesize);
        tempSpace=cal_textarea(temp->node.name, size);
        //���ǵ����룬��û���Ӳ˵���Ҫ����12�����ڱ�ǩ����ʾ����
        if(NULL!=temp->node.child)
        {
          tempSpace.width+=12;
        }
        else
        {
          tempSpace.width+=12;
        }
        if(tempSpace.width>result.width)
        {
          result.width=tempSpace.width;
        }
        if(tempSpace.height>result.height)
        {
          result.height=tempSpace.height;
        }
        temp=GetNextMenuitem(temp);

   }while(temp!=menuitem);
  return result;
}
// =========================================================================
// ��������:��ȡ���˵���ʾ����
// �������:��ǰ��ʾ����
// ������� ��
// ����ֵ      :���˵���ʾ����
// =========================================================================
struct menu_displaypara*  getmainmenudispara(struct menu_displaypara *cur)
{
    //�������
   if(cur==NULL)
   {
       printf("Invalid parameter---getmainmenudispara!\n");
       return NULL;
   }
   else
   {
       while(cur->pre!=NULL)
       {
           cur=cur->pre;
       }
       return cur;
   }
}

// =========================================================================
// ��������:Ϊ������ʾģʽ������ʾ����
// �������:cur��ǰ��ʾ������win_rec�����������ʾ������win_name����������
// ���������
// ����ֵ  :
//��Ҫ˵������:λ��ǰ�涼������ˣ��Ѿ�û��ɶ�Ӻ�˵����
// =========================================================================
bool_t create_menudisparawin(struct menu_displaypara  *menudispara,struct tagRectangle win_rec,const char *win_name)
{

  bool_t result=false;

  struct tagGkWinRsc  *parwin;

  //�������
  if(NULL==menudispara)
  {
    printf("Invalid parameter --- creatdisparawin!\n");
     return result;
  }
  if(NULL==menudispara->pre)//���˵�
  {
    if(NULL==menudispara->AppWin)
    {
      printf("Invalid parameter --- creatdisparawin!\n");
      return result;
    }
    else
    {
     parwin=menudispara->AppWin;
     win_rec.top=menudispara->AppWin->absy0-parwin->absy0;
     win_rec.left=menudispara->AppWin->absx0-parwin->absx0;
     win_rec.right=win_rec.left+menudispara->AppWin->right-menudispara->AppWin->left;
     win_rec.bottom=win_rec.top+menudispara->AppWin->bottom-menudispara->AppWin->top;
    }
  }
  else//�Ӳ˵�
  {
    parwin=menudispara->pre->CurWin;
    if((0==win_rec.bottom)&&(0==win_rec.right)&&(0==win_rec.top)&&(0==win_rec.bottom))//�½�ȫ������
    {
      win_rec.top=menudispara->AppWin->absy0-parwin->absy0;
      win_rec.left=menudispara->AppWin->absx0-parwin->absx0;
      win_rec.right=win_rec.left+menudispara->AppWin->right-menudispara->AppWin->left;
      win_rec.bottom=win_rec.top+menudispara->AppWin->bottom-menudispara->AppWin->top;
    }
    else
    {
      //�Ӳ˵�������С
    }
  }
     //���䴰�ڿռ䣬������䲻����һ�ж��Ǹ���
  menudispara->CurWin=(struct tagGkWinRsc *)M_MallocLc(sizeof(struct tagGkWinRsc),0);

  if(NULL == menudispara->CurWin)
  {
    printf("no enough mem for the displaypara win!\n");
    return result;
  }
  result=GK_ApiCreateGkwin(parwin,menudispara->CurWin,\
                             win_rec.left, win_rec.top,win_rec.right,win_rec.bottom,\
                             menudispara->DisplayColor.menu_color, CN_WINBUF_NONE,\
                             win_name, 0,0,0,0);

  if(!result)//����ʧ�ܣ��ͷ���ռ�õ��ڴ�
  {
    printf("Failed--- create_menudisparawin!\n");
    free(menudispara->CurWin);
    menudispara->CurWin=NULL;
    return result;
  }
  else
  {
    GK_ApiSetBoundMode(menudispara->CurWin, CN_BOUND_UNLIMIT);
    menudispara->CurWin->user_private=menudispara;
    return result;
  }
}












