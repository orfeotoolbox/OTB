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




#include <fstream>
#include "otbSOMMap.h"
#include "otbSOMClassifier.h"
#include "itkListSample.h"

int otbSOMClassifierNew(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{
  typedef double InputPixelType;
  typedef int    LabelPixelType;
  const unsigned int Dimension = 2;

  typedef itk::VariableLengthVector<InputPixelType>                  PixelType;
  typedef itk::Statistics::EuclideanDistanceMetric<PixelType>              DistanceType;
  typedef otb::SOMMap<PixelType, DistanceType, Dimension>            SOMMapType;
  typedef itk::Statistics::ListSample<PixelType>                     SampleType;
  typedef otb::SOMClassifier<SampleType, SOMMapType, LabelPixelType> ClassifierType;

  ClassifierType::Pointer classifier = ClassifierType::New();

  std::cout << classifier << std::endl;

  return EXIT_SUCCESS;
}
