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
#include "otbImageWidget.h"

int otbImageWidgetNew( int argc, char * argv[] )
{
  typedef otb::ImageWidget<> WidgetType;

  WidgetType::Pointer widget = WidgetType::New();

  std::cout<<widget<<std::endl;

  return EXIT_SUCCESS;
}
