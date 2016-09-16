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

#include "otbStreamingTraits.h"

#include "otbImage.h"

#include <iostream>

const unsigned int Dimension = 2;
typedef double     PixelType;

template<class TInterpolatorType>
void TestInstantiate()
{
  typename TInterpolatorType::Pointer instance = TInterpolatorType::New();
  std::cout << instance << std::endl;
}

int otbStreamingTraitsImage(int itkNotUsed(argc), char* itkNotUsed(argv) [])
{
  // Images definition
  typedef otb::Image<PixelType, Dimension>      ImageType;
  typedef otb::StreamingTraits<ImageType>       StreamingTraits;

  // Check that all internal types can be instantiated
  TestInstantiate<StreamingTraits::NearestNeighborInterpolationType>();
  TestInstantiate<StreamingTraits::LinearInterpolationType>();
  TestInstantiate<StreamingTraits::BSplineInterpolationType>();
  TestInstantiate<StreamingTraits::GaussianInterpolationType>();
  TestInstantiate<StreamingTraits::CosineInterpolationType>();
  TestInstantiate<StreamingTraits::HammingInterpolationType>();
  TestInstantiate<StreamingTraits::WelchInterpolationType>();
  TestInstantiate<StreamingTraits::LanczosInterpolationType>();
  TestInstantiate<StreamingTraits::BlackmanInterpolationType>();
  TestInstantiate<StreamingTraits::ProlateInterpolationType>();
  TestInstantiate<StreamingTraits::BCOInterpolationType>();

  return EXIT_SUCCESS;
}

int otbStreamingTraitsVectorImage(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{
  // Images definition
  typedef otb::VectorImage<PixelType, Dimension>      ImageType;
  typedef otb::StreamingTraits<ImageType>             StreamingTraits;

  // Check that all internal types can be instantiated
  TestInstantiate<StreamingTraits::NearestNeighborInterpolationType>();
  TestInstantiate<StreamingTraits::LinearInterpolationType>();
  TestInstantiate<StreamingTraits::GaussianInterpolationType>();
  TestInstantiate<StreamingTraits::BCOInterpolationType>();

  return EXIT_SUCCESS;
}
