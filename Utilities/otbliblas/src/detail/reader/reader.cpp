/******************************************************************************
 * $Id$
 *
 * Project:  libLAS - http://liblas.org - A BSD library for LAS format data.
 * Purpose:  LAS 1.0 reader implementation for C++ libLAS 
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

#include <liblas/liblas.hpp>
#include <liblas/lasheader.hpp>
#include <liblas/laspoint.hpp>
#include <liblas/detail/reader/reader.hpp>
#include <liblas/detail/private_utility.hpp>
// boost
#include <boost/cstdint.hpp>
// std
#include <fstream>
#include <istream>
#include <iostream>
#include <stdexcept>
#include <cstddef> // std::size_t
#include <cstdlib> // std::free
#include <cassert>

using namespace boost;

namespace liblas { namespace detail { 

ReaderImpl::ReaderImpl(std::istream& ifs)
    : m_ifs(ifs)
    , m_size(0)
    , m_current(0)
    , m_point_reader(PointReaderPtr())
    , m_header_reader(new reader::Header(m_ifs))
{
}

ReaderImpl::~ReaderImpl()
{
}

std::istream& ReaderImpl::GetStream() const
{
    return m_ifs;
}

void ReaderImpl::Reset(HeaderPtr header)
{
    m_ifs.clear();
    m_ifs.seekg(0);

    // Reset sizes and set internal cursor to the beginning of file.
    m_current = 0;
    m_size = header->GetPointRecordsCount();
    
    // If we reset the reader, we're ready to start reading points, so 
    // we'll create a point reader at this point.
    if (!m_point_reader)
    {
        m_point_reader = PointReaderPtr(new reader::Point(m_ifs, header));
    } 
}

HeaderPtr ReaderImpl::ReadHeader()
{
    m_header_reader->read();
    HeaderPtr h = m_header_reader->GetHeader();
    
    Reset(h);
    
    return h;
}

liblas::Point const& ReaderImpl::ReadNextPoint(HeaderPtr header)
{
    if (0 == m_current)
    {
        m_ifs.clear();
        m_ifs.seekg(header->GetDataOffset(), std::ios::beg);
        
        m_point_reader->read();
        ++m_current;
        return m_point_reader->GetPoint();        

    }

    if (m_current < m_size)
    {
        m_point_reader->read();
        ++m_current;
        return m_point_reader->GetPoint();

    } else if (m_current == m_size ){
        throw std::out_of_range("file has no more points to read, end of file reached");
    } else {
        throw std::runtime_error("ReadNextPoint: m_current > m_size, something has gone extremely awry");
    }

}

liblas::Point const& ReaderImpl::ReadPointAt(std::size_t n, HeaderPtr header)
{
    if (m_size == n) {
        throw std::out_of_range("file has no more points to read, end of file reached");
    } else if (m_size < n) {
        std::ostringstream msg;
        msg << "ReadPointAt:: Inputted value: " << n << " is greater than the number of points: " << m_size;
        throw std::runtime_error(msg.str());
    } 

    std::streamsize const pos = (static_cast<std::streamsize>(n) * header->GetDataRecordLength()) + header->GetDataOffset();    

    m_ifs.clear();
    m_ifs.seekg(pos, std::ios::beg);

    m_point_reader->read();
    const liblas::Point& point = m_point_reader->GetPoint();
    
    return point;
}

void ReaderImpl::Seek(std::size_t n, HeaderPtr header)
{
    if (m_size == n) {
        throw std::out_of_range("file has no more points to read, end of file reached");
    } else if (m_size < n) {
        std::ostringstream msg;
        msg << "Seek:: Inputted value: " << n << " is greater than the number of points: " << m_size;
        throw std::runtime_error(msg.str());
    } 

    std::streamsize pos = (static_cast<std::streamsize>(n) * header->GetDataRecordLength()) + header->GetDataOffset();    

    m_ifs.clear();
    m_ifs.seekg(pos, std::ios::beg);
    
    m_current = n;
}

// ReaderImpl* ReaderFactory::Create(std::istream& ifs)
// {
//     if (!ifs)
//     {
//         throw std::runtime_error("input stream state is invalid");
//     }
// 
//     // Determine version of given LAS file and
//     // instantiate appropriate reader.
//     // uint8_t verMajor = 0;
//     // uint8_t verMinor = 0;
//     // ifs.seekg(24, std::ios::beg);
//     // detail::read_n(verMajor, ifs, 1);
//     // detail::read_n(verMinor, ifs, 1);
//     
//     return new ReaderImpl(ifs);
//     // return new CachedReaderImpl(ifs, 3);
//     
//     // if (1 == verMajor && 0 == verMinor)
//     // {
//     // 
//     //     return new ReaderImpl(ifs);
//     // }
//     // else if (1 == verMajor && 1 == verMinor)
//     // {
//     //     return new v11::ReaderImpl(ifs);
//     // }
//     // else if (1 == verMajor && 2 == verMinor)
//     // {
//     //     return new v12::ReaderImpl(ifs);
//     // }
//     // else if (2 == verMajor && 0 == verMinor )
//     // {
//     //     // TODO: LAS 2.0 read/write support
//     //     throw std::runtime_error("LAS 2.0+ file detected but unsupported");
//     // }
// 
//     // throw std::runtime_error("LAS file of unknown version");
// }


// void ReaderFactory::Destroy(ReaderImpl* p) 
// {
//     delete p;
//     p = 0;
// }
}} // namespace liblas::detail

