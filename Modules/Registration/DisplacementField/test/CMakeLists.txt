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

set(OTBDisplacementFieldTests
otbDisplacementFieldTestDriver.cxx
otbNNearestTransformsLinearInterpolateDisplacementFieldGenerator.cxx
otbNNearestPointsLinearInterpolateDisplacementFieldGenerator.cxx
otbNearestTransformDisplacementFieldGenerator.cxx
otbNearestPointDisplacementFieldGenerator.cxx
otbBSplinesInterpolateTransformDisplacementFieldGenerator.cxx
otbBSplinesInterpolateDisplacementFieldGenerator.cxx
)

add_executable(otbDisplacementFieldTestDriver ${OTBDisplacementFieldTests})
target_link_libraries(otbDisplacementFieldTestDriver ${OTBDisplacementField-Test_LIBRARIES})
otb_module_target_label(otbDisplacementFieldTestDriver)

# Tests Declaration

otb_add_test(NAME dmTvNNearestTransformsLinearInterpolateDisplacementFieldGenerator COMMAND otbDisplacementFieldTestDriver
  --compare-image ${EPSILON_10}
  ${BASELINE}/dmTvNNearestTransformsLinearInterpolateDeformationField.tif
  ${TEMP}/dmTvNNearestTransformsLinearInterpolateDisplacementField.tif
  otbNNearestTransformsLinearInterpolateDisplacementFieldGenerator
  ${TEMP}/dmTvNNearestTransformsLinearInterpolateDisplacementField.tif
  )

otb_add_test(NAME dmTvNNearestPointsLinearInterpolateDisplacementFieldGenerator COMMAND otbDisplacementFieldTestDriver
  --compare-image ${EPSILON_10}
  ${BASELINE}/dmTvNNearestPointsLinearInterpolateDeformationField.tif
  ${TEMP}/dmTvNNearestPointsLinearInterpolateDisplacementField.tif
  otbNNearestPointsLinearInterpolateDisplacementFieldGenerator
  ${TEMP}/dmTvNNearestPointsLinearInterpolateDisplacementField.tif
  )



otb_add_test(NAME dmTvNearestTransformDisplacementFieldGenerator COMMAND otbDisplacementFieldTestDriver
  --compare-image ${EPSILON_10}
  ${BASELINE}/dmTvNearestTransformDeformationField.tif
  ${TEMP}/dmTvNearestTransformDisplacementField.tif
  otbNearestTransformDisplacementFieldGenerator
  ${TEMP}/dmTvNearestTransformDisplacementField.tif
  )

otb_add_test(NAME dmTvNearestPointDisplacementFieldGenerator COMMAND otbDisplacementFieldTestDriver
  --compare-image ${EPSILON_10}
  ${BASELINE}/dmTvNearestPointDeformationField.tif
  ${TEMP}/dmTvNearestPointDisplacementField.tif
  otbNearestPointDisplacementFieldGenerator
  ${TEMP}/dmTvNearestPointDisplacementField.tif
  )

#otb_add_test(NAME dmTvBSplinesInterpolateTransformDisplacementFieldGenerator COMMAND otbDisplacementFieldTestDriver
  #--compare-image ${EPSILON_4}
  #${BASELINE}/dmTvBSplinesInterpolateTransformDeformationFieldGenerator.tif
  #${TEMP}/dmTvBSplinesInterpolateTransformDisplacementFieldGenerator.tif
  #otbBSplinesInterpolateTransformDisplacementFieldGenerator
  #${TEMP}/dmTvBSplinesInterpolateTransformDisplacementFieldGenerator.tif
  #)

otb_add_test(NAME dmTvBSplinesInterpolateDisplacementFieldGenerator COMMAND otbDisplacementFieldTestDriver
  --compare-image ${EPSILON_10}
  ${BASELINE}/dmTvBSplinesInterpolateDeformationField.tif
  ${TEMP}/dmTvBSplinesInterpolateDisplacementField.tif
  otbBSplinesInterpolateDisplacementFieldGenerator
  ${TEMP}/dmTvBSplinesInterpolateDisplacementField.tif
  )

