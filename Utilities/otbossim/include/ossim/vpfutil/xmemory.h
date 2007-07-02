
/* XMEMORY.H */

#ifndef XMEMORY_DEFINED

#define XMEMORY_DEFINED

#ifdef __cplusplus
extern "C" {
#endif

#include <ossim/ossimConfig.h>

#if __OSSIM_DOS__
#include <alloc.h>
#else
#if __OSSIM_CARBON__
#include <sys/types.h>
#include <sys/malloc.h>
#else
/*  #include <malloc.h> */
#endif
#endif


#define Xmark()
#define Xclear()

#define Xmalloc(size) malloc(size)
#define Xcalloc(nelem, elsize) calloc(nelem, elsize)
#define Xfree(pointer) free(pointer)

#ifdef __cplusplus
}
#endif

#endif
