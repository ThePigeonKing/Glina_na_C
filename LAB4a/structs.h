#ifndef FOUR_STRUCTS
#define FOUR_STRUCTS

typedef struct Info
{
    double num1;
    double num2;
    char* string;
}Info;

typedef struct Node
{
    char *key;
    Info *info;
    struct Node *left;
    struct Node *right;

} Node;


enum CODES{   // статус коды для отлова ошибок
    fail,
    success,
    hold,
    crit_error
};


#endif