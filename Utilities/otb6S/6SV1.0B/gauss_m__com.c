/* OTB patches: replace "f2c.h" by "otb_6S.h" */
/*#include "f2c.h"*/
#include "otb_6S.h"

#ifdef __cplusplus
extern "C" {
#endif

struct {
    doublereal xgm[20], wgm[20];
    integer n;
} gauss_m__;

#ifdef __cplusplus
}
#endif
