"""
/******************************************************************************
 * $Id$
 *
 * Project:  libLAS - http://liblas.org - A BSD library for LAS format data.
 * Purpose:  Python Point implementation
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
import datetime
import time
import math
import color
import ctypes


class Point(object):
    def __init__(self, owned=True, handle=None, copy=False):
        """Instantiates a :obj:`liblas.point.Point` object.  If you are \
        creating and working with your own instances, you should normally \
        not need to use any of the parameters.

        :param owned: boolean
            A flag to denote whether or not the point owns itself and can
            destroy itself upon going out of scope. There are instances where
            :obj:`liblas.file.File` may actually own the point as it is read
            from or written to files.

        :param handle: ctypes object reference
            A handle to an existing ctypes LASPointH object reference. Only
            internal liblas code should have to worry about using this.

        :param copy: boolean
            A flag to denote whether or not to copy the point upon creation.
            Used in coordination with the :obj:`handle` parameter, and it
            should not normally be used.

        :rtype: :obj:`liblas.point.Point` object
        """
        if handle:
            if copy:
                self.handle = core.las.LASPoint_Copy(handle)
                self._owned = True
            else:
                self.handle = handle
                self._owned = False
        else:
            self.handle = core.las.LASPoint_Create()
            self._owned = True

    def __del__(self):
        if self._owned:
            if self.handle and core:
                core.las.LASPoint_Destroy(self.handle)

    def get_x(self):
        return core.las.LASPoint_GetX(self.handle)

    def set_x(self, value):
        """Sets the X coordinate of the LAS point to a floating point value."""
        return core.las.LASPoint_SetX(self.handle, value)

    doc = """X coordinate of the LAS point.

    .. note::
        You are expected to properly de-scale the point according to the
        offset and the X scale if it is a free-standing point. When points are
        written to a LAS file, they will be scaled according to the header
        parameters
    """

    x = property(get_x, set_x, None, doc)

    def get_y(self):
        return core.las.LASPoint_GetY(self.handle)

    def set_y(self, value):
        """Sets the Y coordinate of the LAS point to a floating point
        value."""
        return core.las.LASPoint_SetY(self.handle, value)

    doc = """Y coordinate of the LAS point.

    .. note::
        You are expected to properly de-scale the point according to the
        offset and the X scale if it is a free-standing point. When points are
        written to a LAS file, they will be scaled according to the header
        parameters
    """
    y = property(get_y, set_y, None, doc)

    def get_z(self):
        return core.las.LASPoint_GetZ(self.handle)

    def set_z(self, value):
        """Sets the Z coordinate of the LAS point to a floating point value."""
        return core.las.LASPoint_SetZ(self.handle, value)

    doc = """Z coordinate of the LAS point.

    .. note::
        You are expected to properly de-scale the point according to the
        offset and the X scale if it is a free-standing point. When points are
        written to a LAS file, they will be scaled according to the header
        parameters
    """
    z = property(get_z, set_z, None, doc)

    def get_return_number(self):
        """Returns the return number of the point"""
        return core.las.LASPoint_GetReturnNumber(self.handle)

    def set_return_number(self, value):
        """Sets the return number of the point to an integer"""
        core.las.LASPoint_SetReturnNumber(self.handle, value)
    doc = """The pulse return number for a given laser pulse.

    From the specification_:

        The Return Number is the pulse return number for a given output pulse.
        A given output laser pulse can have many returns, and they must be
        marked in sequence of return. The first return will have a Return
        Number of one, the second a Return Number of two, and so on up to five
        returns.
    """
    return_number = property(get_return_number, set_return_number, None, doc)

    def get_number_of_returns(self):
        """Returns the number of returns for the point"""
        return core.las.LASPoint_GetNumberOfReturns(self.handle)

    def set_number_of_returns(self, value):
        """Sets the number of returns for the point"""
        core.las.LASPoint_SetNumberOfReturns(self.handle, value)
    doc = """The number of returns for a given laser pulse.

    From the specification_:

        The Number of Returns is the total number of returns for a given
        pulse. For example, a laser data point may be return two (Return
        Number) within a total number of five returns.
    """
    number_of_returns = property(get_number_of_returns,
                                 set_number_of_returns,
                                 None,
                                 doc)

    def get_scan_direction(self):
        """Returns the scan direction for the point"""
        return core.las.LASPoint_GetScanDirection(self.handle)

    def set_scan_direction(self, value):
        """Sets the scan direction as an integer for the point"""
        core.las.LASPoint_SetScanDirection(self.handle, value)
    doc = """Scan direction for the point

    From the specification_:

        The Scan Direction Flag denotes the direction at which the scanner
        mirror was traveling at the time of the output pulse. A bit value of 1
        is a positive scan direction, and a bit value of 0 is a negative scan
        direction (where positive scan direction is a scan moving from the
        left side of the in-track direction to the right side and negative the
        opposite).

    """
    scan_direction = property(get_scan_direction,
                              set_scan_direction,
                              None,
                              doc)

    def get_flightline_edge(self):
        """Denotes whether the point is a flight line edge"""
        return core.las.LASPoint_GetFlightLineEdge(self.handle)

    def set_flightline_edge(self, value):
        """Sets the flightline edge as an integer for the point. Must be 0
        (not an edge) or 1 (an edge)"""
        core.las.LASPoint_SetFlightLineEdge(self.handle, value)
    doc = """Flightline edge flag for the point

    From the specification_:

        The Edge of Flight Line data bit has a value of 1 only when the point
        is at the end of a scan. It is the last point on a given scan line
        before it changes direction.
    """
    flightline_edge = property(get_flightline_edge,
                               set_flightline_edge,
                               None,
                               doc)

    def get_scan_flags(self):
        """Returns the raw scan flags for the point.
        See the LAS 1.0 or 1.1 specification for information how to interpret
        """
        return core.las.LASPoint_GetScanFlags(self.handle)

    def set_scan_flags(self, value):
        """Sets the raw scan flags for the point. See the LAS 1.0 or 1.1
        specification for information how to interpret or use the convenience
        functions like flightline_edge, scan_direction, etc.
        """
        core.las.LASPoint_SetScanFlags(self.handle, value)
    doc = """Scan flags for the point. This is a combination of the
    :obj:`flightline_edge` \ :obj:`return_number` :obj:`number_of_returns` and
    :obj:`scan_direction`. Use the individual methods for setting these values
    for convenience.
    """
    scan_flags = property(get_scan_flags, set_scan_flags, None, doc)

    def get_classification(self):
        return core.las.LASPoint_GetClassification(self.handle)

    def set_classification(self, value):
        core.las.LASPoint_SetClassification(self.handle, value)
    doc = """The point's classification as a raw byte value.

        The following large section of information is verboten from the
        specification_ for your convenience:

        This filed represents the "class" attributes of a point. If a point
        has never been classified, this byte must be set to zero. There are no
        user defined classes since all point formats 0 supply 8 bits per point
        for user defined operations.

        .. csv-table:: Classification Bit Field Encoding
            :header: "Bits", "Field Name", "Description"
            :widths: 5, 15, 60

            0:4, "Classification", "Standard ASPRS classification as defined
            in the following classification table."
            5, "Synthetic", "If set then this point was created by a technique
            other than LIDAR collection such as digitized from a
            photogrammetric stereo model or by traversing a waveform."
            6, "Key-point", "If set, this point is considered to be a model
            key- point and thus generally should not be withheld in a thinning
            algorithm."
            7, "Witheld", "If set, this point should not be included in
            processing (synonymous with Deleted)."

        .. note::
            Bits 5, 6 and 7 are treated as flags and can be set or clear in
            any combination. For example, a point with bits 5 and 6 both set
            to one and the lower five bits set to 2 (see table below) would be
            a ground point that had been Synthetically collected and marked as
            a model key-point.

        .. csv-table:: ASPRS Standard LiDAR Point Classes
            :header: "Classification", "Meaning"
            :widths: 20, 60

            0, "Created, never classified"
            1, "Unclassified"
            2, "Ground"
            3, "Low Vegetation"
            4, "Medium Vegetation"
            5, "High Vegetation"
            6, "Building"
            7, "Low Point (noise)"
            8, "Model Key-point (mass point)"
            9, "Water"
            10, "Reserved for ASPRS Definition"
            11, "Reserved for ASPRS Definition"
            12, "Overlap Points"
            13-31, "Reserved for ASPRS Definition"

        .. note::
            We are using both 0 and 1 as Unclassified to maintain
            compatibility with current popular classification software such as
            TerraScan. We extend the idea of classification value 1 to include
            cases in which data have been subjected to a classification
            algorithm but emerged in an undefined state. For example, data
            with class 0 is sent through an algorithm to detect man-made
            structures -- points that emerge without having been assigned as
            belonging to structures could be remapped from class 0 to class 1

        .. note::
            Overlap Points are those points that were immediately culled
            during the merging of overlapping flight lines. In general, the
            Withheld bit should be set since these points are not subsequently
            classified.

    """
    classification = property(get_classification,
                              set_classification,
                              None,
                              doc)

    def get_scan_angle_rank(self):
        """Returns the scan angle rank of the point. It will be between -90
        and 90"""
        return core.las.LASPoint_GetScanAngleRank(self.handle)

    def set_scan_angle_rank(self, value):
        """Sets the scan angle rank of the point. It must be between -90 and
        90"""
        core.las.LASPoint_SetScanAngleRank(self.handle, value)

    doc = """Scan angle of the point.

    From the specification_:
        The Scan Angle Rank is a signed one-byte number with a valid range
        from -90 to +90. The Scan Angle Rank is the angle (rounded to the
        nearest integer in the absolute value sense) at which the laser point
        was output from the laser system including the roll of the aircraft.
        The scan angle is within 1 degree of accuracy from +90 to -90 degrees.
        The scan angle is an angle based on 0 degrees being nadir, and -90
        degrees to the left side of the aircraft in the direction of flight.
    """
    scan_angle = property(get_scan_angle_rank, set_scan_angle_rank, None, doc)

    def get_user_data(self):
        return core.las.LASPoint_GetUserData(self.handle)

    def set_user_data(self, value):
        core.las.LASPoint_SetUserData(self.handle, value)
    doc = """User Data for the point.  This is a single byte of data and \
    and can be anything the software wants to attach to the point"""
    user_data = property(get_user_data, set_user_data, None, doc)

    def get_point_source_id(self):
        return core.las.LASPoint_GetPointSourceId(self.handle)

    def set_point_source_id(self, value):
        core.las.LASPoint_SetUserData(self.handle, value)
    doc = """Point Source ID for the point.

    From the specification_:

        This value indicates the file from which this point originated. Valid
        values for this field are 1 to 65,535 inclusive with zero being used
        for a special case discussed below. The numerical value corresponds to
        the File Source ID from which this point originated. Zero is reserved
        as a convenience to system implementers. A Point Source ID of zero
        implies that this point originated in this file. This implies that
        processing software should set the Point Source ID equal to the File
        Source ID of the file containing this point at some time during
        processing.
        """
    point_source_id = property(get_user_data, set_user_data, None, doc)

    def get_intensity(self):
        return core.las.LASPoint_GetIntensity(self.handle)

    def set_intensity(self, value):
        core.las.LASPoint_SetIntensity(self.handle, value)
    doc = """Intensity value as an short integer for the point

    From the specification_:

        The intensity value is the integer representation of the pulse return
        magnitude. This value is optional and system specific. However, it
        should always be included if available.
    """
    intensity = property(get_intensity, set_intensity, None, doc)

    def get_time(self):
        t = core.las.LASPoint_GetTime(self.handle)
        floor = math.floor(t)
        ms = float(t) - floor

        # clip to 999999
        ms = int(round(ms * 1000000))
        if ms > 999999:
            ms = 999999

        lt = time.gmtime(t)
        value = datetime.datetime(lt[0], lt[1], lt[2],
                                  lt[3], lt[4], lt[5], ms)
        return value

    def set_time(self, value):
        t = time.mktime(value.timetuple())

        ms = value.microsecond
        t = float(t) + ms * 0.000001
        core.las.LASPoint_SetTime(self.handle, t)

    doc = """Interpeted (:class:`datetime.datetime` instance) time value for
    the point.

        Example:

        >>> td = datetime.timedelta(hours=6) # my timezone is -6
        >>> t = datetime.datetime(2008,3,19) - td
        >>> p.time = t
        >>> p.time
        datetime.datetime(2008, 3, 19, 0, 0)

        .. note::
            Because no header is available, no coordination is done with
            respect to the allowable LAS time types. You must coordinate with
            the :obj:`liblas.header.Header.global_encoding` to determine the
            actual time value for the point. If you want to merely preserve
            existing point data, use :obj:`liblas.point.Point.raw_time`

        """
    time = property(get_time, set_time, None, doc)

    def get_raw_time(self):
        t = core.las.LASPoint_GetTime(self.handle)
        return t

    def set_raw_time(self, value):
        core.las.LASPoint_SetTime(self.handle, value)

    doc = """Uninterpeted time value for the point.

        .. note::
            Because no header is available, no coordination is done with
            respect to the allowable LAS time types. You must coordinate with
            the :obj:`liblas.header.Header.global_encoding` to determine the
            actual time value for the point. See the ASPRS LAS 1.1-1.3
            specifications for more detail.
        """
    raw_time = property(get_raw_time, set_raw_time, None, doc)

    def get_color(self):
        return color.Color(handle=core.las.LASPoint_GetColor(self.handle))

    def set_color(self, value):
        return core.las.LASPoint_SetColor(self.handle, value.handle)
    doc = """Raw color value for the point as an :obj:`liblas.color.Color`
    instance.

    .. note::
        RGB values should always be normalized to 16 bit values. For example,
        when encoding an 8 bit per channel pixel, multiply each channel value
        by 256 prior to storage in these fields. This normalization allows
        color values from different camera bit depths to be accurately merged.

        """
    color = property(get_color, set_color, None, doc)

    # def descale(self, header):
    #     """Descales the point with a given :obj:`liblas.header.Header` instance
    # 
    #     :param header: A header with :obj:`liblas.header.Header.offset` and \
    #                    :obj:`liblas.header.Header.scale` values set"""
    #     self.x = (self.x - header.offset[0]) / header.scale[0]
    #     self.y = (self.y - header.offset[1]) / header.scale[1]
    #     self.z = (self.z - header.offset[2]) / header.scale[2]
    # 
    # def scale(self, header):
    #     """Scales the point with a given :obj:`liblas.header.Header` instance
    # 
    #     :param header: A header with :obj:`liblas.header.Header.offset` and \
    #                    :obj:`liblas.header.Header.scale` values set"""
    #     self.x = (self.x * header.scale[0]) + header.offset[0]
    #     self.y = (self.y * header.scale[1]) + header.offset[1]
    #     self.z = (self.z * header.scale[2]) + header.offset[2]

    # def get_data(self):
    #     l = ctypes.pointer(ctypes.c_int())
    #     d = ctypes.pointer(ctypes.c_ubyte())
    #     core.las.LASPoint_GetExtraData(self.handle, ctypes.byref(d), l)
    # 
    #     d2 = ctypes.cast(d, ctypes.POINTER(ctypes.c_ubyte * l.contents.value))
    #     s = (ctypes.c_ubyte * l.contents.value)()
    #     for i in range(l.contents.value):
    #         s[i] = d2.contents[i]
    #     p_d = ctypes.cast(d, ctypes.POINTER(ctypes.c_char_p))
    #     core.las.LASString_Free(p_d)
    # 
    #     return s
    # 
    # def set_data(self, data):
    #     d = ctypes.cast(data, ctypes.POINTER(ctypes.c_ubyte))
    # 
    #     core.las.LASPoint_SetExtraData(self.handle, d, len(data))
    # 
    # doc = """Extra byte data for the point. You can attach a variable amount
    # of extra data to each individual point by setting the
    # :obj:`liblas.header.Header.dataformat_id` to a valid value, and then
    # setting the :obj:`liblas.header.Header.data_record_length` to a value that
    # is larger than the nominal length for that format as noted in the
    # specification_.
    # 
    # Interpreting the data is up to the user, however. Because these are raw
    # bytes, you can store any additional data that you wish.
    # 
    # .. note::
    #     The data will be clipped to exactly the
    #     :obj:`liblas.header.Header.data_record_length` when they are actually
    #     written to the file. If you set data on the point that is larger than
    #     the difference between the nominal point format data record length and
    #     the data record length you set in the header, it will be thrown away.
    # 
    # """
    # data = property(get_data, set_data, None, doc)
