/******************************************************************************
 * $Id$
 *
 * Project:  libLAS - http://liblas.org - A BSD library for LAS format data.
 * Purpose:  LAS point class 
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

#include <liblas/laspoint.hpp>
#include <liblas/lasheader.hpp>
#include <liblas/lasschema.hpp>
#include <liblas/exception.hpp>
#include <liblas/detail/pointrecord.hpp>
// boost
#include <boost/array.hpp>
#include <boost/cstdint.hpp>
#include <liblas/external/property_tree/ptree.hpp>

// std
#include <cstring>
#include <stdexcept>
#include <string>
#include <vector>
#include <iosfwd>
#include <algorithm>
#include <numeric>

using namespace boost;

namespace liblas {

Point::Point()
    : m_header(HeaderPtr())
    , m_default_header(DefaultHeader::get())
{
    m_data.resize(ePointSize3);
    m_data.assign(ePointSize3, 0);
}

Point::Point(HeaderPtr hdr)
    : m_header(hdr)
    , m_default_header(DefaultHeader::get())
{
    m_data.resize(ePointSize3);
    m_data.assign(ePointSize3, 0);
}

Point::Point(Point const& other)
    : m_data(other.m_data)
    , m_header(other.m_header)
    , m_default_header(DefaultHeader::get())
{
}

Point& Point::operator=(Point const& rhs)
{
    if (&rhs != this)
    {
        m_data = rhs.m_data;
        m_header = rhs.m_header;
    }
    return *this;
}


void Point::SetCoordinates(double const& x, double const& y, double const& z)
{
    SetX(x);
    SetY(y);
    SetZ(z);
}


bool Point::equal(Point const& other) const
{
    // TODO - mloskot: Default epsilon is too small.
    //                 Is 0.00001 good as tolerance or too wide?
    //double const epsilon = std::numeric_limits<double>::epsilon(); 
    double const epsilon = 0.00001;

    double const dx = GetX() - other.GetX();
    double const dy = GetY() - other.GetY();
    double const dz = GetZ() - other.GetZ();

    // TODO: Should we compare other data members, besides the coordinates?

    if ((dx <= epsilon && dx >= -epsilon)
     && (dy <= epsilon && dy >= -epsilon)
     && (dz <= epsilon && dz >= -epsilon))
    {
        return true;
    }

    // If we do other members
    // bool compare_classification(uint8_t cls, uint8_t expected)
    // {
    //    // 31 is max index in classification lookup table
    //    clsidx = (cls & 31);
    //    assert(clsidx <= 31); 
    //    return (clsidx == expected);
    // }

    return false;
}

bool Point::Validate() const
{
    unsigned int flags = 0;

    if (this->GetReturnNumber() > 0x07)
        flags |= eReturnNumber;

    if (this->GetNumberOfReturns() > 0x07)
        flags |= eNumberOfReturns;

    if (this->GetScanDirection() > 0x01)
        flags |= eScanDirection;

    if (this->GetFlightLineEdge() > 0x01)
        flags |= eFlightLineEdge;

    if (eScanAngleRankMin > this->GetScanAngleRank()
        || this->GetScanAngleRank() > eScanAngleRankMax)
    {
        flags |= eScanAngleRank;
    }

    if (flags > 0)
    {
        throw invalid_point_data("point data members out of range", flags);
    }

    return true;
}

bool Point::IsValid() const
{
    
    if (eScanAngleRankMin > this->GetScanAngleRank() || this->GetScanAngleRank() > eScanAngleRankMax)
        return false;

    if (this->GetFlightLineEdge() > 0x01)
        return false;

    if (this->GetScanDirection() > 0x01)
        return false;

    if (this->GetNumberOfReturns() > 0x07)
        return false;

    if (this->GetReturnNumber() > 0x07)
        return false;


    return true;
}

void Point::SetHeaderPtr(HeaderPtr header) 
{
    boost::uint16_t wanted_length;
    
    if (header) 
        wanted_length = header->GetDataRecordLength();
    else
        wanted_length = m_default_header.GetDataRecordLength();
    
    // This is hopefully faster than copying everything if we don't have 
    // any data set and nothing to worry about.
    boost::uint32_t sum = std::accumulate(m_data.begin(), m_data.end(), 0);
    
    if (!sum) {
        m_data.resize(wanted_length);
        m_data.assign(wanted_length, 0);
        m_header = header;
        return;
    }
    
    if (wanted_length != m_data.size())
    {
        // Manually copy everything but the header ptr
        // We can't just copy the raw data because its 
        // layout is likely changing as a result of the 
        // schema change.
        Point p(*this);
    
        m_data.resize(wanted_length);
        m_data.assign(wanted_length, 0);
    
        SetX(p.GetX());
        SetY(p.GetY());
        SetZ(p.GetZ());
        
        SetIntensity(p.GetIntensity());
        SetScanFlags(p.GetScanFlags());
        SetClassification(p.GetClassification());
        SetScanAngleRank(p.GetScanAngleRank());
        SetUserData(p.GetUserData());
        SetPointSourceID(p.GetPointSourceID());
        
        try {
            SetTime(p.GetTime());
        } catch (std::runtime_error const&) 
        {
            
        }

        try {
            SetColor(p.GetColor());
        } catch (std::runtime_error const&) 
        {
            
        }
        
        // FIXME: copy other custom dimensions here?  resetting the 
        // headerptr can be catastrophic in a lot of cases.  

        
    }
    
    m_header = header;
}

HeaderPtr Point::GetHeaderPtr() const
{
    return m_header;
}

liblas::property_tree::ptree Point::GetPTree() const
{
    using liblas::property_tree::ptree;
    ptree pt;

    pt.put("x", GetX());
    pt.put("y", GetY());
    pt.put("z", GetZ());

    pt.put("time", GetTime());
    pt.put("intensity", GetIntensity());
    pt.put("returnnumber", GetReturnNumber());
    pt.put("numberofreturns", GetNumberOfReturns());
    pt.put("scandirection", GetScanDirection());
    
    pt.put("scanangle", GetScanAngleRank());
    pt.put("flightlineedge", GetFlightLineEdge());

    pt.put("userdata", GetUserData());
    pt.put("pointsourceid", GetPointSourceID());

    ptree klasses;
    
    liblas::Classification const& c = GetClassification();
    std::string name = c.GetClassName();

    klasses.put("name", name);
    klasses.put("id", c.GetClass());
    klasses.put("withheld", c.IsWithheld());
    klasses.put("keypoint", c.IsKeyPoint());
    klasses.put("synthetic", c.IsSynthetic());

    pt.add_child("classification",klasses);

    ptree colors;
    liblas::Color const& clr = GetColor();

    colors.put("red", clr.GetRed());
    colors.put("green", clr.GetGreen());
    colors.put("blue", clr.GetBlue());
    pt.add_child("color", colors);
    
    return pt;
}

std::ostream& operator<<(std::ostream& os, liblas::Point const& p)
{
    using liblas::property_tree::ptree;
    ptree tree = p.GetPTree();

    os << "---------------------------------------------------------" << std::endl;
    
    os.setf(std::ios_base::fixed, std::ios_base::floatfield);
    os.precision(6);

    os << "  X: \t\t\t" << tree.get<double>("x") << std::endl;
    os << "  Y: \t\t\t" << tree.get<double>("y") << std::endl;
    os << "  Z: \t\t\t" << tree.get<double>("z") << std::endl;
    os << "  Time: \t\t" << tree.get<double>("time") << std::endl;
    os.unsetf(std::ios_base::fixed);
    os.unsetf(std::ios_base::floatfield);
    os << "  Return Number: \t" << tree.get<boost::uint32_t>("returnnumber") << std::endl;
    os << "  Return Count: \t" << tree.get<boost::uint32_t>("numberofreturns") << std::endl;
    os << "  Flightline Edge: \t" << tree.get<boost::uint32_t>("flightlineedge") << std::endl;
    os << "  Intensity: \t\t" << tree.get<boost::uint32_t>("intensity") << std::endl;
    os << "  Scan Direction: \t" << tree.get<boost::uint32_t>("scandirection") << std::endl;
    os << "  Scan Angle Rank: \t" << tree.get<boost::int32_t>("scanangle") << std::endl;
    os << "  Classification: \t" << tree.get<std::string>("classification.name") << std::endl;
    os << "         witheld: \t" << tree.get<std::string>("classification.withheld") << std::endl;
    os << "        keypoint: \t" << tree.get<std::string>("classification.keypoint") << std::endl;
    os << "       synthetic: \t" << tree.get<std::string>("classification.synthetic") << std::endl;
    os << "  RGB Color: \t\t" << tree.get<boost::uint32_t>("color.red") << " " 
                              << tree.get<boost::uint32_t>("color.green") << " "
                              << tree.get<boost::uint32_t>("color.blue") << std::endl;
    os << "---------------------------------------------------------" << std::endl;

    return os;
}


double Point::GetX() const
{
    boost::int32_t v = GetRawX();
    
    double output = 0;
    
    if (m_header.get() != 0 ) 
    { 
        // Scale it
        output  = (v * m_header->GetScaleX()) + m_header->GetOffsetX();
    } else {
        output  = (v * m_default_header.GetScaleX()) + m_default_header.GetOffsetX();
    }
    
    return output;
}

double Point::GetY() const
{
    boost::int32_t v = GetRawY();
    
    double output = 0;
    
    if (m_header.get() != 0 ) 
    { 
        // Scale it
        output  = (v * m_header->GetScaleY()) + m_header->GetOffsetY();
    } else {
        output  = (v * m_default_header.GetScaleY()) + m_default_header.GetOffsetY();
    }
    
    return output;
}

double Point::GetZ() const
{
    boost::int32_t v = GetRawZ();
    
    double output = 0;
    
    if (m_header.get() != 0 ) 
    { 
        // Scale it
        output  = (v * m_header->GetScaleZ()) + m_header->GetOffsetZ();
    } else {
        output  = (v * m_default_header.GetScaleZ()) + m_default_header.GetOffsetZ();
    }
    
    return output;
}

void Point::SetX( double const& value ) 
{
    boost::int32_t v = static_cast<boost::int32_t>(value);
    if (m_header.get() != 0 ) 
    {
        // descale the value given our scale/offset
        v = static_cast<boost::int32_t>(
                             detail::sround(((value - m_header->GetOffsetX()) / 
                                              m_header->GetScaleX())));

    } else 
    {
        v = static_cast<boost::int32_t>(
                             detail::sround(((value - m_default_header.GetOffsetX()) / 
                                              m_default_header.GetScaleX())));
    }

    SetRawX(v);
}

void Point::SetY( double const& value ) 
{
    boost::int32_t v = static_cast<boost::int32_t>(value);
    if (m_header.get() != 0 ) 
    {
        // descale the value given our scale/offset
        v = static_cast<boost::int32_t>(
                             detail::sround(((value - m_header->GetOffsetY()) / 
                                              m_header->GetScaleY())));
    } else 
    {
        v = static_cast<boost::int32_t>(
                             detail::sround(((value - m_default_header.GetOffsetY()) / 
                                              m_default_header.GetScaleY())));
    }
    
    SetRawY(v);
}

void Point::SetZ( double const& value ) 
{
    boost::int32_t v = static_cast<boost::int32_t>(value);
    if (m_header) 
    {
        // descale the value given our scale/offset
        v = static_cast<boost::int32_t>(
                             detail::sround(((value - m_header->GetOffsetZ()) / 
                                              m_header->GetScaleZ())));
    } else
    {
        v = static_cast<boost::int32_t>(
                             detail::sround(((value - m_default_header.GetOffsetZ()) / 
                                              m_default_header.GetScaleZ())));
    }
    
    SetRawZ(v);
}

boost::int32_t Point::GetRawX() const
{
    // std::vector<boost::uint8_t>::size_type pos = GetDimensionPosition("X");
    std::vector<boost::uint8_t>::size_type pos = GetDimensionBytePosition(0);
    boost::int32_t output = liblas::detail::bitsToInt<boost::int32_t>(output, m_data, pos);

    return output;
}

boost::int32_t Point::GetRawY() const
{
    // std::vector<boost::uint8_t>::size_type pos = GetDimensionPosition("Y");
    std::vector<boost::uint8_t>::size_type pos = GetDimensionBytePosition(1);    
    boost::int32_t output = liblas::detail::bitsToInt<boost::int32_t>(output, m_data, pos);

    return output;
}

boost::int32_t Point::GetRawZ() const
{
    boost::int32_t output;
    // std::vector<boost::uint8_t>::size_type pos = GetDimensionPosition("Z");
    std::vector<boost::uint8_t>::size_type pos = GetDimensionBytePosition(2);
    output = liblas::detail::bitsToInt<boost::int32_t>(output, m_data, pos);

    return output;
}

void Point::SetRawX( boost::int32_t const& value)
{
    // std::vector<boost::uint8_t>::size_type pos = GetDimensionPosition("X");
    std::vector<boost::uint8_t>::size_type pos = GetDimensionBytePosition(0);    
    liblas::detail::intToBits<boost::int32_t>(value, m_data, pos);
}

void Point::SetRawY( boost::int32_t const& value)
{
    // std::vector<boost::uint8_t>::size_type pos = GetDimensionPosition("Y");
    std::vector<boost::uint8_t>::size_type pos = GetDimensionBytePosition(1);
    liblas::detail::intToBits<boost::int32_t>(value, m_data, pos);
}

void Point::SetRawZ( boost::int32_t const& value)
{
    // std::vector<boost::uint8_t>::size_type pos = GetDimensionPosition("Z");
    std::vector<boost::uint8_t>::size_type pos = GetDimensionBytePosition(2);    
    liblas::detail::intToBits<boost::int32_t>(value, m_data, pos);
}

boost::uint16_t Point::GetIntensity() const
{
    // Intensity's position is always the 4th dimension
    std::vector<boost::uint8_t>::size_type pos = GetDimensionBytePosition(3);
    boost::uint16_t output = 
        liblas::detail::bitsToInt<boost::uint16_t>(
            output, m_data, pos);

    return output;
}


void Point::SetIntensity(boost::uint16_t const& intensity)
{
    // Intensity's position is always the 4th dimension
    std::vector<boost::uint8_t>::size_type pos = GetDimensionBytePosition(3);    
    liblas::detail::intToBits<boost::uint16_t>(intensity, 
                                               m_data, 
                                               pos);
}

boost::uint8_t Point::GetScanFlags() const
{
    // Scan Flag's position is always the 5th dimension 
    // (the entire byte composed of "Return Number", "Number of Returns", 
    // "Scan Direction", and "Flightline Edge")
    std::vector<boost::uint8_t>::size_type pos = GetDimensionBytePosition(4);    
    return m_data[pos];
}

void Point::SetScanFlags(boost::uint8_t const& flags)
{
    // Scan Flag's position is always the 5th dimension 
    // (the entire byte composed of "Return Number", "Number of Returns", 
    // "Scan Direction", and "Flightline Edge")
    std::vector<boost::uint8_t>::size_type pos = GetDimensionBytePosition(4);       
    m_data[pos] = flags;
}

boost::uint16_t Point::GetReturnNumber() const
{
    // "Return Number" is always the 5th dimension
    std::vector<boost::uint8_t>::size_type pos = GetDimensionBytePosition(4);  
    boost::uint8_t flags = m_data[pos];
    
    // Read bits 1,2,3 (first 3 bits)
    return (flags & 0x07);
}

void Point::SetReturnNumber(uint16_t const& num)
{
    // "Return Number" is always the 5th dimension    
    std::vector<boost::uint8_t>::size_type pos = GetDimensionBytePosition(4);  
        
    boost::uint8_t flags = m_data[pos];
    
    // Store value in bits 0,1,2
    uint8_t mask = 0x7 << 0; // 0b00000111
    flags &= ~mask;
    flags |= mask & (static_cast<uint8_t>(num) << 0);
    m_data[pos] = flags;
}

boost::uint16_t Point::GetNumberOfReturns() const
{
    boost::uint8_t flags;

    // "Number of Returns" is always the 6th dimension    
    std::vector<boost::uint8_t>::size_type pos = GetDimensionBytePosition(5);

    flags = m_data[pos];

    // Read bits 4,5,6
    return ((flags >> 3) & 0x07);
}

void Point::SetNumberOfReturns(uint16_t const& num)
{
    // "Number of Returns" is always the 6th dimension    
    std::vector<boost::uint8_t>::size_type pos = GetDimensionBytePosition(5);
    
    boost::uint8_t flags = m_data[pos];
    
    // Store value in bits 3,4,5
    uint8_t mask = 0x7 << 3; // 0b00111000
    flags &= ~mask;
    flags |= mask & (static_cast<uint8_t>(num) << 3);
    m_data[pos] = flags;

//     // Store value in bits 3,4,5
//     uint8_t mask = 0x7 << 3; // 0b00111000
//     m_flags &= ~mask;
//     m_flags |= mask & (static_cast<uint8_t>(num) << 3);

}

void Point::SetScanDirection(uint16_t const& dir)
{
    // "Scan Direction" is always the 7th dimension    
    std::vector<boost::uint8_t>::size_type pos = GetDimensionBytePosition(6);
    
    boost::uint8_t flags = m_data[pos];
    
    // Store value in bits 6
    uint8_t mask = 0x1 << 6; // 0b01000000
    flags &= ~mask;
    flags |= mask & (static_cast<uint8_t>(dir) << 6);
    m_data[pos] = flags;   
}

boost::uint16_t Point::GetScanDirection() const
{
    // "Scan Direction" is always the 7th dimension    
    std::vector<boost::uint8_t>::size_type pos = GetDimensionBytePosition(6);
    
    boost::uint8_t flags = m_data[pos];

    // Read 6th bit
    return ((flags >> 6) & 0x01);
}

void Point::SetFlightLineEdge(uint16_t const& edge)
{
    // "Flightline Edge" is always the 8th dimension    
    std::vector<boost::uint8_t>::size_type pos = GetDimensionBytePosition(7);

    boost::uint8_t flags = m_data[pos];
    
    // Store value in bits 7
    uint8_t mask = 0x1 << 7; // 0b10000000
    flags &= ~mask;
    flags |= mask & (static_cast<uint8_t>(edge) << 7);
    m_data[pos] = flags;

//     // Store value in bit 7
//     uint8_t mask = 0x1 << 7; // 0b10000000
//     m_flags &= ~mask;
//     m_flags |= mask & (static_cast<uint8_t>(edge) << 7);}

}
    
boost::uint16_t Point::GetFlightLineEdge() const
{
    // "Flightline Edge" is always the 8th dimension    
    std::vector<boost::uint8_t>::size_type pos = GetDimensionBytePosition(7);
    
    boost::uint8_t flags = m_data[pos];

    // Read 8th bit
    return ((flags >> 7) & 0x01);
}


Classification Point::GetClassification() const
{
    // "Classification" is always the 9th dimension    
    std::vector<boost::uint8_t>::size_type pos = GetDimensionBytePosition(8);
    boost::uint8_t kls = m_data[pos];
    return Classification(kls);
}

void Point::SetClassification(Classification const& cls)
{
    // "Classification" is always the 9th dimension    
    std::vector<boost::uint8_t>::size_type pos = GetDimensionBytePosition(8);
    m_data[pos] = cls.GetClass();
}

void Point::SetClassification(Classification::bitset_type const& flags)
{
    // "Classification" is always the 9th dimension    
    std::vector<boost::uint8_t>::size_type pos = GetDimensionBytePosition(8);
    m_data[pos] = Classification(flags).GetClass();
}

void Point::SetClassification(boost::uint8_t const& flags)
{
    // "Classification" is always the 9th dimension    
    std::vector<boost::uint8_t>::size_type pos = GetDimensionBytePosition(8);
    m_data[pos] = Classification(flags).GetClass();
}

void Point::SetScanAngleRank(int8_t const& rank)
{
    // "Scan Angle Rank" is always the 10th dimension    
    std::vector<boost::uint8_t>::size_type pos = GetDimensionBytePosition(9);

    m_data[pos] = rank;

}
boost::int8_t Point::GetScanAngleRank() const
{
    // "Scan Angle Rank" is always the 10th dimension    
    std::vector<boost::uint8_t>::size_type pos = GetDimensionBytePosition(9);

    return m_data[pos];
}

boost::uint8_t Point::GetUserData() const
{
    // "User Data" is always the 11th dimension    
    std::vector<boost::uint8_t>::size_type pos = GetDimensionBytePosition(10);
    
    return m_data[pos];
}

void Point::SetUserData(boost::uint8_t const& flags)
{
    // "User Data" is always the 11th dimension    
    std::vector<boost::uint8_t>::size_type pos = GetDimensionBytePosition(10);
    m_data[pos] = flags;
}

boost::uint16_t Point::GetPointSourceID() const
{
    boost::uint16_t output;
    
    // "Point Source ID" is always the 12th dimension    
    std::vector<boost::uint8_t>::size_type pos = GetDimensionBytePosition(11);
    output = liblas::detail::bitsToInt<boost::uint16_t>(output, 
                                                        m_data, 
                                                        pos);

    return output;
}


void Point::SetPointSourceID(boost::uint16_t const& id)
{
    // "Point Source ID" is always the 12th dimension    
    std::vector<boost::uint8_t>::size_type pos = GetDimensionBytePosition(11);
    liblas::detail::intToBits<boost::uint16_t>(id, 
                                               m_data, 
                                               pos);
}

void Point::SetTime(double const& t)
{

    // "Time" is the 13th dimension if it exists
    std::size_t index_pos = 12;

    PointFormatName f;
    if (m_header) {
        f = m_header->GetDataFormatId();
    } else {
        f = m_default_header.GetDataFormatId();
    }   
    
    if ( f == ePointFormat0 || f == ePointFormat2 ) {
        std::ostringstream msg;
        msg << "Point::SetTime - Unable to set time for ePointFormat0 or ePointFormat2, "
            << "no Time dimension exists on this format";
        throw std::runtime_error(msg.str());
    }
    std::vector<boost::uint8_t>::size_type pos = GetDimensionBytePosition(index_pos);

    const boost::uint8_t* x_b =  reinterpret_cast<const boost::uint8_t*>(&t);
#if defined(LIBLAS_BIG_ENDIAN)
        for (boost::int32_t n = sizeof( double )-1; n >= 0; n--)
#else
        for (boost::uint32_t n = 0; n < sizeof( double ); n++)
#endif 
            m_data[pos+n] = x_b[n];

}

double Point::GetTime() const
{
    // "Time" is the 13th dimension if it exists
    std::size_t index_pos = 12;

    PointFormatName f;
    if (m_header) {
        f = m_header->GetDataFormatId();
    } else {
        f = m_default_header.GetDataFormatId();
    }   
    
    if ( f == ePointFormat0 || f == ePointFormat2 ) {
        // std::ostringstream msg;
        // msg << "Point::GetTime - Unable to get time for ePointFormat0 or ePointFormat2, "
        //     << "no Time dimension exists on this format";
        // throw std::runtime_error(msg.str());
        return 0.0;
    }
    std::vector<boost::uint8_t>::size_type pos = GetDimensionBytePosition(index_pos);


    boost::uint8_t* data = new boost::uint8_t[8];
    
#if defined(LIBLAS_BIG_ENDIAN)
        for (boost::uint32_t n = 0; n < sizeof( double ); n++)
#else
        for (boost::int32_t n = sizeof( double )-1; n >= 0; n--)
#endif  
            data[n] = m_data[pos+n];

    const double* output = reinterpret_cast<const double*>(data);
    double out = *output;
    delete[] data;
    return out;
}


Color Point::GetColor() const
{
    boost::uint16_t red(0);
    boost::uint16_t green(0);
    boost::uint16_t blue(0);

    // "Color" starts at the 14th dimension if it exists
    std::size_t index_pos = 13;

    PointFormatName f;
    if (m_header) {
        f = m_header->GetDataFormatId();
    } else {
        f = m_default_header.GetDataFormatId();
    }   
    
    if ( f == ePointFormat0 || f == ePointFormat1 ) {
        // std::ostringstream msg;
        //         msg << "Point::GetColor - Unable to set color for ePointFormat0 or ePointFormat1, "
        //             << "no Color dimension exists on this format";
        //         throw std::runtime_error(msg.str());
        return Color(0, 0, 0);
    }
    
    assert(!(f == ePointFormat0 || f == ePointFormat1));
    
    using liblas::detail::bitsToInt;
    
    std::vector<boost::uint8_t>::size_type red_pos = GetDimensionBytePosition(index_pos);
    assert(red_pos + sizeof(Color::value_type) <= m_data.size());
    red = bitsToInt<boost::uint16_t>(red, m_data, red_pos);

    std::vector<boost::uint8_t>::size_type green_pos = GetDimensionBytePosition(index_pos + 1);
    assert(green_pos + sizeof(Color::value_type) <= m_data.size());
    green = bitsToInt<boost::uint16_t>(green, m_data, green_pos);

    std::vector<boost::uint8_t>::size_type blue_pos = GetDimensionBytePosition(index_pos + 2);
    assert(blue_pos + sizeof(Color::value_type) <= m_data.size());
    blue = bitsToInt<boost::uint16_t>(blue, m_data, blue_pos);

  return Color(red, green, blue);
}

void Point::SetColor(Color const& value)
{

    // "Color" starts at the 14th dimension if it exists
    std::size_t index_pos = 13;

    PointFormatName f;
    if (m_header) {
        f = m_header->GetDataFormatId();
    } else {
        f = m_default_header.GetDataFormatId();
    }   
    
    if ( f == ePointFormat0 || f == ePointFormat1 ) {
        std::ostringstream msg;
        msg << "Point::SetColor - Unable to set color for ePointFormat0 or ePointFormat1, "
            << "no Color dimension exists on this format";
        throw std::runtime_error(msg.str());
    }

    using liblas::detail::intToBits;
    
    std::vector<boost::uint8_t>::size_type green_pos = GetDimensionBytePosition(index_pos + 1);
    std::vector<boost::uint8_t>::size_type blue_pos = GetDimensionBytePosition(index_pos + 2);
 

    std::vector<boost::uint8_t>::size_type red_pos = GetDimensionBytePosition(index_pos);
    assert(red_pos + sizeof(Color::value_type) <= m_data.size());
    intToBits<boost::uint16_t>(value.GetRed(), m_data, red_pos);
    intToBits<boost::uint16_t>(value.GetGreen(), 
                                               m_data, 
                                               green_pos);
    intToBits<boost::uint16_t>(value.GetBlue(), 
                                               m_data, 
                                               blue_pos);
}





std::vector<boost::uint8_t>::size_type Point::GetDimensionBytePosition(std::size_t dim_pos) const
{
    std::size_t output = 0;
    if (m_header) {
        Dimension const& d = m_header->GetSchema().GetDimension(dim_pos);
        output = d.GetByteOffset();
    } else {
        Dimension const& d = m_default_header.GetSchema().GetDimension(dim_pos);
        output = d.GetByteOffset();
    }   
    return output;
}





boost::any Point::GetValue(Dimension const& d) const
{

    boost::any output;

 
    
    return output;
}


} // namespace liblas
