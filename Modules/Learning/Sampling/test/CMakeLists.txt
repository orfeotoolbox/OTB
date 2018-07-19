#
# Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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

set(OTBSamplingTests
otbSamplingTestDriver.cxx
otbOGRDataToSamplePositionFilterTest.cxx
otbSamplingRateCalculatorTest.cxx
otbOGRDataToClassStatisticsFilterTest.cxx
otbImageSampleExtractorFilterTest.cxx
otbSamplingRateCalculatorListTest.cxx
)

add_executable(otbSamplingTestDriver ${OTBSamplingTests})
target_link_libraries(otbSamplingTestDriver ${OTBSampling-Test_LIBRARIES})
otb_module_target_label(otbSamplingTestDriver)

# Tests Declaration
# ---------------- SamplingRateCalculator ------------------------------------

otb_add_test(NAME leTvSamplingRateCalculator COMMAND otbSamplingTestDriver
  --compare-ascii ${NOTOL}
            ${BASELINE_FILES}/leTvSamplingRateCalculator.txt
            ${TEMP}/leTvSamplingRateCalculator.txt
            otbSamplingRateCalculator 
            ${TEMP}/leTvSamplingRateCalculator.txt)
  
# ----------------- OGRDataToSamplePositionFilter ----------------------------
otb_add_test(NAME leTvOGRDataToSamplePositionFilterPoly COMMAND otbSamplingTestDriver
  otbOGRDataToSamplePositionFilter
  ${INPUTDATA}/variousVectors.sqlite
  0
  ${TEMP}/leTvOGRDataToSamplePositionFilterOutput_Poly.sqlite
  ${BASELINE_FILES}/leTvOGRDataToSamplePositionFilterOutput_Poly.sqlite
  )
  
otb_add_test(NAME leTvOGRDataToSamplePositionFilterPolyPattern COMMAND otbSamplingTestDriver
  otbOGRDataToSamplePositionFilterPattern
  ${INPUTDATA}/variousVectors.sqlite
  ${TEMP}/leTvOGRDataToSamplePositionFilterOutput_PolyPattern.sqlite
  ${BASELINE_FILES}/leTvOGRDataToSamplePositionFilterOutput_PolyPattern.sqlite
  )
  
otb_add_test(NAME leTvOGRDataToSamplePositionFilterLines COMMAND otbSamplingTestDriver
  otbOGRDataToSamplePositionFilter
  ${INPUTDATA}/variousVectors.sqlite
  1
  ${TEMP}/leTvOGRDataToSamplePositionFilterOutput_Lines.sqlite
  ${BASELINE_FILES}/leTvOGRDataToSamplePositionFilterOutput_Lines.sqlite
  )
  
otb_add_test(NAME leTvOGRDataToSamplePositionFilterPoints COMMAND otbSamplingTestDriver
  otbOGRDataToSamplePositionFilter
  ${INPUTDATA}/variousVectors.sqlite
  2
  ${TEMP}/leTvOGRDataToSamplePositionFilterOutput_Points.sqlite
  ${BASELINE_FILES}/leTvOGRDataToSamplePositionFilterOutput_Points.sqlite
  )

# --------------- OGRDataToClassStatisticsFilter -----------------------------
otb_add_test(NAME leTvOGRDataToClassStatisticsFilter COMMAND otbSamplingTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/leTvOGRDataToClassStatisticsFilterOutput.txt
  ${TEMP}/leTvOGRDataToClassStatisticsFilterOutput.txt
  otbOGRDataToClassStatisticsFilter
  ${INPUTDATA}/variousVectors.sqlite
  ${TEMP}/leTvOGRDataToClassStatisticsFilterOutput.txt)

# --------------- ImageSampleExtractorFilter -----------------------------
otb_add_test(NAME leTvImageSampleExtractorFilter COMMAND otbSamplingTestDriver
  --compare-ogr ${EPSILON_6}
  ${BASELINE_FILES}/leTvImageSampleExtractorFilterTest.sqlite
  ${TEMP}/leTvImageSampleExtractorFilterTest.sqlite
  otbImageSampleExtractorFilter
  ${INPUTDATA}/variousVectors.sqlite
  ${TEMP}/leTvImageSampleExtractorFilterTest.sqlite)

otb_add_test(NAME leTvImageSampleExtractorFilterUpdate COMMAND otbSamplingTestDriver
  --compare-ogr ${EPSILON_6}
  ${BASELINE_FILES}/leTvImageSampleExtractorFilterUpdateTest.shp
  ${TEMP}/leTvImageSampleExtractorFilterUpdateTest.shp
  otbImageSampleExtractorFilterUpdate
  ${INPUTDATA}/variousVectors.sqlite
  ${TEMP}/leTvImageSampleExtractorFilterUpdateTest.shp)

# ---------------- SamplingRateCalculatorList ---------------------------------

otb_add_test(NAME leTvSamplingRateCalculatorList COMMAND otbSamplingTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/leTvSamplingRateCalculatorList.txt
  ${TEMP}/leTvSamplingRateCalculatorList.txt
  otbSamplingRateCalculatorList
  ${TEMP}/leTvSamplingRateCalculatorList.txt)
