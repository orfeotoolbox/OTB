"""
/******************************************************************************
 * $Id$
 *
 * Project:  libLAS - http://liblas.org - A BSD library for LAS format data.
 * Purpose:  Python LASGUID implementation
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


class GUID(object):
    def __init__(self, key=None, handle=None):
        """
        :keyword key: GUID string to create with
        :type key: string
        :keyword handle: raw ctypes object

        >>> from liblas import guid
        >>> from liblas import header

        >>> g2 = guid.GUID(key='8388f1b8-aa1b-4108-bca3-6bc68e7b062e')
        >>> g2
        8388f1b8-aa1b-4108-bca3-6bc68e7b062e

        >>> header = header.Header()
        >>> header.guid = g2
        >>> header.guid
        8388f1b8-aa1b-4108-bca3-6bc68e7b062e
        >>> header.project_id
        '8388f1b8-aa1b-4108-bca3-6bc68e7b062e'

        >>> g3 = guid.GUID(key='8388f1b8-aa1b-4108-bca3-6bc68e7b062e')
        >>> g2 == g3
        True

        .. note::
            In Python 2.5+, you can use the :mod:`uuid` module to control the
            creation of UUIDs in alternative ways. Otherwise, the GUID()
            constructor will create them for you, but rather simply.
        """
        self.handle = None
        if handle:
            self.handle = handle
        else:
            if not key:
                self.handle = core.las.LASGuid_Create()
            else:
                self.handle = core.las.LASGuid_CreateFromString(key)

    def __del__(self):
        if self.handle:
            core.las.LASGuid_Destroy(self.handle)

    def __str__(self):
        """String representation of the GUID"""
        return core.las.LASGuid_AsString(self.handle)

    def __eq__(self, other):
        """Test GUID for equality against another :obj:`liblas.guid.GUID`
        instance

        :param other: The :obj:`liblas.guid.GUID` instance to test against
        """
        if isinstance(other, GUID):
            return bool(core.las.LASGuid_Equals(self.handle, other.handle))
        raise core.LASException("GUID can only be compared to other "
                                "GUIDs, not %s" % type(other))

    def __repr__(self):
        return self.__str__()
