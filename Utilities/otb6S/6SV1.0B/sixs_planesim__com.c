
/* OTB patches: replace "f2c.h" by "otb_6S_f2c.h" */
/*#include "f2c.h"*/
#include "otb_6S_f2c.h"

#ifdef __cplusplus
extern "C" {
#endif

struct {
    real zpl[34], ppl[34], tpl[34], whpl[34], wopl[34];
} sixs_planesim__;

#ifdef __cplusplus
}
#endif
