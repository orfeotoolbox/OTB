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



#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageToLabelMapWithAttributesFilter.h"
#include "itkLabelMap.h"
#include "otbAttributesMapLabelObjectWithClassLabel.h"

int otbImageToLabelMapWithAttributesFilterNew(int itkNotUsed(argc), char * itkNotUsed(argv)[])
{

  typedef otb::VectorImage<double, 2>                 ImageType;
  typedef otb::Image<unsigned int, 2>                 LabeledImageType;
  typedef otb::AttributesMapLabelObjectWithClassLabel<double, 2, double, double> LabelObjectType;
  typedef otb::ImageToLabelMapWithAttributesFilter<ImageType, LabeledImageType, unsigned int , LabelObjectType>   FilterType;

  FilterType::Pointer   filter = FilterType::New();

  std::cout << filter << std::endl;

  return EXIT_SUCCESS;
}
