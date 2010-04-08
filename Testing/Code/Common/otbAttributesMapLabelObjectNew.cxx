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
#include "otbAttributesMapLabelObject.h"

int otbAttributesMapLabelObjectNew(int argc, char * argv[])
{
  const unsigned int Dimension = 2;

  typedef unsigned short                                              LabelType;
  typedef otb::AttributesMapLabelObject<LabelType, Dimension, double> LabelObjectType;
  typedef LabelObjectType::Pointer                                    LabelObjectPointerType;

  // Instantiation
  LabelObjectPointerType myAttMapLabelObject = LabelObjectType::New();

  return EXIT_SUCCESS;
}
