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

#include <liblas/lasvariablerecord.hpp>
// boost
#include <boost/cstdint.hpp>
// std
#include <algorithm>
#include <stdexcept>
#include <string>
#include <vector>
#include <cstring> // std::memset, std::memcpy, std::strncpy
#include <cassert>

using namespace boost;

namespace liblas {

VariableRecord::VariableRecord()
    : m_data(40)
    , m_reserved(0)
    , m_record_id(0)
    , m_record_size(0)
{    
    m_user_id.assign(0);
    m_description.assign(0);
}

VariableRecord::VariableRecord(VariableRecord const& other)
    : m_data(other.m_data)
    , m_description(other.m_description)
    , m_user_id(other.m_user_id)
    , m_reserved(other.m_reserved)
    , m_record_id(other.m_record_id)
    , m_record_size(other.m_record_size)
{
}

VariableRecord::~VariableRecord()
{
}

VariableRecord& VariableRecord::operator=(VariableRecord const& rhs)
{
    if (this != &rhs)
    {
        m_data = rhs.m_data;
        m_description = rhs.m_description;
        m_user_id = rhs.m_user_id;
        m_reserved = rhs.m_reserved;
        m_record_id = rhs.m_record_id;
        m_record_size = rhs.m_record_size;
    }
    return (*this);
}

uint16_t VariableRecord::GetReserved() const
{
    return m_reserved;
}

void VariableRecord::SetReserved(uint16_t data)
{
    m_reserved = data;
}

std::string VariableRecord::GetUserId(bool pad /*= false*/) const
{
    // copy array of chars and trim zeros if smaller than 32 bytes
    std::string tmp(std::string(m_user_id.begin(), m_user_id.end()).c_str());

    // pad right side with spaces
    if (pad && tmp.size() < m_user_id.size())
    {
        tmp.resize(m_user_id.size(), 0);
        assert(tmp.size() == m_user_id.size());
    }

    assert(tmp.size() <= m_user_id.size());
    return tmp;
}

void VariableRecord::SetUserId(std::string const& id)
{
    if (id.size() > m_user_id.size())
    {
        std::ostringstream msg;
        msg << "User ID for VLR is too long: " << id.size();
        throw std::invalid_argument(msg.str());
    }

    std::fill(m_user_id.begin(), m_user_id.end(), 0);
    std::copy(id.begin(), id.end(), m_user_id.begin());
}

uint16_t VariableRecord::GetRecordId() const
{
    return m_record_id;
}

void VariableRecord::SetRecordId(uint16_t id)
{
    m_record_id = id;
}

uint16_t VariableRecord::GetRecordLength() const
{
    return m_record_size;
}

void VariableRecord::SetRecordLength(uint16_t length)
{
    m_record_size = length;
}

std::string VariableRecord::GetDescription(bool pad /*= false*/) const
{
    // copy array of chars and trim zeros if smaller than 32 bytes
    std::string tmp(std::string(m_description.begin(), m_description.end()).c_str());

    // pad right side with spaces
    if (pad && tmp.size() < m_description.size())
    {
        tmp.resize(m_description.size(), 0);
        assert(tmp.size() == m_description.size());
    }

    assert(tmp.size() <= m_description.size());
    return tmp;
}

void VariableRecord::SetDescription(std::string const& text)
{
    if (text.size() > m_description.size())
        throw std::invalid_argument("description is too long");
    

    std::fill(m_description.begin(), m_description.end(), 0);
    std::copy(text.begin(), text.end(), m_description.begin());
}


std::vector<uint8_t> const&  VariableRecord::GetData() const
{
    return m_data;
}

void VariableRecord::SetData(const std::vector<uint8_t>& data) 
{
    m_data = data;
}

bool VariableRecord::equal(VariableRecord const& other) const
{
    return m_record_id == other.m_record_id
        && m_user_id == other.m_user_id
        && m_description == other.m_description
        && m_reserved == other.m_reserved
        && m_record_size == other.m_record_size;
}

std::size_t VariableRecord::GetTotalSize() const
{
    // Signature 2 bytes
    // UserID 16 bytes
    // RecordID 2 bytes
    // RecordLength after Header 2 bytes
    // Description 32 bytes
    // Data length -- size of the data's vector * the size of uint8_t
    std::size_t const sum = 2 + 16 + 2 + 2 + 32 + GetData().size() * sizeof(uint8_t);
    return sum;
}

liblas::property_tree::ptree VariableRecord::GetPTree() const
{
    using liblas::property_tree::ptree;
    ptree vlr;

    vlr.put("userid", GetUserId(false));
    vlr.put("description", GetDescription(false));
    vlr.put("length", GetRecordLength());
    vlr.put("id", GetRecordId());
    return vlr;
    
}

std::ostream& operator<<(std::ostream& os, liblas::VariableRecord const& v)
{

    using liblas::property_tree::ptree;
    ptree tree = v.GetPTree();

    os << "    User: '" 
             << tree.get<std::string>("userid")
             << "' - Description: '"
             << tree.get<std::string>("description") 
             <<"'" 
             << std::endl;
    os << "    ID: " << tree.get<boost::uint32_t>("id")
             << " Length: " << tree.get<boost::uint32_t>("length")
             << std::endl;
        
    return os;
}
} // namespace liblas

