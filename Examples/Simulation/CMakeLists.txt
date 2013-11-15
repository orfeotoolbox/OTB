#
# Examples on the use of segmentation algorithms
#

project(ImageSimulationExamples)
include_regular_expression("^.*$")

add_executable(ProsailModel ProsailModel.cxx )
target_link_libraries(ProsailModel OTBSimulation)


add_executable(LAIFromNDVIImageTransform LAIFromNDVIImageTransform.cxx )
target_link_libraries(LAIFromNDVIImageTransform OTBSimulation)


if( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )

set(BASELINE ${OTB_DATA_ROOT}/Baseline/Examples/Simulation)
set(BASELINE_IMAGES_OTB ${OTB_DATA_ROOT}/Baseline/OTB/Images)
set(BASELINE_IMAGES_APPLI ${OTB_DATA_ROOT}/Baseline/OTB-Applications/Images)

set(INPUTDATA_EXAMPLES ${OTB_DATA_ROOT}/Examples)
set(INPUTDATA ${OTB_DATA_ROOT}/Input)
set(TEMP ${OTB_BINARY_DIR}/Testing/Temporary)

set(EXE_TESTS ${CXX_TEST_PATH}/otbSimulationExamplesTests)

set(TOL 0.0)
set(TOL_3 0.001)
set(TOL_8 0.00000001)

# tests#
# ------- ProsailModelExampleTest ----------

add_test(siTvProsailModelExampleTest ${EXE_TESTS}
     --compare-ascii ${TOL_3} ${BASELINE}/siTvProsailModelExampleTest.txt
                   ${TEMP}/siTvProsailModelExampleTest.txt
     ProsailModel
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


add_test(siTvLAIFromNDVIImageTransfromExampleTest  ${EXE_TESTS}
 --compare-image ${TOL_8}   ${BASELINE_IMAGES_OTB}/raRAndNIRVegetationIndex_LAIFromNDVIFormosat2_verySmallFSATSWcRcNir.tif
                  ${TEMP}/siTvLAIFromNDVIImageTransfromExampleTest_verySmallFSATSW.tif
        LAIFromNDVIImageTransform
        ${INPUTDATA_EXAMPLES}/verySmallFSATSW.tif
        ${TEMP}/siTvLAIFromNDVIImageTransfromExampleTest_verySmallFSATSW.tif
        ${TEMP}/verySmallFSATSW_visu.png
        ${TEMP}/siTvLAIFromNDVIImageTransfromExampleTest_verySmallFSATSW_visu.png
        1  # red
        4  # nir
)

add_test(siTvLAIAndPROSAILToSensorResponseExampleTest  ${EXE_TESTS}
 --compare-image ${TOL_8}   ${BASELINE_IMAGES_APPLI}/siTvLAIAndPROSAILToSensorResponseSmall.tif
                  ${TEMP}/siTvLAIAndPROSAILToSensorResponseSmall.tif
        LAIAndPROSAILToSensorResponse
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


include_directories(${OTB_SOURCE_DIR}/Testing/Code)
add_executable(otbSimulationExamplesTests otbSimulationExamplesTests.cxx)
target_link_libraries(otbSimulationExamplesTests  OTBSimulation OTBCommon OTBTesting)

endif()
