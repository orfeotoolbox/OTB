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



#include "itkMacro.h"
#include "itkPolyLineParametricPath.h"
#include "otbOrientationPathFunction.h"
#include "otbPathListToHistogramGenerator.h"

int otbPathListToHistogramGeneratorNew(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{
  const unsigned int Dimension = 2;
  typedef itk::PolyLineParametricPath<Dimension> PathType;
  typedef otb::OrientationPathFunction<PathType> FunctionType;

  typedef otb::PathListToHistogramGenerator<PathType, FunctionType> GeneratorType;

  GeneratorType::Pointer histogram = GeneratorType::New();

  std::cout << histogram << std::endl;

  return EXIT_SUCCESS;
}
