#include "list.h"

#include <inttypes.h>
#include <stdlib.h>

#if INTERFACE
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

// The heart of the xorlist system: a ptrmix holds the exclusive or of 2 pointer
// values. It has the same memory footprint as one pointer, and is able to
// produce either of the original pointer values when provided with the other.
// We wrap it in a structure to make invariants easy to maintain.
struct ptrmix {
  uintptr_t content;
};
#endif // INTERFACE

// mix two pointers with xor.
// properties:
//   ptrmix(a, b) == ptrmix(b, a)
ptrmix mixptr(void *a, void *b) {
  return (ptrmix){(uintptr_t)a ^ (uintptr_t)b};
}

// retrieve a pointer from a ptrmix.
// preconditions:
//   there exists b such that mix == ptrmix(a, b)
// postconditions:
//   the return value is equal to b
void *unmixptr(ptrmix mix, void *a) {
  return (void *)(mix.content ^ (uintptr_t)a);
}

// alter a pointer within the ptrmix.
// preconditions:
//   there exists c such that *mix == ptrmix(a, c)
// postconditions:
//   *mix == ptrmix(b, c)
void remixptr(ptrmix *mix, void *a, void *b) {
  mix->content ^= (uintptr_t)a;
  mix->content ^= (uintptr_t)b;
}

/******************************************************************************/

#if INTERFACE
// Type for the data stored in a list. This choice should be convenient for most
// users: it can store small integers directly, or pointers with arbitrary
// qualifiers using simple casts.
typedef intptr_t list_content;

// Header structure of the list.
struct list {
  struct list_node *first, *last;
};

// Structure of nodes in the list. Incidentally It's the size of 2 consecutive
// pointers, which plays very nice on x86_64.
struct list_node {
  ptrmix mix;
  list_content value;
};
#endif // INTERFACE

static list_node *list_make_node(list_content value, list_node *prev,
                                 list_node *next) {
  list_node *new_node = malloc(sizeof *new_node);
  new_node->value = value;
  new_node->mix = mixptr(prev, next);
  return new_node;
}

void list_insert_front(list *l, list_content value) {
  if (l->first == NULL) {
    l->first = l->last = list_make_node(value, NULL, NULL);
    return;
  }
  list_node *second = unmixptr(l->first->mix, NULL),
            *new_first = list_make_node(value, NULL, l->first);
  l->first->mix = mixptr(new_first, second);
  l->first = new_first;
}

void list_insert_back(list *l, list_content value) {
  if (l->last == NULL) {
    l->first = l->last = list_make_node(value, NULL, NULL);
    return;
  }
  list_node *penultimate = unmixptr(l->last->mix, NULL),
            *new_last = list_make_node(value, l->last, NULL);
  l->last->mix = mixptr(penultimate, new_last);
  l->last = new_last;
}

void list_print(const list *l) {
  printf("(");
  const char *sep = "";
  for_each_list_reverse(l) {
    printf("%s%" PRIdPTR, sep, it.cur->value);
    sep = ", ";
  }
  printf(")");
}

void list_clear(list *l) {
  list_it it = {.cur = l->first};
  while (it.cur != NULL) {
    list_step(&it);
    free(it.prev);
  }
  l->first = l->last = NULL;
}

/******************************************************************************/

#if INTERFACE
struct list_it {
  list_node *cur, *prev;
  bool _skipstep;
};
#define for_each_list_impl(start)                                              \
  for (list_it it = {.cur = (start)}; it.cur != NULL; list_step(&it))

#define for_each_list(l) for_each_list_impl((l)->first)
#define for_each_list_reverse(l) for_each_list_impl((l)->last)
#endif // INTERFACE

void list_step(list_it *it) {
  if (it->_skipstep) {
    it->_skipstep = false;
    return;
  }
  list_node *next = unmixptr(it->cur->mix, it->prev);
  it->prev = it->cur;
  it->cur = next;
}

void list_remove(list *l, list_it *it) {
  if (it->cur == l->first) {
    list_node *second = unmixptr(l->first->mix, NULL);
    l->first = second;
  }
  if (it->cur == l->last) {
    list_node *penultimate = unmixptr(l->last->mix, NULL);
    l->last = penultimate;
  }
  list_node *next = unmixptr(it->cur->mix, it->prev);
  if (it->prev != NULL)
    remixptr(&it->prev->mix, it->cur, next);
  if (next != NULL)
    remixptr(&next->mix, it->cur, it->prev);
  free(it->cur);
  it->cur = next;
  it->_skipstep = true;
}

int main() {
  list l = {0};
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
