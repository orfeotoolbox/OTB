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
    /// LASReadWriteMode enum
    /// </summary>
    public enum LASReadWriteMode
    {
        /// <summary>
        /// Mode of Read
        /// </summary>
        LASModeRead = 0,
        /// <summary>
        /// Mode of Write
        /// </summary>
        LASModeWrite = 1,
        /// <summary>
        /// Mode of ppend
        /// </summary>
        LASModeAppend = 2
    }

    /// <summary>
    /// LASWriter class
    /// </summary>
    public class LASWriter : IDisposable
    {
        private LASWriterH hwriter;

        /// <summary>
        /// The object user should call this method when they finished with the object.
        /// </summary>
        public void Dispose()
        {
            CAPI.LASWriter_Destroy(hwriter);
            // Clean up unmanaged resources here.
            // Dispose other contained disposable objects.
        }

        /// <summary>
        /// Default constructor of the class
        /// </summary>
        /// <param name="filename">string with the path of the LAS file</param>
        /// <param name="hHeader">LASHeader to add the LAS file</param>
        /// <param name="mode">mode to use the file by LASReadWriteMode enumeration</param>
        public LASWriter(String filename, LASHeader hHeader, LASReadWriteMode mode)
        {
            hwriter = CAPI.LASWriter_Create(filename, hHeader.GetPointer(), (int)mode);
        }

        /// <summary>
        /// Write a new point in the LAS file
        /// </summary>
        /// <param name="point">LASPoint to write in the file</param>
        public void WritePoint(LASPoint point)
        {
            LASError error = CAPI.LASWriter_WritePoint(hwriter, point.GetPointer());

            if ((Int32)error != 0)
            {
                LASException e = new LASException("Exception in Set Writer WritePoint.");
                throw e;
            }
        }
    }
}
