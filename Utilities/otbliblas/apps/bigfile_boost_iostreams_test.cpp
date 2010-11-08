/******************************************************************************
 *
 * Project:  libLAS - http://liblas.org - A BSD library for LAS format data.
 * Purpose:  Test reading and writing of large LAS files (>4GB)
 *           using portable Boost.IOStreams library.
 * Author:   Mateusz Loskot, mateusz@loskot.net
 *
 ******************************************************************************
 * Copyright (c) 2010, Mateusz Loskot
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
#include <liblas/laspoint.hpp>
#include <liblas/lasreader.hpp>
#include <liblas/laswriter.hpp>
#ifdef _MSC_VER
#  pragma warning(push)
#  pragma warning(disable: 4702)
#endif
#include <boost/iostreams/device/file_descriptor.hpp>
#include <boost/iostreams/positioning.hpp>
#include <boost/iostreams/stream.hpp>
#ifdef _MSC_VER
#  pragma warning(pop)
#endif()
#include <cassert>
#include <exception>
#include <iostream>
#include <sstream>
#include <stdexcept>

namespace bio = boost::iostreams;
namespace las = liblas;

int main()
{
    try
    {
        // name of generated file
        std::string filename("bigfile_bio_test.las");
        // test writing mor than 204 million points
        bio::stream_offset const n_million_points = 210;
        bio::stream_offset const point_count = 1024 * 1024 * n_million_points;

        std::cout.setf(std::ios::fixed, std::ios::floatfield);
        std::cout.setf(std::ios::showpoint);
        std::cout.precision(2);

        std::cout << "LAS file: " << filename << std::endl;
        std::cout << "Writing " << point_count << " points" << std::endl;
        {
            typedef bio::stream<bio::file_descriptor_sink> bio_ostream;
            bio_ostream bigofs(filename);
            las::Header header;
            las::Writer writer(bigofs, header);

            las::Point empty_point;
            bio::stream_offset i = 0;
            for (i = 0; i < point_count; ++i)
            {
                if (!writer.WritePoint(empty_point))
                {
                    std::ostringstream oss;
                    oss << "failed to write point #" << i;
                    throw std::runtime_error(oss.str());
                }

                if (i % 1000 == 0)
                {
                    std::cout << "\b\b\b\b\b\b\b\b" << double(i)/point_count * 100.0;
                }
            }
            assert(i == point_count);
        }

        std::cout << std::endl << "Reading " << point_count << " points" << std::endl;
        {
            typedef bio::stream<bio::file_descriptor_source> bio_istream;
            bio_istream bigifs(filename); 
            las::Reader reader(bigifs);
    
            bio::stream_offset i = 0;
            while (reader.ReadNextPoint())
            {
                las::Point const& p = reader.GetPoint();

                if (!p.Validate())
                {
                    std::ostringstream oss;
                    oss << "invalid point around #" << i;
                    throw std::runtime_error(oss.str());
                }

                if (i % 1000 == 0)
                {
                    std::cout << "\b\b\b\b\b\b\b\b" << double(i)/point_count * 100.0;
                }
                ++i;
            }
            assert(i == point_count);
        }

        std::cout << std::endl << "Done." << std::endl;

        return EXIT_SUCCESS;
    }
    catch (std::exception const& e)
    {
        std::cerr << e.what() << std::endl;
    }
 
    return EXIT_FAILURE;
}
