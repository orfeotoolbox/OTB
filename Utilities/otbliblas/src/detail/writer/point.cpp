/******************************************************************************
 * $Id$
 *
 * Project:  libLAS - http://liblas.org - A BSD library for LAS format data.
 * Purpose:  Point Writer implementation for C++ libLAS 
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
#include <liblas/lasvariablerecord.hpp>
#include <liblas/lasschema.hpp>
#include <liblas/detail/writer/writer.hpp>
#include <liblas/detail/writer/point.hpp>
#include <liblas/detail/private_utility.hpp>
// boost
#include <boost/cstdint.hpp>
// std
#include <cmath>
#include <sstream> 

using namespace boost;

namespace liblas { namespace detail { namespace writer {

Point::Point(std::ostream& ofs, uint32_t& count, HeaderPtr header)
    : Base(ofs, count)
    , m_ofs(ofs)
    , m_header(header)
    , m_format(header->GetSchema())
{
    setup();
}

Point::~Point()
{

}

void Point::setup()
{
    // If we have extra data on each point by virtue of the 
    // base size of the format being smaller than the total byte 
    // size of the format, we should make a buffer of blank data 
    // that we can write into the file as necessary.  We make it 
    // here instead of at each ::write() invocation to save the 
    // allocation and writing of the array
    if (m_format.GetByteSize() != m_format.GetBaseByteSize()) {
        std::size_t size = m_format.GetByteSize() - m_format.GetBaseByteSize();
        
        if (size < 0) {
            throw std::runtime_error("ByteSize of format was less than BaseByteSize, this cannot happen!");
        }
        
        m_blanks.resize(size);
        m_blanks.assign(size, 0);
    }
}

void Point::write(const liblas::Point& point)
{
    
    std::vector<boost::uint8_t> const& data = point.GetData();    
    detail::write_n(m_ofs, data.front(), m_header->GetDataRecordLength());


    uint32_t& count = GetPointCount();
    count++;
    SetPointCount(count);

    // write in our extra data that the user set on the 
    // point up to the header's specified DataRecordLength
    // if (m_format.GetByteSize() != m_format.GetBaseByteSize()) {
    //     std::vector<uint8_t> const& data = point.GetExtraData();
    // 
    //     std::size_t size = m_format.GetByteSize() - m_format.GetBaseByteSize();
    //     
    //     if (size < 0) {
    //         throw std::runtime_error("ByteSize of format was less than BaseByteSize, this cannot happen!");
    //     }
    //     
    //     if (data.size() == 0) {
    // 
    //         detail::write_n(GetStream(), m_blanks.front(), static_cast<std::streamsize>(size));
    //         
    //     } else if (data.size() < size){ 
    //         // size can be casted now that we have already checked if it is less than 0
    //         int16_t difference = static_cast<uint16_t>(size) - static_cast<uint16_t>(data.size());
    //         detail::write_n(GetStream(), data.front(), data.size());
    //         detail::write_n(GetStream(), m_blanks.front(), static_cast<std::streamsize>(difference));
    // 
    //     } else {
    //         detail::write_n(GetStream(), data.front(), static_cast<std::streamsize>(size));
    //     }
    // }
}

// void Point::fill() 
// {
//     liblas::Point& p = m_point;
// 
//     m_record.x = static_cast<int32_t>(detail::sround(((p.GetX() - m_header->GetOffsetX()) / m_header->GetScaleX())));
//     m_record.y = static_cast<int32_t>(detail::sround(((p.GetY() - m_header->GetOffsetY()) / m_header->GetScaleY())));
//     m_record.z = static_cast<int32_t>(detail::sround(((p.GetZ() - m_header->GetOffsetZ()) / m_header->GetScaleZ())));
// 
//     Classification::bitset_type clsflags(p.GetClassification());
//     m_record.classification = static_cast<uint8_t>(clsflags.to_ulong());
// 
//     m_record.intensity = p.GetIntensity();
//     m_record.flags = p.GetScanFlags();
//     m_record.scan_angle_rank = p.GetScanAngleRank();
//     m_record.user_data = p.GetUserData();
//     m_record.point_source_id = p.GetPointSourceID();
// }

}}} // namespace liblas::detail::reader
