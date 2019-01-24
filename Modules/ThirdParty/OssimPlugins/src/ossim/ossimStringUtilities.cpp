/*
 * Copyright (C) 2005-2019 by Centre National d'Etudes Spatiales (CNES)
 *
 * This file is licensed under MIT license:
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "ossim/ossimStringUtilities.h"
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
