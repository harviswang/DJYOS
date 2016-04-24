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
// ģ������: DJY�˵�ģ��֮�˵���API���Բ˵���������������API����������
// ģ��汾: V1.00(��ʼ���汾)
// ������Ա: zqf
// ����ʱ��: 20121224
// =======================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// =======================================================================
#ifndef _MENU_ITEM_API_H
#define _MENU_ITEM_API_H


#include "Menuitem.h"

// ��ʼ���˵�����Դ
ptu32_t module_init_menu(ptu32_t para);         

//�˵��Ļ������������ɾ����
struct menu_item *  CreateMenuitemTreeNode(char * pTreeName);

struct menu_item *CreateMenuitem(const char *name,const char *byTip,u32 byAttrib,\
                             u32 passWd,u32 wPara, u32 (*MenuFunc)(struct menu_item *menuitem));

bool_t AddSubMenuitem(struct menu_item * pParent, struct menu_item *pNewmenu, bool_t head);

bool_t AddBroMenuitem(struct menu_item * pBrother, struct menu_item *pNewmenu, bool_t insert);

bool_t DelMenuBranchCompletely(struct menu_item  * pMenuitem);

bool_t DelMenuBranch(struct menu_item * pMenuitem);

bool_t FreeMenuitem(struct menu_item *pMenuitem);
//���Ҳ˵���
struct menu_item* SearchSubMenuitemByName(struct menu_item *pParent, const char *name);
struct menu_item* SearchBroMenuitemByName(struct menu_item *pBrother,const char *name);
struct menu_item* SearchSubTernalMenuitem(struct menu_item *pMenu);
struct menu_item* GetNextMenuitem(struct menu_item *pMenuitem);
struct menu_item* GetPreMenuitem(struct menu_item *pMenuitem);
struct menu_item* GetSonMenuitem(struct menu_item *pMenuitem);
struct menu_item* GetParMenuitem(struct menu_item *pMenuitem);

struct menu_item* GetBBmenuitem(struct menu_item  *menuitem);
struct menu_item* GetLBmenuitem(struct menu_item  *menuitem);
struct menu_item* GetMenuitemBynumber(struct menu_item *menuitem, s8 number);

//�򵥼���
u8   CalMenuitemNumber(struct menu_item *menuitem);
u32  CalMenuitemNum(struct menu_item  *menuitem);
u8   CalDisBettween2Menuitem(struct menu_item * menuitem1, struct menu_item *menuitem2 );

//bool_t CalPosRelative2SrcMenuitem(struct menu_item * DestMenuitem, struct menu_item *SrcMenuitem, s32 *RelaPos);

bool_t CalPosRelative2SrcMenuitem(struct menu_item * DestMenuitem, struct menu_item *SrcMenuitem, s8 *RelaPos);
//�޸Ĳ˵�����
bool_t  SetMenuitemPasswd(struct menu_item *pMenuitem, u16 passwd);
bool_t  RenameMenuitem(struct menu_item *pMenuitem, char *pNewName);

#endif
