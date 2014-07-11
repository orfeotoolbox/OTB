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
#include "otbVectorImage.h"

#include <iostream>

const unsigned int Dimension = 2;
typedef double     PixelType;

template<class TInterpolatorType>
void TestInstanciate()
{
  typename TInterpolatorType::Pointer instance = TInterpolatorType::New();
  std::cout << instance << std::endl;
}

int otbStreamingTraitsImage(int itkNotUsed(argc), char* itkNotUsed(argv) [])
{
  // Images definition
  typedef otb::Image<PixelType, Dimension>      ImageType;
  typedef otb::StreamingTraits<ImageType>       StreamingTraits;

  // Check that all internal types can be instanciated
  TestInstanciate<StreamingTraits::NearestNeighborInterpolationType>();
  TestInstanciate<StreamingTraits::LinearInterpolationType>();
  TestInstanciate<StreamingTraits::BSplineInterpolationType>();
  TestInstanciate<StreamingTraits::GaussianInterpolationType>();
  TestInstanciate<StreamingTraits::CosineInterpolationType>();
  TestInstanciate<StreamingTraits::HammingInterpolationType>();
  TestInstanciate<StreamingTraits::WelchInterpolationType>();
  TestInstanciate<StreamingTraits::LanczosInterpolationType>();
  TestInstanciate<StreamingTraits::BlackmanInterpolationType>();
  TestInstanciate<StreamingTraits::ProlateInterpolationType>();
  TestInstanciate<StreamingTraits::BCOInterpolationType>();

  return EXIT_SUCCESS;
}

int otbStreamingTraitsVectorImage(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{
  // Images definition
  typedef otb::VectorImage<PixelType, Dimension>      ImageType;
  typedef otb::StreamingTraits<ImageType>             StreamingTraits;

  // Check that all internal types can be instanciated
  TestInstanciate<StreamingTraits::NearestNeighborInterpolationType>();
  TestInstanciate<StreamingTraits::LinearInterpolationType>();
  TestInstanciate<StreamingTraits::GaussianInterpolationType>();
  TestInstanciate<StreamingTraits::BCOInterpolationType>();

  return EXIT_SUCCESS;
}
