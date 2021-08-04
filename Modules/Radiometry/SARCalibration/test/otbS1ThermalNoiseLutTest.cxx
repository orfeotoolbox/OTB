/*
 * Copyright (C) 2005-2021 Centre National d'Etudes Spatiales (CNES)
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
#include "otbS1ThermalNoiseLookupData.h"

int otbS1ThermalNoiseLutTest(int argc, char* argv[])
{
  typedef double                         RealType;
  typedef otb::SarImageMetadataInterface ImageMetadataInterfaceType;
  typedef otb::SarCalibrationLookupData  LookupDataType;
  typedef otb::Image<double, 2> InputImageType;
  typedef otb::ImageFileReader<InputImageType> ImageReaderType;

  const double tol = 1e-10;

  using S1ThermalNoiseLookupType = otb::S1ThermalNoiseLookupData<double>;
  using S1ThermalNoiseLookupPointerType = typename S1ThermalNoiseLookupType::Pointer;


  if (argc < 3)
  {
    std::cerr << "Usage: otbS1ThermalNoiseLutTest /path/to/input/file lutValue thermalNoise" << std::endl;
    return EXIT_FAILURE;
  }
  ImageReaderType::Pointer reader = ImageReaderType::New();
  reader->SetFileName(argv[1]);
  reader->UpdateOutputInformation();

  ImageMetadataInterfaceType::Pointer imageMetadataInterface = otb::SarImageMetadataInterfaceFactory::CreateIMI(reader->GetOutput()->GetMetaDataDictionary());

  if (!imageMetadataInterface.IsNotNull())
  {
    std::cerr << "cannot create a otb::SarImageMetadataInterface for input image." << std::endl;
    return EXIT_FAILURE;
  }

  const std::string sensorId = imageMetadataInterface->GetSensorID();

  LookupDataType::Pointer lookupDataObj = imageMetadataInterface->GetCalibrationLookupData(0);

  if (!lookupDataObj.IsNotNull())
  {
    std::cerr << "lookupDataObj is Null" << std::endl;
    return EXIT_FAILURE;
  }

  const unsigned int idx1 = 396, idx2 = 333;

  auto lutVal = static_cast<RealType>(lookupDataObj->GetValue(idx1, idx2));
  const auto lutValBaseline = static_cast<RealType>(std::stod(argv[2]));

  if (std::abs(lutVal - lutValBaseline) > tol)
  {
    std::cerr << "LUT value value at [" << idx1 << ", " << idx2 
              << "]: " << lutVal 
              <<  "does not match the baseline: " << lutValBaseline << std::endl;
    return EXIT_FAILURE;
  }

  S1ThermalNoiseLookupPointerType S1ThermaNoise = S1ThermalNoiseLookupType::New();
  S1ThermaNoise->SetImageKeywordlist(reader->GetOutput()->GetImageKeywordlist());

  auto thermalNoise = static_cast<RealType>(S1ThermaNoise->GetValue(idx1, idx2));
  const auto thermalNoiseBaseline = static_cast<RealType>(std::stod(argv[3]));

  if (std::abs(thermalNoise - thermalNoiseBaseline) > tol)
  {
    std::cerr << "Computed thermal noise value at [" << idx1 << ", " << idx2 
              << "]: " << thermalNoise 
              <<  "does not match the baseline: " << thermalNoiseBaseline << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
