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

#include "otbStreamingResampleImageFilter.h"
#include "otbImage.h"

int otbStreamingResampleImageFilterNew(int itkNotUsed(argc), char ** itkNotUsed(argv))
{
  const unsigned int Dimension = 2;
  typedef double                                 InputPixelType;
  typedef unsigned char                          OutputPixelType;
  typedef otb::Image<InputPixelType, Dimension>  InputImageType;
  typedef otb::Image<OutputPixelType, Dimension> OutputImageType;
  typedef double                                 InterpolatorPrecisionType;

  typedef otb::StreamingResampleImageFilter<InputImageType, OutputImageType,
      InterpolatorPrecisionType> StreamingResampleImageFilterType;

  // Instantiating object
  StreamingResampleImageFilterType::Pointer filter = StreamingResampleImageFilterType::New();

  std::cout << filter << std::endl;

  return EXIT_SUCCESS;
}
