#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

struct ptrmix {
  uintptr_t content;
};

struct ptrmix mixptr(void *a, void *b);
void *unmixptr(struct ptrmix mix, void *a);
void remixptr(struct ptrmix *mix, void *a, void *b);


typedef int list_content;

struct list_node {
  struct ptrmix mix;
  list_content value;
};

struct list {
  struct list_node *first;
  struct list_node *last;
};

void list_insert_front(struct list *l, list_content value);
void list_insert_back(struct list *l, list_content value);
void list_print(struct list *l);
void list_clear(struct list *l);


struct list_it {
  struct list_node *cur, *prev;
  bool _skipstep;
};

#define for_each_list_impl(start)                                       \
  for (struct list_it it = {.cur = (start)}; it.cur != NULL; list_step(&it))
inline void list_step(struct list_it *it) {
  if (it->_skipstep) {
    it->_skipstep = false;
    return;
  }
  struct list_node *next = unmixptr(it->cur->mix, it->prev);
  it->prev = it->cur;
  it->cur = next;
}
#define for_each_list(l) for_each_list_impl((l)->first)
#define for_each_list_reverse(l) for_each_list_impl((l)->last)
void list_remove(struct list *l, struct list_it *it);
