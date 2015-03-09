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
#include "otbDisparityMapEstimationMethod.h"
#include "otbImage.h"
#include "itkTranslationTransform.h"

int otbDisparityMapEstimationMethodNew(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{
  const unsigned int Dimension = 2;
  typedef unsigned char                                                         PixelType;
  typedef otb::Image<PixelType, Dimension>                                      ImageType;
  typedef itk::TranslationTransform<double, Dimension>                          TransformType;
  typedef TransformType::ParametersType                                         ParametersType;
  typedef itk::PointSet<ParametersType, Dimension>                              PointSetType;
  typedef otb::DisparityMapEstimationMethod<ImageType, ImageType, PointSetType> DMEstimationType;

  // Instantiation
  DMEstimationType::Pointer dmestimator = DMEstimationType::New();

  std::cout << dmestimator << std::endl;

  return EXIT_SUCCESS;
}
