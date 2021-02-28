#include "stdio.h"
#include "string.h"
#include "stdlib.h"

#define KEY "ponpon"

int getStr();
int convert_to_int(char *arr);
int xor(int a, int b);


int main(){
  int x, i = 0, r = 0;
  char str1[9];
  char res_arr[100];
  res_arr[99] = '\0';

  while (r != -666){
    r = getStr();
    if (r == -666){   // EOF
      break;
    } else if (r != -1){  // когда всё ок
      printf("%c\n", (char)xor(r, (int)KEY[i]));
    }
    i++;
  }

}


int getStr(){
  char array[100];
  array[99] = '\0';
  int r, p;

  r = scanf("%s", array);
  if (r >= 0){
    p = convert_to_int(array);      // конвертирование в цифру
    if (p == -1){
      printf("Not a number. Try again...\n");
      return -1;
    }
    else if (p == -255){
      printf("Number is out of range. Try again...\n");
      return -1;
    }
    else
      return p;
  }
  else{
    printf("EOF\n");
    return -666;
  }
  scanf("%*[^\n]");
}

int convert_to_int(char *arr){
  int result = 0;
  int dec = 1, term = strlen(arr) * 10;


  for (int i = 0; i < strlen(arr); i++){  // переводим в int
    if (arr[i] == '0')
      term = 0;
    else if (arr[i] == '1')
      term = 1;
    else if (arr[i] == '2')
      term = 2;
    else if (arr[i] == '3')
      term = 3;
    else if (arr[i] == '4')
      term = 4;
    else if (arr[i] == '5')
      term = 5;
    else if (arr[i] == '6')
      term = 6;
    else if (arr[i] == '7')
      term = 7;
    else if (arr[i] == '8')
      term = 8;
    else if (arr[i] == '9')
      term = 9;
    else
      return -1;    // если это не похоже на цифру
    result += term * dec;
    dec = dec / 10;
  }
  if (result <= 255 && result >= 0)
    return result;
  else
    return -255;  // если за рамки значений
}

int xor(int a, int b){
  return a^b;
}
