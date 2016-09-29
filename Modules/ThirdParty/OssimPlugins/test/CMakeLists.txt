otb_module_test()


# Common dirs
#include_directories(${PROJECT_SOURCE_DIR}/Modules/ThirdParty/OssimPlugins/src/ossim ${OSSIM_INCLUDE_DIR})
#==== UT for utilities
if (Boost_UNIT_TEST_FRAMEWORK_FOUND)
  #this test is failing on osx due to a bug in AppleClang stdc++ (Luc Hermitte)
 if(NOT APPLE)
   add_executable(ossimStringUtilitiesTest ossimStringUtilitiesTest.cpp)
   target_link_libraries(ossimStringUtilitiesTest
     otbossimplugins
     ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
   otb_module_target_label(ossimStringUtilitiesTest)
   otb_add_test(NAME ossimStringUtilitiesTest COMMAND ossimStringUtilitiesTest )
 endif()

  add_executable(ossimTimeUtilitiesTest ossimTimeUtilitiesTest.cpp)
  target_link_libraries(ossimTimeUtilitiesTest
    # ${OTBOssim_LIBRARIES}
    otbossimplugins
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
  otb_module_target_label(ossimTimeUtilitiesTest)
  otb_add_test(NAME ossimTimeUtilitiesTest COMMAND ossimTimeUtilitiesTest )
else()
   message(STATUS "Boost unit test framework not found, ossimStringUtilitiesTest and ossimTimeUtilitiesTest will be skipped.")
endif()

#==== Benchmarking Time conversion
# (requires google.benchmark)
find_package(GBenchmark)
if (GBENCHMARK_FOUND)
   add_executable(ossimTimeUtilitiesBench ossimTimeUtilitiesBench.cpp)
   include_directories(${GBENCHMARK_INCLUDE_DIRS})
   target_link_libraries(ossimTimeUtilitiesBench
      otbossimplugins
      ${GBENCHMARK_LIBRARIES})
   otb_module_target_label(ossimTimeUtilitiesBench)
else()
   message(STATUS "Google.benchmark not found, ossimTimeUtilitiesBench will be skipped.")
endif()


#===== Domain tests
#Test executables
add_executable(ossimSarSensorModelTest ossimSarSensorModelTest.cpp)
target_link_libraries(ossimSarSensorModelTest otbossimplugins)
otb_module_target_label(ossimSarSensorModelTest)

# S1
add_executable(ossimSentinel1ModelTest ossimSentinel1ModelTest.cpp)
target_link_libraries(ossimSentinel1ModelTest otbossimplugins)
otb_module_target_label(ossimSentinel1ModelTest)

# TSX (old)
add_executable(ossimTerraSarXSarSensorModelTest ossimTerraSarXSarSensorModelTest.cpp)
target_link_libraries(ossimTerraSarXSarSensorModelTest otbossimplugins)
otb_module_target_label(ossimTerraSarXSarSensorModelTest)

otb_add_test(NAME ossimSarSensorModelTest COMMAND ossimSarSensorModelTest )

#S1 tests
file(GLOB s1_files ${INPUTDATA}/ossimPlugins/s1/*.xml)
#Loop over all S1 annotation files
foreach(entry ${s1_files})
  get_filename_component(name ${entry} NAME_WE)
  otb_add_test( NAME s1_inverse_${name} COMMAND ossimSentinel1ModelTest 1 ${entry})
  otb_add_test( NAME s1_forward_${name} COMMAND ossimSentinel1ModelTest 0 ${entry})
endforeach()

## TSX tests
# file(GLOB tsx_dirs ${INPUTDATA}/ossimPlugins/tsx/*)
# Loop over all S1 annotation files
# foreach(entry ${tsx_dirs})
#   file(GLOB tsx_files ${CMAKE_SOURCE_DIR}/data/tsx/*)
#   get_filename_component(name ${entry} NAME_WE)
#   add_test( tsx_inverse_${name} ossimTerraSarXSarSensorModelTest ${entry} )
#   add_test( tsx_forward_${name} ossimSentinel1SarSensorModelTest 0 ${entry})
# endforeach()

otb_add_test( NAME tsx_inverse_PANGKALANBUUN COMMAND ossimTerraSarXSarSensorModelTest 1 ${INPUTDATA}/ossimPlugins/tsx/PANGKALANBUUN/TSX1_SAR__SSC______SM_D_SRA_20080313T221955_20080313T222003.xml ${INPUTDATA}/ossimPlugins/tsx/PANGKALANBUUN/GEOREF.xml)

otb_add_test( NAME tsx_inverse_TORONTO COMMAND ossimTerraSarXSarSensorModelTest 1 ${INPUTDATA}/ossimPlugins/tsx/TORONTO/TSX1_SAR__SSC.xml ${INPUTDATA}/ossimPlugins/tsx/TORONTO/GEOREF.xml)



# otb_add_test( NAME tsx_forward_PANGKALANBUUN COMMAND ossimTerraSarXSarSensorModelTest 0 ${CMAKE_SOURCE_DIR}/data/tsx/PANGKALANBUUN/TSX1_SAR__SSC______SM_D_SRA_20080313T221955_20080313T222003.xml ${CMAKE_SOURCE_DIR}/data/tsx/PANGKALANBUUN/GEOREF.xml)

#Test with GRD product
#otb_add_test( NAME tsx_inverse_UPSALA COMMAND ossimTerraSarXSarSensorModelTest 1 ${INPUTDATA}/ossimPlugins/tsx/UPSALA_GLACIER/TSX1_SAR__MGD.xml ${INPUTDATA}/ossimPlugins/tsx/UPSALA_GLACIER/GEOREF.xml)
#otb_add_test( NAME tsx_forward_UPSALA COMMAND ossimTerraSarXSarSensorModelTest 0 ${INPUTDATA}/ossimPlugins/tsx/UPSALA_GLACIER/TSX1_SAR__MGD.xml ${INPUTDATA}/ossimPlugins/tsx/UPSALA_GLACIER/GEOREF.xml)
