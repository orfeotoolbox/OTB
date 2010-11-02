/******************************************************************************
 * $Id$
 *
 * Project:  libLAS - http://liblas.org - A BSD library for LAS format data.
 * Purpose:  Test cases of LASHeader class, .NET/Mono bindings
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
    public class LASHeaderTest
    {
        [SetUp]
        protected void SetUp()
        {
        }

        [Test]
        public void CopyConstructor()
        {
            LASHeader header = new LASHeader();
            //  string sig = "LASF and garbage";
            //    header.FileSignature = sig;
            //    Assert.AreEqual(header.FileSignature.Length,4);
            //       LASHeader h1;
            Assert.AreEqual(1, 1);
            //h1.SetFileSignature(sig);
            //ensure_not(h1.GetFileSignature() == sig);
            //ensure_equals(h1.GetFileSignature().size(), 4);
            //ensure_equals(h1.GetFileSignature(), LASHeader::FileSignature);

            //LASHeader h2(h1);

            //ensure_not(h2.GetFileSignature() == sig);
            //ensure_equals(h2.GetFileSignature().size(), 4);
            //ensure_equals(h2.GetFileSignature(), LASHeader::FileSignature);
        }

        [Test]
        public void FileSignature()
        {
            LASHeader header = new LASHeader();
            // string sig = "LASF and garbage";

            Assert.AreEqual(header.FileSignature.Length, 4);
            Assert.AreEqual(header.FileSignature, "LASF");

            // I can not set FileSignature from c# API because i am a bad guy.
            //header.FileSignature = "LASF";
        }

        [Test]
        public void FileSourceId()
        {
            LASHeader h1 = new LASHeader();

            UInt16 id1 = 1;
            UInt16 id2 = 65535;
            UInt16 overflowed = 0;

            h1.FileSourceId = id1;
            Assert.AreEqual(h1.FileSourceId, id1);
            h1.FileSourceId = id2;
            Assert.AreEqual(h1.FileSourceId, id2);

            // Unsigned overflow
            // Likely compiler warning: truncation from int to liblas::uint16_t
            h1.FileSourceId = (ushort)(id2 + 1);
            Assert.AreEqual(h1.FileSourceId, overflowed);
        }

        [Test]
        public void Reserved()
        {
            LASHeader h = new LASHeader();

            Assert.AreEqual(h.Reserved, 0);
        }

        [Test]
        public void ProjectId()
        {
            LASHeader h = new LASHeader();

            //string strid="030B4A82-1B7C-11CF-9D53-00AA003C9CB6";
            //      Assert.AreEqual(h.Reserved, 0);
            //      liblas::guid id(strid.c_str());
            //      h.ProjectId=;
            //    std::string strid("030B4A82-1B7C-11CF-9D53-00AA003C9CB6");
            //liblas::guid id(strid.c_str());

            //liblas::LASHeader h;
            //h.SetProjectId(id);

            //ensure_not(h.GetProjectId().is_null());
            //ensure_equals(h.GetProjectId(), id);
        }

        [Test]
        public void MinorMajorVersion()
        {
            LASHeader h = new LASHeader();
            h.VersionMinor = 0;
            h.VersionMajor = 1;
            Assert.AreEqual(h.VersionMinor, 0);
            Assert.AreEqual(h.VersionMajor, 1);

            h.VersionMinor = 1;
            h.VersionMajor = 1;
            Assert.AreEqual(h.VersionMinor, 1);
            Assert.AreEqual(h.VersionMajor, 1);

            try
            {
                h.VersionMajor = 2;
                //don´t get the next line...
                Assert.AreEqual(2, 5);
            }
            catch (LASException e)
            {
                Assert.AreEqual(e.Message, "Exception in Set Header VersionMajor.");
            }

            try
            {
                h.VersionMinor = 2;
                //don´t get the next line...
                Assert.AreEqual(2, 5);
            }
            catch (LASException e)
            {
                Assert.AreEqual(e.Message, "Exception in Set Header VersionMinor.");
            }
        }

        [Test]
        public void SoftwareId()
        {
            LASHeader h = new LASHeader();

            string softid1 = "Short Soft Id"; // 13 bytes
            int len1 = softid1.Length;
            string softid2 = "Long Software Identifier - XX YY"; // 32 bytes
            int len2 = softid2.Length;

            h.SoftwareId = softid1;
            Assert.AreEqual(h.SoftwareId, softid1);
            Assert.AreEqual(h.SoftwareId.Length, len1);
            // Assert.AreEqual(h.GetSoftwareId(true).size(), 32);

            h.SoftwareId = softid2;
            Assert.AreEqual(h.SoftwareId, softid2);
            Assert.AreEqual(h.SoftwareId.Length, len2);
            // Assert.AreEqual(h.GetSoftwareId(true).size(), 32);
        }

        [Test]
        public void PointRecordsByReturnCount()
        {
            LASHeader h = new LASHeader();
            // Assert.AreEqual(h.GetPointRecordsByReturnCount(5),5);

            h.SetPointRecordsByReturnCount(0, 100);
            //ensure_equals(h.GetPointRecordsByReturnCount().size(), 5);
            Assert.AreEqual(h.GetPointRecordsByReturnCount(0), 100);

            h.SetPointRecordsByReturnCount(1, 101);
            //ensure_equals(h.GetPointRecordsByReturnCount().size(), 5);
            Assert.AreEqual(h.GetPointRecordsByReturnCount(1), 101);

            h.SetPointRecordsByReturnCount(2, 102);
            //ensure_equals(h.GetPointRecordsByReturnCount().size(), 5);
            Assert.AreEqual(h.GetPointRecordsByReturnCount(2), 102);

            h.SetPointRecordsByReturnCount(3, 103);
            //ensure_equals(h.GetPointRecordsByReturnCount().size(), 5);
            Assert.AreEqual(h.GetPointRecordsByReturnCount(3), 103);

            h.SetPointRecordsByReturnCount(4, 104);
            //ensure_equals(h.GetPointRecordsByReturnCount().size(), 5);
            Assert.AreEqual(h.GetPointRecordsByReturnCount(4), 104);

            try
            {
                // 5 is out of range
                h.SetPointRecordsByReturnCount(5, 500);
                //don´t get the next line...
                Assert.AreEqual(2, 5);
            }
            catch (LASException e)
            {
                Assert.AreEqual(e.Message, "Exception in Set Header SetPointRecordsByReturnCount.");
            }
        }
    }
}
