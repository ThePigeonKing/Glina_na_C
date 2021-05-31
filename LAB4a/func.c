#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "structs.h"
#include "utils.h"
#include "func.h"
#include "work_with_files.h"


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

enum CODES getDouble(double *num){

    double res;
    int rsp;

    while (1){
        rsp = scanf("%lf", &res);
        if (rsp > 0){
            scanf("%*[^\n]");
            *num = res;

            return success;
        } else if (rsp == 0){
            scanf("%*[^\n]");
            printf("Wrong double, try again..\n");
        } else if (rsp < 0){
            printf(ANSI_COLOR_YELLOW"EOF found!"ANSI_COLOR_RESET);
            
            return crit_error;
        }
    }

    return fail;
}

void print_err(char *msg){
    printf(ANSI_COLOR_YELLOW"%s\n"ANSI_COLOR_RESET, msg);
}

enum CODES getKey(char **arr){
    
    *arr = getLine();

    if (NULL == *arr){
        print_err("Error in getKey!\n");
        return crit_error;
    }

    return success;
}

void print_node(Node *node, char* msg){    // вывод на экран node
    printf("%s", msg);
    printf(ANSI_COLOR_RED"INFO for key \"%s\"\n string: \"%s\"      num1: %.3lf     num2: %.3lf"ANSI_COLOR_RESET, \
        node->key, node->info->string, node->info->num1, node->info->num2);

    return;
}

void postorder_print(Node *p, char *lim){  // вывод в обратном обходе + ограничение
    if (NULL != p){
        postorder_print(p->left, lim);
        postorder_print(p->right, lim); //!ENA
        int compar = strcmp(lim, p->key);
        if (compar > 0){
            printf(ANSI_COLOR_RED"%s "ANSI_COLOR_RESET, p->key);
        }
    }
    
}

void directorder_print(Node *p){
    if (p == NULL)
        return;

    directorder_print(p->left);
    printf(ANSI_COLOR_GREEN"%s "ANSI_COLOR_RESET, p->key);
    directorder_print(p->right);
}

void erase_tree(Node *root){  // очищение всего дерева

    if (root == NULL){
        return;
    }
    if (root->left != NULL){
        erase_tree(root->left);
    }
    if (root->right != NULL){
        erase_tree(root->right);
    }
    free(root->info->string);
    free(root->info);
    free(root->key);  //!шо?! дада я даун
    free(root);

}

Node *find_by_key(Node *root, char *key){   // поиск по ключу
    if (root == NULL || strcmp(root->key, key) == 0){
        return root;    // найдено или такого нет
    }
    if (strcmp(key, root->key) < 0)
        return find_by_key(root->left, key);
    else
        return find_by_key(root->right, key);
}

Node *find_first_bigger(Node *root, char *key, Node* ptr){   // поиск следующего по величине
    
    if (NULL == root){
        return NULL;
    }
    if (strcmp(key, root->key) < 0){

        ptr = root;
        if (NULL == root->left){
            return ptr;
        }
        return find_first_bigger(root->left, key, ptr);
    } else if (strcmp(key, root->key) > 0){
        if (NULL == root->right){
            return ptr;
        }
        return find_first_bigger(root->right, key, ptr);
    } else if (strcmp(key, root->key) == 0){

        if (root->right != NULL){
            root = root->right;
            while(root->left != NULL){
                root = root->left;
            }
            return root;
        } else {

            return ptr;
        }
    }

    //print_err("Something went wrong in find_first_bigger\n");
    return NULL;
}


Node* initialize(char *key_to_init){
    enum CODES stat = hold;
    
    Node *cell = (Node *)malloc(sizeof(Node));
    if (cell == NULL){
        print_err("!Error in allocating memory during initalization!");
        return NULL;
    }

    stat = create_info(&(cell->info));
    if (stat == crit_error){    //!MemError
        free(cell);

        return NULL;
    }

    cell->key = key_to_init;
    cell->left = NULL;
    cell->right = NULL;

    return cell;
}

enum CODES create_info(Info **cell){
    enum CODES stat = hold;

    *cell = (Info *)malloc(sizeof(Info));

    printf("Enter string for info: ");
    (*cell)->string = getLine();
    if ((*cell) -> string == NULL){    // !MemError
        print_err("Error in allocating memory for cell->string!");

        return crit_error;
    }
    printf("Enter NUM1 for info: ");
    stat = getDouble(&((*cell)->num1));
    if (stat == crit_error || stat == fail){
        print_err("Error in num1!");
        free((*cell)->string);
        return crit_error;
    }

    printf("Enter NUM2 for info: ");
    stat = getDouble(&((*cell)->num2));
    if (stat == crit_error || stat == fail){
        print_err("Error in num2!");
        free((*cell)->string);
        return crit_error;
    }

    //scanf("%*c");
    return success;
}

Node* insert(Node *root, char *key, Node **returnable){
    int comparison;
    enum CODES stat = hold;

    if (root == NULL){  // когда создаётся дерево
        root = initialize(key);

        return root;
    }  else {
        Node *root2 = root, *root3 = NULL;

        Node *new_node = (Node *)malloc(sizeof(Node));

        if (new_node == NULL){
            print_err("Error in allocating memory for new_node");
            return NULL;
        }

        while(root2 != NULL){

            root3 = root2;
            comparison = strcmp(key, root2->key);
            
            if (comparison < 0){
                root2 = root2->left;

            } else if (comparison > 0){
                root2 = root2->right;

            } else if (comparison == 0){    // совпал ключ
                print_err("Replacing data\n");
                *returnable = new_node;
                new_node->key = root2->key;
                new_node->info = root2->info;

                stat = create_info(&(root2->info));

                if (stat == crit_error){
                    //DELETE
                    return NULL;
                }

                return root;
            }
        }

        new_node->key = key;
        new_node->left = NULL;
        new_node->right = NULL;
        stat = create_info(&(new_node->info));

        if (stat == crit_error){    //!MemError
            erase_tree(root);

            return NULL;
        }

        if (strcmp(key, root3->key) < 0){
            root3->left = new_node;
        } else {
            root3->right = new_node;
        }

        return root;
    }

    print_err("Something wrong in insert!");
    return NULL;
}

Node *minValue(Node* root){     // поиск минимального значения

    Node *current = root;
    while(current != NULL && current->left != NULL)
        current = current->left;

    return current;
}

void erase_node(Node *root, int indicator){
    if (indicator == 1){
        free(root->key);
    }
    free(root->info->string);
    free(root->info);
    free(root);
}

Node* del_by_key(Node *root, char *key_to_del){    // удаление ячейки по ключу

    Node *tmp = NULL;
    Info *inf_buf = NULL;
    static int depth = 0;

    if (root == NULL){
        print_err("Err\n");
        return root;
    }

    if (strcmp(key_to_del, root->key) < 0){ // влево

        root->left = del_by_key(root->left, key_to_del);
    } else if (strcmp(key_to_del, root->key) > 0){  // вправо

        root->right = del_by_key(root->right, key_to_del);
    } else if (NULL != root->left && NULL != root->right){
        tmp = minValue(root->right);
        free(root->key);
        root->key = tmp->key;
        inf_buf = root->info;
        root->info = tmp->info;
        tmp->info = inf_buf;
        ++depth;
        root->right = del_by_key(root->right, root->key);
    } else {

        if (NULL != root->left){
            tmp = root;
            root = root->left;
            if (depth == 0)
                erase_node(tmp, 1);
            else
                erase_node(tmp, 0);
        } else if (NULL != root->right){
            tmp = root;
            root = root->right;
            if (depth == 0)
                erase_node(tmp, 1);
            else
                erase_node(tmp, 0);
        } else {
            if (depth == 0)
                erase_node(root, 1);
            else
                erase_node(root, 0);
            root = NULL;
        }
    }
    print_debug("%s", "FINALE IN DEL!\n");
    return root;
}

void print_commands(){
    printf(ANSI_COLOR_GREEN"1. Add node\n"ANSI_COLOR_RESET);
    printf(ANSI_COLOR_GREEN"2. Find by key\n"ANSI_COLOR_RESET);
    printf(ANSI_COLOR_GREEN"3. Find first bigger\n"ANSI_COLOR_RESET);
    printf(ANSI_COLOR_GREEN"4. Delete\n"ANSI_COLOR_RESET);
    printf(ANSI_COLOR_GREEN"5. Read from file\n"ANSI_COLOR_RESET);
    printf(ANSI_COLOR_GREEN"6. Postorder print with upper lim\n"ANSI_COLOR_RESET);
    printf(ANSI_COLOR_GREEN"777. Save as graphics \n"ANSI_COLOR_RESET);
    printf(ANSI_COLOR_GREEN"0. Shut down\n"ANSI_COLOR_RESET);

    printf("\n");
}

void mainloop(){
    int command, rsp;
    char *key;
    enum CODES stat;
    Node *root = NULL, *rtrnbl = (Node *)calloc(1, sizeof(Node)), *found = NULL;

    while (1){
        print_commands();
        printf("Choose one --> ");
        rsp = scanf("%d", &command);
        if (rsp < 0){
            printf("EOF found!\n");
            free(rtrnbl);
            erase_tree(root);
            return;
        } else if (rsp == 0){
            print_err("Not number!\n");
            scanf("%*[\n]");
            scanf("%*c");
            continue;
        }

        switch (command)
        {
        case 1: // insert
            printf("Enter key to create a node: ");
            stat = getKey(&key);
            if (stat == crit_error){    //!MemError/EOF
                free(rtrnbl);
                erase_tree(root);
                return;
            }
            root = insert(root, key, &rtrnbl);
            if (rtrnbl->info != NULL){
                print_err("Some data has been replaced");
                print_node(rtrnbl, "Replaced data--->\n");
                printf("\n");

                free(rtrnbl->info->string);
                free(rtrnbl->info);
                rtrnbl->info = NULL;
                
                free(key);
                
            }
            break;
        
        case 2:     // find
            if (NULL == root){
                print_err("Can't find anything in empty tree!");
                break;
            }
            printf("Current tree--->\n");
            directorder_print(root);
            printf("\n");
            printf("Enter key to watch for: ");
            stat = getKey(&key);
            if (stat == crit_error){    //!MemError/EOF
                free(rtrnbl);
                erase_tree(root);
                return;
            }
            found = find_by_key(root, key);
            if (found != NULL)
                print_node(found, "Found item--->\n");
            else
                print_err("Element with key not found!");
            printf("\n");
            free(key);
            break;

        case 3: // поиск следующего по величине
            printf("Enter key find bigger for: ");
            stat = getKey(&key);
            if (stat == crit_error){    //!MemError/EOF
                free(key);
                erase_tree(root);
                return;
            }
            found = find_first_bigger(root, key, NULL);
            if (found != NULL){
                print_node(found, "Next bigger ->\n");
            } else {
                print_err("Element not found!\n");
            }

            free(key);
            break;
        
        case 4:     // del_by_key
            printf("Current tree--->\n");
            directorder_print(root);
            printf("\n");
            printf("Enter key to delete: ");
            stat = getKey(&key);
            if (stat == crit_error){    //!MemError/EOF
                erase_tree(root);
                root = NULL;
                return;
            }
            root = del_by_key(root, key);
            print_err("New tree--->");
            directorder_print(root);
            printf("\n");
            free(key);
            break;

        case 5: // чтение из файла

            erase_tree(root);
            
            root = read_from_file();
            break;

        case 6:
            printf("Enter upper limit for printing --> ");
            stat = getKey(&key);
            if (stat == crit_error){    //!MemError/EOF
                erase_tree(root);
                root = NULL;
                return;
            }
            postorder_print(root, key);
            free(key);
            break;

        case 777:   // save to .dot
            stat = tree_print(root);
            if (stat == success){
                print_err("Saved to graph.dot file!");
                break;
            } else {
                print_err("Failed to save!");
                break;
            }

        case 0:
            print_err("Shutting down...");
            erase_tree(root);
            free(rtrnbl);
            root = NULL;
            return;

        default:
            print_err("Wrong command!");
            scanf("%*[\n]");
            break;
        }

        printf("\n");
    }

    return;
}