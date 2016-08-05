//----------------------------------------------------------------------------
//
// "Copyright Centre National d'Etudes Spatiales"
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
//----------------------------------------------------------------------------
// $Id$
#include "ossimStringUtilities.h"
#include "ossimplugins-config.h"
#if defined(HAVE_STD_SNPRINTF)
#  include <cstdio>
#elif defined(HAVE_MSC_SNPRINTF)
#  include <stdio.h>
#else
#   error "Don't know how to perform snprintf"
#endif

int ossimplugins::vs_printf(char * str, std::size_t size, const char * format, std::va_list ap)
{
#if defined(HAVE_STD_SNPRINTF)
    const int res = std::vsnprintf(str, size, format, ap);
    return res;
#elif defined(HAVE_MSC_SNPRINTF)
    const int res = _vsnprintf(str, size, format, ap);
    // force 0x0 on the last possible character at worst
    str[size-1] = '\0';
    return res;
#else
#   error "Don't know how to perform snprintf"
#endif
}

int ossimplugins::s_printf(char *str, std::size_t size, const char *format, ...)
{
    std::va_list ap;
    va_start(ap, format);
    const int res = ossimplugins::vs_printf(str, size, format, ap);
    va_end(ap);
    return res;
}
