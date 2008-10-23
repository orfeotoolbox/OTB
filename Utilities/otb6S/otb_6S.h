#ifndef otb_6S_h
#define otb_6S_h

/* Include the basic f2c interface.  */
#include "otb_6S_f2c.h"

/* Mangle the netlib symbols and types to have a otb_6S prefix.  */
#include "otb_6S_mangle.h"

/* Enforce const-correctness only outside otb_6S sources.  */
#ifdef OTB_6S_SRC
# define otb_6S_const
#else
# define otb_6S_const const
#endif

/* Modify the interface for C++.  */
#ifdef __cplusplus
# undef complex
# include <vcl_complex.h>
# define otb_6S_complex vcl_complex<float>
# define otb_6S_doublecomplex vcl_complex<double>
#endif

/* Include the netlib interface.  */
#ifdef __cplusplus
extern "C" {
#endif
extern void otb_6S_initialize();
#include "otb_6S_prototypes.h"
#ifdef __cplusplus
}
#endif

/* Cleanup the namespace if not inside a otb_6S source.  */
#ifndef OTB_6S_SRC
# include "otb_6S_unmangle.h"
#endif

/* Automatically initialize the netlib library for C++ clients.  */
/*
#if defined(__cplusplus) && !defined(OTB_6S_SRC)
struct otb_6S_initializer
{
  otb_6S_initializer() { otb_6S_initialize(); }
};
static otb_6S_initializer otb_6S_initializer_instance;
#endif
*/

#endif
