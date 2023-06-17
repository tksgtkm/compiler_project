#include <stdio.h>
#include <string.h>
#include "get_source.h"

// 1桁の最大文字数
#define MAXLINE 120
// これ以上のエラーがあったら終わり
#define MAXERROR 30
// 定数の最大桁数
#define MAXNUM 14
// タブのスペース
#define TAB 5
// 挿入文字の色
#define INSERT_C "#0000FF"
// 削除文字の色
#define DELETE_C "#FF0000"
// タイプエラー文字の色
#define TYPE_C "#00FF00"

// ソースファイル
static FILE *fpi;
// LaTeX出力ファイル
static FILE *fptex;
// 1行分の入力バッファー
static char line[MAXLINE];
// 次に読む文字の位置
static int lineIndex;
// 最後に読んだ文字
static char ch;

// 最後に読んだトークン
static Token cToken;
// 現トークン(ID)の種類
static KindT idKind;
// そのトークンの前のスペースの個数
static int spaces;
// その前のCRの個数
static int CR;
// トークンは印字済みか
static int printed;

// 出力されたエラーの数
static int errorNo = 0;
// 次の文字を読む関数
static char nextChar();
// tは記号か？
static int isKeyNum(KeyId k);
// トークン前のスペースの印字
static void printSpaces();
// トークンの印字
static void printcToken();

// 予約語や記号と名前(KeyId)
struct keyWd {
  char *word;
  KeyId keyId;
};

// 予約語や記号と名前(KeyId)の表
static struct keyWd KeyWdT[] = {
  {"begin", Begin},
	{"end", End},
	{"if", If},
	{"then", Then},
	{"while", While},
	{"do", Do},
	{"return", Ret},
	{"function", Func},
	{"var", Var},
	{"const", Const},
	{"odd", Odd},
	{"write", Write},
	{"writeln",WriteLn},
	{"$dummy1",end_of_KeyWd},
	// 記号と名前(KeyId)の表
	{"+", Plus},
	{"-", Minus},
	{"*", Mult},
	{"/", Div},
	{"(", Lparen},
	{")", Rparen},
	{"=", Equal},
	{"<", Lss},
	{">", Gtr},
	{"<>", NotEq},
	{"<=", LssEq},
	{">=", GtrEq},
	{",", Comma},
	{".", Period},
	{";", Semicolon},
	{":=", Assign},
	{"$dummy2",end_of_KeySym}
};

// キーkは予約語か？
int isKeyWd(KeyId k) {
  return k < end_of_KeyWd;
}

// キーkは記号か？
int isKeySym(KeyId k) {
  if (k < end_of_KeyWd)
    return 0;
  return k < end_of_KeySym;
}

// 文字の種類を示す表にする
static KeyId charClassT[256];

// 文字の種類を示す表を作る関数
static void initCharClassT() {
  int i;
  for (i = 0; i < 256; i++)
    charClassT[i] = others;
  for (i = '0'; i <= '9'; i++)
    charClassT[i] = digit;
  for (i = 'A'; i <= 'Z'; i++)
    charClassT[i] = letter;
  for (i = 'a'; i <= 'z'; i++)
    charClassT[i] = letter;
  charClassT['+'] = Plus; charClassT['-'] = Minus;
	charClassT['*'] = Mult; charClassT['/'] = Div;
	charClassT['('] = Lparen; charClassT[')'] = Rparen;
	charClassT['='] = Equal; charClassT['<'] = Lss;
	charClassT['>'] = Gtr; charClassT[','] = Comma;
	charClassT['.'] = Period; charClassT[';'] = Semicolon;
	charClassT[':'] = colon;
}

// ソースファイルのopen
int openSource(char fileName[]) {
  char fileName0[30];
  if ((fpi = fopen(fileName, "r")) == NULL) {
    printf("can't open %s\n", fileName);
    return 0;
  }
  strcpy(fileName0, fileName);
  strcat(fileName0, ".html");
  // .html(または.tex)ファイルを作る
  if ((fptex = fopen(fileName0, "w")) == NULL) {
    printf("can't open %s\n", fileName0);
    return 0;
  }
  return 1;
}

// ソースファイルと.html(.tex)ファイルをclose
void closeSource() {
  fclose(fpi);
  fclose(fptex);
}

void initSource() {
  // 初期設定
  lineIndex = -1;
  ch = '\n';
  printed = 1;
  initCharClassT();
  fprintf(fptex, "<HTML>\n");
  fprintf(fptex, "<HEAD>\n<TITLE>compiled source program></TITLE>\n</HEAD>\n");
  fprintf(fptex, "<BODY>\n<PRE>\n");
}

void finalSource() {
  if (cToken.kind == Period)
    printcToken();
  else
    errorInsert(Period);
  fprintf(fptex, "\n</PRE>\n</BODY>\n</HTML>\n");
}

// エラーの個数のカウント、多すぎると終わり
void errorNoCheck() {
  if (errorNo++ > MAXERROR) {
    fprintf(fptex, "too many errors\n</PRE>\n</BODY>\n</HTML>\n");
    printf("abort compilation\n");
    exit(1);
  }
}

// 型エラーを.html(.tex)ファイルに出力
void errorType(char *m) {
  printSpaces();
  fprintf(fptex, "<FONT COLOR=%s>%s</FONT>", TYPE_C, m);
  printcToken();
  errorNoCheck();
}

// keyString(k)を.html(.tex)ファイルに挿入
void errorInsert(KeyId k) {
  fprintf(fptex, "<FONT COLOR=%s><b>%s</b></FONT>", INSERT_C, KeyWdT[k].word);
  errorNoCheck();
}

// 名前がないとのメッセージを.html(.tex)ファイルに挿入
void errorMissingId() {
  fprintf(fptex, "<FONT COLOR=%s>Id</FONT>", INSERT_C);
  errorNoCheck();
}

// 演算子がないとのメッセージを.  html(.tex)ファイルに挿入
void errorMissingOp() {
  fprintf(fptex, "<FONT COLOR=%s></FONT>", INSERT_C);
  errorNoCheck();
}

// 今読んだトークンを読み捨てる
void errorDelete() {
  int i = (int)cToken.kind;
  printSpaces();
  printed = 1;
  if (i < end_of_KeyWd)
    fprintf(fptex, "<FONT COLOR=%s><b>%s</b></FONT>", DELETE_C, KeyWdT[i].word);
  else if (i < end_of_KeySym)
    fprintf(fptex, "<FONT COLOR=%s>%s</FONT>", DELETE_C, KeyWdT[i].word);
  else if (i == (int)Id)
    fprintf(fptex, "<FONT COLOR=%s>%s</FONT>", DELETE_C, cToken.u.id);
  else if (i == (int)Num)
    fprintf(fptex, "<FONT COLOR=%s>%s</FONT>", DELETE_C, cToken.u.value);
}

// エラーメッセージを.html(.tex)ファイルに出力
void errorMessage(char *m) {
  fprintf(fptex, "<FONT COLOR=%s>%s</FONT>", TYPE_C, m);
  errorNoCheck();
}

void errorF(char *m) {
  errorMessage(m);
  fprintf(fptex, "fatal errors\n</PRE>\n</BODY>\n</HTML>\n");
  if (errorNo)
    printf("total %d errors\n", errorNo);
  printf("abort compilation\n");
  exit(1);
}

int errorN() {
  return errorNo;
}

// 次の一文字を返す関数
char nextChar() {
  char ch;
  if (lineIndex == -1) {
    if (fgets(line, MAXLINE, fpi) != NULL) {
      lineIndex = 0;
    } else {
      // コンパイル終了
      errorF("end of file\n");
    }
  }
  // chに次の一文字
  if ((ch = line[lineIndex++]) == '\n') {
    // それが改行文字なら次の行の入力準備
    lineIndex = -1;
    // 文字としては改行文字を返す
    return '\n';
  }
  return ch;
}

// 次のトークンを読んで返す関数
Token nextToken() {
  int i = 0;
  int num;
  KeyId cc;
  Token temp;
  char ident[MAXNAME];
  // 前のトークンを印字
  printcToken();
  spaces = 0;
  CR = 0;
  while (1) {
    if (ch == ' ') {
      spaces++;
    } else if (ch == '\t') {
      spaces+=TAB;
    } else if (ch == '\n') {
      spaces = 0;
      CR++;
    } else {
      break;
    }
    ch = nextChar();
  }
  switch(cc = charClassT[ch]) {
    case letter:
      do {
        if (i < MAXNAME)
          ident[i] = ch;
        i++;
        ch = nextChar();
      } while (charClassT[ch] == letter || charClassT[ch] == digit);
      if (i >= MAXNAME) {
        errorMessage("too long");
        i = MAXNAME - 1;
      }
      ident[i] = '\0';
      for (i = 0; i < end_of_KeyWd; i++) {
        if (strcmp(ident, KeyWdT[i].word) == 0) {
          temp.kind = KeyWdT[i].keyId;
          cToken = temp;
          printed = 0;
          return temp;
        }
      }
      // ユーザーの宣言した名前の場合
      temp.kind = Id;
      strcpy(temp.u.id, ident);
      break;
    case digit:
      num = 0;
      do {
        num = 10*num + (ch - '0');
        i++;
        ch = nextChar();
      } while (charClassT[ch] == digit);
      if (i > MAXNAME)
        errorMessage("too large");
      temp.kind = Num;
      temp.u.value = num;
      break;
    case colon:
      if ((ch = nextChar()) == '=') {
        ch = nextChar();
        temp.kind = Assign;
        break;
      } else {
        temp.kind = nul;
        break;
      }
    case Lss:
      if ((ch = nextChar()) == '=') {
        ch = nextChar();
        temp.kind = LssEq;
        break;
      } else if (ch == '>') {
        ch = nextChar();
        temp.kind = NotEq;
        break;
      } else {
        temp.kind = Lss;
        break;
      }
    case Gtr:
      if ((ch = nextChar()) == '=') {
        ch = nextChar();
        temp.kind = GtrEq;
        break;
      } else {
        temp.kind = Gtr;
        break;
      }
    default:
      temp.kind = cc;
      ch = nextChar();
      break;
  }
  cToken = temp;
  printed = 0;
  return temp;
}

// t.kind == kのチェック
Token checkGet(Token t, KeyId k) {
  /*
  t.kind == k なら次のトークンを読んで返す
  t.kind != k ならエラーメッセージを出し、tとkがともに記号または予約語なら
  tを捨て、次のトークンを読んで返す。(tとkで置き換える)
  それ以外の場合、kを挿入したことにしてtを返す
  */
  if (t.kind == k)
    return nextToken();
  if ((isKeyWd(k) && isKeyWd(t.kind)) || (isKeySym(k) && isKeySym(t.kind))) {
    errorDelete();
    errorInsert(k);
    return nextToken();
  }
  errorInsert(k);
  return t;
}

// 空白や改行の印字
static void printSpaces() {
  while (CR-- > 0)
    fprintf(fptex, "\n");
  while (spaces-- > 0)
    fprintf(fptex, " ");
  CR = 0;
  spaces = 0;
}

// 次のトークンの印字
void printcToken() {
  int i = (int)cToken.kind;
  if (printed) {
    printed = 0;
    return;
  }
  printed = 1;
  // トークンの前の空白や改行印字
  printSpaces();
  // 予約語
  if (i < end_of_KeyWd) {
    fprintf(fptex, "<b>%s</b>", KeyWdT[i].word);
  } else if (i < end_of_KeySym) {
    fprintf(fptex, "%s", KeyWdT[i].word);
  } else if (i == (int)Id) {
    switch(idKind) {
      case varId:
        fprintf(fptex, "%s", cToken.u.id);
        return;
      case parId:
        fprintf(fptex, "<i>%s</i>", cToken.u.id);
        return;
      case funcId:
        fprintf(fptex, "<i>%s</i>", cToken.u.id);
        return;
      case constId:
        fprintf(fptex, "<tt>%s</tt>", cToken.u.id);
        return;
    }
  } else if (i == (int)Num) {
    fprintf(fptex, "%d", cToken.u.value);
  }
}

// 現トークン(Id)の種類をセット
void setIdKind(KindT k) {
  idKind = k;
}