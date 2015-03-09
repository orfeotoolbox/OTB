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
#include "itkMacro.h"
#include "itkPointSet.h"
#include "otbVectorImage.h"
#include "otbBSplinesInterpolateTransformDisplacementFieldGenerator.h"

int otbBSplinesInterpolateTransformDisplacementFieldGeneratorNew(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{
  const unsigned int Dimension = 2;
  typedef double                                                                              PixelType;
  typedef otb::VectorImage<PixelType, Dimension>                                              ImageType;
  typedef itk::Array<double>                                                                  ParametersType;
  typedef itk::PointSet<ParametersType, Dimension>                                            PointSetType;
  typedef otb::BSplinesInterpolateTransformDisplacementFieldGenerator<PointSetType, ImageType> FilterType;

  // Instantiating object
  FilterType::Pointer filter = FilterType::New();

  std::cout << filter << std::endl;

  return EXIT_SUCCESS;
}
