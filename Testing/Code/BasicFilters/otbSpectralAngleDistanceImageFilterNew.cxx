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
#include "itkExceptionObject.h"
#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbSpectralAngleDistanceImageFilter.h"

int otbSpectralAngleDistanceImageFilterNew(int argc, char * argv[])
{
  const unsigned int Dimension = 2;
  typedef double PixelType;
  typedef otb::VectorImage<PixelType,Dimension> InputImageType;
  typedef otb::Image<PixelType,Dimension> OutputImageType;
  typedef otb::SpectralAngleDistanceImageFilter<InputImageType,OutputImageType> DistanceFilterType;

  // Instantiating object
  DistanceFilterType::Pointer object = DistanceFilterType::New();


  return EXIT_SUCCESS;
}
