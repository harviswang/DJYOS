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
// MenuPeripheralGeneral.c
// ģ������:
// ģ��汾: ��������ͨ�÷���
// ����ʱ��: 2013-1-16
// ����    : Administrator
// =======================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// =======================================================================

#include "MenuPeripheralGeneral.h"
#include "MenuDisplayGeneral.h"
#include "MenuSpaceGeneral.h"

#include "MenuitemApi.h"
// ==============================================================================================
// ��������:�ж�һ�����Ƿ��ڹ涨��������
// �������:rec���涨������myCoordinatePoint,�����Ե������
// ���������true�����ھ��ο��ڣ�false���ھ��ο���
// ����ֵ  :
// ˵��    :
// ==============================================================================================

bool_t JudgeCoordinatePointInRec(struct tagRectangle rec, struct coordinate_point myCoordinatePoint)
{
    if((rec.left<=myCoordinatePoint.x)&&(rec.right>=myCoordinatePoint.x)&&\
       (rec.top<=myCoordinatePoint.y)&&(rec.bottom>=myCoordinatePoint.y))
    {
        return true;
    }
    else
    {
        return false;
    }
}

// ==============================================================================================
// ��������:���ˮƽ��ʾ����ʾ�������д����ķ�Ӧ
// �������:operatingDispara����ǰ���ڲ�������ʾ������myCoordinatePoint�������ߴ���������
// ���������
// ����ֵ  :�������µĵ�ǰ���ڲ����Ĳ˵���
// ˵��    :�л�����Ӧ������Ĳ˵��(Ŀǰֻ֧���ڲ˵�֮����л����������˵����չ���ͷ���)
// ==============================================================================================
struct menu_displaypara *touchlcdactionhorizontal(struct menu_displaypara *operatingDispara,\
                                                  struct coordinate_point myCoordinatePoint)
{

    u8              menuNum=0,menuitemNumber=0;
    s8              step=0;//��ת�Ĳ���
    u8              FNumber=0;
    u8              ONumber=0;
    struct tagRectangle  temp_rec={0,0,0,0};
    //�ڲ�ʹ�ã��������������


    temp_rec.left=operatingDispara->CurWin->absx0;
    temp_rec.right=operatingDispara->CurWin->absx0+operatingDispara->CurWin->right-operatingDispara->CurWin->left;
    temp_rec.top=operatingDispara->CurWin->absy0;
    temp_rec.bottom=operatingDispara->CurWin->absy0+operatingDispara->CurWin->bottom-operatingDispara->CurWin->top;
    if(JudgeCoordinatePointInRec(temp_rec,myCoordinatePoint))//�ȿ��Ƿ��ڴ����ڣ����ǵĻ�ʲôҲ����
    {
       menuNum=CalDisBettween2Menuitem(operatingDispara->FVisualMenuitem,operatingDispara->LVisualMenuitem)+1;
       temp_rec.left+=cn_space_margin/4;
       temp_rec.top+=cn_space_margin/4;
       temp_rec.right=temp_rec.left+menuNum*(operatingDispara->MenuitemSpace.width);
       temp_rec.bottom=temp_rec.top+operatingDispara->MenuitemSpace.height;
       if(JudgeCoordinatePointInRec(temp_rec,myCoordinatePoint))//���Ƿ��ڴ��ڵĿ��������ڣ�����ʲô��������
       {
           //�������ڵڼ����˵���ӵ�һ�����Ӳ˵�������
           if(operatingDispara->MenuitemSpace.width!=0)
           {
               //֮����-1������Ϊ��������Ǿ����һ�����Ӳ˵����λ��
               menuitemNumber=((myCoordinatePoint.x-temp_rec.left)-1)/(operatingDispara->MenuitemSpace.width);
               //
               FNumber=CalMenuitemNumber(operatingDispara->FVisualMenuitem);
               ONumber=CalMenuitemNumber(operatingDispara->OperatingMenuitem);
               step=FNumber+menuitemNumber-ONumber;
           //    printf("Fnumber=%d,Onumber=%d,menuitemnumber=%d step=%d\n",FNumber,ONumber,menuitemNumber,step);
               goto_menuitem_inslayers(operatingDispara,step);
           }
           else
           {
//             return operatingDispara;
           }

       }
       else
       {
//         return operatingDispara;
       }
    }
    else
    {
//      return operatingDispara;
    }
    return operatingDispara;
}
// ==============================================================================================
// ��������:��Դ�ֱ��ʾ����ʾ�������д����ķ�Ӧ
// �������:operatingDispara����ǰ���ڲ�������ʾ������myCoordinatePoint�������ߴ���������
// ���������
// ����ֵ  :�������µĵ�ǰ���ڲ����Ĳ˵���
// ˵��    :�л�����Ӧ������Ĳ˵��(Ŀǰֻ֧���ڲ˵�֮����л����������˵����չ���ͷ���)
// ==============================================================================================
struct menu_displaypara *touchlcdactionvertical(struct menu_displaypara *operatingDispara,\
                                                struct coordinate_point myCoordinatePoint)
{
    struct tagRectangle  temp_rec={0,0,0,0};
    u8              menuNum=0,menuitemNumber=0;
    s8              step=0;//��ת�Ĳ���
    u8              FNumber=0;
    u8              ONumber=0;

   //�ڲ�ʹ�ã��������������
    temp_rec.left=operatingDispara->CurWin->absx0;
    temp_rec.right=operatingDispara->CurWin->absx0+operatingDispara->CurWin->right-operatingDispara->CurWin->left;
    temp_rec.top=operatingDispara->CurWin->absy0;
    temp_rec.bottom=operatingDispara->CurWin->absy0+operatingDispara->CurWin->bottom-operatingDispara->CurWin->top;
    if(JudgeCoordinatePointInRec(temp_rec,myCoordinatePoint))//�ȿ��Ƿ��ڴ����ڣ����ǵĻ�ʲôҲ����
    {
       menuNum=CalDisBettween2Menuitem(operatingDispara->FVisualMenuitem,operatingDispara->LVisualMenuitem)+1;
       temp_rec.left+=cn_space_margin/4;
       temp_rec.top+=cn_space_margin/4;
       temp_rec.right=temp_rec.left+operatingDispara->MenuitemSpace.width;
       temp_rec.bottom=temp_rec.top+menuNum*(operatingDispara->MenuitemSpace.height);
       if(JudgeCoordinatePointInRec(temp_rec,myCoordinatePoint))//���Ƿ��ڴ��ڵĿ��������ڣ�����ʲô��������
       {
           //�������ڵڼ����˵���ӵ�һ�����Ӳ˵�������
           if(operatingDispara->MenuitemSpace.height!=0)
           {
               //֮����-1������Ϊ��������Ǿ����һ�����Ӳ˵����λ��
               menuitemNumber=((myCoordinatePoint.y-temp_rec.top)-1)/(operatingDispara->MenuitemSpace.height);
               //
               FNumber=CalMenuitemNumber(operatingDispara->FVisualMenuitem);
               ONumber=CalMenuitemNumber(operatingDispara->OperatingMenuitem);
               step=FNumber+menuitemNumber-ONumber;
            //   printf("Fnumber=%d,Onumber=%d,menuitemnumber=%d step=%d--length=%d\n",FNumber,ONumber,\
            //         menuitemNumber,step,(myCoordinatePoint.y-temp_rec.top)-1);
               goto_menuitem_inslayers(operatingDispara,step);
           }
           else
           {
//             return operatingDispara;
           }

       }
       else
       {
//         return operatingDispara;
       }
    }
    else
    {
//      return operatingDispara;
    }
    return operatingDispara;
}

// ==============================================================================================
// ��������:��ԾŸ���ʾ����ʾ����
// �������:operatingDispara����ǰ���ڲ�������ʾ������myCoordinatePoint�������ߴ���������
// ���������
// ����ֵ  :�������µĵ�ǰ���ڲ����Ĳ˵���
// ˵��    :�л�����Ӧ������Ĳ˵��(Ŀǰֻ֧���ڲ˵�֮����л����������˵����չ���ͷ���)
// ==============================================================================================
struct menu_displaypara *touchlcdaction3by3(struct menu_displaypara *operatingDispara,\
                                            struct coordinate_point myCoordinatePoint)
{
    struct tagRectangle  temp_rec={0,0,0,0};
    s8              menuNum=0,menuitemNumber=0;
    s8              step=0;//��ת�Ĳ���
    s8              FNumber=0;
    s8              ONumber=0;
    s8              Line=0;
    s8              Row=0;

   //�ڲ�ʹ�ã��������������
    temp_rec.left=operatingDispara->CurWin->absx0;
    temp_rec.right=operatingDispara->CurWin->absx0+operatingDispara->CurWin->right-operatingDispara->CurWin->left;
    temp_rec.top=operatingDispara->CurWin->absy0;
    temp_rec.bottom=operatingDispara->CurWin->absy0+operatingDispara->CurWin->bottom-operatingDispara->CurWin->top;
    if(JudgeCoordinatePointInRec(temp_rec,myCoordinatePoint))//�ȿ��Ƿ��ڴ����ڣ����ǵĻ�ʲôҲ����
    {
       menuNum=CalDisBettween2Menuitem(operatingDispara->FVisualMenuitem,operatingDispara->LVisualMenuitem)+1;
       temp_rec.left+=(operatingDispara->CurWin->right-operatingDispara->CurWin->left-cn_space_margin)/2-(operatingDispara->MenuitemSpace.width*3)/2;;
       temp_rec.top+=(operatingDispara->CurWin->bottom-operatingDispara->CurWin->top-cn_space_margin)/2-(operatingDispara->MenuitemSpace.height*3)/2;
       temp_rec.right=temp_rec.left+operatingDispara->MenuitemSpace.width*3+cn_space_margin;
       temp_rec.bottom=temp_rec.top+operatingDispara->MenuitemSpace.height*3+cn_space_margin;
       if(JudgeCoordinatePointInRec(temp_rec,myCoordinatePoint))//���Ƿ��ڴ��ڵĿ��������ڣ�����ʲô��������
       {
           //�������ڵڼ����˵���ӵ�һ�����Ӳ˵�������
           if((operatingDispara->MenuitemSpace.height!=0)&&(0!=operatingDispara->MenuitemSpace.width))
           {
               //֮����-1������Ϊ��������Ǿ����һ�����Ӳ˵����λ��
               Line=((myCoordinatePoint.y-temp_rec.top)-cn_space_margin/2)/(operatingDispara->MenuitemSpace.height);
               Row=((myCoordinatePoint.x-temp_rec.left)-cn_space_margin/2)/(operatingDispara->MenuitemSpace.width);
               menuitemNumber=CalDisBettween2Menuitem(operatingDispara->FVisualMenuitem,operatingDispara->LVisualMenuitem);
               if((Line*3+Row)<=menuitemNumber)//�ڿ��Ӳ˵�����
               {
                   FNumber=CalMenuitemNumber(operatingDispara->FVisualMenuitem);
                   ONumber=CalMenuitemNumber(operatingDispara->OperatingMenuitem);
                   step=FNumber+Line*3+Row-ONumber;
               //    printf("Fnumber=%d,Onumber=%d, L=%d R=%d menuitemnumber=%d step=%d\n",FNumber,ONumber,Line,Row,\
               //          menuitemNumber,step);
                   goto_menuitem_inslayers(operatingDispara,step);
               }
           }
           else
           {
//             return operatingDispara;
           }

       }
       else
       {
//         return operatingDispara;
       }
    }
    else
    {
//      return operatingDispara;
    }
    return operatingDispara;
}
// =========================================================================
// ��������:������Ӧ����
// �������:operatingDispara����ǰ���ڲ����Ĳ˵���
// ���������
// ����ֵ  :�������µĵ�ǰ���ڲ����Ĳ˵���ʾ����
// ˵��    :������ڲ˵���ʾ�����Ĳ������޷Ǿ��Ƕ��ڲ˵������ʾ������
// =========================================================================

struct menu_displaypara *peripheralrespond_general(struct menu_displaypara *operatingDispara, void *para)
{
   struct menu_displaypara  *temp;
   u8 cmd=*(u8 *)(para);
   switch (cmd)
   {
      case GOTO_SUBMENU:
            temp=goto_submenu(operatingDispara);
            if(NULL!=temp)
            {
                operatingDispara=temp;
            }
            break;
      case GOTO_PARENT:
            temp=goto_parmenu(operatingDispara);
            if(NULL!=temp)
            {
                operatingDispara=temp;
            }
            break;
      case GOTO_NEXT:
           goto_nextmenuitem(operatingDispara);
           break;
      case GOTO_PRE:
           goto_premenuitem(operatingDispara);
           break;
      case  GOTO_EXE:
            operatingDispara=exe_menuitemfunc(operatingDispara);
            break;
      default:
              break;
   }
   return operatingDispara;
}