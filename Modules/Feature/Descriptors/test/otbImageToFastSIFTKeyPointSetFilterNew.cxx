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

#include "otbSiftFastImageFilter.h"
#include "itkPointSet.h"
#include "itkVariableLengthVector.h"

int otbImageToFastSIFTKeyPointSetFilterNew(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{
  typedef float RealType;
  const unsigned int Dimension = 2;

  typedef otb::Image<RealType, Dimension>                   ImageType;
  typedef itk::VariableLengthVector<RealType>               RealVectorType;
  typedef itk::PointSet<RealVectorType, Dimension>          PointSetType;
  typedef otb::SiftFastImageFilter<ImageType, PointSetType> ImageToFastSIFTKeyPointSetFilterType;

  // Instantiating object
  ImageToFastSIFTKeyPointSetFilterType::Pointer filter = ImageToFastSIFTKeyPointSetFilterType::New();

  std::cout << filter << std::endl;

  return EXIT_SUCCESS;
}
