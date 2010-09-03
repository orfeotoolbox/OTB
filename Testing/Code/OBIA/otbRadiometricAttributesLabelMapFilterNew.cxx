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
#include "otbRadiometricAttributesLabelMapFilter.h"
#include "otbVectorImage.h"
#include "itkLabelMap.h"
#include "otbAttributesMapLabelObject.h"

int otbRadiometricAttributesLabelMapFilterNew(int argc, char * argv[])
{
  const unsigned int Dimension = 2;
  typedef unsigned short LabelType;
  typedef double         PixelType;

  typedef otb::AttributesMapLabelObject<LabelType, Dimension, double>             LabelObjectType;
  typedef itk::LabelMap<LabelObjectType>                                          LabelMapType;
  typedef otb::VectorImage<PixelType, Dimension>                                  VectorImageType;
  typedef otb::RadiometricAttributesLabelMapFilter<LabelMapType, VectorImageType> RadiometricLabelMapFilterType;

  // Instantiation
  RadiometricLabelMapFilterType::Pointer radiometricLabelMapFilter = RadiometricLabelMapFilterType::New();

  return EXIT_SUCCESS;
}
