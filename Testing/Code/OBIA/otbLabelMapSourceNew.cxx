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

#include "otbLabelMapSource.h"
#include "itkLabelMap.h"
#include "otbAttributesMapLabelObject.h"

int otbLabelMapSourceNew(int argc, char * argv[])
{
  const unsigned int Dimension = 2;
  typedef unsigned short LabelType;

  typedef otb::AttributesMapLabelObject<LabelType, Dimension, double> LabelObjectType;

  typedef itk::LabelMap<LabelObjectType>         InputLabelMapType;
  typedef otb::LabelMapSource<InputLabelMapType> LabelMapSourceType;

  // Instantiating LabelMapSource object
  LabelMapSourceType::Pointer labelMap = LabelMapSourceType::New();

  std::cout << labelMap << std::endl;

  return EXIT_SUCCESS;
}
