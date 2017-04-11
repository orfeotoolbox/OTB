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

set(OTBDisplacementFieldTests
otbDisplacementFieldTestDriver.cxx
otbNearestTransformDisplacementFieldGeneratorNew.cxx
otbNNearestTransformsLinearInterpolateDisplacementFieldGenerator.cxx
otbNNearestPointsLinearInterpolateDisplacementFieldGenerator.cxx
otbNearestPointDisplacementFieldGeneratorNew.cxx
otbPointSetToDisplacementFieldGeneratorNew.cxx
otbPointSetWithTransformToDisplacementFieldGeneratorNew.cxx
otbBSplinesInterpolateDisplacementFieldGeneratorNew.cxx
otbNNearestTransformsLinearInterpolateDisplacementFieldGeneratorNew.cxx
otbNNearestPointsLinearInterpolateDisplacementFieldGeneratorNew.cxx
otbNearestTransformDisplacementFieldGenerator.cxx
otbNearestPointDisplacementFieldGenerator.cxx
otbBSplinesInterpolateTransformDisplacementFieldGenerator.cxx
otbBSplinesInterpolateDisplacementFieldGenerator.cxx
otbBSplinesInterpolateTransformDisplacementFieldGeneratorNew.cxx
)

add_executable(otbDisplacementFieldTestDriver ${OTBDisplacementFieldTests})
target_link_libraries(otbDisplacementFieldTestDriver ${OTBDisplacementField-Test_LIBRARIES})
otb_module_target_label(otbDisplacementFieldTestDriver)

# Tests Declaration

otb_add_test(NAME dmTuNearestTransformDisplacementFieldGeneratorNew COMMAND otbDisplacementFieldTestDriver
  otbNearestTransformDisplacementFieldGeneratorNew)

otb_add_test(NAME dmTvNNearestTransformsLinearInterpolateDisplacementFieldGenerator COMMAND otbDisplacementFieldTestDriver
  --compare-image ${EPSILON_10}
  ${BASELINE}/dmTvNNearestTransformsLinearInterpolateDeformationField.hdr
  ${TEMP}/dmTvNNearestTransformsLinearInterpolateDisplacementField.hdr
  otbNNearestTransformsLinearInterpolateDisplacementFieldGenerator
  ${TEMP}/dmTvNNearestTransformsLinearInterpolateDisplacementField.hdr
  )

otb_add_test(NAME dmTvNNearestPointsLinearInterpolateDisplacementFieldGenerator COMMAND otbDisplacementFieldTestDriver
  --compare-image ${EPSILON_10}
  ${BASELINE}/dmTvNNearestPointsLinearInterpolateDeformationField.hdr
  ${TEMP}/dmTvNNearestPointsLinearInterpolateDisplacementField.hdr
  otbNNearestPointsLinearInterpolateDisplacementFieldGenerator
  ${TEMP}/dmTvNNearestPointsLinearInterpolateDisplacementField.hdr
  )

otb_add_test(NAME dmTuNearestPointDisplacementFieldGeneratorNew COMMAND otbDisplacementFieldTestDriver
  otbNearestPointDisplacementFieldGeneratorNew)

otb_add_test(NAME dmTuPointSetToDisplacementFieldGeneratorNew COMMAND otbDisplacementFieldTestDriver
  otbPointSetToDisplacementFieldGeneratorNew)

otb_add_test(NAME dmTuPointSetWithTransformToDisplacementFieldGeneratorNew COMMAND otbDisplacementFieldTestDriver
  otbPointSetWithTransformToDisplacementFieldGeneratorNew)

otb_add_test(NAME dmTuNNearestTransformsLinearInterpolateDisplacementFieldGeneratorNew COMMAND otbDisplacementFieldTestDriver
  otbNNearestTransformsLinearInterpolateDisplacementFieldGeneratorNew)

otb_add_test(NAME dmTuBSplinesInterpolateDisplacementFieldGeneratorNew COMMAND otbDisplacementFieldTestDriver
  otbNNearestPointsLinearInterpolateDisplacementFieldGeneratorNew)

otb_add_test(NAME dmTuNNearestPointsLinearInterpolateDisplacementFieldGeneratorNew COMMAND otbDisplacementFieldTestDriver
  otbNNearestPointsLinearInterpolateDisplacementFieldGeneratorNew)



otb_add_test(NAME dmTvNearestTransformDisplacementFieldGenerator COMMAND otbDisplacementFieldTestDriver
  --compare-image ${EPSILON_10}
  ${BASELINE}/dmTvNearestTransformDeformationField.hdr
  ${TEMP}/dmTvNearestTransformDisplacementField.hdr
  otbNearestTransformDisplacementFieldGenerator
  ${TEMP}/dmTvNearestTransformDisplacementField.hdr
  )

otb_add_test(NAME dmTvNearestPointDisplacementFieldGenerator COMMAND otbDisplacementFieldTestDriver
  --compare-image ${EPSILON_10}
  ${BASELINE}/dmTvNearestPointDeformationField.hdr
  ${TEMP}/dmTvNearestPointDisplacementField.hdr
  otbNearestPointDisplacementFieldGenerator
  ${TEMP}/dmTvNearestPointDisplacementField.hdr
  )

#otb_add_test(NAME dmTvBSplinesInterpolateTransformDisplacementFieldGenerator COMMAND otbDisplacementFieldTestDriver
  #--compare-image ${EPSILON_4}
  #${BASELINE}/dmTvBSplinesInterpolateTransformDeformationFieldGenerator.hdr
  #${TEMP}/dmTvBSplinesInterpolateTransformDisplacementFieldGenerator.hdr
  #otbBSplinesInterpolateTransformDisplacementFieldGenerator
  #${TEMP}/dmTvBSplinesInterpolateTransformDisplacementFieldGenerator.hdr
  #)

otb_add_test(NAME dmTvBSplinesInterpolateDisplacementFieldGenerator COMMAND otbDisplacementFieldTestDriver
  --compare-image ${EPSILON_10}
  ${BASELINE}/dmTvBSplinesInterpolateDeformationField.hdr
  ${TEMP}/dmTvBSplinesInterpolateDisplacementField.hdr
  otbBSplinesInterpolateDisplacementFieldGenerator
  ${TEMP}/dmTvBSplinesInterpolateDisplacementField.hdr
  )

otb_add_test(NAME dmTuBSplinesInterpolateTransformDisplacementFieldGeneratorNew COMMAND otbDisplacementFieldTestDriver
  otbBSplinesInterpolateTransformDisplacementFieldGeneratorNew)

