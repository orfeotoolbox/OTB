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


class ReadProgram
{
    static void Main(string[] args)
    {
        try
        {

            string filename = @"..\..\..\..\..\test\data\TO_core_last_zoom.las";
            LASReader lasreader = new LASReader(filename);
            LASHeader h = lasreader.GetHeader();
            Console.WriteLine("Version  : " + lasreader.GetVersion());
            Console.WriteLine("Signature: : " + h.FileSignature);
            Console.WriteLine("Format   : " + h.DataFormatId);
            Console.WriteLine("Project  : " + h.ProjectId);
            Console.WriteLine("Points count: " + h.PointRecordsCount);
            Console.WriteLine("VLRecords count: " + h.VariableLengthRecordsCount);
            int counter = 0;
            while (lasreader.GetNextPoint())
            {
                LASPoint laspoint = lasreader.GetPoint();
                counter += 1;
            }

            if (lasreader.GetHeader().PointRecordsCount != counter)
                throw new LASException("read incorrect number of point records");


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

        Console.WriteLine("End of file");
        Console.Read();
    }
}
