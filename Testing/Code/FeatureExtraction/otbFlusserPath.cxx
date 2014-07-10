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



#include <iostream>
#include <iomanip>
#include <fstream>
#include "otbFlusserPathFunction.h"
#include "itkPolyLineParametricPath.h"
#include "itkMacro.h"

int otbFlusserPath(int itkNotUsed(argc), char * argv[])
{
  unsigned int       Number;
  const unsigned int Dimension = 2;
  const char *       outputFilename  = argv[1];
  typedef itk::PolyLineParametricPath<Dimension> PathType;
  typedef otb::FlusserPathFunction<PathType>     FunctionType;
  typedef FunctionType::RealType                 RealType;

  // Draw a square
  PathType::ContinuousIndexType cindex;
  PathType::Pointer             pathElt = PathType::New();

  pathElt->Initialize();

  cindex[0] = 30;
  cindex[1] = 30;
  pathElt->AddVertex(cindex);
  cindex[0] = 30;
  cindex[1] = 130;
  pathElt->AddVertex(cindex);
  cindex[0] = 130;
  cindex[1] = 130;
  pathElt->AddVertex(cindex);
  cindex[0] = 130;
  cindex[1] = 30;
  pathElt->AddVertex(cindex);

  FunctionType::Pointer function = FunctionType::New();
  //OTB-FA-00022-CS
  function->SetInputPath(pathElt);

  std::ofstream outputStream(outputFilename);

  outputStream << std::setprecision(10) << "Flusser Path moments: [12]" << std::endl;

  RealType Result;

  for (Number = 1; Number < 12; Number++)
    {
    //OTB-FA-00024-CS
    function->SetMomentNumber(Number);
    Result = function->Evaluate();
    outputStream << "Flusser(" << Number << ") = " << Result << std::endl;
    }

  outputStream.close();

  return EXIT_SUCCESS;
}
