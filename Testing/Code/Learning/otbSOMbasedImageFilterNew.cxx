/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Copyright (c) Institut Telecom; Telecom bretagne. All rights reserved.
  See ITCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "otbVectorImage.h"
#include "itkVariableLengthVector.h"

#include "otbFlexibleDistanceWithMissingValue.h"
#include "otbSOMbasedImageFilter.h"

int otbSOMbasedImageFilterNew(int itkNotUsed(argc), char ** itkNotUsed(argv))
{
  const unsigned int Dimension = 2;
  typedef double                                 PixelType;
  typedef otb::VectorImage<PixelType, Dimension> ImageType;
  typedef ImageType::PixelType                   VectorType;

  typedef otb::Statistics::FlexibleDistanceWithMissingValue<VectorType> DistanceType;

  typedef otb::SOMbasedImageFilter<ImageType, ImageType, DistanceType, ImageType>
  FilterType;
  FilterType::Pointer somFilter = FilterType::New();

  std::cout << somFilter << std::endl;

  return EXIT_SUCCESS;
}
