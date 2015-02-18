/* OTB patches: replace "f2c.h" by "otb_6S.h" */
/*#include "f2c.h"*/
#include "otb_6S.h"

#ifdef __cplusplus
extern "C" {
#endif

struct {
    integer num_z__;
    doublereal alt_z__[101], taer_z__[101], taer55_z__[101];
} aeroprof_;

#ifdef __cplusplus
}
#endif
