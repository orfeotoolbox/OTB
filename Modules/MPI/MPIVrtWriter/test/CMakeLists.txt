otb_module_test()

#${otb-module} will be the name of this module and will not need to be #changed when this module is renamed.

set(${otb-module}Tests
  otbMPIVrtWriterTestDriver.cxx
   otbMPIReadWriteTest.cxx
)

add_executable(otbMPIVrtWriterTestDriver ${${otb-module}Tests}) 
target_link_libraries(otbMPIVrtWriterTestDriver ${${otb-module}-Test_LIBRARIES})
otb_module_target_label(otbMPIVrtWriterTestDriver)


# MPI Hello test
otb_add_test_mpi(NAME otbMPIVrtWriterTest
   NBPROCS 2
   COMMAND otbMPIVrtWriterTestDriver
   --compare-image ${NOTOL}
   ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
   ${TEMP}/otbMPIVrtWriterTestOutput.vrt
   otbMPIReadWriteTest
   ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
   ${TEMP}/otbMPIVrtWriterTestOutput.vrt
   )
