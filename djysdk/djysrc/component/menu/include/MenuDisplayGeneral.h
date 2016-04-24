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
// ģ������: DJY�˵�ģ��֮��ʾ����֮ͨ����ʾ��������
// ģ��汾: V1.00(��ʼ���汾)
// ������Ա: zqf
// ����ʱ��: 20121224
// =======================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// =======================================================================

#ifndef  _MENU_DISPLAY_GENERAL_H
#define  _MENU_DISPLAY_GENREAL_H

#include "MenuDisplay.h"

//������ʾ����,�÷�������ģ���ڲ�ʹ�ã��������������
//bool_t DrawMenuLayerBox(struct menu_displaypara*  dispara);

bool_t display_menu_horizontal(struct menu_displaypara * dispara);
bool_t display_menu_vertical(struct menu_displaypara * dispara);
bool_t display_menu_3by3(struct menu_displaypara * dispara);

//��ʾ�ֶ�
struct menu_displaypara *exe_menuitemfunc(struct menu_displaypara *dispara);
struct menu_displaypara *goto_submenu(struct menu_displaypara * operatingDispara);
struct menu_displaypara *goto_parmenu(struct menu_displaypara *operatingDispara);

void goto_nextmenuitem(struct menu_displaypara  *dispara);

void goto_premenuitem(struct menu_displaypara  *dispara);

void goto_menuitem_inslayers(struct menu_displaypara *pDispara, s8 step);

void  windows_draw_text(struct draw_wintext_para *para,const char *text);


//�ı�д�ֺ������������У�ֻ��Ҫһ����㼴�ɣ�����֤���еĶ���ʾ����ʾ������һ��������Ҫ��

char*  draw_text_in_rec(struct draw_wintext_para *para,const char *text);

char*  draw_text_on_win(struct draw_wintext_para *para,const char *text);

void   draw_digital_in_rec(struct draw_wintext_para *para, s32 data,s8 hex,s8 bitNumber,\
	                       s8 selectBit,u32 selectTextColor,u32 selectBackColor);



#endif















































