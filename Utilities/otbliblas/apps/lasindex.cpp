#include <liblas/liblas.hpp>

#include <cstring>
#include <iterator>
#include <iterator>
#include <fstream>
#include <vector>
#include <sstream>

#ifdef _WIN32
#define compare_no_case(a,b,n)  _strnicmp( (a), (b), (n) )
#else
#define compare_no_case(a,b,n)  strncasecmp( (a), (b), (n) )
#endif

std::istream* OpenInput(std::string filename) 
{
    std::ios::openmode const mode = std::ios::in | std::ios::binary;
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

void usage() {}

using namespace liblas;

void LoadIndex (LASIndex* index, liblas::Reader* reader, long dimension) 
{

    bool read = reader->ReadNextPoint();
    liblas::int64_t id = 0;
    if (read) {
        const liblas::Point& p = reader->GetPoint();
        index->insert(const_cast<liblas::Point&>(p), id);
    }
    while (reader->ReadNextPoint()) {
        id += 1;
        const liblas::Point& p = reader->GetPoint();
        index->insert(const_cast<liblas::Point&>(p), id);
    }

	boost::ignore_unused_variable_warning(dimension);
}
int main(int argc, char* argv[])
{
    std::string input;
    
    long dimension = 3;
    long capacity = 10000;
    bool bBulkLoad = true;
    
    for (int i = 1; i < argc; i++)
    {
        if (    std::strcmp(argv[i],"-h") == 0 ||
                std::strcmp(argv[i],"--help") == 0
            )
        {
            usage();
            exit(0);
        }
        else if (   std::strcmp(argv[i],"--input") == 0  ||
                    std::strcmp(argv[i],"-input") == 0   ||
                    std::strcmp(argv[i],"-i") == 0       ||
                    std::strcmp(argv[i],"-in") == 0
                )
        {
            i++;
            input = std::string(argv[i]);
        }
        else if (   std::strcmp(argv[i],"--dimension") == 0  ||
                    std::strcmp(argv[i],"-dim") == 0     ||
                    std::strcmp(argv[i],"-d") == 0       
                )
        {
            i++;
            dimension = atoi(argv[i]);
        }
        
        else if (   std::strcmp(argv[i],"--capacity") == 0  ||
                    std::strcmp(argv[i],"-cap") == 0     ||
                    std::strcmp(argv[i],"-c") == 0       
                )
        {
            i++;
            capacity = atoi(argv[i]);
        }
        else if (   std::strcmp(argv[i],"--individual") == 0  ||
                    std::strcmp(argv[i],"--non-bulk") == 0     ||
                    std::strcmp(argv[i],"-g") == 0       
                )
        {

            bBulkLoad = false;
        }
        else if (input.empty())
        {
            input = std::string(argv[i]);
        }

        else 
        {
            usage();
            exit(1);
        }
    }
    
    if (input.empty()) {
        usage();
        exit(-1);
    }

    // FIXME: Missing RAII and try-catch, no LAS throws exceptions.
    std::istream* istrm = OpenInput(input);
    liblas::Reader* reader = new liblas::Reader(*istrm);
    std::cout << "Indexing " << input<< " "<<std::endl;

    LASIndexDataStream* idxstrm = 0;
    
    LASIndex* idx = new LASIndex(input);
    idx->SetType(LASIndex::eExternalIndex);
    idx->SetLeafCapacity(capacity);
    idx->SetFillFactor(0.99);
    idx->SetDimension(dimension);

    if (bBulkLoad) {
        idxstrm = new LASIndexDataStream(reader, dimension);
        idx->Initialize(*idxstrm);
    } else {
        idx->Initialize();
        LoadIndex(idx, reader, dimension);
    }

    if (idx)
        delete idx;
    
    if (idxstrm)
        delete idxstrm;
    if (reader)
        delete reader;
    
    delete istrm;
}
