/*****************************************************************************/
/* Operating System Construction                                             */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                                 H E A P                                   */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* This is a global heap for the OS kernel with a fixed size. Use it with    */
/* care, as you might quickly run out of memory.                             */
/*****************************************************************************/

#include "heap.h"

namespace Heap {
  static const int size = 64*1024*1024; // 64 MiB

  struct memory_block {
    struct memory_block *prev;
    struct memory_block *next;
    unsigned int size;
    int used;
  };
  static_assert(sizeof(memory_block) % 8 == 0, "Unexpected size of memory_block!");

  unsigned char memory[size];
  struct memory_block *cmb = nullptr;   // current memory block

  // INIT: Initializes the module. A descriptor for one big free memory
  //       segment is created.
  void init() {
    cmb = (struct memory_block *)memory;
    cmb->prev = cmb;
    cmb->next = cmb;
    cmb->used = 0;
    cmb->size = size - sizeof (struct memory_block);
  }

  //  A number of 'bytes' will be allocated on the heap.
  //  The pointer to the memory segment is return.
  //  If memory is exhausted, the result will be nullptr.
  void *allocate(unsigned int bytes) {
    if (!cmb)
      init();

    // make sure to have some alignment
    if (bytes % 8 != 0)
      bytes += 8 - bytes % 8;

    void *result = nullptr;
    struct memory_block *start = cmb;
    do {
      if (!cmb->used && cmb->size >= bytes) {
        cmb->used = 1;
        result = (void*)(cmb + 1);
        if (cmb->size > bytes + sizeof (struct memory_block)) {
          struct memory_block* new_mb = (struct memory_block*)
              ((char*)cmb + sizeof (struct memory_block) + bytes);
          new_mb->prev = cmb;
          new_mb->next = cmb->next;
          new_mb->used = 0;
          new_mb->size = cmb->size - sizeof (struct memory_block) - bytes;
          new_mb->next->prev = new_mb;
          cmb->next = new_mb;
          cmb->size = bytes;
        }
        break;
      }
      cmb = cmb->next;
    }
    while (cmb != start && !result);
    return result;
  }

  //  Free allocated memory: Only unmodified pointers returned
  //  by 'allocate' shall be passed to this function. Also, don't
  //  do any double-free operations!
  void free(void *segment) {

    struct memory_block *mb = (struct memory_block*)segment - 1;
    if (!mb->used)
      return; // this should *not* happen

    struct memory_block* big_mb;
    mb->used = 0;
    if (!mb->prev->used && mb->prev < mb) {
      big_mb = mb->prev;
      big_mb->next = mb->next;
      big_mb->size += mb->size + sizeof (struct memory_block);
      mb->next->prev = big_mb;
    }
    else
      big_mb = mb;

    if (cmb == mb)
      cmb = big_mb;

    if (!big_mb->next->used && big_mb->next > big_mb) {
      big_mb->size += big_mb->next->size + sizeof (struct memory_block);
      if (cmb == big_mb->next)
          cmb = big_mb;
      big_mb->next = big_mb->next->next;
      big_mb->next->prev = big_mb;
    }
  }
}
