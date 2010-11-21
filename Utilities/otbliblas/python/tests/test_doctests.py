"""
/******************************************************************************
 * $Id$
 *
 * Project:  libLAS - http://liblas.org - A BSD library for LAS format data.
 * Purpose:  A script to marshal .txt files into Python doctests
 * Author:   Sean C. Gillies, sgillies@frii.com
 *
 ******************************************************************************
 *
 * This file is from the Shapely project:
 * http://trac.gispython.org/projects/PCL/browser/Shapely/trunk/tests/test_doctests.py
 *
 * Copyright (c) 2007, Sean C. Gillies
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of Sean C. Gillies nor the names of
 *       its contributors may be used to endorse or promote products derived from
 *       this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 ****
"""

import doctest
import unittest
import glob
import os
import copy

optionflags = (doctest.REPORT_ONLY_FIRST_FAILURE |
               doctest.NORMALIZE_WHITESPACE |
               doctest.ELLIPSIS)

def list_doctests():
    
    files = glob.glob(os.path.join(os.path.dirname(__file__), '*.txt'))
    import liblas
    
    for f in copy.copy(files):
        if liblas.HAVE_LIBGEOTIFF and liblas.HAVE_GDAL:
            
            # run GDAL's tests only
            if 'GeoTIFF' in f:
                files.remove(f)
            if 'SRS.txt' in f:
                files.remove(f)
        if liblas.HAVE_LIBGEOTIFF and not liblas.HAVE_GDAL:
            if 'GDAL' in f:
                files.remove(f)
            if 'SRS.txt' in f:
                files.remove(f)
        
        if not liblas.HAVE_LIBGEOTIFF and not liblas.HAVE_GDAL:
            if 'GDAL' in f:
                files.remove(f)
            if 'GeoTIFF' in f:
                files.remove(f)
    return files

def open_file(filename, mode='r'):
    """Helper function to open files from within the tests package."""
    return open(os.path.join(os.path.dirname(__file__), filename), mode)

def setUp(test):
    test.globs.update(dict(
            open_file = open_file,
            ))

def test_suite():
    return unittest.TestSuite(
        [doctest.DocFileSuite(os.path.basename(filename),
                              optionflags=optionflags,
                              setUp=setUp)
         for filename
         in list_doctests()])

if __name__ == "__main__":
    runner = unittest.TextTestRunner(verbosity=1)
    runner.run(test_suite())
