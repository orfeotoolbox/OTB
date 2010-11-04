// $Id$
//
// (C) Copyright Mateusz Loskot 2008, mateusz@loskot.net
// Distributed under the BSD License
// (See accompanying file LICENSE.txt or copy at
// http://www.opensource.org/licenses/bsd-license.php)
//
#include <liblas/liblas.hpp>
#include <liblas/laspoint.hpp>
#include <liblas/lascolor.hpp>
#include <liblas/exception.hpp>
#include <tut/tut.hpp>

#include <bitset>
#include <stdexcept>
#include <vector>
#include <fstream>

#include "common.hpp"
#include "liblas_test.hpp"

namespace tut
{ 
    struct laspoint_data
    {
        liblas::Point m_default;
        std::string file10_;

        laspoint_data()
            : file10_(g_test_data_path + "//TO_core_last_clip.las")
        {}
    };

    typedef test_group<laspoint_data> tg;
    typedef tg::object to;

    tg test_group_laspoint("liblas::Point");

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
        liblas::Point p(m_default);

        test_default_point(p);
    }

    // Test assignment operator
    template<>
    template<>
    void to::test<3>()
    {
        liblas::Point p;
        p = m_default;

        test_default_point(p);
    }

    // Test equal-to operator
    template<>
    template<>
    void to::test<4>()
    {
        liblas::Point p;
        
        ensure("points are not equal", m_default == p);
    }

    // Test not-equal-to operator
    template<>
    template<>
    void to::test<5>()
    {
        liblas::Point p;
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

        boost::uint16_t const num1 = 7;
        m_default.SetReturnNumber(num1);
        ensure_equals("invalid return number",
                      m_default.GetReturnNumber(), num1);

        boost::uint16_t const num2 = 3;
        m_default.SetReturnNumber(num2);
        ensure_equals("invalid return number",
                      m_default.GetReturnNumber(), num2);

        try
        {
            boost::uint16_t const outofrange = 8;
            m_default.SetReturnNumber(outofrange);
            m_default.Validate(); // throws

            // XXX - mloskot: Flag gets reset because of value overflow,
            // so in-range value is returned.
            //ensure("std::invalid_point_data not thrown", false);
        }
        catch (liblas::invalid_point_data const& e)
        {
            ensure_equals(e.who(), liblas::Point::eReturnNumber,
                liblas::Point::eReturnNumber);
        }
    }

    // Test Get/SetNumberOfReturns
    template<>
    template<>
    void to::test<7>()
    {
        ensure_equals("invalid default number of returns",
                      m_default.GetNumberOfReturns(), 0);

        boost::uint16_t const num1 = 7;
        m_default.SetNumberOfReturns(num1);
        ensure_equals("invalid number of returns",
                      m_default.GetNumberOfReturns(), num1);

        boost::uint16_t const num2 = 3;
        m_default.SetNumberOfReturns(num2);
        ensure_equals("invalid number of returns",
                      m_default.GetNumberOfReturns(), num2);

        try
        {
            boost::uint16_t const outofrange = 8;
            m_default.SetNumberOfReturns(outofrange);
            m_default.Validate(); // throws

            // XXX - mloskot: Flag gets reset because of value overflow,
            // so in-range value is returned.
            //ensure("std::invalid_point_data not thrown", false);
        }
        catch (liblas::invalid_point_data const& e)
        {
            ensure_equals(e.who(), liblas::Point::eNumberOfReturns,
                liblas::Point::eNumberOfReturns);
        }
    }

    // Test Get/SetScanDirection
    template<>
    template<>
    void to::test<8>()
    {
        ensure_equals("invalid default scan direction flag",
                      m_default.GetScanDirection(), 0);

        boost::uint16_t const positive = 1;
        m_default.SetScanDirection(positive);
        ensure_equals("invalid scan direction flag",
                      m_default.GetScanDirection(), positive);

        boost::uint16_t const negative = 0;
        m_default.SetScanDirection(negative);
        ensure_equals("invalid scan direction flag",
                      m_default.GetScanDirection(), negative);

        try
        {
            boost::uint16_t const outofrange = 2;
            m_default.SetScanDirection(outofrange);
            m_default.Validate(); // throws

            // XXX - mloskot: Flag gets reset because of value overflow,
            // so in-range value is returned.
            //ensure("std::invalid_point_data not thrown", false);
        }
        catch (liblas::invalid_point_data const& e)
        {
            ensure_equals(e.who(), liblas::Point::eScanDirection,
                liblas::Point::eScanDirection);
        }
    }

    // Test Get/SetFlightLineEdge
    template<>
    template<>
    void to::test<9>()
    {
        ensure_equals("invalid default edge of flight line",
                      m_default.GetFlightLineEdge(), 0);

        boost::uint16_t const endofscan = 1;
        m_default.SetFlightLineEdge(endofscan);
        ensure_equals("invalid edge of flight line",
                      m_default.GetFlightLineEdge(), endofscan);

        boost::uint16_t const notendofscan = 0;
        m_default.SetFlightLineEdge(notendofscan);
        ensure_equals("invalid edge of flight line",
                      m_default.GetFlightLineEdge(), notendofscan);

        try
        {
            boost::uint16_t const outofrange = 2;
            m_default.SetFlightLineEdge(outofrange);
            m_default.Validate(); // throws

            // XXX - mloskot: Flag gets reset because of value overflow,
            // so in-range value is returned.
            //ensure("std::invalid_point_data not thrown", false);
        }
        catch (liblas::invalid_point_data const& e)
        {
            ensure_equals(e.who(), liblas::Point::eFlightLineEdge,
                liblas::Point::eFlightLineEdge);
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

        liblas::Point copy(m_default);
        bits = copy.GetScanFlags();
        
        ensure_equals("invalid copy of scan flags pattern", bits, expected);
    }

    // Test Get/SetClassification
    template<>
    template<>
    void to::test<11>()
    {
        ensure_equals("invalid default classification",
            m_default.GetClassification(), liblas::Classification::bitset_type());

        liblas::Classification c;
        
        boost::uint8_t const begclass = 0;
        c.SetClass(begclass);
        m_default.SetClassification(c);
        
        ensure_equals("invalid class index",
            m_default.GetClassification().GetClass(), begclass);

        boost::uint8_t const endclass = 31;
        c.SetClass(endclass);
        m_default.SetClassification(c);
        ensure_equals("invalid class index",
            m_default.GetClassification().GetClass(), endclass);
    }

    // Test Get/SetScanAngleRank
    template<>
    template<>
    void to::test<12>()
    {
        ensure_equals("invalid default scan angle rank",
                      m_default.GetScanAngleRank(), 0);

        boost::int8_t const rank1 = -90;
        m_default.SetScanAngleRank(rank1);
        ensure_equals("invalid scan angle rank",
                      m_default.GetScanAngleRank(), rank1);

        boost::int8_t const rank2 = 90;
        m_default.SetScanAngleRank(rank2);
        ensure_equals("invalid scan angle rank",
                      m_default.GetScanAngleRank(), rank2);

        try
        {
            boost::int8_t const outofrange = 91;
            m_default.SetScanAngleRank(outofrange);
            m_default.Validate(); // throws

            ensure("invalid_point_data not thrown", false);
        }
        catch (liblas::invalid_point_data const& e)
        {
            ensure_equals(e.who(), liblas::Point::eScanAngleRank,
                liblas::Point::eScanAngleRank);
        }
    }

    // Test Get/SetUserData
    template<>
    template<>
    void to::test<13>()
    {
        ensure_equals("invalid default user data value",
                      m_default.GetUserData(), 0);

        boost::uint8_t const data = 7; // dummy value
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
            liblas::Point p;
            boost::uint16_t const outofrange = 8;
            p.SetReturnNumber(outofrange);
            // XXX: Bit flag overflowed, so point data recognized as valid
            //ensure_not(p.IsValid());
        }

        {
            liblas::Point p;
            boost::uint16_t const outofrange = 8;
            p.SetNumberOfReturns(outofrange);
            // XXX: Bit flag overflowed, so point data recognized as valid
            //ensure_not(p.IsValid());
        }

        {
            liblas::Point p;
            boost::uint16_t const outofrange = 2;
            p.SetScanDirection(outofrange);
            // XXX: Bit flag overflowed, so point data recognized as valid
            //ensure_not(p.IsValid());
        }

        {
            liblas::Point p;
            boost::uint16_t const outofrange = 2;
            p.SetFlightLineEdge(outofrange);
            // XXX: Bit flag overflowed, so point data recognized as valid
            //ensure_not(p.IsValid());
        }

        {
            liblas::Point p;
            boost::int8_t const outofrange = 91;
            p.SetScanAngleRank(outofrange);
            ensure_not(p.IsValid());
        }
    }

    // Test Get/SetColor
    template<>
    template<>
    void to::test<16>()
    {
        ensure_equals("invalid default red color",
            m_default.GetColor().GetRed(), 0);

        ensure_equals("invalid default green color",
            m_default.GetColor().GetGreen(), 0);

        ensure_equals("invalid default blue color",
            m_default.GetColor().GetBlue(), 0);
                        
        liblas::Color color;
        color.SetRed(211);
        color.SetGreen(211);
        color.SetBlue(211);
        
        m_default.SetColor(color);

        ensure_equals("invalid set red color",
            m_default.GetColor().GetRed(), 211);

        ensure_equals("invalid set green color",
            m_default.GetColor().GetGreen(), 211);

        ensure_equals("invalid set blue color",
            m_default.GetColor().GetBlue(), 211);
        
        liblas::Point p = m_default;

        ensure_equals("invalid copied red color",
            p.GetColor().GetRed(), 211);

        ensure_equals("invalid copied green color",
            p.GetColor().GetGreen(), 211);
            
        ensure_equals("invalid copied blue color",
            p.GetColor().GetBlue(), 211);        
        
    }

    // // Test Get/SetExtraData
    // template<>
    // template<>
    // void to::test<17>()
    // {
    //     typedef std::vector<boost::uint8_t> data_t;
    //     
    //     data_t data;
    //     data.push_back(254);
    //     data.push_back(254);
    //     data.push_back(1);
    //     
    //     ensure_equals("invalid default extra data",
    //         m_default.GetExtraData().size(), 0u);
    // 
    // 
    //     m_default.SetExtraData(data);
    // 
    //     ensure_equals("invalid set extra data size",
    //         m_default.GetExtraData().size(), 3u);
    // 
    //     ensure_equals("invalid set extra data value",
    //         m_default.GetExtraData()[1], 254);
    // 
    //           
    //     
    // }

    template<>
    template<>
    void to::test<18>()
    {
        std::ifstream ifs;
        ifs.open(file10_.c_str(), std::ios::in | std::ios::binary);
        liblas::Reader reader(ifs);
        
        reader.ReadPointAt(0);
        liblas::Point p = reader.GetPoint();
        
        liblas::Schema schema = reader.GetHeader().GetSchema();

        liblas::Dimension x = schema.GetDimension("X");
        
        boost::any d = p.GetValue(x);
    
        ensure_equals("invalid GetRawY value",
            (int) p.GetRawY(), 483450000);
                
        // Scale is 0.01 for this file
        p.SetX(123);
        boost::int32_t x2 = p.GetRawX();
        ensure_equals("invalid raw x value",
            (int) x2, 12300);

        p.SetY(456);
        x2 = p.GetRawY();
        ensure_equals("invalid raw y value",
            (int) x2, 45600);      

        p.SetZ(789);
        x2 = p.GetRawZ();
        ensure_equals("invalid raw z value",
            (int) x2, 78900);          
        
    }
}

