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

#include "otbLabelMapToAttributeImageFilter.h"

#include "itkLabelMap.h"
#include "otbVectorImage.h"

int otbLabelMapToAttributeImageFilterNew(int argc, char* argv[])
{
  typedef otb::AttributesMapLabelObject<unsigned int, 2, float> LabelObjectType;
  typedef itk::LabelMap<LabelObjectType>                        LabelMapType;
  typedef otb::VectorImage<float, 2>                            VectorImageType;
  typedef otb::LabelMapToAttributeImageFilter
      <LabelMapType, VectorImageType>          LabelMapToAttributeImageFilterType;

  // instantiation
  LabelMapToAttributeImageFilterType::Pointer object = LabelMapToAttributeImageFilterType::New();

  std::cout << object << std::endl;

  return EXIT_SUCCESS;
}
