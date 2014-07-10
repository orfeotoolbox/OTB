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


#include "otbLandmark.h"

#include "itkVariableLengthVector.h"
#include "itkPoint.h"

int otbLandmarkNew(int itkNotUsed(argc), char ** itkNotUsed(argv))
{
  typedef itk::Point<double, 2>                           PointType;
  typedef itk::VariableLengthVector<double>               PointDataType;
  typedef otb::Landmark<PointType, PointDataType, double> LandmarkType;

  // instantiation
  LandmarkType::Pointer landmark = LandmarkType::New();

  std::cout << landmark << std::endl;

  return EXIT_SUCCESS;
}
