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

#include "itkLabelMap.h"
#include "otbAttributesMapLabelObject.h"
#include "otbLabelObjectOpeningMuParserFilter.h"

int otbLabelObjectOpeningMuParserFilterNew( int itkNotUsed(argc), char ** itkNotUsed(argv))
{

  const unsigned int     Dimension = 2;

  typedef otb::AttributesMapLabelObject<unsigned int, Dimension, double>     AttributesMapLabelObjectType;
  typedef itk::LabelMap<AttributesMapLabelObjectType>                        AttributesLabelMapType;
  typedef otb::LabelObjectOpeningMuParserFilter<AttributesLabelMapType>      FilterType;

  FilterType::Pointer   filter       = FilterType::New();

  return EXIT_SUCCESS;

}
