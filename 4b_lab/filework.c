#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "structs.h"
#include "utils.h"
#include "funcs.h"
#include "filework.h"


QNode *read_from_file(QNode **old_root, char *filename){
    if ((*old_root) != NULL){
        free((*old_root));
    }

    int x, y;
    char *info = malloc(100 * sizeof(char));
    QNode *new_root = NULL;
    Node *something = NULL;
    FILE *f;
    f = fopen(filename, "r");
    if (NULL == f){
        free(filename);
        free(info);
        return NULL;
    }

    while (fscanf(f, "%d", &x) > 0){
        fscanf(f, "%d", &y);
        fscanf(f, "%*c");
        fscanf(f, "%[^\n]", info);
        fscanf(f, "%*c");

        new_root = insert(new_root, x, y, info, &something);
        if (NULL == new_root){
            printf(ANSI_COLOR_YELLOW"CRIT ERROR!\n"ANSI_COLOR_RESET);
            return NULL;
        }
        if (something != NULL){
            printf("Rewriting data!\n");
            free(something);
            something = NULL;
        }
    }
    printf(ANSI_COLOR_YELLOW"Success!\n"ANSI_COLOR_RESET);
    
    fclose(f);
    free(filename);
    free(info);
    return new_root;
}

CODE convert_to_dot(QNode *root, FILE *f, int counter, int index){

    ++counter;
    int local_index = counter;
    if (NULL == f){
        printf("FILE is NULL!\n");
        return crit_err;
    }

    fprintf(f, "\ta%03d [shape=record, label=\"CORDS:(%d, %d) NUM: %d\"];\n", counter,\
         root->x_cord, root->y_cord, root->exists);

    if (index != -1){
        fprintf(f, "\ta%03d -> a%03d [color=crimson];\n", index, counter);
    }

    if (NULL != root->SW){
        counter = convert_to_dot(root->SW, f, counter, local_index);
    }
    if (NULL != root->SE){
        counter = convert_to_dot(root->SE, f, counter, local_index);
    }
    if (NULL != root->NW){
        counter = convert_to_dot(root->NW, f, counter, local_index);
    }
    if (NULL != root->NE){
        counter = convert_to_dot(root->NE, f, counter, local_index);
    }

    return counter;
}

CODE tree_print(QNode *root){
    
    if (NULL == root){
        return fail;
    }

    CODE stat = hold;
    FILE *f;
    char *filename = malloc(40);
    sprintf(filename, "graph.dot");
    if (NULL != filename){
        f = fopen(filename, "w");
    } else {
        return crit_err;
    }

    free(filename);
    fprintf(f, "digraph {\n");
    stat = convert_to_dot(root, f, 0, -1);
    fprintf(f, "}");
    fclose(f);

    return success;
}

void write_to_file(QNode *root, FILE *f){

    if (root->subdivided == 0){
        for (int i = 0; i < root->exists; ++i){
            Node *cur = root->array[i];
            fprintf(f, "%d\n%d\n%s\n", cur->x, cur->y, cur->info);
        }
    }

    if (NULL != root->SW){
        write_to_file(root->SW, f);
    }
    if (NULL != root->SE){
        write_to_file(root->SE, f);
    }
    if (NULL != root->NW){
        write_to_file(root->NW, f);
    }
    if (NULL != root->NE){
        write_to_file(root->NE, f);
    }
    return;
}

CODE save_to_file(QNode *root, char *filename){
    if (root == NULL){
        return fail;
    }

    FILE *f;
    if (NULL == filename){
        return crit_err;
    } else {
        f = fopen(filename, "w");
    }

    free(filename);
    write_to_file(root, f);
    fclose(f);

    return success;
}
