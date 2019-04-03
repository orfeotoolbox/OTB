#
# Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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

set(OTBIndicesTests
otbIndicesTestDriver.cxx
otbNDVIDataNodeFeatureFunction.cxx
otbLandsatTMIndexNDSITest.cxx
otbLandsatTMIndexBIOTest.cxx
otbLAIFromReflectancesLinearFunctorTest.cxx
otbLandsatTMIndexMIR2Test.cxx
otbLandsatTMIndexNDVITest.cxx
otbLAIFromNDVILogarithmicFunctorTest.cxx
otbLandsatTMIndexVisTest.cxx
otbWaterSqrtSpectralAngleImageFilter.cxx
otbLandsatTMIndexBrightTest.cxx
otbLandsatTMBrightTest.cxx
otbLandsatTMThinCloudTest.cxx
otbLandsatTMIndexMIRTIRTest.cxx
otbLandsatTMIndexMIR1Test.cxx
otbLandsatTMVegetationTest.cxx
otbLandsatTMIndexNDBBBITest.cxx
otbLandsatTMIndexTIRTest.cxx
otbLandsatTMLinguisticLabelsTest.cxx
otbLandsatTMIndexNDSIVisTest.cxx
otbLandsatTMIndexNIRTest.cxx
otbLandsatTMKernelSpectralRules.cxx
otbLandsatTMIndexNDBSITest.cxx
otbLandsatTMThickCloudTest.cxx
)

add_executable(otbIndicesTestDriver ${OTBIndicesTests})
target_link_libraries(otbIndicesTestDriver ${OTBIndices-Test_LIBRARIES})
otb_module_target_label(otbIndicesTestDriver)

# Tests Declaration

otb_add_test(NAME bfTvNDVIDataNodeFeatureFunction_Polygon COMMAND otbIndicesTestDriver
  --compare-ogr ${NOTOL}
  ${BASELINE_FILES}/bfTvNDVIDataNodeFeatureFunctionOutput_Polygon.shp
  ${TEMP}/bfTvNDVIDataNodeFeatureFunctionOutput_Polygon.shp
  otbNDVIDataNodeFeatureFunction
  ${INPUTDATA}/Dempster-Shafer/ROI_QB_TOULOUSE_BUILDINGS.shp
  ${INPUTDATA}/Dempster-Shafer/ROI_QB_TOULOUSE.TIF
  ${INPUTDATA}/DEM/srtm_directory
  ${TEMP}/bfTvNDVIDataNodeFeatureFunctionOutput_Polygon.shp
  0
  )

otb_add_test(NAME bfTvNDVIDataNodeFeatureFunction_Line COMMAND otbIndicesTestDriver
  --compare-ogr ${NOTOL}
  ${BASELINE_FILES}/bfTvNDVIDataNodeFeatureFunctionOutput_Line.shp
  ${TEMP}/bfTvNDVIDataNodeFeatureFunctionOutput_Line.shp
  otbNDVIDataNodeFeatureFunction
  ${INPUTDATA}/Dempster-Shafer/ROI_QB_TOULOUSE_ROADS.shp
  ${INPUTDATA}/Dempster-Shafer/ROI_QB_TOULOUSE.TIF
  ${INPUTDATA}/DEM/srtm_directory
  ${TEMP}/bfTvNDVIDataNodeFeatureFunctionOutput_Line.shp
  0
  )


otb_add_test(NAME raTvLandsatTMIndexNDSITest COMMAND otbIndicesTestDriver
  otbLandsatTMIndexNDSI
  3   #TM1
  4   #TM2
  8   #TM3
  21  #TM4
  33  #TM5
  41  #TM61
  8   #TM62
  21  #TM7
  )

otb_add_test(NAME raTvLandsatTMIndexBIOTest COMMAND otbIndicesTestDriver
  otbLandsatTMIndexBIO
  3   #TM1
  4   #TM2
  8   #TM3
  21  #TM4
  33  #TM5
  41  #TM61
  8   #TM62
  21  #TM7
  )

otb_add_test(NAME raTvLAIFromReflectancesLinearFunctorTest COMMAND otbIndicesTestDriver
  otbLAIFromReflectancesLinear
  3   # red
  4   # nir
  -18   # red coef
  13   # nir coef
  )

otb_add_test(NAME raTvLandsatTMIndexMIR2Test COMMAND otbIndicesTestDriver
  otbLandsatTMIndexMIR2
  3   #TM1
  4   #TM2
  8   #TM3
  21  #TM4
  33  #TM5
  41  #TM61
  8   #TM62
  21  #TM7
  )

otb_add_test(NAME raTvLandsatTMIndexNDVITest COMMAND otbIndicesTestDriver
  otbLandsatTMIndexNDVI
  3   #TM1
  4   #TM2
  8   #TM3
  21  #TM4
  33  #TM5
  41  #TM61
  8   #TM62
  21  #TM7
  )


otb_add_test(NAME raTvLAIFromNDVILogarithmicFunctorTest COMMAND otbIndicesTestDriver
  otbLAIFromNDVILogarithmic
  3   # red
  4   # nir
  0.12   # ndvi soil
  0.91   # ndvi infinity
  0.70   # extinction coefficient
  )

otb_add_test(NAME raTvLandsatTMIndexVisTest COMMAND otbIndicesTestDriver
  otbLandsatTMIndexVis
  3   #TM1
  4   #TM2
  8   #TM3
  21  #TM4
  33  #TM5
  41  #TM61
  8   #TM62
  21  #TM7
  )

otb_add_test(NAME raTvWaterSqrtSpectralAngleImageFilter COMMAND otbIndicesTestDriver
  --compare-image ${EPSILON_12}   ${BASELINE}/raWater_SqrtSpectralAngle.tif
  ${TEMP}/raWater_SqrtSpectralAngle.tif
  otbWaterSqrtSpectralAngleImageFilter
  ${INPUTDATA}/qb_RoadExtract2sub200x200.tif
  ${TEMP}/raWater_SqrtSpectralAngle.tif
  0 1 2 3
  )

otb_add_test(NAME raTvLandsatTMIndexBrightTest COMMAND otbIndicesTestDriver
  otbLandsatTMIndexBright
  3   #TM1
  4   #TM2
  8   #TM3
  21  #TM4
  33  #TM5
  41  #TM61
  8   #TM62
  21  #TM7
  )

otb_add_test(NAME raTvLandsatTMBrightTest COMMAND otbIndicesTestDriver
  otbLandsatTMBrightTest
  ${INPUTDATA}/L5Strip.tif
  ${TEMP}/raTvLandsatTMBrightTest_cloudImage.tif
  )

otb_add_test(NAME raTvLandsatTMThinCloudTest COMMAND otbIndicesTestDriver
  otbLandsatTMThinCloudTest
  ${INPUTDATA}/L5Strip.tif
  ${TEMP}/raTvLandsatTMThinCloudTest_cloudImage.tif
  )


otb_add_test(NAME raTvLandsatTMIndexMIRTIRTest COMMAND otbIndicesTestDriver
  otbLandsatTMIndexMIRTIR
  3   #TM1
  4   #TM2
  8   #TM3
  21  #TM4
  33  #TM5
  41  #TM61
  8   #TM62
  21  #TM7
  )

otb_add_test(NAME raTvLandsatTMIndexMIR1Test COMMAND otbIndicesTestDriver
  otbLandsatTMIndexMIR1
  3   #TM1
  4   #TM2
  8   #TM3
  21  #TM4
  33  #TM5
  41  #TM61
  8   #TM62
  21  #TM7
  )


otb_add_test(NAME raTvLandsatTMVegetationTest COMMAND otbIndicesTestDriver
  otbLandsatTMVegetationTest
  ${INPUTDATA}/L5Strip.tif
  ${TEMP}/raTvLandsatTMVegetationTest_Image.tif
  )

otb_add_test(NAME raTvLandsatTMIndexNDBBBITest COMMAND otbIndicesTestDriver
  otbLandsatTMIndexNDBBBI
  3   #TM1
  4   #TM2
  8   #TM3
  21  #TM4
  33  #TM5
  41  #TM61
  8   #TM62
  21  #TM7
  )

otb_add_test(NAME raTvLandsatTMIndexTIRTest COMMAND otbIndicesTestDriver
  otbLandsatTMIndexTIR
  3   #TM1
  4   #TM2
  8   #TM3
  21  #TM4
  33  #TM5
  41  #TM61
  8   #TM62
  21  #TM7
  )

otb_add_test(NAME raTvLandsatTMLinguisticLabelsTest COMMAND otbIndicesTestDriver
  otbLandsatTMLinguisticLabels
  3   #TM1
  4   #TM2
  8   #TM3
  21  #TM4
  33  #TM5
  41  #TM61
  8   #TM62
  21  #TM7
  )

otb_add_test(NAME raTvLandsatTMLinguisticLabelsTest2 COMMAND otbIndicesTestDriver
  otbLandsatTMLinguisticLabels
  3   #TM1
  4   #TM2
  80   #TM3
  100  #TM4
  33  #TM5
  1  #TM61
  8   #TM62
  51  #TM7
  )

otb_add_test(NAME raTvLandsatTMIndexNDSIVisTest COMMAND otbIndicesTestDriver
  otbLandsatTMIndexNDSIVis
  3   #TM1
  4   #TM2
  8   #TM3
  21  #TM4
  33  #TM5
  41  #TM61
  8   #TM62
  21  #TM7
  )

otb_add_test(NAME raTvLandsatTMIndexNIRTest COMMAND otbIndicesTestDriver
  otbLandsatTMIndexNIR
  3   #TM1
  4   #TM2
  8   #TM3
  21  #TM4
  33  #TM5
  41  #TM61
  8   #TM62
  21  #TM7
  )

otb_add_test(NAME raTvLandsatTMKernelSpectralRulesTest2 COMMAND otbIndicesTestDriver
  otbLandsatTMKernelSpectralRules
  49   #TM1
  50   #TM2
  70   #TM3
  100  #TM4
  69  #TM5
  0  #TM61
  0   #TM62
  0  #TM7
  )

otb_add_test(NAME raTvLandsatTMKernelSpectralRulesTest COMMAND otbIndicesTestDriver
  otbLandsatTMKernelSpectralRules
  63   #TM1
  64   #TM2
  57   #TM3
  150  #TM4
  33  #TM5
  1  #TM61
  8   #TM62
  51  #TM7
  )

otb_add_test(NAME raTvLandsatTMKernelSpectralRulesWithImageTest COMMAND otbIndicesTestDriver
  otbLandsatTMKernelSpectralRulesWithImage
  ${INPUTDATA}/L5Strip.tif
  )

otb_add_test(NAME raTvLandsatTMIndexNDBSITest COMMAND otbIndicesTestDriver
  otbLandsatTMIndexNDBSI
  3   #TM1
  4   #TM2
  8   #TM3
  21  #TM4
  33  #TM5
  41  #TM61
  8   #TM62
  21  #TM7
  )

otb_add_test(NAME raTvLandsatTMThickCloudTest COMMAND otbIndicesTestDriver
  otbLandsatTMThickCloudTest
  ${INPUTDATA}/L5Strip.tif
  ${TEMP}/raTvLandsatTMThickCloudTest_cloudImage.tif
  )

