#include "stdio.h"
#include "string.h"
#include "stdlib.h"

char* getVector();

int main(){
  int x = 0, y, z;

  char str1[100];
  char res_arr[100];
  res_arr[99] = '\0';


  fgets(res_arr, 99, stdin);

  for (int i = 0; i < strlen(res_arr)-1; i++){
    if (res_arr[i] != ' ')
      printf("%d : %c\n", i+1, res_arr[i]);
    if (strlen(res_arr) == 0)
      printf("YES\n");

  }
  if (res_arr[0] == '\n')
    printf("YES\n");
  printf("arr = %d, len = %ld\n", res_arr[0],strlen(res_arr));

}

char* getVector(){
  char arr[100];
  arr[99] = '\0';
  fgets(arr, 99, stdin);
  return "a";
}
