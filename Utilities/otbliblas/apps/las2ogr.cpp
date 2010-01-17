// $Id: las2ogr.cpp 884 2008-09-26 18:04:27Z mloskot $
//
// las2ogr translates LAS file to OGR datasource.
// Inspired by pylas.py script developed by Matthew Perry and Carl Anderson,
// available at http://code.google.com/p/pylas/
//
// (C) Copyright Mateusz Loskot 2008, mateusz@loskot.net
// Distributed under the BSD License
// (See accompanying file LICENSE.txt or copy at
// http://www.opensource.org/licenses/bsd-license.php)
//
#ifdef HAVE_GDAL
#if defined(_MSC_VER) && defined(USE_VLD)
#include <vld.h>
#endif
// liblas
#include <liblas/liblas.hpp>
#include <liblas/laspoint.hpp>
#include <liblas/lasreader.hpp>
#include <liblas/cstdint.hpp>
// ogr
#include <ogr_api.h>
//std
#include <algorithm>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <cassert>

// Anonymous namespace for local definitions
namespace { 

template <typename T>
class ogr_wrapper
{
    typedef void(*deleter_type)(T p);

public:

    ogr_wrapper(T p, deleter_type d)
        : p_(p), del_(d)
    {}

    ~ogr_wrapper()
    {
        do_delete(p_);
    }

    void reset(T p)
    {
        do_delete(p_);
        p_= p;
    }

    operator T()
    {
        return get();
    }
    
    
    T get() const
    {
        return p_;
    }

    void swap(ogr_wrapper& other)
    {
        std::swap(p_, other.p_);
    }

private:

    ogr_wrapper(ogr_wrapper const& other);
    ogr_wrapper& operator=(ogr_wrapper const& rhs);

    void do_delete(T p)
    {
        assert(del_);
        if (0 != p)
            del_(p);
    }

    T p_;
    deleter_type del_;

};

bool term_progress(std::ostream& os, double complete)
{
    static int lastTick = -1;
    int tick = static_cast<int>(complete * 40.0);

    tick = std::min(40, std::max(0, tick));

    // Have we started a new progress run?  
    if (tick < lastTick && lastTick >= 39)
        lastTick = -1;

    if (tick <= lastTick)
        return true;

    while (tick > lastTick)
    {
        lastTick++;
        if (lastTick % 4 == 0)
            os << (lastTick / 4) * 10;
        else
            os << ".";
    }

    if( tick == 40 )
        os << " - done.\n";
    else
        os.flush();

    return true;
}

OGRFieldDefnH create_field(const char* name, OGRFieldType type, int width, int precision)
{
    OGRFieldDefnH fld;
    fld= OGR_Fld_Create(name, type);
    OGR_Fld_SetWidth(fld, width);
    OGR_Fld_SetPrecision(fld, precision);

    return fld;
}

void create_layer_def(OGRLayerH lyr)
{
    assert(0 != lyr);

    std::string fldname("return_num");

    OGRFieldDefnH fld = create_field("return_num", OFTInteger, 10, 0);
    OGRErr err = OGR_L_CreateField(lyr, fld, 0);
    if (OGRERR_NONE != err)
    {
        throw std::runtime_error("return_num field creation failed");
    }
    
    OGR_Fld_Destroy(fld);

    fld = create_field("angle", OFTInteger, 10, 0);
    err = OGR_L_CreateField(lyr, fld, 0);
    if (OGRERR_NONE != err)
    {
        throw std::runtime_error("angle field cration failed");
    }
    
    OGR_Fld_Destroy(fld);
    
    fld = create_field("intensity", OFTInteger, 10, 0);
    err = OGR_L_CreateField(lyr, fld, 0);
    if (OGRERR_NONE != err)
    {
        throw std::runtime_error("intensity field cration failed");
    }
    
    OGR_Fld_Destroy(fld);

    fld = create_field("asprsclass", OFTInteger, 10, 0);
    err = OGR_L_CreateField(lyr, fld, 0);
    if (OGRERR_NONE != err)
    {
        throw std::runtime_error("asprsclass field creation failed");
    }
    
    OGR_Fld_Destroy(fld);

    fld = create_field("return_tot", OFTInteger, 10, 0);
    err = OGR_L_CreateField(lyr, fld, 0);
    if (OGRERR_NONE != err)
    {
        throw std::runtime_error("return_tot field creation failed");
    }
    
    OGR_Fld_Destroy(fld);

    fld = create_field("gpstime", OFTReal, 10, 0);
    err = OGR_L_CreateField(lyr, fld, 0);
    if (OGRERR_NONE != err)
    {
        throw std::runtime_error("gpstime field cration failed");
    }
    
    OGR_Fld_Destroy(fld);
}

void report_ogr_formats(std::ostream& os)
{
    os << "Supported OGR formats:";

    for (int i = 0; i < OGRGetDriverCount(); ++i)
    {
        OGRSFDriverH drv = OGRGetDriver(i);
        assert(0 != drv);

        if (OGR_Dr_TestCapability(drv, ODrCCreateDataSource))
        {
            os << "\n - " << OGR_Dr_GetName(drv);
        }
    }

    os << "\nMore details at http://gdal.org/ogr/ogr_formats.html" << std::endl;
}

void usage()
{
    std::cerr << "Usage: las2ogr OPTIONS\nOptions are:\n"
        << "\t-h print this message\n"
        << "\t-i <infile>\tinput ASPRS LAS file\n"
        << "\t-o <outfile>\toutput file\n"
        << "\t-f <format>\tOGR format for output file\n"
        << "\t-formats\tlist supported OGR formats\n";       
}

} // anonymous namespace

int main(int argc, char* argv[])
{
    int rc = 0;

    try
    {
        OGRRegisterAll();

        // Parse command-line options
        std::string in_file;
        std::string out_file;
        std::string out_frmt;
        {
            int on_arg = 1;
            while (on_arg < argc)
            {
                std::string arg(argv[on_arg]);
                if (arg == "-h")
                {
                    usage();
                    return 0;
                }
                else if (arg == "-formats")
                {
                    report_ogr_formats(std::cout);
                    return 0;
                }
                else if (arg == "-i" && (on_arg + 1 < argc))
                {   
                    ++on_arg;
                    assert(on_arg < argc);
                    in_file = argv[on_arg];
                }
                else if (arg == "-o" && (on_arg + 1 < argc))
                {
                    ++on_arg;
                    assert(on_arg < argc);
                    out_file = argv[on_arg];
                    out_frmt = "ESRI Shapefile"; // default output format
                }
                else if (arg == "-f" && (on_arg + 1 < argc))
                {
                    ++on_arg;
                    assert(on_arg < argc);
                    out_frmt = argv[on_arg];
                }
                else
                {
                    throw std::runtime_error(std::string("unrecognized parameter: ") + arg);
                }
                ++on_arg;
            }

            if (in_file.empty() || out_file.empty() || out_frmt.empty())
            {
                throw std::runtime_error("missing input paremeters");
            }
        }

        //
        // Source
        //
        std::cout << "Source:" << "\n - dataset: " << in_file << std::endl;

        std::ifstream ifs;
        if (!liblas::Open(ifs, in_file.c_str()))
        {
            throw std::runtime_error(std::string("Can not open \'") + in_file + "\'");
        }
        liblas::LASReader reader(ifs);

        //
        // Target
        //
        std::string const lyrname(out_file.substr(0, out_file.find_last_of('.')));

        std::cout << "Target:" 
            << "\n - format: " << out_frmt
            << "\n - dataset: " << out_file
            << "\n - layer: " << lyrname
            << std::endl;

        OGRSFDriverH drv = OGRGetDriverByName(out_frmt.c_str());
        if (0 == drv)
        {
            throw std::runtime_error(out_frmt + " driver not available");
        }

        ogr_wrapper<OGRDataSourceH> ds(OGR_Dr_CreateDataSource(drv, out_file.c_str(), 0), OGR_DS_Destroy);
        if (0 == ds.get())
        {
            throw std::runtime_error(out_file + " datasource creation failed");
        }

        OGRLayerH lyr = OGR_DS_CreateLayer(ds, lyrname.c_str(), 0, wkbPoint25D, 0);
        if (0 == lyr)
        {
            throw std::runtime_error(out_file + " layer creation failed");
        }

        // Prepare layer schema
        create_layer_def(lyr);

        //
        // Translation of points cloud to features set
        //
        liblas::uint32_t i = 0;
        liblas::uint32_t const size = reader.GetHeader().GetPointRecordsCount();

        std::cout << "Translating " << size << " points:\n";

        ogr_wrapper<OGRFeatureH> feat(OGR_F_Create(OGR_L_GetLayerDefn(lyr)), OGR_F_Destroy);
        
        while (reader.ReadNextPoint())
        {
            liblas::LASPoint const& p = reader.GetPoint();   

            OGR_F_SetFieldInteger(feat, 0, p.GetReturnNumber());
            OGR_F_SetFieldInteger(feat, 1, p.GetScanAngleRank());
            OGR_F_SetFieldInteger(feat, 2, p.GetIntensity());
            OGR_F_SetFieldInteger(feat, 3, p.GetClassification());
            OGR_F_SetFieldInteger(feat, 4, p.GetNumberOfReturns());
            OGR_F_SetFieldDouble(feat, 5, p.GetTime());

            ogr_wrapper<OGRGeometryH> geom(OGR_G_CreateGeometry(wkbPoint25D), OGR_G_DestroyGeometry);
            OGR_G_SetPoint(geom, 0, p.GetX(), p.GetY(), p.GetZ());
            if (OGRERR_NONE != OGR_F_SetGeometry(feat, geom))
            {
                throw std::runtime_error("geometry creation failed");
            }

            if (OGRERR_NONE != OGR_L_CreateFeature(lyr, feat))
            {
                throw std::runtime_error("feature creation failed");
            }

            term_progress(std::cout, (i + 1) / static_cast<double>(size));
            i++;
        }

        std::cout << std::endl;
    }
    catch (std::exception const& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        rc = -1;
    }
    catch (...)
    {
        std::cerr << "Unknown error\n";
        rc = -1;
    }

    return rc;
}
#endif // #ifdef HAVE_GDAL

