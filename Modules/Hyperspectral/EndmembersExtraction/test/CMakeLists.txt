otb_module_test()

set(OTBEndmembersExtractionTests
otbEndmembersExtractionTestDriver.cxx
otbEigenvalueLikelihoodMaximization.cxx
otbVirtualDimensionality.cxx
otbVCAImageFilter.cxx
)

add_executable(otbEndmembersExtractionTestDriver ${OTBEndmembersExtractionTests})
target_link_libraries(otbEndmembersExtractionTestDriver ${OTBEndmembersExtraction-Test_LIBRARIES})
otb_module_target_label(otbEndmembersExtractionTestDriver)

# Tests Declaration

otb_add_test(NAME hyTvEigenvalueLikelihoodMaximizationTest COMMAND otbEndmembersExtractionTestDriver
  --compare-ascii ${EPSILON_9}
  ${BASELINE_FILES}/hyTvEigenvalueLikelihoodMaximizationTest.txt
  ${TEMP}/hyTvEigenvalueLikelihoodMaximizationTest.txt
  otbEigenvalueLikelihoodMaximizationTest
  ${INPUTDATA}/Hyperspectral/synthetic/hsi_cube.tif
  ${TEMP}/hyTvEigenvalueLikelihoodMaximizationTest.txt)

otb_add_test(NAME hyTuEigenvalueLikelihoodMaximizationNew COMMAND otbEndmembersExtractionTestDriver
  otbEigenvalueLikelihoodMaximizationNewTest)

otb_add_test(NAME hyTuVirtualDimensionalityNew COMMAND otbEndmembersExtractionTestDriver
  otbVirtualDimensionalityNewTest)

otb_add_test(NAME hyTvVirtualDimensionalityTest COMMAND otbEndmembersExtractionTestDriver
  --compare-ascii ${EPSILON_9}
  ${BASELINE_FILES}/hyTvVirtualDimensionalityTest.txt
  ${TEMP}/hyTvVirtualDimensionalityTest.txt
  otbVirtualDimensionalityTest
  ${INPUTDATA}/Hyperspectral/synthetic/hsi_cube.tif
  ${TEMP}/hyTvVirtualDimensionalityTest.txt
  1E-3)

otb_add_test(NAME hyTvVCAImageFilterTest1 COMMAND otbEndmembersExtractionTestDriver
  --compare-image ${EPSILON_9}
  ${BASELINE}/TvHyVertexComponentAnalysisImage.tif
  ${TEMP}/hyTvVCAImageFilterTest.tif
  otbVCAImageFilterTestHighSNR
  ${INPUTDATA}/Hyperspectral/synthetic/hsi_cube.tif
  ${TEMP}/hyTvVCAImageFilterTest.tif
  5 )

otb_add_test(NAME hyTuVCAImageFilterNew COMMAND otbEndmembersExtractionTestDriver
  otbVCAImageFilterNew
  ${INPUTDATA}/Hyperspectral/synthetic/hsi_cube.tif
  ${TEMP}/synthetic_vca_5.hdr
  5 )

