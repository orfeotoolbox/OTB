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

#include "otbHuPathFunction.h"
#include "itkPolyLineParametricPath.h"
#include "itkMacro.h"

int otbHuPath(int itkNotUsed(argc), char * argv[])
{
  unsigned int       Number;
  const unsigned int Dimension = 2;
  const char *       outputFilename  = argv[1];
  typedef itk::PolyLineParametricPath<Dimension> PathType;
  typedef otb::HuPathFunction<PathType>          FunctionType;
  typedef FunctionType::RealType                 RealType;

  // Dessiner un carre:
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

  RealType Result;
  function->SetInputPath(pathElt);

  std::ofstream outputStream(outputFilename);
  outputStream << std::setprecision(10) << "Hu Path moments: [8]" << std::endl;

  for (Number = 1; Number < 8; Number++)
    {
    //OTB-FA-00024-CS
    function->SetMomentNumber(Number);
    Result = function->Evaluate();
    outputStream << "Hu(" << Number << ") = " << Result << std::endl;
    }
  outputStream.close();

  return EXIT_SUCCESS;
}
