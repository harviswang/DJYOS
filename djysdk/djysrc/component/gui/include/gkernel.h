//----------------------------------------------------
// Copyright (c) 2014, SHENZHEN PENGRUI SOFT CO LTD. All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:

// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
// 3. As a constituent part of djyos,do not transplant it to other software
//    without specific prior written permission.

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
// �����Ȩ�����ʹ���߷��������������������£�����ʹ����ʹ�ü���ɢ����
// �����װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б���
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б��Լ�����
//    ������������
// 3. �������Ϊ�����߲���ϵͳ����ɲ��֣�δ����ǰȡ�õ�������ɣ���������ֲ����
//    �����߲���ϵͳ���������С�

// ����������������Ǳ������Ȩ�������Լ�����������״��"as is"���ṩ��
// �������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
//-----------------------------------------------------------------------------

//����ģ��: gui
//����:  ������.
//�汾��V1.0.0
//�ļ�����: gui kernelӦ�ó���ӿڲ���
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2011-10-07
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#ifndef __GKERNEL_H__
#define __GKERNEL_H__
#include "stdint.h"
#include "rsc.h"
#include "errno.h"
#include "font.h"
#include "charset.h"

#ifdef __cplusplus
extern "C" {
#endif

struct tagDisplayRsc;
struct tagFontRsc;
//λͼ����ɫ�ʸ�ʽ���壬
//�ͻ���ʽ���Կ�driver�ڲ���ʹ���������������˽�и�ʽ��
//Ӧ�ó������ͨ��gk_api_get_pixel_format��ѯ�Կ�ʵ��ʹ�õĸ�ʽ��
//����Կ�ʹ��˽�и�ʽ�����ϲ��ѯ�Կ���ʽʱ������0x8000(CN_CUSTOM_PF).
//Ӧ�ó�����Ƶ㡢�ߡ����ȹ���ʱ������ʹ��cn_sys_pf_e8r8g8b8��ʽ��
//Ӧ�ó������λͼʱ�����ʹ���Կ�ʵ��ʹ�õ����ظ�ʽ����������Ż�����ʾ�ٶȡ�
//ARGBxxxx��ʽ�е�A��֮ΪALPHAͨ����AlphaBlendʱ��ʹ�ø�ALPHAֵ������ʹ��
//  RopCode��Я����Alphaֵ��
//CN_SYS_PF_GRAY1~8�ǻ��ڻ�ɫ�ĻҶ�ͼ����ɫ��struct tagRectBitmap�ṹ�е�
//  ExColor��Ա,ExColor��RGB888��ʽ�ġ���ʵ��ɫ��:�Ҷ�*��ɫ
//CN_SYS_PF_GRAYx��CN_SYS_PF_ALPHAx���������ڣ�������Ҫ����ɫ���롣
//CN_SYS_PF_PALETTE1~8�ǻ��ڵ�ɫ��ģ���ʱstruct tagRectBitmap�ṹ�е�ExColor��
//  ��ɫ��ָ�룬��һ��RGB888��ʽ��ɫ�ʱ�
#define CN_CUSTOM_PF            0x8000              //���λΪ1���Կ��Զ����ʽ
                                                    //bit8~14��������λ��

#define CN_SYS_PF_DISPLAY       (0x0000)            //������display��ͬ��ֻ����
                                                    //��API��ʹ��
#define CN_PF_BITWIDE_MSK       0x7f00              //���ظ�ʽλ������
#define CN_PF_INDEX_MSK         0x00ff              //���ظ�ʽ�������
#define CN_SYS_PF_RGB565        ((16<<8) | 0x00)    //16ɫ��r�ڸ�λ
#define CN_SYS_PF_RGB332        ((8<<8)  | 0x01)    //256ɫ��r�ڸ�λ
#define CN_SYS_PF_RGBE4444      ((16<<8) | 0x02)    //12λɫ,16λ�洢��ռ��12bit��r�ڸ�λ
#define CN_SYS_PF_ERGB4444      ((16<<8) | 0x03)    //12λɫ,16λ�洢��ռռ��12bit��r�ڸ�λ
//5551��ʽ�ܱ�Ť����ʱ���ε�������Ҫʱ�ٸ���ʵ�ʸ�ʽ���ӡ�
//#define CN_SYS_PF_RGBC5551      ((16<<8) | 0x04)    //16λɫ,r�ڸ�λ��cΪrgb���õ�λ
#define CN_SYS_PF_ERGB6666      ((24<<8) | 0x05)    //18λɫ,r�ڸ�λ��24λ�洢����6bit��
#define CN_SYS_PF_E12RGB666     ((32<<8) | 0x06)    //18λɫ,r�ڸ�λ��32λ�洢����14bit��
#define CN_SYS_PF_RGB101010     ((32<<8) | 0x07)    //30λɫ,����λ��
#define CN_SYS_PF_RGB888        ((24<<8) | 0x08)    //24λɫ��r�ڸ��ֽڣ�
#define CN_SYS_PF_ERGB8888      ((32<<8) | 0x09)    //24λɫ��r�ڸ��ֽڣ�����ֽڿ�

#define CN_DISPLAY_PF_LIMIT     0X80                //���ں�λͼר�ø�ʽ��ʼ����
                                                    //��ĸ�ʽ������������ʾ��
#define CN_SYS_PF_GRAY1         ((1<<8)  | 0x83)    //1λ�Ҷ�
#define CN_SYS_PF_GRAY2         ((2<<8)  | 0x84)    //2λ�Ҷ�
#define CN_SYS_PF_GRAY4         ((4<<8)  | 0x85)    //4λ�Ҷ�
#define CN_SYS_PF_GRAY8         ((8<<8)  | 0x86)    //8λ�Ҷ�

#define CN_GKWIN_PF_LIMIT       0xc0                //λͼר�ø�ʽ��ʼ�����µ�
                                                    //��ʽ��ֻ������λͼ
//��ɫ���ʽ������밴����˳����������
#define CN_SYS_PF_PALETTE1      ((1<<8)  | 0xC0)    //1λɫ����ɫ��
#define CN_SYS_PF_PALETTE2      ((2<<8)  | 0xC1)    //2λɫ����ɫ��
#define CN_SYS_PF_PALETTE4      ((4<<8)  | 0xC2)    //4λɫ����ɫ��
#define CN_SYS_PF_PALETTE8      ((8<<8)  | 0xC3)    //8λɫ����ɫ��
#define CN_SYS_PF_ALPHA1        ((1<<8)  | 0xc4)    //1λalpha
#define CN_SYS_PF_ALPHA2        ((2<<8)  | 0xc5)    //2λalpha
#define CN_SYS_PF_ALPHA4        ((4<<8)  | 0xc6)    //4λalpha
#define CN_SYS_PF_ALPHA8        ((8<<8)  | 0xc7)    //8λalpha
#define CN_SYS_PF_ARGB1555      ((16<<8) | 0xc8)    //15λɫ+1λalpha��a�ڸ�λ
#define CN_SYS_PF_RGBA5551      ((16<<8) | 0xc9)    //15λɫ+1λalpha��a�ڸ�λ
#define CN_SYS_PF_ARGB8888      ((32<<8) | 0xca)    //24λɫ+8λalpha��a�ڸ��ֽ�

//��Ԫ��դ������������windows�����һ�£�ֻ��ǰ�����ǰ׺CN
#define CN_R2_BLACK         0   //  0   dest = black����ɫ��
#define CN_R2_NOTMERGEPEN   1   // dpon dest = ~(dest or pen)
#define CN_R2_MASKNOTPEN    2   // dpna dest = ~pen and dest
#define CN_R2_NOTCOPYEN     3   // pn   dest = ~pen
#define CN_R2_MASKPENNOT    4   // pdna dest = ~dest and pen
#define CN_R2_NOT           5   // dn   dest = ~dest
#define CN_R2_XORPEN        6   // dpx  dest = dest xor pen
#define CN_R2_NOTMASKPEN    7   // dpan dest = ~(dest and pen)
#define CN_R2_MASKPEN       8   // dpa  dest = dest and pen
#define CN_R2_NOTXORPEN     9   // dpxn dest = ~(dest xor pen)
#define CN_R2_NOP           10  // d    dest = dest��ԭɫ��
#define CN_R2_MERGENOTPEN   11  // dpno dest = ~pen or dest
#define CN_R2_COPYPEN       12  // p    dest = pen������ɫ��
#define CN_R2_MERGEPENNOT   13  // pdno dest = ~dest or pen
#define CN_R2_MERGEPEN      14  // dpo  dest = dest or pen
#define CN_R2_WHITE         15  //  1   dest = white����ɫ��
#define CN_R2_LAST          15

//�������ģʽ����
#define CN_FILLRECT_MODE_N  0     //ֱ����䣬ֻ��Color0��Ч
#define CN_FILLRECT_MODE_H  1     //ˮƽ��䣬Color0��ʾ�����ɫ��Color1�ұ�
#define CN_FILLRECT_MODE_V  2     //��ֱ��䣬Color0��ʾ�ϱ���ɫ��Color1�±�
#define CN_FILLRECT_MODE_SP 3     //��б��䣬Color0��ʾ���Ͻ���ɫ��Color1���½�
#define CN_FILLRECT_MODE_SN 4     //��б��䣬Color0��ʾ���Ͻ���ɫ��Color1���½�

//������ɫ����
#define CN_COLOR_RED                (0xff<<16)
#define CN_COLOR_GREEN              (0xff<<8)
#define CN_COLOR_BLUE               (0xff)
#define CN_COLOR_BLACK              (0)
#define CN_COLOR_WHITE              (0xffffff)

#define CN_WINBUF_PARENT    0       //�̳и����ڣ���Ϊ���棬��̳�֡����
#define CN_WINBUF_NONE      1       //û�л�����
#define CN_WINBUF_BUF       2       //�л�����

//��ʾ���豸���Ƴ���
#define CN_GET_DISP_SIZE            3   //ȡ��ʾ���ֱ���
#define CN_GET_DISP_FONT            4   //ȡ��ʾ����Ĭ������ָ��
#define CN_GET_DISP_HEIGHT     5   //ȡ�ָ�,���ض��ַ�(����)Ϊ����
#define CN_GET_DISP_WIDTH      6   //ȡ�ֿ�,���ض��ַ�(����)Ϊ����
#define CN_GET_DISP_LINE_HEIGHT     7   //ȡ��������,���и�,
#define CN_GET_DISP_LINE_WIDTH      8   //ȡ��������,������������ʾʱ�õ���,

//������Ϣ����
enum _GK_ERROR_CODE_
{
    EN_GK_NO_ERROR = CN_GK_NO_ERROR,      //û�д���
    EN_GK_GKWIN_LIMIT,                    //��������̫��
    EN_GK_NO_MEMORY,                      //�Դ治��
    EN_GK_BLT_NO_SUPPORT,                  //��֧�ֵ�blt����
    EN_GK_CHARSET_INSTALL_ERROR,          //��װ�ַ�������
    EN_GK_FONT_INSTALL_ERROR,             //��װ�������
};

struct tagPointCdn
{
    s32 x,y;       //��������,��������Ǹ���(������)
};

struct tagRectangle
{
    s32 left,top,right,bottom;      //��������Ǹ���,������right��bottom
};

struct tagRectBitmap        //����λͼ
{
    u32 PixelFormat;        //����ɫ�ʸ�ʽ
    s32 width;              //λͼ�Ŀ��(������Ϊ��λ)
    s32 height;             //λͼ�ĸ߶�(������Ϊ��λ)
    u32 linebytes;          //һ���ö����ֽ�
    ptu32_t ExColor;        //�����ɫ(��pf_type == CN_SYS_PF_GRAY1~8������
                            //  pf_type == CN_SYS_PF_ALPHA1~8)
                            //��ɫ��ָ��(PixelFormat == CN_SYS_PF_PALETTE1~8)
                            //pf_typeȡ����ֵʱ����Ա��Ч
    u8  *bm_bits;           //ָ��洢�������е�����
};

struct tagStAnyBitmap      //������״λͼ
{
    u16 PixelFormat;            //����ɫ�ʸ�ʽ
    u8  *bm_bits;           //ָ��洢�������е�����
    //........����߽����ߣ���˵��
};
//���������
struct tagClipRect
{
    struct tagClipRect *next,*previous;       //�ѿ���������˫��ѭ������
    struct tagRectangle rect;                    //���������λ�úͳߴ硣
};

//���ƺ����У�֧��rop2��alpha��KeyColor͸��
//���������У�ֻ֧��alpha��KeyColor͸������alpha��ʽ��������Ҫdst alpha�������㡣
//byte0: Src Alpha(As),byte1: Dst Alpha(Ad)��
//byte2: alpha���ģʽ����
//byte3: bit31=1��ʾ֧��alpha��bit30=1��ʾ֧��KeyColor͸��,
//bit29=1��ʾ֧��rop2���㣬bit24~27��ʾrop������
//�ر�ע�⣬������˵��alpha����Գ���alpha�ģ�bitmap�е�alphaͨ������Ӱ��
#define CN_ROP_ALPHA_SRC_MSK        0x000000ff  //Src Alpha(As)
#define CN_ROP_ALPHA_SRC_OFF        0
#define CN_ROP_ALPHA_DST_MSK        0x0000ff00  //Dst Alpha(Ad)
#define CN_ROP_ALPHA_DST_OFF        8
#define CN_ROP_ALPHA_MODE_MSK       0x00ff0000  //alpha������㹫ʽ
#define CN_ROP_ALPHA_MODE_OFF       16
#define CN_ROP_ROP2_MSK             0x0f000000  //ROP2���㹫ʽ,�ο�CN_R2_BLACK�峣��
#define CN_ROP_ROP2_OFF             24

//ע:rop2��alpha����ͬʱ֧�֣���������alpha��rop2���Զ�����
#define CN_ROP_ALPHA_EN             0x80000000  //1=ʹ��alpha����
#define CN_ROP_KEYCOLOR_EN          0x40000000  //1=ʹ��KeyColor͸��
#define CN_ROP_ROP2_EN              0x20000000  //1=ʹ��rop2����
//���¶���alpha���㹫ʽ
//�������:bit7=1��ʾ����ʱ��dst���ز��룻bit6=1��ʾ����ʱ��Ad����
#define CN_ALPHA_MODE_AsN           0x00    //dst = S*As+D*(1-As)
#define CN_ALPHA_MODE_AsAdN         0x40    //dst = S*As+D*(1-As)*Ad
#define CN_ALPHA_MODE_AsAd          0x41    //dst = S*As+D*Ad
#define CN_ALPHA_MODE_As            0x80    //dst = S*As
#define CN_ALPHA_MODE_NEED_DST      0x80    //bit7=1,��Ҫdst���ز�������
#define CN_ALPHA_MODE_NEED_AD       0x40    //bit6=1,��Ҫdst alpha��������

#define CN_BOUND_LIMIT      true
#define CN_BOUND_UNLIMIT    false
#define CN_GKWIN_NAME_LIMIT     127

//΢��:ԭ�������˹������뽹��,�����ǵ����Ӧ����Ӧ�ó������,ɾ����.
struct tagGkWinRsc                  //������Դ����
{
    struct tagRscNode node;       //��Դ���
    void *user_private;         //�û����õı�ʶ��ͨ��ָ��һ�����ݽṹ,
                                //���=NULL����ָ��gkwin����
    struct tagGkWinRsc *z_back;   //����Z����������ǰ�����ؿ�
    struct tagGkWinRsc *z_top;
    char win_name[CN_GKWIN_NAME_LIMIT+1];         //��������(����)
//��������У�ÿ�����ڵĿ�����ͨ���˶�������
//���ⴰ���޿�����
//��С���Ĵ����޿�����
//����ȫ�ڵ��Ĵ����޿�����
//���صĴ����޿�����
    struct tagClipRect *visible_clip; //�¿���������,˫��ѭ������
    struct tagClipRect *visible_bak;  //ԭ����������,˫��ѭ������
    struct tagClipRect *redraw_clip;  //��Ҫˢ����ʾ�Ŀ���������
    struct tagClipRect *changed_clip; //���޸ĵ���������
    struct tagClipRect *copy_clip;    //����visible_clip����ʱ����
    struct tagDisplayRsc *disp;       //������������ʾ��
    sfast_t z_prio;             //ǰ�����ȼ���-128~127����ʾ���ֵܴ����е����λ
                                //�ã�0��ʾ�븸winͬ��������0��ʾ����win����
    bool_t direct_screen;       //ֱ��д�����ԣ�������Ϊtrue�������жԱ�win�Ļ�
                                //�Ʋ�����ֱ�ӻ���screen�ϣ������Ƿ���frame
                                //buffer��win buffer��
    bool_t dest_blend;          //true������ʾʱ��Ҫ�ͱ���ɫ��ϣ�false=����Ҫ
    u32 KeyColor;               //͸����ʾ͸��ɫ��rop_code����͸��ɫʱ��,RGB888
    u32 RopCode;               //��դ�������룬��windows����ı�׼��դ�������⣬
                                //������alpha��ϡ�KeyColor͸��
                                //���Ҫ��dst�������㣬�򲻹ܴ�����ʾ�ںδ�����
                                //ȡ�����ڵ���ɫ��alpha������Ҫ�󸸴��ڱ�����
                                //���ڻ��塣
    struct tagRectBitmap changed_msk;//������Ǵ����еı���д����
                                      //ÿbit����8*8����
    struct tagRectBitmap *wm_bitmap; //����ʵ��(��Ϊ���Σ���������Ϊ������״)
//    struct tagRectBitmap *Rop4Msk;//λ���룬ÿλһ�����أ�ֻ��֧��rop4����Ҫ
//    u8 *pat_buf;                //patternλͼ��������֧��rop����
//                                //���ظ�ʽ����ʾ�����ظ�ʽ��ͬ��
    ufast_t change_flag;        //��ȡֵ:cn_gkwin_change_none��
    bool_t bound_limit;         //true=��ʾ��Χ�����ڸ����ڱ߽��ڣ��κ���ʾ����
                                //ֱ�ӷ�������Ĵ��ڣ��ض�����
                                //false=���ܸ����ڱ߽�����,����������������޵ģ�
                                //���������游����
    s32 absx0,absy0;            //���ھ���λ�ã����������screen������ԭ�㣬
    s32 left,top,right,bottom;  //���ھ��Σ�����ڸ����ڣ���������

    s32 limit_left,limit_top; //�����ȴ����и���x�߽磬����ڸ�����
    s32 limit_right,limit_bottom; //�����ȴ����и���y�߽磬����ڸ�����

};

bool_t GK_ApiCreateDesktop(struct tagDisplayRsc *display,
                                        struct tagGkWinRsc *desktop,
                                        s32 width,s32 height,u32 color,
                                        u32 buf_mode,u16 PixelFormat,
                                        u32 BaseColor);
struct tagGkWinRsc *GK_ApiGetDesktop(char *display_name);
bool_t GK_ApiCreateGkwin(struct tagGkWinRsc *parent,
                                        struct tagGkWinRsc *newwin,
                                        s32 left,s32 top,s32 right,s32 bottom,
                                        u32 color,u32 buf_mode,
                                        char *name,u16 PixelFormat,u32 KeyColor,
                                        u32 BaseColor,u32 RopCode);
void GK_ApiFillWin(struct tagGkWinRsc *gkwin,u32 color,u32 sync_time);
void GK_ApiFillRect(struct tagGkWinRsc *gkwin,struct tagRectangle *rect,
                            u32 Color0,u32 Color1,u32 Mode,u32 sync_time);
void GK_ApiSyncShow(u32 sync_time);
void GK_ApiDrawText(struct tagGkWinRsc *gkwin,
                    struct tagFontRsc *pFont,
                    struct tagCharset *pCharset,
                    s32 x,s32 y,
                    const char *text,u32 count,u32 color,
                    u32 Rop2Code,u32 SyncTime);
void GK_ApiSetPixel(struct tagGkWinRsc *gkwin,s32 x,s32 y,
                        u32 color,u32 rop2_code,u32 sync_time);
void GK_ApiLineto(struct tagGkWinRsc *gkwin, s32 x1,s32 y1,
                    s32 x2,s32 y2,u32 color,u32 rop2_code,u32 sync_time);
void GK_ApiLinetoIe(struct tagGkWinRsc *gkwin, s32 x1,s32 y1,
                    s32 x2,s32 y2,u32 color,u32 rop2_code,u32 sync_time);
void GK_ApiDrawBitMap(struct tagGkWinRsc *gkwin,
                                struct tagRectBitmap *bitmap,
                                s32 x,s32 y,
                                u32 KeyColor,u32 RopCode,u32 SyncTime);
void GK_ApiDrawCircle(struct tagGkWinRsc *gkwin,s32 x0,s32 y0,
                    u32 r,u32 color,u32 rop2_code,u32 sync_time);
void GK_ApiDrawBezier(struct tagGkWinRsc *gkwin,float x1,float y1,
                    float x2,float y2,float x3,float y3,float x4,float y4,
                    u32 color,u32 rop2_code,u32 sync_time);
void GK_ApiMoveWin(struct tagGkWinRsc *gkwin,s32 left,s32 top,u32 sync_time);
void GK_ApiChangeWinArea(struct tagGkWinRsc *gkwin, s32 left,s32 top,
                                       s32 right,s32 bottom, u32 SyncTime);
void GK_ApiSetBoundMode(struct tagGkWinRsc *gkwin,bool_t mode);
void GK_ApiSetPrio(struct tagGkWinRsc *gkwin,sfast_t prio,u32 sync_time);
bool_t GK_ApiSetRopCode(struct tagGkWinRsc *gkwin,
                            u32 RopCode,u32 sync_time);
bool_t GK_ApiSetPatBuf(struct tagGkWinRsc *gkwin,
                            struct tagRectBitmap *pattern,
                            u32 sync_time);
bool_t GK_ApiSetTransparentColor(struct tagGkWinRsc *gkwin,
                                    u32 transparentcolor);
bool_t GK_ApiSetRop4Msk(struct tagGkWinRsc *gkwin,
                                struct tagRectBitmap *Rop4Msk,u32 sync_time);
void GK_ApiSetDirectScreen(struct tagGkWinRsc *gkwin,
                                                u32 sync_time);
void GK_ApiCancelDirectScreen(struct tagGkWinRsc *gkwin,
                                                u32 sync_time);
void GK_ApiDestroyWin(struct tagGkWinRsc *gkwin);
u16 GK_ApiGetPixelFormat(struct tagDisplayRsc *display);

u32 GK_ConvertPF2PF(u16 spf_type,u16 dpf_type,u32 color,ptu32_t ExColor);
u32 GK_BlendRop2(u32 dest,u32 pen,u32 rop2_code);
u32 GK_ConvertColorToRGB24(u16 PixelFormat,u32 color,ptu32_t ExColor);
u32 GK_ConvertRGB24ToPF(u16 PixelFormat,u32 color);

u32 GK_GetPixelBm(struct tagRectBitmap *bitmap,s32 x,s32 y);

#ifdef __cplusplus
}
#endif

#endif //__GKERNEL_H__

