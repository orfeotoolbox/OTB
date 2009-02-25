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

#include "otbPanTexTextureImageFunctionFilter.h"
#include "otbImage.h"

int otbPanTexTextureImageFunctionFilterNew(int argc, char * argv[])
{
  const unsigned int Dimension =2;
  typedef double PixelType;
  typedef otb::Image<PixelType,Dimension> ImageType;
  typedef otb::PanTexTextureImageFunctionFilter<ImageType, ImageType> PanTexType;

  // Instantiating object
  PanTexType::Pointer object = PanTexType::New();
  
  return EXIT_SUCCESS;
}
