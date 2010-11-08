/***************************************************************************
 *
 * Project: libLAS -- C/C++ read/write library for LAS LIDAR data
 * Purpose: LAS translation with optional configuration
 * Author:  Howard Butler, hobu.inc at gmail.com
 ***************************************************************************
 * Copyright (c) 2010, Howard Butler, hobu.inc at gmail.com 
 *
 * See LICENSE.txt in this source distribution for more information.
 **************************************************************************/

#include <liblas/liblas.hpp>
#include "laskernel.hpp"

#include <boost/cstdint.hpp>
#include <boost/foreach.hpp>

namespace po = boost::program_options;

using namespace liblas;
using namespace std;

// 0, 4, 16, 64, 256
//std::size_t const default_buffer_size = 0;
//std::size_t const default_buffer_size = 4 * 1024;
//std::size_t const default_buffer_size = 16 * 1024;
//std::size_t const default_buffer_size = 64 * 1024;
//std::size_t const default_buffer_size = 256 * 1024;
std::size_t const default_buffer_size = 1024 * 1024;
std::vector<char> ofs_buffer;
std::vector<char> ifs_buffer;

std::ofstream& set_ofstream_buffer(std::ofstream& ofs, std::size_t buffer_size)
{
    char* buffer = 0; // unbuffered;
    if (buffer_size > 0)
    {
        std::vector<char>(buffer_size).swap(ofs_buffer);
        buffer = &ofs_buffer[0];
    }

    std::streambuf* sb = ofs.rdbuf()->pubsetbuf(buffer, buffer_size);
    if (0 != buffer && 0 == sb)
    {
        throw std::runtime_error("failed to attach non-null buffer to input file stream");
    }
    return ofs;
}

std::ifstream& set_ifstream_buffer(std::ifstream& ifs, std::size_t buffer_size)
{
    char* buffer = 0; // unbuffered;
    if (buffer_size > 0)
    {
        std::vector<char>(buffer_size).swap(ifs_buffer);
        buffer = &ifs_buffer[0];
    }

    std::streambuf* sb = ifs.rdbuf()->pubsetbuf(buffer, buffer_size);
    if (0 != buffer && 0 == sb)
    {
        throw std::runtime_error("failed to attach non-null buffer to input file stream");
    }
    return ifs;
}

liblas::Writer* start_writer(   std::ofstream* strm, 
                                std::string const& output, 
                                liblas::Header const& header)
{
    
    if (!liblas::Create(*strm, output.c_str()))
    {
        std::ostringstream oss;
        oss << "Cannot create " << output << "for write.  Exiting...";
        throw std::runtime_error(oss.str());
    }

    // set_ofstream_buffer(*strm, default_buffer_size);

    liblas::Writer* writer = new liblas::Writer(*strm, header);
    return writer;
    
}

bool process(   std::string const& input,
                std::string const& output,
                liblas::Header & header,
                std::vector<liblas::FilterPtr>& filters,
                std::vector<liblas::TransformPtr>& transforms,
                boost::uint32_t split_mb,
                boost::uint32_t split_pts,
                bool verbose,
                bool min_offset)
{

    std::ifstream ifs;
    if (!liblas::Open(ifs, input.c_str()))
    {
        std::cerr << "Cannot open " << input << "for read.  Exiting...";
        return false;
    }
    // set_ifstream_buffer(ifs, default_buffer_size);

    liblas::Reader reader(ifs);
    liblas::Summary* summary = new liblas::Summary;
    
    reader.SetFilters(filters);
    reader.SetTransforms(transforms);    
    
    if (min_offset) 
    {
        
        liblas::property_tree::ptree tree = reader.Summarize();
        try
        {
            header.SetOffset(tree.get<double>("summary.points.minimum.x"),
                             tree.get<double>("summary.points.minimum.y"),
                             tree.get<double>("summary.points.minimum.z"));
    
                              
        }
        catch (liblas::property_tree::ptree_bad_path const& e) 
        {
            std::cerr << "Unable to write minimum header info.  Does the outputted file have any points?";
            std::cerr << e.what() << std::endl;
            return false;
        }
        if (verbose) 
        {
            
    
            std::cout << "Using minimum offsets ";
            SetStreamPrecision(std::cout, header.GetScaleX());
            std::cout << header.GetOffsetX() << " ";
            SetStreamPrecision(std::cout, header.GetScaleY());
            std::cout << header.GetOffsetY() << " ";
            SetStreamPrecision(std::cout, header.GetScaleZ());
            std::cout << header.GetOffsetZ() << " ";
            std::cout << std::endl;
        }
        reader.Reset();
    }

    std::ofstream* ofs = new std::ofstream;
    std::string out = output;
    liblas::Writer* writer = 0;
    if (!split_mb && !split_pts) {
        writer = start_writer(ofs, output, header);
        
    } else {
        string::size_type dot_pos = output.find_first_of(".");
        out = output.substr(0, dot_pos);
        writer = start_writer(ofs, out+"-1"+".las", header);
    }

    if (verbose)
    std::cout << "Writing output:" 
        << "\n - : " << output
        << std::endl;

    //
    // Translation of points cloud to features set
    //
    boost::uint32_t i = 0;
    boost::uint32_t const size = header.GetPointRecordsCount();
    
    boost::int32_t split_bytes_count = 1024*1024*split_mb;
    boost::uint32_t split_points_count = 0;
    int fileno = 2;

    while (reader.ReadNextPoint())
    {
        
        liblas::Point const& p = reader.GetPoint();
        summary->AddPoint(p);
        writer->WritePoint(p);
        if (verbose)
            term_progress(std::cout, (i + 1) / static_cast<double>(size));
        i++;
        split_points_count++;

        split_bytes_count = split_bytes_count - header.GetSchema().GetByteSize();        
        if (split_bytes_count < 0 && split_mb > 0 && ! split_pts) {
            // The user specifies a split size in mb, and we keep counting 
            // down until we've written that many points into the file.  
            // After that point, we make a new file and start writing into 
            // that.  
            delete writer;
            delete ofs;
            
            ofs = new std::ofstream;
            ostringstream oss;
            oss << out << "-"<< fileno <<".las";

            writer = start_writer(ofs, oss.str(), header);

            ostringstream old_filename;
            old_filename << out << "-" << fileno - 1 << ".las";

            liblas::Header hnew = FetchHeader(old_filename.str());
            RepairHeader(*summary, hnew);
            RewriteHeader(hnew, old_filename.str());

            delete summary;
            summary =  new liblas::Summary; 
            fileno++;
            split_bytes_count = 1024*1024*split_mb;
        }

        if (split_pts > 0 && ! split_mb && split_points_count == split_pts) {
            // The user specifies a split size in pts, and we keep counting 
            // down until we've written that many points into the file.  
            // After that point, we make a new file and start writing into 
            // that.  

            delete writer;
            delete ofs;
            
            ofs = new std::ofstream;
            ostringstream oss;
            oss << out << "-"<< fileno <<".las";

            writer = start_writer(ofs, oss.str(), header);

            ostringstream old_filename;
            old_filename << out << "-" << fileno - 1 << ".las";
            
            liblas::Header hnew = FetchHeader(old_filename.str());
            RepairHeader(*summary, hnew);
            RewriteHeader(hnew, old_filename.str());
            delete summary;
            summary =  new liblas::Summary; 
            fileno++;
            split_points_count = 0;
        }

    }
    if (verbose)
        std::cout << std::endl;
    
    reader.Reset();
    // liblas::property_tree::ptree pts = summary->GetPTree();
    // liblas::property_tree::ptree top;
    // top.add_child("summary.header",reader.GetHeader().GetPTree());
    // top.add_child("summary.points",pts);
    // liblas::property_tree::write_xml("junk.xml", top);
    // liblas::property_tree::write_xml("schema.xml", reader.GetHeader().GetSchema().GetPTree());
    // 
    // std::cout <<  reader.GetHeader().GetSchema() << std::endl;
    delete writer;
    delete ofs;
    
    liblas::Header hnew = FetchHeader(output);
    RepairHeader(*summary, hnew);
    RewriteHeader(hnew, output);

    delete summary;
    
    return true;
}

void OutputHelp( std::ostream & oss, po::options_description const& options)
{
    oss << "--------------------------------------------------------------------\n";
    oss << "    las2las (" << GetFullVersion() << ")\n";
    oss << "--------------------------------------------------------------------\n";

    oss << options;

    oss <<"\nFor more information, see the full documentation for las2las2 at:\n";
    
    oss << " http://liblas.org/utilities/las2las2.html\n";
    oss << "----------------------------------------------------------\n";

}

int main(int argc, char* argv[])
{

    boost::uint32_t split_mb = 0;
    boost::uint32_t split_pts = 0;
    std::string input;
    std::string output;
    
    bool verbose = false;
    bool bMinOffset = false;
    
    std::vector<liblas::FilterPtr> filters;
    std::vector<liblas::TransformPtr> transforms;
    
    liblas::Header header;

    try {

        po::options_description file_options("las2las2 options");
        po::options_description filtering_options = GetFilteringOptions();
        po::options_description header_options = GetHeaderOptions();
        po::options_description transform_options = GetTransformationOptions() ;

        po::positional_options_description p;
        p.add("input", 1);
        p.add("output", 1);

        file_options.add_options()
            ("help,h", "produce help message")
            ("split-mb", po::value<boost::uint32_t>(&split_mb)->default_value(0), "Split file into multiple files with each being this size in MB or less. If this value is 0, no splitting is done")
            ("split-pts", po::value<boost::uint32_t>(&split_pts)->default_value(0), "Split file into multiple files with each being this many points or less. If this value is 0, no splitting is done")
            ("input,i", po::value< string >(), "input LAS file")
            ("output,o", po::value< string >(&output)->default_value("output.las"), "output LAS file")
            ("verbose,v", po::value<bool>(&verbose)->zero_tokens(), "Verbose message output")
        ;

        po::variables_map vm;
        po::options_description options;
        options.add(file_options).add(header_options).add(transform_options).add(filtering_options);
        po::store(po::command_line_parser(argc, argv).
          options(options).positional(p).run(), vm);

        po::notify(vm);

        if (vm.count("help")) 
        {
            OutputHelp(std::cout, options);
            return 1;
        }

        if (split_pts > 0 && split_mb > 0) 
        {
            std::cerr << "Both split-mb and split-pts cannot be used simultaneously." << std::endl; 
            return 1;
        }

        if (vm.count("input")) 
        {
            input = vm["input"].as< string >();
            
            if (verbose)
                std::cout << "Opening " << input << " to fetch Header" << std::endl;

            std::ifstream ifs;
            if (!liblas::Open(ifs, input.c_str()))
            {
                std::cerr << "Cannot open " << input << "for read.  Exiting...";
                return 1;
            }
            // set_ifstream_buffer(ifs, default_buffer_size);

            liblas::Reader reader(ifs);
            header = reader.GetHeader();
        } else {
            std::cerr << "Input LAS file not specified!\n";
            OutputHelp(std::cout, options);
            return 1;
        }

        if (vm.count("min-offset")) 
        {
            if (vm.count("offset")) {
                throw std::runtime_error("min-offset cannot be used with offets.  Use one or the other");
            }
            
            bMinOffset = true;
        } 

        filters = GetFilters(vm, verbose);
        
        // Transforms alter our header as well.  Setting scales, offsets, etc.
        transforms = GetTransforms(vm, verbose, header);
        
        bool op = process(  input, 
                            output,
                            header, 
                            filters,
                            transforms,
                            split_mb,
                            split_pts,
                            verbose,
                            bMinOffset
                            );
        if (!op) {
            return (1);
        }
        
    }
    catch(std::exception& e)
    {
        std::cerr << "error: " << e.what() << "\n";
        return 1;
    }
    catch(...)
    {
        std::cerr << "Exception of unknown type!\n";
    }
    
    return 0;


}

//las2las2 -i lt_srs_rt.las  -o foo.las -c 1,2 -b 2483590,366208,2484000,366612
