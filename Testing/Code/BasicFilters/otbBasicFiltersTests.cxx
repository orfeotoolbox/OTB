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

// this file defines the otbCommonTest for the test driver
// and all it expects is that you have a function called RegisterTests
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include <iostream>
#include "otbTestMain.h" 

void RegisterTests()
{
REGISTER_TEST(otbLeeFilter);
REGISTER_TEST(otbFrostFilterNew);
REGISTER_TEST(otbFrostFilterTest);
REGISTER_TEST(otbFrostFilter);
REGISTER_TEST(otbImageToPointSetFilterTest);
REGISTER_TEST(otbOpeningClosingMorphologicalFilterNew);
REGISTER_TEST(otbOpeningClosingMorphologicalFilter);
REGISTER_TEST(otbClosingOpeningMorphologicalFilterNew);
REGISTER_TEST(otbClosingOpeningMorphologicalFilter);
REGISTER_TEST(otbBinaryImageMinimalBoundingRegionCalculatorNew);
REGISTER_TEST(otbBinaryImageMinimalBoundingRegionCalculator);
REGISTER_TEST(otbLogPolarTransformNew);
REGISTER_TEST(otbLogPolarTransform);
REGISTER_TEST(otbLogPolarTransformResample);
REGISTER_TEST(otbInverseLogPolarTransformNew);
REGISTER_TEST(otbInverseLogPolarTransform);
REGISTER_TEST(otbInverseLogPolarTransformResample);
REGISTER_TEST(otbStreamingShrinkImageFilterNew);
REGISTER_TEST(otbStreamingShrinkImageFilter);
REGISTER_TEST(otbSpatialObjectToImageDrawingFilterNew);
REGISTER_TEST(otbSpatialObjectToImageDrawingFilter);
REGISTER_TEST(otbImportGeoInformationImageFilterNew);
REGISTER_TEST(otbImportGeoInformationImageFilter);
REGISTER_TEST(otbSpectralAngleDistanceImageFilterNew);
REGISTER_TEST(otbSpectralAngleDistanceImageFilter);
REGISTER_TEST(otbVectorRescaleIntensityImageFilterNew);
REGISTER_TEST(otbVectorRescaleIntensityImageFilter);
REGISTER_TEST(otbVectorImageToImageListFilterNew);
REGISTER_TEST(otbVectorImageToImageListFilter);
REGISTER_TEST(otbImageListToVectorImageFilterNew);
REGISTER_TEST(otbImageListToVectorImageFilter);
REGISTER_TEST(otbImageListToImageListApplyFilterNew);
REGISTER_TEST(otbImageListToImageListApplyFilter);
REGISTER_TEST(otbPerBandVectorImageFilterNew);
REGISTER_TEST(otbPerBandVectorImageFilterWithMeanFilter);
REGISTER_TEST(otbPerBandVectorImageFilterWithSobelFilter);
}
