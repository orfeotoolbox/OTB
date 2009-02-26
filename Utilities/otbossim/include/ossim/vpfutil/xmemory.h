
/* XMEMORY.H */

#ifndef XMEMORY_DEFINED

#define XMEMORY_DEFINED

#ifdef __cplusplus
extern "C" {
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
