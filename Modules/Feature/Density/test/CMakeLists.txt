otb_module_test()

set(OTBDensityTests
otbDensityTestDriver.cxx
otbPointSetDensityFunctionNew.cxx
otbPointSetDensityGaussianFunctionNew.cxx
otbPointSetToDensityImageFilterNew.cxx
otbPointSetDensityGaussianFunctionTest.cxx
otbPointSetDensityEpanechnikovFunctionNew.cxx
otbPointSetDensityEpanechnikovFunctionTest.cxx
otbKeyPointDensityImageFilterTest.cxx
otbKeyPointDensityImageFilterNew.cxx
otbPointSetToDensityImageFilterTest.cxx
otbPointSetDensityFunctionTest.cxx
)

add_executable(otbDensityTestDriver ${OTBDensityTests})
target_link_libraries(otbDensityTestDriver ${OTBDensity-Test_LIBRARIES})
otb_module_target_label(otbDensityTestDriver)

# Tests Declaration

otb_add_test(NAME bfTuPointSetDensityFunctionNew COMMAND otbDensityTestDriver
  otbPointSetDensityFunctionNew
  )

otb_add_test(NAME bfTuPointSetDensityGaussianFunctionNew COMMAND otbDensityTestDriver
  otbPointSetDensityGaussianFunctionNew
  )

otb_add_test(NAME bfTuPointSetToDensityImageFilterNew COMMAND otbDensityTestDriver
  otbPointSetToDensityImageFilterNew)

otb_add_test(NAME bfTvPointSetDensityGaussianFunctionTest COMMAND otbDensityTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/bfTvPointSetDensityGaussianFunctionOutputAscii.txt
  ${TEMP}/bfTvPointSetDensityGaussianFunctionOutputAscii.txt
  otbPointSetDensityGaussianFunctionTest
  ${TEMP}/bfTvPointSetDensityGaussianFunctionOutputAscii.txt
  )

otb_add_test(NAME bfTuPointSetDensityEpanechnikovFunctionNew COMMAND otbDensityTestDriver
  otbPointSetDensityEpanechnikovFunctionNew
  )

otb_add_test(NAME bfTvPointSetDensityEpanechnikovFunctionTest COMMAND otbDensityTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/bfTvPointSetDensityEpanechnikovFunctionOutputAscii.txt
  ${TEMP}/bfTvPointSetDensityEpanechnikovFunctionOutputAscii.txt
  otbPointSetDensityEpanechnikovFunctionTest
  ${TEMP}/bfTvPointSetDensityEpanechnikovFunctionOutputAscii.txt
  )

otb_add_test(NAME bfTvKeyPointDensityImageFilterOutputImage COMMAND otbDensityTestDriver
  --compare-image ${NOTOL}
  ${BASELINE}/bfTvKeyPointDensityImageFilterOutputImage.tif
  ${TEMP}/bfTvKeyPointDensityImageFilterOutputImage.tif
  otbKeyPointDensityImageFilterTest
  ${INPUTDATA}/QB_Suburb.png
  ${TEMP}/bfTvKeyPointDensityImageFilterOutputImage.tif
  3 5 10
  )

otb_add_test(NAME bfTuKeyPointDensityImageFilterNew COMMAND otbDensityTestDriver
  otbKeyPointDensityImageFilterNew
  )

otb_add_test(NAME bfTvPointSetToDensityImageFilterTest COMMAND otbDensityTestDriver
  --compare-image ${NOTOL}
  ${BASELINE}/bfTvPointSetToDensityImageFilterOutputImage.tif
  ${TEMP}/bfTvPointSetToDensityImageFilterOutputImage.tif
  otbPointSetToDensityImageFilterTest
  ${INPUTDATA}/QB_Suburb.png
  ${TEMP}/bfTvPointSetToDensityImageFilterOutputImage.tif
  3 5 10
  )

otb_add_test(NAME bfTvPointSetDensityFunctionTest COMMAND otbDensityTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/bfTvPointSetDensityFunctionOutputAscii.txt
  ${TEMP}/bfTvPointSetDensityFunctionOutputAscii.txt
  otbPointSetDensityFunctionTest
  ${TEMP}/bfTvPointSetDensityFunctionOutputAscii.txt
  )

