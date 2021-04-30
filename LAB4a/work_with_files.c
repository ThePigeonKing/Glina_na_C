#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "structs.h"
#include "utils.h"
#include "func.h"



enum CODES create_info_file(Info **cell, FILE *file){
    enum CODES stat = hold;

    *cell = (Info *)malloc(sizeof(Info));

    fscanf(file, "%[^\n]", (*cell)->string);
    fscanf(file, "%*c");
    

    fscanf(file, "%lf%lf", &(*cell)->num1, &(*cell)->num2);

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

    cell->key = key_to_init;
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

        new_node->key = key;
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



Node* read_from_file(){  // основная функция для считывания дерева из файла
    printf(ANSI_COLOR_YELLOW"Enter name of file: "ANSI_COLOR_RESET);
    char *filename = getLine(), *someline;
    Node *new_root = NULL;

    FILE *file;
    file = fopen(filename, "r");

    if (NULL == file){
        printf(ANSI_COLOR_YELLOW"Failed to open file!\n\n"ANSI_COLOR_RESET);
        return NULL;
    }
    while (fscanf(file, "%[^\n]", someline) > 0){
        fscanf(file, "%*c");

        new_root = insert_file(new_root, someline, file);
    }
    fclose(file);

    return new_root;
}