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
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbOpticalImageMetadataInterfaceFactory.h"

int otbOpticalImageMetadataInterfaceTest(int itkNotUsed(argc), char* argv[])
{
  // Verify the number of parameters in the command line
  const char * inputFilename  = argv[1];
  const char * outputFilename  = argv[2];

  typedef otb::VectorImage<double,  2>         InputImageType;
  typedef otb::ImageFileReader<InputImageType> ImageReaderType;

  ImageReaderType::Pointer reader = ImageReaderType::New();
  reader->SetFileName(inputFilename);
  reader->UpdateOutputInformation();

  otb::OpticalImageMetadataInterface::Pointer lImageMetadata = otb::OpticalImageMetadataInterfaceFactory::CreateIMI(
    reader->GetOutput()->GetMetaDataDictionary());

  std::ofstream file;
  file.open(outputFilename);
  file << "GetSunElevation:     " << lImageMetadata->GetSunElevation() << std::endl;
  file << "GetSunAzimuth:       " << lImageMetadata->GetSunAzimuth() << std::endl;
  file << "GetSatElevation:     " << lImageMetadata->GetSatElevation() << std::endl;
  file << "GetSatAzimuth:       " << lImageMetadata->GetSatAzimuth() << std::endl;
  file << "GetPhysicalBias:     " << lImageMetadata->GetPhysicalBias() << std::endl;
  file << "GetPhysicalGain:     " << lImageMetadata->GetPhysicalGain() << std::endl;
  file << "GetSolarIrradiance:  " << lImageMetadata->GetSolarIrradiance() << std::endl;
  file << "GetFirstWavelengths: " << lImageMetadata->GetFirstWavelengths() << std::endl;
  file << "GetLastWavelengths:  " << lImageMetadata->GetLastWavelengths() << std::endl;
  file << "GetSpectralSensitivity:  " << lImageMetadata->GetSpectralSensitivity() << std::endl;
  file.close();

  std::cout << lImageMetadata << std::endl;

  return EXIT_SUCCESS;

}
