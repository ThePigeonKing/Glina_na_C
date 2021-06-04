#ifndef LAB_STRUCTS
#define LAB_STRUCTS

#define N 4

typedef struct Node
{
    int x;
    int y;

    char *info;
}Node;


typedef struct QNode
{
    struct QNode *NW;    // ЛВ
    struct QNode *NE;    // ПВ
    struct QNode *SW;    // ЛН
    struct QNode *SE;    // ПН

    struct QNode *prev; // предыдущий квадрат

    int x_cord;
    int y_cord;

    int exists;   // количество элементов "ниже"
    int subdivided; // есть ли ниже дерево

    Node **array;
}QNode;

typedef enum CODE   // статус коды для удобства
{
    crit_err,
    fail,
    hold,
    success
}CODE;

#define crtnew(t) ((t*)calloc(1, sizeof(t)))

#endif