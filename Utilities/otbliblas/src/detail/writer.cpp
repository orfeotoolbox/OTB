/******************************************************************************
 * $Id: writer.cpp 813 2008-07-25 21:53:52Z mloskot $
 *
 * Project:  libLAS - http://liblas.org - A BSD library for LAS format data.
 * Purpose:  LAS writer implementation for C++ libLAS 
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

#include <liblas/detail/writer.hpp>
#include <liblas/detail/writer10.hpp>
#include <liblas/detail/writer11.hpp>
#include <liblas/lasheader.hpp>
#include <liblas/laspoint.hpp>
// std
#include <fstream>
#include <cassert>
#include <cstdlib> // std::size_t
#include <stdexcept>

namespace liblas { namespace detail {

Writer::Writer()
{
}

Writer::~Writer()
{
}

Writer* WriterFactory::Create(std::ostream& ofs, LASHeader const& header)
{
    if (!ofs)
    {
        throw std::runtime_error("output stream state is invalid");
    }

    // Select writer implementation based on requested LAS version.
    uint8_t major = header.GetVersionMajor();
    uint8_t minor = header.GetVersionMinor();
    
    if (1 == major && 0 == minor)
    {
        return new v10::WriterImpl(ofs);
    }
    if (1 == major && 1 == minor)
    {
        return new v11::WriterImpl(ofs);
    }
    else if (2 == major && 0 == minor)
    {
        // TODO: LAS 2.0 read/write support
        throw std::runtime_error("LAS 2.0 file detected but unsupported");
    }

    throw std::runtime_error("LAS file of unknown version");
}

void WriterFactory::Destroy(Writer* p) 
{
    delete p;
    p = 0;
}

}} // namespace liblas::detail
