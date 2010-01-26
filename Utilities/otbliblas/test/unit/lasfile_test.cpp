// $Id: lasfile_test.cpp 646 2008-05-02 06:01:24Z mloskot $
//
// (C) Copyright Mateusz Loskot 2008, mateusz@loskot.net
// Distributed under the BSD License
// (See accompanying file LICENSE.txt or copy at
// http://www.opensource.org/licenses/bsd-license.php)
//
#include <liblas/lasfile.hpp>
#include <liblas/lasheader.hpp>
#include <liblas/lasreader.hpp>
#include <liblas/laswriter.hpp>
#include <tut/tut.hpp>
#include <stdexcept>
#include <string>
#include "liblas_test.hpp"
#include "common.hpp"

namespace tut
{ 
    struct lasfile_data
    {
        std::string tmpfile_;
        std::string file10_;

        lasfile_data()
            : tmpfile_(g_test_data_path + "//lasfile_tmp.las"),
                file10_(g_test_data_path + "//TO_core_last_clip.las")
        {}

        ~lasfile_data()
        {
            // remove temporary file after each test case
            std::remove(tmpfile_.c_str());
        }
    };

    typedef test_group<lasfile_data> tg;
    typedef tg::object to;

    tg test_group_lasfile("liblas::LASFile");

    // Test default constructor
    template<>
    template<>
    void to::test<1>()
    {
        liblas::LASFile file;
        ensure(file.IsNull());
    }

    // Test custom constructor
    template<>
    template<>
    void to::test<2>()
    {
        liblas::LASFile file(file10_);
        ensure_not(file.IsNull());

        ensure_equals(file.GetName(), file10_);
        ensure_equals(file.GetMode(), liblas::LASFile::eRead);

        liblas::LASHeader const& header1 = file.GetHeader();
        test_file10_header(header1);

        liblas::LASReader& reader = file.GetReader();
        liblas::LASHeader const& header2 = reader.GetHeader();
        test_file10_header(header2);

        try
        {
            liblas::LASWriter& writer = file.GetWriter();
            ensure_equals(writer.GetVersion(), liblas::eLASVersion10);
            ensure("std::runtime_error not thrown", false);
        }
        catch (std::runtime_error const& e)
        {
            ensure_equals(std::string(e.what()).substr(0, 6), std::string("Writer"));
        }
    }

    // Test custom constructor
    template<>
    template<>
    void to::test<3>()
    {
        liblas::LASHeader header;

        try
        {
            liblas::LASFile file(tmpfile_, header, liblas::LASFile::eRead);
            ensure_equals(file.GetName(), tmpfile_);
            ensure("std::runtime_error not thrown", false);
        }
        catch (std::runtime_error const& e)
        {
            ensure_equals(std::string(e.what()),
                std::string("File mode must be eWrite or eAppend"));
        }
    }

    // Test custom constructor
    template<>
    template<>
    void to::test<4>()
    {
        liblas::LASHeader header;
        liblas::LASFile file(tmpfile_, header, liblas::LASFile::eWrite);
        ensure_not(file.IsNull());
        ensure_equals(file.GetName(), tmpfile_);
        ensure_equals(file.GetMode(), liblas::LASFile::eWrite);

        liblas::LASHeader const& header1 = file.GetHeader();
        test_default_header(header1);

        liblas::LASWriter& writer = file.GetWriter();
        liblas::LASHeader const& header2 = writer.GetHeader();
        test_default_header(header2);

        try
        {
            liblas::LASReader& reader = file.GetReader();
            ensure_equals(reader.GetVersion(), liblas::eLASVersion10);
            ensure("std::runtime_error not thrown", false);
        }
        catch (std::runtime_error const& e)
        {
            ensure_equals(std::string(e.what()).substr(0, 6), std::string("Reader"));
        }
    }

    // Test copy constructor on default initialized object
    template<>
    template<>
    void to::test<5>()
    {
        liblas::LASFile file;
        ensure(file.IsNull());

        liblas::LASFile file2(file);
        ensure(file2.IsNull());
    }

    // Test assignment operator
    template<>
    template<>
    void to::test<6>()
    {
        liblas::LASFile file;
        ensure(file.IsNull());

        liblas::LASFile file2;
        file2 = file;
        ensure(file2.IsNull());
    }

    // Test copy constructor on read-only file
    template<>
    template<>
    void to::test<7>()
    {
        liblas::LASFile file(file10_);

        ensure_not(file.IsNull());
        ensure_equals(file.GetName(), file10_);
        ensure_equals(file.GetMode(), liblas::LASFile::eRead);

        liblas::LASHeader const& header1 = file.GetHeader();
        test_file10_header(header1);

        liblas::LASFile file2(file);

        ensure_not(file2.IsNull());
        ensure_equals(file2.GetName(), file10_);
        ensure_equals(file2.GetMode(), liblas::LASFile::eRead);

        liblas::LASHeader const& header2 = file2.GetHeader();
        test_file10_header(header2);
    }

    // Test assignment operator on read-only file
    template<>
    template<>
    void to::test<8>()
    {
        liblas::LASFile file2;

        {
            liblas::LASFile file(file10_);

            ensure_not(file.IsNull());
            ensure_equals(file.GetName(), file10_);
            ensure_equals(file.GetMode(), liblas::LASFile::eRead);

            liblas::LASHeader const& header1 = file.GetHeader();
            test_file10_header(header1);

            file2 = file;

            // file object is deallocated here
        }

        ensure_not(file2.IsNull());
        ensure_equals(file2.GetName(), file10_);
        ensure_equals(file2.GetMode(), liblas::LASFile::eRead);

        liblas::LASHeader const& header1 = file2.GetHeader();
        test_file10_header(header1);
    }

    // Test assignment operator on write-only file
    template<>
    template<>
    void to::test<9>()
    {
        liblas::LASHeader header;
        liblas::LASFile file(tmpfile_, header, liblas::LASFile::eWrite);

        ensure_not(file.IsNull());
        ensure_equals(file.GetName(), tmpfile_);
        ensure_equals(file.GetMode(), liblas::LASFile::eWrite);

        liblas::LASHeader const& header1 = file.GetHeader();
        test_default_header(header1);

        liblas::LASFile file2(file);

        ensure_not(file2.IsNull());
        ensure_equals(file2.GetName(), tmpfile_);
        ensure_equals(file2.GetMode(), liblas::LASFile::eWrite);

        liblas::LASHeader const& header2 = file2.GetHeader();
        test_default_header(header2);
    }

    // Test assignment operator on write-only file
    template<>
    template<>
    void to::test<10>()
    {
        liblas::LASFile file2;

        {
            liblas::LASHeader header;
            liblas::LASFile file(tmpfile_, header, liblas::LASFile::eWrite);

            ensure_not(file.IsNull());
            ensure_equals(file.GetName(), tmpfile_);
            ensure_equals(file.GetMode(), liblas::LASFile::eWrite);

            liblas::LASHeader const& header1 = file.GetHeader();
            test_default_header(header1);

            file2 = file;

            // file object is deallocated here
        }

        ensure_not(file2.IsNull());
        ensure_equals(file2.GetName(), tmpfile_);
        ensure_equals(file2.GetMode(), liblas::LASFile::eWrite);

        liblas::LASHeader const& header1 = file2.GetHeader();
        test_default_header(header1);
    }
}

