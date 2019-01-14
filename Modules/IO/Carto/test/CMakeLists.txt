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

set(OTBCartoTests
otbCartoTestDriver.cxx
otbImageToOSMVectorDataGenerator.cxx
otbMapFileProductWriter.cxx
otbOSMDataToVectorDataTests.cxx
otbCoordinateToNameTest.cxx
)

add_executable(otbCartoTestDriver ${OTBCartoTests})
target_link_libraries(otbCartoTestDriver ${OTBCarto-Test_LIBRARIES})
otb_module_target_label(otbCartoTestDriver)

# Tests Declaration



otb_add_test(NAME ioTvImageToOSMVectorDataGenerator COMMAND otbCartoTestDriver
  --compare-ogr ${EPSILON_9}
  ${BASELINE_FILES}/ioTvImageToOSMVectorDataGenerator_highway_motorway.shp
  ${TEMP}/ioTvImageToOSMVectorDataGenerator_highway_motorway.shp
  otbImageToOSMVectorDataGenerator
  -in  ${EXAMPLEDATA}/qb_RoadExtract.tif
  -osm ${INPUTDATA}/qb_road.osm
  -out ${TEMP}/ioTvImageToOSMVectorDataGenerator_highway_motorway.shp
  -key highvay,motorway
  )
set_property(TEST ioTvImageToOSMVectorDataGenerator PROPERTY RESOURCE_LOCK web_access)

otb_add_test(NAME ioTvMapFileProductWriter COMMAND otbCartoTestDriver
  otbMapFileProductWriter
  ${INPUTDATA}/ExtrZoneNuageuse.tif
  ${TEMP}/mapserver/toulouseExtract.map
  ${TEMP}/mapserver/result               # Shape index path
  http://localhost/cgi-bin/mapserv # cgi executable path
  256
  )
set_property(TEST ioTvMapFileProductWriter PROPERTY RESOURCE_LOCK web_access)

otb_add_test(NAME ioTvOSMToVectorDataGeneratorByName COMMAND otbCartoTestDriver
  --compare-ogr ${EPSILON_9}
  ${BASELINE_FILES}/ioTvOSMToVectorDataFilterByName_highway.shp
  ${TEMP}/ioTvOSMToVectorDataFilterByName_highway.shp
  otbOSMToVectorDataGeneratorByName
  -osm ${INPUTDATA}/qb_road.osm
  -vd ${TEMP}/ioTvOSMToVectorDataFilterByName_highway.shp
  )
set_property(TEST ioTvOSMToVectorDataGeneratorByName PROPERTY RESOURCE_LOCK web_access)

otb_add_test(NAME ioTvOSMToVectorDataGeneratorTest COMMAND otbCartoTestDriver
  --compare-ogr ${EPSILON_9}
  ${BASELINE_FILES}/ioTvOSMToVectorDataFilterTest_highway.shp
  ${TEMP}/ioTvOSMToVectorDataFilterTest_highway.shp
  otbOSMToVectorDataGeneratorTest
  ${INPUTDATA}/qb_road.osm
  ${TEMP}/ioTvOSMToVectorDataFilterTest_highway.shp
  )
set_property(TEST ioTvOSMToVectorDataGeneratorTest PROPERTY RESOURCE_LOCK web_access)

otb_add_test(NAME coTvCoordinateToNameTest COMMAND otbCartoTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE}/CoordinateToNameTest.txt
  ${TEMP}/CoordinateToNameTest.txt
  otbCoordinateToNameTest
  103.78 1.29
  ${TEMP}/CoordinateToNameTest.txt
  )

otb_add_test(NAME coTvCoordinateToNameMultithreadTest COMMAND otbCartoTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE}/CoordinateToNameTest.txt
  ${TEMP}/CoordinateToNameMultithreadTest.txt
  otbCoordinateToNameTest
  103.78 1.29
  ${TEMP}/CoordinateToNameMultithreadTest.txt
  1
  )

