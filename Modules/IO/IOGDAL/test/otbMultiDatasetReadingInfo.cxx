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
#include <iostream>
#include <fstream>
#include <string>

#include "itkMacro.h"
#include "itksys/SystemTools.hxx"
#include "otbGDALImageIO.h"

int otbMultiDatasetReadingInfo(int argc, char* argv[])
{
  if (argc != 3)
    {
    std::cout << argv[0] << "<image HDF> <output filename>" << std::endl;
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

  // Parse all the names to delete the path of the filename
  for( unsigned int itSubDataset = 0; itSubDataset < (unsigned int)names.size(); itSubDataset++ )
    {
    std::size_t posDeb = names[itSubDataset].find(":\"");
    if (posDeb==std::string::npos)
      return EXIT_FAILURE;

    std::size_t posEnd = names[itSubDataset].rfind("\":");
    if (posEnd==std::string::npos)
      return EXIT_FAILURE;

    // Remove the path from the SubDataset_name metadata
    std::string strFilepath = names[itSubDataset].substr(posDeb+2, posEnd - posDeb - 2);
    std::string filename = itksys::SystemTools::GetFilenameName(strFilepath);
    names[itSubDataset].replace(posDeb+2, posEnd - posDeb - 2, filename.c_str() );
    }

  std::ofstream file;
  file.open(outputAsciiFilename);

  for( unsigned int itSubDataset = 0; itSubDataset < (unsigned int)names.size(); itSubDataset++ )
    {
    file << "NAME_" << itSubDataset+1 << " :" << names[itSubDataset] << std::endl;
    file << "DESC_" << itSubDataset+1 << " :" << desc[itSubDataset] << std::endl;
    }

  file.close();

  return EXIT_SUCCESS;
}
