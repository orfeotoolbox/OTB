/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

// iostream is used for general output
#include <iostream>
#include <iterator>
#include <stdlib.h>

#include "otbMacro.h"

#include "otbImage.h"
#include "otbImageFileReader.h"

#include "otbInverseSensorModel.h"
#include "otbForwardSensorModel.h"
#include "otbDEMHandler.h"

int otbCreateInverseForwardSensorModel(int argc, char* argv[])
{

  std::string InputFilename, OutputFilename, pointX, pointY;
  switch (argc)
  {
  case 6 :
    otb::DEMHandler::GetInstance().OpenDEMFile(argv[5]);
  case 5 :
    InputFilename = argv[1];
    OutputFilename = argv[2];
    pointX = argv[3];
    pointY = argv[4];
    break;
  default :
    std::cout << argv[0] << " <input filename> <output filename> <test_point_X> <test_point_Y> [optional DEM path]\n";
    return EXIT_FAILURE;
  }

  typedef otb::Image<unsigned int, 2> ImageType;
  typedef otb::ImageFileReader<ImageType> ReaderType;
  typedef otb::InverseSensorModel<double> InverseModelType;
  typedef otb::ForwardSensorModel<double> ForwardModelType;

  // Allocate pointer
  InverseModelType::Pointer inverse_model = InverseModelType::New();
  ForwardModelType::Pointer forward_model = ForwardModelType::New();
  ReaderType::Pointer       reader        = ReaderType::New();

  // Set parameters ...
  reader->SetFileName(InputFilename);

  // Read meta data (ossimKeywordlist)
  reader->GenerateOutputInformation();
  ImageType::Pointer inputImage = reader->GetOutput();

  otbGenericMsgDebugMacro(<< "Inverse model creation...");
  inverse_model->SetImageGeometry(inputImage->GetImageKeywordlist());
  if (inverse_model->IsValidSensorModel() == false)
  {
    std::cout << "Invalid Model pointer m_Model == NULL!\n The ossim keywordlist is invalid!" << std::endl;
    return EXIT_FAILURE;
  }

  otbGenericMsgDebugMacro(<< "Forward model creation...");
  forward_model->SetImageGeometry(inputImage->GetImageKeywordlist());
  if (forward_model->IsValidSensorModel() == false)
  {
    std::cout << "Invalid Model pointer m_Model == NULL!\n The ossim keywordlist is invalid!" << std::endl;
    return EXIT_FAILURE;
  }

  std::ofstream ofs(OutputFilename, std::ofstream::out);
  ofs.precision(8);
  
  InverseModelType::InputPointType geoPoint;
  geoPoint[0] = atof(pointX.c_str());
  geoPoint[1] = atof(pointY.c_str());

  ofs << "Testing geopoint: " << geoPoint << "\n\n";

  auto indexPoint = inverse_model->TransformPoint(geoPoint);
  ofs << "Testing InverseSensorModel: " << geoPoint << " -> " << indexPoint << "\n";

  auto newGeoPoint = forward_model->TransformPoint(indexPoint);
  ofs << "Testing ForwardSensorModel: " << indexPoint << " -> " << newGeoPoint << "\n";
  
  return EXIT_SUCCESS;
}
