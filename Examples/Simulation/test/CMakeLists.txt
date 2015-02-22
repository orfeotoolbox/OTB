set(BASELINE ${OTB_DATA_ROOT}/Baseline/Examples/Simulation)
set(INPUTDATA ${OTB_DATA_ROOT}/Examples)

# ------- ProsailModelExampleTest ----------

otb_add_test(NAME siTvProsailModelExampleTest COMMAND ${OTB_TEST_DRIVER}
  --compare-ascii ${TOL_3}
    ${BASELINE}/siTvProsailModelExampleTest.txt
    ${TEMP}/siTvProsailModelExampleTest.txt
  Execute $<TARGET_FILE:ProsailModel>
    30.0 #cab
    10.0 #car
    0.0 #CBrown
    0.015 #Cw
    0.009 #Cm
    1.2 #N
    2 #LAI
    50 #Angl
    1 #PSoil
    70 #Skyl
    0.2 #HSpot
    30 #TTS
    0 #TTO
    0 #PSI
    ${TEMP}/siTvProsailModelExampleTest.txt
)


otb_add_test(NAME siTvLAIFromNDVIImageTransfromExampleTest  COMMAND ${OTB_TEST_DRIVER}
  --compare-image ${TOL_8}
    ${BASELINE_IMAGES_OTB}/raRAndNIRVegetationIndex_LAIFromNDVIFormosat2_verySmallFSATSWcRcNir.tif
    ${TEMP}/siTvLAIFromNDVIImageTransfromExampleTest_verySmallFSATSW.tif
  Execute $<TARGET_FILE:LAIFromNDVIImageTransform>
    ${INPUTDATA_EXAMPLES}/verySmallFSATSW.tif
    ${TEMP}/siTvLAIFromNDVIImageTransfromExampleTest_verySmallFSATSW.tif
    ${TEMP}/verySmallFSATSW_visu.png
    ${TEMP}/siTvLAIFromNDVIImageTransfromExampleTest_verySmallFSATSW_visu.png
    1  # red
    4  # nir
)

otb_add_test(NAME siTvLAIAndPROSAILToSensorResponseExampleTest  COMMAND ${OTB_TEST_DRIVER}
--compare-image ${TOL_8}
  ${BASELINE_IMAGES_APPLI}/siTvLAIAndPROSAILToSensorResponseSmall.tif
  ${TEMP}/siTvLAIAndPROSAILToSensorResponseSmall.tif
Execute $<TARGET_FILE:LAIAndPROSAILToSensorResponse>
  ${INPUTDATA}/LAIveryverySmallFSATSW.tif
  ${TEMP}/siTvLAIAndPROSAILToSensorResponseSmall.tif
  ${INPUTDATA}/Simu_label_LAI_small.png
  ${INPUTDATA}/Simu_mask_LAI_0_small.png
  ${INPUTDATA}/label-params-SO-2006-Level-2.txt
  ${INPUTDATA}/acqui-params.txt
  ${INPUTDATA}/Radiometry/FORMOSAT2/RSI/rep6S.dat
  5
  ${INPUTDATA}/
)
