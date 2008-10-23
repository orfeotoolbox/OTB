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
#include "otbVectorizationPathListFilter.h"
#include "otbImage.h"
#include "itkPolyLineParametricPath.h"

int otbVectorizationPathListFilterNew(int argc, char * argv[])
{
  const unsigned int Dimension = 2;
  typedef double PixelType;
  typedef otb::Image<PixelType,Dimension> ImageType;
  typedef itk::PolyLineParametricPath<Dimension> PathType;
  typedef otb::VectorizationPathListFilter<ImageType,ImageType,PathType> VectorizationPathListFilterType;
  
  // Instantiating object
  VectorizationPathListFilterType::Pointer object = VectorizationPathListFilterType::New();

  return EXIT_SUCCESS;
}
