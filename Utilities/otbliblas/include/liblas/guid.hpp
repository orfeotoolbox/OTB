/******************************************************************************
 * $Id$
 *
 * Project:  libLAS - http://liblas.org - A BSD library for LAS format data.
 * Purpose:  GUID implementation
 * Author:   Andy Tompkins (modified by Mateusz Loskot)
 *
 * This file has been stolen from the Boost Vault and
 * modified for libLAS purposes. Here is the original code posted:
 * http://lists.boost.org/boost-users/2007/04/27397.php
 * 
 * (C) Copyright 2006 Andy Tompkins.
 * (C) Copyright 2008 Mateusz Loskot, mateusz@loskot.net.
 * Distributed under the Boost  Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 * 
 * Revision History
 * 06 Feb 2006 - Initial Revision
 * 09 Nov 2006 - fixed variant and version bits for v4 guids
 * 13 Nov 2006 - added serialization
 * 17 Nov 2006 - added name-based guid creation
 * 20 Nov 2006 - add fixes for gcc (from Tim Blechmann)
 * 07 Mar 2007 - converted to header only
 * 20 Jan 2008 - removed dependency of Boost and modified for libLAS (by Mateusz Loskot)
 ******************************************************************************
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

#ifndef LIBLAS_GUID_HPP_INCLUDED
#define LIBLAS_GUID_HPP_INCLUDED

#include <liblas/detail/sha1.hpp>
#include <liblas/detail/private_utility.hpp>
#include <liblas/export.hpp>
// boost
#include <boost/array.hpp>
#include <boost/cstdint.hpp>
#include <boost/random.hpp>
// std
#include <iosfwd>
#include <iomanip>
#include <algorithm>
#include <limits>
#include <stdexcept>
#include <sstream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <cassert>

namespace liblas {

namespace detail {

	inline boost::uint8_t random_byte()
    {
        // Change seed to something better?

        typedef boost::mt19937 engine_t;
        typedef boost::uniform_int<unsigned long> distribution_t;
        typedef boost::variate_generator<engine_t, distribution_t> generator_t;

        static generator_t generator(
            engine_t(static_cast<engine_t::result_type>( std::time(0) )),
            // this line should work and does, but it produces lots of warnings
            // thus we will use unsigned long and cast it to a uint8_t
            //distribution_t((std::numeric_limits<uint8_t>::min)(), (std::numeric_limits<uint8_t>::max)()));
            distribution_t((std::numeric_limits<unsigned long>::min)(), (std::numeric_limits<unsigned long>::max)()));

		return static_cast<boost::uint8_t>(generator() & 0xFF);
    }

} // namespace detail

/// Definition of Globally Unique Identifier type.
/// The GUID is a 16-byte (128-bit) number.
/// This class is used to represent value stored as Project Identifier
/// in public header block (see Header) of a LAS file.
/// All files in a unique project should have the same value of
/// the Project Identifier. It is used together with File Source ID to
/// uniquely identify every LAS, globally.
///
/// \see About GUID in Wikipedia http://en.wikipedia.org/wiki/Globally_Unique_Identifier 
class LAS_DLL guid
{
public:

    /// Default constructor initializes GUID as null.
    /// \exception nothrow
    /// \post guid::is_null() == true.
    guid() /* throw() */
    {
        data_.assign(0);
    }

    /// Initializes from textual representation of valid GUID.
    /// \param str - non-null pointer to string with GUID text.
    /// \exception std::invalid_argument if construction failed.
    /// \post guid::is_null() == false.
    explicit guid(char const* const str)
    {
        if (0 == str)
            throw_invalid_argument();
        construct(std::string(str));
    }

    /// Initializes from textual representation of valid GUID.
    /// \param str - string with GUID text.
    /// \exception std::invalid_argument if construction failed.
    /// \post guid::is_null() == false.
    template <typename ch, typename char_traits, typename alloc>
    explicit guid(std::basic_string<ch, char_traits, alloc> const& str)
    {
        construct(str);
    }

    /// Initializes from 4-fields structure.
    /// \param d1 - field of first 32 bits of GUID number.
    /// \param d2 - field of subsequent 16 bits of GUID number.
    /// \param d3 - third field of 16 bits of GUID number.
    /// \param d4 - last 64 bits of GUID number.
    /// \exception std::invalid_argument if construction failed.
    /// \post guid::is_null() == false.
    guid(boost::uint32_t const& d1, boost::uint16_t const& d2, boost::uint16_t const& d3, boost::uint8_t const (&d4)[8])
    {
        construct(d1, d2, d3, d4);
    }

    /// Copy constructor.
    /// \exception nothrow
    guid(guid const& rhs) /* throw() */
    {
        data_ = rhs.data_;
    }

    /// Destructor.
    /// \exception nothrow
    ~guid() /* throw() */
    {}

    /// Assignment operator.
    /// \exception nothrow
    guid& operator=(guid const& rhs) /* throw() */
    {
        if (&rhs != this)
        {
             data_ = rhs.data_;
        }
        return *this;
    }

    /// Equality operator.
    /// \exception nothrow
    bool operator==(guid const& rhs) const /* throw() */
    {
        return data_ == rhs.data_;
    }

    /// Inequality operator.
    /// \exception nothrow
    bool operator!=(guid const& rhs) const /* throw() */
    {
        return (!(*this == rhs));
    }

    /// Less-than operator.
    /// \param rhs - GUID object on the right-hand side of the comparison.
    /// \return true if the GUID on the left-hand side is lexicographically less
    /// than GUID on the right side; false otherwise.
    /// \exception nothrow
    bool operator<(guid const& rhs) const /* throw() */
    {
        return data_ < rhs.data_;
    }
    
    /// More-than operator.
    /// \param rhs - GUID object on the right-hand side of the comparison.
    /// \return true if the GUID on the left-hand side is not lexicographically less
    /// than GUID on the right side; false otherwise.
    /// \exception nothrow
    bool operator>(guid const& rhs) const /* throw() */
    {
        return data_ > rhs.data_;
    }

    /// Less-than-or-equal-to operator.
    /// \param rhs - GUID object on the right-hand side of the comparison.
    /// \exception nothrow
    bool operator<=(guid const& rhs) const /* throw() */
    {
        return data_ <= rhs.data_;
    }

    /// More-than-or-equal-to operator.
    /// \param rhs - GUID object on the right-hand side of the comparison.
    /// \exception nothrow
    bool operator>=(guid const& rhs) const /* throw() */
    {
        return data_ >= rhs.data_;
    }

    /// Test if the GUID object is null GUID or not.
    /// Null means GUID number value is Zero.
    /// \return true if the GUID is null; false otherwise
    /// \exception nothrow
    bool is_null() const /* throw() */
    {
        return ((*this) == null());
    }

    /// Generate textual representation of the GUID number.
    /// Specialization for std::string type.
    /// \return string with textual representation of GUID
    /// \exception std::runtime_error - thrown on GUID to string conversion failure
    /// \post guid::to_string().empty() == false.
    std::string to_string() const
    {
        return to_basic_string<std::string::value_type, std::string::traits_type, std::string::allocator_type>();
    }
    
    /// Generic generator of textual representation of the GUID number.
    /// \exception std::runtime_error - thrown on GUID to string conversion failure
    /// \post guid::to_basic_string().empty() == false.
    template <typename ch, typename char_traits, typename alloc>
    std::basic_string<ch, char_traits, alloc> to_basic_string() const
    {
        std::basic_string<ch, char_traits, alloc> s;
        std::basic_stringstream<ch, char_traits, alloc> ss;
        liblas::guid const& g = *this;
        ss << g;
        if (!ss || !(ss >> s))
        {
            throw std::runtime_error("failed to convert guid to string");
        }

        assert(!s.empty());
        return s;
    }

    /// Size of the GUID number in bytes.
    /// \return The number of bytes is constant for all GUID objects
    /// and equal to 16 bytes (128-bit number).
    size_t byte_count() const /* throw() */
    {
        return data_.size();
    }

    size_t size() const /* throw() */
    {
        return byte_count();
    }

    /// Send bytes of GUID data to sequenec of bytes using given output iterator.
    /// \exception nothrow
    template <typename ByteOutputIterator>
    void output_bytes(ByteOutputIterator out) const
    {
        std::copy(data_.begin(), data_.end(), out);
    }

    /// Separate bytes of GUID data to distinct buffers.
    /// \param d1 - buffer for first 32 bits of GUID number.
    /// \param d2 - buffer for 16 bits of second chunk of GUID number.
    /// \param d3 - buffer for 16 bits of third chunk of GUID number.
    /// \param d4 - buffer for last 64 bits of GUID number.
    /// \exception nothrow
    void output_data(boost::uint32_t& d1, boost::uint16_t& d2, boost::uint16_t& d3, boost::uint8_t (&d4)[8]) const
    {
        d1 = d2 = d3 = 0;
        std::size_t pos = 0;
        int const charbit = std::numeric_limits<boost::uint8_t>::digits;
        
        for (; pos < 4; ++pos)
        {
            
            d1 <<= charbit;
            d1 |= static_cast<unsigned char>(data_[pos]);
        }

        for (; pos < 6; ++pos)
        {
            d2 <<= charbit;
            d2 |= static_cast<unsigned char>(data_[pos]);
        }

        for (; pos < 8; ++pos)
        {
            d3 <<= charbit;
            d3 |= static_cast<unsigned char>(data_[pos]);
        }

        for (std::size_t j = 0; j < sizeof(d4); ++j)
        {
            d4[j] = data_[j + 8];
        }
    }

    /// Null GUID number generator.
    /// \return null-initialized instance of GUID number.
    /// \exception nothrow
    static guid const& null() /* throw() */
    {
        static const guid n;
        return n;
    }

    /// Random GUID number generator.
    /// \return random-initialized instance of GUID number.
    /// \exception nothrow
    static guid create()
    {
        return create_random_based();
    }
    
    /// Create GUID number based on calculation of SHA1 has for given name.
    /// \exception std::runtime_error on GUID creation failure.
    static guid create(guid const& namespace_guid, char const* name, int name_length)
    {
        return create_name_based(namespace_guid, name, name_length);
    }

    /// Return flag indicating if bracket text form of GUID on output is set.
    /// \exception nothrow
    static inline bool get_showbraces(std::ios_base & iosbase)
    {
        return (iosbase.iword(get_showbraces_index()) != 0);
    }

    /// Request to bracket text form of GUID on output.
    /// \exception nothrow
    static inline void set_showbraces(std::ios_base & iosbase, bool showbraces)
    {
        iosbase.iword(get_showbraces_index()) = showbraces;
    }

    /// Request to bracket text form of GUID on output.
    /// \exception nothrow
    static inline std::ios_base& showbraces(std::ios_base& iosbase)
    {
        set_showbraces(iosbase, true);
        return iosbase;
    }
    static inline std::ios_base& noshowbraces(std::ios_base& iosbase)
    {
        set_showbraces(iosbase, false);
        return iosbase;
    }
    
    /// Overloaded output stream operator for guid type.
    friend std::ostream& operator<<(std::ostream& os, guid const& g);

    /// Overloaded input stream operator for guid type.
    friend std::istream& operator>>(std::istream& is, guid &g);

private:

    void throw_invalid_argument() const
    {
        throw std::invalid_argument("invalid guid string");
    }

    template <typename ch, typename char_traits, typename alloc>
    void construct(std::basic_string<ch, char_traits, alloc> const& str)
    {
        std::basic_stringstream <ch, char_traits, alloc > ss;
        if (!(ss << str) || !(ss >> *this))
        {
            throw_invalid_argument();
        }
    }

    void construct(boost::uint32_t const& d1, boost::uint16_t const& d2, boost::uint16_t const& d3, boost::uint8_t const (&d4)[8])
    {
        std::ostringstream ss;
        ss.flags(std::ios::hex);        
        ss.fill('0');

        ss.width(8);
        ss << d1;
        ss << '-';

        ss.width(4);
        ss << d2;
        ss << '-';

        ss.width(4);
        ss << d3;
        ss << '-';

        for (std::size_t i = 0; i < sizeof(d4); ++i)
        {
            ss.width(2);
            ss << static_cast<boost::uint32_t>(d4[i]);
            if (1 == i)
                ss << '-';
        }

        construct(ss.str());
    }

    //random number based
    static guid create_random_based() // throw()
    {
        guid result;
        static bool init_rand = true;
        if (init_rand)
        {
            std::srand(static_cast<unsigned int>(std::time(0)));
            init_rand = false;
        }
        
        for (size_t i = 0; i < result.data_.size(); i++)
        {
            result.data_[i] = detail::random_byte();
        }
    
        // set variant
        // should be 0b10xxxxxx
        result.data_[8] &= 0xBF;
        result.data_[8] |= 0x80;
        
       // set version
        // should be 0b0100xxxx
        result.data_[6] &= 0x4F; //0b01001111
        result.data_[6] |= 0x40; //0b01000000
    
        return result;
    }
    
    // name based
    static guid create_name_based(guid const& namespace_guid, char const* name, int name_length)
    {
        using boost::uint8_t;
        
        detail::SHA1 sha1;

        sha1.Input(namespace_guid.data_);
        sha1.Input(name, name_length);
		boost::array<unsigned int, 5> digest;
        digest.assign(0);
        
        if (!sha1.Result(digest))
        {
            throw std::runtime_error("create error");
        }
        
        guid result;
        for (int i = 0; i < 4; ++i)
        {
            
            result.data_[i*4+0] = static_cast<uint8_t>((digest[i] >> 24) & 0xFF);
            result.data_[i*4+1] = static_cast<uint8_t>((digest[i] >> 16) & 0xFF);
            result.data_[i*4+2] = static_cast<uint8_t>((digest[i] >> 8) & 0xFF);
            result.data_[i*4+3] = static_cast<uint8_t>((digest[i] >> 0) & 0xFF);
        }
        
        // set variant
        // should be 0b10xxxxxx
        result.data_[8] &= 0xBF;
        result.data_[8] |= 0x80;
        
        // set version
        // should be 0b0101xxxx
        result.data_[6] &= 0x5F; //0b01011111
        result.data_[6] |= 0x50; //0b01010000
    
        return result;
    }

    static int get_showbraces_index()
    {
        static int index = std::ios_base::xalloc();
        return index;
    }
    
private:

    ::boost::array<boost::uint8_t, 16> data_;
};

inline std::ostream& operator<<(std::ostream& os, guid const& g)
{
    using namespace std;

    // TODO: If optional support of Boost is added,
    // use Boost I/O State Savers for safe RAII.
    std::ios_base::fmtflags flags_saver(os.flags());
    std::streamsize width_saver(os.width());
    std::ostream::char_type fill_saver(os.fill());

    const std::ostream::sentry ok(os);
    if (ok)
    {
        bool showbraces = guid::get_showbraces(os);
        if (showbraces)
        {
            os << '{';
        }
        os << hex;
        os.fill('0');
        for (size_t i = 0; i < g.size(); ++i)
        {
            os.width(2);
            os << static_cast<unsigned int>(g.data_[i]);
            if (i == 3 || i == 5 || i == 7 || i == 9)
            {
                os << '-';
            }
        }
        if (showbraces)
        {
            os << '}';
        }
    }

    os.flags(flags_saver);
    os.width(width_saver);
    os.fill(fill_saver);

    return os;
}

inline std::istream& operator>>(std::istream& is, guid &g)
{
    using namespace std;

    typedef std::istream::char_type char_type;
    guid temp_guid;
    const std::istream::sentry ok(is);
    if (ok)
    {
        char_type c;
        c = static_cast<char_type>(is.peek());
        bool bHaveBraces = false;
        if (c == '{')
        {
            bHaveBraces = true;
            is >> c; // read brace
        }

        for (size_t i = 0; i < temp_guid.size() && is; ++i)
        {
            std::stringstream ss;

            // read 2 characters into stringstream
            is >> c; ss << c;
            is >> c; ss << c;

            // extract 2 characters from stringstream as a hex number
            unsigned int val = 0;
            ss >> hex >> val;
            if (!ss)
            {
                is.setstate(ios_base::failbit);
            }

            // check that val is within valid range
            if (val > 255)
            {
                is.setstate(ios_base::badbit);
            }

            temp_guid.data_[i] = static_cast<boost::uint8_t>(val);

            if (is)
            {
                if (i == 3 || i == 5 || i == 7 || i == 9)
                {
                    is >> c;
                    if (c != '-')
                        is.setstate(ios_base::failbit);
                }
            }
        }

        if (bHaveBraces && is)
        {
            is >> c;
            if (c != '}')
                is.setstate(ios_base::failbit);
        }
        
        if (is)
        {
            g = temp_guid;
        }
    }

    return is;
}

} //namespace liblas

#endif // LIBLAS_GUID_HPP_INCLUDED
