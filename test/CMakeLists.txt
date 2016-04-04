otb_module_test()

#${otb-module} will be the name of this module and will not need to be #changed when this module is renamed.

set(${otb-module}Tests
   otbMPIModuleTestDriver.cxx
   otbMPIEmptyTest.cxx
   otbMPIHelloTest.cxx
   otbMPIReadWriteTest.cxx
)

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

add_executable(otbMPIModuleTestDriver ${${otb-module}Tests}) 
target_link_libraries(otbMPIModuleTestDriver ${${otb-module}-Test_LIBRARIES})
otb_module_target_label(otbMPIModuleTestDriver)

# MPI Empty test
otb_add_test_mpi(NAME otbMPIEmptyTest
   NBPROCS 2
   COMMAND otbMPIModuleTestDriver otbMPIEmptyTest )

# MPI Hello test
otb_add_test_mpi(NAME otbMPIHelloTest
   NBPROCS 2
   COMMAND otbMPIModuleTestDriver otbMPIHelloTest )

# MPI Read/write test
otb_add_test_mpi(NAME otbMPIReadWriteTest
   NBPROCS 2
   COMMAND otbMPIModuleTestDriver otbMPIReadWriteTest /ptmp/sarrazie/IMAGE_TIFF/SPOTHERITAGE_SO14001334-29-03_50482610807311047572J7_SPOT5_HRG2_XS_L1A.TIF toto )

#test application
otb_test_application(NAME otbMPIHelloAppTest
                      APP  MPIHelloApp
                      )
