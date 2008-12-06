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
#include "otbImageWidgetTransferFunction.h"
#include <cstdlib>

int otbImageWidgetTransferFunctionsNew(int argc, char * argv[])
{
  typedef double PixelType;
  typedef otb::ImageWidgetAffineTransferFunction<PixelType> AffineType;
  typedef otb::ImageWidgetSquareRootTransferFunction<PixelType> SquareRootType;
  typedef otb::ImageWidgetLogTransferFunction<PixelType> LogType;
  typedef otb::ImageWidgetSquareTransferFunction<PixelType> SquareType;

  // Instantiation
  AffineType::Pointer affine = AffineType::New();
  LogType::Pointer log = LogType::New();
  SquareRootType::Pointer squareRoot = SquareRootType::New();
  SquareType::Pointer square = SquareType::New();

  return EXIT_SUCCESS;
}
