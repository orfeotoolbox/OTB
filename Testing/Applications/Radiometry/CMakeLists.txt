
OTB_TEST_APPLICATION(NAME apTvRaOpticalCalibration_QuickbirdPAN
                     APP  OpticalCalibration
                     OPTIONS -in ${INPUTDATA}/QB_PAN_ROI_1000_100.tif
                             -level toa
                             -out ${TEMP}/apTvRaOpticalCalibration_QuickbirdPAN.tif
                     VALID   --compare-image ${EPSILON_7}
                             ${BASELINE}/raTvLuminanceToReflectanceImageFilterAutoQuickbirdPAN.tif
                             ${TEMP}/apTvRaOpticalCalibration_QuickbirdPAN.tif )

OTB_TEST_APPLICATION(NAME apTvRaOpticalCalibration_QuickbirdXS
                     APP  OpticalCalibration
                     OPTIONS -in ${INPUTDATA}/QB_MUL_ROI_1000_100.tif
                             -level toa
                             -clamp false
                             -out ${TEMP}/apTvRaOpticalCalibration_QuickbirdXS.tif
                     VALID   --compare-image ${EPSILON_7}
                             ${BASELINE}/raTvLuminanceToReflectanceImageFilterAutoQuickbirdXS.tif
                             ${TEMP}/apTvRaOpticalCalibration_QuickbirdXS.tif )

OTB_TEST_APPLICATION(NAME apTvRaOpticalCalibration_Spot5
                     APP  OpticalCalibration
                     OPTIONS -in ${INPUTDATA}/SPOT5_ROI_1000_100.tif
                             -level toa
                             -out ${TEMP}/apTvRaOpticalCalibration_Spot5.tif
                     VALID   --compare-image ${EPSILON_7}
                             ${BASELINE}/raTvLuminanceToReflectanceImageFilterAutoSpot5.tif
                             ${TEMP}/apTvRaOpticalCalibration_Spot5.tif )

OTB_TEST_APPLICATION(NAME apTvRaOpticalCalibration_Ikonos
                     APP  OpticalCalibration
                     OPTIONS -in ${INPUTDATA}/IKONOS_BLOSSEVILLE_ROI_1000_100.tif
                             -level toa
                             -out ${TEMP}/apTvRaOpticalCalibration_Ikonos.tif
                     VALID   --compare-image ${EPSILON_7}
                             ${BASELINE}/raTvLuminanceToReflectanceImageFilterAutoIkonos.tif
                             ${TEMP}/apTvRaOpticalCalibration_Ikonos.tif )

OTB_TEST_APPLICATION(NAME apTvRaOpticalCalibration_WV2PAN
                     APP  OpticalCalibration
                     OPTIONS -in ${INPUTDATA}/WV2_PAN_ROI_1000_100.tif
                             -level toa
                             -out ${TEMP}/apTvRaOpticalCalibration_WV2PAN.tif
                     VALID   --compare-image ${EPSILON_7}
                             ${BASELINE}/raTvLuminanceToReflectanceImageFilterAutoWV2PAN.tif
                             ${TEMP}/apTvRaOpticalCalibration_WV2PAN.tif )

OTB_TEST_APPLICATION(NAME apTvRaOpticalCalibration_WV2Multi
                     APP  OpticalCalibration
                     OPTIONS -in ${INPUTDATA}/WV2_MUL_ROI_1000_100.tif
                             -level toa
                             -out ${TEMP}/apTvRaOpticalCalibration_WV2Multi.tif
                     VALID   --compare-image ${EPSILON_7}
                             ${BASELINE}/raTvLuminanceToReflectanceImageFilterAutoWV2Multi.tif
                             ${TEMP}/apTvRaOpticalCalibration_WV2Multi.tif )

OTB_TEST_APPLICATION(NAME apTvRaOpticalCalibration_Formosat
                     APP  OpticalCalibration
                     OPTIONS -in ${INPUTDATA}/FORMOSAT_ROI_1000_100.tif
                             -level toa
                             -out ${TEMP}/apTvRaOpticalCalibration_Formosat.tif
                     VALID   --compare-image ${EPSILON_7}
                             ${BASELINE}/raTvLuminanceToReflectanceImageFilterAutoFormosat.tif
                             ${TEMP}/apTvRaOpticalCalibration_Formosat.tif )

IF (OTB_DATA_USE_LARGEINPUT)
OTB_TEST_APPLICATION(NAME apTvRaOpticalCalibration_toa_PHR1A
                     APP  OpticalCalibration
                     OPTIONS -in ${LARGEDATA}/PLEIADES/MAIDO_JP2_DIMAPv2_PRIMARY_MS_lossy_12bits/IMG_PHR1A_MS_002/IMG_PHR1A_MS_201206050630064_SEN_559102101-002_R1C1.JP2
                             -level toa
                             -milli 1
                             -clamp 0
                             -out ${TEMP}/apTvRaOpticalCalibration_toa_PHR1A.tif?&box=334:2722:512:512 int16
                     VALID   --compare-image ${EPSILON_7}
                             ${BASELINE}/apTvRaOpticalCalibration_toa_PHR1A.tif
                             ${TEMP}/apTvRaOpticalCalibration_toa_PHR1A.tif )

OTB_TEST_APPLICATION(NAME apTvRaOpticalCalibration_toc_PHR1A
                     APP  OpticalCalibration
                     OPTIONS -in ${LARGEDATA}/PLEIADES/MAIDO_JP2_DIMAPv2_PRIMARY_MS_lossy_12bits/IMG_PHR1A_MS_002/IMG_PHR1A_MS_201206050630064_SEN_559102101-002_R1C1.JP2
                             -level toc
                             -milli 1
                             -clamp 0
                             -atmo.pressure 899
                             -out ${TEMP}/apTvRaOpticalCalibration_toc_PHR1A.tif?&box=334:2722:512:512 int16
                     VALID   --compare-image ${EPSILON_7}
                             ${BASELINE}/apTvRaOpticalCalibration_toc_PHR1A.tif
                             ${TEMP}/apTvRaOpticalCalibration_toc_PHR1A.tif )
ENDIF(OTB_DATA_USE_LARGEINPUT)
