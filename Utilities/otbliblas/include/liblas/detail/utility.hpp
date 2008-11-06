/******************************************************************************
 * $Id: utility.hpp 883 2008-09-26 15:23:11Z hobu $
 *
 * Project:  libLAS - http://liblas.org - A BSD library for LAS format data.
 * Purpose:  A grab bucket 'o fun for C++ libLAS 
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

#ifndef LIBLAS_DETAIL_UTILITY_HPP_INCLUDED
#define LIBLAS_DETAIL_UTILITY_HPP_INCLUDED

#include <liblas/cstdint.hpp>
#include <liblas/detail/endian.hpp>
#include <algorithm>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <iosfwd>
#include <cstring>
#include <cassert>

//
// Private macros definition
//

#ifndef UNREFERENCED_PARAMETER
#define UNREFERENCED_PARAMETER(param) ((void)param)
#endif

//
// Private utilities for internal use in libLAS
//

namespace liblas { namespace detail {

/// Simple RAII wrapper.
/// It's dedicated to use with types associated with custom deleter,
/// opaque pointers and C API objects.
template <typename T>
class raii_wrapper
{
    typedef void(*deleter_type)(T* p);

public:

    raii_wrapper(T* p, deleter_type d)
        : p_(p), del_(d)
    {
        assert(0 != p_);
        assert(0 != del_);
    }

    ~raii_wrapper()
    {
        do_delete(p_);
    }

    void reset(T* p)
    {
        do_delete(p_);
        p_= p;
    }

    T* get() const
    {
        return p_;
    }

    void swap(raii_wrapper& other)
    {
        std::swap(p_, other.p_);
    }

private:

    raii_wrapper(raii_wrapper const& other);
    raii_wrapper& operator=(raii_wrapper const& rhs);

    void do_delete(T* p)
    {
        assert(del_);
        if (0 != p)
            del_(p);
    }

    T* p_;
    deleter_type del_;
};


/// Definition of variable-length record header.
struct VLRHeader
{
  uint16_t reserved;
  char userId[16]; 
  uint16_t recordId;
  uint16_t recordLengthAfterHeader;
  char description[32];
};

struct GeoKeysHeader
{
    uint16_t keyDirectoryVersion;
    uint16_t keyRevision;
    uint16_t minorRevision;
    uint16_t numberOfKeys;
};

struct GeoKeyEntry
{
  uint16_t keyId;
  uint16_t tiffTagLocation;
  uint16_t count;
  uint16_t valueOffset;
};

class VariableLengthRecord
{
    VariableLengthRecord()
        : reserved(0), record_id(0), record_length_after_header(0)
    {
        std::memset(user_id, 0, eUserIdSize);
        std::memset(description, 0, eDescriptionSize);
    }
 
    enum
    {
        eUserIdSize = 16,
        eDescriptionSize = 32
    };

    uint16_t reserved;
    int8_t user_id[eUserIdSize]; 
    uint16_t record_id;
    uint16_t record_length_after_header;
    int8_t description[eDescriptionSize];
    uint8_t data;
};

struct PointRecord
{
    PointRecord() :
        x(0), y(0), z(0),
        intensity(0),
        flags(0),
        classification(0),
        scan_angle_rank(0),
        user_data(0),
        point_source_id(0)
    {}

    int32_t x;
    int32_t y;
    int32_t z;
    uint16_t intensity;
    uint8_t flags; // TODO: Replace with portable std::bitset<8>
    uint8_t classification;
    int8_t scan_angle_rank;
    uint8_t user_data;
    uint16_t point_source_id;
};

template <typename T>
struct Point
{
    Point() : x(T()), y(T()), z(T()) {}
    Point(T const& x, T const& y, T const& z) : x(x), y(y), z(z) {}
    T x;
    T y;
    T z;

    bool equal(Point<T> const& other) const
    {
        return ((x == other.x) && (y == other.y) && (z == other.z));
    }
};

template <typename T>
bool operator==(Point<T> const& lhs, Point<T> const& rhs)
{
    return lhs.equal(rhs);
}

template <typename T>
bool operator!=(Point<T> const& lhs, Point<T> const& rhs)
{
    return (!lhs.equal(rhs));
}

template <typename T>
struct Extents
{
    typename detail::Point < T > min;
    typename detail::Point < T > max;

    bool equal(Extents<T> const& other) const
    {
        return (min == other.min && max == other.max);
    }
};

template <typename T>
bool operator==(Extents<T> const& lhs, Extents<T> const& rhs)
{
    return lhs.equal(rhs);
}

template <typename T>
bool operator!=(Extents<T> const& lhs, Extents<T> const& rhs)
{
    return (!lhs.equal(rhs));
}

template <typename T>
inline T generate_random_byte()
{
    // Requires pseudo-random numbers generator to be initialized
    // in create_random_based() function - a poor man solution.
    T const rmin = std::numeric_limits<T>::min();
    T const rmax = std::numeric_limits<T>::max();
    unsigned int const rnd = std::rand() % rmax + rmin;

    assert(rnd <= 255);
    return static_cast<T>(rnd);
}

template <typename T> 
bool compare_doubles(const T& actual, const T& expected) 
{ 
    const T epsilon = std::numeric_limits<T>::epsilon();  
    const T diff = actual - expected; 

    if ( !((diff <= epsilon) && (diff >= -epsilon )) ) 
    { 
        return false; 
    } 

    return true;
}

template<typename T>
inline char* as_buffer(T& data)
{
    return static_cast<char*>(static_cast<void*>(&data));
}

template<typename T>
inline char* as_buffer(T* data)
{
    return static_cast<char*>(static_cast<void*>(data));
}

template<typename T>
inline char const* as_bytes(T const& data)
{
    return static_cast<char const*>(static_cast<void const*>(&data));
}

template<typename T>
inline char const* as_bytes(T const* data)
{
    return static_cast<char const*>(static_cast<void const*>(data));
}

template <typename C, typename T>
inline void check_stream_state(std::basic_ios<C, T>& srtm)
{
    // NOTE: Detailed stream check disabled in optimized
    //       builds to increase performance.
#if defined(DEBUG) || defined(_DEBUG)
    // Test stream state bits
    if (srtm.eof())
        throw std::out_of_range("end of file encountered");
    else if (srtm.fail())
        throw std::runtime_error("non-fatal I/O error occured");
    else if (srtm.bad())
        throw std::runtime_error("fatal I/O error occured");
#else
    UNREFERENCED_PARAMETER(srtm);
#endif
}

#ifdef _MSC_VER
# pragma warning(push)
# pragma warning(disable: 4127) // conditional expression is constant.
#endif

template <typename T>
inline void read_n(T& dest, std::istream& src, std::streamsize const& num)
{
    // TODO: Review and redesign errors handling logic if necessary
    if (!src)
        throw std::runtime_error("detail::liblas::read_n input stream is not readable");

    src.read(detail::as_buffer(dest), num);

    // Fix little-endian
    LIBLAS_SWAP_BYTES_N(dest, num);

    check_stream_state(src);
}

template <>
inline void read_n<PointRecord>(PointRecord& dest, std::istream& src, std::streamsize const& num)
{
    // TODO: Review and redesign errors handling logic if necessary
    if (!src)
        throw std::runtime_error("detail::liblas::read_n input stream is not readable");

    src.read(detail::as_buffer(dest), num);
    check_stream_state(src);

    // Fix little-endian
    LIBLAS_SWAP_BYTES(dest.x);
    LIBLAS_SWAP_BYTES(dest.y);
    LIBLAS_SWAP_BYTES(dest.z);
    LIBLAS_SWAP_BYTES(dest.intensity);
    LIBLAS_SWAP_BYTES(dest.flags);
    LIBLAS_SWAP_BYTES(dest.classification);
    LIBLAS_SWAP_BYTES(dest.scan_angle_rank);
    LIBLAS_SWAP_BYTES(dest.user_data);
    LIBLAS_SWAP_BYTES(dest.point_source_id);
}

template <>
inline void read_n<VLRHeader>(VLRHeader& dest, std::istream& src, std::streamsize const& num)
{
    // TODO: Review and redesign errors handling logic if necessary
    if (!src)
        throw std::runtime_error("detail::liblas::read_n input stream is not readable");

    src.read(detail::as_buffer(dest), num);
    check_stream_state(src);

    // Fix little-endian
    LIBLAS_SWAP_BYTES(dest.reserved);
    LIBLAS_SWAP_BYTES(dest.recordId);
    LIBLAS_SWAP_BYTES(dest.recordLengthAfterHeader);
}

template <>
inline void read_n<std::string>(std::string& dest, std::istream& src, std::streamsize const& num)
{
    assert(dest.max_size() >= static_cast<std::string::size_type>(num));

    // TODO: Review and redesign errors handling logic if necessary
    if (!src)
        throw std::runtime_error("detail::liblas::read_n input stream is not readable");

    // Read bytes into temporary buffer then assign as string
    char* buf = new char[num];
    src.read(buf, num);
    dest.assign(buf, num);
    delete [] buf;

    assert(dest.size() == static_cast<std::string::size_type>(num));
    check_stream_state(src);
}

template <typename T>
inline void write_n(std::ostream& dest, T const& src, std::streamsize const& num)
{
    if (!dest)
        throw std::runtime_error("detail::liblas::write_n: output stream is not writable");

    // Fix little-endian
    T& tmp = const_cast<T&>(src);
    LIBLAS_SWAP_BYTES_N(tmp, num);

    dest.write(detail::as_bytes(tmp), num);
    check_stream_state(dest);
}

template <>
inline void write_n<PointRecord>(std::ostream& dest, PointRecord const& src, std::streamsize const& num)
{
    if (!dest)
        throw std::runtime_error("detail::liblas::write_n: output stream is not writable");

    // Fix little-endian
    PointRecord& tmp = const_cast<PointRecord&>(src);
    LIBLAS_SWAP_BYTES(tmp.x);
    LIBLAS_SWAP_BYTES(tmp.y);
    LIBLAS_SWAP_BYTES(tmp.z);
    LIBLAS_SWAP_BYTES(tmp.intensity);
    LIBLAS_SWAP_BYTES(tmp.flags);
    LIBLAS_SWAP_BYTES(tmp.classification);
    LIBLAS_SWAP_BYTES(tmp.scan_angle_rank);
    LIBLAS_SWAP_BYTES(tmp.user_data);
    LIBLAS_SWAP_BYTES(tmp.point_source_id);

    dest.write(detail::as_bytes(tmp), num);
    check_stream_state(dest);
}

template <>
inline void write_n<VLRHeader>(std::ostream& dest, VLRHeader const& src, std::streamsize const& num)
{
    if (!dest)
        throw std::runtime_error("detail::liblas::write_n: output stream is not writable");

    // Fix little-endian
    VLRHeader& tmp = const_cast<VLRHeader&>(src);
    LIBLAS_SWAP_BYTES(tmp.reserved);
    LIBLAS_SWAP_BYTES(tmp.recordId);
    LIBLAS_SWAP_BYTES(tmp.recordLengthAfterHeader);
    
    dest.write(detail::as_bytes(tmp), num);
    check_stream_state(dest);
}

template <>
inline void write_n<std::string>(std::ostream& dest, std::string const& src, std::streamsize const& num)
{
    if (!dest)
        throw std::runtime_error("detail::liblas::write_n: output stream is not writable");

    dest.write(src.c_str(), num);
    check_stream_state(dest);
}

#ifdef _MSC_VER
# pragma warning(push)
#endif

}} // namespace liblas::detail

#endif // LIBLAS_DETAIL_UTILITY_HPP_INCLUDED
