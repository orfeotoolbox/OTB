/******************************************************************************
 * $Id$
 *
 * Project:  libLAS - http://liblas.org - A BSD library for LAS format data.
 * Purpose:  
 * Author:   Martin Vales, martin_gnu@mundo-r.com
 *
 ******************************************************************************
 * Copyright (c) 2008, Martin Vales
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

using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;
using LASError = System.Int32;
using LASWriterH = System.IntPtr;
using LASReaderH = System.IntPtr;
using LASPointH = System.IntPtr;
using LASGuidH = System.IntPtr;
using LASVLRH = System.IntPtr;
using LASHeaderH = System.IntPtr;

namespace LibLAS
{
    internal class CAPI
    {
        /** Returns the version string for this library.
         * @return the version string for this library.
         * 
         * Name liblas1.dll is considered as unified across all builders on Windows platform.
         * When building libLAS on Windows using NMAKE and makefile.vc, C API DLL is
         * called liblas1.dll. The same name is used by Visual Studio solution liblas.sln.
         */
        public const string DLL_LAS_VERSION = "liblas1.dll";

        [DllImport(DLL_LAS_VERSION)]
        public static extern String LAS_GetVersion();

        /****************************************************************************/
        /* Error handling                                                           */
        /****************************************************************************/

        /** Resets the error stack for the libLAS C API.  
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern void LASError_Reset();

        /** Pops the top error off of the error stack for the libLAS C API.
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern void LASError_Pop();

        /** Returns the error number of the last error on the error stack.
        *  @return the error number of the last error on the error stack.
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern LASError LASError_GetLastErrorNum();

        /** Returns the name of the method the last error message happened in.
        *  @return the name of the method the last error message happened in.
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern String LASError_GetLastErrorMsg();

        /** Returns the name of the method the last error message happened in.
        *  @return the name of the method the last error message happened in.
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern String LASError_GetLastErrorMethod();

        /** Returns the number of error messages on the error stack.
        *  @return the number of error messages on the error stack.
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern int LASError_GetErrorCount();

        /** Prints the last error message in the error stack to stderr.  If 
        *  there is no error on the error stack, only the message is printed.
        *  The function does not alter the error stack in any way.
        *  @param message Message to include in the stderr output
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern void LASError_Print(String message);

        /****************************************************************************/
        /* Reader operations                                                        */
        /****************************************************************************/

        /** Creates a LASReaderH object that can be used to read LASHeaderH and 
        *  LASPointH objects with.  The LASReaderH must not be created with a 
        *  filename that is opened for read or write by any other API functions.
        *  @return opaque pointer to a LASReaderH instance.
        *  @param filename Filename to open for read 
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern LASReaderH LASReader_Create(String filename);

        /** Reads the next available point on the LASReaderH instance.  If no point 
        *  is available to read, NULL is returned.  If an error happens during 
        *  the reading of the next available point, an error will be added to the 
        *  error stack and can be returned with the LASError_GetLastError* methods.
        *  @param hReader the opaque handle to the LASReaderH 
        *  @return an opaque handle to a LASPointH object, or NULL if no point is 
        *  available to read or an error occured.  Use the 
        *  LASError_GetLastError* methods to confirm the existence of an error 
        *  if NULL is returned.
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern LASPointH LASReader_GetNextPoint(LASReaderH hReader);

        /** Reads a LASPointH from the given position in the LAS file represented 
        *  by the LASReaderH instance.  If no point is available at that location, 
        *  NULL is returned.  If an error happens during the reading of the point, 
        *  an error will be added to the error stack and can be returned with the 
        *  LASError_GetLastError* methods.
        *  @param hReader the opaque handle to the LASReaderH
        *  @param position the integer position of the point in the file to read.
        *  @return an opaque handle to a LASPointH object, or NULL if no point is 
        *  available at the given location or an error occured.  Use the 
        *  LASError_GetLastError* methods to confirm the existence of an error 
        *  if NULL is returned.
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern LASPointH LASReader_GetPointAt(LASReaderH hReader, UInt32 position);

        /** Closes the file for reading operations represented by the LASReaderH instance.
        *  @param hReader the opqaue handle to the LASReaderH
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern void LASReader_Destroy(LASReaderH hReader);

        /** Returns a LASHeaderH representing the header for the file
        *  @param hReader the LASReaderH instance
        *  @return a LASHeaderH representing the header for the file.  NULL is returned 
        *  in the event of an error.  Use the LASError_GetLastError* methods to check
        *  in the event of a NULL return.
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern LASHeaderH LASReader_GetHeader(LASReaderH hReader);

        /****************************************************************************/
        /* Point operations                                                         */
        /****************************************************************************/


        /** Returns the X value for the point.  This value is not scaled or offset
        *  by any header values and stands on its own.  If you need points to have 
        *  a scale and/or offset applied, this must be done in conjunction with the 
        *  header values after the value is read.
        *  @param hPoint the opaque pointer to the LASPointH instance  
        *  @return the X value for the LASPointH
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern double LASPoint_GetX(LASPointH hPoint);

        /** Sets the X value for the point.  This value must be scaled or offset 
        *  by any header values before being set.
        *  @param hPoint the opaque pointer to the LASPointH instance
        *  @param value the double value to set for the X value of the point
        *  @return an error number if an error occured during the setting of the point.
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern LASError LASPoint_SetX(LASPointH hPoint, double value);

        /** Returns the Y value for the point.  This value is not scaled or offset
        *  by any header values and stands on its own.  If you need points to have 
        *  a scale and/or offset applied, this must be done in conjunction with the 
        *  header values after the value is read.
        *  @param hPoint the opaque pointer to the LASPointH instance  
        *  @return the Y value for the LASPointH
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern double LASPoint_GetY(LASPointH hPoint);

        /** Sets the Y value for the point.  This value must be scaled or offset 
        *  by any header values before being set.
        *  @param hPoint the opaque pointer to the LASPointH instance
        *  @param value the double value to set for the Y value of the point
        *  @return an error number if an error occured during the setting of the point.
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern LASError LASPoint_SetY(LASPointH hPoint, double value);

        /** Returns the Z value for the point.  This value is not scaled or offset
        *  by any header values and stands on its own.  If you need points to have 
        *  a scale and/or offset applied, this must be done in conjunction with the 
        *  header values after the value is read.
        *  @param hPoint the opaque pointer to the LASPointH instance  
        *  @return the Z value for the LASPointH
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern double LASPoint_GetZ(LASPointH hPoint);

        /** Sets the Z value for the point.  This value must be scaled or offset 
        *  by any header values before being set.
        *  @param hPoint the opaque pointer to the LASPointH instance
        *  @param value the double value to set for the Z value of the point
        *  @return an error number if an error occured during the setting of the point.
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern LASError LASPoint_SetZ(LASPointH hPoint, double value);

        /** Returns the intensity value for the point.  This value is the pulse return 
        *  magnitude, it is optional, and it is LiDAR system specific.
        *  @return the intensity value for the point.
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern UInt16 LASPoint_GetIntensity(LASPointH hPoint);

        /** Sets the intensity value for the point.
        *  @param hPoint the opaque pointer to the LASPointH instance
        *  @param value the value to set the intensity to
        *  @return an error number if an error occured.
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern LASError LASPoint_SetIntensity(LASPointH hPoint, UInt16 value);

        /** Returns the return number for the point.  The return number is "the pulse
        *  return number for a given output pulse."  The first return number starts with
        *  the value 1.
        *  @param hPoint LASPointH instance
        *  @return a return number, valid from 1-6, for the point.  Use the LASError 
        *  methods to determine if an error occurred during this operation if 0 
        *  is returned.
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern UInt16 LASPoint_GetReturnNumber(LASPointH hPoint);

        /** Sets the return number for the point.  Valid values are from 1-6.
        *  @param hPoint LASPointH instance
        *  @param value the value to set for the return number
        *  @return LASError value determine success or failure.
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern LASError LASPoint_SetReturnNumber(LASPointH hPoint, UInt16 value);

        /** Returns the total number of returns for a given pulse.
        *  @param hPoint LASPointH instance
        *  @return total number of returns for this pulse.
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern UInt16 LASPoint_GetNumberOfReturns(LASPointH hPoint);

        /** Sets the number of returns for the point.  Valid values are from 1-5.
        *  @param hPoint LASPointH instance
        *  @param value the value to set for the number of returns
        *  @return LASError value determine success or failure.
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern LASError LASPoint_SetNumberOfReturns(LASPointH hPoint, UInt16 value);

        /** Returns the scan direction for a given pulse.
        *  @param hPoint LASPointH instance
        *  @return the scan direction for a given pulse.
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern UInt16 LASPoint_GetScanDirection(LASPointH hPoint);

        /** Sets the scan direction for a given pulse.  Valid values are 0 or 1, with 
        *  1 being a positive scan direction and 0 being a negative scan direction.
        *  @param hPoint LASPointH instance
        *  @param value the value to set for scan direction
        *  @return LASError value determine success or failure.
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern LASError LASPoint_SetScanDirection(LASPointH hPoint, UInt16 value);

        /** Returns whether or not a given pulse is an edge point
        *  @param hPoint LASPointH instance
        *  @return whether or not a given pulse is an edge point.
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern UInt16 LASPoint_GetFlightLineEdge(LASPointH hPoint);

        /** Sets the edge marker for a given pulse.  Valid values are 0 or 1, with 
        *  1 being an edge point and 0 being interior.
        *  @param hPoint LASPointH instance
        *  @param value the value to set for flightline edge
        *  @return LASError value determine success or failure.
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern LASError LASPoint_SetFlightLineEdge(LASPointH hPoint, UInt16 value);

        /** Returns all of the scan flags for the point -- Return number, number of 
        *  returns, flightline edge, scan direction, and scan angle rank.
        *  @param hPoint LASPointH instance
        *  @return all of the scan flags for the point
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern byte LASPoint_GetScanFlags(LASPointH hPoint);

        /** Sets all of the scan flags for the point.  No validation is done. 
        *  @param hPoint LASPointH instance
        *  @param value the value to set for the flags
        *  @return LASError value determine success or failure.
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern LASError LASPoint_SetScanFlags(LASPointH hPoint, byte value);

        /** Returns the classification for the point
        *  @param hPoint LASPointH instance
        *  @return the classification for the point
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern byte LASPoint_GetClassification(LASPointH hPoint);

        /** Sets the classification for the point.  No validation is done. 
        *  @param hPoint LASPointH instance
        *  @param value the value to set for the classification
        *  @return LASError value determine success or failure.
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern LASError LASPoint_SetClassification(LASPointH hPoint, byte value);

        /** Returns the time for the point
        *  @param hPoint LASPointH instance
        *  @return the time for the point
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern double LASPoint_GetTime(LASPointH hPoint);

        /** Sets the time for the point.  No validation is done. 
        *  @param hPoint LASPointH instance
        *  @param value the value to set for the time
        *  @return LASError value determine success or failure.
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern LASError LASPoint_SetTime(LASPointH hPoint, double value);

        /** Returns the scan angle for the point
        *  @param hPoint LASPointH instance
        *  @return the scan angle for the point
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern SByte LASPoint_GetScanAngleRank(LASPointH hPoint);

        /** Sets the scan angle for the point.  No validation is done. 
        *  @param hPoint LASPointH instance
        *  @param value the value to set for the scan angle
        *  @return LASError value determine success or failure.
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern LASError LASPoint_SetScanAngleRank(LASPointH hPoint, SByte value);

        /** Returns the arbitrary user data for the point
        *  @param hPoint LASPointH instance
        *  @return the arbitrary user data for the point
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern byte LASPoint_GetUserData(LASPointH hPoint);

        /** Sets the arbitrary user data for the point.  No validation is done. 
        *  @param hPoint LASPointH instance
        *  @param value the value to set for the arbitrary user data
        *  @return LASError value determine success or failure.
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern LASError LASPoint_SetUserData(LASPointH hPoint, byte value);

        /** Returns a bitfield representing the validity of various members
        *  enum DataMemberFlag
        {
        eReturnNumber = 1,
        eNumberOfReturns = 2,
        eScanDirection = 4,
        eFlightLineEdge = 8,
        eClassification = 16,
        eScanAngleRank = 32,
        eTime = 64
        };
        *  @param hPoint LASPointH instance
        *  @return bitfield representing the validity of various members.
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern int LASPoint_Validate(LASPointH hPoint);

        /** Returns a boolean whether or not the point is valid
        *  @param hPoint LASPointH instance
        *  @return a boolean (1 or 0) whether or not the point is valid.
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern int LASPoint_IsValid(LASPointH hPoint);

        /** Creates a new empty LASPointH instance 
        *  @return LASPointH instance.  If the value is NULL use the 
        *  LASError_GetLastError* methods to determine the problem
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern LASPointH LASPoint_Create();

        /** Creates a copy of a LASPointH instance
        *  @param hPoint the LASPointH instance to copy 
        *  @return new LASPointH instance.  If the value is NULL use the 
        *  LASError_GetLastError* methods to determine the problem
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern LASPointH LASPoint_Copy(LASPointH hPoint);

        /** Destroys/deletes a LASPointH instance
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern void LASPoint_Destroy(LASPointH hPoint);

        /****************************************************************************/
        /* Header operations                                                        */
        /****************************************************************************/

        /** Copies a LASHeaderH instance
        *  @param hHeader the LASHeaderH to copy
        *  @return a LASHeaderH instance or NULL on error
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern LASHeaderH LASHeader_Copy(LASHeaderH hHeader);

        /** Creates an empty LASHeaderH with default values
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern LASHeaderH LASHeader_Create();

        /** Destroys/deletes a LASHeader instance
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern void LASHeader_Destroy(LASHeaderH hHeader);

        /** Returns the file signature the the file.  This should always be 'LASF'
        *  @param hHeader LASHeaderH instance
        *  @return the file signature the the file.  This should always be 'LASF'
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern String LASHeader_GetFileSignature(LASHeaderH hHeader);

        /** Returns the file source id for the file.  It is a number from 1-65535
        *  @param hHeader LASHeaderH instance
        *  @return the file source id for the file.
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern UInt16 LASHeader_GetFileSourceId(LASHeaderH hHeader);

        [DllImport(DLL_LAS_VERSION)]
        public static extern LASError LASHeader_SetFileSourceId(LASHeaderH hHeader, UInt16 value);

        /** Returns the project id for the header as a GUID string
        *  @return the project id for the header as a GUID string
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern String LASHeader_GetProjectId(LASHeaderH hHeader);

        /** Sets the project id/GUID for the header
        *  @param hHeader LASHeaderH instance
        *  @param hId LASGuidH instance to set the GUID for the header to
        *  @return LASError enum
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern LASError LASHeader_SetGUID(LASHeaderH hHeader, LASGuidH hId);

        /** Returns the major version number for the header.  This value is expected 
        *  to always be 1.
        *  @param hHeader LASHeaderH instance
        *  @return major version number for the header.
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern byte LASHeader_GetVersionMajor(LASHeaderH hHeader);

        /** Sets the major version number for the header.  All values other than 1 
        *  are invalid.
        *  @param hHeader LASHeaderH instance
        *  @param value integer value to set the major version to (only the value 1 is valid)
        *  @return LASError enum
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern LASError LASHeader_SetVersionMajor(LASHeaderH hHeader, byte value);

        /** Returns the min version number for the header.  This value is expected 
        *  to be 1 or 0 representing LAS 1.1 or LAS 1.0
        *  @param hHeader LASHeaderH instance
        *  @return minor version number for the header.
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern byte LASHeader_GetVersionMinor(LASHeaderH hHeader);

        /** Sets the minor version number for the header.  All values other than 1 or 0 
        *  are invalid.
        *  @param hHeader LASHeaderH instance
        *  @param value integer value to set the minor version to (only the values 1 or 0 are valid)
        *  @return LASError enum
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern LASError LASHeader_SetVersionMinor(LASHeaderH hHeader, byte value);

        /** Returns the System ID for the header.  The caller assumes ownership of the returned string
        *  @return the system id for the header as a character array
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern String LASHeader_GetSystemId(LASHeaderH hHeader);

        /** Sets the System ID for the header.  By default, this value is "libLAS" if it 
        *  is not explicitly set.  See the LAS specification for details on what this
        *  value should logically be set to.
        *  @param hHeader LASHeaderH instance
        *  @param value the value to set as the System ID for the header
        *  @return LASError enum
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern LASError LASHeader_SetSystemId(LASHeaderH hHeader, String value);

        /** Returns the Software ID for the header.  The caller assumes ownership of the returned string
        *  @return the software id for the header as a character array
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern String LASHeader_GetSoftwareId(LASHeaderH hHeader);

        /** Sets the Software ID for the header.  By default, this value is "libLAS 1.0" if it 
        *  is not explicitly set.  See the LAS specification for details on what this
        *  value should logically be set to.
        *  @param hHeader LASHeaderH instance
        *  @param value the value to set as the Software ID for the header
        *  @return LASError enum
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern LASError LASHeader_SetSoftwareId(LASHeaderH hHeader, String value);

        /** Returns the reserved value for the header.  This should aways be 0.
        *  @return the reserved value for the header.
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern Int16 LASHeader_GetReserved(LASHeaderH hHeader);

        /** Returns the file creation day of the year.  The values start from 1, being January 1st, 
        *  and end at 365 or 366 being December 31st, depending on leap year.
        *  @return the day of the year as an integer starting from 1 for the file creation.
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern UInt16 LASHeader_GetCreationDOY(LASHeaderH hHeader);

        /** Sets the file creation day of the year.  The values start from 1, being January 1st.  No
        *  date validation is done
        *  @param hHeader LASHeaderH instance
        *  @param value the value to set as the creation day
        *  @return LASError enum
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern LASError LASHeader_SetCreationDOY(LASHeaderH hHeader, UInt16 value);

        /** Returns the file creation year.  This is a four digit number representing the 
        *  year for the file, ie 2003, 2008, etc.
        *  @return the creation year for the file or 0 if none is set
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern UInt16 LASHeader_GetCreationYear(LASHeaderH hHeader);

        /** Sets the file creation year.  This should be a four digit number representing
        *  the year for the file, ie 2003, 2008, etc.  No validation on the value is done
        *  @param hHeader LASHeaderH instance
        *  @param value the value to set for the creation year
        *  @return LASError enum
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern LASError LASHeader_SetCreationYear(LASHeaderH hHeader, UInt16 value);

        /** Returns the size of the header for the file in bytes.
        *  @return the size of the header for the file in bytes.
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern UInt16 LASHeader_GetHeaderSize(LASHeaderH hHeader);

        /** Returns the byte offset to the start of actual point data for the file
        *  @param hHeader LASHeaderH instance
        *  @return the type offset to the start of actual point data for the file
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern UInt32 LASHeader_GetDataOffset(LASHeaderH hHeader);

        /** Returns the number of variable length records in the header
        *  @param hHeader LASHeaderH instance
        *  @return the number of variable length records in the header
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern UInt32 LASHeader_GetRecordsCount(LASHeaderH hHeader);

        /** Returns the record length for the points based on their data format id in bytes
        *  @param hHeader LASHeaderH instance
        *  @return the record length for the points based on their data format id in bytes
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern UInt16 LASHeader_GetDataRecordLength(LASHeaderH hHeader);

        /** Returns the data format id.  If this value is 1, the point data have time values
        *  associated with them.  If it is 0, the point data do not have time values.  
        *  @param hHeader LASHeaderH instance
        *  @return the data format id for the file.
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern byte LASHeader_GetDataFormatId(LASHeaderH hHeader);

        /** Sets the data format id for the file.  The value should be 1 or 0, with 1 being
        *  points that contain time values and 0 being points that do not.
        *  @param hHeader LASHeaderH instance
        *  @param value the value for the data format id, 1 or 0 are valid values.
        *  @return LASError enum
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern LASError LASHeader_SetDataFormatId(LASHeaderH hHeader, byte value);

        /** Returns the number of point records in the file.  This value may not reflect the actual 
        *  number of point records in the file.
        *  @param hHeader LASHeaderH instance
        *  @return the number of point records in the file
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern UInt32 LASHeader_GetPointRecordsCount(LASHeaderH hHeader);

        /** Sets the number of point records for the file.
        *  @param hHeader LASHeaderH instance
        *  @param value the long integer to set for the number of point records in the file
        *  @return LASError enum
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern LASError LASHeader_SetPointRecordsCount(LASHeaderH hHeader, UInt32 value);

        /** Returns the number of point records by return.
        *  @param hHeader LASHeaderH instance
        *  @param index the return number to fetch the count for
        *  @return the number of point records for a given return
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern UInt32 LASHeader_GetPointRecordsByReturnCount(LASHeaderH hHeader, int index);

        /** Sets the number of point records for a given return
        *  @param hHeader LASHeaderH instance
        *  @param index the return number to set the count for
        *  @param value the number of point records for the return 
        *  @return LASError enum
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern LASError LASHeader_SetPointRecordsByReturnCount(LASHeaderH hHeader, int index, UInt32 value);

        /** Return the X scale factor
        *  @param hHeader LASHeaderH instance
        *  @return the X scale factor
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern double LASHeader_GetScaleX(LASHeaderH hHeader);

        /** Return the Y scale factor
        *  @param hHeader LASHeaderH instance
        *  @return the Y scale factor
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern double LASHeader_GetScaleY(LASHeaderH hHeader);

        /** Return the Z scale factor
        *  @param hHeader LASHeaderH instance
        *  @return the Z scale factor
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern double LASHeader_GetScaleZ(LASHeaderH hHeader);

        /** Sets the scale factors
        *  @param hHeader LASHeaderH instance
        *  @param x the x scale factor
        *  @param y the y scale factor
        *  @param z the z scale factor
        *  @return LASError enum
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern LASError LASHeader_SetScale(LASHeaderH hHeader, double x, double y, double z);

        /** Return the X offset
        *  @param hHeader LASHeaderH instance
        *  @return the X offset
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern double LASHeader_GetOffsetX(LASHeaderH hHeader);

        /** Return the Y offset
        *  @param hHeader LASHeaderH instance
        *  @return the Y offset
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern double LASHeader_GetOffsetY(LASHeaderH hHeader);

        /** Return the Z offset
        *  @param hHeader LASHeaderH instance
        *  @return the Z offset
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern double LASHeader_GetOffsetZ(LASHeaderH hHeader);

        /** Sets the offset values
        *  @param hHeader LASHeaderH instance
        *  @param x the x offset
        *  @param y the y offset
        *  @param z the z offset
        *  @return LASError enum
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern LASError LASHeader_SetOffset(LASHeaderH hHeader, double x, double y, double z);

        /** Return the minimum x value
        *  @param hHeader LASHeaderH instance
        *  @return the minimum x value
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern double LASHeader_GetMinX(LASHeaderH hHeader);

        /** Return the minimum y value
        *  @param hHeader LASHeaderH instance
        *  @return the minimum y value
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern double LASHeader_GetMinY(LASHeaderH hHeader);

        /** Return the minimum z value
        *  @param hHeader LASHeaderH instance
        *  @return the minimum z value
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern double LASHeader_GetMinZ(LASHeaderH hHeader);

        /** Sets the minimum values
        *  @param hHeader LASHeaderH instance
        *  @param x the x minimum
        *  @param y the y minimum
        *  @param z the z minimum
        *  @return LASError enum
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern LASError LASHeader_SetMin(LASHeaderH hHeader, double x, double y, double z);

        /** Return the maximum x value
        *  @param hHeader LASHeaderH instance
        *  @return the maximum x value
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern double LASHeader_GetMaxX(LASHeaderH hHeader);

        /** Return the maximum y value
        *  @param hHeader LASHeaderH instance
        *  @return the maximum y value
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern double LASHeader_GetMaxY(LASHeaderH hHeader);

        /** Return the maximum z value
        *  @param hHeader LASHeaderH instance
        *  @return the maximum z value
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern double LASHeader_GetMaxZ(LASHeaderH hHeader);

        /** Sets the maximum values
        *  @param hHeader LASHeaderH instance
        *  @param x the x maximum
        *  @param y the y maximum
        *  @param z the z maximum
        *  @return LASError enum
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern LASError LASHeader_SetMax(LASHeaderH hHeader, double x, double y, double z);

        /** Returns the proj.4 string describing the spatial reference of the 
        *  header if it is available
        *  @param hHeader LASHeaderH instance
        *  @return the proj.4 string or NULL if none is available.  The caller
        *  owns the string.
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern String LASHeader_GetProj4(LASHeaderH hHeader);

        /** Sets the proj4 stirng describing the spatial reference of the header.
        *  @param hHeader LASHeaderH instance
        *  @param value the proj4 string to set for the header
        *  @return LASError enum
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern LASError LASHeader_SetProj4(LASHeaderH hHeader, String value);

        /** Returns the VLR record for the given index.  Use LASHeader_GetRecordsCount to 
        *  determine the number of VLR records available on the header.
        *  @param hHeader the LASHeaderH instance
        *  @param i the index starting from 0 of the VLR to fetch
        *  @return LASVLRH instance that models the Variable Length Record
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern LASVLRH LASHeader_GetVLR(LASHeaderH hHeader, UInt32 i);

        /** Deletes a VLR record from the header for the given index.
        *  @param hHeader the LASHeaderH instance
        *  @param index the index starting from 0 of the VLR to delete
        *  @return LASErrorEnum
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern LASError LASHeader_DeleteVLR(LASHeaderH hHeader, UInt32 index);

        /** Adds a VLR record to the header. 
        *  @param hHeader the LASHeaderH instance
        *  @param hVLR the VLR to add to the header
        *  @return LASErrorEnum
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern LASError LASHeader_AddVLR(LASHeaderH hHeader, LASVLRH hVLR);

        /****************************************************************************/
        /* Writer Operations                                                        */
        /****************************************************************************/

        /** Creates a new LASWriterH for write operations on LAS files.  The file may 
        *  be opened in either LAS_MODE_APPEND or LAS_MODE_WRITE, but the file cannot 
        *  be open by another other operations (another LASReaderH or LASWriterH).  
        *  @param filename The filename to open to write
        *  @param hHeader an opaque pointer to a LASHeaderH that will be written to 
        *  the file as part of the opening for write operation.
        *  @param mode a mode value to denote whether to open for write or append 
        *  operations.  Valid values are LAS_MODE_APPEND and LAS_MODE_WRITE.
        */

        [DllImport(DLL_LAS_VERSION)]
        public static extern LASWriterH LASWriter_Create(String filename, LASHeaderH hHeader, int mode);

        /** Writes a point to the file.  The location of where the point is writen is 
        *  determined by the mode the file is opened in, and what the last operation was.  
        *  For example, if the file was opened for append, the next point would be written 
        *  at the end of the file.  Likewise, if the file is opened in write mode, even 
        *  if the file already existed, the next WritePoint operation will happen at the 
        *  end of the header and all of the existing points in the file will be overwritten.
        *  @param hWriter opaque pointer to the LASWriterH instance
        *  @param hPoint the opaque LASPointH pointer to write
        *  @return LE_None if no error occurred during the write operation.
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern LASError LASWriter_WritePoint(LASWriterH hWriter, LASPointH hPoint);

        /** Overwrites the header for the file represented by the LASWriterH.  It does 
        *  not matter if the file is opened for append or for write.
        *  @param hWriter opaque pointer to the LASWriterH instance
        *  @param hHeader LASHeaderH instance to write into the file
        *  @return LE_None if no error occurred during the operation.
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern LASError LASWriter_WriteHeader(LASWriterH hWriter, LASHeaderH hHeader);

        /** Destroys the LASWriterH instance, effectively closing the file and performing 
        *  housekeeping operations.
        *  @param hWriter LASWriterH instance to close
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern void LASWriter_Destroy(LASWriterH hWriter);

        /****************************************************************************/
        /* GUID Operations                                                          */
        /****************************************************************************/

        /** Returns the GUID value for the header as an opaque LASGuidH pointer.
        *  @param hHeader the opaque pointer to the LASHeaderH
        *  @return the GUID value for the header as an opaque LASGuidH pointer.
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern LASGuidH LASHeader_GetGUID(LASHeaderH hHeader);

        /** Returns a new random GUID.
        *  @return a new random GUID
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern LASGuidH LASGuid_Create();

        /** Creates a new GUID opaque pointer using the given string.  
        *  @param string A GUID string in the form "00000000-0000-0000-0000-000000000000"
        *  An example GUID might be something like '8388F1B8-AA1B-4108-BCA3-6BC68E7B062E'
        *  @return the GUID value as an opaque LASGuidH pointer.
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern LASGuidH LASGuid_CreateFromString(String string_);

        /** Destroys a GUID opaque pointer and removes it from the heap
        *  @param hId the GUID value to destroy as an opaque LASGuidH pointer.
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern void LASGuid_Destroy(LASGuidH hId);

        /** Determines if two GUIDs are equal.
        *  @param hId1 the first GUID
        *  @param hId2 the second GUID
        *  @return 0 if false, 1 if true.  Use the LASError_GetLastError* methods to 
        *  determine if an error occured during the operation of this function.
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern int LASGuid_Equals(LASGuidH hId1, LASGuidH hId2);

        /** Returns a string representation of the GUID opqaue pointer.  The caller 
        *  owns the string.
        *  @param hId the LASGuidH pointer
        *  @return a string representation of the GUID opaque pointer.
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern String LASGuid_AsString(LASGuidH hId);

        /****************************************************************************/
        /* VLR Operations                                                           */
        /****************************************************************************/

        /** Creates a new VLR record
        *  @return a new VLR record
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern LASVLRH LASVLR_Create();

        /** Destroys a VLR record and removes it from the heap
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern void LASVLR_Destroy(LASVLRH hVLR);

        /** Returns the User Id for the VLR 
        *  @param hVLR the LASVLRH instance
        *  @return the User Id for the VLR
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern String LASVLR_GetUserId(LASVLRH hVLR);

        /** Sets the User Id for the VLR
        *  @param hVLR the LASVLRH instance
        *  @param value the value to set for the User Id.  It will be clipped to fit 
        *  within 16 characters
        *  @return LASErrorEnum
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern LASError LASVLR_SetUserId(LASVLRH hVLR, String value);

        /** Gets the description for the VLR
        *  @param hVLR the LASVLRH instance
        *  @return the description for the VLR
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern String LASVLR_GetDescription(LASVLRH hVLR);

        /** Sets the description for the VLR
        *  @param hVLR the LASVLRH instance
        *  @param value the value to set for the description.  It will be clipped to fit 
        *  within 32 characters
        *  @return LASErrorEnum
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern LASError LASVLR_SetDescription(LASVLRH hVLR, String value);

        /** Returns the record length of the data stored in the VLR
        *  @param hVLR the LASVLRH instance
        *  @return the record length of the data stored in the VLR
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern UInt16 LASVLR_GetRecordLength(LASVLRH hVLR);

        /** Sets the record length of the data stored in the VLR
        *  @param hVLR the LASVLRH instance
        *  @param value the length to set for the VLR data length
        *  @return LASErrorEnum
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern LASError LASVLR_SetRecordLength(LASVLRH hVLR, UInt16 value);

        /** Gets the record id for the VLR
        *  @param hVLR the LASVLRH instance
        *  @return the record id for the VLR
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern UInt16 LASVLR_GetRecordId(LASVLRH hVLR);

        /** Sets the record id for the VLR
        *  @param hVLR the LASVLRH instance
        *  @param value the record id to set
        *  @return LASErrorEnum
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern LASError LASVLR_SetRecordId(LASVLRH hVLR, UInt16 value);

        /** Gets the reserved value of the VLR.  This should be 0 and should aways be 0.
        *  @param hVLR the LASVLRH instance
        *  @return the reserved value of the VLR.
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern UInt16 LASVLR_GetReserved(LASVLRH hVLR);

        /** Sets the reserved value of the VLR.  This should be 0 and you should not 
        *  have to ever monkey with this value according to the spec.
        *  @param hVLR the LASVLRH instance
        *  @param value the value to set for the reserved value
        *  @return LASErrorEnum
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern LASError LASVLR_SetReserved(LASVLRH hVLR, UInt16 value);

        /** Gets the data stream for the VLR as an array of bytes
        *  @param hVLR the LASVLRH instance
        *  @param data a pointer to where place the array
        *  @param length a pointer to where to place the length of the array
        *  @return LASErrorEnum
        */
        [DllImport(DLL_LAS_VERSION)]
        public static extern LASError LASVLR_GetData(LASVLRH hVLR, out byte[] data);//, ref int length);

        [DllImport(DLL_LAS_VERSION)]
        public static extern LASError LASVLR_SetData(LASVLRH hVLR, ref byte[] data, int length);
    }
}
