/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#include "itkLabelMap.h"
#include "otbAttributesMapLabelObject.h"
#include "otbLabelObjectOpeningMuParserFilter.h"

int otbLabelObjectOpeningMuParserFilterNew( int itkNotUsed(argc), char * itkNotUsed(argv) [])
{

  const unsigned int     Dimension = 2;

  typedef otb::AttributesMapLabelObject<unsigned int, Dimension, double>     AttributesMapLabelObjectType;
  typedef itk::LabelMap<AttributesMapLabelObjectType>                        AttributesLabelMapType;
  typedef otb::LabelObjectOpeningMuParserFilter<AttributesLabelMapType>      FilterType;

  FilterType::Pointer   filter       = FilterType::New();

  return EXIT_SUCCESS;

}
