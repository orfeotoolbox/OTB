// $Id: laspoint_test.cpp 637 2008-05-02 02:50:47Z mloskot $
//
// (C) Copyright Mateusz Loskot 2008, mateusz@loskot.net
// Distributed under the BSD License
// (See accompanying file LICENSE.txt or copy at
// http://www.opensource.org/licenses/bsd-license.php)
//
#include <liblas/laspoint.hpp>
#include <liblas/exception.hpp>
#include <tut/tut.hpp>
#include <bitset>
#include <stdexcept>
#include "common.hpp"

namespace tut
{ 
    struct laspoint_data
    {
        liblas::LASPoint m_default;
    };

    typedef test_group<laspoint_data> tg;
    typedef tg::object to;

    tg test_group_laspoint("liblas::LASPoint");

    // Test default construction
    template<>
    template<>
    void to::test<1>()
    {
        test_default_point(m_default);
    }

    // Test copy constructor
    template<>
    template<>
    void to::test<2>()
    {
        liblas::LASPoint p(m_default);

        test_default_point(p);
    }

    // Test assignment operator
    template<>
    template<>
    void to::test<3>()
    {
        liblas::LASPoint p;
        p = m_default;

        test_default_point(p);
    }

    // Test equal-to operator
    template<>
    template<>
    void to::test<4>()
    {
        liblas::LASPoint p;
        
        ensure("points are not equal", m_default == p);
    }

    // Test not-equal-to operator
    template<>
    template<>
    void to::test<5>()
    {
        liblas::LASPoint p;
        p.SetCoordinates(1.123, 2.456, 3.789);
        
        ensure("points are equal", m_default != p);
    }

    // Test Get/SetReturnNumber
    template<>
    template<>
    void to::test<6>()
    {
        ensure_equals("invalid default return number",
                      m_default.GetReturnNumber(), 0);

        liblas::uint16_t const num1 = 7;
        m_default.SetReturnNumber(num1);
        ensure_equals("invalid return number",
                      m_default.GetReturnNumber(), num1);

        liblas::uint16_t const num2 = 3;
        m_default.SetReturnNumber(num2);
        ensure_equals("invalid return number",
                      m_default.GetReturnNumber(), num2);

        try
        {
            liblas::uint16_t const outofrange = 8;
            m_default.SetReturnNumber(outofrange);
            m_default.Validate(); // throws

            // XXX - mloskot: Flag gets reset because of value overflow,
            // so in-range value is returned.
            //ensure("std::invalid_point_data not thrown", false);
        }
        catch (liblas::invalid_point_data const& e)
        {
            ensure_equals(e.who() & liblas::LASPoint::eReturnNumber,
                liblas::LASPoint::eReturnNumber);
        }
    }

    // Test Get/SetNumberOfReturns
    template<>
    template<>
    void to::test<7>()
    {
        ensure_equals("invalid default number of returns",
                      m_default.GetNumberOfReturns(), 0);

        liblas::uint16_t const num1 = 7;
        m_default.SetNumberOfReturns(num1);
        ensure_equals("invalid number of returns",
                      m_default.GetNumberOfReturns(), num1);

        liblas::uint16_t const num2 = 3;
        m_default.SetNumberOfReturns(num2);
        ensure_equals("invalid number of returns",
                      m_default.GetNumberOfReturns(), num2);

        try
        {
            liblas::uint16_t const outofrange = 8;
            m_default.SetNumberOfReturns(outofrange);
            m_default.Validate(); // throws

            // XXX - mloskot: Flag gets reset because of value overflow,
            // so in-range value is returned.
            //ensure("std::invalid_point_data not thrown", false);
        }
        catch (liblas::invalid_point_data const& e)
        {
            ensure_equals(e.who() & liblas::LASPoint::eNumberOfReturns,
                liblas::LASPoint::eNumberOfReturns);
        }
    }

    // Test Get/SetScanDirection
    template<>
    template<>
    void to::test<8>()
    {
        ensure_equals("invalid default scan direction flag",
                      m_default.GetScanDirection(), 0);

        liblas::uint16_t const positive = 1;
        m_default.SetScanDirection(positive);
        ensure_equals("invalid scan direction flag",
                      m_default.GetScanDirection(), positive);

        liblas::uint16_t const negative = 0;
        m_default.SetScanDirection(negative);
        ensure_equals("invalid scan direction flag",
                      m_default.GetScanDirection(), negative);

        try
        {
            liblas::uint16_t const outofrange = 2;
            m_default.SetScanDirection(outofrange);
            m_default.Validate(); // throws

            // XXX - mloskot: Flag gets reset because of value overflow,
            // so in-range value is returned.
            //ensure("std::invalid_point_data not thrown", false);
        }
        catch (liblas::invalid_point_data const& e)
        {
            ensure_equals(e.who() & liblas::LASPoint::eScanDirection,
                liblas::LASPoint::eScanDirection);
        }
    }

    // Test Get/SetFlightLineEdge
    template<>
    template<>
    void to::test<9>()
    {
        ensure_equals("invalid default edge of flight line",
                      m_default.GetFlightLineEdge(), 0);

        liblas::uint16_t const endofscan = 1;
        m_default.SetFlightLineEdge(endofscan);
        ensure_equals("invalid edge of flight line",
                      m_default.GetFlightLineEdge(), endofscan);

        liblas::uint16_t const notendofscan = 0;
        m_default.SetFlightLineEdge(notendofscan);
        ensure_equals("invalid edge of flight line",
                      m_default.GetFlightLineEdge(), notendofscan);

        try
        {
            liblas::uint16_t const outofrange = 2;
            m_default.SetFlightLineEdge(outofrange);
            m_default.Validate(); // throws

            // XXX - mloskot: Flag gets reset because of value overflow,
            // so in-range value is returned.
            //ensure("std::invalid_point_data not thrown", false);
        }
        catch (liblas::invalid_point_data const& e)
        {
            ensure_equals(e.who() & liblas::LASPoint::eFlightLineEdge,
                liblas::LASPoint::eFlightLineEdge);
        }
    }

    // Test Get/SetScanFlags
    template<>
    template<>
    void to::test<10>()
    {
        std::bitset<8> zeros;
        std::bitset<8> bits = m_default.GetScanFlags();
        
        ensure_equals("invalid default scan flags", bits, zeros);

        m_default.SetReturnNumber(3);
        m_default.SetNumberOfReturns(7);
        m_default.SetScanDirection(0);
        m_default.SetFlightLineEdge(1);

        std::bitset<8> expected(std::string("10111011"));
        bits = m_default.GetScanFlags();
        
        ensure_equals("invalid scan flags pattern", bits, expected);

        liblas::LASPoint copy(m_default);
        bits = copy.GetScanFlags();
        
        ensure_equals("invalid copy of scan flags pattern", bits, expected);
    }

    // Test Get/SetClassification
    template<>
    template<>
    void to::test<11>()
    {
        ensure_equals("invalid default classification",
            m_default.GetClassification(), 0);

        liblas::uint8_t const begclass = 0;
        m_default.SetClassification(begclass);
        ensure_equals("invalid classification",
            m_default.GetClassification(), begclass);

        liblas::uint8_t const endclass = 31;
        m_default.SetClassification(endclass);
        ensure_equals("invalid classification",
            m_default.GetClassification(), endclass);
    }

    // Test Get/SetScanAngleRank
    template<>
    template<>
    void to::test<12>()
    {
        ensure_equals("invalid default scan angle rank",
                      m_default.GetScanAngleRank(), 0);

        liblas::int8_t const rank1 = -90;
        m_default.SetScanAngleRank(rank1);
        ensure_equals("invalid scan angle rank",
                      m_default.GetScanAngleRank(), rank1);

        liblas::int8_t const rank2 = 90;
        m_default.SetScanAngleRank(rank2);
        ensure_equals("invalid scan angle rank",
                      m_default.GetScanAngleRank(), rank2);

        try
        {
            liblas::int8_t const outofrange = 91;
            m_default.SetScanAngleRank(outofrange);
            m_default.Validate(); // throws

            ensure("invalid_point_data not thrown", false);
        }
        catch (liblas::invalid_point_data const& e)
        {
            ensure_equals(e.who() & liblas::LASPoint::eScanAngleRank,
                liblas::LASPoint::eScanAngleRank);
        }
    }

    // Test Get/SetUserData
    template<>
    template<>
    void to::test<13>()
    {
        ensure_equals("invalid default user data value",
                      m_default.GetUserData(), 0);

        liblas::uint8_t const data = 7; // dummy value
        m_default.SetUserData(data);

        ensure_equals("invalid user data value",
                      m_default.GetUserData(), data);
    }

    // Test Get/SetTime
    template<>
    template<>
    void to::test<14>()
    {
        ensure_equals("invalid default time",
            m_default.GetTime(), 0);

        double const time = 3.14; // dummy value
        m_default.SetTime(time);

        ensure_equals("invalid time",
            m_default.GetTime(), time);
    }

    // Test IsValid method
    template<>
    template<>
    void to::test<15>()
    {
        {
            liblas::LASPoint p;
            liblas::uint16_t const outofrange = 8;
            p.SetReturnNumber(outofrange);
            // XXX: Bit flag overflowed, so point data recognized as valid
            //ensure_not(p.IsValid());
        }

        {
            liblas::LASPoint p;
            liblas::uint16_t const outofrange = 8;
            p.SetNumberOfReturns(outofrange);
            // XXX: Bit flag overflowed, so point data recognized as valid
            //ensure_not(p.IsValid());
        }

        {
            liblas::LASPoint p;
            liblas::uint16_t const outofrange = 2;
            p.SetScanDirection(outofrange);
            // XXX: Bit flag overflowed, so point data recognized as valid
            //ensure_not(p.IsValid());
        }

        {
            liblas::LASPoint p;
            liblas::uint16_t const outofrange = 2;
            p.SetFlightLineEdge(outofrange);
            // XXX: Bit flag overflowed, so point data recognized as valid
            //ensure_not(p.IsValid());
        }

        {
            liblas::LASPoint p;
            liblas::int8_t const outofrange = 91;
            p.SetScanAngleRank(outofrange);
            ensure_not(p.IsValid());
        }
    }
}

