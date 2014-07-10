/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "itkMacro.h"
#include "otbImage.h"
#include <iostream>

#include "itkBinaryBallStructuringElement.h"
#include "otbNeighborhoodMajorityVotingImageFilter.h"

int otbNeighborhoodMajorityVotingImageFilterNew(int itkNotUsed(argc), char ** itkNotUsed(argv))
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
