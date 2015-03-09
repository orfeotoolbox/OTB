otb_module_test()

set(OTBPanSharpeningTests
otbPanSharpeningTestDriver.cxx
otbSimpleRcsPanSharpeningFusionImageFilter.cxx
otbLmvmPanSharpeningFusionImageFilterNew.cxx
otbBayesianFusionFilterNew.cxx
otbBayesianFusionFilter.cxx
otbFusionImageBaseNew.cxx
otbSimpleRcsPanSharpeningFusionImageFilterNew.cxx
otbLmvmPanSharpeningFusionImageFilter.cxx
)

add_executable(otbPanSharpeningTestDriver ${OTBPanSharpeningTests})
target_link_libraries(otbPanSharpeningTestDriver ${OTBPanSharpening-Test_LIBRARIES})
otb_module_target_label(otbPanSharpeningTestDriver)

# Tests Declaration

otb_add_test(NAME fuTvSimpleRcsPanSharpeningFusionImageFilter COMMAND otbPanSharpeningTestDriver
  --compare-image ${EPSILON_8}  ${BASELINE}/fuTvRcsPanSharpeningFusion.tif
  ${TEMP}/fuTvRcsPanSharpeningFusion.tif
  otbSimpleRcsPanSharpeningFusionImageFilter
  ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
  ${INPUTDATA}/QB_Toulouse_Ortho_XS.tif
  ${TEMP}/fuTvRcsPanSharpeningFusion.tif
  )

otb_add_test(NAME fuTuLmvmPanSharpeningFusionImageFilterNew COMMAND otbPanSharpeningTestDriver
  otbLmvmPanSharpeningFusionImageFilterNew
  )

otb_add_test(NAME fuTuBayesianFusionFilterNew COMMAND otbPanSharpeningTestDriver
  otbBayesianFusionFilterNew
  )

otb_add_test(NAME fuTvBayesianFusionFilter COMMAND otbPanSharpeningTestDriver
  --compare-image ${NOTOL}  ${BASELINE}/fuTvBayesianFusion.tif
  ${TEMP}/fuTvBayesianFusion.tif
  otbBayesianFusionFilter
  ${INPUTDATA}/multiSpect.tif
  ${INPUTDATA}/multiSpectInterp.tif
  ${INPUTDATA}/panchro.tif
  ${TEMP}/fuTvBayesianFusion.tif
  )

otb_add_test(NAME fuTuFusionImageBaseNew COMMAND otbPanSharpeningTestDriver
  otbFusionImageBaseNew
  )

otb_add_test(NAME fuTuSimpleRcsPanSharpeningFusionImageFilterNew COMMAND otbPanSharpeningTestDriver
  otbSimpleRcsPanSharpeningFusionImageFilterNew
  )

otb_add_test(NAME fuTvLmvmPanSharpeningFusionImageFilter COMMAND otbPanSharpeningTestDriver
  --compare-image ${EPSILON_8}  ${BASELINE}/fuTvLmvmPanSharpeningFusion.tif
  ${TEMP}/fuTvLmvmPanSharpeningFusion.tif
  otbLmvmPanSharpeningFusionImageFilter
  ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
  ${INPUTDATA}/QB_Toulouse_Ortho_XS.tif
  ${TEMP}/fuTvLmvmPanSharpeningFusion.tif
  )

