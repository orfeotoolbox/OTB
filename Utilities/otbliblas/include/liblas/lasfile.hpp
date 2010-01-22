/******************************************************************************
 * $Id: lasfile.hpp 813 2008-07-25 21:53:52Z mloskot $
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

#ifndef LIBLAS_LASFILE_HPP_INCLUDED
#define LIBLAS_LASFILE_HPP_INCLUDED

#include <liblas/detail/file.hpp> // complete type required
#include <liblas/detail/sharedptr.hpp>
#include <string>
#include <cassert>

namespace liblas {

/// \todo to be documented
class LASFile
{
public:

    enum Mode
    {
        eRead = 0,
        eWrite = 1,
        eAppend = 2
    };

    LASFile();
    LASFile(std::string const& filename);
    LASFile(std::string const& filename, LASHeader const& header, Mode mode);
    LASFile(LASFile const& other);
    LASFile& operator=(LASFile const& rhs);

    bool IsNull() const;
    std::string GetName() const;
    Mode GetMode() const;
    LASHeader const& GetHeader() const;
    LASReader& GetReader();
    LASWriter& GetWriter();

private:

    detail::SharedPtr<detail::FileImpl> m_pimpl;
};

} // namespace liblas

#endif // LIBLAS_LASFILE_HPP_INCLUDED
