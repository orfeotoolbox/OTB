#--- Pansharpening ---#

OTB_TEST_APPLICATION(NAME  apTvPansharpening_RCS
                     APP  Pansharpening
                     OPTIONS -inp ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
                             -inxs ${INPUTDATA}/QB_Toulouse_Ortho_XS.tif
                 	     -out ${TEMP}/apTvRcsPanSharpeningFusion.tif uint16
                 	     -method rcs
                     #VALID   --compare-image ${EPSILON_8}
                     #         ${BASELINE}/fuTvRcsPanSharpeningFusion.tif
                     #        ${TEMP}/apTvRcsPanSharpeningFusion.tif
                     )

OTB_TEST_APPLICATION(NAME  apTvPansharpening_LMVM
                     APP  Pansharpening
                     OPTIONS -inp ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
                             -inxs ${INPUTDATA}/QB_Toulouse_Ortho_XS.tif
                 	     -out ${TEMP}/apTvLmvmPanSharpeningFusion.tif uint16
                 	     -method lmvm
                             -method.lmvm.radiusx 5
                             -method.lmvm.radiusy 5
                     #VALID   --compare-image ${EPSILON_8}
                     #         ${BASELINE}/fuTvLmvmPanSharpeningFusion.tif
                     #	      ${TEMP}/apTvLmvmPanSharpeningFusion.tif
                     )
#TODO add validation and test relaterd to bayesian image fusion

