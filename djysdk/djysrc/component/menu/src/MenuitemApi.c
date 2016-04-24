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
// ģ������: DJY�˵�ģ��֮�˵���API���Բ˵���������������API������
// ģ��汾: V1.00(��ʼ���汾)
// ������Ա: zqf
// ����ʱ��: 20121224
// =============================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// =============================================================================
#include "MenuitemApi.h"
#include "MenuFunction.h"

#define                            cn_limit_menuitem        1000 //max menuitem num
static struct menu_item             sMenuitemRsc[cn_limit_menuitem];  // todo:
static struct tagRscNode             *pg_menuitem_root;  // the menuitem tree
struct tagMemCellPool               *pg_menuitem_pool;// the menuitem pool
//static struct menu_item             *pg_operating_menuitem;// the current operating menuitem ,for the visible

#define                            CN_MENU_ROOT_NUM           10    //the maxi node son of pg_menuitem_rsc_tree
static  u8                         menuitem_tree_count=0;     //the current node son of pg_menuitem_rsc_tree
//struct tagSemaphoreLCB              *pg_menuitem_semp;//use to limit the menuitem rsc tree operation limit

// =========================================================================
// ��������:�˵�ģ���ʼ����������ʼ����Ӧ��Դ����Ӹ��ڵ�
// �������:    ��
// �����������
// ����ֵ  :
// ˵��    :�����˵�����ڴ�غ͸��ڵ�
// =========================================================================
ptu32_t module_init_menu(ptu32_t para)
{
   static struct tagRscNode sMenuitemRoot;
   bool_t  result=false;

   //init the menuitem tree
   pg_menuitem_root= Rsc_AddTree(&sMenuitemRoot, sizeof(struct tagRscNode),RSC_RSCNODE,"MenuItem_root");
   if(NULL==pg_menuitem_root)
   {
      printf("create the pg_menuitem_root failed!\n");
      return result;
   }
   //init the menuitem pool
   pg_menuitem_pool = Mb_CreatePool((void*)sMenuitemRsc,
                                    cn_limit_menuitem,
                                    sizeof(struct menu_item),
                                    20,100,
                                    "MenuItem_pool");
   if(NULL==pg_menuitem_pool)
    {
       printf("create the pg_menuitem_pool failed!\n");
       return result;
    }
    result=true;
    return result;
}

// =========================================================================
// ��������:�ͷŲ˵�����ռ�õĿռ�
// �������:���ͷŵĲ˵���pMenuitem
// ���������
// ����ֵ  : true���ɹ���false��ʧ�ܣ���node�ڵ�û��clean��
// ˵��    :�������������ͷ���Щû���߼������Ĳ˵�������node��û��clean�����򲻻��ͷ�
// =========================================================================
bool_t FreeMenuitem(struct menu_item * pMenuitem)
{
   bool_t result=false;
   if(NULL==pMenuitem)
   {
      printf("This is an empty pMenuitem!\n");
      result=true;
      return result;
   }
   else if((pMenuitem->node.child!=NULL)||(pMenuitem->node.parent!=NULL)||\
    (pMenuitem->node.previous!=NULL)||(pMenuitem->node.next!=NULL))
   {
      printf("use DelMenuBranchCompletely(struct menu_item * pMenuitem)!\n");
      return result;
   }
   else
   {
     Mb_Free(pg_menuitem_pool,(void *)pMenuitem);
     result = true;
     return result;
   }
}

// =========================================================================
// ��������:�����˵����ڵ�
// �������:pTreeName���˵������ڵ�����֣����Ϊ����Զ��������֣��Զ���������ֻ�
//         ���������Լ���Ҫ������׼��
// ���������
// ����ֵ    :�����Ĳ˵������ڵ�
// ˵��          :�˵������ڵ����������ֲ�ͬ�Ĳ˵����������Ĳ˵������ڵ��Ѿ���ӵ��˵�����
//          menu_root�ڵ���
// =========================================================================
struct menu_item *CreateMenuitemTreeNode(char * pTreeName)
{

   u8     charLen=0;
   u8     treeNumber=0;
   struct menu_item *result=NULL;
   char   name[namesize];//��̬��������ʱʹ��

   if(menuitem_tree_count>(CN_MENU_ROOT_NUM-1))
   {
     printf("has achived the max menuitem root number!");
     return result;
   }
   if((NULL==pTreeName)||(0==strlen(pTreeName,namesize))||\
      (NULL!=SearchSubMenuitemByName(pg_menuitem_root, pTreeName)))//����Ϊ�ա�����Ϊ0���������ᵼ�´���ʧ��
   {
      printf("Invalid name-----CreateMenuitemTreeNode\n");
      for(treeNumber=0;treeNumber<CN_MENU_ROOT_NUM;treeNumber++)
      {
         itoa(treeNumber,name,16);
         if(NULL==SearchSubMenuitemByName(pg_menuitem_root, name))
         {
           break;
         }
      }
      if(CN_MENU_ROOT_NUM==treeNumber)//û�п�ʹ�õ����֣���Ȼ�ǲ����ܵģ�ǰ���Ѿ���tree number�����жϡ�
      {
        return result;
      }
      else
      {
        pTreeName=name;
      }
   }

   result=(struct menu_item *)Mb_Malloc(pg_menuitem_pool,0);
   if(NULL==result)
   {
      printf("menuitem mrmory pool is not enough!\n");
      return result;
   }
   charLen=strlen(pTreeName,namesize);
   if(namesize<=charLen)//Դ�ַ���̫��
   {
     memcpy(result->name,pTreeName,namesize);
     result->name[namesize-1]='\0';
   }
   else
   {
     strcpy(result->name,pTreeName);
   }

   Rsc_CleanNode((struct tagRscNode *) result);
   Rsc_AddHeadSon((struct tagRscNode *)(pg_menuitem_root),\
       (struct tagRscNode *) result, sizeof(struct menu_item),\
        RSCTYPE_USER,result->name);
   menuitem_tree_count++;
   printf("menuitem_tree_node:%s\n",result->node.name);

   return result;
}

// =========================================================================
// ��������:�����˵���
// �������:name���˵�����Դ�ڵ�����֣�byTip:��ʾ�ַ���passwd�����룬wpara������
//         FuncName�������Ĺ��Ӻ���������
// ���������
// ����ֵ    :ָ�򴴽��Ĳ˵����ָ��
// ˵��          :�����Ĳ˵�������ֵĳ�����namesize�涨�����˻�ض�
// =========================================================================
struct menu_item *CreateMenuitem(const char *name,const char *byTip,u32 byAttrib,u32 passWd,u32 wPara,\
    const char*FuncName)
{

    u8     charLen=0;
    struct menu_item  *result=NULL;

    if(strnlen(name,namesize)==0)
    {
     printf("invalid name for create the menuitem!\n");
     return result;
    }
    result=(struct menu_item *)Mb_Malloc(pg_menuitem_pool,0);
    if(NULL==result)
    {
      printf("Not enough mem for createmenuitem!\n");
      return result;
    }
    //��ʼ������ͷ
    Rsc_CleanNode(&result->node);
    //���廯��Ա����
    //��������
    charLen=strnlen(name,namesize);
    if(namesize<=charLen)//Դ�ַ���̫��
    {
      memcpy(result->name,name,namesize);
      result->name[namesize-1]='\0';
    }
    else
    {
       strcpy(result->name,name);
    }
    result->node.name=result->name;
    //����bytip
    charLen=strnlen(byTip,namesize);
    if(namesize<=charLen)//Դ�ַ���̫��
    {
      memcpy(result->bytip,byTip,namesize);
      result->name[namesize-1]='\0';
    }
    else
    {
      strcpy(result->bytip,byTip);
    }
    //��ʼ����Ҫ����
    result->byAttrib=byAttrib;
    result->MenuitemFunction=getfunc_byname(FuncName);
    result->wPassword=passWd;
    result->wPara=wPara;
    result->font=0;

    return result;
}

// =========================================================================
// ��������:���Ӳ˵�����Ѱ������Ϊname���Ӳ˵�
// ���������pParent�����˵��name���Ӳ˵��������
// ���������
// ����ֵ    :�ҵ����Ӳ˵�Ϊname�Ĳ˵��NULLû���ҵ���
// ˵��          :
// =========================================================================
struct menu_item* SearchSubMenuitemByName(struct menu_item * pParent, const char * name)
{
   struct menu_item *result=NULL;

   result=(struct menu_item*)Rsc_SearchSon((struct tagRscNode * )pParent, name);//�Ѿ����˲����ļ��
   return result;
}

// =========================================================================
// ��������:��ͬ��˵���Ѱ������Ϊname���ֵܲ˵�
// ���������pBrother:�ṩ��ͬ��˵���name��Ѱ�ҵ�����
// ���������
// ����ֵ    :�ҵ��Ĳ˵���Ϊname�Ĳ˵��NULLû���ҵ���
// ˵��          :
// =========================================================================
struct menu_item * SearchBroMenuitemByName(struct menu_item * pBrother, const char * name)
{

    struct menu_item * result=NULL;

    result=(struct menu_item *)Rsc_SearchSibling((struct tagRscNode *)pBrother,  name);

    return result;
}

// =========================================================================
// ��������:Ϊ���˵�������Ӳ˵�
// ���������pParent:���˵��pNewmenuitem������ӵĲ˵��head��true,��ӵ��ǵճ��Ӳ˵�
//          false,��ӵ�����С�Ӳ˵���
// ���������
// ����ֵ    :true����ӳɹ���false��ʧ��
// ˵��          :����ͬ�����Ӳ˵�����ʱ�������ʧ�ܵ�
// =========================================================================

bool_t AddSubMenuitem(struct menu_item * pParent, struct menu_item *pNewmenuitem, bool_t head)
{
    bool_t  result=false;
    struct tagRscNode  *temp_node;

    //check if any sub menuitem named this
    if(NULL!=SearchSubMenuitemByName(pParent,pNewmenuitem->name))
    {
        printf("There has been a sub menuitem named this!\n");
        return result;
    }
    if (head)
    {
        temp_node= Rsc_AddHeadSon((struct tagRscNode *)pParent, (struct tagRscNode *)pNewmenuitem, \
            sizeof(struct menu_item),RSCTYPE_USER,pNewmenuitem->name);
    }
    else
    {
        temp_node= Rsc_AddSon((struct tagRscNode *)pParent, (struct tagRscNode *)pNewmenuitem, \
            sizeof(struct menu_item),RSCTYPE_USER,pNewmenuitem->name);
    }

    if(temp_node!=NULL)
        result=true;
    return  result;
}

// =========================================================================
// ��������:����ֵܲ˵���
// ���������pBrother:�ֵܲ˵��pNewmenuitem������ӵĲ˵��insert��true,��ӵ����ֲ˵���pre��
//          false,��ӵ��ǵܲ˵���
// ���������
// ����ֵ    :true���ɹ���falseʧ��
// ˵��          :��ͬ��˵����Ѿ���ͬ���Ĳ˵������ʱ�����ʧ��
// =========================================================================
bool_t AddBroMenuitem(struct menu_item * pBrother, struct menu_item *pNewmenuitem, bool_t insert)
{
    bool_t result=false;
    struct tagRscNode *temp_node;

    //check any brother menuitem named this
    if(NULL!=SearchBroMenuitemByName(pBrother, pNewmenuitem->name))
    {
       printf("There has been a brother menuitem named this!\n");
       return result;
    }
    if(insert)
    {
         temp_node=Rsc_AddToPrevious((struct tagRscNode *)pBrother, (struct tagRscNode *)pNewmenuitem,\
            sizeof(struct menu_item), RSCTYPE_USER,pNewmenuitem->name);
    }
    else
    {
         temp_node=Rsc_AddToNext(&(pBrother->node), &(pNewmenuitem->node),\
                    sizeof(struct menu_item),RSCTYPE_USER, pNewmenuitem->name);
    }
    if(NULL!=temp_node)
        result=true;

    return  result;
}

// =========================================================================
// ��������:Ѱ�ҵ�ǰ�˵����һ���˵㣨�ò˵��������Ӳ˵���,ĩ�ҽڵ�
// ���������pMnuitem����Ѱ�ҵ����Ȳ˵�
// ���������
// ����ֵ    :�ҵ���û���Ӳ˵��ĺ���˵�
// ˵��          :��ǰ����ɨ�裬
// =========================================================================
struct menu_item* SearchSubTernalMenuitem(struct menu_item * pMenuitem)
{

   struct menu_item* result=NULL;
   result=(struct menu_item *)Rsc_GetTwig((struct  tagRscNode *)pMenuitem);
   return result;
}

// =========================================================================
// ��������:ɾ�������˵���֧�������Ƚڵ�
// ���������pMenuitem����ɾ���Ĳ˵���֧���Ƚڵ�
// ���������
// ����ֵ  :true�ɹ� falseʧ��
// ˵��    :ɾ����ǰ�˵�������к���˵��Լ���ǰ�˵����߼��Ϻ������ϣ���ͨ��ѭ��ɾ��
//          �˽ڵ���ʵ��
// =========================================================================
bool_t DelMenuBranchCompletely(struct menu_item * pMenuitem)
{
   bool_t             result=false;
   struct menu_item    *temp_menuitem=pMenuitem;

  if((struct tagRscNode *)pMenuitem==pg_menuitem_root)//this is an menuitem root node
  {
     printf("Invalid para---DelMenuBranchCompletely\n");
    return result;
  }
  while((temp_menuitem=SearchSubTernalMenuitem(pMenuitem))!=NULL)
  {
    printf("begin to delete the menuitem=%s\n",temp_menuitem->node.name);
    Rsc_DelNode((struct tagRscNode *)temp_menuitem);
    Mb_Free(pg_menuitem_pool, temp_menuitem);
  }
  if(Rsc_GetParent((struct tagRscNode *)pMenuitem)==pg_menuitem_root)
  {
     menuitem_tree_count--;
  }
  printf("begin to delete the menuitem=%s\n",pMenuitem->node.name);
  Rsc_DelNode((struct tagRscNode *)(&(pMenuitem->node)));
  Mb_Free(pg_menuitem_pool, pMenuitem);
  result=true;

    return result;

}
// =========================================================================
// ��������:ɾ���˵���֧����������֧���Ƚڵ㣩
// ���������pMenuitem����ɾ���Ĳ˵�������Ȳ˵�
// ���������
// ����ֵ :true�ɹ� falseʧ��
// ˵��   :ɾ����ǰ�˵�������к���˵���������ǰ�˵����߼��Ϻ������ϣ���ͨ��ѭ��ɾ��
//          �˽ڵ���ʵ��
// =========================================================================
bool_t DelMenuBranch(struct menu_item * pMenuitem)
{
   bool_t             result=false;
   struct menu_item    *temp_menuitem=pMenuitem;

  if((struct tagRscNode *)pMenuitem==pg_menuitem_root)//this is an menuitem root node
  {
     printf("Invalid para---DelMenuBranchCompletely\n");
    return result;
  }
  while((temp_menuitem=SearchSubTernalMenuitem(pMenuitem))!=NULL)
  {
    printf("begin to delete the menuitem=%s\n",temp_menuitem->node.name);
    Rsc_DelNode((struct tagRscNode *)temp_menuitem);
    Mb_Free(pg_menuitem_pool, temp_menuitem);
  }
  result=true;

    return result;

}
// =========================================================================
// ��������:��ȡ�ṩ�˵���ĺ�һ���˵�
// ���������pMenuitem���ṩ�Ĳ˵���
// ���������
// ����ֵ    :pMenuitem��next�˵���
// ˵��          :
// =========================================================================

struct menu_item *GetNextMenuitem(struct menu_item * pMenuitem)
{
    struct menu_item * result=NULL;

    result=(struct menu_item *)Rsc_GetNext((struct tagRscNode *)pMenuitem);

    return result;


}
// =========================================================================
// ��������:��ȡ�ṩ�˵����ǰһ���˵�
// ���������pMenuitem���ṩ�Ĳ˵���
// ���������
// ����ֵ    :pMenuitem��pre�˵���
// ˵��          :
// =========================================================================

struct menu_item *GetPreMenuitem(struct menu_item * pMenuitem)
{
    struct menu_item * result=NULL;

    result=(struct menu_item *)Rsc_GetPrevious((struct tagRscNode *)pMenuitem);

    return result;


}
// =========================================================================
// ��������:��ȡ�ṩ�˵���ĵճ��Ӳ˵���
// ���������pMenuitem���ṩ�Ĳ˵���
// ���������
// ����ֵ    :pMenuitem�ĵճ��Ӳ˵���
// ˵��          :
// =========================================================================

struct menu_item *GetSonMenuitem(struct menu_item * pMenuitem)
{
    struct menu_item * result=NULL;
    result=(struct menu_item *)Rsc_GetSon((struct tagRscNode *)pMenuitem);
    return result;
}
// =========================================================================
// ��������:��ȡ�ṩ�˵���ĸ��˵�
// ���������pMenuitem���ṩ�Ĳ˵���
// ���������
// ����ֵ    :pMenuitem��parent�˵���
// ˵��          :
// =========================================================================

struct menu_item *GetParMenuitem(struct menu_item * pMenuitem)
{
    struct menu_item * result=NULL;

    result=(struct menu_item *)Rsc_GetParent((struct tagRscNode *)pMenuitem);

    return result;
}

// =========================================================================
// ��������:��ȡ�ṩ�˵������С�ܲ˵�
// ���������pMenuitem���ṩ�Ĳ˵���
// ���������
// ����ֵ    :pMenuitem����Сbrother��next����δ��
// ˵��          :
// =========================================================================
struct menu_item * GetLBmenuitem(struct menu_item  *menuitem)
{
    struct menu_item   *result=NULL;


    result=GetBBmenuitem(menuitem);
    result=GetPreMenuitem(result);
    return result;
}
//��ȡ����brother�˵���
// =========================================================================
// ��������:��ȡ�ṩ�˵�������brother�˵�
// ���������pMenuitem���ṩ�Ĳ˵���
// ���������
// ����ֵ    :pMenuitem�����brother��pre����ǰ��
// ˵��          :
// =========================================================================
struct menu_item * GetBBmenuitem(struct menu_item  *menuitem)
{
    struct menu_item   *result=NULL;

    result=(struct menu_item *)Rsc_GetHead((struct tagRscNode *)menuitem);
    return  result;
}
// =========================================================================
// ��������:����ͬ��˵��Ĳ˵���Ŀ
// ���������pMenuitem���ṩ�Ĳ˵���
// ���������
// ����ֵ    :ͬ��˵�����Ŀ
// ˵��          :0x00��ʾ�߼������������˵�������
// =========================================================================
u32  CalMenuitemNum(struct menu_item  *menuitem)
{
     u32    result=0;
     struct menu_item   *temp;
     temp=menuitem;
     do{
          result++;
          temp=GetNextMenuitem(temp);
          if((0xFF==result)||(NULL==temp))
          {
              result=0x00;
              break;//�߼����������ѭ��״̬
          }
     }while(temp!=menuitem);
     return result;
}
// =========================================================================
// ��������:���㵱ǰ�˵�����ͬ��˵���ĵڼ�λ
// ���������pMenuitem���ṩ�Ĳ˵���
// ���������
// ����ֵ    :��ǰ�˵�����ͬ��˵���ĵڼ���
// ˵��          :0x00��ʾ������,˳���Ǵӵճ���Ϊ��һ������
// =========================================================================
u8 CalMenuitemNumber(struct menu_item *menuitem)   //todo:���Ƶĺ���һ���
{
   u8   result=0;

   struct menu_item *tempmenuitem=NULL;
   tempmenuitem=GetBBmenuitem(menuitem);
   if(NULL==tempmenuitem)
   {
      printf("Invalid para for calmenuitemNumber!\n");
      result=0x00;
      return result;
   }

   result=1;
   while(menuitem!=tempmenuitem)
   {
      result++;
      if(0xFF==result)
      {
        result=0x00;
        break;
      }
      tempmenuitem=GetNextMenuitem(tempmenuitem);
   }

   return result;
}
// =========================================================================
// ��������:��ȡͬ��˵���ĵ�number���˵���
// ���������pMenuitem��ͬ��˵��number����Ѱ�ҵĲ˵���ĺ���
// ���������
// ����ֵ    :ͬ��˵���ĵ�number���˵���
// ˵��          :Ѱ�ҵ�˳����ͬ��˵���ĵճ���Ϊ��һ�����𣬲���ѭ��ȡ��ķ���
// =========================================================================
struct menu_item *GetMenuitemBynumber(struct menu_item *pMenuitem, s8 number)
{
   struct menu_item *result=NULL;

   u8    TotalNum=0;

   if(NULL==pMenuitem)//||(û��numberΪ0�Ĳ˵���)
   {
       printf("Invalid parameter for GetMenuitemByNunber!\n");
       return result;
   }

   TotalNum=CalMenuitemNum(pMenuitem);
   result=GetBBmenuitem(pMenuitem);
   //auto adjust the no of the searching menuitem,even the number is negative,we also adjust it
   while(number<=0)//���������Ҫ����
   {
      number=number+TotalNum;
   }
   if(number>TotalNum)
   {
      number=number%TotalNum;
   }
   TotalNum=1;
   while(TotalNum!=number)
   {
      result=GetNextMenuitem(result);
      if(NULL==result)
      {
          break;
      }
      TotalNum++;
   }

   return result;
}
// =========================================================================
// ��������:����ͬ�������˵���֮��ľ��루��������˵����
// ���������menuitem1����һ���˵��menuitem2���ڶ����˵���
// ���������
// ����ֵ    :��ǰ�˵�����ͬ��˵���ĵڼ���
// ˵��          :0xFF��ʾ������,˳���Ǵӵճ��ӵ���С�ӵķ���
// =========================================================================
u8  CalDisBettween2Menuitem(struct menu_item * menuitem1, struct menu_item *menuitem2 )
{
     u8   result;//������
     u8   num1=0,num2=0;//��������˵������


     if((NULL==menuitem1)||(NULL==menuitem2))//�������
     {
           printf("Invalid parameter--CalDisBetween2Menuitem!\n");
           return 0xFF;
     }
     if(GetParMenuitem(menuitem1)!=GetParMenuitem(menuitem2))//����ͬһ��˵�
     {
        printf("They are not the same slayers!\n");
        return 0xFF;
     }
     //�������ǵ����
     //�������ǵ����
     num1=CalMenuitemNumber(menuitem1);
     num2=CalMenuitemNumber(menuitem2);
     if((num1==0)||(num2==0))
     {
         return 0xFF;
     }
     else
     {
         if(num1>num2)
         {
              result=num1-num2;
         }
         else
         {
              result=num2-num1;
         }
         return result;
     }
}
// =========================================================================
// ��������:����Ŀ�Ĳ˵������Դ�˵����λ��
// ���������DestMenuitem:Ŀ�Ĳ˵��SrcMenuitem��Դ�˵���
// ���������Relative,Ŀ�Ĳ˵��������Դ�˵����λ�ã�������ʾǰ�棬����ʾ���棬
// ����ֵ    :
// ˵��          :-128��ʾ������,˳���Ǵӵճ��ӵ���С�ӵķ���
// =========================================================================
bool_t CalPosRelative2SrcMenuitem(struct menu_item * DestMenuitem, struct menu_item *SrcMenuitem, s8 *RelaPos)
{
     bool_t   result=false;
     u8  num1=0,num2=0;//��������˵������
     s8   num=0;

     if((NULL==DestMenuitem)||(NULL==SrcMenuitem))//�������
     {
           printf("Invalid parameter---CalPosRelative2SrcMenuitem\n");
           return result;
     }
     if(GetParMenuitem(DestMenuitem)!=GetParMenuitem(SrcMenuitem))//����ͬһ��˵�
     {
        printf("Not same layers----CalPosRelative2SrcMenuitem\n");
        return result;
     }
     //�������ǵ����
     num1=CalMenuitemNumber(DestMenuitem);
     num2=CalMenuitemNumber(SrcMenuitem);
     if((num1==0)||(num2==0))
     {
         return result;
     }
     else
     {
         num=num1-num2;
         *RelaPos=num;
         result=true;
         return result;
     }
}
//���ò˵�������
// =========================================================================
// ��������:���ò˵�������
// �������:pMenuitem ���޸ĵĲ˵��pName��������
// �������:
// ����ֵ    :true�ɹ� falseʧ��
// ˵��          :��Ϊ����������������û�����֣�������Щ�����ᵼ��ʧ�ܡ�
// =========================================================================
bool_t  RenameMenuitem(struct menu_item *pMenuitem, char *pNewName)
{
   bool_t result=false;
   u8     charLen=0;

   if((NULL==pMenuitem)||(NULL==pNewName)||(0==(charLen=strnlen(pNewName,namesize))))
   {
       printf("Invalid para for rename the Menuitem!\n");
       return result;
   }
   if(charLen<namesize)
   {
     strcpy(pMenuitem->name,pNewName);
   }
   else
   {
      memcpy(pMenuitem->name,pNewName,namesize);
      pMenuitem->name[namesize-1]='\0';
   }
   result=true;
   return result;
}
// =========================================================================
// ��������:���ò˵�������
// �������:pMenuitem ���޸ĵĲ˵��u16 passwd��������
// �������:
// ����ֵ    :true�ɹ� falseʧ��
// ˵��          :��Ϊ����������������û�����֣�������Щ�����ᵼ��ʧ�ܡ�
// =========================================================================
bool_t  SetMenuitemPasswd(struct menu_item *pMenuitem, u16 passwd)
{
   bool_t result=false;

   if(NULL==pMenuitem)
   {
       printf("Invalid para for rename the Menuitem!\n");
   }
   else
   {
       pMenuitem->wPassword=passwd;
       result=true;
   }
   return result;
}
