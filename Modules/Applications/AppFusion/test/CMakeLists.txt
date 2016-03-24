otb_module_test()
#----------- BundleToPerfectSensor TESTS ----------------
otb_test_application(NAME apTvPrBundleToPerfectSensor
                     APP BundleToPerfectSensor
                     OPTIONS -inp  ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
                             -inxs ${INPUTDATA}/QB_Toulouse_Ortho_XS_ROI_170x230.tif
                             -elev.dem ${INPUTDATA}/DEM/srtm_directory
                             -out ${TEMP}/apTvPrBundleToPerfectSensor.tif int16
                     VALID  --compare-image ${EPSILON_7}
                        ${BASELINE}/apTvPrBundleToPerfectSensor.tif
                        ${TEMP}/apTvPrBundleToPerfectSensor.tif)
                      
otb_test_application(NAME apTvPrBundleToPerfectSensor_phr
                     APP BundleToPerfectSensor
                     OPTIONS -inp  ${INPUTDATA}/phr_pan.tif
                             -inxs ${INPUTDATA}/phr_xs.tif
                             -out ${TEMP}/apTvPrBundleToPerfectSensorPHR.tif int16
                     VALID  --compare-image ${EPSILON_7}
                        ${BASELINE}/apTvPrBundleToPerfectSensorPHR.tif
                        ${TEMP}/apTvPrBundleToPerfectSensorPHR.tif)

#----------- Pansharpening TESTS ----------------
otb_test_application(NAME  apTvFuPansharpening_LMVM
                     APP  Pansharpening
                     OPTIONS -inp ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
                             -inxs ${INPUTDATA}/QB_Toulouse_Ortho_XS.tif
                       -out ${TEMP}/apTvFuPanSharpeningLmvm.tif double
                       -method lmvm
                             -method.lmvm.radiusx 5
                             -method.lmvm.radiusy 5
                     VALID   --compare-image ${EPSILON_6}
                             ${BASELINE}/fuTvLmvmPanSharpeningFusion.tif
                            ${TEMP}/apTvFuPanSharpeningLmvm.tif
                     )

otb_test_application(NAME  apTvFuPansharpening_Bayes
                     APP  Pansharpening
                     OPTIONS -inp ${INPUTDATA}/panchro.tif
                             -inxs ${INPUTDATA}/multiSpectInterp.tif
                       -out ${TEMP}/apTvFuPanSharpeningBayes.tif double
                       -method bayes
                     VALID   --compare-image ${EPSILON_6}
                             ${BASELINE}/apTvFuPanSharpeningBayes.tif
                            ${TEMP}/apTvFuPanSharpeningBayes.tif
                     )

otb_test_application(NAME  apTvFuPansharpening_RCS
                     APP  Pansharpening
                     OPTIONS -inp ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
                             -inxs ${INPUTDATA}/QB_Toulouse_Ortho_XS.tif
                       -out ${TEMP}/apTvFuPanSharpeningRcs.tif double
                       -method rcs
                     VALID   --compare-image ${EPSILON_6}
                             ${BASELINE}/fuTvRcsPanSharpeningFusion.tif
                            ${TEMP}/apTvFuPanSharpeningRcs.tif
                     )

