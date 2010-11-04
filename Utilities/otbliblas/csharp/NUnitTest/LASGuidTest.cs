/******************************************************************************
 * $Id$
 *
 * Project:  libLAS - http://liblas.org - A BSD library for LAS format data.
 * Purpose:  Test cases of LASGuid class, .NET/Mono bindings
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
    public class LASGuidTest
    {
        // LASPoint.

        [SetUp]
        protected void SetUp()
        {
        }

        [Test]
        public void ConstructionfromString()
        {
            LASGuid guid1 = new LASGuid("00000000-0000-0000-0000-000000000000");

            LASGuid guid2 = new LASGuid("3F2504E0-4F89-11D3-9A0C-0305E82C3301");

            Assert.AreNotSame(guid1, guid2);
            Assert.AreEqual(guid1.ToString(), "00000000-0000-0000-0000-000000000000");
            Assert.AreEqual(guid2.ToString().ToUpper(), "3F2504E0-4F89-11D3-9A0C-0305E82C3301");
            Assert.IsNotNull(guid1);
            Assert.IsNotNull(guid2);
        }

        [Test]
        public void Equals()
        {
            LASGuid guid1 = new LASGuid("3F2504E0-4F89-11D3-9A0C-0305E82C3301");
            LASGuid guid2 = new LASGuid("3F2504E0-4F89-11D3-9A0C-0305E82C3301");

            Assert.IsTrue(guid1.Equals(guid2));
        }
    }
}
