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
// MenuInitGeneral.c
// ģ������: �˵���ʾģ��֮��ʼ���˵���ʾ����
// ģ��汾:
// ����ʱ��: 2012-12-31
// ����    : Administrator
//           �˵���ʾ�����ĳ�ʼ����ָ���Ƕ�����ʾģʽ��������Ӧ�����ĳ�ʼ����
// =============================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// =============================================================================

#include "MenuInitGeneral.h"

#include "MenuSpaceGeneral.h"
#include "MenuInit_9cell.h"
#include "MenuInit_pull.h"

// =============================================================================
// ��������:�˵���ʼ�������������һ������չ������Ŀǰֻ�оŹ���ģʽ������ģʽ
// �������:menuitem�������Ĳ˵����ĵ�һ���˵��win��Ӧ�ó����ṩ��window
// ���������
// ����ֵ  :��ʼ�������ʾ�����������������˵�����ʾ
// ˵����Ŀǰ����ʵ���˾Ź��������ģʽ������Ҫ���µ�ģʽ������������;Ź������ʽ
//       �Լ�ʵ��
// =============================================================================
struct menu_displaypara* menuappinit_general(struct menu_item *menuitem,\
        struct tagGkWinRsc *Appwin, u8 mydismode)
{
    struct menu_displaypara *operatingDispara;

    switch (mydismode)
    {
        case DIS_9CELL:
            operatingDispara=module_menu_init_9cell(menuitem,Appwin);
            break;
        case DIS_PULL:
            operatingDispara=module_menu_init_pull(menuitem,Appwin);
            break;
        default:
            break;
    }

    return operatingDispara;
}
// =============================================================================
// ��������:�˵��˳���ʾ
// �������:menuitem�������Ĳ˵����ĵ�һ���˵��win��Ӧ�ó����ṩ��window
// ���������
// ����ֵ    :
// =============================================================================
bool_t menu_exit(struct menu_displaypara *dispara)
{
    dispara = getmainmenudispara(dispara);
    deldisplaypara(dispara);
    return true;
}
// =============================================================================
// ��������:ִ�в˵���ʾ��������ʾ�����������˵�����ʾ����
// �������:dispara������ʾ����ʾ����
// ���������
// ����ֵ  :
// ˵��    : ���ø���ʾ��������ʾ����������ʾ����Ȼ��ĺ���ָ��Ϊ�յĻ��������κη�Ӧ��
//          �����㱾��ϣ������0000������������������ǵ��¡�
// =============================================================================
void menu_display(struct menu_displaypara* dispara)
{
    if((NULL==dispara)||(NULL==dispara->MyMenuDispalyFunction))
    {
        printf("Invalid parameter--- MenuDisplay!\n");
        return ;
    }
    else
    {
        dispara->MyMenuDispalyFunction(dispara);
        return ;
    }
}
/*
// =============================================================================
// ��������:�˵�������Ӧ���������ص��Ǿ�������裩
// �������:operatingDispara ����ǰ���ڲ�������ʾ������para�����������Ҫ�Ĳ���
// ���������
// ����ֵ    :���¹�����ʾ�������ȷ�˵�½�һ���˵���ʾ���������߷��ص����˵���
// ˵��         : ���ø���ʾ��������ʾ����������ʾ����Ȼ��ĺ���ָ��Ϊ�յĻ��������κη�Ӧ��
//          �����㱾��ϣ������0000������������������ǵ��¡�
//          ���ص���ʾ����Ӧ�úܺúñ��棬��Ҫ�����޸ģ���Ϊ�´ε���ʱ�õľ��������Ĳ���
//          �ú�����Ե��Ǿ�������衣�ȷ�˵���̻�������ý�飬�����ڳ�ʼ����ʱ��װ�Ķ�Ӧ�����衣
// =============================================================================
struct menu_displaypara*  menuperipheral_respond(\
        struct menu_displaypara *operatingDispara, void* para )
{
    if((NULL == operatingDispara)||\
            (NULL == operatingDispara->MyMenuDispalyFunction))
    {
        printf("Invalid parameter----menuperipheral_respond!\n");
        return operatingDispara;
    }
    else
    {
        operatingDispara = operatingDispara->MyDisplayPeripheralRespond(\
                operatingDispara,para);
        return operatingDispara;
    }
}
*/
// =============================================================================
// ��������:ִ�в˵���ʾ�����м�����Ӧ����
// �������:operatingDispara ����ǰ���ڲ�������ʾ������para�����������Ҫ�Ĳ���
// ���������
// ����ֵ :���¹�����ʾ�������ȷ�˵�½�һ���˵���ʾ���������߷��ص����˵���
// ˵��: ���ø���ʾ��������ʾ����������ʾ����Ȼ��ĺ���ָ��Ϊ�յĻ��������κη�Ӧ��
//          �����㱾��ϣ������0000������������������ǵ��¡�
//          ���ص���ʾ����Ӧ�úܺúñ��棬��Ҫ�����޸ģ���Ϊ�´ε���ʱ�õľ��������Ĳ���
//          �ú�����Ե��Ǿ�������衣�ȷ�˵���̻�������ý�飬�����ڳ�ʼ����ʱ��װ�Ķ�Ӧ�����衣
// =============================================================================
struct menu_displaypara*  menukeyboard_respond(\
        struct menu_displaypara *operatingDispara, void* para )
{
    if((NULL == operatingDispara)||\
            (NULL == operatingDispara->menukeyboard_action))
    {
        printf("Invalid parameter-menuperipheral_respond!\n");
        return operatingDispara;
    }
    else
    {
        operatingDispara = operatingDispara->menukeyboard_action(\
                operatingDispara,para);
        return operatingDispara;
    }
}

// =============================================================================
// ��������:ִ�в˵���ʾ��������괥������Ӧ����
// �������:operatingDispara ����ǰ���ڲ�������ʾ������para�����������Ҫ�Ĳ���
// ���������
// ����ֵ :���¹�����ʾ�������ȷ�˵�½�һ���˵���ʾ���������߷��ص����˵���
// ˵��: ���ø���ʾ��������ʾ����������ʾ����Ȼ��ĺ���ָ��Ϊ�յĻ��������κη�Ӧ��
//          �����㱾��ϣ������0000������������������ǵ��¡�
//          ���ص���ʾ����Ӧ�úܺúñ��棬��Ҫ�����޸ģ���Ϊ�´ε���ʱ�õľ��������Ĳ���
//          �ú�����Ե��Ǿ�������衣�ȷ�˵���̻�������ý�飬�����ڳ�ʼ����ʱ��װ�Ķ�Ӧ�����衣
// =============================================================================
struct menu_displaypara*  menumousetouch_respond(\
        struct menu_displaypara *operatingDispara, void* para )
{
    if((NULL == operatingDispara)||\
            (NULL == operatingDispara->menumousetouch_ction))
    {
        printf("Invalid parameter----menuperipheral_respond!\n");
        return operatingDispara;
    }
    else
    {
        operatingDispara = operatingDispara->menumousetouch_ction(\
                operatingDispara,para);
        return operatingDispara;
    }
}


// =============================================================================
// ��������:�˵����趯������(ͨ�õİ汾)
// �������:operatingDispara ����ǰ���ڲ�������ʾ������para�����������Ҫ�Ĳ���
// ���������
// ����ֵ :���¹�����ʾ�������ȷ�˵�½�һ���˵���ʾ���������߷��ص����˵���
// ˵��   : ���ø���ʾ��������ʾ����������ʾ����Ȼ��ĺ���ָ��Ϊ�յĻ��������κη�Ӧ��
//          �����㱾��ϣ������0000������������������ǵ��¡�
//          ���ص���ʾ����Ӧ�úܺúñ��棬��Ҫ�����޸ģ���Ϊ�´ε���ʱ�õľ��������Ĳ���
//          һ����Ըú�����Ե���һ���ͨ�õ����裬�ɵ������Ѿ�����������˳�������
// =============================================================================
struct menu_displaypara*  menupeiripheralrespondgeneral(\
        struct menu_displaypara *operatingDispara, void* para )
{
    if((NULL==operatingDispara)||\
            (NULL==operatingDispara->MyPeripheralGeneralAction))
    {
        printf("Invalid para------menupeiripheralrespondgeneral\n");

        return operatingDispara;
    }
    else
    {
        operatingDispara=operatingDispara->MyPeripheralGeneralAction(\
                operatingDispara,para);

        return operatingDispara;
    }
}

