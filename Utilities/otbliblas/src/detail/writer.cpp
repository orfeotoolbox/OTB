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

#include <liblas/detail/writer.hpp>
#include <liblas/detail/writer10.hpp>
#include <liblas/detail/writer11.hpp>
#include <liblas/detail/writer12.hpp>
#include <liblas/lasheader.hpp>
#include <liblas/laspoint.hpp>
#include <liblas/lasspatialreference.hpp>

#ifdef HAVE_GDAL
#include <ogr_srs_api.h>
#endif


// std
#include <fstream>
#include <cassert>
#include <cstdlib> // std::size_t
#include <stdexcept>

namespace liblas { namespace detail {

Writer::Writer(std::ostream& ofs) : m_ofs(ofs), m_transform(0), m_in_ref(0), m_out_ref(0)
{
}

Writer::~Writer()
{
#ifdef HAVE_GDAL
    if (m_transform) {
        OCTDestroyCoordinateTransformation(m_transform);
    }
    if (m_in_ref) {
        OSRDestroySpatialReference(m_in_ref);
    }
    if (m_out_ref) {
        OSRDestroySpatialReference(m_out_ref);
    }
#endif
}

std::ostream& Writer::GetStream() const
{
    return m_ofs;
}


void Writer::FillPointRecord(PointRecord& record, const LASPoint& point, const LASHeader& header) 
{
    if (m_transform) {
        // let's just copy the point for now.
        LASPoint p = LASPoint(point);
        Project(p);
        record.x = static_cast<int32_t>((p.GetX() - header.GetOffsetX()) / header.GetScaleX());
        record.y = static_cast<int32_t>((p.GetY() - header.GetOffsetY()) / header.GetScaleY());
        record.z = static_cast<int32_t>((p.GetZ() - header.GetOffsetZ()) / header.GetScaleZ());
    } else {
        record.x = static_cast<int32_t>((point.GetX() - header.GetOffsetX()) / header.GetScaleX());
        record.y = static_cast<int32_t>((point.GetY() - header.GetOffsetY()) / header.GetScaleY());
        record.z = static_cast<int32_t>((point.GetZ() - header.GetOffsetZ()) / header.GetScaleZ());
    }

    record.intensity = point.GetIntensity();
    record.flags = point.GetScanFlags();
    record.classification = point.GetClassification();
    record.scan_angle_rank = point.GetScanAngleRank();
    record.user_data = point.GetUserData();
    record.point_source_id = point.GetPointSourceID();
}

uint32_t Writer::WriteVLR(LASHeader const& header) 
{
    // If this function returns a value, it is the size that the header's 
    // data offset must be increased by in order for the VLRs to fit in 
    // the header.  
    m_ofs.seekp(header.GetHeaderSize(), std::ios::beg);

    // if the VLRs won't fit because the data offset is too 
    // small, we need to throw an error.
    uint32_t vlr_total_size = 0;
        
    // Calculate a new data offset size
    for (uint32_t i = 0; i < header.GetRecordsCount(); ++i)
    {
        LASVariableRecord vlr = header.GetVLR(i);
        vlr_total_size += vlr.GetTotalSize();
    }
    
    int32_t difference = header.GetDataOffset() - (vlr_total_size + header.GetHeaderSize());

    if (difference < 0) 
    {
        return difference;
    }
    
    for (uint32_t i = 0; i < header.GetRecordsCount(); ++i)
    {
        LASVariableRecord vlr = header.GetVLR(i);

        detail::write_n(m_ofs, vlr.GetReserved(), sizeof(uint16_t));
        detail::write_n(m_ofs, vlr.GetUserId(true).c_str(), 16);
        detail::write_n(m_ofs, vlr.GetRecordId(), sizeof(uint16_t));
        detail::write_n(m_ofs, vlr.GetRecordLength(), sizeof(uint16_t));
        detail::write_n(m_ofs, vlr.GetDescription(true).c_str(), 32);
        std::vector<uint8_t> const& data = vlr.GetData();
        std::streamsize const size = static_cast<std::streamsize>(data.size());
        detail::write_n(m_ofs, data.front(), size);
    }
    
    // if we had more room than we need for the VLRs, we need to pad that with 
    // 0's.  We must also not forget to add the 1.0 pad bytes to the end of this
    // but the impl should be the one doing that, not us.
    if (difference > 0) {
        detail::write_n(m_ofs, "\0", difference);
    }
    return 0;
}


void Writer::SetSRS(const LASSpatialReference& srs )
{
    m_out_srs = srs;
#ifdef HAVE_GDAL
    m_in_ref = OSRNewSpatialReference(0);
    m_out_ref = OSRNewSpatialReference(0);

    int result = OSRSetFromUserInput(m_in_ref, m_in_srs.GetWKT().c_str());
    if (result != OGRERR_NONE) 
    {
        std::ostringstream msg; 
        msg << "Could not import input spatial reference for Writer::" << CPLGetLastErrorMsg() << result;
        std::string message(msg.str());
        throw std::runtime_error(message);
    }
    
    result = OSRSetFromUserInput(m_out_ref, m_out_srs.GetWKT().c_str());
    if (result != OGRERR_NONE) 
    {
        std::ostringstream msg; 
        msg << "Could not import output spatial reference for Writer::" << CPLGetLastErrorMsg() << result;
        std::string message(msg.str());
        throw std::runtime_error(message);
    }

    m_transform = OCTNewCoordinateTransformation( m_in_ref, m_out_ref);
    
#endif
}

void Writer::Project(LASPoint& p)
{
#ifdef HAVE_GDAL
    
    int ret = 0;
    double x = p.GetX();
    double y = p.GetY();
    double z = p.GetZ();
    
    ret = OCTTransform(m_transform, 1, &x, &y, &z);
    
    if (!ret) {
        std::ostringstream msg; 
        msg << "Could not project point for Writer::" << CPLGetLastErrorMsg() << ret;
        std::string message(msg.str());
        throw std::runtime_error(message);
    }
    
    p.SetX(x);
    p.SetY(y);
    p.SetZ(z);
#else
    UNREFERENCED_PARAMETER(p);
#endif
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
    if (1 == major && 2 == minor)
    {
        return new v12::WriterImpl(ofs);
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
