#include <stdio.h>
#include "get_source.h"


int main() {
  // ソースプログラムファイルの名前
  char fileName[30];
  printf("enter source file name\n");
  scanf("%s", fileName);
  // ソースプログラムファイルのopen
  if (!openSource(fileName))
    // open失敗すれば終わり
    return;
  // コンパイルして
  if (compile())
    // エラーがなければ実行
    execute();
  // ソースプログラムファイルのclose
  closeSource();
}