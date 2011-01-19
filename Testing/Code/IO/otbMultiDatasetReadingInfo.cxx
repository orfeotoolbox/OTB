/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "itkExceptionObject.h"
#include <iostream>
#include <fstream>
#include <string>

#include "otbGDALImageIO.h"

int otbMultiDatasetReadingInfo(int argc, char* argv[])
{
	if (argc != 3)
    {
		std::cout << argv[0] << "<image HDF> <ouput filename>" << std::endl;
		return EXIT_FAILURE;
    }
	const char * inputFilename  = argv[1];
	const char * outputAsciiFilename  = argv[2];

	otb::GDALImageIO::Pointer readerGDAL = otb::GDALImageIO::New();

	readerGDAL->SetFileName(inputFilename);
	if (readerGDAL->CanReadFile(inputFilename))
		std::cout << "can read file OK" << std::endl;
	else
	{
		std::cout << "can read file K0" << std::endl;
		return EXIT_FAILURE;
	}

	std::vector<std::string> names;
	std::vector<std::string> desc;

	bool readingSubDatasetInfo = readerGDAL->GetSubDatasetInfo(names, desc);
	if (readingSubDatasetInfo == false )
		return EXIT_FAILURE;

	std::ofstream file;
	file.open(outputAsciiFilename);

	for( unsigned int itSubDataset = 0; itSubDataset < names.size() ; itSubDataset++ )
	{
		file << "NAME_"<< itSubDataset+1 << " :" <<  names[itSubDataset] <<std::endl;
		//std::cout << "NAME_"<< itSubDataset+1 << " :" <<  names[itSubDataset] <<std::endl;
		file << "DESC_" << itSubDataset+1 << " :" << desc[itSubDataset] <<std::endl;
		//std::cout << "DESC_"<< itSubDataset+1 << " :" <<  desc[itSubDataset] <<std::endl;
	}

	file.close();

	return EXIT_SUCCESS;
}
