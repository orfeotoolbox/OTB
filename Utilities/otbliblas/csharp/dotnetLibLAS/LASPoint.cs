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
    /// LASPoint class
    /// </summary>
    public class LASPoint : IDisposable
    {
        /// <summary>
        /// Returns a bitfield representing the validity of various members.
        /// </summary>
        public enum DataMemberFlag
        {
            /// <summary>
            /// ReturnNumber property flag.
            /// </summary>
            eReturnNumber = 1,
            /// <summary>
            /// NumberOfReturns property flag.
            /// </summary>
            eNumberOfReturns = 2,
            /// <summary>
            /// ScanDirection property flag.
            /// </summary>
            eScanDirection = 4,
            /// <summary>
            /// FlightLineEdge property flag.
            /// </summary>
            eFlightLineEdge = 8,
            /// <summary>
            /// Classification property flag.
            /// </summary>
            eClassification = 16,
            /// <summary>
            /// ScanAngleRank property flag.
            /// </summary>
            eScanAngleRank = 32,
            /// <summary>
            /// Time property flag.
            /// </summary>
            eTime = 64
        };

        private LASPointH hPoint;

        /// <summary>
        /// Create a new LASPoint from the LASPointH opaque structure
        /// </summary>
        /// <param name="hLASPoint">LASPointH opaque structure</param>
        public LASPoint(LASPointH hLASPoint)
        {
            hPoint = hLASPoint;
        }

        /// <summary>
        /// Create a generic LASPoint
        /// </summary>
        public LASPoint()
        {
            hPoint = CAPI.LASPoint_Create();
        }

        /// <summary>
        /// Compare 2 LASPoint to be equal
        /// </summary>
        /// <param name="lasPoint">LASPoint object</param>
        /// <returns>true if lasPoint is equals to the instance.</returns>
        public bool Equals(LASPoint lasPoint)
        {
            if (X == lasPoint.X && Y == lasPoint.Y && Z == lasPoint.Z)
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        /// <summary>
        /// test is the LASPoint is Valid
        /// </summary>
        /// <returns>true is is valid</returns>
        public bool IsValid()
        {
            if (CAPI.LASPoint_IsValid(hPoint) == 1)
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        /// <summary>
        /// The object user should call this method when they finished with the object. In .NET is magaged by the GC.
        /// </summary>
        public void Dispose()
        {
            CAPI.LASPoint_Destroy(hPoint);
            // Clean up unmanaged resources here.
            // Dispose other contained disposable objects.
        }

        enum ClassificationType
        {
            eCreated = 0,
            eUnclassified,
            eGround,
            eLowVegetation,
            eMediumVegetation,
            eHighVegetation,
            eBuilding,
            eLowPoint,
            eModelKeyPoint,
            eWater = 9,
            // = 10 // reserved for ASPRS Definition
            // = 11 // reserved for ASPRS Definition
            eOverlapPoints = 12
            // = 13-31 // reserved for ASPRS Definition
        };

        enum ScanAngleRankRange
        {
            eScanAngleRankMin = -90,
            eScanAngleRankMax = 90
        };

        /// <summary>
        /// gets the opaque pointer to the LASPointH instance.
        /// </summary>
        /// <returns>the opaque pointer to the LASPointH instance</returns>
        internal LASPointH GetPointer()
        {
            return hPoint;
        }

        /// <summary>
        /// Copy a LASPoint object creating a new one.
        /// </summary>
        /// <returns>a new LASPoint instance copied.</returns>
        public LASPoint Copy()
        {
            LASPointH laspointhTemp = CAPI.LASPoint_Copy(hPoint);
            return new LASPoint(laspointhTemp);
        }

        /// <summary>
        ///
        /// </summary>
        /// <returns></returns>
        public bool Validate()
        {
            int error = CAPI.LASPoint_Validate(hPoint);

            if (error != 0)
            {
                LASException e = new LASException("Point data members out of range.");
                e.Data.Add("DataMemberFlag", error);
                throw e;
            }

            return true;
        }

        /// <summary>
        /// X value for the point.
        /// </summary>
        /// <remarks>This value must be scaled or offset by any header values before being set.</remarks>
        public double X
        {
            get
            {
                return CAPI.LASPoint_GetX(hPoint);
            }
            set
            {
                LASError error = CAPI.LASPoint_SetX(hPoint, value);
                if ((Int32)error != 0)
                {
                    LASException e = new LASException("Exception in Set Point X.");
                    throw e;
                }
            }
        }

        /// <summary>
        /// Y value for the point.
        /// </summary>
        /// <remarks>This value must be scaled or offset by any header values before being set.</remarks>
        public double Y
        {
            get
            {
                return CAPI.LASPoint_GetY(hPoint);
            }
            set
            {
                LASError error = CAPI.LASPoint_SetY(hPoint, value);
                if ((Int32)error != 0)
                {
                    LASException e = new LASException("Exception in Set Point Y.");
                    throw e;
                }
            }
        }

        /// <summary>
        /// Z value for the point.
        /// </summary>
        /// <remarks>This value must be scaled or offset by any header values before being set.</remarks>
        /// 
        public double Z
        {
            get
            {
                return CAPI.LASPoint_GetZ(hPoint);
            }
            set
            {
                LASError error = CAPI.LASPoint_SetZ(hPoint, value);
                if ((Int32)error != 0)
                {
                    LASException e = new LASException("Exception in Set Point Z.");
                    throw e;
                }
            }
        }

        /// <summary>
        ///  intensity value for the point.
        /// </summary>
        /// <remarks>This value is the pulse return magnitude, it is optional, and it is LiDAR system specific. 
        /// </remarks>
        public UInt16 Intensity
        {
            get
            {
                return CAPI.LASPoint_GetIntensity(hPoint);
            }
            set
            {
                LASError error = CAPI.LASPoint_SetIntensity(hPoint, value);
                if ((Int32)error != 0)
                {
                    LASException e = new LASException("Exception in Set Point Intensity.");
                    throw e;
                }
            }
        }

        /// <summary>
        ///  scan flags for the point -- Return number, number of returns, flightline edge, scan direction, and scan angle rank.
        /// </summary>
        /// <remarks></remarks>
        public byte ScanFlags
        {
            get
            {
                return CAPI.LASPoint_GetScanFlags(hPoint);
            }
            set
            {
                LASError error = CAPI.LASPoint_SetScanFlags(hPoint, value);
                if ((Int32)error != 0)
                {
                    LASException e = new LASException("Exception in Set Point ScanFlags.");
                    throw e;
                }

            }
        }

        /// <summary>
        ///  return number for the point.
        /// </summary>
        /// <remarks>The return number is "the pulse return number for a given output pulse."  The first return number starts with the value 1. valid from 1 to 6.</remarks>
        public UInt16 ReturnNumber
        {
            get
            {
                return CAPI.LASPoint_GetReturnNumber(hPoint);
            }
            set
            {
                LASError error = CAPI.LASPoint_SetReturnNumber(hPoint, value);
                if ((Int32)error != 0)
                {
                    LASException e = new LASException("Exception in Set Point ReturnNumber.");
                    throw e;
                }
            }
        }

        /// <summary>
        ///  total number of returns for a given pulse.
        /// </summary>
        /// <remarks>Valid values are from 1-5. </remarks>
        public UInt16 NumberOfReturns
        {
            get
            {
                return CAPI.LASPoint_GetNumberOfReturns(hPoint);
            }
            set
            {
                LASError error = CAPI.LASPoint_SetNumberOfReturns(hPoint, value);
                if ((Int32)error != 0)
                {
                    LASException e = new LASException("Exception in Set Point NumberOfReturns.");
                    throw e;
                }
            }
        }

        /// <summary>
        /// scan direction for a given pulse.
        /// </summary>
        /// <remarks>Valid values are 0 or 1, with 1 being a positive scan direction and 0 being a negative scan direction.</remarks>
        public UInt16 ScanDirection
        {
            get
            {
                return CAPI.LASPoint_GetScanDirection(hPoint);
            }
            set
            {
                LASError error = CAPI.LASPoint_SetScanDirection(hPoint, value);
                if ((Int32)error != 0)
                {
                    LASException e = new LASException("Exception in Set Point ScanDirection.");
                    throw e;
                }
            }
        }

        /// <summary>
        /// whether or not a given pulse is an edge point.
        /// </summary>
        /// <remarks>Valid values are 0 or 1, with 1 being an edge point and 0 being interior.</remarks>
        public UInt16 FlightLineEdge
        {
            get
            {
                return CAPI.LASPoint_GetFlightLineEdge(hPoint);
            }
            set
            {
                LASError error = CAPI.LASPoint_SetFlightLineEdge(hPoint, value);
                if ((Int32)error != 0)
                {
                    LASException e = new LASException("Exception in Set Point FlightLineEdge.");
                    throw e;
                }
            }
        }

        /// <summary>
        /// classification for the point.
        /// </summary>
        public byte Classification
        {
            get
            {
                return CAPI.LASPoint_GetClassification(hPoint);
            }
            set
            {
                LASError error = CAPI.LASPoint_SetClassification(hPoint, value);
                if ((Int32)error != 0)
                {
                    LASException e = new LASException("Exception in Set Point Classification.");
                    throw e;
                }
            }
        }

        /// <summary>
        /// the scan angle for the point.
        /// </summary>
        public SByte ScanAngleRank
        {
            get
            {
                return CAPI.LASPoint_GetScanAngleRank(hPoint);
            }
            set
            {

                LASError error = CAPI.LASPoint_SetScanAngleRank(hPoint, value);
                if ((Int32)error != 0)
                {
                    LASException e = new LASException("Exception in Set Point ScanAngleRank.");
                    throw e;
                }
            }
        }

        /// <summary>
        /// arbitrary user data for the point.
        /// </summary>
        public byte UserData
        {
            get
            {
                return CAPI.LASPoint_GetUserData(hPoint);
            }
            set
            {
                LASError error = CAPI.LASPoint_SetUserData(hPoint, value);
                if ((Int32)error != 0)
                {
                    LASException e = new LASException("Exception in Set Point UserData.");
                    throw e;
                }
            }
        }

        /// <summary>
        ///  time for the point.
        /// </summary>
        public double Time
        {
            get
            {
                return CAPI.LASPoint_GetTime(hPoint);
            }
            set
            {
                LASError error = CAPI.LASPoint_SetTime(hPoint, value);
                if ((Int32)error != 0)
                {
                    LASException e = new LASException("Exception in Set Point Time.");
                    throw e;
                }
            }
        }
    }
}
