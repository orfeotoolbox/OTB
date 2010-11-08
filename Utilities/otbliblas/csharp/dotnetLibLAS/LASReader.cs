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
    /// LASReader class
    /// </summary>
    public class LASReader : IDisposable
    {
        private LASReaderH hReader;
        private LASPoint laspoint;

        /// <summary>
        /// Creates a LASReaderH object that can be used to read LASHeaderH and LASPointH objects with.
        /// </summary>
        /// <remarks>The LASReaderH must not be created with a filename that is opened for read or write by any other API functions. </remarks>
        /// <param name="filename">filename to open for read</param>
        public LASReader(String filename)
        {
            hReader = CAPI.LASReader_Create(filename);
        }

        /// <summary>
        /// Reads the next available point on the LASReaderH instance. 
        /// </summary>
        /// <returns>true if we have next point</returns>
        public bool GetNextPoint()
        {
            IntPtr pointer = CAPI.LASReader_GetNextPoint(hReader);

            if (IntPtr.Zero != pointer)
            {
                laspoint = new LASPoint(pointer);
                return true;
            }
            else
            {
                return false;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        public string GetVersion()
        {
            return CAPI.LAS_GetVersion();
        }
       
        /// <summary>
        /// get the current LASPoint.
        /// </summary>
        /// <returns>current LASPoint object</returns>
        public LASPoint GetPoint()
        {
            return laspoint;
        }

        /// <summary>
        /// Reads a LASPointH from the given position in the LAS file represented by the LASReader instance.
        /// </summary>
        /// <remarks> If no point is available at that location, NULL is returned. </remarks>
        /// <param name="position">the integer position of the point in the file to read.</param>
        /// <returns>LASPoint object</returns>
        public LASPoint GetPointAt(UInt32 position)
        {
            return new LASPoint(CAPI.LASReader_GetPointAt(hReader, position));
        }

        /// <summary>
        /// Get the header for the file associated with this Reader Class.
        /// </summary>
        /// <returns>LASHeader representing the header for the file.</returns>
        public LASHeader GetHeader()
        {
            return new LASHeader(CAPI.LASReader_GetHeader(hReader));
        }

        /// <summary>
        /// The object user should call this method when they finished with the object. In .NET is magaged by the GC.
        /// </summary>
        public void Dispose()
        {
            CAPI.LASReader_Destroy(hReader);
            // Clean up unmanaged resources here.
            // Dispose other contained disposable objects.
        }
    }
}
