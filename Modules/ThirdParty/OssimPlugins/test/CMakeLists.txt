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


# Common dirs
#include_directories(${PROJECT_SOURCE_DIR}/Modules/ThirdParty/OssimPlugins/src/ossim ${OSSIM_INCLUDE_DIR})
#==== UT for utilities
if (Boost_UNIT_TEST_FRAMEWORK_FOUND)
  #this test is failing on osx due to a bug in AppleClang stdc++ (Luc Hermitte)
 if(NOT APPLE)
   add_executable(OTBossimStringUtilitiesTest ossimStringUtilitiesTest.cpp)
   target_link_libraries(OTBossimStringUtilitiesTest
     otbossimplugins
     ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
   otb_module_target_label(OTBossimStringUtilitiesTest)
   otb_add_test(NAME TuossimStringUtilitiesTest COMMAND OTBossimStringUtilitiesTest )
 endif()

  add_executable(OTBossimTimeUtilitiesTest ossimTimeUtilitiesTest.cpp)
  target_link_libraries(OTBossimTimeUtilitiesTest
    # ${OTBOssim_LIBRARIES}
    otbossimplugins
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
  otb_module_target_label(OTBossimTimeUtilitiesTest)
  otb_add_test(NAME TuossimTimeUtilitiesTest COMMAND OTBossimTimeUtilitiesTest )
else()
   message(STATUS "Boost unit test framework not found, ossimStringUtilitiesTest and ossimTimeUtilitiesTest will be skipped.")
endif()

#==== Benchmarking Time conversion
# (requires google.benchmark)
find_package(GBenchmark)
if (GBENCHMARK_FOUND)
  add_executable(OTBossimTimeUtilitiesBench ossimTimeUtilitiesBench.cpp)
  include_directories(${GBENCHMARK_INCLUDE_DIRS})
  target_link_libraries(OTBossimTimeUtilitiesBench
    otbossimplugins
    ${GBENCHMARK_LIBRARIES})
  otb_module_target_label(OTBossimTimeUtilitiesBench)
# else()
# Even if GBenchmark is found, test is not launched because it is not added to ctest
endif()


#===== Domain tests
#Test executables
add_executable(OTBossimSarSensorModelTest ossimSarSensorModelTest.cpp)
target_link_libraries(OTBossimSarSensorModelTest otbossimplugins)
otb_module_target_label(OTBossimSarSensorModelTest)

# S1
add_executable(OTBossimSentinel1ModelTest ossimSentinel1ModelTest.cpp)
target_link_libraries(OTBossimSentinel1ModelTest otbossimplugins)
otb_module_target_label(OTBossimSentinel1ModelTest)

# TSX (old)
add_executable(OTBossimTerraSarXSarSensorModelTest ossimTerraSarXSarSensorModelTest.cpp)
target_link_libraries(OTBossimTerraSarXSarSensorModelTest otbossimplugins)
otb_module_target_label(OTBossimTerraSarXSarSensorModelTest)

otb_add_test(NAME TuossimSarSensorModelTest COMMAND OTBossimSarSensorModelTest )

#S1 tests
file(GLOB s1_files ${INPUTDATA}/ossimPlugins/s1/*.xml)
#Loop over all S1 annotation files
foreach(entry ${s1_files})
  get_filename_component(name ${entry} NAME_WE)
  otb_add_test( NAME s1_inverse_${name} COMMAND OTBossimSentinel1ModelTest 1 ${entry})
  otb_add_test( NAME s1_forward_${name} COMMAND OTBossimSentinel1ModelTest 0 ${entry})
endforeach()

# #TSX tests
# file(GLOB tsx_files ${CMAKE_SOURCE_DIR}/data/tsx/*)
# #Loop over all S1 annotation files
# foreach(entry ${tsx_files})
#   file(GLOB tsx_files ${CMAKE_SOURCE_DIR}/data/tsx/*)
#   get_filename_component(name ${entry} NAME_WE)
#   add_test( tsx_inverse_${name} ossimTerraSarXSarSensorModelTest ${entry} )
#    add_test( tsx_forward_${name} ossimSentinel1SarSensorModelTest 0 ${entry})
# endforeach()

# otb_add_test( NAME tsx_inverse_PANGKALANBUUN COMMAND ossimTerraSarXSarSensorModelTest 1 ${CMAKE_SOURCE_DIR}/data/tsx/PANGKALANBUUN/TSX1_SAR__SSC______SM_D_SRA_20080313T221955_20080313T222003.xml ${CMAKE_SOURCE_DIR}/data/tsx/PANGKALANBUUN/GEOREF.xml)
# otb_add_test( NAME tsx_forward_PANGKALANBUUN COMMAND ossimTerraSarXSarSensorModelTest 0 ${CMAKE_SOURCE_DIR}/data/tsx/PANGKALANBUUN/TSX1_SAR__SSC______SM_D_SRA_20080313T221955_20080313T222003.xml ${CMAKE_SOURCE_DIR}/data/tsx/PANGKALANBUUN/GEOREF.xml)
# otb_add_test( NAME tsx_inverse_UPSALA COMMAND ossimTerraSarXSarSensorModelTest 1 ${CMAKE_SOURCE_DIR}/data/tsx/UPSALA_GLACIER/TSX1_SAR__MGD.xml ${CMAKE_SOURCE_DIR}/data/tsx/UPSALA_GLACIER/GEOREF.xml)
# otb_add_test( NAME tsx_forward_UPSALA COMMAND ossimTerraSarXSarSensorModelTest 0 ${CMAKE_SOURCE_DIR}/data/tsx/UPSALA_GLACIER/TSX1_SAR__MGD.xml ${CMAKE_SOURCE_DIR}/data/tsx/UPSALA_GLACIER/GEOREF.xml)
