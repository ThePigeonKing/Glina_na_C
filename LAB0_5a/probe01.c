#include "stdio.h"
#include "string.h"
#include "stdlib.h"

#define KEY "abcde"


int main() {
  char a[] = "abc\0";
  char b[] = "ponpon\0";
  int x = (int)a[0];
  char res[100];
  res[99] = '\0';
  printf("%ld\n", strlen(a));
  for (int i = 0; i < strlen(a)-1; i++)
    res[i] = a[i] ^ b[i];
  res[99] = '\0';
  printf("%c\n", (char)x);

  printf("KEY: %d\n", (int)KEY[0]);
  return 0;
}
