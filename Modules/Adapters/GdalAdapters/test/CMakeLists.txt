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

set(OTBOGRTests
otbOGRTestDriver.cxx
otbOGRExtendedFilenameToOptionsTest.cxx
otbOGRExtendedFilenameToOptionsGDALTest.cxx
)

add_executable(otbOGRTestDriver ${OTBOGRTests})
target_link_libraries(otbOGRTestDriver ${OTBGdalAdapters-Test_LIBRARIES})
otb_module_target_label(otbOGRTestDriver)

otb_add_test(NAME TvOGRExtendedFilename
         COMMAND otbOGRTestDriver
         --compare-ascii ${NOTOL}
         ${BASELINE}/TvOGRExtendedFilename.txt
         ${TEMP}/TvOGRExtendedFilenameTest.txt
         otbOGRExtendedFileName
         test.shp?&writegeom=ON&gdal:co:QUALITY=75&gdal:co:TILED=YES&gdal:co:BLOCKYSIZE=1024&gdal:lco:layeroption=OPTION&gdal:oo:openoption=OPTION
         ${TEMP}/TvOGRExtendedFilenameTest.txt     )

#Problem with error thrown by GDAL : unable to catch it with "CPLGetLastErrorMsg"
# otb_add_test(NAME TvOGRExtendedFilenameGDALOpen
#          COMMAND otbOGRTestDriver
#          otbOGRExtendedFileNameGDALOpen
#          ${INPUTDATA}/ToulousePoints-examples.shp?&gdal:oo:openOption=OPTION
#           )

otb_add_test(NAME TvOGRExtendedFilenameGDALCreate
         COMMAND otbOGRTestDriver
         otbOGRExtendedFileNameGDALCreate
         test_OGRExtendedFilenameGDALCreate.shp?gdal:co:creationOption=OPTION
          )

otb_add_test(NAME TvOGRExtendedFilenameGDALLayer
         COMMAND otbOGRTestDriver
         otbOGRExtendedFileNameGDALLayer
         test_OGRExtendedFilenameGDALLayer.shp?&gdal:lco:layeroption=OPTION
          )

otb_add_test(NAME TvOGRExtendedFilenameGDALLayerOption
         COMMAND otbOGRTestDriver
         otbOGRExtendedFileNameGDALLayerOption
         test_OGRExtendedFilenameGDALLayerOption.shp
         )
       
add_executable(otbGdalAdaptersTestDriver
               otbGdalAdaptersTestDriver.cxx
               otbSpatialReferenceTest.cxx
               otbCoordinateTransformationTest.cxx
               )

target_link_libraries(otbGdalAdaptersTestDriver ${OTBGdalAdapters-Test_LIBRARIES})
             
otb_module_target_label(otbGdalAdaptersTestDriver)

otb_add_test(NAME TuSpatialReferenceTest
            COMMAND otbGdalAdaptersTestDriver otbSpatialReferenceTest)
  
otb_add_test(NAME TuOCoordinateTransformationTest
            COMMAND otbGdalAdaptersTestDriver otbCoordinateTransformationTest)
