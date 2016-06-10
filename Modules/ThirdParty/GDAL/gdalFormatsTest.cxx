#include <iostream>
#include <fstream>
#include <stdexcept>
#include <gdal.h>
#include <gdal_priv.h>

//http://www.gdal.org/formats_list.html

using namespace std;

int main(int argc, char * argv[])
{
	string driverName(argv[1]);
	GDALAllRegister();

	GDALDriverH drv = NULL;
	unsigned int count = GDALGetDriverCount();

	// Remember gdal formats
	if (argc == 3) // ie. ./exe driverName fileName
	{
		const char * inputFilename  = argv[2];
		ofstream file(inputFilename, ios::out | ios::trunc);
		if(file)
		  {
			file << "###############################" << std::endl;
			file << "# ID / Short name / Long name #" << std::endl;
			file << "###############################" << std::endl;
			file << std::endl;
			file << "# Nb of drivers : " << count << std::endl;
			file << std::endl;

			for (unsigned int i = 0; i < count; i++)
			{
				drv = GDALGetDriver(i);
				file << "i = " << i << " / " << GDALGetDriverShortName(drv) << " / " << GDALGetDriverLongName(drv) << std::endl;
			}

			file.close();
		  }
	}

	//std::cout << GDALGetDriverShortName(GDALGetDriverByName("GTiff")) << std::endl;

	//Driver check
	for (unsigned int i = 0; i < count; i++)
	{
		drv = GDALGetDriver(i);
		if (string(GDALGetDriverShortName(drv)).compare(driverName) == 0 )
			return EXIT_SUCCESS; // SUCCESS

	}



    return EXIT_FAILURE; // FAIL

}
