otb_module_test()

set(OTBUnmixingTests
otbUnmixingTestDriver.cxx
otbMDMDNMFImageFilter.cxx
otbNCLSUnmixingImageFilter.cxx
otbISRAUnmixingImageFilter.cxx
otbUnConstrainedLeastSquareImageFilter.cxx
otbSparseUnmixingImageFilterNew.cxx
otbSparseUnmixingImageFilter.cxx
)

add_executable(otbUnmixingTestDriver ${OTBUnmixingTests})
target_link_libraries(otbUnmixingTestDriver ${OTBUnmixing-Test_LIBRARIES})
otb_module_target_label(otbUnmixingTestDriver)

# Tests Declaration

otb_add_test(NAME hyTvMDMDNMFImageFilterTest2 COMMAND otbUnmixingTestDriver
  otbMDMDNMFImageFilterTest2
  ${INPUTDATA}/Hyperspectral/synthetic/hsi_cube.tif
  ${INPUTDATA}/Hyperspectral/synthetic/endmembers.tif
  ${TEMP}/hyTvMDMDNMFImageFilterTest2.tif
  100)

otb_add_test(NAME hyTvMDMDNMFImageFilterTest COMMAND otbUnmixingTestDriver
  --compare-image ${EPSILON_9}
  ${BASELINE}/hyTvMDMDNMFImageFilterTest.tif
  ${TEMP}/hyTvMDMDNMFImageFilterTest.tif
  otbMDMDNMFImageFilterTest
  ${INPUTDATA}/cupriteSubHsi.tif
  ${TEMP}/hyTvMDMDNMFImageFilterTest.tif
  100)

otb_add_test(NAME hyTuMDMDNMFImageFilterNew COMMAND otbUnmixingTestDriver
  otbMDMDNMFImageFilterNewTest)

otb_add_test(NAME hyTvNCLSUnmixingImageFilterTest COMMAND otbUnmixingTestDriver
  --compare-image ${EPSILON_9}
  ${BASELINE_FILES}/hyTvNCLSImageFilterTest.tif
  ${TEMP}/hyTvNCLSImageFilterTest.tif
  otbNCLSUnmixingImageFilterTest
  ${INPUTDATA}/Hyperspectral/synthetic/hsi_cube.tif
  ${INPUTDATA}/Hyperspectral/synthetic/endmembers.tif
  ${TEMP}/hyTvNCLSImageFilterTest.tif
  10)

otb_add_test(NAME hyTuNCLSUnmixingImageFilterNew COMMAND otbUnmixingTestDriver
  otbNCLSUnmixingImageFilterNewTest)

otb_add_test(NAME hyTvISRAUnmixingImageFilterTest COMMAND otbUnmixingTestDriver
  --compare-image ${EPSILON_9}
  ${BASELINE_FILES}/hyTvISRAUnmixingImageFilterTest.tif
  ${TEMP}/hyTvISRAUnmixingImageFilterTest.tif
  otbISRAUnmixingImageFilterTest
  ${INPUTDATA}/Hyperspectral/synthetic/hsi_cube.tif
  ${INPUTDATA}/Hyperspectral/synthetic/endmembers.tif
  ${TEMP}/hyTvISRAUnmixingImageFilterTest.tif
  10)

otb_add_test(NAME hyTuISRAUnmixingImageFilterNew COMMAND otbUnmixingTestDriver
  otbISRAUnmixingImageFilterNewTest)

otb_add_test(NAME hyTuUnConstrainedLeastSquareImageFilterNew COMMAND otbUnmixingTestDriver
  otbUnConstrainedLeastSquareImageFilterNewTest)

otb_add_test(NAME hyTvUnConstrainedLeastSquareImageFilterTest COMMAND otbUnmixingTestDriver
  --compare-image ${EPSILON_9}
  ${BASELINE_FILES}/hyTvUnConstrainedLeastSquareImageFilterTest.tif
  ${TEMP}/hyTvUnConstrainedLeastSquareImageFilterTest.tif
  otbUnConstrainedLeastSquareImageFilterTest
  ${INPUTDATA}/Hyperspectral/synthetic/hsi_cube.tif
  ${INPUTDATA}/Hyperspectral/synthetic/endmembers.tif
  ${TEMP}/hyTvUnConstrainedLeastSquareImageFilterTest.tif)

otb_add_test(NAME hyTuSparseUnmixingImageFilterNew COMMAND otbUnmixingTestDriver
  otbSparseUnmixingImageFilterNew)

