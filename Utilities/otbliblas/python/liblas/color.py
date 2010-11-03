"""
/******************************************************************************
 * $Id$
 *
 * Project:  libLAS - http://liblas.org - A BSD library for LAS format data.
 * Purpose:  Python Color implementation
 * Author:   Howard Butler, hobu.inc@gmail.com
 *
 ******************************************************************************
 * Copyright (c) 2009, Howard Butler
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following
 * conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in
 *       the documentation and/or other materials provided
 *       with the distribution.
 *     * Neither the name of the Martin Isenburg or Iowa Department
 *       of Natural Resources nor the names of its contributors may be
 *       used to endorse or promote products derived from this software
 *       without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 ****************************************************************************/
 """
import core
import ctypes


class Color(object):
    def __init__(self, red=0, green=0, blue=0, owned=True, handle=None):
        """
        :keyword red: Red value for the point
        :type red: integer
        :keyword green: Green value for the point
        :type green: integer
        :keyword blue: Blue value for the point
        :type blue: integer
        :keyword owned: flag to denote whether or not the instance owns
                        its shadow
        :type owned: boolean
        :keyword handle: raw ctypes object

        >>> from liblas import color
        >>> c = color.Color()
        >>> c.red
        0
        >>> c.green
        0
        >>> c.blue
        0

        >>> c = color.Color(red = 123, blue = 125, green = 124)
        >>> c.red
        123
        >>> c.green
        124
        >>> c.blue
        125

        .. note::
            RGB values should always be normalized to 16 bit values. For
            example, when encoding an 8 bit per channel pixel, multiply each
            channel value by 256 prior to storage in these fields. This
            normalization allows color values from different camera bit depths
            to be accurately merged.
        """
        if handle:
            self.handle = handle
        else:
            self.handle = core.las.LASColor_Create()

        if red:
            self.red = red
        if green:
            self.green = green
        if blue:
            self.blue = blue

        self.owned = owned

    def __del__(self):
        if self.owned:
            if self.handle and core:
                core.las.LASColor_Destroy(self.handle)

    def get_red(self):
        return core.las.LASColor_GetRed(self.handle)

    def set_red(self, value):
        return core.las.LASColor_SetRed(self.handle, value)
    doc = "Red value of the color triple"
    red = property(get_red, set_red, None, doc)

    def get_green(self):
        return core.las.LASColor_GetGreen(self.handle)

    def set_green(self, value):
        return core.las.LASColor_SetGreen(self.handle, value)
    doc = "Green value of the color triple"
    green = property(get_green, set_green, None, doc)

    def get_blue(self):
        return core.las.LASColor_GetBlue(self.handle)

    def set_blue(self, value):
        return core.las.LASColor_SetBlue(self.handle, value)
    doc = "Blue value of the color triple"
    blue = property(get_blue, set_blue, None, doc)

    def __iter__(self):
        """Iterator support.  Values are returned in [R,G,B] order

          >>> for i in c:
          ...     print i
          123
          124
          125

          >>> list(c)
          [123, 124, 125]
        """
        r = False
        g = False
        b = False
        if not r:
            r = True
            yield self.red
        if not g:
            g = True
            yield self.green
        if not b:
            b = True
            yield self.blue
