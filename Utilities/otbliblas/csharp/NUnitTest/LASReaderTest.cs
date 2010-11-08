/******************************************************************************
 * $Id$
 *
 * Project:  libLAS - http://liblas.org - A BSD library for LAS format data.
 * Purpose:  Test cases of LASReader class, .NET/Mono bindings
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
using NUnit.Framework;
using LibLAS;

namespace NUnitTest
{
    [TestFixture]
    public class LASReaderTest
    {
        private string folder = @"C:\las\data";

        private string TO_core_last_zoom;

        [SetUp]
        protected void SetUp()
        {
            TO_core_last_zoom = folder + "\\TO_core_last_zoom.las";
        }

        [Test]
        public void hhhhh()
        {
            // LASReader lasreader = new LASReader(@"c:\las\sample_our2.las");
            LASReader lasreader = new LASReader(@"C:\las\data\TO_core_last_clip.las");

            LASPoint laspoint;

            //laspoint = lasreader.GetPointAt(0);
            //Console.WriteLine(laspoint.X + "," + laspoint.Y + "," + laspoint.Z + "  " + laspoint.Intensity );

            LASHeader lasheader = lasreader.GetHeader();

            LASWriter laswriter = new LASWriter(@"c:\las\sample_our.las", lasheader, LASReadWriteMode.LASModeWrite);

            //LASReader lasreader = new LASReader(@"C:\las\data\TO_core_last_clip.las");
            //LASPoint laspoint = new LASPoint();

            laspoint = lasreader.GetPointAt(0);
            //laspoint.X = 23.0;
            //Assert.AreEqual(laspoint.X, 23.0);
            //byte gg = laspoint.Classification;
            //bool d = lasreader.GetNextPoint();
            ////d = lasreader.GetNextPoint();
            //LASPoint point = lasreader.GetPointAt(1);
            //Assert.LessOrEqual(Math.Abs(point.X - 630262.30), 0.0001);
            //Assert.LessOrEqual(Math.Abs(point.Y - 4834500.0), 0.0001);
            //Assert.LessOrEqual(Math.Abs(point.Z - 51.53), 0.0001);
            Assert.AreEqual(laspoint.Intensity, 670);
            // Assert.AreEqual(laspoint.Classification,(byte) 1);

            //Assert.AreEqual(point.ScanAngleRank, 0);
            //Assert.AreEqual(point.UserData, 3);
            //Assert.AreEqual(point.ScanFlags, 9);
            //Assert.LessOrEqual(Math.Abs(point.Time - 413665.23360000004), 0.0001);
        }

        //[Test]
        //public void Point2()
        //{
        //    LASReader lasreader = new LASReader(@"C:\las\data\TO_core_last_clip.las");
        //   // LASHeader lasheader = lasreader.GetHeader();
        //    if (lasreader.GetNextPoint())
        //    {
        //        LASPoint point = lasreader.GetPointAt(2);
        //        Assert.LessOrEqual(Math.Abs(point.X - 630282.45), 0.0001);
        //        Assert.LessOrEqual(Math.Abs(point.Y - 4834500.0), 0.0001);
        //        Assert.LessOrEqual(Math.Abs(point.Z - 51.63), 0.0001);
        //        Assert.AreEqual(point.Intensity, 350);
        //        Assert.AreEqual(point.Classification, 1);
        //        Assert.AreEqual(point.ScanAngleRank, 0);
        //        Assert.AreEqual(point.UserData, 3);
        //        Assert.AreEqual(point.ScanFlags, 9);
        //        Assert.LessOrEqual(Math.Abs(point.Time - 413665.52880000003), 0.0001);
        //    }
        //}

    }
}
