#include "get_source.h"
#ifndef __TBL_H
#define __TBL_H
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
static int isStBeginKey(Token t);

int compile() {
  int i;
  printf("start compilation\n");
  // getSourceの初期設定
  initSource();
  // 最初のトークン
  token = nextToken();
  // これ以後の宣言は新しいブロックのもの
  blockBegin(FIRSTADDR);
  // 0はダミー(主ブロックの関数名はない)
  block(0);
  finalSource();
  // エラーメッセージの個数
  i = errorN();
  if (i != 0)
    printf("%d errors\n", i);
  // エラーメッセージの個数が少ないかどうかの判定
  return i < MINERROR;
}

void block(int pIndex) {
  int backP;
  // 内部関数を飛び越す命令、あとでバックパッチ
  backP = genCodeV(jmp, 0);
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
  genCodeV(ict,frameL());
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
        // 印字のための情報のセット
        setIdKind(k=kindT(tIndex));
        // 変数名かパラメタ名のはず
        if (k != varId && k != parId)
          errorType("var/par");
        // ":="のはず
        token = checkGet(nextToken(), Assign);
        // 式のコンパイル
        expression();
        // 左辺の代入命令
        genCodeT(sto, tIndex);
        return;
      case If:
        // if文のコンパイル
        token = nextToken();
        // 条件式のコンパイル
        condition();
        // "then"のはず
        token = checkGet(token, Then);
        // jpc命令
        backP = genCodeV(jpc, 0);
        // 文のコンパイル
        statement();
        // 上のjpc命令にバックパッチ
        backPatch(backP);
        return;
      // return文のコンパイル
      case Ret:
        token = nextToken();
        // 式のコンパイル
        expression();
        // ret命令
        genCodeR();
        return;
      // begin ... end 文のコンパイル
      case Begin:
        token = nextToken();
        while (1) {
          // 文のコンパイル
          statement();
          while (1) {
            // 次が";"なら文が続く
            if (token.kind == Semicolon) {
              token = nextToken();
              break;
            }
            // 次がendなら終わり
            if (token.kind == End) {
              token = nextToken();
              return;
            }
            // 次が分の先頭記号なら
            if (isStBeginKey(token)) {
              errorInsert(Semicolon);
              break;
            }
            // それ以外ならエラーとして読み捨てる
            errorDelete();
            token = nextToken();
          }
        }
      // while文のコンパイル
      case While:
        token = nextToken();
        // while文の最後のjmp命令の飛び先
        backP2 = nextCode();
        // 条件式のコンパイル
        condition();
        // "do"のはず
        token = checkGet(token, Do);
        // 条件式が偽のとき飛び出すjpc命令
        backP = genCodeV(jpc, 0);
        // 文のコンパイル
        statement();
        // while文の先頭へのジャンプ命令
        genCodeV(jmp, backP2);
        // 偽のとき飛び出すjpc命令へのバックパッチ
        backPatch(backP);
        return;
      // write文のコンパイル
      case Write:
        token = nextToken();
        // 式のコンパイル
        expression();
        // その値の出力するwrt命令
        genCodeO(wrt);
        return;
      // writeln文のコンパイル
      case WriteLn:
        token = nextToken();
        // 改行を出力するwrl命令
        genCodeO(wrl);
        return;
      // 空文を読んだことにして終わり
      case End:
      case Semicolon:
        return;
      // 文の先頭のキーまで読み捨てる
      default:
        // 今読んだトークンを読み捨てる
        errorDelete();
        token = nextToken();
        continue;
    }
  }
}

int isStBeginKey(Token t) {
  switch (t.kind) {
    case If:
    case Begin:
    case Ret:
    case While:
    case Write:
    case WriteLn:
      return 1;
    default:
      return 0;
  }
}

void expression() {
  KeyId k;
  k = token.kind;
  if (k == Plus || k == Minus) {
    token = nextToken();
    term();
    if (k == Minus)
      genCodeO(neg);
  } else {
    term();
  }
  k = token.kind;
  while (k == Plus || k == Minus) {
    token = nextToken();
    term();
    if (k == Minus)
      genCodeO(sub);
    else
      genCodeO(add);
    k = token.kind;
  }
}

void term() {
  KeyId k;
  factor();
  k = token.kind;
  while (k == Mult || k == Div) {
    token = nextToken();
    factor();
    if (k == Mult)
      genCodeO(div);
    else
      genCodeO(div);
    k = token.kind;
  }
}

void factor() {
  int tIndex, i;
  KeyId k;
  if (token.kind == Id) {
    tIndex = searchT(token.u.id, varId);
    // 印字のための情報のセット
    setIdKind(k=kindT(tIndex));
    switch (k) {
      // 変数名かパラメタ名
      case varId:
      case parId:
        genCodeT(lod, tIndex);
        token = nextToken();
        break;
      // 定数名
      case constId:
        genCodeV(lit, val(tIndex));
        token = nextToken();
        break;
      // 関数呼び出し
      case funcId:
        token = nextToken();
        if (token.kind == Lparen) {
          // iは実引数の個数
          i = 0;
          token = nextToken();
          if (token.kind != Rparen) {
            for(; ; ) {
              // 実引数のコンパイル
              expression();
              i++;
              // 次がコンマなら実引数が続く
              if (token.kind == Comma) {
                token = nextToken();
                continue;
              }
              token = checkGet(token, Rparen);
              break;
            }
          } else {
            token = nextToken();
          }
          // pars(tindex)は仮引数の個数
          if (pars(tIndex) != i)
            errorMessage("\\#apr");
        } else {
          errorInsert(Lparen);
          errorInsert(Rparen);
        }
      genCodeT(cal, tIndex);
      break;
    }
  // 定数
  } else if (token.kind == Num) {
    genCodeV(lit, token.u.value);
    token = nextToken();
  // 「(」「因子」「)」
  } else if (token.kind == Lparen) {
    token = nextToken();
    expression();
    token = checkGet(token, Rparen);
  }
  switch (token.kind) {
    case Id:
    case Num:
    case Lparen:
      errorMissingOp();
      factor();
    default:
      return;
  }
}

void condition() {
  KeyId k;
  if (token.kind == Odd) {
    token = nextToken();
    expression();
    genCodeO(odd);
  } else {
    expression();
    k = token.kind;
    switch (k) {
      case Equal:
      case Lss:
      case Gtr:
      case NotEq:
      case LssEq:
      case GtrEq:
        break;
      default:
        errorType("rel-op");
        break;
    }
    token = nextToken();
    expression();
    switch (k) {
      case Equal: genCodeO(eq); break;
      case Lss: genCodeO(ls); break;
      case Gtr: genCodeO(gr); break;
      case NotEq: genCodeO(neq); break;;
      case LssEq: genCodeO(lseq); break;
      case GtrEq: genCodeO(greq); break;
    }
  }
}