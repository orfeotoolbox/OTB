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

// this file defines the otbMultiScaleTest for the test driver
// and all it expects is that you have a function called RegisterTests
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif
#include <iostream>
#include "otbTestMain.h"

void RegisterTests()
{
REGISTER_TEST(MathematicalMorphologyBinaryFiltersTest);
REGISTER_TEST(DanielssonDistanceMapImageFilterTest);
REGISTER_TEST(MeanImageFilterTest);
REGISTER_TEST(ThresholdImageFilterTest);
REGISTER_TEST(GradientMagnitudeRecursiveGaussianImageFilterTest);
REGISTER_TEST(GradientAnisotropicDiffusionImageFilterTest);
REGISTER_TEST(GradientMagnitudeImageFilterTest);
REGISTER_TEST(MathematicalMorphologyGrayscaleFiltersTest);
REGISTER_TEST(LaplacianRecursiveGaussianImageFilter1Test);
REGISTER_TEST(LaplacianRecursiveGaussianImageFilter2Test);
REGISTER_TEST(CannyEdgeDetectionImageFilterTest);
REGISTER_TEST(DiscreteGaussianImageFilterTest);
REGISTER_TEST(MedianImageFilterTest);
REGISTER_TEST(BinaryThresholdImageFilterTest);
REGISTER_TEST(DerivativeImageFilterTest);
REGISTER_TEST(RationalQuotientResampleImageFilterTest);
}

#undef main
#define main MathematicalMorphologyBinaryFiltersTest
#include "MathematicalMorphologyBinaryFilters.cxx"

#undef main
#define main DanielssonDistanceMapImageFilterTest
#include "DanielssonDistanceMapImageFilter.cxx"

#undef main
#define main MeanImageFilterTest
#include "MeanImageFilter.cxx"

#undef main
#define main ThresholdImageFilterTest
#include "ThresholdImageFilter.cxx"

#undef main
#define main GradientMagnitudeRecursiveGaussianImageFilterTest
#include "GradientMagnitudeRecursiveGaussianImageFilter.cxx"

#undef main
#define main GradientAnisotropicDiffusionImageFilterTest
#include "GradientAnisotropicDiffusionImageFilter.cxx"

#undef main
#define main GradientMagnitudeImageFilterTest
#include "GradientMagnitudeImageFilter.cxx"

#undef main
#define main MathematicalMorphologyGrayscaleFiltersTest
#include "MathematicalMorphologyGrayscaleFilters.cxx"

#undef main
#define main LaplacianRecursiveGaussianImageFilter1Test
#include "LaplacianRecursiveGaussianImageFilter1.cxx"

#undef main
#define main LaplacianRecursiveGaussianImageFilter2Test
#include "LaplacianRecursiveGaussianImageFilter2.cxx"

#undef main
#define main CannyEdgeDetectionImageFilterTest
#include "CannyEdgeDetectionImageFilter.cxx"

#undef main
#define main DiscreteGaussianImageFilterTest
#include "DiscreteGaussianImageFilter.cxx"

#undef main
#define main MedianImageFilterTest
#include "MedianImageFilter.cxx"

#undef main
#define main BinaryThresholdImageFilterTest
#include "BinaryThresholdImageFilter.cxx"

#undef main
#define main DerivativeImageFilterTest
#include "DerivativeImageFilter.cxx"

#undef main
#define main RationalQuotientResampleImageFilterTest
#include "RationalQuotientResampleImageFilter.cxx"

