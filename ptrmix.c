#include "ptrmix.h"

#if INTERFACE
#include <stdint.h>

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
