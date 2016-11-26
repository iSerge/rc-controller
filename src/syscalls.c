/* based on a example-code from Keil for CS G++ */

/* for caddr_t (typedef char * caddr_t;) */
#include <sys/types.h>

int __errno;

extern int  __HEAP_START;

caddr_t _sbrk ( int incr )
{
  static char* heap = (void*)0;
  char* prev_heap;

  if (heap == (void*)0) {
    heap = (void*)&__HEAP_START;
  }
  prev_heap = heap;

  /* Normally here should be check for stack pointer overlapping.
     Not really care for it under FreeRTOS environment. Every task
     have it's own fixed length stack. */

  heap += incr;

  return (caddr_t) prev_heap;
}
