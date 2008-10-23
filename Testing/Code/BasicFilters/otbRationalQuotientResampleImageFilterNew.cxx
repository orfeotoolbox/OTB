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
#include "otbRationalQuotientResampleImageFilter.h"
#include "otbImage.h"

int otbRationalQuotientResampleImageFilterNew( int argc, char* argv[] )
{
  typedef float PixelType;
  const unsigned int Dimension = 2;
  
  // Rational quotient resample
  typedef otb::Image<PixelType,Dimension> ImageType;
  typedef otb::RationalQuotientResampleImageFilter<ImageType,ImageType> ResampleType;
  typedef ResampleType::Pointer ResamplePointerType;
  
  // Instanciation object
  ResamplePointerType rersample = ResampleType::New();
  
  return EXIT_SUCCESS;
}
