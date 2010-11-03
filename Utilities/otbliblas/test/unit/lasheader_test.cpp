// $Id$
//
// (C) Copyright Mateusz Loskot 2008, mateusz@loskot.net
// Distributed under the BSD License
// (See accompanying file LICENSE.txt or copy at
// http://www.opensource.org/licenses/bsd-license.php)
//
#include <liblas/liblas.hpp>
#include <liblas/lasheader.hpp>
#include <liblas/lasspatialreference.hpp>
#include <liblas/guid.hpp>
#include <tut/tut.hpp>
#include <string>
#include <stdexcept>
#include "common.hpp"

namespace tut
{ 
    struct lasheader_data
    {
        liblas::Header m_default;
    };

    typedef test_group<lasheader_data> tg;
    typedef tg::object to;

    tg test_group_lasheader("liblas::Header");

    // Test default constructor
    template<>
    template<>
    void to::test<1>()
    {
        test_default_header(m_default);
    }

    // Test copy constructor
    template<>
    template<>
    void to::test<2>()
    {
        using liblas::Header;
        
        Header copy_of_default(m_default);
        test_default_header(copy_of_default);

        std::string sig("LASF and garbage");

        Header h1;
        
        h1.SetFileSignature(sig);
        ensure_not(h1.GetFileSignature() == sig);
        ensure_equals(h1.GetFileSignature().size(), std::string::size_type(4));
        ensure_equals(h1.GetFileSignature(), Header::FileSignature);

        Header h2(h1);

        ensure_not(h2.GetFileSignature() == sig);
        ensure_equals(h2.GetFileSignature().size(), std::string::size_type(4));
        ensure_equals(h2.GetFileSignature(), Header::FileSignature);
    }


    // Test assignment operator
    template<>
    template<>
    void to::test<3>()
    {
        using liblas::Header;
        
        Header copy_of_default;
        copy_of_default = m_default;
        test_default_header(copy_of_default);

    }

    // Test Get/SetFileSignature
    template<>
    template<>
    void to::test<4>()
    {
        using liblas::Header;

        std::string sig("LASF and garbage");

        Header h1;
        h1.SetFileSignature(sig);

        ensure_not(h1.GetFileSignature() == sig);
        ensure_equals(h1.GetFileSignature().size(), std::string::size_type(4));
        ensure_equals(h1.GetFileSignature(), Header::FileSignature);

        Header h2;
        h2 = h1;

        ensure_not(h2.GetFileSignature() == sig);
        ensure_equals(h2.GetFileSignature().size(), std::string::size_type(4));
        ensure_equals(h2.GetFileSignature(), Header::FileSignature);
    }

    // Test Get/SetFileSourceId
    template<>
    template<>
    void to::test<5>()
    {
        using liblas::Header;
        using boost::uint16_t;

        uint16_t const id1 = 1;
        uint16_t const id2 = 65535;
        uint16_t const overflowed = 0;

        Header h1;
        h1.SetFileSourceId(id1);
        ensure_equals(h1.GetFileSourceId(), id1);
        h1.SetFileSourceId(id2);
        ensure_equals(h1.GetFileSourceId(), id2);

#ifdef _MSC_VER
# pragma warning(push)
# pragma warning(disable: 4305) //  truncation from 'int' to 'boost::uint16_t'
# pragma warning(disable: 4309) // conditional expression is constant.
#endif
        // Unsigned overflow
        // Likely compiler warning: truncation from int to boost::uint16_t
        h1.SetFileSourceId(id2 + 1);
        ensure_equals(h1.GetFileSourceId(), overflowed);

#ifdef _MSC_VER
# pragma warning(push)
#endif
    }

    // Test GetReserved
    template<>
    template<>
    void to::test<6>()
    {
        liblas::Header h;
        ensure_equals(h.GetReserved(), 0);
    }

    // Test Set/GetProjectId
    template<>
    template<>
    void to::test<7>()
    {
        std::string strid("030B4A82-1B7C-11CF-9D53-00AA003C9CB6");
        liblas::guid id(strid.c_str());

        liblas::Header h;
        h.SetProjectId(id);
        
        ensure_not(h.GetProjectId().is_null());
        ensure_equals(h.GetProjectId(), id);
    }

    // Test handling minor/major version
    template<>
    template<>
    void to::test<8>()
    {
        liblas::Header h;

        h.SetVersionMajor(1);
        h.SetVersionMinor(0);
        ensure_equals(h.GetVersionMajor(), 1);
        ensure_equals(h.GetVersionMinor(), 0);

        h.SetVersionMajor(1);
        h.SetVersionMinor(1);
        ensure_equals(h.GetVersionMajor(), 1);
        ensure_equals(h.GetVersionMinor(), 1);

        try
        {
            h.SetVersionMajor(2);
            ensure("std::out_of_range was not thrown", false);
        }
        catch (std::out_of_range const& e)
        {
            ensure(e.what(), true);
        }

        try
        {
            h.SetVersionMinor(4);
            ensure("std::out_of_range was not thrown", false);
        }
        catch (std::out_of_range const& e)
        {
            ensure(e.what(), true);
        }
    }

    // Test Get/SetSystemId
    template<>
    template<>
    void to::test<9>()
    {
        using liblas::Header;

        std::string sysid1("Short Sys Id"); // 12 bytes
        std::string::size_type const len1 = sysid1.size();
        std::string sysid2("Long System Identifier - XXX YYY"); // 32 bytes
        std::string::size_type const len2 = sysid2.size();

        Header h;

        h.SetSystemId(sysid1);
        ensure_equals(h.GetSystemId(), sysid1);
        ensure_equals(h.GetSystemId().size(), len1);
        ensure_equals(h.GetSystemId(true).size(), std::string::size_type(32));

        h.SetSystemId(sysid2);
        ensure_equals(h.GetSystemId(), sysid2);
        ensure_equals(h.GetSystemId().size(), len2);
        ensure_equals(h.GetSystemId(true).size(), std::string::size_type(32));
    }

    // Test Get/SetSoftwareId
    template<>
    template<>
    void to::test<10>()
    {
        using liblas::Header;

        std::string softid1("Short Soft Id"); // 13 bytes
        std::string::size_type const len1 = softid1.size();
        std::string softid2("Long Software Identifier - XX YY"); // 32 bytes
        std::string::size_type const len2 = softid2.size();

        Header h;
        h.SetSoftwareId(softid1);
        ensure_equals(h.GetSoftwareId(), softid1);
        ensure_equals(h.GetSoftwareId().size(), len1);
        ensure_equals(h.GetSoftwareId(true).size(), std::string::size_type(32));

        h.SetSoftwareId(softid2);
        ensure_equals(h.GetSoftwareId(), softid2);
        ensure_equals(h.GetSoftwareId().size(), len2);
        ensure_equals(h.GetSoftwareId(true).size(), std::string::size_type(32));
    }

    // Test GetPointRecordsByReturnCount
    template<>
    template<>
    void to::test<11>()
    {
        typedef ::liblas::Header::RecordsByReturnArray::size_type size_type;
        typedef ::boost::uint32_t count_type;

        liblas::Header h;
        // NOTE: The committee in its infinite stupidity decided to increase the size of this array to 7 at 1.3.
        ensure(h.GetPointRecordsByReturnCount().size() >= 5);
        ensure(h.GetPointRecordsByReturnCount().size() <= 7);

        h.SetPointRecordsByReturnCount(0, 100);
        ensure(h.GetPointRecordsByReturnCount().size() >= 5);
        ensure(h.GetPointRecordsByReturnCount().size() <= 7);
        ensure_equals(h.GetPointRecordsByReturnCount().at(0), count_type(100));

        h.SetPointRecordsByReturnCount(1, 101);
        ensure(h.GetPointRecordsByReturnCount().size() >= 5);
        ensure(h.GetPointRecordsByReturnCount().size() <= 7);
        ensure_equals(h.GetPointRecordsByReturnCount().at(1), count_type(101));

        h.SetPointRecordsByReturnCount(2, 102);
        ensure(h.GetPointRecordsByReturnCount().size() >= 5);
        ensure(h.GetPointRecordsByReturnCount().size() <= 7);
        ensure_equals(h.GetPointRecordsByReturnCount().at(2), count_type(102));

        h.SetPointRecordsByReturnCount(3, 103);
        ensure(h.GetPointRecordsByReturnCount().size() >= 5);
        ensure(h.GetPointRecordsByReturnCount().size() <= 7);
        ensure_equals(h.GetPointRecordsByReturnCount().at(3), count_type(103));

        h.SetPointRecordsByReturnCount(4, 104);
        ensure(h.GetPointRecordsByReturnCount().size() >= 5);
        ensure(h.GetPointRecordsByReturnCount().size() <= 7);
        ensure_equals(h.GetPointRecordsByReturnCount().at(4), count_type(104));

        try
        {
            // 8 is out of range
            h.SetPointRecordsByReturnCount(8, 500);
            ensure("std::out_of_range not thrown", false);
        }
        catch (std::out_of_range const& e)
        {
            ensure(e.what(), true);
        }
    }

    // Test GetSRS
    template<>
    template<>
    void to::test<12>()
    {
        liblas::Header h;
        liblas::SpatialReference srs = h.GetSRS();

        ensure_equals(srs.GetProj4(), "");
        ensure_equals(srs.GetWKT(), "");

    }
}

