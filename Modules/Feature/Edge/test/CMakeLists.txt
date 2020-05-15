#
# Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
#
# This file is part of Orfeo Toolbox
#
#     https://www.orfeo-toolbox.org/
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

otb_module_test()

set(OTBEdgeTests
otbEdgeTestDriver.cxx
otbEdgeDetectorImageFilter.cxx
otbHorizontalSobelVectorImageFilter.cxx
otbAsymmetricFusionOfLineDetector.cxx
otbAssociativeSymmetricalSum.cxx
otbPersistentVectorizationFilter.cxx
otbEdgeDensityImageFilter.cxx
otbLineCorrelationDetector.cxx
otbSobelVectorImageFilter.cxx
otbPixelSuppressionByDirection.cxx
otbLineRatioDetector.cxx
otbTouziEdgeDetectorDirection.cxx
otbVerticalSobelVectorImageFilter.cxx
otbStreamingLineSegmentDetector.cxx
otbTouziEdgeDetector.cxx
otbLineRatioDetectorLinear.cxx
otbLineSegmentDetector.cxx
otbLineCorrelationDetectorLinear.cxx
)

add_executable(otbEdgeTestDriver ${OTBEdgeTests})
target_link_libraries(otbEdgeTestDriver ${OTBEdge-Test_LIBRARIES})
otb_module_target_label(otbEdgeTestDriver)

# Tests Declaration

otb_add_test(NAME bfTvEdgeDetectorImageFilter COMMAND otbEdgeTestDriver
  --compare-image ${NOTOL}
  ${BASELINE}/bfTvEdgeDetectorImageFilter.tif
  ${TEMP}/bfTvEdgeDetectorImageFilter.tif
  otbEdgeDetectorImageFilter
  ${INPUTDATA}/poupees_sub_c1.png
  ${TEMP}/bfTvEdgeDetectorImageFilter.tif
  0   # lower threshold
  255 # upper  threshold
  )

otb_add_test(NAME bfTvHorizontalSobelVectorImageFilter COMMAND otbEdgeTestDriver
  --compare-image ${EPSILON_7}
  ${BASELINE}/bfTvHorizontalSobelVectorImageFilter.tif
  ${TEMP}/bfTvHorizontalSobelVectorImageFilter.tif
  otbHorizontalSobelVectorImageFilterTest
  ${INPUTDATA}/cupriteSubHsi.tif
  ${TEMP}/bfTvHorizontalSobelVectorImageFilter.tif)

otb_add_test(NAME feTvAsymmetricFusionOfLineDetector COMMAND otbEdgeTestDriver
  --compare-image ${EPSILON_8}  ${BASELINE}/feFiltreAsymmetricFusion_amst_2_3.tif
  ${TEMP}/feFiltreAsymmetricFusion_amst_2_3.tif
  otbAsymmetricFusionOfLineDetector
  ${INPUTDATA}/amst.png
  ${TEMP}/feFiltreAsymmetricFusion_amst_2_3.tif
  2 3)

otb_add_test(NAME feTvAssociativeSymmetricalSum COMMAND otbEdgeTestDriver
  --compare-image ${EPSILON_8}  ${BASELINE}/feFiltreASS_amst_2_3.tif
  ${TEMP}/feFiltreASS_amst_2_3.tif
  otbAssociativeSymmetricalSum
  ${BASELINE}/feFiltreLineRatioLinear_amst_2_3.tif
  ${BASELINE}/feFiltreLineCorrelationLinear_amst_2_3.tif
  ${TEMP}/feFiltreASS_amst_2_3.tif)

otb_add_test(NAME bfTvPersistentVectorizationImageFilter COMMAND otbEdgeTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/bfTvPersistentVectorizationImageFilterOutput.txt
  ${TEMP}/bfTvPersistentVectorizationImageFilterOutput.txt
  otbPersistentVectorizationFilter
  ${INPUTDATA}/labelImage_UnsignedChar.tif
  ${TEMP}/bfTvPersistentVectorizationImageFilterOutput.txt
  )

otb_add_test(NAME bfTvEdgeDensityImageFilter COMMAND otbEdgeTestDriver
  --compare-image ${EPSILON_7}
  ${BASELINE}/bfTvEdgeDensityImageFilterOutputImage.tif
  ${TEMP}/bfTvEdgeDensityImageFilterOutputImage.tif
  otbEdgeDensityImageFilter
  ${INPUTDATA}/scene.png
  ${TEMP}/bfTvEdgeDensityImageFilterOutputImage.tif
  1 # radius
  15 3  1. 0.01  #Canny Parameters
  )

otb_add_test(NAME feTvLineCorrelation COMMAND otbEdgeTestDriver
  --compare-image ${EPSILON_8}  ${BASELINE}/feFiltreLineCorrelation_amst_2_3.tif
  ${TEMP}/feFiltreLineCorrelation_amst_2_3.tif
  otbLineCorrelationDetector
  ${INPUTDATA}/amst.png
  ${TEMP}/feFiltreLineCorrelation_amst_2_3.tif
  2 3)

otb_add_test(NAME bfTvSobelVectorImageFilter COMMAND otbEdgeTestDriver
  --compare-image ${EPSILON_7}
  ${BASELINE}/bfTvSobelVectorImageFilter.tif
  ${TEMP}/bfTvSobelVectorImageFilter.tif
  otbSobelVectorImageFilterTest
  ${INPUTDATA}/cupriteSubHsi.tif
  ${TEMP}/bfTvSobelVectorImageFilter.tif)

otb_add_test(NAME feTvPixelSuppressionByDirection COMMAND otbEdgeTestDriver
  --compare-image ${NOTOL}  ${BASELINE}/feFiltrePixelSuppr_ImageLine_2_0_3.tif
  ${TEMP}/feFiltrePixelSuppr_ImageLine_2_0_3.tif
  otbPixelSuppressionByDirection
  ${INPUTDATA}/ImageLine_hd.bsq.hdr
  ${INPUTDATA}/ImageLineDir.bsq.hdr
  ${TEMP}/feFiltrePixelSuppr_ImageLine_2_0_3.tif
  2 0.3)

otb_add_test(NAME feTvLineRatio COMMAND otbEdgeTestDriver
  --compare-image ${EPSILON_8}  ${BASELINE}/feFiltreLineRatio_amst_2_3.tif
  ${TEMP}/feFiltreLineRatio_amst_2_3.tif
  otbLineRatioDetector
  ${INPUTDATA}/amst.png
  ${TEMP}/feFiltreLineRatio_amst_2_3.tif
  2 3)

otb_add_test(NAME feTvTouziDir COMMAND otbEdgeTestDriver
  --compare-image ${EPSILON_8}  ${BASELINE}/feFiltreTouzi_amst_dir_3.tif
  ${TEMP}/feFiltreTouzi_amst_dir_3.tif
  otbTouziEdgeDetectorDirection
  ${INPUTDATA}/amst.png
  ${TEMP}/feFiltreTouzi_amst_dir_3.tif
  3)

otb_add_test(NAME bfTvVerticalSobelVectorImageFilter COMMAND otbEdgeTestDriver
  --compare-image ${EPSILON_7}
  ${BASELINE}/bfTvVerticalSobelVectorImageFilter.tif
  ${TEMP}/bfTvVerticalSobelVectorImageFilter.tif
  otbVerticalSobelVectorImageFilterTest
  ${INPUTDATA}/cupriteSubHsi.tif
  ${TEMP}/bfTvVerticalSobelVectorImageFilter.tif)

otb_add_test(NAME feTvStreamingLineSegmentDetector10 COMMAND otbEdgeTestDriver
  --compare-ogr  ${EPSILON_8}
  ${BASELINE_FILES}/feTvStreamingLineSegmentDetectorOutput10.shp
  ${TEMP}/feTvStreamingLineSegmentDetectorOutput10.shp
  otbStreamingLineSegmentDetector
  ${INPUTDATA}/scene.png
  ${TEMP}/feTvStreamingLineSegmentDetectorOutput10.shp
  10
  )

otb_add_test(NAME feTvStreamingLineSegmentDetector1000 COMMAND otbEdgeTestDriver
  --compare-ogr  ${EPSILON_8}
  ${BASELINE_FILES}/feTvStreamingLineSegmentDetectorOutput1000.shp
  ${TEMP}/feTvStreamingLineSegmentDetectorOutput1000.shp
  otbStreamingLineSegmentDetector
  ${INPUTDATA}/scene.png
  ${TEMP}/feTvStreamingLineSegmentDetectorOutput1000.shp
  1000
  )

otb_add_test(NAME feTvTouzi COMMAND otbEdgeTestDriver
  --compare-image ${EPSILON_8}  ${BASELINE}/feFiltreTouzi_amst_3.tif
  ${TEMP}/feFiltreTouzi_amst_3.tif
  otbTouziEdgeDetector
  ${INPUTDATA}/amst.png
  ${TEMP}/feFiltreTouzi_amst_3.tif
  3)

otb_add_test(NAME feTvLineRatioLinearAmsters1 COMMAND otbEdgeTestDriver
  --compare-n-images ${EPSILON_8} 2
  ${BASELINE}/feFiltreLineRatioLinear_amst_ers1_ima_extrait.tif
  ${TEMP}/feFiltreLineRatioLinear_amst_ers1_ima_extrait.tif
  ${BASELINE}/feFiltreLineRatioLinear_amst_dir_ers1_ima_extrait.tif
  ${TEMP}/feFiltreLineRatioLinear_amst_dir_ers1_ima_extrait.tif
  otbLineRatioDetectorLinear
  ${INPUTDATA}/amst_ers1.ima.extrait.419_187_70_66.raw.hdr
  ${TEMP}/feFiltreLineRatioLinear_amst_ers1_ima_extrait.tif
  ${TEMP}/feFiltreLineRatioLinear_amst_dir_ers1_ima_extrait.tif
  1 2)

otb_add_test(NAME feTvLineRatioLinear COMMAND otbEdgeTestDriver
  --compare-n-images ${EPSILON_8} 2
  ${BASELINE}/feFiltreLineRatioLinear_amst_2_3.tif
  ${TEMP}/feFiltreLineRatioLinear_amst_2_3.tif
  ${BASELINE}/feFiltreLineRatioLinear_amst_dir_2_3.tif
  ${TEMP}/feFiltreLineRatioLinear_amst_dir_2_3.tif
  otbLineRatioDetectorLinear
  ${INPUTDATA}/amst.png
  ${TEMP}/feFiltreLineRatioLinear_amst_2_3.tif
  ${TEMP}/feFiltreLineRatioLinear_amst_dir_2_3.tif
  2 3)

otb_add_test(NAME feTvLineSegmentDetector COMMAND otbEdgeTestDriver
  --compare-ogr  ${EPSILON_8}
  ${BASELINE_FILES}/feTvLineSegmentDetectorOutput.shp
  ${TEMP}/feTvLineSegmentDetectorOutput.shp
  otbLineSegmentDetector
  ${INPUTDATA}/scene.png
  ${TEMP}/feTvLineSegmentDetectorOutput.shp
  )

otb_add_test(NAME feTvLineCorrelationLinear COMMAND otbEdgeTestDriver
  --compare-n-images ${EPSILON_8} 2
  ${BASELINE}/feFiltreLineCorrelationLinear_amst_2_3.tif
  ${TEMP}/feFiltreLineCorrelationLinear_amst_2_3.tif
  ${BASELINE}/feFiltreLineCorrelationLinear_amst_dir_2_3.tif
  ${TEMP}/feFiltreLineCorrelationLinear_amst_dir_2_3.tif
  otbLineCorrelationDetectorLinear
  ${INPUTDATA}/amst.png
  ${TEMP}/feFiltreLineCorrelationLinear_amst_2_3.tif
  ${TEMP}/feFiltreLineCorrelationLinear_amst_dir_2_3.tif
  2 3)
