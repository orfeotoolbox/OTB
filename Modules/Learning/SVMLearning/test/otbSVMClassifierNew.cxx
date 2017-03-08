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




#include "itkPointSetToListSampleAdaptor.h"
#include "itkSubsample.h"
#include "otbSVMClassifier.h"

int otbSVMClassifierNew(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{
  typedef double                           InputPixelType;
  typedef int                              LabelPixelType;
  typedef itk::PointSet<InputPixelType, 2> PointSetType;

  typedef itk::Statistics::PointSetToListSampleAdaptor<PointSetType>
  DataSampleType;

  typedef otb::SVMClassifier<DataSampleType, LabelPixelType> ClassifierType;

  ClassifierType::Pointer classifier = ClassifierType::New();

  std::cout << classifier << std::endl;

  return EXIT_SUCCESS;
}
