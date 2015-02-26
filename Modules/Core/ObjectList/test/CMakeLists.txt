otb_module_test()
set(OTBObjectListTests
  otbObjectList.cxx
  otbObjectListNew.cxx
  otbObjectListTestDriver.cxx  )

add_executable(otbObjectListTestDriver ${OTBObjectListTests})
target_link_libraries(otbObjectListTestDriver ${OTBObjectList-Test_LIBRARIES})
otb_module_target_label(otbObjectListTestDriver)
otb_add_test(NAME coTvObjectList COMMAND otbImageBaseTestDriver
  otbObjectList
  ${INPUTDATA}/couleurs.jpg
  ${INPUTDATA}/poupees.png
  ${INPUTDATA}/couleurs_extrait.png
  )
otb_add_test(NAME coTuObjectListNew COMMAND otbImageBaseTestDriver
  otbObjectListNew)
