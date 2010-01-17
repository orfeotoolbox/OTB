/******************************************************************************
 * $Id: reader10.cpp 831 2008-08-17 04:13:27Z hobu $
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

#include <liblas/detail/reader10.hpp>
#include <liblas/detail/utility.hpp>
#include <liblas/lasrecordheader.hpp>
#include <liblas/liblas.hpp>
#include <liblas/lasheader.hpp>
#include <liblas/laspoint.hpp>
// GeoTIFF
#ifdef HAVE_LIBGEOTIFF
#include <geotiff.h>
#include <geo_simpletags.h>
#include "geo_normalize.h"
#include "geo_simpletags.h"
#include "geovalues.h"
#endif // HAVE_LIBGEOTIFF
// std
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <cstdlib> // std::size_t, std::free
#include <cassert>

namespace liblas { namespace detail { namespace v10 {

ReaderImpl::ReaderImpl(std::istream& ifs) : Base(), m_ifs(ifs)
{
}

std::size_t ReaderImpl::GetVersion() const
{
    return eLASVersion10;
}

bool ReaderImpl::ReadHeader(LASHeader& header)
{
    using detail::read_n;

    // Helper variables
    uint8_t n1 = 0;
    uint16_t n2 = 0;
    uint32_t n4 = 0;
    double x1 = 0;
    double y1 = 0;
    double z1 = 0;
    double x2 = 0;
    double y2 = 0;
    double z2 = 0;
    std::string buf;
    std::string fsig;

    m_ifs.seekg(0);

    // 1. File Signature
    read_n(fsig, m_ifs, 4);
    header.SetFileSignature(fsig);

    // 2. Reserved
    // This data must always contain Zeros.
    read_n(n4, m_ifs, sizeof(n4));

    // 3-6. GUID data
    uint32_t d1 = 0;
    uint16_t d2 = 0;
    uint16_t d3 = 0;
    uint8_t d4[8] = { 0 };
    read_n(d1, m_ifs, sizeof(d1));
    read_n(d2, m_ifs, sizeof(d2));
    read_n(d3, m_ifs, sizeof(d3));
    read_n(d4, m_ifs, sizeof(d4));
    liblas::guid g(d1, d2, d3, d4);
    header.SetProjectId(g);

    // 7. Version major
    read_n(n1, m_ifs, sizeof(n1));
    header.SetVersionMajor(n1);

    // 8. Version minor
    read_n(n1, m_ifs, sizeof(n1));
    header.SetVersionMinor(n1);

    // 9. System ID
    read_n(buf, m_ifs, 32);
    header.SetSystemId(buf);

    // 10. Generating Software ID
    read_n(buf, m_ifs, 32);
    header.SetSoftwareId(buf);

    // 11. Flight Date Julian
    read_n(n2, m_ifs, sizeof(n2));
    header.SetCreationDOY(n2);

    // 12. Year
    read_n(n2, m_ifs, sizeof(n2));
    header.SetCreationYear(n2);

    // 13. Header Size
    // NOTE: Size of the stanard header block must always be 227 bytes
    read_n(n2, m_ifs, sizeof(n2));

    // 14. Offset to data
    read_n(n4, m_ifs, sizeof(n4));
    if (n4 < header.GetHeaderSize())
    {
        // TODO: Move this test to LASHeader::Validate()
        throw std::domain_error("offset to point data smaller than header size");
    }
    header.SetDataOffset(n4);

    // 15. Number of variable length records
    read_n(n4, m_ifs, sizeof(n4));
    header.SetRecordsCount(n4);

    // 16. Point Data Format ID
    read_n(n1, m_ifs, sizeof(n1));
    if (n1 == LASHeader::ePointFormat0)
        header.SetDataFormatId(LASHeader::ePointFormat0);
    else if (n1 == LASHeader::ePointFormat1)
        header.SetDataFormatId(LASHeader::ePointFormat1);
    else
        throw std::domain_error("invalid point data format");

    // 17. Point Data Record Length
    // NOTE: No need to set record length because it's
    // determined on basis of point data format.
    read_n(n2, m_ifs, sizeof(n2));

    // 18. Number of point records
    read_n(n4, m_ifs, sizeof(n4));
    header.SetPointRecordsCount(n4);

    // 19. Number of points by return
    std::vector<uint32_t>::size_type const srbyr = 5;
    uint32_t rbyr[srbyr] = { 0 };
    read_n(rbyr, m_ifs, sizeof(rbyr));
    for (std::size_t i = 0; i < srbyr; ++i)
    {
        header.SetPointRecordsByReturnCount(i, rbyr[i]);
    }

    // 20-22. Scale factors
    read_n(x1, m_ifs, sizeof(x1));
    read_n(y1, m_ifs, sizeof(y1));
    read_n(z1, m_ifs, sizeof(z1));
    header.SetScale(x1, y1, z1);

    // 23-25. Offsets
    read_n(x1, m_ifs, sizeof(x1));
    read_n(y1, m_ifs, sizeof(y1));
    read_n(z1, m_ifs, sizeof(z1));
    header.SetOffset(x1, y1, z1);

    // 26-27. Max/Min X
    read_n(x1, m_ifs, sizeof(x1));
    read_n(x2, m_ifs, sizeof(x2));

    // 28-29. Max/Min Y
    read_n(y1, m_ifs, sizeof(y1));
    read_n(y2, m_ifs, sizeof(y2));

    // 30-31. Max/Min Z
    read_n(z1, m_ifs, sizeof(z1));
    read_n(z2, m_ifs, sizeof(z2));

    header.SetMax(x1, y1, z1);
    header.SetMin(x2, y2, z2);

    m_offset = header.GetDataOffset();
    m_size = header.GetPointRecordsCount();
    m_recordlength = header.GetDataRecordLength();

    return true;
}

bool ReaderImpl::ReadVLR(LASHeader& header)
{
    VLRHeader vlrh = { 0 };

    m_ifs.seekg(header.GetHeaderSize(), std::ios::beg);
    uint32_t count = header.GetRecordsCount();
    header.SetRecordsCount(0);
    for (uint32_t i = 0; i < count; ++i)
    {
        read_n(vlrh, m_ifs, sizeof(VLRHeader));

        uint16_t length = vlrh.recordLengthAfterHeader;
        if (length < 1) {
            throw std::domain_error("VLR record length must be at least 1 byte long");
        }
         
        std::vector<uint8_t> data;
        data.resize(length);

        read_n(data.front(), m_ifs, length);
         
        LASVLR vlr;
        vlr.SetReserved(vlrh.reserved);
        vlr.SetUserId(std::string(vlrh.userId));
        vlr.SetDescription(std::string(vlrh.description));
        vlr.SetRecordLength(vlrh.recordLengthAfterHeader);
        vlr.SetRecordId(vlrh.recordId);
        vlr.SetData(data);

        header.AddVLR(vlr);
    }
    
    return true;
}

bool ReaderImpl::ReadGeoreference(LASHeader& header)
{
#ifndef HAVE_LIBGEOTIFF
    UNREFERENCED_PARAMETER(header);
#else

    std::string const uid("LASF_Projection");

    detail::raii_wrapper<ST_TIFF> st(ST_Create(), ST_Destroy);

    for (uint16_t i = 0; i < header.GetRecordsCount(); ++i)
    {
        LASVLR record = header.GetVLR(i);
        std::vector<uint8_t> data = record.GetData();
        if (uid == record.GetUserId(true).c_str() && 34735 == record.GetRecordId())
        {
            int16_t count = data.size()/sizeof(int16_t);
            ST_SetKey(st.get(), record.GetRecordId(), count, STT_SHORT, &(data[0]));
        }

        if (uid == record.GetUserId(true).c_str() && 34736 == record.GetRecordId())
        {
            int count = data.size() / sizeof(double);
            ST_SetKey(st.get(), record.GetRecordId(), count, STT_DOUBLE, &(data[0]));
        }        

        if (uid == record.GetUserId(true).c_str() && 34737 == record.GetRecordId())
        {
            uint8_t count = data.size()/sizeof(uint8_t);
            ST_SetKey(st.get(), record.GetRecordId(), count, STT_ASCII, &(data[0]));
        }
    }

    if (st.get()->key_count)
    {
        raii_wrapper<GTIF> gtif(GTIFNewSimpleTags(st.get()), GTIFFree);

        GTIFDefn defn;
        if (GTIFGetDefn(gtif.get(), &defn)) 
        {
            char* proj4def = GTIFGetProj4Defn(&defn);
            std::string tmp(proj4def);
            std::free(proj4def);

            header.SetProj4(tmp);
        }

        return true;
    }

#endif /* def HAVE_LIBGEOTIFF */

    return false;
}

bool ReaderImpl::ReadNextPoint(detail::PointRecord& record)
{
    // Read point data record format 0

    // TODO: Replace with compile-time assert
    assert(LASHeader::ePointSize0 == sizeof(record));

    if (0 == m_current)
    {
        m_ifs.clear();
        m_ifs.seekg(m_offset, std::ios::beg);
    }

    if (m_current < m_size)
    {
        try
        {
            detail::read_n(record, m_ifs, sizeof(PointRecord));
            ++m_current;    
        }
        catch (std::out_of_range const& e) // we reached the end of the file
        {
            std::cerr << e.what() << std::endl;
            return false;
        }

        return true;
    }

    return false;
}

bool ReaderImpl::ReadNextPoint(detail::PointRecord& record, double& time)
{
    // Read point data record format 1

    // TODO: Replace with compile-time assert
    assert(LASHeader::ePointSize1 == sizeof(record) + sizeof(time));

    bool hasData = ReadNextPoint(record);
    if (hasData)
    {
        detail::read_n(time, m_ifs, sizeof(double));
    }

    return hasData;
}

bool ReaderImpl::ReadPointAt(std::size_t n, PointRecord& record)
{
    // Read point data record format 0

    // TODO: Replace with compile-time assert
    assert(LASHeader::ePointSize0 == sizeof(record));

    if (m_size <= n)
        return false;
    std::streamsize pos = (static_cast<std::streamsize>(n) * m_recordlength) + m_offset;


    m_ifs.clear();
    m_ifs.seekg(pos, std::ios::beg);
    detail::read_n(record, m_ifs, sizeof(record));

    return true;
}

bool ReaderImpl::ReadPointAt(std::size_t n, PointRecord& record, double& time)
{
    // Read point data record format 1

    // TODO: Replace with compile-time assert
    assert(LASHeader::ePointSize1 == sizeof(record) + sizeof(time));

    bool hasData = ReadPointAt(n, record);
    if (hasData)
    {
        detail::read_n(time, m_ifs, sizeof(double));
    }

    return hasData;
}

std::istream& ReaderImpl::GetStream() const
{
    return m_ifs;
}

}}} // namespace liblas::detail::v10

