#include "stdio.h"
#include "stdlib.h"
#include "stddef.h"

typedef struct Line{
  int n; // количество элементов в строке
  double *a; // массив элементов
} Line;

typedef struct Matrix{
  int lines; // кол-во строк
  Line *matr; // массив строк матрицы
} Matrix;

//функи
int getInt(int *);
int getDouble(double *);
int input(Matrix *a); // ввод матрицы
void output(const char *msg, Matrix a); // печать матрицы
struct Matrix calculate(Matrix *a); // изменение строк
void erase(Matrix *a); // освобождение памяти


int main(){

  Matrix matr = {0, NULL}; // исходная матрица
  double res;

  if (input(&matr) == 0){
    printf("%s\n", "Конец файла");
    return 1;
  }


  return 0;
}


int input(Matrix *rm){
  const char *pr = ""; // ошибка
  int m; // количество строк
  int i, j;
  double *p;
  do{
    printf("%s\n", pr);
    printf("Введите кол-во строк ---> ");
    pr = "Ошибка! Повторите ввод ";
    if (getInt(&m) == 0){
      return 0; // конец файла => выход
    }
  } while (m < 1);
  rm->lines = m;  // аналог (*rm).lines = m

  // память под строки матрицы
  rm->matr = (Line *)calloc(m, sizeof(Line));
  for (i = 0; i < rm->lines; i++){ // кол-во столбцов для каждой строки
    pr = "";
    do{
      printf("%s\n", pr);
      printf("Введите кол-во элементов в строке %d---> ", i+1);
      pr = "Ошибка, повторите ввод!";
      if (getInt(&m) == 0){
        rm->lines = i; // количество выделенных строк
        erase(rm);
        return 0;
      }
    } while (m < 1);
    rm->matr[i].n = m;
    p = (double *)malloc(m * sizeof(double));
    rm->matr[i].a = p;

    printf("Введите значения для элементов строки %d: \n", i+1);
    for (j = 0; j < m; ++i, ++p){
      if (getDouble(p) == 0){
        rm->lines = i + 1;
        erase(rm);
        return 0;
      }
    }
  }
  return 1;
}

void output(const char *msg, Matrix a){
  int i, j;
  double *p;
  printf("%s\n", msg);
  for (i = 0; i < a.lines; ++i){
    p = a.matr[i].a; //???????
    for (j = 0; j < a.matr[i].n; ++i, ++p)
      printf("%10lf ", *p);
    printf("\n");
  }
}

int getInt(int *a){
  int n;
  do{
    n = scanf("%d", a);
    if (n < 0)  // конец файла
      return 0;
    if (n == 0){ // некорректный формат
      printf("%s\n", "Ошибка типа, повторите ввод");
      scanf("%*[^\n]");
    }
  } while (n == 0);

  return 1;   }

int getDouble(double *a){
  int n;
  do{
    n = scanf("%lf", a);
    if (n < 0) // конец файла
      return 0;
    if (n == 0){
      printf("%s\n", "Ошибка, повторите ввод");
      scanf("%*[^\n]");
    }
  } while (n == 0);

  return 1;
}

void erase(Matrix *a){
  int i;
  for (i = 0; i < a->lines; ++i)
    free(a->matr[i].a);
  free(a->matr);
  a->lines = 0;
  a->matr = NULL;

}
