/******************************************************************************
 * $Id: liblas.hpp 813 2008-07-25 21:53:52Z mloskot $
 *
 * Project:  libLAS - http://liblas.org - A BSD library for LAS format data.
 * Purpose:  LAS include file
 * Author:   Mateusz Loskot, mateusz@loskot.net
 *
 ******************************************************************************
 * Copyright (c) 2008, Mateusz Loskot
 * Copyright (c) 2008, Phil Vachon
 * Copyright (c) 2008, Howard Butler
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


#ifndef LIBLAS_HPP_INCLUDED
#define LIBLAS_HPP_INCLUDED

#include <liblas/cstdint.hpp>
#include <fstream>
#include <string>

/// Namespace grouping all elements of libLAS public interface.
/// \note
/// User's may notice there is namespace \em detail nested
/// in the \em liblas. The \em detail should be considered as
/// private namespace dedicated for implementation details,
/// so libLAS are not supposed to access it directly,
/// nor included headers from the \em detail subdirectory of \em liblas include folder.
namespace liblas
{

/// Version numbers of the ASPRS LAS Specification.
/// Numerical representation of versions is calculated according to 
/// following formula: <em>major * 100000 + minor</em>
enum LASFileVersion
{
    eLASVersion10 = 1 * 100000 + 0, ///< LAS Format 1.0
    eLASVersion11 = 1 * 100000 + 1, ///< LAS Format 1.1
    eLASVersion20 = 2 * 100000 + 0  ///< LAS Format 2.0
};

/// Open file to read in binary mode.
/// The input file is also attached to input stream.
/// \param ifs - reference to input file stream to
/// which opened file is attached
/// \param filename - name of file to open
/// \return true if file has been opened with success, false otherwise
/// \exception No throw
///
inline bool Open(std::ifstream& ifs, std::string const& filename) // throw()
{
    ifs.open(filename.c_str(), std::ios::in | std::ios::binary);
    return ifs.is_open();
}

/// Create file and open to write in binary mode.
/// The output file is also attached to output stream.
/// \param ofs - reference to output file stream to
/// which created file is attached
/// \param filename - name of file to open
/// \return true if file has been create with success, false otherwise
/// \exception No throw
///
inline bool Create(std::ofstream& ofs, std::string const& filename) // throw()
{
    ofs.open(filename.c_str(), std::ios::out | std::ios::binary);
    return ofs.is_open();
}

} // namespace liblas

#endif // LIBLAS_HPP_INCLUDED
