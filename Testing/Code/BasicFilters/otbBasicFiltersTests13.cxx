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


#include "otbTestMain.h"

void RegisterTests()
{
  REGISTER_TEST(otbBandMathImageFilterNew);
  REGISTER_TEST(otbBandMathImageFilterXNew);
  REGISTER_TEST(otbBandMathImageFilter);
  REGISTER_TEST(otbBandMathImageFilterX);
  REGISTER_TEST(otbBandMathImageFilterWithIdx);
  REGISTER_TEST(otbBandMathImageFilterXWithIdx);
  REGISTER_TEST(otbComplexToIntensityFilterTest);
  REGISTER_TEST(otbRealAndImaginaryImageToComplexImageFilterTest);
  REGISTER_TEST(otbRealImageToComplexImageFilterTest);
  REGISTER_TEST(otbImaginaryImageToComplexImageFilterTest);
  REGISTER_TEST(otbComplexToVectorImageCastFilterNew);
  REGISTER_TEST(otbComplexToVectorImageCastFilterTest);
  REGISTER_TEST(otbLog10ThresholdedImageFilterTest);
  REGISTER_TEST(otbMatrixImageFilterNew);
  REGISTER_TEST(otbMatrixImageFilterTest);
  REGISTER_TEST(otbMaskMuParserFilterNew);
  REGISTER_TEST(otbMaskMuParserFilterTest);
  REGISTER_TEST(otbProjectiveProjectionTestHighSNR);
  REGISTER_TEST(otbPCAImageFilterNewTest);
  REGISTER_TEST(otbPCAImageFilterTest);
  REGISTER_TEST(otbMNFImageFilterNewTest);
  REGISTER_TEST(otbMNFImageFilterTest);
  REGISTER_TEST(otbNAPCAImageFilterNewTest);
  REGISTER_TEST(otbNAPCAImageFilterTest);
  REGISTER_TEST(otbNormalizeVectorImageFilterNewTest);
  REGISTER_TEST(otbNormalizeVectorImageFilterTest);
  REGISTER_TEST(otbLocalActivityVectorImageFilterNewTest);
  REGISTER_TEST(otbLocalActivityVectorImageFilterTest);
  REGISTER_TEST(otbLocalGradientVectorImageFilterNewTest);
  REGISTER_TEST(otbLocalGradientVectorImageFilterTest);
  REGISTER_TEST(otbHorizontalSobelVectorImageFilterNewTest);
  REGISTER_TEST(otbHorizontalSobelVectorImageFilterTest);
  REGISTER_TEST(otbVerticalSobelVectorImageFilterNewTest);
  REGISTER_TEST(otbVerticalSobelVectorImageFilterTest);
  REGISTER_TEST(otbSobelVectorImageFilterNewTest);
  REGISTER_TEST(otbSobelVectorImageFilterTest);
  REGISTER_TEST(otbFastICAInternalOptimizerVectorImageFilterNewTest);
  REGISTER_TEST(otbFastICAImageFilterNewTest);
  REGISTER_TEST(otbFastICAImageFilterTest);
  REGISTER_TEST(otbVectorImageToMatrixNewTest);
  REGISTER_TEST(otbVectorImageToMatrixTest);
  REGISTER_TEST(otbProjectiveProjectionNew);
  REGISTER_TEST(otbProjectiveProjectionTestHighSNR);
  REGISTER_TEST(otbConcatenateScalarValueImageFilterNew);
  REGISTER_TEST(otbConcatenateScalarValueImageFilterTest);
  REGISTER_TEST(otbAngularProjectionImageFilterNew);
//  REGISTER_TEST(otbAngularProjectionImageFilterTest);
  REGISTER_TEST(otbAngularProjectionSetImageFilterNew);
//  REGISTER_TEST(otbAngularProjectionSetImageFilterTest);
  REGISTER_TEST(otbSparseWvltToAngleMapperListFilterNew);
//  REGISTER_TEST(otbSparseWvltToAngleMapperListFilterTest);

}
