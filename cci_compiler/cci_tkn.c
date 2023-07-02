#include "cci.h"
#include "cci_prot.h"

// 字句と種別の対応を管理
typedef struct {
  char *keyName; // voidやforなど
  TknKind keyKind; // 対応する値。TknKindはcci.hで定義
} KeyWord;

// 予約語や記号と種別の対応表
KeyWord KeyWdTbl[] = {
  {"void"   , Void  }, {"int"     , Int    },
  {"if"     , If    }, {"else"    , Else   },
  {"for"    , For   }, {"while"   , While  },
  {"do"     , Do    }, {"switch"  , Switch },
  {"case"   , Case  }, {"default" , Default},
  {"break"  , Break }, {"continue", Continue },
  {"return" , Return}, {"printf"  , Printf },
  {"input"   , Input}, {"exit"    , Exit   },

  {"("  , Lparen    }, {")"  , Rparen    },
  {"{"  , Lbrace    }, {"}"  , Rbrace    },
  {"["  , Lbracket  }, {"]"  , Rbracket  },
  {"+"  , Plus      }, {"-"  , Minus     },
  {"*"  , Multi     }, {"/"  , Divi      },
  {"++" , Incre     }, {"--" , Decre     },
  {"==" , Equal     }, {"!=" , NotEq     },
  {"<"  , Less      }, {"<=" , LessEq    },
  {">"  , Great     }, {">=" , GreatEq   },
  {"&&" , And       }, {"||" , Or        },
  {"!"  , Not       }, {"%"  , Mod       },
  {":"  , Colon     }, {";"  , Semicolon },
  {"="  , Assign    }, {"#"  , Sharp     },
  {"\\" , Yen       }, {","  , Comma     },
  {"'"  , SngQ      }, {"\"" , DblQ      },
  {"$dummy", END_KeyList},
};

// ソースの一行の最大長
#define LIN_SIZ 250
// ファイルの処理
FILE *fin;
// ソースの行番号
int srcLineco = 0;
// 文字種表の配列
TknKind ctyp[256];

// 文字種表の設定
void initChTyp() {
  int i;
  for (i=0; i<256; i++) {
    ctyp[i] = Others;
  }
  for (i='0'; i<='9'; i++) {
    ctyp[i] = Digit;
  }
  for (i='A'; i<='Z'; i++) {
    ctyp[i] = Letter;
  }
  for (i='a'; i<='z'; i++) {
    ctyp[i] = Letter;
  }
  ctyp['_'] = Letter;
  ctyp['('] = Lparen;    ctyp[')'] = Rparen;
  ctyp['{'] = Lbrace;    ctyp['}'] = Rbrace;
  ctyp['['] = Lbracket;  ctyp[']'] = Rbracket;
  ctyp['<'] = Less;      ctyp['>'] = Great;
  ctyp['+'] = Plus;      ctyp['-'] = Minus;
  ctyp['*'] = Multi;     ctyp['/'] = Divi;
  ctyp['!'] = Not;       ctyp['%'] = Mod;
  ctyp[':'] = Colon;     ctyp[';'] = Semicolon;
  ctyp['='] = Assign;
  ctyp['\\'] = Yen;      ctyp[','] = Comma;
  ctyp['\''] = SngQ;     ctyp['\"'] = DblQ;
}

// ファイルを開く
void fileOpen(char *fname) {
  if ((fin = fopen(fname, "r")) == NULL) {
    fprintf(stderr, "%sをオープンできません\n", fname);
    exit(1);
  }
  srcLineco = 1;
}

#define P_SET() *p++=ch, ch=nextCh()
// 次のトークン
Token nextTkn() {
  Token tkn = {NulKind, "", 0};
  int errF = FALSE, num, ct, n;
  char *p = tkn.text, *p_end31 = p + ID_SIZ;
  char lite[100 + 1], *lite_end = lite + 100;
  // 前回文字を保持するためstatic
  static int ch = ' ';

  // 空白読み捨て
  while (isspace(ch)) {
    ch = nextCh();
  }
  return tkn;
}

// 次の一文字
int nextCh() {
  static int c = 1, quot_ch = 0;
  int c2;

  if (c == '\n')
    ++srcLineco;
  // 前回読み込み文字で行カウント更新
  if (c == '\n')
    ++srcLineco;

  c = fgetc(fin);
  // 終了
  if (c == EOF) {
    fclose(fin);
    return c;
  }
  // 文字リテラル内部
  if (quot_ch) {
    // リテラル終了または閉じていない
    if (c == quot_ch || c == '\n')
      quot_ch = 0;
    return c;
  }

  if (c == '\'' || c == '"') {
    quot_ch = c;
  } else if (c == '/') {
    switch (c2 = fgetc(fin)) {
      case '/':
        while ((c = fgetc(fin)) != EOF && c != '\n');
        return c;
      case '*':
        for (c = 0; (c2 = fgetc(fin)) != EOF; c = c2) {
          if (c2 == '\n')
            ++srcLineco;
          if (c == '*' && c2 == '/') {
            c = ' ';
            return c;
          }
          err_s("/* に対応する */ がない");
          exit(1);
        }
    }
    ungetc(c2, fin);
  } else if (c == '*') {
    if ((c2 = fgetc(fin)) == '/') {
      err_s("/* がないのに */を検出した");
      exit(1);
    }
    ungetc(c2, fin);
  }
  return c;
}

// 2文字演算子なら真
int is_ope2(int c1, int c2) {
  char s[] = "    ";
  s[1] = c1;
  s[2] = c2;
  return strstr(" ++ -- <= >= == != && || ", s) != NULL;
}

// トークン種別設定
Token set_kind(Token tk) {
  int i, ch = tk.text[0];

  tk.kind = Others;
  for (i = 0; KeyWdTbl[i].keyKind != END_KeyList; i++) {
    if (strcmp(tk.text, KeyWdTbl[i].keyName) == 0) {
      tk.kind = KeyWdTbl[i].keyKind;
      return tk;
    }
  }
  
  if (ctyp[ch] == Digit)
    tk.kind = Ident;
  else if (ctyp[ch] == Digit)
    tk.kind = IntNum;
  
  return tk;
}

// tk.kind==kdなら次のトークンを返す。異なるときはそのままtkを返す
Token chk_nextTkn(Token tk, TknKind kd) {
  char ss[100];
  if (tk.kind == kd) {
    return nextTkn();
  } else {
    sprintf(ss, "%s の前に %c がありません。", tk.text, kd);
    err_s(ss);
    return tk;
  }
}

// 読み込み中の行番号
int get_lineNo() {
  return srcLineco;
}