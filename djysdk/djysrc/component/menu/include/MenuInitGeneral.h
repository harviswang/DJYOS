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
// MenuInitGeneral.h
// ģ������: 
// ģ��汾: 
// ����ʱ��: 2012-12-31
// ����    : Administrator
// ========================================================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// =========================================================================================================

#ifndef MENUINITGENERAL_H_
#define MENUINITGENERAL_H_


#include "MenuDisplay.h"

//Ӧ�ó���Ӧ�õ��ô˺���ʵ�ֲ˵��ĳ�ʼ������ʾ
struct menu_displaypara* menuappinit_general(\
		struct menu_item *menuitem,struct tagGkWinRsc *Appwin, u8 mydismode);

//�˵���ʾ�˳�
bool_t menu_exit(struct menu_displaypara *dispara);

//ִ����ʾ��������ʾ����ָ��
void   menu_display(struct menu_displaypara *dispara);


//ִ����ʾ������������Ӧָ�뺯����һ����Ǿ�������,��ʵ����ͨ�ú����Ѿ��������ṩ��
//�����˵���Ľӿڣ���˺�������ɾ�������ľ������裬������������̫���ˣ�����Ӧ��ֻ
//ʹ��ͨ������ӿڼ��ɡ�--TODO
struct menu_displaypara*  menuperipheral_respond(\
		struct menu_displaypara *operatingDispara, void* para );

//ִ����ʾ������ͨ�����躯����para�Ǿ��������μ�"MenuDisplay.h"
struct menu_displaypara*  menupeiripheralrespondgeneral(\
		struct menu_displaypara *operatingDispara, void* para );


struct menu_displaypara*  menukeyboard_respond(\
		struct menu_displaypara *operatingDispara, void* para );

struct menu_displaypara*  menumousetouch_respond(\
		struct menu_displaypara *operatingDispara, void* para );

#endif /* MENUINITGENERAL_H_ */
