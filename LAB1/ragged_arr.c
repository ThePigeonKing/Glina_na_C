#include "stdio.h"
#include "stdlib.h"

#define DEBUG 0
#define print_debug(fmt, ...) \
	do { if (DEBUG) fprintf(stderr, fmt, __VA_ARGS__); } while (0)


typedef struct Line{ // строка матрицы
  int n;
  double *arr;
} Line;

typedef struct Matrix{  // сама матрица
  int lines;
  Line *matr;
} Matrix;

int getInt(int *a);
int getDouble(double *a);
int Matr_in(Matrix *a); // ввод матрицы
void Matr_out(Matrix a); // вывод матрицы
void Matr_free(Matrix *a); // free для матрицы
Matrix do_something(Matrix *a); // удаление единичных элементов
int in_arr(double *ptr, int len, int index); // проверка находится ли в массиве




int main() {
  double res;
  Matrix matr = {0, NULL}; // исходная м-ца
  Matrix new_matr;
  if (Matr_in(&matr) == 0){
    printf("%s\n", "!EOF found!");
    return 1;
  }

  printf("Исходная матрица:\n");
  Matr_out(matr);

  new_matr = do_something(&matr);
  printf("Финальная матрица:\n");
  Matr_out(new_matr);

  return 0;
}

int getInt(int *a){
  int response = 0;
  while(response == 0){
    response = scanf("%d", a);
    if (response < 0){  // EOF
      return 0;
    } else if (response == 0){
      printf("%s\n", "INT error, try again..");
      scanf("%*[^\n]");
    }
  }
  return 1;
}

int getDouble(double *a){
  int response = 0;
  while(response == 0){
    response = scanf("%lf", a);
    if (response < 0){  // EOF
      return 0;
    } else if (response == 0){
      printf("%s\n", "DOUBLE error, try again..");
      scanf("%*[^\n]");
    }
  }
  return 1;
}

int Matr_in(Matrix *rm){
  const char *err = ""; // err message
  int m; // кол-во строк
  int i, j;
  double *ptr;

  do{
      printf("%s\n", err);
      printf("Enter number of lines -->  ");
      err = "Wrong input! Try again..";
      if(getInt(&m) == 0)
        return 0; //EOF
  } while (m < 1);
  (*rm).lines = m;

  //память под массив строк
  (*rm).matr = (Line *)calloc(m, sizeof(Line));

  print_debug("Matr with %d lines", (*rm).lines);
  for (i = 0; i < (*rm).lines; ++i){
    err = "";
    // память под каждую строку матрицы
    do{
      print_debug("%s\n", err);
      print_debug("Enter amount of elements in line %d ---> ", i +1);
      err = "Wrong input! Try again..";
      if (getInt(&m) == 0){
        //очищение памяти
        (*rm).lines = i;
        Matr_free(rm);
        return 0;
      }
    } while (m < 1);

    (*rm).matr[i].n = m;
    // память под элементы строки
    ptr = (double *)malloc(sizeof(double) * m);
    (*rm).matr[i].arr = ptr;

    //элементы строки
    printf("Enter elements for line #%d: \n", i+1);
    for (j = 0; j < m; ++j){
      if(getDouble(ptr) == 0){
        //очистка памяти
        (*rm).lines = i + 1;
        Matr_free(rm);
        return 0;
      }
      print_debug("Appending elem %lf in line %d \n", *ptr, m);
      ++ptr;
    }

  }
  return 1;
}

void Matr_out(Matrix a){

  double *ptr;
  for (int i = 0; i < a.lines; ++i){
    ptr = a.matr[i].arr;
    for (int j = 0; j < a.matr[i].n; ++j){
      printf("| %10.2lf ", *ptr);
      ++ptr;
    }
    printf("|\n");
  }
}

void Matr_free(Matrix *a){
  for (int i = 0; i < (*a).lines; ++i){
    free((*a).matr[i].arr);
  }
  free((*a).matr);
  (*a).lines = 0;
  (*a).matr = NULL;
}

Matrix do_something(Matrix *a){
  int num = 0, counter = 0; // количество обрезанных элементов
  double *p;
  Matrix new_matr = {0, NULL};
  new_matr.lines = a->lines;
  //print_debug("Lines in second matrix - %d \n", new_matr.lines);
  new_matr.matr = (Line *)calloc(new_matr.lines, sizeof(Line));


  for (int i = 0; i < a->lines; ++i){ // по строкам
    new_matr.matr[i].n = a->matr[i].n;
    print_debug("Elems in line %d = %d\n", i+1, new_matr.matr[i].n);

    p = (double *)malloc(sizeof(double) * new_matr.matr[i].n);
    new_matr.matr[i].arr = p;

    for (int j = 0; j < a->matr[i].n; ++j){ // по элементам
      print_debug("// j = %d\n", j);
      print_debug("CALL: lenght = %d, index = %d \n", new_matr.matr[i].n, j);
      for (int k = 0; k < new_matr.matr[i].n; ++k)
        print_debug("ELEM %d = %lf\n", k+1, a->matr[i].arr[k]);

      if (in_arr(a->matr[i].arr, new_matr.matr[i].n, j) == 1){
          print_debug("Trying to write new_matr.matr[%d].arr[%d]\n", i, counter);
          p[counter] = a->matr[i].arr[j];
          counter++;
      }
    } // j
    print_debug("counter - %d\n", counter);
    new_matr.matr[i].arr = realloc(new_matr.matr[i].arr, (counter) * sizeof(double));
    new_matr.matr[i].n = counter;
    counter = 0;

  } // i

  return new_matr;
}


int in_arr(double *ptr, int len, int index){
  print_debug("%s", "| GIVEN ARRAY | - ");
  for (int j = 0; j < len; ++j)
    print_debug("%.2lf / ", ptr[j]);

  for (int i = 0; i < len; ++i){

      if ((ptr[index] == ptr[i] && index != i) || ptr[index] == 0){
        return 1;
      }
  }
  return 0;
}
