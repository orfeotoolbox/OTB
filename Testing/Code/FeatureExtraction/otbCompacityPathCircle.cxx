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

int otbCompacityPathCircle(int itkNotUsed(argc), char * argv[])
{
  unsigned int NbOfPoints((unsigned int) ::atoi(argv[1]));

  const unsigned int Dimension = 2;
  typedef itk::PolyLineParametricPath<Dimension> PathType;
  typedef otb::CompacityPathFunction<PathType>   FunctionType;
  typedef FunctionType::RealType                 RealType;

  PathType::ContinuousIndexType cindex;
  PathType::Pointer             pathElt = PathType::New();

  if (NbOfPoints < 2)
    {
    std::cout << "NbOfPoints must be greater than 2 !" << std::endl;
    return EXIT_FAILURE;
    }

  RealType deltaTheta;
  RealType Rho = 100.0;

  deltaTheta = 2. * otb::CONST_PI / static_cast<RealType>(NbOfPoints);

  pathElt->Initialize();

  for (unsigned int noTheta = 0; noTheta < NbOfPoints; noTheta++)
    {
    RealType Theta = deltaTheta * static_cast<RealType>(noTheta);

    cindex[0] = (Rho * vcl_cos(Theta));
    cindex[1] = (Rho * vcl_sin(Theta));
    pathElt->AddVertex(cindex);
    }

  FunctionType::Pointer function = FunctionType::New();
  function->SetInputPath(pathElt);

  RealType Result = function->Evaluate();
  std::cout << "Compacity result: " << Result << std::endl;

  RealType Error;
  Error = vcl_abs(Result - static_cast<RealType>(1.0));

  if (Error > 1.E-5)
    {
    std::cout << "Error in estimation !" << std::endl;
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}
