"""
/******************************************************************************
 * $Id$
 *
 * Project:  libLAS - http://liblas.org - A BSD library for LAS format data.
 * Purpose:  Python ctypes function calls
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

import atexit
import os
import re
import sys
import ctypes
from ctypes.util import find_library

from ctypes import PyDLL


class LASException(Exception):
    "libLAS exception, indicates a libLAS-related error."
    pass


def check_return(result, func, cargs):
    "Error checking for LASError calls"
    if result != 0:
        msg = 'LASError in "%s": %s' % (func.__name__,
                                        las.LASError_GetLastErrorMsg())
        las.LASError_Reset()
        raise LASException(msg)
    return True


def check_void(result, func, cargs):
    "Error checking for void* returns"
    if not bool(result):
        msg = 'LASError in "%s": %s' % (func.__name__,
                                        las.LASError_GetLastErrorMsg())
        las.LASError_Reset()
        raise LASException(msg)
    return result


def check_void_done(result, func, cargs):
    "Error checking for void* returns that might be empty with no error"
    if las.LASError_GetErrorCount():
        msg = 'LASError in "%s": %s' % (func.__name__,
                                        las.LASError_GetLastErrorMsg())
        las.LASError_Reset()
        raise LASException(msg)

    return result


def check_value(result, func, cargs):
    "Error checking proper value returns"
    count = las.LASError_GetErrorCount()
    if count != 0:
        msg = 'LASError in "%s": %s' % (func.__name__,
                                        las.LASError_GetLastErrorMsg())
        las.LASError_Reset()
        raise LASException(msg)
    return result


def check_value_free(result, func, cargs):
    "Error checking proper value returns"
    count = las.LASError_GetErrorCount()
    if count != 0:
        msg = 'LASError in "%s": %s' % (func.__name__,
                                        las.LASError_GetLastErrorMsg())
        las.LASError_Reset()
        raise LASException(msg)

    retval = ctypes.string_at(result)[:]
    return retval


def free_returned_char_p(result, func, cargs):

    size = ctypes.c_int()
    retvalue = ctypes.string_at(result)
    free(result)
    return retvalue


try:
    from numpy import array, ndarray
    HAS_NUMPY = True
except ImportError:
    HAS_NUMPY = False

if os.name == 'nt':
# stolen from Shapely
# http://trac.gispython.org/projects/PCL/browser/Shapely/trunk/shapely/geos.py
    lib_name = 'liblas1.dll'
    try:
        local_dlls = os.path.abspath(os.__file__ + "../../../DLLs")
        original_path = os.environ['PATH']
        os.environ['PATH'] = "%s;%s" % (local_dlls, original_path)
        las = ctypes.CDLL(lib_name)

        def free(m):
            try:
                free = ctypes.cdll.msvcrt.free(m)
            except WindowsError:
                pass
    except (ImportError, WindowsError):
        raise


elif os.name == 'posix':
    platform = os.uname()[0]
    if platform == 'Darwin':
        lib_name = 'liblas_c.dylib'
        free = ctypes.CDLL(find_library('libc')).free
    else:
        lib_name = 'liblas_c.so'
        free = ctypes.CDLL(find_library('libc.so.6')).free
    las = ctypes.CDLL(lib_name)
else:
    raise LASException('Unsupported OS "%s"' % os.name)


def get_version():
    return las.LAS_GetVersion()

version = get_version()

las.LAS_IsGDALEnabled.restype = ctypes.c_int
las.LAS_IsLibGeoTIFFEnabled.restype = ctypes.c_int

las.LAS_GetVersion.restype = ctypes.POINTER(ctypes.c_char)
las.LAS_GetVersion.errcheck = free_returned_char_p

las.LASError_GetLastErrorNum.restype = ctypes.c_int

las.LASError_GetLastErrorMsg.restype = ctypes.POINTER(ctypes.c_char)
las.LASError_GetLastErrorMsg.errcheck = free_returned_char_p

las.LASError_GetLastErrorMethod.restype = ctypes.POINTER(ctypes.c_char)
las.LASError_GetLastErrorMethod.errcheck = free_returned_char_p

las.LASError_GetErrorCount.restype = ctypes.c_int

las.LASReader_Create.argtypes = [ctypes.c_char_p]
las.LASReader_Create.restype = ctypes.c_void_p
las.LASReader_Create.errcheck = check_void

las.LASReader_CreateWithHeader.argtypes = [ctypes.c_char_p, ctypes.c_void_p]
las.LASReader_CreateWithHeader.restype = ctypes.c_void_p
las.LASReader_CreateWithHeader.errcheck = check_void

las.LASReader_GetNextPoint.restype = ctypes.c_void_p
las.LASReader_GetNextPoint.argtypes = [ctypes.c_void_p]
las.LASReader_GetNextPoint.errcheck = check_void_done

las.LASReader_GetPointAt.restype = ctypes.c_void_p
las.LASReader_GetPointAt.argtypes = [ctypes.c_void_p, ctypes.c_ulong]
las.LASReader_GetPointAt.errcheck = check_void_done

las.LASReader_Seek.argtypes = [ctypes.c_void_p, ctypes.c_ulong]
las.LASReader_Seek.errcheck = check_return

las.LASReader_SetSRS.argtypes = [ctypes.c_void_p, ctypes.c_void_p]
las.LASReader_SetSRS.errcheck = check_return

las.LASReader_SetInputSRS.argtypes = [ctypes.c_void_p, ctypes.c_void_p]
las.LASReader_SetInputSRS.errcheck = check_return

las.LASReader_SetOutputSRS.argtypes = [ctypes.c_void_p, ctypes.c_void_p]
las.LASReader_SetOutputSRS.errcheck = check_return

las.LASReader_Destroy.argtypes = [ctypes.c_void_p]
las.LASReader_Destroy.errcheck = check_void_done
las.LASReader_Destroy.restype = None

las.LASPoint_GetX.restype = ctypes.c_double
las.LASPoint_GetX.argtypes = [ctypes.c_void_p]
las.LASPoint_GetX.errcheck = check_value
las.LASPoint_SetX.restype = ctypes.c_int
las.LASPoint_SetX.argtypes = [ctypes.c_void_p, ctypes.c_double]
las.LASPoint_SetX.errcheck = check_return

las.LASPoint_GetY.restype = ctypes.c_double
las.LASPoint_GetY.argtypes = [ctypes.c_void_p]
las.LASPoint_GetY.errcheck = check_value
las.LASPoint_SetY.restype = ctypes.c_int
las.LASPoint_SetY.argtypes = [ctypes.c_void_p, ctypes.c_double]
las.LASPoint_SetY.errcheck = check_return

las.LASPoint_GetZ.restype = ctypes.c_double
las.LASPoint_GetZ.argtypes = [ctypes.c_void_p]
las.LASPoint_GetZ.errcheck = check_value
las.LASPoint_SetZ.restype = ctypes.c_int
las.LASPoint_SetZ.argtypes = [ctypes.c_void_p, ctypes.c_double]
las.LASPoint_SetZ.errcheck = check_return

las.LASPoint_GetIntensity.restype = ctypes.c_short
las.LASPoint_GetIntensity.argtypes = [ctypes.c_void_p]
las.LASPoint_GetIntensity.errcheck = check_value
las.LASPoint_SetIntensity.restype = ctypes.c_int
las.LASPoint_SetIntensity.argtypes = [ctypes.c_void_p, ctypes.c_short]
las.LASPoint_SetIntensity.errcheck = check_return

las.LASPoint_GetReturnNumber.restype = ctypes.c_ushort
las.LASPoint_GetReturnNumber.argtypes = [ctypes.c_void_p]
las.LASPoint_GetReturnNumber.errcheck = check_value
las.LASPoint_SetReturnNumber.restype = ctypes.c_int
las.LASPoint_SetReturnNumber.argtypes = [ctypes.c_void_p, ctypes.c_ushort]
las.LASPoint_SetReturnNumber.errcheck = check_return

las.LASPoint_GetNumberOfReturns.restype = ctypes.c_ushort
las.LASPoint_GetNumberOfReturns.argtypes = [ctypes.c_void_p]
las.LASPoint_GetNumberOfReturns.errcheck = check_value
las.LASPoint_SetNumberOfReturns.restype = ctypes.c_int
las.LASPoint_SetNumberOfReturns.argtypes = [ctypes.c_void_p, ctypes.c_ushort]
las.LASPoint_SetNumberOfReturns.errcheck = check_return

las.LASPoint_GetScanDirection.restype = ctypes.c_ushort
las.LASPoint_GetScanDirection.argtypes = [ctypes.c_void_p]
las.LASPoint_GetScanDirection.errcheck = check_value
las.LASPoint_SetScanDirection.restype = ctypes.c_int
las.LASPoint_SetScanDirection.argtypes = [ctypes.c_void_p, ctypes.c_ushort]
las.LASPoint_SetScanDirection.errcheck = check_return

las.LASPoint_GetFlightLineEdge.restype = ctypes.c_ushort
las.LASPoint_GetFlightLineEdge.argtypes = [ctypes.c_void_p]
las.LASPoint_GetFlightLineEdge.errcheck = check_value
las.LASPoint_SetFlightLineEdge.restype = ctypes.c_int
las.LASPoint_SetFlightLineEdge.argtypes = [ctypes.c_void_p, ctypes.c_ushort]
las.LASPoint_SetFlightLineEdge.errcheck = check_return

las.LASPoint_GetScanFlags.restype = ctypes.c_ubyte
las.LASPoint_GetScanFlags.argtypes = [ctypes.c_void_p]
las.LASPoint_GetScanFlags.errcheck = check_value
las.LASPoint_SetScanFlags.restype = ctypes.c_int
las.LASPoint_SetScanFlags.argtypes = [ctypes.c_void_p, ctypes.c_ubyte]
las.LASPoint_SetScanFlags.errcheck = check_return

las.LASPoint_GetClassification.restype = ctypes.c_ubyte
las.LASPoint_GetClassification.argtypes = [ctypes.c_void_p]
las.LASPoint_GetClassification.errcheck = check_value
las.LASPoint_SetClassification.restype = ctypes.c_int
las.LASPoint_SetClassification.argtypes = [ctypes.c_void_p, ctypes.c_ubyte]
las.LASPoint_SetClassification.errcheck = check_return

las.LASPoint_GetTime.restype = ctypes.c_double
las.LASPoint_GetTime.argtypes = [ctypes.c_void_p]
las.LASPoint_GetTime.errcheck = check_value
las.LASPoint_SetTime.restype = ctypes.c_int
las.LASPoint_SetTime.argtypes = [ctypes.c_void_p, ctypes.c_double]
las.LASPoint_SetTime.errcheck = check_return

las.LASPoint_GetScanAngleRank.restype = ctypes.c_int8
las.LASPoint_GetScanAngleRank.argtypes = [ctypes.c_void_p]
las.LASPoint_GetScanAngleRank.errcheck = check_value
las.LASPoint_SetScanAngleRank.restype = ctypes.c_int
las.LASPoint_SetScanAngleRank.argtypes = [ctypes.c_void_p, ctypes.c_int8]
las.LASPoint_SetScanAngleRank.errcheck = check_return

las.LASPoint_GetUserData.restype = ctypes.c_ubyte
las.LASPoint_GetUserData.argtypes = [ctypes.c_void_p]
las.LASPoint_GetUserData.errcheck = check_value
las.LASPoint_SetUserData.restype = ctypes.c_int
las.LASPoint_SetUserData.argtypes = [ctypes.c_void_p, ctypes.c_ubyte]
las.LASPoint_SetUserData.errcheck = check_return

las.LASPoint_SetPointSourceId.restype = ctypes.c_short
las.LASPoint_SetPointSourceId.argtypes = [ctypes.c_void_p]
las.LASPoint_SetPointSourceId.errcheck = check_value
las.LASPoint_GetPointSourceId.restype = ctypes.c_short
las.LASPoint_GetPointSourceId.argtypes = [ctypes.c_void_p]
las.LASPoint_GetPointSourceId.errcheck = check_value

# las.LASPoint_GetExtraData.argtypes = [ctypes.c_void_p,
#                             ctypes.POINTER(ctypes.POINTER(ctypes.c_ubyte)),
#                             ctypes.POINTER(ctypes.c_int)]
# las.LASPoint_GetExtraData.errcheck = check_value
# las.LASPoint_GetExtraData.restype = ctypes.c_int
# 
# las.LASPoint_SetExtraData.argtypes = [ctypes.c_void_p,
#                                         ctypes.POINTER(ctypes.c_ubyte),
#                                         ctypes.c_int]
# las.LASPoint_SetExtraData.errcheck = check_value
# las.LASPoint_SetExtraData.restype = ctypes.c_int

las.LASPoint_Create.restype = ctypes.c_void_p
las.LASPoint_Create.errcheck = check_void

las.LASPoint_Copy.restype = ctypes.c_void_p
las.LASPoint_Copy.argtypes = [ctypes.c_void_p]
las.LASPoint_Copy.errcheck = check_void

las.LASReader_GetHeader.restype = ctypes.c_void_p
las.LASReader_GetHeader.argtypes = [ctypes.c_void_p]
las.LASReader_GetHeader.errcheck = check_void

las.LASHeader_Destroy.argtypes = [ctypes.c_void_p]
las.LASHeader_Destroy.errcheck = check_void_done
las.LASHeader_Destroy.restype = None

las.LASHeader_Copy.restype = ctypes.c_void_p
las.LASHeader_Copy.argtypes = [ctypes.c_void_p]
las.LASHeader_Copy.errcheck = check_void

las.LASHeader_Create.restype = ctypes.c_void_p
las.LASHeader_Create.errcheck = check_void

las.LASHeader_GetFileSignature.argtypes = [ctypes.c_void_p]
las.LASHeader_GetFileSignature.errcheck = check_value_free
las.LASHeader_GetFileSignature.restype = ctypes.c_char_p

las.LASHeader_GetFileSourceId.restype = ctypes.c_ushort
las.LASHeader_GetFileSourceId.argtypes = [ctypes.c_void_p]
las.LASHeader_GetFileSourceId.errcheck = check_value

las.LASHeader_SetFileSourceId.restype = ctypes.c_short
las.LASHeader_SetFileSourceId.argtypes = [ctypes.c_void_p]
las.LASHeader_SetFileSourceId.errcheck = check_value

las.LASHeader_SetReserved.restype = ctypes.c_short
las.LASHeader_SetReserved.argtypes = [ctypes.c_void_p]
las.LASHeader_SetReserved.errcheck = check_value
las.LASHeader_GetReserved.restype = ctypes.c_short
las.LASHeader_GetReserved.argtypes = [ctypes.c_void_p]
las.LASHeader_GetReserved.errcheck = check_value

las.LASHeader_GetProjectId.argtypes = [ctypes.c_void_p]
las.LASHeader_GetProjectId.errcheck = check_value_free
las.LASHeader_GetProjectId.restype = ctypes.c_char_p

las.LASHeader_GetVersionMajor.restype = ctypes.c_ubyte
las.LASHeader_GetVersionMajor.argtypes = [ctypes.c_void_p]
las.LASHeader_GetVersionMajor.errcheck = check_value
las.LASHeader_SetVersionMajor.restype = ctypes.c_int
las.LASHeader_SetVersionMajor.argtypes = [ctypes.c_void_p, ctypes.c_ubyte]
las.LASHeader_SetVersionMajor.errcheck = check_return

las.LASHeader_GetVersionMinor.restype = ctypes.c_ubyte
las.LASHeader_GetVersionMinor.argtypes = [ctypes.c_void_p]
las.LASHeader_GetVersionMinor.errcheck = check_value
las.LASHeader_SetVersionMinor.restype = ctypes.c_int
las.LASHeader_SetVersionMinor.argtypes = [ctypes.c_void_p, ctypes.c_ubyte]
las.LASHeader_SetVersionMinor.errcheck = check_return

las.LASHeader_GetSystemId.argtypes = [ctypes.c_void_p]
las.LASHeader_GetSystemId.errcheck = check_value_free
las.LASHeader_GetSystemId.restype = ctypes.c_char_p
las.LASHeader_SetSystemId.restype = ctypes.c_int
las.LASHeader_SetSystemId.argtypes = [ctypes.c_void_p, ctypes.c_char_p]
las.LASHeader_SetSystemId.errcheck = check_return

las.LASHeader_GetSoftwareId.argtypes = [ctypes.c_void_p]
las.LASHeader_GetSoftwareId.errcheck = check_value_free
las.LASHeader_GetSoftwareId.restype = ctypes.c_char_p
las.LASHeader_SetSoftwareId.restype = ctypes.c_int
las.LASHeader_SetSoftwareId.argtypes = [ctypes.c_void_p, ctypes.c_char_p]
las.LASHeader_SetSoftwareId.errcheck = check_return

las.LASHeader_GetCreationDOY.restype = ctypes.c_ushort
las.LASHeader_GetCreationDOY.argtypes = [ctypes.c_void_p]
las.LASHeader_GetCreationDOY.errcheck = check_value
las.LASHeader_SetCreationDOY.restype = ctypes.c_int
las.LASHeader_SetCreationDOY.argtypes = [ctypes.c_void_p, ctypes.c_ushort]
las.LASHeader_SetCreationDOY.errcheck = check_return

las.LASHeader_GetCreationYear.restype = ctypes.c_ushort
las.LASHeader_GetCreationYear.argtypes = [ctypes.c_void_p]
las.LASHeader_GetCreationYear.errcheck = check_value
las.LASHeader_SetCreationYear.restype = ctypes.c_int
las.LASHeader_SetCreationYear.argtypes = [ctypes.c_void_p, ctypes.c_ushort]
las.LASHeader_SetCreationYear.errcheck = check_return

las.LASHeader_GetHeaderSize.restype = ctypes.c_ushort
las.LASHeader_GetHeaderSize.argtypes = [ctypes.c_void_p]
las.LASHeader_GetHeaderSize.errcheck = check_value

las.LASHeader_GetDataOffset.restype = ctypes.c_ulong
las.LASHeader_GetDataOffset.argtypes = [ctypes.c_void_p]
las.LASHeader_GetDataOffset.errcheck = check_value

las.LASHeader_SetDataOffset.restype = ctypes.c_int
las.LASHeader_SetDataOffset.argtypes = [ctypes.c_void_p, ctypes.c_int]
las.LASHeader_SetDataOffset.errcheck = check_return

las.LASHeader_GetRecordsCount.restype = ctypes.c_ulong
las.LASHeader_GetRecordsCount.argtypes = [ctypes.c_void_p]
las.LASHeader_GetRecordsCount.errcheck = check_value

las.LASHeader_GetDataFormatId.restype = ctypes.c_ubyte
las.LASHeader_GetDataFormatId.argtypes = [ctypes.c_void_p]
las.LASHeader_GetDataFormatId.errcheck = check_value

las.LASHeader_SetDataFormatId.restype = ctypes.c_int
las.LASHeader_SetDataFormatId.argtypes = [ctypes.c_void_p, ctypes.c_int]
las.LASHeader_SetDataFormatId.errcheck = check_return

las.LASHeader_GetDataRecordLength.restype = ctypes.c_ushort
las.LASHeader_GetDataRecordLength.argtypes = [ctypes.c_void_p]
las.LASHeader_GetDataRecordLength.errcheck = check_value

las.LASHeader_GetPointRecordsCount.restype = ctypes.c_ulong
las.LASHeader_GetPointRecordsCount.argtypes = [ctypes.c_void_p]
las.LASHeader_GetPointRecordsCount.errcheck = check_value
las.LASHeader_SetPointRecordsCount.restype = ctypes.c_int
las.LASHeader_SetPointRecordsCount.argtypes = [ctypes.c_void_p, ctypes.c_ulong]
las.LASHeader_SetPointRecordsCount.errcheck = check_return

las.LASHeader_GetPointRecordsByReturnCount.restype = ctypes.c_ulong
las.LASHeader_GetPointRecordsByReturnCount.argtypes = [ctypes.c_void_p,
                                                        ctypes.c_int]
las.LASHeader_GetPointRecordsByReturnCount.errcheck = check_value
las.LASHeader_SetPointRecordsByReturnCount.restype = ctypes.c_int
las.LASHeader_SetPointRecordsByReturnCount.argtypes = [ctypes.c_void_p,
                                                        ctypes.c_int,
                                                        ctypes.c_ulong]
las.LASHeader_SetPointRecordsByReturnCount.errcheck = check_return

las.LASHeader_GetScaleX.restype = ctypes.c_double
las.LASHeader_GetScaleX.argtypes = [ctypes.c_void_p]
las.LASHeader_GetScaleX.errcheck = check_value
las.LASHeader_GetScaleY.restype = ctypes.c_double
las.LASHeader_GetScaleY.argtypes = [ctypes.c_void_p]
las.LASHeader_GetScaleY.errcheck = check_value
las.LASHeader_GetScaleZ.restype = ctypes.c_double
las.LASHeader_GetScaleZ.argtypes = [ctypes.c_void_p]
las.LASHeader_GetScaleZ.errcheck = check_value
las.LASHeader_SetScale.restype = ctypes.c_int
las.LASHeader_SetScale.argtypes = [ctypes.c_void_p,
                                    ctypes.c_double,
                                    ctypes.c_double,
                                    ctypes.c_double]
las.LASHeader_SetScale.errcheck = check_return

las.LASHeader_GetOffsetX.restype = ctypes.c_double
las.LASHeader_GetOffsetX.argtypes = [ctypes.c_void_p]
las.LASHeader_GetOffsetX.errcheck = check_value
las.LASHeader_GetOffsetY.restype = ctypes.c_double
las.LASHeader_GetOffsetY.argtypes = [ctypes.c_void_p]
las.LASHeader_GetOffsetY.errcheck = check_value
las.LASHeader_GetOffsetZ.restype = ctypes.c_double
las.LASHeader_GetOffsetZ.argtypes = [ctypes.c_void_p]
las.LASHeader_GetOffsetZ.errcheck = check_value
las.LASHeader_SetOffset.restype = ctypes.c_int
las.LASHeader_SetOffset.argtypes = [ctypes.c_void_p,
                                    ctypes.c_double,
                                    ctypes.c_double,
                                    ctypes.c_double]
las.LASHeader_SetOffset.errcheck = check_return

las.LASHeader_GetMinX.restype = ctypes.c_double
las.LASHeader_GetMinX.argtypes = [ctypes.c_void_p]
las.LASHeader_GetMinX.errcheck = check_value
las.LASHeader_GetMinY.restype = ctypes.c_double
las.LASHeader_GetMinY.argtypes = [ctypes.c_void_p]
las.LASHeader_GetMinY.errcheck = check_value
las.LASHeader_GetMinZ.restype = ctypes.c_double
las.LASHeader_GetMinZ.argtypes = [ctypes.c_void_p]
las.LASHeader_GetMinZ.errcheck = check_value
las.LASHeader_SetMin.restype = ctypes.c_int
las.LASHeader_SetMin.argtypes = [ctypes.c_void_p,
                                    ctypes.c_double,
                                    ctypes.c_double,
                                    ctypes.c_double]
las.LASHeader_SetMin.errcheck = check_return

las.LASHeader_GetMaxX.restype = ctypes.c_double
las.LASHeader_GetMaxX.argtypes = [ctypes.c_void_p]
las.LASHeader_GetMaxX.errcheck = check_value
las.LASHeader_GetMaxY.restype = ctypes.c_double
las.LASHeader_GetMaxY.argtypes = [ctypes.c_void_p]
las.LASHeader_GetMaxY.errcheck = check_value
las.LASHeader_GetMaxZ.restype = ctypes.c_double
las.LASHeader_GetMaxZ.argtypes = [ctypes.c_void_p]
las.LASHeader_GetMaxZ.errcheck = check_value
las.LASHeader_SetMax.restype = ctypes.c_int
las.LASHeader_SetMax.argtypes = [ctypes.c_void_p,
                                ctypes.c_double,
                                ctypes.c_double,
                                ctypes.c_double]
las.LASHeader_SetMax.errcheck = check_return

las.LASHeader_GetVLR.argtypes = [ctypes.c_void_p, ctypes.c_int]
las.LASHeader_GetVLR.errcheck = check_void
las.LASHeader_GetVLR.restype = ctypes.c_void_p

las.LASHeader_DeleteVLR.argtypes = [ctypes.c_void_p, ctypes.c_int]
las.LASHeader_DeleteVLR.errcheck = check_return

las.LASHeader_AddVLR.argtypes = [ctypes.c_void_p, ctypes.c_void_p]
las.LASHeader_AddVLR.errcheck = check_return
las.LASHeader_AddVLR.restype = ctypes.c_int

las.LASWriter_Create.restype = ctypes.c_void_p
las.LASWriter_Create.argtypes = [ctypes.c_char_p,
                                    ctypes.c_void_p,
                                    ctypes.c_int]
las.LASWriter_Create.errcheck = check_void

las.LASWriter_WritePoint.restype = ctypes.c_int
las.LASWriter_WritePoint.argtypes = [ctypes.c_void_p,
                                        ctypes.c_void_p]
las.LASWriter_WritePoint.errcheck = check_return

las.LASWriter_WriteHeader.restype = ctypes.c_int
las.LASWriter_WriteHeader.argtypes = [ctypes.c_void_p,
                                        ctypes.c_void_p]
las.LASWriter_WriteHeader.errcheck = check_return

las.LASWriter_SetSRS.argtypes = [ctypes.c_void_p, ctypes.c_void_p]
las.LASWriter_SetSRS.errcheck = check_return

las.LASWriter_SetInputSRS.argtypes = [ctypes.c_void_p, ctypes.c_void_p]
las.LASWriter_SetInputSRS.errcheck = check_return

las.LASWriter_SetOutputSRS.argtypes = [ctypes.c_void_p, ctypes.c_void_p]
las.LASWriter_SetOutputSRS.errcheck = check_return

las.LASWriter_GetHeader.restype = ctypes.c_void_p
las.LASWriter_GetHeader.argtypes = [ctypes.c_void_p]
las.LASWriter_GetHeader.errcheck = check_void

las.LASWriter_Destroy.argtypes = [ctypes.c_void_p]
las.LASWriter_Destroy.errcheck = check_void_done
las.LASWriter_Destroy.restype = None

las.LASGuid_Destroy.argtypes = [ctypes.c_void_p]
las.LASGuid_Destroy.errcheck = check_void_done
las.LASGuid_Destroy.restype = None

las.LASGuid_AsString.argtypes = [ctypes.c_void_p]
las.LASGuid_AsString.errcheck = check_value_free
las.LASGuid_AsString.restype = ctypes.c_char_p

las.LASGuid_Equals.argtypes = [ctypes.c_void_p, ctypes.c_void_p]
las.LASGuid_Equals.restype = ctypes.c_int
las.LASGuid_Equals.errcheck = check_value

las.LASGuid_CreateFromString.argtypes = [ctypes.c_char_p]
las.LASGuid_CreateFromString.errcheck = check_void
las.LASGuid_CreateFromString.restype = ctypes.c_void_p

las.LASGuid_Create.errcheck = check_void
las.LASGuid_Create.restype = ctypes.c_void_p

las.LASHeader_GetGUID.argtypes = [ctypes.c_void_p]
las.LASHeader_GetGUID.errcheck = check_void
las.LASHeader_GetGUID.restype = ctypes.c_void_p
las.LASHeader_SetGUID.argtypes = [ctypes.c_void_p, ctypes.c_void_p]
las.LASHeader_SetGUID.errcheck = check_value
las.LASHeader_SetGUID.restype = ctypes.c_int

las.LASVLR_Create.errcheck = check_void
las.LASVLR_Create.restype = ctypes.c_void_p

las.LASVLR_Destroy.argtypes = [ctypes.c_void_p]
las.LASVLR_Destroy.errcheck = check_void_done
las.LASVLR_Destroy.restype = None

las.LASVLR_GetUserId.argtypes = [ctypes.c_void_p]
las.LASVLR_GetUserId.errcheck = check_value_free
las.LASVLR_GetUserId.restype = ctypes.c_char_p
las.LASVLR_SetUserId.argtypes = [ctypes.c_void_p, ctypes.c_char_p]
las.LASVLR_SetUserId.errcheck = check_return
las.LASVLR_SetUserId.restype = ctypes.c_int

las.LASVLR_GetDescription.argtypes = [ctypes.c_void_p]
las.LASVLR_GetDescription.errcheck = check_value_free
las.LASVLR_GetDescription.restype = ctypes.c_char_p
las.LASVLR_SetDescription.argtypes = [ctypes.c_void_p, ctypes.c_char_p]
las.LASVLR_SetDescription.errcheck = check_return
las.LASVLR_SetDescription.restype = ctypes.c_int

las.LASVLR_GetRecordLength.argtypes = [ctypes.c_void_p]
las.LASVLR_GetRecordLength.errcheck = check_value
las.LASVLR_GetRecordLength.restype = ctypes.c_ushort
las.LASVLR_SetRecordLength.argtypes = [ctypes.c_void_p, ctypes.c_ushort]
las.LASVLR_SetRecordLength.errcheck = check_return
las.LASVLR_SetRecordLength.restype = ctypes.c_int

las.LASVLR_GetRecordId.argtypes = [ctypes.c_void_p]
las.LASVLR_GetRecordId.errcheck = check_value
las.LASVLR_GetRecordId.restype = ctypes.c_ushort
las.LASVLR_SetRecordId.argtypes = [ctypes.c_void_p, ctypes.c_ushort]
las.LASVLR_SetRecordId.errcheck = check_return
las.LASVLR_SetRecordId.restype = ctypes.c_int

las.LASVLR_GetReserved.argtypes = [ctypes.c_void_p]
las.LASVLR_GetReserved.errcheck = check_value
las.LASVLR_GetReserved.restype = ctypes.c_ushort
las.LASVLR_SetReserved.argtypes = [ctypes.c_void_p, ctypes.c_ushort]
las.LASVLR_SetReserved.errcheck = check_return
las.LASVLR_SetReserved.restype = ctypes.c_int

las.LASVLR_GetData.argtypes = [ctypes.c_void_p, ctypes.POINTER(ctypes.c_ubyte)]
las.LASVLR_GetData.errcheck = check_value
las.LASVLR_GetData.restype = ctypes.c_int

las.LASVLR_SetData.argtypes = [ctypes.c_void_p,
                                ctypes.POINTER(ctypes.c_ubyte),
                                ctypes.c_int]
las.LASVLR_SetData.errcheck = check_value
las.LASVLR_SetData.restype = ctypes.c_int


las.LASColor_Create.errcheck = check_void
las.LASColor_Create.restype = ctypes.c_void_p

las.LASColor_Destroy.argtypes = [ctypes.c_void_p]
las.LASColor_Destroy.errcheck = check_void_done
las.LASColor_Destroy.restype = None

las.LASColor_GetRed.restype = ctypes.c_short
las.LASColor_GetRed.argtypes = [ctypes.c_void_p]
las.LASColor_GetRed.errcheck = check_value
las.LASColor_SetRed.restype = ctypes.c_int
las.LASColor_SetRed.argtypes = [ctypes.c_void_p, ctypes.c_short]
las.LASColor_SetRed.errcheck = check_return

las.LASColor_GetGreen.restype = ctypes.c_short
las.LASColor_GetGreen.argtypes = [ctypes.c_void_p]
las.LASColor_GetGreen.errcheck = check_value
las.LASColor_SetGreen.restype = ctypes.c_int
las.LASColor_SetGreen.argtypes = [ctypes.c_void_p, ctypes.c_short]
las.LASColor_SetGreen.errcheck = check_return

las.LASColor_GetBlue.restype = ctypes.c_short
las.LASColor_GetBlue.argtypes = [ctypes.c_void_p]
las.LASColor_GetBlue.errcheck = check_value
las.LASColor_SetBlue.restype = ctypes.c_int
las.LASColor_SetBlue.argtypes = [ctypes.c_void_p, ctypes.c_short]
las.LASColor_SetBlue.errcheck = check_return

las.LASPoint_GetColor.argtypes = [ctypes.c_void_p]
las.LASPoint_GetColor.errcheck = check_void
las.LASPoint_GetColor.restype = ctypes.c_void_p

las.LASPoint_SetColor.argtypes = [ctypes.c_void_p, ctypes.c_void_p]
las.LASPoint_SetColor.errcheck = check_return

las.LASPoint_Destroy.argtypes = [ctypes.c_void_p]
las.LASPoint_Destroy.errcheck = check_void_done
las.LASPoint_Destroy.restype = None

las.LASSRS_Create.errcheck = check_void
las.LASSRS_Create.restype = ctypes.c_void_p

las.LASSRS_Destroy.argtypes = [ctypes.c_void_p]
las.LASSRS_Destroy.errcheck = check_void_done
las.LASSRS_Destroy.restype = None

las.LASSRS_GetProj4.argtypes = [ctypes.c_void_p]
las.LASSRS_GetProj4.errcheck = check_value_free
las.LASSRS_GetProj4.restype = ctypes.c_char_p
las.LASSRS_SetProj4.restype = ctypes.c_int
las.LASSRS_SetProj4.argtypes = [ctypes.c_void_p, ctypes.c_char_p]
las.LASSRS_SetProj4.errcheck = check_return

las.LASSRS_SetVerticalCS.restype = ctypes.c_int
las.LASSRS_SetVerticalCS.argtypes = [ctypes.c_void_p, ctypes.c_int,
                                     ctypes.c_char_p, ctypes.c_int,
                                     ctypes.c_int]
las.LASSRS_SetVerticalCS.errcheck = check_return

las.LASSRS_GetWKT_CompoundOK.argtypes = [ctypes.c_void_p]
las.LASSRS_GetWKT_CompoundOK.errcheck = check_value_free
las.LASSRS_GetWKT_CompoundOK.restype = ctypes.c_char_p
las.LASSRS_GetWKT.argtypes = [ctypes.c_void_p]
las.LASSRS_GetWKT.errcheck = check_value_free
las.LASSRS_GetWKT.restype = ctypes.c_char_p
las.LASSRS_SetWKT.restype = ctypes.c_int
las.LASSRS_SetWKT.argtypes = [ctypes.c_void_p, ctypes.c_char_p]
las.LASSRS_SetWKT.errcheck = check_return
las.LASSRS_SetFromUserInput.restype = ctypes.c_int
las.LASSRS_SetFromUserInput.argtypes = [ctypes.c_void_p, ctypes.c_char_p]
las.LASSRS_SetFromUserInput.errcheck = check_return

las.LASSRS_AddVLR.argtypes = [ctypes.c_void_p, ctypes.c_void_p]
las.LASSRS_AddVLR.errcheck = check_value
las.LASSRS_AddVLR.restype = ctypes.c_int

las.LASSRS_GetVLR.argtypes = [ctypes.c_void_p, ctypes.c_int]
las.LASSRS_GetVLR.errcheck = check_void
las.LASSRS_GetVLR.restype = ctypes.c_void_p

las.LASSRS_GetVLRCount.argtypes = [ctypes.c_void_p]
las.LASSRS_GetVLRCount.restype = ctypes.c_int

las.LASHeader_GetSRS.argtypes = [ctypes.c_void_p]
las.LASHeader_GetSRS.errcheck = check_void
las.LASHeader_GetSRS.restype = ctypes.c_void_p

las.LASHeader_SetSRS.argtypes = [ctypes.c_void_p, ctypes.c_void_p]
las.LASHeader_SetSRS.errcheck = check_return

las.LASHeader_GetSchema.argtypes = [ctypes.c_void_p]
las.LASHeader_GetSchema.errcheck = check_void
las.LASHeader_GetSchema.restype = ctypes.c_void_p

las.LASHeader_SetSchema.argtypes = [ctypes.c_void_p, ctypes.c_void_p]
las.LASHeader_SetSchema.errcheck = check_return
las.LASHeader_SetSchema.restype = ctypes.c_int

las.LASSchema_Create.argtypes = [ctypes.c_uint32]
                                        
las.LASSchema_Create.errcheck = check_void
las.LASSchema_Create.restype = ctypes.c_void_p

las.LASSchema_GetByteSize.restype = ctypes.c_ushort
las.LASSchema_GetByteSize.argtypes = [ctypes.c_void_p]
las.LASSchema_GetByteSize.errcheck = check_value


las.LASSchema_Destroy.argtypes = [ctypes.c_void_p]
las.LASSchema_Destroy.errcheck = check_void_done
las.LASSchema_Destroy.restype = None

las.LASString_Free.argtypes = [ctypes.POINTER(ctypes.c_char_p)]
las.LASString_Free.restype = None
