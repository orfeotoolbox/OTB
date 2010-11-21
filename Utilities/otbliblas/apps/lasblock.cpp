// $Id$
//
// lasblock generates block output for las2oci to store lidar data in OPC tables
//
//
// (C) Copyright Howard Butler 2010, hobu.inc@gmail.com
//
// Distributed under the BSD License
// (See accompanying file LICENSE.txt or copy at
// http://www.opensource.org/licenses/bsd-license.php)
//
#include <liblas/liblas.hpp>
#include "chipper.hpp"
// std
#include <fstream>
#include <vector>

// boost
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4512)
#endif

#include <boost/program_options.hpp>

#ifdef _MSC_VER
#pragma warning(pop)
#endif

namespace po = boost::program_options;

#ifdef _WIN32
#define compare_no_case(a,b,n)  _strnicmp( (a), (b), (n) )
#else
#define compare_no_case(a,b,n)  strncasecmp( (a), (b), (n) )
#endif

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


using namespace liblas;


int main(int argc, char* argv[])
{
    std::string input;
    std::string output;
    
    long capacity = 3000;
    long precision = 8;
    bool verbose = false;


    po::options_description desc("Allowed lasblock options");
    po::positional_options_description p;
    p.add("input", 1);
    p.add("output", 1);

    desc.add_options()
        ("help,h", "Produce this help message")
        ("capacity,c", po::value<long>(&capacity)->default_value(3000), "Number of points to nominally put into each block (note that this number will not be exact)")
        ("precision,p", po::value<long>(&precision)->default_value(8), "Number of decimal points to write for each bbox")
        ("input,i", po::value< std::string >(), "input LAS file")
        ("output,o", po::value< std::string >(&output)->default_value(""), "The output .kdx file (defaults to input filename + .kdx)")
        ("verbose,v", po::value<bool>(&verbose)->zero_tokens(), "Verbose message output")

    ;
    
    po::variables_map vm;        
    po::store(po::command_line_parser(argc, argv).
      options(desc).positional(p).run(), vm);

    po::notify(vm);

    if (vm.count("help")) 
    {
        std::cout << desc << "\n";
            std::cout <<"\nFor more information, see the full documentation for lasblock at:\n";
            
            std::cout << " http://liblas.org/utilities/block.html\n";
            std::cout << "----------------------------------------------------------\n";
        return 1;
    }
        
    if (vm.count("input")) 
    {
        input = vm["input"].as< std::string >();
        std::ifstream ifs;

        if (!liblas::Open(ifs, input.c_str()))
        {
            std::cerr << "Cannot open file '" << input << "'for read.  Exiting...";
            return 1;
        }
    } 
    

    if (output.empty())
    {
        output = std::string(input) + ".kdx";
    }

    std::ifstream in;
    if (!liblas::Open(in, input.c_str()))
    {
        std::cerr << "Cannot open " << input << "for read.  Exiting...";
        return 1;
    }
            
    std::ofstream out(output.c_str());
    
    liblas::Reader reader( in );
    
    liblas::chipper::Chipper c(&reader, capacity);

    if (verbose)
        std::cout << "Blocking " << input<< " to " << output <<std::endl;

    c.Chip();
    

    boost::uint32_t num_blocks = c.GetBlockCount();
    
    if (verbose)
        std::cout << "Writing " << num_blocks << " blocks to " << output << std::endl;

    boost::uint32_t prog = 0;
    
    for ( boost::uint32_t i = 0; i < num_blocks; ++i )
    {
        const liblas::chipper::Block& b = c.GetBlock(i);

        std::vector<boost::uint32_t> ids = b.GetIDs();
        out << i << " " << ids.size() << " ";
        
        out.setf(std::ios::fixed,std::ios::floatfield);
        out.precision(precision);
        liblas::Bounds<double> const& bnd = b.GetBounds();
        out << bnd.min(0) << " " << bnd.min(1) << " " << bnd.max(0) << " " <<  bnd.max(1) << " " ;
        
        for ( boost::uint32_t pi = 0; pi < ids.size(); ++pi )
        {
            out << ids[pi] << " ";
        }

        out << std::endl;

        // Set back to writing decimals
        out.setf(std::ios::dec);

        if (verbose)
            term_progress(std::cout, (prog + 1) / static_cast<double>(num_blocks));
        prog++;
    }
}
