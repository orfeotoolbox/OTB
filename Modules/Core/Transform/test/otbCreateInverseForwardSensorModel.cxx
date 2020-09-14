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

#include "otbRPCInverseTransform.h"
#include "otbRPCForwardTransform.h"
#include "otbMetaDataKey.h"

int otbCreateInverseForwardSensorModel(int argc, char* argv[])
{
  if (argc != 2)
  {
    std::cout << argv[0] << " <input filename>" << std::endl;
    return EXIT_FAILURE;
  }

  typedef otb::Image<unsigned int, 2> ImageType;
  typedef otb::ImageFileReader<ImageType> ReaderType;
  typedef otb::RPCInverseTransform<double> InverseRPCModelType;
  typedef otb::RPCForwardTransform<double> ForwardRPCModelType;

  // Allocate pointer
  InverseRPCModelType::Pointer inverse_rpc_model = InverseRPCModelType::New();
  ForwardRPCModelType::Pointer forward_rpc_model = ForwardRPCModelType::New();
  ReaderType::Pointer          reader            = ReaderType::New();

  // Set parameters ...
  reader->SetFileName(argv[1]);

  // Read metadata
  reader->GenerateOutputInformation();
  ImageType::Pointer inputImage = reader->GetOutput();

  otbGenericMsgDebugMacro(<< "Inverse model creation...");
  if (!inverse_rpc_model->SetMetadata(inputImage->GetImageMetadata()))
  {
    std::cout << "Error while reading model. This is not a RPC model!" << std::endl;
    return EXIT_FAILURE;
  }
  if (!inverse_rpc_model->IsValidSensorModel())
  {
    std::cout << "Model not set!" << std::endl;
    return EXIT_FAILURE;
  }

  otbGenericMsgDebugMacro(<< "Forward model creation...");
  if (!forward_rpc_model->SetMetadata(inputImage->GetImageMetadata()))
  {
    std::cout << "Error while reading model. This is not a RPC model!" << std::endl;
    return EXIT_FAILURE;
  }
  if (!forward_rpc_model->IsValidSensorModel())
  {
    std::cout << "Model not set!" << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
