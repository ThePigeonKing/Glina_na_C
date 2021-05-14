#ifndef LAB_STRUCTS
#define LAB_STRUCTS

typedef struct Info
{
    char* string;
}Info;

typedef struct QNode
{
    struct QNode *NW;    // ЛВ
    struct QNode *NE;    // ПВ
    struct QNode *SW;    // ЛН
    struct QNode *SE;    // ПН

    int N;  // предел по количеству ячеек
    int x_cord;
    int y_cord;

    int children;   // количество элементов "ниже"

    Info *info; // информация
}QNode;

enum CODE   // статус коды для удобства
{
    crit_err,
    fail,
    hold,
    success
};



#endif