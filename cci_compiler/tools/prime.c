#include <stdio.h>

int is_prime(int n) {
  int i;

  if (n < 2) return 0;
  if (n == 2) return 1;
  if (n%2 == 0) return 0;

  for (int i = 3; i*i <= n; i = i+2) {
    if (n%i == 0)
      return 0;
  }
  return 1;
}

int main() {
  int n;

  for (n = 1; n <= 1000; n++) {
    if (is_prime(n))
      printf("%d ", n);
  }
  printf("\n");
}