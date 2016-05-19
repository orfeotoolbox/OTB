# cmake_minimum_required(VERSION 2.6)
# SET(CMAKE_MODULE_PATH "${${PROJECT_NAME}_SOURCE_DIR}/cmake;${CMAKE_MODULE_PATH}")
otb_module_test()

message(STATUS "UT: ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}")


# Common dirs
include_directories(${PROJECT_SOURCE_DIR}/Modules/ThirdParty/OssimPlugins/src/ossim ${OSSIM_INCLUDE_DIR})

#==== UT for utilities
add_executable(ossimStringUtilitiesTest ossimStringUtilitiesTest.cpp)
target_link_libraries(ossimStringUtilitiesTest
   otbossimplugins
   ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
otb_module_target_label(ossimStringUtilitiesTest)
otb_add_test(NAME ossimStringUtilitiesTest COMMAND ossimStringUtilitiesTest )


#===== Domain tests
# s1_inversemodel
add_executable(s1_inversemodel s1_inversemodel.cpp)
target_link_libraries(s1_inversemodel otbossimplugins)
otb_module_target_label(s1_inversemodel)

#Test executables
add_executable(ossimSarSensorModelTest ossimSarSensorModelTest.cpp)
target_link_libraries(ossimSarSensorModelTest otbossimplugins)
otb_module_target_label(ossimSarSensorModelTest)

add_executable(ossimSentinel1SarSensorModelTest ossimSentinel1SarSensorModelTest.cpp)
target_link_libraries(ossimSentinel1SarSensorModelTest otbossimplugins)

add_executable(ossimTerraSarXSarSensorModelTest ossimTerraSarXSarSensorModelTest.cpp)
target_link_libraries(ossimTerraSarXSarSensorModelTest otbossimplugins)

#Use ctest
# enable_testing()
otb_add_test(NAME ossimSarSensorModelTest COMMAND ossimSarSensorModelTest )

#S1 tests
file(GLOB s1_files ${CMAKE_CURRENT_SOURCE_DIR}/data/s1/*.xml)
#Loop over all S1 annotation files
foreach(entry ${s1_files})
  get_filename_component(name ${entry} NAME_WE)
  otb_add_test( NAME s1_inverse_${name} COMMAND ossimSentinel1SarSensorModelTest 1 ${entry})
  otb_add_test( NAME s1_forward_${name} COMMAND ossimSentinel1SarSensorModelTest 0 ${entry})
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

otb_add_test( NAME tsx_inverse_PANGKALANBUUN COMMAND ossimTerraSarXSarSensorModelTest 1 ${CMAKE_SOURCE_DIR}/data/tsx/PANGKALANBUUN/TSX1_SAR__SSC______SM_D_SRA_20080313T221955_20080313T222003.xml ${CMAKE_SOURCE_DIR}/data/tsx/PANGKALANBUUN/GEOREF.xml)
otb_add_test( NAME tsx_forward_PANGKALANBUUN COMMAND ossimTerraSarXSarSensorModelTest 0 ${CMAKE_SOURCE_DIR}/data/tsx/PANGKALANBUUN/TSX1_SAR__SSC______SM_D_SRA_20080313T221955_20080313T222003.xml ${CMAKE_SOURCE_DIR}/data/tsx/PANGKALANBUUN/GEOREF.xml)
otb_add_test( NAME tsx_inverse_UPSALA COMMAND ossimTerraSarXSarSensorModelTest 1 ${CMAKE_SOURCE_DIR}/data/tsx/UPSALA_GLACIER/TSX1_SAR__MGD.xml ${CMAKE_SOURCE_DIR}/data/tsx/UPSALA_GLACIER/GEOREF.xml)
otb_add_test( NAME tsx_forward_UPSALA COMMAND ossimTerraSarXSarSensorModelTest 0 ${CMAKE_SOURCE_DIR}/data/tsx/UPSALA_GLACIER/TSX1_SAR__MGD.xml ${CMAKE_SOURCE_DIR}/data/tsx/UPSALA_GLACIER/GEOREF.xml)
