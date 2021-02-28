#include <stdio.h>
#include <stdlib.h>

void swap(int *n1, int *n2){
  int tmp = *n1;
  *n1 = *n2;
  *n2 = tmp;
}



void bubble_sort(int arr[], int len, int reverse, int arr_add[], int vary){
  int tmp = 0;
  if(vary == 1){    // если arr надо менять
    for(int i = 0; i < len-1; i++){
      for(int j = 0; j < (len-i-1); j++){
        if (arr[j] > arr[j+1] && reverse == 0) {
          swap(&arr[j], &arr[j+1]);
          swap(&arr_add[j], &arr_add[j+1]);
        } else if(arr[j] < arr[j+1] && reverse == 1){
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

int main(){
  int x, y, r_sem = 0, remainder;
  int arr[5];
  int arr_add[5] = {10, 20, 30, 40, 50};
  for(int i = 0; i < 5; i++){
    printf("Enter %d value: ", i+1);
    scanf("%d", &arr[i]);
    //printf("%d \n", arr[i]);
  }
  bubble_sort(arr, 5, 0, arr_add, 1);

  for(int i = 0; i < 5; i++){
    printf("%d value = %d  || arr2 = %d \n", i+1, arr[i], arr_add[i]);  }
}
