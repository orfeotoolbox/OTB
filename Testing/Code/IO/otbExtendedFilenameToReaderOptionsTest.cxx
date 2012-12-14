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

  file << helper->SimpleFileNameIsSet() << std::endl;
  file << helper->GetSimpleFileName() << std::endl;

  file << helper->ExtGEOMFileNameIsSet() << std::endl;
  file << helper->GetExtGEOMFileName() << std::endl;

  file << helper->SubDatasetIndexIsSet() << std::endl;
  file << helper->GetSubDatasetIndex() << std::endl;

  file << helper->ResolutionFactorIsSet() << std::endl;
  file << helper->GetResolutionFactor() << std::endl;

  file << helper->SkipCartoIsSet() << std::endl;
  file << helper->GetSkipCarto() << std::endl;

  return EXIT_SUCCESS;
}
