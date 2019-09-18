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


/*!
 *
 * PURPOSE:
 *
 * Application to rproject an image region into gepgraphical coordinates
 * usinf un Interpolator+regionextractor and an Iterator.
 *
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

int otbCreateInverseForwardSensorModel(int argc, char* argv[])
{
  if (argc != 2)
  {
    std::cout << argv[0] << " <input filename>" << std::endl;
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
  reader->SetFileName(argv[1]);

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

  return EXIT_SUCCESS;
}
