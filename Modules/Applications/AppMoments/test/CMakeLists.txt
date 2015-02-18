otb_module_test()
#----------- LocalStatisticExtraction TESTS ----------------
otb_test_application(NAME  apTvFELocalStatisticExtraction
                     APP  LocalStatisticExtraction
                     OPTIONS -in ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
                             -channel 1
                             -radius 3
                             -out ${TEMP}/apTvFELocalStatisticExtraction.tif
                     VALID   --compare-image ${NOTOL}
                   			 ${BASELINE}/feTvRadiometricMomentsImageFilter.tif
                 		     ${TEMP}/apTvFELocalStatisticExtraction.tif)

