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

#include "itkMacro.h"
#include "otbImage.h"
#include <iostream>

#include "otbNeighborhoodMajorityVotingImageFilter.h"

int otbNeighborhoodMajorityVotingImageFilterNew(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{
  typedef unsigned char InputLabelPixelType; // 8 bits
  typedef unsigned short OutputLabelPixelType; // 16 bits
  const unsigned int Dimension = 2;

  typedef otb::Image<InputLabelPixelType, Dimension> InputLabelImageType;
  typedef otb::Image<OutputLabelPixelType, Dimension> OutputLabelImageType;

  // Binary ball Structuring Element type
  typedef itk::BinaryBallStructuringElement<InputLabelPixelType, Dimension> BallStructuringType;

  // Neighborhood majority voting filter types
  typedef otb::NeighborhoodMajorityVotingImageFilter<InputLabelImageType, OutputLabelImageType, BallStructuringType> NeighborhoodMajorityVotingFilter3ArgsType;
  typedef otb::NeighborhoodMajorityVotingImageFilter<InputLabelImageType, OutputLabelImageType> NeighborhoodMajorityVotingFilter2ArgsType;
  typedef otb::NeighborhoodMajorityVotingImageFilter<InputLabelImageType> NeighborhoodMajorityVotingFilter1ArgType;

  // Neighborhood majority voting filters
  NeighborhoodMajorityVotingFilter3ArgsType::Pointer NeighMajVotingFilter3Args = NeighborhoodMajorityVotingFilter3ArgsType::New();
  NeighborhoodMajorityVotingFilter2ArgsType::Pointer NeighMajVotingFilter2Args = NeighborhoodMajorityVotingFilter2ArgsType::New();
  NeighborhoodMajorityVotingFilter1ArgType::Pointer NeighMajVotingFilter1Arg = NeighborhoodMajorityVotingFilter1ArgType::New();

  std::cout << NeighMajVotingFilter3Args << std::endl;
  std::cout << std::endl;
  std::cout << NeighMajVotingFilter2Args << std::endl;
  std::cout << std::endl;
  std::cout << NeighMajVotingFilter1Arg << std::endl;

  return EXIT_SUCCESS;
}
