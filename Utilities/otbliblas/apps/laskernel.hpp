/******************************************************************************
 * $Id$
 *
 * Project:  libLAS - http://liblas.org - A BSD library for LAS format data.
 * Purpose:  Processing Kernel
 * Author:   Howard Butler, hobu.inc at gmail.com
 *
 ******************************************************************************
 * Copyright (c) 2010, Howard Butler
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

#ifndef LIBLAS_KERNEL_HPP_INCLUDED
#define LIBLAS_KERNEL_HPP_INCLUDED

#include <liblas/liblas.hpp>
#include <liblas/utility.hpp>
#include <liblas/external/property_tree/ptree.hpp>
#include <liblas/external/property_tree/xml_parser.hpp>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <string>
#include <functional>
#include <algorithm>

#include <boost/program_options.hpp>
#include <boost/tokenizer.hpp>
#include <boost/array.hpp>
#include <boost/foreach.hpp>


using namespace std;
namespace po = boost::program_options;

#define SEPARATORS ",|"
typedef boost::tokenizer<boost::char_separator<char> > tokenizer;

bool IsDualRangeFilter(std::string parse_string) ;

liblas::FilterPtr MakeReturnFilter(std::vector<boost::uint16_t> const& returns, liblas::FilterI::FilterType ftype) ;
liblas::FilterPtr MakeClassFilter(std::vector<liblas::Classification> const& classes, liblas::FilterI::FilterType ftype) ;
liblas::FilterPtr MakeBoundsFilter(liblas::Bounds<double> const& bounds, liblas::FilterI::FilterType ftype) ;
liblas::FilterPtr MakeIntensityFilter(std::string intensities, liblas::FilterI::FilterType ftype) ;
liblas::FilterPtr MakeTimeFilter(std::string times, liblas::FilterI::FilterType ftype) ;
liblas::FilterPtr MakeScanAngleFilter(std::string intensities, liblas::FilterI::FilterType ftype) ;
liblas::FilterPtr MakeColorFilter(liblas::Color const& low, liblas::Color const& high, liblas::FilterI::FilterType ftype); 


po::options_description GetFilteringOptions();
po::options_description GetTransformationOptions();
po::options_description GetHeaderOptions();

std::vector<liblas::FilterPtr> GetFilters(po::variables_map vm, bool verbose);
std::vector<liblas::TransformPtr> GetTransforms(po::variables_map vm, bool verbose, liblas::Header& header);

#ifdef _WIN32
#define compare_no_case(a,b,n)  _strnicmp( (a), (b), (n) )
#else
#define compare_no_case(a,b,n)  strncasecmp( (a), (b), (n) )
#endif

std::istream* OpenInput(std::string filename, bool bEnd);
std::string TryReadFileData(std::string filename);
std::vector<char> TryReadRawFileData(std::string filename);
bool term_progress(std::ostream& os, double complete);
void SetStreamPrecision(std::ostream& os, double scale);

liblas::Header FetchHeader(std::string const& filename);
void RewriteHeader(liblas::Header const& header, std::string const& filename);
void RepairHeader(liblas::Summary const& summary, liblas::Header& header);

#endif // LIBLAS_ITERATOR_HPP_INCLUDED
