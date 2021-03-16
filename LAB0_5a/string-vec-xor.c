#include "stdio.h"
#include "string.h"
#include "stdlib.h"

#define DEBUG 0
#define print_debug(fmt, ...) \
	do { if (DEBUG) fprintf(stderr, fmt, __VA_ARGS__); } while (0)

#define KEY "ponpon"
#define KEYLEN 6


char* getLine(char *arr);  // получение всей строки целиком
void split_line(char *arr, char* delim); // разбиение строки + обработка
int convert_to_int(char *convert); // перевод в целочисленные
int xor(int a, int b);


int main(int argc, char const *argv[]) {

  int response = 777;
  char* arr;
  char* delim = " ";

  printf("Enter lines (end with Ctrl+D)-->\n");

  while ((arr = getLine(arr)) != NULL){  // ждёт EOF'а

    print_debug("Entered line(main) - %s\n", arr);
    split_line(arr, delim);
  }
  free(arr);
  printf("    EOF found. Shutting down...\n");

  return 0;
}


char* getLine(char *arr){   // получение цельной строки
  int rsp, space = 100;
  arr = (char *)malloc(space * sizeof(char));
  char *tmp;
  rsp = scanf("%99[^\n]", arr);
  print_debug("READ LINE - %s\n", arr);
  print_debug("RSP1 = %d\n", rsp);
  while (rsp == 1){
    space += 99;
    print_debug("RSP2 = %d, SPACE = %d\n", rsp, space);
    tmp = realloc(arr, space * sizeof(char)); // выделяем ещё место
    if (tmp == NULL){
      rsp = -1;
      break;
    }
    rsp = scanf("%99[^\n]", &arr[space - 100]);
    print_debug("READ LINE - %s\n", arr);
  }

  scanf("%*1[\n]");
  if (rsp < 0){
    free(arr);
    return NULL; // EOF
  }

  return arr;
}


void split_line(char *arr, char *delim){
  int num, counter = 0;
  char *ptr = strtok(arr, delim);

  while (ptr != NULL){
    print_debug("LINE - %s \n", ptr);
    num = convert_to_int(ptr); // конвертирование
    if (num != -1){
      printf("| %c ", (char)xor(num, (int)KEY[counter%KEYLEN]));
      counter++;
    } else {
      printf("| Err ");
    }

    ptr = strtok(NULL, delim);
  }

  printf("\n");
}


int convert_to_int(char *convert){
  int x;
  x = atoi(convert);
  if (x != 0 || strcmp(convert, "0") == 0){ // отличие 0 как число от 0 как ошибка

    if (x <= 255 && x >= 0){
        print_debug("Second if, %d\n", x);
        return x;
    }
  }

  return -1;  // Ошибка
}


int xor(int a, int b){
  return a^b;
}
