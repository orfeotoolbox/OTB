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

#include "itkPolyLineParametricPath.h"
#include "otbVectorImage.h"
#include "otbRoadExtractionFilter.h"

int otbRoadExtractionFilterNew(int argc, char * argv[])
{
  const unsigned int Dimension = 2;
  typedef otb::VectorImage<double,Dimension> InputImageType;
  typedef itk::PolyLineParametricPath<Dimension> PathType;

  typedef otb::RoadExtractionFilter<InputImageType,PathType> RoadExtractionFilterType;

  // Instantiating object
  RoadExtractionFilterType::Pointer object = RoadExtractionFilterType::New();


  return EXIT_SUCCESS;
}
