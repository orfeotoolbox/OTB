/******************************************************************************
 * $Id: lasrecordheader.cpp 861 2008-08-22 16:02:32Z mloskot $
 *
 * Project:  libLAS - http://liblas.org - A BSD library for LAS format data.
 * Purpose:  LAS record header class 
 * Author:   Phil Vachon, philippe@cowpig.ca
 *
 ******************************************************************************
 * Copyright (c) 2008, Phil Vachon
 * Copyright (c) 2008, Mateusz Loskot, mateusz@loskot.net
 * Copyright (c) 2008, Howard Butler, hobu.inc@gmail.com
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

#include <liblas/lasrecordheader.hpp>
#include <liblas/cstdint.hpp>

// std
#include <algorithm>
#include <stdexcept>
#include <string>
#include <vector>
#include <cstring> // std::memset, std::memcpy, std::strncpy
#include <cassert>

namespace liblas {

LASVLR::LASVLR() :
    m_reserved(0), m_recordId(0), m_recordLength(0)
{    
    std::memset(m_userId, 0, eUIDSize);
    std::memset(m_desc, 0, eDescriptionSize);
    
    m_data.resize(40);

}

LASVLR::LASVLR(LASVLR const& other) :
    m_reserved(other.m_reserved),
    m_recordId(other.m_recordId),
    m_recordLength(other.m_recordLength)
{
    void* p = 0;

    p = std::memcpy(m_userId, other.m_userId, eUIDSize);
    assert(p == m_userId);

    p = std::memcpy(m_desc, other.m_desc, eDescriptionSize);
    assert(p == m_desc);
    
    std::vector<uint8_t>(other.m_data).swap(m_data);
}

LASVLR::~LASVLR()
{

}

LASVLR& LASVLR::operator=(LASVLR const& rhs)
{
    void* p = 0;
    if (this != &rhs)
    {
        m_reserved = rhs.m_reserved;
        m_recordId = rhs.m_recordId;
        m_recordLength = rhs.m_recordLength;

        p = std::memcpy(m_userId, rhs.m_userId, eUIDSize);
        assert(p == m_userId);

        p = std::memcpy(m_desc, rhs.m_desc, eDescriptionSize);
        assert(p == m_desc);

        std::vector<uint8_t>(rhs.m_data).swap(m_data);
    }
    return (*this);
}

uint16_t LASVLR::GetReserved() const
{
    return m_reserved;
}

void LASVLR::SetReserved(uint16_t id)
{
    m_reserved = id;
}

std::string LASVLR::GetUserId(bool pad /*= false*/) const
{
    // copy array of chars and trim zeros if smaller than 32 bytes
    std::string tmp(std::string(m_userId, eUIDSize).c_str());

    // pad right side with spaces
    if (pad && tmp.size() < eUIDSize)
    {
        tmp.resize(eUIDSize, 0);
        assert(tmp.size() == eUIDSize);
    }

    assert(tmp.size() <= eUIDSize);
    return tmp;
}

void LASVLR::SetUserId(std::string const& v)
{
    if (v.size() > eUIDSize)
        throw std::invalid_argument("user id too long");
    

    std::fill(m_userId, m_userId + eUIDSize, 0);
    std::strncpy(m_userId, v.c_str(), eUIDSize);
}


uint16_t LASVLR::GetRecordId() const
{
    return m_recordId;
}

void LASVLR::SetRecordId(uint16_t v) {
    m_recordId = v;
}

uint16_t LASVLR::GetRecordLength() const
{
    return m_recordLength;
}

void LASVLR::SetRecordLength(uint16_t v) {
    m_recordLength = v;
}

std::string LASVLR::GetDescription(bool pad /*= false*/) const
{
    // copy array of chars and trim zeros if smaller than 32 bytes
    std::string tmp(std::string(m_desc, eDescriptionSize).c_str());

    // pad right side with spaces
    if (pad && tmp.size() < eDescriptionSize)
    {
        tmp.resize(eDescriptionSize, 0);
        assert(tmp.size() == eDescriptionSize);
    }

    assert(tmp.size() <= eDescriptionSize);
    return tmp;
}

void LASVLR::SetDescription(std::string const& v)
{
    if (v.size() > eDescriptionSize)
        throw std::invalid_argument("description is too long");
    

    std::fill(m_desc, m_desc + eDescriptionSize, 0);
    std::strncpy(m_desc, v.c_str(), eDescriptionSize);
}


std::vector<uint8_t> const&  LASVLR::GetData() const
{
    return m_data;
}

void LASVLR::SetData(const std::vector<uint8_t>& v) 
{
    m_data = v;
}

bool LASVLR::equal(LASVLR const& other) const
{
    return (m_recordId == other.m_recordId
            && std::string(m_userId) == std::string(other.m_userId) 
            && std::string(m_desc) == std::string(other.m_desc)
            && m_reserved == other.m_reserved
            && m_recordLength == other.m_recordLength);
}

uint32_t LASVLR::GetTotalSize() const
{
    // Signature 2 bytes
    // UserID 16 bytes
    // RecordID 2 bytes
    // RecordLength after Header 2 bytes
    // Description 32 bytes
    // Data length -- size of the data's vector * the size of uint8_t
    std::size_t const sum = 2 + 16 + 2 + 2 + 32 + GetData().size() * sizeof(uint8_t);
    return static_cast<uint32_t>(sum);
}

} // namespace liblas

