otb_module_test()

set(OTBEdisonMeanShiftTests
otbEdisonMeanShiftTestDriver.cxx
otbMeanShiftImageFilterNew.cxx
otbMeanShiftImageFilter.cxx
otbMeanShiftVectorImageFilterNew.cxx
otbMeanShiftVectorImageFilter.cxx
)

add_executable(otbEdisonMeanShiftTestDriver ${OTBEdisonMeanShiftTests})
target_link_libraries(otbEdisonMeanShiftTestDriver ${OTBEdisonMeanShift-Test_LIBRARIES})
otb_module_target_label(otbEdisonMeanShiftTestDriver)

# Tests Declaration

otb_add_test(NAME bfTvMeanShiftImageFilter COMMAND otbEdisonMeanShiftTestDriver
  --compare-n-images ${EPSILON_7} 4
  ${BASELINE}/bfMeanShiftImageFilterOutput.tif
  ${TEMP}/bfMeanShiftImageFilterOutput.tif
  ${BASELINE}/bfMeanShiftImageFilterClusteredOutput.tif
  ${TEMP}/bfMeanShiftImageFilterClusteredOutput.tif
  ${BASELINE}/bfMeanShiftImageFilterLabeledClusteredOutput.tif
  ${TEMP}/bfMeanShiftImageFilterLabeledClusteredOutput.tif
  ${BASELINE}/bfMeanShiftImageFilterClusterBoundariesOutput.tif
  ${TEMP}/bfMeanShiftImageFilterClusterBoundariesOutput.tif
  otbMeanShiftImageFilter
  ${INPUTDATA}/QB_Suburb.png
  ${TEMP}/bfMeanShiftImageFilterOutput.tif
  ${TEMP}/bfMeanShiftImageFilterClusteredOutput.tif
  ${TEMP}/bfMeanShiftImageFilterLabeledClusteredOutput.tif
  ${TEMP}/bfMeanShiftImageFilterClusterBoundariesOutput.tif
  16 16 10 1.0
  )

otb_add_test(NAME bfTvMeanShiftImageFilterEDISON COMMAND otbEdisonMeanShiftTestDriver
  --compare-image ${EPSILON_7}
  ${BASELINE}/bfTvMeanShiftFilterSpectralOutputOptim.tif
  ${TEMP}/bfTvMeanShiftImageFilterOutputOptim.tif
  otbMeanShiftImageFilter
  ${INPUTDATA}/MeanShiftTest.tif
  ${TEMP}/bfTvMeanShiftImageFilterOutputOptim.tif
  ${TEMP}/bfTvMeanShiftImageFilterClusteredOutputOptim.tif
  ${TEMP}/bfTvMeanShiftImageFilterLabeledClusteredOutputOptim.tif
  ${TEMP}/bfTvMeanShiftImageFilterClusterBoundariesOutpuOptim.tif
  2 10 10 0.1
  )

otb_add_test(NAME bfTvMeanShiftImageFilterQBSuburb COMMAND otbEdisonMeanShiftTestDriver
  otbMeanShiftImageFilter
  ${INPUTDATA}/QB_Suburb.png
  ${TEMP}/bfMeanShiftImageFilterOutput_QBSuburb.tif
  ${TEMP}/bfMeanShiftImageFilterClusteredOutput_QBSuburb.tif
  ${TEMP}/bfMeanShiftImageFilterLabeledClusteredOutput_QBSuburb.tif
  ${TEMP}/bfMeanShiftImageFilterClusterBoundariesOutput_QBSuburb.tif
  4 50 10 1.0
  )

otb_add_test(NAME bfTuMeanShiftImageFilterNew COMMAND otbEdisonMeanShiftTestDriver
  otbMeanShiftImageFilterNew )

otb_add_test(NAME bfTuMeanShiftSmoothingImageFilterNew COMMAND otbEdisonMeanShiftTestDriver
  otbMeanShiftImageFilterNew )

otb_add_test(NAME bfTuMeanShiftVectorImageFilterNew COMMAND otbEdisonMeanShiftTestDriver
  otbMeanShiftVectorImageFilterNew )

otb_add_test(NAME bfTvMeanShiftVectorImageFilterQBPAN COMMAND otbEdisonMeanShiftTestDriver
  otbMeanShiftVectorImageFilter
  ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
  ${TEMP}/bfMeanShiftImageFilterOutput_PAN.tif
  ${TEMP}/bfMeanShiftImageFilterClusteredOutputValid_PAN.tif
  ${TEMP}/bfMeanShiftImageFilterLabeledClusteredOutputValid_PAN.tif
  ${TEMP}/bfMeanShiftImageFilterClusterBoundariesOutputValid_PAN.tif
  5 15 100 1.0
  )

otb_add_test(NAME bfTvMeanShiftVectorImageFilter COMMAND otbEdisonMeanShiftTestDriver
  --compare-n-images ${EPSILON_7} 4
  ${BASELINE}/bfMeanShiftVectorImageFilterOutput.tif
  ${TEMP}/bfMeanShiftVectorImageFilterOutput.tif
  ${BASELINE}/bfMeanShiftVectorImageFilterClusteredOutput.tif
  ${TEMP}/bfMeanShiftVectorImageFilterClusteredOutput.tif
  ${BASELINE}/bfMeanShiftVectorImageFilterLabeledClusteredOutput.tif
  ${TEMP}/bfMeanShiftVectorImageFilterLabeledClusteredOutput.tif
  ${BASELINE}/bfMeanShiftVectorImageFilterClusterBoundariesOutput.tif
  ${TEMP}/bfMeanShiftVectorImageFilterClusterBoundariesOutput.tif
  otbMeanShiftVectorImageFilter
  ${INPUTDATA}/qb_RoadExtract2sub200x200.tif
  ${TEMP}/bfMeanShiftVectorImageFilterOutput.tif
  ${TEMP}/bfMeanShiftVectorImageFilterClusteredOutput.tif
  ${TEMP}/bfMeanShiftVectorImageFilterLabeledClusteredOutput.tif
  ${TEMP}/bfMeanShiftVectorImageFilterClusterBoundariesOutput.tif
  16 16 10 1.0
  )

otb_add_test(NAME bfTuMeanShiftVectorImageFilterSPOT5 COMMAND otbEdisonMeanShiftTestDriver
  otbMeanShiftVectorImageFilter
  ${INPUTDATA}/SPOT5_EXTRACTS/Arcachon/Arcachon_extrait_3852_3319_546_542.tif
  ${TEMP}/bfMeanShiftImageFilterOutput_SPOT5.tif
  ${TEMP}/bfMeanShiftImageFilterClusteredOutput_SPOT5.tif
  ${TEMP}/bfMeanShiftImageFilterLabeledClusteredOutput_SPOT5.tif
  ${TEMP}/bfMeanShiftImageFilterClusterBoundariesOutput_SPOT5.tif
  4 10 100 1.0
  )

otb_add_test(NAME bfTuMeanShiftVectorImageFilterQBRoad COMMAND otbEdisonMeanShiftTestDriver
  otbMeanShiftVectorImageFilter
  ${INPUTDATA}/qb_RoadExtract.img
  ${TEMP}/bfMeanShiftImageFilterOutput_QBRoad.tif
  ${TEMP}/bfMeanShiftImageFilterClusteredOutput_QBRoad.tif
  ${TEMP}/bfMeanShiftImageFilterLabeledClusteredOutput_QBRoad.tif
  ${TEMP}/bfMeanShiftImageFilterClusterBoundariesOutput_QBRoad.tif
  4 30 100 1.0
  )

otb_add_test(NAME bfTuMeanShiftVectorImageFilterROIQBMul4 COMMAND otbEdisonMeanShiftTestDriver
  otbMeanShiftVectorImageFilter
  ${INPUTDATA}/ROI_QB_MUL_4.tif
  ${TEMP}/bfMeanShiftImageFilterOutput_ROIQBMul4.tif
  ${TEMP}/bfMeanShiftImageFilterClusteredOutput_ROIQBMul4.tif
  ${TEMP}/bfMeanShiftImageFilterLabeledClusteredOutput_ROIQBMul4.tif
  ${TEMP}/bfMeanShiftImageFilterClusterBoundariesOutput_ROIQBMul4.tif
  4 50 100 1.0
  )

