otb_module_test()

set(OTBColorMapTests
otbColorMapTestDriver.cxx
otbScalarToRainbowRGBPixelFunctor.cxx
otbAmplitudePhaseToRGBFunctorNew.cxx
otbScalarToRainbowRGBPixelFunctorNew.cxx
otbAmplitudePhaseToRGBFunctor.cxx
)

add_executable(otbColorMapTestDriver ${OTBColorMapTests})
target_link_libraries(otbColorMapTestDriver ${OTBColorMap-Test_LIBRARIES})
otb_module_target_label(otbColorMapTestDriver)

# Tests Declaration

otb_add_test(NAME bfTvScalarToRainbowRGBPixelFunctor COMMAND otbColorMapTestDriver
  --compare-image ${EPSILON_7}
  ${BASELINE}/bfTvScalarToRainbowRGBPixelFunctor.png
  ${TEMP}/bfTvScalarToRainbowRGBPixelFunctor.png
  otbScalarToRainbowRGBPixelFunctor
  ${INPUTDATA}/QB_Suburb.png
  ${TEMP}/bfTvScalarToRainbowRGBPixelFunctor.png
  )

otb_add_test(NAME bfTuAmplitudePhaseToRGBFunctorNew COMMAND otbColorMapTestDriver
  otbAmplitudePhaseToRGBFunctorNew)

otb_add_test(NAME bfTuScalarToRainbowRGBPixelFunctorNew COMMAND otbColorMapTestDriver
  otbScalarToRainbowRGBPixelFunctorNew)

otb_add_test(NAME bfTvAmplitudePhaseToRGBFunctor COMMAND otbColorMapTestDriver
  --compare-image ${EPSILON_7}
  ${BASELINE}/bfTvAmplitudePhaseToRGBFunctor.png
  ${TEMP}/bfTvAmplitudePhaseToRGBFunctor.png
  otbAmplitudePhaseToRGBFunctor
  ${INPUTDATA}/RSAT_imagery_HH.tif
  ${TEMP}/bfTvAmplitudePhaseToRGBFunctor.png
  )

