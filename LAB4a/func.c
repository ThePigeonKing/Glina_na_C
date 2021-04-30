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
    printf(ANSI_COLOR_RED"INFO:\n string: %s\n num1: %.3lf    num2: %.3lf"ANSI_COLOR_RESET, \
        node->info->string, node->info->num1, node->info->num2);

    return;
}

void postorder_print(Node *p, int indent){  // очень кривой показ дерева

    if (p != NULL){
        if (p->right != NULL){
            postorder_print(p->right, indent+4);
        }
        if (indent != 0){
            for (int i = 0; i < indent; ++i)
                printf(" ");
        }
        if (p->right){
            printf(" /\n");
            for (int i = 0; i < indent; ++i)
                printf(" ");
        }
        printf("%s\n", p->key);
        if (p->left){
            for (int i = 0; i < indent+1; ++i)
                printf(" ");
            printf("\\\n");
            postorder_print(p->left, indent+4);
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
    //free(root->key);  //!шо?!
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

//TODO
// Node *find_next_bigger(Node *root, char *key){   // поиск следующего по величине
    
//     Node *found = NULL, *last = NULL;

//     while (root != NULL){

//         if (strcmp(key, root->key) < 0){
//             last = root;
//             root = root->left;
//         }

//     }
// }

//TODO
// Node *find_for_rev_order(Node *root, char *key){    // для обратного порядка ключей

// }

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

Node* del_by_key(Node *root, char *key_to_del){    // удаление ячейки по ключу

    if (root == NULL){
        print_err("Err\n");
        return root;
    }

    //*левое поддерево
    if (strcmp(key_to_del, root->key) < 0){
        root->left = del_by_key(root->left, key_to_del);
    }

    //*правое поддерево
    else if (strcmp(key_to_del, root->key) > 0){
        root->right = del_by_key(root->right, key_to_del);
    } else {
        //*когда только 1 или нет подуровней
        if (NULL == root->left){
            Node *temp = root->right;
            free(root->info->string);
            free(root->info);
            free(root);
            return temp;
        }
        else if (NULL == root->right){
            Node *temp = root->left;
            free(root->info->string);
            free(root->info);
            free(root);
            return temp;
        }

        //*минимальное в правом поддереве
        Node *temp = minValue(root->right);
        root->key = temp->key;
        root->info = temp->info;

        root->right = del_by_key(root->right, temp->key);
    }
    return root;
}

void print_commands(){
    printf(ANSI_COLOR_GREEN"1. Add node\n"ANSI_COLOR_RESET);
    printf(ANSI_COLOR_GREEN"2. Find by key\n"ANSI_COLOR_RESET);
    printf(ANSI_COLOR_GREEN"3. Find any bigger\n"ANSI_COLOR_RESET);
    printf(ANSI_COLOR_GREEN"4. Delete\n"ANSI_COLOR_RESET);
    //printf(ANSI_COLOR_GREEN"4. Find (Task)\n"ANSI_COLOR_RESET);
    printf(ANSI_COLOR_GREEN"5. Read from file\n"ANSI_COLOR_RESET);
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
        printf("Choose one -->");
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

            print_err("Currently doesn't work\n");
            
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