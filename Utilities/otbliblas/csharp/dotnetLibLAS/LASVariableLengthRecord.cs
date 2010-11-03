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
    /// LASVariableLengthRecord class
    /// </summary>
    public class LASVariableLengthRecord : IDisposable
    {
        private LASVLRH hvlrh;

        /// <summary>
        /// The object user should call this method when they finished with the object.
        /// </summary>
        public void Dispose()
        {
            CAPI.LASVLR_Destroy(hvlrh);
            // Clean up unmanaged resources here.
            // Dispose other contained disposable objects.
        }

        /// <summary>
        /// Default constructor.
        /// </summary>
        public LASVariableLengthRecord()
        {
            hvlrh = CAPI.LASVLR_Create();
        }

        /// <summary>
        /// Creates a new variable length record uing the LASVLRH opaque pointer
        /// </summary>
        /// <param name="hVlrh">LASVLRH opaque pointer</param>
        public LASVariableLengthRecord(LASVLRH hVlrh)
        {
            hvlrh = hVlrh;
        }

        //public override string ToString()
        //{
        //    return CAPI.LASGuid_AsString(hguid);
        //}

        /// <summary>
        /// Get the LASVLRH opaque pointer.
        /// </summary>
        /// <returns>LASVLRH opaque pointer </returns>
        public LASVLRH GetPointer()
        {
            return hvlrh;
        }

        /// <summary>
        /// the User Id for the variable length record.
        /// </summary>
        /// <remarks> It will be clipped to fit within 16 characters</remarks>
        public string UserId
        {
            get
            {
                return CAPI.LASVLR_GetUserId(hvlrh);
            }

            set
            {
                LASError error = CAPI.LASVLR_SetUserId(hvlrh, value);
                if ((Int32)error != 0)
                {
                    LASException e = new LASException("Exception in Set VLR UserId.");
                    throw e;
                }
            }
        }

        /// <summary>
        /// the description for the variable length record.
        /// </summary>
        /// <remarks> It will be clipped to fit within 32 characters</remarks>
        public string Description
        {
            get
            {
                return CAPI.LASVLR_GetDescription(hvlrh);
            }

            set
            {
                LASError error = CAPI.LASVLR_SetDescription(hvlrh, value);
                if ((Int32)error != 0)
                {
                    LASException e = new LASException("Exception in Set VLR Description.");
                    throw e;
                }
            }
        }

        /// <summary>
        /// the record length of the data stored in the variable length record.
        /// </summary>
        public UInt16 RecordLength
        {
            get
            {
                return CAPI.LASVLR_GetRecordLength(hvlrh);
            }

            set
            {
                LASError error = CAPI.LASVLR_SetRecordLength(hvlrh, value);
                if ((Int32)error != 0)
                {
                    LASException e = new LASException("Exception in Set VLR RecordLength.");
                    throw e;
                }
            }
        }

        /// <summary>
        /// the record id for the variable length record.
        /// </summary>
        public UInt16 RecordId
        {
            get
            {
                return CAPI.LASVLR_GetRecordId(hvlrh);
            }

            set
            {
                LASError error = CAPI.LASVLR_SetRecordId(hvlrh, value);
                if ((Int32)error != 0)
                {
                    LASException e = new LASException("Exception in Set VLR RecordId.");
                    throw e;
                }
            }
        }

        /// <summary>
        /// the reserved value of the variable length record.  
        /// </summary>
        /// <remarks>This should be 0 and should always be 0.</remarks>
        public UInt16 Reserved
        {
            get
            {
                return CAPI.LASVLR_GetReserved(hvlrh);
            }

            set
            {
                LASError error = CAPI.LASVLR_SetReserved(hvlrh, value);
                if ((Int32)error != 0)
                {
                    LASException e = new LASException("Exception in Set VLR Reserved.");
                    throw e;
                }
            }
        }

        /// <summary>
        /// Gets the data stream for the variable length record as an array of bytes
        /// </summary>
        /// <param name="data">a empty array of bytes where place the array</param>
        public void GetData(out byte[] data)
        {
            LASError error = CAPI.LASVLR_GetData(hvlrh, out data);
            if ((Int32)error != 0)
            {
                LASException e = new LASException("Exception in VLR GetData.");
                throw e;
            }
        }

        /// <summary>
        /// Sets the data stream for the variable length record as an array of bytes
        /// </summary>
        /// <param name="data">array of bytes</param>
        public void SetData(ref byte[] data)
        {
            UInt16 lenght = (UInt16)data.Length;
            LASError error = CAPI.LASVLR_SetData(hvlrh, ref data, lenght);
            if ((Int32)error != 0)
            {
                LASException e = new LASException("Exception in VLR SetData.");
                throw e;
            }
        }
    }
}
