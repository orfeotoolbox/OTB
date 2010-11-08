/******************************************************************************
 * $Id$
 *
 * Project:  libLAS - http://liblas.org - A BSD library for LAS format data.
 * Purpose:  Point Reader implementation for C++ libLAS 
 * Author:   Howard Butler, hobu.inc@gmail.com
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
 
#include <liblas/lasheader.hpp>
#include <liblas/detail/reader/reader.hpp>
#include <liblas/detail/reader/point.hpp>
#include <liblas/detail/private_utility.hpp>
// boost
#include <boost/array.hpp>
#include <boost/cstdint.hpp>
// std
#include <cstring>
#include <sstream> 

using namespace boost;
using namespace std;

namespace liblas { namespace detail { namespace reader {

Point::Point(std::istream& ifs, HeaderPtr header)
    : m_ifs(ifs)
    , m_header(header)
    , m_point(m_header)
    , m_raw_data(m_header->GetSchema().GetByteSize())
{
}

Point::~Point()
{
}

std::istream& Point::GetStream() const
{
    return m_ifs;
}

void Point::read()
{
    assert(m_header);
    assert(m_point.GetHeaderPtr());
    assert(m_raw_data.size() > 0);

    try
    {
        detail::read_n(m_raw_data.front(), m_ifs, m_raw_data.size());
    }
    catch (std::out_of_range const& e) // we reached the end of the file
    {
        // FIXME: Why do we kill the error? It cause setting m_point with invali data, doesn't it? --mloskot
        std::cerr << e.what() << std::endl;
    }    
    
    m_point.SetData(m_raw_data);
}

}}} // namespace liblas::detail::reader
