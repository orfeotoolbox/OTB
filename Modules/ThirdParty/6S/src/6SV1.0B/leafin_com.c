/* OTB patches: replace "f2c.h" by "otb_6S.h" */
/*#include "f2c.h"*/
#include "otb_6S.h"

#ifdef __cplusplus
extern "C" {
#endif

union {
    struct {
	doublereal nnl, vai, kk;
    } _1;
    struct {
	doublereal nn, vai, k;
    } _2;
} leafin_;

#ifdef __cplusplus
}
#endif
