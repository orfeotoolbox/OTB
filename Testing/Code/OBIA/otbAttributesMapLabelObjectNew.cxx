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
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "otbAttributesMapLabelObject.h"

int otbAttributesMapLabelObjectNew(int argc, char* argv[])
{
  typedef otb::AttributesMapLabelObject<unsigned short, 2, double> LabelObjectType;

  // instantiation
  LabelObjectType::Pointer object = LabelObjectType::New();

  std::cout << object << std::endl;

  return EXIT_SUCCESS;
}
