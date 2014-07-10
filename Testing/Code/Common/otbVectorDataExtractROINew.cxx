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
#include "otbVectorData.h"
#include "otbVectorDataExtractROI.h"

int otbVectorDataExtractROINew(int itkNotUsed(argc), char ** itkNotUsed(argv))
{
  typedef otb::VectorData<>                         VectorDataType;
  typedef otb::VectorDataExtractROI<VectorDataType> FilterType;

  FilterType::Pointer filter = FilterType::New();

  std::cout << filter << std::endl;

  return EXIT_SUCCESS;
}
