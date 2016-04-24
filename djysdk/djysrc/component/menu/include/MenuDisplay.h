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
// ģ������: DJY�˵�ģ��֮��ʾģ����Ҫ���ݽṹ����
// ģ��汾: V1.00(��ʼ���汾)
// ������Ա: zqf
// ����ʱ��: 20121224
// =======================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// =======================================================================

#ifndef __MENUDISPLAY_H__
#define __MENUDISPLAY_H__

#include "stdint.h"
#include "Menuitem.h"

#define cn_space_margin   4//���ڵ�����,�ĸ�����

//����״̬������ɫ
#define  cn_column_space   cn_space_margin//״̬�����
#define  color_back        CN_COLOR_BLACK //״̬������ɫ
#define  color_light       CN_COLOR_WHITE //״̬������ɫ

//Ĭ�ϵĲ˵���ɫ
#define cn_menu_text_color      CN_COLOR_WHITE //�˵��ı���ɫ
#define cn_menu_color           0x00c0c0c0     //�˵���ɫ
#define cn_menu_lighted_color   CN_COLOR_BLUE  //�˵�����ɫ

enum KEYVALUE
{
  KEYL=37,//key left
  KEYR=39,//key  right
  KETU=38,//key  up
  KEYD=40,//key down
  KEYE=13,//key enter
  KEYESC=27,//key esc
};

enum  DIS_CMD
{
   GOTO_SUBMENU=0,//�л����Ӳ˵�
   GOTO_PARENT,//�л������˵�
   GOTO_NEXT,//�л�����һ���˵���
   GOTO_PRE,//�л�����һ���˵���
   GOTO_EXE,//ִ�в˵����Ӻ���
   GOTO_SLAYSERS//��ͬ��˵���֮���л�
};

//Ŀǰ�������ʾģʽ��������������µ���ʾģʽ������������
enum  DISPLAY_MODE
{
   DIS_PULL=0,
   DIS_9CELL
};
struct display_color
{
 //  s32 win_color;�˵���ɫ���Ǵ�����ɫ
   s32 menu_color;  //�˵���ɫ
   s32 text_color;   //�ı���ɫ
   s32 menu_lighted_color;//������ɫ
};
//��¼�˵���ռ�õĿռ�:��͸�
struct menu_item_space
{
   s32 width;
   s32 height;
};
//�����꣨x��y��
struct coordinate_point
{
   s32  x;
   s32  y;
};

struct menu_displaypara
{
  //ͨ�����ݳ�Ա����
 //  u8    dismode;
  //��¼·��
   struct menu_displaypara  *pre;
   struct menu_displaypara  *next;
  //��¼��ǰ�˵������ʾ����
  struct display_color   DisplayColor; //���ڱ�����ʾ����ɫ
  struct menu_item_space  MenuitemSpace;//���ڼ�¼�˵���Ŀ�Ŀռ䣬��һ�μ����ͼ�¼������ÿ�ζ�����һ��

  //��ǰ�˵���ı�Ƕ���
  struct menu_item   *OperatingMenuitem;//��ǰ��˵����ڲ����Ĳ˵���
  struct menu_item   *FVisualMenuitem;//��ǰ��˵���ʾ�ĵ�һ���˵���
  struct menu_item   *LVisualMenuitem;//��ǰ��˵���ʾ�����һ���˵���
  //����
  struct tagGkWinRsc      *AppWin;   //����Ӧ�ó��򴴽��Ĵ���
  struct tagGkWinRsc      *CurWin;   //��ʾ����˵��Ĵ���

  //������ʾ����
  ptu32_t (*CreateMenuDisparaBypre)(struct menu_displaypara *pre,\
          struct menu_item *menuitem,struct tagGkWinRsc  *Appwin);
  //��ʾ����
  ptu32_t (*MyMenuDispalyFunction)(struct menu_displaypara *para);
  //�������Ӳ˵���
  ptu32_t (*AdjustVisualMenuitem)(struct menu_displaypara  *para);
  // ��Ե��Ǿ�������裬������̴�����ȵ�ʵ�֡�
  ptu32_t  (*menukeyboard_action)(struct menu_displaypara *operatingDispara, void *para );
  ptu32_t  (*menumousetouch_ction)(struct menu_displaypara *operatingDispara, void *para );

  ptu32_t  (*MyDisplayPeripheralRespond)(struct menu_displaypara *operatingDispara, void *para );
  //���趯��������ʵ�ֵ���һ���չ�����л������߷���
  ptu32_t (*MyPeripheralGeneralAction)(struct menu_displaypara *operatingDispara,void *para);
  //��ʼ������
  ptu32_t  (*MyMenuInitFunction)(struct menu_item *FirstMenuitem,struct tagGkWinRsc *Appwin);

  void  *myprivate;//ָ��ÿ�ַ���Լ������ݳ�Ա������ÿ����ʾ�������е�һЩ���ݣ�ָ�����һ������ָ��ṹ�塣
};

struct menu_display_private
{
  //������ʾ����
  ptu32_t (*CreateMenuDisparaBypre)(struct menu_displaypara *pre,\
          struct menu_item *menuitem,struct tagGkWinRsc  *Appwin);
  //��ʾ����
  ptu32_t (*MyMenuDispalyFunction)(struct menu_displaypara *para);
  //�������Ӳ˵���
  ptu32_t (*AdjustVisualMenuitem)(struct menu_displaypara  *para);
  // ��Ե��Ǿ�������裬������̵ȵ�ʵ�֡�
  ptu32_t  (*MyDisplayPeripheralRespond)(struct menu_displaypara *operatingDispara, void *para );
  //���趯��������ʵ�ֵ���һ���չ�����л������߷���
  ptu32_t (*MyPeripheralGeneralAction)(struct menu_displaypara *operatingDispara,void *para);

};
enum draw_text_direction
{
   Left2Right=0,
   Right2Left,
   Top2Bottom,
   Bottom2Top
};


//���ڻ��˵��ı�
struct draw_wintext_para
{
    u8  draw_type;                  //������ʾ��ʽ:1�����ң�2���ҵ��� 3���ϵ���
    u8  line_limit;                 //������ֵ�ͻỻ��
    s32 x, y;                       //��ʾ�������ʼλ�ã������gkwin����ʾ��ʼλ��
    s32 width,height;               //��ʾ����Ŀ�Ⱥ͸߶�
    s32 count;                      //���������ȣ��ֽ�������������������
    u32 colortext;                  //������ɫ
    u32 colorback;                  //������ɫ
    struct tagGkWinRsc* gkwin;        //Ŀ�괰��
};

#endif
