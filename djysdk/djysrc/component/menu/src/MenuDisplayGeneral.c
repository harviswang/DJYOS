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
// ģ������: DJY�˵�ģ��֮��ʾ����֮ͨ����ʾ�����Ķ���
// ģ��汾: V1.00(��ʼ���汾)
// ������Ա: zqf
// ����ʱ��: 20121224
// =======================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// =======================================================================
#include "MenuDisplayGeneral.h"
#include "MenuSpaceGeneral.h"
#include "MenuitemApi.h"

#include <math.h>//����ָ�������ȣ�pow(a,b),a��b�η�

//////////////////////////////////////////////////////////////////////////
////////////////////////////�ڲ�ʹ�ú���,�������������
//////////////////////////////////////////////////////////////////////////

 // =========================================================================
 // ��������:�Բ˵�����л�����
 // �������:���л������ʾ����
 // ˵��          :����Χ������ʾ����ͬ��һ������(��ֱ��ʾ��ʱ��)
 // =========================================================================
static bool_t DrawMenuLayerBox_V(struct menu_displaypara*  dispara)
 {
    bool_t result=false;
    u8   num_item=0;
    struct tagRectangle temp_rec={0,0,0,0};
    struct tagGkWinRsc  *win;

    win=dispara->CurWin;
    num_item=CalDisBettween2Menuitem(dispara->FVisualMenuitem, dispara->LVisualMenuitem)+1;
    temp_rec.left=cn_space_margin/4;
    temp_rec.top=cn_space_margin/4;
    temp_rec.bottom=(dispara->MenuitemSpace.height)*num_item+cn_space_margin/2;
    temp_rec.right=dispara->MenuitemSpace.width+cn_space_margin/2;

    GK_ApiLineto(win, temp_rec.left, temp_rec.top, temp_rec.right, temp_rec.top,\
                  CN_COLOR_BLACK, CN_R2_COPYPEN, 0);
    GK_ApiLineto(win, temp_rec.left, temp_rec.bottom, temp_rec.right, temp_rec.bottom,\
                  CN_COLOR_BLACK, CN_R2_COPYPEN, 0);
    GK_ApiLineto(win, temp_rec.left, temp_rec.top, temp_rec.left, temp_rec.bottom,\
                   CN_COLOR_BLACK, CN_R2_COPYPEN,0);
    GK_ApiLineto(win, temp_rec.right, temp_rec.top, temp_rec.right, temp_rec.bottom,\
                   CN_COLOR_BLACK, CN_R2_COPYPEN, CN_TIMEOUT_FOREVER);

     result=true;

    return result;
 }

  // =========================================================================
  // ��������:�Բ˵�����л�����
  // �������:���л������ʾ����
  // ˵��          :����Χ������ʾ����ͬ��һ������(ˮƽ��ʾ��ʱ��)
  // =========================================================================
 static bool_t DrawMenuLayerBox_H(struct menu_displaypara*  dispara)
  {
    bool_t result=false;
    u8   num_item=0;
    struct tagRectangle temp_rec={0,0,0,0};
    struct tagGkWinRsc  *win;

    win=dispara->CurWin;
    num_item=CalDisBettween2Menuitem(dispara->FVisualMenuitem, dispara->LVisualMenuitem)+1;
    temp_rec.left=cn_space_margin/4;
    temp_rec.top=cn_space_margin/4;
    temp_rec.bottom=dispara->MenuitemSpace.height+cn_space_margin/2;
    temp_rec.right=(dispara->MenuitemSpace.width)*num_item+cn_space_margin/2;
    GK_ApiLineto(win, temp_rec.left, temp_rec.top, temp_rec.right, temp_rec.top,\
                  CN_COLOR_BLACK, CN_R2_COPYPEN, 0);
    GK_ApiLineto(win, temp_rec.left, temp_rec.bottom, temp_rec.right, temp_rec.bottom,\
                  CN_COLOR_BLACK, CN_R2_COPYPEN, 0);
    GK_ApiLineto(win, temp_rec.left, temp_rec.top, temp_rec.left, temp_rec.bottom,\
                  CN_COLOR_BLACK, CN_R2_COPYPEN,0);
    GK_ApiLineto(win, temp_rec.right, temp_rec.top, temp_rec.right, temp_rec.bottom,\
                  CN_COLOR_BLACK, CN_R2_COPYPEN, CN_TIMEOUT_FOREVER);

    result=true;
    return result;
  }
  // =========================================================================
  // ��������:�Բ˵�����л�����
  // �������:���л������ʾ����
  // ˵��          :����Χ������ʾ����ͬ��һ������(ˮƽ��ʾ��ʱ��)
  // =========================================================================
static bool_t DrawMenuLayerBox_3by3(struct menu_displaypara*  dispara)
 {
       bool_t result=false;
       struct tagRectangle temp_rec={0,0,0,0};
       struct tagGkWinRsc  *win;

      win=dispara->CurWin;
      temp_rec.left=cn_space_margin/4;
      temp_rec.top=cn_space_margin/4;
      temp_rec.bottom=dispara->CurWin->bottom-dispara->CurWin->top-cn_space_margin/4;
      temp_rec.right=dispara->CurWin->right-dispara->CurWin->left-cn_space_margin/4;

      GK_ApiLineto(win, temp_rec.left, temp_rec.top, temp_rec.right, temp_rec.top,\
                    CN_COLOR_BLACK, CN_R2_COPYPEN, 0);
      GK_ApiLineto(win, temp_rec.left, temp_rec.bottom, temp_rec.right, temp_rec.bottom,\
                      CN_COLOR_BLACK, CN_R2_COPYPEN, 0);
      GK_ApiLineto(win, temp_rec.left, temp_rec.top, temp_rec.left, temp_rec.bottom,\
                      CN_COLOR_BLACK, CN_R2_COPYPEN,0);
      GK_ApiLineto(win, temp_rec.right, temp_rec.top, temp_rec.right, temp_rec.bottom,\
                     CN_COLOR_BLACK, CN_R2_COPYPEN, CN_TIMEOUT_FOREVER);
      result=true;

    return result;
 }

// ===================================================================================================
// ��������:���ڰ׵���ת��Ϊ32λ���ɫͼ
// �������:src,����Դ��count������Դ���ֽ�����data1��1��Ӧ����ֵ��data0��0��Ӧ����ֵ
// ���������dest��ת���������
// ����ֵ  :
// ˵��    :�ڲ�ʹ�ú�������ͼ�����ݣ�BIT�������У�ת��Ϊ���ɫ��������
// ==================================================================================================
static void changeB2I(u8* dest, u8* src, u32 count, u32 data1, u32 data0)
{
   u8 i=0,j=0;
   u8  data;
   for(i=0;i<count;i++)//�ֽ���
   {
      data=*((u8 *)(src+i));
      for(j=0;j<8;j++)//�Լ�λ
      {
         if(data&(1<<(7-j)))
         {
           *((u32 *)(dest+i*32+j*4))=data1;
         }
         else
         {
             *((u32 *)(dest+i*32+j*4))=data0;
         }
      }
    }
}

// =========================================================================
// �������ܣ����ı����յ�ǰʹ�õ�����д����Ӧ�Ĵ���λ����
// �������:para��д�ı��������������������ɫ����Ϣ��text���ı��ַ���
// ���������
// ����ֵ  :
// ˵��    :�ڲ�ʹ�ú���
// =========================================================================
static void  draw_win_text(struct draw_wintext_para *para,const char *text)//ȷ��
{

    s32 len, src_len,size,size_bak;
    u32 wc;
    s32 colortext; //������ɫ
    s32 colorback;  //������ɫ  int k=0;
    s32  colorsize=32*16;
    struct tagFontRsc* cur_font;
    u8       *colorbuf;//�洢�����ַ���������
    struct tagGkscParaDrawBitmapRop bitmap_para;
    struct tagGkscParaDrawBitmapRop mpara;
    u8 buf[128],*dbuf;  //����һ���㹻��32�����ֵĻ������������Ҫ��ʾ���ַ�
                        //�����˵��󣬾���Ҫ��̬����

    colortext=para->colortext;
    colorback=para->colorback;
    cur_font = Font_GetCurFont();
    if(cur_font == NULL)
        return;

    // �����ֽ���
    src_len = para->count;
    bitmap_para.x = para->x;
    bitmap_para.gkwin = para->gkwin;
    bitmap_para.y = para->y;
    bitmap_para.bitmap.bm_bits = NULL;
    dbuf = NULL;
    size_bak = 0;

    colorbuf=M_MallocLc(32*32,0);
    memset(colorbuf,0,colorsize);

    for(; src_len > 0;)
    {
        len= mbtowc(&wc, text, src_len);
        if(len == -1)
        { // ��Ч�ַ�
            src_len--;
            text++;
        }else
        { // ��Ч�ַ�
            text += len;
            src_len -= len;
            // ȡ����
            bitmap_para.bitmap.bm_bits = NULL;      //NULL=��ѯbitmap.
            cur_font->GetBitmap(wc,0,0,&(bitmap_para.bitmap));
            size = bitmap_para.bitmap.linebytes * bitmap_para.bitmap.height;

            if((size*32)>colorsize)
            {
              free(colorbuf);
              colorsize=size*32;
              colorbuf=M_MallocLc(colorsize,0);
              memset(colorbuf,0,colorsize);
              if(colorbuf==NULL)
                return;
            }
            if(size < 128)
            {
                bitmap_para.bitmap.bm_bits = buf;
            }
            else
            {
                if(size > size_bak)
                {
                    free(dbuf);
                    dbuf = M_MallocLc(size,0);
                    if(dbuf == NULL)
                        return;
                    size_bak = size;
                }
                bitmap_para.bitmap.bm_bits = dbuf;
            }
            cur_font->GetBitmap(wc,0,0,&(bitmap_para.bitmap));


            changeB2I(colorbuf, bitmap_para.bitmap.bm_bits,\
                      size, colortext, colorback);

             mpara.gkwin=bitmap_para.gkwin;
             mpara.x=bitmap_para.x;
             mpara.y=bitmap_para.y;
             mpara.bitmap.height=bitmap_para.bitmap.height;
             mpara.bitmap.width=bitmap_para.bitmap.width;
             mpara.bitmap.linebytes=32*bitmap_para.bitmap.linebytes;
             mpara.bitmap.PixelFormat=CN_SYS_PF_ERGB8888;
             mpara.bitmap.bm_bits=colorbuf;
             //ÿ����ͼ��Ҫ������ɣ���Ȼ������ڴ汻��д��������ʾ����
             GK_ApiDrawBitMap(mpara.gkwin, &(mpara.bitmap), mpara.x, mpara.y,0,0, CN_TIMEOUT_FOREVER);
             //���ǵ�Ҫ�ͷ��ڴ棬���Ա���ȴ�����Ժ��ٷ��أ��������߳��л���
              bitmap_para.x += bitmap_para.bitmap.width;
        }
    }

    free(colorbuf);
    free(dbuf);
}
// =========================================================================
// �������ܣ����˵���Ľڵ�����д�ڶ�Ӧ�Ĵ��ڵĶ�Ӧλ����
// �������:dispara����ʾ������menu��Ҫд�Ĳ˵���
// ���������
// ����ֵ  :
// ˵��    :�������صı�ʶ����"+"��ʾ���Ӳ˵���"."��ʾû���Ӳ˵�
// =========================================================================
static bool_t DrawMenuitemText(struct menu_displaypara  *dispara,struct menu_item  *menu,s32 x,s32 y)
{

   bool_t result=true;
   char  *apendSon="+";//���Ӳ˵�
   char  *apendNull=".";//���Ӳ˵�
   char content[50];//Ϊ�˲������޸Ĳ˵�����
   struct draw_wintext_para  para;

   content[0]='\0';
   if(NULL!=menu->node.child)//���Ӳ˵�����ӱ�־
   {
      strcat(content,apendSon);
      strcat(content,menu->node.name);
   }
   else
   {
      strcat(content,apendNull);
      strcat(content,menu->node.name);
   }
    //���para�ṹ��
   if(menu==dispara->OperatingMenuitem)
   {
       para.colorback=dispara->DisplayColor.menu_lighted_color;
   }
   else
   {
      para.colorback=dispara->DisplayColor.menu_color;
   }
   para.colortext=dispara->DisplayColor.text_color;
   para.gkwin=dispara->CurWin;
   para.count=strnlen(content,namesize);
   para.line_limit=namesize;
   para.x=x;
   para.y=y;
   para.draw_type=Left2Right;
    //��ɲ˵�����ʾ
   draw_win_text(&para,content);

   return result;
}


////////////////////////////////////////////////////
//////////////������ʾģ�麯��
////////////////////////////////////////////////////
// =========================================================================
// �������ܣ�ˮƽ��ʾ����
// �������:dispara������ʾ����ʾ����
// ���������
// ����ֵ    :
// ˵��          :��dispra�еĲ˵�����ͬ����С��ˮƽ��ʾ����Ӧ�Ĵ����ϣ���ʾ��ʱ���Ǵ�
//         dispara��Fvisual��Lvisual��ʹ���ߵ��������ĺ�������������ӵĵ�һ���˵�
//         �����һ���˵�
// =========================================================================
bool_t display_menu_horizontal(struct menu_displaypara * dispara)//����д�˵����ݺ����ͻ���������ɲ˵�����ʾ
{
      bool_t  result=false;
      s32     x,y;                //�洢д�ֵ�λ��
      struct menu_item   *temp;//��ʱmenuitem����
      struct tagRectangle    LightedRec;

      if(NULL==dispara->FVisualMenuitem)
      {
          dispara->FVisualMenuitem=dispara->OperatingMenuitem;
      }
      if(NULL==dispara->LVisualMenuitem)
      {
          dispara->LVisualMenuitem=dispara->OperatingMenuitem;
      }
      temp=dispara->FVisualMenuitem;
      x=cn_space_margin/2;
      y=cn_space_margin/2;
          //����Ҫ������������ɫ��ԭ,��Ӧ��������������Ȼ���˷���Դ�ģ�ÿ�ζ�Ҫˢ����̫�����ˣ�
    //  GK_ApiFillWin(dispara->CurWin, dispara->DisplayColor.menu_color, CN_TIMEOUT_FOREVER);
      do{
          //������ɨ��ǰѩ��ÿ��menuitem��д�붼Ҫ���һ��
          LightedRec.left=x,
          LightedRec.top=y;
          LightedRec.right=x+dispara->MenuitemSpace.width;
          LightedRec.bottom=y+dispara->MenuitemSpace.height;
          if(temp==dispara->OperatingMenuitem)//������ǰ������
          {
              //���ʱ����Ҫ�л��¼��������д�ֺ����������¼���ֱ��д��
              GK_ApiFillRect(dispara->CurWin, &LightedRec,
                           dispara->DisplayColor.menu_lighted_color,
                           0,CN_FILLRECT_MODE_N,0);
          }
          else
          {
              GK_ApiFillRect(dispara->CurWin, &LightedRec,
                             dispara->DisplayColor.menu_color,
                             0,CN_FILLRECT_MODE_N,0);
          }

          DrawMenuitemText(dispara, temp,x,y);

          x=x+dispara->MenuitemSpace.width;//ˮƽ��ʾֻ��Ҫ����x����
          temp=GetNextMenuitem(temp);
      }while(temp!=GetNextMenuitem(dispara->LVisualMenuitem));

      //draw the layers box
      DrawMenuLayerBox_H(dispara);
      result=true;

      return result;
}


// =========================================================================
// �������ܣ���ֱ��ʾ����
// �������:dispara������ʾ����ʾ����
// ���������
// ����ֵ  :
// ˵��    :��dispra�еĲ˵�����ͬ����С�Ĵ�ֱ��ʾ����Ӧ�Ĵ����ϣ���ʾ��ʱ���Ǵ�
//         dispara��Fvisual��Lvisual��ʹ���ߵ��������ĺ�������������ӵĵ�һ���˵�
//         �����һ���˵�
// =========================================================================
//��ֱ��ʾͬ��˵�
bool_t display_menu_vertical(struct menu_displaypara * dispara)//����д�˵����ݺ����ͻ���������ɲ˵�����ʾ
{
      bool_t  result=false;
      s32     x,y;                //�洢д�ֵ�λ��
      struct menu_item   *temp;//��ʱmenuitem����
      struct tagRectangle    LightedRec;

      if(NULL==dispara->FVisualMenuitem)
      {
          dispara->FVisualMenuitem=dispara->OperatingMenuitem;
      }
      if(NULL==dispara->LVisualMenuitem)
      {
          dispara->LVisualMenuitem=dispara->OperatingMenuitem;
      }
      //Draw the menuitem text
      temp=dispara->FVisualMenuitem;
      x=cn_space_margin/2;
      y=cn_space_margin/2;
    //����Ҫ������������ɫ��ԭ,��Ӧ��������������Ȼ���˷���Դ�ģ�ÿ�ζ�Ҫˢ����̫�����ˣ�
    //  GK_ApiFillWin(dispara->CurWin, dispara->DisplayColor.menu_color, CN_TIMEOUT_FOREVER);
      do{
          LightedRec.left=x,
          LightedRec.top=y;
          LightedRec.right=x+dispara->MenuitemSpace.width;
          LightedRec.bottom=y+dispara->MenuitemSpace.height;
          if(temp==dispara->OperatingMenuitem)//������ǰ������
          {
              //���ʱ����Ҫ�л��¼��������д�ֺ����������¼���ֱ��д��
              GK_ApiFillRect(dispara->CurWin, &LightedRec,
                             dispara->DisplayColor.menu_lighted_color,
                             0,CN_FILLRECT_MODE_N,0);
          }
          else
          {
              GK_ApiFillRect(dispara->CurWin, &LightedRec,
                             dispara->DisplayColor.menu_color,
                             0,CN_FILLRECT_MODE_N,0);
          }
          DrawMenuitemText(dispara, temp,x,y);

          y=y+dispara->MenuitemSpace.height;//��ֱ��ʾֻ��Ҫ����y����
          temp=GetNextMenuitem(temp);

      }while(temp!=GetNextMenuitem(dispara->LVisualMenuitem));
        //draw the layers box
      DrawMenuLayerBox_V(dispara);
      result=true;

  return result;
}

//������ʾģʽ����
 // =========================================================================
 // �������ܣ��Ÿ���ʾ����
 // �������:dispara������ʾ����ʾ����
 // ���������
 // ����ֵ    :
 // ˵��          :��dispra�еĲ˵�����ͬ����С�ķ�������ʾ����Ӧ�Ĵ����ϵ�����λ�ã���ʾ��ʱ���Ǵ�
 //         dispara��Fvisual��Lvisual��ʹ���ߵ��������ĺ�������������ӵĵ�һ���˵�
 //         �����һ���˵�
 // =========================================================================

//�����ҡ����ϵ��µ����9����ʾ
bool_t display_menu_3by3(struct menu_displaypara * dispara)//����д�˵����ݺ����ͻ���������ɲ˵�����ʾ
{
  bool_t  result=false;
  u8      k=0;                //���ڱ�ǲ��
  s32     x,y;                //�洢д�ֵ�λ
  s32     x0,y0;              //��¼��ʼ��ʾ��ԭ��
  struct menu_item   *temp;//��ʱmenuitem����
  struct tagRectangle    LightedRec;//����������


  if(NULL==dispara->FVisualMenuitem)
  {
     dispara->FVisualMenuitem=dispara->OperatingMenuitem;
  }
  if(NULL==dispara->LVisualMenuitem)
  {
      dispara->LVisualMenuitem=dispara->OperatingMenuitem;
  }
  temp=dispara->FVisualMenuitem;

  //����Ҫ������������ɫ��ԭ
  GK_ApiFillWin(dispara->CurWin, dispara->DisplayColor.menu_color, CN_TIMEOUT_FOREVER);

  x0=(dispara->CurWin->right-dispara->CurWin->left-cn_space_margin)/2-(dispara->MenuitemSpace.width*3)/2;
  y0=(dispara->CurWin->bottom-dispara->CurWin->top-cn_space_margin)/2-(dispara->MenuitemSpace.height*3)/2;
  x=x0;
  y=y0;
  do{
      LightedRec.left=x;
      LightedRec.top=y;
      LightedRec.right=x+dispara->MenuitemSpace.width;
      LightedRec.bottom=y+dispara->MenuitemSpace.height;
      if(temp==dispara->OperatingMenuitem)//������ǰ������
      {
          //���ʱ����Ҫ�л��¼��������д�ֺ����������¼���ֱ��д��
          GK_ApiFillRect(dispara->CurWin, &LightedRec,
                         dispara->DisplayColor.menu_lighted_color,
                         0,CN_FILLRECT_MODE_N,0);
      }
      else
      {
          GK_ApiFillRect(dispara->CurWin, &LightedRec,
                         dispara->DisplayColor.menu_color,
                         0,CN_FILLRECT_MODE_N,0);
      }
      result= DrawMenuitemText(dispara, temp,x,y);
      k++;
      if(9==k)//��ʾ��Ÿ�
      {
        break;//����ѭ��������Ĳ�����ʾ
      }
      if(0==(k%3))//����
      {
        y=y+dispara->MenuitemSpace.height+cn_space_margin/2;//
        x=x0;
      }
      else
      {
         x=x+dispara->MenuitemSpace.width+cn_space_margin/2;//
      }
      temp=GetNextMenuitem(temp);
  }while(temp!=GetNextMenuitem(dispara->LVisualMenuitem));
  //����Ҫ��ʾ9�������಻����ȻҲ�Ǵ�FV��ʼ��LV����
  //draw the layers box
  DrawMenuLayerBox_3by3(dispara);
  result=true;

  return result;
}
// =========================================================================
// ��������:ִ����ʾ�����Ĺ��Ӻ���
// �������:dispara����ǰ�����Ĳ˵���Ĳ˵���ʾ����
// ���������
// ����ֵ    :
// ˵��          :ִ�в˵���ʾ�����еĵ�ǰ�����Ĳ˵���Ĺ��Ӻ���
// =========================================================================
//ִ�в˵��ص�����
struct menu_displaypara *exe_menuitemfunc(struct menu_displaypara *operatingDispara)
{
 //  u8      dismode=0;//��ʾģʽ
   struct menu_item  *OperatingMenuitem=NULL;
   struct menu_displaypara  *pMainmenuDispara=NULL;
   struct menu_item  *FirstMenuitem=NULL;//�����˵��ĵ�һ���˵�����ڻָ�ʱ��
   struct tagGkWinRsc  *Appwin=NULL;

   ptu32_t (*menu_init_function)(struct menu_item *FirstMenuitem, struct tagGkWinRsc *Appwin);

   if(NULL==operatingDispara)
   {
      printf("Invalid parameter-----exe_menuitemfunc\n");
      return operatingDispara;
   }
   OperatingMenuitem=operatingDispara->OperatingMenuitem;
   OperatingMenuitem->myprivate=operatingDispara->AppWin;

   if((NULL==OperatingMenuitem)||(NULL==OperatingMenuitem->MenuitemFunction))
   {
      printf("fatal error!--exe_menuitemfunc!\n");
      return operatingDispara;
   }

   operatingDispara=getmainmenudispara(operatingDispara);
   //������صļ�¼�����ڻָ�
   FirstMenuitem=GetBBmenuitem(operatingDispara->OperatingMenuitem);
   Appwin=operatingDispara->AppWin;
   menu_init_function=operatingDispara->MyMenuInitFunction;
  // dismode=operatingDispara->dismode;

   //ɾ�������˵���ִ�й��Ӻ���
   deldisplaypara(operatingDispara);
   operatingDispara=NULL;
   OperatingMenuitem->MenuitemFunction(OperatingMenuitem);//ִ�й��Ӻ���

   //�ָ����˵�
   operatingDispara=menu_init_function(FirstMenuitem,Appwin);
   printf("recover main:mem=%08x  size=%08x\n",operatingDispara,sizeof(struct menu_displaypara));
   if(NULL==operatingDispara)
   {
       printf("Failed to recover the main menu!----exe_menuitemfunc\n");
   }
   else
   {
      if(NULL==operatingDispara->MyMenuDispalyFunction)
      {
          printf("Failed to recover the main menu!----exe_menuitemfunc\n");
      }
      else
      {
         // operatingDispara->MyMenuDispalyFunction(operatingDispara);
      }
   }

   return operatingDispara;

}
// =========================================================================
// �������ܣ����ǵ��ж�����ʾ������ı���ʾ����
// �������:
// ���������
// ����ֵ :
// ˵��   :�������ð汾��
// =========================================================================
void  my_draw_text(struct draw_wintext_para *para,const char *text)//ȷ��
{
    struct tagGkscParaDrawBitmapRop bitmap_para;
    s32 len, src_len,size,size_bak;
    u32 wc;
    struct tagFontRsc* cur_font;
    u8 buf[128],*dbuf;  //����һ���㹻��32�����ֵĻ������������Ҫ��ʾ���ַ�
                        //�����˵��󣬾���Ҫ��̬����
//�޸ĵĲ���
    u8       *colorbuf;//�洢�����ַ���������
    struct tagGkscParaDrawBitmapRop mpara;
    s32 colortext; //������ɫ
    s32 colorback;  //������ɫ
    s32 k=0;
    s32  colorsize=32*16;//�洢����32λ���ɫͼ��
    u32  number=0;

    colortext=para->colortext;
    colorback=para->colorback;

    cur_font = Font_GetCurFont();
    if(cur_font == NULL)
        return;

    // �����ֽ���
    src_len = para->count;

    bitmap_para.x = para->x;
    bitmap_para.gkwin = para->gkwin;
    bitmap_para.y = para->y;
    bitmap_para.bitmap.bm_bits = NULL;
    dbuf = NULL;
    size_bak = 0;

    colorbuf=M_MallocLc(colorsize,0);
    memset(colorbuf,0,colorsize);


    for(; src_len > 0;)
    {
       // printf("------------src_len=%d-------------\n",src_len);
       number++;
        len= mbtowc(&wc, text, src_len);
        if(len == -1)
        { // ��Ч�ַ�
            src_len--;
            text++;
        }else
        { // ��Ч�ַ�
            text += len;
            src_len -= len;

            // ȡ����
            bitmap_para.bitmap.bm_bits = NULL;      //NULL=��ѯbitmap.
            cur_font->GetBitmap(wc,0,0,&(bitmap_para.bitmap));
            size = bitmap_para.bitmap.linebytes * bitmap_para.bitmap.height;

           if((size*32)>colorsize)
            {
              free(colorbuf);
              colorsize=size*32;
              colorbuf=M_MallocLc(colorsize,0);

              if(colorbuf==NULL)
                return;
              memset(colorbuf,0,colorsize);
           }

            if(size < 128)
            {
                bitmap_para.bitmap.bm_bits = buf;
            }
            else
            {
                if(size > size_bak)
                {
                    free(dbuf);
                    dbuf = M_MallocLc(size,0);
                    if(dbuf == NULL)
                        return;
                    size_bak = size;
                }
                bitmap_para.bitmap.bm_bits = dbuf;
            }
            cur_font->GetBitmap(wc,0,0,&(bitmap_para.bitmap));
            changeB2I(colorbuf, bitmap_para.bitmap.bm_bits,\
                      size, colortext, colorback);
            mpara.gkwin=bitmap_para.gkwin;
            //�����ǲ���Ҫ����
            if((number%44)==0)
            {
              bitmap_para.x=para->x;
              bitmap_para.y=bitmap_para.y+16;
              printf("x=%d  y=%d",bitmap_para.x,bitmap_para.y);
              printf("����\n");
            }
             //����д�ֵķ�ʽ��ͬ
            switch(para->draw_type)
            {
             case 1://������
                   mpara.x=bitmap_para.x;
                   mpara.y=bitmap_para.y;
                   bitmap_para.x += bitmap_para.bitmap.width;
                   break;
             case 2://��������
                   mpara.x=bitmap_para.x-bitmap_para.bitmap.width;
                   mpara.y=bitmap_para.y;
                   bitmap_para.x -= bitmap_para.bitmap.width;
                   break;
              default://������
                   mpara.x=bitmap_para.x;
                   mpara.y=bitmap_para.y;
                   bitmap_para.x += bitmap_para.bitmap.width;
                   break;
             }
             mpara.bitmap.height=bitmap_para.bitmap.height;
             mpara.bitmap.width=bitmap_para.bitmap.width;
             mpara.bitmap.linebytes=32*bitmap_para.bitmap.linebytes;
             mpara.bitmap.PixelFormat=CN_SYS_PF_ERGB8888;
             mpara.bitmap.bm_bits=colorbuf;
             //���ǵ�Ҫ�ͷ��ڴ棬���Ա���ȴ�����Ժ��ٷ��أ��������߳��л���
             GK_ApiDrawBitMap(mpara.gkwin, &(mpara.bitmap), mpara.x, mpara.y,0,0, CN_TIMEOUT_FOREVER);
        }
    }
    free(colorbuf);//
    free(dbuf);
    return;
 //   printf("Gk_api_draw_mtext end!\n");
}
////////////////////////////////////////////////////////////////////////////////////////
////////ʵ�ֻ�������ʾ����������չ�����л��˵�����أ�
////////////////////////////////////////////////////////////////////////////////////////
// =========================================================================
// ��������:������ʾ����֮��ʾ�Ӳ˵�
// �������:operatingDispara����ǰ���ڲ�������ʾ����
// ���������
// ����ֵ     :�Ӳ˵�����ʾ����
// ˵��          :���ڷ���ֵ���û�ע�Ᵽ�桢��顢�͸��£����û���Ӳ˵������û�з�Ӧ
// =========================================================================
struct menu_displaypara *goto_submenu(struct menu_displaypara *operatingDispara)
{
    struct menu_displaypara   *temp;

    //�������
    if((NULL==operatingDispara)||(NULL==operatingDispara->OperatingMenuitem)||(NULL==operatingDispara->CurWin))
    {
         printf("Invalid parameter----goto_submenu\n");
         return operatingDispara;
    }
    if(NULL==GetSonMenuitem(operatingDispara->OperatingMenuitem))
    {
      printf("No sub menu ---goto_submenu\n");
      return operatingDispara;
    }
    temp=operatingDispara->CreateMenuDisparaBypre(operatingDispara, NULL, NULL);
    if(NULL==temp)
    {
      printf("Failed --goto_submenu\n");
      return operatingDispara;
    }
    //��������
    temp->AdjustVisualMenuitem(temp);
    //��ʾ
    GK_ApiFillWin(temp->CurWin, operatingDispara->DisplayColor.menu_color, 0);
    temp->MyMenuDispalyFunction(temp);

    return temp;
}
// =========================================================================
// ��������:������ʾ����֮���ص����˵�
// �������:operatingDispara����ǰ���ڲ�������ʾ����
// ���������
// ����ֵ     :���˵�����ʾ����
// ˵��          :���ڷ���ֵ���û�ע�Ᵽ�桢��顢�͸��£��ò�����ɾ����ǰ����������ʾ����
// =========================================================================
struct menu_displaypara *goto_parmenu(struct menu_displaypara *operatingDispara)
{
    struct menu_displaypara *temp;
    struct menu_displaypara *pMainmenuDispara;

    //�������
    if(NULL==operatingDispara)
    {
         printf("Invalid parameter--- goto_parmenu!\n");
         return operatingDispara;
    }
    if(NULL==operatingDispara->pre)//���˵������ٷ���
    {
        printf("MainMenu----goto_parmenu\n");
        return operatingDispara;
    }
    else//�Ӳ˵�
    {
        temp=operatingDispara;
        operatingDispara=operatingDispara->pre;
        operatingDispara->next=NULL;
//        GK_ApiFillWin(temp->CurWin, temp->DisplayColor.menu_color, CN_TIMEOUT_FOREVER);
        deldisplaypara(temp);
//��Ϊ�Ӳ˵����ܵ�ס�˺ุܶ�˵�������Ҫ���λظ����еĲ˵�������Ŀǰ���࣬���Ǻ�����������Ҫ--TODO
//��Ϊ�ǻָ��˵������Դ�ʱ��û�б�Ҫ�����˵���ġ�
        pMainmenuDispara=getmainmenudispara(operatingDispara);
        temp=pMainmenuDispara;
        while(temp!=operatingDispara)
        {
            GK_ApiFillWin(temp->CurWin, operatingDispara->DisplayColor.menu_color, 0);
            temp->MyMenuDispalyFunction(temp);
            temp=temp->next;
        }
        GK_ApiFillWin(temp->CurWin, operatingDispara->DisplayColor.menu_color, 0);
        temp->MyMenuDispalyFunction(temp);
         return operatingDispara;
    }
}

// =========================================================================
// ��������:������ʾ����֮�л�����һ���˵��next��
// �������:operatingDispara����ǰ���ڲ�������ʾ����
// ���������
// ����ֵ    :
// ˵��         :
// =========================================================================
void goto_nextmenuitem(struct menu_displaypara  *operatingDispara)
{
    if((NULL==operatingDispara)||(NULL==operatingDispara->OperatingMenuitem)||(NULL==operatingDispara->CurWin))
    {
         printf("Invalid parameter---GotoNextmeuitem!\n");
         return;
    }
    operatingDispara->OperatingMenuitem=GetNextMenuitem(operatingDispara->OperatingMenuitem);
//    GK_ApiFillWin(operatingDispara->CurWin, operatingDispara->DisplayColor.menu_color, 0);
    operatingDispara->AdjustVisualMenuitem(operatingDispara);
    operatingDispara->MyMenuDispalyFunction(operatingDispara);
}

// =========================================================================
// ��������:������ʾ����֮�л�����һ���˵��pre��
// �������:operatingDispara����ǰ���ڲ�������ʾ����
// ���������
// ����ֵ  :
// ˵��    :
// =========================================================================
void goto_premenuitem(struct menu_displaypara  *operatingDispara)
{
    if((NULL==operatingDispara)||(NULL==operatingDispara->OperatingMenuitem)||(NULL==operatingDispara->CurWin))
    {
         printf("Invalid parameter---goto_premenuitem!\n");
         return;
    }
    operatingDispara->OperatingMenuitem=GetPreMenuitem(operatingDispara->OperatingMenuitem);
//   GK_ApiFillWin(operatingDispara->CurWin, operatingDispara->DisplayColor.menu_color, 0);
    operatingDispara->AdjustVisualMenuitem(operatingDispara);
    operatingDispara->MyMenuDispalyFunction(operatingDispara);
}
// =========================================================================
// ��������:������ʾ����֮��ͬ��˵������Բ���step�����л�
// �������:operatingDispara����ǰ���ڲ�������ʾ����
// ���������
// ����ֵ    :
// ˵��         :stepΪ��������ֵ�������next���Ĳ����л�����Ϊ��ֵ������ǰ��pre���Ĳ����л�
//         ��ͬ���˵������л���������ָ��������ѭ������ķ���
// =========================================================================

void goto_menuitem_inslayers(struct menu_displaypara *operatingDispara, s8 step)
{
     s8     MenuitemNo;
     struct menu_item   *pDestMenuitem;
     if((NULL==operatingDispara)||(NULL==operatingDispara->OperatingMenuitem)||(NULL==operatingDispara->CurWin))
     {
       printf("Invalid parameter---goto_menuitem_inslayers!\n");
       return;
     }
     if(0==step)//���ǵ�ǰ�˵������Ҫ���ػ��
     {
       return;
     }
     MenuitemNo=CalMenuitemNumber(operatingDispara->OperatingMenuitem);
     MenuitemNo+=step;

     pDestMenuitem=GetMenuitemBynumber(operatingDispara->OperatingMenuitem,MenuitemNo);
     if(NULL==pDestMenuitem)
     {
       printf("Can not found the dest menuitem ---goto_menuitem_inslayers\n");
       return;
     }
    //�ػ浱ǰ�˵�����
    operatingDispara->OperatingMenuitem=pDestMenuitem;
//    GK_ApiFillWin(operatingDispara->CurWin,operatingDispara->DisplayColor.menu_color, 0);
    operatingDispara->AdjustVisualMenuitem(operatingDispara);
    operatingDispara->MyMenuDispalyFunction(operatingDispara);
    return;
}

// =========================================================================
// �������ܣ����ı����յ�ǰʹ�õ�����д����Ӧ�Ĵ���λ����
// �������:para��д�ı��������������������ɫ����Ϣ��text���ı��ַ���
// ���������
// ����ֵ  :
// ˵��    :�ڲ�ʹ�ú���
// =========================================================================
 void  windows_draw_text(struct draw_wintext_para *para,const char *text)//ȷ��
{

    s32 len, src_len,size,size_bak;
    u32 wc;
    s32 colortext; //������ɫ
    s32 colorback;  //������ɫ  int k=0;
    s32  colorsize=32*16;
    struct tagFontRsc* cur_font;
    u8       *colorbuf;//�洢�����ַ���������
    struct tagGkscParaDrawBitmapRop bitmap_para;
    struct tagGkscParaDrawBitmapRop mpara;
    u8 buf[128],*dbuf;  //����һ���㹻��32�����ֵĻ������������Ҫ��ʾ���ַ�
                        //�����˵��󣬾���Ҫ��̬����

    colortext=para->colortext;
    colorback=para->colorback;
    cur_font = Font_GetCurFont();
    if(cur_font == NULL)
        return;

    // �����ֽ���
    src_len = para->count;
    bitmap_para.x = para->x;
    bitmap_para.gkwin = para->gkwin;
    bitmap_para.y = para->y;
    bitmap_para.bitmap.bm_bits = NULL;
    dbuf = NULL;
    size_bak = 0;

    colorbuf=M_MallocLc(32*32,0);
    memset(colorbuf,0,colorsize);

    for(; src_len > 0;)
    {
        if('/'==*text)
        {
          printf("���У�---windows_draw_text\n");
          bitmap_para.x = para->x;
          bitmap_para.y =bitmap_para.y+16;
          src_len--;
          text++;
          continue;
        }
        len= mbtowc(&wc, text, src_len);
        if(len == -1)
        { // ��Ч�ַ�
            src_len--;
            text++;
        }else
        { // ��Ч�ַ�
            text += len;
            src_len -= len;
            // ȡ����
            bitmap_para.bitmap.bm_bits = NULL;      //NULL=��ѯbitmap.
            cur_font->GetBitmap(wc,0,0,&(bitmap_para.bitmap));
            size = bitmap_para.bitmap.linebytes * bitmap_para.bitmap.height;

            if((size*32)>colorsize)
            {
              free(colorbuf);
              colorsize=size*32;
              colorbuf=M_MallocLc(colorsize,0);
              memset(colorbuf,0,colorsize);
              if(colorbuf==NULL)
                return;
            }
            if(size < 128)
            {
                bitmap_para.bitmap.bm_bits = buf;
            }
            else
            {
                if(size > size_bak)
                {
                    free(dbuf);
                    dbuf = M_MallocLc(size,0);
                    if(dbuf == NULL)
                        return;
                    size_bak = size;
                }
                bitmap_para.bitmap.bm_bits = dbuf;
            }
            cur_font->GetBitmap(wc,0,0,&(bitmap_para.bitmap));


            changeB2I(colorbuf, bitmap_para.bitmap.bm_bits,\
                      size, colortext, colorback);

             mpara.gkwin=bitmap_para.gkwin;
             mpara.x=bitmap_para.x;
             mpara.y=bitmap_para.y;
             mpara.bitmap.height=bitmap_para.bitmap.height;
             mpara.bitmap.width=bitmap_para.bitmap.width;
             mpara.bitmap.linebytes=32*bitmap_para.bitmap.linebytes;
             mpara.bitmap.PixelFormat=CN_SYS_PF_ERGB8888;
             mpara.bitmap.bm_bits=colorbuf;
             //ÿ����ͼ��Ҫ������ɣ���Ȼ������ڴ汻��д��������ʾ����
             printf("x=%d  y=%d\n",mpara.x,mpara.y);
             GK_ApiDrawBitMap(mpara.gkwin, &(mpara.bitmap), mpara.x, mpara.y,0,0, CN_TIMEOUT_FOREVER);
             //���ǵ�Ҫ�ͷ��ڴ棬���Ա���ȴ�����Ժ��ٷ��أ��������߳��л���
              bitmap_para.x += bitmap_para.bitmap.width;
        }
    }

    free(colorbuf);
    free(dbuf);
}

//// =========================================================================
//// �������ܣ����ı���ʾ���ض��ľ������򣬳����Ĳ��ֽ����ᱻ��ʾ��
//// �������: para��д�ı��������������������ɫ����Ϣ��text���ı��ַ���
//// ���������
//// ����ֵ  : ��ʾ�Ľ�β�ַ�����Ȼ�п���û����ʾ�꣬���Բ�һ�����ַ����Ľ�β��
//// ˵��    : ��Ȳ�����������ʾ���߶Ȳ����ᾡ����ʾһ���֡�
//// =========================================================================
// char*  draw_text_in_rec(struct draw_wintext_para *para,const char *text)
//{
//    s32 len, src_len,size,size_bak;
//    u32 wc;
//  s32 width_limit,height_limit;//���Ƶĳ��Ⱥ͸߶ȣ��������ʼ��
//  s32 width_draw,height_draw;//��ǰ��ʾ�ĳ��Ⱥ͸߶ȣ��������ʼ�㣬
//  s32 height_line=0;//��ǰ�еĸ߶ȣ�����ߵ�һ���ַ���
//  s32 colortext; //������ɫ
//  s32 colorback;  //������ɫ
//  s32  colorsize=32*128;
//    struct tagFontRsc* cur_font;
//    u8       *colorbuf;//�洢�����ַ���������
//    struct tagGkscParaDrawBitmap bitmap_para;
//    struct tagGkscParaDrawBitmap mpara;
//    u8 buf[128],*dbuf;  //����һ���㹻��32�����ֵĻ������������Ҫ��ʾ���ַ�
//                        //�����˵��󣬾���Ҫ��̬����
//
//  //�����Ϸ��Լ��
//  if((NULL==para)||(NULL==para->gkwin)||(NULL==text))
//  {
//      printf("Invalid parameter----draw_text_in_rec\n");
//      return text;
//  }
//
//  colortext=para->colortext;
//  colorback=para->colorback;
//    cur_font = Font_GetCurFont();
//  if(cur_font == NULL)
//      return;
//
//    // �����ֽ���
//    src_len = para->count;
//
//    bitmap_para.bitmap.bm_bits = NULL;
//    dbuf = NULL;
//    size_bak = 0;
//
//    colorbuf=M_MallocLc(colorsize,0);
//    memset(colorbuf,0,colorsize);
//
//    for(; src_len > 0;)
//    {
//      if('/'==*text)
//      {
//        printf("���У�---windows_draw_text\n");
//        width_draw =0;
//        height_draw+=height_line;
//        src_len--;
//        free(colorbuf);
//            colorsize=size*32;
//            colorbuf=M_MallocLc(colorsize,0);
//            memset(colorbuf,0,colorsize);
//            if(colorbuf==NULL)
//              goto exit;
//        continue;
//      }
//        len= mbtowc(&wc, text, src_len);
//        if(len == -1)
//        { // ��Ч�ַ�
//            src_len--;
//            text++;
//        }else
//        { // ��Ч�ַ�
//            text += len;
//            src_len -= len;
//            // ȡ����
//            bitmap_para.bitmap.bm_bits = NULL;      //NULL=��ѯbitmap.
//            cur_font->GetBitmap(wc,0,0,&(bitmap_para.bitmap));
//            size = bitmap_para.bitmap.linebytes * bitmap_para.bitmap.height;
//
//            if((size*32)>colorsize)
//              {
//              free(colorbuf);
//            colorsize=size*32;
//            colorbuf=M_MallocLc(colorsize,0);
//            memset(colorbuf,0,colorsize);
//            if(colorbuf==NULL)
//              goto exit;
//          }
//            if(size < 128)
//            {
//                bitmap_para.bitmap.bm_bits = buf;
//          }
//            else
//            {
//                if(size > size_bak)
//                {
//                    free(dbuf);
//                    dbuf = M_MallocLc(size,0);
//                    if(dbuf == NULL)
//                        goto exit;
//                  free(colorbuf);
//                  colorsize=size*32;
//                  colorbuf=M_MallocLc(colorsize,0);
//                  memset(colorbuf,0,colorsize);
//                  if(colorbuf==NULL)
//                     goto exit;
//                    size_bak = size;
//                }
//                bitmap_para.bitmap.bm_bits = dbuf;
//            }
//            cur_font->GetBitmap(wc,0,0,&(bitmap_para.bitmap));
//
//          if((width_draw+bitmap_para.bitmap.width)>width_limit)//��Ҫ���߽磬���ã����ǲ����˰�
//          {
//               width_draw+=bitmap_para.bitmap.width;
//
//               continue;//�������Ż��л��е�
//          }
//          else if(height_draw>=height_limit)//�߶��Ѿ������ˣ��������ǲ�Ӧ�ٻ��ˣ�
//          {
//              goto exit;
//          }
//          else if((height_draw+bitmap_para.bitmap.height)>height_limit)//�߶ȼ������꣬���ǻ�û�г��꣬�ͻ���ôһ���Ϳ�����
//          {
//              bitmap_para.bitmap.height=height_limit-height_draw;//��Ϊ���޸ĸ߶�
//              size = bitmap_para.bitmap.linebytes * bitmap_para.bitmap.height;
//
//              changeB2I(colorbuf, bitmap_para.bitmap.bm_bits,size, colortext, colorback);
//              mpara.gkwin=para->gkwin;
//              mpara.x=para->x+width_draw;
//              mpara.y=para->y+height_draw;
//              mpara.bitmap.height=bitmap_para.bitmap.height;
//              mpara.bitmap.width=bitmap_para.bitmap.width;
//              mpara.bitmap.linebytes=32*bitmap_para.bitmap.linebytes;
//              mpara.bitmap.pf_type=CN_SYS_PF_E8R8G8B8;
//              mpara.bitmap.bm_bits=colorbuf;
//           //ÿ����ͼ��Ҫ������ɣ���Ȼ������ڴ汻��д��������ʾ����
//              printf("x=%d  y=%d\n",mpara.x,mpara.y);
//              GK_ApiDrawBitMap(mpara.gkwin, &(mpara.bitmap), mpara.x, mpara.y, CN_TIMEOUT_FOREVER);
//              //������ʾ�Ŀ�Ⱥ͸߶�
//              width_draw+=bitmap_para.bitmap.width;
//              height_draw+=bitmap_para.bitmap.height;
//             //���ǵ�Ҫ�ͷ��ڴ棬���Ա���ȴ�����Ժ��ٷ��أ��������߳��л���
//          }
//          else//��������������ھ��ο�����ʾ
//          {
//              changeB2I(colorbuf, bitmap_para.bitmap.bm_bits,size, colortext, colorback);
//              mpara.gkwin=para->gkwin;
//              mpara.x=para->x+width_draw;
//              mpara.y=para->y+height_draw;
//              mpara.bitmap.height=bitmap_para.bitmap.height;
//              mpara.bitmap.width=bitmap_para.bitmap.width;
//              mpara.bitmap.linebytes=32*bitmap_para.bitmap.linebytes;
//              mpara.bitmap.pf_type=CN_SYS_PF_E8R8G8B8;
//              mpara.bitmap.bm_bits=colorbuf;
//           //ÿ����ͼ��Ҫ������ɣ���Ȼ������ڴ汻��д��������ʾ����
//              printf("x=%d  y=%d\n",mpara.x,mpara.y);
//              GK_ApiDrawBitMap(mpara.gkwin, &(mpara.bitmap), mpara.x, mpara.y, CN_TIMEOUT_FOREVER);
//              //������ʾ�Ŀ�Ⱥ͸߶�
//              width_draw+=bitmap_para.bitmap.width;
//              height_draw+=bitmap_para.bitmap.height;
//          }
//
//        }
//    }
//exit:
//  free(colorbuf);
//    free(dbuf);
//  printf("end ----window_draw_text!\n");
//  return text;
//}
//// ========================================================================================================
//// �������ܣ������ְ�λ��Ϊ������ʽ��ʾ�����ƺ���ʾ�ĳ����Լ�ָ��
//// �������: para����Ҫ����ʾ���ı���һЩ��Ϣ��
////           data:����ʾ�����ݣ�hexָ���Ľ��ƣ�
////           bitNumber����ʾ�ɼ�λ�������Ĳ�0�������ĵ�λ����ʾ�����ݲ�׼ȷ��Ӧ�����ҵ�����,����������
////                      �������ո�+����ֵ����������-��+����ֵ
////           selectBit,�༭�ĵڼ�λ��
////           selectTextColor,selectBackColor,�༭�����ݺͱ�����ɫ
//// ���������
//// ����ֵ  :
//// ˵��    : ��Ȳ�����������ʾ���߶Ȳ����ᾡ����ʾһ���֡�
////           ������ȫ���������Ӧ�ö��ǰ�������ʽ�ģ���API���ǰ������ģ�
////           ������ǣ���ô������Ӧ���޸ģ����Ӧ�ò��Ǻ��ѡ�
//// ==========================================================================================================
//void   draw_digital_in_rec(struct draw_wintext_para para, s32 data,s8 hex,s8 bitNumber,\
//                         s8 selectBit,u32 selectTextColor,u32 selectBackColor)
//{
//
//  u32  i=0,k=0;
//  bool_t signedflag=true;//������������true������false
//    //�������
//  if((NULL==para.gkwin)||(hex<2))//Ŀǰ��û��2�������µ����ݣ����������ϵ��
//  {
//      printf("Invalid parameter----draw_digital_in_rec\n");
//    return;
//  }
//  //�����ڸý�������������ж೤
//  //�ж�������
//  if(data<0)
//  {
//    signedflag=false;
//    k=-data;
//  }
//  else
//  {
//    signedflag=true;
//    k=data;
//  }
//
//}


