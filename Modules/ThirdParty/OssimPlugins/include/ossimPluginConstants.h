/*
 * Copyright (C) 2010-2016 by David Burken
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

#ifndef ossimPluginConstants_HEADER
#define ossimPluginConstants_HEADER

/**
 * DLL IMPORT/EXORT SECTION
 */
#if defined(OSSIM_STATIC)
#  define OSSIMPLUGINSEXPORT
#  define OSSIMPLUGINSIMPORT
#  define OSSIMPLUGINSDLLEXPORT
#  define OSSIM_PLUGINS_DLL
#elif defined(__MINGW32__) || defined(__CYGWIN__) || defined(_MSC_VER) || defined(__VISUALC__) || defined(__BORLANDC__) || defined(__WATCOMC__)
#  define OSSIMPLUGINSEXPORT __declspec(dllexport)
#  define OSSIMPLUGINSIMPORT __declspec(dllimport)
#  ifdef OSSIMPLUGINSMAKINGDLL
#    define OSSIMPLUGINSDLLEXPORT OSSIMPLUGINSEXPORT
#    define OSSIM_PLUGINS_DLL       OSSIMPLUGINSEXPORT
#  else
#    define OSSIMPLUGINSDLLEXPORT OSSIMPLUGINSIMPORT
#    define OSSIM_PLUGINS_DLL      OSSIMPLUGINSIMPORT
#  endif
#else /* not #if defined(_MSC_VER) */
#  define OSSIMPLUGINSEXPORT
#  define OSSIMPLUGINSIMPORT
#  define OSSIMPLUGINSDLLEXPORT
#  define OSSIM_PLUGINS_DLL
#endif /* #if defined(_MSC_VER) */

/** Export nullptr_t from std namespace as ossimRefPtr needs it */
#include <cstddef>
using std::nullptr_t;

#endif /* #ifndef ossimPluginConstants_HEADER */
