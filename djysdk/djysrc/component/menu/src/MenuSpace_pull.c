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
// MenuSpace_pull.c
// ģ������: DJY�˵�ģ��֮����ģʽ�ռ���㷽��
// ģ��汾:
// ����ʱ��: 2012-12-27
// ����          : Administrator
// =======================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// =======================================================================


#include "MenuSpace_pull.h"

#include "MenuitemApi.h"

// =========================================================================
// ��������:Ϊ����ģʽ�ּ�����ʾ�ռ�
// �������:pre��ǰ����ʾ������cur��ǰ��ʾ����
// ���������
// ����ֵ     :ͨ������õ�������ģʽ���˵������Ӳ˵�����Ŀռ�
// ˵��          :preΪNULL����֤����Ϊ���˵�����ռ䣬������Ϊ�Ӳ˵�����ռ�
//          ����ʾ������ռ�õĿռ䣬��Ȼ��Ҫ����ǰ���˵�
//          ����������ʾģʽ���ԣ�ֻ�����˵��Ǻ�������Ķ���������ʾ
// =========================================================================
//�������ܵ�����������޸�
//�������˵�����Ȼ�����������������Ĵ���
//�����Լ����˵��������ڵ�ǰ������������棨���ȣ������λ�ò�����Ҫ�����ƶ���
//���ڶ����Ӳ˵������Ժ���Ӳ˵��������Ƚ����ڵ�ǰ�˵����ұߣ�����ұ�λ�ò�������������ߣ�
struct tagRectangle   CalDisAreaFromPre_pull(struct menu_displaypara *pre,struct menu_displaypara  *cur)
{
   u8        number=0;//���ڼ���ڼ������ڵ�����
   u32                     menuitem_num;//��ǰ�˵��ж��ٸ��˵���
   s32     limit_end;//������߽��
   s32     limit_start;//�������
   s32     cal_end;//����߽�
   s32     cal_start;//�������
   s32     Midpoint;//�м�λ��
   u32     length=0;
   s32    length_left=0;//�洢���Ի��ֵ���ʾ����
   s32    length_need=0;//�洢��Ҫ����ʾ����
   struct tagRectangle  result={0,0,0,0};//����Ľ��
   struct menu_displaypara  *pMainMenuDispara;//��ʱҳ�ڵ�

    //ֻ���ڲ����ã����ٽ��в�����⣬�����Ч��
    cur->MenuitemSpace=cal_menuitemarea(cur->FVisualMenuitem);
    if(NULL==pre)//���˵�
    {
      return  result;//�½�һ�����������Ĵ���
    }
    else//���Ҫ���Ƕ���������Ƚϸ���
    {
       menuitem_num=CalMenuitemNum(cur->FVisualMenuitem);
       number=CalDisBettween2Menuitem(pre->FVisualMenuitem, pre->OperatingMenuitem);
      //����ˮƽλ��
       if(NULL==pre->pre)//ǰ����ˮƽ�������˵�
       {
          limit_start=(pre->MenuitemSpace.width)*number+cn_space_margin;
          limit_end=pre->CurWin->right-pre->CurWin->left;
          cal_start=limit_start;
          cal_end=cal_start+cur->MenuitemSpace.width+cn_space_margin;
          if(cal_end>limit_end)//ˮƽλ�ò����������ܷ���е���
          {
            //�ұ߲������������ƶ���ʵ�ڲ����Ǿ�û�а취��
            if((limit_end)>(cal_end-cal_start))//��ǰ�㻹����������
            {
                result.left=limit_end-cur->MenuitemSpace.width-cn_space_margin;
                result.right=limit_end;
            }
            else//���ɲ��£�һ����ǲ����ܵģ�������Ļ�㹻խ�����ߵ�ǰ�˵���̫��
            {
               result.left=0;
               result.right=limit_end;
            }
          }
          else//ˮƽλ�ù���
          {
             result.left=cal_start;
             result.right=cal_end;
          }
      }
      else//�����Ӳ˵���������,�ȿ��ұ߹����������õĻ����ұߣ������Ļ�����ߣ�
      {
          length_need=cur->MenuitemSpace.width+cn_space_margin;
          length_left=pre->AppWin->absx0+pre->AppWin->right-pre->AppWin->left-pre->CurWin->right-pre->CurWin->absx0+pre->CurWin->left;
          if(length_need>length_left)//�ұ߲�����
          {
            //������߹�����
            length_left=pre->CurWin->absx0-pre->AppWin->absx0;
            length_need=cur->MenuitemSpace.width+cn_space_margin;
            if(length_need>length_left)//���Ҳ����
            {
              //��������߶࣬�����ұ߶࣬�ı߶�ͷ��ı�
              length_need=pre->AppWin->absx0+pre->AppWin->right-pre->AppWin->left-pre->CurWin->right-pre->CurWin->absx0+pre->CurWin->left;
              if(length_left>length_need)//��߶�
              {
                 result.left=-(length_left);
                 result.right=0;
              }
              else//�ұ߶�
              {
                 result.left=pre->CurWin->right-pre->CurWin->left;
                 result.right=result.left+length_need;
              }

            }
            else//��߹�
            {
                result.right=0;
                result.left=result.right-length_need;
            }
          }
          else//�ұ߹�
          {
             result.left=pre->CurWin->right-pre->CurWin->left;
             result.right=result.left+length_need;
          }
      }
      //�������ܵ������������һҳ�ġ�top��bottom�����˵�ˮƽ��ʾ������
      pMainMenuDispara=getmainmenudispara(pre);//��ȡ���˵�
      if(NULL==pMainMenuDispara)
      {
          printf("fata_err_in cal pull space!");
          return result;
      }

      limit_start=pre->AppWin->absy0+pMainMenuDispara->MenuitemSpace.height+cn_space_margin-pre->CurWin->absy0;
      length_need=(cur->MenuitemSpace.height)*menuitem_num+cn_space_margin;
      limit_end=cur->AppWin->absy0+cur->AppWin->bottom-cur->AppWin->top-pre->CurWin->absy0;
      if(NULL==pre->pre)//ֱ�������˵��£�ˮƽ������ֱ�˵�
      {
          if(length_need>(limit_end-limit_start))//���ǿռ��С,�ռ䲻�㣬���Ƿ�����������
          {
            result.top=limit_start;
            result.bottom=result.top+((limit_end-limit_start)/(cur->MenuitemSpace.height))*(cur->MenuitemSpace.height)+cn_space_margin;
            return result;
          }
          else
          {
             result.top=limit_start;
             result.bottom=result.top+length_need;
             return result;
          }
      }
      else//��ֱ������ֱ�˵�
      {
          if(length_need>(limit_end-limit_start))//�ռ䲻��ԣ�����ؿ����м�λ����
          {
             result.top=limit_start;
             result.bottom=limit_end;
             return result;
          }
          else//�и��࣬�����м�λ��
          {
             Midpoint=pre->MenuitemSpace.height*number+pre->MenuitemSpace.height/2;
             if(((Midpoint-limit_start)>=length_need/2)&&(limit_end-Midpoint)>=length_need/2)//���߶����㣬���Է����м�
             {
                 result.top=Midpoint-length_need/2;
                 result.bottom=length_need+result.top;
             }
             else if((Midpoint-limit_start)<length_need/2)//�ϱ߲���
             {
                 result.top=limit_start;
                 result.bottom=length_need+result.top;
             }
             else//�±߲���
             {
                  result.bottom=limit_end;
                  result.top=result.bottom-length_need;
             }
             return  result;
          }
      }
    }
}
// =========================================================================
// ��������:��������ģ������ʾ��ĵ���
// �������:����������ʾ����dispara
// ���������
// ����ֵ :
// ˵��   :���޸���ʾ������FVisualMenuitem��LVisualMenuitem���������˵��򲻻��޸ģ�
//          ��������Ҫ������������������ܵ���ʾ����Ĳ˵���
// =========================================================================
void adjustvisualmenuitem_pull(struct menu_displaypara  *dispara)
{

     u8            dis_num;//���ڿ�����ʾ�Ĳ˵�����Ŀ
     u8           OperaNumber=0;
     u8           FVisualNumber=0;
     u8           LVisualNumber=0;
     s32           length_Visual;//������Ŀ��
     s32           length_E;//ÿ���˵���ռ�õĳ��ȣ�ˮƽָ����width����ֱָ����height
     struct menu_item   *BBmenuitem=NULL,*LBmenuitem=NULL,*tempmenuitem=NULL;

    //�������
    if((NULL==dispara)||(NULL==dispara->OperatingMenuitem))
    {
        printf("Invalid para for Adjust!\n");
        return;
    }
    if(NULL==dispara->pre)//���˵�������
    {
        length_Visual=dispara->CurWin->right-dispara->CurWin->left;
    //  deep_limit=dispara->AppWin->absx0+dispara->AppWin->right-dispara->AppWin->left;
        length_E=dispara->MenuitemSpace.width;
    }
    else//�Ӳ˵�������
    {
        length_Visual=dispara->CurWin->bottom-dispara->CurWin->top;
    //    deep_limit=dispara->AppWin->absy0+dispara->AppWin->bottom-dispara->AppWin->top;
        length_E=dispara->MenuitemSpace.height;
    }
    if(length_E==0)
    {
        printf("Menu space is NULL, so do nothing!\n");
        return;
    }
     dis_num=length_Visual/(length_E);

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
    //���е��Ե���Ϣ��������ʾУ�ԣ����Ƿ���ȷ��
  //  printf("BB_menuitem=%s\n",BBmenuitem->node.name);
//  printf("Fv_menuitem=%s\n",dispara->FVisualMenuitem->node.name);
//  printf("Op_menuitem=%s\n",dispara->OperatingMenuitem->node.name);
//  printf("Lv_menuitem=%s\n",dispara->LVisualMenuitem->node.name);
//  printf("LB_menuitem=%s\n\n",LBmenuitem->node.name);
    return;
}

// =========================================================================
// ��������:Ϊ������ʾģʽ������ʾ����
// �������:pre��ǰ����ʾ������menuitem����ǰ��ʾ�����������Ĳ˵��Appwin,Ӧ�ó����ṩ����ʾ����
//          ע�⵱preΪ�յ�ʱ���ʾ���Ǵ�������ʾ��������ʱ����õ�APPWIN��menuitem,������������
//          pre����ȡ
// ���������
// ����ֵ  :
//��Ҫ˵������:λ��ǰ�涼������ˣ��Ѿ�û��ɶ�Ӻ�˵����
// =========================================================================
struct menu_displaypara *createmenudisparabypre_pull(struct menu_displaypara *pre,struct menu_item *menuitem,struct tagGkWinRsc  *Appwin)
{

  bool_t  result;
  struct menu_displaypara  *cur=NULL;
  struct tagRectangle   Disparawin_rec;
 // struct tagGkWinRsc  *win;
   //�򵥵ĸ���ǰһ���˵����ڴ���

  if((NULL==pre)&&((NULL==menuitem)||(NULL==Appwin)))
  {
     printf("Invalid parameter----createmenudisparabypre_pull\n");
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
   if((NULL==cur->OperatingMenuitem)||(NULL==cur->AppWin))
   {
      deldisplaypara(cur);
      return cur;
   }
  //����ռ�
   Disparawin_rec=CalDisAreaFromPre_pull(pre,cur);
 //��������
   result=create_menudisparawin(cur, Disparawin_rec, cur->OperatingMenuitem->node.parent->name);

   if(!result)
   {
      printf("Failed---createmenudisparabypre_pull!\n");
      deldisplaypara(cur);
      cur=NULL;
      return cur;
   }

   return cur;
}

