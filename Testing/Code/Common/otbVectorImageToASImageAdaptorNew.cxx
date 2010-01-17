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
#include <cstdlib>

#include "otbVectorImageToASImageAdaptor.h"
#include "otbVectorImage.h"

int otbVectorImageToASImageAdaptorNew(int argc, char * argv[])
{
  typedef otb::VectorImage<double, 2>                 ImageType;
  typedef otb::VectorImageToASImageAdaptor<ImageType> FilterType;

  // Instantiating object
  FilterType::Pointer object = FilterType::New();


  return EXIT_SUCCESS;
}
