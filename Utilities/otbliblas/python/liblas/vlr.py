"""
/******************************************************************************
 * $Id$
 *
 * Project:  libLAS - http://liblas.org - A BSD library for LAS format data.
 * Purpose:  Python VLR implementation
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


class VLR(object):
    def __init__(self, owned=True, handle=None):
        """
        :keyword owned: flag to denote whether or not the VLR owns itself.  \
                        If it is owned, the underlying C object will be \
                        destroyed once it goes out of scope.
        :keyword handle: raw ctypes object

        From the specification_:

            The Public Header Block is followed by one or more Variable Length
            Records (There is one mandatory Variable Length Record,
            GeoKeyDirectoryTag). The number of Variable Length Records is
            specified in the "Number of Variable Length Records"
            (:obj:`liblas.header.Header.records_count`) field in the Public
            Header Block. The Variable Length Records must be accessed
            sequentially since the size of each variable length record is
            contained in the Variable Length Record Header. Each Variable
            Length Record Header is 60 bytes in length.

        >>> from liblas import vlr
        >>> v = vlr.VLR()
        >>> v.reserved
        0
        >>> v.recordid
        0
        >>> v.recordid = 2
        >>> v.recordid
        2
        >>> v.userid
        ''
        >>> v.userid = 'liblas.org'
        >>> v.userid
        'liblas.org'

        >>> v.description
        ''
        >>> v.description = 'libLAS'
        >>> v.description
        'libLAS'

        """
        if handle:
            self.handle = handle
        else:
            self.handle = core.las.LASVLR_Create()
        self.owned = owned

    def __del__(self):
        if self.owned:
            if self.handle and core:
                core.las.LASVLR_Destroy(self.handle)

    def get_userid(self):
        return core.las.LASVLR_GetUserId(self.handle)

    def set_userid(self, value):
        return core.las.LASVLR_SetUserId(self.handle, value)
    doc = """User ID key for this VLR (clipped to 16 bytes long)
    
    The User ID for libLAS is ``liblas``, and it will be used 
    for all extra VLRs that libLAS itself can interpret.  
    
    .. seealso::
        :ref:`liblas_vlr_key` has more detail on the key and name.
        
    From the specification_:
        The User ID field is ASCII character data that identifies the user
        which created the variable length record. It is possible to have many
        Variable Length Records from different sources with different User
        IDs. If the character data is less than 16 characters, the remaining
        data must be null. The User ID must be registered with the LAS
        specification managing body. The management of these User IDs ensures
        that no two individuals accidentally use the same User ID. The
        specification will initially use two IDs: one for globally specified
        records (LASF_Spec), and another for projection types
        (LASF_Projection). Keys may be requested at
        http://www.asprs.org/lasform/keyform.html
        
    """
    userid = property(get_userid, set_userid, None, doc)
    user = userid

    def get_description(self):
        return core.las.LASVLR_GetDescription(self.handle)

    def set_description(self, value):
        return core.las.LASVLR_SetDescription(self.handle, value)
    doc = """Description of this VLR instance (clipped to 32 bytes long)

    From the specification_:
        Optional, null terminated text description of the data. Any remaining
        characters not used must be null.
    """
    description = property(get_description, set_description, None, doc)

    def get_recordlength(self):
        return core.las.LASVLR_GetRecordLength(self.handle)

    def set_recordlength(self, value):
        return core.las.LASVLR_SetRecordLength(self.handle, value)
    doc = """The number of bytes long the VLR is"""
    recordlength = property(get_recordlength, set_recordlength, None, doc)
    
    def __len__(self):
        return self.recordlength
        
    def get_recordid(self):
        return core.las.LASVLR_GetRecordId(self.handle)

    def set_recordid(self, value):
        return core.las.LASVLR_SetRecordId(self.handle, value)
    doc = """Record ID for the VLR

    From the specification_:
        The Record ID is dependent upon the User ID. There can be 0 to 65535
        Record IDs for every User ID. The LAS specification manages its own
        Record IDs (User IDs owned by the specification), otherwise Record IDs
        will be managed by the owner of the given User ID. Thus each User ID
        is allowed to assign 0 to 65535 Record IDs in any manner they desire.
        Publicizing the meaning of a given Record ID is left to the owner of
        the given User ID. Unknown User ID/Record ID combinations should be
        ignore
    """
    recordid = property(get_recordid, set_recordid, None, doc)

    def get_reserved(self):
        return core.las.LASVLR_GetReserved(self.handle)

    def set_reserved(self, value):
        return core.las.LASVLR_SetReserved(self.handle, value)
    doc = """Reserved value for the VLR.  Currently unused."""
    reserved = property(get_reserved, set_reserved, None, doc)

    def get_data(self):
        length = self.recordlength
        data = (ctypes.c_ubyte * length)()
        core.las.LASVLR_GetData(self.handle, data)
        return data

    def set_data(self, data):
        self.recordlength = len(data)
        pdata = ctypes.cast(data, ctypes.POINTER(ctypes.c_ubyte))
        core.las.LASVLR_SetData(self.handle, pdata, self.recordlength)
    doc = """Raw data (in the form of an array of :class:`ctypes.c_ubyte`)"""
    data = property(get_data, set_data, None, doc)
