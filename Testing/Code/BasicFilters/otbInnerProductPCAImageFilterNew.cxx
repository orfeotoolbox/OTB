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
#include "otbInnerProductPCAImageFilter.h"
#include "otbVectorImage.h"

int otbInnerProductPCAImageFilterNew( int argc, char* argv[] )
{
  typedef double PixelType;
  const unsigned int Dimension = 2;

  // Typedef 
  typedef otb::VectorImage<PixelType,Dimension> ImageType;
  typedef otb::InnerProductPCAImageFilter<ImageType,ImageType> FilterType;

  // Instanciation object
  FilterType::Pointer filter = FilterType::New();

  return EXIT_SUCCESS;
}
