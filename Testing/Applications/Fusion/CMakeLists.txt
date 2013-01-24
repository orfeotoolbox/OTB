#--- Pansharpening ---#

OTB_TEST_APPLICATION(NAME  apTvFuPansharpening_RCS
                     APP  Pansharpening
                     OPTIONS -inp ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
                             -inxs ${INPUTDATA}/QB_Toulouse_Ortho_XS.tif
                       -out ${TEMP}/apTvFuPanSharpeningRcs.tif double
                       -method rcs
                     VALID   --compare-image ${EPSILON_6}
                             ${BASELINE}/fuTvRcsPanSharpeningFusion.tif
                            ${TEMP}/apTvFuPanSharpeningRcs.tif
                     )

OTB_TEST_APPLICATION(NAME  apTvFuPansharpening_LMVM
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

OTB_TEST_APPLICATION(NAME  apTvFuPansharpening_Bayes
                     APP  Pansharpening
                     OPTIONS -inp ${INPUTDATA}/panchro.tif
                             -inxs ${INPUTDATA}/multiSpectInterp.tif
                       -out ${TEMP}/apTvFuPanSharpeningBayes.tif double
                       -method bayes
                     VALID   --compare-image ${EPSILON_6}
                             ${BASELINE}/apTvFuPanSharpeningBayes.tif
                            ${TEMP}/apTvFuPanSharpeningBayes.tif
                     )
