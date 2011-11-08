
#--- MaximumAutocorrelationFactor ---#

OTB_TEST_APPLICATION(NAME   apTvChMaximumAutocorrelationFactor
                     APP  MaximumAutocorrelationFactor
                     OPTIONS --in ${INPUTDATA}/cupriteSubHsi.tif
                             --out ${TEMP}/apTvChMaximumAutocorrelationFactor.tif
                     VALID   --compare-image 0.025
                             ${BASELINE}/bfTvMaximumAutocorrelationFactorImageFilterOutput.tif
                             ${TEMP}/apTvChMaximumAutocorrelationFactor.tif)


