#include "list.h"

#include <stdlib.h>

struct ptrmix mixptr(void *a, void *b) {
  return (struct ptrmix){(uintptr_t)a ^ (uintptr_t)b};
}
void *unmixptr(struct ptrmix mix, void *a) {
  return (void *)(mix.content ^ (uintptr_t)a);
}

void remixptr(struct ptrmix *mix, void *a, void *b) {
  mix->content ^= (uintptr_t)a;
  mix->content ^= (uintptr_t)b;
}

static struct list_node *list_make_node(list_content value,
                                        struct list_node *prev,
                                        struct list_node *next) {
  struct list_node *new_node = malloc(sizeof *new_node);
  new_node->value = value;
  new_node->mix = mixptr(prev, next);
  return new_node;
}

void list_insert_front(struct list *l, list_content value) {
  if (l->first == NULL) {
    l->first = l->last = list_make_node(value, NULL, NULL);
    return;
  }
  struct list_node *second = unmixptr(l->first->mix, NULL),
                   *new_first = list_make_node(value, NULL, l->first);
  l->first->mix = mixptr(new_first, second);
  l->first = new_first;
}

void list_insert_back(struct list *l, list_content value) {
  if (l->last == NULL) {
    l->first = l->last = list_make_node(value, NULL, NULL);
    return;
  }
  struct list_node *penultimate = unmixptr(l->last->mix, NULL),
                   *new_last = list_make_node(value, l->last, NULL);
  l->last->mix = mixptr(penultimate, new_last);
  l->last = new_last;
}

void list_print(struct list *l) {
  printf("(");
  const char *sep = "";
  for_each_list_reverse(l) {
    printf("%s%d", sep, it.cur->value);
    sep = ", ";
  }
  printf(")");
}

void list_clear(struct list *l) {
  struct list_it it = {.cur = l->first};
  while (it.cur != NULL) {
    list_step(&it);
    free(it.prev);
  }
  l->first = l->last = NULL;
}

extern inline void list_step(struct list_it *it);

void list_remove(struct list *l, struct list_it *it) {
  if (it->cur == l->first) {
    struct list_node *second = unmixptr(l->first->mix, NULL);
    l->first = second;
  }
  if (it->cur == l->last) {
    struct list_node *penultimate = unmixptr(l->last->mix, NULL);
    l->last = penultimate;
  }
  struct list_node *next = unmixptr(it->cur->mix, it->prev);
  if (it->prev != NULL)
    remixptr(&it->prev->mix, it->cur, next);
  if (next != NULL)
    remixptr(&next->mix, it->cur, it->prev);
  free(it->cur);
  it->cur = next;
  it->_skipstep = true;
}

int main() {
  struct list l = {};
  list_print(&l);
  printf("\n");
  list_insert_front(&l, 0);
  list_insert_front(&l, 1);
  list_insert_front(&l, 2);
  list_insert_front(&l, 3);
  list_insert_front(&l, 4);
  for_each_list(&l) {
    if (it.cur->value % 2 == 0)
      list_remove(&l, &it);
  }
  list_print(&l);
  printf("\n");
  list_clear(&l);
  return EXIT_SUCCESS;
}
