#if defined (WIN32)
#define SQLITE_API __declspec(dllexport)
#endif

#include "sqlite3.c"