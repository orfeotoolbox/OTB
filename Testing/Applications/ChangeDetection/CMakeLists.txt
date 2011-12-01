
#--- MultivariateAlterationDetector ---#

OTB_TEST_APPLICATION(NAME   apTvChMultivariateAlterationDetector
                     APP  MultivariateAlterationDetector
                     OPTIONS -in1 ${INPUTDATA}/Spot5-Gloucester-before.tif
                             -in2 ${INPUTDATA}/Spot5-Gloucester-after.tif
                             -out ${TEMP}/apTvChMultivariateAlterationDetectorSameNbBands.tif
                     VALID   --compare-image 0.025
                             ${BASELINE}/cdTvMultivariateAlterationDetectorImageFilterOutputSameNbBands.tif
                  			 ${TEMP}/apTvChMultivariateAlterationDetectorSameNbBands.tif)
