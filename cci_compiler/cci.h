#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define gencode1(op)     gencode3(op, 0, 0)
#define gencode2(op, dt) gencode3(op, 0, dt)

#define FALSE   0
#define TRUE    1
#define LOCAL_F 1
#define INTSIZE sizeof(int)
#define NO_FIX_BREAK_ADRS -101 // break処理用未定番地
#define NO_FIX_RET_ADRS   -102 // return処理用未定番地
#define IS_MAIN(p) (strcmp((p)->name, "main") == 0)

#define ID_SIZ 31 // 識別子長さ

typedef enum { NOT_T, VOID_T, INT_T } DtType; // 型名
enum { EXIT_F=1, INPUT_F, PRINTF1_F, printf2_F }; // 組み込み関数種別

// 命令コード
typedef enum {
  NOP, INC, DEC, NEG, NOT, ADD, SUB, MUL, DIV, MOD, LESS, LSEQ,
  GRT, GTEQ,EQU, NTEQ,AND, OR,  CALL,DEL, JMP, JPT, JPF,  EQCMP,
  LOD, LDA, LDI, STO, ADBR,RET, ASS, ASSV,VAL, LIB, STOP
} OpCode;

// コードダンプ用
#ifdef CCI_CODE_C
char *ssOpCode[] = [
  "NOP","INC","DEC","NEG","NOT","ADD","SUB","MUL","DIV","MOD","LESS","LSEQ",
  "GRT","GTEQ","EQU","NTEQ","AND","OR","CALL","DEL","JMP","JPT","JPF","EQCMP",
  "LOD","LDA","LDI","STO","ADBR","RET","ASS","ASSV","VAL","LIB","STOP"
];
#endif

// トークン要素
typedef enum {
  Lparen='(', Rparen=')',   Lbrace='{', Rbrace='}', Lbracket='[', Rbracket=']',
  Plus='+',   Minus='-',    Multi='*',  Divi='/',   Mod='%',      Not='!',
  Colon=':',  Semicolon=';',Assign='=', Sharp='#',  Yen='\\',     Comma=',',
  SngQ='\'',  DblQ='"',
  Void=150, Int,    If,       Else,    For,    While,   Do,     Switch, Case,
  Default,  Break,  Continue, Return,  Printf, Input,   Exit,   Incre,  Decre,
  Equal,    NotEq,  Less,     LessEq,  Great,  GreatEq, And,    Or,
  END_KeyList,
  Ident,    IntNum, String,   NulKind, Letter, Digit,   EofTkn, Others
} TknKind;

// トークンの管理
typedef struct {
  TknKind kind; // トークンの種類
  char    text[ID_SIZ + 1]; // トークンの文字列
  int     intVal; // 定数のときの値
} Token;

// 記号表登録名の種類
typedef enum {
  noId, varId, fncId, protId, paraId
} SymKind;

// 記号表構成
typedef struct {
  char    *name;  // 変数や関数の名前
  SymKind nmKind; // 種類
  DtType  dtType; // 変数、関数の型
  int     aryLen; // 配列長 0: 単純変数
  char    level;  // 定義レベル 0: 大域　1: 局所
  char    args;   // 関数の場合の引数個数
  int     adrs;   // 変数、関数の番地
} SymTbl;

// 命令語
typedef struct {
  unsigned char opcode; // 命令コード
  unsigned char flag;   // フラグ
  int           opdata; // 数値か番地
} Inst;