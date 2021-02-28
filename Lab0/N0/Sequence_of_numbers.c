#include "stdio.h"
#include "stdlib.h"
#include "stddef.h"

struct refund getNat();
void sum_of_numbs(int x, int *amount, int *sum_n);  // сумма + кол-во цифр
void bubble_sort(int arr[], int len,int reverse, int arr_add[], int vary);  // банально, но наверное сойдет
void swap(int *n1, int *n2); // для смены элементов в массиве
void printArray(int arr[], int len);
int reverse(int n);

struct refund   // return было уже занято
{
  int number;
  int proof;
} refund;

int main() {
  int x, n;
  int amount = 0, sum_n = 0, flag = 0;
  struct refund nat_return, int_ret;
  printf("Введите количество элементов: ");
  while(flag == 0){
    int_ret = getNat();
    if (int_ret.proof == 1){
      flag = 1;
      n = int_ret.number;
    }
  }
  int* arr = malloc(n * sizeof(int)); // массив для ввода данных
  int* arr_sum =  malloc(n * sizeof(int)); // сумма цифр
  int* arr_amnt =  malloc(n * sizeof(int)); // количество цифр
  int* arr_res =  malloc(2 * n * sizeof(int)); // финальный массив
  int* arr_bckup =  malloc(n * sizeof(int)); // исходных данных
  printf("Введите элементы:\n");
  for (int i = 0; i < n; i++) {
    nat_return = getNat();
    if (nat_return.proof == 0){
      i -= 1;
    } else {
      arr[i] = nat_return.number;
      sum_of_numbs(nat_return.number, &amount, &sum_n);
      arr_sum[i] = sum_n;
      arr_amnt[i] = amount;
    }
    sum_n = 0;
    amount = 0;
  }
  for (int i = 0; i < n; i++){
    arr_bckup[i] = arr[i];
  }

  bubble_sort(arr, n, 0, arr_amnt, 1);
  for (int i = 0; i < n; i++){
    arr_res[i] = arr[i];
  }

  bubble_sort(arr_sum, n, 1, NULL, 0);
  for (int i = n; i < 2*n; i++){
    arr_res[i] = arr_sum[i-n];
  }

  printf("Исходный массив:\n");
  printArray(arr_bckup, n);
  printf("Массив преобразованный(упорядоченные по количеству цифр + сумма цифр): \n");
  printArray(arr_res, 2*n);

  free(arr);
  free(arr_sum);
  free(arr_amnt);
  free(arr_res);
  free(arr_bckup);

  return 0;
}


struct refund getNat() {
  struct refund myrefund;
  int r = scanf("%d", &myrefund.number);
  if (r == 0 || myrefund.number <= 0){
    printf("Wrong input!Try again\n");
    scanf("%*[^\n]");
    myrefund.proof = 0;
  }
  else {
    myrefund.proof = 1;
  }

  return myrefund;
}


void sum_of_numbs(int x, int *amount, int *sum_n){
  int numbers = 0, tmp = x;
  //x = reverse(x); // если слева направло
  while(x > 0){
    *sum_n += x % 10;
    *amount += 1;
    x = x / 100;
  }
}


void swap(int *n1, int *n2){
  int tmp = *n1;
  *n1 = *n2;
  *n2 = tmp;
}


void bubble_sort(int arr[], int len, int reverse, int arr_add[], int vary){
  int tmp = 0;
  if (arr_add == NULL){
    for (int i = 0; i < len-1; i++){
      for (int j = 0; j < (len-i-1); j++){
        if (arr[j] > arr[j+1]){
          swap(&arr[j], &arr[j+1]);
        }
      }
    }
    if (reverse == 1){
      for (int i = 0; i < len-1; i++){
        for (int j = 0; j < (len-i-1); j++){
          if (arr[j] < arr[j+1]){
            swap(&arr[j], &arr[j+1]);
          }
        }
      }
    }

  }else if(vary == 1){    // если arr надо менять
    for(int i = 0; i < len-1; i++){
      for(int j = 0; j < (len-i-1); j++){
        if (arr[j] > arr[j+1] && reverse == 0) {
          swap(&arr[j], &arr[j+1]);
          swap(&arr_add[j], &arr_add[j+1]);
        }else if(arr[j] < arr[j+1] && reverse == 1){
          swap(&arr[j], &arr[j+1]);
          swap(&arr_add[j], &arr_add[j+1]);
        }
      }
    }
  } else if(vary == 0) {      // если массив arr не надо менять
      int* arr_tmp = (int*) malloc(len * sizeof(int));
      for (int i = 0; i < len; i++){
        arr_tmp[i] = arr[i];
      }
      for(int i = 0; i < len-1; i++){
        for(int j = 0; j < (len-i-1); j++){
          if (arr_tmp[j] > arr_tmp[j+1] && reverse == 0) {
            swap(&arr_tmp[j], &arr_tmp[j+1]);
            swap(&arr_add[j], &arr_add[j+1]);
          } else if(arr_tmp[j] < arr_tmp[j+1] && reverse == 1){
            swap(&arr_tmp[j], &arr_tmp[j+1]);
            swap(&arr_tmp[j], &arr_tmp[j+1]);
          }
        }
      }
    }
  }

int reverse(int n) {
      int rev = 0;
      while (n != 0) {
          rev = (rev * 10) + (n % 10);
          n /= 10;
      }
      return rev;
}

void printArray(int arr[], int len){
  for (int i = 0; i < len; i++){
    printf("№%d = %d \n", i+1, arr[i]);
  }
}
