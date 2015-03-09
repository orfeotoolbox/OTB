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
#include "otbImageFileReader.h"
#include "otbComplexMomentPathFunction.h"
#include "itkMacro.h"
#include "itkPolyLineParametricPath.h"

int otbComplexMomentPath(int itkNotUsed(argc), char * argv[])
{
  unsigned int p((unsigned int) ::atoi(argv[1]));
  unsigned int q((unsigned int) ::atoi(argv[2]));
  const char * outputFilename  = argv[3];

  const unsigned int Dimension = 2;

  typedef itk::PolyLineParametricPath<Dimension>   PathType;
  typedef otb::ComplexMomentPathFunction<PathType> CMType;
  typedef CMType::ComplexType                      ComplexType;

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

  CMType::Pointer function = CMType::New();

  function->SetQ(q);
  function->SetP(p);

  ComplexType Result;

  std::ofstream outputStream(outputFilename);

  Result = function->Evaluate(*pathElt);
  outputStream << std::setprecision(10) << "function->Evaluate(Path)" << Result << std::endl;
  outputStream.close();

  return EXIT_SUCCESS;
}
