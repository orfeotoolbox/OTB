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



#include "otbImage.h"
#include "otbAddCarvingPathFilter.h"
#include "itkPolyLineParametricPath.h"

int otbAddCarvingPathFilterNew(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{
  typedef double PixelType;
  const unsigned int Dimension = 2;

  typedef otb::Image<PixelType, Dimension>       InputImageType;
  typedef otb::Image<PixelType, Dimension>       OutputImageType;
  typedef itk::PolyLineParametricPath<Dimension> PathType;

  typedef otb::AddCarvingPathFilter<InputImageType, PathType,
      OutputImageType>   AddCarvingPathFilterType;

  AddCarvingPathFilterType::Pointer filter = AddCarvingPathFilterType::New();

  std::cout << filter << std::endl;

  return EXIT_SUCCESS;
}
