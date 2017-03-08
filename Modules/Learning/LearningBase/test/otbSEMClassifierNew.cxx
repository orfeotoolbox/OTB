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




#include "itkMacro.h"
#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbSEMClassifier.h"

#include <iostream>

int otbSEMClassifierNew(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{
  typedef double                                            PixelType;
  typedef otb::VectorImage<PixelType, 2>                    ImageType;
  typedef otb::Image<unsigned char, 2>                      OutputImageType;
  typedef otb::SEMClassifier<ImageType, OutputImageType>    ClassifType;
  typedef itk::Statistics::ListSample<ImageType::PixelType> SampleType;
  typedef itk::Statistics::Subsample<SampleType>            ClassSampleType;

  typedef otb::Statistics::ModelComponentBase<ClassSampleType> ComponentType;

  ClassifType::Pointer   classifier = ClassifType::New();
  ComponentType::Pointer component = ComponentType::New();

  std::cout << classifier << std::endl;

  return EXIT_SUCCESS;
}
