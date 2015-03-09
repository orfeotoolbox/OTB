otb_module_test()

set(OTBFuzzyTests
otbFuzzyTestDriver.cxx
otbFuzzyVariableSetValue.cxx
otbFuzzyVariable2Values.cxx
otbFuzzyVariableNew.cxx
otbFuzzyVariableDSApplied.cxx
otbFuzzyVariableGetMaxVar.cxx
otbFuzzyDescriptorsModelManager.cxx
)

add_executable(otbFuzzyTestDriver ${OTBFuzzyTests})
target_link_libraries(otbFuzzyTestDriver ${OTBFuzzy-Test_LIBRARIES})
otb_module_target_label(otbFuzzyTestDriver)

# Tests Declaration

otb_add_test(NAME fzTvFuzzyVariableSetValue COMMAND otbFuzzyTestDriver
  otbFuzzyVariableSetValue)

otb_add_test(NAME fzTvFuzzyVariable2Values COMMAND otbFuzzyTestDriver
  otbFuzzyVariable2Values)

otb_add_test(NAME fzTuFuzzyVariableNew COMMAND otbFuzzyTestDriver
  otbFuzzyVariableNew)

otb_add_test(NAME fzTvFuzzyVariableDSApplied COMMAND otbFuzzyTestDriver
  otbFuzzyVariableDSApplied)

otb_add_test(NAME fzTvFuzzyVariableGetMaxVar COMMAND otbFuzzyTestDriver
  otbFuzzyVariableGetMaxVar)

otb_add_test(NAME fzTvFuzzyDescriptorsModelManagerTest COMMAND otbFuzzyTestDriver
  otbFuzzyDescriptorsModelManagerTest
  ${TEMP}/fzTvFuzzyDescriptorsModelManager.xml
  )

otb_add_test(NAME fzTuFuzzyDescriptorsModelManagerNew COMMAND otbFuzzyTestDriver
  otbFuzzyDescriptorsModelManagerNew)

