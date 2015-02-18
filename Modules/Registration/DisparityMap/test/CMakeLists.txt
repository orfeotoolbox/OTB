otb_module_test()

set(OTBDisparityMapTests
otbDisparityMapTestDriver.cxx
otbDisparityMapEstimationMethod.cxx
otbDisparityMapToDEMFilter.cxx
otbDisparityMapMedianFilter.cxx
otbDisparityTranslateFilter.cxx
otbSubPixelDisparityImageFilter.cxx
otbDisparityMapEstimationMethodNew.cxx
otbDisparityMapTo3DFilter.cxx
otbMultiDisparityMapTo3DFilter.cxx
)

add_executable(otbDisparityMapTestDriver ${OTBDisparityMapTests})
target_link_libraries(otbDisparityMapTestDriver ${OTBDisparityMap-Test_LIBRARIES})
otb_module_target_label(otbDisparityMapTestDriver)

# Tests Declaration

otb_add_test(NAME dmTvDisparityMapEstimationMethod COMMAND otbDisparityMapTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/dmDisparityMapEstimationOutput1.txt
  ${TEMP}/dmDisparityMapEstimationOutput1.txt
  otbDisparityMapEstimationMethod
  ${INPUTDATA}/fixed.png
  ${INPUTDATA}/moving.png
  ${INPUTDATA}/pointSet.png
  ${TEMP}/dmDisparityMapEstimationOutput1.txt
  20 20
  )

otb_add_test(NAME dmTvDisparityMapToDEMFilter COMMAND otbDisparityMapTestDriver
  --compare-image ${NOTOL}
  ${BASELINE_EXAMPLE}/elevationOutput.tif
  ${TEMP}/dmTvDisparityMapToDEMFilterOutput.tif
  otbDisparityMapToDEMFilter
  ${INPUTDATA}/sensor_stereo_blockmatching_output.tif
  ${EXAMPLEDATA}/sensor_stereo_left.tif
  ${EXAMPLEDATA}/sensor_stereo_right.tif
  ${INPUTDATA}/sensor_stereo_rectif_left.tif
  ${INPUTDATA}/sensor_stereo_rectif_right.tif
  ${TEMP}/dmTvDisparityMapToDEMFilterOutput.tif
  130
  220
  140
  2.5
  ${INPUTDATA}/sensor_stereo_dmap_mask.tif
  )

otb_add_test(NAME dmTuDisparityMapToDEMFilterNew COMMAND otbDisparityMapTestDriver
  otbDisparityMapToDEMFilterNew)

otb_add_test(NAME dmTuDisparityMapMedianFilterNew COMMAND otbDisparityMapTestDriver
  otbDisparityMapMedianFilterNew)

otb_add_test(NAME dmTvDisparityMapMedianFilter COMMAND otbDisparityMapTestDriver
  --compare-image ${NOTOL}
  ${BASELINE}/dmTvDisparityMapMedianFilterOutput.tif
  ${TEMP}/dmTvDisparityMapMedianFilterOutput.tif
  otbDisparityMapMedianFilter
  ${INPUTDATA}/StereoHDisparity.tif
  ${TEMP}/dmTvDisparityMapMedianFilterOutput.tif
  2
  2.0
  )

otb_add_test(NAME dmTvDisparityTranslateFilter COMMAND otbDisparityMapTestDriver
  --compare-image ${EPSILON_6}
  ${BASELINE}/dmTvDisparityTranslateFilterOutput.tif
  ${TEMP}/dmTvDisparityTranslateFilterOutput.tif
  otbDisparityTranslateFilter
  ${INPUTDATA}/sensor_stereo_blockmatching_output.tif
  ${INPUTDATA}/sensor_stereo_rectif_inv_left.tif
  ${INPUTDATA}/sensor_stereo_rectif_right.tif
  ${EXAMPLEDATA}/sensor_stereo_left.tif
  ${TEMP}/dmTvDisparityTranslateFilterOutput.tif
  )

otb_add_test(NAME dmTuDisparityTranslateFilterNew COMMAND otbDisparityMapTestDriver
  otbDisparityTranslateFilterNew)

otb_add_test(NAME dmTvSubPixelDisparityImageFilterNCC COMMAND otbDisparityMapTestDriver
  --compare-n-images ${NOTOL} 3
  ${BASELINE}/dmTvSubPixelWiseBlockMatchingImageFilterNCCOutputHDisparity.tif
  ${TEMP}/dmTvSubPixelWiseBlockMatchingImageFilterNCCOutputHDisparity.tif
  ${BASELINE}/dmTvSubPixelWiseBlockMatchingImageFilterNCCOutputVDisparity.tif
  ${TEMP}/dmTvSubPixelWiseBlockMatchingImageFilterNCCOutputVDisparity.tif
  ${BASELINE}/dmTvSubPixelWiseBlockMatchingImageFilterNCCOutputMetric.tif
  ${TEMP}/dmTvSubPixelWiseBlockMatchingImageFilterNCCOutputMetric.tif
  otbSubPixelDisparityImageFilter
  ${EXAMPLEDATA}/StereoFixed.png
  ${EXAMPLEDATA}/StereoMoving.png
  ${INPUTDATA}/StereoPixelWiseBlockMatchingDisparity.tif
  ${INPUTDATA}/StereoPixelWiseBlockMatchingMetric.tif
  ${TEMP}/dmTvSubPixelWiseBlockMatchingImageFilterNCCOutputHDisparity.tif
  ${TEMP}/dmTvSubPixelWiseBlockMatchingImageFilterNCCOutputVDisparity.tif
  ${TEMP}/dmTvSubPixelWiseBlockMatchingImageFilterNCCOutputMetric.tif
  1 # NCC
  1 # Parabolic
  2 # radius
  -3 +3 # hdisp treshold
  -2 2 # vdisp treshold
  )

otb_add_test(NAME dmTuSubPixelDisparityImageFilterNew COMMAND otbDisparityMapTestDriver
  otbSubPixelDisparityImageFilterNew)

otb_add_test(NAME dmTuDisparityMapEstimationMethodNew COMMAND otbDisparityMapTestDriver
  otbDisparityMapEstimationMethodNew)

otb_add_test(NAME dmTvDisparityMapTo3DFilter COMMAND otbDisparityMapTestDriver
  --compare-image ${EPSILON_6}
  ${BASELINE}/dmTvDisparityMapTo3DFilterOutput.tif
  ${TEMP}/dmTvDisparityMapTo3DFilterOutput.tif
  otbDisparityMapTo3DFilter
  ${INPUTDATA}/sensor_stereo_blockmatching_output.tif
  ${EXAMPLEDATA}/sensor_stereo_left.tif
  ${EXAMPLEDATA}/sensor_stereo_right.tif
  ${INPUTDATA}/sensor_stereo_rectif_left.tif
  ${INPUTDATA}/sensor_stereo_rectif_right.tif
  ${TEMP}/dmTvDisparityMapTo3DFilterOutput.tif
  ${INPUTDATA}/sensor_stereo_dmap_mask.tif
  )

otb_add_test(NAME dmTuDisparityMapTo3DFilterNew COMMAND otbDisparityMapTestDriver
  otbDisparityMapTo3DFilterNew)

otb_add_test(NAME dmTuMultiDisparityMapTo3DFilterNew COMMAND otbDisparityMapTestDriver
  otbMultiDisparityMapTo3DFilterNew)

otb_add_test(NAME dmTvMultiDisparityMapTo3DFilter COMMAND otbDisparityMapTestDriver
  --compare-n-images ${EPSILON_6} 2
  ${BASELINE}/dmTvMultiDisparityMapTo3DFilterOutput.tif
  ${TEMP}/dmTvMultiDisparityMapTo3DFilterOutput.tif
  ${BASELINE}/dmTvMultiDisparityMapTo3DFilterResidue.tif
  ${TEMP}/dmTvMultiDisparityMapTo3DFilterResidue.tif
  otbMultiDisparityMapTo3DFilter
  LARGEINPUT{PLEIADES/tristereo_sample/master_pan.tif}
  LARGEINPUT{PLEIADES/tristereo_sample/slave_pan_1.tif}
  LARGEINPUT{PLEIADES/tristereo_sample/slave_pan_2.tif}
  LARGEINPUT{PLEIADES/tristereo_sample/phys_disp_1.tif}
  LARGEINPUT{PLEIADES/tristereo_sample/phys_disp_2.tif}
  LARGEINPUT{PLEIADES/tristereo_sample/mask_1.tif}
  LARGEINPUT{PLEIADES/tristereo_sample/mask_2.tif}
  ${TEMP}/dmTvMultiDisparityMapTo3DFilterOutput.tif
  ${TEMP}/dmTvMultiDisparityMapTo3DFilterResidue.tif
  )

