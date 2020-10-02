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
set(OTBMathParserXTests
  otbParserXTest.cxx
  otbBandMathXImageFilter.cxx
  otbMathParserXTestDriver.cxx  )

add_executable(otbMathParserXTestDriver ${OTBMathParserXTests})
target_link_libraries(otbMathParserXTestDriver ${OTBMathParserX-Test_LIBRARIES})
otb_module_target_label(otbMathParserXTestDriver)
otb_add_test(NAME coTvParserX COMMAND otbMathParserXTestDriver
  otbParserXTest
  )

otb_add_test(NAME bfTvBandMathXImageFilterConv COMMAND otbMathParserXTestDriver
  otbBandMathXImageFilterConv
  ${BASELINE_FILES}/bfTvExportBandMathX.txt
  ${TEMP}/bfTvExportBandMathXOut.txt
  )
otb_add_test(NAME bfTvBandMathXImageFilter COMMAND otbMathParserXTestDriver
  otbBandMathXImageFilter)
otb_add_test(NAME bfTvBandMathXImageFilterBandsFailures COMMAND otbMathParserXTestDriver
  otbBandMathXImageFilterBandsFailures)
otb_add_test(NAME bfTvBandMathXImageFilterWithIdx COMMAND otbMathParserXTestDriver
  otbBandMathXImageFilterWithIdx
  ${TEMP}/bfTvBandMathImageFilterWithIdx1.tif
  ${TEMP}/bfTvBandMathImageFilterWithIdx2.tif
  )
otb_add_test(NAME bfTvBandMathXImageFilterTxt COMMAND otbMathParserXTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/bfTvExportBandMathX.txt
  ${TEMP}/bfTvExportBandMathXOut.txt
  otbBandMathXImageFilterTxt
  ${BASELINE_FILES}/bfTvExportBandMathX.txt
  ${TEMP}/bfTvExportBandMathXOut.txt
  )
