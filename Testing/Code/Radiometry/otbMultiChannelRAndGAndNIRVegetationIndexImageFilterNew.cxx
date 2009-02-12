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

#include "otbMultiChannelRAndGAndNIRIndexImageFilter.h"
#include "otbImage.h"
#include "otbVectorImage.h"


int otbMultiChannelRAndGAndNIRVegetationIndexImageFilterNew(int argc, char * argv[])
{
  const unsigned int Dimension = 2;
  typedef otb::VectorImage<unsigned char,Dimension> InputImageType;
  typedef otb::Image<float,Dimension> OutputImageType;
  typedef otb::MultiChannelRAndGAndNIRIndexImageFilter<InputImageType,OutputImageType> MultiChannelRAndGAndNIRIndexImageFilterType;

  // Instantiating object
  MultiChannelRAndGAndNIRIndexImageFilterType::Pointer object = MultiChannelRAndGAndNIRIndexImageFilterType::New();


  return EXIT_SUCCESS;
}
