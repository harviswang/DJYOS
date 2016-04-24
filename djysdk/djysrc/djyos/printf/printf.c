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
// ģ������:printf.c/p1020-core0
// ģ��汾: V1.0.0
// ������Ա: zhangqf_cyg
// ����ʱ��: 9:37:56 AM/Nov 6, 2013
// =============================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// =============================================================================
#include "stdarg.h"
#include "stdio.h"
#include "string.h"
#include "ctype.h"
#include "stdint.h"
#include "stddef.h"
#include "stdio.h"
#include "systime.h"        //todo : stdout��׼����ɾ����
#include "driver.h"     //todo : stdout��׼����ɾ����
#include "char_term.h"
#define cn_buf_length   0x100
//#define ADDCH(str, ch)    (*(str)++ = (ch))

#define ZEROPAD     1           /* pad with zero */
#define SIGN        2           /* unsigned/s32 */
#define PLUS        4           /* show plus */
#define SPACE       8           /* space if plus */
#define LEFT        16          /* left justified */
#define SMALL       32          /* Must be 32 == 0x20 */
#define SPECIAL     64          /* 0x */

//void (*djy_putc_direct)(char *c) = NULL;

//ctype�����һЩ����
#define is_digit(c)    ((c) >= '0' && (c) <= '9')
static s32 skip_atoi(const char **s)
{
    s32 i = 0;

    while (is_digit(**s))
        i = i * 10 + *((*s)++) - '0';

    return i;
}

//----��ʱ�洢����--------------------------------------------------------------
//����: ��ʱ�洢�������ַ���������ṩ�����������ô���洢����ʱ���������մ洢�ռ�
//����: str���洢�ռ䣬 ch,���洢�ַ��� position,���λ�ã�strputfunc���������
//����: ��һ�����õĴ洢�ռ�
//˵����
//-----------------------------------------------------------------------------
u32  ADDDCH_With_push(char *str,char ch, u32 position,\
        s32 (*strputfunc)(char *str))
{
    if(position >= (cn_buf_length-1))
    {
        if(strputfunc != NULL)
        {
            *(str + cn_buf_length -1) = '\0';
            strputfunc(str);
            position = position %(cn_buf_length - 1);
            *(str + position) = ch;
            position ++;
        }
        else
        {
            *(str + position) = ch;
            position ++;
        }
    }
    else
    {
        *(str + position) = ch;
        position ++;
    }
    return position;
}

u32 __div64_32(u64 *n, u32 base)
{
    u64 rem = *n;
    u64 b = base;
    u64 res, d = 1;
    u32  high = rem >> 32;

    /* Reduce the thing a bit first */
    res = 0;
    if (high >= base) {
        high /= base;
        res = (u64) high << 32;
        rem -= (u64) (high*base) << 32;
    }

    while ((u64 )b > 0 && b < rem) {
        b = b+b;
        d = d+d;
    }

    do {
        if (rem >= b) {
            rem -= b;
            res += d;
        }
        b >>= 1;
        d >>= 1;
    } while (d);

    *n = res;
    return rem;
}

/* Decimal conversion is by far the most typical, and is used
 * for /proc and /sys data. This directly impacts e.g. top performance
 * with many processes running. We optimize it for speed
 * using code from
 * http://www.cs.uiowa.edu/~jones/bcd/decimal.html
 * (with permission from the author, Douglas W. Jones). */

/* Formats correctly any integer in [0,99999].
 * Outputs from one to five digits depending on input.
 * On i386 gcc 4.1.2 -O2: ~250 bytes of code. */
static char *put_dec_trunc(char *buf, unsigned q)
{
    unsigned d3, d2, d1, d0;
    d1 = (q>>4) & 0xf;
    d2 = (q>>8) & 0xf;
    d3 = (q>>12);

    d0 = 6*(d3 + d2 + d1) + (q & 0xf);
    q = (d0 * 0xcd) >> 11;
    d0 = d0 - 10*q;
    *buf++ = d0 + '0'; /* least significant digit */
    d1 = q + 9*d3 + 5*d2 + d1;
    if (d1 != 0) {
        q = (d1 * 0xcd) >> 11;
        d1 = d1 - 10*q;
        *buf++ = d1 + '0'; /* next digit */

        d2 = q + 2*d2;
        if ((d2 != 0) || (d3 != 0)) {
            q = (d2 * 0xd) >> 7;
            d2 = d2 - 10*q;
            *buf++ = d2 + '0'; /* next digit */

            d3 = q + 4*d3;
            if (d3 != 0) {
                q = (d3 * 0xcd) >> 11;
                d3 = d3 - 10*q;
                *buf++ = d3 + '0';  /* next digit */
                if (q != 0)
                    *buf++ = q + '0'; /* most sign. digit */
            }
        }
    }
    return buf;
}
/* Same with if's removed. Always emits five digits */
static char *put_dec_full(char *buf, unsigned q)
{
    /* BTW, if q is in [0,9999], 8-bit ints will be enough, */
    /* but anyway, gcc produces better code with full-sized ints */
    unsigned d3, d2, d1, d0;
    d1 = (q>>4) & 0xf;
    d2 = (q>>8) & 0xf;
    d3 = (q>>12);

    /*
     * Possible ways to approx. divide by 10
     * gcc -O2 replaces multiply with shifts and adds
     * (x * 0xcd) >> 11: 11001101 - shorter code than * 0x67 (on i386)
     * (x * 0x67) >> 10:  1100111
     * (x * 0x34) >> 9:    110100 - same
     * (x * 0x1a) >> 8:     11010 - same
     * (x * 0x0d) >> 7:      1101 - same, shortest code (on i386)
     */

    d0 = 6*(d3 + d2 + d1) + (q & 0xf);
    q = (d0 * 0xcd) >> 11;
    d0 = d0 - 10*q;
    *buf++ = d0 + '0';
    d1 = q + 9*d3 + 5*d2 + d1;
        q = (d1 * 0xcd) >> 11;
        d1 = d1 - 10*q;
        *buf++ = d1 + '0';

        d2 = q + 2*d2;
            q = (d2 * 0xd) >> 7;
            d2 = d2 - 10*q;
            *buf++ = d2 + '0';

            d3 = q + 4*d3;
                q = (d3 * 0xcd) >> 11; /* - shorter code */
                /* q = (d3 * 0x67) >> 10; - would also work */
                d3 = d3 - 10*q;
                *buf++ = d3 + '0';
                    *buf++ = q + '0';
    return buf;
}
/* No inlining helps gcc to use registers better */
static  char *put_dec(char *buf, u64  num)
{
    while (1) {
        unsigned rem;
        if (num < 100000)
            return put_dec_trunc(buf, num);
//        rem = do_div(num, 100000);
        rem = num % 100000;
        num = num / 100000;
        buf = put_dec_full(buf, rem);
    }
}

static u32 number(char *buf, char *end, u64  num,\
        s32 base, s32 size, s32 precision, s32 type, u32 position,\
        s32 (*strputfunc)(char *str))
{
    /* we are called with base 8, 10 or 16, only, thus don't need "G..."  */
    static const char digits[ ] = "0123456789ABCDEF";

    char tmp[66];
    char sign;
    char locase;
    s32 need_pfx = ((type & SPECIAL) && base != 10);
    s32 i;

    /* locase = 0 or 0x20. ORing digits or letters with 'locase'
     * produces same digits or (maybe lowercased) letters */
    locase = (type & SMALL);
    if (type & LEFT)
        type &= ~ZEROPAD;
    sign = 0;
    if (type & SIGN) {
        if ((s64) num < 0) {
            sign = '-';
            num = -(s64) num;
            size--;
        } else if (type & PLUS) {
            sign = '+';
            size--;
        } else if (type & SPACE) {
            sign = ' ';
            size--;
        }
    }
    if (need_pfx) {
        size--;
        if (base == 16)
            size--;
    }

    /* generate full string in tmp[], in reverse order */
    i = 0;
    if (num == 0)
        tmp[i++] = '0';
    /* Generic code, for any base:
    else do {
        tmp[i++] = (digits[do_div(num,base)] | locase);
    } while (num != 0);
    */
    else if (base != 10) { /* 8 or 16 */
        s32 mask = base - 1;
        s32 shift = 3;

        if (base == 16)
            shift = 4;

        do {
            tmp[i++] = (digits[((unsigned char)num) & mask]
                    | locase);
            num >>= shift;
        } while (num);
    } else { /* base 10 */
        i = put_dec(tmp, num) - (char *)tmp;
    }

    /* printing 100 using %2d gives "100", not "00" */
    if (i > precision)
        precision = i;
    /* leading space padding */
    size -= precision;
    if (!(type & (ZEROPAD + LEFT))) {
        while (--size >= 0)
            position = ADDDCH_With_push(buf, ' ', position, strputfunc);
    }
    /* sign */
    if (sign)
        position = ADDDCH_With_push(buf, sign, position, strputfunc);
    /* "0x" / "0" prefix */
    if (need_pfx) {
        position = ADDDCH_With_push(buf, '0', position, strputfunc);
        if (base == 16)
            position = ADDDCH_With_push(buf, 'X' | locase, position, strputfunc);
    }
    /* zero or space padding */
    if (!(type & LEFT)) {
        char c = (type & ZEROPAD) ? '0' : ' ';

        while (--size >= 0)
            position = ADDDCH_With_push(buf, c, position, strputfunc);
    }
    /* hmm even more zero padding? */
    while (i <= --precision)
        position = ADDDCH_With_push(buf, '0', position, strputfunc);
    /* actual digits of result */
    while (--i >= 0)
        position = ADDDCH_With_push(buf, tmp[i], position, strputfunc);
    /* trailing space padding */
    while (--size >= 0)
        position = ADDDCH_With_push(buf, ' ', position, strputfunc);
    return position;
}

static u32 string(char *buf, char *end, char *s, s32 field_width,
        s32 precision, s32 flags, u32 position, s32 (*strputfunc)(char *str))
{
    s32 len, i;

    if (s == NULL)
        s = "<NULL>";

    len = strnlen(s, precision);

    if (!(flags & LEFT))
        while (len < field_width--)
            position = ADDDCH_With_push(buf, ' ', position, strputfunc);
    for (i = 0; i < len; ++i)
        position = ADDDCH_With_push(buf, *s++, position, strputfunc);
    while (len < field_width--)
        position = ADDDCH_With_push(buf, ' ', position, strputfunc);
    return position;
}
static u32 pointer(const char *fmt, char *buf, char *end, void *ptr,
        s32 field_width, s32 precision, s32 flags, u32 position,
        s32 (*strputfunc)(char *str))
{
    /*
     * Being a boot loader, we explicitly allow pointers to
     * (physical) address null.
     */
#if 0
    if (!ptr)
        return string(buf, end, "(null)", field_width, precision,
                  flags);
#endif

#ifdef CONFIG_CMD_NET
    switch (*fmt) {
    case 'm':
        flags |= SPECIAL;
        /* Fallthrough */
    case 'M':
        return mac_address_string(buf, end, ptr, field_width,
                      precision, flags);
    case 'i':
        flags |= SPECIAL;
        /* Fallthrough */
    case 'I':
        if (fmt[1] == '6')
            return ip6_addr_string(buf, end, ptr, field_width,
                           precision, flags);
        if (fmt[1] == '4')
            return ip4_addr_string(buf, end, ptr, field_width,
                           precision, flags);
        flags &= ~SPECIAL;
        break;
    }
#endif
    flags |= SMALL;
    if (field_width == -1) {
        field_width = 2*sizeof(void *);
        flags |= ZEROPAD;
    }
    return number(buf, end, (u32)ptr, 16, field_width,
              precision, flags,position, strputfunc);
}
//---------------------------------------------------------------------------
//����: ���ո�ʽ�����в����Ľ�����
//����:
//����: ת�����ַ�������
//˵���������û�ָ�����ַ�������ָ���ĸ�ʽ����ת��������Ӧ�Ĳ���ת���ɿ��ӵ��ַ���
//      Ŀǰû�м��븡���ӡ--TODO
//-----------------------------------------------------------------------------

static s32 djy_vsnprintf_internal(char *buf, s32 size, const char *fmt,
                  va_list args, s32 (*strputfunc)(char *str))
{
    u64  num;
    u32 position;
    s32 base;
    char *str;

    s32 flags;        /* flags to number() */

    s32 field_width;    /* width of output field */
    s32 precision;        /* min. # of digits for integers; max
                   number of chars for from string */
    s32 qualifier;        /* 'h', 'l', or 'L' for integer fields */
                /* 'z' support added 23/7/1999 S.H.    */
                /* 'z' changed to 'Z' --davidm 1/25/99 */
                /* 't' added for ptrdiff_t */
    char *end = buf + size;

    str = buf;
    position = 0;

    for (; *fmt ; ++fmt) {
        if (*fmt != '%') {
            position = ADDDCH_With_push(str, *fmt, position, strputfunc);
            continue;
        }

        /* process flags */
        flags = 0;
repeat:
            ++fmt;        /* this also skips first '%' */
            switch (*fmt) {
            case '-':
                flags |= LEFT;
                goto repeat;
            case '+':
                flags |= PLUS;
                goto repeat;
            case ' ':
                flags |= SPACE;
                goto repeat;
            case '#':
                flags |= SPECIAL;
                goto repeat;
            case '0':
                flags |= ZEROPAD;
                goto repeat;
            }

        /* get field width */
        field_width = -1;
        if (is_digit(*fmt))
            field_width = skip_atoi(&fmt);
        else if (*fmt == '*') {
            ++fmt;
            /* it's the next argument */
            field_width = va_arg(args, s32);
            if (field_width < 0) {
                field_width = -field_width;
                flags |= LEFT;
            }
        }

        /* get the precision */
        precision = -1;
        if (*fmt == '.') {
            ++fmt;
            if (is_digit(*fmt))
                precision = skip_atoi(&fmt);
            else if (*fmt == '*') {
                ++fmt;
                /* it's the next argument */
                precision = va_arg(args, s32);
            }
            if (precision < 0)
                precision = 0;
        }

        /* get the conversion qualifier */
        qualifier = -1;
        if (*fmt == 'h' || *fmt == 'l' || *fmt == 'L' ||
            *fmt == 'Z' || *fmt == 'z' || *fmt == 't') {
            qualifier = *fmt;
            ++fmt;
            if (qualifier == 'l' && *fmt == 'l') {
                qualifier = 'L';
                ++fmt;
            }
        }

        /* default base */
        base = 10;

        switch (*fmt) {
        case 'c':
            if (!(flags & LEFT)) {
                while (--field_width > 0)
                    position = ADDDCH_With_push(str, ' ', position, strputfunc);
            }
            position = ADDDCH_With_push(str, (unsigned char) va_arg(args, s32),\
                                        position, strputfunc);
            while (--field_width > 0)
                position = ADDDCH_With_push(str, ' ', position, strputfunc);
            continue;

        case 's':
            position = string(str, end, va_arg(args, char *),
                     field_width, precision, flags, position, strputfunc);
            continue;

        case 'p':
            position = pointer(fmt + 1, str, end,
                    va_arg(args, void *),
                    field_width, precision, flags, position, strputfunc);
            /* Skip all alphanumeric pointer suffixes */
            while (isalnum((s32)fmt[1]))
                fmt++;
            continue;

        case 'n':
            if (qualifier == 'l') {
                s32 *ip = va_arg(args, s32 *);
                *ip = (str - buf);
            } else {
                s32 *ip = va_arg(args, s32 *);
                *ip = (str - buf);
            }
            continue;

        case '%':
            position = ADDDCH_With_push(str, '%', position, strputfunc);
            continue;

        /* integer number formats - set up the flags and "break" */
        case 'o':
            base = 8;
            break;

        case 'x':
            flags |= SMALL;
        case 'X':
            base = 16;
            break;

        case 'd':
        case 'i':
            flags |= SIGN;
        case 'u':
            break;

        default:
            position = ADDDCH_With_push(str, '%', position, strputfunc);
            if (*fmt)
                position = ADDDCH_With_push(str, *fmt, position, strputfunc);
            else
                --fmt;
            continue;
        }
        if (qualifier == 'L')  /* "quad" for 64 bit variables */
            num = va_arg(args, u64);
        else if (qualifier == 'l') {
            num = va_arg(args, u32);
            if (flags & SIGN)
                num = (s32) num;
        } else if (qualifier == 'Z' || qualifier == 'z') {
            num = va_arg(args, s32);
        } else if (qualifier == 't') {
            num = va_arg(args, s32);
        } else if (qualifier == 'h') {
            num = (u16) va_arg(args, s32);
            if (flags & SIGN)
                num = (s16) num;
        } else {
            num = va_arg(args, u32);
            if (flags & SIGN)
                num = (s32) num;
        }
        position = number(str, end, num, base, field_width, precision,
                 flags, position, strputfunc);
    }


    *(str + position) = '\0';
    position ++;
    /* the trailing null byte doesn't count towards the total */
    return  position;
}
s32 djy_vsprintf(char *buf, const char *fmt, va_list args,\
        s32 (*strputfunc)(char *str))
{
    s32 i;
    s32 buf_len=0;
    for(i=0;i<cn_buf_length;i++)
    {
        if(*(fmt+i)!='\0')
        {
            buf_len++;
        }
        else
        {
            break;
        }
    }


    return djy_vsnprintf_internal(buf, buf_len, fmt, args, strputfunc);
}
//----��ʽ��ӡ-----------------------------------------------------------------
//����: ����ʽ��ӡ�ַ�������C�⺯�����ƣ���ʹ�ö�̬���䣬һ�δ�ӡ�ĳ�������Ϊ255
//      �����ֽ��ַ������Ч���ֽ��ַ���
//����: const char *fmt, ...
//����: ת�����ַ�������
//˵�����ô�ӡ���������ն��豸��÷��񣬽�Ϊ���ӣ���ӡ�й������롣��˴������
//      �׶���ʹ�øô�ӡ������лл����������������£�ϣ��ֱ���������ʹ��
//      djy_printk������
//-----------------------------------------------------------------------------
s32 printf (const char *fmt, ...)
{
    va_list args;
    s32  i;
    char printbuffer[cn_buf_length];

    va_start (args, fmt);

    /* For this to work, printbuffer must be larger than
     * anything we ever want to print.
     */
    i = djy_vsprintf (printbuffer, fmt, args, (s32 (*)(char *str))Djy_Puts);
    va_end (args);

    /* Print the string */
    Djy_Puts (printbuffer);
    return i;
}

//----��ʽ���ַ���--------------------------------------------------------------
//����: ��ʽ���ַ�������C��һ��������һ���ĸ�ʽ����ʽ���ַ���
//����: const char *fmt, ...
//����: ת�����ַ�������
//˵����
//-----------------------------------------------------------------------------
s32 sprintf(char *buf, const char *fmt, ...)
{
    va_list args;
    u32  i;

    va_start (args, fmt);

    /* For this to work, printbuffer must be larger than
     * anything we ever want to print.
     */
    i = djy_vsprintf (buf, fmt, args,false);
    va_end (args);
    return i;
}

//----��ʽ���ַ���--------------------------------------------------------------
//����: ��ʽ���ַ�������C��һ��������һ���ĸ�ʽ����ʽ���ַ���
//����: const char *fmt, ...
//����: ת�����ַ�������
//˵����
//-----------------------------------------------------------------------------
s32 fprintf(FILE *fp, const char *fmt, ...)
{
    va_list args;
    u32  i;
    char printbuffer[cn_buf_length];
    va_start (args, fmt);

    /* For this to work, printbuffer must be larger than
     * anything we ever want to print.
     */
    i = djy_vsprintf (printbuffer, fmt, args,NULL);
    va_end (args);

    /* Print the string */
        Djy_Puts (printbuffer);
        return i;
}
//----��ʽ��ӡ-----------------------------------------------------------------
//����: ����ʽ��ӡ�ַ�������C�⺯�����ƣ���ʹ�ö�̬���䣬һ�δ�ӡ�ĳ�������Ϊ255
//      �����ֽ��ַ������Ч���ֽ��ַ���
//����: const char *fmt, ...
//����: ת�����ַ�������
//˵������printf��һ������ֱ��������ն��豸�ϡ�
//-----------------------------------------------------------------------------
extern s32 Uart_SendServiceDirectly(char *str);
s32 printk (const char *fmt, ...)
{
    va_list args;
    u32  i;
    char printbuffer[cn_buf_length];

    va_start (args, fmt);

    /* For this to work, printbuffer must be larger than
    anything we ever want to print.
    */
    i = djy_vsprintf (printbuffer, fmt, args,(s32 (*)(char *str))Uart_SendServiceDirectly);
    va_end (args);
    Uart_SendServiceDirectly(printbuffer);

    return i;
}

