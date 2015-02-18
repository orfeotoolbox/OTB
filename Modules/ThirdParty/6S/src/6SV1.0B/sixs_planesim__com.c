/* OTB patches: replace "f2c.h" by "otb_6S.h" */
/*#include "f2c.h"*/
#include "otb_6S.h"

#ifdef __cplusplus
extern "C" {
#endif

struct {
    doublereal zpl[34], ppl[34], tpl[34], whpl[34], wopl[34];
} sixs_planesim__;

#ifdef __cplusplus
}
#endif
