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
#include "otbImageFileReader.h"
#include "otbVectorImage.h"


int otbMultiResolutionReadingInfo(int argc, char* argv[])
{
  if (argc != 3)
    {
    std::cout << argv[0] << "<input filename> <output filename>" << std::endl;
    return EXIT_FAILURE;
    }
  const char * inputFilename  = argv[1];
  const char * outputAsciiFilename  = argv[2];

  typedef double PixelType;
  const unsigned int Dimension = 2;
  typedef otb::VectorImage<PixelType, Dimension> ImageType;

  typedef otb::ImageFileReader<ImageType> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();

  reader->SetFileName(inputFilename);
  reader->UpdateOutputInformation();

  
  std::vector<std::string> desc = reader->GetOverviewsInfo();
  if (desc.empty())
    return EXIT_FAILURE;

  std::ofstream file;
  file.open(outputAsciiFilename);

  // Parse all the names to delete the path of the filename
  for( unsigned int itRes = 0; itRes < reader->GetOverviewsCount(); itRes++ )
    {
    file << "RESOLUTION: " << itRes << "; " << "DESC: " << desc[itRes] << std::endl;
    }

  file.close();

  return EXIT_SUCCESS;
}
