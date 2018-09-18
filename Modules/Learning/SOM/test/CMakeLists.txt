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

set(OTBSOMTests
otbSOMTestDriver.cxx
otbSOM.cxx
otbSOMImageClassificationFilter.cxx
otbSOMActivationBuilder.cxx
otbSOMWithMissingValue.cxx
otbSOMMap.cxx
otbPeriodicSOM.cxx
otbSOMClassifier.cxx
otbSOMbasedImageFilter.cxx
)

add_executable(otbSOMTestDriver ${OTBSOMTests})
target_link_libraries(otbSOMTestDriver ${OTBSOM-Test_LIBRARIES})
otb_module_target_label(otbSOMTestDriver)

# Tests Declaration

otb_add_test(NAME leTvSOM COMMAND otbSOMTestDriver
  --compare-image ${EPSILON_10}
  ${BASELINE}/leSOMPoupeesSubOutputMap1.hdr
  ${TEMP}/leSOMPoupeesSubOutputMap1.hdr
  otbSOM
  ${INPUTDATA}/poupees_sub.png
  ${TEMP}/leSOMPoupeesSubOutputMap1.hdr
  32 32 10 10 5 1.0 0.1 0)

otb_add_test(NAME leTvSOMImageClassificationFilter COMMAND otbSOMTestDriver
  --compare-image ${NOTOL}
  ${BASELINE}/leSOMPoupeesClassified.hdr
  ${TEMP}/leSOMImageClassificationFilterOutput.hdr
  otbSOMImageClassificationFilter
  ${INPUTDATA}/poupees_sub.png
  ${BASELINE}/leSOMPoupeesSubOutputMap1.hdr
  ${TEMP}/leSOMImageClassificationFilterOutput.hdr
  )

otb_add_test(NAME leTvSOMActivationBuilder COMMAND otbSOMTestDriver
  --compare-image ${NOTOL}
  ${BASELINE}/leSOMPoupeesSubActivationMap1.hdr
  ${TEMP}/leSOMPoupeesSubActivationMap1.tif
  otbSOMActivationBuilder
  ${INPUTDATA}/poupees_sub.png
  ${BASELINE}/leSOMPoupeesSubOutputMap1.hdr
  ${TEMP}/leSOMPoupeesSubActivationMap1.tif
  32 32 10 10 5 1.0 0.1 128)

otb_add_test(NAME leTvSOMWithMissingValue COMMAND otbSOMTestDriver
  --compare-image ${EPSILON_10}
  ${BASELINE}/leSOMWithMissingValue.hdr
  ${TEMP}/leSOMWithMissingValue.hdr
  otbSOMWithMissingValueTest
  ${INPUTDATA}/poupees_sub.png
  ${TEMP}/leSOMWithMissingValue.hdr
  32 32 10 10 5 1.0 0.1 0)

otb_add_test(NAME leTvSOMMap COMMAND otbSOMTestDriver
  otbSOMMap)

otb_add_test(NAME leTvPeriodicSOM COMMAND otbSOMTestDriver
  --compare-image ${EPSILON_10}
  ${BASELINE}/lePeriodicSOMPoupeesSubOutputMap1.hdr
  ${TEMP}/lePeriodicSOMPoupeesSubOutputMap1.hdr
  otbPeriodicSOMTest
  ${INPUTDATA}/poupees_sub.png
  ${TEMP}/lePeriodicSOMPoupeesSubOutputMap1.hdr
  32 32 10 10 5 1.0 0.1 0)

otb_add_test(NAME leTvSOMClassifier COMMAND otbSOMTestDriver
  --compare-image ${NOTOL}
  ${BASELINE}/leSOMPoupeesClassified.hdr
  ${TEMP}/leSOMPoupeesClassified.hdr
  otbSOMClassifier
  ${INPUTDATA}/poupees_sub.png
  ${BASELINE}/leSOMPoupeesSubOutputMap1.hdr
  ${TEMP}/leSOMPoupeesClassified.hdr
  )

otb_add_test(NAME leTvSOMbasedImageFilter COMMAND otbSOMTestDriver
  --compare-image ${NOTOL}
  ${BASELINE}/leSOMbasedImageFilterOutput.hdr
  ${TEMP}/leSOMbasedImageFilterOutput.hdr
  otbSOMbasedImageFilterTest
  ${INPUTDATA}/poupees_sub.png
  ${BASELINE}/leSOMPoupeesSubOutputMap1.hdr
  ${TEMP}/leSOMbasedImageFilterOutput.hdr
  )

