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



#include "itkMacro.h"

#include <fstream>
#include <iostream>
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageMetadataInterfaceBase.h"
#include "otbImageMetadataInterfaceFactory.h"

int otbImageMetadataInterfaceTest2(int itkNotUsed(argc), char* argv[])
{
  // Verify the number of parameters in the command line
  const char * inputFilename  = argv[1];
  const char * outputFilename  = argv[2];

  typedef otb::Image<double,  2>               InputImageType;
  typedef otb::ImageFileReader<InputImageType> ImageReaderType;

  ImageReaderType::Pointer reader = ImageReaderType::New();
  reader->SetFileName(inputFilename);
  reader->UpdateOutputInformation();

  otb::ImageMetadataInterfaceBase::Pointer lImageMetadata = otb::ImageMetadataInterfaceFactory::CreateIMI(
    reader->GetOutput()->GetMetaDataDictionary());

  std::ofstream file;
  file.open(outputFilename);

  //Test the image interface
  lImageMetadata->SetImage(reader->GetOutput());
  file << lImageMetadata->GetProjectionRef() << std::endl;
  file.close();

  return EXIT_SUCCESS;

}
