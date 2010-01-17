/******************************************************************************
 * $Id: writer10.cpp 813 2008-07-25 21:53:52Z mloskot $
 *
 * Project:  libLAS - http://liblas.org - A BSD library for LAS format data.
 * Purpose:  LAS 1.0 writer implementation for C++ libLAS 
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

#include <liblas/detail/writer10.hpp>
#include <liblas/detail/utility.hpp>
#include <liblas/lasheader.hpp>
#include <liblas/laspoint.hpp>
#include <liblas/liblas.hpp>
// std
#include <vector>
#include <fstream>
#include <stdexcept>
#include <cstdlib> // std::size_t
#include <cassert>

namespace liblas { namespace detail { namespace v10 {

WriterImpl::WriterImpl(std::ostream& ofs) :
    Base(), m_ofs(ofs), m_pointCount(0)
{
}

std::size_t WriterImpl::GetVersion() const
{
    return eLASVersion10;
}

void WriterImpl::WriteHeader(LASHeader& header)
{
    uint8_t n1 = 0;
    uint16_t n2 = 0;
    uint32_t n4 = 0;

    // Rewrite the georeference VLR entries if they exist
    header.SetGeoreference();
    
    // Seek to the beginning
    m_ofs.seekp(0, std::ios::beg);
    std::ios::pos_type beginning = m_ofs.tellp();

    // Seek to the end
    m_ofs.seekp(0, std::ios::end);
    std::ios::pos_type end = m_ofs.tellp();
    
    // Figure out how many points we already have.  Each point record 
    // should be 20 bytes long, and header.GetDataOffset tells
    // us the location to start counting points from.  
    
    // This test should only be true if we were opened in both 
    // std::ios::in *and* std::ios::out, otherwise it should return false 
    // and we won't adjust the point count.
    
    if ((beginning != end) && ((uint32_t)end != 0)) {
        m_pointCount = ((uint32_t) end - header.GetDataOffset())/header.GetDataRecordLength();

        // Position to the beginning of the file to start writing the header
        m_ofs.seekp(0, std::ios::beg);
    }

    // 1. File Signature
    std::string const filesig(header.GetFileSignature());
    assert(filesig.size() == 4);
    detail::write_n(m_ofs, filesig, 4);
    
    // 2. Reserved
    n4 = header.GetReserved();
    detail::write_n(m_ofs, n4, sizeof(n4));

    // 3-6. GUID data
    uint32_t d1 = 0;
    uint16_t d2 = 0;
    uint16_t d3 = 0;
    uint8_t d4[8] = { 0 };
    liblas::guid g = header.GetProjectId();
    g.output_data(d1, d2, d3, d4);
    detail::write_n(m_ofs, d1, sizeof(d1));
    detail::write_n(m_ofs, d2, sizeof(d2));
    detail::write_n(m_ofs, d3, sizeof(d3));
    detail::write_n(m_ofs, d4, sizeof(d4));
    
    // 7. Version major
    n1 = header.GetVersionMajor();
    assert(1 == n1);
    detail::write_n(m_ofs, n1, sizeof(n1));
    
    // 8. Version minor
    n1 = header.GetVersionMinor();
    assert(0 == n1);
    detail::write_n(m_ofs, n1, sizeof(n1));

    // 9. System ID
    std::string sysid(header.GetSystemId(true));
    assert(sysid.size() == 32);
    detail::write_n(m_ofs, sysid, 32);
    
    // 10. Generating Software ID
    std::string softid(header.GetSoftwareId(true));
    assert(softid.size() == 32);
    detail::write_n(m_ofs, softid, 32);

    // 11. Flight Date Julian
    n2 = header.GetCreationDOY();
    detail::write_n(m_ofs, n2, sizeof(n2));

    // 12. Year
    n2 = header.GetCreationYear();
    detail::write_n(m_ofs, n2, sizeof(n2));

    // 13. Header Size
    n2 = header.GetHeaderSize();
    assert(227 <= n2);
    detail::write_n(m_ofs, n2, sizeof(n2));

    // 14. Offset to data
    n4 = header.GetDataOffset();
    detail::write_n(m_ofs, n4, sizeof(n4));

    // 15. Number of variable length records
    // TODO: This value must be updated after new variable length record is added.
    n4 = header.GetRecordsCount();
    detail::write_n(m_ofs, n4, sizeof(n4));

    // 16. Point Data Format ID
    n1 = static_cast<uint8_t>(header.GetDataFormatId());
    detail::write_n(m_ofs, n1, sizeof(n1));

    // 17. Point Data Record Length
    n2 = header.GetDataRecordLength();
    detail::write_n(m_ofs, n2, sizeof(n2));

    // 18. Number of point records
    // This value is updated if necessary, see UpdateHeader function.
    n4 = header.GetPointRecordsCount();
    detail::write_n(m_ofs, n4, sizeof(n4));

    // 19. Number of points by return
    std::vector<uint32_t>::size_type const srbyr = 5;
    std::vector<uint32_t> const& vpbr = header.GetPointRecordsByReturnCount();
    assert(vpbr.size() <= srbyr);
    uint32_t pbr[srbyr] = { 0 };
    std::copy(vpbr.begin(), vpbr.end(), pbr);
    detail::write_n(m_ofs, pbr, sizeof(pbr));

    // 20-22. Scale factors
    detail::write_n(m_ofs, header.GetScaleX(), sizeof(double));
    detail::write_n(m_ofs, header.GetScaleY(), sizeof(double));
    detail::write_n(m_ofs, header.GetScaleZ(), sizeof(double));

    // 23-25. Offsets
    detail::write_n(m_ofs, header.GetOffsetX(), sizeof(double));
    detail::write_n(m_ofs, header.GetOffsetY(), sizeof(double));
    detail::write_n(m_ofs, header.GetOffsetZ(), sizeof(double));

    // 26-27. Max/Min X
    detail::write_n(m_ofs, header.GetMaxX(), sizeof(double));
    detail::write_n(m_ofs, header.GetMinX(), sizeof(double));

    // 28-29. Max/Min Y
    detail::write_n(m_ofs, header.GetMaxY(), sizeof(double));
    detail::write_n(m_ofs, header.GetMinY(), sizeof(double));

    // 30-31. Max/Min Z
    detail::write_n(m_ofs, header.GetMaxZ(), sizeof(double));
    detail::write_n(m_ofs, header.GetMinZ(), sizeof(double));


    WriteVLR(header);

    uint8_t const sgn1 = 0xCC;
    uint8_t const sgn2 = 0xDD;
    detail::write_n(m_ofs, sgn1, sizeof(uint8_t));
    detail::write_n(m_ofs, sgn2, sizeof(uint8_t));

    // If we already have points, we're going to put it at the end of the file.  
    // If we don't have any points,  we're going to leave it where it is.
    if (m_pointCount != 0)
        m_ofs.seekp(0, std::ios::end);
}

void WriterImpl::UpdateHeader(LASHeader const& header)
{
    if (m_pointCount != header.GetPointRecordsCount())
    {
        // Skip to first byte of number of point records data member
        std::streamsize const dataPos = 107; 
        m_ofs.seekp(dataPos, std::ios::beg);

        detail::write_n(m_ofs, m_pointCount , sizeof(m_pointCount));
    }
}

void WriterImpl::WritePointRecord(detail::PointRecord const& record)
{
    // TODO: Static assert would be better
    assert(20 == sizeof(record));
    detail::write_n(m_ofs, record, sizeof(record));

    ++m_pointCount;
}

void WriterImpl::WritePointRecord(detail::PointRecord const& record, double const& time)
{
    // TODO: Static assert would be better
    assert(28 == sizeof(record) + sizeof(time));

    // Write point data record format 1
    WritePointRecord(record);

    detail::write_n(m_ofs, time, sizeof(double));
}

void WriterImpl::WriteVLR(LASHeader const& header) 
{
    m_ofs.seekp(header.GetHeaderSize(), std::ios::beg);

    for (uint32_t i = 0; i < header.GetRecordsCount(); ++i)
    {
        LASVLR vlr = header.GetVLR(i);

        detail::write_n(m_ofs, vlr.GetReserved(), sizeof(uint16_t));
        detail::write_n(m_ofs, vlr.GetUserId(true).c_str(), 16);
        detail::write_n(m_ofs, vlr.GetRecordId(), sizeof(uint16_t));
        detail::write_n(m_ofs, vlr.GetRecordLength(), sizeof(uint16_t));
        detail::write_n(m_ofs, vlr.GetDescription(true).c_str(), 32);
        std::vector<uint8_t> const& data = vlr.GetData();
        std::streamsize const size = static_cast<std::streamsize>(data.size());
        detail::write_n(m_ofs, data.front(), size);
    }
}

std::ostream& WriterImpl::GetStream() const
{
    return m_ofs;
}

}}} // namespace liblas::detail::v10
