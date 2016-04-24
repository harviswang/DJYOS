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
// ģ������: DJY�˵�ģ��֮�˵�������Ķ���
// ģ��汾: V1.00(��ʼ���汾)
// ������Ա: zqf
// ����ʱ��: 20121224
// =============================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// =============================================================================
#include "MenuDesigner.h"
#include "MenuitemApi.h"
// =============================================================================
// ��������:�ж��û��ṩ�Ĳ˵�������Ը�Ƿ�Ϸ������Ϲ���
// �������:tab����������Ը����tab_size����Ը�ĸ���
// ���������
// ����ֵ  :true�����Ϲ���  false�����Ϸ�
// ˵��    :�жϹ���1���󼶵�class�������ǰ����2��������
//                 2��ÿһ����class������Ϊ��
//                 3, ������û������
//                 4����һ���classһ���Ϊ0
// =============================================================================
bool_t  CheckTagMenuitemModelTab(const struct _tagMenuitemModel *tab,u32 tab_size)
{
   bool_t    result=false;
   s8        temp_class=0;
   u32       num=0;

   if((NULL==tab)||(0==tab_size))
   {
      printf("Invalid parameter----CheckTagMenuitemModelTab\n");
      return result;
   }
   temp_class=tab[0].byClass;
   for(num=0; num< tab_size; num ++)
    {
         if(tab[num].byClass<0)//������classС��0��
         {
              printf("The menuitem model tab is illegal!num=%d--class=%d\n",\
              tab[num].byClass);
              return result;
         }
         if((tab[num].byClass-temp_class)>1)//������󼶵�class��ǰ����2��������
         {
                printf("The menuitem model tab is illegal!--Num=%d-CurClass=%d,preClass=%d\n",\
                    num,tab[num].byClass,temp_class);
                return result;
          }
         if(strlen(tab[num].byName,namesize)==0)
         {
             printf("The menuitem model tab is illegal!--NULL name!\n");
             return result;
         }
         temp_class=tab[num].byClass;
    }
   result=true;
   return result;
}

// =========================================================================
// ��������:�����û���������Ը��Ĵ�С
// �������:tablist����������Ը����
// ���������
// ����ֵ  :tablist�������Ĳ˵�����Ը�ĸ���
// ˵��    :��tab_list������Ϊ�յ�ʱ���ʾ������
// =========================================================================
u32  CalTagMenuitemModuleTabSize(const struct _tagMenuitemModel * tab_list)
{
    u32  num=0;
    bool_t flag=true;
    while(flag)
    {
        if(strlen(tab_list[num].byName,namesize)==0)
        {
          flag=false;
        }
        else
        {
          num++;
        }
    }
    return num;
}

// ===================================================================================
// ��������:�Դ��ڵĲ˵�����һ���ڵ�Ϊ���ڵ����һ���µĲ˵���
// �������:menuitem_tree�����ڵĲ˵�����һ���ڵ㣻tab_list���û��˵�����Ը��
// ���������
// ����ֵ:��ӵĲ˵����ĸ��ڵ㣬һ���Ϊmenuitem_tree������Ӳ��ɹ�
//          ������·���NULL��
// ˵��:���ȼ��tab_list�ĺϷ��ԣ�Ȼ�������λ�ý������,tablist�ĵ�һ��������
//          �����˵��������߲˵���Σ�class��С��
// ===================================================================================
struct menu_item*  append_menu_branch(struct menu_item * menuitem_tree, const struct _tagMenuitemModel * tab_list)
{
     bool_t    result=false;
     u8        current_class=0;
     s8        layers=0;//todo;
     u32       num;
     u32       count=0;  //record how many tab has been added
     u32       size=CalTagMenuitemModuleTabSize(tab_list);

     struct menu_item  *current_menuitem=NULL;
     struct menu_item  *temp_menuitem=NULL;
     struct menu_item  *Mresult=NULL;

     //�������
     if(menuitem_tree==NULL)
     {
        return Mresult;
     }
     size=CalTagMenuitemModuleTabSize(tab_list);
     //����б����,������ֳ��ȡ�class�ĵȼ��ȹ���
     result= CheckTagMenuitemModelTab( tab_list,size);
     if(result==false)
     {
        return Mresult;
     }
     //��ʼ��
     current_menuitem=menuitem_tree;
     current_class=tab_list[0].byClass;
     //from here we begin to add the tab to the menuitem_0x_n tree
     for(num=0;num<size; num++)
     {
        temp_menuitem=CreateMenuitem(tab_list[num].byName, tab_list[num].byTip, tab_list[num].byAttrib, \
                                     tab_list[num].passWd, tab_list[num].wPara, tab_list[num].FunctionName);
        if(NULL==temp_menuitem)
        {
           printf("Failed to creat the menuitem--not enught mem!\n");
           //�ڴ治�㣬��ô�������Ȼ�������ɹ�
           DelMenuBranch(menuitem_tree);
           return Mresult;
        }
        if(tab_list[num].byClass>current_class)
        {
           //�϶�ֻ����1����Ϊ�ڹ����������Ѿ�����ˡ�
           result=AddSubMenuitem(current_menuitem,temp_menuitem,false);
           if(result==false)
           {
              printf("Low_layer: the num.%d name=%s.layers=%d class=%d,priviouse class=%d!\n",\
                          num,temp_menuitem->node.name,layers,tab_list[num].byClass,current_class);
              FreeMenuitem(temp_menuitem);
              DelMenuBranch(menuitem_tree);
              return Mresult;
            }
        }
        else if(tab_list[num].byClass==current_class)
        {
              if(num==0)
              {
                 result=AddSubMenuitem(current_menuitem,temp_menuitem,true);//the first tab
              }
              else
              {
                 result=AddBroMenuitem(current_menuitem,temp_menuitem,false);
              }
              if(result==false)
              {
                printf("Equ_layer: the num.%d name=%s.layers=%d class=%d,priviouse class=%d!\n",\
                                      num,temp_menuitem->node.name,layers,tab_list[num].byClass,\
                                               current_class);
                FreeMenuitem(temp_menuitem);
                DelMenuBranch(menuitem_tree);
                  return Mresult;
              }
              else
              {

              }
        }
        else
        {
           for(layers=tab_list[num].byClass;layers<current_class;layers++)
           {
              current_menuitem=GetParMenuitem(current_menuitem);
           }
           result= AddBroMenuitem(current_menuitem, temp_menuitem,false);//first menuitem node
           if(result==false)
           {
              printf("High_layer: the num.%d name=%s.layers=%d class=%d,priviouse class=%d!\n",\
                   num,temp_menuitem->node.name,layers,tab_list[num].byClass,current_class);
              FreeMenuitem(temp_menuitem);
              DelMenuBranch(menuitem_tree);
                  return Mresult;
           }
         }
         current_menuitem=temp_menuitem;
         current_class=tab_list[num].byClass;
         count++;
     }
     if(count==size)
     {
         printf("Has mount the tab completely!---num=%d\n",count);
     }
     else
     {
         printf("Has mount part of the tab!count_should_be=%d  count=%d\n",\
         size,count+1);
         result= false;
     }
     Mresult=menuitem_tree;

     return Mresult;
}

// =========================================================================
// ��������:��menu_root�ڵ��´���һ���˵���
// �������:treename�����ĸ��ڵ�����֣����Ϊ�ջ����Ѿ���������Զ��������֣�
//                   �Զ���������ֺ�������Ҫ�Լ�������׼��
//                   tab_list���û��˵�����Ը��
// ���������
// ����ֵ  :��ӵĲ˵����ĸ��ڵ㣬�ýڵ��Ѿ�������menu_root���ӽڵ��£���������Ӳ�
//        �ɹ���ʱ�������·���NULL��
// ˵��    :���ȼ��tab_list�ĺϷ��ԣ�Ȼ�������λ�ý������
// =========================================================================
struct menu_item  *create_menu_tree(const char *treename,const struct _tagMenuitemModel * tab_list)
{

     struct menu_item    *result;//used to store the result
     struct menu_item    *temp;//used to store the result

     result=CreateMenuitemTreeNode(treename);
     if(NULL==result)
     {
         printf("failed create tree node---create_menu_tree\n");
         return result;
     }
     temp=append_menu_branch(result, tab_list);
     if(NULL==temp)
     {
       DelMenuBranchCompletely(result);
       result=NULL;
     }

     return result;
}

