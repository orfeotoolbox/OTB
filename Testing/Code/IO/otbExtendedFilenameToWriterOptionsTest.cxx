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
#include "otbExtendedFilenameToWriterOptions.h"
#include <iostream>
#include <fstream>

typedef otb::ExtendedFilenameToWriterOptions FilenameHelperType;

int otbExtendedFilenameToWriterOptions(int itkNotUsed(argc), char* argv[])
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

  file << helper->WriteGEOMFileIsSet() << std::endl;
  file << helper->GetWriteGEOMFile() << std::endl;

  file << helper->gdalCreationOptionsIsSet() << std::endl;
  if (helper->gdalCreationOptionsIsSet())
    for (unsigned int i=0; i<helper->GetgdalCreationOptions().size(); i++)
    {
    file << helper->GetgdalCreationOptions()[i] << std::endl;
    }

  return EXIT_SUCCESS;
}
