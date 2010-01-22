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

#include "otbBSplineDecompositionImageFilter.h"
#include "otbImage.h"

int otbBSplineDecompositionImageFilterNew(int argc, char * argv[])
{
  typedef otb::Image<double,2> ImageType;
  typedef otb::BSplineDecompositionImageFilter<ImageType,ImageType> BSplineDecompositionImageFilterType;

  // Instantiating object
  BSplineDecompositionImageFilterType::Pointer filter = BSplineDecompositionImageFilterType::New();

  return EXIT_SUCCESS;
}
