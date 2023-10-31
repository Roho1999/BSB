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

#ifndef __heap_include__
#define __heap_include__

namespace Heap {
    //  A number of 'bytes' will be allocated on the heap.
    //  The pointer to the memory segment is returned.
    //  If memory is exhausted, the result will be nullptr.
    void *allocate(unsigned int bytes);

    //  Free allocated memory: Only unmodified pointers returned
    //  by 'allocate' shall be passed to this function. Also, don't
    //  do any double-free operations!
    void free(void *segment);
}

#endif
