//----------------------------------------------------
//Copyright (C), 2004-2009,  lst.
//版权所有 (C), 2004-2009,   lst.
//所属模块: 调度器
//作者：lst
//版本：V1.0.1
//文件描述: 调度器中与CPU直接相关的代码。
//其他说明:
//修订历史:
//2. 日期: 2009-04-24
//   作者: lst
//   新版本号: V1.0.1
//   修改说明: 删除了一些为dlsp版本准备的东西
//1. 日期: 2009-01-04
//   作者: lst
//   新版本号: V1.0.0
//   修改说明: 原始版本
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
            //读串口设备
            reads = dev_read_left(pg_char_term_lhdl,0,(ptu32_t)command,255*mS);
            for(offset = 0; offset < reads; offset++)
            {
                msg.time = djy_get_time_tick();
                msg.input_char = (u32)command[offset];
                stdin_input_msg(s32g_char_term_id,(u8*)&msg,sizeof(msg));
            }
        } while (reads ==255); //直到所有的数据都被读出
        //执行本句后，事件将被阻塞，由于被同步的事件类型就是本事件的事件类型，
        //所以同步条件就是：再次发生本类型事件。
        //参数 ‘1’表示只要发生一次事件就可以了
        //参数cn_timeout_forever表示如果一直不发生，就永久等待
        //关于事件类型弹出同步，参见《都江堰操作系统与嵌入式系统设计》的第5.3节
        djy_wait_evtt_pop(djy_my_evtt_id(),1,cn_timeout_forever);
    }
}

//----终端模块初始化函数-------------------------------------------------------
//功能: 初始化调试模块
//参数: 无
//返回: 1
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
    term_scan_evtt = djy_evtt_regist(enum_correlative,          //关联型事件
                                cn_prio_real,       //默认优先级
                                0,                  //虚拟机保留数，mark型无效
                                1,                  
                                term_scan,          //入口函数
                                1024,               //栈尺寸
                                "term_scan"         //事件类型名
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
    //设置uart_port->evtt_right_write为shell_evtt
    dev_ctrl_left(pg_char_term_lhdl,cn_uart_connect_recv_evtt,term_scan_evtt,0);
    dev_ctrl_left(pg_char_term_lhdl,cn_uart_dma_used,0,0);
    //设置中断号为0
    dev_ctrl_left(pg_char_term_lhdl,cn_uart_start,0,0);
    //设置串口波特率为9600，串口初始化的时候默认设置9600，在这里
    //可以修改成任意的波特率
    dev_ctrl_left(pg_char_term_lhdl,cn_uart_set_baud,57600,0);
    printf_ng("\r\n欢迎使用都江堰操作系统\r\n");
    printf_ng("  版本编译时间：%s %s\r\n", __DATE__, __TIME__);
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

//----格式输入---------------------------------------------------------
//功能: 按格式输入字符串，与C库函数类似，但功能弱化了许多。
//参数: const char *fmt, ...
//返回:
//注意：当前仅用于获取字符串，其它功能全未实现！
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
    			if(chars < 255)  //命令串最长是32字节，超长的被忽略
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

//----数值转换成字符串---------------------------------------------------------
//功能: 将数值d转换成为指定进制数的字符串
//参数: d，要转换的数值
//      buf，存放转换得的字符串的缓冲区
//      scale，转换的进制
//返回: 1
//范例：d=5628, scale=10 运算后结果为 buf="5628"
//-----------------------------------------------------------------------------
void itoa_ng(int d, char *buf, int scale)
{
	unsigned char str_inv[16];	// 逆序的数组
	unsigned int ud;	// d的无符号表示
	char *str = buf;
	u8 i, j, k;

	i = 0;
	// 将符号去掉
	if (d < 0)
	{
		ud = (unsigned int)(-d);
		str_inv[i++] = '-';
	}
	else
	{
		ud = (unsigned int)d;
	}

	// 16进制数转换
	if (scale == 16)
	{
		do	// 最高4位是否有值
		{
			str_inv[i] = ud & 0x0F;	// 最末4位的值
			ud >>= 4;	// 右移4位

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
		i--;	// 退格到数据位来

		// 倒序回来
		if (str_inv[0] == '-')
		{
			*str++ = '-';
			j = 1;
		}
		else
		{
			j = 0;
		}

		k = i-j;	// 多少位数
		k += 1;		// 循环读取次数
		for (; k; k--)
		{
			*str++ = str_inv[i--];
		}
		*str = '\0';	// 结束符
	}
	else if (scale == 10)
	{
		// 逆序转换
		do
		{
			str_inv[i++] = (ud % 10) + '0';
			ud /= 10;
		} while (ud);
		i--;	// 退格到数据位来

		// 倒序回来
		if (str_inv[0] == '-')
		{
			*str++ = '-';
			j = 1;
		}
		else
		{
			j = 0;
		}

		k = i-j;	// 多少位数
		k += 1;		// 循环读取次数
		for (; k; k--)
		{
			*str++ = str_inv[i--];
		}
		*str = '\0';	// 结束符
	}
}

//----格式打印---------------------------------------------------------
//功能: 按格式打印字符串，与C库函数类似，但功能弱化了许多。
//参数: const char *fmt, ...
//返回: 1
//-----------------------------------------------------------------------------
int printf_ng(const char *fmt, ...)
{
	const char *s;
	int d;
	u32 len, decbits = 0;
	char buf[16];
	char fillch = ' ';
	va_list argp;	// 参数链表
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
		// 填充字符
		if ((*fmt == ' ') || (*fmt == '0'))
		{
			fillch = *fmt;
			fmt++;
		}
		// 指定打印长度
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
		case 'd':	// 10进制数
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
		case 'x':	// 16进制数
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
			//添加其它参数
		default :
			putchar_ng(*fmt);
			break;
		}
		fmt++;
	}
	va_end(argp);

	return 1;	// 原本要返回
}

//----把字符串转换成长整型---------------------------------------------------------
//功能: 把字符串转换成长整型，未准备好ctype环境，不能使用atol、isdigit之类的函数
//参数:
//返回:
//-----------------------------------------------------------------------------
u32 __sh_atol(char *s)
{
    ufast_t offset;
    u32 result = 0;
    if((s[0] == '0') && ((s[1] == 'x') || (s[1] == 'X')))   //16进制
    {
        offset = 2;
        while(s[offset] == '0')
            offset++;
        while(s[offset] != '\0')
        {
            if((s[offset]>='0')&&(s[offset]<= '9'))         //0~9范围
                result = (result<<4) + s[offset] - 0x30;
            else if((s[offset]>='A')&&(s[offset]<= 'F'))    //A~F范围
                result = (result<<4) + s[offset] - 0x37;
            else if((s[offset]>='a')&&(s[offset]<= 'f'))    //a~f范围
                result = (result<<4) + s[offset] - 0x57;
            else                       //非法字符
                return result;
            offset++;
        }
    }else       //10进制
    {
        offset = 0;
        while(s[offset] == '0')
            offset++;
        while(s[offset] != '\0')
        {
            if((s[offset]>='0')&&(s[offset]<= '9'))         //0~9范围
                result = result*10 + s[offset] - 0x30;
            else                       //非法字符
                return result;
            offset++;
        }
    }
    return result;
}

