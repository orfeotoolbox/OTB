/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */



#include <iostream>
#include "itkMacro.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbSarImageMetadataInterfaceFactory.h"

int otbSarCalibrationLookupDataTest(int argc, char* argv[])
{
  typedef double                                  RealType;
  typedef otb::SarImageMetadataInterface             ImageMetadataInterfaceType;
  typedef otb::SarCalibrationLookupData              LookupDataType;
  typedef otb::Image<double,  2>                     InputImageType;
  typedef otb::ImageFileReader<InputImageType>       ImageReaderType;

  if (argc < 3 )
    {
    std::cerr << "Usage: otbSarCalibationLookupDataTest /path/to/input/file /path/to/output/file  !"<< std::endl;
    return EXIT_FAILURE;
    }
  ImageReaderType::Pointer reader = ImageReaderType::New();
  reader->SetFileName(argv[1]);
  reader->UpdateOutputInformation();

  const char *  outFileName = argv[2];
  std::ofstream outfile;
  outfile.open(outFileName);

  ImageMetadataInterfaceType::Pointer imageMetadataInterface =
    otb::SarImageMetadataInterfaceFactory::CreateIMI( reader->GetOutput()->GetMetaDataDictionary() );

  if (!imageMetadataInterface.IsNotNull())
    {
    std::cerr << "cannot create a otb::SarImageMetadataInterface for input image." << std::endl;
    return EXIT_FAILURE;
    }

  const std::string sensorId = imageMetadataInterface->GetSensorID();

  outfile << sensorId << std::endl;

  LookupDataType::Pointer lookupDataObj = imageMetadataInterface->GetCalibrationLookupData(0);

  if (!lookupDataObj.IsNotNull())
    {
    std::cerr << "lookupDataObj is Null"<< std::endl;
    return EXIT_FAILURE;
    }

  RealType lutVal = static_cast<RealType>(lookupDataObj->GetValue(10,19));

  outfile << imageMetadataInterface->HasCalibrationLookupDataFlag() << std::endl;
  outfile << lutVal << std::endl;

  return EXIT_SUCCESS;

}
