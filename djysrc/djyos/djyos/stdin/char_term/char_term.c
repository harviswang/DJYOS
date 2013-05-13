//----------------------------------------------------
//Copyright (C), 2004-2009,  lst.
//��Ȩ���� (C), 2004-2009,   lst.
//����ģ��: ������
//���ߣ�lst
//�汾��V1.0.1
//�ļ�����: ����������CPUֱ����صĴ��롣
//����˵��:
//�޶���ʷ:
//2. ����: 2009-04-24
//   ����: lst
//   �°汾��: V1.0.1
//   �޸�˵��: ɾ����һЩΪdlsp�汾׼���Ķ���
//1. ����: 2009-01-04
//   ����: lst
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include "os_inc.h"
#include "cpu_peri.h"
#include "char_term.h"
#include "shell_fs.h"

#include <string.h>
#include <stdarg.h>

static s32 s32g_char_term_id;
djy_handle_t pg_char_term_lhdl;
struct semaphore_LCB *semp_char_term_hdl;

void term_scan(void)
{
    struct char_termineral_msg msg;
    char command[257];
    u8 reads;
    u8 offset;
    while(1)
    {
        do
        {
            //�������豸
            reads = dev_read_left(pg_char_term_lhdl,0,(ptu32_t)command,255*mS);
            for(offset = 0; offset < reads; offset++)
            {
                msg.time = djy_get_time_tick();
                msg.input_char = (u32)command[offset];
                stdin_input_msg(s32g_char_term_id,(u8*)&msg,sizeof(msg));
            }
        } while (reads ==255); //ֱ�����е����ݶ�������
        //ִ�б�����¼��������������ڱ�ͬ�����¼����;��Ǳ��¼����¼����ͣ�
        //����ͬ���������ǣ��ٴη����������¼���
        //���� ��1����ʾֻҪ����һ���¼��Ϳ�����
        //����cn_timeout_forever��ʾ���һֱ�������������õȴ�
        //�����¼����͵���ͬ�����μ��������߲���ϵͳ��Ƕ��ʽϵͳ��ơ��ĵ�5.3��
        djy_wait_evtt_pop(djy_my_evtt_id(),1,cn_timeout_forever);
    }
}

//----�ն�ģ���ʼ������-------------------------------------------------------
//����: ��ʼ������ģ��
//����: ��
//����: 1
//-----------------------------------------------------------------------------
ptu32_t module_init_char_term(ptu32_t para)
{
    u16 term_scan_evtt;
    
    s32g_char_term_id = stdin_install_device("char_term", enum_char_termineral);
    if(s32g_char_term_id == -1)
        goto exit_instell_device;
    semp_char_term_hdl = semp_create(1, 1, NULL);
    if (semp_char_term_hdl == NULL)
    	goto exit_create_semp;
    term_scan_evtt = djy_evtt_regist(enum_correlative,          //�������¼�
                                cn_prio_real,       //Ĭ�����ȼ�
                                0,                  //�������������mark����Ч
                                1,                  
                                term_scan,          //��ں���
                                1024,               //ջ�ߴ�
                                "term_scan"         //�¼�������
                                );
    if(term_scan_evtt == cn_invalid_evtt_id)
    {
        goto exit_install_evtt;
    }
    if(djy_event_pop(term_scan_evtt,false,NULL,0,&para,sizeof(ptu32_t),0,0)
                        == (uint16_t)cn_invalid_event_id)
    {
        goto exit_pop_event;
    }

    pg_char_term_lhdl = dev_open_left(cfg_stdin_device,0);
    if(pg_char_term_lhdl == NULL)
    {
        goto exit_open_uart;
    }
    //����uart_port->evtt_right_writeΪshell_evtt
    dev_ctrl_left(pg_char_term_lhdl,cn_uart_connect_recv_evtt,term_scan_evtt,0);
    dev_ctrl_left(pg_char_term_lhdl,cn_uart_dma_used,0,0);
    //�����жϺ�Ϊ0
    dev_ctrl_left(pg_char_term_lhdl,cn_uart_start,0,0);
    //���ô��ڲ�����Ϊ9600�����ڳ�ʼ����ʱ��Ĭ������9600��������
    //�����޸ĳ�����Ĳ�����
    dev_ctrl_left(pg_char_term_lhdl,cn_uart_set_baud,57600,0);
    printf_ng("\r\n��ӭʹ�ö����߲���ϵͳ\r\n");
    printf_ng("  �汾����ʱ�䣺%s %s\r\n", __DATE__, __TIME__);
    printf_ng(">");
    return s32g_char_term_id;
    
exit_open_uart:
exit_pop_event:
    djy_evtt_unregist(term_scan_evtt);
exit_install_evtt:
    stdin_uninstall_device("char_term");
exit_instell_device:
	semp_delete(semp_char_term_hdl);
exit_create_semp:
    return -1;
}

s32 djy_getchar(void)
{
    struct input_device_msg *input_msg;
	u32 ch;
    input_msg = (struct input_device_msg *)djy_get_event_para(NULL);
    if(input_msg != NULL)
        return input_msg->input_msg.char_termineral.input_char;
    else
    {
        do
        {
            djy_wait_evtt_pop(djy_my_evtt_id(),1,20*mS);
            input_msg = (struct input_device_msg *)djy_get_event_para(NULL);
            djy_task_completed(0);
        }while(input_msg == NULL);
        ch = input_msg->input_msg.char_termineral.input_char;
    }
    return ch;
}

s32 djy_gets(char buf[])
{
//    char ch;
//
//    buf[0] = '\0';
//	do
//	{
//		ch = (char)djy_getchar();
//		if ((ch != '\r') && (ch != '\n'))
//		{
//			buf++ = ch;
//		}
//		else
//		{
//			buf++ = '\0';
//			return 1;
//		}
//	} while (1);
//
	return 0;
}

//----��ʽ����---------------------------------------------------------
//����: ����ʽ�����ַ�������C�⺯�����ƣ���������������ࡣ
//����: const char *fmt, ...
//����:
//ע�⣺��ǰ�����ڻ�ȡ�ַ�������������ȫδʵ�֣�
//-----------------------------------------------------------------------------
s32 djy_scanf(const char *format, char *buf)
{
    char ch;
    u8 chars = 0;

    semp_pend(semp_char_term_hdl, cn_timeout_forever);
    stdin_set_focus("char_term",djy_my_evtt_id());
    while (1)
    {
    	ch = (char)djy_getchar();
    	if ((ch == '\r') || (ch == '\n'))
    	{
            buf[chars] = '\0';
            printf_ng("\r\n");
            break;
    	}
    	else
    	{
    		if (ch == 8)	// Backspace
    		{
                if(chars > 0)
                {
                    chars --;
                    printf_ng("\b \b");
                }
    		}
    		else
    		{
    			if(chars < 255)  //������32�ֽڣ������ı�����
				{
    			    putchar_ng(ch);
					buf[chars] = ch;
					chars++;
				}
    		}

    	}
    }
    semp_post(semp_char_term_hdl);
    return chars;
}

void putchar_ng(char c)
{
    dev_write_left(pg_char_term_lhdl,(ptu32_t)&c,0,1);
}

//----��ֵת�����ַ���---------------------------------------------------------
//����: ����ֵdת����Ϊָ�����������ַ���
//����: d��Ҫת������ֵ
//      buf�����ת���õ��ַ����Ļ�����
//      scale��ת���Ľ���
//����: 1
//������d=5628, scale=10 �������Ϊ buf="5628"
//-----------------------------------------------------------------------------
void itoa_ng(int d, char *buf, int scale)
{
	unsigned char str_inv[16];	// ���������
	unsigned int ud;	// d���޷��ű�ʾ
	char *str = buf;
	u8 i, j, k;

	i = 0;
	// ������ȥ��
	if (d < 0)
	{
		ud = (unsigned int)(-d);
		str_inv[i++] = '-';
	}
	else
	{
		ud = (unsigned int)d;
	}

	// 16������ת��
	if (scale == 16)
	{
		do	// ���4λ�Ƿ���ֵ
		{
			str_inv[i] = ud & 0x0F;	// ��ĩ4λ��ֵ
			ud >>= 4;	// ����4λ

			if ((str_inv[i] >= 0) && (str_inv[i] <= 9))
			{
				str_inv[i] += '0';
			}
			else if ((str_inv[i] >= 0x0A) && (str_inv[i] <= 0x0F))
			{
				str_inv[i] += ('a'-0x0A);
			}

			i++;
		} while (ud);
		i--;	// �˸�����λ��

		// �������
		if (str_inv[0] == '-')
		{
			*str++ = '-';
			j = 1;
		}
		else
		{
			j = 0;
		}

		k = i-j;	// ����λ��
		k += 1;		// ѭ����ȡ����
		for (; k; k--)
		{
			*str++ = str_inv[i--];
		}
		*str = '\0';	// ������
	}
	else if (scale == 10)
	{
		// ����ת��
		do
		{
			str_inv[i++] = (ud % 10) + '0';
			ud /= 10;
		} while (ud);
		i--;	// �˸�����λ��

		// �������
		if (str_inv[0] == '-')
		{
			*str++ = '-';
			j = 1;
		}
		else
		{
			j = 0;
		}

		k = i-j;	// ����λ��
		k += 1;		// ѭ����ȡ����
		for (; k; k--)
		{
			*str++ = str_inv[i--];
		}
		*str = '\0';	// ������
	}
}

//----��ʽ��ӡ---------------------------------------------------------
//����: ����ʽ��ӡ�ַ�������C�⺯�����ƣ���������������ࡣ
//����: const char *fmt, ...
//����: 1
//-----------------------------------------------------------------------------
int printf_ng(const char *fmt, ...)
{
	const char *s;
	int d;
	u32 len, decbits = 0;
	char buf[16];
	char fillch = ' ';
	va_list argp;	// ��������
	va_start(argp, fmt);

	while (*fmt)
	{
		decbits = 0;
		fillch = ' ';
		if (*fmt != '%')
		{
			putchar_ng(*fmt++);
			continue ;
		}

		fmt++;
		// ����ַ�
		if ((*fmt == ' ') || (*fmt == '0'))
		{
			fillch = *fmt;
			fmt++;
		}
		// ָ����ӡ����
		while (('0' < *fmt) && (*fmt <= '9' ))
		{
			decbits = decbits*10 + (*fmt - '0');
			fmt++;
		}

		switch (*fmt)
		{
		case 's':
			s = va_arg(argp, const char *);
			len = strlen(s);
			if (len<decbits)
			{
				for (; len<decbits; decbits--)
				{
					putchar_ng(' ');
				}
			}
			for (; *s; s++)
			{
				putchar_ng(*s);
			}
			break;
		case 'd':	// 10������
			d = va_arg(argp, int);
			itoa_ng(d, buf, 10);
			len = strlen(buf);
			if (len<decbits)
			{
				for (; len<decbits; decbits--)
				{
					putchar_ng(fillch);
				}
			}
			for (s=buf; *s; s++)
			{
				putchar_ng(*s);
			}
			break;
		case 'x':	// 16������
			d = va_arg(argp, int);
			itoa_ng(d, buf, 16);
			len = strlen(buf);
			if (len<decbits)
			{
				for (; len<decbits; decbits--)
				{
					putchar_ng('0');
				}
			}
			for (s=buf; *s; s++)
			{
				putchar_ng(*s);
			}
			break;
			//�����������
		default :
			putchar_ng(*fmt);
			break;
		}
		fmt++;
	}
	va_end(argp);

	return 1;	// ԭ��Ҫ����
}

//----���ַ���ת���ɳ�����---------------------------------------------------------
//����: ���ַ���ת���ɳ����ͣ�δ׼����ctype����������ʹ��atol��isdigit֮��ĺ���
//����:
//����:
//-----------------------------------------------------------------------------
u32 __sh_atol(char *s)
{
    ufast_t offset;
    u32 result = 0;
    if((s[0] == '0') && ((s[1] == 'x') || (s[1] == 'X')))   //16����
    {
        offset = 2;
        while(s[offset] == '0')
            offset++;
        while(s[offset] != '\0')
        {
            if((s[offset]>='0')&&(s[offset]<= '9'))         //0~9��Χ
                result = (result<<4) + s[offset] - 0x30;
            else if((s[offset]>='A')&&(s[offset]<= 'F'))    //A~F��Χ
                result = (result<<4) + s[offset] - 0x37;
            else if((s[offset]>='a')&&(s[offset]<= 'f'))    //a~f��Χ
                result = (result<<4) + s[offset] - 0x57;
            else                       //�Ƿ��ַ�
                return result;
            offset++;
        }
    }else       //10����
    {
        offset = 0;
        while(s[offset] == '0')
            offset++;
        while(s[offset] != '\0')
        {
            if((s[offset]>='0')&&(s[offset]<= '9'))         //0~9��Χ
                result = result*10 + s[offset] - 0x30;
            else                       //�Ƿ��ַ�
                return result;
            offset++;
        }
    }
    return result;
}

