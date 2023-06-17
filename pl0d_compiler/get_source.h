

#include <stdio.h>

#ifndef __TBL_H
#define __TBL_H

#include "table.h"

#endif

// 名前の最大長
#define MAXNAME 31

// キーの文字の種類
typedef enum keys {
  // 予約語の名前
  Begin, End,
  If, Then,
  While, Do,
  Ret, Func,
  Var, Const, Odd,
  Write, WriteLn,
  end_of_KeyWd,
  // 予約語の名前はここまで
  // 演算子と区切り記号の名前
  Plus, Minus,
  Mult, Div,
  Lparen, Rparen,
  Equal, Lss, Gtr,
  NotEq, LssEq, GtrEq,
  Comma, Period, Semicolon,
  Assign,
  end_of_KeySym,
  // 演算子と区切り文字の名前はここまで
  // トークンの種類
  Id, Num, nul,
  end_of_Token,
  // 上記以外の文字の種類
  letter, digit, colon, others
} KeyId;

// トークンの型
typedef struct token {
  // トークンの種類かキーの名前
  KeyId kind;
  union {
    // Identifierの時、その名前
    char id[MAXNAME];
    // Numの時、その値
    int value;
  } u;
} Token;

// 次のトークンを読んで返す
Token nextToken();
// t.kind == kのチェック
Token checkGet(Token t, KeyId k);

/*
t.kind==kなら、次のトークンを読んで返す。
t.kind!=kならエラーメッセージを出し、tとkがともに記号、または予約語なら
tを捨て、次のトークンを読んで返す。(tをkに置き換える)
それ以外の場合、kを挿入したことにしてtを返す。
*/

// ソースファイルのopen
int openSource(char fileName[]);
// ソースファイルのclose
void closeSource();
// テーブルの初期設定、texファイルの初期設定
void initSource();
// ソースの最後のチェック、texファイルの最終設定
void finalSource();
// 型エラーを.texファイルに入力
void errorType(char *m);
// keyString(k)を.texファイルに出力
void errorInsert(KeyId k);
// 名前がないとのメッセージを.texファイルに挿入
void errorMissingId();
// 演算子がないとのメッセージを.texファイルに挿入
void errorMissingOp();
// 今読んだトークンを読み捨て(.texファイルに出力)
void errorDelete();
// エラーメッセージを.texファイルに出力
void errorMessage(char *m);
// エラーメッセージを出力し、コンパイル終了
void errorF(char *m);
// エラーの個数を返す
int errorN();

// 現トークン(Id)の種類をセット(.texファイル出力のため)
void setIdKind(KindT k);

