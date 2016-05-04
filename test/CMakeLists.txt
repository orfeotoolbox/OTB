otb_module_test()

#${otb-module} will be the name of this module and will not need to be #changed when this module is renamed.

set(${otb-module}Tests
   otbMPIModuleTestDriver.cxx
   otbMPIEmptyTest.cxx
   otbMPIHelloTest.cxx
   otbReadWriteTest.cxx
   otbMPIReadWriteTest.cxx
   otbMPISPTWReadWriteTest.cxx
   otbMPIBundleToPerfectSensorTest.cxx
)

# Function otb_add_test_mpi
function(otb_add_test_mpi)
   set( _OPTIONS_ARGS )
   set( _ONE_VALUE_ARGS NAME NBPROCS COMMAND)
   set( _MULTI_VALUE_ARGS )
   cmake_parse_arguments( TEST_MPI "${_OPTIONS_ARGS}" "${_ONE_VALUE_ARGS}" "${_MULTI_VALUE_ARGS}" ${ARGN} )

   # Test nb procs
   if( NOT TEST_MPI_NBPROCS )
     set(TEST_MPI_NBPROCS 2)
   endif()
   # Test command line
   foreach(arg IN LISTS TEST_MPI_UNPARSED_ARGUMENTS)
     list(APPEND ARGS ${arg})
   endforeach()
   set (test_parameters -np ${TEST_MPI_NBPROCS} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TEST_MPI_COMMAND} ${ARGS})
   otb_add_test(NAME ${TEST_MPI_NAME} COMMAND "mpirun" ${test_parameters})
endfunction()

# Boost configuration
find_package(Boost COMPONENTS system chrono REQUIRED)

add_executable(otbMPIModuleTestDriver ${${otb-module}Tests}) 
target_link_libraries(otbMPIModuleTestDriver ${${otb-module}-Test_LIBRARIES} ${Boost_SYSTEM_LIBRARY} ${Boost_CHRONO_LIBRARY})
otb_module_target_label(otbMPIModuleTestDriver)

# MPI Empty test
otb_add_test_mpi(NAME otbMPIEmptyTest
   NBPROCS 2
   COMMAND otbMPIModuleTestDriver otbMPIEmptyTest )

# MPI Hello test
otb_add_test_mpi(NAME otbMPIHelloTest
   NBPROCS 2
   COMMAND otbMPIModuleTestDriver otbMPIHelloTest )

# Read/write test
otb_add_test(NAME otbReadWriteTest
   COMMAND otbMPIModuleTestDriver 
   --compare-image ${NOTOL} ${INPUTDATA}/IMAGE_TIFF/SPOTHERITAGE_SO14001334-29-03_50482610807311047572J7_SPOT5_HRG2_XS_L1A.TIF
   ${TEMP}/imageReadWriteTest.tif
   otbReadWriteTest 
   ${INPUTDATA}/IMAGE_TIFF/SPOTHERITAGE_SO14001334-29-03_50482610807311047572J7_SPOT5_HRG2_XS_L1A.TIF
   ${TEMP}/imageReadWriteTest.tif
   )

# MPI Read/write test with 1 procs
otb_add_test_mpi(NAME otbMPIReadWriteTestNb1
   NBPROCS 1
   COMMAND otbMPIModuleTestDriver 
   --compare-image ${NOTOL} ${INPUTDATA}/IMAGE_TIFF/SPOTHERITAGE_SO14001334-29-03_50482610807311047572J7_SPOT5_HRG2_XS_L1A.TIF
   ${TEMP}/imageMPIReadWriteTestNb1.vrt
   otbMPIReadWriteTest 
   ${INPUTDATA}/IMAGE_TIFF/SPOTHERITAGE_SO14001334-29-03_50482610807311047572J7_SPOT5_HRG2_XS_L1A.TIF
   ${TEMP}/imageMPIReadWriteTestNb1
   )

# MPI Read/write test with 2 procs
otb_add_test_mpi(NAME otbMPIReadWriteTestNb2
   NBPROCS 2
   COMMAND otbMPIModuleTestDriver 
   --compare-image ${NOTOL} ${INPUTDATA}/IMAGE_TIFF/SPOTHERITAGE_SO14001334-29-03_50482610807311047572J7_SPOT5_HRG2_XS_L1A.TIF
   ${TEMP}/imageMPIReadWriteTestNb2.vrt
   otbMPIReadWriteTest 
   ${INPUTDATA}/IMAGE_TIFF/SPOTHERITAGE_SO14001334-29-03_50482610807311047572J7_SPOT5_HRG2_XS_L1A.TIF
   ${TEMP}/imageMPIReadWriteTestNb2
   )
   
# MPI Read/write test with 4 procs
otb_add_test_mpi(NAME otbMPIReadWriteTestNb4
   NBPROCS 4
   COMMAND otbMPIModuleTestDriver 
   --compare-image ${NOTOL} ${INPUTDATA}/IMAGE_TIFF/SPOTHERITAGE_SO14001334-29-03_50482610807311047572J7_SPOT5_HRG2_XS_L1A.TIF
   ${TEMP}/imageMPIReadWriteTestNb4.vrt
   otbMPIReadWriteTest 
   ${INPUTDATA}/IMAGE_TIFF/SPOTHERITAGE_SO14001334-29-03_50482610807311047572J7_SPOT5_HRG2_XS_L1A.TIF
   ${TEMP}/imageMPIReadWriteTestNb4
   )
   
# MPI/SPTW Read/write test with 1 procs
otb_add_test_mpi(NAME otbMPISPTWReadWriteTestNb1
   NBPROCS 1
   COMMAND otbMPIModuleTestDriver 
   --compare-image ${NOTOL} ${INPUTDATA}/IMAGE_TIFF/SPOTHERITAGE_SO14001334-29-03_50482610807311047572J7_SPOT5_HRG2_XS_L1A.TIF
   ${TEMP}/imageMPISPTWReadWriteTestNb1.tif
   otbMPISPTWReadWriteTest 
   ${INPUTDATA}/IMAGE_TIFF/SPOTHERITAGE_SO14001334-29-03_50482610807311047572J7_SPOT5_HRG2_XS_L1A.TIF
   ${TEMP}/imageMPISPTWReadWriteTestNb4.tif
   )
   
# MPI/SPTW Read/write test with 4 procs
otb_add_test_mpi(NAME otbMPISPTWReadWriteTestNb4
   NBPROCS 4
   COMMAND otbMPIModuleTestDriver 
   --compare-image ${NOTOL} ${INPUTDATA}/IMAGE_TIFF/SPOTHERITAGE_SO14001334-29-03_50482610807311047572J7_SPOT5_HRG2_XS_L1A.TIF
   ${TEMP}/imageMPISPTWReadWriteTestNb4.tif
   otbMPISPTWReadWriteTest 
   ${INPUTDATA}/IMAGE_TIFF/SPOTHERITAGE_SO14001334-29-03_50482610807311047572J7_SPOT5_HRG2_XS_L1A.TIF
   ${TEMP}/imageMPISPTWReadWriteTestNb2.tif
   )

# MPI Bundle To Perfect Sensor test with 1 procs
otb_add_test_mpi(NAME otbMPIBundleToPerfectSensorTestNb1
   NBPROCS 1
   COMMAND otbMPIModuleTestDriver 
   otbMPIBundleToPerfectSensorTest 
   ${INPUTDATA}/FCGC600359918/IMG_PHR1A_P_001/IMG_PHR1A_P_201604170156370_ORT_1743911101-001_R1C1.TIF
   ${INPUTDATA}/FCGC600359918/IMG_PHR1A_MS_002/IMG_PHR1A_MS_201604170156370_ORT_1743911101-002_R1C1.TIF
   ${TEMP}/imageMPIBundleToPerfectSensorTestNb1.tif
   )

# MPI Bundle To Perfect Sensor test with 2 procs
otb_add_test_mpi(NAME otbMPIBundleToPerfectSensorTestNb2
   NBPROCS 2
   COMMAND otbMPIModuleTestDriver 
   otbMPIBundleToPerfectSensorTest 
   ${INPUTDATA}/FCGC600359918/IMG_PHR1A_P_001/IMG_PHR1A_P_201604170156370_ORT_1743911101-001_R1C1.TIF
   ${INPUTDATA}/FCGC600359918/IMG_PHR1A_MS_002/IMG_PHR1A_MS_201604170156370_ORT_1743911101-002_R1C1.TIF
   ${TEMP}/imageMPIBundleToPerfectSensorTestNb2.tif
   )

#test application
otb_test_application(NAME otbMPIHelloAppTest
                      APP  MPIHelloApp
                      )
