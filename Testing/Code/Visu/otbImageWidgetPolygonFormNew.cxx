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
#include "otbFullResolutionImageWidget.h"
#include "otbImageFileReader.h"
#include "otbImageWidgetPolygonForm.h"
#include <FL/Fl.H>


int otbImageWidgetPolygonFormNew( int argc, char * argv[] )
{
  typedef otb::ImageWidgetPolygonForm<>               PolyFormType;   
  PolyFormType::Pointer polyForm = PolyFormType::New();
  
  
  return EXIT_SUCCESS;
}
