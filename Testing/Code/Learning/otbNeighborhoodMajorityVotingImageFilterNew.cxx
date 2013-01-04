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

#include "otbNeighborhoodMajorityVotingImageFilter.h"

int otbNeighborhoodMajorityVotingImageFilterNew(int argc, char* argv[])
{    
  typedef unsigned char LabelPixelType;
  const unsigned int Dimension = 2;

  typedef otb::Image<LabelPixelType, Dimension> LabelImageType;
      
  typedef otb::NeighborhoodMajorityVotingImageFilter<LabelImageType> NeighborhoodMajorityVotingFilterType;
  
  NeighborhoodMajorityVotingFilterType::Pointer NeighMajVotingFilter = NeighborhoodMajorityVotingFilterType::New();
  
  std::cout << NeighMajVotingFilter << std::endl;

  return EXIT_SUCCESS;
}
