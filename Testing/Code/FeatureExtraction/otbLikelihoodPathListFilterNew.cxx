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

#include "otbLikelihoodPathListFilter.h"
#include "otbPolyLineParametricPathWithValue.h"
#include "otbImage.h"

int otbLikelihoodPathListFilterNew(int itkNotUsed(argc), char ** itkNotUsed(argv))
{
  const unsigned int Dimension = 2;
  typedef double                                                  PixelType;
  typedef otb::Image<PixelType, Dimension>                        ImageType;
  typedef otb::PolyLineParametricPathWithValue<double, Dimension> PathType;
  typedef otb::LikelihoodPathListFilter<PathType, ImageType>      LinkPathListFilterType;

  // Instantiating object
  LinkPathListFilterType::Pointer filter = LinkPathListFilterType::New();

  std::cout << filter << std::endl;

  return EXIT_SUCCESS;
}
