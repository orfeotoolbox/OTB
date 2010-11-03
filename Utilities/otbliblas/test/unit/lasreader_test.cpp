// $Id$
//
// (C) Copyright Mateusz Loskot 2008, mateusz@loskot.net
// Distributed under the BSD License
// (See accompanying file LICENSE.txt or copy at
// http://www.opensource.org/licenses/bsd-license.php)
//
#include <liblas/lasreader.hpp>
#include <liblas/lasheader.hpp>
#include <liblas/laspoint.hpp>
#include <liblas/liblas.hpp>
#include <tut/tut.hpp>
#include <fstream>
#include <string>
#include "liblas_test.hpp"
#include "common.hpp"

namespace tut
{ 
    struct lasreader_data
    {
        std::string file10_;

        lasreader_data()
            : file10_(g_test_data_path + "//TO_core_last_clip.las")
        {}
    };

    typedef test_group<lasreader_data> tg;
    typedef tg::object to;

    tg test_group_lasreader("liblas::Reader");

    // Test user-declared constructor
    template<>
    template<>
    void to::test<1>()
    {
        std::ifstream ifs;
        ifs.open(file10_.c_str(), std::ios::in | std::ios::binary);
        liblas::Reader reader(ifs);

        ensure_equals(reader.GetHeader().GetVersionMinor(), 0);
    }

    // Test reading header
    template<>
    template<>
    void to::test<2>()
    {
        std::ifstream ifs;
        ifs.open(file10_.c_str(), std::ios::in | std::ios::binary);
        liblas::Reader reader(ifs);
        liblas::Header const& hdr = reader.GetHeader();

        test_file10_header(hdr);
    }

    // Test GetPoint method
    template<>
    template<>
    void to::test<3>()
    {
        std::ifstream ifs;
        ifs.open(file10_.c_str(), std::ios::in | std::ios::binary);
        liblas::Reader reader(ifs);

        // uninitialized point object, a null-point
        liblas::Point const& p = reader.GetPoint();
        ensure(p == liblas::Point());
    }

    // Test ReadPoint and GetPoint pair
    template<>
    template<>
    void to::test<4>()
    {
        std::ifstream ifs;
        ifs.open(file10_.c_str(), std::ios::in | std::ios::binary);
        liblas::Reader reader(ifs);

        // read 1st point
        reader.ReadNextPoint();
        test_file10_point1(reader.GetPoint());

        // read 2nd point
        reader.ReadNextPoint();
        test_file10_point2(reader.GetPoint());

        // read and skip 3rd point
        reader.ReadNextPoint();

        // read 4th point
        reader.ReadNextPoint();
        test_file10_point4(reader.GetPoint());

        // read and count remaining points from 5 to 8
        unsigned short c = 0;
        while (reader.ReadNextPoint())
        {
            ++c;
        }
        ensure_equals(c, 4);
    }

    // Test ReadPointAt and GetPoint pair
    template<>
    template<>
    void to::test<5>()
    {
        std::ifstream ifs;
        ifs.open(file10_.c_str(), std::ios::in | std::ios::binary);
        liblas::Reader reader(ifs);

        // read 1st point
        reader.ReadPointAt(0);
        test_file10_point1(reader.GetPoint());

        // read 4th point
        reader.ReadPointAt(3);
        test_file10_point4(reader.GetPoint());

        // read back to 2nd point
        reader.ReadPointAt(1);
        test_file10_point2(reader.GetPoint());
    }

    // Test operator[] and GetPoint pair
    template<>
    template<>
    void to::test<6>()
    {
        std::ifstream ifs;
        ifs.open(file10_.c_str(), std::ios::in | std::ios::binary);
        liblas::Reader reader(ifs);

        // read 1st point
        test_file10_point1(reader[0]);

        // read 4th point
        test_file10_point4(reader[3]);

        // read back to 2nd point
        test_file10_point2(reader[1]);
    }

    // Test GetStream method
    template<>
    template<>
    void to::test<7>()
    {
        std::ifstream ifs;
        ifs.open(file10_.c_str(), std::ios::in | std::ios::binary);
        liblas::Reader reader(ifs);

        std::istream& is = reader.GetStream();

        ensure_equals(ifs, is);
    }


    // Test seek nd GetPoint pair
    template<>
    template<>
    void to::test<8>()
    {
        std::ifstream ifs;
        ifs.open(file10_.c_str(), std::ios::in | std::ios::binary);
        liblas::Reader reader(ifs);

        // read 1st point
	reader.ReadNextPoint();
        test_file10_point1(reader.GetPoint());
        
	// seek to 4th point
        reader.seek(4);

	// read 4th point
        test_file10_point4(reader[3]);

    }

    // Test GetStream method
}

