/******************************************************************************
 * $Id$
 *
 * Project:  libLAS - http://liblas.org - A BSD library for LAS format data.
 * Purpose:  Sample program demonstrating use of libLAS for .NET/Mono
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
using System.Text;
using LibLAS;

class Program
{
    static void Main(string[] args)
    {
        try
        {
            LASReader lasreader = new LASReader(@"c:\las\sample_our2.las");

            LASPoint laspoint;

            LASHeader lasheader = lasreader.GetHeader();
            Console.WriteLine(lasheader.SoftwareId);// 
            lasheader.VersionMinor = 0;
            LASWriter laswriter = new LASWriter(@"c:\las\sample_our.las", lasheader, LASReadWriteMode.LASModeWrite);

            Console.WriteLine("Number of points in file= {0}", lasheader.PointRecordsCount);

            while (lasreader.GetNextPoint())
            {
                laspoint = lasreader.GetPoint();
                laspoint.X = laspoint.X + 3;

                //Console.WriteLine(laspoint.X + "," + laspoint.Y + "," + laspoint.Z);

                laswriter.WritePoint(laspoint);
            }
        }
        catch (LASException e)
        {
            Console.WriteLine("\nLASException! Msg: {0}", e.Message);
        }
        catch (SystemException e)
        {
            Console.WriteLine("\nException! Msg: {0}", e.Message);
        }
        catch
        {
            Console.WriteLine("Unknown exception caught");
        }
        finally
        {
            Console.WriteLine("Do i need something to do?");
        }
        Console.WriteLine("End of file");
        Console.Read();
    }
}
