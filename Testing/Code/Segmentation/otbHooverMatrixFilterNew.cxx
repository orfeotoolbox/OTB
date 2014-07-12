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

#include "otbHooverMatrixFilter.h"

#include "itkLabelMap.h"
#include "itkLabelObject.h"

int otbHooverMatrixFilterNew(int itkNotUsed(argc), char* itkNotUsed(argv)[])
{
  typedef itk::LabelObject<unsigned int, 2>          LabelObjectType;
  typedef itk::LabelMap<LabelObjectType>            LabelMapType;
  typedef otb::HooverMatrixFilter<LabelMapType>  HooverMatrixFilterType;

  // instantiation
  HooverMatrixFilterType::Pointer object = HooverMatrixFilterType::New();

  std::cout << object << std::endl;

  return EXIT_SUCCESS;
}
