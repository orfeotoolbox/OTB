// $Id: lasspatialreference_test.cpp 1102 2009-03-16 14:57:12Z hobu $
//
// (C) Copyright Mateusz Loskot 2008, mateusz@loskot.net
// Distributed under the BSD License
// (See accompanying file LICENSE.txt or copy at
// http://www.opensource.org/licenses/bsd-license.php)
//
#include <liblas/liblas.hpp>
#include <tut/tut.hpp>
#include <string>
#include <stdexcept>
#include "common.hpp"
#include "liblas_test.hpp"

namespace tut
{ 
    struct lasspatialreference_data
    {
        liblas::SpatialReference m_default;
        std::string utm17_filename;
        std::string utm15_filename;

        lasspatialreference_data()
            : utm17_filename(g_test_data_path + "//srs.las")
            , utm15_filename(g_test_data_path + "//1.2_3.las")
        {}
    };

    typedef test_group<lasspatialreference_data> tg;
    typedef tg::object to;

    tg test_group_lasspatialreference("liblas::SpatialReference");


    // Test default constructor
    template<>
    template<>
    void to::test<1>()
    {
        
        ensure_equals(m_default.GetProj4(), "");
        ensure_equals(m_default.GetWKT(), "");
    }
    
#ifdef HAVE_GDAL

    // Test fetching SRS from an existing file
    template<>
    template<>
    void to::test<2>()
    {
        std::ifstream ifs;
        ifs.open(utm17_filename.c_str(), std::ios::in | std::ios::binary);
        liblas::Reader reader(ifs);
        
        liblas::Header const& header = reader.GetHeader();
        liblas::SpatialReference const& ref = header.GetSRS();
        
        const char* wkt_c = "PROJCS[\"WGS 84 / UTM zone 17N\",GEOGCS[\"WGS 84\",DATUM[\"WGS_1984\",SPHEROID[\"WGS 84\",6378137,298.257223563,AUTHORITY[\"EPSG\",\"7030\"]],AUTHORITY[\"EPSG\",\"6326\"]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.0174532925199433],AUTHORITY[\"EPSG\",\"4326\"]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-81],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",0],UNIT[\"metre\",1,AUTHORITY[\"EPSG\",\"9001\"]],AUTHORITY[\"EPSG\",\"32617\"]]";
        ensure_equals("WKT comparison", wkt_c, ref.GetWKT());
        
        const char* proj4_c = "+proj=utm +zone=17 +datum=WGS84 +units=m +no_defs ";
        ensure_equals("Proj.4 comparison", proj4_c, ref.GetProj4());

    }

    // Test round-tripping proj.4 string
    template<>
    template<>
    void to::test<3>()
    {
        liblas::SpatialReference ref;
        const char* proj4_c = "+proj=utm +zone=15 +datum=WGS84 +units=m +no_defs ";
        ref.SetProj4(proj4_c);
        
        ensure_equals("Proj.4 comparison", proj4_c, ref.GetProj4());
        
    }

    // Test setting EPSG:4326 from User string
    template<>
    template<>
    void to::test<4>()
    {
        liblas::SpatialReference ref;
        const char* code = "EPSG:4326";
        const char* proj4_c = "+proj=longlat +datum=WGS84 +no_defs ";
        const char* wkt_c = "GEOGCS[\"WGS 84\",DATUM[\"WGS_1984\",SPHEROID[\"WGS 84\",6378137,298.257223563,AUTHORITY[\"EPSG\",\"7030\"]],AUTHORITY[\"EPSG\",\"6326\"]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.0174532925199433],AUTHORITY[\"EPSG\",\"4326\"]]";
        ref.SetFromUserInput(code);
        
        ensure_equals("Proj.4 comparison", proj4_c, ref.GetProj4());
        ensure_equals("WKT comparison", wkt_c, ref.GetWKT());
        
    }

    // Test reprojecting UTM 15 to DD with a filter
    template<>
    template<>
    void to::test<5>()
    {
        std::ifstream ifs;
        ifs.open(utm15_filename.c_str(), std::ios::in | std::ios::binary);
        liblas::Reader reader(ifs);
        
        liblas::Header const& header = reader.GetHeader();
        liblas::SpatialReference const& in_ref = header.GetSRS();
        
        const char* utm15_wkt = "PROJCS[\"NAD83 / UTM zone 15N\",GEOGCS[\"NAD83\",DATUM[\"North_American_Datum_1983\",SPHEROID[\"GRS 1980\",6378137,298.2572221010002,AUTHORITY[\"EPSG\",\"7019\"]],AUTHORITY[\"EPSG\",\"6269\"]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.0174532925199433],AUTHORITY[\"EPSG\",\"4269\"]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-93],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",0],UNIT[\"metre\",1,AUTHORITY[\"EPSG\",\"9001\"]],AUTHORITY[\"EPSG\",\"26915\"]]";
        ensure_equals("Input WKT comparison", utm15_wkt, in_ref.GetWKT());

        const char* epsg4326_wkt = "GEOGCS[\"WGS 84\",DATUM[\"WGS_1984\",SPHEROID[\"WGS 84\",6378137,298.257223563,AUTHORITY[\"EPSG\",\"7030\"]],AUTHORITY[\"EPSG\",\"6326\"]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.0174532925199433],AUTHORITY[\"EPSG\",\"4326\"]]";
        
        liblas::SpatialReference out_ref;
        out_ref.SetWKT(epsg4326_wkt);
        ensure_equals("Output WKT comparison", epsg4326_wkt, out_ref.GetWKT());
        
        liblas::HeaderPtr out_hdr = liblas::HeaderPtr(new liblas::Header(header));
        out_hdr->SetScale(0.00000001, 0.00000001, 0.01);
        out_hdr->SetOffset(0,0,0);
        liblas::TransformPtr srs_transform = liblas::TransformPtr(new liblas::ReprojectionTransform(in_ref, out_ref, out_hdr));
        
        std::vector<liblas::TransformPtr> transforms;
        transforms.push_back(srs_transform);
        reader.ReadPointAt(0);

        liblas::Point unprojected_point = reader.GetPoint();
        
        ensure_distance("unprojected_point.GetX()", 
                        unprojected_point.GetX(), 
                        double(470692.44), 
                        0.01);

        ensure_distance("unprojected_point.GetY()", 
                        unprojected_point.GetY(), 
                        double(4602888.90), 
                        0.01);
                        
        reader.SetTransforms(transforms);

        // This should throw an out of range exception because the given scale/offset 
        // combination is not sufficient to store the data.
        try
        {
            reader.ReadPointAt(0);
            ensure("std::domain_error was not thrown", false);
        }
        catch (std::domain_error const& e)
        {
            ensure(e.what(), true);
        }
        

        out_hdr->SetScale(0.0000001, 0.0000001, 0.01);
        out_hdr->SetOffset(0,0,0);
        srs_transform = liblas::TransformPtr(new liblas::ReprojectionTransform(in_ref, out_ref, out_hdr));
        
        transforms.clear();
        transforms.push_back(srs_transform);
        reader.SetTransforms(transforms);
        
        reader.Reset();
        reader.ReadPointAt(0);

        
        liblas::Point const& projected_point = reader.GetPoint();

        ensure_distance("projected_point.GetX()", 
                        projected_point.GetX(), 
                        double(-93.35156259), 
                        0.0000001);
        ensure_distance("projected_point.GetY()", 
                        projected_point.GetY(), 
                        double(41.57714839), 
                        0.000001);
        
    }

    // Test VLR sizes from setting SRS
    template<>
    template<>
    void to::test<6>()
    {
        liblas::SpatialReference ref;
        const char* code = "EPSG:4326";
        ref.SetFromUserInput(code);
        
        std::vector<liblas::VariableRecord> const& vlrs = ref.GetVLRs();
        ensure_equals("VLR count", boost::uint32_t(3), vlrs.size());
        ensure_equals("First record size", boost::uint32_t(64), vlrs[0].GetRecordLength());
        
    }

#endif

}

