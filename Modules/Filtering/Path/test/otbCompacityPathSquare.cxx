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


#include <cstdlib>
#include "otbCompacityPathFunction.h"
#include "itkPolyLineParametricPath.h"
#include "itkMacro.h"
#include "otbMath.h"

int otbCompacityPathSquare(int itkNotUsed(argc), char * argv[])
{
  double A ((double) ::atof(argv[1]));

  const unsigned int Dimension = 2;
  typedef itk::PolyLineParametricPath<Dimension> PathType;
  typedef otb::CompacityPathFunction<PathType>   FunctionType;
  typedef FunctionType::RealType                 RealType;

  PathType::ContinuousIndexType cindex;
  PathType::Pointer             pathElt = PathType::New();

  if (A < 0)
    {
    std::cout << "square must be greater than 0.0 !" << std::endl;
    return EXIT_FAILURE;
    }

  pathElt->Initialize();

  cindex[0] = 100;
  cindex[1] = 100;
  pathElt->AddVertex(cindex);
  cindex[0] = 100 + A;
  cindex[1] = 100;
  pathElt->AddVertex(cindex);
  cindex[0] = 100 + A;
  cindex[1] = 100 + A;
  pathElt->AddVertex(cindex);
  cindex[0] = 100;
  cindex[1] = 100 + A;
  pathElt->AddVertex(cindex);

  FunctionType::Pointer function = FunctionType::New();
  function->SetInputPath(pathElt);

  RealType Result = function->Evaluate();
  std::cout << "Compacity result: " << Result << std::endl;

  RealType Error;
  Error = vcl_abs(Result - static_cast<RealType>(otb::CONST_PI_4));

  if (Error > 1.E-9)
    {
    std::cout << "Error in Theta estimation :" << std::endl;
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}
