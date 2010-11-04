"""
/******************************************************************************
 * $Id$
 *
 * Project:  libLAS - http://liblas.org - A BSD library for LAS format data.
 * Purpose:  Python LASHeader implementation

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
import guid
import vlr
import srs
import schema


def leap_year(year):
    if (year % 400) == 0:
        return True
    elif (year % 100) == 0:
        return True
    elif (year % 4) == 0:
        return False
    return False


class Header(object):
    def __init__(self, owned=True, handle=None, copy=False):
        if handle:
            if copy:
                self.handle = core.las.LASHeader_Copy(handle)
                self.owned = True
            else:
                self.handle = handle
                self.owned = False
        else:
            self.handle = core.las.LASHeader_Create()
            self.owned = True

    def __del__(self):
        if self.owned:
            if self.handle and core:
                core.las.LASHeader_Destroy(self.handle)

    def get_filesignature(self):
        """Returns the file signature for the file. It should always be
        LASF"""
        return core.las.LASHeader_GetFileSignature(self.handle)
    doc = """The file signature for the file.  It should always be 'LASF'

    From the specification_:
        The file signature must contain the four characters "LASF", and it is
        required by the LAS specification. These four characters can be
        checked by user software as a quick look initial determination of file
        type.
    """
    file_signature = property(get_filesignature, None, None, doc)

    def get_filesourceid(self):
        return core.las.LASHeader_GetFileSourceId(self.handle)

    def set_filesourceid(self, value):
        return core.las.LASHeader_SetFileSourceId(self.handle, value)
    doc = """File Source ID for the file.

    From the specification_:
        This field should be set to a value between 1 and 65,535, inclusive. A
        value of zero (0) is interpreted to mean that an ID has not been
        assigned. In this case, processing software is free to assign any
        valid number. Note that this scheme allows a LIDAR project to contain
        up to 65,535 unique sources. A source can be considered an original
        flight line or it can be the result of merge and/or extract
        operations.
    """
    filesource_id = property(get_filesourceid, set_filesourceid, None, doc)
    file_source_id = filesource_id

    def get_global_encoding(self):
        return core.las.LASHeader_GetReserved(self.handle)

    def set_global_encoding(self, value):
        return core.las.LASHeader_SetReserved(self.handle, value)
    doc = """Global encoding for the file.

    From the specification_:

        This is a bit field used to indicate certain global properties about
        the file. In LAS 1.2 (the version in which this field was introduced),
        only the low bit is defined (this is the bit, that if set, would have
        the unsigned integer yield a value of 1). This bit field is defined
        as:

        .. csv-table:: Global Encoding - Bit Field Encoding
            :header: "Bits", "Field Name", "Description"
            :widths: 10, 20, 60

            0, "GPS Time Type", "The meaning of GPS Time in the Point Records
            0 (not set) -> GPS time in the point record fields is GPS Week
            Time (the same as previous versions of LAS) 1 (set) -> GPS Time is
            standard GPS Time (satellite GPS Time) minus 1 x 10^9 (Adjusted
            Standard GPS Time). The offset moves the time back to near zero to
            improve floating point resolution."
            1, "Waveform Data Packets Internal", "If this bit is set, the
            waveform data packets are located within this file (note that this
            bit is mutually exclusive with bit 2)"
            2, "Waveform Data Packets External", "If this bit is set, the
            waveform data packets are located external to this file in an
            auxiliary file with the same base name as this file and the
            extension \".wdp\". (note that this bit is mutually exclusive with
            bit 1)"
            3, "Return numbers have been synthetically generated", "If set,
            the point return numbers in the Point Data Records have been
            synthetically generated. This could be the case, for example, when
            a composite file is created by combining a First Return File and a
            Last Return File. In this case, first return data will be labeled
            \"1 of 2\" and second return data will be labeled \"2 of 2\""
            4:15, "Reserved", "Must be set to zero"

    """
    global_encoding = property(get_global_encoding,
                               set_global_encoding,
                               None,
                               doc)
    encoding = global_encoding

    def get_projectid(self):
        """Returns the ProjectID/GUID for the file.  \
        libLAS does not currently support setting this value from Python"""
        return core.las.LASHeader_GetProjectId(self.handle)
    doc = """ProjectID for the file.  \
        libLAS does not currently support setting this value from Python, as
        it is the same as :obj:`liblas.header.Header.guid`. Use that to
        manipulate the ProjectID for the file.

        From the specification_:
            The four fields that comprise a complete Globally Unique Identifier
            (GUID) are now reserved for use as a Project Identifier (Project
            ID). The field remains optional. The time of assignment of the
            Project ID is at the discretion of processing software. The
            Project ID should be the same for all files that are associated
            with a unique project. By assigning a Project ID and using a File
            Source ID (defined above) every file within a project and every
            point within a file can be uniquely identified, globally.

        """
    project_id = property(get_projectid, None, None, doc)

    def get_guid(self):
        """Returns the GUID for the file as a :class:`liblas.guid.GUID`
        instance"""
        return guid.GUID(handle=core.las.LASHeader_GetGUID(self.handle))

    def set_guid(self, value):
        """Sets the GUID for the file. It must be a :class:`liblas.guid.GUID`
        instance"""
        return core.las.LASHeader_SetGUID(self.handle, value.handle)
    doc = """The GUID/:obj:`liblas.header.Header.project_id` for the file."""
    guid = property(get_guid, set_guid, None, doc)

    def get_majorversion(self):
        """Returns the major version for the file. Expect this value to always
        be 1"""
        return core.las.LASHeader_GetVersionMajor(self.handle)

    def set_majorversion(self, value):
        """Sets the major version for the file. Only the value 1 is accepted
        at this time"""
        return core.las.LASHeader_SetVersionMajor(self.handle, value)
    doc = """Major version number for the file.  For all practical purposes, \
    this is always '1'"""
    major_version = property(get_majorversion, set_majorversion, None, doc)
    version_major = major_version
    major = major_version

    def get_minorversion(self):
        """Returns the minor version of the file. Expect this value to always
        be 0, 1, or 2"""
        return core.las.LASHeader_GetVersionMinor(self.handle)

    def set_minorversion(self, value):
        """Sets the minor version of the file. The value should be 0 for 1.0
        LAS files, 1 for 1.1 LAS files ..."""
        return core.las.LASHeader_SetVersionMinor(self.handle, value)
    doc = """Minor version for the file. [0, 1, 2] are currently supported."""
    minor_version = property(get_minorversion, set_minorversion, None, doc)
    version_minor = minor_version
    minor = minor_version

    def set_version(self, value):
        major, minor = value.split('.')
        self.major_version = int(major)
        self.minor_version = int(minor)

    def get_version(self):
        major = self.major_version
        minor = self.minor_version
        return '%d.%d' % (major, minor)
    doc = """The version as a dotted string for the file (ie, '1.0', '1.1',
    etc)"""
    version = property(get_version, set_version, None, doc)

    def get_systemid(self):
        """Returns the system identifier specified in the file"""
        return core.las.LASHeader_GetSystemId(self.handle)

    def set_systemid(self, value):
        """Sets the system identifier. The value is truncated to 31
        characters"""
        return core.las.LASHeader_SetSystemId(self.handle, value[0:31])
    doc = """The system identifier. The value is truncated to 31 characters and
            defaults to 'libLAS'

        :arg value: a string that will automatically be truncated to 31 \
        characters
        :type value: string

        From the specification_:

            .. csv-table:: System Identifier
                :header: "Generating Agent", "System ID"
                :widths: 40, 60

                "Hardware system", "String identifying hardware (e.g. \"ALTM
                1210\" or \"ALS50\""
                "Merge of one or more files","MERGE"
                "Modification of a single file","MODIFICATION"
                "Extraction from one or more files", "EXTRACTION"
                "Reprojection, rescaling, warping, etc.","TRANSFORMATION"
                "Some other operation","\"OTHER\" or a string up to 32
                characters identifying the operation"

        >>> h.system_id
        ''
        >>> h.system_id = 'MODIFICATION'
        >>> h.system_id
        'MODIFICATION'
    """
    system_id = property(get_systemid, set_systemid, None, doc)

    def get_softwareid(self):
        """Returns the software identifier specified in the file"""
        return core.las.LASHeader_GetSoftwareId(self.handle)

    def set_softwareid(self, value):
        """Sets the software identifier.
        """
        return core.las.LASHeader_SetSoftwareId(self.handle, value[0:31])
    doc = """The software identifier. The value is truncated to 31 characters
    and defaults to 'libLAS 1.LASVERSION' (ie, libLAS 1.6 for the 1.6
    release)

        :arg value: a string that will automatically be truncated to 31 \
        characters
        :type value: string

        From the specification_:
            This information is ASCII data describing the generating software
            itself. This field provides a mechanism for specifying which
            generating software package and version was used during LAS file
            creation (e.g. "TerraScan V-10.8", "REALM V-4.2" and etc.). If the
            character data is less than 32 characters, the remaining data must
            be null.

        >>> h.software_id
        'libLAS 1.0'
        >>> h.software_id = 'hobu'
        >>> h.software_id
        'hobu'
        >>> h.software_id = 'hobu'*9
        >>> h.software_id
        'hobuhobuhobuhobuhobuhobuhobuhob'
        """
    software_id = property(get_softwareid, set_softwareid, None, doc)

    def get_date(self):
        """Return the header's date as a datetime.datetime. If no date is set
        in the header, None is returned.

        Note that dates in LAS headers are not transitive because the header
        only stores the year and the day number.
        """
        day = core.las.LASHeader_GetCreationDOY(self.handle)
        year = core.las.LASHeader_GetCreationYear(self.handle)
        if year == 0 and day == 0:
            return None
        if not leap_year(year):
            return datetime.datetime(year, 1, 1) + datetime.timedelta(day)
        else:
            return datetime.datetime(year, 1, 1) + datetime.timedelta(day - 1)

    def set_date(self, value=datetime.datetime.now()):
        """Set the header's date from a datetime.datetime instance.
        """
        delta = value - datetime.datetime(value.year, 1, 1)
        if not leap_year(value.year):
            core.las.LASHeader_SetCreationDOY(self.handle, delta.days)
        else:
            core.las.LASHeader_SetCreationDOY(self.handle, delta.days + 1)
        core.las.LASHeader_SetCreationYear(self.handle, value.year)

    doc = """The header's date from a :class:`datetime.datetime` instance.

        :arg value: :class:`datetime.datetime` instance or none to use the \
        current time


        >>> t = datetime.datetime(2008,3,19)
        >>> hdr.date = t
        >>> hdr.date
        datetime.datetime(2008, 3, 19, 0, 0)

        .. note::
            LAS files do not support storing full datetimes in their headers,
            only the year and the day number. The conversion is handled for
            you if you use :class:`datetime.datetime` instances, however.
        """
    date = property(get_date, set_date, None, doc)

    def get_headersize(self):
        """Return the size of the header block of the LAS file in bytes.

        Should not be needed in Python land
        """
        return core.las.LASHeader_GetHeaderSize(self.handle)
    doc = """The number of bytes that the header contains. For libLAS, this is
    always 227, and it is not configurable."""
    header_size = property(get_headersize, None, None, doc)
    header_length = header_size

    def get_dataoffset(self):
        """Returns the location in bytes of where the data block of the LAS
        file starts"""
        return core.las.LASHeader_GetDataOffset(self.handle)

    def set_dataoffset(self, value):
        """Sets the data offset

        Any space between this value and the end of the VLRs will be written
        with 0's
        """
        return core.las.LASHeader_SetDataOffset(self.handle, value)
    doc = """The number of bytes of offset between the end of the header and
    the start of the point data in the file. Set this to a large value if you
    plan to include many :obj:`liblas.vlr.VLR`'s to the file.

    .. note::
        libLAS will manage this value for you as you add or remove VLRs or
        :class:`liblas.srs.SRS` instances to the header. Make sure to adjust
        your header information before opening a new file to write, as that is
        when the header's VLRs are written to the file, and they cannot be
        adjusted after that.
    """
    data_offset = property(get_dataoffset, set_dataoffset, None, doc)

    def get_recordscount(self):
        return core.las.LASHeader_GetRecordsCount(self.handle)
    doc = """Returns the number of user-defined header records in the header.
    libLAS will manage this value you for you as you add new
    :class:`liblas.srs.SRS` or :class:`liblas.vlr.VLR` instances to the
    header.
    """
    records_count = property(get_recordscount, None, None, doc)
    num_vlrs = records_count

    def get_dataformatid(self):
        """The point format value as an integer
        """
        return core.las.LASHeader_GetDataFormatId(self.handle)

    def set_dataformatid(self, value):
        if value not in [3, 2, 1, 0]:
            raise core.LASException("Format ID must be 3, 2, 1, or 0")
        return core.las.LASHeader_SetDataFormatId(self.handle, value)
    doc = """The point format as an integer. See the specification_ for more
    detail.

        It can be 3, 2, 1, or 0.

        .. note::
            Use :obj:`liblas.schema.Schema` and set the
            :obj:`liblas.header.Header.schema` object instead of using the
            dataformat_id directly. Otherwise, you will have to account for
            the :obj:`liblas.header.Header.data_record_length`.
        """
    dataformat_id = property(get_dataformatid, set_dataformatid, None, doc)
    data_format_id = dataformat_id

    def get_datarecordlength(self):
        return core.las.LASHeader_GetDataRecordLength(self.handle)

    doc = """The length in bytes of the point format. Use
    :class:`liblas.schema.Schema` and \ the :obj:`liblas.header.Header.schema`
    instead of manipulating this directly"""
    data_record_length = property(get_datarecordlength,
                                  None,
                                  None,
                                  doc)

    def get_schema(self):
        return schema.Schema(handle=core.las.LASHeader_GetSchema(
        self.handle))

    def set_schema(self, value):
        return core.las.LASHeader_SetSchema(self.handle, value.handle)
    doc = """The :class:`liblas.schmea.Schema` for this file

    Use the schema to set whether or not color or time should be stored
    on the points:

    >>> h = liblas.header.Header()
    >>> f = liblas.schema.Schema()
    >>> f.time = True
    >>> f.color = True
    >>> h.schema = f
    >>> h.schema.color
    True
    >>> h.schema.time
    True
    >>> h.schema.size
    34
    >>> h.data_record_length
    34

    The following example demonstrates how to make a point schema that does
    not store color or time, but also provides 22 extra bytes to store
    :obj:`liblas.point.Point.data`

    >>> h = liblas.header.Header()
    >>> h.schema
    <liblas.schema.Schema object at 0x100779f90>
    >>> h.schema.time
    False
    >>> h.schema.color
    False
    >>> h.schema.size
    20
    >>> f = h.schema
    >>> f.size = 42
    >>> f.size
    42
    >>> f.color
    False
    >>> f.time
    False
    >>> h.schema = f
    >>> h.schema.size
    42
    """
    schema = property(get_schema, set_schema, None, doc)

    def get_pointrecordscount(self):
        """Returns the expected number of point records in the file.

        .. note::
            This value can be grossly out of sync with the actual number of
            points in the file, because some some softwares are not careful to
            keep it up-to-date. If libLAS detects a case where it is not
            properly written, an exception will be thrown.
        """
        return core.las.LASHeader_GetPointRecordsCount(self.handle)

    def set_pointrecordscount(self, value):
        """Sets the number of point records expected in the file.

        .. note::
            Don't use this unless you have a damn good reason to. As you write
            points to a file, libLAS is going to keep this up-to-date for you
            and write it back into the header of the file once the file is
            closed after writing data.
        """
        return core.las.LASHeader_SetPointRecordsCount(self.handle, value)
    set_count = set_pointrecordscount
    get_count = get_pointrecordscount
    point_records_count = property(get_pointrecordscount,
                                   set_pointrecordscount)
    count = property(get_count, set_count)

    __len__ = get_pointrecordscount

    def get_pointrecordsbyreturncount(self):
        """Gets the histogram of point records by return number for returns
        0...8

        >>> h.point_return_count
        [0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L]

        """
        output = []
        for i in range(8):
            output.append(core.las.LASHeader_GetPointRecordsByReturnCount(
                          self.handle, i))
        return output

    def set_pointrecordsbyreturncount(self, value):
        """Sets the histogram of point records by return number from a list of
        returns 0..8

        >>> l = [1341235L, 3412341222L, 0L, 0L, 4321L, 0L, 0L, 0L]
        >>> h.point_return_count = l
        >>> h.point_return_count
        [1341235L, 3412341222L, 0L, 0L, 4321L, 0L, 0L, 0L]

        """
        for i in value[0:7]:
            core.las.LASHeader_SetPointRecordsByReturnCount(self.handle,
                                                            value.index(i), i)
    doc = """The histogram of point records by return number for returns 0...8

        .. note::
            libLAS does not manage these values automatically for you. You
            must cumulate and generate the histogram manually if you wish to
            keep these data up-to-date with what actually exists in the file.

        >>> hdr.point_return_count
        [0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L]
        >>> l = [1341235L, 3412341222L, 0L, 0L, 4321L, 0L, 0L, 0L]
        >>> hdr.point_return_count = l
        >>> hdr.point_return_count
        [1341235L, 3412341222L, 0L, 0L, 4321L, 0L, 0L, 0L]


    """
    point_return_count = property(get_pointrecordsbyreturncount,
                                  set_pointrecordsbyreturncount,
                                  None,
                                  doc)
    return_count = point_return_count

    def get_scale(self):
        """Gets the scale factors in [x, y, z] for the point data.

        .. note::
            libLAS uses this header value when reading/writing raw point data
            to the file. If you change it in the middle of writing data,
            expect the unexpected.

        >>> h.scale
        [0.01, 0.01, 0.01]
        """
        x = core.las.LASHeader_GetScaleX(self.handle)
        y = core.las.LASHeader_GetScaleY(self.handle)
        z = core.las.LASHeader_GetScaleZ(self.handle)
        return [x, y, z]

    def set_scale(self, value):
        """Sets the scale factors in [x, y, z] for the point data.

        >>> h.scale = [0.5, 0.5, 0.001]
        >>> h.scale
        [0.5, 0.5, 0.001]
        """
        return core.las.LASHeader_SetScale(self.handle,
                                           value[0],
                                           value[1],
                                           value[2])
    doc = """The scale factors in [x, y, z] for the point data.  libLAS uses \
        the scale factors plus the :obj:`liblas.header.Header.offset` values
        to store the point coordinates as integers in the file.

        From the specification_:
            The scale factor fields contain a double floating point value that
            is used to scale the corresponding X, Y, and Z long values within
            the point records. The corresponding X, Y, and Z scale factor must
            be multiplied by the X, Y, or Z point record value to get the
            actual X, Y, or Z coordinate. For example, if the X, Y, and Z
            coordinates are intended to have two decimal point values, then
            each scale factor will contain the number 0.01

        Coordinates are calculated using the following formula(s):
            * x = (x_int * x_scale) + x_offset
            * y = (y_int * y_scale) + y_offset
            * z = (z_int * z_scale) + z_offset

        .. note::
            libLAS uses this header value when reading/writing raw point data
            to the file. If you change it in the middle of writing data,
            expect the unexpected.


        >>> hdr.scale
        [0.01, 0.01, 0.01]
        >>> hdr.scale = [0.5, 0.5, 0.001]
        >>> hdr.scale
        [0.5, 0.5, 0.001]
    """
    scale = property(get_scale, set_scale, None, doc)

    def get_offset(self):
        """Gets the offset factors in [x, y, z] for the point data.
        """
        x = core.las.LASHeader_GetOffsetX(self.handle)
        y = core.las.LASHeader_GetOffsetY(self.handle)
        z = core.las.LASHeader_GetOffsetZ(self.handle)
        return [x, y, z]

    def set_offset(self, value):
        """Sets the offset factors in [x, y, z] for the point data.
        """
        return core.las.LASHeader_SetOffset(self.handle,
                                            value[0],
                                            value[1],
                                            value[2])
    doc = """The offset factors in [x, y, z] for the point data.

        From the specification_:
            The offset fields should be used to set the overall offset for the
            point records. In general these numbers will be zero, but for
            certain cases the resolution of the point data may not be large
            enough for a given projection system. However, it should always be
            assumed that these numbers are used. So to scale a given X from
            the point record, take the point record X multiplied by the X
            scale factor, and then add the X offset.

        Coordinates are calculated using the following formula(s):
            * x = (x_int * x_scale) + x_offset
            * y = (y_int * y_scale) + y_offset
            * z = (z_int * z_scale) + z_offset

        .. note::
            libLAS uses this header value when reading/writing raw point data
            to the file. If you change it in the middle of writing data,
            expect the unexpected.

        >>> hdr.offset
        [0.0, 0.0, 0.0]
        >>> hdr.offset = [32, 32, 256]
        >>> hdr.offset
        [32.0, 32.0, 256.0]

    """
    offset = property(get_offset, set_offset, None, doc)

    def get_min(self):
        """Gets the minimum values of [x, y, z] for the data.
        """
        x = core.las.LASHeader_GetMinX(self.handle)
        y = core.las.LASHeader_GetMinY(self.handle)
        z = core.las.LASHeader_GetMinZ(self.handle)
        return [x, y, z]

    def set_min(self, value):
        """Sets the minimum values of [x, y, z] for the data.

        """
        return core.las.LASHeader_SetMin(self.handle,
                                         value[0],
                                         value[1],
                                         value[2])
    doc = """The minimum values of [x, y, z] for the data in the file.

        .. note::
            libLAS does not manage these values automatically for you. You
            must cumulate and generate the histogram manually if you wish to
            keep these data up-to-date with what actually exists in the file.

        >>> hdr.min
        [0.0, 0.0, 0.0]
        >>> hdr.min = [33452344.2333, 523442.344, -90.993]
        >>> hdr.min
        [33452344.2333, 523442.34399999998, -90.992999999999995]

    """
    min = property(get_min, set_min, None, doc)

    def get_max(self):
        x = core.las.LASHeader_GetMaxX(self.handle)
        y = core.las.LASHeader_GetMaxY(self.handle)
        z = core.las.LASHeader_GetMaxZ(self.handle)
        return [x, y, z]

    def set_max(self, value):
        """Sets the maximum values of [x, y, z] for the data.
        """
        return core.las.LASHeader_SetMax(self.handle,
                                         value[0],
                                         value[1],
                                         value[2])
    doc = """The maximum values of [x, y, z] for the data in the file.

        .. note::
            libLAS does not manage these values automatically for you. You
            must cumulate and generate the histogram manually if you wish to
            keep these data up-to-date with what actually exists in the file.

        >>> hdr.max
        [0.0, 0.0, 0.0]
        >>> hdr.max = [33452344.2333, 523442.344, -90.993]
        >>> hdr.max
        [33452344.2333, 523442.34399999998, -90.992999999999995]
    """
    max = property(get_max, set_max, None, doc)

    def add_vlr(self, value):
        return core.las.LASHeader_AddVLR(self.handle, value.handle)

    def get_vlr(self, value):
        return vlr.VLR(handle=core.las.LASHeader_GetVLR(self.handle, value))
    GetVLR = get_vlr

    def delete_vlr(self, value):
        core.las.LASHeader_DeleteVLR(self.handle, value)
    DeleteVLR = delete_vlr

    def get_vlrs(self):
        output = []
        for i in range(self.num_vlrs):
            output.append(self.get_vlr(i))
        return output

    def set_vlrs(self, value):
        for i in value:
            self.add_vlr(i)
    doc = """Get/set the :class:`liblas.vlr.VLR`'s for the header as a list

    .. note::
        Existing VLRs are left untouched, and if you are wishing to overwrite
        existing data, you must first delete them from the header using
        :obj:`liblas.header.header.delete_vlr`
    """
    vlrs = property(get_vlrs, set_vlrs, None, doc)

    def get_srs(self):
        return srs.SRS(handle=core.las.LASHeader_GetSRS(self.handle))

    def set_srs(self, value):
        return core.las.LASHeader_SetSRS(self.handle, value.handle)
    srs = property(get_srs, set_srs)
