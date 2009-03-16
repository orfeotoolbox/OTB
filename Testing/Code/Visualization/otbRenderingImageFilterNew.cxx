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
#include "otbRenderingImageFilter.h"
#include "otbImage.h"
#include "otbVectorImage.h"

int otbRenderingImageFilterNew( int argc, char * argv[] )
{
  typedef double PixelType;
  typedef otb::Image<PixelType,2>       ImageType;
  typedef otb::VectorImage<PixelType,2> VectorImageType;
  typedef otb::RenderingImageFilter<ImageType> RenderingFilterType;
  typedef otb::RenderingImageFilter<VectorImageType> VectorRenderingFilterType;

  RenderingFilterType::Pointer rendering = RenderingFilterType::New();
  VectorRenderingFilterType::Pointer vrendering = VectorRenderingFilterType::New();
  return EXIT_SUCCESS;
}
