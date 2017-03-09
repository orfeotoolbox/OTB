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


#include "otbImage.h"
#include "otbWaveletOperator.h"
#include "otbWaveletFilterBank.h"
#include "otbWaveletTransform.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbCommandLineArgumentParser.h"

#include "itkListSample.h"

#include "otbSparseWvltToAngleMapperListFilter.h"

int otbSparseWvltToAngleMapperListFilterNew ( int itkNotUsed(argc), char * itkNotUsed(argv) [])
{
  const unsigned int Dimension = 2;
  const unsigned int SpaceDimension = 3;

  typedef float  PixelType;
  typedef double PrecisionType;

  typedef itk::FixedArray< PrecisionType, SpaceDimension > AngleType;
  typedef itk::Statistics::ListSample< AngleType >         AngleListType;

  typedef otb::Image< PixelType, Dimension >  ImageType;
  typedef otb::ImageList< ImageType >         ImageListType;
  typedef otb::SparseWvltToAngleMapperListFilter<
    ImageListType, AngleListType, SpaceDimension+1 > AngleListFilterType;

  AngleListFilterType::Pointer filter = AngleListFilterType::New();

  return EXIT_SUCCESS;
}
