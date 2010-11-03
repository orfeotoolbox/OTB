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
    /// LASGuid class
    /// </summary>
    public class LASGuid : IDisposable
    {
        private LASGuidH hguid;

        /// <summary>
        /// The object user should call this method when they finished with the object.
        /// </summary>
        /// 
        public void Dispose()
        {
            CAPI.LASGuid_Destroy(hguid);
            // Clean up unmanaged resources here.
            // Dispose other contained disposable objects.
        }

        /// <summary>
        /// Create a new random GUID.
        /// </summary>
        public LASGuid()
        {
            hguid = CAPI.LASGuid_Create();
        }

        /// <summary>
        ///  Creates a new GUID using a opaque pointer.  
        /// </summary>
        /// <param name="hGuid">LASGuidH opaque pointer</param>
        public LASGuid(LASGuidH hGuid)
        {
            hguid = hGuid;
        }

        /// <summary>
        /// Creates a new GUID opaque pointer using the given string.  
        /// </summary>
        /// <remarks>An example GUID might be something like '8388F1B8-AA1B-4108-BCA3-6BC68E7B062E'</remarks>
        /// <param name="guidString">string A GUID string in the form "00000000-0000-0000-0000-000000000000"</param>
        public LASGuid(String guidString)
        {
            hguid = CAPI.LASGuid_CreateFromString(guidString);
        }

        /// <summary>
        /// Gets the String value of the Guid
        /// </summary>
        /// <returns>the String value of the Guid</returns>
        public override string ToString()
        {
            return CAPI.LASGuid_AsString(hguid);
        }

        /// <summary>
        /// Gets the GUID opaque pointer
        /// </summary>
        /// <returns>the GUID opaque pointer</returns>
        internal LASGuidH GetPointer()
        {
            return hguid;
        }

        /// <summary>
        /// test if is equal to other object.
        /// </summary>
        /// <param name="obj">object to compare</param>
        /// <returns>true if both are equal</returns>
        public override bool Equals(object obj)
        {
            //if parameter is null return false
            if (obj == null)
            {
                return false;
            }

            //if parameter cannot be cast to Guid return false
            LASGuid guidd = obj as LASGuid;
            if ((object)guidd == null)
            {
                return false;
            }

            if (CAPI.LASGuid_Equals(hguid, guidd.GetPointer()) == 1)
            {
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
        public override int GetHashCode()
        {
            return base.GetHashCode();
        }

        /// <summary>
        /// test if is equal to other LASGuid.
        /// </summary>
        /// <param name="obj">LASGuid to compare</param>
        /// <returns>true if both are equal</returns>
        public bool Equals(LASGuid obj)
        {
            //if parameter cannot be cast to Guid return false

            if ((object)obj == null)
            {
                return false;
            }

            if (CAPI.LASGuid_Equals(hguid, obj.GetPointer()) == 1)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    }
}
