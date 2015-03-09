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

#include "otbMultiChannelRAndBAndNIRIndexImageFilter.h"
#include "otbImage.h"
#include "otbVectorImage.h"

int otbMultiChannelRAndBAndNIRVegetationIndexImageFilterNew(int itkNotUsed(argc), char * itkNotUsed(argv)[])
{
  const unsigned int Dimension = 2;
  typedef otb::VectorImage<unsigned char, Dimension> InputImageType;
  typedef otb::Image<float, Dimension>               OutputImageType;
  typedef otb::MultiChannelRAndBAndNIRIndexImageFilter<InputImageType,
      OutputImageType> MultiChannelRAndBAndNIRIndexImageFilterType;

  // Instantiating object
  MultiChannelRAndBAndNIRIndexImageFilterType::Pointer filter = MultiChannelRAndBAndNIRIndexImageFilterType::New();

  std::cout << filter << std::endl;

  return EXIT_SUCCESS;
}
