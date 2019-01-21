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

set(OTBAppTestTests
otbWrapperApplicationDocTests.cxx
otbAppTestTestDriver.cxx
)

add_executable(otbAppTestTestDriver ${OTBAppTestTests})
target_link_libraries(otbAppTestTestDriver ${OTBAppTest-Test_LIBRARIES})
otb_module_target_label(otbAppTestTestDriver)


otb_add_test(NAME apCheckDocumentation COMMAND otbAppTestTestDriver
  otbWrapperApplicationDocTest ${OTB_BINARY_DIR}/lib/otb/applications)

otb_test_application(NAME apTvTestApplicationOutputXML_All
  APP TestApplication
  OPTIONS -boolean 1
          -int 2
          -float -0.7
          -string foo
          -filename ${INPUTDATA}/letter.scale
          -outfilename ${TEMP}/foo.txt
          -directory ${INPUTDATA}/world_boundaries
          -choice.choice1.floatchoice1 6.4e5
          -ingroup.integer 4
          -outgroup.outputimage ${TEMP}/foo.tif double
          -il ${INPUTDATA}/anaglyphInput1.tif ${INPUTDATA}/anaglyphInput2.tif
          -cl Choice2
          -cin ${INPUTDATA}/RADARSAT2_ALTONA_300_300_VV.tif
          -cout ${TEMP}/fooCplx.tif
          -outxml ${TEMP}/apTvTestApplicationOutputXML_All.xml
  VALID --compare-ascii ${NOTOL}
    ${OTBAPP_BASELINE_FILES}/apTvTestApplicationOutputXML_All.xml
    ${TEMP}/apTvTestApplicationOutputXML_All.xml
    --ignore-lines-with 8 letter.scale foo.txt world_boundaries foo.tif anaglyphInput1.tif anaglyphInput2.tif RADARSAT2_ALTONA_300_300_VV.tif <version>
  )
