#include <stdio.h>
#include "get_source.h"


int main() {
  char fileName[30];
  printf("enter source file name\n");
  scanf("%s", fileName);
  if (!openSource(fileName))
    return;
  if (compile())
    execute();
  closeSource();
}