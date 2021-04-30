#ifndef TPK_FUNCS
#define TPK_FUNCS
#include "structs.h"

void mainloop();
enum CODES create_info(Info **cell);
char *getLine();
Node* initialize(char *key_to_init);

#endif