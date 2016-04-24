
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
//����ģ��:menu manager  block
//����:  .
//�汾��V1.0.0
//�ļ�����:�����˵����ڵĹ��߶���
//����˵��:����ʵ�ְ��������ֵİ�װ�ͽ���
//�޶���ʷ:
//
//1. ����: 2012-10-26
//   ����:
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------


#include "Menu_arabic.h"

//��װ����������

#define cfg_charset_arabic  1
#define cfg_arabic  1
#define cn_nls_charset_arabic   "arabic"
#define cn_font_arabic  "arabic_fontx16"

//����������ı���͵����ļ�����fontmaker���ɵ�
//unsigned char *ArialFile="E:\\�������ɹ���\\fontmaker 2.01\\output\\arialuni_U16.bin";
unsigned char *ArialFile="arialuni_U16.bin";
//���ļ����ݶ�ȡ�����������
unsigned char *arabic_file_code=NULL;
//unicode���������
static   unsigned  nSections=0;
//ָ���һ������ͷ
static struct  tagFlSectionInfo   *pSectionInfo=NULL;

// ע�Ͳ��� encoding.h-> tagCharset -> mbs_to_uc4s
s32 __arabic_mbs_to_ucs4s(u32* pwcs, const char* mbs, s32 n)
{
    return 0;
}

s32 __arabic_ucs4s_to_mbs(u32* pwcs, const char* mbs, s32 n)
{
    return 0;
}
s32 __arabic_ucs4_to_mb(u32* pwcs, const char* mbs, s32 n)
{
    return 0;
}
//ԭ��unicode������������
s32 __arabic_mb_to_ucs4(u32* pwc, const u16* mbs, s32 n)
{
    u16 code=0;
    s32 result=-1;
    int i=0;
    u32 wc=0;


 //   printf("In arabic mb to ucs4!\n");

    if((NULL==pwc)||(NULL==mbs))
        {
          return result;
        }
    code=*(u16 *)mbs;
    //code=0x0632;
    //����ת������unicode���룬����
     for(i=0;i<nSections;i++)
        {

          if((code>=pSectionInfo[i].First)&&(code<=pSectionInfo[i].Last))
            {

              wc=*((u32 *)(arabic_file_code+pSectionInfo[i].OffAddr+(code-pSectionInfo[i].First)*4));
              //printf("Orignal code=%04x:in section %d,wc=%08x\n",*mbs,i,wc);
              break;
          }
     }
     if(i<nSections)//�ҵ�����ص�����
        {
          if(wc<=(sizeof(struct tagFontLibHeader)+nSections*sizeof(struct tagFlSectionInfo)))//С���ַ�ͷ�ļ�
          {
              result = -1;
              printf("In the file header!\n");

          }
          else
            {

             result=2;
          }
     }
     else //û�ж�Ӧ������
     {
         result=-1;
     }
    *pwc=wc;
    return result;
}


unsigned char * GetLanguagecodeFromFile(const char *FileName)
{

   FILE  *fp;
   struct tagFontLibHeader fileHeader;
   u32    FileLength;
   unsigned char *LanguageCode=NULL;

   fp=fopen(FileName,"rb");
   if(fp==NULL)
    {
      printf("open the file failed!\n");
      goto exit;
   }
   fseek(fp,0L,SEEK_SET);

   fread(&fileHeader,sizeof(struct tagFontLibHeader),1,fp);

    if((fileHeader.magic[0] != 'U' && fileHeader.magic[0] != 'M'))
    {
        printf("Cann't support file format!\n");
        return 0;
    }

    if('M' == fileHeader.magic[0])     //mbcs ����
    {
        printf("This is the Mbcs code!\n");
    }
    else
    {
        printf("This is the Unicode code!\n");
    }
    //��fileHeader�����ݵ�˳����е���
  //  AdjustProcedure( &fileHeader.Size, &fileHeader.Size, sizeof(fileHeader.Size));
//  AdjustProcedure( &fileHeader.wCpFlag, &fileHeader.wCpFlag, sizeof(fileHeader.wCpFlag));

    printf("filesize=%08x\n",fileHeader.Size);
    printf("LanguageF=%04x\n",fileHeader.wCpFlag);

    LanguageCode=M_MallocLc(fileHeader.Size,0);
    if(NULL==LanguageCode)
        {

      fclose(fp);
      goto exit;
    }
    fseek(fp,0L,SEEK_SET);
    FileLength=fread(LanguageCode,fileHeader.Size,sizeof(unsigned char),fp);
    printf("---File--Length=%08x\n",FileLength);
//  memcpy(&fileHeader,LanguageCode,0x10);
    fclose(fp);

exit:
    return LanguageCode;
}


/*
������:��װ���������ֵĽ��벿��
��������:����������İ�װ

*/


void module_init_encode_arabic(ptu32_t para)
{
    static struct tagCharset encoding;

    int i=0;
   //�����ֿ�
     printf("Init the code\n");
    arabic_file_code=GetLanguagecodeFromFile(ArialFile);
    if(arabic_file_code==NULL)
        {
         printf("Get file code failed!\n");
         return;
    }
    nSections=((struct tagFontLibHeader*)arabic_file_code)->nSection;
    pSectionInfo=(struct tagFlSectionInfo *)(arabic_file_code+0x10);

    //��ӡ������Ϣ
    for(i=0;i<nSections;i++)
        {
     printf("Section=%d ,First=%04x,Last=%04x,Addr=%08x\n",i,pSectionInfo[i].First,\
        pSectionInfo[i].Last,pSectionInfo[i].OffAddr);

    }

    encoding.max_len = 1;
    encoding.mb_to_ucs4 = __arabic_mb_to_ucs4;
    encoding.ucs4_to_mb = __arabic_ucs4_to_mb;
    encoding.mbs_to_ucs4s = __arabic_mbs_to_ucs4s;
    encoding.ucs4s_to_mbs = __arabic_ucs4s_to_mbs;
    if( Charset_NlsInstallCharset(&encoding, cn_nls_charset_arabic))
    {
        printf("arabic encoding install sucess\n\r");
        return 1;
    }else
    {
        Djy_SaveLastError(EN_GK_CHARSET_INSTALL_ERROR);
        printf("arabic encoding install fail\n\r");
        return 0;
    }


}

//----������ȡ-----------------------------------------------------------------
//����: ��װ����������
//����: charcode������������

//      bitmap����������ȡ�ĵ����λͼ���������ɵ������ṩ
//����: true=������false=charcode���Ǹ�������֧�ֵ��ַ�����Χ������ʱ��Ȼ����
//      Ĭ���ַ��ĵ���
//-----------------------------------------------------------------------------
bool_t __arabic_get_char_bitmap(u32 charcode, u32 size,u32 resv,
                                    struct tagRectBitmap *bitmap)
{
    u8 i;
    u32 offset;
    bool_t result = true;

    struct tagFontLibHeader fileHeader;

    u32  AddrOffset;
    u8   width;
    u8   lineBytes;
    u8   height;

 //   printf("In arabic mb to get char bitmap!\n");
    if(NULL==arabic_file_code)
        {

          printf("No init the arabic_file_code pointer!\n");
          return false;
    }


    fileHeader=*(struct tagFontLibHeader *)arabic_file_code;


//  printf("charcode=%08x\n",charcode);
    AddrOffset=(charcode&0x03FFFFFF);//��26λ��ʾƫ�Ƶ�ַ
    width=(charcode>>24);//��6λ��ʾ���
    width=width>>2;
 //   printf("width=%02x\n",width);
    height=fileHeader.YSize;

    lineBytes=(width+7)/8;
//    printf("linbte=%02x\n",lineBytes);
//   printf("width=%02x\n",width);
/*
   if(AddrOffset<=(sizeof(struct tagFontLibHeader)+nSections*sizeof(struct tagFlSectionInfo)))//С���ַ�ͷ�ļ�
    {
          result = false;
          printf("In the file header!\n");
          goto exit;

   }
   */
    if((AddrOffset+lineBytes*height)>fileHeader.Size)//�����ַ��ļ��ķ�Χ
        {

          result = false;
          printf("ecceed!\n");
          goto exit;
    }

    // �õ��ַ���ģ�������е�ƫ����
    bitmap->pf_type = CN_SYS_PF_GRAY1;
    bitmap->width = width;
    bitmap->height = height;
    bitmap->linebytes = lineBytes;

    if(bitmap->bm_bits != NULL)
    {
        for(i=0; i<lineBytes*height; i++)
        {
            bitmap->bm_bits[i] = arabic_file_code[AddrOffset+i];
        }
    }

exit:
    return result;
}
bool_t __arabic_load_font(u8* zk_addr)
{


   return true;//do nothing


}
bool_t __arabic_unload_font(u8* zk_addr)
{

   return true;//do nothing


}
/*
������:��װ���������ֵĵ��󲿷�
��������:�����������ӳ��
*/

ptu32_t module_init_font_arabic(ptu32_t para)
{
    static struct tagFontRsc font_arabic;
  //  __Charset_Gb2312_816_1616LoadFont((u8*)para);  //�����ֿ�
    font_arabic.fi.type = CN_FONT_TYPE_DOT;
    font_arabic.LoadFont = __arabic_load_font;
    font_arabic.UnloadFont = __arabic_unload_font;
    font_arabic.GetBitmap = __arabic_get_char_bitmap;

    if(Gk_FontInstallFont(&font_arabic, cn_font_arabic))
    {
        printf("arabic  font install sucess\n\r");
        return 1;
    }else
    {
        Djy_SaveLastError(EN_GK_FONT_INSTALL_ERROR);
        printf("arabic font install fail\n\r");
        return 0;
    }
}


//���԰��������ֵİ�װ

void test_arabic( void)
{
//��ʼ���������


  struct tagFontRsc  *pfont=NULL,font;
  struct tagCharset  *pencode=NULL,encode;

  #if (cfg_charset_arabic== 1)       //��ʼ���������ַ���
    module_init_encode_arabic(0);
  #endif


//���³�ʼ���������
  #if (cfg_arabic== 1)     //��ʼ�������������ֿ�
    module_init_font_arabic(0);
  #endif

  //����arabicΪĬ������

  encode.node.name="arabic";
  pencode=Charset_NlsSetCurCharset(&encode);
  font.node.name="arabic_fontx16";
  pfont=Font_SetCurFont(&font);

  printf("cur_encode=%s\n",pencode->node.name);
  printf("cur_font=%s\n",pfont->node.name);

}

//0621��064a�ı����룬�����˲��ֵĶ����Լ���������

const u16 Arbic_Position[][4]=  // first, last, middle, alone
{
{ 0xfe80, 0xfe80, 0xfe80, 0xfe80},    // 0x621
{ 0xfe82, 0xfe81, 0xfe82, 0xfe81},
{ 0xfe84, 0xfe83, 0xfe84, 0xfe83},
{ 0xfe86, 0xfe85, 0xfe86, 0xfe85},
{ 0xfe88, 0xfe87, 0xfe88, 0xfe87},
{ 0xfe8a, 0xfe8b, 0xfe8c, 0xfe89},
{ 0xfe8e, 0xfe8d, 0xfe8e, 0xfe8d},
{ 0xfe90, 0xfe91, 0xfe92, 0xfe8f},   // 0x628
{ 0xfe94, 0xfe93, 0xfe93, 0xfe93},
{ 0xfe96, 0xfe97, 0xfe98, 0xfe95},   // 0x62A
{ 0xfe9a, 0xfe9b, 0xfe9c, 0xfe99},
{ 0xfe9e, 0xfe9f, 0xfea0, 0xfe9d},
{ 0xfea2, 0xfea3, 0xfea4, 0xfea1},
{ 0xfea6, 0xfea7, 0xfea8, 0xfea5},
{ 0xfeaa, 0xfea9, 0xfeaa, 0xfea9},
{ 0xfeac, 0xfeab, 0xfeac, 0xfeab},   // 0x630
{ 0xfeae, 0xfead, 0xfeae, 0xfead},
{ 0xfeb0, 0xfeaf, 0xfeb0, 0xfeaf},
{ 0xfeb2, 0xfeb3, 0xfeb4, 0xfeb1},
{ 0xfeb6, 0xfeb7, 0xfeb8, 0xfeb5},
{ 0xfeba, 0xfebb, 0xfebc, 0xfeb9},
{ 0xfebe, 0xfebf, 0xfec0, 0xfebd},
{ 0xfec2, 0xfec3, 0xfec4, 0xfec1},
{ 0xfec6, 0xfec7, 0xfec8, 0xfec5},  // 0x638
{ 0xfeca, 0xfecb, 0xfecc, 0xfec9},
{ 0xfece, 0xfecf, 0xfed0, 0xfecd},  //0x63A
{ 0x63b, 0x63b, 0x63b, 0x63b},
{ 0x63c, 0x63c, 0x63c, 0x63c},
{ 0x63d, 0x63d, 0x63d, 0x63d},
{ 0x63e, 0x63e, 0x63e, 0x63e},
{ 0x63f, 0x63f, 0x63f, 0x63f},
{ 0x640, 0x640, 0x640, 0x640},   // 0x640
{ 0xfed2, 0xfed3, 0xfed4, 0xfed1},
{ 0xfed6, 0xfed7, 0xfed8, 0xfed5},
{ 0xfeda, 0xfedb, 0xfedc, 0xfed9},
{ 0xfede, 0xfedf, 0xfee0, 0xfedd},
{ 0xfee2, 0xfee3, 0xfee4, 0xfee1},
{ 0xfee6, 0xfee7, 0xfee8, 0xfee5},
{ 0xfeea, 0xfeeb, 0xfeec, 0xfee9},
{ 0xfeee, 0xfeed, 0xfeee, 0xfeed},   // 0x648
{ 0xfef0, 0xfeef, 0xfef0, 0xfeef},
{0xfef2, 0xfef3, 0xfef4, 0xfef1},   // 0x64A
};
//�ַ���һ�������ж�ǰ��
const u16 theSet1[23]={
        0x62c, 0x62d, 0x62e, 0x647, 0x639, 0x63a, 0x641, 0x642,
        0x62b, 0x635, 0x636, 0x637, 0x643, 0x645, 0x646, 0x62a,
        0x644, 0x628, 0x64a, 0x633, 0x634, 0x638, 0x626};

//�ַ������������жϺ���
const u16 theSet2[35]={
        0x62c, 0x62d, 0x62e, 0x647, 0x639, 0x63a, 0x641, 0x642,
        0x62b, 0x635, 0x636, 0x637, 0x643, 0x645, 0x646, 0x62a,
        0x644, 0x628, 0x64a, 0x633, 0x634, 0x638, 0x626,
        0x627, 0x623, 0x625, 0x622, 0x62f, 0x630, 0x631, 0x632,
        0x648, 0x624, 0x629, 0x649};
//�������֣�������д
static u16 arabic_specs[][2]=
{
{0xFEF5,0xFEF6},
{0xFEF7,0xFEF8},
{0xFEF9,0xFEFA},
{0xFEFB,0xFEFC},
};

/*
��������:JudgeFirst()
��������:arabic_code����ǰ�ַ���ǰһ��arabic����
����ֵ:true����ǰ��
       false:����ǰ��
��������:�ж��ǲ���ǰ���ַ�
*/
bool_t JudgeFirst(u16 arabic_code)
{
   int i=0;

   for(i=0;i<23;i++)
    {
      if(arabic_code==theSet1[i])
        {
          break;
        }
   }
   if(i<23)
    {
     return true;
   }
   else
    {
     return false;
   }
}


/*
��������:JudgeLast()
��������:arabic_code����ǰ�ַ��ĺ�һ��arabic����
����ֵ:true���Ǻ���
       false:���Ǻ���
��������:�ж��ǲ��Ǻ����ַ�
*/
bool_t JudgeLast(u16 arabic_code)
{
   int i=0;

   for(i=0;i<35;i++)
    {
      if(arabic_code==theSet2[i])
        {
          break;
        }
   }
   if(i<35)
    {
     return true;
   }
   else
    {
     return false;
   }
}

/*

��������:JudgeSpecCode()
����:u16 curcode,nexcode,
����ֵ���жϵ�ǰ�����ǲ������ַ�

true:�����ַ�
false:�������ַ�

*/

bool_t JudgeSpecCode(u16 curcode, u16 nexcode)
{

    bool_t  result=false;
    if(curcode!=0x0644)
        {
     return result;
    }
    if((0x0622==nexcode)||(0x0623==nexcode)||(0x0625==nexcode)||(0x0627==nexcode))
        {
      result=true;

    }
    return result;
}



/*
��������:GetDeformdArial()
��������:precode,curcode,nexcode;ǰһ���ַ�,��ǰ�ַ�����һ���ַ�
����:��ȡ�ı��κ������
*/

u16  GetDeformArial(u16 precode,u16 curcode, u16 nexcode)
{

    u8   type=0;
    u8   offset;
    u16  result;

    bool_t bFirst;
    bool_t bLast;

    if((0x064a<curcode)||(0x0621>curcode))//������ͱ�����ͬ
        {
      return curcode;
    }

    offset=curcode-0x0621;


    bFirst=JudgeFirst(precode);
    bLast=JudgeLast(nexcode);
    if(bFirst)
        {

      type|=1<<0;
    }
    if(bLast)
        {
      type|=1<<1;
    }
    switch(type)
        {
          case 0:
                  result=Arbic_Position[offset][3];//����
                  break;
          case 1 :
                  result=Arbic_Position[offset][0];//ǰ��
                  break;
          case 2:
                  result=Arbic_Position[offset][1];//����
                  break;
          case 3:
                  result=Arbic_Position[offset][2];//�м�
                  break;
           default:
                 result=Arbic_Position[offset][3];//����
                 break;
    }

     return result;

}

/*
��������:GetSpecCode()
����:u16 precode,nexcode;��ǰ�ַ���ǰһ���ַ�����ǰ�ַ�����ǰ�ַ��ĺ�һ���ַ�
����ֵ:
      ���ַ���unicode����
��������:��ȡ���ַ������֣���0x644�ͺ���һ�����ֵ�����,ִ�иú���ǰӦ�����ж��ǲ���������

*/

u16 GetSpecCode(u16 precode,u16 nexcode)
{
   bool_t Inset1;//ǰ��һ���ַ��ǲ����ڼ���1��
   u16   result=0x0000;
   Inset1=JudgeFirst(precode);
   if(Inset1)
    {
       switch(nexcode)
        {
           case 0x0622:
                     result=arabic_specs[0][1];
                     break;
           case 0x0623:
                     result=arabic_specs[1][1];
                     break;
           case 0x0625:
                     result=arabic_specs[2][1];
                     break;
           case 0x0627:
                     result=arabic_specs[3][1];
                     break;
            default:
                     printf("fataerr!In getspeccode!\n");
                     break;
       }

    }
   else
    {

       switch(nexcode)
        {
           case 0x0622:
                     result=arabic_specs[0][0];
                     break;
           case 0x0623:
                     result=arabic_specs[1][0];
                     break;
           case 0x0625:
                     result=arabic_specs[2][0];
                     break;
           case 0x0627:
                     result=arabic_specs[3][0];
                     break;
            default:
                     printf("fataerr!In getspeccode!\n");
                     break;
        }

     }

    return result;
}


/*
 ��������:ChUps2Ufs()
 ����: u16 *Ups;ֻ���Ա�׼��ĸ��������//�ռ������û��ṩ
       u32 count ups�ĸ���
       u16 *Ufs;ת�����ȫ��������//�ռ����û��ṩ
       u32 *number;ת������ַ�����Ŀ

����ֵ: true,ת���ɹ���
        false:����
��������:�ú����ܹ�����׼��������ת����ȫ������ʽ�İ������ģ�Ӧ�ø����ֿ���ֱ���ҵ���Ӧ�İ������ģ�
         ������д�����֡�
*/
bool_t ChUps2Ufs(u16 *Ups, u32 count, u16 *Ufs,u32 *number)
{
    u32 num=0;
    u32 code_num=0;
    bool_t result=true;

    if(0==count)//����Ϊ0
        {
        *number=code_num;
        return result;
    }
    else if(1==count)//����Ϊ1
        {
         *Ufs=*Ups;
         *number=1;
         return result;
    }

    while(num<count)
    {
      if(0==num)//��һ��
      {
         *Ufs=GetDeformArial(0, *Ups, *(Ups+1));
    //   printf("This the first one!--%04x\n",*Ufs);
         num++;
         Ufs++;
         Ups++;
         code_num++;

      }
      else if(count-1==num)//���һ��
      {
         *Ufs=GetDeformArial(*(Ups-1),*Ups,0);
          num++;
          code_num++;
      }
      else//�м���ַ�
      {
        if(JudgeSpecCode(*Ups, *(Ups+1)))//�����ַ�
            {
              *Ufs=GetSpecCode(*(Ups-1),*(Ups+1));
             // printf("This the spec code!--%04x\n",*Ufs);

              Ufs++;
              Ups=Ups+2;
              num=num+2;
             }
         else
            {
               *Ufs=GetDeformArial(*(Ups-1), *(Ups), *(Ups+1));
            //   printf("This the deformed code!--%04x\n",*Ufs);

               Ufs++;
               Ups++;
               num++;
             }//end for whether spec code
          code_num++;

      }//end for num

    }//end for while

    *number=code_num;
    return result;

}//end for function

/*
��������:CalUpsLength
����:u16 *Ups,  Ups������
     u32   count;UPs����
����ֵ: u32  Ups����Ufs�ĸ���
*/

u32 CalUpsLength(u16 *Ups, u32 count)
{
    u32 num=0;
    u32 code_num=0;

    if(0==count)//����Ϊ0
        {
        return code_num;
    }
    else if(1==count)//����Ϊ1
        {
         return 1;
    }

    while(num<count)
    {
      if(0==num)//��һ��
      {
         num++;
         Ups++;
         code_num++;

      }
      else if(count-1==num)//���һ��
      {
          num++;
          code_num++;
      }
      else//�м���ַ�
      {
        if(JudgeSpecCode(*Ups, *(Ups+1)))//�����ַ�
            {
              Ups=Ups+2;
              num=num+2;
             }
         else
            {
               Ups++;
               num++;
             }//end for whether spec code
          code_num++;
      }//end for num
    }//end for while
    return code_num;
}//end for function


/*

��������:AdjustUfsProcedure
��������:u16 *ufs,�洢ufscode,�洢�����û����𣬰�ȫ�����û�����
         u32  count;ufs�ĸ���
��������:���ڰ���������������������ʾ�ģ�����Ҫ�����ַ�����Ϊ�����Ǵ�������ʾ��
*/

void AdjustUfsProcedure(u16 *Ufs, u32 count)
{


   int i=0,j=0,k=0;

   u16 temp;

   printf("Begin to adjust the data procedure!\n");


   if(NULL==Ufs)
    {
      printf("You have ab NULL pointer for AdjustUfs!\n");
      return;
   }

   while(i<count)//��������UFS��
    {
       if(((*(Ufs))>=0x30)&&((*(Ufs))<=0x39))//�ж��ǲ������֣����ֱ����Ѿ�ת����U16��ʽ��
        {
           j=0;
           printf("Got an digital number--%d\n",i);
           while(((*(Ufs+j))>=0x30)&&((*(Ufs+j))<=0x39))//�ж��ж��ٸ�������
            {
              j++;
             }//end while

           printf("Got an digital number--%d--%d link!\n",i,j);
           for(k=0;k<(j+1)/2;k++)
            {
              temp=*(u16 *)(Ufs+k);
              *(u16 *)(Ufs+k)=*(u16 *)(Ufs+j-1-k);
              *(u16 *)(Ufs+j-1-k)=temp;
            }//end for
            Ufs=Ufs+j;
            i=i+j;
         }//end if
        else
            {
          i++;
          Ufs++;
         }
     }//end while

}//end function

//���ڲ��԰�����������ʾ

















