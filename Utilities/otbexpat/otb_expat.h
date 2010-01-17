#ifndef __otb_expat_h
#define __otb_expat_h

/* Use the expat library configured for OTB.  */
#ifdef OTB_USE_INTERNAL_EXPAT
# include "lib/expat.h"
#else
# include <expat.h>
#endif

#endif
