otb_module_test()

set(OTBConvolutionTests
otbConvolutionTestDriver.cxx
otbConvolutionImageFilter.cxx
otbConvolutionImageFilterNew.cxx
otbOverlapSaveConvolutionImageFilterNew.cxx
otbOverlapSaveConvolutionImageFilter.cxx
otbCompareOverlapSaveAndClassicalConvolutionWithGaborFilter.cxx
otbGaborFilterGenerator.cxx
otbGaborFilterGeneratorNew.cxx
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

if(ITK_USE_FFTWD)
otb_add_test(NAME bfTvOverlapSaveConvolutionImageFilter COMMAND otbConvolutionTestDriver
  --compare-image ${EPSILON_7}
  ${BASELINE}/bfTvConvolutionImageFilter.tif
  ${TEMP}/bfTvOverlapSaveConvolutionImageFilter.tif
  otbOverlapSaveConvolutionImageFilter
  ${INPUTDATA}/QB_Suburb.png
  ${TEMP}/bfTvOverlapSaveConvolutionImageFilter.tif
  )

otb_add_test(NAME bfTvCompareOverlapSaveAndClassicalConvolutionWithGaborFilter COMMAND otbConvolutionTestDriver
  --compare-image ${EPSILON_7}
  ${TEMP}/bfTvCompareConvolutionOutput.tif
  ${TEMP}/bfTvCompareOSConvolutionoutput.tif
  otbCompareOverlapSaveAndClassicalConvolutionWithGaborFilter
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles_sub.tif
  ${TEMP}/bfTvCompareConvolutionOutput.tif
  ${TEMP}/bfTvCompareOSConvolutionoutput.tif
  64 64 #Radius
  0.02 0.025 # a b
  -45 # theta
  0.0125 0.0125 #u0 v0
  0
  )
endif()

otb_add_test(NAME bfTvGaborFilterGenerator COMMAND otbConvolutionTestDriver
  --compare-image ${EPSILON_7}
  ${BASELINE}/bfGaborFilterGeneratorTest.tif
  ${TEMP}/bfGaborFilterGeneratorTest.tif
  otbGaborFilterGenerator
  ${TEMP}/bfGaborFilterGeneratorTest.tif
  64 64 #Radius
  0.02 0.025 # a b
  -45 # theta
  0.0125 0.0125 #u0 v0
  0
  )
otb_add_test(NAME bfTuGaborFilterGeneratorNew COMMAND otbConvolutionTestDriver
  otbGaborFilterGeneratorNew
  )
