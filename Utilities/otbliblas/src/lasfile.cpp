/******************************************************************************
 * $Id: lasfile.cpp 813 2008-07-25 21:53:52Z mloskot $
 *
 * Project:  libLAS - http://liblas.org - A BSD library for LAS format data.
 * Purpose:  A composite class to handle read/write operations
 * Author:   Mateusz Loskot, mateusz@loskot.net
 *
 ******************************************************************************
 * Copyright (c) 2008, Mateusz Loskot
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

#include <liblas/lasfile.hpp>
#include <liblas/lasheader.hpp>
#include <liblas/detail/file.hpp>

#include <string>
#include <stdexcept>

namespace liblas {

LASFile::LASFile()
{
}

LASFile::LASFile(std::string const& filename)
    : m_pimpl(new detail::FileImpl(filename))
{
}

LASFile::LASFile(std::string const& filename, LASHeader const& header, Mode mode)
    : m_pimpl(new detail::FileImpl(filename, header, mode))
{
}

LASFile::LASFile(LASFile const& other)
    : m_pimpl(other.m_pimpl)
{
}

LASFile& LASFile::operator=(LASFile const& rhs)
{
    m_pimpl = rhs.m_pimpl;
    return (*this);
}

bool LASFile::IsNull() const
{
     return (0 == m_pimpl.get());
}

std::string LASFile::GetName() const
{
    return m_pimpl->GetName();
}

LASFile::Mode LASFile::GetMode() const
{
    int const mode = m_pimpl->GetMode();
    if (mode == 0)
        return eRead;
    else if (mode == 1)
        return eWrite;
    else if (mode == 2)
        return eAppend;

    assert("SHOULD NEVER GET HERE");
    return eRead;
}

LASHeader const& LASFile::GetHeader() const
{
    return m_pimpl->GetHeader();
}

LASReader& LASFile::GetReader()
{
    return m_pimpl->GetReader();
}

LASWriter& LASFile::GetWriter()
{
    return m_pimpl->GetWriter();
}

} // namespace liblas
