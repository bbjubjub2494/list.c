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

// mix two pointers with xor.
// properties:
//   ptrmix(a, b) == ptrmix(b, a)
struct ptrmix mixptr(void *a, void *b);

// retrieve a pointer from a ptrmix.
// preconditions:
//   there exists b such that mix == ptrmix(a, b)
// postconditions:
//   the return value is equal to b
void *unmixptr(struct ptrmix mix, void *a);

// alter a pointer within the ptrmix.
// preconditions:
//   there exists c such that *mix == ptrmix(a, c)
// postconditions:
//   *mix == ptrmix(b, c)
void remixptr(struct ptrmix *mix, void *a, void *b);

/******************************************************************************/

// Type for the data stored in a list. This choice should be convenient for most
// users: it can store small integers directly, or pointers with arbitrary
// qualifiers using simple casts.
typedef intptr_t list_content;

struct list_node {
  struct ptrmix mix;
  list_content value;
};

struct list {
  struct list_node *first, *last;
};

void list_insert_front(struct list *l, list_content value);
void list_insert_back(struct list *l, list_content value);
void list_print(const struct list *l);
void list_clear(struct list *l);

/******************************************************************************/

struct list_it {
  struct list_node *cur, *prev;
  bool _skipstep;
};

#define for_each_list_impl(start)                                              \
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
