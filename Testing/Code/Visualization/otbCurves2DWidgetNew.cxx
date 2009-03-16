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
#include "otbCurves2DWidget.h"

int otbCurves2DWidgetNew( int argc, char * argv[] )
{
  typedef otb::Curves2DWidget WidgetType;

  WidgetType::Pointer widget = WidgetType::New();

  std::cout<<widget<<std::endl;

  return EXIT_SUCCESS;
}
