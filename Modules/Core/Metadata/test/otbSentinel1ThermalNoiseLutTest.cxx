/*
 * Copyright (C) 2005-2022 Centre National d'Etudes Spatiales (CNES)
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
#include "otbSentinel1ThermalNoiseLookupData.h"

int otbSentinel1ThermalNoiseLutTest(int argc, char* argv[])
{
  using RealType = double;
  using InputImageType = otb::Image<double, 2>;
  using ImageReaderType = otb::ImageFileReader<InputImageType>;

  const double tol = 1e-10;

  if (argc < 3)
  {
    std::cerr << "Usage: otbS1ThermalNoiseLutTest /path/to/input/file lutValue thermalNoise" << std::endl;
    return EXIT_FAILURE;
  }

  ImageReaderType::Pointer reader = ImageReaderType::New();
  reader->SetFileName(argv[1]);
  reader->UpdateOutputInformation();
  const auto & imd = reader->GetOutput()->GetImageMetadata();

  /** Fetch the SARCalib */
  std::unique_ptr<otb::SARCalib> sarCalibPtr;
  if (imd.Has(otb::MDGeom::SARCalib))
  {
    sarCalibPtr = std::make_unique<otb::SARCalib>(boost::any_cast<otb::SARCalib>(imd[otb::MDGeom::SARCalib]));
  }
  else
  {
    std::cerr <<  "Unable to fetch the SARCalib metadata from the input product.";
  }

  auto lut = sarCalibPtr->calibrationLookupData[otb::SarCalibrationLookupData::SIGMA];

  const unsigned int idx1 = 396, idx2 = 333;

  auto lutVal = static_cast<RealType>(lut->GetValue(idx1, idx2));
  const auto lutValBaseline = static_cast<RealType>(std::stod(argv[2]));

  if (std::abs(lutVal - lutValBaseline) > tol)
  {
    std::cerr << "LUT value value at [" << idx1 << ", " << idx2 
              << "]: " << lutVal 
              <<  "does not match the baseline: " << lutValBaseline << std::endl;
    return EXIT_FAILURE;
  }

  auto thermalNoiseLut = sarCalibPtr->calibrationLookupData[otb::SarCalibrationLookupData::NOISE];

  auto thermalNoise = static_cast<RealType>(thermalNoiseLut->GetValue(idx1, idx2));
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
