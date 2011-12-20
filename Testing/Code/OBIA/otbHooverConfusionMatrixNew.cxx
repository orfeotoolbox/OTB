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

#include "otbHooverConfusionMatrix.h"

#include "itkLabelMap.h"
#include "itkLabelObject.h"

int otbHooverConfusionMatrixNew(int argc, char* argv[])
{
  typedef itk::LabelObject<unsigned int, 2>          LabelObjectType;
  typedef itk::LabelMap<LabelObjectType>            LabelMapType;
  typedef otb::HooverConfusionMatrix<LabelMapType>  HooverConfusionMatrixType;

  // instantiation
  HooverConfusionMatrixType::Pointer object = HooverConfusionMatrixType::New();

  std::cout << object << std::endl;

  return EXIT_SUCCESS;
}
