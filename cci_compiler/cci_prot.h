
/* cci_tkn.c */
void initChTyp();
void fileOpen();
// Token nextTkn();

/* cci_misc.c */
int is_kanji(int ch);
int get_kanji_mode();
char *s_malloc(char *s);
void incVar(int *var, int size, char *errmsg);
void err_s(char *s);
void err_fi(char *fmt, int idt);
void err_ss(char *s1, char *s2);