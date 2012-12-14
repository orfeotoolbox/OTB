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
#include "otbExtendedFilenameToReaderOptions.h"
#include <iostream>
#include <fstream>

typedef otb::ExtendedFilenameToReaderOptions FilenameHelperType;

int otbExtendedFilenameToReaderOptions(int argc, char* argv[])
{
  // Verify the number of parameters in the command line
  const char * inputExtendedFilename  = argv[1];
  const char * outputFilename = argv[2];

  FilenameHelperType::Pointer helper = FilenameHelperType::New();

  helper->SetExtendedFileName(inputExtendedFilename);

  std::ofstream file;
  file.open(outputFilename);

  std::cout << helper->SimpleFileNameIsSet() << std::endl;
  std::cout << helper->GetSimpleFileName() << std::endl;

  std::cout << helper->ExtGEOMFileNameIsSet() << std::endl;
  std::cout << helper->GetExtGEOMFileName() << std::endl;

  std::cout << helper->SubDatasetIndexIsSet() << std::endl;
  std::cout << helper->GetSubDatasetIndex() << std::endl;

  std::cout << helper->ResolutionFactorIsSet() << std::endl;
  std::cout << helper->GetResolutionFactor() << std::endl;

  std::cout << helper->SkipCartoIsSet() << std::endl;
  std::cout << helper->GetSkipCarto() << std::endl;

  return EXIT_SUCCESS;
}
