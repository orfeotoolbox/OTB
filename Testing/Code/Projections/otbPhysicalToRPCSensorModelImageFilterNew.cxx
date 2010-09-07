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

#include "otbPhysicalToRPCSensorModelImageFilter.h"
#include "otbVectorImage.h"

int otbPhysicalToRPCSensorModelImageFilterNew(int argc, char* argv[])
{
  // Images definition
  const unsigned int Dimension = 2;
  typedef double                                      PixelType;
  typedef otb::VectorImage<PixelType, Dimension>      ImageType;
  
  typedef otb::PhysicalToRPCSensorModelImageFilter<ImageType> PhysicalToSensorModelType;

  // Instanciation of the filter
  PhysicalToSensorModelType::Pointer resampler = PhysicalToSensorModelType::New();
  
  return 0;
}
