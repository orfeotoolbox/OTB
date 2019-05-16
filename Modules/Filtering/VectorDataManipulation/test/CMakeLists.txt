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

set(OTBVectorDataManipulationTests
otbVectorDataManipulationTestDriver.cxx
otbVectorDataExtractROI.cxx
otbPolygonCompacityFunctor.cxx
otbVectorDataToRandomLineGenerator.cxx
otbConcatenateVectorDataFilter.cxx
)

add_executable(otbVectorDataManipulationTestDriver ${OTBVectorDataManipulationTests})
target_link_libraries(otbVectorDataManipulationTestDriver ${OTBVectorDataManipulation-Test_LIBRARIES})
otb_module_target_label(otbVectorDataManipulationTestDriver)

# Tests Declaration

otb_add_test(NAME coTvVectorDataExtractROI COMMAND otbVectorDataManipulationTestDriver
  --compare-ogr ${NOTOL}
  ${BASELINE_FILES}/coVectorDataExtractROIOutput.shp
  ${TEMP}/coVectorDataExtractROIOutput.shp
  otbVectorDataExtractROI
  ${INPUTDATA}/ToulousePoints-examples.shp
  ${TEMP}/coVectorDataExtractROIOutput.shp
  374369.48850211215904 4853951.786124914  # Origin of the RemoteSensingRegion
  1000.25 -25000.2                               # Size of the Cartoregion
  )

otb_add_test(NAME coTvVectorDataExtractROI2 COMMAND otbVectorDataManipulationTestDriver
  --compare-ogr ${NOTOL}
  ${BASELINE_FILES}/coVectorDataExtractROIOutput2.shp
  ${TEMP}/coVectorDataExtractROIOutput2.shp
  otbVectorDataExtractROI
  ${INPUTDATA}/ToulousePoints-examples.shp
  ${TEMP}/coVectorDataExtractROIOutput2.shp
  374369.48850211215904 4828951.58612491376698  # Origin of the RemoteSensingRegion
  1000.25 25000.2                               # Size of the Cartoregion
  )

otb_add_test(NAME bfTvPolygonCompacityFunctor COMMAND otbVectorDataManipulationTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/bfPolygonCompacityFunctorTest.txt
  ${TEMP}/bfPolygonCompacityFunctorTest.txt
  otbPolygonCompacityFunctor
  ${INPUTDATA}/polygon.png
  ${TEMP}/bfPolygonCompacityFunctorTest.txt
  )

otb_add_test(NAME bfTvVectorDataToRandomLineGenerator COMMAND otbVectorDataManipulationTestDriver
  --compare-ogr ${NOTOL}
  ${BASELINE_FILES}/bfTvVectorDataToRandomLineGeneratorOutput.shp
  ${TEMP}/bfTvVectorDataToRandomLineGeneratorOutput.shp
  otbVectorDataToRandomLineGenerator
  ${INPUTDATA}/Capitole-Shadows.shp
  ${TEMP}/bfTvVectorDataToRandomLineGeneratorOutput.shp
  )

otb_add_test(NAME bfTvConcatenateVectorDataFilter COMMAND otbVectorDataManipulationTestDriver
  --compare-ogr ${NOTOL}
  ${BASELINE_FILES}/otbConcatenateVectorDataFilter_Output.shp
  ${TEMP}/otbConcatenateVectorDataFilter_Output.shp
  otbConcatenateVectorDataFilter
  ${INPUTDATA}/france_coastline.shp
  ${INPUTDATA}/waterways.shp
  ${TEMP}/otbConcatenateVectorDataFilter_Output.shp
  )
