#include <stdio.h>

int main() {
  int n, sum;

  sum = 0;
  printf("数値を入力してください。0を入力すれば終了します。\n");
  while (scanf("%d", &n)) {
    sum = sum + n;
    if (n == 0)
      break;
    printf("    > %d\n", sum);
  }
}