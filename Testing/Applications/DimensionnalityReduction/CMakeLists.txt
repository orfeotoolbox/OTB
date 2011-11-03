
#--- MaximumAutocorrelationFactor ---#

add_test(NAME apTvChMaximumAutocorrelationFactor
         COMMAND otbTestDriver
               --compare-image 0.025
                   ${BASELINE}/bfTvMaximumAutocorrelationFactorImageFilterOutput.tif
                   ${TEMP}/apTvChMaximumAutocorrelationFactor.tif
                 Execute $<TARGET_FILE:otbApplicationLauncherCommandLine>
                 MaximumAutocorrelationFactor
                 $<TARGET_FILE_DIR:otbapp_MaximumAutocorrelationFactor>
                 --in ${INPUTDATA}/cupriteSubHsi.tif
                 --out ${TEMP}/apTvChMaximumAutocorrelationFactor.tif )

