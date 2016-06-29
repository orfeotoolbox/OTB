# cmake_minimum_required(VERSION 2.6)
# SET(CMAKE_MODULE_PATH "${${PROJECT_NAME}_SOURCE_DIR}/cmake;${CMAKE_MODULE_PATH}")
otb_module_test()

message(STATUS "UT: ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}")

#===== find_cots function
include(FindPackageHandleStandardArgs)
function(find_cots NAME inc_file lib_file target)
   string(TOLOWER ${NAME} name)
   MESSAGE(STATUS "search: ${NAME}/${name} in: VAR=${${NAME}_DIR} $VAR=$ENV{${NAME}_DIR}")
   set(THE_ENV "$ENV{${NAME}_DIR}")
   # MESSAGE(STATUS "THE_ENV: ${THE_ENV}")

   # The root directory
   if    ((NOT "${${NAME}_DIR}"  STREQUAL "") AND EXISTS "${${NAME}_DIR}" AND IS_DIRECTORY "${${NAME}_DIR}")
      set(DIR "${${NAME}_DIR}")
      # nothing more to do
   elseif((NOT "${THE_ENV}"  STREQUAL "") AND EXISTS "${THE_ENV}" AND IS_DIRECTORY "${THE_ENV}")
      # MESSAGE(STATUS "setting from ${NAME}_DIR: ${THE_ENV}")
      set(DIR "${THE_ENV}")
   endif()

   # MESSAGE(STATUS "DIR: ${DIR}")
   set(${NAME}_DIR ${DIR} )

   # The include/ dir
   find_path(${NAME}_INCLUDE_DIR
      NAMES ${inc_file}
      HINTS ${DIR}/include
      PATHS ${DIR}/include)

   # The lib/ dir
   find_library(${NAME}_LIBRARY
      NAMES ${lib_file}
      HINTS ${DIR}/lib
      PATHS ${DIR}/lib)

   find_package_handle_standard_args(${NAME} DEFAULT_MSG ${NAME}_DIR ${NAME}_LIBRARY ${NAME}_INCLUDE_DIR)
   if (FIND_COTS_DEBUG)
      message(STATUS "${NAME}_DIR:         ${${NAME}_DIR}")
      message(STATUS "${NAME}_INCLUDE_DIR: ${${NAME}_INCLUDE_DIR}")
      message(STATUS "${NAME}_LIBRARY:     ${${NAME}_LIBRARY}")
   endif()
   if(${NAME}_FOUND)
      set(${NAME}_DIR          ${${NAME}_DIR}          CACHE PATH "Path to ${NAME} root installation directory" FORCE)
      set(${NAME}_LIBRARY_DIRS ${${NAME}_DIR}/lib CACHE PATH "Path to ${NAME} libraries" FORCE)
   endif()
   # set(${NAME}_INCLUDE_DIR ${NAME}_INCLUDE_DIR  PARENT_SCOPE)
   # set(${NAME}_LIBRARY     ${NAME}_LIBRARY      PARENT_SCOPE)
   set(${NAME}_FOUND     ${${NAME}_FOUND}      PARENT_SCOPE)
endfunction()

# Common dirs
include_directories(${PROJECT_SOURCE_DIR}/Modules/ThirdParty/OssimPlugins/src/ossim ${OSSIM_INCLUDE_DIR})

#==== UT for utilities
add_executable(ossimStringUtilitiesTest ossimStringUtilitiesTest.cpp)
target_link_libraries(ossimStringUtilitiesTest
   otbossimplugins
   ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
otb_module_target_label(ossimStringUtilitiesTest)
otb_add_test(NAME ossimStringUtilitiesTest COMMAND ossimStringUtilitiesTest )

add_executable(ossimTimeUtilitiesTest ossimTimeUtilitiesTest.cpp)
target_link_libraries(ossimTimeUtilitiesTest
   # ${OTBOssim_LIBRARIES}
   otbossimplugins
   ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
otb_module_target_label(ossimTimeUtilitiesTest)
otb_add_test(NAME ossimTimeUtilitiesTest COMMAND ossimTimeUtilitiesTest )

#==== Benchmarking Time conversion
# (requires google.benchmark)
find_cots(GBenchmark benchmark/benchmark.h benchmark "OSSIM Time utilities")

if (GBenchmark_FOUND)
   add_executable(ossimTimeUtilitiesBench ossimTimeUtilitiesBench.cpp)
   include_directories(${GBenchmark_INCLUDE_DIR})
   target_link_libraries(ossimTimeUtilitiesBench
      otbossimplugins
      ${GBenchmark_LIBRARY})
   otb_module_target_label(ossimTimeUtilitiesBench)
else()
   message(STATUS "Google.benchmark not found, OSSIM Time utilities benchmarking ignored.")
endif()


#===== Domain tests
#Test executables
add_executable(ossimSarSensorModelTest ossimSarSensorModelTest.cpp)
target_link_libraries(ossimSarSensorModelTest otbossimplugins)
otb_module_target_label(ossimSarSensorModelTest)

# S1
add_executable(ossimSentinel1ModelTest ossimSentinel1ModelTest.cpp)
target_link_libraries(ossimSentinel1ModelTest otbossimplugins)

# TSX (old)
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
  otb_add_test( NAME s1_inverse_${name} COMMAND ossimSentinel1ModelTest 1 ${entry})
  otb_add_test( NAME s1_forward_${name} COMMAND ossimSentinel1ModelTest 0 ${entry})
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
