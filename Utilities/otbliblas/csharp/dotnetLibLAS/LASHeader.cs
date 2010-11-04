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
using LASError = System.Int32;
using LASWriterH = System.IntPtr;
using LASReaderH = System.IntPtr;
using LASPointH = System.IntPtr;
using LASGuidH = System.IntPtr;
using LASVLRH = System.IntPtr;
using LASHeaderH = System.IntPtr;

namespace LibLAS
{
    /// <summary>
    /// LASHeader class
    /// </summary>
    public class LASHeader : IDisposable
    {

        /// <summary>
        /// The object user should call this method when they finished with the object.
        /// In .NET is magaged by the GC.
        /// </summary>
        public void Dispose()
        {

            CAPI.LASHeader_Destroy(hHeader);
            // Clean up unmanaged resources here.
            // Dispose other contained disposable objects.
        }

        enum FormatVersion
        {
            /// Minimum of major component
            eVersionMajorMin = 1,
            ///Maximum of major component        
            eVersionMajorMax = 1,
            /// Minimum of minor component
            eVersionMinorMin = 0,
            /// Maximum of minor component
            eVersionMinorMax = 1
        };

        /// Versions of point record format.
        public enum PointFormat
        {
            ///Point Data Format \e 0
            ePointFormat0 = 0,
            /// Point Data Format \e 1
            ePointFormat1 = 1
        };

        /// Number of bytes of point record storage in particular format.
        public enum PointSize
        {
            ///Size of point record in data format \e 0
            ePointSize0 = 20,
            /// Size of point record in data format \e 1
            ePointSize1 = 28
        };

        //// Official signature of ASPRS LAS file format, always \b "LASF".
        //public static readonly string FileSignature_;

        //// Default system identifier used by libLAS, always \b "libLAS".
        //public static readonly string SystemIdentifier;

        ////Default software identifier used by libLAS, always \b "libLAS X.Y".
        //public static readonly string SoftwareIdentifier;

        private LASHeaderH hHeader;

        /// <summary>
        /// gets the opaque pointer to the LASHeaderH instance.
        /// </summary>
        /// <returns>opaque pointer to the LASHeaderH instance.</returns>
        internal LASHeaderH GetPointer()
        {
            return hHeader;
        }

        /// <summary>
        /// LASHeader constructor using the LASHeaderH opaque pointer.
        /// </summary>
        /// <param name="hLASHeader"></param>
        public LASHeader(LASHeaderH hLASHeader)
        {
            hHeader = hLASHeader;
        }

        /// <summary>
        /// Default constructor.
        /// </summary>
        /// <remarks>The default constructed header is configured according
        /// to the ASPRS LAS 1.1 Specification, point data format set to 0.
        /// Other fields filled with 0.</remarks>
        public LASHeader()
        {
            hHeader = CAPI.LASHeader_Create();
        }

        /// <summary>
        /// Copy the LASHeader in a new instance
        /// </summary>
        /// <returns>new LASHeader instance.</returns>
        public LASHeader Copy()
        {
            return new LASHeader(CAPI.LASHeader_Copy(hHeader));
        }

        /// <summary>
        /// Destroy the unmanaged resources to the instance.
        /// </summary>
        /// <remarks>The user could call this method when they finished with the object.</remarks>
        public void Destroy()
        {
            CAPI.LASHeader_Destroy(hHeader);
        }

        /// <summary>
        /// Comparison overload to the LASHeader
        /// </summary>
        /// <param name="lasHeader1">LASHeader instance to be compared</param>
        /// <param name="lasHeader2">LASHeader instance to be compared</param>
        /// <returns>true if lasHeader1==lasHeader2</returns>
        public static bool operator ==(LASHeader lasHeader1, LASHeader lasHeader2)
        {
            return lasHeader1.Equals(lasHeader2);
        }

        /// <summary>
        /// Comparison overload to the LASHeader
        /// </summary>
        /// <param name="lasHeader1">LASHeader instance to be compared</param>
        /// <param name="lasHeader2">LASHeader instance to be compared</param>
        /// <returns></returns>
        public static bool operator !=(LASHeader lasHeader1, LASHeader lasHeader2)
        {
            return !(lasHeader1 == lasHeader2);
        }

        /// <summary>
        /// Get ASPRS LAS file signature.
        /// </summary>
        /// <remarks>The only value allowed as file signature is "LASF",</remarks>
        public string FileSignature
        {
            get
            {
                return CAPI.LASHeader_GetFileSignature(hHeader);
            }
        }

        /// <summary>
        /// file source identifier.
        /// </summary>
        /// <remarks> should be set to a value between 1 and 65535.</remarks>
        public UInt16 FileSourceId
        {
            get
            {
                return CAPI.LASHeader_GetFileSourceId(hHeader);
            }
            set
            {
                LASError error = CAPI.LASHeader_SetFileSourceId(hHeader, value);
                if ((Int32)error != 0)
                {
                    LASException e = new LASException("Exception in Set Header SetFileSourceId.");
                    throw e;
                }
            }
        }

        /// <summary>
        /// Get value field reserved by the ASPRS LAS Specification.
        /// </summary>
        /// <remarks>This field is always filled with 0.</remarks>
        public Int16 Reserved
        {
            get
            {
                return CAPI.LASHeader_GetReserved(hHeader);
            }
        }

        /// <summary>
        /// Get project identifier.
        /// </summary>
        /// <remarks>return Global Unique Identifier.</remarks>
        public String ProjectId
        {
            get
            {
                return CAPI.LASHeader_GetProjectId(hHeader);
            }
        }

        /// <summary>
        /// major component of version of LAS format.
        /// </summary>
        /// <remarks>Always 1 as the only valid value. value between
        /// eVersionMajorMin and eVersionMajorMax (always 1).</remarks>
        public byte VersionMajor
        {
            get
            {
                return CAPI.LASHeader_GetVersionMajor(hHeader);
            }
            set
            {
                LASError error = CAPI.LASHeader_SetVersionMajor(hHeader, value);
                if ((Int32)error != 0)
                {
                    LASException e = new LASException("Exception in Set Header VersionMajor.");
                    throw e;
                }
            }
        }

        /// <summary>
        /// minor component of version of LAS format.
        /// </summary>
        /// <remarks>Valid values are 1 or 0. value between
        /// eVersionMinorMin and eVersionMinorMax.</remarks>
        public byte VersionMinor
        {
            get
            {

                return CAPI.LASHeader_GetVersionMinor(hHeader);
            }
            set
            {
                LASError error = CAPI.LASHeader_SetVersionMinor(hHeader, value);
                if ((Int32)error != 0)
                {
                    LASException e = new LASException("Exception in Set Header VersionMinor.");
                    throw e;
                }
            }
        }

        /// <summary>
        /// system identifier
        /// </summary>
        /// <remarks>Default value is "libLAS" specified as the SystemIdentifier constant.
        /// string is padded right with spaces and its length is 32 bytes.</remarks>
        public String SystemId
        {
            get
            {
                return CAPI.LASHeader_GetSystemId(hHeader);
            }
            set
            {
                LASError error = CAPI.LASHeader_SetSystemId(hHeader, value);
                if ((Int32)error != 0)
                {
                    LASException e = new LASException("Exception in Set Header SystemId.");
                    throw e;
                }
            }
        }

        /// <summary>
        /// software identifier
        /// </summary>
        /// <remarks>Default value is "libLAS 1.0", specified as the SoftwareIdentifier constant.
        /// String is padded right with spaces and its length is 32 bytes.</remarks>
        public String SoftwareId
        {
            get
            {

                return CAPI.LASHeader_GetSoftwareId(hHeader);
            }
            set
            {
                LASError error = CAPI.LASHeader_SetSoftwareId(hHeader, value);
                if ((Int32)error != 0)
                {
                    LASException e = new LASException("Exception in Set Header SoftwareId.");
                    throw e;
                }
            }
        }

        /// <summary>
        /// day of year of file creation date.
        /// </summary>
        /// <remarks>Use full date structure instead of Julian date number.
        /// value is lower than number 366.</remarks>
        public UInt16 CreationDOY
        {
            get
            {
                return CAPI.LASHeader_GetCreationDOY(hHeader);
            }
            set
            {
                LASError error = CAPI.LASHeader_SetCreationDOY(hHeader, value);
                if ((Int32)error != 0)
                {
                    LASException e = new LASException("Exception in Set Header CreationDOY.");
                    throw e;
                }
            }
        }

        /// <summary>
        /// year of file creation date.
        /// </summary>
        /// <remarks>value is lower than number 9999.</remarks>
        public UInt16 CreationYear
        {
            get
            {
                return CAPI.LASHeader_GetCreationYear(hHeader);
            }
            set
            {
                LASError error = CAPI.LASHeader_SetCreationYear(hHeader, value);
                if ((Int32)error != 0)
                {
                    LASException e = new LASException("Exception in Set Header CreationYear.");
                    throw e;
                }
            }
        }

        /// <summary>
        /// number of bytes of generic verion of public header block storage.
        /// </summary>
        /// <remarks>Standard version of the public header block is 227 bytes long.</remarks>
        public UInt16 HeaderSize
        {
            get
            {
                return CAPI.LASHeader_GetHeaderSize(hHeader);
            }
        }

        /// <summary>
        /// number of bytes from the beginning to the first point record.
        /// </summary>
        public UInt32 DataOffset
        {
            get
            {
                return CAPI.LASHeader_GetDataOffset(hHeader);
            }
        }

        /// <summary>
        /// Returns the number of variable length records in the header
        /// </summary>
        public UInt32 VariableLengthRecordsCount
        {
            get
            {
                return CAPI.LASHeader_GetRecordsCount(hHeader);
            }
        }

        /// <summary>
        /// the data format id for the file.  
        /// </summary>
        /// <remarks>The value should be 1 or 0, with 1 being points that contain
        /// time values and 0 being points that do not.</remarks>
        public byte DataFormatId
        {
            get
            {
                return CAPI.LASHeader_GetDataFormatId(hHeader);
            }
            set
            {
                LASError error = CAPI.LASHeader_SetDataFormatId(hHeader, value);
                if ((Int32)error != 0)
                {
                    LASException e = new LASException("Exception in Set Header DataFormatId.");
                    throw e;
                }
            }
        }

        /// <summary>
        /// return the record length for the points based on their data format id in bytes.
        /// </summary>
        public UInt16 DataRecordLength
        {
            get
            {
                return CAPI.LASHeader_GetDataRecordLength(hHeader);
            }
        }

        /// <summary>
        /// number of point records in the file.  
        /// </summary>
        /// <remarks>This value may not reflect the actual number of point
        /// records in the file.</remarks>
        public UInt32 PointRecordsCount
        {
            get
            {
                //Get total number of point records stored in the LAS file.
                return CAPI.LASHeader_GetPointRecordsCount(hHeader);
            }
            set
            {
                //Set number of point records that will be stored in a new LAS file.
                LASError error = CAPI.LASHeader_SetPointRecordsCount(hHeader, value);
                if ((Int32)error != 0)
                {
                    LASException e = new LASException("Exception in Set Header PointRecordsCount.");
                    throw e;
                }
            }
        }

        /// <summary>
        /// Returns the number of point records by return.
        /// </summary>
        /// <param name="index">the return number to fetch the count for</param>
        /// <returns>the number of point records for a given return</returns>
        public UInt32 GetPointRecordsByReturnCount(int index)
        {
            // Get array of the total point records per return.
            return CAPI.LASHeader_GetPointRecordsByReturnCount(hHeader, index);
        }

        //    /// Set values of 5-elements array of total point records per return.
        //    /// \exception std::out_of_range - if index is bigger than 4.
        //    /// \param index - subscript (0-4) of array element being updated.
        //    /// \param v - new value to assign to array element identified by index.
        /// <summary>
        /// Sets the number of point records for a given return
        /// </summary>
        /// <param name="index">the return number to set the count for</param>
        /// <param name="value">the number of point records for the return </param>
        public void SetPointRecordsByReturnCount(int index, UInt32 value)
        {
            LASError error = CAPI.LASHeader_SetPointRecordsByReturnCount(hHeader, index, value);
            if ((Int32)error != 0)
            {
                LASException e = new LASException("Exception in Set Header SetPointRecordsByReturnCount.");
                throw e;
            }
        }

        /// <summary>
        /// Get scale factor for X coordinate.
        /// </summary>
        /// <returns>scale factor for X coordinate.</returns>
        public double GetScaleX()
        {
            return CAPI.LASHeader_GetScaleX(hHeader);
        }

        /// <summary>
        /// Get scale factor for Y coordinate.
        /// </summary>
        /// <returns>scale factor for Y coordinate.</returns>
        public double GetScaleY()
        {
            return CAPI.LASHeader_GetScaleY(hHeader);
        }

        /// <summary>
        /// Get scale factor for Z coordinate.
        /// </summary>
        /// <returns>scale factor for Z coordinate.</returns>
        public double GetScaleZ()
        {
            return CAPI.LASHeader_GetScaleZ(hHeader);
        }

        /// <summary>
        /// Set values of scale factor for X, Y and Z coordinates.
        /// </summary>
        /// <param name="x">X scale factor of the coordinate</param>
        /// <param name="y">Y scale factor of the coordinate</param>
        /// <param name="z">Z scale factor of the coordinate</param>
        public void SetScale(double x, double y, double z)
        {
            LASError error = CAPI.LASHeader_SetScale(hHeader, x, y, z);
            if ((Int32)error != 0)
            {
                LASException e = new LASException("Exception in Set Header SetScale.");
                throw e;
            }
        }

        /// <summary>
        /// Get X coordinate offset.
        /// </summary>
        /// <returns>X coordinate offset.</returns>
        public double GetOffsetX()
        {
            return CAPI.LASHeader_GetOffsetX(hHeader);
        }

        /// <summary>
        /// Get Y coordinate offset.
        /// </summary>
        /// <returns>Y coordinate offset.</returns>
        public double GetOffsetY()
        {
            return CAPI.LASHeader_GetOffsetY(hHeader);
        }

        /// <summary>
        /// Get Z coordinate offset.
        /// </summary>
        /// <returns>Z coordinate offset.</returns>
        public double GetOffsetZ()
        {
            return CAPI.LASHeader_GetOffsetZ(hHeader);
        }

        //Set values of X, Y and Z coordinates offset.
        /// <summary>
        /// Set values of X, Y and Z coordinates offset.
        /// </summary>
        /// <param name="x">X coordinate offset.</param>
        /// <param name="y">Y coordinate offset.</param>
        /// <param name="z">Z coordinate offset.</param>
        public void SetOffset(double x, double y, double z)
        {
            LASError error = CAPI.LASHeader_SetOffset(hHeader, x, y, z);
            if ((Int32)error != 0)
            {
                LASException e = new LASException("Exception in Set Header SetOffset.");
                throw e;
            }
        }

        /// <summary>
        /// Get maximum value of extent of X coordinate.
        /// </summary>
        /// <returns>maximum value of extent of X coordinate.</returns>
        public double MaxX()
        {
            return CAPI.LASHeader_GetMaxX(hHeader);
        }

        /// <summary>
        /// Get maximum value of extent of Y coordinate.
        /// </summary>
        /// <returns>maximum value of extent of Y coordinate.</returns>
        public double GetMaxY()
        {
            return CAPI.LASHeader_GetMaxY(hHeader);
        }

        /// <summary>
        /// Get maximum value of extent of Z coordinate.
        /// </summary>
        /// <returns>maximum value of extent of Z coordinate.</returns>
        public double GetMaxZ()
        {
            return CAPI.LASHeader_GetMaxZ(hHeader);
        }

        /// <summary>
        /// Get minimum value of extent of X coordinate.
        /// </summary>
        /// <returns>minimum value of extent of X coordinate.</returns>
        public double GetMinX()
        {
            return CAPI.LASHeader_GetMinX(hHeader);
        }

        /// <summary>
        /// Get minimum value of extent of Y coordinate.
        /// </summary>
        /// <returns>minimum value of extent of Y coordinate.</returns>
        public double GetMinY()
        {
            return CAPI.LASHeader_GetMinY(hHeader);
        }

        /// <summary>
        /// Get minimum value of extent of Z coordinate.
        /// </summary>
        /// <returns>minimum value of extent of Z coordinate.</returns>
        public double GetMinZ()
        {
            return CAPI.LASHeader_GetMinZ(hHeader);
        }

        /// <summary>
        /// Set maximum values of extent of X, Y and Z coordinates.
        /// </summary>
        /// <param name="x">maximum value of extent of X coordinate.</param>
        /// <param name="y">maximum value of extent of Y coordinate.</param>
        /// <param name="z">maximum value of extent of Z coordinate.</param>
        public void SetMax(double x, double y, double z)
        {
            LASError error = CAPI.LASHeader_SetMax(hHeader, x, y, z);
            if ((Int32)error != 0)
            {
                LASException e = new LASException("Exception in Set Header SetMax.");
                throw e;
            }
        }

        /// <summary>
        /// Set minimum values of extent of X, Y and Z coordinates.
        /// </summary>
        /// <param name="x">Set minimum value of extent of X coordinate.</param>
        /// <param name="y">Set minimum value of extent of Y coordinate.</param>
        /// <param name="z">Set minimum value of extent of Z coordinate.</param>
        public void SetMin(double x, double y, double z)
        {
            LASError error = CAPI.LASHeader_SetMin(hHeader, x, y, z);
            if ((Int32)error != 0)
            {
                LASException e = new LASException("Exception in Set Header SetMin.");
                throw e;
            }
        }

        /// <summary>
        /// Adds a variable length record instance to the header. 
        /// </summary>
        /// <param name="variableLengthRecord">variable Length record instance to add</param>
        public void AddVariableLengthRecord(LASVariableLengthRecord variableLengthRecord)
        {
            LASError error = CAPI.LASHeader_AddVLR(hHeader, variableLengthRecord.GetPointer());
            if ((Int32)error != 0)
            {
                LASException e = new LASException("Exception in Header AddVariableLengthRecord.");
                throw e;
            }
        }

        /// <summary>
        /// Returns the variable length record for the given index.  
        /// </summary>
        /// <param name="i">the index starting from 0 of the variable length record to fetch</param>
        /// <returns>a new variable length record instance</returns>
        /// <remarks>Use VariableLengthRecordsCount property to determine the number of
        /// variable length records available on the header.</remarks>
        public LASVariableLengthRecord GetVariableLengthRecord(UInt32 i)
        {
            LASVLRH vlrh = CAPI.LASHeader_GetVLR(hHeader, i);
            return new LASVariableLengthRecord(vlrh);
        }

        /// <summary>
        /// Deletes a variable length record from the header for the given index.
        /// </summary>
        /// <param name="index">the index starting from 0 of the variable length record to delete</param>
        public void DeleteVariableLengthRecord(UInt32 index)
        {
            LASError error = CAPI.LASHeader_DeleteVLR(hHeader, index);
            if ((Int32)error != 0)
            {
                LASException e = new LASException("Exception in Header DeleteVariableLengthRecord.");
                throw e;
            }
        }

        /// <summary>
        /// GUID value for the header.
        /// </summary>
        /// <remarks>sets and gets the LASGuid instance of the header.</remarks>
        public LASGuid GUID
        {
            get
            {
                LASGuidH guidh = CAPI.LASHeader_GetGUID(hHeader);
                return new LASGuid(guidh);
            }
            set
            {

                LASError error = CAPI.LASHeader_SetGUID(hHeader, value.GetPointer());
                if ((Int32)error != 0)
                {
                    LASException e = new LASException("Exception in Set Header GUID.");
                    throw e;
                }
            }
        }

        /// <summary>
        /// sets and gets the Proj4 in the header.
        /// </summary>
        public string Proj4
        {
            get
            {
                return CAPI.LASHeader_GetProj4(hHeader);
            }
            set
            {
                LASError error = CAPI.LASHeader_SetProj4(hHeader, value);
                if ((Int32)error != 0)
                {
                    LASException e = new LASException("Exception in Set Header Proj4.");
                    throw e;
                }
            }
        }
    }
}
