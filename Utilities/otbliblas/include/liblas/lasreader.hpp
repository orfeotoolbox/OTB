/******************************************************************************
 * $Id: lasreader.hpp 813 2008-07-25 21:53:52Z mloskot $
 *
 * Project:  libLAS - http://liblas.org - A BSD library for LAS format data.
 * Purpose:  LAS reader class 
 * Author:   Mateusz Loskot, mateusz@loskot.net
 *
 ******************************************************************************
 * Copyright (c) 2008, Mateusz Loskot
 * Copyright (c) 2008, Phil Vachon
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

#ifndef LIBLAS_LASREADER_HPP_INCLUDED
#define LIBLAS_LASREADER_HPP_INCLUDED

#include <liblas/lasheader.hpp>
#include <liblas/laspoint.hpp>
#include <liblas/lasrecordheader.hpp>
#include <liblas/detail/fwd.hpp>
// std
#include <iosfwd>
#include <string>
#include <memory>
#include <cstdlib> // std::size_t

namespace liblas {

/// \todo To be documented.
class LASReader
{
public:

    LASReader(std::istream& ifs);
    ~LASReader();
    
    std::size_t GetVersion() const;
    LASHeader const& GetHeader() const;
    LASPoint const& GetPoint() const;
    std::vector<LASVLR> const& GetVLRs() const;
    
    bool ReadNextPoint();
    bool ReadPointAt(std::size_t n);
    bool ReadVLR();

    // The operator is not const because it updates file stream position.
    LASPoint const& operator[](std::size_t n);

    // Allow fetching of the stream
    std::istream& GetStream() const;
    
private:

    // Blocked copying operations, declared but not defined.
    LASReader(LASReader const& other);
    LASReader& operator=(LASReader const& rhs);

    void Init(); // throws on error
    void MakePoint(double const& time);

    const std::auto_ptr<detail::Reader> m_pimpl;
    LASHeader m_header;
    LASPoint m_point;
    detail::PointRecord m_record;
    std::vector<LASVLR> m_vlrs;

};

} // namespace liblas

#endif // ndef LIBLAS_LASREADER_HPP_INCLUDED
