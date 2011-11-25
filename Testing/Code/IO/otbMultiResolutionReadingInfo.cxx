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
#include "otbJPEG2000ImageIO.h"

int otbMultiResolutionReadingInfo(int argc, char* argv[])
{
  if (argc != 3)
    {
    std::cout << argv[0] << "<image JPEG2000> <ouput filename>" << std::endl;
    return EXIT_FAILURE;
    }
  const char * inputFilename  = argv[1];
  const char * outputAsciiFilename  = argv[2];

  otb::JPEG2000ImageIO::Pointer readerJPEG2000 = otb::JPEG2000ImageIO::New();

  readerJPEG2000->SetFileName(inputFilename);
  if (readerJPEG2000->CanReadFile(inputFilename))
    std::cout << "can read file OK" << std::endl;
  else
    {
    std::cout << "can read file K0" << std::endl;
    return EXIT_FAILURE;
    }

  std::vector<unsigned int> res;
  std::vector<std::string> desc;

  bool readingResolutionInfo = readerJPEG2000->GetResolutionInfo(res, desc);
  if (readingResolutionInfo == false )
    return EXIT_FAILURE;

  std::ofstream file;
  file.open(outputAsciiFilename);

  // Parse all the names to delete the path of the filename
  for( unsigned int itRes = 0; itRes < (unsigned int)res.size(); itRes++ )
    {
    file << "RESOLUTION: " << res[itRes] << "; " << "DESC: " << desc[itRes] << std::endl;
    }

  file.close();

  return EXIT_SUCCESS;
}
