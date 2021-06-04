#ifndef TPK_FILES
#define TPK_FILES

#include "structs.h"

QNode *read_from_file(QNode **old_root, char *filename);
CODE tree_print(QNode *root);
CODE save_to_file(QNode *root, char *filename);

#endif