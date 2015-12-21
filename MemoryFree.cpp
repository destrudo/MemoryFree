#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#include "MemoryFree.h"

#ifdef __AVR__ /* AVR specific code */

extern unsigned int __heap_start;
extern void *__brkval;

/*
 * The free list structure as maintained by the
 * avr-libc memory allocation routines.
 */
struct __freelist
{
  size_t sz;
  struct __freelist *nx;
};

/* The head of the free list structure */
extern struct __freelist *__flp;


/* Calculates the size of the free list */
int freeListSize()
{
  struct __freelist* current;
  int total = 0;
  for (current = __flp; current; current = current->nx)
  {
    total += 2; /* Add two bytes for the memory block's header  */
    total += (int) current->sz;
  }

  return total;
}

int freeMemory()
{
  int free_memory;
  if ((int)__brkval == 0)
  {
    free_memory = ((int)&free_memory) - ((int)&__heap_start);
  }
  else
  {
    free_memory = ((int)&free_memory) - ((int)__brkval);
    free_memory += freeListSize();
  }
  return (int)free_memory;
}


#elif defined(__arm__) /*
			* All arm stuff should be fine using this. (Tested on
			* DUE and SAMD [zero and sparkfun])
			*/

#include <malloc.h>
#include <stdlib.h>
#include <stdio.h>

extern "C" char *sbrk(int i);

int freeMemory()
{
	char *heapend=sbrk(0);
	register char * stack_ptr asm ("sp");
	struct mallinfo mi=mallinfo();
	/*
	 * The accuracy of this is to within 8 bytes.  Less than ideal, but
	 * still usable.
	 */
	return (stack_ptr - heapend + mi.fordblks);
}


#elif ESP8266
int freeMemory() /* ESP8266 code not implemented yet */
{
	return 0;
}
#endif
