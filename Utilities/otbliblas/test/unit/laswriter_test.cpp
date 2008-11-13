// $Id: laswriter_test.cpp 812 2008-07-25 21:52:56Z mloskot $
//
// (C) Copyright Mateusz Loskot 2008, mateusz@loskot.net
// Distributed under the BSD License
// (See accompanying file LICENSE.txt or copy at
// http://www.opensource.org/licenses/bsd-license.php)
//
#include <liblas/laswriter.hpp>
#include <liblas/lasreader.hpp>
#include <liblas/lasheader.hpp>
#include <liblas/laspoint.hpp>
#include <liblas/cstdint.hpp>
#include <liblas/liblas.hpp>
#include <tut/tut.hpp>
#include <fstream>
#include <string>
#include <cstdio>
#include "liblas_test.hpp"
#include "common.hpp"

namespace tut
{ 
    struct laswriter_data
    {
        std::string tmpfile_;
        std::string file10_;

        laswriter_data()
            : tmpfile_(g_test_data_path + "//tmp.las"),
                file10_(g_test_data_path + "//TO_core_last_clip.las")
        {}

        ~laswriter_data()
        {
            // remove temporary file after each test case
            int const rc = std::remove(tmpfile_.c_str());
            ensure_equals(rc, 0);
        }

        void test_file10_point1(liblas::LASPoint const& p)
        {
            ensure_distance(p.GetX(), double(630262.30), 0.0001);
            ensure_distance(p.GetY(), double(4834500), 0.0001);
            ensure_distance(p.GetZ(), double(51.53), 0.0001);
            ensure_equals(p.GetIntensity(), 670);
            ensure_equals(p.GetClassification(), 1);
            ensure_equals(p.GetScanAngleRank(), 0);
            ensure_equals(p.GetUserData(), 0);
            ensure_equals(p.GetScanFlags(), 9);
            ensure_distance(p.GetTime(), double(413665.23360000004), 0.0001);
        }

        void test_file10_point2(liblas::LASPoint const& p)
        {
            ensure_distance(p.GetX(), double(630282.45), 0.0001);
            ensure_distance(p.GetY(), double(4834500), 0.0001);
            ensure_distance(p.GetZ(), double(51.63), 0.0001);
            ensure_equals(p.GetIntensity(), 350);
            ensure_equals(p.GetClassification(), 1);
            ensure_equals(p.GetScanAngleRank(), 0);
            ensure_equals(p.GetUserData(), 0);
            ensure_equals(p.GetScanFlags(), 9);
            ensure_distance(p.GetTime(), double(413665.52880000003), 0.0001);
        }

        void test_file10_point4(liblas::LASPoint const& p)
        {
            ensure_distance(p.GetX(), double(630346.83), 0.0001);
            ensure_distance(p.GetY(), double(4834500), 0.0001);
            ensure_distance(p.GetZ(), double(50.90), 0.0001);
            ensure_equals(p.GetIntensity(), 150);
            ensure_equals(p.GetClassification(), 1);
            ensure_equals(p.GetScanAngleRank(), 0);
            ensure_equals(p.GetUserData(), 0);
            ensure_equals(p.GetScanFlags(), 18);
            ensure_distance(p.GetTime(), double(414093.84360000002), 0.0001);
        }
    };

    typedef test_group<laswriter_data> tg;
    typedef tg::object to;

    tg test_group_laswriter("liblas::LASWriter");

    // Test user-declared constructor
    template<>
    template<>
    void to::test<1>()
    {
        // Create new LAS file using default header block
        {
            std::ofstream ofs;
            ofs.open(tmpfile_.c_str(), std::ios::out | std::ios::binary);

            // LAS 1.1, Point Format 0
            liblas::LASHeader header;
            liblas::LASWriter writer(ofs, header);

            ensure_equals<std::size_t>(writer.GetVersion(), liblas::eLASVersion11);

            liblas::LASHeader const& hdr_default = writer.GetHeader();
            test_default_header(hdr_default);
        }

        // Read previously created LAS file and check its header block
        {
            std::ifstream ifs;
            ifs.open(tmpfile_.c_str(), std::ios::in | std::ios::binary);
            liblas::LASReader reader(ifs);

            ensure_equals<std::size_t>(reader.GetVersion(), liblas::eLASVersion11);
            
            liblas::LASHeader const& hdr_default = reader.GetHeader();
            test_default_header(hdr_default);
        }
    }

    // Test WritePoint method
    template<>
    template<>
    void to::test<2>()
    {
        {
            std::ofstream ofs;
            ofs.open(tmpfile_.c_str(), std::ios::out | std::ios::binary);

            // LAS 1.1, Point Format 0
            liblas::LASHeader header;
            liblas::LASWriter writer(ofs, header);

            liblas::LASPoint point;

            // Write 1st point
            point.SetCoordinates(10, 20, 30);
            point.SetIntensity(5);
            point.SetReturnNumber(1);
            point.SetNumberOfReturns(1);
            point.SetScanDirection(1);
            point.SetFlightLineEdge(1);
            point.SetClassification(7);
            point.SetScanAngleRank(90);
            point.SetUserData(0);

            writer.WritePoint(point);

            // write 2nd point
            point.SetCoordinates(40, 50, 60);
            writer.WritePoint(point);

            // write 3rd point
            point.SetCoordinates(70, 80, 90);
            writer.WritePoint(point);
        }

        // Read previously create LAS file with 3 point records
        {
            std::ifstream ifs;
            ifs.open(tmpfile_.c_str(), std::ios::in | std::ios::binary);
            liblas::LASReader reader(ifs);

            liblas::LASPoint point; // reusable cache

            // read 1st point
            reader.ReadNextPoint();
            point = reader.GetPoint();
            ensure_distance(point.GetX(), 10.0, 0.1);
            ensure_distance(point.GetY(), 20.0, 0.1);
            ensure_distance(point.GetZ(), 30.0, 0.1);
            ensure_equals(point.GetIntensity(), 5);
            ensure_equals(point.GetReturnNumber(), 1);
            ensure_equals(point.GetNumberOfReturns(), 1);
            ensure_equals(point.GetScanDirection(), 1);
            ensure_equals(point.GetFlightLineEdge(), 1);
            ensure_equals(point.GetClassification(), 7);
            ensure_equals(point.GetScanAngleRank(), 90);
            ensure_equals(point.GetUserData(), 0);

            // read 3rd point
            reader.ReadNextPoint();
            point = reader.GetPoint();
            ensure_distance(point.GetX(), 40.0, 0.1);
            ensure_distance(point.GetY(), 50.0, 0.1);
            ensure_distance(point.GetZ(), 60.0, 0.1);
            ensure_equals(point.GetIntensity(), 5);
            ensure_equals(point.GetReturnNumber(), 1);
            ensure_equals(point.GetNumberOfReturns(), 1);
            ensure_equals(point.GetScanDirection(), 1);
            ensure_equals(point.GetFlightLineEdge(), 1);
            ensure_equals(point.GetClassification(), 7);
            ensure_equals(point.GetScanAngleRank(), 90);
            ensure_equals(point.GetUserData(), 0);

            // read 1st point
            reader.ReadNextPoint();
            point = reader.GetPoint();
            ensure_distance(point.GetX(), 70.0, 0.1);
            ensure_distance(point.GetY(), 80.0, 0.1);
            ensure_distance(point.GetZ(), 90.0, 0.1);
            ensure_equals(point.GetIntensity(), 5);
            ensure_equals(point.GetReturnNumber(), 1);
            ensure_equals(point.GetNumberOfReturns(), 1);
            ensure_equals(point.GetScanDirection(), 1);
            ensure_equals(point.GetFlightLineEdge(), 1);
            ensure_equals(point.GetClassification(), 7);
            ensure_equals(point.GetScanAngleRank(), 90);
            ensure_equals(point.GetUserData(), 0);
        }
    }

    
    // Test WriteHeader method
    template<>
    template<>
    void to::test<3>()
    {
        {
            std::ofstream ofs;
            ofs.open(tmpfile_.c_str(), std::ios::out | std::ios::binary);

            liblas::LASHeader header;
            liblas::LASWriter writer(ofs, header);

            // test initially written header
            liblas::LASHeader const& hdr_default = writer.GetHeader();
            test_default_header(hdr_default);

            // update some header data and overwrite header block
            header.SetFileSourceId(65535);
            header.SetSystemId("Unit Test libLAS System");
            header.SetSoftwareId("Unit Test libLAS Software");
            header.SetCreationDOY(100);
            header.SetCreationYear(2008);
            header.SetScale(1.123, 2.123, 3.123);
            header.SetOffset(4.321, 5.321, 6.321);

            writer.WriteHeader(header);
        }

        // read and check updated header block
        {
            std::ifstream ifs;
            ifs.open(tmpfile_.c_str(), std::ios::in | std::ios::binary);
            liblas::LASReader reader(ifs);

            liblas::LASHeader const& header = reader.GetHeader();
            ensure_equals(header.GetFileSourceId(), 65535);
            ensure_equals(header.GetSystemId(), std::string("Unit Test libLAS System"));
            ensure_equals(header.GetSoftwareId(), std::string("Unit Test libLAS Software"));
            ensure_equals(header.GetCreationDOY(), 100);
            ensure_equals(header.GetCreationYear(), 2008);
            ensure_equals(header.GetScaleX(), 1.123);
            ensure_equals(header.GetScaleY(), 2.123);
            ensure_equals(header.GetScaleZ(), 3.123);
            ensure_equals(header.GetOffsetX(), 4.321);
            ensure_equals(header.GetOffsetY(), 5.321);
            ensure_equals(header.GetOffsetZ(), 6.321);
        }
    }

    // Test GetStream method
    template<>
    template<>
    void to::test<4>()
    {
        std::ofstream ofs;
        ofs.open(tmpfile_.c_str(), std::ios::out | std::ios::binary);

        liblas::LASHeader header;
        liblas::LASWriter writer(ofs, header);

        std::ostream& os = writer.GetStream();

        ensure_equals(ofs, os); // same streams
    }
}

