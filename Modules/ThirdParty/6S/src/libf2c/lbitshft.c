
/* OTB patches: replace "f2c.h" by "otb_6S_f2c.h" */
/*#include "f2c.h"*/
#include "otb_6S_f2c.h"
#ifdef __cplusplus
extern "C" {
#endif

 integer
#ifdef KR_headers
lbit_shift(a, b) integer a; integer b;
#else
lbit_shift(integer a, integer b)
#endif
{
	return b >= 0 ? a << b : (integer)((uinteger)a >> -b);
	}
#ifdef __cplusplus
}
#endif
