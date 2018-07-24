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
#include "otbExtern.h"


#include "otbMultiToMonoChannelExtractROI.h"

int otbMultiToMonoChannelExtractROINew(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{

  //        typedef unsigned char                                    InputPixelType;
  //        typedef unsigned char                                    OutputPixelType;

  typedef std::complex<float>  InputPixelType;
  typedef std::complex<double> OutputPixelType;

  typedef otb::MultiToMonoChannelExtractROI<InputPixelType,
      OutputPixelType>  ExtractROIFilterType;

  ExtractROIFilterType::Pointer extractROIFilter = ExtractROIFilterType::New();

  std::cout << extractROIFilter << std::endl;

  return EXIT_SUCCESS;
}
