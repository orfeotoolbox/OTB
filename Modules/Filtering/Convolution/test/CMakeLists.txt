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

set(OTBConvolutionTests
otbConvolutionTestDriver.cxx
otbConvolutionImageFilter.cxx
otbOverlapSaveConvolutionImageFilter.cxx
otbCompareOverlapSaveAndClassicalConvolutionWithGaborFilter.cxx
otbGaborFilterGenerator.cxx
)

add_executable(otbConvolutionTestDriver ${OTBConvolutionTests})
target_link_libraries(otbConvolutionTestDriver ${OTBConvolution-Test_LIBRARIES})
otb_module_target_label(otbConvolutionTestDriver)

# Tests Declaration

otb_add_test(NAME bfTvConvolutionImageFilter COMMAND otbConvolutionTestDriver
  --compare-image ${EPSILON_7}
  ${BASELINE}/bfTvConvolutionImageFilter.tif
  ${TEMP}/bfTvConvolutionImageFilter.tif
  otbConvolutionImageFilter
  ${INPUTDATA}/QB_Suburb.png
  ${TEMP}/bfTvConvolutionImageFilter.tif
  )

if(ITK_USE_FFTWD)

if(MSVC AND (CMAKE_SIZEOF_VOID_P EQUAL "4"))
  message(WARNING "Disable bfTvOverlapSaveConvolutionImageFilter")
else()
otb_add_test(NAME bfTvOverlapSaveConvolutionImageFilter COMMAND otbConvolutionTestDriver
  --compare-image ${EPSILON_7}
  ${BASELINE}/bfTvConvolutionImageFilter.tif
  ${TEMP}/bfTvOverlapSaveConvolutionImageFilter.tif
  otbOverlapSaveConvolutionImageFilter
  ${INPUTDATA}/QB_Suburb.png
  ${TEMP}/bfTvOverlapSaveConvolutionImageFilter.tif
  )
endif()

otb_add_test(NAME bfTvCompareOverlapSaveAndClassicalConvolutionWithGaborFilter COMMAND otbConvolutionTestDriver
  --compare-image ${EPSILON_7}
  ${TEMP}/bfTvCompareConvolutionOutput.tif
  ${TEMP}/bfTvCompareOSConvolutionoutput.tif
  otbCompareOverlapSaveAndClassicalConvolutionWithGaborFilter
  ${INPUTDATA}/QB_PAN_ROI_1000_100.tif
  ${TEMP}/bfTvCompareConvolutionOutput.tif
  ${TEMP}/bfTvCompareOSConvolutionoutput.tif
  32 32 #Radius
  0.02 0.025 # a b
  -45 # theta
  0.0125 0.0125 #u0 v0
  0
  )
endif()

otb_add_test(NAME bfTvGaborFilterGenerator COMMAND otbConvolutionTestDriver
  --compare-image ${EPSILON_7}
  ${BASELINE}/bfGaborFilterGeneratorTest.tif
  ${TEMP}/bfGaborFilterGeneratorTest.tif
  otbGaborFilterGenerator
  ${TEMP}/bfGaborFilterGeneratorTest.tif
  64 64 #Radius
  0.02 0.025 # a b
  -45 # theta
  0.0125 0.0125 #u0 v0
  0
  )
