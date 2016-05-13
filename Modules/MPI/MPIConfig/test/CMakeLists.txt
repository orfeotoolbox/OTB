otb_module_test()

#${otb-module} will be the name of this module and will not need to be #changed when this module is renamed.

set(${otb-module}Tests
   otbMPIConfigTestDriver.cxx
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

add_executable(otbMPIConfigTestDriver ${${otb-module}Tests}) 
target_link_libraries(otbMPIConfigTestDriver ${${otb-module}-Test_LIBRARIES} ${Boost_SYSTEM_LIBRARY} ${Boost_CHRONO_LIBRARY})
otb_module_target_label(otbMPIConfigTestDriver)


# MPI Hello test
otb_add_test_mpi(NAME otbMPIHelloTest
   NBPROCS 2
   COMMAND otbMPIConfigTestDriver otbMPIHelloTest )
