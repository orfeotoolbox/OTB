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

set(OTBDensityTests
otbDensityTestDriver.cxx
otbPointSetDensityGaussianFunctionTest.cxx
otbPointSetDensityEpanechnikovFunctionTest.cxx
otbKeyPointDensityImageFilterTest.cxx
otbPointSetToDensityImageFilterTest.cxx
otbPointSetDensityFunctionTest.cxx
)

add_executable(otbDensityTestDriver ${OTBDensityTests})
target_link_libraries(otbDensityTestDriver ${OTBDensity-Test_LIBRARIES})
otb_module_target_label(otbDensityTestDriver)

# Tests Declaration



otb_add_test(NAME bfTvPointSetDensityGaussianFunctionTest COMMAND otbDensityTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/bfTvPointSetDensityGaussianFunctionOutputAscii.txt
  ${TEMP}/bfTvPointSetDensityGaussianFunctionOutputAscii.txt
  otbPointSetDensityGaussianFunctionTest
  ${TEMP}/bfTvPointSetDensityGaussianFunctionOutputAscii.txt
  )


otb_add_test(NAME bfTvPointSetDensityEpanechnikovFunctionTest COMMAND otbDensityTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/bfTvPointSetDensityEpanechnikovFunctionOutputAscii.txt
  ${TEMP}/bfTvPointSetDensityEpanechnikovFunctionOutputAscii.txt
  otbPointSetDensityEpanechnikovFunctionTest
  ${TEMP}/bfTvPointSetDensityEpanechnikovFunctionOutputAscii.txt
  )

otb_add_test(NAME bfTvKeyPointDensityImageFilterOutputImage COMMAND otbDensityTestDriver
  --compare-image ${NOTOL}
  ${BASELINE}/bfTvKeyPointDensityImageFilterOutputImage.tif
  ${TEMP}/bfTvKeyPointDensityImageFilterOutputImage.tif
  otbKeyPointDensityImageFilterTest
  ${INPUTDATA}/QB_Suburb.png
  ${TEMP}/bfTvKeyPointDensityImageFilterOutputImage.tif
  3 5 10
  )


otb_add_test(NAME bfTvPointSetToDensityImageFilterTest COMMAND otbDensityTestDriver
  --compare-image ${NOTOL}
  ${BASELINE}/bfTvPointSetToDensityImageFilterOutputImage.tif
  ${TEMP}/bfTvPointSetToDensityImageFilterOutputImage.tif
  otbPointSetToDensityImageFilterTest
  ${INPUTDATA}/QB_Suburb.png
  ${TEMP}/bfTvPointSetToDensityImageFilterOutputImage.tif
  3 5 10
  )

otb_add_test(NAME bfTvPointSetDensityFunctionTest COMMAND otbDensityTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/bfTvPointSetDensityFunctionOutputAscii.txt
  ${TEMP}/bfTvPointSetDensityFunctionOutputAscii.txt
  otbPointSetDensityFunctionTest
  ${TEMP}/bfTvPointSetDensityFunctionOutputAscii.txt
  )

