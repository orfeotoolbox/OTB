/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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


#include <iomanip>

#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbGeometricSarSensorModelAdapter.h"

int otbGeometricSarSensorModelAdapterTest(int argc, char* argv[])
{
 if (argc != 3)
    {
    std::cerr << "Usage: " << argv[0] << " imageFile output" << std::endl;
    return EXIT_FAILURE;
    }

  typedef std::complex<double> PixelType;
  typedef otb::Image<PixelType,2> ImageType;

  typedef otb::ImageFileReader<ImageType> ReaderType;

  ReaderType::Pointer image = ReaderType::New();

  image->SetFileName(argv[1]);

  image->UpdateOutputInformation();

  typedef otb::GeometricSarSensorModelAdapter SarSensorModelType;
  SarSensorModelType::Pointer imageSarModel = SarSensorModelType::New();

  imageSarModel->CreateSensorModel(image->GetOutput()->GetImageKeywordlist());

  std::vector<double> imagePosition;
  std::vector<double> imageSpeed;
  imagePosition.resize(3);
  imageSpeed.resize(3);


  imageSarModel->GetPlatformPositionAtLine(0, imagePosition, imageSpeed);

  std::ofstream file;
  file.open(argv[2]);

  file << std::setprecision(15);

  file << "Image:\n";
  //file << "Start Time: " << masterSarModel->getTime(0)<< " m\n";
  file << "Pos X: " << imagePosition[0] << " m\n";
  file << "Pos Y: " << imagePosition[1] << " m\n";
  file << "Pos Z: " << imagePosition[2] << " m\n";
  file << "Spd X: " << imageSpeed[0] << " m/s\n";
  file << "Spd Y: " << imageSpeed[1] << " m/s\n";
  file << "Spd Z: " << imageSpeed[2] << " m/s\n";
  file.close();

  return EXIT_SUCCESS;
}
