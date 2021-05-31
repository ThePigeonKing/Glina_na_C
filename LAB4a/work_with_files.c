#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "structs.h"
#include "utils.h"
#include "func.h"

int convert_to_dot(Node *root, FILE *fl, int counter, int index){
    
    ++counter;
    int local_index = counter;
    if (NULL == fl){
        printf(ANSI_COLOR_YELLOW"fl is NULL!\n"ANSI_COLOR_RESET);
        return crit_error;
    }
    // создание ячейки
    fprintf(fl, "\ta%03d [shape=record, label=\"%s\"];\n", counter, root->key);

    // создание связи
    if (index != -1){
        fprintf(fl, "\ta%03d -> a%03d [color=grey];\n", index, counter);
    }

    if (NULL != root->left){
        counter = convert_to_dot(root->left, fl, counter, local_index);
    }
    if (NULL != root->right){
        counter = convert_to_dot(root->right, fl, counter, local_index);
    }

    return counter;    
}

enum CODES tree_print(Node *root){

    if(NULL == root){
        printf(ANSI_COLOR_YELLOW"Can't draw NULL!\n"ANSI_COLOR_RESET);
        return hold;
    }
    FILE *fl;
    char *filename = malloc(40);
    sprintf(filename, "graph.dot");
    if (NULL != filename){
        fl = fopen(filename, "w");
    } else {
        printf(ANSI_COLOR_YELLOW"Filename is something strange..\n"ANSI_COLOR_RESET);
        return crit_error;
    }

    free(filename);
    fprintf(fl, "digraph {\n");
    convert_to_dot(root, fl, 0, -1);
    fprintf(fl, "}");
    fclose(fl);

    return success;
}

enum CODES create_info_file(Info **cell, FILE *file){
    enum CODES stat = hold;

    *cell = (Info *)malloc(sizeof(Info));
    (*cell)->string = (char *)malloc(100 * sizeof(char));

    fscanf(file, "%[^\n]", (*cell)->string);
    fscanf(file, "%*c");
    

    fscanf(file, "%lf%lf", &(*cell)->num1, &(*cell)->num2);
    fscanf(file, "%*c");

    return success;
}

Node* initialize_file(char *key_to_init, FILE *file){
    enum CODES stat = hold;
    
    Node *cell = (Node *)malloc(sizeof(Node));
    if (cell == NULL){  //!MemError
        printf(ANSI_COLOR_YELLOW"!Error in allocating memory during initalization!"ANSI_COLOR_RESET);
        return NULL;
    }

    cell->info = (Info *)malloc(sizeof(Info));
    cell->info->string = (char *)malloc(100 * sizeof(char));
    fscanf(file, "%[^\n]", cell->info->string);
    fscanf(file, "%*c");
    fscanf(file, "%lf%lf", &(cell->info->num1), &(cell->info->num2));
    fscanf(file, "%*c");

    cell->key = (char *)calloc(strlen(key_to_init) + 1, sizeof(char));
    strcat(cell->key, key_to_init);
    cell->left = NULL;
    cell->right = NULL;

    return cell;
}


Node* insert_file(Node *root, char *key, FILE *file){

    int comparison;
    enum CODES stat = hold;

    if (root == NULL){  // когда создаётся дерево
        root = initialize_file(key, file);

        return root;
    }  else {
        Node *root2 = root, *root3 = NULL;

        Node *new_node = (Node *)malloc(sizeof(Node));

        while(root2 != NULL){

            root3 = root2;
            comparison = strcmp(key, root2->key);
            
            if (comparison < 0){
                root2 = root2->left;

            } else if (comparison > 0){
                root2 = root2->right;

            } else if (comparison == 0){    // совпал ключ
                // print_err("Replacing data\n");
                // *returnable = new_node;
                // new_node->key = root2->key;
                // new_node->info = root2->info;

                stat = create_info_file(&(root2->info), file);

                return root;
            }
        }

        new_node->key = (char *)calloc(strlen(key) + 1, sizeof(char));
        strcat(new_node->key, key);
        new_node->left = NULL;
        new_node->right = NULL;
        stat = create_info_file(&(new_node->info), file);

        if (strcmp(key, root3->key) < 0){
            root3->left = new_node;
        } else {
            root3->right = new_node;
        }

        return root;
    }

    printf(ANSI_COLOR_YELLOW"Something wrong in file insert!"ANSI_COLOR_RESET);
    return NULL;
}

char* get_rnd_string(int length) {
	char* rng = malloc((length+1) * sizeof(char));
	char array[60] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
	for (int i = 0; i < length; ++i) {
		rng[i] = array[rand()%52];
	}
	rng[length] = '\0';
	return rng;
}


Node* read_from_file(){  // основная функция для считывания дерева из файла
    printf(ANSI_COLOR_YELLOW"Enter name of file: "ANSI_COLOR_RESET);
    char *filename = getLine(), *someline = (char *)malloc(sizeof(char) * 80);
    Node *new_root = NULL;

    FILE *file;
    

    if (strcmp(filename, "rng") == 0){  // со случайной генерацией
        int rsp, amount, rng_num;
        char *key, *rng_str;
        printf("Enter amount of random generations: ");
        rsp = scanf("%d", &amount);

        if (rsp <= 0){
            printf(ANSI_COLOR_YELLOW"Error in reading amount of generations!\n"ANSI_COLOR_RESET);
            return NULL;
        }

        srand((unsigned)time(NULL));
        file = fopen("rng_test", "w");
        free(filename);
        for (int i = 0; i < amount; ++i){   // 100500 итераций
            fprintf(file, "%s\n", (rng_str = get_rnd_string(8)));
            fprintf(file, "abc info\n");
            fprintf(file, "123 321\n");
            free(rng_str);
        }
        fclose(file);
        
        file = fopen("rng_test", "r");
        while (fscanf(file, "%[^\n]", someline) > 0){
            fscanf(file, "%*c");

            new_root = insert_file(new_root, someline, file);
        }
        printf(ANSI_COLOR_YELLOW"Created tree with %d vertices!\n"ANSI_COLOR_RESET, amount);
        fclose(file);
        free(someline);

        return new_root;
    }

    file = fopen(filename, "r");

    if (NULL == file){
        printf(ANSI_COLOR_YELLOW"Failed to open file!\n\n"ANSI_COLOR_RESET);
        free(someline);
        return NULL;
    }
    while (fscanf(file, "%[^\n]", someline) > 0){
        fscanf(file, "%*c");

        new_root = insert_file(new_root, someline, file);
    }
    printf(ANSI_COLOR_YELLOW"Success!\n"ANSI_COLOR_RESET);
    fclose(file);
    free(someline);
    free(filename);

    return new_root;
}