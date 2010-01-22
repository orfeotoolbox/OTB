#include <string.h>

int
main(int argc, char *argv[])
{
/* void *memmove (void *dest, const void *src, size_t n); */
        char * foo;
        const char * cfoo;
        foo = memmove((void*)foo,(const void *)cfoo,(size_t)1);
        return 1;
}
