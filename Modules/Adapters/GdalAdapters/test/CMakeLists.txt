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

if (Boost_UNIT_TEST_FRAMEWORK_FOUND)
  add_executable(otbOGRTests otbOGRDataSourceWrapperNew.cxx)
  target_link_libraries(
    otbOGRTests
    ${OTBGdalAdapters-Test_LIBRARIES}
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} )
  
  add_test(NAME coTuOGRDataSourceWrapperNew
           COMMAND otbOGRTests coTuOGRDataSourceWrapperNew
           WORKING_DIRECTORY ${TEMP})
endif()

add_executable(otbOGRTestsIO otbOGRDataSourceWrapperIO.cxx)
target_link_libraries(otbOGRTestsIO ${OTBGdalAdapters-Test_LIBRARIES})

otb_add_test(NAME coTuOGRDataSourceWrapperIO
         COMMAND otbOGRTestsIO ${INPUTDATA}/ToulousePoints-examples.shp )

add_executable(otbOGRExtendedFilenameTest otbOGRExtendedFilenameToOptionsTest.cxx)
target_link_libraries(otbOGRExtendedFilenameTest ${OTBGdalAdapters-Test_LIBRARIES})

otb_add_test(NAME TvOGRExtendedFilename
         COMMAND otbOGRExtendedFilenameTest
         --compare-ascii ${NOTOL}
  ${BASELINE}/TvOGRExtendedFilename.txt
  ${TEMP}/TvOGRExtendedFilename.txt
         test.shp?&writegeom=ON&gdal:co:QUALITY=75&gdal:co:TILED=YES&gdal:co:BLOCKYSIZE=1024&gdal:lco:layeroption=OPTION&gdal:oo:openoption=OPTION
         ${TEMP}/TvOGRExtendedFilename.txt     )
otb_module_target_label(otbOGRExtendedFilenameTest)

message(STATUS "Test link : ${OTBGdalAdapters-Test_LIBRARIES}")