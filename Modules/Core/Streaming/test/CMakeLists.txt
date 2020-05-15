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

set(OTBStreamingTests
otbStreamingTestDriver.cxx
otbStreamingManager.cxx
otbPipelineMemoryPrintCalculatorTest.cxx
)

add_executable(otbStreamingTestDriver ${OTBStreamingTests})
target_link_libraries(otbStreamingTestDriver ${OTBStreaming-Test_LIBRARIES})
otb_module_target_label(otbStreamingTestDriver)

# Tests Declaration

otb_add_test(NAME coTvRAMDrivenAdaptativeStreamingManager COMMAND otbStreamingTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/coTvRAMDrivenAdaptativeStreamingManager.txt
  ${TEMP}/coTvRAMDrivenAdaptativeStreamingManager.txt
  otbRAMDrivenAdaptativeStreamingManager
  ${TEMP}/coTvRAMDrivenAdaptativeStreamingManager.txt
  )

otb_add_test(NAME coTvRAMDrivenStrippedStreamingManager COMMAND otbStreamingTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/coTvRAMDrivenStrippedStreamingManager.txt
  ${TEMP}/coTvRAMDrivenStrippedStreamingManager.txt
  otbRAMDrivenStrippedStreamingManager
  ${TEMP}/coTvRAMDrivenStrippedStreamingManager.txt
  )

otb_add_test(NAME coTvNumberOfLinesStrippedStreamingManager COMMAND otbStreamingTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/coTvNumberOfLinesStrippedStreamingManager.txt
  ${TEMP}/coTvNumberOfLinesStrippedStreamingManager.txt
  otbNumberOfLinesStrippedStreamingManager
  ${TEMP}/coTvNumberOfLinesStrippedStreamingManager.txt
  )

otb_add_test(NAME coTvRAMDrivenTiledStreamingManager COMMAND otbStreamingTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/coTvRAMDrivenTiledStreamingManager.txt
  ${TEMP}/coTvRAMDrivenTiledStreamingManager.txt
  otbRAMDrivenTiledStreamingManager
  ${TEMP}/coTvRAMDrivenTiledStreamingManager.txt
  )


otb_add_test(NAME coTvTileDimensionTiledStreamingManager COMMAND otbStreamingTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/coTvTileDimensionTiledStreamingManager.txt
  ${TEMP}/coTvTileDimensionTiledStreamingManager.txt
  otbTileDimensionTiledStreamingManager
  ${TEMP}/coTvTileDimensionTiledStreamingManager.txt
  )

otb_add_test(NAME coTvPipelineMemoryPrintCalculator COMMAND otbStreamingTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/coTvPipelineMemoryPrintCalculatorOutput.txt
  ${TEMP}/coTvPipelineMemoryPrintCalculatorOutput.txt
  otbPipelineMemoryPrintCalculatorTest
  ${INPUTDATA}/qb_RoadExtract.img
  ${TEMP}/coTvPipelineMemoryPrintCalculatorOutput.txt
  )
