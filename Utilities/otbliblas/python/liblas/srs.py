"""
/******************************************************************************
 * $Id$
 *
 * Project:  libLAS - http://liblas.org - A BSD library for LAS format data.
 * Purpose:  Python SRS implementation
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
import vlr


class SRS(object):
    """While the ASPRS specification mandates using GeoTIFF keys for its \
    coordinate system description, these are unwieldy to use in an application.
    libLAS provides the most featureful access when by linking GDAL_.  The
    :obj:`liblas.srs.SRS` object can be set on the :obj:`liblas.header.Header`
    or it may be set on the :obj:`liblas.file.File` in various ways of
    controlling reprojection of input or output.  """
    def __init__(self, owned=True, handle=None):
        """
        :keyword owned: flag to denote whether or not the VLR owns itself
        :keyword handle: raw ctypes object


        """
        if handle:
            self.handle = handle
        else:
            self.handle = core.las.LASSRS_Create()
        self.owned = owned

    def __del__(self):
        if self.owned:
            if self.handle and core:
                core.las.LASSRS_Destroy(self.handle)

    def set_verticalcs(self, verticalCSType, citation='',
                       verticalDatum=-1,
                       verticalUnits=9001):
        """Sets the vertical coordinate system using geotiff key values. This
        operation should normally be done after setting the horizontal portion
        of the coordinate system with something like :meth:`set_wkt`,
        :meth:`set_proj4`, or :meth:`set_userinput`

        :arg verticalCSType: An EPSG vertical coordinate system code, \
        normally in the range 5600 to 5799, or -1 if one is not available.
        :keyword citation: a textual description of the vertical coordinate \
        system or an empty string if nothing is available.
        :keyword verticalDatum: the EPSG vertical datum code, often in the \
        range 5100 to 5299 - implied by verticalCSType if that is provided, \
        or -1 if no value is available.
        :keyword verticalUnits: the EPSG vertical units code, often 9001 for \
        Metre.
        """
        return core.las.LASSRS_SetVerticalCS(self.handle, verticalCSType,
                                             citation, verticalDatum,
                                             verticalUnits)

    def get_wkt(self):
        """Returns the horizontal-only WKT for the SRS"""
        return core.las.LASSRS_GetWKT(self.handle)

    def get_wkt_compoundok(self):
        """Gets the WKT for the SRS with COMP_CS elements describing the
        vertical datum and other extra information"""
        return core.las.LASSRS_GetWKT_CompoundOK(self.handle)

    def set_wkt(self, value):
        """Sets the wkt for the SRS. An exception will be thrown if the WKT is
        invalid, GDAL can't ingest it, or GDAL_ is not linked into libLAS"""
        return core.las.LASSRS_SetWKT(self.handle, value)
    doc = "WKT description of the SRS"
    wkt = property(get_wkt, set_wkt, None, doc)

    def set_userinput(self, value):
        """Sets the SRS description using GDAL_'s SetFromUserInput_ method"""
        return core.las.LASSRS_SetFromUserInput(self.handle, value)

    def get_proj4(self):
        """Returns a Proj.4_ string that describes the SRS"""
        return core.las.LASSRS_GetProj4(self.handle)

    def set_proj4(self, value):
        """Sets the SRS description with a given Proj.4_ string"""
        return core.las.LASSRS_SetProj4(self.handle, value)
    doc = "Proj.4_ description of the SRS"
    proj4 = property(get_proj4, set_proj4)

    def AddVLR(self, vlr):
        return core.las.LASSRS_AddVLR(self.handle, vlr.handle)

    def GetVLR(self, value):
        return vlr.VLR(handle=core.las.LASSRS_GetVLR(self.handle, value))

    def vlr_count(self):
        return core.las.LASSRS_GetVLRCount(self.handle)
