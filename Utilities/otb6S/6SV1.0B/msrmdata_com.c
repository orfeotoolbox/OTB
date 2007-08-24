
/* OTB patches: replace "f2c.h" by "otb_6S_f2c.h" */
/*#include "f2c.h"*/
#include "otb_6S_f2c.h"

#ifdef __cplusplus
extern "C" {
#endif

struct {
    doublereal th10, rncoef, cab, cw, bq;
} msrmdata_;

#ifdef __cplusplus
}
#endif
