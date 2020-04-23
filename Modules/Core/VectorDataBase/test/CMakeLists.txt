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

set(OTBVectorDataBaseTests
otbVectorDataBaseTestDriver.cxx
otbVectorData.cxx
otbVectorDataKeywordlistTest.cxx
otbPolygon.cxx
otbDataNodeTest.cxx
otbRemoteSensingRegion.cxx
)

add_executable(otbVectorDataBaseTestDriver ${OTBVectorDataBaseTests})
target_link_libraries(otbVectorDataBaseTestDriver ${OTBVectorDataBase-Test_LIBRARIES})
otb_module_target_label(otbVectorDataBaseTestDriver)

# Tests Declaration

otb_add_test(NAME coTvVectorData COMMAND otbVectorDataBaseTestDriver
  --compare-ascii ${EPSILON_3}
  ${BASELINE_FILES}/coTvVectorData.txt
  ${TEMP}/coTvVectorData.txt
  --ignore-lines-with 1 RTTI
  otbVectorData
  ${TEMP}/coTvVectorData.txt
  )


otb_add_test(NAME ioTvVectorDataKeywordlist COMMAND otbVectorDataBaseTestDriver
  --compare-ascii ${EPSILON_9}
  ${BASELINE_FILES}/ioTuVectorDataKeywordlist.txt
  ${TEMP}/ioTuVectorDataKeywordlist.txt
  otbVectorDataKeywordlist
  ${INPUTDATA}/waterways.shp
  ${TEMP}/ioTuVectorDataKeywordlist.txt
  )

otb_add_test(NAME coTvPolygon COMMAND otbVectorDataBaseTestDriver
  --compare-ascii ${EPSILON_3}
  ${BASELINE_FILES}/otbPolygonTest.txt
  ${TEMP}/otbPolygonTest.txt
  otbPolygon
  ${TEMP}/otbPolygonTest.txt
  2 10 7 10 7 9 5 9 5 8 7 8 7 6 9 6 9 5 7 5 7 3 2 3
  next 6 10 11 10 11 2 4 2 3 4 8 4 8 7 6 7
  )

otb_add_test(NAME coTvDataNode COMMAND otbVectorDataBaseTestDriver
  otbDataNodeTest
  )

otb_add_test(NAME coTvRemoteSensingRegion COMMAND otbVectorDataBaseTestDriver
  otbRemoteSensingRegion
  1.21 2.33 58.47 59.49  #size et index of the region
  0. 0.                  # Point to check
  )
