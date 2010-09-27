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

#include "otbImage.h"
#include "itkRGBPixel.h"
#include "otbStandardImageViewer.h"

//This test is to check the correct compilation of the Visualization
//framework with Image<RGBPixel>
int otbStandardImageViewerRGBNew(int argc, char * argv[])
{
  typedef otb::Image<itk::RGBPixel<unsigned char>, 2> ImageType;
  typedef otb::StandardImageViewer<ImageType>         ViewerType;
  ViewerType::Pointer viewer = ViewerType::New();

  std::cout << viewer << std::endl;

  return EXIT_SUCCESS;
}
