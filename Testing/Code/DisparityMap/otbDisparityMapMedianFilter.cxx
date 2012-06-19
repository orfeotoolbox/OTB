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
#include "otbDisparityMapMedianFilter.h"
#include "otbImage.h"
   
int otbDisparityMapMedianFilterNew(int argc, char* argv[])
{
  const unsigned int Dimension = 2;
  typedef float                                                         PixelType;
  typedef otb::Image<PixelType, Dimension>                                      FloatImageType;

  typedef otb::DisparityMapMedianFilter<FloatImageType,FloatImageType,FloatImageType>   DisparityMapMedianFilterType;
  // Instantiation
  DisparityMapMedianFilterType::Pointer filter = DisparityMapMedianFilterType::New();

   return EXIT_SUCCESS;
}


