#include "stdio.h"
#include "string.h"
#include "stdlib.h"

#define KEY "ponpon"
#define KEYLEN 6

int getStr();
int convert_to_int(char *arr);
int xor(int a, int b);
char getVector();


int main(){
  int i = 0, r = 0;
  char res_arr[100];
  char vector[100];
  res_arr[99] = '\0';
  vector[99] = '\0';

  while (r != -666){
    r = getStr();
    if (r == -666){   // EOF
      break;
    } else if (r != -1){  // когда всё ок
      // printf("Xoring %d with: %c\n", r, KEY[i%6]); // DEBUG
      printf("%c | ", (char)xor(r, (int)KEY[i%KEYLEN]));
    } else {
      i--;
    }
    i++;
  }
}

char getVector(int iter){
  char vec_arr[100];
  vec_arr[99] = '\0';
  fgets(vec_arr, 99, stdin);
  return vec_arr[iter];
}

int getStr(){
  char array[100];
  char vec_bck;
  array[99] = '\0';
  int p, iter = 0, arr_i = 0;

  while (1 == 1){
    vec_bck = getVector(iter);
    iter += 1;
    if (vec_bck != ' '){
      array[arr_i] = vec_bck;
      arr_i += 1;
    } else if (vec_bck == ' ' && array[0] == '\n'){
      continue;
    } else if (vec_bck == ' ' && array[0] != '\n'){
      
    }
  }
  if (r >= 0){
    p = convert_to_int(array);      // конвертирование в цифру
    if (p == -1){
      printf("/*Wrong input. Try again...*/ ");
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
  int dec = 1, term;

  for (int i = 0; i < strlen(arr)-1; i++){
    dec = dec * 10;  // недоpow
    //printf("%d\n", dec);  // DEBUG
  }


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
    return -1;  // если за рамки значений
}

int xor(int a, int b){
  return a^b;
}
