// =============================================================================
// Copyright (C) 2012-2020 ��԰�̱��Զ������޹�˾ All Rights Reserved
// �ļ����ƣ�vncdisplay.c
// ģ������: vncserver
// ģ��汾: V1.00
// ����ʱ��: 2013-6-24
// ԭʼ����: Administrator
// ����˵������ͷ�ļ�����ʵ�ֲ���������ʾ�Ľӿ�
// =============================================================================
#include "stdint.h"
#include "vnc_keyboard.h"
#include "cpu_peri.h"
#include "keysymdef.h"
#include "rfbproto.h"
#include "debug.h"
#include "vnc_bmp.h"//���ļ��г�ʼ��keymap

#if CN_CFG_KEYBOARD == 1

static u32 vnc_key_value = 0;
//����ֵ��������ϼ��Ľ��룬���ǽ���֧����������ͬʱ���£�����VNC���ԣ���Ҫ�Ȱ������
//����Ȼ������Ҫ��ϵİ��� ��Ȼ������Ҫ��ϵļ���Ȼ������ϼ���ʾ��Ͻ���
//��key_value�ĸ�8λ�����λ����ʾ�ǲ�����ϼ�
static u8  vnc_key_flag = 0;


#define cn_vnc_keyboard_name "vnc keyboard"
#define cn_vnc_touchlcd_name  "vnc_touchboard"
// =============================================================================
// ��������:vnc������ȡ����
// �������:server
// ���������
// ����ֵ:
// ˵��:��Դ����Ҫ���ٽ���Դ����������Ϊ���漰���������⣬��ʵʱ��Ҫ�󲻸�
// =============================================================================
u32 vnc_keyboard_read(void)
{
    if(vnc_key_flag)//�ñ�־����vnc����Ϣ�������������ã�����������Ժ�Żἤ��
    {
//      debug_printf("read an key =%d \n",vnc_key_value);
        vnc_key_flag = 0x00;

        return vnc_key_value;
    }
    else
    {
    //  debug_printf("no key---vnc soft keyboard!\n");
        return 0;
    }
}

//cygװ��ר�ü��̵�ӳ���ϵ
//const u8 cyg_keyboard[cn_vnc_key_num] = {37,38,39,40,13,27,112,113,cn_vnc_func_key};
const u8 cyg_keyboard[cn_vnc_key_num] = {37,38,39,40,13,27,112,113};
// =============================================================================
// ��������:ʵ�ּ���ֵ��ת��
// �������:key����Ҫת���ļ���ֵ
// ���������
// ����ֵ:ת����ļ���ֵ��0XFF���Ҳ����������
// ˵��:����ϣ������ļ���ֵ����0-32��ã�ʵ���ϲ��������ģ����ֱ���������ֲ�����
//      �˺�����������ǽ�����ֵת��Ϊ������Ҫ��0-32�������ڱ�ǡ���˲�ͬ�ļ��̵�ֵ
//      �Լ�����ת������Ӧ�����
// =============================================================================
u8  translate_key(u8 key)
{
    u8 i = 0;
    for(i=0; i < cn_vnc_key_num; i++)
    {
        if(cyg_keyboard[i] == key)
        {
            return i;
        }
    }
    return cn_invalid_key;//�Ҳ�����Ӧ��key
}
// =============================================================================
// ��������:vnc����������װ
// �������:server,������
// ���������
// ����ֵ:
// ˵��:֮���Է���server�У�����Ϊserver�˳�ʱ��ȻҪ����deviceɾ��
// =============================================================================
bool_t install_vncserver_keyboard(rfbScreenInfoPtr server)
{
    s32 ret = 0;
    server->vnc_keyboard.key_bak = 0;
    server->vnc_keyboard.key_now = 0;
    server->vnc_keyboard.vtime_limit = 0;
    server->vnc_keyboard.read_keyboard = vnc_keyboard_read;
    ret = Keyboard_InstallDevice(cn_vnc_keyboard_name,&(server->vnc_keyboard));
    if(-1==ret)
    {
        debug_printf("##failed to install the vnc keyboard!\n");
    }
    else
    {
        debug_printf("--succeed to install the vnc keyboard!\n");
    }
    return 1;
}

// =============================================================================
// ��������:vnc�������ȡ����
// �������:
// ���������touch_data,���ڴ洢��ȡ������ֵ
// ����ֵ:
// ˵��:
// =============================================================================
static s32  vnc_touch_x = 0;
static s32  vnc_touch_y = 0;
static s32  vnc_touch_z = 0;
static bool_t   vnc_touched_flag = false;
bool_t read_touch_vnclcd(struct tagSingleTouchMsg *touch_data)
{
    if(vnc_touched_flag == true)
    {
        touch_data->x = vnc_touch_x;
        touch_data->y = vnc_touch_y;
        touch_data->z = vnc_touch_z;
        vnc_touched_flag = false;
        return true;
    }
    else
    {
        return false;
    }
}
// =============================================================================
// ��������:vnc����������װ
// �������:server,��������msg �����Ϣ
// ���������
// ����ֵ:
// ˵��:֮���Է���server�У�����Ϊserver�˳�ʱ��ȻҪ����deviceɾ��
// =============================================================================
bool_t install_vncserver_touchlcd(rfbScreenInfoPtr server)
{
    s32 ret = 0;

    server->vnc_lcdtouch.read_touch = read_touch_vnclcd;
    ret = Touch_InstallDevice(cn_vnc_touchlcd_name,&(server->vnc_lcdtouch));
    if(ret)
    {
        debug_printf("##failed to install the vnc touchlcd!\n");
    }
    else
    {
        debug_printf("--succeed to install the vnc touchlcd!\n");
    }
    return 1;
}
// =============================================================================
// ��������:����һ���ͻ��˽��ܵ�һ������ʱ�ľٶ�
// �������:
// ���������
// ����ֵ:
// ˵��:
// =============================================================================
void vnc_input_key_message(rfbScreenInfoPtr server,u8 key_number, bool_t down)
{
    static u8 key_received = 0;//������ϼ�
    static u8 multi_key = 0;  //����Ƿ�����ϼ�����
    static u32  key_bak = 0;//�洢��ΰ�����
    u8  keyvalue = 0;//��ʱ����
    u8  i = 0;//��ʱ����
    u8 keyvalue_bak = 0;//��ʱ����
    u8 keyvalue_numberbak;//��ʱ����
    u8 keydel_flag = 0;//ȡ����ϼ��İ������

    if(key_number >= cn_vnc_key_num)
    {
        return;//����İ���,û��ɶ�Ӻ�����
    }
    keyvalue = cyg_keyboard[key_number];
//��ʼ����԰������Ӿ�����
    if(multi_key) //����ϼ����Ѿ�����ϼ�������
    {
        if(cn_vnc_func_key == keyvalue)
        {
            if(cn_key_down == down)//���ܼ��ٴα����£���ʾ��Ͻ���
            {
                //��ǰ���İ������������
                debug_printf("has received =%d key,keybak=%08x\n",key_received,key_bak);
                for(i=0; i<key_received; i++ )
                {
                    keyvalue_bak = (u8)((key_bak>>(i<<0x03))&0xFF);
                    keyvalue_numberbak = translate_key(keyvalue_bak);
                    //����Ӿ�����
                    server->key_update_list = \
                            server->key_update_list ^(1<<keyvalue_numberbak);
                    //���Ϊ����״̬
                    server->key_state = \
                     server->key_state &(~(cn_key_down << keyvalue_numberbak));
                    debug_printf("multi up key= %d keynumver = %d\n",keyvalue_bak,keyvalue_numberbak);
                }
                //���ܼ��Լ�Ҳ�����,����
                server->key_update_list = \
                        server->key_update_list ^(1<<key_number);
                //���Ϊ����״̬
                server->key_state = \
                 server->key_state &(~(cn_key_down << key_number));

                multi_key = 0;//��ϼ�����
                key_received = 0;//����
                //������vnc ��key board��������
                vnc_key_value = key_bak;
                key_bak = 0;
                vnc_key_flag = 1;
            }
            else//��ϼ�ֻ���ٴΰ����Ӿ��Ż�ָ�������������û���õ�
            {

            }
        }
        else//�������ӻ���ȡ���Ѿ�ѡ��İ�����
        {
            if(down)
            {
                //�����ǲ����Ѿ���������ϼ��У������Ļ��ͱ�ʾȡ��
                keydel_flag = 0x00;
                for(i = 0; i < key_received; i++)
                {
                    keyvalue_bak = (u8)((key_bak>>(i<<0x03))&0xFF);
                    if(keyvalue_bak == keyvalue)
                    {
                        keydel_flag = 0x01;//��Ҫȡ���˰�����
                    }
                }
                if(keydel_flag)//ȡ������
                {
                    //����Ӿ�����
                    server->key_update_list = \
                            server->key_update_list ^(1<<key_number);
                    //���Ϊ����״̬
                    server->key_state = \
                     server->key_state &(~(cn_key_down << key_number));
                    key_received--;
                }
                else//������ϼ�
                {
                    //�����Ƿ񳬳��꣬����Ļ���ȡ����ϵ����һ����
                    if(key_received >= cn_max_multi_key_num)//ʵ����������
                    {
                        key_received--;
                        //��ȡ���һ�ΰ���
                        keyvalue_bak = \
                                (u8)((key_bak >>(key_received<<0x03)) &0xFF);
                        keyvalue_numberbak = translate_key(keyvalue_bak);
                        //��Ҫ�����Ӿ�Ч��һ��
                        server->key_update_list = \
                                server->key_update_list ^(1<<keyvalue_numberbak);
                        //���Ϊ����״̬
                        server->key_state = \
                         server->key_state &(~(cn_key_down << keyvalue_numberbak));

                        key_bak=key_bak &(~((u32)(0xFF<<(key_received<<0x03))));
                        key_bak |=(u32)(keyvalue)<<(key_received<<0x03);
                        //��Ҫ�����Ӿ�Ч��һ��
                        server->key_update_list = \
                                server->key_update_list ^(1<<key_number);
                        //���Ϊ����״̬
                        server->key_state = \
                                server->key_state |(cn_key_down << key_number);
                        key_received++;

                    }
                    else//û�г���
                    {
                        key_bak=key_bak &(~((u32)(0xFF<<(key_received<<0x03))));
                        key_bak =key_bak |(u32)((keyvalue)<<(key_received<<0x03));
                        //��Ҫ�����Ӿ�Ч��һ��
                        server->key_update_list = \
                                server->key_update_list ^(1<<key_number);
                        //���Ϊ����״̬
                        server->key_state = \
                                server->key_state |(cn_key_down << key_number);
                        key_received++;

                    }
                }
            }
            else//����ϼ��������£���������û���õģ���Ǹ��
            {

            }

        }

    }
    else
    {
        if(cn_vnc_func_key == keyvalue)//��ȡ���ǹ��ܼ�
        {
            if(down)//���ܼ���һ�ΰ���
            {
                //��Ҫ�����Ӿ�Ч��һ��
                server->key_update_list = \
                        server->key_update_list ^(1<<key_number);
                //���Ϊ����״̬
                server->key_state = \
                        server->key_state |(cn_key_down << key_number);
                multi_key = 1;
                key_received = 0;
                vnc_key_flag = 0;
                key_bak = 0;
            }
            else//���ܼ����ΰ��·��ɵ���,��˹��ܼ��ĵ�����û�������
            {

            }
        }
        else//��ͨ����
        {
            //������״̬��ͬ�����ô���
            if( (((u8)(server->key_state >> key_number)&0x01)) == down)//�Ƚϱ�־λ
            {

            }
            else//����״̬�ı�Ǹ���
            {
                server->key_update_list = \
                        server->key_update_list ^(1<<key_number);
                //����״̬��������Ӧ�ı��
                if(down)
                {
                    server->key_state = \
                            server->key_state |(cn_key_down << key_number);
                    vnc_key_value = (u32)keyvalue;
                    multi_key = 0;
                    vnc_key_flag = 1;
                }
                else
                {
                    server->key_state = \
                     server->key_state &(~(cn_key_down << key_number));
                    debug_printf("key up = %d\n",key_number);
                }

            }
        }
    }

    return;
}
// =============================================================================
// ��������:��serverдһ����������
// �������:
// ���������
// ����ֵ:
// ˵��:
// =============================================================================
void vnc_input_touch_message(int x, int y, int z)
{
    vnc_touch_x = x*X_SCALE;
    vnc_touch_y = y*Y_SCALE;
    vnc_touch_z = z;
    vnc_touched_flag = true;
}

// =============================================================================
// ��������:ʵ����Ϣ��Ӧ�ж��ڿͻ��˼�����Ϣ����Ӧ
// �������:server,��������key��keyֵ��state��key��״̬
// ���������
// ����ֵ:
// ˵��:���̵�ֵĿǰ���ǵ���ֵ���������Ƕ��ֵ�ļ���--TODO
// =============================================================================
void  vncclient_keyevent(rfbScreenInfoPtr server, rfbClientToServerMsg msg)
{
    u8 key_translate = 0xFF;
    u8 key = 0;
    u8 state = 0;

//test
    static u8 times = 0;
//  ��keysymdefת����DJY�Լ��İ˼�����
    switch (msg.ke.key)
    {
        case XK_Return://enter
            key = 13;
            break;
        case XK_Left:
            key=37;
            break;
        case XK_Right:
            key=39;
            break;
        case XK_Up:
            key=38;
            break;
        case XK_Down:
            key=40;
            break;
        case XK_Escape:
            key=27;
            break;
        default:
            key=(u8)(msg.ke.key);
            break;
    }
    state = msg.ke.down;
//    debug_printf("--client key =%d\n",key);
    key_translate = translate_key(key);//ת����0��ʼ�İ���


    if(key_translate == cn_invalid_key)
    {
        return;//�Ҳ������Ǿ�ʲô����������
    }
    //��0���Ǳ�����1���ȡ��
    //��ӵ�м��̵��¼�������Ϣ��
    vnc_input_key_message(server,key_translate, state);
    return;//����
}
// =============================================================================
// ��������:ʵ�ַ������м��̶�keylist��Ӱ��
// �������:server,��������key��keyֵ��state��key��״̬
// ���������
// ����ֵ:
// ˵��:���̵�ֵĿǰ���ǵ���ֵ���������Ƕ��ֵ�ļ���--TODO
// =============================================================================
void  vncserver_keyevent(rfbScreenInfoPtr server, u8 key, u8 state)
{
    u8 key_translate = 0xFF;

    key_translate = translate_key(key);
    if(key_translate == cn_invalid_key)
    {
        return;//�Ҳ������Ǿ�ʲô����������
    }
    //�ж��Ƿ���ͬһ������
    if( (server->key_state) & (state<<key_translate))
    {
        return ;
    }//���up����downһ��Ķ����ǲ����ܵģ������Ǵ�����ʽ������������Ҳ��û�������

    //�µİ���
    if(cn_key_down == state)
    {
        server->key_state = server->key_state &(~(cn_key_up << key_translate));
    }
    else
    {
        server->key_state = server->key_state &(~(cn_key_up << key_translate));
    }
    server->key_update_list = server->key_update_list ^(1<<key_translate);
    //��0����Ǳ�����1���ȡ��
    return;//����
}

// =============================================================================
// ��������:����bit������ӳ���565��ɫ�ĸ�ʽ������
// �������:dest�����ݴ洢Ŀ�ĵ�ַ��src������ת��Դ��data1,bit1����Ӧ�����ݣ�
//         data0 bit0����Ӧ�����ݣ�coutԴ���ݵ��ֽ���,times,ӳ��Ĵ���
// ���������
// ����ֵ:
// ˵��:
// =============================================================================
static void changeB2W(u8* dest, u8* src, u32 count, u16 data1, u16 data0,u16 times)
{
   u8 i=0,j=0;
   u8  data;
   u16 mapTimes = 0;
   for(i=0;i<count;i++)//�ֽ���
   {
      data=*((u8 *)(src+i));
      for(j=0;j<8;j++)//�Լ�λ����
      {
         if(data&(1<<(7-j)))
         {
           *((u16 *)(dest+i*16+j*2))=data1;
         }
         else
         {
             *((u16 *)(dest+i*16+j*2))=data0;
         }

         mapTimes++;
         if(mapTimes == times)//ӳ���������
         {
             return;
         }
      }
    }
}
// =============================================================================
// ��������:��24λ888��ʽ������ת��Ϊ16��565��ʽ�����ݣ�
// �������:cout��ʾҪת���Ĵ���
// ���������
// ����ֵ:
// ˵��:RGB,R�ڸ�λ
// =============================================================================
static void change24to16(u8* dest, u8* src, u32 count)
{
   u16 i=0;
   u8 *src_buf;
   u16 *dst_buf;
   src_buf = src;
   dst_buf =(u16 *)dest;

   for(i=0; i < count; i++)//�ֽ���
   {
       *dst_buf = ((*(src_buf))&0x1F)|(((*(src_buf+1))&0x3F)<<0x05)|\
       (((*(src_buf+1))&0x1F)<<0x0B);//(bgr˳��)

       src_buf += 3;
       dst_buf++;
      //  debug_printf("src buf =%08x  dst_buf=%08x\n",src_buf,dst_buf);
   }
}

// =============================================================================
// ��������:��bitmap�������ڴ�
// �������:cout��ʾҪת���Ĵ���
// ���������
// ����ֵ:
// ˵��:��Ҫ���ƣ���Ҫ������frambuf�ķ�Χ����Ȼ�Ļ���������
// =============================================================================
void vnc_cp_bitmap2serverbuf(rfbScreenInfoPtr server,\
        struct tagRectBitmap *bitmap,u16 x, u16 y)
{
    u32 i = 0;//ָʾ�߶�
    u32 bm_height = 0;
    u32 bm_width = 0;
    u32 bm_linebytes = 0;
    u32 fm_linebytes = 0;
    u8 *frambuf;
    u8 *bitbuf;
    u32 offset = 0;

    if(NULL == bitmap->bm_bits)
    {
        return;
    }
    //��������ʼ��Ӧ����framebuf�ķ�Χ��
    //��ֹ�����X �� Y
    if((x >= server->width)||(y >= server->height))
    {
        return;//��Ȼ��������ô��ʲô������
    }
    //��ֹ�������磬��Ȼ���ĺܲ�
    if((x+bitmap->width) > server->width)
    {
        bm_width = server->width - x;
    }
    else
    {
        bm_width = bitmap->width;
    }
    if((y + bitmap->height) > server->height)
    {
        bm_height = server->height - y;
    }
    else
    {
        bm_height = bitmap->height;
    }

    bm_linebytes = bitmap->linebytes;
    fm_linebytes = ((server->rfbServerFormat.bitsPerPixel+7)/8)*server->width;
    bitbuf = bitmap->bm_bits ;
    offset = fm_linebytes*y + x*((server->rfbServerFormat.bitsPerPixel+7)/8);
    frambuf = (u8 *)(server->frameBuffer + offset);
    for(i=0; i < bm_height; i++)
    {
        if(CN_SYS_PF_GRAY1 == bitmap->PixelFormat)//��bit��map
        {
            changeB2W(frambuf, bitbuf, bm_linebytes, 0x4444,0x9999, bm_width);
        }
        else if(CN_SYS_PF_RGB888 == bitmap->PixelFormat)//24λ��map
        {
            change24to16(frambuf, bitbuf,bm_width);
        }
        frambuf += fm_linebytes;
        bitbuf += bm_linebytes;
    }
}
static struct tagRectBitmap  key_map[cn_vnc_key_num];
static struct tagRectBitmap  keyboardmap;

u8 keydown_map[32]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
        0x00,0x00,0x00,0x00,0x00,0x70,0x0E,0x38,0x1C,0x1C,0x38,0x0E,0x70,0x07,\
        0xE0,0x03,0xC0,0x01,0x80,0x00,0x00,0x00,0x00};

u8 keyup_map[32]={0x00,0x00,0x00,0x00,0x01,0x80,0x03,0xC0,0x07,0xE0,0x0E,0x70,\
        0x1C,0x38,0x38,0x1C,0x70,0x0E,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
        0x00,0x00,0x00,0x00,0x00,0x00};

u8 keyleft_map[32]={0x00,0x00,0x00,0x80,0x01,0x80,0x03,0x80,0x07,0x00,0x0E,\
        0x00,0x1C,0x00,0x38,0x00,0x38,0x00,0x1C,0x00,0x0E,0x00,0x07,0x00,0x03,\
        0x80,0x01,0x80,0x00,0x80,0x00,0x00};

u8 keyright_map[32]={0x00,0x00,0x01,0x00,0x01,0x80,0x01,0xC0,0x00,0xE0,0x00,\
        0x70,0x00,0x38,0x00,0x1C,0x00,0x1C,0x00,0x38,0x00,0x70,0x00,0xE0,0x01,\
        0xC0,0x01,0x80,0x01,0x00,0x00,0x00};

u8 keyenter_map[32]={0x00,0x00,0x00,0x00,0x7F,0x42,0x7F,0x62,0x60,0x72,0x60,\
        0x5A,0x7F,0x5A,0x5F,0x5A,0x7F,0x5A,0x60,0x4E,0x60,0x46,0x60,0x46,0x7F,\
        0x42,0x7F,0x42,0x00,0x00,0x00,0x00};

u8 keyesc_map[32]={0x00,0x00,0x00,0x00,0x7F,0x0E,0x7F,0x11,0x60,0x20,0x60,0x20,\
        0x7F,0x20,0x5F,0x20,0x7F,0x20,0x60,0x20,0x60,0x20,0x60,0x20,0x7F,0x11,\
        0x7F,0x0F,0x00,0x00,0x00,0x00};

u8 keyf1_map[32]={0x00,0x00,0x3F,0xBC,0x1F,0x18,0x18,0x18,0x18,0x18,0x18,0x18,\
        0x1F,0x98,0x1F,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x10,0x18,\
        0x00,0x00,0x00,0x00,0x00,0x00};

u8 keyf2_map[32]={0x00,0x00,0x3F,0xBF,0x1F,0x01,0x18,0x01,0x18,0x01,0x18,0x01,\
        0x1F,0x9F,0x1F,0x3E,0x18,0x20,0x18,0x20,0x18,0x20,0x18,0x20,0x18,0x21,\
        0x18,0x3F,0x10,0x00,0x00,0x00};
u8 keyfn_map[32]={0x00,0x00,0x3F,0xA1,0x3F,0x31,0x30,0x29,0x30,0x2D,0x30,0x2F,\
        0x3F,0xAF,0x3F,0x2F,0x30,0x2F,0x30,0x27,0x30,0x23,0x30,0x21,0x30,0x21,\
        0x20,0x21,0x00,0x00,0x00,0x00};//��ϼ���ť 0x80
// =============================================================================
// ��������:Ϊbitmap�������ͼ�Σ�ͼ�������ڴ�
// �������:server,������
// ���������
// ����ֵ:
// ˵��:
// =============================================================================
static void vnc_init_key_bitmap(struct tagRectBitmap *bitmap)
{
    u8 i = 0;

//��ʼ��Ϊ8����bitλͼ
    bitmap[0].bm_bits = keyleft_map;
    bitmap[1].bm_bits = keyup_map;
    bitmap[2].bm_bits = keyright_map;
    bitmap[3].bm_bits = keydown_map;
    bitmap[4].bm_bits = keyenter_map;
    bitmap[5].bm_bits = keyesc_map;
    bitmap[6].bm_bits = keyf1_map;
    bitmap[7].bm_bits = keyf2_map;
//���ӹ��ܼ�
    bitmap[8].bm_bits = keyfn_map;
    for(i = 0; i < cn_vnc_key_num; i++)
    {
        bitmap[i].PixelFormat = CN_SYS_PF_GRAY1;//��bit��ɫ
        bitmap[i].width = cn_vnc_key_width;
        bitmap[i].height = cn_vnc_key_height;
        bitmap[i].linebytes = 2;//
    }
}
// =============================================================================
// ��������:Ϊbitmap�������ͼ�Σ�ͼ�������ļ�
// �������:server,������
// ���������
// ����ֵ:
// ˵��:bitmapde bits���ڴ��ڴ˶�̬����
// =============================================================================
static void vnc_init_key_bitmap_from_file(struct tagRectBitmap *bitmap)
{
    u8 i = 0;
    char file_name[30] = "d:\\keymap\\keymap0.bmp";
    u8 file_namelen = strlen(file_name);
    for(i =0; i < cn_vnc_key_num; i++)
    {

        vnc_get_image_from_bmp(file_name,&(bitmap[i]));
        file_name[file_namelen-5]++;//��0��ʼ�����ļ�����Ӧ��key���б�,.bmp��׺
    }
}

static void vnc_init_keyboardbackground_from_file(struct tagRectBitmap *bitmap)
{

    char file_name[30] = "d:\\keymap\\keyboard.bmp";
    vnc_get_image_from_bmp(file_name,&keyboardmap);
}

//֮��������ȫ�ֱ�������Ϊ�˺������Ҽ��Ͱ�����λ�ø�����
static u8 vnc_line_keynum = 0;//ÿ�����ŵ�key��Ŀ
static u8 vnc_keyline_num = 0;//һ���ŵ�key������
static u8 vnc_key_num = 0;// Ҫ��ʾ��key����Ŀ
static u16 vnc_space_h_left = 0;//���ð�����ʣ������ء���ˮƽ����
static u16 vnc_space_v_left = 0;//����key��ʣ�������-��ֱ����
// =============================================================================
// ��������:ʵ��vncserver�Լ��̵Ŀ��ӻ��ĳ�ʼ��
// �������:server,������
// ���������
// ����ֵ:
// ˵��:���̵�ֵĿǰ���ǵ���ֵ���������Ƕ��ֵ�ļ���--TODO
// =============================================================================
bool_t vnc_keyboard_visual_init(rfbScreenInfoPtr server)
{
    u8 i=0;
    u8 keyline_num_visual = 0;//һ���ŵ�key������
    u16 x = 0;
    u16 y = 0;
    //��ʼ��bitmap
    server->key_state = 0;
    server->key_update_list = 0;

    //��ʼ������̵ı���
    //���̱�������Ҫ���õİ�
    vnc_init_keyboardbackground_from_file(&keyboardmap);
    vnc_cp_bitmap2serverbuf(server,&keyboardmap,0, server->height- cn_vnc_keyboard_height);
    free(&(keyboardmap.bm_bits));
    debug_printf("set soft keyboard back ground success!\n");



    //vnc_init_key_bitmap(key_map);//���ڴ��ж�ȡ
    vnc_init_key_bitmap_from_file(key_map);//��bmp�ļ��л�ȡ

    vnc_line_keynum = (server->width)/(cn_vnc_key_width + cn_vnc_keybotton_h_margin);
    keyline_num_visual = (cn_vnc_keyboard_height)/\
            (cn_vnc_key_height + cn_vnc_keybotton_v_margin);
    if((vnc_line_keynum ==0)||(keyline_num_visual == 0))
    {
        debug_printf("#init softkey board failed:no space left for show key!\n");
        return false;
    }
//����ʵ�ʵİ�����Ŀ���е���,��֤���������������λ��
    vnc_keyline_num = (cn_vnc_key_num +vnc_line_keynum -1)/ vnc_line_keynum;
    if(vnc_keyline_num > keyline_num_visual )
    {
        vnc_keyline_num = keyline_num_visual;
        vnc_key_num = vnc_keyline_num * vnc_line_keynum;
    }
    else
    {
        vnc_key_num = cn_vnc_key_num;
    }
    vnc_space_h_left =(u16)(server->width - \
            vnc_line_keynum*(cn_vnc_key_width + cn_vnc_keybotton_h_margin));
    vnc_space_v_left =(u16)(cn_vnc_keyboard_height - \
            vnc_keyline_num*(cn_vnc_keybotton_v_margin + cn_vnc_key_height));

    x = vnc_space_h_left/2;
    y = server->height -cn_vnc_keyboard_height + (vnc_space_v_left+1)/2;

    for(i = 0; i < vnc_key_num; i++)
    {
        if((x+ cn_vnc_key_width + cn_vnc_keybotton_h_margin) > server->width)
        {
            y += cn_vnc_key_height + cn_vnc_keybotton_v_margin;
            x = vnc_space_h_left/2;
        }//���д���
        //��֤����������botton������
        x = x + (cn_vnc_keybotton_h_margin+1)/2;
        vnc_cp_bitmap2serverbuf(server,&key_map[i],x, y);
        x += cn_vnc_key_width + (cn_vnc_keybotton_h_margin+1)/2;
    }
    return true;
}

#define cn_invalid_keynumber 0xFF//ĿǰӦ�û�û��255�ļ��̰�
// =============================================================================
// ��������:���������������̵�key
// �������:����x y λ��
// ���������
// ����ֵ:key��number�ţ������keyֵ�����key tab����ӳ��
// ˵��:
// =============================================================================
u8 findsoftkey_by_point(rfbScreenInfoPtr server, u16 x, u16 y)
{
    u8 key_number = cn_invalid_keynumber;
    u8 v_number = 0;//��ֱ��
    u8 h_number = 0;//ˮƽ��

    v_number = (y + cn_vnc_keyboard_height-server->height - (vnc_space_v_left+1)/2)/\
            (cn_vnc_key_height + cn_vnc_keybotton_v_margin);
    h_number = (x - (vnc_space_h_left+1)/2)/\
            (cn_vnc_keybotton_h_margin + cn_vnc_key_width);

    key_number = v_number*vnc_line_keynum + h_number;

    return key_number;
}
// =============================================================================
// ��������:ʵ�ֿͻ��˶�VNCdisplay������̵Ĳ���
// �������:server,��������msg �����Ϣ
// ���������
// ����ֵ:
// ˵��:�����������ͣ���ĵط�������̵�����ʱ������ô˷������д���
// =============================================================================
void vnc_softkeyboard(rfbScreenInfoPtr server, rfbClientToServerMsg msg)
{

    u8 key_number = 0;
    u8 state = 0;
    u8  key = 0;

    if(msg.pe.buttonMask & (0x01))//����
    {
        state = cn_key_down;
    }
    else
    {
        state = cn_key_up;
    }
    key_number = findsoftkey_by_point(server, msg.pe.x, msg.pe.y);

    if(key_number >= vnc_key_num)
    {
        return;//������Χ���϶�����Ҫ����
    }
//  //�ж��Ƿ���ͬһ���������ͣ��
//  if( (((u8)(server->key_state >> key_number)&0x01)) == state)//�Ƚϱ�־λ
//  {
//      if(state == cn_key_up)//�������������ֻ��������·������
//      {
//          return ;//ͬһ�������������up�ǲ����ܵ����飬�������key��û�ж���
//      }
//      else
//      {
//      //��ΰ��£���Ӧ���޸�changed״̬����Ϊ�˰���һֱ�ڰ���״̬������Ӧ�÷���key
//      }
//  }
//  else//�˰���״̬�����˱仯
//  {
//      if(cn_key_down == state)
//      {
//          server->key_state = server->key_state |(cn_key_down << key_number);
//            debug_printf("key_%d down \n",key_number);
//      }
//      else
//      {
//          server->key_state = server->key_state &(~(cn_key_down << key_number));
//            debug_printf("key_%d  up \n",key_number);
//      }
//      server->key_update_list = server->key_update_list ^(1<<key_number);
//  }
//  //���������Ϣ��װ�ɼ��̰�����Ϣ���д���
//  key = cyg_keyboard[key_number];

    vnc_input_key_message(server,key_number, state);
}
// =============================================================================
// ��������:ʵ�ֿͻ��˵������Ӧ
// �������:server,��������msg �����Ϣ
// ���������
// ����ֵ:
// ˵��:
// =============================================================================
void vncclient_pointevent(rfbScreenInfoPtr server, rfbClientToServerMsg msg)
{
    if(msg.pe.y > (server->height - cn_vnc_keyboard_height))//�������
    {
        vnc_softkeyboard(server,msg);
    }
    else//��Ļ��ʾ��
    {
        if(msg.pe.buttonMask & (0x01))//����²����Ǵ���һ��
        {
            vnc_input_touch_message(msg.pe.x,msg.pe.y,0);
        }

    }
}
// =============================================================================
// ��������:���server�˵�ĳ��key�ǲ���Ҫ�����Ӿ�����
// �������:server,������, check_bit��������λ����key���ڸü����µĺ��룩
// ���������
// ����ֵ:true����Ҫ���£�false������Ҫ����
// ˵��:
// =============================================================================
bool_t vnc_check_key_changed(rfbScreenInfoPtr server, u8 check_bit)
{
    if(server->key_update_list & (0x01<<check_bit))
    {
        return true;
    }
    else
    {
        return false;
    }
}
// =============================================================================
// ��������:���sever�˵����ж��ٸ�key��Ҫ����
// �������:server,������
// ���������
// ����ֵ:��Ҫ���µ�key�ĸ���
// ˵��:
// =============================================================================
u8 vnc_get_keychanged_numbers(rfbScreenInfoPtr server)
{
    u8 ret = 0;
    u8 i = 0;

    if(server->key_update_list == 0)//û�и���
    {
        return ret;
    }
    for(i = 0; i < cn_vnc_key_num; i++)
    {
      if( vnc_check_key_changed(server, i))
      {
        ret++;
      }
    }
    return ret;
}
// =============================================================================
// ��������:�õ��ض�key��λ����Ϣ
// �������:server,������, key_number,�õ�key_number��λ����Ϣ,x,y �洢λ��
// ���������
// ����ֵ:
// ˵��:
// =============================================================================
void vnc_get_keyposition_by_number(rfbScreenInfoPtr server, u8 key_number,\
        u16 *x, u16 *y)
{
    u8  i = 0;

    i = key_number % vnc_line_keynum;
    *x = (cn_vnc_key_width + cn_vnc_keybotton_h_margin )*i+ \
            (vnc_space_h_left+1)/2 + (cn_vnc_keybotton_h_margin +1)/2;

    i = key_number / vnc_line_keynum;
    *y = server->height - cn_vnc_keyboard_height +\
    (cn_vnc_key_height + cn_vnc_keybotton_v_margin)*i + (vnc_space_v_left+1)/2;

    return;
}
// =============================================================================
// ��������:��server��ĳ��keyָ������Ӧ��״̬���Ӿ�ģʽ����䵽��Ӧkey���Դ棩
// �������:server,������, key_number,�ü��̵ĵڼ���key��state�����º͵���״̬��
// ���������
// ����ֵ:��
// ˵��:Ŀǰ�����Ը�key���Դ��������Ӽ���������Ľ��ɿ���������ͼƬ--TODO
// =============================================================================
void vnc_set_keymap2server(rfbScreenInfoPtr server, u8 key_number, u8 state)
{
//�������ų�ʼ����ʱ����ʵ�Ѿ���֤�˲���д��sever��buf��Χ
//��ʱ����
    u8  pixel_bytes = 0;
    u8  i = 0;
    u16 key_height= 0;
    u16 key_width = 0;
    u16 startpoint_x;
    u16 startpoint_y;
    u32 server_linebytes = 0;
    u8  *buf;
    //��ȡλ��
    vnc_get_keyposition_by_number(server,key_number,&startpoint_x,&startpoint_y);
    //�ҵ�bufλ��
    pixel_bytes = server->rfbServerFormat.bitsPerPixel / 8;
    server_linebytes = (server->width)*pixel_bytes;
    buf = (u8 *)server->frameBuffer + startpoint_x*pixel_bytes +\
            startpoint_y*server_linebytes;

   //�򵥴�����
    for(key_height = 0; key_height < cn_vnc_key_height; key_height ++)
    {
        for(key_width = 0; key_width <cn_vnc_key_width*pixel_bytes; key_width ++)
        {
            *(buf + key_width) = ~(*(buf + key_width));//������ȡ�����߼���Ч����--TODO
        }
        buf+=server_linebytes;
    }
}
// =============================================================================
// ��������:��server�ķ����ı��keyȫ��ˢ�½�����
// �������:server,������
// ���������
// ����ֵ:��
// ˵��:����server��key��״̬��¼��changgelist�����У��ú���Ӧ������Ҫˢ��cl��ʱ��
//      ���ã�һ����ˢ�����е�key��״̬
// =============================================================================
void vnc_changed_keyboard_visual(rfbScreenInfoPtr server)
{
    u8 i = 0;
    u8 state = 0;
    if(server->key_update_list)//û����Ҫˢ�µ�Ӧ������ĵ�����
    {
        for(i = 0; i < cn_vnc_key_num; i++)
        {
            if(vnc_check_key_changed(server, i))//��key��Ҫˢ��
            {
               state = (server->key_state >> i) & 0x01;
               vnc_set_keymap2server(server, i, state);
            }
        }
    }
}
// =============================================================================
// ��������:��λserver��key statlist
// �������:server,������
// ���������
// ����ֵ:��
// ˵��:
// =============================================================================
void vnc_reset_keyboard_statelist(rfbScreenInfoPtr server)
{
    server->key_update_list = 0;
}

// =============================================================================
// ��������:ж������
// �������:server,������
// ���������
// ����ֵ:��
// ˵��:
// =============================================================================
bool_t uninstall_vnc_keyboard(void)
{
    return Stddev_UnInstallDevice(cn_vnc_keyboard_name);
}

bool_t uninstall_vnc_touchboard(void)
{
    return Stddev_UnInstallDevice(cn_vnc_touchlcd_name);
}

#endif




