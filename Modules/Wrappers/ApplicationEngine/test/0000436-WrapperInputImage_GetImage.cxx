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


#include "otbWrapperInputImageParameter.h"


#include "otbLineSegmentDetector.h"

#include "otbVectorDataFileWriter.h"
#include "otbImageFileWriter.h"

// Code showing a problem calling twice GetImputImageParameter
// http://bugs.orfeo-toolbox.org/view.php?id=436

int main(int argc, char* argv[])
{
  if (argc != 3)
  {
    std::cout << "Usage : <inputImage>" << std::endl;

    return EXIT_FAILURE;
  }

  typedef otb::Wrapper::InputImageParameter InputImageParameterType;
  InputImageParameterType::Pointer          param = InputImageParameterType::New();

  param->SetFromFileName(argv[2]);

  if (param->GetImage() != param->GetImage())
  {
    return EXIT_FAILURE;
  }


  return EXIT_SUCCESS;
}
