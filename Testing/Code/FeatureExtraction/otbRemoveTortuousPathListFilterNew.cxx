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
#include "itkExceptionObject.h"

#include "otbRemoveTortuousPathListFilter.h"
#include "itkPolyLineParametricPath.h"
#include <cstdlib>

int otbRemoveTortuousPathListFilterNew(int argc, char * argv[])
{
  const unsigned int Dimension =2;
  typedef itk::PolyLineParametricPath<Dimension> PathType;
  typedef otb::RemoveTortuousPathListFilter<PathType> RemoveTortuousPathListFilterType;

  // Instantiating object
  RemoveTortuousPathListFilterType::Pointer object = RemoveTortuousPathListFilterType::New();

  return EXIT_SUCCESS;
}
