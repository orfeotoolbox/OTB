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

#include "otbDEMToOrthoImageGenerator.h"
#include "otbMapProjections.h"
#include "otbImage.h"

int otbDEMToOrthoImageGeneratorNew(int argc, char * argv[])
{
  const unsigned int Dimension = 2;
  typedef otb::Image<unsigned char,Dimension>      ImageType;
  typedef otb::DEMToOrthoImageGenerator<ImageType, otb::UtmForwardProjection> DEMToImageGeneratorType;

  // Instantiating object
  DEMToImageGeneratorType::Pointer object = DEMToImageGeneratorType::New();


  return EXIT_SUCCESS;
}
