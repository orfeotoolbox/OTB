/******************************************************************************
 * $Id$
 *
 * Project:  libLAS - http://liblas.org - A BSD library for LAS format data.
 * Purpose:  Test cases of LASPoint class, .NET/Mono bindings
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
    public class LASPointTest
    {
        LASPoint point = new LASPoint();

        [SetUp]
        protected void SetUp()
        {
        }

        [Test]
        public void SetPointCoordinates()
        {
            LASPoint pointB = new LASPoint();
            pointB.X = 23.0;

            Assert.AreEqual(pointB.X, 23.0);

            Assert.IsFalse(point.Equals(pointB));
            pointB.X = 0.0;
            point.X = 0.0;
            Assert.IsTrue(point.Equals(point));

            pointB.X = 1.123;
            pointB.Y = 2.456;
            pointB.Z = 3.789;
            LASPoint pointC = new LASPoint();
            pointC.X = 1.123;
            pointC.Y = 2.456;
            pointC.Z = 3.789;
            Assert.IsTrue(pointC.Equals(pointB));
        }

        [Test]
        public void PointValid()
        {
            LASPoint pointB = new LASPoint();
            pointB.X = 23.0;

            Assert.IsTrue(point.IsValid());
            Assert.IsTrue(pointB.IsValid());
        }

        [Test]
        public void ReturnNumber()
        {
            Assert.AreEqual(point.ReturnNumber, 0);

            UInt16 num1 = 7;
            point.ReturnNumber = num1;
            Assert.AreEqual(point.ReturnNumber, num1);

            UInt16 num2 = 3;
            point.ReturnNumber = num2;
            Assert.AreEqual(point.ReturnNumber, num2);

            try
            {
                //out of range
                point.ReturnNumber = 8;
                Assert.IsTrue(point.Validate());
            }
            catch (LASException e)
            {
                Assert.AreEqual((int)e.Data["DataMemberFlag"] & (int)LASPoint.DataMemberFlag.eReturnNumber,
                    (int)LASPoint.DataMemberFlag.eReturnNumber);
            }
        }

        [Test]
        public void NumberOfReturns()
        {
            Assert.AreEqual(point.NumberOfReturns, 0);

            UInt16 num1 = 7;
            point.NumberOfReturns = num1;
            Assert.AreEqual(point.NumberOfReturns, num1);

            UInt16 num2 = 3;
            point.NumberOfReturns = num2;
            Assert.AreEqual(point.NumberOfReturns, num2);

            try
            {
                //out of range
                point.NumberOfReturns = 8;
                Assert.IsTrue(point.Validate());
            }
            catch (LASException e)
            {
                Assert.AreEqual((int)e.Data["DataMemberFlag"] & (int)LASPoint.DataMemberFlag.eNumberOfReturns,
                    (int)LASPoint.DataMemberFlag.eNumberOfReturns);
            }
        }

        [Test]
        public void ScanDirection()
        {
            //Assert.AreEqual(point.ScanDirection, 0);

            //UInt16 num1 = 0;
            //point.ScanDirection = num1;
            //Assert.AreEqual(point.ScanDirection, num1);
            //UInt16 num2 = 1;
            //point.ScanDirection = num2;
            //Assert.AreEqual(point.ScanDirection, num2);

            //try
            //{
            //    //out of range
            //    point.ScanDirection = 2;
            //    Assert.IsTrue(point.Validate());
            //}
            //catch (LASException e)
            //{
            //    Assert.AreEqual((int)e.Data["DataMemberFlag"] & (int)LASPoint.DataMemberFlag.eScanDirection,
            //        (int)LASPoint.DataMemberFlag.eScanDirection);
            //}
        }

        [Test]
        public void FlightLineEdge()
        {
            Assert.AreEqual(point.FlightLineEdge, 0);

            UInt16 num1 = 0;
            point.FlightLineEdge = num1;
            Assert.AreEqual(point.FlightLineEdge, num1);

            UInt16 num2 = 1;
            point.FlightLineEdge = num2;
            Assert.AreEqual(point.FlightLineEdge, num2);

            try
            {
                //out of range
                point.FlightLineEdge = 2;

                Assert.IsTrue(point.Validate());
            }
            catch (LASException e)
            {
                Assert.AreEqual((int)e.Data["DataMemberFlag"] & (int)LASPoint.DataMemberFlag.eFlightLineEdge,
                    (int)LASPoint.DataMemberFlag.eFlightLineEdge);
            }
        }

        [Test]
        public void ScanFlags()
        {
            byte zeros = 0;
            LASPoint pointS = new LASPoint();
            Assert.AreEqual(pointS.ScanFlags, zeros);
            pointS.ReturnNumber = 3;
            pointS.NumberOfReturns = 7;
            pointS.ScanDirection = 0;
            pointS.FlightLineEdge = 1;
            string expected = "10111011";
            byte bits = pointS.ScanFlags;
            Assert.AreEqual(Convert.ToString(pointS.ScanFlags, 2), (expected));

            LASPoint pointN;
            pointN = pointS.Copy();

            Assert.AreEqual(pointN.ScanFlags, pointS.ScanFlags);
        }

        [Test]
        public void Classification()
        {
            Assert.AreEqual(point.Classification, 0);

            byte num1 = 0;
            point.Classification = num1;
            Assert.AreEqual(point.Classification, num1);

            byte num2 = 31;
            point.Classification = num2;
            Assert.AreEqual(point.Classification, num2);

            //try
            //{
            //    //out of range
            //    point.Classification = 2;

            //    Assert.IsTrue(point.Validate());
            //}
            //catch (LASException e)
            //{
            //    Assert.AreEqual((int)e.Data["DataMemberFlag"] & (int)LASPoint.DataMemberFlag.eClassification,
            //        (int)LASPoint.DataMemberFlag.eClassification);
            //}
        }

        [Test]
        public void ScanAngleRank()
        {
            Assert.AreEqual(point.ScanAngleRank, 0);

            SByte num1 = 90;
            point.ScanAngleRank = num1;
            Assert.AreEqual(point.ScanAngleRank, num1);

            SByte num2 = -90;
            point.ScanAngleRank = num2;
            Assert.AreEqual(point.ScanAngleRank, num2);

            try
            {
                //out of range
                point.ScanAngleRank = 91;

                Assert.IsTrue(point.Validate());
            }
            catch (LASException e)
            {
                Assert.AreEqual((int)e.Data["DataMemberFlag"] & (int)LASPoint.DataMemberFlag.eScanAngleRank,
                    (int)LASPoint.DataMemberFlag.eScanAngleRank);
            }
        }

        [Test]
        public void UserData()
        {
            Assert.AreEqual(point.UserData, 0);

            byte num1 = 7;
            point.UserData = num1;
            Assert.AreEqual(point.UserData, num1);
        }
    }
}
