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

#include "otbProlateInterpolateImageFunction.h"
#include "otbImage.h"

int otbProlateInterpolateImageFunctionNew(int argc, char * argv[])
{
  typedef otb::Image<double,2> ImageType;
  typedef otb::ProlateInterpolateImageFunction<ImageType>    ProlateInterpolatorType;
  
  // Instantiating object
  ProlateInterpolatorType::Pointer prolate = ProlateInterpolatorType::New();
  
  return EXIT_SUCCESS;
}
