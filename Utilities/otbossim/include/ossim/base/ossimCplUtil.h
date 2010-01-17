/*
 * Copied from "gdal" project. See licence below.
 *
 * Global functions for gdal code compatibility.
 *
 * Project:  ISO 8211 Access
 * Purpose:  Main declarations for ISO 8211.
 * Author:   Frank Warmerdam, warmerdam@pobox.com
 *
 ******************************************************************************
 * Copyright (c) 1999, Frank Warmerdam <warmerdam@pobox.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 ******************************************************************************
 * $Id: ossimCplUtil.h 13013 2008-06-10 13:34:01Z dburken $
 */

#ifndef ossimCplUtil_HEADER
#define ossimCplUtil_HEADER

#include <cstdio> /* to pick up "size_t" */
#include <ossim/base/ossimConstants.h>


OSSIMDLLEXPORT char** ossimCSLTokenizeString(const char *pszString );

OSSIMDLLEXPORT char** ossimCSLTokenizeStringComplex(const char *pszString,
                                                    const char *pszDelimiter,
                                                    int bHonourStrings,
                                                    int bAllowEmptyTokens );

OSSIMDLLEXPORT char** ossimCSLTokenizeString2( const char *pszString, 
                                               const char *pszDelimeter, 
                                               int nCSLTFlags );

OSSIMDLLEXPORT int ossimCSLCount(char **papszStrList);

OSSIMDLLEXPORT void ossimCSLDestroy(char **papszStrList);

OSSIMDLLEXPORT void* ossimCPLRealloc( void * pData, size_t nNewSize );

#endif /* ossimCplUtil_HEADER */
