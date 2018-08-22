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

set(OTBIOTileMapTests
otbIOTileMapTestDriver.cxx
otbTileMapImageIOHelperTest.cxx
otbTileMapImageIOTest.cxx
otbTileMapWriter.cxx
)

add_executable(otbIOTileMapTestDriver ${OTBIOTileMapTests})
target_link_libraries(otbIOTileMapTestDriver ${OTBIOTileMap-Test_LIBRARIES})
otb_module_target_label(otbIOTileMapTestDriver)

# Tests Declaration


otb_add_test(NAME ioTvTileMapImageIOHelperTest COMMAND otbIOTileMapTestDriver
  --compare-ascii ${EPSILON_9} ${BASELINE_FILES}/ioTvTileMapImageIOHelperTest.txt
  ${TEMP}/ioTvTileMapImageIOHelperTest.txt
  otbTileMapImageIOHelperTest
  ${TEMP}/ioTvTileMapImageIOHelperTest.txt
  )

otb_add_test(NAME ioTvTileMapImageIOWeb COMMAND otbIOTileMapTestDriver
  otbTileMapImageIOTest
  http://tile.openstreetmap.org/
  ${TEMP}/ioTvTileMapImageIOWebOutput.png
  ${TEMP}
  1.4835345 43.55968261 13
  )
set_property(TEST ioTvTileMapImageIOWeb PROPERTY RESOURCE_LOCK web_access)

otb_add_test(NAME ioTvTileMapImageIOFile COMMAND otbIOTileMapTestDriver
  otbTileMapImageIOTest
  ${OTB_DATA_ROOT}/Examples/osmfile.otb
  ${TEMP}/ioTvTileMapImageIOFileOutput.png
  ${TEMP}
  1.4835345 43.55968261 13
  )
set_property(TEST ioTvTileMapImageIOFile PROPERTY RESOURCE_LOCK web_access)

#otb_add_test(NAME ioTvTileMapWriter COMMAND otbIOTileMapTestDriver
  #otbTileMapWriter
  #LARGEINPUT{SPOT5/TEHERAN/IMAGERY.TIF}
  #${OTB_DATA_ROOT}/Input/DEM/srtm_directory
  #${TEMP}
  #500 # extract index[0]
  #500 # extract index[1]
  #200 # extract size[0]
  #200 # extract size[1]
  #)

