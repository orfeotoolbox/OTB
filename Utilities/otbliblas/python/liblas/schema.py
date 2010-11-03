"""
/******************************************************************************
 * $Id$
 *
 * Project:  libLAS - http://liblas.org - A BSD library for LAS format data.
 * Purpose:  Python LASSchema implementation
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


class Schema(object):
    """:class:`liblas.schema.Schema` is an object that keeps track of the
    point format sizes and what data elements the point formats have (color,
    time, etc), as well as accounting for their size in bytes. """

    def __init__(self,  point_format=0,
                        handle=None):
        """
        :keyword major: Major version value (always 1 for libLAS)
        :type major: integer
        :keyword minor: Minor version value (no validation done)
        :type minor: integer
        :keyword time: flag to denote whether or not the format has time \
        values
        :type time: boolean
        :keyword color: flag to denote whether or not the format has color \
        values
        :type owned: boolean
        :keyword size: The size to set for the format. If this value is set \
        to something that is smaller than can represent the given \
        parameters, an exception will be thrown.
        :keyword size: integer
        :keyword handle: raw ctypes object

        >>> f = schema.Schema()
        >>> f.size
        20L
        >>> f.color
        False
        >>> f.time
        False
        >>> f.major, f.minor
        (1, 2)

        """
        self.handle = None

        if handle:
            self.handle = handle
        else:
            self.handle = core.las.LASSchema_Create(point_format)

    def __del__(self):
        if self.handle and core:
            core.las.LASSchema_Destroy(self.handle)



    def get_size(self):
        return core.las.LASSchema_GetByteSize(self.handle)

    def set_size(self, value):
        return core.las.LASSchema_SetByteSize(self.handle, int(value))

    doc = """Size in bytes of the format.  """
    size = property(get_size, set_size, None, doc)


