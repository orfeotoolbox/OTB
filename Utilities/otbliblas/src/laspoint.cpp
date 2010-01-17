/******************************************************************************
 * $Id: laspoint.cpp 813 2008-07-25 21:53:52Z mloskot $
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
#include <liblas/cstdint.hpp>
#include <liblas/exception.hpp>
#include <liblas/detail/utility.hpp>
// std
#include <cstring>

namespace liblas {

LASPoint::LASPoint() :
    m_intensity(0), m_flags(0), m_class(0),
        m_angleRank(0), m_userData(0), m_gpsTime(0)
{
    std::memset(m_coords, 0, sizeof(m_coords));
}

LASPoint::LASPoint(LASPoint const& other) :
    m_intensity(other.m_intensity),
        m_flags(other.m_flags), m_class(other.m_class),
            m_angleRank(other.m_angleRank), m_userData(other.m_userData),
                m_gpsTime(other.m_gpsTime)
{
    std::memcpy(m_coords, other.m_coords, sizeof(m_coords));
}

LASPoint& LASPoint::operator=(LASPoint const& rhs)
{
    if (&rhs != this)
    {
        m_coords[0] = rhs.m_coords[0];
        m_coords[1] = rhs.m_coords[1];
        m_coords[2] = rhs.m_coords[2];
        m_intensity = rhs.m_intensity;
        m_flags = rhs.m_flags;
        m_class = rhs.m_class;
        m_angleRank = rhs.m_angleRank;
        m_userData = rhs.m_userData;
        m_gpsTime = rhs.m_gpsTime;
    }
    return *this;
}

void LASPoint::SetCoordinates(LASHeader const& header, double x, double y, double z)
{
    double const cx = x * header.GetScaleX() + header.GetOffsetX();
    double const cy = y * header.GetScaleY() + header.GetOffsetY();
    double const cz = z * header.GetScaleZ() + header.GetOffsetZ();

    SetCoordinates(cx, cy, cz);
}

void LASPoint::SetReturnNumber(uint16_t const& num)
{
    // Store value in bits 1,2,3
    uint8_t val = static_cast<uint8_t>(num);
    uint16_t const begin = 1;
    uint8_t mask = uint8_t(~0);
    m_flags &= ~(mask << (begin - 1)); 
    m_flags |= ((val & mask) << (begin - 1));
}

void LASPoint::SetNumberOfReturns(uint16_t const& num)
{
    // Store value in bits 4,5,6
    uint8_t val = static_cast<uint8_t>(num);
    uint16_t const begin = 4;
    uint8_t mask = uint8_t(~0);
    m_flags &= ~(mask << (begin - 1)); 
    m_flags |= ((val & mask) << (begin - 1));
}

void LASPoint::SetScanDirection(uint16_t const& dir)
{
    // Store value in bit 7th
    uint8_t val = static_cast<uint8_t>(dir);
    uint16_t const begin = 7;
    uint8_t mask = uint8_t(~0);
    m_flags &= ~(mask << (begin - 1)); 
    m_flags |= ((val & mask) << (begin - 1));
}

void LASPoint::SetFlightLineEdge(uint16_t const& edge)
{
    // Store value in bit 8th
    uint8_t val = static_cast<uint8_t>(edge);
    uint16_t const begin = 8;
    uint8_t mask = uint8_t(~0);
    m_flags &= ~(mask << (begin - 1)); 
    m_flags |= ((val & mask) << (begin - 1));
}

void LASPoint::SetScanAngleRank(int8_t const& rank)
{
    m_angleRank = rank;
}

void LASPoint::SetUserData(uint8_t const& data)
{
    m_userData = data;
}

bool LASPoint::equal(LASPoint const& other) const
{
    double const epsilon = std::numeric_limits<double>::epsilon(); 

    double const dx = m_coords[0] - other.m_coords[0];
    double const dy = m_coords[1] - other.m_coords[1];
    double const dz = m_coords[2] - other.m_coords[2];

    // TODO: Should we compare other data members, besides the coordinates?

    if (((dx <= epsilon) && (dx >= -epsilon ))
        || ((dy <= epsilon) && (dy >= -epsilon ))
        || ((dz <= epsilon) && (dz >= -epsilon )))
    {
        return true;
    }

    return false;
}

bool LASPoint::Validate() const
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


bool LASPoint::IsValid() const
{
    
    if( eScanAngleRankMin > this->GetScanAngleRank() 
        || this->GetScanAngleRank() > eScanAngleRankMax
      )
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
} // namespace liblas
