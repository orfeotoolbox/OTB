#include <fstream>
#include <iostream>

#include <sstream>
#include <cstdlib>
#include <cstdio>

using namespace std;

//----------------- if internal libtiff/libgeotiff are detected, symbol renaming is mandatory -------------------------
int main(int argc, char * argv[])
{

  const char * inputFilename  = argv[1];

  string pattern1("TIFFClose");
  string pattern2("gdal_");
  ifstream file(inputFilename, ios::in);
  if(file)
  {
	string line;
	while(getline(file, line))
	{
		if (line.find(pattern1) !=  string::npos)
			if (line.find(pattern2) == string::npos)
				{
        cout << "Internal versions of libtiff/libgeotiff detected without symbol renaming!" << std::endl;
        cout << "When configuring GDAL, if options --with-libtiff or --with-geotiff are set to 'internal', then options --with-rename-internal-libtiff-symbols and --with-rename-internal-libgeotiff-symbols should be set to 'yes')" << std::endl;
					return EXIT_FAILURE;
				}

	}
	file.close();
   }


  return EXIT_SUCCESS;

}
