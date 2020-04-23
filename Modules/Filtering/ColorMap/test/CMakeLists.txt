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

set(OTBColorMapTests
otbColorMapTestDriver.cxx
otbScalarToRainbowRGBPixelFunctor.cxx
)

add_executable(otbColorMapTestDriver ${OTBColorMapTests})
target_link_libraries(otbColorMapTestDriver ${OTBColorMap-Test_LIBRARIES})
otb_module_target_label(otbColorMapTestDriver)

# Tests Declaration

otb_add_test(NAME bfTvScalarToRainbowRGBPixelFunctor COMMAND otbColorMapTestDriver
  --compare-image ${EPSILON_7}
  ${BASELINE}/bfTvScalarToRainbowRGBPixelFunctor.png
  ${TEMP}/bfTvScalarToRainbowRGBPixelFunctor.png
  otbScalarToRainbowRGBPixelFunctor
  ${INPUTDATA}/QB_Suburb.png
  ${TEMP}/bfTvScalarToRainbowRGBPixelFunctor.png
  )



