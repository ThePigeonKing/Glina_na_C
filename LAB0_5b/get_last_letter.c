#include "stdio.h"
#include "string.h"
#include "stdlib.h"

#define DEBUG 0
#define print_debug(fmt, ...) \
	do { if (DEBUG) fprintf(stderr, fmt, __VA_ARGS__); } while (0)

typedef struct ListCell{
  char simb; // хранимый символ
  struct ListCell *next;
  struct ListCell *global_head;
} ListCell;

char* getLine(){
  char *ptr = (char *)malloc(1);
  char *tmp_ptr = (char *)malloc(1);
  char buf[81];
  int rsp, len = 0;
  *ptr = '\0';
  printf("Enter line (or Ctrl-D to exit) --->\n");

  do {
    rsp = scanf("%80[^\n]", buf);
    if (rsp < 0){
      free(ptr);
      printf("EOF found, shutting down...\n");
      return NULL;
    } else if (rsp == 0){
      scanf("%*c"); // считываем \n
    } else {  // n > 0
      len += strlen(buf);
      tmp_ptr = (char*)realloc(ptr, len+1);
      if (tmp_ptr == NULL){
        free(ptr);
        return NULL; // ошибка выделения памяти
      }
      ptr = tmp_ptr;
      strcat(ptr, buf);
    }
  } while (rsp > 0);

  return ptr;
}

//СТРУКТУРЫ----------------------------------------------
ListCell* create_node(ListCell *lst, char a){
  ListCell* tmp = (ListCell *)malloc(sizeof(ListCell));
  ListCell* p = (ListCell *)malloc(sizeof(ListCell));

  if (lst == NULL){ // инициализация списка
    print_debug("CREATING NEW LIST - \"%c\"\n", a);

    tmp->simb = a;
    tmp->next = NULL;
    tmp->global_head = tmp;

  } else {  // добавление элемента
    print_debug("APPENDING NEW ELEM - \"%c\" \n", a);
    p = lst->next; // второй перест. указатель
    lst->next = tmp;  // первый перест. указатель
    tmp->simb = a;
    tmp->next = p;
    tmp->global_head = lst->global_head;

  }

  return tmp;
}

void transfer_global_head(ListCell *head){
  ListCell *p = head->next, *new_head = head->next; // на который переставляем
  print_debug("%s", "Transfering head..\n");
  while (p != NULL){
    p->global_head = new_head;
    p = p->next;
  }
  return;
}

ListCell* list_del_elem(ListCell* removable){
  ListCell *p = removable, *prev = removable->global_head;

  if (removable != removable->global_head){ // если не голова
    //print_debug("Trying to delete \"%c\"\n", removable->simb);

    while(prev->next != p){ // находим пердыдущий элемент
      prev = prev->next;

      if (prev->next == NULL){
        printf("!Element not found in list!\n");
        return NULL;
      }
    }
    prev->next = p->next;
    free(p);
    return prev->next;
  }
  else {  // если головной элемент
    transfer_global_head(removable);
    p = removable->next;
    free(removable);
    return p;
  }
}


void list_normalize(ListCell *lst){  // удалить пробелы и заменить табуляции
  ListCell *prev = lst->global_head;
  ListCell *current = prev->next;
  while(current != NULL){

    // конвертация из табуляций
    if (prev->simb == '\t'){
      prev->simb = ' ';
    }
    if (current->simb == '\t'){
      current->simb = ' ';
    }
    if (prev->simb == ' ' && current->simb == ' '){
      current = list_del_elem(current); // удаление элемента
    } else {
      prev = current;
      current = current->next;
    }
  }
}

void do_magic(ListCell *lst){
  print_debug("%s", "DO MAGIC BEGIN\n");

  ListCell *prev = lst->global_head;
  ListCell *current = prev->next;
  print_debug("prev - %c, current - %c\n", prev->simb, current->simb);

  while(current != NULL){
    if (current->simb != ' ' && prev->simb != ' '){
      current = current->next;
      prev = list_del_elem(prev);

    } else {
      print_debug("Passig elems \"%c\" and \"%c\" \n", prev->simb, current->simb);
      prev = prev->next;
      current = current->next;
    }
  }

  return;
}


void list_print(ListCell *lst, char* msg){
  ListCell* current = lst->global_head;
  printf("%s", msg);
  print_debug("global_head simb - %c\n", lst->global_head->simb);

  //
  while(current){
    // if(current == N)
    //   printf("NULL NULL NULL /////////////%c\n", current->simb);

    printf("%c", current->simb);

    current = current->next;
  }
  printf("\n");
  return;
}

void list_erase(ListCell *lst){
  ListCell *p = lst->global_head, *tmp;
  print_debug("Erasing list with head elem = %c\n", p->simb);
  while(p != NULL){
    tmp = p;
    p = p->next;
    free(tmp);
  }
  return;
}

int main(int argc, char const *argv[]) {

  char* arr;
  ListCell *head = NULL;

  while((arr = getLine()) != NULL){
    ListCell *current = NULL;

    if (strlen(arr) != 0){
      for (int i = 0; i < strlen(arr); ++i){
        current = create_node(current, arr[i]);
      }

    //list_print(current, "Original list--->\n");

    list_normalize(current);

    //list_print(current, "Normalized list--->\n");

    do_magic(current);

    list_print(current, "Magiced list--->\n");


    list_erase(current);
    } else {
      printf("Entered empty line, skipping...\n");
    }
  }



  return 0;
}
