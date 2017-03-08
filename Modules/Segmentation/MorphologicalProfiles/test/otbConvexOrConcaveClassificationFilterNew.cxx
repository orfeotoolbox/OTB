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

#include "otbConvexOrConcaveClassificationFilter.h"
#include "otbImage.h"

int otbConvexOrConcaveClassificationFilterNew(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{
  typedef otb::Image<double, 2>        InputImageType;
  typedef otb::Image<unsigned char, 2> OutputImageType;

  typedef otb::ConvexOrConcaveClassificationFilter<InputImageType, OutputImageType> ClassificationFilterType;

  // Instantiating object
  ClassificationFilterType::Pointer filter = ClassificationFilterType::New();

  std::cout << filter << std::endl;

  return EXIT_SUCCESS;
}
