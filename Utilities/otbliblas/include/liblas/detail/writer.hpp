/******************************************************************************
 * $Id$
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

#ifndef LIBLAS_DETAIL_WRITER_HPP_INCLUDED
#define LIBLAS_DETAIL_WRITER_HPP_INCLUDED

#include <liblas/lasspatialreference.hpp>
#include <liblas/detail/fwd.hpp>
#include <liblas/detail/utility.hpp>

#ifndef HAVE_GDAL
    typedef struct OGRCoordinateTransformationHS *OGRCoordinateTransformationH;
    typedef struct OGRSpatialReferenceHS *OGRSpatialReferenceH;
#endif

// std
#include <iosfwd>

namespace liblas { namespace detail {

class Writer
{
public:

    Writer(std::ostream& ofs);
    virtual ~Writer();
    virtual std::size_t GetVersion() const = 0;
    virtual void WriteHeader(LASHeader& header) = 0;
    virtual void UpdateHeader(LASHeader const& header) = 0;
    virtual void WritePointRecord(LASPoint const& point, const LASHeader& header) = 0;
    std::ostream& GetStream() const;
    uint32_t WriteVLR(LASHeader const& header);

    void SetSRS(const LASSpatialReference& srs);
    
protected:
    PointRecord m_record;
    std::ostream& m_ofs;

    void FillPointRecord(PointRecord& record, const LASPoint& point, const LASHeader& header);

    void Project(LASPoint& point);      
    LASSpatialReference m_out_srs;
    LASSpatialReference m_in_srs;
    
    OGRCoordinateTransformationH m_transform;
    OGRSpatialReferenceH m_in_ref;
    OGRSpatialReferenceH m_out_ref;
    
private:

    // Blocked copying operations, declared but not defined.
    Writer(Writer const& other);
    Writer& operator=(Writer const& rhs);
    
};

class WriterFactory
{
public:

    static Writer* Create(std::ostream& ofs, LASHeader const& header);
    static void Destroy(Writer* p);
};

}} // namespace liblas::detail

#endif // LIBLAS_DETAIL_WRITER_HPP_INCLUDED
