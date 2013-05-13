void term_scan(void);
ptu32_t module_init_char_term(ptu32_t para);
s32 djy_getchar(void);
s32 djy_scanf( const char *format, char *buf );

extern void itoa_ng(int d, char *buf, int scale);
extern u32 __sh_atol(char *s);
extern void putchar_ng(char c);
extern int printf_ng(const char *fmt, ...);
