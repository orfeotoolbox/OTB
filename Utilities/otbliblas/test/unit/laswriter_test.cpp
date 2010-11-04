// $Id$
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
#include <liblas/lasclassification.hpp>
#include <liblas/liblas.hpp>
#include <tut/tut.hpp>
#include <cstdio>
#include <bitset>
#include <fstream>
#include <string>
#include "liblas_test.hpp"
#include "common.hpp"

namespace tut
{ 
    struct laswriter_data
    {
        std::string tmpfile_;
        std::string file10_;

        laswriter_data() :
            tmpfile_(g_test_data_path + "//tmp.las"),
            file10_(g_test_data_path + "//TO_core_last_clip.las")
        {}

        ~laswriter_data()
        {
            // remove temporary file after each test case
            int const ret = std::remove(tmpfile_.c_str());
            if (0 != ret)
            {
                ; // ignore, file may not exist
            }
        }
    };

    typedef test_group<laswriter_data> tg;
    typedef tg::object to;

    tg test_group_laswriter("liblas::Writer");

    // Test user-declared constructor
    template<>
    template<>
    void to::test<1>()
    {
        // Create new LAS file using default header block
        {
            std::ofstream ofs;
            ofs.open(tmpfile_.c_str(), std::ios::out | std::ios::binary);

            // LAS 1.2, Point Format 0
            liblas::Header header;
            liblas::Writer writer(ofs, header);

            ensure_equals(writer.GetHeader().GetVersionMinor(), 2);

            liblas::Header const& hdr_default = writer.GetHeader();
            test_default_header(hdr_default);
        }

        // Read previously created LAS file and check its header block
        {
            std::ifstream ifs;
            ifs.open(tmpfile_.c_str(), std::ios::in | std::ios::binary);
            ensure(ifs.is_open());

            liblas::Reader reader(ifs);

            ensure_equals(reader.GetHeader().GetVersionMinor(), 2);
            
            liblas::Header const& hdr_default = reader.GetHeader();
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
            liblas::Header header;
            liblas::Writer writer(ofs, header);

            liblas::Point point;

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
            point.SetPointSourceID(1);

            writer.WritePoint(point);

            // write 2nd point
            point.SetCoordinates(40, 50, 60);
            point.SetPointSourceID(2);
            writer.WritePoint(point);

            // write 3rd point
            point.SetCoordinates(70, 80, 90);
            point.SetPointSourceID(3);
            writer.WritePoint(point);
        }

        // Read previously create LAS file with 3 point records
        {
            std::ifstream ifs;
            ifs.open(tmpfile_.c_str(), std::ios::in | std::ios::binary);
            ensure(ifs.is_open());
            liblas::Reader reader(ifs);

            liblas::Point point; // reusable cache

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
            ensure_equals(point.GetScanAngleRank(), 90);
            ensure_equals(point.GetUserData(), 0);
            ensure_equals(point.GetPointSourceID(), 1);

            typedef liblas::Classification::bitset_type bitset_type;
            ensure_equals(bitset_type(point.GetClassification()), bitset_type(7));

            // read 2nd point
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
            ensure_equals(point.GetScanAngleRank(), 90);
            ensure_equals(point.GetUserData(), 0);
            ensure_equals(point.GetPointSourceID(), 2);

            // read 3rd point
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
            ensure_equals(point.GetScanAngleRank(), 90);
            ensure_equals(point.GetUserData(), 0);
            ensure_equals(point.GetPointSourceID(), 3);

            typedef liblas::Classification::bitset_type bitset_type;
            ensure_equals(bitset_type(point.GetClassification()), bitset_type(7));
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

            liblas::Header header;
            liblas::Writer writer(ofs, header);

            // test initially written header
            liblas::Header const& hdr_default = writer.GetHeader();
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
            liblas::Header const& written_header = writer.GetHeader();
            ensure_equals("written_header fileSource id", written_header.GetFileSourceId(), 65535);

        }

        // read and check updated header block
        {
            std::ifstream ifs;
            ifs.open(tmpfile_.c_str(), std::ios::in | std::ios::binary);
            ensure(ifs.is_open());
            liblas::Reader reader(ifs);

            liblas::Header const& header = reader.GetHeader();
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

        liblas::Header header;
        liblas::Writer writer(ofs, header);

        std::ostream& os = writer.GetStream();

        ensure_equals(ofs, os); // same streams
    }
}

