#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "structs.h"
#include "utils.h"
#include "funcs.h"
#include "filework.h"

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

void info_print(Node *node){
    printf("NODE (%d, %d) with info \"%s\"", node->x, node->y, node->info);
}

void qtree_erase(QNode **root){
    if (NULL == (*root)){
        return;
    }

    if ((*root)->NE != NULL){
        qtree_erase(&((*root)->NE));
    } 
    if ((*root)->NW != NULL){
        qtree_erase(&((*root)->NW));
    } 
    if ((*root)->SE != NULL){
        qtree_erase(&((*root)->SE));
    } 
    if ((*root)->SW != NULL){
        qtree_erase(&((*root)->SW));
    } 
    for (int i = 0; i < (*root)->exists; ++i){
        free((*root)->array[i]->info);
        free((*root)->array[i]);
    }
    free((*root)->array);
    free((*root));
}

//-------------------------
void swap(int *xp, int *yp)
{
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}

void bubbleSort(int arr[], int n)
{
   int i, j;
   for (i = 0; i < n-1; i++) {      
       for (j = 0; j < n-i-1; j++){
           if (arr[j] > arr[j+1])
              swap(&arr[j], &arr[j+1]);
       }
   }
}

void calc_median(Node **arr, int *x_new, int *y_new){
    int x_cords[N+1], y_cords[N+1];
    for (int i = 0; i < N+1; ++i){
        x_cords[i] = arr[i]->x;
        y_cords[i] = arr[i]->y;
    }
    bubbleSort(x_cords, N+1);
    bubbleSort(y_cords, N+1);

    *x_new = x_cords[N/2];
    *y_new = y_cords[N/2];
    return;
}
//------------------------

void append_info(Node **arr, int x, int y, char* new_info, int len){

    arr[len] = crtnew(Node);
    if (NULL == arr[len]){  //!Mem
        print_yel("Error in allocating memory for new info!\n");
        free(new_info);
        exit(1);
    }

    arr[len]->info = (char *)calloc(strlen(new_info) + 1, sizeof(char));
    if (NULL == arr[len]->info){
        print_yel("Error in allocating info!\n");
        exit(1);
    }
    strcat(arr[len]->info, new_info);
    arr[len]->x = x;
    arr[len]->y = y;
    
}

CODE subdivide(QNode **root, int x_med, int y_med){
    int cur_x = -1, cur_y = -1;

    (*root)->x_cord = x_med;
    (*root)->y_cord = y_med;

    (*root)->NE = crtnew(QNode);
    if (NULL == (*root)->NE){
        print_yel("Error in allocating NE!\n");
        exit(1);
    }    
    (*root)->NW = crtnew(QNode);
    if (NULL == (*root)->NW){
        print_yel("Error in allocating NW!\n");
        exit(1);
    }
    (*root)->SE = crtnew(QNode);
    if (NULL == (*root)->SE){
        print_yel("Error in allocating SE!\n");
        exit(1);
    }
    (*root)->SW = crtnew(QNode);
    if (NULL == (*root)->SW){
        print_yel("Error in allocating SW!\n");
        exit(1);
    }
    (*root)->NE->array = calloc(N+1, sizeof(Node*));
    (*root)->NW->array = calloc(N+1, sizeof(Node*));
    (*root)->SE->array = calloc(N+1, sizeof(Node*));
    (*root)->SW->array = calloc(N+1, sizeof(Node*));
    if ((*root)->SW->array == NULL || (*root)->SE->array == NULL || (*root)->NW->array \
        == NULL || (*root)->NE->array == NULL){
            print_yel("Error in allocatin some of array in subdivide!\n");
            exit(1);
    } 

    for (int i = 0; i < N+1; ++i){
        cur_y = (*root)->array[i]->y;
        cur_x = (*root)->array[i]->x;   
        if (cur_x >= x_med && cur_y >= y_med){          //NE
            append_info((*root)->NE->array, cur_x, cur_y, (*root)->array[i]->info, (*root)->NE->exists);
            (*root)->NE->exists++;
            (*root)->NE->prev = *root;
        } else if (cur_x <= x_med && cur_y >= y_med) {  //NW
            append_info((*root)->NW->array, cur_x, cur_y, (*root)->array[i]->info, (*root)->NW->exists);
            (*root)->NW->exists++;
            (*root)->NW->prev = *root;
        } else if (cur_x <= x_med && cur_y <= y_med) {  //SW
            append_info((*root)->SW->array, cur_x, cur_y, (*root)->array[i]->info, (*root)->SW->exists);
            (*root)->SW->exists++;
            (*root)->SW->prev = *root;
        } else if (cur_x >= x_med && cur_y <= y_med) {  //SE
            append_info((*root)->SE->array, cur_x, cur_y, (*root)->array[i]->info, (*root)->SE->exists);
            (*root)->SE->exists++;
            (*root)->SE->prev = *root;
        } else {
            //print_yel("Something went completely wrong in subdivide!\n");
            return crit_err;
        }
    }
    (*root)->subdivided = 1;
    (*root)->exists = 0;
    for (int i = 0; i < N+1; ++i){
        free((*root)->array[i]->info);
        free((*root)->array[i]);
    }
    free((*root)->array);
    (*root)->array = NULL;

    return success;
}

int find_info(Node **root, int xtofind, int ytofind, int len){  // поиск есть ли такая точка
    for (int i = 0; i < len; ++i){
        if (root[i]->x == xtofind && root[i]->y == ytofind){
            return i;
        }
    }
    return -1;    
}

QNode *insert(QNode *root, int x, int y, char *new_info, Node **rtrnbl){
    CODE stat = hold;
    int index, x_med, y_med;

    if (NULL == root){
        root = crtnew(QNode);
        root->x_cord = 0;
        root->y_cord = 0;
        if (NULL == root){
            print_yel("Error in allocating NULL root!\n");
            exit(1);
        }
        root->array = calloc(N+1, sizeof(Node*));
        if (NULL == root->array){
            print_yel("Error in allocating NULL array!\n");
            exit(1);
        }
        append_info(root->array, x, y, new_info, root->exists);
        root->exists++;

        return root;
    }
    if (root->subdivided == 0){ // пространство не разделено
        if (root->exists == N){ // уже заполнено
            if ((index = find_info(root->array, x, y, root->exists)) == -1){    // не дублируются
                append_info(root->array, x, y, new_info, root->exists);
                
                root->exists += 1;
                calc_median(root->array, &x_med, &y_med);
                stat = subdivide(&root, x_med, y_med);
                if (stat == crit_err){
                    qtree_erase(&root);
                    return NULL;
                }

                return root;
            } else {    // повторение информации
                (*rtrnbl) = crtnew(Node);
                (*rtrnbl)->info = root->array[index]->info;
                (*rtrnbl)->x = root->array[index]->x;
                (*rtrnbl)->y = root->array[index]->y;
                root->array[index]->info = new_info;
                return root;
            }
        } else {    // не переполнено
            if ((index = find_info(root->array, x, y, root->exists)) == -1){
                append_info(root->array, x, y, new_info, root->exists);
                
                root->exists++;
                return root;
            } else {
                (*rtrnbl) = crtnew(Node);
                (*rtrnbl)->info = root->array[index]->info;
                (*rtrnbl)->x = root->array[index]->x;
                (*rtrnbl)->y = root->array[index]->y;
                root->array[index]->info = new_info;
                return root;
            }
        }
    } else {    // subd = 1
        if (x >= root->x_cord && y >= root->y_cord){            // NE
            root->NE = insert(root->NE, x, y, new_info, rtrnbl);
            return root;
        } else if (x <= root->x_cord && y >= root->y_cord){     // NW
            root->NW = insert(root->NW, x, y, new_info, rtrnbl);
            return root;
        } else if (x <= root->x_cord && y <= root->y_cord){     // SW
            root->SW = insert(root->SW, x, y, new_info, rtrnbl);
            return root;
        } else if (x >= root->x_cord && y <= root->y_cord){     // SE
            root->SE = insert(root->SE, x, y, new_info, rtrnbl);
            return root;
        } 
    }

    printf("Something went completely wrong in insert!\n");
    return NULL;
}

Node *find_by_key(QNode *root, int x, int y){
    QNode *root1 = root;
    if (NULL == root){
        return NULL;
    }

    while(root1->subdivided != 0){
        if (x >= root1->x_cord && y >= root1->y_cord){            // NE
            root1 = root1->NE;
        } else if (x <= root1->x_cord && y >= root1->y_cord){     // NW
            root1 = root1->NW;
        } else if (x <= root1->x_cord && y <= root1->y_cord){     // SW
            root1 = root1->SW;
        } else if (x >= root1->x_cord && y <= root1->y_cord){     // SE
            root1 = root1->SE;
        } 
    }
    if (NULL == root1){
        return NULL;
    }
    int index = find_info(root1->array, x, y, root1->exists);
    if (index == -1){
        return NULL;
    } else {
        return root1->array[index];
    }
}

void lnr_coords(QNode *root){
    if (NULL == root){
        return;
    }
    if (NULL != root->SW){
        lnr_coords(root->SW);
    }
    if (NULL != root->SE){
        lnr_coords(root->SE);
    }
    if (NULL != root->NW){
        lnr_coords(root->NW);
    }
    if (NULL != root->NE){
        lnr_coords(root->NE);
    }
    printf("{");
    for (int i = 0; i < root->exists; ++i){
        printf(ANSI_COLOR_RED"(%d, %d)"ANSI_COLOR_RESET, root->array[i]->x,
             root->array[i]->y);
        if (i != root->exists-1){
            printf(ANSI_COLOR_RED", "ANSI_COLOR_RESET);
        }
    }
    printf("}--->");
    return;
}

void lnr_info(QNode *root){
    if (NULL == root){
        return;
    }
    if (NULL != root->SW){
        lnr_info(root->SW);
    }
    if (NULL != root->SE){
        lnr_info(root->SE);
    }
    if (NULL != root->NW){
        lnr_info(root->NW);
    }
    if (NULL != root->NE){
        lnr_info(root->NE);
    }
    printf("{");
    for (int i = 0; i < root->exists; ++i){
        printf(ANSI_COLOR_RED"(%s)"ANSI_COLOR_RESET, root->array[i]->info);
        if (i != root->exists-1){
            printf(ANSI_COLOR_RED", "ANSI_COLOR_RESET);
        }
    }
    printf("}->");
    return;
}

// поиск с пределами по ключам
void nlr_with_limits(QNode *root, int low_x, int low_y, int up_x, int up_y){
    if (NULL == root){
        return;
    }
    if (root->SW != NULL){
        nlr_with_limits(root->SW, low_x, low_y, up_x, up_y);
    }
    if (root->SE != NULL){
        nlr_with_limits(root->SE, low_x, low_y, up_x, up_y);
    }
    if (root->NW != NULL){
        nlr_with_limits(root->NW, low_x, low_y, up_x, up_y);
    }
    if (root->NE != NULL){
        nlr_with_limits(root->NE, low_x, low_y, up_x, up_y);
    }

    if (root->subdivided == 0){
        int x, y;
        for (int i = 0; i < root->exists; ++i){
            x = root->array[i]->x;
            y = root->array[i]->y;

            if (x > low_x && x < up_x && y > low_y && y < up_y){
                printf("(%d, %d) ", x, y);
            }
        }
        printf("\n");
        return;
    } else {
        return;
    }
}

// удаление по ключу
CODE del_by_key(QNode *root, int x, int y){
    if (NULL == root){
        return fail;
    }

    while (root->subdivided != 0){
        if (x >= root->x_cord && y >= root->y_cord){            // NE
            root = root->NE;
        } else if (x <= root->x_cord && y >= root->y_cord){     // NW
            root = root->NW;
        } else if (x <= root->x_cord && y <= root->y_cord){     // SW
            root = root->SW;
        } else if (x >= root->x_cord && y <= root->y_cord){     // SE
            root = root->SE;
        } 
    }
    if (NULL == root){
        return fail;
    }

    int index = find_info(root->array, x, y, root->exists);
    if (index == -1){
        return fail;
    } else {
        if (index == (root->exists - 1)){ // last element of vector deleted
            free(root->array[index]->info);
            free(root->array[index]);
            root->array[index] = NULL;
        } else {    // NOT last elem
            free(root->array[index]->info);
            free(root->array[index]);
            root->array[index] = root->array[root->exists-1];
            root->array[root->exists] = NULL;
        }
        root->exists--;
        return success;
    }

    return crit_err;
}

//TODO поиск наиболее отличающегося
Node *find_distinguished(QNode *root, int x, int y, int *dist){
    // чтобы не парится с точностью, храним квадраты расстояний
    if (NULL == root){
        *dist = -1;
        return NULL;
    }

    Node *nw, *ne, *sw, *se, *local;
    int d_ne = -1, d_nw = -1, d_sw = -1, d_se = -1; 
    int distance = -1;

    if (root->NE != NULL){
        ne = find_distinguished(root->NE, x, y, &d_ne);
    }
    if (root->NW != NULL){
        nw = find_distinguished(root->NW, x, y, &d_nw);
    }
    if (root->SW != NULL){
        sw = find_distinguished(root->SW, x, y, &d_sw);
    }
    if (root->SE != NULL){
        se = find_distinguished(root->SE, x, y, &d_se);
    }

    if (root->subdivided == 0){
        int x_some, y_some;
        for (int i = 0; i < root->exists; ++i){
            x_some = root->array[i]->x - x;
            y_some = root->array[i]->y - y;
            if ((x_some*x_some + y_some*y_some) > distance){
                distance = (x_some*x_some + y_some*y_some);
                local = root->array[i];
            }
        }
    }

    if (distance >= d_sw && distance >= d_se && distance >= d_nw && distance >= d_ne){
        distance = distance;
        local = local;
    } else if (d_ne >= distance && d_ne >= d_nw && d_ne >= d_sw && d_ne >= d_se){
        distance = d_ne;
        local = ne;
    } else if (d_nw >= distance && d_nw >= d_ne && d_nw >= d_sw && d_ne >= d_se){
        distance = d_nw;
        local = nw;
    } else if (d_sw >= distance && d_sw >= d_se && d_sw >= d_nw && d_sw >= d_se){
        distance = d_sw;
        local = sw;
    } else if (d_se >= distance && d_se >= d_ne && d_se >= d_nw && d_ne >= d_sw){
        distance = d_se;
        local = se;
    } else {
        print_yel("SOMETHING WENT COMPLETELY WRONG IN FIND_BIGGER!\n");
        exit(1);
    }

    *dist = distance;
    return local;
}

void commands_print(){
    printf("\n");
    printf(ANSI_COLOR_RED"1. Insert new node\n");
    printf("2. Find by coordinates\n");
    printf("3. Print with limits\n");
    printf("4. Delete by coordinates\n");
    printf("5. Prinf all tree\n");
    printf("6. Find the most distant\n");

    printf("7. Save current tree\n");
    printf("8. Convert to dot file\n");
    printf("9. Read from file\n");

    printf("0. Exit\n"ANSI_COLOR_RESET);
    printf("\n");
}

void mainloop(){

    enum CODE status;
    QNode *root = NULL, *tmp_node, *found = NULL;
    Node *rtrnbl = NULL;
    int command, x_to_add, y_to_add, x_another, y_another;
    char *new_info, *filename;    

    while (1){
        
        commands_print();
        printf("Choose one: ");
        status = getInt(&command);
        if (status != success){     //!Mem
            print_yel("Error in getting command #!\n");
            qtree_erase(&root);
            return;
        }

        switch (command)
        {
        case 1: // insert
            
            status = getcoords(&x_to_add, &y_to_add);
            if (status != success){
                print_yel("Error in getting coordinates!\n");
                qtree_erase(&root);
                return;
            }
            printf("Enter new info: ");
            new_info = getLine();
            if (NULL == new_info){  // !EOF/Mem
                print_yel("Error in getting new info!\n");
                qtree_erase(&root);
                return;
            }

            root = insert(root, x_to_add, y_to_add, new_info, &rtrnbl);

            free(new_info);
            if (rtrnbl != NULL){
                print_yel("Some data has been rewritten! 🠗🠗🠗\n");
                printf("For (%d, %d) info was \"%s\"\n", rtrnbl->x, rtrnbl->y, rtrnbl->info);

                free(rtrnbl->info);
                free(rtrnbl);
                rtrnbl = NULL;
            }
            
            break;

        case 2:
            printf("Current QTree --->\n");
            
            printf("\n");

            status = getcoords(&x_to_add, &y_to_add);
            if (status != success){
                print_yel("Error in getting coordinates!\n");
                //erase_qtree(root);
                return;
            }
            rtrnbl = find_by_key(root, x_to_add, y_to_add);
            if (NULL == rtrnbl){
                print_yel("Not found!\n");
            } else {
                print_yel("Found!\n");
                info_print(rtrnbl);
                rtrnbl = NULL;
            }
            
            break;

        //nlr_with_limits
        case 3:
            print_yel("Enter LOWER limits --->\n");
            status = getcoords(&x_to_add, &y_to_add);
            if (status != success){
                print_yel("Error in getting upper coordinates!\n");
                qtree_erase(&root);
                return;
            }
            print_yel("Enter UPPER limits --->\n");
            status = getcoords(&x_another, &y_another);
            if (status != success){
                print_yel("Error in getting lower coordinates!\n");
                qtree_erase(&root);
                return;
            }
            printf(ANSI_COLOR_RESET"\n\n");
            nlr_with_limits(root, x_to_add, y_to_add, x_another, y_another);
            printf(ANSI_COLOR_RESET"");
            break;

        case 4: // del_by_key
            printf(ANSI_COLOR_RED"Enter coordinates to be deleted\n"ANSI_COLOR_RESET);
            status = getcoords(&x_to_add, &y_to_add);
            if (status != success){
                print_yel("Error in getting coordinates!\n");
                qtree_erase(&root);
                return;
            }
            status = del_by_key(root, x_to_add, y_to_add);
            if (status == fail){
                print_yel("Failed!\n");
            } else {
                print_yel("Success!\n");
            }

            break;

        case 5:
            printf("KEYS:\n");
            lnr_coords(root);
            printf("\n");
            printf("INFOS:\n");
            lnr_info(root);
            
            break;

        //most distingiushed
        case 6:
            printf("Enter coords to find most distand from -->\n");
            status = getcoords(&x_to_add, &y_to_add);
            if (status != success){
                print_yel("Error in getting coordinates!\n");
                qtree_erase(&root);
                return;
            }
            rtrnbl = find_distinguished(root, x_to_add, y_to_add, &command);
            if (NULL == rtrnbl){
                print_yel("Error!\n");
            } else {
                printf(ANSI_COLOR_RED"\nMost distand node is (%d, %d) with info \"%s\"\n(distance ~ %.3lf)\n"ANSI_COLOR_RESET, \
                rtrnbl->x, rtrnbl->y, rtrnbl->info, sqrt((double)command));
            }
            rtrnbl = NULL;
            break;

        // save tree
        case 7:
            printf("Enter the name of the file to save to --> ");
            status = save_to_file(root, getLine());
            if (status == crit_err){
                print_yel("Error in getting filename!\n");
                exit(1);
            } else if (status == fail){
                print_yel("Failed to save!\n");
            } else if (status == success){
                print_yel("Success!\n");
            }

            break;

        case 8:
            status = tree_print(root);
            if (status == fail){
                print_yel("Error in converting to .dot!\n");
            } else if (status == crit_err){
                print_yel("Error in creating file!\n");
            } else if (status == success){
                print_yel("Successfully saved!\n");
            }
            break;

        case 9:
            printf("Enter name of file with tree ---> ");
            filename = getLine();
            if (NULL == filename){
                printf("Error in getting filename!\n");
                qtree_erase(&root);
                return;
            }

            qtree_erase(&root);
            root = NULL;
            root = read_from_file(&root, filename);
            if (NULL == root){
                print_yel("Error in reading file!\n");
            }

            break;

        case 0:
            print_yel("Shutting down...\n");
            qtree_erase(&root);
            return;
        
        default:
            print_yel("Wrong command!\n");
            break;
        }

        printf("\n\n");        
    }
    
    print_yel("End of programm!\n");
    return;
}