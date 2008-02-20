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
#include "otbImageWidgetTransfertFunction.h"
#include <fstream>

int otbImageWidgetTransfertFunctions(int argc, char * argv[])
{
  const char * outfname = argv[1];

  typedef double PixelType;
  typedef otb::ImageWidgetAffineTransfertFunction<PixelType> AffineType;
  typedef otb::ImageWidgetSquareRootTransfertFunction<PixelType> SquareRootType;
  typedef otb::ImageWidgetLogTransfertFunction<PixelType> LogType;
  typedef otb::ImageWidgetSquareTransfertFunction<PixelType> SquareType;
  
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

  file<<"Affine tranfert function: "<<std::endl;
  file<<"Out of bounds (lower): "<<(int)affine->Map(0)<<std::endl;
  file<<"Out of bounds (lower): "<<(int)affine->Map(30)<<std::endl;
  file<<"Inside bounds (lower): "<<(int)affine->Map(15)<<std::endl;
  file<<"Log transfert function: "<<std::endl;
  file<<"Out of bounds (lower): "<<(int)log->Map(0)<<std::endl;
  file<<"Out of bounds (lower): "<<(int)log->Map(30)<<std::endl;
  file<<"Inside bounds (lower): "<<(int)log->Map(15)<<std::endl;
  file<<"Square root transfert function: "<<std::endl;
  file<<"Out of bounds (lower): "<<(int)squareRoot->Map(0)<<std::endl;
  file<<"Out of bounds (lower): "<<(int)squareRoot->Map(30)<<std::endl;
  file<<"Inside bounds (lower): "<<(int)squareRoot->Map(15)<<std::endl;
  file<<"Square Transfert function: "<<std::endl;
  file<<"Out of bounds (lower): "<<(int)square->Map(0)<<std::endl;
  file<<"Out of bounds (lower): "<<(int)square->Map(30)<<std::endl;
  file<<"Inside bounds (lower): "<<(int)square->Map(15)<<std::endl;

  file.close();

  return EXIT_SUCCESS;
}
