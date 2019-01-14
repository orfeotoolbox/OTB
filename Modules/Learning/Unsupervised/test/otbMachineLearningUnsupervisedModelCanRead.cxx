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

#include <otbConfigure.h>
#include <otbMachineLearningModel.h>

typedef otb::MachineLearningModel<float,short>         MachineLearningModelType;
typedef MachineLearningModelType::InputValueType       InputValueType;
typedef MachineLearningModelType::InputSampleType      InputSampleType;
typedef MachineLearningModelType::InputListSampleType  InputListSampleType;
typedef MachineLearningModelType::TargetValueType      TargetValueType;
typedef MachineLearningModelType::TargetSampleType     TargetSampleType;
typedef MachineLearningModelType::TargetListSampleType TargetListSampleType;

#ifdef OTB_USE_SHARK

#include "otbSharkKMeansMachineLearningModel.h"

int otbSharkKMeansMachineLearningModelCanRead(int argc, char *argv[])
{
  if( argc != 2 )
    {
    std::cerr << "Usage: " << argv[0] << "<model>" << std::endl;
    std::cerr << "Called here with " << argc << " arguments\n";
    for( int i = 1; i < argc; ++i )
      {
      std::cerr << " - " << argv[i] << "\n";
      }
    return EXIT_FAILURE;
    }
  std::string filename( argv[1] );
  typedef otb::SharkKMeansMachineLearningModel<InputValueType, TargetValueType> KMType;
  KMType::Pointer classifier = KMType::New();
  bool lCanRead = classifier->CanReadFile( filename );
  if( !lCanRead )
    {
    std::cerr << "Error otb::SharkKMeansMachineLearningModel : impossible to open the file " << filename << "."
              << std::endl;
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}

#endif
