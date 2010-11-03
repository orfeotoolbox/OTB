/******************************************************************************
 * $Id$
 *
 * Project:  libLAS - http://liblas.org - A BSD library for LAS format data.
 * Purpose:  Implementation of Classification type.
 * Author:   Mateusz Loskot, mateusz@loskot.net
 *
 ******************************************************************************
 * Copyright (c) 2009, Mateusz Loskot
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

#include <liblas/lasclassification.hpp>
#include <liblas/detail/private_utility.hpp>
// boost
#include <boost/cstdint.hpp>
// std
#include <cstddef>
#include <limits>
#include <string>

using namespace boost;

namespace {

static std::string g_class_names[] =
{
    "Created, never classified",
    "Unclassified",
    "Ground",
    "Low Vegetation",
    "Medium Vegetation",
    "High Vegetation",
    "Building",
    "Low Point (noise)",
    "Model Key-point (mass point)",
    "Water",
    "Reserved for ASPRS Definition",
    "Reserved for ASPRS Definition",
    "Overlap Points",
    "Reserved for ASPRS Definition",
    "Reserved for ASPRS Definition",
    "Reserved for ASPRS Definition",
    "Reserved for ASPRS Definition",
    "Reserved for ASPRS Definition",
    "Reserved for ASPRS Definition",
    "Reserved for ASPRS Definition",
    "Reserved for ASPRS Definition",
    "Reserved for ASPRS Definition",
    "Reserved for ASPRS Definition",
    "Reserved for ASPRS Definition",
    "Reserved for ASPRS Definition",
    "Reserved for ASPRS Definition",
    "Reserved for ASPRS Definition",
    "Reserved for ASPRS Definition",
    "Reserved for ASPRS Definition",
    "Reserved for ASPRS Definition",
    "Reserved for ASPRS Definition",
    "Reserved for ASPRS Definition"
};

} // namespace anonymous

namespace liblas {

uint32_t const Classification::class_table_size = detail::static_array_size(g_class_names);

std::string Classification::GetClassName() const
{
    uint32_t const index = GetClass();
    check_class_index(index);
    
    return g_class_names[index];
}

uint8_t Classification::GetClass() const
{
    bitset_type bits(m_flags);
    
    bitset_type const mask(static_cast<boost::uint64_t>(class_table_size) - 1);
    bits &= mask;

    uint32_t const index = static_cast<uint32_t>(bits.to_ulong());
    assert(index < class_table_size);
    assert(index <= std::numeric_limits<uint8_t>::max());

    return static_cast<uint8_t>(index);
}

void Classification::SetClass(boost::uint32_t index)
{
    check_class_index(index);

    bitset_type binval(static_cast<boost::uint64_t>(index));
    binval <<= 0;

    // Store value in bits 0,1,2,3,4
    bitset_type const mask(0x1F);
    m_flags &= ~mask;
    m_flags |= mask & binval;
}

void Classification::check_class_index(boost::uint32_t index) const
{
    if (index > class_table_size - 1 || !(index <= std::numeric_limits<uint8_t>::max()))
    {
        std::ostringstream msg;
        msg << "given index is " << index
            << ", but must fit between 0 and " << (class_table_size - 1);
        throw std::out_of_range(msg.str());
    }
}

} // namespace liblas
