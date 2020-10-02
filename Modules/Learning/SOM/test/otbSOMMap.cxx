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

#include "itkMacro.h"
#include "otbSOMMap.h"
#include "itkRGBPixel.h"

int otbSOMMap(int itkNotUsed(argc), char* itkNotUsed(argv)[])
{
  const unsigned int                                          Dimension = 2;
  typedef float                                               InternalPixelType;
  typedef itk::VariableLengthVector<InternalPixelType>        PixelType;
  typedef itk::Statistics::EuclideanDistanceMetric<PixelType> DistanceType;
  typedef otb::SOMMap<PixelType, DistanceType, Dimension> SOMMapType;

  // Instantiation
  SOMMapType::Pointer somMap = SOMMapType::New();

  // Allocation of the som map
  SOMMapType::RegionType region;
  SOMMapType::IndexType  index;
  SOMMapType::SizeType   size;
  index.Fill(0);
  size.Fill(64);
  region.SetIndex(index);
  region.SetSize(size);
  somMap->SetRegions(region);
  somMap->SetNumberOfComponentsPerPixel(3);
  somMap->Allocate();

  // Filling with null pixels
  PixelType nullPixel;
  nullPixel.SetSize(3);
  nullPixel.Fill(0);
  somMap->FillBuffer(nullPixel);

  // Definition of a non-null pixel
  PixelType winner;
  winner.SetSize(3);
  winner.Fill(1);
  index.Fill(32);
  somMap->SetPixel(index, winner);

  // Test of the GetWinner method
  SOMMapType::IndexType winnerIndex = somMap->GetWinner(winner);
  if (winnerIndex != index)
  {
    std::cout << "Bad GetWinner function return." << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
