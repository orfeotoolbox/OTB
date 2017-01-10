otb_module_test()

set(OTBStereoTests
otbStereoTestDriver.cxx
otbBijectionCoherencyFilter.cxx
otbMulti3DMapToDEMFilter.cxx
otbAdhesionCorrectionFilter.cxx
otbStereoSensorModelToElevationMapFilter.cxx
otbStereorectificationDisplacementFieldSource.cxx
)

add_executable(otbStereoTestDriver ${OTBStereoTests})
target_link_libraries(otbStereoTestDriver ${OTBStereo-Test_LIBRARIES})
otb_module_target_label(otbStereoTestDriver)

# Tests Declaration


otb_add_test(NAME dmTuBijectionCoherencyFilterNew COMMAND otbStereoTestDriver
  otbBijectionCoherencyFilterNew)

otb_add_test(NAME dmTvBijectionCoherencyFilter COMMAND otbStereoTestDriver
  --compare-image ${EPSILON_6}
  ${BASELINE}/dmTvBijectionCoherencyFilterOutput.tif
  ${TEMP}/dmTvBijectionCoherencyFilterOutput.tif
  otbBijectionCoherencyFilter
  ${INPUTDATA}/sensor_stereo_blockmatching_output.tif
  ${INPUTDATA}/sensor_stereo_reverse_blockmatching.tif
  -24
  0
  0
  0
  ${TEMP}/dmTvBijectionCoherencyFilterOutput.tif
  )

otb_add_test(NAME dmTvMulti3DMapToDEMFilterStadiumMinROI COMMAND otbStereoTestDriver
  --compare-image ${EPSILON_6}
  ${BASELINE}/dmTvMulti3DMapToDEMFilterOutputStadiumMin.tif
  ${TEMP}/dmTvMulti3DMapToDEMFilterOutputStadiumMinROI.tif
  otbMulti3DMapToDEMFilter
  ${INPUTDATA}/Stadium3DMap1.tif
  ${INPUTDATA}/Stadium3DMapMask1.tif
  ${INPUTDATA}/Stadium3DMap2.tif
  ${INPUTDATA}/Stadium3DMapMask2.tif
  ${INPUTDATA}/Stadium3DMap3.tif
  ${INPUTDATA}/Stadium3DMapMask3.tif
  ${INPUTDATA}/Stadium3DMap4.tif
  ${INPUTDATA}/Stadium3DMapMask4.tif
  ${INPUTDATA}/Stadium3DMap5.tif
  ${INPUTDATA}/Stadium3DMapMask5.tif
  ${TEMP}/dmTvMulti3DMapToDEMFilterOutputStadiumMinROI.tif
  2.5
  0
  6
  4
  )

otb_add_test(NAME dmTvMulti3DMapToDEMFilterStadiumMin COMMAND otbStereoTestDriver
  --compare-image ${EPSILON_6}
  ${BASELINE}/dmTvMulti3DMapToDEMFilterOutputStadiumMin.tif
  ${TEMP}/dmTvMulti3DMapToDEMFilterOutputStadiumMin.tif
  otbMulti3DMapToDEMFilter
  ${INPUTDATA}/Stadium3DMap.tif
  ${INPUTDATA}/Stadium3DMapMask.tif
  ${INPUTDATA}/Stadium3DMapBis.tif
  ${INPUTDATA}/Stadium3DMapMask.tif
  ${TEMP}/dmTvMulti3DMapToDEMFilterOutputStadiumMin.tif
  2.5
  0
  1
  1
  )

otb_add_test(NAME dmTuMulti3DMapToDEMFilterStadiumAcc COMMAND otbStereoTestDriver
  otbMulti3DMapToDEMFilter
  ${INPUTDATA}/Stadium3DMap.tif
  ${INPUTDATA}/Stadium3DMapMask.tif
  ${INPUTDATA}/Stadium3DMap.tif
  ${INPUTDATA}/Stadium3DMapMask.tif
  ${TEMP}/dmTuMulti3DMapToDEMFilterOutputStadiumAcc.tif
  2.5
  3
  1
  1
  )

otb_add_test(NAME dmTvMulti3DMapToDEMFilterStadiumMeanEPSGWGS84 COMMAND otbStereoTestDriver
  --compare-image ${EPSILON_6}
  ${BASELINE}/dmTvMulti3DMapToDEMFilterOutputStadiumMean.tif
  ${TEMP}/dmTvMulti3DMapToDEMFilterOutputStadiumMeanEPSGWGS84.tif
  otbMulti3DMapToDEMFilterEPSG
  ${INPUTDATA}/Stadium3DMap.tif
  ${INPUTDATA}/Stadium3DMapMask.tif
  ${INPUTDATA}/Stadium3DMapBis.tif
  ${INPUTDATA}/Stadium3DMapMask.tif
  ${TEMP}/dmTvMulti3DMapToDEMFilterOutputStadiumMeanEPSGWGS84.tif
  2.5
  2
  1
  1
  0
  0
  1.43283322941157
  43.5843844479961
  103
  107
  0.000031003154124
  -0.000022457882103
  4326
  )

otb_add_test(NAME dmTuMulti3DMapToDEMFilterStadiumMeanEPSGUTM31N COMMAND otbStereoTestDriver
  otbMulti3DMapToDEMFilterEPSG
  ${INPUTDATA}/Stadium3DMap.tif
  ${INPUTDATA}/Stadium3DMapMask.tif
  ${INPUTDATA}/Stadium3DMapBis.tif
  ${INPUTDATA}/Stadium3DMapMask.tif
  ${TEMP}/dmTvMulti3DMapToDEMFilterOutputStadiumMeanEPSGUTM31N.tif
  2.5
  2
  1
  1
  0
  0
  373473
  4826907
  103
  107
  2.5
  -2.5
  32631
  )

otb_add_test(NAME dmTvMulti3DMapToDEMFilterStadiumMean COMMAND otbStereoTestDriver
  --compare-image ${EPSILON_6}
  ${BASELINE}/dmTvMulti3DMapToDEMFilterOutputStadiumMean.tif
  ${TEMP}/dmTvMulti3DMapToDEMFilterOutputStadiumMean.tif
  otbMulti3DMapToDEMFilter
  ${INPUTDATA}/Stadium3DMap.tif
  ${INPUTDATA}/Stadium3DMapMask.tif
  ${INPUTDATA}/Stadium3DMapBis.tif
  ${INPUTDATA}/Stadium3DMapMask.tif
  ${TEMP}/dmTvMulti3DMapToDEMFilterOutputStadiumMean.tif
  2.5
  2
  1
  1
  )

otb_add_test(NAME dmTuMulti3DMapToDEMFilterMeanMultiThreadMultiStream COMMAND otbStereoTestDriver
  otbMulti3DMapToDEMFilter
  ${BASELINE}/dmTvMultiDisparityMapTo3DFilterOutput.tif
  LARGEINPUT{PLEIADES/tristereo_sample/mask_1.tif}
  ${BASELINE}/dmTvMultiDisparityMapTo3DFilterOutput.tif
  LARGEINPUT{PLEIADES/tristereo_sample/mask_1.tif}
  ${TEMP}/dmTuMulti3DMapToDEMFilterOutputMeanMultiThreadMultiStream.tif
  2.5
  2
  8
  4)

otb_add_test(NAME dmTuMulti3DMapTo3DEMFilterNew COMMAND otbStereoTestDriver
  otbMulti3DMapToDEMFilterNew)

otb_add_test(NAME dmTvMulti3DMapToDEMFilterStadiumMeanNoData COMMAND otbStereoTestDriver
  --compare-image ${EPSILON_6}
  ${BASELINE}/dmTvMulti3DMapToDEMFilterOutputStadiumMeanNoData.tif
  ${TEMP}/dmTvMulti3DMapToDEMFilterOutputStadiumMeanNoData.tif
  otbMulti3DMapToDEMFilter
  ${INPUTDATA}/Stadium3DMap.tif
  ${INPUTDATA}/Stadium3DMapMask.tif
  ${INPUTDATA}/Stadium3DMapBis.tif
  ${INPUTDATA}/Stadium3DMapMask.tif
  ${TEMP}/dmTvMulti3DMapToDEMFilterOutputStadiumMeanNoData.tif
  0.7
  2
  1
  1
  )

otb_add_test(NAME dmTuMulti3DMapToDEMFilterAccMultiThreadMultiStream COMMAND otbStereoTestDriver
  otbMulti3DMapToDEMFilter
  ${BASELINE}/dmTvMultiDisparityMapTo3DFilterOutput.tif
  LARGEINPUT{PLEIADES/tristereo_sample/mask_1.tif}
  ${BASELINE}/dmTvMultiDisparityMapTo3DFilterOutput.tif
  LARGEINPUT{PLEIADES/tristereo_sample/mask_1.tif}
  ${TEMP}/dmTuMulti3DMapToDEMFilterOutputAccMultiThreadMultiStream.tif
  2.5
  3
  8
  4
  )

otb_add_test(NAME dmTvMulti3DMapToDEMFilterStadiumMax COMMAND otbStereoTestDriver
  --compare-image ${EPSILON_6}
  ${BASELINE}/dmTvMulti3DMapToDEMFilterOutputStadiumMax.tif
  ${TEMP}/dmTvMulti3DMapToDEMFilterOutputStadiumMax.tif
  otbMulti3DMapToDEMFilter
  ${INPUTDATA}/Stadium3DMap.tif
  ${INPUTDATA}/Stadium3DMapMask.tif
  ${INPUTDATA}/Stadium3DMapBis.tif
  ${INPUTDATA}/Stadium3DMapMask.tif
  ${TEMP}/dmTvMulti3DMapToDEMFilterOutputStadiumMax.tif
  2.5
  1
  1
  1
  )

otb_add_test(NAME dmTuMulti3DMapToDEMFilterStadiumMeanLarge COMMAND otbStereoTestDriver
  otbMulti3DMapToDEMFilter
  ${INPUTDATA}/Stadium3DMap.tif
  ${INPUTDATA}/Stadium3DMapMask.tif
  ${INPUTDATA}/Stadium3DMapBis.tif
  ${INPUTDATA}/Stadium3DMapMask.tif
  ${TEMP}/dmTuMulti3DMapToDEMFilterOutputStadiumMeanLarge.tif
  10
  2
  8
  4
  )

otb_add_test(NAME dmTvMulti3DMapToDEMFilterStadiumMeanMultiThreadMultiStream COMMAND otbStereoTestDriver
  --compare-image ${EPSILON_6}
  ${BASELINE}/dmTvMulti3DMapToDEMFilterOutputStadiumMean.tif
  ${TEMP}/dmTvMulti3DMapToDEMFilterOutputStadiumMeanMultiThreadMultiStream.tif
  otbMulti3DMapToDEMFilter
  ${INPUTDATA}/Stadium3DMap.tif
  ${INPUTDATA}/Stadium3DMapMask.tif
  ${INPUTDATA}/Stadium3DMapBis.tif
  ${INPUTDATA}/Stadium3DMapMask.tif
  ${TEMP}/dmTvMulti3DMapToDEMFilterOutputStadiumMeanMultiThreadMultiStream.tif
  2.5
  2
  6
  4
  )










otb_add_test(NAME dmTvAdhesionCorrectionFilter COMMAND otbStereoTestDriver
  --compare-n-images ${EPSILON_4} 3
  ${BASELINE}/dmTuAdhesionCorrectionMethod_Corrected.tif
  ${TEMP}/dmTuAdhesionCorrectionMethod_Corrected.tif
  ${BASELINE}/dmTuAdhesionCorrectionMethod_CorrectedMask.png
  ${TEMP}/dmTuAdhesionCorrectionMethod_CorrectedMask.png
  ${BASELINE}/dmTuAdhesionCorrectionMethod_RiskEdges.tif
  ${TEMP}/dmTuAdhesionCorrectionMethod_RiskEdges.tif
  otbAdhesionCorrectionFilter
  ${INPUTDATA}/AdhesionCorrectionFilter_MedianInput
  ${INPUTDATA}/AdhesionCorrectionFilter_MedianMaskInput.png
  ${INPUTDATA}/AdhesionCorrectionFilter_SubpixelDisparityInput
  ${INPUTDATA}/AdhesionCorrectionFilter_SubpixelMaskInput.png
  ${INPUTDATA}/AdhesionCorrectionFilter_CannyRefInput
  ${INPUTDATA}/AdhesionCorrectionFilter_CannyMedianInput
  4 0.25
  ${TEMP}/dmTuAdhesionCorrectionMethod_Corrected.tif
  ${TEMP}/dmTuAdhesionCorrectionMethod_CorrectedMask.png
  ${TEMP}/dmTuAdhesionCorrectionMethod_RiskEdges.tif
  )

otb_add_test(NAME dmTuAdhesionCorrectionFilterNew COMMAND otbStereoTestDriver
  otbAdhesionCorrectionFilterNew)

otb_add_test(NAME dmTuStereoSensorModelToElevationFilterNew COMMAND otbStereoTestDriver
  otbStereoSensorModelToElevationMapFilterNew)

#otb_add_test(NAME dmTvStereoSensorModelToElevationFilter COMMAND otbStereoTestDriver
  #--compare-image ${EPSILON_10}
  #${BASELINE}/feTvStereoSensorModelToElevationFilterOutput.tif
  #${TEMP}/feTvStereoSensorModelToElevationFilterOutput.tif
  #otbStereoSensorModelToElevationMapFilter
  #${EXAMPLEDATA}/sensor_stereo_left.tif
  #${EXAMPLEDATA}/sensor_stereo_right.tif
  #${TEMP}/feTvStereoSensorModelToElevationFilterOutput.tif
  #${INPUTDATA}/DEM/srtm_directory/
  #${INPUTDATA}/DEM/egm96.grd
  #-20
  #20
  #1
  #3
  #)

otb_add_test(NAME dmTvStereorectificationDisplacementFieldSource COMMAND otbStereoTestDriver
  --compare-n-images ${EPSILON_10} 2
  ${BASELINE}/feTvStereorectificationDeformationFieldSourceOutput1.tif
  ${TEMP}/feTvStereorectificationDisplacementFieldSourceOutput1.tif
  ${BASELINE}/feTvStereorectificationDeformationFieldSourceOutput2.tif
  ${TEMP}/feTvStereorectificationDisplacementFieldSourceOutput2.tif
  otbStereorectificationDisplacementFieldSource
  ${EXAMPLEDATA}/sensor_stereo_left.tif
  ${EXAMPLEDATA}/sensor_stereo_right.tif
  ${TEMP}/feTvStereorectificationDisplacementFieldSourceOutput1.tif
  ${TEMP}/feTvStereorectificationDisplacementFieldSourceOutput2.tif
  300
  0.5
  5
  )
otb_add_test(NAME dmTuStereorectificationDisplacementFieldSourceNew COMMAND otbStereoTestDriver
  otbStereorectificationDisplacementFieldSourceNew)
