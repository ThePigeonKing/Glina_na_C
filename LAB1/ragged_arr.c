#include "stdio.h"
#include "stdlib.h"
#include "stddef.h"

typedef struct Line{
  int n;  // количество элементов в строке
  double *arr; // указатель на первый элемент
} Line;

typedef struct Matrix{
  int lines; // количество строк
  Line *matr; // динамический массив строк матрицы к такой дряни обращаться ->
                                                    // pmatr->x = (*pmatr).x
} Matrix;

// если функция возвращает 0, то найден конец файла
int getDouble(double *a);
int getInt(int *a);
int m_input(Matrix *rm);
void m_output(Matrix a);
void do_work(Matrix a); // удаление единичных значений
int in_array(double num, double *p, int telly); // проверка находится ли в массиве
void arr_copy(int *a[], int *b[], int len);

int main(){
  int x, y;
  double a, b;
  char msg;

  Matrix matr = {0, NULL}; // исходник
  double res;

  if (m_input(&matr) == 0){
    printf("Конец файла!\n");
    return 1;
  }
  printf("--- Исходная матрица ---\n");
  m_output(matr);
  do_work(matr);

}

int getInt(int *a){
  int tmp;
  do{
      tmp = scanf("%d", a);
      if (tmp < 0) // конец файла
        return 0;
      if (tmp == 0){ // TypeError
          printf("Wrong input(not int)!Try again...\n");
          scanf("%*[^\n]");
      }
  }while(tmp == 0);
  return 1;
}

int getDouble(double *a){
  int tmp;
  do{
      tmp = scanf("%lf", a);
      if (tmp < 0)
        return 0;
      if (tmp == 0){
        printf("Wrong input(not double)!Try again...\n");
        scanf("%*[^\n]");
      }
  }while(tmp == 0);
  return 1;
}


int m_input(Matrix *rm){
  int m; // строки матрицы
  const char *err = "";   // ошибка
  int i, j;
  double *p;
  do {
      printf("%s\n", err);
      printf("Введите количество строк --->\n");
      err = "Ошибка! Повторите ввод...";
      if (getInt(&m) == 0)
        return 0;
  }   while(m < 1);
  rm->lines = m;

  // память под строки
  rm->matr = (Line*)calloc(m, sizeof(Line));

  for (i = 0; i < rm->lines; i++){
    err = "";
    do{
        printf("%s\n", err);
        printf("Введите количество элементов в строке %d --->\n", i+1);
        err = "Ошибка! Повторите ввод...";
        if (getInt(&m) == 0){
          // конец файла
          rm->lines = i;
          // DELETE
          return 0;
        }
    }   while(m < 1);
    rm->matr[i].n = m;
    // память под строку
    p = (double *)malloc(sizeof(double) * m);
    rm->matr[i].arr = p;

    //ввод элементов строки
    printf("Введите элементы строки #%d: \n", i+1);
    for (j = 0; j < m; j++){
      if (getDouble(p) == 0){
        //конец файла
        rm->lines = i + 1;
        //DELETE
        return 0;
      }
      p += sizeof(double);
    }
  }
  return 1;
}


void m_output(Matrix a){
  int i, j;
  double *p;
  for (i = 0; i < a.lines; ++i){
      p = a.matr[i].arr;
      for (j = 0; j < a.matr[i].n; j++){
        printf("%10.2lf | ", *p);
        p += sizeof(double);
      }
      printf("\n");
  }
}

void do_work(Matrix a){
  int i, j, un = 0;
  double *p;

  for (i = 0; i < a.lines; i++){ // цикл по строкам
    double *copied_arr = (double *)malloc(a.matr[i].n * sizeof(double));
      for (j = 0; j < a.matr[i].n; j++){ // цикл по элементам строки
          copied_arr[j] = a.matr[i].arr[j];
          printf("%lf\n", a.matr[i].arr[j]);
      }
      // for (j = 0; j < a.matr[i].n; i ++)
      //   printf("%lf | ", *(a.matr[i].arr + j));
free(copied_arr);
  }

}


int in_array(double num, double *p, int tally){
  for (int i = 0; i < tally; i++){
    if (*(p + i) == num)
      return 1;
  }
  return 0;
}
