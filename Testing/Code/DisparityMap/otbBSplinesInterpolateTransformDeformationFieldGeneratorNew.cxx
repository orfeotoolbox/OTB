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
#include "itkPointSet.h"
#include "otbVectorImage.h"
#include "otbBSplinesInterpolateTransformDeformationFieldGenerator.h"

int otbBSplinesInterpolateTransformDeformationFieldGeneratorNew(int argc, char * argv[])
{
  const unsigned int Dimension = 2;
  typedef double PixelType;
  typedef otb::VectorImage<PixelType,Dimension> ImageType;
  typedef itk::Array<double> ParametersType;
  typedef itk::PointSet<ParametersType,Dimension> PointSetType;
  typedef otb::BSplinesInterpolateTransformDeformationFieldGenerator<PointSetType,ImageType> FilterType;

  // Instantiating object
  FilterType::Pointer filter = FilterType::New();


  return EXIT_SUCCESS;
}
