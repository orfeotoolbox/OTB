otb_module_test()

#${otb-module} will be the name of this module and will not need to be #changed when this module is renamed.

set(${otb-module}Tests
   otbMPIConfigTestDriver.cxx
   otbMPIConfigTest.cxx
)

add_executable(otbMPIConfigTestDriver ${${otb-module}Tests}) 
target_link_libraries(otbMPIConfigTestDriver ${${otb-module}-Test_LIBRARIES} ${Boost_SYSTEM_LIBRARY} ${Boost_CHRONO_LIBRARY})
otb_module_target_label(otbMPIConfigTestDriver)


# MPI Hello test
otb_add_test_mpi(NAME otbMPIConfigTest
   NBPROCS 2
   COMMAND otbMPIConfigTestDriver otbMPIConfigTest )
