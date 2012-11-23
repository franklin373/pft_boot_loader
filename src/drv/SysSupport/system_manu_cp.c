#include <string.h>

#if 0
#define RAM_START (0x10000000)
/* copy all bytes between s (inclusive) and e (exclusive) to d */
void system_manu_cp_activate(void * s, void * e, void * d)
{
  size_t size = (size_t)e - (size_t)s;
#if 0  
  memcpy(d,s,size);
#else
	{
		int i;
		for(i=0;i<size;i++){
			((char *)d)[i]=((char *)s)[i];
		}
	}
#endif
}

/* copy the bytes from RAMCODE_ID to RAMCODE */
void system_manu_cp(void)
{
  #pragma segment=".intvec"
  system_manu_cp_activate(__sfb(".intvec"), __sfe(".intvec"), (void *)RAM_START);
}
#endif