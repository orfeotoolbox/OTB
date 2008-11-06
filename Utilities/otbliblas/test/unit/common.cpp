// $Id: common.cpp 812 2008-07-25 21:52:56Z mloskot $
//
// (C) Copyright Mateusz Loskot 2008, mateusz@loskot.net
// Distributed under the BSD License
// (See accompanying file LICENSE.txt or copy at
// http://www.opensource.org/licenses/bsd-license.php)
//
#include <liblas/lasheader.hpp>
#include <liblas/cstdint.hpp>
#include <liblas/guid.hpp>
#include <tut/tut.hpp>
#include "common.hpp"

namespace tut
{

void test_default_header(liblas::LASHeader const& h)
{
    using liblas::LASHeader;

    ensure_equals("wrong default file signature",
        h.GetFileSignature(), LASHeader::FileSignature);

    ensure_equals("wrong default file source id",
        h.GetFileSourceId(), 0);
    ensure_equals("wrong default reserved value",
        h.GetReserved(), 0);

    liblas::guid g;
    ensure_equals("wrong default project guid",
        h.GetProjectId(), g);

    ensure_equals("wrong default major version",
        h.GetVersionMajor(), 1);
    ensure_equals("wrong default minor version",
        h.GetVersionMinor(), 1);

    ensure_equals("wrong default system id",
        h.GetSystemId(), LASHeader::SystemIdentifier);
    ensure_equals("wrong default software id",
        h.GetSoftwareId(), LASHeader::SoftwareIdentifier);

    // TODO: Fix me to use todays day # and year
    // ensure_equals("wrong default creation day-of-year",
    //     h.GetCreationDOY(), 0);
    // ensure_equals("wrong default creation year",
    //     h.GetCreationYear(), 0);
    ensure_equals("wrong default header size",
        h.GetHeaderSize(), liblas::uint16_t(227));
    
    liblas::uint32_t offset = 229;
    if (h.GetVersionMinor() == 1)
    {
        offset = 227;
    }
    ensure_equals("wrong default data offset",
        h.GetDataOffset(), offset);
    
    ensure_equals("wrong default records count",
        h.GetRecordsCount(), liblas::uint32_t(0));
    ensure_equals("wrong default data format id",
        h.GetDataFormatId(), LASHeader::ePointFormat0);
    ensure_equals("wrong default data record length",
        h.GetDataRecordLength(), LASHeader::ePointSize0);
    ensure_equals("wrong default point records count",
        h.GetPointRecordsCount(), liblas::uint32_t(0));

    ensure_equals("wrong default X scale", h.GetScaleX(), double(0.01));
    ensure_equals("wrong default Y scale", h.GetScaleY(), double(0.01));
    ensure_equals("wrong default Z scale", h.GetScaleZ(), double(0.01));

    ensure_equals("wrong default X offset", h.GetOffsetX(), double(0));
    ensure_equals("wrong default Y offset", h.GetOffsetY(), double(0));
    ensure_equals("wrong default Z offset", h.GetOffsetZ(), double(0));

    ensure_equals("wrong default min X", h.GetMinX(), double(0));
    ensure_equals("wrong default max X", h.GetMaxX(), double(0));
    ensure_equals("wrong default min Y", h.GetMinY(), double(0));
    ensure_equals("wrong default max Y", h.GetMaxY(), double(0));
    ensure_equals("wrong default min Z", h.GetMinZ(), double(0));
    ensure_equals("wrong default max Z", h.GetMaxZ(), double(0));
}

void test_default_point(liblas::LASPoint const& p)
{
    ensure_equals("wrong default X coordinate",
        p.GetX(), double(0));
    ensure_equals("wrong default Y coordinate",
        p.GetY(), double(0));
    ensure_equals("wrong default Z coordinate",
        p.GetZ(), double(0));
    ensure_equals("wrong defualt intensity",
        p.GetIntensity(), 0);
    ensure_equals("wrong defualt return number",
        p.GetReturnNumber(), 0);
    ensure_equals("wrong defualt number of returns",
        p.GetNumberOfReturns(), 0);
    ensure_equals("wrong defualt scan direction",
        p.GetScanDirection(), 0);
    ensure_equals("wrong defualt edge of flight line",
        p.GetFlightLineEdge(), 0);
    ensure_equals("wrong defualt classification",
        p.GetClassification(), 0);
    ensure_equals("wrong defualt scan angle rank",
        p.GetScanAngleRank(), 0);
    ensure_equals("wrong defualt user data value",
        p.GetUserData(), 0);
    ensure_equals("wrong defualt time",
        p.GetTime(), double(0));

    ensure("invalid defualt point record", p.IsValid());
}

void test_file10_header(liblas::LASHeader const& h)
{
    ensure_equals(h.GetFileSignature(), liblas::LASHeader::FileSignature);
    ensure_equals(h.GetFileSourceId(), 0);
    ensure_equals(h.GetReserved(), 0);

    liblas::guid g;
    ensure(g.is_null());
    ensure_equals(h.GetProjectId(), g);

    ensure_equals(h.GetVersionMajor(), 1);
    ensure_equals(h.GetVersionMinor(), 0);
    ensure_equals(h.GetSystemId(), std::string(""));
    ensure_equals(h.GetSoftwareId(), std::string("TerraScan"));
    ensure_equals(h.GetCreationDOY(), 0);
    ensure_equals(h.GetCreationYear(), 0);
    ensure_equals(h.GetHeaderSize(), liblas::uint16_t(227));
    ensure_equals(h.GetDataOffset(), liblas::uint32_t(229));
    ensure_equals(h.GetRecordsCount(), liblas::uint32_t(0));
    ensure_equals(h.GetDataFormatId(), liblas::LASHeader::ePointFormat1);
    ensure_equals(h.GetDataRecordLength(), liblas::LASHeader::ePointSize1);
    ensure_equals(h.GetPointRecordsCount(), liblas::uint32_t(8));
    ensure_equals(h.GetScaleX(), double(0.01));
    ensure_equals(h.GetScaleY(), double(0.01));
    ensure_equals(h.GetScaleZ(), double(0.01));
    ensure_equals(h.GetOffsetX(), double(-0));
    ensure_equals(h.GetOffsetY(), double(-0));
    ensure_equals(h.GetOffsetZ(), double(-0));
    ensure_equals(h.GetMinX(), double(630262.3));
    ensure_equals(h.GetMaxX(), double(630346.83));
    ensure_equals(h.GetMinY(), double(4834500));
    ensure_equals(h.GetMaxY(), double(4834500));
    ensure_equals(h.GetMinZ(), double(50.9));
    ensure_equals(h.GetMaxZ(), double(55.26));
}

}

