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

#include "otbSurfaceAdjencyEffect6SCorrectionSchemeFilter.h"
#include "otbVectorImage.h"
#include "otbImage.h"


int otbSurfaceAdjencyEffect6SCorrectionSchemeFilterNew(int argc, char * argv[])
{
  const unsigned int Dimension = 2;
  typedef double PixelType;
  typedef otb::VectorImage<PixelType,Dimension> InputImageType;
  typedef otb::SurfaceAdjencyEffect6SCorrectionSchemeFilter<InputImageType,InputImageType> SurfaceAdjencyEffect6SCorrectionSchemeFilterType;

  // Instantiating object
  SurfaceAdjencyEffect6SCorrectionSchemeFilterType::Pointer filter = SurfaceAdjencyEffect6SCorrectionSchemeFilterType::New();

  return EXIT_SUCCESS;
}
