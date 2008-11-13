/******************************************************************************
 * $Id: reader.hpp 813 2008-07-25 21:53:52Z mloskot $
 *
 * Project:  libLAS - http://liblas.org - A BSD library for LAS format data.
 * Purpose:  Reader implementation for C++ libLAS 
 * Author:   Mateusz Loskot, mateusz@loskot.net
 *
 ******************************************************************************
 * Copyright (c) 2008, Mateusz Loskot
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
 
#ifndef LIBLAS_DETAIL_READER_HPP_INCLUDED
#define LIBLAS_DETAIL_READER_HPP_INCLUDED

#include <liblas/cstdint.hpp>
#include <liblas/detail/fwd.hpp>
// std
#include <iosfwd>

namespace liblas { namespace detail {

struct PointRecord;

class Reader
{
public:

    Reader();
    virtual ~Reader();
    virtual std::size_t GetVersion() const = 0;
    virtual bool ReadHeader(LASHeader& header) = 0;
    virtual bool ReadNextPoint(PointRecord& record) = 0;
    virtual bool ReadNextPoint(PointRecord& record, double& time) = 0;
    virtual bool ReadPointAt(std::size_t n, PointRecord& record) = 0;
    virtual bool ReadPointAt(std::size_t n, PointRecord& record, double& time) = 0;
    virtual bool ReadVLR(LASHeader& header) = 0;
    virtual bool ReadGeoreference(LASHeader& header) = 0; 
    virtual std::istream& GetStream() const = 0;
    
protected:
    
    std::streamoff m_offset;
    uint32_t m_size;
    uint32_t m_current;
    uint32_t m_recordlength;

private:

    // Blocked copying operations, declared but not defined.
    Reader(Reader const& other);
    Reader& operator=(Reader const& rhs);
};

class ReaderFactory
{
public:

    // TODO: prototypes
    static Reader* Create(std::istream& ifs);
    static void Destroy(Reader* p);
};

}} // namespace liblas::detail

#endif // LIBLAS_DETAIL_READER_HPP_INCLUDED
