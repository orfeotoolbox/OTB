otb_module_test()

set(OTBConvolutionTests
otbConvolutionTestDriver.cxx
otbConvolutionImageFilter.cxx
otbConvolutionImageFilterNew.cxx
otbOverlapSaveConvolutionImageFilterNew.cxx
otbOverlapSaveConvolutionImageFilter.cxx
)

add_executable(otbConvolutionTestDriver ${OTBConvolutionTests})
target_link_libraries(otbConvolutionTestDriver ${OTBConvolution-Test_LIBRARIES})
otb_module_target_label(otbConvolutionTestDriver)

# Tests Declaration

otb_add_test(NAME bfTvConvolutionImageFilter COMMAND otbConvolutionTestDriver
  --compare-image ${EPSILON_7}
  ${BASELINE}/bfTvConvolutionImageFilter.tif
  ${TEMP}/bfTvConvolutionImageFilter.tif
  otbConvolutionImageFilter
  ${INPUTDATA}/QB_Suburb.png
  ${TEMP}/bfTvConvolutionImageFilter.tif
  )

otb_add_test(NAME bfTuConvolutionImageFilterNew COMMAND otbConvolutionTestDriver
  otbConvolutionImageFilterNew)

otb_add_test(NAME bfTuOverlapSaveConvolutionImageFilterNew COMMAND otbConvolutionTestDriver
  otbOverlapSaveConvolutionImageFilterNew)

otb_add_test(NAME bfTvOverlapSaveConvolutionImageFilter COMMAND otbConvolutionTestDriver
  --compare-image ${EPSILON_7}
  ${BASELINE}/bfTvConvolutionImageFilter.tif
  ${TEMP}/bfTvOverlapSaveConvolutionImageFilter.tif
  otbOverlapSaveConvolutionImageFilter
  ${INPUTDATA}/QB_Suburb.png
  ${TEMP}/bfTvOverlapSaveConvolutionImageFilter.tif
  )

