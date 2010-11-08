
#include "laskernel.hpp"

std::istream* OpenInput(std::string const& filename, bool bEnd) 
{
    std::ios::openmode mode = std::ios::in | std::ios::binary;
    if (bEnd == true) {
        mode = mode | std::ios::ate;
    }
    std::istream* istrm;
    if (compare_no_case(filename.c_str(),"STDIN",5) == 0)
    {
        istrm = &std::cin;
    }
    else 
    {
        istrm = new std::ifstream(filename.c_str(), mode);
    }
    
    if (!istrm->good())
    {
        delete istrm;
        throw std::runtime_error("Reading stream was not able to be created");
    }
    return istrm;
}

std::string TryReadFileData(std::string const& filename)
{
    std::vector<char> data = TryReadRawFileData(filename);

    // FIXME: What is this construction supposed to grab? --mloskot
    return std::string(&data[0], data.size());
}

std::vector<char> TryReadRawFileData(std::string const& filename)
{
    std::istream* infile = OpenInput(filename.c_str(), true);
    std::ifstream::pos_type size;
    // char* data;
    std::vector<char> data;
    if (infile->good()){
        size = infile->tellg();
        data.resize(size);
        // data = new char [size];
        infile->seekg (0, std::ios::beg);
        infile->read (&data.front(), size);
        // infile->close();

        // delete[] data;
        delete infile;
        return data;
    } 
    else 
    {   
        delete infile;
        return data;
    }
}

bool term_progress(std::ostream& os, double complete)
{
    static int lastTick = -1;
    int tick = static_cast<int>(complete * 40.0);

    tick = (std::min)(40, std::max(0, tick));

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

void SetStreamPrecision(std::ostream& os, double scale)
{
    os.setf(std::ios_base::fixed, std::ios_base::floatfield);

    double frac = 0;
    double integer = 0;
    frac = std::modf(scale, &integer);

    boost::uint32_t prec = static_cast<boost::uint32_t>(std::fabs(std::floor(std::log10(frac))));
    os.precision(prec);    
}

liblas::Header FetchHeader(std::string const& filename)
{
    
    std::ifstream ifs;
    if (!liblas::Open(ifs, filename.c_str()))
    {
        std::ostringstream oss;
        oss << "Cannot open " << filename << "for read.  Exiting...";
        throw std::runtime_error(oss.str());
    }
    liblas::Reader reader(ifs);
    liblas::Header header = reader.GetHeader();
    ifs.close();
    return header;
}
void RewriteHeader(liblas::Header const& header, std::string const& filename)
{

    std::ios::openmode m = std::ios::out | std::ios::in | std::ios::binary | std::ios::ate;

    // Write a blank PointRecordsByReturnCount first
    std::ofstream ofs(filename.c_str(), m);
    liblas::Writer writer(ofs, header);
    ofs.close();

    // Write our updated header with summary info
    std::ofstream ofs2(filename.c_str(), m);
    liblas::Writer writer2(ofs2, header);
    ofs2.close();
    
}  
  
void RepairHeader(liblas::Summary const& summary, liblas::Header& header)
{

    for (boost::uint32_t i = 0; i < 5; i++)
    {
        header.SetPointRecordsByReturnCount(i, 0);
    }

    liblas::property_tree::ptree tree = summary.GetPTree();
    
    try
    {
        header.SetMin(tree.get<double>("summary.points.minimum.x"),
                      tree.get<double>("summary.points.minimum.y"),
                      tree.get<double>("summary.points.minimum.z"));
    
        header.SetMax(tree.get<double>("summary.points.maximum.x"),
                      tree.get<double>("summary.points.maximum.y"),
                      tree.get<double>("summary.points.maximum.z"));
        
    }     catch (liblas::property_tree::ptree_bad_path const& ) 
    {
        std::cerr << "Unable to write header bounds info.  Does the outputted file have any points?";
        return;
    }

    try
    {

        for (boost::uint32_t i = 0; i < 5; i++)
        {
            header.SetPointRecordsByReturnCount(i, 0);
        }
    
        BOOST_FOREACH(ptree::value_type &v,
                tree.get_child("summary.points.points_by_return"))
        {
            boost::uint32_t i = v.second.get<boost::uint32_t>("id");
            boost::uint32_t count = v.second.get<boost::uint32_t>("count");
            header.SetPointRecordsByReturnCount(i-1, count);        
        } 
        
    }     catch (liblas::property_tree::ptree_bad_path const& ) 
    {
        std::cerr << "Unable to write header point return count info.  Does the outputted file have any points?";
        return;
    }
    
}

bool IsDualRangeFilter(std::string parse_string) 
{

string::size_type dash = parse_string.find_first_of("-");

if (dash != std::string::npos) {
    return true;
}
return false;
}

liblas::FilterPtr  MakeReturnFilter(std::vector<boost::uint16_t> const& returns, liblas::FilterI::FilterType ftype) 
{

    liblas::ReturnFilter* return_filter = new liblas::ReturnFilter(returns, false);
    return_filter->SetType(ftype);
    return liblas::FilterPtr(return_filter);
}


liblas::FilterPtr  MakeClassFilter(std::vector<liblas::Classification> const& classes, liblas::FilterI::FilterType ftype) 
{

    liblas::ClassificationFilter* class_filter = new liblas::ClassificationFilter(classes); 
    class_filter->SetType(ftype);
    return liblas::FilterPtr(class_filter);
}

liblas::FilterPtr  MakeBoundsFilter(liblas::Bounds<double> const& bounds, liblas::FilterI::FilterType ftype) 
{
    liblas::BoundsFilter* bounds_filter = new liblas::BoundsFilter(bounds);
    bounds_filter->SetType(ftype);
    return liblas::FilterPtr(bounds_filter);
}

liblas::FilterPtr  MakeIntensityFilter(std::string intensities, liblas::FilterI::FilterType ftype) 
{
    liblas::ContinuousValueFilter<boost::uint16_t>::filter_func f = &liblas::Point::GetIntensity;
    liblas::ContinuousValueFilter<boost::uint16_t>* intensity_filter = new liblas::ContinuousValueFilter<boost::uint16_t>(f, intensities);
    intensity_filter->SetType(ftype);
    return liblas::FilterPtr(intensity_filter);
}

liblas::FilterPtr MakeTimeFilter(std::string times, liblas::FilterI::FilterType ftype) 
{
    liblas::ContinuousValueFilter<double>::filter_func f = &liblas::Point::GetTime;
    liblas::ContinuousValueFilter<double>* time_filter = new liblas::ContinuousValueFilter<double>(f, times);
    time_filter->SetType(ftype);
    return liblas::FilterPtr(time_filter);
}

liblas::FilterPtr MakeScanAngleFilter(std::string intensities, liblas::FilterI::FilterType ftype) 
{
    liblas::ContinuousValueFilter<boost::int8_t>::filter_func f = &liblas::Point::GetScanAngleRank;
    liblas::ContinuousValueFilter<boost::int8_t>* intensity_filter = new liblas::ContinuousValueFilter<boost::int8_t>(f, intensities);
    intensity_filter->SetType(ftype);
    return liblas::FilterPtr(intensity_filter);
}

liblas::FilterPtr MakeColorFilter(liblas::Color const& low, liblas::Color const& high, liblas::FilterI::FilterType ftype)
{
    liblas::ColorFilter* filter = new liblas::ColorFilter(low, high);
    filter->SetType(ftype);
    return liblas::FilterPtr(filter);
}

po::options_description GetFilteringOptions() 
{

po::options_description filtering_options("Filtering options");

filtering_options.add_options()
    ("extent,e", po::value< string >(), "Extent window that points must fall within to keep.\nUse a comma-separated or quoted, space-separated list, for example, \n -e minx, miny, maxx, maxy\n or \n -e minx, miny, minz, maxx, maxy, maxz\n -e \"minx miny minz maxx maxy maxz\"")     
    ("minx", po::value< double >(), "Extent must be greater than or equal to minx to be kept. \n --minx 1234.0")
    ("miny", po::value< double >(), "Extent must be greater than or equal to miny to be kept. \n --miny 5678.0")
    ("minz", po::value< double >(), "Extent must be greater than or equal to minz to be kept. If maxx and maxy are set but not minz *and maxz, all z values are kept. \n --minz 0.0")
    ("maxx", po::value< double >(), "Extent must be less than or equal to maxx to be kept. \n --maxx 1234.0")
    ("maxy", po::value< double >(), "Extent must be less than or equal to maxy to be kept. \n --maxy 5678.0")
    ("maxz", po::value< double >(), "Extent must be less than or equal to maxz to be kept. If maxx and maxy are set but not maxz *and minz, all z values are kept. \n --maxz 10.0")
    ("thin,t", po::value<boost::uint32_t>()->default_value(0), "Simple decimation-style thinning.\nThin the file by removing every t'th point from the file.")
    ("last_return_only", po::value<bool>()->zero_tokens(), "Keep last returns (cannot be used with --first_return_only)")
    ("first_return_only", po::value<bool>()->zero_tokens(), "Keep first returns (cannot be used with --last_return_only")
    ("keep-returns", po::value< std::vector<boost::uint16_t> >()->multitoken(), "A list of return numbers to keep in the output file: \n--keep-returns 1 2 3")
    ("drop-returns", po::value< std::vector<boost::uint16_t> >()->multitoken(), "Return numbers to drop.\nFor example, --drop-returns 2 3 4 5")
    ("valid_only", po::value<bool>()->zero_tokens(), "Keep only valid points")
    ("keep-classes", po::value< std::vector<boost::uint32_t > >()->multitoken(), "A list of classifications to keep:\n--keep-classes 2 4 12\n--keep-classes 2")
    ("drop-classes", po::value< std::vector<boost::uint32_t > >()->multitoken(), "A comma-separated list of classifications to drop:\n--drop-classes 1,7,8\n--drop-classes 2")
    ("keep-intensity", po::value< string >(), "Range in which to keep intensity.\nThe following expression types are supported:  \n--keep-intensity 0-100 \n--keep-intensity <200 \n--keep-intensity >400 \n--keep-intensity >=200")
    ("drop-intensity", po::value< string >(), "Range in which to drop intensity.\nThe following expression types are supported:  \n--drop-intensity <200 \n--drop-intensity >400 \n--drop-intensity >=200")
    ("keep-time", po::value< string >(), "Range in which to keep time.\nThe following expression types are supported:  \n--keep-time 413665.2336-414092.8462 \n--keep-time <414094.8462 \n--keep-time >413665.2336 \n--keep-time >=413665.2336")
    ("drop-time", po::value< string >(), "Range in which to drop time.\nThe following expression types are supported:  \n--drop-time <413666.2336 \n--drop-time >413665.2336 \n--drop-time >=413665.2336")
    ("keep-scan-angle", po::value< string >(), "Range in which to keep scan angle.\nThe following expression types are supported:  \n--keep-scan-angle 0-100 \n--keep-scan-angle <100\n--keep-scan-angle <=100")
    ("drop-scan-angle", po::value< string >(), "Range in which to drop scan angle.\nThe following expression types are supported:  \n--drop-scan-angle <30 \n--drop-scan-angle >100 \n--drop-scan-angle >=100")
    ("keep-color", po::value< string >(), "Range in which to keep colors.\nDefine colors as two 3-tuples (R,G,B-R,G,B):  \n--keep-color '0,0,0-125,125,125'")
    ("drop-color", po::value< string >(), "Range in which to drop colors.\nDefine colors as two 3-tuples (R,G,B-R,G,B):  \n--drop-color '255,255,255-65536,65536,65536'")
;
return filtering_options;    
}

po::options_description GetTransformationOptions() 
{
    po::options_description transform_options("Transformation options");

    transform_options.add_options()
        ("t_srs", po::value< string >(), "Coordinate system to reproject output LAS file to.  Use --a_srs or verify that your input LAS file has a coordinate system according to lasinfo")
    ;
    
    return transform_options;
}

po::options_description GetHeaderOptions() 
{
    po::options_description transform_options("Header modification options");

    transform_options.add_options()
        ("a_srs", po::value< string >(), "Coordinate system to assign to input LAS file")
        ("a_vertcs", po::value< std::vector<string> >()->multitoken(), "Override vertical coordinate system information.  Use --a_vertcs \"verticalCSType [citation [verticalDatum [verticalUnits]]]\"\nFor example: --a_vertcs 5703 \"North American Vertical Datum of 1988 (NAVD88)\" 5103 9001")   
        ("offset", po::value< string >(), "A comma-separated or quoted, space-separated list of offsets to set on the output file: \n--offset 0,0,0\n--offset \"1234 5678 91011\"")
        ("scale", po::value< std::vector<double> >()->multitoken(), "A list of scales to set on the output file. Scales *cannot* be negative, and should always be a negative power of 10 \n--scale 0.1 0.1 0.00001")
        ("format,f", po::value< string >(), "Set the LAS format of the new file (only 1.0-1.2 supported at this time): \n--format 1.2\n-f 1.1")
        ("pad-header", po::value< string >(), "Add extra bytes to the existing header")
        ("min-offset", po::value<bool>()->zero_tokens(), "Set the offset of the header to the minimums of all values in the file.  Note that this requires multiple read passes through the file to achieve.")
        ("file-creation", po::value< std::vector<string> >()->multitoken(), "Set the header's day/year.  Specify either as \"1 2010\" for the first day of 2010, or as \"now\" to specify the current day/year")
        ("add-schema", po::value<bool>()->zero_tokens(), "Add the liblas.org schema VLR record to the file.")
        ("delete-vlr", po::value<std::vector<std::string> >()->multitoken(), "Removes VLRs with the given name and id combination. --delete-vlr LASF_Projection 34737")
        ("add-vlr", po::value<std::vector<std::string> >()->multitoken(), "Add VLRs with the given name and id combination. --add-vlr hobu 1234 \"Description of the VLR\" \"filename.ext\"")
        ("system-identifier", po::value<std::string>(), "Set the SystemID for the file. --system_identifier \"MODIFICATION\"")
        ("generating-software", po::value<std::string>(), "Set the SoftwareID for the file. --generating_software \"liblas.org\"")
        ("point-translate", po::value<std::string>(), "An expression to translate the X, Y, Z values of the point. For example, converting Z units that are in meters to feet: --point-translate \"x*1.0 y*1.0 z*3.2808399\"")

    ;
    
    return transform_options;
}
std::vector<liblas::FilterPtr> GetFilters(po::variables_map vm, bool verbose)
{
    std::vector<liblas::FilterPtr> filters;
    liblas::Bounds<double> extent;
    bool bSetExtent = false;
    
    if (vm.count("keep-classes")) 
    {
        std::vector<boost::uint32_t> classes = vm["keep-classes"].as< std::vector<boost::uint32_t> >();
        
        std::vector<liblas::Classification> klasses;

        ostringstream oss;
        
        for (std::vector<boost::uint32_t>::const_iterator i = classes.begin();
             i != classes.end();
             i++) 
            {
                oss << *i << " ";
                klasses.push_back(liblas::Classification(*i, false, false, false));
            }
        if (verbose)
        {

                std::cout << "Keeping classes with the values: " << oss.str() << std::endl;
        }
        
            
        liblas::FilterPtr class_filter = MakeClassFilter(  klasses, 
                                                          liblas::FilterI::eInclusion);
        filters.push_back(class_filter); 
    }

    if (vm.count("drop-classes")) 
    {
        std::vector<boost::uint32_t> classes = vm["drop-classes"].as< std::vector<boost::uint32_t> >();

        std::vector<liblas::Classification> klasses;

        ostringstream oss;
        
        for (std::vector<boost::uint32_t>::const_iterator i = classes.begin();
             i != classes.end();
             i++) 
            {
                oss << *i << " ";
                klasses.push_back(liblas::Classification(*i,false, false, false));
            }
        if (verbose)
        {

                std::cout << "Dropping classes with the values: " << oss.str() << std::endl;
        }
        liblas::FilterPtr class_filter = MakeClassFilter(  klasses, 
                                                            liblas::FilterI::eExclusion);
        filters.push_back(class_filter);
    }

    if (vm.count("keep-returns")) 
    {
        std::vector<boost::uint16_t> returns = vm["keep-returns"].as< std::vector<boost::uint16_t> >();


        if (verbose)
        {
            ostringstream oss;
            for (std::vector<boost::uint16_t>::const_iterator i = returns.begin();
                 i != returns.end();
                 i++) 
                {
                    oss << *i << " ";
                }
                std::cout << "Keeping returns with the values: " << oss.str() << std::endl;
        }

        liblas::FilterPtr return_filter = MakeReturnFilter(  returns, 
                                                            liblas::FilterI::eInclusion);
        filters.push_back(return_filter); 
    }

    if (vm.count("drop-returns")) 
    {
        std::vector<boost::uint16_t> returns = vm["keep-returns"].as< std::vector<boost::uint16_t> >();

        if (verbose)
        {
            ostringstream oss;
            for (std::vector<boost::uint16_t>::const_iterator i = returns.begin();
                 i != returns.end();
                 i++) 
                {
                    oss << *i << " ";
                }
                std::cout << "Dropping returns with the values: " << oss.str() << std::endl;
        }

        liblas::FilterPtr return_filter = MakeReturnFilter(  returns, 
                                                            liblas::FilterI::eExclusion);
        filters.push_back(return_filter); 
    }

    if (vm.count("minx")) 
    {
        double minx = vm["minx"].as< double >();
        extent.min(0, minx);
        bSetExtent = true;
        if (verbose)
            std::cout << "Setting minx to: " << minx << std::endl;
    }

    if (vm.count("maxx")) 
    {
        double maxx = vm["maxx"].as< double >();
        extent.max(0, maxx);
        bSetExtent = true;
        if (verbose)
            std::cout << "Setting maxx to: " << maxx << std::endl;
    }

    if (vm.count("miny")) 
    {
        double miny = vm["miny"].as< double >();
        extent.min(1, miny);
        bSetExtent = true;
        if (verbose)
            std::cout << "Setting miny to: " << miny << std::endl;
    }

    if (vm.count("maxx")) 
    {
        double maxy = vm["maxy"].as< double >();
        extent.max(1, maxy);
        bSetExtent = true;
        if (verbose)
            std::cout << "Setting maxy to: " << maxy << std::endl;
    }

    if (vm.count("minz")) 
    {
        double minz = vm["minz"].as< double >();
        extent.min(2, minz);
        bSetExtent = true;
        if (verbose)
            std::cout << "Setting minz to: " << minz << std::endl;
    }

    if (vm.count("maxz")) 
    {
        double maxz = vm["maxz"].as< double >();
        extent.max(2, maxz);
        bSetExtent = true;
        if (verbose)
            std::cout << "Setting maxz to: " << maxz << std::endl;
    }
    
    if (vm.count("extent")) 
    {
        std::string bounds_string = vm["extent"].as< string >();

        boost::char_separator<char> sep(SEPARATORS);

        std::vector<double> vbounds;
        tokenizer tokens(bounds_string, sep);
        liblas::Bounds<double> bounds;
        for (tokenizer::iterator t = tokens.begin(); t != tokens.end(); ++t) {
            vbounds.push_back(atof((*t).c_str()));
        }
        if (vbounds.size() == 4) 
        {
            bounds = liblas::Bounds<double>(vbounds[0], 
                                    vbounds[1], 
                                    vbounds[2], 
                                    vbounds[3]);
        } else if (vbounds.size() == 6)
        {
            bounds = liblas::Bounds<double>(vbounds[0], 
                                    vbounds[1], 
                                    vbounds[2], 
                                    vbounds[3], 
                                    vbounds[4], 
                                    vbounds[5]);
        } else {
            ostringstream oss;
            oss << "Bounds must be specified as a 4-tuple or "
                   "6-tuple, not a "<< vbounds.size()<<"-tuple" << "\n";
            throw std::runtime_error(oss.str());
        }
        
        if ( bSetExtent ) 
        {
            if (verbose) 
            {
                std::cout << " Growing --extent bounds with those that were set via --[x|y|z][min|max]" << std::endl;
            }
            bounds.grow(extent);
        }
    
        if (verbose)
        {
            std::cout << "---------------------------------------------------------" << std::endl;
            std::cout << " Clipping file to the extent" << std::endl;
            std::cout << "---------------------------------------------------------" << std::endl;

            std::cout.setf(std::ios_base::fixed, std::ios_base::floatfield);
            std::cout.precision(6);
    
            std::cout << " minx: " << bounds.minx() 
                      << " miny: " << bounds.miny() 
                      << " minz: " << bounds.minz() 
                      << std::endl;
            std::cout << " maxx: " << bounds.maxx() 
                      << " maxy: " << bounds.maxy() 
                      << " maxz: " << bounds.maxz() 
                      << std::endl;
            std::cout << "---------------------------------------------------------" << std::endl;
        }

        liblas::FilterPtr bounds_filter = MakeBoundsFilter(bounds, liblas::FilterI::eInclusion);
        // Set to false because we are using this opportunity to set the filter
        // If it were still true after this point, *another* BoundsFilter would be 
        // added to the filters list at the end of this function
        if (bSetExtent)
            bSetExtent = false; 
        filters.push_back(bounds_filter);
        
    }
    if (vm.count("keep-intensity")) 
    {
        std::string intensities = vm["keep-intensity"].as< string >();
        if (verbose)
            std::cout << "Keeping intensities with values: " << intensities << std::endl;
        if (IsDualRangeFilter(intensities)) {
            // We need to make two filters
            // Given a range 0-200, split the expression into two filters 
            string::size_type dash = intensities.find_first_of("-");
            std::string low = intensities.substr(0,dash);
            std::string high = intensities.substr(dash+1, intensities.size());

            liblas::FilterPtr lt_filter = MakeIntensityFilter(">="+low, liblas::FilterI::eInclusion);
            filters.push_back(lt_filter);
            liblas::FilterPtr gt_filter = MakeIntensityFilter("<="+high, liblas::FilterI::eInclusion);
            filters.push_back(gt_filter);                
        } else {
            liblas::FilterPtr intensity_filter = MakeIntensityFilter(intensities, liblas::FilterI::eInclusion);
            filters.push_back(intensity_filter);
            
        }
    }
    if (vm.count("drop-intensity")) 
    {
        std::string intensities = vm["drop-intensity"].as< string >();
        if (verbose)
            std::cout << "Dropping intensities with values: " << intensities << std::endl;

        if (IsDualRangeFilter(intensities)) {
            throw std::runtime_error("Range filters are not supported for drop-intensity");
        } else {
            liblas::FilterPtr intensity_filter = MakeIntensityFilter(intensities, liblas::FilterI::eExclusion);
            filters.push_back(intensity_filter);   
        }
    }
    if (vm.count("keep-scan-angle")) 
    {
        std::string angles = vm["keep-scan-angle"].as< string >();
        if (verbose)
            std::cout << "Keeping scan angles with values: " << angles << std::endl;
        if (IsDualRangeFilter(angles)) {
            // We need to make two filters
            // Given a range 0-200, split the expression into two filters 
            string::size_type dash = angles.find_first_of("-");
            std::string low = angles.substr(0,dash);
            std::string high = angles.substr(dash+1, angles.size());

            liblas::FilterPtr lt_filter = MakeScanAngleFilter(">="+low, liblas::FilterI::eInclusion);
            filters.push_back(lt_filter);
            liblas::FilterPtr gt_filter = MakeScanAngleFilter("<="+high, liblas::FilterI::eInclusion);
            filters.push_back(gt_filter);                
        } else {
            liblas::FilterPtr angle_filter = MakeScanAngleFilter(angles, liblas::FilterI::eInclusion);
            filters.push_back(angle_filter);
            
        }
    }
    if (vm.count("drop-scan-angle")) 
    {
        std::string angles = vm["drop-scan-angle"].as< string >();
        if (verbose)
            std::cout << "Dropping scan angles with values: " << angles << std::endl;

        if (IsDualRangeFilter(angles)) {
            throw std::runtime_error("Range filters are not supported for drop-scan-angle");
        } else {
            liblas::FilterPtr angle_filter = MakeScanAngleFilter(angles, liblas::FilterI::eExclusion);
            filters.push_back(angle_filter);   
        }
    }
    
    if (vm.count("keep-time")) 
    {
        std::string times = vm["keep-time"].as< string >();
        if (verbose)
            std::cout << "Keeping times with values: " << times << std::endl;
        if (IsDualRangeFilter(times)) {
            // We need to make two filters
            // Given a range 0-200, split the expression into two filters 
            string::size_type dash = times.find_first_of("-");
            std::string low = times.substr(0,dash);
            std::string high = times.substr(dash+1, times.size());

            liblas::FilterPtr lt_filter = MakeTimeFilter(">="+low, liblas::FilterI::eInclusion);
            filters.push_back(lt_filter);
            liblas::FilterPtr gt_filter = MakeTimeFilter("<="+high, liblas::FilterI::eInclusion);
            filters.push_back(gt_filter);                
        } else {
            liblas::FilterPtr time_filter = MakeTimeFilter(times, liblas::FilterI::eInclusion);
            filters.push_back(time_filter);
            
        }
    }
    if (vm.count("drop-time")) 
    {
        std::string times = vm["drop-time"].as< string >();
        if (verbose)
            std::cout << "Dropping times with values: " << times << std::endl;
            
        if (IsDualRangeFilter(times)) {
            throw std::runtime_error("Range filters are not supported for drop-time");
        } else {
            liblas::FilterPtr time_filter = MakeTimeFilter(times, liblas::FilterI::eExclusion);
            filters.push_back(time_filter);   
        }
    }

    if (vm.count("keep-color")) 
    {
        std::string keepers = vm["keep-color"].as< string >();
        if (verbose)
            std::cout << "Keeping colors in range:: " << keepers << std::endl;
            
        // Pull apart color ranges in the form: R,G,B-R,G,B
        boost::char_separator<char> sep_dash("-");
        boost::char_separator<char> sep_comma(",");
        std::vector<liblas::Color> colors;
        tokenizer low_high(keepers, sep_dash);
        for (tokenizer::iterator t = low_high.begin(); t != low_high.end(); ++t) {
            
            tokenizer rgbs((*t), sep_comma);
            std::vector<liblas::Color::value_type> rgb;
            for(tokenizer::iterator c = rgbs.begin(); c != rgbs.end(); ++c)
            {
                int color_val = atoi((*c).c_str());
                if (color_val < std::numeric_limits<boost::uint16_t>::min() || 
                    color_val > std::numeric_limits<boost::uint16_t>::max()) 
                {
                    ostringstream oss;
                    oss << "Color value must be between 0-65536, not " << color_val;
                    throw std::runtime_error( oss.str() );
                    
                }
                rgb.push_back(static_cast<boost::uint16_t>(color_val));
            }
            liblas::Color color(rgb[0], rgb[1], rgb[2]);
            colors.push_back(color);
        }
        
        liblas::FilterPtr color_filter = MakeColorFilter(colors[0], colors[1], liblas::FilterI::eInclusion);
        filters.push_back(color_filter);
    }
    if (vm.count("drop-color")) 
    {
        std::string dropers = vm["drop-color"].as< string >();
        if (verbose)
            std::cout << "Dropping colors in range:: " << dropers << std::endl;
            
        // Pull apart color ranges in the form: R,G,B-R,G,B
        boost::char_separator<char> sep_dash("-");
        boost::char_separator<char> sep_comma(",");
        std::vector<liblas::Color> colors;
        tokenizer low_high(dropers, sep_dash);
        for (tokenizer::iterator t = low_high.begin(); t != low_high.end(); ++t) {
            
            tokenizer rgbs((*t), sep_comma);
            std::vector<liblas::Color::value_type> rgb;
            for(tokenizer::iterator c = rgbs.begin(); c != rgbs.end(); ++c)
            {
                int color_val = atoi((*c).c_str());
                if (color_val < std::numeric_limits<boost::uint16_t>::min() || 
                    color_val > std::numeric_limits<boost::uint16_t>::max()) 
                {
                    ostringstream oss;
                    oss << "Color value must be between 0-65536, not " << color_val;
                    throw std::runtime_error( oss.str() );
                    
                }
                rgb.push_back(static_cast<boost::uint16_t>(color_val));
            }
            liblas::Color color(rgb[0], rgb[1], rgb[2]);
            colors.push_back(color);
        }
        
        liblas::FilterPtr color_filter = MakeColorFilter(colors[0], colors[1], liblas::FilterI::eExclusion);
        filters.push_back(color_filter);
    }

    if (vm.count("thin")) 
    {
        boost::uint32_t thin = vm["thin"].as< boost::uint32_t >();
        if (thin != 0) {
            if (verbose)
                std::cout << "Thining file by keeping every "<<thin<<"'th point "  << std::endl;
            
            liblas::FilterPtr thin_filter = liblas::FilterPtr(new liblas::ThinFilter(thin));
            filters.push_back(thin_filter);    
        }
    }


    if (vm.count("first_return_only") && vm.count("last_return_only")) {
        throw std::runtime_error( "--first_return_only and --last_return_only cannot "
                     "be used simultaneously.  Use --keep-returns 1 in "
                     "combination with --last_return_only");
    }

    if (vm.count("last_return_only")) {
        if (verbose)
            std::cout << "Keeping last returns only."  << std::endl;
        std::vector<boost::uint16_t> returns;
        liblas::FilterPtr last_filter = liblas::FilterPtr(new liblas::ReturnFilter(returns, true));
        filters.push_back(last_filter);
    }

    if (vm.count("first_return_only")){
        if (verbose)
            std::cout << "Keeping first returns only."  << std::endl;
        std::vector<boost::uint16_t> returns;
        returns.push_back(1);
        liblas::FilterPtr return_filter = liblas::FilterPtr(new liblas::ReturnFilter(returns, false));
        filters.push_back(return_filter);
    }
    
    if (vm.count("valid_only")){
        if (verbose)
            std::cout << "Keeping valid points only."  << std::endl;
        liblas::FilterPtr valid_filter = liblas::FilterPtr(new liblas::ValidationFilter());
        filters.push_back(valid_filter);            
    }


    // If we have bSetExtent and we haven't turned it off by merging with a --extent 
    // BoundsFilter, make a filter
    if (bSetExtent)
    {
        liblas::FilterPtr bounds_filter = MakeBoundsFilter(extent, liblas::FilterI::eInclusion);
        filters.push_back(bounds_filter);
    }
    
    return filters;
}

std::vector<liblas::TransformPtr> GetTransforms(po::variables_map vm, bool verbose, liblas::Header& header)
{
    std::vector<liblas::TransformPtr> transforms;

    if (vm.count("offset")) 
    {
        std::string offset_string = vm["offset"].as< string >();
        if (verbose)
            std::cout << "Setting offsets to: " << offset_string << std::endl;
        boost::char_separator<char> sep(SEPARATORS);
        std::vector<double> offsets;
        tokenizer tokens(offset_string, sep);
        bool mins = false;
        std::string m("min");
        for (tokenizer::iterator t = tokens.begin(); t != tokens.end(); ++t) {
            // Check if the user set --offset min,min,min
            // FIXME: make this so the user could do --offset min,min,20.00
            if (!(*t).compare(m))
            {   
                mins = true;
                continue;
            }
            else
            {
                mins = false;
                offsets.push_back(atof((*t).c_str()));
            }
        }
        if (offsets.size() != 3) 
        {
            throw std::runtime_error("All three values for setting the offset must be floats, and there must be three values");

        }
        header.SetOffset(offsets[0], offsets[1], offsets[2]);
    }


    if (vm.count("scale")) 
    {
        std::vector<double> scales = vm["scale"].as< std::vector<double> >();

        if (scales.size() != 3) {
            ostringstream oss;
            oss << "Three arguments must be given to scale. "
                << "--scale x y z";

            throw std::runtime_error(oss.str());
        }

        
        if (verbose)
        {
            ostringstream oss;
            for (std::vector<double>::const_iterator i = scales.begin();
                 i != scales.end();
                 i++) 
                {
                    oss << *i << " ";
                }
                std::cout << "Setting scales to: " << oss.str() << std::endl;
        }


        header.SetScale(scales[0], scales[1], scales[2]);
    }
    
    if (vm.count("format")) 
    {
        std::string format_string = vm["format"].as< string >();
        if (verbose)
            std::cout << "Setting format to: " << format_string << std::endl;
            
        boost::char_separator<char> sep(".");
        std::vector<int> versions;
        tokenizer tokens(format_string, sep);
        for (tokenizer::iterator t = tokens.begin(); t != tokens.end(); ++t) {
            const char* v =(*t).c_str();
            int i = atoi(v);
            versions.push_back(i);
        }
        if (versions.size() < 2)
        {
            ostringstream oss;
            oss << "Format version must dotted -- ie, '1.0' or '1.2', not " << format_string;
            throw std::runtime_error(oss.str());
        }
        
        int minor = versions[1];
        if (minor > 2){
            ostringstream oss;
            oss << "Format version must dotted -- ie, '1.0' or '1.2', not " << format_string;
            throw std::runtime_error(oss.str());
        }
        header.SetVersionMinor(static_cast<boost::uint8_t>(minor)); 
    }
    if (vm.count("pad-header")) 
    {
        std::string header_pad = vm["pad-header"].as< string >();
        if (verbose)
            std::cout << "Increasing header pad to: " << header_pad << std::endl;

        boost::uint32_t offset = header.GetDataOffset();
        if (atoi(header_pad.c_str()) == 0) {
            ostringstream oss;
            oss << "Header pad was 0.  It must be greater than "<<offset<< " bytes";
            throw std::runtime_error(oss.str());
            
        }
        header.SetDataOffset(atoi(header_pad.c_str()));
    }

    if (vm.count("file-creation"))
    {
        std::vector<std::string> creation = vm["file-creation"].as< std::vector<std::string> >();

        if (verbose)
        {
            ostringstream oss;
            for (std::vector<std::string>::const_iterator i = creation.begin();
                 i != creation.end();
                 i++) 
                {
                    oss << *i << " ";
                }
                std::cout << "Setting file creation to " << oss.str() << std::endl;
        }
            
        std::string m("now");
        bool now = false;
        if (creation.size() == 1 ) 
        {
            if (!(creation[0].compare(m))) 
            {
                now = true;
            }            
        }
        
        boost::int32_t day = 0;
        boost::int32_t year = 0;
        
        
        if (creation.size() == 2) 
        {
            day = atoi(creation[0].c_str());
            year = atoi(creation[1].c_str());
            
            if (day < 0 || day > 366) {
                ostringstream oss;
                oss << "Day must be between 1-366, not " << day;
                throw std::runtime_error(oss.str());
            }
            if (year < 0)
            {
                ostringstream oss;
                oss << "Year must be greater than 0, not " << year;
                throw std::runtime_error(oss.str());
            }
            
        }
        
        if (now == true) 
        {
            liblas::Header h;
            header.SetCreationDOY(h.GetCreationDOY());
            header.SetCreationYear(h.GetCreationYear());
        } else {
            header.SetCreationDOY(static_cast<boost::uint16_t>(day));
            header.SetCreationYear(static_cast<boost::uint16_t>(year));
            
        }
    }

    if (vm.count("add-schema")) 
    {
        liblas::VariableRecord vlr = header.GetSchema().GetVLR();
        header.AddVLR(vlr);
    }

    if (vm.count("delete-vlr")) 
    {
        std::vector<std::string> vlrs = vm["delete-vlr"].as< std::vector<std::string> >();
        
        
        if (vlrs.size() % 2 != 0) {
            ostringstream err;
            err << "VLR descriptions must be in pairs of 2 -- A name and an ID";
            throw std::runtime_error(err.str());
        }
        ostringstream oss;
        
        for (std::vector<std::string>::const_iterator i = vlrs.begin();
             i != vlrs.end();
             i++) 
            {
                oss << *i << " ";
            }
        if (verbose)
        {

                std::cout << "Deleting VLRs with the values: " << oss.str() << std::endl;
        }
        
        for (std::vector<std::string>::size_type i = 0; i < vlrs.size(); i=i+2)
        {
            boost::int32_t id = atoi(vlrs[i+1].c_str());
            if (id < 0)
            {
                throw std::runtime_error("VLR ID must be > 0");
            }
            if (id > std::numeric_limits<boost::uint16_t>::max()) {
                ostringstream oss;
                oss << "ID must be less than "<< std::numeric_limits<boost::uint16_t>::max() <<", not " << id;
                throw std::runtime_error(oss.str());
            }
            header.DeleteVLRs(vlrs[i], static_cast<boost::uint16_t>(id));
        }
    }

    if (vm.count("add-vlr")) 
    {
        std::vector<std::string> vlrs = vm["add-vlr"].as< std::vector<std::string> >();
        
        
        if (vlrs.size() < 3) {
            ostringstream err;
            err << "VLR additions must be at least 3 arguments -- --add-vlr NAME 42 \"filename.ext\"";
            throw std::runtime_error(err.str());
        }
        if (vlrs.size() > 4)
            throw std::runtime_error("Only one VLR may be added at a time");

        ostringstream oss;
        
        for (std::vector<std::string>::const_iterator i = vlrs.begin();
             i != vlrs.end();
             i++) 
            {
                oss << *i << " ";
            }

        
        liblas::VariableRecord v;
        v.SetUserId(vlrs[0]);

        boost::int32_t id = atoi(vlrs[1].c_str());
        if (id < 0)
        {
            throw std::runtime_error("VLR ID must be > 0");
        }
        if (id > std::numeric_limits<boost::uint16_t>::max()) {
            ostringstream oss;
            oss << "ID must be less than "<< std::numeric_limits<boost::uint16_t>::max() <<", not " << id;
            throw std::runtime_error(oss.str());
        }

        v.SetRecordId(static_cast<boost::uint16_t>(id));
        
        std::vector<boost::uint8_t> data;
        
        std::string data_or_filename;
        if (vlrs.size() == 4){

            v.SetDescription(vlrs[2]);
            data_or_filename = vlrs[3];
        } else {
            data_or_filename = vlrs[2];
        } 

        try {
            std::vector<char> d;
            d = TryReadRawFileData(data_or_filename);
            for (std::vector<char>::const_iterator i = d.begin(); i != d.end(); ++i) 
            {
                data.push_back(*i);
            }
            
        } catch (std::runtime_error const& ) {
            std::string::const_iterator i;
            for (i = data_or_filename.begin(); i != data_or_filename.end(); ++i)
            {
                data.push_back(*i);
            }
        }    

        if (data.size() > std::numeric_limits<boost::uint16_t>::max()) {
            std::ostringstream oss;
            oss << "This VLR with length " << data.size() << " does" 
                << " not fit within the maximum VLR size of " 
                << std::numeric_limits<boost::uint16_t>::max();
            throw std::runtime_error(oss.str());
        }

        if (verbose)
        {

                std::cout << "Adding VLRs with the values: " << oss.str() << std::endl;
        }


        v.SetData(data);
        v.SetRecordLength(static_cast<boost::uint16_t>(data.size()));
        header.AddVLR(v);
    }

    if (vm.count("generating-software")) 
    {
        std::string software = vm["generating-software"].as< std::string >();
        if (verbose)
        {

                std::cout << "Setting Software ID to: " << software<< std::endl;
        }
        header.SetSoftwareId(software);
    }

    if (vm.count("system-identifier")) 
    {
        std::string id = vm["system-identifier"].as< std::string >();
        

        if (verbose)
        {

                std::cout << "Setting System ID to: " << id<< std::endl;
        }
        
        header.SetSystemId(id);
    }
    

    if (vm.count("a_srs")) 
    {
        liblas::SpatialReference in_ref;
        
        std::string input_srs = vm["a_srs"].as< string >();
        if (verbose)
            std::cout << "Setting input SRS to " << input_srs << std::endl;
        in_ref.SetFromUserInput(input_srs);
        header.SetSRS(in_ref);
    }

    if (vm.count("a_vertcs"))
    {
        liblas::SpatialReference vert_ref = header.GetSRS();

        std::vector<std::string> vertical_vec = vm["a_vertcs"].as< std::vector<std::string> >();
        if (vertical_vec.size() > 4) {
            ostringstream oss;
            oss << "Too many arguments were given to a_vertcs. "
                << "--a_vertcs verticalCSType citation verticalDatum verticalUnits  "
                << "All except verticalCSType are optional, but they are "
                << "applied in order, so if you want to set verticalUnits, "
                << "you must set all the others";

            throw std::runtime_error(oss.str());
        }
        if (vertical_vec.size() < 1) {
            ostringstream oss;
            oss << "At least verticalCSType must be given to a_vertcs. "
                << "--a_vertcs verticalCSType citation verticalDatum verticalUnits  "
                << "All except verticalCSType are optional, but they are "
                << "applied in order, so if you want to set verticalUnits, "
                << "you must set all the others";

            throw std::runtime_error(oss.str());
        }
        
        if (verbose)
        {
            ostringstream oss;
            for (std::vector<std::string>::const_iterator i = vertical_vec.begin();
                 i != vertical_vec.end();
                 i++) 
                {
                    oss << *i << " ";
                }
                std::cout << "Setting vertical info to: " << oss.str() << std::endl;
        }
            
        boost::int32_t verticalCSType = boost::lexical_cast<boost::int32_t>(vertical_vec[0]);
        
        std::string citation;
        int verticalDatum = -1;
        int verticalUnits = 9001;
        
        if (vertical_vec.size() > 1) {
            citation = boost::lexical_cast<std::string>(vertical_vec[1]);
        }
        
        if (vertical_vec.size() > 2) {
            verticalDatum = boost::lexical_cast<boost::int32_t>(vertical_vec[2]);
        }

        if (vertical_vec.size() > 3) {
            verticalUnits = boost::lexical_cast<boost::int32_t>(vertical_vec[3]);
        }
        
        vert_ref.SetVerticalCS(verticalCSType, citation, verticalDatum, verticalUnits);
        header.SetSRS(vert_ref);      
    }
    
    if (vm.count("t_srs")) 
    {
        liblas::SpatialReference in_ref;
        liblas::SpatialReference out_ref;
        
        std::string output_srs = vm["t_srs"].as< string >();
        if (verbose)
            std::cout << "Setting output SRS to " << output_srs << std::endl;
        out_ref.SetFromUserInput(output_srs);

        if (vm.count("a_srs")){
            std::string input_srs = vm["a_srs"].as< string >();
            in_ref.SetFromUserInput(input_srs);
        } else {
            // If the user didn't assign an input SRS, we'll try to take 
            // it from our existing header.
            in_ref = header.GetSRS();
            if (in_ref.GetVLRs().size() == 0)
            {
                throw std::runtime_error("No input SRS is available on the file you have specified.  Please use --a_srs to assign one");
            }
            
        }
        // Set the header's SRS to the output SRS now.  We've already 
        // made the transformation, and this SRS will be used to 
        // write the new file(s)
        header.SetSRS(out_ref);
        
        liblas::Bounds<double> b = header.GetExtent();
        b.project(in_ref, out_ref);
        header.SetExtent(b);
        liblas::TransformPtr srs_transform = liblas::TransformPtr(new liblas::ReprojectionTransform(in_ref, out_ref, liblas::HeaderPtr(new liblas::Header(header))));
        transforms.push_back(srs_transform);
    }

    if (vm.count("point-translate")) 
    {
        std::string translate = vm["point-translate"].as< std::string >();
        if (verbose)
        {

                std::cout << "Translating points with expression: " << translate << std::endl;
        }
        liblas::TransformPtr trans_trans = liblas::TransformPtr(new liblas::TranslationTransform(translate));
        transforms.push_back(trans_trans);
    }
    return transforms;
}


