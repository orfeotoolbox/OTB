// $Id$
//
// (C) Copyright Mateusz Loskot 2008, mateusz@loskot.net
// Distributed under the BSD License
// (See accompanying file LICENSE.txt or copy at
// http://www.opensource.org/licenses/bsd-license.php)
//
#include <liblas/cstdint.hpp>
#include <liblas/iterator.hpp>
#include <liblas/laspoint.hpp>
#include <liblas/lasreader.hpp>
#include <liblas/detail/utility.hpp>
#include <tut/tut.hpp>
#include <algorithm>
#include <fstream>
#include <iterator>
#include <list>
#include "liblas_test.hpp"
#include "common.hpp"
using namespace liblas;

namespace tut
{ 
    struct lasreader_iterator_data
    {
        std::string file10_;
        std::ifstream ifs_;
        LASReader reader_;

        lasreader_iterator_data() :
            file10_(g_test_data_path + "//TO_core_last_clip.las"),
            ifs_(file10_.c_str(), std::ios::in | std::ios::binary),
            reader_(ifs_)
        {}
    };

    typedef test_group<lasreader_iterator_data> tg;
    typedef tg::object to;

    tg test_group_lasreader_iterator("liblas::lasreader_iterator");

    // Test default constructor
    template<>
    template<>
    void to::test<1>()
    {
        lasreader_iterator it;
    }

    // Test user-defined constructor
    template<>
    template<>
    void to::test<2>()
    {
        lasreader_iterator it(reader_);
    }

    // Test copy constructor with default initialized iterator
    template<>
    template<>
    void to::test<3>()
    {
        lasreader_iterator it1;
        lasreader_iterator it2(it1);

        ensure(it1 == it2);
    }

    // Test copy constructor with initialized iterator
    template<>
    template<>
    void to::test<4>()
    {
        lasreader_iterator it1(reader_);
        lasreader_iterator it2(it1);

        ensure(it1 == it2);
    }

    // Test assignment operator with default initialized iterator
    template<>
    template<>
    void to::test<5>()
    {
        lasreader_iterator it1;
        lasreader_iterator it2;
        it1 = it2;

        ensure(it1 == it2);
    }

    // Test assignment operator with initialized iterator
    template<>
    template<>
    void to::test<6>()
    {
        lasreader_iterator it1(reader_);
        lasreader_iterator it2;
        it1 = it2;

        ensure(it1 == it2);
    }

    // Test dereference operator
    template<>
    template<>
    void to::test<7>()
    {
        lasreader_iterator it(reader_);

        test_file10_point1(*it);
    }

    // Test pointer-to-member operator
    template<>
    template<>
    void to::test<8>()
    {
        lasreader_iterator it(reader_);

        // test 1st point data record 
        ensure_distance(it->GetX(), double(630262.30), 0.0001);
        ensure_distance(it->GetY(), double(4834500), 0.0001);
        ensure_distance(it->GetZ(), double(51.53), 0.0001);
        ensure_equals(it->GetIntensity(), 670);
        ensure_equals(it->GetClassification(), liblas::uint8_t(1));
        ensure_equals(it->GetScanAngleRank(), 0);
        ensure_equals(it->GetUserData(), 3);
        ensure_equals(it->GetScanFlags(), 9);
        ensure_distance(it->GetTime(), double(413665.23360000004), 0.0001);
    }

    // Test pre-increment operator
    template<>
    template<>
    void to::test<9>()
    {
        lasreader_iterator it(reader_); // move to 1st point
        ++it; // move to 2nd record

        test_file10_point2(*it);
    }

    // Test post-increment operator
    template<>
    template<>
    void to::test<10>()
    {
        lasreader_iterator it(reader_); // move to 1st point
        it++; // move to 2nd record

        test_file10_point2(*it);
    }

    // Test equal-to operator
    template<>
    template<>
    void to::test<11>()
    {
        lasreader_iterator it(reader_); // move to 1st point
        lasreader_iterator end;

        ensure_not(end == it);
    }

    // Test not-equal-to operator
    template<>
    template<>
    void to::test<12>()
    {
        lasreader_iterator it(reader_); // move to 1st point
        lasreader_iterator end;

        ensure(end != it);
    }

    // Test iteration
    template<>
    template<>
    void to::test<13>()
    {
        liblas::uint32_t const cnt = reader_.GetHeader().GetPointRecordsCount();
        lasreader_iterator it(reader_); // move to 1st point
        lasreader_iterator end;

        liblas::uint32_t s = 0;
        while (end != it)
        {
            s++;
            ++it;
        }

        ensure_equals(cnt, s);
    }

    // Test std::distance operation
    template<>
    template<>
    void to::test<14>()
    {
        liblas::uint32_t const cnt = reader_.GetHeader().GetPointRecordsCount();
        lasreader_iterator it(reader_); // move to 1st point
        lasreader_iterator end;

        lasreader_iterator::difference_type const d = std::distance(it, end);
        ensure_equals(d, cnt);
    }

    // Test std::distance operation
    template<>
    template<>
    void to::test<15>()
    {
        std::size_t a = std::distance(lasreader_iterator(reader_), lasreader_iterator());

        // Reader state is set to "past-the-end-of-file"
        // So, reset is needed
        reader_.Reset();

        std::size_t b = std::distance(lasreader_iterator(reader_), lasreader_iterator());

        ensure_equals(a, b);
    }

    // Test std::advance operation
    template<>
    template<>
    void to::test<16>()
    {
        lasreader_iterator it(reader_); // move to 1st point

        std::advance(it, 1); // move to 2nd record
        test_file10_point2(*it);

        std::advance(it, 2); // move to 4th record
        test_file10_point4(*it);
    }

    // Test std::copy algorithm
    template<>
    template<>
    void to::test<17>()
    {
        liblas::uint32_t const size = reader_.GetHeader().GetPointRecordsCount();
        lasreader_iterator it(reader_);
        lasreader_iterator end;

        typedef std::list<LASPoint> list_t;
        typedef std::back_insert_iterator<list_t> inserter_t;
        list_t cache;

        // Test copying LAS records to std::list based cache
        std::copy(it, end, inserter_t(cache));
        ensure_equals(cache.size(), size);

        // Test copied data
        list_t::const_iterator cit = cache.begin(); // 1st element
        std::advance(cit, 1); // move to 2nd element in cache
        test_file10_point2(*cit);
        std::advance(cit, 2); // move to 4th element in cache
        test_file10_point4(*cit);
    }

    // Test std::count algorithm
    template<>
    template<>
    void to::test<18>()
    {
        // Construct copy of 2nd point record from tested file
        LASPoint pt;
        pt.SetCoordinates(630282.45, 4834500, 51.63);
        pt.SetIntensity(350);
        pt.SetClassification(1);
        pt.SetScanAngleRank(0);
        pt.SetUserData(3);
        pt.SetScanFlags(9);
        pt.SetTime(413665.52880000003);
        ensure(pt.IsValid());
        test_file10_point2(pt);

        lasreader_iterator it(reader_);
        lasreader_iterator end;

        // Count records equal to given point object
        lasreader_iterator::difference_type const expected = 1;
        lasreader_iterator::difference_type n = std::count(it, end, pt);
        ensure_equals(n, expected);
    }

    // Test std::equal algorithm
    template<>
    template<>
    void to::test<19>()
    {
        std::ifstream ifs(file10_.c_str(), std::ios::in | std::ios::binary);
        LASReader reader(ifs);

        // Copy LAS records to std::list based cache
        typedef std::list<LASPoint> list_t;
        typedef std::back_insert_iterator<list_t> inserter_t;
        list_t cache;
        {
            lasreader_iterator it(reader);
            lasreader_iterator end;
            ensure(it != end);

            std::copy(it, end, inserter_t(cache));
            ensure_equals(cache.size(), reader.GetHeader().GetPointRecordsCount());
        }

        // Reset reader to the beginning of LAS file
        reader.Reset();

        // Compare LAS file with cache
        {
            lasreader_iterator it(reader);
            lasreader_iterator end;
            ensure(it != end);

            bool eq = std::equal(it, end, cache.begin());
            ensure(eq);
        }
    }

    // Test std::find algorithm
    template<>
    template<>
    void to::test<20>()
    {
        // Construct copy of 2nd point record from tested file
        LASPoint pt;
        pt.SetCoordinates(630282.45, 4834500, 51.63);
        pt.SetIntensity(350);
        pt.SetClassification(1);
        pt.SetScanAngleRank(0);
        pt.SetUserData(3);
        pt.SetScanFlags(9);
        pt.SetTime(413665.52880000003);
        ensure(pt.IsValid());
        test_file10_point2(pt);

        lasreader_iterator it(reader_);
        lasreader_iterator end;

        // find 2nd point data record
        lasreader_iterator fit;
        fit = std::find(it, end, pt);
        ensure(fit != end);
        test_file10_point2(*fit);
    }

    // Test std::find_if algorithm
    template<>
    template<>
    void to::test<21>()
    {
        lasreader_iterator it(reader_);
        lasreader_iterator end;

        // find 2nd point data record comparing XY coordinates
        lasreader_iterator fit;
        fit = std::find_if(it, end, is_xy(630282.45, 4834500, 0.0001));
        ensure(fit != end);
        test_file10_point2(*fit);
    }

    // Test std::for_each algorithm
    template<>
    template<>
    void to::test<22>()
    {
        lasreader_iterator it(reader_);
        lasreader_iterator end;

        typedef liblas::detail::Point<double> point_t;
        typedef liblas::detail::Extents<double> bbox_t;

        LASHeader const& h = reader_.GetHeader();
        bbox_t lasbbox(point_t(h.GetMinX(), h.GetMinY(), h.GetMinZ()),
                       point_t(h.GetMaxX(), h.GetMaxY(), h.GetMaxZ()));

        // Accumulate points extents to common bounding box
        bbox_t bbox;
        std::for_each(it, end, bbox_calculator(bbox));

        ensure(lasbbox == bbox);
    }
}
