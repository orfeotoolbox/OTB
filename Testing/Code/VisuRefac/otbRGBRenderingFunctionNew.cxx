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
#include "otbRGBRenderingFunction.h"
#include "itkRGBPixel.h"

int otbRGBRenderingFunctionNew( int argc, char * argv[] )
{
  typedef double                       PixelType;
  typedef itk::RGBPixel<unsigned char> RGBPixelType;
  typedef otb::Function::RGBRenderingFunction
    <PixelType,RGBPixelType> RGBRenderingFunctionType;
  RGBRenderingFunctionType::Pointer function = RGBRenderingFunctionType::New();

  return EXIT_SUCCESS;
}
