// $Id$
//
// (C) Copyright Mateusz Loskot 2008, mateusz@loskot.net
// Distributed under the BSD License
// (See accompanying file LICENSE.txt or copy at
// http://www.opensource.org/licenses/bsd-license.php)
//
#if defined(_MSC_VER) && defined(USE_VLD)
#include <vld.h>
#endif
// liblas
#include <liblas/liblas.hpp>
#include <liblas/laspoint.hpp>
#include <liblas/lasfile.hpp>
//std
#include <algorithm>
#include <exception>
#include <list>
#include <iostream>
#include <cassert>

// Reports object of LASFile
void print_file(liblas::LASFile& f)
{
    if (f.IsNull())
    {
        std::cout << "{ null file }\n";
    }
    else
    {
        std::cout << "{ name: " << f.GetName() << "; mode: " << f.GetMode() << "; signature: ";

        if (f.GetMode() == liblas::LASFile::eRead)
            std::cout << f.GetReader().GetHeader().GetFileSignature();
        else
            std::cout << f.GetWriter().GetHeader().GetFileSignature();

        std::cout << " }\n";
    }
}

int main()
{
    using liblas::LASFile;

    try
    {
        {
            LASFile f;
            {
                LASFile f0;
                LASFile f1("test2.las", liblas::LASHeader(), liblas::LASFile::eWrite);
                LASFile f2("test.las"); // throws if file missing

                std::list<LASFile> files;
                files.push_back(f0);
                files.push_back(f1);
                files.push_back(f2);

                std::for_each(files.begin(), files.end(), print_file);

                f = f1; // save

                liblas::LASPoint p;
                p.SetCoordinates(10, 20, 30);
                f.GetWriter().WritePoint(p);

                files.clear(); // get rid of all but f1 (now f)
            }

            print_file(f); // f (previously f1) is still alive
        
        } // f goes to hell and header gots updated here
    }
    catch (std::exception const& e)
    {
        std::cout << "Error: " << e.what() << std::endl;
    }
    catch (...)
    {
        std::cerr << "Unknown error\n";
    }

    return 0;
}
