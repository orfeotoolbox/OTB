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
#include <fstream>
#include <cstdlib>

int otbImageWidgetTransferFunctions(int argc, char * argv[])
{
  const char * outfname = argv[1];

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

  PixelType min = 10;
  PixelType max = 20;

  affine->SetLowerBound(min);
  affine->SetUpperBound(max);
  log->SetLowerBound(min);
  log->SetUpperBound(max);
  square->SetLowerBound(min);
  square->SetUpperBound(max);
  squareRoot->SetLowerBound(min);
  squareRoot->SetUpperBound(max);

  std::ofstream file;
  file.open(outfname);

  file<<"Affine transfer function: "<<std::endl;
  file<<"Out of bounds (lower): "<<(int)affine->Map(0)<<std::endl;
  file<<"Out of bounds (lower): "<<(int)affine->Map(30)<<std::endl;
  file<<"Inside bounds (lower): "<<(int)affine->Map(15)<<std::endl;
  file<<"Log transfer function: "<<std::endl;
  file<<"Out of bounds (lower): "<<(int)log->Map(0)<<std::endl;
  file<<"Out of bounds (lower): "<<(int)log->Map(30)<<std::endl;
  file<<"Inside bounds (lower): "<<(int)log->Map(15)<<std::endl;
  file<<"Square root transfer function: "<<std::endl;
  file<<"Out of bounds (lower): "<<(int)squareRoot->Map(0)<<std::endl;
  file<<"Out of bounds (lower): "<<(int)squareRoot->Map(30)<<std::endl;
  file<<"Inside bounds (lower): "<<(int)squareRoot->Map(15)<<std::endl;
  file<<"Square transfer function: "<<std::endl;
  file<<"Out of bounds (lower): "<<(int)square->Map(0)<<std::endl;
  file<<"Out of bounds (lower): "<<(int)square->Map(30)<<std::endl;
  file<<"Inside bounds (lower): "<<(int)square->Map(15)<<std::endl;

  file.close();

  return EXIT_SUCCESS;
}
