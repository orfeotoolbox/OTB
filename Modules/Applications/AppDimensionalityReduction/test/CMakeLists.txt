otb_module_test()
#----------- DimensionalityReduction TESTS ----------------
otb_test_application(NAME   apTvFEDimensionalityReductionMAF
                     APP  DimensionalityReduction
                     OPTIONS -in ${INPUTDATA}/cupriteSubHsi.tif
                             -out ${TEMP}/apTvFEDimensionalityReductionMAF.tif
                             -method maf
                     VALID   --compare-image 0.025
                             ${BASELINE}/bfTvMaximumAutocorrelationFactorImageFilterOutput.tif
                             ${TEMP}/apTvFEDimensionalityReductionMAF.tif)

otb_test_application(NAME   apTvFEDimensionalityReductionPCA
                     APP  DimensionalityReduction
                     OPTIONS -in ${INPUTDATA}/cupriteSubHsi.tif
                             -out ${TEMP}/apTvChDimensionalityReductionPCA.tif
                             -method pca
                     VALID   --compare-image 0.025
                             ${BASELINE}/bfTvPCAImageFilter3.tif
                             ${TEMP}/apTvChDimensionalityReductionPCA.tif)

