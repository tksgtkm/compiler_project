#include "get_source.h"
#ifndef __TABLE_H
#define __TABLE_H
#include "table.h"
#endif
#include "codegen.h"

// エラーがこれ以下なら実行
#define MINERROR 3
// 各ブロックの最初の変数アドレス
#define FIRSTADDR 2

// 次のトークンを入れておく
static Token token;

// ブロックのコンパイル
static void block(int pIndex);
// pindexはこのブロックの関数名のインデックス
// 定数宣言のコンパイル
static void constDecl();
// 変数宣言のコンパイル
static void varDecl();
// 関数宣言のコンパイル
static void funcDecl();
// 文のコンパイル
static void statement();
// 式のコンパイル
static void expression();
// 式の項のコンパイル
static void term();
// 式の因子のコンパイル
static void factor();
// 条件式のコンパイル
static void condition();
// トークンtは文の先頭のキーか？
static int isStBeingKey(Token t);

void block(int pIndex) {
  int backP;
  // 内部関数を飛び越す命令、あとでバックパッチ
  backP = getCodeV(jmp, 0);
  // 宣言部を飛び越す命令
  while (1) {
    switch(token.kind) {
      // 定数宣言部のコンパイル
      case Const:
        token = nextToken();
        constDecl();
        continue;
      // 変数宣言部のコンパイル
      case Var:
        token = nextToken();
        varDecl();
        continue;
      // 関数宣言部のコンパイル
      case Func:
        token = nextToken();
        funcDecl();
        continue;
      default:
        break;
    }
    break;
  }
  // 内部関数を飛び越す命令にパッチ
  backPatch(backP);
  // この関数の開始番地を修正
  changeV(pIndex, nextCode());
  // このブロックの実行時の必要記憶域をとる命令
  getCodeV(ict,frameL());
  // このブロックの主文
  statement();
  // リターン命令
  genCodeR();
  // ブロックが終わったことをtableに連絡
  blockEnd();
}

void constDecl() {
  Token temp;
  while (1) {
    if (token.kind == Id) {
      // 印字のための情報のセット
      setIdKind(constId);
      // 名前を入れておく
      temp = token;
      // 名前の次は"="のはず
      token = checkGet(nextToken(), Equal);
      if (token.kind == Num)
        enterTconst(temp.u.id, token.u.value); // 定数名と値をテーブルに
      else 
        errorType("number");
      token = nextToken();
    } else {
      errorMissingId();
    }

    // 次がコンマなら定数宣言が続く
    if (token.kind != Comma) {
      // 次が名前ならコンマを忘れる
      if (token.kind == Id) {
        errorInsert(Comma);
        continue;
      } else {
        break;
      }
    }
    token = nextToken();
  }
  // 最後は";"のはず
  token = checkGet(token, Semicolon);
}

void varDecl() {
  while (1) {
    if (token.kind == Id) {
      // 印字のための情報のセット
      setIdKind(varId);
      // 変数名をテーブルに、番地はtableが決める
      enterTvar(token.u.id);
      token = nextToken();
    } else {
      errorMissingId();
    }
    // 次がコンマなら変数宣言が続く
    if (token.kind != Comma) {
      // 次が名前ならコンマを忘れたことになる
      if (token.kind == Id) {
        errorInsert(Comma);
        continue;
      } else {
        break;
      }
    }
    token = nextToken();
  }
  // 最後は";"のはず
  token = checkGet(token, Semicolon);
}

void funcDecl() {
  int fIndex;
  if (token.kind == Id) {
    // 印字のための情報のセット
    setIdKind(funcId);
    // 関数名をテーブルに登録
    fIndex = enterTfunc(token.u.id, nextCode());
    // その先頭番地はまず、次のコードの番地nextCode()とする
    token = checkGet(nextToken(), Lparen);
    // パラメタ名のレベルは関数のブロックと同じ
    blockBegin(FIRSTADDR);
    while (1) {
      // パラメタ名がある場合
      if (token.kind == Id) {
        // 印字のための情報のセット
        setIdKind(parId);
        // パラメタ名をテーブルに登録
        enterTpar(token.u.id);
        token = nextToken();
      } else {
        break;
      }
      // 次がコンマならパラメタ名が続く
      if (token.kind != Comma) {
        if (token.kind == Id) {
          errorInsert(Comma);
          continue;
        } else {
          break;
        }
      }
      token = nextToken();
    }
    // 最後は")"のはず
    token = checkGet(token, Rparen);
    // パラメタ部が終わったことをテーブルに連絡
    endpar();
    if (token.kind == Semicolon) {
      errorDelete();
      token = nextToken();
    }
    // ブロックのコンパイル、その関数名のインデックスを壊す
    block(fIndex);
    // 最後は";"のはず
    token = checkGet(token, Semicolon);
  } else {
    // 関数名がない
    errorMissingId();
  }
}

void statement() {
  int tIndex;
  KindT k;
  // バックパッチ用
  int backP, backP2;

  while (1) {
    switch(token.kind) {
      // 代入文のコンパイル
      case Id:
        // 左辺の変数のインデックス
        tIndex = searchT(token.u.id, varId);
        
    }
  }
}