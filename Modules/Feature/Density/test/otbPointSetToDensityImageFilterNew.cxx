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

#include <stdio.h>
#include "otbImage.h"
#include "otbPointSetToDensityImageFilter.h"
#include "itkVariableLengthVector.h"

int otbPointSetToDensityImageFilterNew(int, char*[])
{

  const unsigned int Dimension = 2;
  typedef float PixelType;

  typedef otb::Image<PixelType, Dimension>                            ImageType;
  typedef itk::VariableLengthVector<PixelType>                        RealVectorType;
  typedef itk::PointSet<RealVectorType, Dimension>                    PointSetType;
  typedef otb::PointSetToDensityImageFilter <PointSetType, ImageType> FunctionType;

  /**Instantiation of an object*/

  FunctionType::Pointer filter = FunctionType::New();

  std::cout << filter << std::endl;

  return EXIT_SUCCESS;
}
