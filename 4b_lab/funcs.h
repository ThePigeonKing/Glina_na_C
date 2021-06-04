#ifndef TPK_FUNCS_SOME
#define TPK_FUNCS_SOME

#include "structs.h"
#include "utils.h"

void mainloop();
QNode *insert(QNode *root, int x, int y, char *new_info, Node **rtrnbl);
int find_info(Node **root, int xtofind, int ytofind,int len);
CODE subdivide(QNode **root, int x_med, int y_med);
void append_info(Node **arr, int x, int y, char* new_info, int len);
void swap(int *xp, int *yp);
void bubbleSort(int arr[], int n);
void calc_median(Node **arr, int *x_new, int *y_new);
void info_print(Node *node);
void qtree_erase(QNode **root);
char* getLine();
    
#endif