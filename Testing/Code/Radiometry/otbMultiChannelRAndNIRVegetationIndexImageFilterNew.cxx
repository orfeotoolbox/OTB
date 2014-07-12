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

#include "otbMultiChannelRAndNIRIndexImageFilter.h"
#include "otbImage.h"
#include "otbVectorImage.h"

int otbMultiChannelRAndNIRVegetationIndexImageFilterNew(int itkNotUsed(argc), char * itkNotUsed(argv)[])
{
  const unsigned int Dimension = 2;
  typedef otb::VectorImage<unsigned char, Dimension> InputImageType;
  typedef otb::Image<float, Dimension>               OutputImageType;
  typedef otb::MultiChannelRAndNIRIndexImageFilter<InputImageType,
      OutputImageType> MultiChannelRAndNIRIndexImageFilterType;

  // Instantiating object
  MultiChannelRAndNIRIndexImageFilterType::Pointer filter = MultiChannelRAndNIRIndexImageFilterType::New();

  std::cout << filter << std::endl;

  return EXIT_SUCCESS;
}
