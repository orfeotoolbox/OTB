OTB_TEST_APPLICATION(NAME apTvRaOpticalCalibration_UnknownSensor
                     APP  OpticalCalibration
                     OPTIONS 
           -in ${EXAMPLEDATA}/Romania_Extract.tif
           -out ${TEMP}/apTvRaOpticalCalibration_Spot4_UnknownSensor_test.tif
           -level toc
           -acqui.gainbias ${INPUTDATA}/apTvRaOpticalCalibrationUnknownSensorGainsBiases2.txt
           -acqui.day 4
           -acqui.month 12
           -acqui.sun.elev 62.7
           -acqui.sun.azim 152.7
           -acqui.view.elev 87.5
           -acqui.view.azim -77.0
           -acqui.solarilluminations ${INPUTDATA}/apTvRaOpticalCalibrationUnknownSensorSolarIllumations2.txt
           -atmo.rsr ${INPUTDATA}/apTvRaOpticalCalibrationUnknownSensorRSR.txt
           -atmo.pressure 1013.0
           -atmo.wa 2.48134
           -atmo.oz 0.34400
           -atmo.aerosol continental
           -atmo.opt 0.199854
           -atmo.radius 2
           -atmo.pixsize 0.02
           -milli false
           -clamp false
                     VALID   --compare-image ${EPSILON_6}
                             ${OTB_DATA_ROOT}/Baseline/Examples/Radiometry/Example_RomaniaAtmosphericCorrectionSequencement.tif
                             ${TEMP}/apTvRaOpticalCalibration_Spot4_UnknownSensor_test.tif )

OTB_TEST_APPLICATION(NAME apTvRaOpticalCalibration_Reverse_UnknownSensor
                     APP  OpticalCalibration
                     OPTIONS 
			     -in ${BASELINE}/raTvLuminanceToReflectanceImageFilterAutoQuickbirdXS.tif
			     -out ${TEMP}/apTvRaOpticalCalibration_Rev_QB-XS_UnknownSensor_test.tif
			     -level toatoim 
			     -acqui.gainbias ${INPUTDATA}/apTvRaOpticalCalibrationUnknownSensorGainsBiases.txt
			     -acqui.day 1
			     -acqui.month 4
			     -acqui.sun.elev 48.6
			     -acqui.solarilluminations ${INPUTDATA}/apTvRaOpticalCalibrationUnknownSensorSolarIllumations.txt
		       -milli false
			     -clamp false
                     VALID   --compare-image ${EPSILON_6}
                             ${INPUTDATA}/QB_MUL_ROI_1000_100.tif
                             ${TEMP}/apTvRaOpticalCalibration_Rev_QB-XS_UnknownSensor_test.tif )


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


# FIXME toc reflectance computation on a Pleaides MS data over the Maido in La Reunion
# Optical calibration leads to negative values. It may be due to slope effects which are not
# take into consideration in the process (only altitude can be modified with the atmospheric pressure parameter)
# TODO Reactivate these tests when we'll add slope effects correction


# if(OTB_DATA_USE_LARGEINPUT)
# OTB_TEST_APPLICATION(NAME apTvRaOpticalCalibration_toa_PHR1A
#                      APP  OpticalCalibration
#                      OPTIONS -in ${LARGEDATA}/PLEIADES/MAIDO_JP2_DIMAPv2_PRIMARY_MS_lossy_12bits/IMG_PHR1A_MS_002/IMG_PHR1A_MS_201206050630064_SEN_559102101-002_R1C1.JP2
#                              -level toa
#                              -milli 1
#                              -clamp 0
#                              -out ${TEMP}/apTvRaOpticalCalibration_toa_PHR1A.tif?&box=334:2722:512:512 int16
#                      VALID   --compare-image ${EPSILON_7}
#                              ${BASELINE}/apTvRaOpticalCalibration_toa_PHR1A.tif
#                              ${TEMP}/apTvRaOpticalCalibration_toa_PHR1A.tif )

# #for TOC computation, for now we use for baseline the output of the toc computation + 200 (milli reflectance) to avoid negative values.
# #FIXME update with a proper baseline after investigatio
# OTB_TEST_APPLICATION(NAME apTvRaOpticalCalibration_toc_PHR1A
#                      APP  OpticalCalibration
#                      OPTIONS -in ${LARGEDATA}/PLEIADES/MAIDO_JP2_DIMAPv2_PRIMARY_MS_lossy_12bits/IMG_PHR1A_MS_002/IMG_PHR1A_MS_201206050630064_SEN_559102101-002_R1C1.JP2
#                              -level toc
#                              -milli 1
#                              -clamp 0
#                              # change the atmospheric pressure as scene is acquired over a region at 1000 meters of altitude
#                              -atmo.pressure 899
#                              -out ${TEMP}/apTvRaOpticalCalibration_toc_PHR1A.tif?&box=334:2722:512:512 int16
#                      VALID   --compare-image ${EPSILON_7}
#                              ${BASELINE}/apTvRaOpticalCalibration_toc_PHR1A.tif
#                              ${TEMP}/apTvRaOpticalCalibration_toc_PHR1A.tif )
# endif()
