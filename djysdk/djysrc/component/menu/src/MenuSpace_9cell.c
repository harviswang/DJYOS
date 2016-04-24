
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
// ģ������: DJY�˵�ģ��֮�Ź�����ʾ�ռ����
// ģ��汾: V1.00(��ʼ���汾)
// ������Ա: zqf
// ����ʱ��: 20121224
// =======================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// =======================================================================

#include "MenuSpace_9cell.h"

#include "MenuitemApi.h"

// =========================================================================
// ��������:Ϊ9������ʾ����������ʾ�ռ�
// �������:pre��ǰ����ʾ������cur��ǰ��ʾ����
// ���������
// ����ֵ  :ͨ������õ��ľŹ������˵������Ӳ˵�����Ŀռ�
// ˵��    :preΪNULL����֤����Ϊ���˵�����ռ�
//          pre->pre==NULL,��֤���Ǵ����˵������Ӳ˵�
//          ��������Ӳ˵������Ӳ˵�
//          ����ʾ������ռ�õĿռ䣬��Ȼ��Ҫ����ǰ���˵�
//          ���ھŹ�����ԣ��Ӳ˵������Ӳ˵��ǴӴ�ֱ�˵�������ֱ�˵�
//                           ���˵������Ӳ˵��ǴӾŸ񵯳���ֱ�˵�
// =========================================================================
/*
static struct tagRectangle   CalDisAreaFromPre_9cell(struct menu_displaypara *pre,struct menu_displaypara  *cur)
{


   struct tagRectangle  result={0,0,0,0};//����Ľ��
   //�ڲ����ã�����������

   cur->MenuitemSpace=cal_menuitemarea(cur->OperatingMenuitem);
    //����������������������˵������Ӳ˵��������½���һ��ȫҳ�˵���
    return result;
}
*/
struct tagRectangle   CalDisAreaFromPre_9cell(struct menu_displaypara *pre,struct menu_displaypara  *cur)
{

   u32       menuitem_num;//��ǰ�˵��ж��ٸ��˵���

   struct tagRectangle  result={0,0,0,0};//����Ľ��

   u8        number=0;//���ڼ���ڼ������ڵ�����

    s32     limit_end;//������߽��
    s32     limit_start;//�������
    s32     cal_end;//����߽�
    s32     cal_start;//�������

    s32     Midpoint;//�м�λ��

    u32     length=0;


    //�������
    if(NULL==cur)
    {
      printf("Invalid para for cal 9cell space!\n");
      return result;
    }
    if(NULL==cur->FVisualMenuitem)
    {
      printf("Invalid para for cal 9cell space!\n");
      return result;
    }

    cur->MenuitemSpace=cal_menuitemarea(cur->FVisualMenuitem);
    if(NULL==pre)//���˵�
    {
      return  result;//�½�ҳ
    }
    else if(NULL==pre->pre)//���˵������Ӳ˵�
    {
      return result;//�½�ҳ
    }
    else//���Ҫ���Ƕ���������Ƚϸ���
    {

       menuitem_num=CalMenuitemNum(cur->FVisualMenuitem);
       number=CalDisBettween2Menuitem(pre->FVisualMenuitem, pre->OperatingMenuitem);
      //����ˮƽλ��
      limit_start=pre->MenuitemSpace.width+cn_space_margin;
      limit_end=cur->AppWin->absx0+cur->AppWin->right-cur->AppWin->left-pre->CurWin->absx0;
      cal_start=limit_start;//�ھŹ���������������ͬ��
      cal_end=cal_start+cur->MenuitemSpace.width+cn_space_margin;
      if(cal_end>limit_end)//ˮƽλ�ò�������һƱ���
      {
        return result;
      }
      else
      {
         result.left=cal_start;
         result.right=cal_end;
      }
      //���㴹ֱλ�ã��ǵ÷����м�
      length=(cur->MenuitemSpace.height)*menuitem_num+cn_space_margin;
      limit_start=cur->AppWin->absy0-pre->CurWin->absy0;
      limit_end=limit_start+cur->AppWin->bottom-cur->AppWin->top;
      if(length>(limit_end-limit_start))//�ռ䲻��ԣ�����ؿ����м�λ����
      {
         result.top=limit_start;
         result.bottom=limit_end;
         return result;
      }
      else//�и��࣬�����м�λ��
      {
          Midpoint=pre->MenuitemSpace.height*number+pre->MenuitemSpace.height/2;
          if(((Midpoint-limit_start)>=length/2)&&(limit_end-Midpoint)>=length/2)//���߶����㣬���Է����м�
           {
                 result.top=Midpoint-length/2;
                 result.bottom=length+result.top;
           }
          else if((Midpoint-limit_start)<length/2)//�ϱ߲���
          {
               result.top=limit_start;
               result.bottom=length+result.top;
          }
          else//�±߲���
          {
              result.bottom=limit_end;
              result.top=result.bottom-length;
          }
          return  result;
       }
     }
}

// =========================================================================
// ��������:���ݾŸ����ʽ������ʾ��ĵ���
// �������:����������ʾ����dispara
// ���������
// ����ֵ    :
// ˵��         :���޸���ʾ������FVisualMenuitem��LVisualMenuitem���������˵��򲻻��޸ģ�
//          ��������Ҫ������������������ܵ���ʾ����Ĳ˵���
// =========================================================================
void adjustvisualmenuitem_9cell(struct menu_displaypara  *dispara)
{

     u8            dis_num;//���ڿ�����ʾ�Ĳ˵�����Ŀ
     u8           OperaNumber=0;
     u8           FVisualNumber=0;
     u8           LVisualNumber=0;

     s32           dis_height;//������Ŀ��
     s32           deep_limit;
     s32           deep_cal;

    struct menu_item   *BBmenuitem=NULL,*LBmenuitem=NULL,*tempmenuitem=NULL;
    //�������
    if((NULL==dispara)||(NULL==dispara->OperatingMenuitem))
    {
        printf("Invalid parameter----adjustvisualmenuitem_9cell\n");
        return;
    }
    if(NULL==dispara->pre)
    {
    //  printf("No need to adjsutVisualMenuitem for main menuitem!\n");
      dis_num=9;
    }
    else
    {
       deep_cal=dispara->CurWin->absy0+dispara->CurWin->bottom-dispara->CurWin->top;
       deep_limit=dispara->AppWin->absy0+dispara->AppWin->bottom-dispara->AppWin->top;
       if(deep_cal>deep_limit)//�г����Ĳ���
       {
        dis_height=deep_limit-dispara->CurWin->absy0;
       }
       else
       {
        dis_height=deep_cal-dispara->CurWin->absy0;
       }
       if(dis_height==0)
       {
            printf("Menu space is NULL, so do nothing!\n");
            return;
       }
       dis_num=dis_height/(dispara->MenuitemSpace.height);
    }

    BBmenuitem=GetBBmenuitem(dispara->OperatingMenuitem);
    LBmenuitem=GetLBmenuitem(dispara->OperatingMenuitem);
    tempmenuitem=dispara->OperatingMenuitem;
    if(dis_num>=CalMenuitemNum(dispara->OperatingMenuitem))//�ܹ�����ȫ��;
    {
         dispara->FVisualMenuitem=BBmenuitem;
         dispara->LVisualMenuitem=LBmenuitem;
    }
    else//���ɲ���
    {
        //���Ƿ�ҳ�����
        FVisualNumber=CalMenuitemNumber(dispara->FVisualMenuitem);
        LVisualNumber=CalMenuitemNumber(dispara->LVisualMenuitem);
        OperaNumber=CalMenuitemNumber(dispara->OperatingMenuitem);
        if(OperaNumber<FVisualNumber)//��ǰ�������Ѿ�ת����һ��ʾ��ǰ�棬��ǰ��һҳ
        {
          if(dispara->OperatingMenuitem==BBmenuitem)//����δλ������ǰ�棬��Ϊ��ѭ����
          {
            tempmenuitem=dispara->OperatingMenuitem;
            while(dis_num>1) //��LastVisualָ����
             {
               if(tempmenuitem==LBmenuitem)
                {
                   break;
                }
               tempmenuitem=GetNextMenuitem(tempmenuitem);
               dis_num--;
              }
             dispara->LVisualMenuitem=tempmenuitem;
             dispara->FVisualMenuitem=dispara->OperatingMenuitem;
          }
          else//˳�򵽴��
          {
            tempmenuitem=dispara->OperatingMenuitem;
            while(dis_num>1) //��SlayerFristָ��ǰ��
             {
               if(tempmenuitem==BBmenuitem)
                {
                   break;
                }
               tempmenuitem=GetPreMenuitem(tempmenuitem);
               dis_num--;
              }
             dispara->FVisualMenuitem=tempmenuitem;
             dispara->LVisualMenuitem=dispara->OperatingMenuitem;
           }
        }
        else if(OperaNumber>LVisualNumber)//��ǰ�������Ѿ�ת�������ʾ����棬���һҳ
        {
          if(dispara->OperatingMenuitem==LBmenuitem)//����ǰλ��������棬��Ϊ��ѭ����
          {
            tempmenuitem=dispara->OperatingMenuitem;
            while(dis_num>1) //��FirstVisualָ����
             {
               if(tempmenuitem==BBmenuitem)
                {
                   break;
                }
               tempmenuitem=GetPreMenuitem(tempmenuitem);
               dis_num--;
              }
             dispara->FVisualMenuitem=tempmenuitem;
             dispara->LVisualMenuitem=dispara->OperatingMenuitem;
          }
          else//˳�򵽴��
          {
            tempmenuitem=dispara->OperatingMenuitem;
            while(dis_num>1) //��SlayerFristָ��ǰ��
             {
               if(tempmenuitem==LBmenuitem)
                {
                   break;
                }
               tempmenuitem=GetNextMenuitem(tempmenuitem);
               dis_num--;
              }
             dispara->LVisualMenuitem=tempmenuitem;
             dispara->FVisualMenuitem=dispara->OperatingMenuitem;
           }
        }
        else//û�г�����ʾҳ�������ܵ���ʾ��
        {
          if(dis_num<=CalDisBettween2Menuitem(dispara->FVisualMenuitem, dispara->LVisualMenuitem))//�����ʼ�����ʱ������
          {
             tempmenuitem=dispara->OperatingMenuitem;
             while(dis_num>1) //��SlayerFristָ��ǰ��
             {
               if(tempmenuitem==BBmenuitem)
               {
                  break;
               }
               tempmenuitem=GetPreMenuitem(tempmenuitem);
               dis_num--;
             }
             dispara->FVisualMenuitem=tempmenuitem;
             tempmenuitem=dispara->OperatingMenuitem;
             while(dis_num>1) //��SlayerLastָ����
             {
               if(tempmenuitem==LBmenuitem)
               {
                 break;
               }
              tempmenuitem=GetNextMenuitem(tempmenuitem);
               dis_num--;
             }
             dispara->LVisualMenuitem=tempmenuitem;
          }
          else
          {
           //����ı���ӵĵ�һ������һ��
          }

        }

     }
    //���е��Ե���Ϣ���������׶Է�
  //  printf("BB_menuitem=%s\n",BBmenuitem->node.name);
 // printf("Fv_menuitem=%s\n",dispara->FVisualMenuitem->node.name);
//  printf("Op_menuitem=%s\n",dispara->OperatingMenuitem->node.name);
//  printf("Lv_menuitem=%s\n",dispara->LVisualMenuitem->node.name);
//  printf("LB_menuitem=%s\n\n",LBmenuitem->node.name);
    return;
}

// =========================================================================
// ��������:Ϊ�Ÿ���ʾ����������ʾ����
// �������:pre��ǰ����ʾ������menuitem����ǰ��ʾ�����������Ĳ˵��Appwin,Ӧ�ó����ṩ����ʾ����
//          ע�⵱preΪ�յ�ʱ���ʾ���Ǵ�������ʾ��������ʱ����õ�APPWIN��menuitem,������������
//          pre����ȡ
// ���������
// ����ֵ  :
//��Ҫ˵������:λ��ǰ�涼������ˣ��Ѿ�û��ɶ�Ӻ�˵����
// =========================================================================
struct menu_displaypara *createmenudisparabypre_9cell(struct menu_displaypara *pre,struct menu_item *menuitem,struct tagGkWinRsc  *Appwin)
{

  bool_t  result;
  struct menu_displaypara  *cur=NULL;
  struct tagRectangle   Disparawin_rec;
 // struct tagGkWinRsc  *win;
   //�򵥵ĸ���ǰһ���˵����ڴ���

  if((NULL==pre)&&((NULL==menuitem)||(NULL==Appwin)))
  {
     printf("Not enough  para to create the menudispara,You must be joking!\n");
  }
  cur=mallocdisplaypara();
  if(NULL==cur)
  {
     printf("Not enough mem for CreateParaMenuwinBypre!\n");
     return cur;
  }

  if(NULL==pre)
  {
     // printf("This is the main menu!\n");

      cur->AppWin=Appwin;
      cur->FVisualMenuitem=GetBBmenuitem(menuitem);
      cur->OperatingMenuitem=menuitem;
      cur->LVisualMenuitem=GetPreMenuitem(cur->FVisualMenuitem);

      cur->MyMenuDispalyFunction=NULL;
      cur->MyPeripheralGeneralAction=NULL;
      cur->MyDisplayPeripheralRespond=NULL;
      cur->AdjustVisualMenuitem=NULL;
      cur->CreateMenuDisparaBypre=NULL;
      cur->MyMenuInitFunction=NULL;
  }
  else
  {
      // printf("This is the sub menu!\n");
       cur->AppWin=pre->AppWin;
       cur->FVisualMenuitem=GetSonMenuitem(pre->OperatingMenuitem);
       cur->OperatingMenuitem=cur->FVisualMenuitem;
       cur->LVisualMenuitem=GetPreMenuitem(cur->FVisualMenuitem);
       //��ʼ��һЩ����ָ��
       cur->menukeyboard_action = pre->menukeyboard_action;
       cur->menumousetouch_ction = pre->menumousetouch_ction;
       cur->MyPeripheralGeneralAction = pre->MyPeripheralGeneralAction;

       cur->MyMenuDispalyFunction=pre->MyMenuDispalyFunction;
       cur->MyPeripheralGeneralAction=pre->MyPeripheralGeneralAction;
       cur->MyDisplayPeripheralRespond=pre->MyDisplayPeripheralRespond;
       cur->AdjustVisualMenuitem=pre->AdjustVisualMenuitem;
       cur->CreateMenuDisparaBypre=pre->CreateMenuDisparaBypre;
       cur->MyMenuInitFunction=pre->MyMenuInitFunction;
       //��ӵ�������
       adddisplaypara(pre,cur);
    }
  //����ռ�
   Disparawin_rec=CalDisAreaFromPre_9cell(pre,cur);
 //��������
   result=create_menudisparawin(cur, Disparawin_rec, cur->FVisualMenuitem->node.name);
   if(!result)
   {
      printf("Failed--- createmenudisparabypre_9cell!\n");
      deldisplaypara(cur);
      cur=NULL;
      return cur;
   }
   return cur;
}





