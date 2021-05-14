#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "structs.h"
#include "utils.h"
#include "funcs.h"

void print_yel(char *msg){
    printf(ANSI_COLOR_YELLOW"%s"ANSI_COLOR_RESET, msg);
}

char* getLine(){    // считывание строки произвольной длины

    char *line = (char *)malloc(sizeof(char));
    *line = '\0';
    if (line == NULL)   //!MemError
        return NULL;
    char buf[81];
    int rsp, bf_len, ln_len = 0;
    
    do{
        rsp = scanf("%80[^\n]", buf);
        if (rsp == 0){
            scanf("%*[\n]");
        }
        if (rsp > 0){
            bf_len = strlen(buf);
            if (bf_len == 0){   // пустая строка

                rsp = 0;
                continue;
            } 
            ln_len += bf_len;
            line = realloc(line, (ln_len + 1)* sizeof(char));
            if (line == NULL){  //!MemError
                return NULL; 
            }
            strcat(line, buf);
        }
    } while( rsp == 0 || bf_len == 80);

    if (rsp < 0){
        printf("EOF found!\n");
        free(line);
        return NULL;
    }
    
    
    return line;
}

enum CODE getInt(int *num){

    int rsp;
    while(1){

    rsp = scanf("%d", num);

    if (rsp > 0){
        scanf("%*[^\n]");

        return success;
    } else if (rsp == 0){
        scanf("%*[^\n]");
        print_yel("Wrong int! Try again..\n");
    }   else {
        print_yel("EOF found!\n"); // !EOF
        return crit_err;
    }
}
    return fail;
}

void commands_print(){
    printf("\n");
    printf("1. Insert new node\n");
    printf("2. Find by coordinates\n");
    printf("3. Print with limits\n");
    printf("0. Exit\n");
    printf("\n");
}

void inorder_print(QNode *p){

    if (NULL == p){
        return;
    }

    inorder_print(p->SW);
    inorder_print(p->NW);
    printf(ANSI_COLOR_RED"(%d, %d) | "ANSI_COLOR_RESET, p->x_cord, p->y_cord);
    inorder_print(p->SE);
    inorder_print(p->NE);
}

void preorder_with_lim(QNode *p, int x_up, int x_low, int y_up, int y_low){

    if (NULL == p){
        return;
    }

    if ((x_low < p->x_cord && x_up > p->x_cord) && (y_low < p->y_cord && y_up > p->y_cord)){
        printf(ANSI_COLOR_RED"(%d, %d) --> %s \n"ANSI_COLOR_RESET, p->x_cord, p->y_cord, p->info->string);
    }
    preorder_with_lim(p->SW, x_up, x_low, y_up, y_low);
    preorder_with_lim(p->NW, x_up, x_low, y_up, y_low);
    preorder_with_lim(p->SE, x_up, x_low, y_up, y_low);
    preorder_with_lim(p->NE, x_up, x_low, y_up, y_low);
}

void erase_qtree(QNode *root){

    if (NULL == root){
        return;
    }

    erase_qtree(root->SE);
    erase_qtree(root->SW);
    erase_qtree(root->NW);
    erase_qtree(root->NE);

    free(root->info->string);
    free(root->info);
    free(root);
}

void info_print(Info *node){
    printf("=================\n");
    printf("%s\n", node->string);
    printf("=================\n\n\n");
}

enum CODE getcoords(int *x, int *y){    // получение (x, y)

    enum CODE status;
    printf("Enter x key: ");
    status = getInt(x);
    if (status != success){     //!MemError
        print_yel("Error in getting x key!\n");
        return crit_err;
    }

    printf("Enter y key: ");
    status = getInt(y);
    if (status != success){     //!MemError
        print_yel("Error in getting x key!\n");
        return crit_err;
    }

    return success;
}

Info *info_create(){    // создание информации
    Info *new_info = (Info *)calloc(1, sizeof(Info));
    int rsp;
    enum CODE stat = hold;

    printf("Enter string for new info: ");
    new_info->string = getLine();
    if (NULL == new_info->string){
        print_yel("Error in string in info_create!\n");
        free(new_info);
        return NULL;
    }

    return new_info;    
}

QNode *create_node(){

    QNode *new_node = (QNode *)malloc(sizeof(QNode));
    if (NULL == new_node){  // !MemError
        print_yel("Error in allocating memory for node!\n");
        return NULL;
    }

    //new_node->N = 4;
    new_node->children = 0;
    new_node->NE = NULL;
    new_node->NW = NULL;
    new_node->SW = NULL;
    new_node->SE = NULL;
    new_node->info = NULL;

    return new_node;
}

QNode *insert(QNode *root, int x, int y, Info **rtrnbl){

    if (NULL == root){  // в конце списка, добавляем ячейку
        QNode *new_node = create_node();
        
        assert(new_node != NULL);   // !MemError

        new_node->info = info_create();

        assert(new_node->info != NULL);
        new_node->x_cord = x;
        new_node->y_cord = y;  

        return new_node;  
    }
        

    if (root->x_cord == x && root->y_cord == y){    // перезапись информации

        *rtrnbl = root->info;
        root->info = info_create();

        assert(root->info != NULL);     //!MemError/EOF
    } else if ((x >= root->x_cord && y > root->y_cord) || (x > root->x_cord && y >= root->y_cord)) { //*NE

        root->NE = insert(root->NE, x, y, rtrnbl);
    } else if ((x < root->x_cord && y >= root->y_cord) || (x <= root->x_cord && y > root->y_cord)){   //*NW

        root->NW = insert(root->NW, x, y, rtrnbl);
    } else if ((x < root->x_cord && y <= root->y_cord) || (x <= root->x_cord && y < root->y_cord)){   //*SW

        root->SW = insert(root->SW, x, y, rtrnbl);
    } else if ((x > root->x_cord && y <= root->y_cord) || (x >= root->x_cord && y < root->y_cord)){   //*SE

        root->SE = insert(root->SE, x, y, rtrnbl);
    }   else {
        print_yel("SOMETHING COMPLETELY WRONG IN INSERT!\n");
        assert(0);
    }

    return root;
}

QNode *find_by_key(QNode *node, int x, int y){  // поиск по ключу

    if (NULL == node){
        return NULL;
    }

    if (node->x_cord == x && node->y_cord == y){    // перезапись информации

        return node;

    } else if ((x >= node->x_cord && y > node->y_cord) || (x > node->x_cord && y >= node->y_cord)) { //*NE

        return find_by_key(node->NE, x, y);
    } else if ((x < node->x_cord && y >= node->y_cord) || (x <= node->x_cord && y > node->y_cord)){   //*NW

        return find_by_key(node->NW, x, y);
    } else if ((x < node->x_cord && y <= node->y_cord) || (x <= node->x_cord && y < node->y_cord)){   //*SW

        return find_by_key(node->SW, x, y);
    } else if ((x > node->x_cord && y <= node->y_cord) || (x >= node->x_cord && y < node->y_cord)){   //*SE

        return find_by_key(node->SE, x, y);
    }  else {
        print_yel("SOMETHING COMPLETELY WRONG IN FIND!\n");
        assert(0);
    }
}

void mainloop(){

    enum CODE status;
    QNode *root = NULL, *tmp_node, *found = NULL;
    Info *rtrnbl = NULL;
    int N, command, x_to_add, y_to_add, x_another, y_another;

    // printf("Enter limit of nodes in quadrants: ");   //?Надо ли
    // status = getInt(&N);

    // if (status != success){     //!MemError
    //     print_yel("Error in getting limiter!\n");
    //     return;
    // }

    while (1){
        
        commands_print();
        printf("Choose one: ");
        status = getInt(&command);
        if (status != success){     //!MemError
            print_yel("Error in getting command #!\n");
            return;
        }

        switch (command)
        {
        case 1: // insert
            
            status = getcoords(&x_to_add, &y_to_add);
            if (status != success){
                print_yel("Error in getting coordinates!\n");
                erase_qtree(root);
                return;
            }

            root = insert(root, x_to_add, y_to_add, &rtrnbl);

            if (root == NULL){  // !EOF/MemError
                print_yel("Error in insert!\n");
                erase_qtree(root);
                return;
            }

            if (rtrnbl != NULL){
                print_yel("Some info was rewritten:\n");
                info_print(rtrnbl);
                free(rtrnbl->string);
                free(rtrnbl);
                rtrnbl = NULL;
            }   
            break;

        case 2:
            printf("Current QTree --->\n");
            inorder_print(root);
            printf("\n");

            status = getcoords(&x_to_add, &y_to_add);
            if (status != success){
                print_yel("Error in getting coordinates!\n");
                erase_qtree(root);
                return;
            }
            found = find_by_key(root, x_to_add, y_to_add);
            if (NULL == found){
                print_yel("No such node found!\n");
            } else {
                printf(ANSI_COLOR_RED"Found info: \n"ANSI_COLOR_RESET);
                info_print(found->info);
            }

            found = NULL;
            break;

        case 3:
            print_yel("Enter upper limits --->\n");
            status = getcoords(&x_to_add, &y_to_add);
            if (status != success){
                print_yel("Error in getting upper coordinates!\n");
                erase_qtree(root);
                return;
            }
            print_yel("Enter lower limits --->\n");
            status = getcoords(&x_another, &y_another);
            if (status != success){
                print_yel("Error in getting lower coordinates!\n");
                erase_qtree(root);
                return;
            }

            printf("\n");
            preorder_with_lim(root, x_to_add, x_another, y_to_add, y_another);

            break;

        case 0:
            print_yel("Shutting down...\n");
            erase_qtree(root);
            return;
        
        default:
            print_yel("Wrong command!\n");
            break;
        }

        
    }
    
    print_yel("End of programm!\n");
    return;
}