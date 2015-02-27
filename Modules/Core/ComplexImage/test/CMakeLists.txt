otb_module_test()
set(OTBComplexImageTests
  otbAmplitudePhaseToRGBFunctor.cxx
  otbAmplitudePhaseToRGBFunctorNew.cxx
  otbComplexImageTestDriver.cxx  )

add_executable(otbComplexImageTestDriver ${OTBComplexImageTests})
target_link_libraries(otbComplexImageTestDriver ${OTBComplexImage-Test_LIBRARIES})
otb_module_target_label(otbComplexImageTestDriver)
otb_add_test(NAME bfTvAmplitudePhaseToRGBFunctor COMMAND otbComplexImageTestDriver
  --compare-image ${EPSILON_7}
  ${BASELINE}/bfTvAmplitudePhaseToRGBFunctor.png
  ${TEMP}/bfTvAmplitudePhaseToRGBFunctor.png
  otbAmplitudePhaseToRGBFunctor
  ${INPUTDATA}/RSAT_imagery_HH.tif
  ${TEMP}/bfTvAmplitudePhaseToRGBFunctor.png
  )
otb_add_test(NAME bfTuAmplitudePhaseToRGBFunctorNew COMMAND otbComplexImageTestDriver
  otbAmplitudePhaseToRGBFunctorNew)
