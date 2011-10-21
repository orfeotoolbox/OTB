
IF(OTB_DATA_USE_LARGEINPUT)

add_test(NAME apTvRaOpticalCalibration_Spot5
         COMMAND otbTestDriver
                 --compare-image ${EPSILON_12}
                 ${BASELINE}/raTvLuminanceToReflectanceImageFilterAutoSpot5.tif
                 ${TEMP}/apTvRaOpticalCalibration_Spot5.tif
                 Execute $<TARGET_FILE:otbApplicationLauncherCommandLine>
                 OpticalCalibration
                 $<TARGET_FILE_DIR:otbapp_OpticalCalibration>
                 --in ${OTB_DATA_LARGEINPUT_ROOT}/SPOT5/TEHERAN/IMAGERY.TIF
                 --out ${TEMP}/apTvRaOpticalCalibration_Spot5.tif )

add_test(NAME apTvRaOpticalCalibration_Ikonos
         COMMAND otbTestDriver
                 --compare-image ${EPSILON_12}
                 ${BASELINE}/raTvLuminanceToReflectanceImageFilterAutoIkonos.tif
                 ${TEMP}/apTvRaOpticalCalibration_Ikonos.tif
                 Execute $<TARGET_FILE:otbApplicationLauncherCommandLine>
                 OpticalCalibration
                 $<TARGET_FILE_DIR:otbapp_OpticalCalibration>
                 --in ${OTB_DATA_LARGEINPUT_ROOT}/IKONOS/BLOSSEVILLE/po_2619900_pan_0000000.tif
                 --out ${TEMP}/apTvRaOpticalCalibration_Ikonos.tif )

add_test(NAME apTvRaOpticalCalibration_WV2PAN
         COMMAND otbTestDriver
                 --compare-image ${EPSILON_12}
                 ${BASELINE}/raTvLuminanceToReflectanceImageFilterAutoWV2PAN.tif
                 ${TEMP}/apTvRaOpticalCalibration_WV2PAN.tif
                 Execute $<TARGET_FILE:otbApplicationLauncherCommandLine>
                 OpticalCalibration
                 $<TARGET_FILE_DIR:otbapp_OpticalCalibration>
                 --in ${OTB_DATA_LARGEINPUT_ROOT}/WORLDVIEW2/ROME/WV-2_standard_8band_bundle_16bit/052298844010_01_P001_PAN/09DEC10103019-P2AS-052298844010_01_P001.TIF
                 --out ${TEMP}/apTvRaOpticalCalibration_WV2PAN.tif )

add_test(NAME apTvRaOpticalCalibration_WV2MUL
         COMMAND otbTestDriver
                 --compare-image ${EPSILON_12}
                 ${BASELINE}/raTvLuminanceToReflectanceImageFilterAutoWV2Multi.tif
                 ${TEMP}/apTvRaOpticalCalibration_WV2MUL.tif
                 Execute $<TARGET_FILE:otbApplicationLauncherCommandLine>
                 OpticalCalibration
                 $<TARGET_FILE_DIR:otbapp_OpticalCalibration>
                 --in ${OTB_DATA_LARGEINPUT_ROOT}/WORLDVIEW2/ROME/WV-2_standard_8band_bundle_16bit/052298844010_01_P001_MUL/09DEC10103019-M2AS-052298844010_01_P001.TIF
                 --out ${TEMP}/apTvRaOpticalCalibration_WV2MUL.tif )

add_test(NAME apTvRaOpticalCalibration_QuickbirdPAN
         COMMAND otbTestDriver
                 --compare-image ${EPSILON_12}
                 ${BASELINE}/raTvLuminanceToReflectanceImageFilterAutoQuickbirdPAN.tif
                 ${TEMP}/apTvRaOpticalCalibration_QuickbirdPAN.tif
                 Execute $<TARGET_FILE:otbApplicationLauncherCommandLine>
                 OpticalCalibration
                 $<TARGET_FILE_DIR:otbapp_OpticalCalibration>
                 --in ${OTB_DATA_LARGEINPUT_ROOT}/QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF
                 --out ${TEMP}/apTvRaOpticalCalibration_QuickbirdPAN.tif )

add_test(NAME apTvRaOpticalCalibration_QuickbirdXS
         COMMAND otbTestDriver
                 --compare-image ${EPSILON_12}
                 ${BASELINE}/raTvLuminanceToReflectanceImageFilterAutoQuickbirdXS.tif
                 ${TEMP}/apTvRaOpticalCalibration_QuickbirdXS.tif
                 Execute $<TARGET_FILE:otbApplicationLauncherCommandLine>
                 OpticalCalibration
                 $<TARGET_FILE_DIR:otbapp_OpticalCalibration>
                 --in ${OTB_DATA_LARGEINPUT_ROOT}/QUICKBIRD/TOULOUSE/000000128955_01_P001_MUL/02APR01105228-M1BS-000000128955_01_P001.TIF
                 --out ${TEMP}/apTvRaOpticalCalibration_QuickbirdXS.tif )

add_test(NAME apTvRaOpticalCalibration_Formosat
         COMMAND otbTestDriver
                 --compare-image ${EPSILON_12}
                 ${BASELINE}/raTvLuminanceToReflectanceImageFilterAutoFormosat.tif
                 ${TEMP}/apTvRaOpticalCalibration_Formosat.tif
                 Execute $<TARGET_FILE:otbApplicationLauncherCommandLine>
                 OpticalCalibration
                 $<TARGET_FILE_DIR:otbapp_OpticalCalibration>
                 --in ${OTB_DATA_LARGEINPUT_ROOT}/FORMOSAT/Sudouest_20071013_MS_fmsat/IMAGERY.TIF
                 --out ${TEMP}/apTvRaOpticalCalibration_Formosat.tif )

ENDIF(OTB_DATA_USE_LARGEINPUT)
