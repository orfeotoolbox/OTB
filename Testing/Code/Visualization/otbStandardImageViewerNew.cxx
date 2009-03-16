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
#include "otbStandardImageViewer.h"
#include "otbImage.h"

int otbStandardImageViewerNew( int argc, char * argv[] )
{
  typedef otb::Image<double,2> ImageType;
  typedef otb::StandardImageViewer<ImageType> ViewerType;

  ViewerType::Pointer layer = ViewerType::New();

  return EXIT_SUCCESS;
}
