
#--- MultivariateAlterationDetector ---#

add_test(NAME apTvChMultivariateAlterationDetector
         COMMAND otbTestDriver
               --compare-image 0.025
                   ${BASELINE}/cdTvMultivariateAlterationDetectorImageFilterOutputSameNbBands.tif
                   ${TEMP}/apTvChMultivariateAlterationDetectorSameNbBands.tif
                 Execute $<TARGET_FILE:otbApplicationLauncherCommandLine>
                 MultivariateAlterationDetector
                 $<TARGET_FILE_DIR:otbapp_MultivariateAlterationDetector>
                 --in1 ${INPUTDATA}/Spot5-Gloucester-before.tif
                 --in2 ${INPUTDATA}/Spot5-Gloucester-after.tif
                 --out ${TEMP}/apTvChMultivariateAlterationDetectorSameNbBands.tif )


