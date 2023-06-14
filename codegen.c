
#include <stdio.h>
#include "codegen.h"
#ifndef __TBL_H
#define __TBL_H
#include "table.h"
#endif
#include "get_source.h"

// 目的コードの最大長
#define MAXCODE 200
// 実行時スタックの長さ
#define MAXMEM 2000
// 演算レジスタスタックの長さ
#define MAXREG 20
// ブロックの最大深さ
#define MAXLEVEL 5

// 命令語の型
typedef struct inst {
  OpCode opCode;
  union {
    RelAddr addr;
    int value;
    Operator optr;
  } u;
} Inst;

// 目的コードが入る
static Inst code[MAXCODE];
// 最後に生成した命令語のインデックス
static int cIndex = -1;
// 目的コードのインデックスの増加とチェック
static void checkMax();
// 命令語の印字
static void printCode(int i);

// 次の命令語のアドレスを返す
int nextCode() {
  return cIndex + 1;
}

// 命令語の生成、アドレス部に
int getCodeV(OpCode op, int v) {
  checkMax();
  code[cIndex].opCode = op;
  code[cIndex].u.value = v;
  return cIndex;
}