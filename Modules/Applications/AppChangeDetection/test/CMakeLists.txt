otb_module_test()
#----------- MultivariateAlterationDetector TESTS ----------------
otb_test_application(NAME   apTvChMultivariateAlterationDetector
                     APP  MultivariateAlterationDetector
                     OPTIONS -in1 ${INPUTDATA}/Spot5-Gloucester-before.tif
                             -in2 ${INPUTDATA}/Spot5-Gloucester-after.tif
                             -out ${TEMP}/apTvChMultivariateAlterationDetectorSameNbBands.tif
                     VALID   --compare-image 0.025
                             ${BASELINE}/cdTvMultivariateAlterationDetectorImageFilterOutputSameNbBands.tif
                  			 ${TEMP}/apTvChMultivariateAlterationDetectorSameNbBands.tif)

