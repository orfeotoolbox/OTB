
#--- MaximumAutocorrelationFactor ---#

OTB_TEST_APPLICATION(NAME   apTvChMaximumAutocorrelationFactor
                     APP  MaximumAutocorrelationFactor
                     OPTIONS -in ${INPUTDATA}/cupriteSubHsi.tif
                             -out ${TEMP}/apTvChMaximumAutocorrelationFactor.tif
                     VALID   --compare-image 0.025
                             ${BASELINE}/bfTvMaximumAutocorrelationFactorImageFilterOutput.tif
                             ${TEMP}/apTvChMaximumAutocorrelationFactor.tif)


OTB_TEST_APPLICATION(NAME   apTvChDimensionnalityReductionPCA
                     APP  DimensionnalityReduction
                     OPTIONS -in ${INPUTDATA}/cupriteSubHsi.tif
                             -out ${TEMP}/apTvChDimensionnalityReductionPCA.tif
                             -method pca
                     VALID   --compare-image 0.025
                             ${BASELINE}/bfTvPCAImageFilter3.tif
                             ${TEMP}/apTvChDimensionnalityReductionPCA.tif)
                             