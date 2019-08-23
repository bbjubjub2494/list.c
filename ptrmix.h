#include <stdint.h>

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
